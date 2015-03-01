// file      : odb/sqlite/database.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifdef _WIN32
#  include <odb/details/win32/windows.hxx> // WideCharToMultiByte
#endif

#include <sqlite3.h>

#include <cassert>
#include <sstream>

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/connection-factory.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/transaction.hxx>
#include <odb/sqlite/error.hxx>
#include <odb/sqlite/exceptions.hxx>

#include <odb/sqlite/details/options.hxx>

using namespace std;

namespace odb
{
  namespace sqlite
  {
    using odb::details::transfer_ptr;

    database::
    ~database ()
    {
    }

    database::
    database (const string& name,
              int flags,
              bool foreign_keys,
              const string& vfs,
              transfer_ptr<connection_factory> factory)
        : odb::database (id_sqlite),
          name_ (name),
          flags_ (flags),
          foreign_keys_ (foreign_keys),
          vfs_ (vfs),
          factory_ (factory.transfer ())
    {
      if (!factory_)
        factory_.reset (new connection_pool_factory ());

      factory_->database (*this);
    }

#ifdef _WIN32
    database::
    database (const wstring& name,
              int flags,
              bool foreign_keys,
              const string& vfs,
              transfer_ptr<connection_factory> factory)
        : odb::database (id_sqlite),
          flags_ (flags),
          foreign_keys_ (foreign_keys),
          vfs_ (vfs),
          factory_ (factory.transfer ())
    {
      // Convert UTF-16 name to UTF-8 using the WideCharToMultiByte() Win32
      // function.
      //
      int n (
        WideCharToMultiByte (
          CP_UTF8,
          0,
          name.c_str (),
          static_cast<int> (name.size ()),
          0,
          0,
          0,
          0));

      if (n == 0)
        throw database_exception (
          SQLITE_CANTOPEN, SQLITE_CANTOPEN, "unable to open database file");

      // This string is not shared so we are going to modify the underlying
      // buffer directly.
      //
      name_.resize (static_cast<string::size_type> (n));

      n = WideCharToMultiByte (
        CP_UTF8,
        0,
        name.c_str (),
        static_cast<int> (name.size ()),
        const_cast<char*> (name_.c_str ()),
        n,
        0,
        0);

      if (n == 0)
        throw database_exception (
          SQLITE_CANTOPEN, SQLITE_CANTOPEN, "unable to open database file");

      if (!factory_)
        factory_.reset (new connection_pool_factory ());

      factory_->database (*this);
    }
#endif

    database::
    database (int& argc,
              char* argv[],
              bool erase,
              int flags,
              bool foreign_keys,
              const string& vfs,
              transfer_ptr<connection_factory> factory)
        : odb::database (id_sqlite),
          flags_ (flags),
          foreign_keys_ (foreign_keys),
          vfs_ (vfs),
          factory_ (factory.transfer ())
    {
      using namespace details;

      try
      {
        cli::argv_file_scanner scan (argc, argv, "--options-file", erase);
        options ops (scan, cli::unknown_mode::skip, cli::unknown_mode::skip);

        name_ = ops.database ();

        if (ops.create ())
          flags_ |= SQLITE_OPEN_CREATE;

        if (ops.read_only ())
          flags_ = (flags_ & ~SQLITE_OPEN_READWRITE) | SQLITE_OPEN_READONLY;
      }
      catch (const cli::exception& e)
      {
        ostringstream ostr;
        ostr << e;
        throw cli_exception (ostr.str ());
      }

      if (!factory_)
        factory_.reset (new connection_pool_factory ());

      factory_->database (*this);
    }

    void database::
    print_usage (std::ostream& os)
    {
      details::options::print_usage (os);
    }

    transaction_impl* database::
    begin ()
    {
      return new transaction_impl (*this, transaction_impl::deferred);
    }

    transaction_impl* database::
    begin_immediate ()
    {
      return new transaction_impl (*this, transaction_impl::immediate);
    }

    transaction_impl* database::
    begin_exclusive ()
    {
      return new transaction_impl (*this, transaction_impl::exclusive);
    }

    odb::connection* database::
    connection_ ()
    {
      connection_ptr c (factory_->connect ());
      return c.release ();
    }

    const database::schema_version_info& database::
    load_schema_version (const string& name) const
    {
      schema_version_info& svi (schema_version_map_[name]);

      // Construct the SELECT statement text.
      //
      string text ("SELECT \"version\", \"migration\" FROM ");

      if (!svi.version_table.empty ())
        text += svi.version_table; // Already quoted.
      else if (!schema_version_table_.empty ())
        text += schema_version_table_; // Already quoted.
      else
        text += "\"schema_version\"";

      text += " WHERE \"name\" = ?";

      // Bind parameters and results.
      //
      size_t psize[1] = {name.size ()};
      bind pbind[1] = {{bind::text,
                        const_cast<char*> (name.c_str ()),
                        &psize[0],
                        0, 0, 0}};
      binding param (pbind, 1);
      param.version++;

      long long migration;
      bool rnull[2];
      bind rbind[2] = {{bind::integer, &svi.version, 0, 0, &rnull[0], 0},
                       {bind::integer, &migration, 0, 0, &rnull[1], 0}};
      binding result (rbind, 2);
      result.version++;

      // If we are not in transaction, SQLite will start an implicit one
      // which suits us just fine.
      //
      connection_ptr cp;
      if (!transaction::has_current ())
        cp = factory_->connect ();

      sqlite::connection& c (
        cp != 0 ? *cp : transaction::current ().connection ());

      try
      {
        select_statement st (c,
                             text,
                             false, // Don't process.
                             false, // Don't optimize.
                             param,
                             result);
        st.execute ();
        auto_result ar (st);

        switch (st.fetch ())
        {
        case select_statement::success:
          {
            svi.migration = migration != 0;
            assert (st.fetch () == select_statement::no_data);
            break;
          }
        case select_statement::no_data:
          {
            svi.version = 0; // No schema.
            break;
          }
        case select_statement::truncated:
          {
            assert (false);
            break;
          }
        }
      }
      catch (const database_exception& e)
      {
        // Try to detect the case where there is no version table. SQLite
        // doesn't have an extended error code for this so we have to use
        // the error text.
        //
        if (e.error () == SQLITE_ERROR &&
            e.message ().compare (0, 14, "no such table:") == 0)
          svi.version = 0; // No schema.
        else
          throw;
      }

      return svi;
    }
  }
}
// file      : odb/sqlite/connection.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <new>    // std::bad_alloc
#include <string>
#include <cassert>

#include <odb/details/lock.hxx>

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/transaction.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/statement-cache.hxx>
#include <odb/sqlite/prepared-query.hxx>
#include <odb/sqlite/error.hxx>
#include <odb/sqlite/exceptions.hxx> // deadlock

#include <odb/sqlite/details/config.hxx> // LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY

using namespace std;

extern "C" void
odb_sqlite_connection_unlock_callback(void**, int);

namespace odb
{
	namespace sqlite
	{
		connection::
			connection(database_type& db, int extra_flags)
			: odb::connection(db),
			db_(db),
			unlock_cond_(unlock_mutex_),
			statements_(0)
		{
			int f(db.flags() | extra_flags);
			const string& n(db.name());

			// If we are opening a temporary database, then add the create flag.
			//
			if (n.empty() || n == ":memory:")
				f |= SQLITE_OPEN_CREATE;

			// A connection can only be used by a single thread at a time. So
			// disable locking in SQLite unless explicitly requested.
			//
#if defined(SQLITE_OPEN_NOMUTEX)
			if ((f & SQLITE_OPEN_FULLMUTEX) == 0)
				f |= SQLITE_OPEN_NOMUTEX;
#endif

			sqlite3* h(0);

			// sqlite3_open_v2() was only addedin SQLite 3.5.0.
			//
#if SQLITE_VERSION_NUMBER >= 3005000
			const string& vfs(db.vfs());
			int e(
				sqlite3_open_v2(
				n.c_str(), &h, f, (vfs.empty() ? 0 : vfs.c_str())));
#else
			// Readonly opening not supported in SQLite earlier than 3.5.0.
			//
			assert((f & SQLITE_OPEN_READONLY) == 0);
			int e(sqlite3_open(n.c_str(), &h));
#endif

			handle_.reset(h);

			if (e != SQLITE_OK)
			{
				if (handle_ == 0)
					throw bad_alloc();

				translate_error(e, *this);
			}

			init();
		}

		connection::
			connection(database_type& db, sqlite3* handle)
			: odb::connection(db),
			db_(db),
			handle_(handle),
			unlock_cond_(unlock_mutex_),
			statements_(0)
		{
			init();
		}

		void connection::
			init()
		{
			// Enable/disable foreign key constraints.
			//
			generic_statement st(
				*this,
				db_.foreign_keys()
				? "PRAGMA foreign_keys=ON"
				: "PRAGMA foreign_keys=OFF",
				db_.foreign_keys() ? 22 : 23);
			st.execute();

			// Create statement cache.
			//
			statement_cache_.reset(new statement_cache_type(*this));
		}

		connection::
			~connection()
		{
			// Destroy prepared query statements before freeing the connections.
			//
			recycle();
			clear_prepared_map();
		}

		transaction_impl* connection::
			begin()
		{
			return new transaction_impl(
				connection_ptr(inc_ref(this)), transaction_impl::deferred);
		}

		transaction_impl* connection::
			begin_immediate()
		{
			return new transaction_impl(
				connection_ptr(inc_ref(this)), transaction_impl::immediate);
		}

		transaction_impl* connection::
			begin_exclusive()
		{
			return new transaction_impl(
				connection_ptr(inc_ref(this)), transaction_impl::exclusive);
		}

		unsigned long long connection::
			execute(const char* s, std::size_t n)
		{
			generic_statement st(*this, s, n);
			return st.execute();
		}

		inline void
			connection_unlock_callback(void** args, int n)
		{
			for (int i(0); i < n; ++i)
			{
				connection* c(static_cast<connection*> (args[i]));
				details::lock l(c->unlock_mutex_);
				c->unlocked_ = true;
				c->unlock_cond_.signal();
			}
		}

		void connection::
			wait()
		{
#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
			unlocked_ = false;

			// unlock_notify() returns SQLITE_OK or SQLITE_LOCKED (deadlock).
			//
			int e(sqlite3_unlock_notify(handle_,
				&odb_sqlite_connection_unlock_callback,
				this));
			if (e == SQLITE_LOCKED)
				throw deadlock();

			details::lock l(unlock_mutex_);

			while (!unlocked_)
				unlock_cond_.wait();
#else
			translate_error(SQLITE_LOCKED, *this);
#endif
		}

		void connection::
			clear()
		{
			// The current first statement will remove itself from the list
			// and make the second statement (if any) the new first.
			//
			while (statements_ != 0)
				statements_->reset();
		}
	}
}

extern "C" void
odb_sqlite_connection_unlock_callback(void** args, int n)
{
	odb::sqlite::connection_unlock_callback(args, n);
}
// file      : odb/sqlite/connection-factory.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/lock.hxx>

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/connection-factory.hxx>

#include <odb/sqlite/details/config.hxx> // LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY

using namespace std;

namespace odb
{
	using namespace details;

	namespace sqlite
	{
		//
		// connection_factory
		//

		connection_factory::
			~connection_factory()
		{
		}

		//
		// single_connection_factory
		//

		single_connection_factory::
			~single_connection_factory()
		{
			// If the connection is currently in use, wait for it to return to
			// the factory.
			//
			lock l(mutex_);
		}

		single_connection_factory::single_connection_ptr
			single_connection_factory::
			create()
		{
			return single_connection_ptr(new (shared)single_connection(*db_));
		}

		connection_ptr single_connection_factory::
			connect()
		{
			mutex_.lock();
			connection_->factory_ = this;
			connection_ptr r(connection_);
			connection_.reset();
			return r;
		}

		void single_connection_factory::
			database(database_type& db)
		{
			db_ = &db;
			connection_ = create();
		}

		bool single_connection_factory::
			release(single_connection* c)
		{
			c->factory_ = 0;
			connection_.reset(inc_ref(c));
			connection_->recycle();
			mutex_.unlock();
			return false;
		}

		//
		// single_connection_factory::single_connection
		//

		single_connection_factory::single_connection::
			single_connection(database_type& db, int extra_flags)
			: connection(db, extra_flags), factory_(0)
		{
			callback_.arg = this;
			callback_.zero_counter = &zero_counter;
			shared_base::callback_ = &callback_;
		}

		single_connection_factory::single_connection::
			single_connection(database_type& db, sqlite3* handle)
			: connection(db, handle), factory_(0)
		{
			callback_.arg = this;
			callback_.zero_counter = &zero_counter;
			shared_base::callback_ = &callback_;
		}

		bool single_connection_factory::single_connection::
			zero_counter(void* arg)
		{
			single_connection* c(static_cast<single_connection*> (arg));
			return c->factory_ ? c->factory_->release(c) : true;
		}

		//
		// new_connection_factory
		//

		connection_ptr new_connection_factory::
			connect()
		{
			return connection_ptr(
				new (shared)connection(*db_, extra_flags_));
		}

		void new_connection_factory::
			database(database_type& db)
		{
			db_ = &db;

			// Unless explicitly disabled, enable shared cache.
			//
#if SQLITE_VERSION_NUMBER >= 3006018 && defined(LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY)
			if ((db_->flags() & SQLITE_OPEN_PRIVATECACHE) == 0)
				extra_flags_ |= SQLITE_OPEN_SHAREDCACHE;
#endif
		}

		//
		// connection_pool_factory
		//

		connection_pool_factory::pooled_connection_ptr connection_pool_factory::
			create()
		{
			return pooled_connection_ptr(
				new (shared)pooled_connection(*db_, extra_flags_));
		}

		connection_pool_factory::
			~connection_pool_factory()
		{
			// Wait for all the connections currently in use to return to the pool.
			//
			lock l(mutex_);
			while (in_use_ != 0)
			{
				waiters_++;
				cond_.wait();
				waiters_--;
			}
		}

		connection_ptr connection_pool_factory::
			connect()
		{
			lock l(mutex_);

			while (true)
			{
				// See if we have a spare connection.
				//
				if (connections_.size() != 0)
				{
					shared_ptr<pooled_connection> c(connections_.back());
					connections_.pop_back();

					c->pool_ = this;
					in_use_++;
					return c;
				}

				// See if we can create a new one.
				//
				if (max_ == 0 || in_use_ < max_)
				{
					shared_ptr<pooled_connection> c(create());
					c->pool_ = this;
					in_use_++;
					return c;
				}

				// Wait until someone releases a connection.
				//
				waiters_++;
				cond_.wait();
				waiters_--;
			}
		}

		void connection_pool_factory::
			database(database_type& db)
		{
			db_ = &db;

			// Unless explicitly disabled, enable shared cache.
			//
#if SQLITE_VERSION_NUMBER >= 3006018 && defined(LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY)
			if ((db_->flags() & SQLITE_OPEN_PRIVATECACHE) == 0)
				extra_flags_ |= SQLITE_OPEN_SHAREDCACHE;
#endif

			if (min_ > 0)
			{
				connections_.reserve(min_);

				for (size_t i(0); i < min_; ++i)
					connections_.push_back(create());
			}
		}

		bool connection_pool_factory::
			release(pooled_connection* c)
		{
			c->pool_ = 0;

			lock l(mutex_);

			// Determine if we need to keep or free this connection.
			//
			bool keep(waiters_ != 0 ||
				min_ == 0 ||
				(connections_.size() + in_use_ <= min_));

			in_use_--;

			if (keep)
			{
				connections_.push_back(pooled_connection_ptr(inc_ref(c)));
				connections_.back()->recycle();
			}

			if (waiters_ != 0)
				cond_.signal();

			return !keep;
		}

		//
		// connection_pool_factory::pooled_connection
		//

		connection_pool_factory::pooled_connection::
			pooled_connection(database_type& db, int extra_flags)
			: connection(db, extra_flags), pool_(0)
		{
			callback_.arg = this;
			callback_.zero_counter = &zero_counter;
			shared_base::callback_ = &callback_;
		}

		connection_pool_factory::pooled_connection::
			pooled_connection(database_type& db, sqlite3* handle)
			: connection(db, handle), pool_(0)
		{
			callback_.arg = this;
			callback_.zero_counter = &zero_counter;
			shared_base::callback_ = &callback_;
		}

		bool connection_pool_factory::pooled_connection::
			zero_counter(void* arg)
		{
			pooled_connection* c(static_cast<pooled_connection*> (arg));
			return c->pool_ ? c->pool_->release(c) : true;
		}
	}
}
// file      : odb/sqlite/error.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <sqlite3.h>

#include <new>    // std::bad_alloc
#include <string>

#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/exceptions.hxx>

#include <odb/sqlite/details/config.hxx> // LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY

using namespace std;

namespace odb
{
	namespace sqlite
	{
		void
			translate_error(int e, connection& c)
		{
			sqlite3* h(c.handle());

			// Extended error codes are only available in 3.6.5 and later.
			//
#if SQLITE_VERSION_NUMBER >= 3006005
			int ee(sqlite3_extended_errcode(h));
#else
			int ee(0);
#endif
			string m;

			switch (e)
			{
			case SQLITE_NOMEM:
			{
				throw bad_alloc();
			}
			case SQLITE_MISUSE:
			{
				// In case of SQLITE_MISUSE, error code/message may or may not
				// be set.
				//
				ee = e;
				m = "SQLite API misuse";
				break;
			}
#ifdef SQLITE_ABORT_ROLLBACK
			case SQLITE_ABORT:
			{
				if (ee == SQLITE_ABORT_ROLLBACK)
					throw forced_rollback();

				break;
			}
#endif
			case SQLITE_LOCKED:
			{
#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
				if (ee != SQLITE_LOCKED_SHAREDCACHE)
					throw deadlock(); // The DROP TABLE special case.
#endif
				// Getting SQLITE_LOCKED_SHAREDCACHE here means we don't have
				// the unlock notify support. Translate this to timeout.
				//
				throw timeout();
			}
			case SQLITE_BUSY:
			case SQLITE_IOERR:
			{
#if SQLITE_VERSION_NUMBER >= 3006005
				if (e != SQLITE_IOERR || ee == SQLITE_IOERR_BLOCKED)
					throw timeout();
#endif
				break;
			}
			default:
				break;
			}

			if (m.empty())
				m = sqlite3_errmsg(h);

			throw database_exception(e, ee, m);
		}
	}
}
// file      : odb/sqlite/exceptions.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <sstream>

#include <odb/sqlite/exceptions.hxx>

using namespace std;

namespace odb
{
	namespace sqlite
	{
		//
		// forced_rollback
		//

		const char* forced_rollback::
			what() const throw ()
		{
			return "transaction is forced to rollback";
		}

		//
		// database_exception
		//

		database_exception::
			~database_exception() throw ()
		{
		}

		database_exception::
			database_exception(int e, int ee, const string& m)
			: error_(e), extended_error_(ee), message_(m)
		{
			ostringstream ostr;
			ostr << error_;

			if (error_ != extended_error_)
				ostr << " (" << extended_error_ << ")";

			ostr << ": " << message_;
			what_ = ostr.str();
		}

		const char* database_exception::
			what() const throw ()
		{
			return what_.c_str();
		}

		//
		// cli_exception
		//

		cli_exception::
			cli_exception(const std::string& what)
			: what_(what)
		{
		}

		cli_exception::
			~cli_exception() throw ()
		{
		}

		const char* cli_exception::
			what() const throw ()
		{
			return what_.c_str();
		}
	}
}
// This code was generated by CLI, a command line interface
// compiler for C++.
//

// Begin prologue.
//
//
// End prologue.

#include "details/options.hxx"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <cstring>
#include <fstream>

namespace odb
{
	namespace sqlite
	{
		namespace details
		{
			namespace cli
			{
				// unknown_option
				//
				unknown_option::
					~unknown_option() throw ()
				{
				}

				void unknown_option::
					print(::std::ostream& os) const
				{
					os << "unknown option '" << option().c_str() << "'";
				}

				const char* unknown_option::
					what() const throw ()
				{
					return "unknown option";
				}

				// unknown_argument
				//
				unknown_argument::
					~unknown_argument() throw ()
				{
				}

				void unknown_argument::
					print(::std::ostream& os) const
				{
					os << "unknown argument '" << argument().c_str() << "'";
				}

				const char* unknown_argument::
					what() const throw ()
				{
					return "unknown argument";
				}

				// missing_value
				//
				missing_value::
					~missing_value() throw ()
				{
				}

				void missing_value::
					print(::std::ostream& os) const
				{
					os << "missing value for option '" << option().c_str() << "'";
				}

				const char* missing_value::
					what() const throw ()
				{
					return "missing option value";
				}

				// invalid_value
				//
				invalid_value::
					~invalid_value() throw ()
				{
				}

				void invalid_value::
					print(::std::ostream& os) const
				{
					os << "invalid value '" << value().c_str() << "' for option '"
						<< option().c_str() << "'";
				}

				const char* invalid_value::
					what() const throw ()
				{
					return "invalid option value";
				}

				// eos_reached
				//
				void eos_reached::
					print(::std::ostream& os) const
				{
					os << what();
				}

				const char* eos_reached::
					what() const throw ()
				{
					return "end of argument stream reached";
				}

				// file_io_failure
				//
				file_io_failure::
					~file_io_failure() throw ()
				{
				}

				void file_io_failure::
					print(::std::ostream& os) const
				{
					os << "unable to open file '" << file().c_str() << "' or read failure";
				}

				const char* file_io_failure::
					what() const throw ()
				{
					return "unable to open file or read failure";
				}

				// unmatched_quote
				//
				unmatched_quote::
					~unmatched_quote() throw ()
				{
				}

				void unmatched_quote::
					print(::std::ostream& os) const
				{
					os << "unmatched quote in argument '" << argument().c_str() << "'";
				}

				const char* unmatched_quote::
					what() const throw ()
				{
					return "unmatched quote";
				}

				// scanner
				//
				scanner::
					~scanner()
				{
				}

				// argv_scanner
				//
				bool argv_scanner::
					more()
				{
					return i_ < argc_;
				}

				const char* argv_scanner::
					peek()
				{
					if (i_ < argc_)
						return argv_[i_];
					else
						throw eos_reached();
				}

				const char* argv_scanner::
					next()
				{
					if (i_ < argc_)
					{
						const char* r(argv_[i_]);

						if (erase_)
						{
							for (int i(i_ + 1); i < argc_; ++i)
								argv_[i - 1] = argv_[i];

							--argc_;
							argv_[argc_] = 0;
						}
						else
							++i_;

						return r;
					}
					else
						throw eos_reached();
				}

				void argv_scanner::
					skip()
				{
					if (i_ < argc_)
						++i_;
					else
						throw eos_reached();
				}

				// argv_file_scanner
				//
				bool argv_file_scanner::
					more()
				{
					if (!args_.empty())
						return true;

					while (base::more())
					{
						// See if the next argument is the file option.
						//
						const char* a(base::peek());
						const option_info* oi;

						if (!skip_ && (oi = find(a)))
						{
							base::next();

							if (!base::more())
								throw missing_value(oi->option);

							if (oi->search_func != 0)
							{
								std::string f(oi->search_func(base::next(), oi->arg));

								if (!f.empty())
									load(f);
							}
							else
								load(base::next());

							if (!args_.empty())
								return true;
						}
						else
						{
							if (!skip_)
								skip_ = (std::strcmp(a, "--") == 0);

							return true;
						}
					}

					return false;
				}

				const char* argv_file_scanner::
					peek()
				{
					if (!more())
						throw eos_reached();

					return args_.empty() ? base::peek() : args_.front().c_str();
				}

				const char* argv_file_scanner::
					next()
				{
					if (!more())
						throw eos_reached();

					if (args_.empty())
						return base::next();
					else
					{
						hold_.swap(args_.front());
						args_.pop_front();
						return hold_.c_str();
					}
				}

				void argv_file_scanner::
					skip()
				{
					if (!more())
						throw eos_reached();

					if (args_.empty())
						return base::skip();
					else
						args_.pop_front();
				}

				const argv_file_scanner::option_info* argv_file_scanner::
					find(const char* a) const
				{
					for (std::size_t i(0); i < options_count_; ++i)
						if (std::strcmp(a, options_[i].option) == 0)
							return &options_[i];

					return 0;
				}

				void argv_file_scanner::
					load(const std::string& file)
				{
					using namespace std;

					ifstream is(file.c_str());

					if (!is.is_open())
						throw file_io_failure(file);

					while (!is.eof())
					{
						string line;
						getline(is, line);

						if (is.fail() && !is.eof())
							throw file_io_failure(file);

						string::size_type n(line.size());

						// Trim the line from leading and trailing whitespaces.
						//
						if (n != 0)
						{
							const char* f(line.c_str());
							const char* l(f + n);

							const char* of(f);
							while (f < l && (*f == ' ' || *f == '\t' || *f == '\r'))
								++f;

							--l;

							const char* ol(l);
							while (l > f && (*l == ' ' || *l == '\t' || *l == '\r'))
								--l;

							if (f != of || l != ol)
								line = f <= l ? string(f, l - f + 1) : string();
						}

						// Ignore empty lines, those that start with #.
						//
						if (line.empty() || line[0] == '#')
							continue;

						string::size_type p(line.find(' '));

						if (p == string::npos)
						{
							if (!skip_)
								skip_ = (line == "--");

							args_.push_back(line);
						}
						else
						{
							string s1(line, 0, p);

							// Skip leading whitespaces in the argument.
							//
							n = line.size();
							for (++p; p < n; ++p)
							{
								char c(line[p]);

								if (c != ' ' && c != '\t' && c != '\r')
									break;
							}

							string s2(line, p);

							// If the string is wrapped in quotes, remove them.
							//
							n = s2.size();
							char cf(s2[0]), cl(s2[n - 1]);

							if (cf == '"' || cf == '\'' || cl == '"' || cl == '\'')
							{
								if (n == 1 || cf != cl)
									throw unmatched_quote(s2);

								s2 = string(s2, 1, n - 2);
							}

							const option_info* oi;
							if (!skip_ && (oi = find(s1.c_str())))
							{
								if (s2.empty())
									throw missing_value(oi->option);

								if (oi->search_func != 0)
								{
									std::string f(oi->search_func(s2.c_str(), oi->arg));

									if (!f.empty())
										load(f);
								}
								else
									load(s2);
							}
							else
							{
								args_.push_back(s1);
								args_.push_back(s2);
							}
						}
					}
				}

				template <typename X>
				struct parser
				{
					static void
						parse(X& x, scanner& s)
					{
						std::string o(s.next());

						if (s.more())
						{
							std::string v(s.next());
							std::istringstream is(v);
							if (!(is >> x && is.eof()))
								throw invalid_value(o, v);
						}
						else
							throw missing_value(o);
					}
				};

				template <>
				struct parser<bool>
				{
					static void
						parse(bool& x, scanner& s)
					{
						s.next();
						x = true;
					}
				};

				template <>
				struct parser<std::string>
				{
					static void
						parse(std::string& x, scanner& s)
					{
						const char* o(s.next());

						if (s.more())
							x = s.next();
						else
							throw missing_value(o);
					}
				};

				template <typename X>
				struct parser<std::vector<X> >
				{
					static void
						parse(std::vector<X>& c, scanner& s)
					{
						X x;
						parser<X>::parse(x, s);
						c.push_back(x);
					}
				};

				template <typename X>
				struct parser<std::set<X> >
				{
					static void
						parse(std::set<X>& c, scanner& s)
					{
						X x;
						parser<X>::parse(x, s);
						c.insert(x);
					}
				};

				template <typename K, typename V>
				struct parser<std::map<K, V> >
				{
					static void
						parse(std::map<K, V>& m, scanner& s)
					{
						std::string o(s.next());

						if (s.more())
						{
							std::string ov(s.next());
							std::string::size_type p = ov.find('=');

							if (p == std::string::npos)
							{
								K k = K();

								if (!ov.empty())
								{
									std::istringstream ks(ov);

									if (!(ks >> k && ks.eof()))
										throw invalid_value(o, ov);
								}

								m[k] = V();
							}
							else
							{
								K k = K();
								V v = V();
								std::string kstr(ov, 0, p);
								std::string vstr(ov, p + 1);

								if (!kstr.empty())
								{
									std::istringstream ks(kstr);

									if (!(ks >> k && ks.eof()))
										throw invalid_value(o, ov);
								}

								if (!vstr.empty())
								{
									std::istringstream vs(vstr);

									if (!(vs >> v && vs.eof()))
										throw invalid_value(o, ov);
								}

								m[k] = v;
							}
						}
						else
							throw missing_value(o);
					}
				};

				template <typename X, typename T, T X::*M>
				void
					thunk(X& x, scanner& s)
				{
					parser<T>::parse(x.*M, s);
				}
			}
		}
	}
}

#include <map>
#include <cstring>

namespace odb
{
	namespace sqlite
	{
		namespace details
		{
			// options
			//

			options::
				options()
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
			}

			options::
				options(int& argc,
				char** argv,
				bool erase,
				::odb::sqlite::details::cli::unknown_mode opt,
				::odb::sqlite::details::cli::unknown_mode arg)
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
				::odb::sqlite::details::cli::argv_scanner s(argc, argv, erase);
				_parse(s, opt, arg);
			}

			options::
				options(int start,
				int& argc,
				char** argv,
				bool erase,
				::odb::sqlite::details::cli::unknown_mode opt,
				::odb::sqlite::details::cli::unknown_mode arg)
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
				::odb::sqlite::details::cli::argv_scanner s(start, argc, argv, erase);
				_parse(s, opt, arg);
			}

			options::
				options(int& argc,
				char** argv,
				int& end,
				bool erase,
				::odb::sqlite::details::cli::unknown_mode opt,
				::odb::sqlite::details::cli::unknown_mode arg)
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
				::odb::sqlite::details::cli::argv_scanner s(argc, argv, erase);
				_parse(s, opt, arg);
				end = s.end();
			}

			options::
				options(int start,
				int& argc,
				char** argv,
				int& end,
				bool erase,
				::odb::sqlite::details::cli::unknown_mode opt,
				::odb::sqlite::details::cli::unknown_mode arg)
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
				::odb::sqlite::details::cli::argv_scanner s(start, argc, argv, erase);
				_parse(s, opt, arg);
				end = s.end();
			}

			options::
				options(::odb::sqlite::details::cli::scanner& s,
				::odb::sqlite::details::cli::unknown_mode opt,
				::odb::sqlite::details::cli::unknown_mode arg)
				: database_(),
				create_(),
				read_only_(),
				options_file_()
			{
				_parse(s, opt, arg);
			}

			void options::
				print_usage(::std::ostream& os)
			{
				os << "--database <filename> SQLite database file name. If the database file is not" << ::std::endl
					<< "                      specified then a private, temporary on-disk database will" << ::std::endl
					<< "                      be created. Use the ':memory:' special name to create a" << ::std::endl
					<< "                      private, temporary in-memory database." << ::std::endl;

				os << "--create              Create the SQLite database if it does not already exist." << ::std::endl
					<< "                      By default opening the database fails if it does not" << ::std::endl
					<< "                      already exist." << ::std::endl;

				os << "--read-only           Open the SQLite database in read-only mode. By default" << ::std::endl
					<< "                      the database is opened for reading and writing if" << ::std::endl
					<< "                      possible, or reading only if the file is write-protected" << ::std::endl
					<< "                      by the operating system." << ::std::endl;

				os << "--options-file <file> Read additional options from <file>. Each option" << ::std::endl
					<< "                      appearing on a separate line optionally followed by space" << ::std::endl
					<< "                      and an option value. Empty lines and lines starting with" << ::std::endl
					<< "                      '#' are ignored." << ::std::endl;
			}

			typedef
				std::map<std::string, void(*) (options&, ::odb::sqlite::details::cli::scanner&)>
				_cli_options_map;

			static _cli_options_map _cli_options_map_;

			struct _cli_options_map_init
			{
				_cli_options_map_init()
				{
					_cli_options_map_["--database"] =
						&::odb::sqlite::details::cli::thunk< options, std::string, &options::database_ >;
					_cli_options_map_["--create"] =
						&::odb::sqlite::details::cli::thunk< options, bool, &options::create_ >;
					_cli_options_map_["--read-only"] =
						&::odb::sqlite::details::cli::thunk< options, bool, &options::read_only_ >;
					_cli_options_map_["--options-file"] =
						&::odb::sqlite::details::cli::thunk< options, std::string, &options::options_file_ >;
				}
			};

			static _cli_options_map_init _cli_options_map_init_;

			bool options::
				_parse(const char* o, ::odb::sqlite::details::cli::scanner& s)
			{
				_cli_options_map::const_iterator i(_cli_options_map_.find(o));

				if (i != _cli_options_map_.end())
				{
					(*(i->second)) (*this, s);
					return true;
				}

				return false;
			}

			void options::
				_parse(::odb::sqlite::details::cli::scanner& s,
				::odb::sqlite::details::cli::unknown_mode opt_mode,
				::odb::sqlite::details::cli::unknown_mode arg_mode)
			{
				bool opt = true;

				while (s.more())
				{
					const char* o = s.peek();

					if (std::strcmp(o, "--") == 0)
					{
						s.skip();
						opt = false;
						continue;
					}

					if (opt && _parse(o, s));
					else if (opt && std::strncmp(o, "-", 1) == 0 && o[1] != '\0')
					{
						switch (opt_mode)
						{
						case ::odb::sqlite::details::cli::unknown_mode::skip:
						{
							s.skip();
							continue;
						}
						case ::odb::sqlite::details::cli::unknown_mode::stop:
						{
							break;
						}
						case ::odb::sqlite::details::cli::unknown_mode::fail:
						{
							throw ::odb::sqlite::details::cli::unknown_option(o);
						}
						}

						break;
					}
					else
					{
						switch (arg_mode)
						{
						case ::odb::sqlite::details::cli::unknown_mode::skip:
						{
							s.skip();
							continue;
						}
						case ::odb::sqlite::details::cli::unknown_mode::stop:
						{
							break;
						}
						case ::odb::sqlite::details::cli::unknown_mode::fail:
						{
							throw ::odb::sqlite::details::cli::unknown_argument(o);
						}
						}

						break;
					}
				}
			}
		}
	}
}

// Begin epilogue.
//
//
// End epilogue.

// file      : odb/sqlite/prepared-query.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/prepared-query.hxx>

namespace odb
{
	namespace sqlite
	{
		prepared_query_impl::
			~prepared_query_impl()
		{
		}
	}
}
// file      : odb/sqlite/query.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstddef> // std::size_t
#include <cstring> // std::memset

#include <odb/sqlite/query.hxx>

using namespace std;

namespace odb
{
	namespace sqlite
	{
		// query_param
		//

		query_param::
			~query_param()
		{
		}

		// query_params
		//

		query_params::
			query_params(const query_params& x)
			: details::shared_base(x),
			params_(x.params_), bind_(x.bind_), binding_(0, 0)
		{
			// Here and below we want to maintain up to date binding info so
			// that the call to binding() below is an immutable operation,
			// provided the query does not have any by-reference parameters.
			// This way a by-value-only query can be shared between multiple
			// threads without the need for synchronization.
			//
			if (size_t n = bind_.size())
			{
				binding_.bind = &bind_[0];
				binding_.count = n;
				binding_.version++;
			}
		}

		query_params& query_params::
			operator= (const query_params& x)
		{
			if (this != &x)
			{
				params_ = x.params_;
				bind_ = x.bind_;

				size_t n(bind_.size());
				binding_.bind = n != 0 ? &bind_[0] : 0;
				binding_.count = n;
				binding_.version++;
			}

			return *this;
		}

		query_params& query_params::
			operator+= (const query_params& x)
		{
			size_t n(bind_.size());

			params_.insert(params_.end(), x.params_.begin(), x.params_.end());
			bind_.insert(bind_.end(), x.bind_.begin(), x.bind_.end());

			if (n != bind_.size())
			{
				binding_.bind = &bind_[0];
				binding_.count = bind_.size();
				binding_.version++;
			}

			return *this;
		}

		void query_params::
			add(details::shared_ptr<query_param> p)
		{
			params_.push_back(p);
			bind_.push_back(sqlite::bind());
			binding_.bind = &bind_[0];
			binding_.count = bind_.size();
			binding_.version++;

			sqlite::bind* b(&bind_.back());
			memset(b, 0, sizeof(sqlite::bind));
			p->bind(b);
		}

		void query_params::
			init()
		{
			bool inc_ver(false);

			for (size_t i(0); i < params_.size(); ++i)
			{
				query_param& p(*params_[i]);

				if (p.reference())
				{
					if (p.init())
					{
						p.bind(&bind_[i]);
						inc_ver = true;
					}
				}
			}

			if (inc_ver)
				binding_.version++;
		}

		// query_base
		//

		query_base::
			query_base(const query_base& q)
			: clause_(q.clause_),
			parameters_(new (details::shared) query_params(*q.parameters_))
		{
		}

		query_base& query_base::
			operator= (const query_base& q)
		{
			if (this != &q)
			{
				clause_ = q.clause_;
				*parameters_ = *q.parameters_;
			}

			return *this;
		}

		query_base& query_base::
			operator+= (const query_base& q)
		{
			clause_.insert(clause_.end(), q.clause_.begin(), q.clause_.end());
			*parameters_ += *q.parameters_;
			return *this;
		}

		void query_base::
			append(const string& q)
		{
			if (!clause_.empty() &&
				clause_.back().kind == clause_part::kind_native)
			{
				string& s(clause_.back().part);

				char first(!q.empty() ? q[0] : ' ');
				char last(!s.empty() ? s[s.size() - 1] : ' ');

				// We don't want extra spaces after '(' as well as before ','
				// and ')'.
				//
				if (last != ' ' && last != '\n' && last != '(' &&
					first != ' ' && first != '\n' && first != ',' && first != ')')
					s += ' ';

				s += q;
			}
			else
				clause_.push_back(clause_part(clause_part::kind_native, q));
		}

		void query_base::
			append(const char* table, const char* column)
		{
			string s(table);
			s += '.';
			s += column;

			clause_.push_back(clause_part(clause_part::kind_column, s));
		}

		void query_base::
			append(details::shared_ptr<query_param> p, const char* conv)
		{
			clause_.push_back(clause_part(clause_part::kind_param));

			if (conv != 0)
				clause_.back().part = conv;

			parameters_->add(p);
		}

		static bool
			check_prefix(const string& s)
		{
			string::size_type n;

			// It is easier to compare to upper and lower-case versions
			// rather than getting involved with the portable case-
			// insensitive string comparison mess.
			//
			if (s.compare(0, (n = 5), "WHERE") == 0 ||
				s.compare(0, (n = 5), "where") == 0 ||
				s.compare(0, (n = 6), "SELECT") == 0 ||
				s.compare(0, (n = 6), "select") == 0 ||
				s.compare(0, (n = 8), "ORDER BY") == 0 ||
				s.compare(0, (n = 8), "order by") == 0 ||
				s.compare(0, (n = 8), "GROUP BY") == 0 ||
				s.compare(0, (n = 8), "group by") == 0 ||
				s.compare(0, (n = 6), "HAVING") == 0 ||
				s.compare(0, (n = 6), "having") == 0)
			{
				// It either has to be an exact match, or there should be
				// a whitespace following the keyword.
				//
				if (s.size() == n || s[n] == ' ' || s[n] == '\n' || s[n] == '\t')
					return true;
			}

			return false;
		}

		void query_base::
			optimize()
		{
			// Remove a single TRUE literal or one that is followe by one of
			// the other clauses. This avoids useless WHERE clauses like
			//
			// WHERE TRUE GROUP BY foo
			//
			clause_type::iterator i(clause_.begin()), e(clause_.end());

			if (i != e && i->kind == clause_part::kind_bool && i->bool_part)
			{
				clause_type::iterator j(i + 1);

				if (j == e ||
					(j->kind == clause_part::kind_native && check_prefix(j->part)))
					clause_.erase(i);
			}
		}

		const char* query_base::
			clause_prefix() const
		{
			if (!clause_.empty())
			{
				const clause_part& p(clause_.front());

				if (p.kind == clause_part::kind_native && check_prefix(p.part))
					return "";

				return "WHERE ";
			}

			return "";
		}

		string query_base::
			clause() const
		{
			string r;

			for (clause_type::const_iterator i(clause_.begin()),
				end(clause_.end());
				i != end;
			++i)
			{
				char last(!r.empty() ? r[r.size() - 1] : ' ');

				switch (i->kind)
				{
				case clause_part::kind_column:
				{
					if (last != ' ' && last != '\n' && last != '(')
						r += ' ';

					r += i->part;
					break;
				}
				case clause_part::kind_param:
				{
					if (last != ' ' && last != '\n' && last != '(')
						r += ' ';

					// Add the conversion expression, if any.
					//
					string::size_type p(0);
					if (!i->part.empty())
					{
						p = i->part.find("(?)");
						r.append(i->part, 0, p);
					}

					r += '?';

					if (!i->part.empty())
						r.append(i->part, p + 3, string::npos);

					break;
				}
				case clause_part::kind_native:
				{
					// We don't want extra spaces after '(' as well as before ','
					// and ')'.
					//
					const string& p(i->part);
					char first(!p.empty() ? p[0] : ' ');

					if (last != ' ' && first != '\n' && last != '(' &&
						first != ' ' && last != '\n' && first != ',' && first != ')')
						r += ' ';

					r += p;
					break;
				}
				case clause_part::kind_bool:
				{
					if (last != ' ' && last != '\n' && last != '(')
						r += ' ';

					r += i->bool_part ? "1" : "0";
					break;
				}
				}
			}

			return clause_prefix() + r;
		}

		query_base
			operator&& (const query_base& x, const query_base& y)
		{
			// Optimize cases where one or both sides are constant truth.
			//
			bool xt(x.const_true()), yt(y.const_true());

			if (xt && yt)
				return x;

			if (xt)
				return y;

			if (yt)
				return x;

			query_base r("(");
			r += x;
			r += ") AND (";
			r += y;
			r += ")";
			return r;
		}

		query_base
			operator|| (const query_base& x, const query_base& y)
		{
			query_base r("(");
			r += x;
			r += ") OR (";
			r += y;
			r += ")";
			return r;
		}

		query_base
			operator! (const query_base& x)
		{
			query_base r("NOT (");
			r += x;
			r += ")";
			return r;
		}
	}
}
// file      : odb/sqlite/query-const-expr.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/query.hxx>

namespace odb
{
	namespace sqlite
	{
		// Sun CC cannot handle this in query.cxx.
		//
		const query_base query_base::true_expr(true);
	}
}
// file      : odb/sqlite/query-dynamic.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstddef> // std::size_t

#include <odb/sqlite/query-dynamic.hxx>

using namespace std;

namespace odb
{
	namespace sqlite
	{
		static const char* logic_operators[] = { ") AND (", ") OR (" };
		static const char* comp_operators[] = { "=", "!=", "<", ">", "<=", ">=" };

		static void
			translate(query_base& q, const odb::query_base& s, size_t p)
		{
			typedef odb::query_base::clause_part part;

			const part& x(s.clause()[p]);

			switch (x.kind)
			{
			case part::kind_column:
			{
				const query_column_base* c(
					static_cast<const query_column_base*> (
					x.native_info[id_sqlite].column));

				q.append(c->table(), c->column());
				break;
			}
			case part::kind_param_val:
			case part::kind_param_ref:
			{
				const query_column_base* c(
					static_cast<const query_column_base*> (
					x.native_info[id_sqlite].column));

				query_param_factory f(
					reinterpret_cast<query_param_factory> (
					x.native_info[id_sqlite].param_factory));

				const odb::query_param* p(
					reinterpret_cast<const odb::query_param*> (x.data));

				q.append(f(p->value, x.kind == part::kind_param_ref),
					c->conversion());
				break;
			}
			case part::kind_native:
			{
				q.append(s.strings()[x.data]);
				break;
			}
			case part::kind_true:
			case part::kind_false:
			{
				q.append(x.kind == part::kind_true);
				break;
			}
			case part::op_add:
			{
				translate(q, s, x.data);
				translate(q, s, p - 1);
				break;
			}
			case part::op_and:
			case part::op_or:
			{
				q += "(";
				translate(q, s, x.data);
				q += logic_operators[x.kind - part::op_and];
				translate(q, s, p - 1);
				q += ")";
				break;
			}
			case part::op_not:
			{
				q += "NOT (";
				translate(q, s, p - 1);
				q += ")";
				break;
			}
			case part::op_null:
			case part::op_not_null:
			{
				translate(q, s, p - 1);
				q += (x.kind == part::op_null ? "IS NULL" : "IS NOT NULL");
				break;
			}
			case part::op_in:
			{
				size_t b(p - x.data);

				translate(q, s, b - 1); // column
				q += "IN (";

				for (size_t i(b); i != p; ++i)
				{
					if (i != b)
						q += ",";

					translate(q, s, i);
				}

				q += ")";
				break;
			}
			case part::op_like:
			{
				translate(q, s, p - 2); // column
				q += "LIKE";
				translate(q, s, p - 1); // pattern
				break;
			}
			case part::op_like_escape:
			{
				translate(q, s, p - 3); // column
				q += "LIKE";
				translate(q, s, p - 2); // pattern
				q += "ESCAPE";
				translate(q, s, p - 1); // escape
				break;
			}
			case part::op_eq:
			case part::op_ne:
			case part::op_lt:
			case part::op_gt:
			case part::op_le:
			case part::op_ge:
			{
				translate(q, s, x.data);
				q += comp_operators[x.kind - part::op_eq];
				translate(q, s, p - 1);
				break;
			}
			}
		}

		query_base::
			query_base(const odb::query_base& q)
			: parameters_(new (details::shared) query_params)
		{
			if (!q.empty())
				translate(*this, q, q.clause().size() - 1);
		}
	}
}
// file      : odb/sqlite/simple-object-statements.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/simple-object-statements.hxx>

namespace odb
{
	namespace sqlite
	{
		object_statements_base::
			~object_statements_base()
		{
		}
	}
}
// file      : odb/sqlite/statement.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/tracer.hxx>
#include <odb/exceptions.hxx> // object_not_persistent

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/error.hxx>

#include <odb/sqlite/details/config.hxx> // LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY

using namespace std;

namespace odb
{
	namespace sqlite
	{
		// statement
		//

		statement::
			~statement()
		{
			if (empty())
				return;

			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->deallocate(conn_, *this);
			}

			if (next_ != this)
				list_remove();

			stmt_.reset();
		}

		void statement::
			init(const char* text,
			std::size_t text_size,
			statement_kind sk,
			const binding* proc,
			bool optimize)
		{
			string tmp;
			if (proc != 0)
			{
				switch (sk)
				{
				case statement_select:
					process_select(text,
						&proc->bind->buffer, proc->count, sizeof(bind),
						'"', '"',
						optimize,
						tmp);
					break;
				case statement_insert:
					process_insert(text,
						&proc->bind->buffer, proc->count, sizeof(bind),
						'?',
						tmp);
					break;
				case statement_update:
					process_update(text,
						&proc->bind->buffer, proc->count, sizeof(bind),
						'?',
						tmp);
					break;
				case statement_delete:
				case statement_generic:
					assert(false);
				}

				text = tmp.c_str();
				text_size = tmp.size();
			}

			// Empty statement.
			//
			if (*text == '\0')
				return;

			int e;
			sqlite3_stmt* stmt(0);

			// sqlite3_prepare_v2() is only available since SQLite 3.3.9
			// but is buggy until 3.3.11.
			//
#if SQLITE_VERSION_NUMBER >= 3003011
			while ((e = sqlite3_prepare_v2(conn_.handle(),
				text,
				static_cast<int> (text_size),
				&stmt,
				0)) == SQLITE_LOCKED)
			{
				conn_.wait();
			}
#else
			e = sqlite3_prepare(conn_.handle(),
				text,
				static_cast<int> (text_size),
				&stmt,
				0);
#endif

			if (e != SQLITE_OK)
				translate_error(e, conn_);

			stmt_.reset(stmt);

#if SQLITE_VERSION_NUMBER < 3005003
			text_.assign(text, text_size);
#endif

			active_ = false;

			prev_ = 0;
			next_ = this;

			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->prepare(conn_, *this);
			}
		}

		const char* statement::
			text() const
		{
			// sqlite3_sql() is only available since 3.5.3.
			//
#if SQLITE_VERSION_NUMBER >= 3005003
			return sqlite3_sql(stmt_);
#else
			return text_.c_str();
#endif
		}

		void statement::
			bind_param(const bind* p, size_t n)
		{
			int e(SQLITE_OK);

			// SQLite parameters are counted from 1.
			//
			for (size_t i(0), j(1); e == SQLITE_OK && i < n; ++i)
			{
				const bind& b(p[i]);

				if (b.buffer == 0) // Skip NULL entries.
					continue;

				int c(static_cast<int> (j++));

				if (b.is_null != 0 && *b.is_null)
				{
					e = sqlite3_bind_null(stmt_, c);
					continue;
				}

				switch (b.type)
				{
				case bind::integer:
				{
					long long v(*static_cast<long long*> (b.buffer));

					e = sqlite3_bind_int64(stmt_,
						c,
						// Prior to 3.5.0, sqlite3_int64 was called sqlite_int64.
#if SQLITE_VERSION_NUMBER >= 3005000
						static_cast<sqlite3_int64> (v)
#else
						static_cast<sqlite_int64> (v)
#endif
						);
					break;
				}
				case bind::real:
				{
					double v(*static_cast<double*> (b.buffer));
					e = sqlite3_bind_double(stmt_, c, v);
					break;
				}
				case bind::text:
				{
					e = sqlite3_bind_text(stmt_,
						c,
						static_cast<const char*> (b.buffer),
						static_cast<int> (*b.size),
						SQLITE_STATIC);
					break;
				}
				case bind::text16:
				{
					e = sqlite3_bind_text16(stmt_,
						c,
						b.buffer,
						static_cast<int> (*b.size),
						SQLITE_STATIC);
					break;
				}
				case bind::blob:
				{
					e = sqlite3_bind_blob(stmt_,
						c,
						b.buffer,
						static_cast<int> (*b.size),
						SQLITE_STATIC);
					break;
				}
				}
			}

			if (e != SQLITE_OK)
				translate_error(e, conn_);
		}

		bool statement::
			bind_result(const bind* p, size_t count, bool truncated)
		{
			bool r(true);
			int col_count(sqlite3_data_count(stmt_));

			int col(0);
			for (size_t i(0); i != count && col != col_count; ++i)
			{
				const bind& b(p[i]);

				if (b.buffer == 0) // Skip NULL entries.
					continue;

				int c(col++);

				if (truncated && (b.truncated == 0 || !*b.truncated))
					continue;

				if (b.truncated != 0)
					*b.truncated = false;

				// Check for NULL unless we are reloading a truncated result.
				//
				if (!truncated)
				{
					*b.is_null = sqlite3_column_type(stmt_, c) == SQLITE_NULL;

					if (*b.is_null)
						continue;
				}

				switch (b.type)
				{
				case bind::integer:
				{
					*static_cast<long long*> (b.buffer) =
						static_cast<long long> (sqlite3_column_int64(stmt_, c));
					break;
				}
				case bind::real:
				{
					*static_cast<double*> (b.buffer) =
						sqlite3_column_double(stmt_, c);
					break;
				}
				case bind::text:
				case bind::text16:
				case bind::blob:
				{
					// SQLite documentation recommends that we first call *_text(),
					// *_text16(), or *_blob() function in order to force the type
					// conversion, if any.
					//
					const void* d;

					if (b.type != bind::text16)
					{
						d = b.type == bind::text
							? sqlite3_column_text(stmt_, c)
							: sqlite3_column_blob(stmt_, c);
						*b.size = static_cast<size_t> (sqlite3_column_bytes(stmt_, c));
					}
					else
					{
						d = sqlite3_column_text16(stmt_, c);
						*b.size = static_cast<size_t> (
							sqlite3_column_bytes16(stmt_, c));
					}

					if (*b.size > b.capacity)
					{
						if (b.truncated != 0)
							*b.truncated = true;

						r = false;
						continue;
					}

					memcpy(b.buffer, d, *b.size);
					break;
				}
				}
			}

			// Make sure that the number of columns in the result returned by
			// the database matches the number that we expect. A common cause
			// of this assertion is a native view with a number of data members
			// not matching the number of columns in the SELECT-list.
			//
			assert(col == col_count);

			return r;
		}

		// generic_statement
		//

		generic_statement::
			generic_statement(connection_type& conn, const string& text)
			: statement(conn,
			text, statement_generic,
			0, false),
			result_set_(stmt_ ? sqlite3_column_count(stmt_) != 0 : false)
		{
		}

		generic_statement::
			generic_statement(connection_type& conn, const char* text)
			: statement(conn,
			text, statement_generic,
			0, false),
			result_set_(stmt_ ? sqlite3_column_count(stmt_) != 0 : false)
		{
		}

		generic_statement::
			generic_statement(connection_type& conn,
			const char* text,
			std::size_t text_size)
			: statement(conn,
			text, text_size, statement_generic,
			0, false),
			result_set_(stmt_ ? sqlite3_column_count(stmt_) != 0 : false)
		{
		}

		unsigned long long generic_statement::
			execute()
		{
			if (stmt_ == 0) // Empty statement or comment.
				return 0;

			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->execute(conn_, *this);
			}

			unsigned long long r(0);

			int e;
			sqlite3* h(conn_.handle());

#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
			// Only the first call to sqlite3_step() can return SQLITE_LOCKED.
			//
			while ((e = sqlite3_step(stmt_)) == SQLITE_LOCKED)
			{
				if (sqlite3_extended_errcode(h) != SQLITE_LOCKED_SHAREDCACHE)
					break;

				sqlite3_reset(stmt_);
				conn_.wait();
			}
#else
			e = sqlite3_step(stmt_);
#endif

			for (; e == SQLITE_ROW; e = sqlite3_step(stmt_))
				r++;

			// sqlite3_step() will return a detailed error code only if we used
			// sqlite3_prepare_v2(). Otherwise, sqlite3_reset() returns the
			// error.
			//
#if SQLITE_VERSION_NUMBER >= 3003011
			sqlite3_reset(stmt_);

			if (e != SQLITE_DONE)
				translate_error(e, conn_);
#else
			e = sqlite3_reset(stmt_);

			if (e != SQLITE_OK)
			{
				// If the schema has changed, try to re-prepare and re-execute the
				// statement. That's what newer versions of SQLite do automatically.
				//
				if (e == SQLITE_SCHEMA)
				{
					sqlite3_stmt* stmt(0);
					e = sqlite3_prepare(h,
						text_.c_str(),
						static_cast<int> (text_.size() + 1),
						&stmt,
						0);

					if (e != SQLITE_OK)
						translate_error(e, conn_);

					stmt_.reset(stmt);
					return execute(); // Try again by recursively calling ourselves.
				}
				else
					translate_error(e, conn_);
			}
#endif

			if (!result_set_)
				r = static_cast<unsigned long long> (sqlite3_changes(h));

			return r;
		}

		// select_statement
		//

		select_statement::
			select_statement(connection_type& conn,
			const string& text,
			bool process,
			bool optimize,
			binding& param,
			binding& result)
			: statement(conn,
			text, statement_select,
			(process ? &result : 0), optimize),
			param_(&param),
			result_(result)
		{
		}

		select_statement::
			select_statement(connection_type& conn,
			const char* text,
			bool process,
			bool optimize,
			binding& param,
			binding& result)
			: statement(conn,
			text, statement_select,
			(process ? &result : 0), optimize),
			param_(&param),
			result_(result)
		{
		}

		select_statement::
			select_statement(connection_type& conn,
			const string& text,
			bool process,
			bool optimize,
			binding& result)
			: statement(conn,
			text, statement_select,
			(process ? &result : 0), optimize),
			param_(0),
			result_(result)
		{
		}

		select_statement::
			select_statement(connection_type& conn,
			const char* text,
			bool process,
			bool optimize,
			binding& result)
			: statement(conn,
			text, statement_select,
			(process ? &result : 0), optimize),
			param_(0),
			result_(result)
		{
		}

		void select_statement::
			execute()
		{
			if (active())
				reset();

			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->execute(conn_, *this);
			}

			done_ = false;

			if (param_ != 0)
				bind_param(param_->bind, param_->count);

			active(true);
		}

		void select_statement::
			free_result()
		{
			reset();
			done_ = true;
		}

		bool select_statement::
			next()
		{
			if (!done_)
			{
				int e;

#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
				sqlite3* h(conn_.handle());
				while ((e = sqlite3_step(stmt_)) == SQLITE_LOCKED)
				{
					if (sqlite3_extended_errcode(h) != SQLITE_LOCKED_SHAREDCACHE)
						break;

					sqlite3_reset(stmt_);
					conn_.wait();
				}
#else
				e = sqlite3_step(stmt_);
#endif

				if (e != SQLITE_ROW)
				{
					done_ = true;

					// sqlite3_step() will return a detailed error code only if we used
					// sqlite3_prepare_v2(). Otherwise, sqlite3_reset() returns the
					// error.
					//
#if SQLITE_VERSION_NUMBER >= 3003011
					reset();

					if (e != SQLITE_DONE)
#else
					e = reset();

					if (e != SQLITE_OK)
#endif
						translate_error(e, conn_);
				}
			}

			return !done_;
		}

		select_statement::result select_statement::
			load()
		{
			if (done_)
				return no_data;

			return bind_result(result_.bind, result_.count) ? success : truncated;
		}

		void select_statement::
			reload()
		{
			assert(!done_);

			if (!bind_result(result_.bind, result_.count, true))
				assert(false);
		}

		// insert_statement
		//

		insert_statement::
			insert_statement(connection_type& conn,
			const string& text,
			bool process,
			binding& param)
			: statement(conn,
			text, statement_insert,
			(process ? &param : 0), false),
			param_(param)
		{
		}

		insert_statement::
			insert_statement(connection_type& conn,
			const char* text,
			bool process,
			binding& param)
			: statement(conn,
			text, statement_insert,
			(process ? &param : 0), false),
			param_(param)
		{
		}

		bool insert_statement::
			execute()
		{
			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->execute(conn_, *this);
			}

			bind_param(param_.bind, param_.count);

			int e;

#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
			sqlite3* h(conn_.handle());
			while ((e = sqlite3_step(stmt_)) == SQLITE_LOCKED)
			{
				if (sqlite3_extended_errcode(h) != SQLITE_LOCKED_SHAREDCACHE)
					break;

				sqlite3_reset(stmt_);
				conn_.wait();
			}
#else
			e = sqlite3_step(stmt_);
#endif

			// sqlite3_step() will return a detailed error code only if we used
			// sqlite3_prepare_v2(). Otherwise, sqlite3_reset() returns the
			// error.
			//
#if SQLITE_VERSION_NUMBER >= 3003011
			sqlite3_reset(stmt_);

			if (e != SQLITE_DONE)
#else
			e = sqlite3_reset(stmt_);

			if (e != SQLITE_OK)
#endif
			{
				// SQLITE_CONSTRAINT error code covers more than just a duplicate
				// primary key. Unfortunately, there is nothing more precise that
				// we can use (even sqlite3_errmsg() returns generic "constraint
				// failed").
				//
				if (e == SQLITE_CONSTRAINT)
					return false;
				else
					translate_error(e, conn_);
			}

			return true;
		}

		unsigned long long insert_statement::
			id()
		{
			return static_cast<unsigned long long> (
				sqlite3_last_insert_rowid(conn_.handle()));
		}

		// update_statement
		//

		update_statement::
			update_statement(connection_type& conn,
			const string& text,
			bool process,
			binding& param)
			: statement(conn,
			text, statement_update,
			(process ? &param : 0), false),
			param_(param)
		{
		}

		update_statement::
			update_statement(connection_type& conn,
			const char* text,
			bool process,
			binding& param)
			: statement(conn,
			text, statement_update,
			(process ? &param : 0), false),
			param_(param)
		{
		}

		unsigned long long update_statement::
			execute()
		{
			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->execute(conn_, *this);
			}

			bind_param(param_.bind, param_.count);

			int e;
			sqlite3* h(conn_.handle());

#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
			while ((e = sqlite3_step(stmt_)) == SQLITE_LOCKED)
			{
				if (sqlite3_extended_errcode(h) != SQLITE_LOCKED_SHAREDCACHE)
					break;

				sqlite3_reset(stmt_);
				conn_.wait();
			}
#else
			e = sqlite3_step(stmt_);
#endif

			// sqlite3_step() will return a detailed error code only if we used
			// sqlite3_prepare_v2(). Otherwise, sqlite3_reset() returns the
			// error.
			//
#if SQLITE_VERSION_NUMBER >= 3003011
			sqlite3_reset(stmt_);

			if (e != SQLITE_DONE)
#else
			e = sqlite3_reset(stmt_);

			if (e != SQLITE_OK)
#endif
				translate_error(e, conn_);

			return static_cast<unsigned long long> (sqlite3_changes(h));
		}

		// delete_statement
		//

		delete_statement::
			delete_statement(connection_type& conn,
			const string& text,
			binding& param)
			: statement(conn,
			text, statement_delete,
			0, false),
			param_(param)
		{
		}

		delete_statement::
			delete_statement(connection_type& conn,
			const char* text,
			binding& param)
			: statement(conn,
			text, statement_delete,
			0, false),
			param_(param)
		{
		}

		unsigned long long delete_statement::
			execute()
		{
			{
				odb::tracer* t;
				if ((t = conn_.transaction_tracer()) ||
					(t = conn_.tracer()) ||
					(t = conn_.database().tracer()))
					t->execute(conn_, *this);
			}

			bind_param(param_.bind, param_.count);

			int e;
			sqlite3* h(conn_.handle());

#ifdef LIBODB_SQLITE_HAVE_UNLOCK_NOTIFY
			while ((e = sqlite3_step(stmt_)) == SQLITE_LOCKED)
			{
				if (sqlite3_extended_errcode(h) != SQLITE_LOCKED_SHAREDCACHE)
					break;

				sqlite3_reset(stmt_);
				conn_.wait();
			}
#else
			e = sqlite3_step(stmt_);
#endif

			// sqlite3_step() will return a detailed error code only if we used
			// sqlite3_prepare_v2(). Otherwise, sqlite3_reset() returns the
			// error.
			//
#if SQLITE_VERSION_NUMBER >= 3003011
			sqlite3_reset(stmt_);

			if (e != SQLITE_DONE)
#else
			e = sqlite3_reset(stmt_);

			if (e != SQLITE_OK)
#endif
				translate_error(e, conn_);

			return static_cast<unsigned long long> (sqlite3_changes(h));
		}
	}
}
// file      : odb/sqlite/statement-cache.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/statement-cache.hxx>

namespace odb
{
	using namespace details;

	namespace sqlite
	{
		statement_cache::
			statement_cache(connection& conn)
			: conn_(conn),
			version_seq_(conn.database().schema_version_sequence()),
			// String lengths below include '\0', as per SQLite manual
			// suggestions.
			//
			begin_(new (shared)generic_statement(conn_, "BEGIN", 6)),
			commit_(new (shared)generic_statement(conn_, "COMMIT", 7)),
			rollback_(new (shared)generic_statement(conn_, "ROLLBACK", 9))
		{
		}

		void statement_cache::
			begin_immediate_statement_() const
		{
			begin_immediate_.reset(
				new (shared)generic_statement(conn_, "BEGIN IMMEDIATE", 16));
		}

		void statement_cache::
			begin_exclusive_statement_() const
		{
			begin_exclusive_.reset(
				new (shared)generic_statement(conn_, "BEGIN EXCLUSIVE", 16));
		}
	}
}
// file      : odb/sqlite/statements-base.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/statements-base.hxx>

namespace odb
{
	namespace sqlite
	{
		statements_base::
			~statements_base()
		{
		}
	}
}
// file      : odb/sqlite/tracer.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/tracer.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/statement.hxx>

namespace odb
{
	namespace sqlite
	{
		tracer::
			~tracer()
		{
		}

		void tracer::
			prepare(connection&, const statement&)
		{
		}

		void tracer::
			execute(connection& c, const statement& s)
		{
			execute(c, s.text());
		}

		void tracer::
			deallocate(connection&, const statement&)
		{
		}

		void tracer::
			prepare(odb::connection& c, const odb::statement& s)
		{
			prepare(static_cast<connection&> (c),
				static_cast<const statement&> (s));
		}

		void tracer::
			execute(odb::connection& c, const odb::statement& s)
		{
			execute(static_cast<connection&> (c),
				static_cast<const statement&> (s));
		}

		void tracer::
			execute(odb::connection& c, const char* s)
		{
			execute(static_cast<connection&> (c), s);
		}

		void tracer::
			deallocate(odb::connection& c, const odb::statement& s)
		{
			deallocate(static_cast<connection&> (c),
				static_cast<const statement&> (s));
		}
	}
}
// file      : odb/sqlite/traits.cxx
// copyright : Copyright (c) 2005-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/sqlite/traits.hxx>

using namespace std;

namespace odb
{
	namespace sqlite
	{
		using details::buffer;

		//
		// default_value_traits<std::string>
		//
		void default_value_traits<string, id_text>::
			set_image(buffer& b, size_t& n, bool& is_null, const string& v)
		{
			is_null = false;
			n = v.size();

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v.c_str(), n);
		}

		//
		// c_string_value_traits
		//
		void c_string_value_traits::
			set_image(buffer& b, size_t& n, bool& is_null, const char* v)
		{
			is_null = false;
			n = strlen(v);

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v, n);
		}

		//
		// c_array_value_traits_base
		//
		void c_array_value_traits_base::
			set_value(char* const& v,
			const details::buffer& b,
			size_t n,
			bool is_null,
			size_t N)
		{
			if (!is_null)
			{
				n = n < N ? n : N;

				if (n != 0)
					memcpy(v, b.data(), n);
			}
			else
				n = 0;

			if (n != N) // Append '\0' if there is space.
				v[n] = '\0';
		}

		void c_array_value_traits_base::
			set_image(details::buffer& b,
			size_t& n,
			bool& is_null,
			const char* v,
			size_t N)
		{
			is_null = false;

			// Figure out the length. We cannot use strlen since it may
			// not be 0-terminated (strnlen is not standard).
			//
			for (n = 0; n != N && v[n] != '\0'; ++n);

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v, n);
		}

		//
		// default_value_traits<std::wstring>
		//
#ifdef _WIN32
		void default_value_traits<wstring, id_text>::
			set_image(buffer& b, size_t& n, bool& is_null, const wstring& v)
		{
			is_null = false;
			n = v.size() * 2;

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v.c_str(), n);
		}

		//
		// c_wstring_value_traits
		//
		void c_wstring_value_traits::
			set_image(buffer& b, size_t& n, bool& is_null, const wchar_t* v)
		{
			is_null = false;
			n = wcslen(v) * 2;

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v, n);
		}

		//
		// c_warray_value_traits_base
		//
		void c_warray_value_traits_base::
			set_value(wchar_t* const& v,
			const details::buffer& b,
			size_t n,
			bool is_null,
			size_t N)
		{
			if (!is_null)
			{
				n /= 2;
				n = n < N ? n : N;

				if (n != 0)
					memcpy(v, b.data(), n * sizeof(wchar_t));
			}
			else
				n = 0;

			if (n != N) // Append '\0' if there is space.
				v[n] = L'\0';
		}

		void c_warray_value_traits_base::
			set_image(details::buffer& b,
			size_t& n,
			bool& is_null,
			const wchar_t* v,
			size_t N)
		{
			is_null = false;

			// Figure out the length. We cannot use wcslen since it may
			// not be 0-terminated (wcsnlen is not standard).
			//
			for (n = 0; n != N && v[n] != L'\0'; ++n);

			n *= 2;

			if (n > b.capacity())
				b.capacity(n);

			if (n != 0)
				memcpy(b.data(), v, n);
		}
#endif // _WIN32

		//
		// default_value_traits<vector<char>, id_blob>
		//
		// std::vector has to be qualified for Sun CC.
		//
		void default_value_traits<std::vector<char>, id_blob>::
			set_image(details::buffer& b,
			size_t& n,
			bool& is_null,
			const value_type& v)
		{
			is_null = false;
			n = v.size();

			if (n > b.capacity())
				b.capacity(n);

			// std::vector::data() may not be available in older compilers.
			//
			if (n != 0)
				memcpy(b.data(), &v.front(), n);
		}

		//
		// default_value_traits<vector<unsigned char>, id_blob>
		//
		// std::vector has to be qualified for Sun CC.
		//
		void default_value_traits<std::vector<unsigned char>, id_blob>::
			set_image(details::buffer& b,
			size_t& n,
			bool& is_null,
			const value_type& v)
		{
			is_null = false;
			n = v.size();

			if (n > b.capacity())
				b.capacity(n);

			// std::vector::data() may not be available in older compilers.
			//
			if (n != 0)
				memcpy(b.data(), &v.front(), n);
		}
	}
}
// file      : odb/sqlite/transaction.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cassert>

#include <odb/sqlite/transaction.hxx>

namespace odb
{
	namespace sqlite
	{
		transaction& transaction::
			current()
		{
			// While the impl type can be of the concrete type, the transaction
			// object can be created as either odb:: or odb::sqlite:: type. To
			// work around that we are going to hard-cast one two the other
			// relying on the fact that they have the same representation and
			// no virtual functions. The former is checked in the tests.
			//
			odb::transaction& b(odb::transaction::current());
			assert(dynamic_cast<transaction_impl*> (&b.implementation()) != 0);
			return reinterpret_cast<transaction&> (b);
		}
	}
}
// file      : odb/sqlite/transaction-impl.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <sqlite3.h>

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/statement-cache.hxx>
#include <odb/sqlite/transaction-impl.hxx>

namespace odb
{
	namespace sqlite
	{
		transaction_impl::
			transaction_impl(database_type& db, lock l)
			: odb::transaction_impl(db), lock_(l)
		{
		}

		transaction_impl::
			transaction_impl(connection_ptr c, lock l)
			: odb::transaction_impl(c->database(), *c),
			connection_(c),
			lock_(l)
		{
		}

		transaction_impl::
			~transaction_impl()
		{
		}

		void transaction_impl::
			start()
		{
			// Grab a connection if we don't already have one.
			//
			if (connection_ == 0)
			{
				connection_ = static_cast<database_type&> (database_).connection();
				odb::transaction_impl::connection_ = connection_.get();
			}

			statement_cache& sc(connection_->statement_cache());

			switch (lock_)
			{
			case deferred:
			{
				sc.begin_statement().execute();
				break;
			}
			case immediate:
			{
				sc.begin_immediate_statement().execute();
				break;
			}
			case exclusive:
			{
				sc.begin_exclusive_statement().execute();
				break;
			}
			}
		}

		// In SQLite, when a commit fails (e.g., because of the deferred
		// foreign key constraint violation), the transaction may not
		// be automatically rolled back. So we have to do it ourselves.
		//
		struct commit_guard
		{
			commit_guard(connection& c) : c_(&c) {}
			void release() { c_ = 0; }

			~commit_guard()
			{
				if (c_ != 0 && sqlite3_get_autocommit(c_->handle()) == 0)
				{
					// This is happening while another exception is active.
					//
					try
					{
						c_->statement_cache().rollback_statement().execute();
					}
					catch (...) {}
				}
			}

		private:
			connection* c_;
		};

		void transaction_impl::
			commit()
		{
			// Invalidate query results.
			//
			connection_->invalidate_results();

			// Reset active statements. Active statements will prevent COMMIT
			// from completing (write statements) or releasing the locks (read
			// statements). Normally, a statement is automatically reset on
			// completion, however, if an exception is thrown, that may not
			// happen.
			//
			connection_->clear();

			{
				commit_guard cg(*connection_);
				connection_->statement_cache().commit_statement().execute();
				cg.release();
			}

			// Release the connection.
			//
			connection_.reset();
		}

		void transaction_impl::
			rollback()
		{
			// Invalidate query results.
			//
			connection_->invalidate_results();

			// Reset active statements. Active statements will prevent ROLLBACK
			// from completing (write statements) or releasing the locks (read
			// statements). Normally, a statement is automatically reset on
			// completion, however, if an exception is thrown, that may not
			// happen.
			//
			connection_->clear();

			connection_->statement_cache().rollback_statement().execute();

			// Release the connection.
			//
			connection_.reset();
		}
	}
}
