// A Bison parser, made by GNU Bison 3.0.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex   jsonlex

// First part of user declarations.
#line 68 "parser.y" // lalr1.cc:398

#include "driver.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex lexer.lex


#line 49 "parser.cpp" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "parser.hpp"

// User implementation prologue.

#line 63 "parser.cpp" // lalr1.cc:406


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if JSONDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !JSONDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !JSONDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace json {
#line 149 "parser.cpp" // lalr1.cc:473

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (class Driver& driver_yyarg, class Scanner& lexer_yyarg)
    :
#if JSONDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg),
      lexer (lexer_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 18: // object
      case 19: // members
      case 20: // array
      case 21: // elements
      case 22: // value
        value.move< JsonValuePtr > (that.value);
        break;

      case 11: // "boolean"
        value.move< bool > (that.value);
        break;

      case 14: // "double"
        value.move< double > (that.value);
        break;

      case 12: // "integer"
        value.move< int > (that.value);
        break;

      case 13: // "long integer"
        value.move< long long > (that.value);
        break;

      case 15: // "string"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 18: // object
      case 19: // members
      case 20: // array
      case 21: // elements
      case 22: // value
        value.copy< JsonValuePtr > (that.value);
        break;

      case 11: // "boolean"
        value.copy< bool > (that.value);
        break;

      case 14: // "double"
        value.copy< double > (that.value);
        break;

      case 12: // "integer"
        value.copy< int > (that.value);
        break;

      case 13: // "long integer"
        value.copy< long long > (that.value);
        break;

      case 15: // "string"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if JSONDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 11: // "boolean"

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< bool > (); }
#line 359 "parser.cpp" // lalr1.cc:616
        break;

      case 12: // "integer"

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< int > (); }
#line 366 "parser.cpp" // lalr1.cc:616
        break;

      case 13: // "long integer"

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< long long > (); }
#line 373 "parser.cpp" // lalr1.cc:616
        break;

      case 14: // "double"

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< double > (); }
#line 380 "parser.cpp" // lalr1.cc:616
        break;

      case 15: // "string"

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< std::string > (); }
#line 387 "parser.cpp" // lalr1.cc:616
        break;

      case 18: // object

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< JsonValuePtr > (); }
#line 394 "parser.cpp" // lalr1.cc:616
        break;

      case 19: // members

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< JsonValuePtr > (); }
#line 401 "parser.cpp" // lalr1.cc:616
        break;

      case 20: // array

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< JsonValuePtr > (); }
#line 408 "parser.cpp" // lalr1.cc:616
        break;

      case 21: // elements

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< JsonValuePtr > (); }
#line 415 "parser.cpp" // lalr1.cc:616
        break;

      case 22: // value

#line 66 "parser.y" // lalr1.cc:616
        { yyoutput << yysym.value.template as< JsonValuePtr > (); }
#line 422 "parser.cpp" // lalr1.cc:616
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if JSONDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // JSONDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 37 "parser.y" // lalr1.cc:726
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &driver.streamname;
}

#line 546 "parser.cpp" // lalr1.cc:726

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 action is NOT applied when using
       variants.  */
      switch (yyr1_[yyn])
    {
      case 18: // object
      case 19: // members
      case 20: // array
      case 21: // elements
      case 22: // value
        yylhs.value.build< JsonValuePtr > ();
        break;

      case 11: // "boolean"
        yylhs.value.build< bool > ();
        break;

      case 14: // "double"
        yylhs.value.build< double > ();
        break;

      case 12: // "integer"
        yylhs.value.build< int > ();
        break;

      case 13: // "long integer"
        yylhs.value.build< long long > ();
        break;

      case 15: // "string"
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 2:
#line 85 "parser.y" // lalr1.cc:846
    {driver.root = yystack_[0].value.as< JsonValuePtr > ();}
#line 686 "parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 86 "parser.y" // lalr1.cc:846
    {driver.root = yystack_[0].value.as< JsonValuePtr > ();}
#line 692 "parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 90 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonObject>(); }
#line 698 "parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 91 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = yystack_[1].value.as< JsonValuePtr > (); }
#line 704 "parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 95 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonObject>(std::move(yystack_[2].value.as< std::string > ()), yystack_[0].value.as< JsonValuePtr > ());}
#line 710 "parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 96 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > ()=yystack_[0].value.as< JsonValuePtr > (); std::dynamic_pointer_cast<JsonObject>(yylhs.value.as< JsonValuePtr > ())->Add(std::move(yystack_[4].value.as< std::string > ()), yystack_[2].value.as< JsonValuePtr > ());}
#line 716 "parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 100 "parser.y" // lalr1.cc:846
    {yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonArray>();}
#line 722 "parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 101 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = yystack_[1].value.as< JsonValuePtr > ();}
#line 728 "parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 105 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonArray>(yystack_[0].value.as< JsonValuePtr > ());}
#line 734 "parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 106 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = yystack_[0].value.as< JsonValuePtr > (); std::dynamic_pointer_cast<JsonArray>(yylhs.value.as< JsonValuePtr > ())->Insert(yystack_[2].value.as< JsonValuePtr > ());}
#line 740 "parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 109 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = yystack_[0].value.as< JsonValuePtr > (); }
#line 746 "parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 110 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = yystack_[0].value.as< JsonValuePtr > (); }
#line 752 "parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 111 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonString>(std::move(yystack_[0].value.as< std::string > ()));}
#line 758 "parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 112 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonInteger>(yystack_[0].value.as< int > ());}
#line 764 "parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 113 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonLongInteger>(yystack_[0].value.as< long long > ());}
#line 770 "parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 114 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonDouble>(yystack_[0].value.as< double > ()); }
#line 776 "parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 115 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonBool>(yystack_[0].value.as< bool > ()); }
#line 782 "parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 116 "parser.y" // lalr1.cc:846
    { yylhs.value.as< JsonValuePtr > () = std::make_shared<JsonNull>(); }
#line 788 "parser.cpp" // lalr1.cc:846
    break;


#line 792 "parser.cpp" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* $$ was initialized before running the user action.  */
    YY_SYMBOL_PRINT ("Error: discarding", yylhs);
    yylhs.~stack_symbol_type();
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -6;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
      20,     0,    -4,     1,    -6,    -6,    -6,    -5,    11,    -6,
      -6,    -6,    -6,    -6,    -6,    -6,    -6,    -6,     6,     9,
      -6,     8,    -6,    -6,     8,    17,    -6,    12,    -6
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       0,     0,     0,     0,     2,     3,     4,     0,     0,     8,
      19,    18,    15,    16,    17,    14,    13,    12,     0,    10,
       1,     0,     5,     9,     0,     6,    11,     0,     7
  };

  const signed char
  Parser::yypgoto_[] =
  {
      -6,    -6,    28,     2,    30,     7,    13
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     3,    16,     8,    17,    18,    19
  };

  const unsigned char
  Parser::yytable_[] =
  {
       1,    20,     2,     9,    21,     6,    10,    11,    12,    13,
      14,    15,     1,    23,     2,     7,    22,    24,    10,    11,
      12,    13,    14,    15,     1,    27,     2,     7,     4,    28,
       5,    26,     0,     0,    25
  };

  const signed char
  Parser::yycheck_[] =
  {
       4,     0,     6,     7,     9,     5,    10,    11,    12,    13,
      14,    15,     4,     7,     6,    15,     5,     8,    10,    11,
      12,    13,    14,    15,     4,     8,     6,    15,     0,    27,
       0,    24,    -1,    -1,    21
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,     4,     6,    17,    18,    20,     5,    15,    19,     7,
      10,    11,    12,    13,    14,    15,    18,    20,    21,    22,
       0,     9,     5,     7,     8,    22,    21,     8,    19
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    16,    17,    17,    18,    18,    19,    19,    20,    20,
      21,    21,    22,    22,    22,    22,    22,    22,    22,    22
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     3,     3,     5,     2,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"end of line\"", "\"{\"",
  "\"}\"", "\"[\"", "\"]\"", "\",\"", "\":\"", "\"null\"", "\"boolean\"",
  "\"integer\"", "\"long integer\"", "\"double\"", "\"string\"", "$accept",
  "start", "object", "members", "array", "elements", "value", YY_NULL
  };

#if JSONDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,    85,    85,    86,    90,    91,    95,    96,   100,   101,
     105,   106,   109,   110,   111,   112,   113,   114,   115,   116
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // JSONDEBUG



} // json
#line 1174 "parser.cpp" // lalr1.cc:1156
#line 121 "parser.y" // lalr1.cc:1157
 /*** Additional Code ***/

void json::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
