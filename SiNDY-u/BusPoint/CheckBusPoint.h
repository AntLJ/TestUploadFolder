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
 * @class   CCheckBusPoint
 * @brief	バスデータDBチェックのためのメインクラス
 *          TODO: バスライブラリ活用＆機能移植
 *          TODO: チェック類をバスライブラリに移植し、本クラスは呼ぶ側だけとなる想定
 */
class CCheckBusPoint
{
public:
	/**
	 * @brief	コンストラクタ
 	 * @param	args [in] 各種オプション
	 */
	CCheckBusPoint( const Arguments& Args ){
		m_strChkLog  = (Args.db_check_log).c_str();	
		m_strBusDB   = (Args.bus_db).c_str();
		m_strRoadDB  = (Args.road_db).c_str();

	};

	/**
	 * @brief	デストラクタ
	 */
	~CCheckBusPoint(){};

	/**
	 * @brief	各種処理を実行するために最初に呼ばれる関数
	 * @retval	true  処理成功
	 * @retval	false 処理失敗
	 */
	bool execute();

private:

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<CString, long>       CLPAIR;
	typedef std::pair<long, long>          LLPAIR;
	typedef std::vector<long>              LONGVEC;
	typedef std::pair<long, CCPAIR>        LCCPAIR;
	typedef std::pair<CLPAIR, CCPAIR>      CLCCPAIR;
	typedef std::vector<CLPAIR>            CLPVEC;
	typedef std::pair<CString, CCPAIR>     CCCPAIR;
	typedef std::pair<LLPAIR, CCCPAIR>     LLPCCCP;
	typedef std::vector<LLPCCCP>           LLCCCPVEC;
	typedef std::pair<long, LLPAIR>        LLLPAIR;

	typedef std::pair<LLLPAIR, CCCPAIR>    LLLCCCPAIR;
	typedef std::vector<LLLCCCPAIR>        LLLCCCPVEC;
	typedef std::vector<CLCCPAIR>          CLCCPVEC;

	typedef std::map<LLPAIR, LONGVEC>      BUSSTOPP;       //!< バス停ポイント同士の属性比較用map
	typedef std::map<LCCPAIR, CLPVEC>      SAMELINEMASTER; //!< バス路線マスタの事業者名、系統番号、経路が同一がないか確認用map
	typedef std::map<long, CLCCPVEC>       SAMELINENUM;    //!< バス路線マスタの同一系統番号で、系統名が異なるものの確認用map
	typedef std::map<long, LLLCCCPVEC>     SAMESEQ;        //!< 同一路線の停車順確認用map
	typedef std::map<long, LLCCCPVEC>      SAMELINESEQ;    //!< 停車順のシーケンス確認用map

	typedef std::map<CString, CComVariant> MAPATTR;        //!< フィールド名と要素格納用map

	/**
	 * @brief   各種初期化
	 * @retval  true  初期化成功
	 * @retval  false 初期化失敗
	 */
	bool Init();

	/**
	 * @brief   バス停ポイントチェック
	 */
	void CheckStopPoint();

	/**
	 * @brief   バスルート探索用ポイントチェック
	 */
	void CheckRoutePoint();

	/**
	 * @brief   バス路線マスタチェック
	 */
	void CheckLineMaster();

	/**
	 * @brief   バス路線マスタ同士の確認
	 * @param   sameLineMaster [in]  事業者名、系統番号、経路をキーに必要な情報を詰めたmap
	 * @param   sameLineNum    [in]  系統番号をキーに必要な情報を詰めたmap
	 */
	void CheckLineMasterAttrs( const SAMELINEMASTER& sameLineMaster, const SAMELINENUM& sameLineNum );

	/**
	 * @brief   バス停車順管理チェック
	 */
	void CheckStopSeq();

	/**
	 * @brief   同一路線の停車順確認
	 * @param   sameSeq [in]  路線IDをキーに必要な情報を詰めたmap
	 */
	void CheckSameSeqs( const SAMESEQ& sameSeq );

	/**
	 * @brief   バス停ポイントに対応するルート探索用ポイントを取得
	            TODO: 必要に応じて1つ以上は、カーソルで返すよう変更
	 * @param   oID   [in]  バス停ポイントのObjectID
	 * @param   count [out] 見つかった個数
	 * @retval  count =  1：ルート探索用ポイント
	 * @retval  count != 1：nullptr
	 */
	IFeaturePtr GetRoutePoint( const long& oID, long& count );

	/**
	 * @brief   フィーチャ間の距離(m)を測定
	 * @param   ipSrcFeature  [in]  比較元フィーチャ
	 * @param   ipDestFeature [in]  比較先フィーチャ
	 * @retval  double        フィーチャ間の距離(m)
	 */
	double GetDistFeature( const IFeaturePtr& ipSrcFeature, const IFeaturePtr& ipDestFeature );

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
	_ICursorPtr GetStopSeqs( const long& oID, const CString& field, long& count );

	/**
	 * @brief   停車順管理テーブルの路線情報IDから路線情報管理テーブルを取得
     *          TODO: 必要に応じて、1個以上はカーソル＆個数を返すよう変更
	 * @param   oID   [in]  停車順管理の路線情報ID
	 * @retval  count != 0 ：見つかったRow
	 * @retval  count  = 0 ：nullptr
	 */
	_IRowPtr GetLineMaster( const long& lineID ); 

	/**
	 * @brief   バス停ポイントの指定近傍のバス停ポイントを取得
	 * @note    指定近傍は、現状は20m固定で、バッファを使用する
	 * @param   ipFeature [in]  基準となるバス停ポイント
	 * @param   count     [out] 見つかった個数
	 * @retval  count != 0 ：バス停ポイントのカーソル
	 * @retval  count  = 0 ：nullptr
	 */
	IFeatureCursorPtr GetNearStopPoints( const IFeaturePtr& ipFeature, long& count );

	/**
	 * @brief   バス停ポイントのカーソルから、標柱ポイント数をカウント
	 * @param   ipCursor [in]  バス停ポイントのカーソル
	 * @retval  count    標柱ポイント数
	 */
	long CountPolePoint( const IFeatureCursorPtr& ipCursor );

	/**
	 * @brief   ルート探索用ポイントor停車順に対応するバス停ポイントを取得
	 * @param   oID   [in]  ルート探索用ポイントor停車順のバス停ポイントID
	 * @param   count [out] 見つかった個数
	 * @retval  count =  1 ：バス停ポイント
	 * @retval  count != 1 ：nullptr
	 */
	IFeaturePtr GetStopPoint( const long& stopID, long& count );

	/**
	 * @brief   ルート探索用ポイントの指定近傍の道路リンクを取得
	 * @note    指定近傍は、現状は0.5m(50cm)固定で、バッファを使用する
	 * @param   ipFeature [in]  基準となるルート探索用ポイント
	 * @retval  フィーチャ(見つからなかった場合は、NULL)
	 */
	IFeaturePtr GetNearRoadLink( const IFeaturePtr& ipFeature );




private:

	CString           m_strChkLog;         //!< DBチェックログファイル名

	CString           m_strBusDB;          //!< バス関連データベース接続名
	CString           m_strRoadDB;         //!< 道路データベース接続名

	std::ofstream     m_ofsChk;            //!< DBチェックログ

	IWorkspacePtr     m_ipBusWorkspace;    //!< バス関連ワークスペース
	IWorkspacePtr     m_ipRoadWorkspace;   //!< 道路ワークスペース
	
	IFeatureClassPtr  m_ipBusStopPointFC;  //!< バス停ポイントフィーチャクラス
	ITablePtr         m_ipBusLineMasterTable; //!< バス路線マスタテーブル
	ITablePtr         m_ipBusStopSeqTable; //!< バス停車順管理テーブル
	IFeatureClassPtr  m_ipBusRoutePointFC; //!< バスルート探索用ポイントフィーチャクラス
	IFeatureClassPtr  m_ipRoadLinkFC;      //!< 道路リンクフィーチャクラス

	CFieldMap         m_busStopPointFM;    //!< バス停ポイントフィールドマップ
	CFieldMap         m_busLineMasterFM;   //!< バス路線マスタフィールドマップ
	CFieldMap         m_busStopSeqFM;      //!< バス停車順管理フィールドマップ
	CFieldMap         m_busRoutePointFM;   //!< バスルート探索用ポイントフィールドマップ
};

