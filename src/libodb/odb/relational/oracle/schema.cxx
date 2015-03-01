// file      : odb/relational/oracle/schema.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/relational/schema.hxx>

#include <odb/relational/oracle/common.hxx>
#include <odb/relational/oracle/context.hxx>

using namespace std;

namespace relational
{
  namespace oracle
  {
    namespace schema
    {
      namespace relational = relational::schema;
      using relational::table_set;

      struct sql_emitter: relational::sql_emitter
      {
        sql_emitter (const base& x): base (x) {}

        virtual void
        line (const std::string& l)
        {
          // SQLPlus doesn't like empty line in the middle of a statement.
          //
          if (!l.empty ())
          {
            base::line (l);
            last_ = l;
          }
        }

        virtual void
        post ()
        {
          if (!first_) // Ignore empty statements.
          {
            if (last_ == "END;")
              os << endl
                 << '/' << endl
                 << endl;

            else
              os << ';' << endl
                 << endl;
          }
        }

      private:
        string last_;
      };
      entry<sql_emitter> sql_emitter_;

      //
      // File.
      //

      struct sql_file: relational::sql_file, context
      {
        sql_file (const base& x): base (x) {}

        virtual void
        prologue ()
        {
          // Quiet down SQLPlus and make sure it exits with an error
          // code if there is an error.
          //
          os << "SET FEEDBACK OFF;" << endl
             << "WHENEVER SQLERROR EXIT FAILURE;" << endl
             << "WHENEVER OSERROR EXIT FAILURE;" << endl
             << endl;
        }

        virtual void
        epilogue ()
        {
          os << "EXIT;" << endl;
        }
      };
      entry<sql_file> sql_file_;

      //
      // Drop.
      //

      struct drop_column: relational::drop_column, context
      {
        drop_column (base const& x): base (x) {}

        virtual void
        traverse (sema_rel::drop_column& dc)
        {
          if (first_)
            first_ = false;
          else
            os << "," << endl
               << "        ";

          os << quote_id (dc.name ());
        }
      };
      entry<drop_column> drop_column_;

      struct drop_foreign_key: relational::drop_foreign_key, context
      {
        drop_foreign_key (base const& x): base (x) {}

        virtual void
        traverse (sema_rel::drop_foreign_key& dfk)
        {
          os << endl;
          drop (dfk);
        }
      };
      entry<drop_foreign_key> drop_foreign_key_;

      struct drop_index: relational::drop_index, context
      {
        drop_index (base const& x): base (x) {}

        virtual string
        name (sema_rel::index& in)
        {
          // In Oracle, index names can be qualified with the schema.
          //
          sema_rel::table& t (static_cast<sema_rel::table&> (in.scope ()));
          sema_rel::qname n (t.name ().qualifier ());
          n.append (in.name ());
          return quote_id (n);
        }
      };
      entry<drop_index> drop_index_;

      struct drop_table: relational::drop_table, context
      {
        drop_table (base const& x): base (x) {}

        virtual void
        drop (sema_rel::table& t, bool migration)
        {
          using sema_rel::primary_key;

          sema_rel::table::names_iterator i (t.find ("")); // Special name.
          primary_key* pk (i != t.names_end ()
                           ? &dynamic_cast<primary_key&> (i->nameable ())
                           : 0);

          string qt (quote_id (t.name ()));
          string qs (pk != 0 && pk->auto_ ()
                     ? quote_id (qname::from_string (pk->extra ()["sequence"]))
                     : "");

          if (migration)
          {
            pre_statement ();
            os << "DROP TABLE " << qt << endl;
            post_statement ();

            // Drop the sequence if we have auto primary key.
            //
            if (!qs.empty ())
            {
              pre_statement ();
              os << "DROP SEQUENCE " << qs << endl;
              post_statement ();
            }
          }
          else
          {
            // Oracle has no IF EXISTS conditional for dropping objects. The
            // PL/SQL approach below seems to be the least error-prone and the
            // most widely used of the alternatives.
            //
            pre_statement ();
            os << "BEGIN" << endl
               << "  BEGIN" << endl
               << "    EXECUTE IMMEDIATE 'DROP TABLE " << qt << " CASCADE " <<
              "CONSTRAINTS';" << endl
               << "  EXCEPTION" << endl
               << "    WHEN OTHERS THEN" << endl
               << "      IF SQLCODE != -942 THEN RAISE; END IF;" << endl
               << "  END;" << endl;

            // Drop the sequence if we have auto primary key.
            //
            if (!qs.empty ())
            {
              os << "  BEGIN" << endl
                 << "    EXECUTE IMMEDIATE 'DROP SEQUENCE " << qs <<
                "';" << endl
                 << "  EXCEPTION" << endl
                 << "    WHEN OTHERS THEN" << endl
                 << "      IF SQLCODE != -2289 THEN RAISE; END IF;" << endl
                 << "  END;" << endl;
            }

            os << "END;" << endl;
            post_statement ();
          }
        }

        virtual void
        traverse (sema_rel::table& t, bool migration)
        {
          // For migration drop foreign keys explicitly in pre-migration.
          //
          if (migration)
          {
            base::traverse (t, migration);
            return;
          }

          // For schema creation we use the CASCADE clause to drop foreign
          // keys.
          //
          if (pass_ != 2)
            return;

          drop (t, migration);
        }
      };
      entry<drop_table> drop_table_;

      //
      // Create.
      //

      struct create_column: relational::create_column, context
      {
        create_column (base const& x): base (x) {}

        virtual void
        traverse (sema_rel::add_column& ac)
        {
          if (first_)
            first_ = false;
          else
            os << "," << endl
               << "       ";

          create (ac);
        }

        virtual void
        constraints (sema_rel::column& c, sema_rel::primary_key* pk)
        {
          // Oracle wants DEFAULT before NULL even though we can end
          // up with mouthfulls like DEFAULT NULL NULL.
          //
          if (!c.default_ ().empty ())
            os << " DEFAULT " << c.default_ ();

          null (c);

          // If this is a single-column primary key, generate it inline.
          //
          if (pk != 0 && pk->contains_size () == 1)
            primary_key ();

          if (pk != 0 && pk->auto_ ())
            auto_ (*pk);
        }
      };
      entry<create_column> create_column_;

      struct create_foreign_key: relational::create_foreign_key, context
      {
        create_foreign_key (base const& x): base (x) {}

        virtual void
        traverse_add (sema_rel::foreign_key& fk)
        {
          os << endl
             << "  ADD CONSTRAINT ";
          create (fk);
        }
      };
      entry<create_foreign_key> create_foreign_key_;

      struct create_index: relational::create_index, context
      {
        create_index (base const& x): base (x) {}

        virtual string
        name (sema_rel::index& in)
        {
          // In Oracle, index names can be qualified with the schema.
          //
          sema_rel::table& t (static_cast<sema_rel::table&> (in.scope ()));
          sema_rel::qname n (t.name ().qualifier ());
          n.append (in.name ());
          return quote_id (n);
        }
      };
      entry<create_index> create_index_;

      struct create_table: relational::create_table, context
      {
        create_table (base const& x): base (x) {}

        void
        traverse (sema_rel::table& t)
        {
          base::traverse (t);

          if (pass_ == 1)
          {
            // Create the sequence if we have auto primary key.
            //
            using sema_rel::primary_key;

            sema_rel::table::names_iterator i (t.find ("")); // Special name.
            primary_key* pk (i != t.names_end ()
                             ? &dynamic_cast<primary_key&> (i->nameable ())
                             : 0);

            if (pk != 0 && pk->auto_ ())
            {
              string qs (
                quote_id (qname::from_string (pk->extra ()["sequence"])));

              pre_statement ();
              os_ << "CREATE SEQUENCE " << qs << endl
                  << "  START WITH 1 INCREMENT BY 1" << endl;
              post_statement ();
            }
          }
        }
      };
      entry<create_table> create_table_;

      //
      // Alter.
      //

      struct alter_column: relational::alter_column, context
      {
        alter_column (base const& x): base (x) {}

        virtual void
        traverse (sema_rel::column& c)
        {
          // Relax (NULL) in pre and tighten (NOT NULL) in post.
          //
          if (pre_ != c.null ())
            return;

          if (first_)
            first_ = false;
          else
            os << "," << endl
               << "          ";

          os << quote_id (c.name ()) << (c.null () ? " NULL" : " NOT NULL");
        }
      };
      entry<alter_column> alter_column_;

      struct alter_table_pre: relational::alter_table_pre, context
      {
        alter_table_pre (base const& x): base (x) {}

        virtual void
        alter (sema_rel::alter_table& at)
        {
          // Oracle can only alter certain kinds of things together but
          // grouped one at a time.
          //
          if (check<sema_rel::drop_foreign_key> (at))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ());

            instance<drop_foreign_key> dfc (*this);
            trav_rel::unames n (*dfc);
            names (at, n);
            os << endl;

            post_statement ();
          }

          if (check<sema_rel::add_column> (at))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ()) << endl
               << "  ADD (";

            instance<create_column> cc (*this);
            trav_rel::unames n (*cc);
            names (at, n);
            os << ")" << endl;

            post_statement ();
          }

          if (check_alter_column_null (at, true))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ()) << endl
               << "  MODIFY (";

            bool tl (true); // (Im)perfect forwarding.
            instance<alter_column> ac (*this, tl);
            trav_rel::unames n (*ac);
            names (at, n);
            os << ")" << endl;

            post_statement ();
          }
        }
      };
      entry<alter_table_pre> alter_table_pre_;

      struct alter_table_post: relational::alter_table_post, context
      {
        alter_table_post (base const& x): base (x) {}

        virtual void
        alter (sema_rel::alter_table& at)
        {
          // Oracle can only alter certain kinds of things together but
          // grouped one at a time.
          //
          if (check<sema_rel::drop_column> (at))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ()) << endl
               << "  DROP (";

            instance<drop_column> dc (*this);
            trav_rel::unames n (*dc);
            names (at, n);
            os << ")" << endl;

            post_statement ();
          }

          if (check_alter_column_null (at, false))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ()) << endl
               << "  MODIFY (";

            bool fl (false); // (Im)perfect forwarding.
            instance<alter_column> ac (*this, fl);
            trav_rel::unames n (*ac);
            names (at, n);
            os << ")" << endl;

            post_statement ();
          }

          if (check<sema_rel::add_foreign_key> (at))
          {
            pre_statement ();

            os << "ALTER TABLE " << quote_id (at.name ());

            instance<create_foreign_key> cfc (*this);
            trav_rel::unames n (*cfc);
            names (at, n);
            os << endl;

            post_statement ();
          }
        }
      };
      entry<alter_table_post> alter_table_post_;

      //
      // Schema version table.
      //

      struct version_table: relational::version_table, context
      {
        version_table (base const& x)
            : base (x)
        {
          // If the schema name is empty, replace it with a single space
          // to workaround the VARCHAR2 empty/NULL issue.
          //
          if (qs_ == "''")
            qs_ = "' '";
        }

        virtual void
        create_table ()
        {
          pre_statement ();

          os << "BEGIN" << endl
             << "  EXECUTE IMMEDIATE 'CREATE TABLE " << qt_ << " (" << endl
             << "    " << qn_ << " VARCHAR2(512) NOT NULL PRIMARY KEY," << endl
             << "    " << qv_ << " NUMBER(20) NOT NULL," << endl
             << "    " << qm_ << " NUMBER(1) NOT NULL)';" << endl
             << "EXCEPTION" << endl
             << "  WHEN OTHERS THEN" << endl
             << "    IF SQLCODE != -955 THEN RAISE; END IF;" << endl
             << "END;" << endl;

          post_statement ();
        }

        virtual void
        create (sema_rel::version v)
        {
          pre_statement ();

          os << "MERGE INTO " << qt_ << " USING DUAL ON (" << qn_ << " = " <<
            qs_ << ")" << endl
             << "  WHEN NOT MATCHED THEN INSERT (" << endl
             << "    " << qn_ << ", " << qv_ << ", " << qm_ << ")" << endl
             << "    VALUES (" << qs_ << ", " << v << ", 0)" << endl;

          post_statement ();
        }
      };
      entry<version_table> version_table_;
    }
  }
}
