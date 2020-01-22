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

#include "stlaux.h"
#include <set>
#include <regex>
#include <limits>

//typedef long        int_t;
//typedef double      float_t;

static void fChangeType(const std::string& v, long&        lv) { lv = atol(v.c_str()); }
static void fChangeType(const std::string& v, double&      dv) { dv = atof(v.c_str()); }
static void fChangeType(const std::string& v, std::string& sv) { sv = v;               }

class IChecker {
public:
	virtual ~IChecker() {}
	IChecker() {}

	virtual bool isSafe(const std::string& v) const = 0;
};

class CFixedFormatChecker : public IChecker {
public:
	~CFixedFormatChecker() {}
	CFixedFormatChecker (const char* fixed_value) : fixed_value(fixed_value) {}

	inline bool isSafe(const std::string& v) const {
		return (v == fixed_value);
	}

protected:
	const std::string fixed_value;
};

template<typename _Typ>
class CRangeChecker : public IChecker {
public:
	~CRangeChecker() {}
	CRangeChecker (const _Typ& min_, const _Typ& max_) : min_(min_), max_(max_) {}

	virtual bool isSafe(const std::string& v) const {
		_Typ tv = _Typ();
		fChangeType(v, tv);

		return (min_ <= tv && tv <= max_);
	}

private:
	const _Typ  min_;
	const _Typ  max_;
};

template<typename _Typ>
class CSpRngChecker : public IChecker {
public:
	~CSpRngChecker() {}
	CSpRngChecker (const _Typ& min_, const _Typ& max_, const std::initializer_list<_Typ>& list) : min_(min_), max_(max_) {
		for (const auto& itr : list) {
			if (! dic.insert(itr).second) { assert(false); }
		}
	}

	virtual bool isSafe(const std::string& v) const {
		_Typ tv = _Typ();
		fChangeType(v, tv);

		if (dic.find(tv) != dic.end()) { return true; }
		return (min_ <= tv && tv <= max_);
	}

private:
	const _Typ  min_;
	const _Typ  max_;
	std::set<_Typ>  dic;
};

template<typename _Typ>
class CListChecker : public IChecker {
public:
	~CListChecker() {}
	CListChecker (const std::initializer_list<_Typ>& list) {
		for (const auto& itr : list) {
			if (! dic.insert(itr).second) { assert(false); }
		}
	}

	virtual bool isSafe(const std::string& v) const {
		_Typ tv = _Typ();
		fChangeType(v, tv);

		return (dic.find(tv) != dic.end());
	}

private:
	std::set<_Typ>  dic;
};

class CFlagChecker : public CListChecker<long> {
public:
	~CFlagChecker() {}
	CFlagChecker () : CListChecker({0, 1}) {}
};

class CFormatChecker : public IChecker {
public:
	virtual ~CFormatChecker() {}
	CFormatChecker(bool nullable = false) : nullable(nullable) {}

	inline bool isSafe(const std::string& v) const {
		if (nullable && v.empty()) { return true; }
		return std::regex_match(v, std::regex(format.c_str()));
	}

protected:
	std::string format;
	bool        nullable;
};

class CNumberFormatChecker : public CFormatChecker {
public:
	~CNumberFormatChecker() {}
	CNumberFormatChecker (int int_digits, int flt_digits, bool nullable = false) : CFormatChecker(nullable) {
		format = "^[-]?";                           // êÊí[:ïÑçÜ
		format += form("\\d{1,%d}", int_digits);    // êÆêîÇÃåÖêî
		if (flt_digits > 0) {
			format += form("[\\.]?\\d{%d}", flt_digits);
			//format += form("([\\.]?\\d{1,%d})?", flt_digits);
		}
		format += "$";                              // èIí[
	}
};

class CStringFormatChecker : public CFormatChecker {
public:
	~CStringFormatChecker() {}
	CStringFormatChecker (int digits, bool nullable = false) : CFormatChecker(nullable) {
		format = form("^.{1,%d}$", digits);
	}
};

class CRegexFormatChecker : public CFormatChecker {
public:
	~CRegexFormatChecker() {}
	CRegexFormatChecker (const std::string& spec, bool nullable = false) : CFormatChecker(nullable) {
		format = spec;
	}
};


class CNoneChecker : public IChecker {
public:
	~CNoneChecker() {}
	CNoneChecker () {}

	inline bool isSafe(const std::string& ) const { return true; }
};
