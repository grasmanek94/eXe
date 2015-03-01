// file      : odb/cxx-lexer.cxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/gcc.hxx>

#include <stdio.h>
#include <stdarg.h>

#include <new>      // std::bad_alloc
#include <cassert>
#include <iostream>

#include <odb/cxx-lexer.hxx>

using namespace std;

//
// cxx_lexer
//

// Token spelling. See cpplib.h for details.
//
#define OP(e, s) s ,
#define TK(e, s) #e ,
char const* cxx_lexer::token_spelling[N_TTYPES + 1] = { TTYPE_TABLE "KEYWORD"};
#undef OP
#undef TK

cxx_lexer::
~cxx_lexer ()
{
}

//
// cxx_tokens_lexer
//

void cxx_tokens_lexer::
start (cxx_tokens const& ts, location_t start_loc)
{
  tokens_ = &ts;
  cur_ = ts.begin ();
  loc_ = start_loc;
}

cpp_ttype cxx_tokens_lexer::
next (std::string& token, tree* node)
{
  if (cur_ != tokens_->end ())
  {
    loc_ = cur_->loc;
    token = cur_->literal;
    if (node != 0)
      *node = cur_->node;
    return static_cast<cpp_ttype> (cur_++->type);
  }
  else
    return CPP_EOF;
}

location_t cxx_tokens_lexer::
location () const
{
  return loc_;
}

//
// cxx_pragma_lexer
//

void cxx_pragma_lexer::
start ()
{
  token_ = &token_data_;
  type_ = &type_data_;
}

string cxx_pragma_lexer::
start (tree& token, cpp_ttype& type)
{
  token_ = &token;
  type_ = &type;

  return translate ();
}

cpp_ttype cxx_pragma_lexer::
next (string& token, tree* node)
{
  *type_ = pragma_lex (token_);

  // See if this is a keyword using the C++ parser machinery and
  // the current C++ dialect.
  //
  if (*type_ == CPP_NAME && C_IS_RESERVED_WORD (*token_))
    *type_ = CPP_KEYWORD;

  if (node != 0 && node != token_)
    *node = *token_;

  token = translate ();
  return *type_;
}

location_t cxx_pragma_lexer::
location () const
{
  return input_location;
}

string cxx_pragma_lexer::
translate ()
{
  string r;

  if (*type_ == CPP_NAME || *type_ == CPP_KEYWORD)
    r = IDENTIFIER_POINTER (*token_);
  else if (*type_ == CPP_STRING)
    r = TREE_STRING_POINTER (*token_);

  return r;
}

//
// cxx_string_lexer
//

// Diagnostics callback.
//
extern "C" bool
cpp_error_callback (
  cpp_reader* reader,
  int level,
#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 5
  int /*reason*/, // Added in GCC 4.6.0.
#endif
  location_t,
  unsigned int,
  char const* msg,
  va_list *ap)
{
  char const* kind (0);
  switch (level)
  {
  case CPP_DL_NOTE:
  case CPP_DL_WARNING_SYSHDR:
  case CPP_DL_WARNING:
  case CPP_DL_PEDWARN:
    // Ignore these.
    break;
  case CPP_DL_ERROR:
  case CPP_DL_FATAL:
    kind = "error";
    break;
  case CPP_DL_ICE:
    kind = "ice";
    break;
  default:
    kind = "unknown";
    break;
  }

  if (kind != 0)
  {
    fprintf (stderr, "%s: ", kind);
    vfprintf (stderr, msg, *ap);
    fprintf (stderr, "\n");

    // By resetting the error callback we indicate to cxx_string_lexer
    // that there was an error.
    //
    cpp_get_callbacks (reader)->error = 0;
    return true;
  }

  return false;
}

cxx_string_lexer::
cxx_string_lexer ()
    : reader_ (0)
{
  linemap_init (&line_map_);

#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 6
  line_map_.round_alloc_size = ggc_round_alloc_size;
#endif

  linemap_add (&line_map_, LC_ENTER, 0, "<memory>", 0);

  reader_ = cpp_create_reader (
    cxx_dialect == cxx0x
#if BUILDING_GCC_MAJOR > 4 || BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR > 6
    ? CLK_CXX11
#else
    ? CLK_CXX0X
#endif
    : CLK_CXX98,
    0,
    &line_map_);

  if (reader_ == 0)
    throw bad_alloc ();

  callbacks_ = cpp_get_callbacks (reader_);
}

cxx_string_lexer::
~cxx_string_lexer ()
{
  if (reader_ != 0)
    cpp_destroy (reader_);

  // Was removed as "dead code" in GCC 4.7.0.
  //
#if BUILDING_GCC_MAJOR == 4 && BUILDING_GCC_MINOR <= 6
  linemap_free (&line_map_);
#endif
}

void cxx_string_lexer::
start (string const& data)
{
  // The previous lexing session should have popped the buffer.
  //
  assert (cpp_get_buffer (reader_) == 0);
  callbacks_->error = &cpp_error_callback;

  data_ = data;
  buf_ = data;
  buf_ += '\n';
  loc_ = 0;

  cpp_push_buffer (
    reader_,
    reinterpret_cast<unsigned char const*> (buf_.c_str ()),
    buf_.size (),
    true);
}

cpp_ttype cxx_string_lexer::
next (string& token, tree* node)
{
  token.clear ();
  cpp_token const* t (cpp_get_token (reader_));

  // If there was an error, the error callback will be reset to 0.
  // Diagnostics has already been issued.
  //
  if (callbacks_->error == 0)
    throw invalid_input ();

  cpp_ttype tt (t->type);

  switch (tt)
  {
  case CPP_NAME:
    {
      char const* name (
        reinterpret_cast<char const*> (NODE_NAME (t->val.node.node)));

      // See if this is a keyword using the C++ parser machinery and
      // the current C++ dialect.
      //
      tree id (get_identifier (name));

      if (C_IS_RESERVED_WORD (id))
        tt = CPP_KEYWORD;

      if (node != 0)
        *node = id;

      token = name;
      break;
    }
  case CPP_STRING:
  case CPP_NUMBER:
    {
      if (node != 0)
        *node = 0; // Doesn't seem to be available.

      cpp_string const& s (t->val.str);
      token.assign (reinterpret_cast<char const*> (s.text), s.len);
      break;
    }
  default:
    {
      if (tt <= CPP_LAST_PUNCTUATOR)
      {
        if (node != 0)
          *node = 0;
        token = token_spelling[tt];
      }
      else
      {
        cerr << "unexpected token '" << token_spelling[tt] << "' in '" <<
          data_ << "'" << endl;
        throw invalid_input ();
      }
      break;
    }
  }

  // Cache the location of this token.
  //
  loc_ = t->src_loc;

  return tt;
}

location_t cxx_string_lexer::
location () const
{
  return loc_;
}
