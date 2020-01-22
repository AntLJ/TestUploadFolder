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
#include "Config.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include "../Json/driver.h"

// string to wstring
std::wstring Config::s2w(const std::string& str)
{
	// the UTF-8 / UCS2 standard conversion facet
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
	try
	{
		std::wstring ucs2 = ucs2conv.from_bytes(str);
		return ucs2;
	}
	catch (const std::range_error&)
	{
		std::wstring ucs2 = ucs2conv.from_bytes(str.substr(0, ucs2conv.converted()));
		std::cout << "UCS2 failed after producing " << std::dec << ucs2.size() << " characters:\n";
		for (wchar_t c : ucs2)
			std::cout << std::hex << std::showbase << c << '\n';
		return std::wstring();
	}
}

// wstring to string
std::string Config::w2s(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
	try
	{
		std::string mbs = ucs2conv.to_bytes(str);
		return mbs;
	}
	catch (const std::range_error&)
	{
		std::string mbs = ucs2conv.to_bytes(str.substr(0, ucs2conv.converted()));
		std::cout << "UCS2 failed after producing " << std::dec << mbs.size() << " characters:\n";
		for (char c : mbs)
			std::cout << std::hex << std::showbase << c << '\n';
		return std::string();
	}
}

const json::JsonValuePtr Config::GetValue(const json::JsonObjectPtr pObject, const std::wstring& strName)
{
	return (*pObject)[w2s(strName)];
}

const json::JsonValuePtr Config::GetValue(const json::JsonArrayPtr pArray, size_t n)
{
	return (*pArray)[n];
}

bool Config::GetString(const json::JsonValuePtr pValue, std::wstring& str)
{
	if (pValue && typeid(*pValue) == typeid(json::JsonString))
	{
		str = s2w(*std::dynamic_pointer_cast<json::JsonString>(pValue));
		return true;
	}
	return false;
}

bool Config::GetInt(const json::JsonValuePtr pValue, int& i)
{
	if (pValue && typeid(*pValue) == typeid(json::JsonInteger))
	{
		i = *std::dynamic_pointer_cast<json::JsonInteger>(pValue);
		return true;
	}
	return false;
}

bool Config::GetDouble(const json::JsonValuePtr pValue, double& d)
{
	if (pValue && typeid(*pValue) == typeid(json::JsonDouble))
	{
		d = *std::dynamic_pointer_cast<json::JsonDouble>(pValue);
		return true;
	}
	return false;
}

bool Config::GetLongLong(const json::JsonValuePtr pValue, long long& ll)
{
	if (pValue && typeid(*pValue) == typeid(json::JsonLongInteger))
	{
		ll = *std::dynamic_pointer_cast<json::JsonLongInteger>(pValue);
		return true;
	}
	return false;
}

bool Config::GetBool(const json::JsonValuePtr pValue, bool& b)
{
	if (pValue && typeid(*pValue) == typeid(json::JsonBool))
	{
		b = *std::dynamic_pointer_cast<json::JsonBool>(pValue);
		return true;
	}
	return false;
}

std::wstring Config::Get(const json::JsonValuePtr pValue, const std::wstring& strDefault)
{
	std::wstring strValue(strDefault);
	GetString(pValue, strValue);
	return strValue;
}

int Config::Get(const json::JsonValuePtr pValue, int nDefault)
{
	int nValue(nDefault);
	GetInt(pValue, nValue);
	return nValue;
}

double Config::Get(const json::JsonValuePtr pValue, double dDefault)
{
	double dValue(dDefault);
	GetDouble(pValue, dValue);
	return dValue;
}

long long Config::Get(const json::JsonValuePtr pValue, long long llDefault)
{
	long long llValue(llDefault);
	GetLongLong(pValue, llValue);
	return llValue;
}

bool Config::Get(const json::JsonValuePtr pValue, bool bDefault)
{
	bool bValue(bDefault);
	GetBool(pValue, bValue);
	return bValue;
}

bool Config::Load(const std::wstring& strName)
{
	std::string str = w2s(strName);
	json::Driver driver;
	std::ifstream infile(str);

	if (!infile.good())
		return false;

	if (!driver.parse_stream(infile, str))
	{
		return false;
	}
	if (typeid(*driver.root) != typeid(json::JsonObject))
	{
		std::wcerr << L"Contents of configure file error" << std::endl;
		return false;
	}

	json::JsonObjectPtr pObject = std::dynamic_pointer_cast<json::JsonObject>(driver.root);
#ifdef TEST
	pObject->Print(std::cout, 0);
#endif
	return OnLoad(pObject);
}

// ////////////////////////////////////////////////////////////////////////////////////
//
// This is a sample of Configure
bool AutoSnapConfig::OnLoad(json::JsonObjectPtr pObject)
{
	type = Type(Get(GetValue(pObject, L"type"), 1));

	radius = Get(GetValue(pObject, L"radius"), 0.0000005);
	log = Get(GetValue(pObject, L"log"), std::wstring(L"log.txt"));
	mesh_list = Get(GetValue(pObject, L"mesh_list"), std::wstring(L"mesh_list.txt"));

	snap = Get(GetValue(pObject, L"snap"), std::wstring(L"output.txt"));
	snap_layer = Get(GetValue(pObject, L"snap_layer"), std::wstring(L"snap_layer.txt"));
	lot = Get(GetValue(pObject, L"lot"), std::wstring(L"lot"));
	mesh = Get(GetValue(pObject, L"mesh"), std::wstring(L".\\Data"));
	mesh_name = Get(GetValue(pObject, L"mesh_name"), std::wstring(L"CITYMESH"));

	snap_format = Get(GetValue(pObject, L"snap_format"), std::wstring(L"%08d.log"));
	log_format = Get(GetValue(pObject, L"log_format"), std::wstring(L"%08d_snap.log"));
	
	std::transform(mesh_name.begin(), mesh_name.end(), mesh_name.begin(), towupper);

	json::JsonValuePtr pValue = GetValue(pObject, std::wstring(L"mini_length"));
	json::JsonObjectPtr pMin = std::dynamic_pointer_cast<json::JsonObject>(pValue);
	if (pMin)
	{
		const json::JsonObject::RawData& data = *pMin;
		for (const auto& p : data)
		{
			std::wstring str = s2w(p.first);
			//Store uppercase string
			std::transform(str.begin(), str.end(), str.begin(), ::towupper);
			mini_list[str] = Get(p.second, 0.0000005);
		}
	}

	if(type == T_MULTI_FGDB || type == T_MULTI_LOG)
	{
		//Verify FGDB path
		DWORD attr = GetFileAttributesW(lot.c_str());
		//Checking the path
		if(attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::wcerr << L"Path is invalid: " << lot << std::endl;
			return false;

		}

		//Append the slash
		if(*lot.rbegin() != L'\\')
		{
			lot += L'\\';
		}
	}
	if(type == T_MULTI_LOG)
	{
		//Verify Snap log path
		DWORD attr = GetFileAttributesW(snap.c_str());
		//Checking the path
		if(attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::wcerr << L"Path is invalid: " << snap << std::endl;
			return false;

		}

		//Append the slash
		if(*snap.rbegin() != L'\\')
		{
			snap += L'\\';
		}


		//Verify Log Path
		attr = GetFileAttributesW(log.c_str());
		//Checking the path
		if(attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::wcerr << L"Path is invalid: " << log << std::endl;
			return false;

		}

		//Append the slash
		if(*log.rbegin() != L'\\')
		{
			log += L'\\';
		}

	}

	return true;
}
