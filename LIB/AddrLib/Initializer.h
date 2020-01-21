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

#include "CodeConverter.h"
#include "YomiConverter.h"
#include "AreaCodeHelper.h"

namespace addr
{
	/**
	 * @brief	<b>文字付き住所コードコンバータークラスの初期化</b>
	 * @note	文字付き住所を考慮した住所コードが欲しい場合は、必ず最初に呼び出す
	 * @param	lpcszCodeConvDB	[in]	コード変換DB(AccessDBパス)
	 * @return	bool
	 */
	bool Init_CodeConverter( const _TCHAR* lpcszCodeConvDB )
	{
		return CCodeConverter::GetCodeConverter().Init(lpcszCodeConvDB);
	}

	/**
	 * @brief	<b>文字付き住所用ヨミ付与クラスの初期化</b>
	 * @note	文字付き住所のヨミが欲しい場合は、必ず最初に呼び出す
	 * @param	lpcszYomiDB	[in]	ヨミ付与DB(AccessDBパス)
	 * @return	bool
	 */
	bool Init_YomiConverter( const _TCHAR* lpcszYomiDB )
	{
		return CYomiConverter::GetYomiConverter().Init(lpcszYomiDB);
	}

	/**
	 * @brief	<b>住所マスタヘルパークラス初期化関数</b>
	 * @note	住所名称欲しい場合は、必ず最初に呼び出す
	 * @param	lpcszDBProp	[in]	接続プロパティ(デフォルトは、Releaseユーザ)
	 * @return	bool
	 */
	bool Init_AreaCodeHelper( const _TCHAR* lpcszDBProp = _T("release/release@delltcp") )
	{		
		return g_cAreaCodeHelper.Connect(lpcszDBProp);
	}


}
