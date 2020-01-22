/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include <fstream>
#include <sstream>

#include "driver.h"

namespace json {

Driver::Driver()
    : trace_scanning(false),
      trace_parsing(false)
{
}

bool Driver::parse_stream(std::istream& in, const std::string& sname)
{
    streamname = sname;
	Scanner lexer(&in);
    lexer.set_debug(trace_scanning);

	Parser parser(*this, lexer);
    parser.set_debug_level(trace_parsing);
    return (parser.parse() == 0);
}

bool Driver::parse_file(const std::string &filename)
{
    std::ifstream in(filename.c_str());
    if (!in.good()) return false;
    return parse_stream(in, filename);
}

bool Driver::parse_string(const std::string &input, const std::string& sname)
{
    std::istringstream iss(input);
    return parse_stream(iss, sname);
}

void Driver::error(const class location& loc,const std::string& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

void Driver::error(const std::string& msg)
{
    std::cerr << msg << std::endl;
}

} // namespace json
