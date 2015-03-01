// file      : odb/relational/common.txx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

namespace relational
{
  //
  // member_base_impl
  //

  template <typename T>
  void member_base_impl<T>::
  traverse (semantics::data_member& m)
  {
    if (transient (m))
      return;

    string var;

    if (!var_override_.empty ())
      var = var_override_;
    else
    {
      string const& name (m.name ());
      var = name + (name[name.size () - 1] == '_' ? "" : "_");
    }

    bool cq (type_override_ != 0 ? false : const_type (m.type ()));
    semantics::type& t (type_override_ != 0 ? *type_override_ : utype (m));

    semantics::type* cont;
    if (semantics::class_* c = object_pointer (t))
    {
      semantics::type& t (utype (*id_member (*c)));
      semantics::class_* comp (composite_wrapper (t));

      member_info mi (m,
                      (comp != 0 ? *comp : t),
                      (comp != 0 && wrapper (t) ? &t : 0),
                      cq,
                      var,
                      fq_type_override_); // Pointer type.

      mi.ptr = c;

      if (comp == 0)
        mi.st = &member_sql_type (m);

      if (pre (mi))
      {
        traverse_object_pointer (mi);
        post (mi);
      }
    }
    else if (semantics::class_* c = composite_wrapper (t))
    {
      // If t is a wrapper, pass the wrapped type. Also pass the
      // original, wrapper type.
      //
      member_info mi (m,
                      *c,
                      (wrapper (t) ? &t : 0),
                      cq,
                      var,
                      fq_type_override_);
      if (pre (mi))
      {
        traverse_composite (mi);
        post (mi);
      }
    }
    // This cannot be a container if we have a type override.
    //
    else if (type_override_ == 0 && (cont = context::container (m)))
    {
      // The same unwrapping logic as for composite values.
      //
      member_info mi (m,
                      *cont,
                      (wrapper (t) ? &t : 0),
                      cq,
                      var,
                      fq_type_override_);
      if (pre (mi))
      {
        traverse_container (mi);
        post (mi);
      }
    }
    else
    {
      member_info mi (m, t, 0, cq, var, fq_type_override_);
      mi.st = &member_sql_type (m);

      if (pre (mi))
      {
        traverse_simple (mi);
        post (mi);
      }
    }
  }

  template <typename T>
  void member_base_impl<T>::
  traverse_object_pointer (member_info& mi)
  {
    if (composite (mi.t)) // Already unwrapped.
      traverse_composite (mi);
    else
      traverse_simple (mi);
  }
}
