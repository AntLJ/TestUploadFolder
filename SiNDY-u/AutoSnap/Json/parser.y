/*** yacc/bison Declarations ***/

/* Require bison 3.0 or later */
%require "3.0"

/* add debug output code to generated parser. disable this for release
 * versions. */
 //%debug

/* start symbol is named "start" */
%start start

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* write out a header file containing the token defines */
%defines

/* namespace to enclose parser in */
%define api.prefix {json}

/* set the parser's class identifier */
%define parser_class_name {Parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver } { class Scanner& lexer }

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* verbose error messages */
%error-verbose

%define api.token.prefix {TOK_}
%token
						END	     0		"end of file"
						EOL				"end of line"
						O_START			"{"
						O_END			"}"
						A_START			"["
						A_END			"]"
						COMMA			","
						COLON			":"
						J_NULL			"null"
%token <bool>			BOOLEAN			"boolean"
%token <int>		 	INTEGER			"integer"
%token <long long>		LONGINTEGER		"long integer"
%token <double> 		DOUBLE			"double"
%token <std::string> 	STRING			"string"

%type <JsonValuePtr> object array value members elements

 /*** END JSON - Change the JSON grammar's tokens above ***/

 %printer { yyoutput << $$; } <*>;

%{
#include "driver.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex lexer.lex

%}

%% /*** Grammar Rules ***/
		

 /*** BEGIN JSON - Change the json grammar rules below ***/

start	
		:	object	{driver.root = $1;}
		|	array	{driver.root = $1;}
		;
		
object  
		: O_START O_END { $$ = std::make_shared<JsonObject>(); } 
        | O_START members O_END { $$ = $2; }
		;

members 
		: STRING COLON value { $$ = std::make_shared<JsonObject>(std::move($1), $3);}
        | STRING COLON value COMMA members { $$=$5; std::dynamic_pointer_cast<JsonObject>($$)->Add(std::move($1), $3);}
		;

array   
		: A_START A_END {$$ = std::make_shared<JsonArray>();}
        | A_START elements A_END { $$ = $2;}
		;

elements
		: value { $$ = std::make_shared<JsonArray>($1);}
        | value COMMA elements { $$ = $3; std::dynamic_pointer_cast<JsonArray>($$)->Insert($1);}
		;
value   
		: array  { $$ = $1; }
        | object { $$ = $1; }
        | STRING  { $$ = std::make_shared<JsonString>(std::move($1));}
        | INTEGER  { $$ = std::make_shared<JsonInteger>($1);}
		| LONGINTEGER { $$ = std::make_shared<JsonLongInteger>($1);}
        | DOUBLE { $$ = std::make_shared<JsonDouble>($1); }
        | BOOLEAN { $$ = std::make_shared<JsonBool>($1); }
        | J_NULL { $$ = std::make_shared<JsonNull>(); } 
		;

 /*** END JSON - Change the json grammar rules above ***/

%% /*** Additional Code ***/

void json::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}