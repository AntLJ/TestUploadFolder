/**
 * @file driver.h
 * @brief JSON Object parser library
 * @details This is a simple sample of using flex/bison with C++.
 * @copyright Copyright 2015 Increment P Corp.
 * @author Gu
 * @date Jan. 25 2015
 * @version 0.1
 */
#ifndef JSON_DRIVER_H
#define JSON_DRIVER_H

#include <string>

//
// The C++ source files generated by flex/bison are not for humans reading,
// they are only for C++ compiler.
// So do not care the source: parser.cpp scanner.cpp,
// just compile them and check the warnings and errors.
//
// Sometime, it may also be a happy thing when compilers can understand the source code.
//

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.
#ifndef YY_DECL
#define	YY_DECL json::Parser::symbol_type json::Scanner::lex()
#endif

//Rename the class name yyFlexLexer as JsonFlexLexer
#ifndef __FLEX_LEXER_H
#define yyFlexLexer JsonFlexLexer
#include "FlexLexer.h"
#undef yyFlexLexer
#endif

// JSON objects
#include "json.h"
// Token definition, generated by bison.
#include "parser.hpp"

/** The json namespace is used to encapsulate the three parser classes
 * json::Parser, json::Scanner and json::Driver */
namespace json {


/** 
 * @brief flex class
 * @details Scanner is a derived class to add some extra function to the scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to JsonFlexLexer. However we change the context of the generated
 * yylex() function to be contained within the Scanner class. This is required
 * because the yylex() defined in JsonFlexLexer has no parameters. 
 */
class Scanner : public JsonFlexLexer
{
public:
    /**
	 * @brief construct
	 * @details Create a new scanner object. The streams arg_yyin and arg_yyout default
     * to cin and cout, but that assignment is only made when initializing in
     * yylex(). 
	 *
	 */
    Scanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
    virtual ~Scanner();

    /** 
	 * @brief Wrapper function
	 * @details This is the main lexing function. It is generated by flex according to
     * the macro declaration YY_DECL above. The generated bison parser then
     * calls this virtual function to fetch new tokens. */
    virtual Parser::symbol_type lex();

    /** 
	 * @brief Enable debug output (via arg_yyout) if compiled into the scanner. 
	 */
    void set_debug(bool b);

protected:
	/** For lexer records the location in the source file.*/
	Parser::location_type loc;
};


/**
 * @brief Driver
 * @details The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. 
 */
class Driver
{
public:
    /// construct a new parser driver context
    Driver();

    /// enable debug output in the flex scanner
    bool trace_scanning;

    /// enable debug output in the bison parser
    bool trace_parsing;

    /// stream name (file or input stream) used for error messages.
    std::string streamname;

	/** Parsed result*/
	JsonValuePtr root;

    /** 
	 * @brief Invoke the scanner and parser for a stream.
     * @param[in] in	input stream
     * @param[in] sname	stream name for error messages
     * @return true if successfully parsed
     */
    bool parse_stream(std::istream& in,
		      const std::string& sname = "stream input");

    /** 
	 * @brief Invoke the scanner and parser on an input string.
	 *
     * @param[in] input	input string
     * @param[in] sname	stream name for error messages
     * @return true if successfully parsed
     */
    bool parse_string(const std::string& input,
		      const std::string& sname = "string stream");

    /** 
	 * @brief Invoke the scanner and parser on a file. Use parse_stream with a
     * std::ifstream if detection of file reading errors is required.
	 *
     * @param[in] filename	input file name
     * @return true if successfully parsed
     */
    bool parse_file(const std::string& filename);

	//
    // To demonstrate pure handling of parse errors, instead of
    // simply dumping them on the standard error output, we will pass
    // them to the driver using the following two member functions.
	//
    /**
	 * @brief Error handling with associated line number. This can be modified to
     * output the error e.g. to a dialog box. 
	 *
	 * param[in] loc location
	 * param[in] msg message
	 */
    void error(const class location& loc, const std::string& msg);

    /** 
	 * @brief General error handling. This can be modified to output the error
     * e.g. to a dialog box. 
	 *
	 * @param[in] msg Message
	 */
    void error(const std::string& msg);
};

} // namespace json

#endif // JSON_DRIVER_H