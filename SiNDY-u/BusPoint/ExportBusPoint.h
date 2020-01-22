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
#include "Util.h"
#include "FieldMap.h"

/**
 * @class   CExportBusPoint
 * @brief	バスデータエクスポートのためのメインクラス
 *          TODO: バスライブラリ活用＆機能移植
 *          TODO: 関連テーブルからの取得関数は全てライブラリ側とする想定
 */
class CExportBusPoint
{
public:
	/**
	 * @brief	コンストラクタ
 	 * @param	args [in] 各種オプション
	 */
	CExportBusPoint( const Arguments& Args ){
		m_allCompany     = Args.bAllCompany;
		m_strExpLog      = (Args.export_log).c_str();	
		m_strCompanyList = (Args.company_list).c_str();
		m_strExpDir      = (Args.export_dir).c_str();
		m_strBusDB       = (Args.bus_db).c_str();
	};

	/**
	 * @brief	デストラクタ
	 */
	~CExportBusPoint(){};

	/**
	 * @brief	各種処理を実行するために最初に呼ばれる関数
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool execute();

private:

	typedef std::map<CString, CComVariant> MAPATTR;     //!< フィールド名と要素格納用map

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<long, CString>       LCPAIR;
	typedef std::pair<LCPAIR, CCPAIR>      LINEMASTER;
	typedef std::vector<LINEMASTER>        LCCCPVEC;
	typedef std::map<long, MAPATTR>        MAPATTRMAP;

	typedef std::set<long>                 OIDSET;      //!< ID格納用set

	typedef std::map<long, LCCCPVEC>       LINEMASTERS; //!< バス路線マスタ格納用map
	typedef std::multimap<long, MAPATTR>   STOPSEQS;    //!< 同一経路の停車順格納用multimap
	typedef std::map<long, MAPATTRMAP>     STOPPOINTS;	//!< バス停ポイント格納用map(2重キー)

	/**
	 * @brief	経路昇順ソート用関数オブジェクト
	 * @param	p1, p2	[in]	比較用オブジェクト
	 */
	struct lessDetail : public std::binary_function<LINEMASTER, LINEMASTER, bool>
	{
		bool operator ()(const LINEMASTER& p1, const LINEMASTER& p2) const {
		
			// first:( first:ObjectID, second:系統名 ), second:( first:経路, second:経由地等備考 )
			// 経路で昇順
			if( p1.second.first != p2.second.first ){
				return p1.second.first < p2.second.first;
			}

			// ObjectIDで昇順(念のため)
			return p1.first.first < p2.first.first;
		}
	};

	/**
	 * @brief   各種初期化
	 * @retval  true  初期化成功
	 * @retval  false 初期化失敗
	 */
	bool Init();

	/**
	 * @brief   バス路線マスタから全事業者リストを作成する
	 * @retval  true  作成成功
	 * @retval  false 作成失敗
	 */
	bool CreateCompanyList();

	/**
	 * @brief   出力対象事業者リストを読み込み、事業者名1行ずつをリストに格納
	 * @retval  true  読み込み成功
	 * @retval  false 読み込み失敗
	 */
	bool LoadList();

	/**
	 * @brief   出力メイン関数
	 * @param   companyName [in]  出力対象事業者名
	 */
	void Export( const CString& companyName );

	/**
	 * @brief   バス路線マスタから該当事業者の情報を取得
	 * @param   companyName [in]  取得したい事業者名
	 * @param   lineMasters [out] 取得したバス路線マスタの情報格納用(経路情報出力で使用)
	 * @param   lineOIDs    [out] 取得したバス路線マスタのObjectIDリスト(異表記情報出力で使用)
	 */
	void GetLineMaster( const CString& companyName, LINEMASTERS& lineMasters, OIDSET& lineOIDs );

	/**
	 * @brief   バス停経路情報出力
	 * @param   lineMasters [in]  バス路線マスタの情報(系統番号がキーのmap)
	 */
	void ExportDetail( LINEMASTERS& lineMasters );

	/**
	 * @brief   バス停留所名異表記情報出力
	 * @param   lineOIDs [in]  バス路線マスタのObjectIDリスト
	 */
	void ExportChanged( const OIDSET& lineOIDs );

	/**
	 * @brief   該当停車順と紐づくバス停ポイントの(変更後)停留所名取得
	 * @note    変更後停留所名がNULLじゃない場合は、変更後停留所名が優先
	 * @param   mapSeqAttr [in]  停車順の属性コンテナ
	 * @retval  変更後停留所名 != NULL：変更後停留所名
	 *          変更後停留所名  = NULL：停留所名
	 */
	CString GetStopName( const MAPATTR& mapSeqAttr );

	/**
	 * @brief   フィーチャから、1/256秒緯度・経度を取得
	 * @param   ipFeature [in]  位置を取得したいフィーチャ
	 * @param   lat256    [out] 1/256秒緯度
	 * @param   lon256    [out] 1/256秒経度
	 */
	void Get256LatLon( const IFeaturePtr& ipFeature, int& lat256, int& lon256 );

	/**
	 * @brief   バス停経路情報出力ファイルに1行出力
	 * @param   lineIdx       [in]  行番号
	 * @param   lat256        [in]  1/256秒緯度
	 * @param   lon256        [in]  1/256秒経度
	 * @param   routeNum      [in]  系統番号
	 * @param   routeName     [in]  系統名
	 * @param   routeDetail   [in]  経路
	 * @param   routeRemarks  [in]  経由地等備考
	 * @param   startStopName [in]  発停留所名(経路内の停車順順序番号が1の(変更後)停留所名)
	 * @param   endStopName   [in]  着停留所名(経路内の停車順順序番号が最大の(変更後)停留所名)
	 * @param   mapSeqAttr    [in]  停車順の属性コンテナ
	 * @param   mapStopAttr   [in]  バス停ポイントの属性コンテナ
	 * @param   routeFlag     [in]  ルート探索用ポイント出力時true(デフォルト：false)
	 * @retval  true  1行出力成功
	 * @retval  false 1行出力失敗
	 */
	bool ExportDetailLine( const long lineIdx, const int lat256, const int lon256, const long routeNum, const CString& routeName, const CString& routeDetail, const CString& routeRemarks, const CString& startStopName, const CString& endStopName, const MAPATTR& mapSeqAttr, const MAPATTR& mapStopAttr, bool routeFlag = false );

	/**
	 * @brief   バス停留所名異表記情報出力ファイルに1行出力
	 * @param   lineIdx       [in]  行番号
	 * @param   mapStopAttr   [in]  バス停ポイントの属性コンテナ
	 * @retval  true  1行出力成功
	 * @retval  false 1行出力失敗
	 */
	bool ExportChangedLine( const long lineIdx, const MAPATTR& mapStopAttr );

	/**
	 * @brief   指定フィールドとObjectIDに対応する停車順管理テーブルを取得
	 * @note    バス停ポイントからは、バス停ポイントのObjectIDとそのIDフィールド名
	 * @note    バス路線マスタからは、バス路線マスタのObjectIDとそのIDフィールド名
	 * @param   oID   [in]  バス停ポイントのObjectID or バス路線マスタのObjectID
	 * @param   field [in]  フィールド名
	 * @param   count [out] 見つかった個数
	 * @retval  count != 0 ：停車順管理のカーソル
	 * @retval  count  = 0 ：nullptr
	 */
	_ICursorPtr GetStopSeqs( const long oID, const CString& field, long& count );

	/**
	 * @brief   ルート探索用ポイントor停車順に対応するバス停ポイントを取得
	 * @param   oID   [in]  ルート探索用ポイントor停車順のバス停ポイントID
	 * @param   count [out] 見つかった個数
	 * @retval  count =  1 ：バス停ポイント
	 * @retval  count != 1 ：nullptr
	 */
	IFeaturePtr GetStopPoint( const long stopID, long& count );

	/**
	 * @brief   バス停ポイントに対応するルート探索用ポイントを取得
	            TODO: 必要に応じて1つ以上は、カーソルで返すよう変更
	 * @param   oID   [in]  バス停ポイントのObjectID
	 * @param   count [out] 見つかった個数
	 * @retval  count =  1：ルート探索用ポイント
	 * @retval  count != 1：nullptr
	 */
	IFeaturePtr GetRoutePoint( const long oID, long& count );

private:

	bool              m_allCompany;        //!< 全事業者出力モード
	CString           m_strExpLog;         //!< DBチェックログファイル名
	CString           m_strCompanyList;    //!< 出力事業者名リスト
	CString           m_strExpDir;         //!< 出力先ディレクトリ名

	CString           m_strBusDB;          //!< バス関連データベース接続名

	std::ofstream     m_ofsExp;            //!< DBチェックログ
	std::ofstream     m_ofsDetailFile;     //!< バス停経路情報出力ファイル
	std::ofstream     m_ofsChangedFile;    //!< バス停留所名異表記情報出力ファイル

	IWorkspacePtr     m_ipBusWorkspace;    //!< バス関連ワークスペース
	
	IFeatureClassPtr  m_ipBusStopPointFC;  //!< バス停ポイントフィーチャクラス
	ITablePtr         m_ipBusLineMasterTable; //!< バス路線マスタテーブル
	ITablePtr         m_ipBusStopSeqTable; //!< バス停車順管理テーブル
	IFeatureClassPtr  m_ipBusRoutePointFC; //!< バスルート探索用ポイントフィーチャクラス

	CFieldMap         m_busStopPointFM;    //!< バス停ポイントフィールドマップ
	CFieldMap         m_busLineMasterFM;   //!< バス路線マスタフィールドマップ
	CFieldMap         m_busStopSeqFM;      //!< バス停車順管理フィールドマップ
	CFieldMap         m_busRoutePointFM;   //!< バスルート探索用ポイントフィールドマップ

	std::set<CString> m_companySet;        //!< 出力対象事業者名リスト
};
