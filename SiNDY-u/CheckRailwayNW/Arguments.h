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


/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
//	Arguments() : m_meshMode(false), m_queryMode(false) {};
	Arguments(){};

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true    引数OK
	 * @retval false   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  実行モード確認
	 * @param  optMode1
	 */
//	bool SetMode( const boost::program_options::variables_map& vm, const char* const meshMode, const char* const queryMode );

public:
	uh::tstring  m_strRailNwDB;        //!< 鉄道NW接続先
	uh::tstring  m_strRailPoiDB;       //!< 鉄道POI接続先
	uh::tstring  m_strMeshDB;          //!< メッシュDB接続先
	uh::tstring  m_strStationDB;       //!< 中縮駅舎接続先
	uh::tstring  m_strLog;             //!< ログファイル
//	uh::tstring  m_strMeshLog;         //!< メッシュモードログファイル
//	uh::tstring  m_strQueryLog;        //!< クエリモードログファイル
	uh::tstring  m_strMeshList;        //!< メッシュリスト
	uh::tstring  m_dist_threshold;     //!< 微小距離閾値(m)
	uh::tstring  m_neighbor_threshold; //!< 近傍距離閾値(m)
//	bool         m_meshMode;           //!< メッシュモード
//	bool         m_queryMode;          //!< クエリモード

};
