%{ /*** C/C++ Declarations ***/

#include <string>
#include "driver.h"
#include <cstdlib>
#define YY_NO_UNISTD_H
%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "JsonFlexLexer" */
%option prefix="Json"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
 //%option debug

/* no support for include files is planned */
%option yywrap nounput 

/* enables the use of start condition stacks */
%option stack

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  loc.columns(yyleng);
std::string specialString;
%}
JSONTrue								true
JSONFalse								false
JSONNull								null
JSONInteger								[-+]?[0-9]{1,9}
JSONLongInteger							[-+]?[0-9]{10,}
JSONFloat								([-+]?([0-9]*)?\.[0-9]*([eE][-+]?[0-9]+)?)|([-+]?([0-9]+)(\.[0-9]*)?[eE][-+]?[0-9]+)
JSONIdent								[a-zA-Z_]+[0-9a-zA-Z_]*

%x COMMENT STRING

%%
 /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    loc.step();
%}

 /** Multi line comment:*/
\/* loc.step();	loc.step();BEGIN(COMMENT);
<COMMENT>\n loc.step();loc.lines(1);
<COMMENT>\*\/ loc.step();BEGIN(INITIAL);
<COMMENT>. loc.step();

 /** Single line comment **/
\/\/.*\n {
	
	loc.step();
	loc.lines(1);
}

\" specialString.clear(); BEGIN(STRING);
<STRING>\\\" specialString.push_back('"');
<STRING>\\\\ specialString.push_back('\\');
<STRING>\\\/ specialString.push_back('/');
<STRING>\\b specialString.push_back('\b');
<STRING>\\n specialString.push_back('\n');
<STRING>\\f specialString.push_back('\f');
<STRING>\\r specialString.push_back('\r');
<STRING>\\t specialString.push_back('\t');
<STRING>\\u specialString.push_back('\\');specialString.push_back('u');
<STRING>\\	std::cerr << "Synax error! Skip:" << loc << " " << yytext << std::endl; 
<STRING>[^\"] specialString.push_back(*yytext);
<STRING>\" BEGIN(INITIAL);return json::Parser::make_STRING(specialString, loc);

\[	{ return json::Parser::make_A_START(loc);  }
\]	{ return json::Parser::make_A_END(loc); }
\{	{ return json::Parser::make_O_START(loc);  }
\}	{ return json::Parser::make_O_END(loc);  }
,	{ return json::Parser::make_COMMA(loc); }
:	{ return json::Parser::make_COLON(loc); }

\n {
    loc.lines(yyleng); 
	loc.step();
}

[ \t] {
    loc.step();
}

{JSONInteger}	{
	return json::Parser::make_INTEGER(atoi(yytext), loc);
}

{JSONLongInteger}	{
#ifdef _MSC_VER
	return json::Parser::make_LONGINTEGER(_atoi64(yytext), loc);
#else
	return json::Parser::make_LONGINTEGER(atoll(yytext), loc);
#endif
}

{JSONFloat}	{
    return json::Parser::make_DOUBLE(atof(yytext), loc);
}

{JSONTrue}	{
	return json::Parser::make_BOOLEAN(true, loc); 
}

{JSONFalse}	{
	return json::Parser::make_BOOLEAN(false, loc); 
}

{JSONNull}	{
	return json::Parser::make_J_NULL(loc); 
}

. {
	/** For Japanese space , Just skip the strange character. **/ 
	loc.step(); 
	std::cerr << "Synax error! Skip:" << loc << " " << yytext << std::endl; 
 }

<<EOF>>    return json::Parser::make_END(loc);

 /*** END JSON - Change the json lexer rules above ***/

%% /*** Additional Code ***/

namespace json {

Scanner::Scanner(std::istream* in, std::ostream* out)
    : JsonFlexLexer(in, out)
{
}

Scanner::~Scanner()
{
}

void Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}

}

/* This implementation of JsonFlexLexer::yylex() is required to fill the
 * vtable of the class JsonFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

int JsonFlexLexer::yylex()
{
    std::cerr << "in JsonFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int JsonFlexLexer::yywrap()
{
    return 1;
}
