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
#include "FileTable.h"

#include <regex>

using namespace std;

//Get uncommented string
//Extract all characters before comment "#"
//e.g. "aaa bbb#ccc ddd"->"aaa bbb"
const wregex rgxComment(L"^[^#]+");

//Split string with tab
//e.g. "\taaa\t\tbbb\t"->"aaa","bbb"
const wregex rgxSplit(L"[^\\t]+");

//Separator for output
const wchar_t szSeparator[] = L"\t";

CFileTable::~CFileTable(void)
{
	//Try to close the file
	Close();
}

bool CFileTable::Open(const std::wstring& strName, bool isRead)
{
	if(isRead)
	{
		//Read mode
		m_stream.open(strName, ios_base::in);
		return m_stream.good();
	}
	else
	{
		//Write mode
		m_stream.open(strName, ios_base::out);
		return m_stream.good();
	}
}

void CFileTable::Close()
{
	if(m_stream.is_open())
	{
		m_stream.close();
	}
}

size_t CFileTable::Read(Records& rds)
{
	Record rd;
	size_t count = 0;

	//Read all records until EOF
	while(ReadRecord(rd))
	{
		//rds.push_back(rd);
		rds.push_back(std::move(rd));
		++count;
		//rd.clear();
	}

	return count;
}

bool CFileTable::Read(Record& rd)
{
	return ReadRecord(rd);
}

bool CFileTable::ReadLine(std::wstring& str)
{
	//File is not opened or EOF or other error states
	if(m_stream.good())
	{
		getline(m_stream, str);
		return true;
	}
	return false;
}

//Process string via std regular express
bool CFileTable::ReadRecord(Record& rd)
{
	//Line from file
	wstring strLine;
	//Read lines until the line is valid or end of file
	while(ReadLine(strLine))
	{
		wsmatch matchLine;
		//Get the first matched string, and only can be caught one result
		if(regex_search(strLine, matchLine, rgxComment))
		{
			//It is only one result with the regular express
			//Maybe do not need to check this
			if(!matchLine.empty())
			{
				//Clear all contents
				rd.clear();
				//Get the first sub match (the uncommented string until the first comment character)
				strLine = matchLine[0].str();
				wsmatch matchField;
				//Get all matched result (fields)
				while(regex_search(strLine, matchField, rgxSplit))
				{
					//It only can be one result with the regular express
					if(!matchField.empty())
					{
						rd.push_back(matchField[0].str());
					}
					strLine = matchField.suffix().str();
				}
				//Fields exist
				if(!rd.empty())
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CFileTable::Write(const Record& rd)
{
	std::wstring strLine;

	for(Record::const_iterator it = rd.begin(); it != rd.end(); ++it)
	{
		if(it != rd.begin())
		{
			strLine += szSeparator;
		}
		strLine += *it;
	}

	return Write(strLine);
}

bool CFileTable::Write(const Records& rds)
{
	for(const auto& rd : rds)
	{
		if(!Write(rd))
		{
			return false;
		}
	}
	return true;
}

bool CFileTable::Write(const std::wstring& str)
{
	m_stream << str << endl;
	return m_stream.good();
}

bool CFileTable::Write(const wchar_t* str)
{
	m_stream << str << endl;
	return m_stream.good();
}
