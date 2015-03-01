// file      : odb/traversal/relational/key.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/traversal/relational/key.hxx>
#include <odb/traversal/relational/column.hxx>

namespace traversal
{
  namespace relational
  {
    void contains::
    traverse (type& c)
    {
      dispatch (c.column ());
    }
  }
}
