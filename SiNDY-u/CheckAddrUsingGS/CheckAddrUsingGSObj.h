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

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include "ToolBase.h"
#include <oo4o/include/oracl.h>
#pragma comment( lib, "../../LIB/oo4o/lib/ORACLM32.LIB" )

/**
 * @brief  ログ出力情報クラス
 */
class CLogInfo
{
public:

	/**
	 * @brief  コンストラクタ
	 */
	CLogInfo() : e_lOID(0) { }

	/**
	 * @brief  コンストラクタ
	 * @param  lOID [in] オブジェクトID
	 */
	CLogInfo( long lOID ) : e_lOID( lOID ) { }

	/**
	 * @brief  デストラクタ
	 */
	~CLogInfo() { }

	long	e_lOID;				//!< オブジェクトID
	CString	e_strAddrName;		//!< 住所名称（iPC）
	CString	e_strAddrNameGeo;	//!< 住所名称（GEOSPACE）
};

/**
 * @brief  GSデータによる住所妥当性チェッククラス
 */
class CCheckAddrUsingGSObj : public CToolBase
{
public:

	/**
	 * @brief  コンストラクタ
	 */
	CCheckAddrUsingGSObj(void);

	/**
	 * @brief  デストラクタ
	 */
	~CCheckAddrUsingGSObj(void);

	/**
	 * @brief  初期化
	 * @param  argc [in]  コマンドライン引数個数
	 * @param  argv [in]  コマンドライン引数文字列
	 * @return bool 正常に初期化できたかどうか
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief  実行
	 * @return bool 正常に処理を実行できたかどうか
	 */
	bool Execute();

private:

	/**
	 * @brief  ログヘッダの出力
	 * @note   親クラスの関数にオーバライド
	 */
	void WriteLogHeader();

	/**
	 * @brief  引数チェック
	 * @param  argc [in]  コマンドライン引数個数
	 * @param  argv [in]  コマンドライン引数文字列
	 * @return bool 引数を正常に解析できたかどうか
	 */
	bool CheckArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief  入力ファイルオープン
	 * @return bool 入力ファイルを正常に読み込めたかどうか
	 */
	bool OpenInputFile();

	/**
	 * @brief  タブ区切りのレコードをフィールドに分解
	 * @param  strBuf    [in]  レコードバッファ
	 * @param  vecFields [out] フィールド群
	 */
	void Split(const CString& strBuf, std::vector< CString >& vecFields );

	/**
	 * @brief  ログの出力
	 * @param  cLogInfo [in] ログ情報
	 * @param  strMsg   [in] メッセージ
	 */
	void WriteLogInfo( const CLogInfo& cLogInfo, const CString& strMsg );

	/**
	 * @brief  住所DBへの接続
	 * @param  strConnect [in]  DB接続文字列
	 * @param  cDB        [out] 接続先DBソース
	 * @return 正常に接続できたかどうか
	 */
	bool ConnectJDB( const CString& strConnect, ODatabase& cDB );

	/**
	 * @brief  ポイントを含むポリゴンを検索
	 * @param  ipPoint        [in]  ポイント
	 * @param  ipFeatureClass [in]  検索対象のフィーチャクラス
	 * @return ポリゴンフィーチャ
	 */
	IFeaturePtr SearchPolygon( IPointPtr ipPoint, sindy::CFeatureClass ipFeatureClass );

	/**
	 * @brief  ポイントフィーチャを含む都市行政界フィーチャを検索
	 * @param  ipF_Point [in]  ポイントフィーチャ
	 * @return 都市行政界フィーチャ
	 */
	IFeaturePtr SearchAdmin( IFeaturePtr ipF_Point );

	/**
	 * @brief  ポイントフィーチャを含む家形フィーチャを検索
	 * @param  ipF_Point [in]  ポイントフィーチャ
	 * @return 家形フィーチャ
	 */
	IFeaturePtr SearchBuilding( IFeaturePtr ipF_Point );

	/**
	 * @brief  当該GeospaceIDを持つGEOSPACE家形フィーチャを検索
	 * @param  strGeospaceID [in]  GeospaceID
	 * @return GEOSPACE家形フィーチャ
	 */
	IFeaturePtr SearchBuildingGeo( const CString& strGeospaceID );

	/**
	 * @brief  11桁住所コードを国地協コードに変換
	 * @param  strAddrCode [in] 11桁住所コード（加除ベース）
	 * @return 11桁住所コード（国地協）
	 */
	CString ConvAddrCode2JGDC( const CString& strAddrCode );

	/**
	 * @brief  11桁住所コードから住所文字列を取得
	 * @param  strAddrCode [in] 11桁住所コード（加除ベース）
	 * @return 住所文字列
	 */
	CString GetAddrName( const CString& strAddrCode );

private:

	CString			m_strConnectAdr;	//!< SDE接続文字列（住所）
	CString			m_strConnectBld;	//!< SDE接続文字列（家形）
	CString			m_strConnectGeo;	//!< SDE接続文字列（GEOSPACE）
	CString			m_strConnectJDB;	//!< 住所DB接続文字列（住所文字列取得）
	CString			m_strConnectJGDC;	//!< 住所DB接続文字列（加除→国地協）
	CString			m_strLogInput;		//!< 入力ログファイル名
	CString			m_strLogOutput;		//!< 出力ログファイル名

	ODatabase		m_cDBSourceJDB;		//!< DBソース（住所文字列取得）
	ODatabase		m_cDBSourceJGDC;	//!< DBソース（加除→国地協）

	std::list<int>	m_listOID_GouPoint;	//!< チェック対象OIDリスト（号ポイント）

	std::map<CString, CString>	m_mapAddrName;		//!< 住所文字列取得用キャッシュ
	std::map<CString, CString>	m_mapConvAddr;		//!< 11桁住所コード変換用キャッシュ（加除→国地協）

	sindy::CFeatureClass	m_cFCGouPoint;			//!< 号ポイントフィーチャクラス
	sindy::CFeatureClass	m_cFCCityAdmin;			//!< 都市行政界フィーチャクラス
	sindy::CFeatureClass	m_cFCBuilding;			//!< 家形ポリゴンフィーチャクラス
	sindy::CFeatureClass	m_cFCBuildingGeo;		//!< GEOSPACE家形ポリゴンフィーチャクラス

	sindy::CSPFieldMap		m_cFMGouPoint;			//!< 号ポイントフィールドマップ
	sindy::CSPFieldMap		m_cFMCityAdmin;			//!< 都市行政界フィールドマップ
	sindy::CSPFieldMap		m_cFMBuilding;			//!< 家形ポリゴンフィールドマップ
	sindy::CSPFieldMap		m_cFMBuildingGeo;		//!< GEOSPACE家形ポリゴンフィールドマップ

	sindy::CSPTableNameString	m_cTNGouPoint;		//!< 号ポイントテーブルネーム
	sindy::CSPTableNameString	m_cTNCityAdmin;		//!< 都市行政界テーブルネーム
	sindy::CSPTableNameString	m_cTNBuilding;		//!< 家形ポリゴンテーブルネーム
	sindy::CSPTableNameString	m_cTNBuildingGeo;	//!< GEOSPACE家形ポリゴンテーブルネーム
};
