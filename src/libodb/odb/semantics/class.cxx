// file      : odb/semantics/class.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/gcc.hxx> // TYPE_HAS_DEFAULT_CONSTRUCTOR

#include <cutl/compiler/type-info.hxx>
#include <odb/semantics/class.hxx>

namespace semantics
{
  inherits::
  inherits (access_type access, bool virt)
      : virt_ (virt), access_ (access)
  {
  }

  class_::
  class_ (path const& file, size_t line, size_t column, tree tn)
      : node (file, line, column, tn)
  {
  }

  bool class_::
  default_ctor () const
  {
    return TYPE_HAS_DEFAULT_CONSTRUCTOR (tree_node ());
  }

  bool class_::
  complete () const
  {
    return COMPLETE_TYPE_P (tree_node ());
  }

  bool class_::
  abstract () const
  {
    return CLASSTYPE_PURE_VIRTUALS (tree_node ());
  }

  names* class_::
  lookup (string const& name,
          type_id const& ti,
          unsigned int flags,
          bool* ph) const
  {
    bool h (false);
    bool& rh (ph != 0 ? *ph : h);

    names* r (scope::lookup (name, ti, flags | exclude_outer, &rh));

    if (r != 0)
      return r;

    // If we found a name but the types didn't match, then bail out
    // unless we want hidden names.
    //
    if (rh && (flags & include_hidden) == 0)
      return 0;

    // Look in the base classes unless requested not to. For the name
    // lookup purposes, bases can be viewed as a parallel set of outer
    // scopes that are searched after the class scope and before any
    // real outer scope. Interestingly, outer scopes of bases are not
    // considered during this lookup, only their bases.
    //
    if ((flags & exclude_base) == 0)
    {
      // Being hidden in one base doesn't mean it is also hidden in the
      // other. Normally that would be an ambiguous lookup, but we use
      // relaxed rules.
      //
      bool any_h (false); // Indicates whether any base hides the name.

      for (inherits_iterator i (inherits_begin ()); i != inherits_end (); ++i)
      {
        bool h (false); // Indicates whether this base hides the name.
        names* br (i->base ().lookup (name, ti, flags | exclude_outer, &h));
        any_h = any_h || h;

        if (br != 0)
        {
          if (r != 0)
            throw ambiguous (*r, *br);

          r = br;

          if (h)
            rh = true;
        }
      }

      if (r != 0)
        return r;

      if (any_h)
      {
        rh = true;
        if ((flags & include_hidden) == 0)
          return 0;
      }
    }

    // Look in the outer scope unless requested not to.
    //
    if ((flags & exclude_outer) == 0)
      return scope ().lookup (name, ti, flags, &rh);

    return 0;
  }

  // type info
  //
  namespace
  {
    struct init
    {
      init ()
      {
        using compiler::type_info;

        // inherits
        //
        {
          type_info ti (typeid (inherits));
          ti.add_base (typeid (edge));
          insert (ti);
        }

        // class_
        //
        {
          type_info ti (typeid (class_));
          ti.add_base (typeid (type));
          ti.add_base (typeid (scope));
          insert (ti);
        }
      }
    } init_;
  }
}
