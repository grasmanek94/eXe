// file      : odb/details/shared-ptr/base.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/shared-ptr/base.hxx>

using std::size_t;

namespace odb
{
  namespace details
  {
    share shared = share (1);
    share exclusive = share (2);

    const char* not_shared::
    what () const throw ()
    {
      return "object is not shared";
    }

    bool shared_base::
    _dec_ref_callback ()
    {
      bool r (--counter_ == 0);

      if (r)
        r = callback_->zero_counter (callback_->arg);

      return r;
    }
  }
}

#pragma warning(disable: 4290)

void*
operator new (size_t n, odb::details::share s) throw (std::bad_alloc)
{
  if (s == odb::details::shared)
  {
    // Here we need to make sure we don't break the alignment of the
    // returned block. For that we need to know the maximum alignment
    // of this platform. Twice the pointer size is a good guess for
    // most platforms.
    //
    size_t* p = static_cast<size_t*> (operator new (n + 2 * sizeof (size_t)));
    *p++ = 1;          // Initial count.
    *p++ = 0xDEADBEEF; // Signature.
    return p;
  }
  else
    return operator new (n);

}

void
operator delete (void* p, odb::details::share s) throw ()
{
  // This version of operator delete is only called when the c-tor
  // fails. In this case there is no object and we can just free the
  // memory.
  //
  if (s == odb::details::shared)
  {
    size_t* sp = static_cast<size_t*> (p);
    sp -= 2;
    operator delete (sp);
  }
  else
    operator delete (p);
}

// file      : odb/details/buffer.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cstring> // std::memcpy

#include <odb/details/buffer.hxx>

using namespace std;

namespace odb
{
	namespace details
	{
		void basic_buffer_base::
			capacity(size_t c, size_t data_size)
		{
			if (c > capacity_)
			{
				size_t n(capacity_ * 2 > c ? capacity_ * 2 : c);
				void* d(operator new (n));

				if (data_ != 0)
				{
					if (data_size != 0)
						memcpy(d, data_, data_size);

					operator delete (data_);
				}

				data_ = d;
				capacity_ = n;
			}
		}
	}
}


// file      : odb/callback.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/callback.hxx>

namespace odb
{
	// This otherwise unnecessary file is here to allow instantiation
	// of inline functions for exporting.
}

// file      : odb/details/condition.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/condition.hxx>

namespace odb
{
	namespace details
	{
		// This otherwise unnecessary file is here to allow instantiation
		// of inline functions for exporting.
	}
}

// file      : odb/details/win32/condition.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/windows.hxx>
#include <odb/details/win32/condition.hxx>
#include <odb/details/win32/exceptions.hxx>

namespace odb
{
	namespace details
	{
		void condition::
			signal()
		{
			mutex_.lock();

			if (waiters_ > signals_)
			{
				if (signals_++ == 0)
				{
					if (SetEvent(event_) == 0)
						throw win32_exception();
				}
			}

			mutex_.unlock();
		}

		void condition::
			wait()
		{
			// When we enter this functions the mutex is locked. When we
			// return from this function the mutex must be locked.
			//
			waiters_++;
			mutex_.unlock();

			if (WaitForSingleObject(event_, INFINITE) != 0)
				throw win32_exception();

			mutex_.lock();
			waiters_--;
			signals_--;

			if (signals_ > 0)
			{
				// Wake up the next thread.
				//
				if (SetEvent(event_) == 0)
					throw win32_exception();
			}
		}
	}
}

// file      : odb/connection.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/database.hxx>
#include <odb/connection.hxx>
#include <odb/result.hxx>
#include <odb/prepared-query.hxx>
#include <odb/exceptions.hxx> // prepared_*

using namespace std;

namespace odb
{
	connection::
		~connection()
	{
		assert(prepared_queries_ == 0);
		assert(prepared_map_.empty());
	}

	void connection::
		clear_prepared_map()
	{
		for (prepared_map_type::iterator i(prepared_map_.begin()),
			e(prepared_map_.end()); i != e; ++i)
		{
			if (i->second.params != 0)
				i->second.params_deleter(i->second.params);
		}

		prepared_map_.clear();
	}

	void connection::
		recycle()
	{
		while (prepared_queries_ != 0)
		{
			prepared_queries_->stmt.reset();
			prepared_queries_->list_remove();
		}
	}

	void connection::
		invalidate_results()
	{
		while (results_ != 0)
		{
			results_->invalidate();
			results_->list_remove();
		}
	}

	void connection::
		cache_query_(prepared_query_impl* pq,
		const type_info& ti,
		void* params,
		const type_info* params_info,
		void(*params_deleter) (void*))
	{
		pair<prepared_map_type::iterator, bool> r(
			prepared_map_.insert(
			prepared_map_type::value_type(pq->name, prepared_entry_type())));

		if (!r.second)
			throw prepared_already_cached(pq->name);

		prepared_entry_type& e(r.first->second);

		// Mark this prepared query as cached , get its ref count to 1
		// (prepared_query instances now reference this impl object),
		// and remove it from the invalidation list.
		//
		pq->cached = true;

		while (pq->_ref_count() > 1)
			pq->_dec_ref();

		pq->list_remove();

		e.prep_query.reset(pq);
		e.type_info = &ti;
		e.params = params;
		e.params_info = params_info;
		e.params_deleter = params_deleter;
	}

	prepared_query_impl* connection::
		lookup_query_(const char* name,
		const type_info& ti,
		void** params,
		const type_info* params_info) const
	{
		prepared_map_type::const_iterator i(prepared_map_.find(name));

		if (i == prepared_map_.end())
		{
			// See if there is a factory.
			//
			database_type::query_factory_type f(
				database_.lookup_query_factory(name));

			if (f)
			{
				f(name, const_cast<connection&> (*this));
				i = prepared_map_.find(name);
			}
		}

		if (i == prepared_map_.end())
			return 0;

		// Make sure the types match.
		//
		if (*i->second.type_info != ti)
			throw prepared_type_mismatch(name);

		if (params != 0)
		{
			if (*i->second.params_info != *params_info)
				throw prepared_type_mismatch(name);

			*params = i->second.params;
		}

		return i->second.prep_query.get();
	}
}

// file      : odb/database.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/database.hxx>

#include <odb/details/lock.hxx>

using namespace std;

namespace odb
{
	using details::lock;

	database::
		~database()
	{
	}

	unsigned long long database::
		execute(const char* st, std::size_t n)
	{
		connection_type& c(transaction::current().connection());
		return c.execute(st, n);
	}

	const database::schema_version_migration_type& database::
		schema_version_migration(const string& name) const
	{
		lock l(mutex_); // Prevents concurrent loading.

		schema_version_map::const_iterator i(schema_version_map_.find(name));
		return i != schema_version_map_.end() && i->second.version != 0
			? i->second
			: load_schema_version(name);
	}

	void database::
		schema_version_migration(const schema_version_migration_type& svm,
		const string& name)
	{
		// Note: no lock, not thread-safe.

		schema_version_info& svi(schema_version_map_[name]);
		if (svi.version != svm.version || svi.migration != svm.migration)
		{
			svi.version = svm.version;
			svi.migration = svm.migration;
			schema_version_seq_++;
		}
	}
}

// file      : odb/details/win32/dll.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

// If we are building a static library from VC++ (LIBODB_STATIC_LIB) or
// a static library from automake (!DLL_EXPORT), then omit DllMain.
//

#if (defined(_MSC_VER) && defined(LIBODB_DYNAMIC_LIB)) || \
    (!defined(_MSC_VER) && defined(DLL_EXPORT))

#include <odb/details/win32/windows.hxx>
#include <odb/details/win32/init.hxx>

using namespace odb::details;

extern "C" BOOL WINAPI
DllMain(HINSTANCE, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		process_start();
		thread_start();
		break;
	}

	case DLL_THREAD_ATTACH:
	{
		thread_start();
		break;
	}

	case DLL_THREAD_DETACH:
	{
		thread_end();
		break;
	}

	case DLL_PROCESS_DETACH:
	{
		thread_end();
		process_end(reserved == NULL);
		break;
	}
	}

	return 1;
}

#endif

// file      : odb/details/win32/exceptions.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/exceptions.hxx>

namespace odb
{
	namespace details
	{
		const char* win32_exception::
			what() const throw ()
		{
			return "Win32 API error";
		}
	}
}

// file      : odb/exceptions.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <sstream>
#include <odb/exceptions.hxx>

using namespace std;

namespace odb
{
	const char* null_pointer::
		what() const throw ()
	{
		return "NULL pointer";
	}

	const char* already_in_transaction::
		what() const throw ()
	{
		return "transaction already in progress in this thread";
	}

	const char* not_in_transaction::
		what() const throw ()
	{
		return "operation can only be performed in transaction";
	}

	const char* transaction_already_finalized::
		what() const throw ()
	{
		return "transaction already committed or rolled back";
	}

	const char* already_in_session::
		what() const throw ()
	{
		return "session already in effect in this thread";
	}

	const char* not_in_session::
		what() const throw ()
	{
		return "session not in effect in this thread";
	}

	const char* session_required::
		what() const throw ()
	{
		return "session required to load this object relationship";
	}

	const char* deadlock::
		what() const throw ()
	{
		return "transaction aborted due to deadlock";
	}

	const char* connection_lost::
		what() const throw ()
	{
		return "connection to database lost";
	}

	const char* timeout::
		what() const throw ()
	{
		return "database operation timeout";
	}

	const char* object_not_persistent::
		what() const throw ()
	{
		return "object not persistent";
	}

	const char* object_already_persistent::
		what() const throw ()
	{
		return "object already persistent";
	}

	const char* object_changed::
		what() const throw ()
	{
		return "object changed concurrently";
	}

	const char* result_not_cached::
		what() const throw ()
	{
		return "query result is not cached";
	}

	const char* abstract_class::
		what() const throw ()
	{
		return "database operation on instance of abstract class";
	}

	const char* no_type_info::
		what() const throw ()
	{
		return "no type information";
	}

	prepared_already_cached::
		prepared_already_cached(const char* name)
		: name_(name)
	{
		what_ = "prepared query '";
		what_ += name;
		what_ += "' is already cached";
	}

	prepared_already_cached::
		~prepared_already_cached() throw ()
	{
	}

	const char* prepared_already_cached::
		what() const throw ()
	{
		return what_.c_str();
	}

	prepared_type_mismatch::
		prepared_type_mismatch(const char* name)
		: name_(name)
	{
		what_ = "type mismatch while looking up prepared query '";
		what_ += name;
		what_ += "'";
	}

	prepared_type_mismatch::
		~prepared_type_mismatch() throw ()
	{
	}

	const char* prepared_type_mismatch::
		what() const throw ()
	{
		return what_.c_str();
	}

	unknown_schema::
		unknown_schema(const std::string& name)
		: name_(name)
	{
		what_ = "unknown database schema '";
		what_ += name;
		what_ += "'";
	}

	unknown_schema::
		~unknown_schema() throw ()
	{
	}

	const char* unknown_schema::
		what() const throw ()
	{
		return what_.c_str();
	}

	unknown_schema_version::
		unknown_schema_version(schema_version v)
		: version_(v)
	{
		ostringstream os;
		os << v;
		what_ = "unknown database schema version ";
		what_ += os.str();
	}

	unknown_schema_version::
		~unknown_schema_version() throw ()
	{
	}

	const char* unknown_schema_version::
		what() const throw ()
	{
		return what_.c_str();
	}

	const char* section_not_loaded::
		what() const throw ()
	{
		return "section is not loaded";
	}

	const char* section_not_in_object::
		what() const throw ()
	{
		return "section instance is not part of an object (section was copied?)";
	}
}

// file      : odb/details/win32/init.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/init.hxx>
#include <odb/details/win32/once-init.hxx>
#include <odb/details/win32/tls-init.hxx>

namespace odb
{
	namespace details
	{
		void
			process_start()
		{
			// The order is important.
			//
			once_process_start();
			tls_process_start();
		}

		void
			process_end(bool safe)
		{
			// The order is important.
			//
			tls_process_end(safe);
			once_process_end(safe);
		}

		void
			thread_start()
		{
		}

		void
			thread_end()
		{
			tls_thread_end();
		}
	}
}

// file      : odb/lazy-ptr-impl.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/lazy-ptr-impl.hxx>

namespace odb
{
	// This otherwise unnecessary file is here to allow instantiation
	// of inline functions for exporting.
}

// file      : odb/details/lock.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/lock.hxx>

namespace odb
{
	namespace details
	{
		// This otherwise unnecessary file is here to allow instantiation
		// of inline functions for exporting.
	}
}

// file      : odb/details/mutex.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/mutex.hxx>

namespace odb
{
	namespace details
	{
		// This otherwise unnecessary file is here to allow instantiation
		// of inline functions for exporting.
	}
}
// file      : odb/details/win32/once.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/windows.hxx>
#include <odb/details/win32/once.hxx>
#include <odb/details/win32/once-init.hxx>

namespace odb
{
	namespace details
	{
		CRITICAL_SECTION win32_once_cs_;

		void
			once_process_start()
		{
			InitializeCriticalSection(&win32_once_cs_);
		}

		void
			once_process_end(bool)
		{
			DeleteCriticalSection(&win32_once_cs_);
		}
	}
}
// file      : odb/prepared-query.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/connection.hxx>
#include <odb/prepared-query.hxx>

namespace odb
{
	prepared_query_impl::
		~prepared_query_impl()
	{
		if (next_ != this)
			list_remove();
	}

	prepared_query_impl::
		prepared_query_impl(connection& c)
		: cached(false), conn(c), prev_(0), next_(this)
	{
		// Add to the list.
		//
		next_ = conn.prepared_queries_;
		conn.prepared_queries_ = this;

		if (next_ != 0)
			next_->prev_ = this;
	}

	void prepared_query_impl::
		list_remove()
	{
		(prev_ == 0 ? conn.prepared_queries_ : prev_->next_) = next_;

		if (next_ != 0)
			next_->prev_ = prev_;

		prev_ = 0;
		next_ = this;
	}
}
// file      : odb/query-dynamic.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/query-dynamic.hxx>

using namespace std;

namespace odb
{
	// query_param
	//
	query_param::
		~query_param()
	{
	}

	// query_base
	//
	void query_base::
		clear()
	{
		for (clause_type::iterator i(clause_.begin()); i != clause_.end(); ++i)
		{
			if (i->kind == clause_part::kind_param_val ||
				i->kind == clause_part::kind_param_ref)
			{
				query_param* qp(reinterpret_cast<query_param*> (i->data));

				if (qp != 0 && qp->_dec_ref())
					delete qp;
			}
		}

		clause_.clear();
		strings_.clear();
	}

	void query_base::
		append(const string& native)
	{
		strings_.push_back(native);
		clause_.push_back(clause_part());
		clause_.back().kind = clause_part::kind_native;
		clause_.back().data = strings_.size() - 1;
	}

	void query_base::
		append(const query_base& x)
	{
		size_t i(clause_.size()), delta(i);
		size_t n(i + x.clause_.size());
		clause_.resize(n);

		for (size_t j(0); i < n; ++i, ++j)
		{
			const clause_part& s(x.clause_[j]);
			clause_part& d(clause_[i]);

			d = s;

			// We need to increment the param references, update pointers
			// to strings and update argument positions.
			//
			switch (s.kind)
			{
			case clause_part::kind_param_val:
			case clause_part::kind_param_ref:
			{
				reinterpret_cast<query_param*> (d.data)->_inc_ref();
				break;
			}
			case clause_part::kind_native:
			{
				strings_.push_back(x.strings_[s.data]);
				d.data = strings_.size() - 1;
				break;
			}
			case clause_part::op_add:

			case clause_part::op_and:
			case clause_part::op_or:

			case clause_part::op_eq:
			case clause_part::op_ne:
			case clause_part::op_lt:
			case clause_part::op_gt:
			case clause_part::op_le:
			case clause_part::op_ge:
			{
				d.data += delta;
				break;
			}
			// Do not use default here to remember to handle new op codes.
			//
			case clause_part::kind_column:
			case clause_part::kind_true:
			case clause_part::kind_false:
			case clause_part::op_not:
			case clause_part::op_null:
			case clause_part::op_not_null:
			case clause_part::op_in:
			case clause_part::op_like:
			case clause_part::op_like_escape:
				break;
			}
		}
	}

	void query_base::
		append_ref(const void* ref, const native_column_info* c)
	{
		clause_.push_back(clause_part());
		clause_part& p(clause_.back());

		p.kind = clause_part::kind_param_ref;
		p.data = 0; // In case new below throws.
		p.native_info = c;

		p.data = reinterpret_cast<std::size_t> (
			new (details::shared) query_param(ref));
	}

	query_base& query_base::
		operator+= (const std::string& native)
	{
		if (!native.empty())
		{
			size_t p(clause_.size());
			append(native);

			if (p != 0)
				append(clause_part::op_add, p - 1);
		}

		return *this;
	}

	query_base& query_base::
		operator+= (const query_base& x)
	{
		if (!x.empty())
		{
			size_t p(clause_.size());
			append(x);

			if (p != 0)
				append(clause_part::op_add, p - 1);
		}

		return *this;
	}

	query_base
		operator&& (const query_base& x, const query_base& y)
	{
		// Optimize cases where one or both sides are constant truth.
		//
		bool xt(x.const_true()), yt(y.const_true());

		if (xt && yt)
			return x;

		if (xt || x.empty())
			return y;

		if (yt || y.empty())
			return x;

		query_base r(x);
		r.append(y);
		r.append(query_base::clause_part::op_and, x.clause().size() - 1);
		return r;
	}

	query_base
		operator|| (const query_base& x, const query_base& y)
	{
		if (x.empty())
			return y;

		if (y.empty())
			return x;

		query_base r(x);
		r.append(y);
		r.append(query_base::clause_part::op_or, x.clause().size() - 1);
		return r;
	}

	query_base
		operator! (const query_base& x)
	{
		if (x.empty())
			return x;

		query_base r(x);
		r.append(query_base::clause_part::op_not, 0);
		return r;
	}
}
// file      : odb/result.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/result.hxx>
#include <odb/connection.hxx>

namespace odb
{
	result_impl::
		~result_impl()
	{
		if (next_ != this)
			list_remove();
	}

	result_impl::
		result_impl(connection& c)
		: db_(c.database()), conn_(c), prev_(0), next_(this)
	{
		// Add to the list.
		//
		next_ = conn_.results_;
		conn_.results_ = this;

		if (next_ != 0)
			next_->prev_ = this;
	}

	void result_impl::
		list_remove()
	{
		(prev_ == 0 ? conn_.results_ : prev_->next_) = next_;

		if (next_ != 0)
			next_->prev_ = prev_;

		prev_ = 0;
		next_ = this;
	}
}
// file      : odb/schema-catalog.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <map>
#include <vector>
#include <cassert>

#include <odb/database.hxx>
#include <odb/exceptions.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/schema-catalog-impl.hxx>

using namespace std;

namespace odb
{
	// Schema.
	//
	typedef bool(*create_function) (database&, unsigned short pass, bool drop);
	typedef bool(*migrate_function) (database&, unsigned short pass, bool pre);

	typedef pair<database_id, string> key;
	typedef vector<create_function> create_functions;
	typedef vector<migrate_function> migrate_functions;
	typedef map<schema_version, migrate_functions> version_map;

	struct schema_functions
	{
		create_functions create;
		version_map migrate;
	};
	typedef map<key, schema_functions> schema_map;

	// Data. Normally the code would be database-independent, though there
	// could be database-specific migration steps.
	//
	typedef pair<string, schema_version> data_key;

	struct data_function
	{
		typedef schema_catalog::data_migration_function_type function_type;

		data_function() {}
		data_function(database_id i, function_type m) : id(i), migrate(m) {}

		database_id id;
		function_type migrate;
	};
	typedef vector<data_function> data_functions;
	typedef map<data_key, data_functions> data_map;

	struct schema_catalog_impl
	{
		schema_map schema;
		data_map data;
	};

	// Static initialization.
	//
	schema_catalog_impl* schema_catalog_init::catalog = 0;
	size_t schema_catalog_init::count = 0;

	struct schema_catalog_init_extra
	{
		bool initialized;

		schema_catalog_init_extra() : initialized(false) {}
		~schema_catalog_init_extra()
		{
			if (initialized && --schema_catalog_init::count == 0)
				delete schema_catalog_init::catalog;
		}
	};

	static schema_catalog_init_extra schema_catalog_init_extra_;

	bool schema_catalog::
		exists(database_id id, const string& name)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		return c.schema.find(key(id, name)) != c.schema.end();
	}

	void schema_catalog::
		create_schema(database& db, const string& name, bool drop)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		schema_map::const_iterator i(c.schema.find(key(db.id(), name)));

		if (i == c.schema.end())
			throw unknown_schema(name);

		const create_functions& fs(i->second.create);

		if (drop)
			drop_schema(db, name);

		// Run the passes until we ran them all or all the functions
		// return false, which means no more passes necessary.
		//
		for (unsigned short pass(1); pass < 3; ++pass)
		{
			bool done(true);

			for (create_functions::const_iterator j(fs.begin()), e(fs.end());
				j != e; ++j)
			{
				if ((*j) (db, pass, false))
					done = false;
			}

			if (done)
				break;
		}
	}

	void schema_catalog::
		drop_schema(database& db, const string& name)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		schema_map::const_iterator i(c.schema.find(key(db.id(), name)));

		if (i == c.schema.end())
			throw unknown_schema(name);

		const create_functions& fs(i->second.create);

		// Run the passes until we ran them all or all the functions
		// return false, which means no more passes necessary.
		//
		for (unsigned short pass(1); pass < 3; ++pass)
		{
			bool done(true);

			for (create_functions::const_iterator j(fs.begin()), e(fs.end());
				j != e; ++j)
			{
				if ((*j) (db, pass, true))
					done = false;
			}

			if (done)
				break;
		}
	}

	void schema_catalog::
		migrate_schema_impl(database& db,
		schema_version v,
		const string& name,
		migrate_mode m)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		schema_map::const_iterator i(c.schema.find(key(db.id(), name)));

		if (i == c.schema.end())
			throw unknown_schema(name);

		const version_map& vm(i->second.migrate);
		version_map::const_iterator j(vm.find(v));

		if (j == vm.end())
			throw unknown_schema_version(v);

		const migrate_functions& fs(j->second);

		// Run the passes until we ran them all or all the functions
		// return false, which means no more passes necessary.
		//
		for (bool pre(m != migrate_post);; pre = false)
		{
			for (unsigned short pass(1); pass < 3; ++pass)
			{
				bool done(true);

				for (migrate_functions::const_iterator i(fs.begin()), e(fs.end());
					i != e; ++i)
				{
					if ((*i) (db, pass, pre))
						done = false;
				}

				if (done)
					break;
			}

			if (!pre || m != migrate_both)
				break;
		}

		// Update the schema version on the database instance.
		//
		db.schema_version_migration(v, m == migrate_pre, name);
	}

	size_t schema_catalog::
		migrate_data(database& db, schema_version v, const string& name)
	{
		if (v == 0)
		{
			if (!db.schema_migration())
				return 0;

			v = db.schema_version();
		}

		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		data_map::const_iterator i(c.data.find(data_key(name, v)));

		if (i == c.data.end())
			return 0; // No data migration for this schema/version.

		size_t r(0);

		const data_functions& df(i->second);
		for (data_functions::const_iterator i(df.begin()), e(df.end());
			i != e; ++i)
		{
			if (i->id == id_common || i->id == db.id())
			{
				i->migrate(db);
				r++;
			}
		}

		return r;
	}

	void schema_catalog::
		data_migration_function(database_id id,
		schema_version v,
		data_migration_function_type f,
		const string& name)
	{
		// This function can be called from a static initializer in which
		// case the catalog might not have yet been created.
		//
		if (schema_catalog_init::count == 0)
		{
			schema_catalog_init::catalog = new schema_catalog_impl;
			++schema_catalog_init::count;
			schema_catalog_init_extra_.initialized = true;
		}

		schema_catalog_impl& c(*schema_catalog_init::catalog);
		c.data[data_key(name, v)].push_back(data_function(id, f));
	}

	void schema_catalog::
		migrate(database& db, schema_version v, const string& name)
	{
		schema_version cur(current_version(db, name));

		if (v == 0)
			v = cur;
		else if (v > cur)
			throw unknown_schema_version(v);

		schema_version i(db.schema_version(name));

		// If there is no schema, then "migrate" by creating it.
		//
		if (i == 0)
		{
			// Schema creation can only "migrate" straight to current.
			//
			if (v != cur)
				throw unknown_schema_version(v);

			create_schema(db, name, false);
			return;
		}

		for (i = next_version(db, i, name);
			i <= v;
			i = next_version(db, i, name))
		{
			migrate_schema_pre(db, i, name);
			migrate_data(db, i, name);
			migrate_schema_post(db, i, name);
		}
	}

	schema_version schema_catalog::
		base_version(database_id id, const string& name)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		schema_map::const_iterator i(c.schema.find(key(id, name)));

		if (i == c.schema.end())
			throw unknown_schema(name);

		const version_map& vm(i->second.migrate);
		assert(!vm.empty());
		return vm.begin()->first;
	}

	schema_version schema_catalog::
		current_version(database_id id, const string& name)
	{
		const schema_catalog_impl& c(*schema_catalog_init::catalog);
		schema_map::const_iterator i(c.schema.find(key(id, name)));

		if (i == c.schema.end())
			throw unknown_schema(name);

		const version_map& vm(i->second.migrate);
		assert(!vm.empty());
		return vm.rbegin()->first;
	}

	schema_version schema_catalog::
		next_version(database_id id, schema_version v, const string& name)
	{
		const schema_catalog_impl& sc(*schema_catalog_init::catalog);
		schema_map::const_iterator i(sc.schema.find(key(id, name)));

		if (i == sc.schema.end())
			throw unknown_schema(name);

		const version_map& vm(i->second.migrate); // Cannot be empty.

		schema_version b(vm.begin()->first);
		schema_version c(vm.rbegin()->first);

		if (v == 0)
			return c; // "Migration" to the current via schema creation.
		else if (v < b)
			throw unknown_schema_version(v); // Unsupported migration.

		version_map::const_iterator j(vm.upper_bound(v));
		return j != vm.end() ? j->first : c + 1;
	}

	// schema_catalog_init
	//
	schema_catalog_init::
		schema_catalog_init()
	{
		if (count == 0)
			catalog = new schema_catalog_impl;

		++count;
	}

	schema_catalog_init::
		~schema_catalog_init()
	{
		if (--count == 0)
			delete catalog;
	}

	// schema_catalog_create_entry
	//
	schema_catalog_create_entry::
		schema_catalog_create_entry(database_id id,
		const char* name,
		create_function cf)
	{
		schema_catalog_impl& c(*schema_catalog_init::catalog);
		c.schema[key(id, name)].create.push_back(cf);
	}

	// schema_catalog_migrate_entry
	//
	schema_catalog_migrate_entry::
		schema_catalog_migrate_entry(database_id id,
		const char* name,
		schema_version v,
		migrate_function mf)
	{
		schema_catalog_impl& c(*schema_catalog_init::catalog);
		c.schema[key(id, name)].migrate[v].push_back(mf);
	}
}
// file      : odb/session.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/exceptions.hxx>
#include <odb/session.hxx>

#include <odb/details/tls.hxx>

namespace odb
{
	using namespace details;

	static ODB_TLS_POINTER(session) current_session;

	session::
		session(bool make_current)
	{
		if (make_current)
		{
			if (has_current())
				throw already_in_session();

			current_pointer(this);
		}
	}

	session::
		~session()
	{
		// If we are the current thread's session, reset it.
		//
		if (current_pointer() == this)
			reset_current();
	}

	session* session::
		current_pointer()
	{
		return tls_get(current_session);
	}

	void session::
		current_pointer(session* s)
	{
		tls_set(current_session, s);
	}

	session& session::
		current()
	{
		session* cur(tls_get(current_session));

		if (cur == 0)
			throw not_in_session();

		return *cur;
	}

	//
	// object_map_base
	//
	session::object_map_base::
		~object_map_base()
	{
	}
}
// file      : odb/section.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/section.hxx>

namespace odb
{
	void section::
		disarm()
	{
		transaction& t(transaction::current());
		t.callback_unregister(this);
		state_.armed = 0;
	}

	void section::
		transacion_callback(unsigned short event, void* key, unsigned long long)
	{
		section& s(*static_cast<section*> (key));

		if (event == transaction::event_rollback && s.state_.restore)
			s.state_.changed = 1;

		s.state_.armed = 0;
		s.state_.restore = 0;
	}
}
// file      : odb/statement.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/statement.hxx>

namespace odb
{
	statement::
		~statement()
	{
	}
}
// file      : odb/statement-processing.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <cassert>

#include <odb/statement-processing-common.hxx>

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
#  include <iostream>
#endif

#include <odb/statement.hxx>

using namespace std;

namespace odb
{
	typedef const void* const* bind_type;

	static inline const void*
		bind_at(size_t i, bind_type bind, size_t bind_skip)
	{
		const char* b(reinterpret_cast<const char*> (bind));
		return *reinterpret_cast<bind_type> (b + i * bind_skip);
	}

	void statement::
		process_insert(const char* s,
		bind_type bind,
		size_t bind_size,
		size_t bind_skip,
		char param_symbol,
		string& r)
	{
#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		assert(bind_size != 0); // Cannot be versioned.
#endif

		bool fast(true); // Fast case (if all present).
		for (size_t i(0); i != bind_size && fast; ++i)
		{
			if (bind_at(i, bind, bind_skip) == 0)
				fast = false;
		}

		// Fast path: just remove the "structure".
		//
#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		if (fast)
		{
			process_fast(s, r);
			return;
		}
#endif

		// Scan the statement and store the positions of various parts.
		//
		size_t n(traits::length(s));
		const char* e(s + n);

		// Header.
		//
		const char* p(find(s, e, '\n'));
		assert(p != 0);
		size_t header_size(p - s);
		p++;

		// Column list.
		//
		const char* columns_begin(0);
		if (*p == '(')
		{
			columns_begin = p;

			// Find the end of the column list.
			//
			for (const char* ce(paren_begin(p, e)); ce != 0; paren_next(p, ce, e))
				;
		}

		// OUTPUT
		//
		const char* output_begin(0);
		size_t output_size(0);
		if (e - p > 7 && traits::compare(p, "OUTPUT ", 7) == 0)
		{
			output_begin = p;
			p += 7;
			p = find(p, e, '\n');
			assert(p != 0);
			output_size = p - output_begin;
			p++;
		}

		// VALUES or DEFAULT VALUES
		//
		bool empty(true); // DEFAULT VALUES case (if none present).
		const char* values_begin(0);
		if (e - p > 7 && traits::compare(p, "VALUES\n", 7) == 0)
		{
			p += 7;
			values_begin = p;

			size_t bi(0);
			for (const char* ve(paren_begin(p, e)); ve != 0; paren_next(p, ve, e))
			{
				// We cannot be empty if we have a non-parameterized value, e.g.,
				// INSERT ... VALUES(1,?). We also cannot be empty if this value
				// is present in the bind array.
				//
				if (find(p, ve, param_symbol) == 0 ||
					bind_at(bi++, bind, bind_skip) != 0)
					empty = false;
			}
		}
		else
		{
			// Must be DEFAULT VALUES.
			//
			assert(traits::compare(p, "DEFAULT VALUES", 14) == 0);
			p += 14;

			if (*p == '\n')
				p++;
		}

		// Trailer.
		//
		const char* trailer_begin(0);
		size_t trailer_size(0);
		if (e - p != 0)
		{
			trailer_begin = p;
			trailer_size = e - p;
		}

		// Empty case.
		//
		if (empty)
		{
			r.reserve(header_size +
				(output_size == 0 ? 0 : output_size + 1) +
				15 + // For " DEFAULT VALUES"
				(trailer_size == 0 ? 0 : trailer_size + 1));

			r.assign(s, header_size);

			if (output_size != 0)
			{
				r += ' ';
				r.append(output_begin, output_size);
			}

			r += " DEFAULT VALUES";

			if (trailer_size != 0)
			{
				r += ' ';
				r.append(trailer_begin, trailer_size);
			}

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
			if (r.size() != n)
				cerr << endl
				<< "old: '" << s << "'" << endl << endl
				<< "new: '" << r << "'" << endl << endl;
#endif

			return;
		}

		// Assume the same size as the original. It can only shrink, and in
		// most cases only slightly. So this is a good approximation.
		//
		r.reserve(n);
		r.assign(s, header_size);

		// Column list.
		//
		{
			r += ' ';

			size_t i(0), bi(0);

			for (const char *c(columns_begin), *ce(paren_begin(c, e)),
				*v(values_begin), *ve(paren_begin(v, e));
				ce != 0; paren_next(c, ce, e), paren_next(v, ve, e))
			{
				// See if the value contains the parameter symbol and, if so,
				// whether it is present in the bind array.
				//
				if (find(v, ve, param_symbol) != 0 &&
					bind_at(bi++, bind, bind_skip) == 0)
					continue;

				// Append the column.
				//
				if (i++ == 0)
					r += '(';
				else
					r += ", "; // Add the space for consistency with the fast path.

				r.append(c, ce - c);
			}

			r += ')';
		}

		// OUTPUT
		//
		if (output_size != 0)
		{
			r += ' ';
			r.append(output_begin, output_size);
		}

		// Value list.
		//
	{
		r += " VALUES ";

		size_t i(0), bi(0);

		for (const char* v(values_begin), *ve(paren_begin(v, e));
			ve != 0; paren_next(v, ve, e))
		{
			// See if the value contains the parameter symbol and, if so,
			// whether it is present in the bind array.
			//
			if (find(v, ve, param_symbol) != 0 &&
				bind_at(bi++, bind, bind_skip) == 0)
				continue;

			// Append the value.
			//
			if (i++ == 0)
				r += '(';
			else
				r += ", "; // Add the space for consistency with the fast path.

			r.append(v, ve - v);
		}

		r += ')';
	}

	// Trailer.
	//
	if (trailer_size != 0)
	{
		r += ' ';
		r.append(trailer_begin, trailer_size);
	}

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
	if (r.size() != n)
		cerr << endl
		<< "old: '" << s << "'" << endl << endl
		<< "new: '" << r << "'" << endl << endl;
#endif
	}

	void statement::
		process_update(const char* s,
		bind_type bind,
		size_t bind_size,
		size_t bind_skip,
		char param_symbol,
		string& r)
	{
		bool fast(true); // Fast case (if all present).
		for (size_t i(0); i != bind_size && fast; ++i)
		{
			if (bind_at(i, bind, bind_skip) == 0)
				fast = false;
		}

		// Fast path: just remove the "structure".
		//
#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		if (fast)
		{
			process_fast(s, r);
			return;
		}
#endif

		// Scan the statement and store the positions of various parts.
		//
		size_t n(traits::length(s));
		const char* e(s + n);

		// Header.
		//
		const char* p(find(s, e, '\n'));
		assert(p != 0);
		size_t header_size(p - s);
		p++;

		// SET
		//
		bool empty(true); // Empty SET case.
		const char* set_begin(0);
		if (e - p > 4 && traits::compare(p, "SET\n", 4) == 0)
		{
			p += 4;
			set_begin = p;

			// Scan the SET list.
			//
			size_t bi(0);
			for (const char* pe(comma_begin(p, e)); pe != 0; comma_next(p, pe, e))
			{
				if (empty)
				{
					// We cannot be empty if we have a non-parameterized set expression,
					// e.g., UPDATE ... SET ver=ver+1 ... We also cannot be empty if
					// this expression is present in the bind array.
					//
					if (find(p, pe, param_symbol) == 0 ||
						bind_at(bi++, bind, bind_skip) != 0)
						empty = false;
				}
			}
		}

		// Empty case.
		//
		if (empty)
		{
			r.clear();

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
			if (n != 0)
				cerr << endl
				<< "old: '" << s << "'" << endl << endl
				<< "new: '" << r << "'" << endl << endl;
#endif

			return;
		}

		// Trailer.
		//
		const char* trailer_begin(0);
		size_t trailer_size(0);
		if (e - p != 0)
		{
			trailer_begin = p;
			trailer_size = e - p;
		}

		// Assume the same size as the original. It can only shrink, and in
		// most cases only slightly. So this is a good approximation.
		//
		r.reserve(n);
		r.assign(s, header_size);

		// SET list.
		//
		{
			r += " SET ";

			size_t i(0), bi(0);

			for (const char* p(set_begin), *pe(comma_begin(p, e));
				pe != 0; comma_next(p, pe, e))
			{
				// See if the value contains the parameter symbol and, if so,
				// whether it is present in the bind array.
				//
				if (find(p, pe, param_symbol) != 0 &&
					bind_at(bi++, bind, bind_skip) == 0)
					continue;

				// Append the expression.
				//
				if (i++ != 0)
					r += ", "; // Add the space for consistency with the fast path.

				r.append(p, pe - p);
			}
		}

		// Trailer.
		//
		if (trailer_size != 0)
		{
			r += ' ';
			r.append(trailer_begin, trailer_size);
		}

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
		if (r.size() != n)
			cerr << endl
			<< "old: '" << s << "'" << endl << endl
			<< "new: '" << r << "'" << endl << endl;
#endif
	}

	void statement::
		process_select(const char* s,
		bind_type bind,
		size_t bind_size,
		size_t bind_skip,
		char quote_open,
		char quote_close,
#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		bool optimize,
#else
		bool,
#endif
		string& r,
		bool as)
	{
		bool empty(true); // Empty case (if none present).
		bool fast(true);  // Fast case (if all present).
		for (size_t i(0); i != bind_size && (empty || fast); ++i)
		{
			if (bind_at(i, bind, bind_skip) != 0)
				empty = false;
			else
				fast = false;
		}

		// Empty.
		//
		if (empty)
		{
			r.clear();

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
			if (*s != '\0')
				cerr << endl
				<< "old: '" << s << "'" << endl << endl
				<< "new: '" << r << "'" << endl << endl;
#endif
			return;
		}

		// Fast path: just remove the "structure".
		//
#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		if (fast && !optimize)
		{
			process_fast(s, r);
			return;
		}
#endif

		// Scan the statement and store the positions of various parts.
		//
		size_t n(traits::length(s));
		const char* e(s + n);

		// Header.
		//
		const char* p(find(s, e, '\n'));
		assert(p != 0);
		size_t header_size(p - s);
		p++;

		// Column list.
		//
		const char* columns_begin(p);
		for (const char* ce(comma_begin(p, e)); ce != 0; comma_next(p, ce, e))
			;

		// FROM.
		assert(traits::compare(p, "FROM ", 5) == 0);
		const char* from_begin(p);
		p = find(p, e, '\n'); // May not end with '\n'.
		if (p == 0)
			p = e;
		size_t from_size(p - from_begin);
		if (p != e)
			p++;

		// JOIN list.
		//
		const char* joins_begin(0), *joins_end(0);
		if (e - p > 5 && traits::compare(p, "LEFT JOIN ", 10) == 0)
		{
			joins_begin = p;

			// Find the end of the JOIN list.
			//
			for (const char* je(newline_begin(p, e));
				je != 0; newline_next(p, je, e, "LEFT JOIN ", 10))
				;

			joins_end = (p != e ? p - 1 : p);
		}

#ifndef LIBODB_DEBUG_STATEMENT_PROCESSING
		if (fast && joins_begin == 0)
		{
			// No JOINs to optimize so can still take the fast path.
			//
			process_fast(s, r);
			return;
		}
#endif

		// Trailer (WHERE, ORDER BY, etc).
		//
		const char* trailer_begin(0);
		size_t trailer_size(0);
		if (e - p != 0)
		{
			trailer_begin = p;
			trailer_size = e - p;
		}

		// Assume the same size as the original. It can only shrink, and in
		// most cases only slightly. So this is a good approximation.
		//
		r.reserve(n);
		r.assign(s, header_size);

		// Column list.
		//
		{
			r += ' ';

			size_t i(0), bi(0);

			for (const char *c(columns_begin), *ce(comma_begin(c, e));
				ce != 0; comma_next(c, ce, e))
			{
				// See if the column is present in the bind array.
				//
				if (bind_at(bi++, bind, bind_skip) == 0)
					continue;

				// Append the column.
				//
				if (i++ != 0)
					r += ", "; // Add the space for consistency with the fast path.

				r.append(c, ce - c);
			}
		}

		// From.
		//
		r += ' ';
		r.append(from_begin, from_size);

		// JOIN list, pass 1.
		//
		size_t join_pos(0);
		if (joins_begin != 0)
		{
			// Fill in the JOIN "area" with spaces.
			//
			r.resize(r.size() + joins_end - joins_begin + 1, ' ');
			join_pos = r.size() + 1; // End of the last JOIN.
		}

		// Trailer.
		//
		if (trailer_size != 0)
		{
			r += ' ';
			r.append(trailer_begin, trailer_size);
		}

		// JOIN list, pass 2.
		//
		if (joins_begin != 0)
		{
			// Splice the JOINs into the pre-allocated area.
			//
			for (const char* je(joins_end), *j(newline_rbegin(je, joins_begin));
				j != 0; newline_rnext(j, je, joins_begin))
			{
				size_t n(je - j);

				// Get the alias or, if none used, the table name.
				//
				p = find(j + 10, je, ' '); // 10 for "LEFT JOIN ".
				const char* table_end(p);
				p++; // Skip space.

				// We may or may not have the AS keyword.
				//
				const char* alias_begin(0);
				size_t alias_size(0);
				if (je - p > 3 && traits::compare(p, "ON ", 3) != 0)
				{
					if (as)
						p += 3;

					alias_begin = p;
					alias_size = find(p, je, ' ') - alias_begin;
				}
				else
				{
					alias_begin = j + 10;
					alias_size = table_end - alias_begin;
				}

				// The alias must be quoted.
				//
				assert(*alias_begin == quote_open &&
					*(alias_begin + alias_size - 1) == quote_close);

				// We now need to see if the alias is used in either the SELECT
				// list, the WHERE conditions, or the ON condition of any of the
				// JOINs that we have already processed and decided to keep.
				//
				// Instead of re-parsing the whole thing again, we are going to
				// take a shortcut and simply search for the alias in the statement
				// we have constructed so far (that's why we have have added the
				// trailer before filling in the JOINs). To make it more robust,
				// we are going to do a few extra sanity checks, specifically,
				// that the alias is a top level identifier and is followed by
				// only a single identifer (column). This will catch cases like
				// [s].[t].[c] where [s] is also used as an alias or LEFT JOIN [t]
				// where [t] is also used as an alias in another JOIN.
				//
				bool found(false);
				for (size_t p(r.find(alias_begin, 0, alias_size));
					p != string::npos;
					p = r.find(alias_begin, p + alias_size, alias_size))
				{
					size_t e(p + alias_size);

					// If we are not a top-level qualifier or not a bottom-level,
					// then we are done (3 is for at least "[a]").
					//
					if ((p != 0 && r[p - 1] == '.') ||
						(e + 3 >= r.size() || (r[e] != '.' || r[e + 1] != quote_open)))
						continue;

					// The only way to distinguish the [a].[c] from FROM [a].[c] or
					// LEFT JOIN [a].[c] is by checking the prefix.
					//
					if ((p > 5 && r.compare(p - 5, 5, "FROM ") == 0) ||
						(p > 10 && r.compare(p - 10, 10, "LEFT JOIN ") == 0))
						continue;

					// Check that we are followed by a single identifier.
					//
					e = r.find(quote_close, e + 2);
					if (e == string::npos || (e + 1 != r.size() && r[e + 1] == '.'))
						continue;

					found = true;
					break;
				}

				join_pos -= n + 1; // Extra one for space.
				if (found)
					r.replace(join_pos, n, j, n);
				else
					r.erase(join_pos - 1, n + 1); // Extra one for space.
			}
		}

#ifdef LIBODB_TRACE_STATEMENT_PROCESSING
		if (r.size() != n)
			cerr << endl
			<< "old: '" << s << "'" << endl << endl
			<< "new: '" << r << "'" << endl << endl;
#endif
	}
}
// file      : odb/details/win32/thread.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/windows.hxx>
#include <process.h> // _beginthreadex, _endthreadex

#include <odb/details/unique-ptr.hxx>
#include <odb/details/win32/thread.hxx>
#include <odb/details/win32/exceptions.hxx>

unsigned int __stdcall
odb_thread_thunk(void* arg)
{
	odb::details::thread::thread_thunk(arg);
	_endthreadex(0);
	return 0;
}

namespace odb
{
	namespace details
	{
		void thread::
			thread_thunk(void* arg)
		{
			data* d(static_cast<data*> (arg));
			d->ret = d->func(d->arg);
			d->mutex.lock();
			unsigned char count = --d->count;
			d->mutex.unlock();

			if (count == 0)
				delete d;
		}

		thread::
			~thread()
		{
			if (handle_ != 0)
			{
				CloseHandle(handle_);

				// Win32 mutex implementation does not throw.
				//
				data_->mutex.lock();
				unsigned char count = --data_->count;
				data_->mutex.unlock();

				if (count == 0)
					delete data_;
			}
		}

		thread::
			thread(void* (*func) (void*), void* arg)
		{
			unique_ptr<data> d(new data);
			d->func = func;
			d->arg = arg;
			d->count = 2; // One for the thread and one for us.

			handle_ = (HANDLE)_beginthreadex(
				0, 0, &odb_thread_thunk, d.get(), 0, 0);

			if (handle_ == 0)
				throw win32_exception();

			data_ = d.release();
		}

		void* thread::
			join()
		{
			void* r;

			if (WaitForSingleObject(handle_, INFINITE) != 0)
				throw win32_exception();

			r = data_->ret;

			CloseHandle(handle_);
			delete data_;
			handle_ = 0;
			data_ = 0;
			return r;
		}
	}
}
// file      : odb/details/win32/tls.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/details/win32/windows.hxx>
#include <winerror.h> // ERROR_INVALID_INDEX

#include <new>
#include <cstddef> // std::size_t

#include <odb/details/win32/lock.hxx>
#include <odb/details/win32/tls.hxx>
#include <odb/details/win32/tls-init.hxx>
#include <odb/details/win32/exceptions.hxx>

#ifdef _MSC_VER
#  pragma warning (disable:4200) // zero-sized array in struct
#endif

using namespace std;

namespace odb
{
	namespace details
	{
		typedef void(*dtor_func) (void*);

		struct entry
		{
			void* value;
			dtor_func dtor;
		};

		struct thread_data
		{
			size_t size;
			size_t capacity;
			entry entries[0];
		};

		struct process_data
		{
			size_t size;
			size_t capacity;
			dtor_func dtors[0];
		};

		static DWORD index_ = TLS_OUT_OF_INDEXES;
		static CRITICAL_SECTION cs_;
		static process_data* proc_data_;

		const size_t init_capacity = 4;

		void
			tls_process_start()
		{
			index_ = TlsAlloc();

			if (index_ == TLS_OUT_OF_INDEXES)
				throw win32_exception();

			InitializeCriticalSection(&cs_);

			process_data* pd(
				static_cast<process_data*> (
				operator new (
				sizeof(process_data) + sizeof(dtor_func) * init_capacity)));

			pd->size = 0;
			pd->capacity = init_capacity;
			memset(pd->dtors, 0, sizeof(dtor_func) * init_capacity);

			proc_data_ = pd;
		}

		void
			tls_process_end(bool)
		{
			operator delete (proc_data_);
			DeleteCriticalSection(&cs_);

			if (index_ != TLS_OUT_OF_INDEXES)
			{
				if (!TlsFree(index_))
					throw win32_exception();
			}
		}

		void
			tls_thread_end()
		{
			if (thread_data* d = static_cast<thread_data*> (TlsGetValue(index_)))
			{
				// Call destructors. Implement the pthread semantics in that the
				// destructors are called until all the values become 0.
				//
				for (bool pass(true); pass;)
				{
					pass = false;

					for (size_t i(0); i < d->size; ++i)
					{
						if (d->entries[i].dtor != 0 && d->entries[i].value != 0)
						{
							pass = true;
							void* tmp(d->entries[i].value);
							d->entries[i].value = 0;
							d->entries[i].dtor(tmp);
						}
					}
				}

				operator delete (d);
			}
		}

		//
		// tls_common
		//

		std::size_t tls_common::
			_allocate(dtor_func dtor)
		{
			win32_lock l(cs_);

			size_t n(proc_data_->size);
			size_t c(proc_data_->capacity);

			if (n == c)
			{
				c *= 2;

				// Try to do "atomic" switch-over so that proc_data_ always points
				// to memory that can be freed even if this thread is killed in the
				// middle.
				//
				process_data* pd(
					static_cast<process_data*> (
					operator new (sizeof(process_data) + sizeof(dtor_func) * c)));

				memcpy(pd->dtors, proc_data_->dtors, n * sizeof(dtor_func));
				memset(pd->dtors + n, 0, sizeof(dtor_func) * (c - n));

				pd->size = n;
				pd->capacity = c;

				process_data* old(proc_data_);
				proc_data_ = pd;
				operator delete (old);
			}

			proc_data_->dtors[n] = dtor;
			return proc_data_->size++;
		}

		void* tls_common::
			_get(std::size_t key)
		{
			if (thread_data* d = static_cast<thread_data*> (TlsGetValue(index_)))
			{
				if (key < d->size)
					return d->entries[key].value;
			}

			// Check if this key is valid.
			//
			win32_lock l(cs_);

			if (key < proc_data_->size)
				return 0;

			throw win32_exception(ERROR_INVALID_INDEX);
		}

		void tls_common::
			_set(std::size_t key, void* value)
		{
			thread_data* d(static_cast<thread_data*> (TlsGetValue(index_)));

			if (d != 0 && key < d->capacity)
			{
				if (key >= d->size)
				{
					// Check if this key is valid. If so then we need to copy
					// dtors for new slots.
					//
					win32_lock l(cs_);

					size_t n(proc_data_->size);

					if (key >= n)
						throw win32_exception(ERROR_INVALID_INDEX);

					for (size_t i(d->size); i < n; ++i)
						d->entries[i].dtor = proc_data_->dtors[i];

					d->size = n;
				}

				d->entries[key].value = value;
			}
			else
			{
				// Check if this key is valid. If so then we need to (re)-allocate
				// our storage.
				//
				win32_lock l(cs_);

				size_t n(proc_data_->size);

				if (key >= n)
					throw win32_exception(ERROR_INVALID_INDEX);

				size_t c(proc_data_->capacity);

				thread_data* nd(
					static_cast<thread_data*> (
					operator new (sizeof(thread_data) + sizeof(entry) * c)));

				size_t on(d == 0 ? 0 : d->size);

				// Copy over the data.
				//
				if (on != 0)
					memcpy(nd->entries, d->entries, sizeof(entry) * on);

				// Zero out the rest.
				//
				memset(nd->entries + on, 0, sizeof(entry) * (c - on));

				// Assign destructors to new slots [on, n).
				//
				for (size_t i(on); i < n; ++i)
					nd->entries[i].dtor = proc_data_->dtors[i];

				nd->size = n;
				nd->capacity = c;

				operator delete (d);
				TlsSetValue(index_, nd);

				nd->entries[key].value = value;
			}
		}
	}
}
// file      : odb/tracer.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <iostream>

#include <odb/tracer.hxx>
#include <odb/statement.hxx>

using namespace std;

namespace odb
{
	//
	// tracer
	//

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

	//
	// stderr_tracer
	//

	class stderr_tracer_type : public tracer
	{
		virtual void
			execute(connection&, const char* statement);

		// Override the other version to get rid of a Sun CC warning.
		//
		virtual void
			execute(connection&, const statement&);
	};

	void stderr_tracer_type::
		execute(connection&, const char* s)
	{
		cerr << s << endl;
	}

	void stderr_tracer_type::
		execute(connection& c, const statement& s)
	{
		execute(c, s.text());
	}

	static stderr_tracer_type stderr_tracer_;
	tracer& stderr_tracer = stderr_tracer_;
}
// file      : odb/transaction.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/transaction.hxx>
#include <odb/exceptions.hxx>

#include <odb/details/tls.hxx>

using namespace std;

namespace odb
{
	using namespace details;

	//
	// transaction
	//

	static ODB_TLS_POINTER(transaction) current_transaction;

	transaction::
		~transaction()
	{
		if (!finalized_)
			try { rollback(); }
		catch (...) {}
	}

	void transaction::
		reset(transaction_impl* impl, bool make_current)
	{
		details::unique_ptr<transaction_impl> i(impl);

		if (!finalized_)
			rollback();

		impl_.reset(i.release());

		if (make_current && tls_get(current_transaction) != 0)
			throw already_in_transaction();

		impl_->start();
		finalized_ = false;

		if (make_current)
			tls_set(current_transaction, this);
	}

	bool transaction::
		has_current()
	{
		return tls_get(current_transaction) != 0;
	}

	transaction& transaction::
		current()
	{
		transaction* cur(tls_get(current_transaction));

		if (cur == 0)
			throw not_in_transaction();

		return *cur;
	}

	void transaction::
		current(transaction& t)
	{
		tls_set(current_transaction, &t);
	}

	void transaction::
		reset_current()
	{
		transaction* t(0);
		tls_set(current_transaction, t);
	}

	struct rollback_guard
	{
		rollback_guard(transaction& t) : t_(&t) {}
		~rollback_guard()
		{
			if (t_ != 0) t_->callback_call(transaction::event_rollback);
		}
		void release() { t_ = 0; }
	private:
		transaction* t_;
	};

	void transaction::
		commit()
	{
		if (finalized_)
			throw transaction_already_finalized();

		finalized_ = true;
		rollback_guard rg(*this);

		impl_->connection().transaction_tracer_ = 0;

		if (tls_get(current_transaction) == this)
		{
			transaction* t(0);
			tls_set(current_transaction, t);
		}

		impl_->commit();
		rg.release();

		if (callback_count_ != 0)
			callback_call(event_commit);
	}

	void transaction::
		rollback()
	{
		if (finalized_)
			throw transaction_already_finalized();

		finalized_ = true;
		rollback_guard rg(*this);

		impl_->connection().transaction_tracer_ = 0;

		if (tls_get(current_transaction) == this)
		{
			transaction* t(0);
			tls_set(current_transaction, t);
		}

		impl_->rollback();
		rg.release();

		if (callback_count_ != 0)
			callback_call(event_rollback);
	}

	void transaction::
		callback_call(unsigned short event)
	{
		size_t stack_count(callback_count_ < stack_callback_count
			? callback_count_ : stack_callback_count);
		size_t dyn_count(callback_count_ - stack_count);

		// We need to be careful with the situation where a callback
		// throws and we neither call the rest of the callbacks nor
		// reset their states. To make sure this doesn't happen, we
		// do a first pass and reset all the states.
		//
		for (size_t i(0); i < stack_count; ++i)
		{
			callback_data& d(stack_callbacks_[i]);
			if (d.event != 0 && d.state != 0)
				*d.state = 0;
		}

		for (size_t i(0); i < dyn_count; ++i)
		{
			callback_data& d(dyn_callbacks_[i]);
			if (d.event != 0 && d.state != 0)
				*d.state = 0;
		}

		// Now do the actual calls.
		//
		for (size_t i(0); i < stack_count; ++i)
		{
			callback_data& d(stack_callbacks_[i]);
			if (d.event & event)
				d.func(event, d.key, d.data);
		}

		for (size_t i(0); i < dyn_count; ++i)
		{
			callback_data& d(dyn_callbacks_[i]);
			if (d.event & event)
				d.func(event, d.key, d.data);
		}

		// Clean things up in case this instance is going to be reused.
		//
		if (dyn_count != 0)
			dyn_callbacks_.clear();

		free_callback_ = max_callback_count;
		callback_count_ = 0;
	}

	void transaction::
		callback_register(callback_type func,
		void* key,
		unsigned short event,
		unsigned long long data,
		transaction** state)
	{
		callback_data* s;

		// If we have a free slot, use it.
		//
		if (free_callback_ != max_callback_count)
		{
			s = (free_callback_ < stack_callback_count)
				? stack_callbacks_ + free_callback_
				: &dyn_callbacks_[free_callback_ - stack_callback_count];

			free_callback_ = reinterpret_cast<size_t> (s->key);
		}
		// If we have space in the stack, grab that.
		//
		else if (callback_count_ < stack_callback_count)
		{
			s = stack_callbacks_ + callback_count_;
			callback_count_++;
		}
		// Otherwise use the dynamic storage.
		//
		else
		{
			dyn_callbacks_.push_back(callback_data());
			s = &dyn_callbacks_.back();
			callback_count_++;
		}

		s->func = func;
		s->key = key;
		s->event = event;
		s->data = data;
		s->state = state;
	}

	size_t transaction::
		callback_find(void* key)
	{
		if (callback_count_ == 0)
			return 0;

		size_t stack_count;

		// See if this is the last slot registered. This will be a fast path if,
		// for example, things are going to be unregistered from destructors.
		//
		if (callback_count_ <= stack_callback_count)
		{
			if (stack_callbacks_[callback_count_ - 1].key == key)
				return callback_count_ - 1;

			stack_count = callback_count_;
		}
		else
		{
			if (dyn_callbacks_.back().key == key)
				return callback_count_ - 1;

			stack_count = stack_callback_count;
		}

		// Otherwise do a linear search.
		//
		for (size_t i(0); i < stack_count; ++i)
			if (stack_callbacks_[i].key == key)
				return i;

		for (size_t i(0), dyn_count(callback_count_ - stack_count);
			i < dyn_count; ++i)
			if (dyn_callbacks_[i].key == key)
				return i + stack_callback_count;

		return callback_count_;
	}

	void transaction::
		callback_unregister(void* key)
	{
		size_t i(callback_find(key));

		// It is ok for this function not to find the key.
		//
		if (i == callback_count_)
			return;

		// See if this is the last slot registered.
		//
		if (i == callback_count_ - 1)
		{
			if (i >= stack_callback_count)
				dyn_callbacks_.pop_back();

			callback_count_--;
		}
		else
		{
			callback_data& d(
				i < stack_callback_count
				? stack_callbacks_[i]
				: dyn_callbacks_[i - stack_callback_count]);

			// Add to the free list.
			//
			d.event = 0;
			d.key = reinterpret_cast<void*> (free_callback_);
			free_callback_ = i;
		}
	}

	void transaction::
		callback_update(void* key,
		unsigned short event,
		unsigned long long data,
		transaction** state)
	{
		size_t i(callback_find(key));

		// It is ok for this function not to find the key.
		//
		if (i == callback_count_)
			return;

		callback_data& d(
			i < stack_callback_count
			? stack_callbacks_[i]
			: dyn_callbacks_[i - stack_callback_count]);

		d.event = event;
		d.data = data;
		d.state = state;
	}

	//
	// transaction_impl
	//

	transaction_impl::
		~transaction_impl()
	{
	}
}
// file      : odb/vector-impl.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/vector-impl.hxx>

#include <cstring>   // std::memcpy, std::memset
#include <algorithm> // std::swap

using namespace std;

namespace odb
{
	// vector_impl
	//
	const unsigned char vector_impl::mask_[4] = { 0x3, 0xC, 0x30, 0xC0 };
	const unsigned char vector_impl::shift_[4] = { 0, 2, 4, 6 };

	vector_impl::
		vector_impl(const vector_impl& x)
		: state_(x.state_), size_(0), tail_(0), capacity_(0), data_(0)
	{
		// Copy the data over if we are tracking.
		//
		if (state_ == state_tracking && size_ != 0)
		{
			realloc(x.size_ < 1024 ? 1024 : x.size_);
			memcpy(data_, x.data_, x.size_ / 4 + (x.size_ % 4 == 0 ? 0 : 1));
			size_ = x.size_;
			tail_ = x.tail_;
		}
	}

	void vector_impl::
		realloc(size_t n)
	{
		// The new capacity can be less or greater than the old one, but
		// it cannot be less than size.
		//
		size_t b(n / 4 + (n % 4 == 0 ? 0 : 1));

		if (b != capacity_ * 4)
		{
			unsigned char* d(static_cast<unsigned char*> (operator new (b)));

			if (size_ != 0)
				memcpy(d, data_, size_ / 4 + (size_ % 4 == 0 ? 0 : 1));

			if (data_ != 0)
				operator delete (data_);

			data_ = d;
			capacity_ = b * 4;
		}
	}

	void vector_impl::
		shrink_to_fit()
	{
		if (size_ != capacity_)
		{
			if (size_ != 0)
				realloc(size_);
			else
			{
				operator delete (data_);
				data_ = 0;
				capacity_ = 0;
			}
		}
	}

	void vector_impl::
		start(size_t n)
	{
		if (n != 0)
		{
			if (capacity_ < n)
			{
				size_ = 0;
				realloc(n < 1024 ? 1024 : n);
			}

			memset(data_, 0, n / 4 + (n % 4 == 0 ? 0 : 1));
		}

		state_ = state_tracking;
		size_ = tail_ = n;
	}

	void vector_impl::
		push_back(size_t n)
	{
		for (; n != 0; --n)
		{
			size_t i(tail_);

			element_state_type s;
			if (i != size_)
				// We have an erased element we can reuse.
				//
				s = state_updated;
			else
			{
				if (size_ == capacity_)
				{
					size_t c(capacity_ == 0 ? 1024 : capacity_ * 2);
					if (c < size_ + n)
						c = size_ + n;
					realloc(c);
				}

				s = state_inserted;
				size_++;
			}

			set(i, s);
			tail_++;
		}
	}

	void vector_impl::
		pop_back(size_t n)
	{
		for (; n != 0; --n)
		{
			size_t i(tail_ - 1);

			if (state(i) != state_inserted)
				set(i, state_erased);
			else
				size_--; // tail_ == size_

			tail_--;
		}
	}

	void vector_impl::
		insert(size_t i, size_t n)
	{
		for (; i != tail_; ++i)
			if (state(i) != state_inserted)
				set(i, state_updated);

		push_back(n);
	}

	void vector_impl::
		erase(size_t i, size_t n)
	{
		pop_back(n);

		for (; i != tail_; ++i)
			if (state(i) != state_inserted)
				set(i, state_updated);
	}

	void vector_impl::
		clear()
	{
		// The idea is to drop any inserted elements from the back and
		// set everything else to erased.
		//
		if (tail_ == size_)
		{
			while (size_ != 0 && state(size_ - 1) == state_inserted)
				size_--;

			tail_ = size_;
		}

		if (tail_ != 0)
			memset(data_, 0xFF, tail_ / 4 + (tail_ % 4 == 0 ? 0 : 1));

		tail_ = 0;
	}

	// vector_base
	//
	void vector_base::
		rollback(unsigned short, void* key, unsigned long long)
	{
		// Mark as changed.
		//
		static_cast<vector_base*> (key)->impl_.change();
	}

	void vector_base::
		swap_tran(vector_base& x)
	{
		// If either instance is armed, then we need to update the
		// callback registration.
		//
		transaction* t(x.tran_);
		if (tran_ != 0)
		{
			tran_->callback_unregister(this);
			x._arm(*tran_);
		}

		if (t != 0)
		{
			t->callback_unregister(&x);
			_arm(*t);
		}

		std::swap(tran_, x.tran_);
	}
}
