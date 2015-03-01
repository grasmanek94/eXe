// file      : odb/relational/sqlite/source.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/relational/source.hxx>

#include <odb/relational/sqlite/common.hxx>
#include <odb/relational/sqlite/context.hxx>

using namespace std;

namespace relational
{
  namespace sqlite
  {
    namespace source
    {
      namespace relational = relational::source;

      //
      // bind
      //

      struct bind_member: relational::bind_member_impl<sql_type>,
                          member_base
      {
        bind_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << b << ".type = sqlite::bind::integer;"
             << b << ".buffer = &" << arg << "." << mi.var << "value;"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << b << ".type = sqlite::bind::real;"
             << b << ".buffer = &" << arg << "." << mi.var << "value;"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_text (member_info& mi)
        {
          os << b << ".type = sqlite::image_traits<" << endl
             << "  " << mi.fq_type () << "," << endl
             << "  sqlite::id_text>::bind_value;"
             << b << ".buffer = " << arg << "." << mi.var << "value.data ();"
             << b << ".size = &" << arg << "." << mi.var << "size;"
             << b << ".capacity = " << arg << "." << mi.var <<
            "value.capacity ();"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_blob (member_info& mi)
        {
          os << b << ".type = sqlite::bind::blob;"
             << b << ".buffer = " << arg << "." << mi.var << "value.data ();"
             << b << ".size = &" << arg << "." << mi.var << "size;"
             << b << ".capacity = " << arg << "." << mi.var <<
            "value.capacity ();"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }
      };
      entry<bind_member> bind_member_;

      //
      // grow
      //

      struct grow_member: relational::grow_member, member_base
      {
        grow_member (base const& x)
            : member_base::base (x), // virtual base
              base (x),
              member_base (x)
        {
        }

        virtual bool
        pre (member_info& mi)
        {
          if (container (mi))
            return false;

          // Ignore polymorphic id references; they are not returned by
          // the select statement.
          //
          if (mi.ptr != 0 && mi.m.count ("polymorphic-ref"))
            return false;

          ostringstream ostr;
          ostr << "t[" << index_ << "UL]";
          e = ostr.str ();

          if (var_override_.empty ())
          {
            os << "// " << mi.m.name () << endl
               << "//" << endl;

            semantics::class_* comp (composite (mi.t));

            // If the member is soft- added or deleted, check the version.
            //
            unsigned long long av (added (mi.m));
            unsigned long long dv (deleted (mi.m));

            // If this is a composite member, see if it is summarily
            // added/deleted.
            //
            if (comp != 0)
            {
              unsigned long long cav (added (*comp));
              unsigned long long cdv (deleted (*comp));

              if (cav != 0 && (av == 0 || av < cav))
                av = cav;

              if (cdv != 0 && (dv == 0 || dv > cdv))
                dv = cdv;
            }

            // If the addition/deletion version is the same as the section's,
            // then we don't need the test.
            //
            if (user_section* s = dynamic_cast<user_section*> (section_))
            {
              if (av == added (*s->member))
                av = 0;

              if (dv == deleted (*s->member))
                dv = 0;
            }

            if (av != 0 || dv != 0)
            {
              os << "if (";

              if (av != 0)
                os << "svm >= schema_version_migration (" << av << "ULL, true)";

              if (av != 0 && dv != 0)
                os << " &&" << endl;

              if (dv != 0)
                os << "svm <= schema_version_migration (" << dv << "ULL, true)";

              os << ")"
                 << "{";
            }
          }

          return true;
        }

        virtual void
        post (member_info& mi)
        {
          semantics::class_* comp (composite (mi.t));

          if (var_override_.empty ())
          {
            unsigned long long av (added (mi.m));
            unsigned long long dv (deleted (mi.m));

            if (comp != 0)
            {
              unsigned long long cav (added (*comp));
              unsigned long long cdv (deleted (*comp));

              if (cav != 0 && (av == 0 || av < cav))
                av = cav;

              if (cdv != 0 && (dv == 0 || dv > cdv))
                dv = cdv;
            }

            if (user_section* s = dynamic_cast<user_section*> (section_))
            {
              if (av == added (*s->member))
                av = 0;

              if (dv == deleted (*s->member))
                dv = 0;
            }

            if (av != 0 || dv != 0)
              os << "}";
          }

          if (comp != 0)
            index_ += column_count (*comp).total;
          else
            index_++;
        }

        virtual void
        traverse_composite (member_info& mi)
        {
          os << "if (composite_value_traits< " << mi.fq_type () <<
            ", id_sqlite >::grow (" << endl
             << "i." << mi.var << "value, t + " << index_ << "UL" <<
            (versioned (*composite (mi.t)) ? ", svm" : "") << "))" << endl
             << "grew = true;"
             << endl;
        }

        virtual void
        traverse_integer (member_info&)
        {
          os << e << " = false;"
             << endl;
        }

        virtual void
        traverse_real (member_info&)
        {
          os << e << " = false;"
             << endl;
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << "if (" << e << ")" << endl
             << "{"
             << "i." << mi.var << "value.capacity (i." << mi.var << "size);"
             << "grew = true;"
             << "}";
        }

      private:
        string e;
      };
      entry<grow_member> grow_member_;

      //
      // init image
      //

      struct init_image_member: relational::init_image_member_impl<sql_type>,
                                member_base
      {
        init_image_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        set_null (member_info& mi)
        {
          os << "i." << mi.var << "null = true;";
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;";
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;";
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << "std::size_t cap (i." << mi.var << "value.capacity ());"
             << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "size," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;"
             << "grew = grew || (cap != i." << mi.var << "value.capacity ());";
        }
      };
      entry<init_image_member> init_image_member_;

      //
      // init value
      //

      struct init_value_member: relational::init_value_member_impl<sql_type>,
                                member_base
      {
        init_value_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        get_null (member_info& mi)
        {
          os << "i." << mi.var << "null";
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "null);"
             << endl;
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "null);"
             << endl;
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "size," << endl
             << "i." << mi.var << "null);"
             << endl;
        }
      };
      entry<init_value_member> init_value_member_;

      struct container_traits: relational::container_traits, context
      {
        container_traits (base const& x): base (x) {}

        virtual void
        cache_result (string const&)
        {
          // Caching is not necessary since SQLite can execute several
          // interleaving statements.
          //
        }
      };
      entry<container_traits> container_traits_;

      struct class_: relational::class_, context
      {
        class_ (base const& x): base (x) {}

        virtual void
        init_auto_id (semantics::data_member&, string const& im)
        {
          os << im << "null = true;";
        }
      };
      entry<class_> class_entry_;
    }
  }
}
