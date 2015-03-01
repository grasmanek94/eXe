// file      : odb/database.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DATABASE_HXX
#define ODB_DATABASE_HXX

#include <odb/pre.hxx>

#include <odb/details/config.hxx> // ODB_CXX11

#include <map>
#include <string>
#include <memory>  // std::auto_ptr, std::unique_ptr
#include <cstddef> // std::size_t

#ifdef ODB_CXX11
#  include <functional> // std::function
#endif

#include <odb/traits.hxx>
#include <odb/forward.hxx>
#include <odb/schema-version.hxx>
#include <odb/query.hxx>
#include <odb/prepared-query.hxx>
#include <odb/result.hxx>
#include <odb/connection.hxx>
#include <odb/exceptions.hxx>

#include <odb/details/export.hxx>
#include <odb/details/mutex.hxx>
#include <odb/details/c-string.hxx>

namespace odb
{
  class transaction_impl;

  class LIBODB_EXPORT database
  {
  public:
    virtual
    ~database ();

    // Object persistence API.
    //
  public:

    // Make the object persistent.
    //
    template <typename T>
    typename object_traits<T>::id_type
    persist (T& object);

    template <typename T>
    typename object_traits<T>::id_type
    persist (T* obj_ptr);

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (const P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    typename object_traits<T>::id_type
    persist (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    typename object_traits<T>::id_type
    persist (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    typename object_traits<T>::id_type
    persist (P<T, A1>& obj_ptr);

    template <typename T>
    typename object_traits<T>::id_type
    persist (const typename object_traits<T>::pointer_type& obj_ptr);

    // Load an object. Throw object_not_persistent if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    load (const typename object_traits<T>::id_type& id);

    template <typename T>
    void
    load (const typename object_traits<T>::id_type& id, T& object);

    // Load (or reload, if it is already loaded) a section of an object.
    //
    template <typename T>
    void
    load (T& object, section&);

    // Reload an object.
    //
    template <typename T>
    void
    reload (T& object);

    template <typename T>
    void
    reload (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    reload (const P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    reload (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    reload (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    reload (P<T, A1>& obj_ptr);

    template <typename T>
    void
    reload (const typename object_traits<T>::pointer_type& obj_ptr);

    // Loan an object if found. Return NULL/false if not found.
    //
    template <typename T>
    typename object_traits<T>::pointer_type
    find (const typename object_traits<T>::id_type& id);

    template <typename T>
    bool
    find (const typename object_traits<T>::id_type& id, T& object);

    // Update the state of a modified objects.
    //
    template <typename T>
    void
    update (T& object);

    template <typename T>
    void
    update (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    update (const P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    update (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    update (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    update (P<T, A1>& obj_ptr);

    template <typename T>
    void
    update (const typename object_traits<T>::pointer_type& obj_ptr);

    // Update a section of an object. Throws section_not_loaded exception
    // if section is not loaded. Note also that this function does not
    // clear the changed flag if it is set.
    //
    template <typename T>
    void
    update (const T& object, const section&);

    // Make the object transient. Throw object_not_persistent if not
    // found.
    //
    template <typename T>
    void
    erase (const typename object_traits<T>::id_type& id);

    template <typename T>
    void
    erase (T& object);

    template <typename T>
    void
    erase (T* obj_ptr);

    template <typename T, template <typename> class P>
    void
    erase (const P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    erase (const P<T, A1>& obj_ptr);

    template <typename T, template <typename> class P>
    void
    erase (P<T>& obj_ptr);

    template <typename T, typename A1, template <typename, typename> class P>
    void
    erase (P<T, A1>& obj_ptr);

    template <typename T>
    void
    erase (const typename object_traits<T>::pointer_type& obj_ptr);

    // Erase multiple objects matching a query predicate.
    //
    template <typename T>
    unsigned long long
    erase_query ();

    template <typename T>
    unsigned long long
    erase_query (const char*);

    template <typename T>
    unsigned long long
    erase_query (const std::string&);

    template <typename T>
    unsigned long long
    erase_query (const odb::query<T>&);

    // Query API.
    //
    template <typename T>
    result<T>
    query (bool cache = true);

    template <typename T>
    result<T>
    query (const char*, bool cache = true);

    template <typename T>
    result<T>
    query (const std::string&, bool cache = true);

    template <typename T>
    result<T>
    query (const odb::query<T>&, bool cache = true);

    // Query preparation.
    //
    template <typename T>
    prepared_query<T>
    prepare_query (const char* name, const char*);

    template <typename T>
    prepared_query<T>
    prepare_query (const char* name, const std::string&);

    template <typename T>
    prepared_query<T>
    prepare_query (const char* name, const odb::query<T>&);

    template <typename T>
    void
    cache_query (const prepared_query<T>&);

    template <typename T, typename P>
    void
    cache_query (const prepared_query<T>&, std::auto_ptr<P> params);

#ifdef ODB_CXX11
    template <typename T, typename P>
    void
    cache_query (const prepared_query<T>&, std::unique_ptr<P> params);
#endif

    template <typename T>
    prepared_query<T>
    lookup_query (const char* name) const;

    template <typename T, typename P>
    prepared_query<T>
    lookup_query (const char* name, P*& params) const;

    // Prepared query factory.
    //
  public:
    typedef odb::connection connection_type;

#ifdef ODB_CXX11
    typedef
    std::function<void (const char*, connection_type&)>
    query_factory_type;
#else
    typedef void (*query_factory_type) (const char*, connection_type&);
#endif

    void
    query_factory (const char* name, query_factory_type);

    query_factory_type
    lookup_query_factory (const char* name) const;

    // Native database statement execution.
    //
  public:
    unsigned long long
    execute (const char* statement);

    unsigned long long
    execute (const std::string& statement);

    unsigned long long
    execute (const char* statement, std::size_t length);

    // Transactions.
    //
  public:
    virtual transaction_impl*
    begin () = 0;

    // Connections.
    //
  public:
    connection_ptr
    connection ();

    // SQL statement tracing.
    //
  public:
    typedef odb::tracer tracer_type;

    void
    tracer (tracer_type&);

    void
    tracer (tracer_type*);

    tracer_type*
    tracer () const;

    // Database schema version.
    //
  public:
    typedef odb::schema_version schema_version_type;
    typedef odb::schema_version_migration schema_version_migration_type;

    schema_version_type
    schema_version (const std::string& schema_name = "") const;

    bool
    schema_migration (const std::string& schema_name = "") const;

    // Note that there is code that relies on the returned reference
    // being valid until the version is changed or the database instance
    // is destroyed.
    //
    const schema_version_migration_type&
    schema_version_migration (const std::string& schema_name = "") const;

    // Set schema version and migration state manually.
    //
    // Note that the modifier API is not thread-safe. That is, you should
    // not modify the schema version while other threads may be accessing
    // or modifying the same information.
    //
    void
    schema_version_migration (schema_version_type,
                              bool migration,
                              const std::string& schema_name = "");

    void
    schema_version_migration (const schema_version_migration_type&,
                              const std::string& schema_name = "");

    // Set default schema version table for all the schema names. The table
    // name should already be quoted if necessary.
    //
    void
    schema_version_table (const std::string& table_name);

    // Set schema version table for a specific schema.
    //
    void
    schema_version_table (const std::string& table_name,
                          const std::string& schema_name);

    // Schema version sequence number. It is incremented every time the
    // schema version or migration flag is changed and can be used to
    // detect overall version changes. The starting value is 1.
    //
    unsigned int
    schema_version_sequence () const;

  protected:
    struct schema_version_info: schema_version_migration_type
    {
      std::string version_table;
    };

    virtual const schema_version_info&
    load_schema_version (const std::string& schema_name) const = 0;

  private:
    const schema_version_info&
    schema_version_migration_ (const std::string& schema_name) const;

    // Database id.
    //
  public:
    database_id
    id () const;

  protected:
    database (database_id);

  private:
    database (const database&);
    database& operator= (const database&);

  protected:
    virtual connection_type*
    connection_ () = 0;

  protected:
    template <typename T, database_id DB>
    typename object_traits<T>::id_type
    persist_ (T&);

    template <typename T, database_id DB>
    typename object_traits<T>::id_type
    persist_ (const typename object_traits<T>::pointer_type&);

    template <typename T, database_id DB>
    typename object_traits<T>::pointer_type
    load_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    void
    load_ (const typename object_traits<T>::id_type&, T&);

    template <typename T, database_id DB>
    void
    load_ (T&, section&);

    template <typename T, database_id DB>
    void
    reload_ (T&);

    template <typename T, database_id DB>
    typename object_traits<T>::pointer_type
    find_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    bool
    find_ (const typename object_traits<T>::id_type&, T&);

    template <typename T, database_id DB>
    void
    update_ (T&);

    template <typename T, database_id DB>
    void
    update_ (const typename object_traits<T>::pointer_type&);

    template <typename T, database_id DB>
    void
    update_ (const T&, const section&);

    template <typename T, database_id DB>
    void
    erase_ (const typename object_traits<T>::id_type&);

    template <typename T, database_id DB>
    void
    erase_ (T&);

    template <typename T, database_id DB>
    void
    erase_ (const typename object_traits<T>::pointer_type&);

    template <typename T,
              database_id DB,
              class_kind kind = class_traits<T>::kind>
    struct query_;

  protected:
    typedef
    std::map<const char*, query_factory_type, details::c_string_comparator>
    query_factory_map;

    typedef std::map<std::string, schema_version_info> schema_version_map;

    database_id id_;
    tracer_type* tracer_;
    query_factory_map query_factory_map_;

    mutable details::mutex mutex_;
    mutable schema_version_map schema_version_map_;
    std::string schema_version_table_;
    unsigned int schema_version_seq_;
  };
}

#include <odb/database.ixx>
#include <odb/database.txx>

#include <odb/post.hxx>

#endif // ODB_DATABASE_HXX
