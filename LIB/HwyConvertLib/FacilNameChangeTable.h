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

#include <string>
#include <map>

/// 変換対象レコードとする条件
struct FacilNameChangeCondition
{
	long			m_RoadCode;
	long			m_RoadSeq;
	std::string		m_FacilName;

	FacilNameChangeCondition(long cRoadCode, long cRoadSeq, std::string cFacilName) :
		m_RoadCode(cRoadCode), m_RoadSeq(cRoadSeq), m_FacilName(cFacilName) { }

	bool operator< (FacilNameChangeCondition rhs) const
	{
		return	(m_RoadCode != rhs.m_RoadCode)	? (m_RoadCode < rhs.m_RoadCode) :
				(m_RoadSeq != rhs.m_RoadSeq)	? (m_RoadSeq < rhs.m_RoadSeq)	:
				(m_FacilName < rhs.m_FacilName);
	}
};

/// 変換後の施設名称
struct ChangedFacilName
{
	std::string		m_Kanji;
	std::string		m_Yomi;

	ChangedFacilName(std::string cKanji, std::string cYomi) :
		m_Kanji(cKanji), m_Yomi(cYomi) {}
};

/// 施設名称変換テーブルファイルの情報格納クラス
class FacilNameChangeTable
{
public:
	FacilNameChangeTable(void);
	FacilNameChangeTable(std::string cFilePath, bool cEnable);
	~FacilNameChangeTable(void);

	std::auto_ptr<ChangedFacilName> getChangedFacilName(long cRoadCode, long cRoadSeq, std::string cFacilName) const;

private:
	void read(std::string cFilePath);

private:
	typedef std::map<FacilNameChangeCondition, ChangedFacilName> Data;
	Data	m_Data;

	bool	m_EnabledToChange;			// 施設名称変換を行うか否か
};
