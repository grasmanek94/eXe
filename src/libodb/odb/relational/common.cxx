// file      : odb/relational/common.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/relational/common.hxx>

using namespace std;

namespace relational
{
  // member_database_type_id
  //
  string member_database_type_id::
  database_type_id (semantics::data_member&)
  {
    assert (false);
    return string ();
  }
}
