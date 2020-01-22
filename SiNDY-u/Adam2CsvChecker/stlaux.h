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


#pragma once

// ï∂éöÉRÅ[Éh: UTF-8

//#include "include_stdstl.h"
#include <string>
#include <vector>

std::string form   (const char* format, ...);
std::string vform  (const char* format, va_list ap);
std::string trim   (const std::string& str, char ch = ' ');
std::string trim   (const std::string& str, char right, char left);
std::string replace(const std::string& str, const char* src, const char* dst);
std::string concat (const std::vector<std::string>& v, char delim = ',');
bool match(const char* str, const char* spec);
bool regex(const char* str, const char* spec, bool is_match = true);
void split(const std::string& str, char        delim, std::vector<std::string>& list);
void token(const std::string& str, const char* delim, std::vector<std::string>& list);
std::string getnow(int flag = 1);
std::string charset(const std::string& str, unsigned int src = 0, unsigned int dst = 65001);

std::string  GetNowString (int flag = 1);
std::wstring GetNowWString(int flag = 1);

/*/
struct equ : public std::binary_function<std::string, std::string, bool>
{
	bool operator () (const std::string& str1, const std::string& str2) const { return _stricmp(str1.c_str(), str2.c_str()) < 0; }
};
//*/

template<typename _TContainer>
std::string concat(const _TContainer& c, char ch = ' ') {
	std::string str;
	for (const auto& itr : c)
	{
		if (! str.empty()) { str += ch; }
		str += std::to_string(itr);
	}
	return str;
}
