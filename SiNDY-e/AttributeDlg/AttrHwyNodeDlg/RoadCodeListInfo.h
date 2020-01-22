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
#include <set>

// Road_Code_Listから取得した情報を格納する構造体
struct RoadCodeListRecord
{
	long		m_ObjectID;		///< オブジェクトID
	long		m_RoadCode;		///< 路線コード
	std::string	m_NameKanji;	///< 路線名称漢字
	std::string	m_NameYomi;		///< 路線名称読み
	long		m_LineClass;	///< 路線種別コード
};

// 施設名称読み＋路線コードでRoadCodeListRecordをソートするためのファンクタ
struct lessYomiAndRoadCode : public std::binary_function<RoadCodeListRecord, RoadCodeListRecord, bool>
{
	bool operator() (const RoadCodeListRecord& lhs, const RoadCodeListRecord& rhs) const
	{
		return
			lhs.m_NameYomi != rhs.m_NameYomi ? lhs.m_NameYomi < rhs.m_NameYomi :
											   lhs.m_RoadCode < rhs.m_RoadCode;
	}
};

// RoadCodeListRecordを格納するコンテナ
typedef std::set<RoadCodeListRecord, lessYomiAndRoadCode> RoadCodeListArray;

