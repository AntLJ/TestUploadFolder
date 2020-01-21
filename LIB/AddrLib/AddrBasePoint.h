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

#include <SiNDYLib/Feature.h>
#include <SiNDYLib/FeatureClass.h>
#include "CityAdmin.h"

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CAddrBasePoint
 * @brief	各種住所ポイントクラスの親クラス(抽象)
 *          ここで共有処理などをまとめておく
 */
class CAddrBasePoint : public CFeature
{
public:
	CAddrBasePoint(){}
	virtual ~CAddrBasePoint(){ DeleteCache(); }

	/**
	 * @brief	<b>住所コード取得</b>
 	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @return	住所コード
	 */
	virtual CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false ) = 0;

	/**
	 * @brief	<b>住所名称取得</b>
 	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
 	 * @note	※拡張したい場合は、継承先で実装してください。
	 *          ※Initializer.h の Init_AreaCodeHelper関数を呼び出してから使用すること！
	 * @return	住所名称
	 */
	virtual CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>ポイントが属する行政界を取得</b>
	 * @note	NULL時は、号ポイントと同一ワークスペースから検索
	 * @return	都市地図行政界
	 */
	const CCityAdmin* GetCityAdmin();

	/**
	 * @brief	<b>ポイントが属する行政界を取得</b>
 	 * @param	cCityAdminClass	[in]	都市行政界テーブル
	 * @return	都市地図行政界
	 */
	const CCityAdmin* GetCityAdmin( const CFeatureClass& cCityAdminClass );

	/**
	 * @brief	<b>ポイントが属する行政界をセット</b>
	 * @param	cAdminTable	[in]	行政界テーブル
	 * @retval	true  : 取得成功
	 * @retval	false : 取得できない
	 */
	bool SetCityAdmin( const CFeatureClass& cAdminTable );

	/**
	 * @brief	<b>ポイントが属する行政界をセット(行政界直接指定Ver)</b>
	 * @param	cAdmin	[in]	行政界
	 */
	void SetCityAdmin( const CSPCityAdmin& cAdmin );

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * CRow::DeleteCacheをオーバロード
	 */
	virtual void DeleteCache( bool bDeleteContainerCache = false )
	{
		CRow::DeleteCache( bDeleteContainerCache );
		m_strAddrName = _T("");
	}

protected:

	mutable CString m_strAddrName;	//!< 住所名(キャッシュ用)

};

}	// namespace addr
