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

#include <map>
#include <cassert>

#include <atlcomcli.h>  // CComVariant
#include <atlstr.h>


class CDataChecker;

struct Position {
	double  lon;
	double  lat;
	double  alt;

	explicit Position(const std::vector<std::string>& arr) {
		assert(arr.size() >= 3);
		lon = atof(arr[0].c_str());
		lat = atof(arr[1].c_str());
		alt = atof(arr[2].c_str());
	}

	Position(std::initializer_list<double> list) {
		auto f = list.begin();
		lon = *f; ++f;
		lat = *f; ++f;
		alt = *f;
	}

	bool operator < (const Position& rhs) const {
		if (lon < rhs.lon) { return true;  }
		if (lon > rhs.lon) { return false; }
		if (lat < rhs.lat) { return true;  }
		if (lat > rhs.lat) { return false; }
		if (alt < rhs.alt) { return true;  }
		if (alt > rhs.alt) { return false; }

		return false;
	}

	bool operator != (const Position& rhs) const {
		if (lon != rhs.lon) { return true; }
		if (lat != rhs.lat) { return true; }
		if (alt != rhs.alt) { return true; }
		return false;
	}
	bool operator == (const Position& rhs) const {
		return (! (operator != (rhs)));
	}

	inline std::string toString(char delim = ',') const {
		return form("(%3.8lf%c%3.8lf%c%3.8lf)", lon, delim, lat, delim, alt);
	}

	bool equal2(const Position& rhs) const {
		if (lon != rhs.lon) { return false; }
		if (lat != rhs.lat) { return false; }
		return true;
	}
	bool equal3(const Position& rhs) const {
		if (lon != rhs.lon) { return false; }
		if (lat != rhs.lat) { return false; }
		if (alt != rhs.alt) { return false; }
		return true;
	}

public:
	// XY2é≤
	Position XY() const {
		Position p = *this;
		p.alt = 0;
		return p;
	}

};
struct lessPointArr : public std::binary_function<std::vector<Position>, std::vector<Position>, bool> {
	bool operator() (const std::vector<Position>& lhs, const std::vector<Position>& rhs) const {
		if (lhs.size() != rhs.size()) { assert(false); }

		size_t N = min(lhs.size(), rhs.size());
		for (size_t i=0 ; i<N ; i++)
		{
			if (lhs[i].lon < rhs[i].lon) { return true;  }
			if (lhs[i].lon > rhs[i].lon) { return false; }
			if (dimension >= 2) {
				if (lhs[i].lat < rhs[i].lat) { return true;  }
				if (lhs[i].lat > rhs[i].lat) { return false; }
			}
			if (dimension >= 3) {
				if (lhs[i].alt < rhs[i].alt) { return true;  }
				if (lhs[i].alt > rhs[i].alt) { return false; }
			}
		}

		return false;
	}

	static int dimension;
};


struct Row {
	std::vector<std::string>  attr;
};

struct Feature : public Row {
	std::vector<Position>     shape;
};

static const char kDELIMITER  = ',';

static const int kFEATURE_OID = 3;
static const int kTABLE_OID   = 0;

template<long index> long   GetLong  (const std::vector<std::string>& arr) { return atol(arr[index].c_str()); }
template<long index> double GetDouble(const std::vector<std::string>& arr) { return atof(arr[index].c_str()); }
template<long index> long   GetLong  (const Row& feat) { return atol(feat.attr[index].c_str()); }
template<long index> double GetDouble(const Row& feat) { return atof(feat.attr[index].c_str()); }


template<long index> CComVariant GetVariant(const Row& feat) {
	CComVariant v(feat.attr[index].c_str());
	if (feat.attr[index].empty()) { v.vt = VT_NULL; }
	return v;
}

template<typename _Typ> bool fLoad(const TCHAR* fname, const CDataChecker* checker, std::vector<_Typ>& list);

// CheckerÉNÉâÉXÇ…éùÇΩÇπÇΩï˚Ç™ó«Ç¢Ç¡Ç€ÅH...
bool fIsEqualAttr(const Feature& lhs, const Feature& rhs, const std::vector<long>& indices);

bool fIsEqualPosition3D(const Feature& lhs, const Feature& rhs, bool is_warning = true);
bool fIsEqualPosition2D(const Feature& lhs, const Feature& rhs, bool is_warning = true);

#if defined(_CHECK_GEOMETRY_2D_)
#define fIsEqualPosition fIsEqualPosition2D
#else
#define fIsEqualPosition fIsEqualPosition3D
#endif

double fCalcHubeny(const Position& lhs, const Position& rhs, double& distx, double& disty);
double fCalcHubeny(const Position& lhs, const Position& rhs);

template<typename _Typ> bool fBuildDic(const std::vector<_Typ>& arr, long index[2], std::map<long, std::map<long, const _Typ*> >& dic);
template<typename _Typ> bool fBuildDic(const std::vector<_Typ>& arr, long index, std::map<long, const _Typ*>& dic, bool output_log = true);
template<typename _Typ> bool fBuildDic(const std::vector<_Typ>& arr, long index, std::map<long, std::vector<const _Typ*> >& confs);

