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

#include "diffshapeadd.h"

#include <DBFHandle.h>
#include <SHPHandle.h>
#include <SHPObject.h>

class CDiffAdd {
public:
	CDiffAdd(){}
	CDiffAdd(CInputData& cInputData){ m_cInputData = cInputData; }
	~CDiffAdd(){}

public:
	bool run();

private:
	// リストファイルから追加用データを取得する
	// データ構造はm_mapAddList
	bool get_diffaddlist();

private:
	CInputData m_cInputData;

	std::map<CString,std::set<long>> m_mapAddList;		//!< メッシュとFIDリストのマップ
};
