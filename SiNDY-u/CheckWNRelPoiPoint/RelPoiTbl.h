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

#include "stdafx.h"
#include "Log.h"

class CRelPoiTbl
{
public:
	CRelPoiTbl(void);
	~CRelPoiTbl(void);

	/**
	 * @brief	道路・歩行者ノード-POI関連テーブル読み込み
	 * @param	sTblPath	[in]	POI関連テーブルパス
	 * @param	relTbl		[out]	道路・歩行者ノード-POI関連テーブル
	 * @return						処理成否
	 */
	bool readRelTbl(std::string sTblPath, MAP_REL_TBL &relTbl);
};


