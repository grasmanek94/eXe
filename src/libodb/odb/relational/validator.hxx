// file      : odb/relational/validator.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#ifndef ODB_RELATIONAL_VALIDATOR_HXX
#define ODB_RELATIONAL_VALIDATOR_HXX

#include <odb/options.hxx>
#include <odb/features.hxx>
#include <odb/semantics/unit.hxx>

namespace relational
{
  class validator
  {
  public:
    struct failed {};

    // The first pass is performed before processing. The second -- after.
    //
    void
    validate (options const&,
              features&,
              semantics::unit&,
              semantics::path const&,
              unsigned short pass);

    validator () {}

  private:
    validator (validator const&);
    validator& operator= (validator const&);
  };
}

#endif // ODB_RELATIONAL_VALIDATOR_HXX
