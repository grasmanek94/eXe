// file      : odb/gcc.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_GCC_HXX
#define ODB_GCC_HXX

#include <odb/gcc-fwd.hxx>

// GCC header includes to get the plugin and parse tree declarations.
// The order is important and doesn't follow any kind of logic.
//

#include <stdlib.h>
#include <gmp.h>

#include <cstdlib> // Include before GCC poisons some declarations.

#include <bversion.h>

// GCC 4.7 can be built using either C or C++ compiler. From 4.8 it
// is always built as C++.
//
#if BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR <= 6
#  define ODB_GCC_PLUGIN_C
#elif BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR <= 7
#  include <config.h>
#  ifndef ENABLE_BUILD_WITH_CXX
#    define ODB_GCC_PLUGIN_C
#  endif
#endif

#ifdef ODB_GCC_PLUGIN_C
extern "C"
{
#endif

#include <gcc-plugin.h>

#include <config.h>
#include <system.h>
#include <coretypes.h>
#include <tree.h>
#include <real.h>

#include <tm.h>

#include <cpplib.h>
#include <cp/cp-tree.h>

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 5
#  include <c-family/c-common.h>
#  include <c-family/c-pragma.h>
#else
#  include <c-common.h>
#  include <c-pragma.h>
#endif

#include <diagnostic.h>
#include <output.h>

#ifdef ODB_GCC_PLUGIN_C
} // extern "C"
#endif

// Only since GCC 4.7.0.
//
#ifndef LOCATION_COLUMN
#define LOCATION_COLUMN(LOC) (expand_location (LOC).column)
#endif

#ifndef DECL_SOURCE_COLUMN
#define DECL_SOURCE_COLUMN(NODE) LOCATION_COLUMN (DECL_SOURCE_LOCATION (NODE))
#endif

// Only since GCC 4.6.0.
//
#ifndef DECL_CHAIN
#define DECL_CHAIN(x) TREE_CHAIN(x)
#endif

#endif // ODB_GCC_HXX
