// file      : odb/location.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/location.hxx>
#include <odb/diagnostics.hxx>

location::
location (location_t l)
    : file (location_file (l)),
      line (location_line (l)),
      column (location_column (l))
{
}
