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


#include "StlAux.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <locale>
#include <regex>
using namespace std;

#if defined(_MSC_VER)
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#else
#  include <string.h>
#  include <time.h>
#  include <sys/time.h>
#endif

class InitLocale {
public:
	InitLocale() {
	#if defined(_MSC_VER)
	#if _MSC_VER < 1900
		setlocale(LC_ALL, "japanese");
	#else
		locale::global(locale("C"));
	#endif
	#endif
		//setlocale(LC_ALL, "ja_JP.UTF-8");
		//locale::global(locale("C"));
	}
};
InitLocale kLocaleInstance;

string form(const char* format, ...)
{
	string ret;
	{
		va_list  ap;
		va_start(ap, format);
		ret = vform(format, ap);
		va_end(ap);
	}
	return ret;
}

string vform(const char* format, va_list ap)
{
	string ret;

#if defined(_MSC_VER)
	int n = _vscprintf(format, ap);
#else
    va_list apc;
    va_copy(apc, ap);
    int n = vsnprintf(0, 0, format, apc);
#endif
	char* buf = new char[n + 1];
	if (buf)
	{
		vsprintf(buf, format, ap);
		ret = buf;
		delete[] buf;
	}
	return ret;
}

string trim(const string& str, char ch)
{
	if (str.length() == 0) { return string(); }

	size_t beg = str.find_first_not_of(ch);
	size_t end = str.find_last_not_of (ch);
	if (beg == string::npos) { return string(); }

	return str.substr(beg, end - beg + 1);
}

string trim(const string& str, char right, char left)
{
	if (str.length() == 0) { return string(); }

	size_t beg = str.find_first_not_of(right);
	size_t end = str.find_last_not_of (left);
	if (beg == string::npos) { return string(); }
	if (beg == end)          { return string(); }

	return str.substr(beg, end - beg + 1);
}

string replace(const string& str, const char* src, const char* dst)
{
	if (! src || strlen(src) == 0) { return str; }
	if (! dst) { dst = ""; }

	string::size_type n = strlen(src);
	string clone = str;
	string::size_type pos;
	while ((pos = clone.find(src)) != string::npos) { clone.replace(pos, n, dst); }

	return clone;
}

string concat(const vector<string>& v, char delim)
{
	string line;
	for (vector<string>::const_iterator it=v.begin() ; it!=v.end() ; ++it)
	{
		if (it != v.begin()) { line += delim; }
		line += *it;
	}
	return line;
}

bool match(const char* str, const char* spec)
{
	switch (*spec)
	{
	case '\0': return (*str == '\0');
	case '*':  return match(str, spec+1) || ((*str != '\0') && match(str+1, spec));
	case '?':  return (*str != '\0' && match(str+1, spec+1));
	default:   return (*str == *spec) && match(str+1, spec+1);
	}
}

bool regex(const char* str, const char* spec, bool is_match)
{
	// NOTE: is_match
	// true:  完全に正規表現に当てはまるもののみ
	// false: 一部でも正規表現に当てはまればOK


	//std::cmatch cm;
	//regex_match(str, cm, regex(spec, std::regex_constants::awk | std::regex_constants::icase));
	if (is_match) { return regex_match (str, std::regex(spec)); }
	else          { return regex_search(str, std::regex(spec)); }
	/**
	 * 正規表現の書き方
	 * ■文字
	 *   .          任意の1文字
	 *   \.         文字としての . →.()[]^$\*?←はエスケープが必要
	 * ■文字クラス
	 *   [A-Za-z]   英字のどれか
	 *   [^abc]     abc以外のどれか
	 *   \d         数字1文字
	 *   \D         数字以外
	 *   \w         英数字とアンダースコア
	 *   \W         英字とアンダースコア以外
	 *   \s         空白1文字 [空白・改行・タブも含む]
	 *   \S         空白以外
	 *   \n         改行     [\x0a] と同じ
	 *   \r         リターン [\x0d] と同じ
	 * ■境界
	 *   ^un        行頭 un  で始まる
	 *   ing$       行末 ing で終わる
	 *   \b         単語の境界と一致 [単語の終わりを意味するようだ...]
	 * ■数量子
	 *   ?          直前の文字が0 or 1個
	 *   *          直前の文字が0個以上
	 *   +          直前の文字が1個以上
	 *   \{n,m}     直前の文字が n個以上 m個以下 (,mは省略可)
	 */
}

void split(const string& str, char delim, vector<string>& list)
{
	list.clear();
	size_t beg = 0;
	while (true)
	{
		size_t end = str.find(delim, beg);
		if (end == string::npos)
		{
			list.push_back(str.substr(beg));
			return;
		}
		list.push_back(str.substr(beg, end-beg));
		beg = end + 1;
	}
}

void token(const string& str, const char* delim, vector<string>& list)
{
	char* p = new char[str.length() + 1];
	strcpy(p, str.c_str());

	char* tok = strtok(p, delim);
	if (tok)
	{
		do {
			list.push_back(tok);
		} while (tok = strtok(NULL, delim));
	}
	delete[] p;
}

std::string charset(const std::string& str, unsigned int src, unsigned int dst)
{
#if defined(_WIN32)
	int n = ::MultiByteToWideChar(src, 0, str.c_str(), -1, NULL, 0);
	vector<wchar_t> wbuf(n * 3 + 1);
	vector<char>    sbuf(n * 3 + 1);

	::MultiByteToWideChar(src, 0, str.c_str(), -1, &wbuf[0], n + 1);
	::WideCharToMultiByte(dst, 0, &wbuf[0], n, &sbuf[0], n, NULL, NULL);

	return string(&sbuf[0]);
#else
	assert(false);
	return str;
#endif
}

string getnow(int flag)
{
#if defined(_MSC_VER)
	SYSTEMTIME lt;
	::GetLocalTime(&lt);

	#define YEAR    lt.wYear
	#define MONTH   lt.wMonth
	#define DAY     lt.wDay
	#define HOUR    lt.wHour
	#define MINUTE  lt.wMinute
	#define SECOND  lt.wSecond
	#define MILLI   lt.wMilliseconds
#else
	struct timeval stv;
	gettimeofday(&stv, NULL);

	struct tm* pn = localtime(&stv.tv_sec);

	#define YEAR    int(pn->tm_year + 1900)
	#define MONTH   int(pn->tm_mon  + 1)
	#define DAY     int(pn->tm_mday)
	#define HOUR    int(pn->tm_hour)
	#define MINUTE  int(pn->tm_min)
	#define SECOND  int(pn->tm_sec)
	#define MILLI   int(stv.tv_usec) / 1000
#endif

	char aTime[64]= {};
	switch (flag) {
		case   1:  sprintf(aTime, "%04d/%02d/%02d,%02d:%02d:%02d:%03d", YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLI); break;
		case   2:  sprintf(aTime, "%04d/%02d/%02d,%02d:%02d:%02d",      YEAR, MONTH, DAY, HOUR, MINUTE, SECOND);        break;
		case   3:  sprintf(aTime, "%04d/%02d/%02d_%02d:%02d:%02d:%03d", YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLI); break;
		case   4:  sprintf(aTime, "%04d/%02d/%02d_%02d:%02d:%02d",      YEAR, MONTH, DAY, HOUR, MINUTE, SECOND);        break;
		case   5:  sprintf(aTime, "%04d/%02d/%02d %02d:%02d:%02d:%03d", YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLI); break;
		case   6:  sprintf(aTime, "%04d/%02d/%02d %02d:%02d:%02d",      YEAR, MONTH, DAY, HOUR, MINUTE, SECOND);        break;

		case  11:  sprintf(aTime, "%04d%02d%02d%02d%02d%02d%03d",       YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLI); break;
		case  12:  sprintf(aTime, "%04d%02d%02d%02d%02d%02d",           YEAR, MONTH, DAY, HOUR, MINUTE, SECOND);        break;
		case  13:  sprintf(aTime, "%04d%02d%02d_%02d%02d%02d%03d",      YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLI); break;
		case  14:  sprintf(aTime, "%04d%02d%02d_%02d%02d%02d",          YEAR, MONTH, DAY, HOUR, MINUTE, SECOND);        break;

		case  -1:  sprintf(aTime, "%04d/%02d/%02d",      YEAR, MONTH, DAY); break;
		case  -2:  sprintf(aTime, "%04d%02d%02d",        YEAR, MONTH, DAY); break;
		case -11:  sprintf(aTime, "%02d:%02d:%02d:%03d", HOUR, MINUTE, SECOND, MILLI); break;
		case -12:  sprintf(aTime, "%02d%02d%02d%03d",    HOUR, MINUTE, SECOND, MILLI); break;
	}
	return string(aTime);
}

std::string  GetNowString (int flag)
{
	return getnow(flag);
}

std::wstring GetNowWString(int flag)
{
	auto str = getnow(flag);
	vector<wchar_t> vw(str.length() * 3 + 1);

	wchar_t* wstr = &vw[0];
	mbstowcs(wstr, str.c_str(), str.length() * 1);

	return wstring(wstr);
}
