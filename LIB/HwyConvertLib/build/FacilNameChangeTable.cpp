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

#include "FacilNameChangeTable.h"
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

FacilNameChangeTable::FacilNameChangeTable(void)
{
}

FacilNameChangeTable::FacilNameChangeTable(std::string cFilePath, bool cEnable) :
m_EnabledToChange(cEnable)
{
	read(cFilePath);
}


FacilNameChangeTable::~FacilNameChangeTable(void)
{
}

/**
 * @brief 施設名称変換テーブルファイルの読み込み
 * @param	cFilePath	[in]	入力ファイルパス
 **/
void FacilNameChangeTable::read(std::string cFilePath)
{
	using namespace std;
	using namespace boost;

	if (! m_EnabledToChange) {
		return;
	}

	ifstream aFin(cFilePath.c_str());
	if (!aFin)
	{
		throw runtime_error(string("Error") + "\t" + "入力ファイル " + lexical_cast<string>(cFilePath) + "を開けません。");
	}

	string aLineBuff;
	while (getline(aFin, aLineBuff))
	{
		tokenizer<char_separator<char> > aToken(aLineBuff, char_separator<char>("\t"));
		vector<string> aRecord;
		copy(aToken.begin(), aToken.end(), back_inserter(aRecord));
		if (aRecord.size() != 5)
		{
			throw runtime_error(string("Error") + "\t" + "施設名称変換テーブルファイルのフィールド数が不正");
		}

		m_Data.insert(
			make_pair(
				FacilNameChangeCondition(
					atoi(aRecord[0].c_str()),
					atoi(aRecord[1].c_str()),
					aRecord[2]
				),
				ChangedFacilName(aRecord[3], aRecord[4])
			)
		);
	}
}

/**
 * @brief 与えられた条件（変換前のRoadCode, RoadSeq, 施設名称）に対応する、変換後の施設名称を返す
 * @param	cRoadCode	[in]	変換前の路線コード
 * @param	cRoadSeq	[in]	変換前の路線内シーケンス番号
 * @param	cFacilName	[in]	変換前の施設名称
 * @return	変換後の施設名称
 * @retval	NULL	変換対象ではない場合はNULLを返す
 **/
std::auto_ptr<ChangedFacilName> FacilNameChangeTable::getChangedFacilName(long cRoadCode, long cRoadSeq, std::string cFacilName) const
{
	if (! m_EnabledToChange) {
		return std::auto_ptr<ChangedFacilName>();
	}

	Data::const_iterator aRecordIt = m_Data.find(FacilNameChangeCondition(cRoadCode, cRoadSeq, cFacilName));
	if (aRecordIt != m_Data.end())
	{
		return std::auto_ptr<ChangedFacilName>(
			new ChangedFacilName(
				aRecordIt->second.m_Kanji,
				aRecordIt->second.m_Yomi
			)
		);
	}

	return std::auto_ptr<ChangedFacilName>();
}


