// file      : odb/generator.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_GENERATOR_HXX
#define ODB_GENERATOR_HXX

#include <vector>

#include <odb/options.hxx>
#include <odb/features.hxx>
#include <odb/semantics/unit.hxx>

class generator
{
public:
  class failed {};

  void
  generate (options const&,
            features&,
            semantics::unit&,
            semantics::path const& file,
            std::vector<semantics::path> const& inputs);

  generator () {}

private:
  generator (generator const&);
  generator& operator= (generator const&);
};

#endif // ODB_GENERATOR_HXX
