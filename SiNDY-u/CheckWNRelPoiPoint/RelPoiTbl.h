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
	 * @brief	���H�E���s�҃m�[�h-POI�֘A�e�[�u���ǂݍ���
	 * @param	sTblPath	[in]	POI�֘A�e�[�u���p�X
	 * @param	relTbl		[out]	���H�E���s�҃m�[�h-POI�֘A�e�[�u��
	 * @return						��������
	 */
	bool readRelTbl(std::string sTblPath, MAP_REL_TBL &relTbl);
};


