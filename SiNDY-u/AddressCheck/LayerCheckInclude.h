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

/**
 * @file	LayerCheckInclude.h
 * @brief	各レイヤヘッダーファイルインクルードするファイル
 * ここに各レイヤのヘッダーファイルをインクルードする
 */

#pragma once

#include ".\AddrPolygonCheck\AddrPolygonCheck.h"
#include ".\CityPolygonCheck\CityPolygonCheck.h"
/**
 * @brief	チェックレイヤインスタンス取得関数
 * @param	[in] layer			レイヤ名
 * @return	チェックレイヤ
 */
static shared_ptr<LayerCheckBase> setLayerCheckInstance( const CString& layer )
{
	try
	{
		// コンポーネント追加されたら以下にじゃんじゃん追加
		if( addr_polygon::kTableName == layer )         return shared_ptr<LayerCheckBase>( new AddrPolygonCheck() );
		if( city_polygon::kTableName == layer )         return shared_ptr<LayerCheckBase>( new CityPolygonCheck() );
	}
	catch( bad_alloc )
	{
		return nullptr;	// new が失敗したとき、落ちないように
	}

	return nullptr;
}
