// file      : odb/plugin.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/gcc.hxx> // Keep it first.

#include <memory>  // std::auto_ptr
#include <string>
#include <vector>
#include <cstring> // std::strcpy
#include <cassert>
#include <iostream>

#include <cutl/re.hxx>
#include <cutl/fs/path.hxx>

#include <odb/pragma.hxx>
#include <odb/parser.hxx>
#include <odb/options.hxx>
#include <odb/option-functions.hxx>
#include <odb/features.hxx>
#include <odb/profile.hxx>
#include <odb/version.hxx>
#include <odb/validator.hxx>
#include <odb/processor.hxx>
#include <odb/generator.hxx>
#include <odb/semantics/unit.hxx>

using namespace std;
using namespace semantics;

using cutl::fs::path;
using cutl::fs::invalid_path;

typedef vector<path> paths;

int plugin_is_GPL_compatible;
auto_ptr<options const> options_;
paths profile_paths_;
path file_;    // File being compiled.
paths inputs_; // List of input files in at-once mode or just file_.


// A prefix of the _cpp_file struct. This struct is not part of the
// public interface so we have to resort to this technique (based on
// libcpp/files.c).
//
struct cpp_file_prefix
{
  char const* name;
  char const* path;
  char const* pchname;
  char const* dir_name;
};

extern "C" void
start_unit_callback (void*, void*)
{
  // Set the directory of the main file (stdin) to that of the orginal
  // file.
  //
  cpp_buffer* b (cpp_get_buffer (parse_in));
  _cpp_file* f (cpp_get_file (b));
  char const* p (cpp_get_path (f));
  cpp_file_prefix* fp (reinterpret_cast<cpp_file_prefix*> (f));

  // Perform sanity checks.
  //
  if (p != 0 && *p == '\0'     // The path should be empty (stdin).
      && cpp_get_prev (b) == 0 // This is the only buffer (main file).
      && fp->path == p         // Our prefix corresponds to the actual type.
      && fp->dir_name == 0)    // The directory part hasn't been initialized.
  {
    path d (file_.directory ());
    char* s;

    if (d.empty ())
    {
      s = XNEWVEC (char, 1);
      *s = '\0';
    }
    else
    {
      size_t n (d.string ().size ());
      s = XNEWVEC (char, n + 2);
      strcpy (s, d.string ().c_str ());
      s[n] = path::traits::directory_separator;
      s[n + 1] = '\0';
    }

    fp->dir_name = s;
  }
  else
  {
    cerr << "ice: unable to initialize main file directory" << endl;
    exit (1);
  }
}

extern "C" void
gate_callback (void*, void*)
{
  // If there were errors during compilation, let GCC handle the
  // exit.
  //
  if (errorcount || sorrycount)
    return;

  int r (0);

  try
  {
    // Post process pragmas.
    //
    post_process_pragmas ();

    // Parse the GCC tree to semantic graph.
    //
    parser p (*options_, loc_pragmas_, ns_loc_pragmas_, decl_pragmas_);
    auto_ptr<unit> u (p.parse (global_namespace, file_));


    features f;

    // Validate, pass 1.
    //
    validator v;
    v.validate (*options_, f, *u, file_, 1);

    // Process.
    //
    processor pr;
    pr.process (*options_, f, *u, file_);

    // Validate, pass 2.
    //
    v.validate (*options_, f, *u, file_, 2);

    // Generate.
    //
    generator g;
    g.generate (*options_, f, *u, file_, inputs_);
  }
  catch (cutl::re::format const& e)
  {
    cerr << "error: invalid regex: '" << e.regex () << "': " <<
      e.description () << endl;
    r = 1;
  }
  catch (pragmas_failed const&)
  {
    // Diagnostics has aready been issued.
    //
    r = 1;
  }
  catch (parser::failed const&)
  {
    // Diagnostics has aready been issued.
    //
    r = 1;
  }
  catch (validator::failed const&)
  {
    // Diagnostics has aready been issued.
    //
    r = 1;
  }
  catch (processor::failed const&)
  {
    // Diagnostics has aready been issued.
    //
    r = 1;
  }
  catch (generator::failed const&)
  {
    // Diagnostics has aready been issued.
    //
    r = 1;
  }

  exit (r);
}

static char const* const odb_version = ODB_COMPILER_VERSION_STR;

typedef vector<string> strings;

extern "C" int
plugin_init (plugin_name_args* plugin_info, plugin_gcc_version*)
{
  int r (0);
  plugin_info->version = odb_version;

  try
  {
    // Parse options.
    //
    {
      strings argv_str;
      vector<char*> argv;

      argv_str.push_back (plugin_info->base_name);
      argv.push_back (const_cast<char*> (argv_str.back ().c_str ()));

      for (int i (0); i < plugin_info->argc; ++i)
      {
        plugin_argument& a (plugin_info->argv[i]);

        // A value cannot contain '=' so it is passed as the backspace
        // character.
        //
        string v (a.value != 0 ? a.value : "");
        for (size_t i (0); i < v.size (); ++i)
          if (v[i] == '\b')
            v[i] = '=';

        // Handle service options.
        //
        if (strcmp (a.key, "svc-path") == 0)
        {
          profile_paths_.push_back (path (v));
          continue;
        }

        if (strcmp (a.key, "svc-file") == 0)
        {
          // First is the main file. Subsequent are inputs in the at-once
          // mode.
          //
          if (file_.empty ())
            file_ = path (v);
          else
            inputs_.push_back (path (v));

          continue;
        }

        string opt (strlen (a.key) > 1 ? "--" : "-");
        opt += a.key;

        argv_str.push_back (opt);
        argv.push_back (const_cast<char*> (argv_str.back ().c_str ()));

        if (!v.empty ())
        {
          argv_str.push_back (v);
          argv.push_back (const_cast<char*> (argv_str.back ().c_str ()));
        }
      }

      if (inputs_.empty ())
        inputs_.push_back (file_);

      // Two-phase options parsing, similar to the driver.
      //
      int argc (static_cast<int> (argv.size ()));

      cli::argv_file_scanner::option_info oi[3];
      oi[0].option = "--options-file";
      oi[0].search_func = 0;
      oi[1].option = "-p";
      oi[2].option = "--profile";

      database db;
      {
        oi[1].search_func = &profile_search_ignore;
        oi[2].search_func = &profile_search_ignore;

        cli::argv_file_scanner scan (argc, &argv[0], oi, 3);
        options ops (scan);
        assert (ops.database_specified ());
        db = ops.database ()[0];
      }

      profile_data pd (profile_paths_, db, "odb plugin");
      oi[1].search_func = &profile_search;
      oi[2].search_func = &profile_search;
      oi[1].arg = &pd;
      oi[2].arg = &pd;

      cli::argv_file_scanner scan (argc, &argv[0], oi, 3);
      auto_ptr<options> ops (
        new options (scan, cli::unknown_mode::fail, cli::unknown_mode::fail));

      // Process options.
      //
      process_options (*ops);

      options_ = ops;
      pragma_db_ = db;
      pragma_multi_ = options_->multi_database ();
    }

    if (options_->trace ())
      cerr << "starting plugin " << plugin_info->base_name << endl;

    // Disable assembly output. GCC doesn't define HOST_BIT_BUCKET
    // correctly for MinGW (it still used /dev/null which fails to
    // open).
    //
#ifdef _WIN32
    asm_file_name = "nul";
#else
    asm_file_name = HOST_BIT_BUCKET;
#endif

    // Register callbacks.
    //
    register_callback (plugin_info->base_name,
                       PLUGIN_PRAGMAS,
                       register_odb_pragmas,
                       0);

    register_callback (plugin_info->base_name,
                       PLUGIN_START_UNIT,
                       start_unit_callback,
                       0);

    register_callback (plugin_info->base_name,
                       PLUGIN_OVERRIDE_GATE,
                       &gate_callback,
                       0);
  }
  catch (cli::exception const& ex)
  {
    cerr << ex << endl;
    r = 1;
  }

  if (r != 0)
    exit (r);

  return r;
}
