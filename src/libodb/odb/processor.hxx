// file      : odb/processor.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_PROCESSOR_HXX
#define ODB_PROCESSOR_HXX

#include <odb/options.hxx>
#include <odb/features.hxx>
#include <odb/semantics/unit.hxx>

class processor
{
public:
  class failed {};

  void
  process (options const&,
           features&,
           semantics::unit&,
           semantics::path const&);

  processor () {}

private:
  processor (processor const&);
  processor& operator= (processor const&);
};

#endif // ODB_PROCESSOR_HXX
