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

#ifndef __ENVIRON_H__
#define __ENVIRON_H__

/**
 * @class	CEnviron
 * @brief	環境変数パラメータ
 */
class CEnviron
{
public:
	CString	m_strTPDBProp;			/// 住所系SDE接続プロパティ(TelTPポイント用)
	CString	m_strAddrDBProp;		/// 住所系SDE接続プロパティ(行政界用)
	CString	m_strCityDBProp;		/// 地図系SDE接続プロパティ(都市地図)
	CString	m_strBaseDBProp;		/// 地図系SDE接続プロパティ(中縮)
	CString	m_strTelTPFName;		/// 電話番号検索ポイントフィーチャクラス名
	CString	m_strCityAdminFName;	/// 都市地図行政界フィーチャクラス名
	CString	m_strCitySiteFName;		/// 都市地図背景フィーチャクラス名	
	CString	m_strBaseSiteFName;		/// 中縮背景フィーチャクラス名
	CString m_strPOIDBProp;			/// POI系SDE接続プロパティ(Bug 5701対応 07/11/30追加)
	CString m_strPOIFName;			/// POIフィーチャクラス名(Bug 5701対応 07/11/30追加)
};

#endif
