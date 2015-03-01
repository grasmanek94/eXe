// file      : odb/schema-catalog.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SCHEMA_CATALOG_HXX
#define ODB_SCHEMA_CATALOG_HXX

#include <odb/pre.hxx>

#include <odb/details/config.hxx> // ODB_CXX11

#include <string>
#include <cstddef> // std::size_t

#ifdef ODB_CXX11
#  include <functional> // std::function
#endif

#include <odb/forward.hxx> // schema_version, odb::core

#include <odb/details/export.hxx>
#include <odb/details/unused.hxx>
#include <odb/details/meta/static-assert.hxx>

namespace odb
{
  class LIBODB_EXPORT schema_catalog
  {
  public:
    // Schema creation.
    //
    static void
    create_schema (database&, const std::string& name = "", bool drop = true);

    static void
    drop_schema (database&, const std::string& name = "");

    // Schema migration.
    //
  public:
    static void
    migrate_schema_pre (database& db,
                        schema_version v,
                        const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_pre);
    }

    static void
    migrate_schema_post (database& db,
                         schema_version v,
                         const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_post);
    }

    static void
    migrate_schema (database& db,
                    schema_version v,
                    const std::string& name = "")
    {
      migrate_schema_impl (db, v, name, migrate_both);
    }

    // Data migration.
    //
  public:
    // If version is 0, then use the current database version and also
    // check whether we are in migration. Returns the number of calls made.
    //
    static std::size_t
    migrate_data (database&,
                  schema_version = 0,
                  const std::string& name = "");

#ifdef ODB_CXX11
    typedef std::function<void (database&)> data_migration_function_type;
#else
    typedef void (*data_migration_function_type) (database&);
#endif

    // The following three variants of the registration functions make
    // sure that the version is greater that the base model version.
    // This helps with identifying and removing data migration function
    // that are no longer required.
    //
    // Data migration functions are called in the order of registration.
    //
    template <schema_version v, schema_version base>
    static void
    data_migration_function (data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function<v, base> (id_common, f, name);
    }

    // Database-specific data migration.
    //
    template <schema_version v, schema_version base>
    static void
    data_migration_function (database& db,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function<v, base> (db.id (), f, name);
    }

    template <schema_version v, schema_version base>
    static void
    data_migration_function (database_id id,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      // If the data migration version is below the base model version
      // then it will never be called.
      //
#ifdef ODB_CXX11
      static_assert (v > base || base == 0,
                     "data migration function is no longer necessary");
#else
      // Poor man's static_assert.
      //
      typedef details::meta::static_assert_test<(v > base || base == 0)>
      data_migration_function_is_no_longer_necessary;

      char sa [sizeof (data_migration_function_is_no_longer_necessary)];
      ODB_POTENTIALLY_UNUSED (sa);
#endif

      data_migration_function (id, v, f, name);
    }

    // The same as above but take the version as an argument and do
    // not check whether it is greater than the base model version.
    //
    static void
    data_migration_function (schema_version v,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function (id_common, v, f, name);
    }

    static void
    data_migration_function (database& db,
                             schema_version v,
                             data_migration_function_type f,
                             const std::string& name = "")
    {
      data_migration_function (db.id (), v, f, name);
    }

    static void
    data_migration_function (database_id,
                             schema_version,
                             data_migration_function_type,
                             const std::string& name = "");

    // Combined schema and data migration.
    //
  public:
    // Migrate both schema and data to the specified version. If version
    // is not specified, then migrate to the current model version.
    //
    static void
    migrate (database&, schema_version = 0, const std::string& name = "");

    // Schema version information.
    //
  public:
    // Return the base model version.
    //
    static schema_version
    base_version (const database& db, const std::string& name = "")
    {
      return base_version (db.id (), name);
    }

    static schema_version
    base_version (database_id, const std::string& name = "");

    // Return the current model version.
    //
    static schema_version
    current_version (const database& db, const std::string& name = "")
    {
      return current_version (db.id (), name);
    }

    static schema_version
    current_version (database_id, const std::string& name = "");

    // Return current model version + 1 (that is, one past current) if
    // the passed version is equal to or greater than current. If the
    // version is not specified, then use the current database version.
    //
    static schema_version
    next_version (const database& db,
                  schema_version v = 0,
                  const std::string& name = "")
    {
      return next_version (db.id (), v == 0 ? db.schema_version () : v, name);
    }

    static schema_version
    next_version (database_id,
                  schema_version,
                  const std::string& name = "");

    // Schema existence.
    //
  public:
    // Test for presence of a schema with a specific name.
    //
    static bool
    exists (const database& db, const std::string& name = "")
    {
      return exists (db.id (), name);
    }

    static bool
    exists (database_id, const std::string& name = "");

  private:
    enum migrate_mode
    {
      migrate_pre,
      migrate_post,
      migrate_both
    };

    static void
    migrate_schema_impl (database&,
                         schema_version,
                         const std::string& name,
                         migrate_mode);
  };

  // Static data migration function registration.
  //
  template <schema_version v, schema_version base>
  struct data_migration_entry
  {
    typedef schema_catalog::data_migration_function_type function_type;

    data_migration_entry (function_type f, const std::string& name = "")
    {
      schema_catalog::data_migration_function<v, base> (f, name);
    }

    data_migration_entry (database_id id,
                          function_type f,
                          const std::string& name = "")
    {
      schema_catalog::data_migration_function<v, base> (id, v, f, name);
    }
  };

  namespace common
  {
    using odb::schema_catalog;
    using odb::data_migration_entry;
  }
}

#include <odb/post.hxx>

#endif // ODB_SCHEMA_CATALOG_HXX
