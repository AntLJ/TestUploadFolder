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

#include "stdafx.h"
#include <regex>
#include "Util.h"


bool util::isInteger(const std::string & str)
{
	if(str.empty()) 
		return true;

	return std::regex_match(str, std::regex("^-?([1-9][0-9]*|0)$"));
}

bool util::isInteger(const CString& str)
{
	std::string astr = CT2A(str.GetString());
	return isInteger(astr);
}

bool util::isInteger(const CComVariant& val)
{
	CString str = uh::str_util::ToString(val);
	return isInteger(str);
}

bool util::isFloat(const std::string & str)
{
	if(str.empty()) 
		return true;

	return std::regex_match(str, std::regex("^-?[0-9]+\.[0-9]+$"));
}

bool util::isFloat(const CString& str)
{
	std::string astr = CT2A(str.GetString());
	return isFloat(astr);
}

bool util::isFloat(const CComVariant & val)
{
	CString str = uh::str_util::ToString(val);
	return util::isFloat(str);
}

bool util::isNumericType(const CComVariant & val)
{
	switch (val.vt)
	{
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_R4:
	case VT_R8:
	case VT_INT:
	case VT_UINT:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
		return true;

	default:
		break;
	}

	return false;
}

bool util::isIntegerType(const CComVariant & val)
{
	switch (val.vt)
	{
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_INT:
	case VT_UINT:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
		return true;

	default:
		break;
	}

	return false;
}