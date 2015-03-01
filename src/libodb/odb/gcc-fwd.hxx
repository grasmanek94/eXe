// file      : odb/gcc-fwd.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_GCC_FWD_HXX
#define ODB_GCC_FWD_HXX

extern "C"
{
#include <coretypes.h>

typedef unsigned int source_location; // <line-map.h>
typedef source_location location_t;   // <input.h>
}

#endif // ODB_GCC_FWD_HXX
