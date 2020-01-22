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

#include "ConvertTable.h"

/**
 * @brief レコードチェック用
 */
class CBaseRow
{
public:
	CBaseRow(){}
	virtual ~CBaseRow(){}
	
protected:
	/**
	 * @brief エラー出力用ID取得関数
	 * @param cConvTableInfo [in] 変換テーブル情報
	 * @param ipRow [in] レコード
	 * @return ID文字列
	 */
	CString getID(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow);
};

