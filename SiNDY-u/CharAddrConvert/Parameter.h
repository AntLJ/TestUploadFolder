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
 * @file	Parameter.h
 * @brief	設定情報格納構造体定義
 */

#include <string>

/**
 * @enum	emConvMode
 * @brief	変換モード
 */
enum emConvMode
{
	kNormalConv		= 1,	//!< ナビ向け
	kOutsideConv	= 2		//!< 外部提供向け(Bug7599)
};

/**
 * @struct	CParameter
 * @brief	設定情報
 */
struct CParameter
{
	emConvMode		m_emConvMode;			//!< 変換モード
	std::string		m_strAddrDBProp;		//!< 住所系SDE接続プロパティ
	std::string		m_strMapDBProp;			//!< 住所系SDE接続プロパティ

	std::string		m_strCityAdminFCName;	//!< 都市地図行政界フィーチャクラス名
	std::string		m_strGouPointFCName;	//!< 号ポイントフィーチャクラス名
	std::string		m_strCitySiteFCName;	//!< 都市地図背景フィーチャクラス名
	std::string		m_strBuildingFCName;	//!< 都市建物フィーチャクラス名
	std::string		m_strBuildingRoofFCName;	//!< 都市建物屋上フィーチャクラス名
	std::string		m_strCityMeshFCName;	//!< 都市地図メッシュフィーチャクラス名

	std::string		m_strCodeConvDB;		//!< 住所コード変換DB
	std::string		m_strYomiConvDB;		//!< ヨミ仮名変換DB

	std::string		m_strJusyoDB;			//!< 住所DB接続プロパティ

	std::string		m_strPntDir;			//!< pntexデータ出力先
	std::string		m_strGouDir;			//!< gouexデータ出力先

	CParameter() : m_emConvMode(kNormalConv){}
};
