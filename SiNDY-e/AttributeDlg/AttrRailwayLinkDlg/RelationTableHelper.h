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

#include <atlstr.h>
#include <FieldMap.h>
#include <list>

#include "RelTableData.h"


/**
 * @brief 鉄道リンク紐付きテーブルに対しての処理を行うためのヘルパークラス
 * @note  鉄道リンク紐月テーブルからの関連先の鉄道路線テーブルの値の取得処理、<br>
 *        及び、鉄道路線テーブルのFKから他テーブルの値を取得する処理も本クラスで実装
 */
class CRelationTableHelper
{
public:

	CRelationTableHelper() : m_Oid(-1) {}
	~CRelationTableHelper(){}

	/**
	 * @brief 初期化処理
	 * @note  最初に本関数を必ず呼び出す必要あり
	 * @param[in] ipRow     この引数と同じオーナーのテーブルを開く
	 * @param[in] cApp      アプリケーションヘルパー
	 * @retval true  初期化成功
	 * @retval false 初期化失敗
	 */
	bool init( _IRowPtr& ipRow, const CApplication& cApp );

	/**
	 * @brief 鉄道リンク紐付けテーブルの読み込み
	 * @param [out] cData 読み込んだデータ 
	 */
	bool readRelRailwayLink( CRelTableData& cData );

	/**
	 * @brief 駅ポイントIDのOIDから鉄道路線データを読み込む
	 * @param [in]  lId    読み込み対象のSTATION_ID
	 * @param [out] cData  読み込んだ路線データ群
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readRailLineFromStaPtOid(long lStaPtOid, CRelTableData& cData);

	/**
	 * @brief 駅名から鉄道路線データを読み込む
	 * @param [in]  strStaName    読み込み対象の駅名
	 * @param [out] cData         読み込んだ鉄道路線データ
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readRailwayLineFromStaName(const CString& strStaName, CRelTableData& cData);

	/**
	 * @brief 鉄道リンク紐付けテーブルへデータ追加
	 * @param  [in]   cData   追加するレコード群
	 * @retval true   追加成功
	 * @retval false  追加失敗
	 */
	bool addData( const CRelTableData& cData ) const;

	/**
	 * @brief 鉄道リンク紐付けテーブルのデータ削除
	 * @param [in]   IDs   削除対象の鉄道路線OID群
	 * @retval true  削除成功
	 * @retval false 削除失敗
	 */
	bool deleteData( const std::vector<long>& IDs ) const;

	/**
	 * @brief   鉄道路線のOIDを指定して該当する鉄道リンクのフィーチャー群を取得する
	 * @param   [in]   lRwLineId  鉄道路線のOID 
	 * @return  鉄道リンクのフィーチャ群
	 */
	std::vector<CAdapt<IFeaturePtr>> getRailwayLinkFeatures( long lRwLineId );


private:

	/**
	 * @brief   鉄道リンク紐付けテーブルの削除条件分作成
	 * @param   [in]     cData      削除対象の鉄道路線OID群
	 * @return  削除条件文
	 */
	CString getDeleteWhereClause( const std::vector<long>& IDs ) const;

	/**
	 * @brief 鉄道路線テーブルのOIDからデータを取得する
	 * @param [in]   lOid    鉄道路線テーブルのOID
	 * @param [out]  record  読み込んだレコード 
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readRailwayLineRecord( long lOid, CRelTableData::RailwayLineRecord& record );

	/**
	 * @brief 鉄道路線テーブルのデータを駅情報IDから取得する
	 * @param [in]   lOid   鉄道路線テーブルの鉄道会社ID
	 * @param [out]  cData  読み込んだデータ 
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readRailwayLineFromStaInfoId( long lStaInfoId, CRelTableData& cData );

	/**
	 * @brief  鉄道会社IDから会社名を取得する
	 * @note   会社名を取得できなければ、空文字列を返す
	 * @param  [in]  lCorpId  鉄道会社ID
	 * @return 会社名 
	 */
	CString getCorpName( long lCorpId );


private:

	long	m_Oid;								//!< 関連元レコードのOID

	ITablePtr         m_ipRelRailwayLink;		//!< 鉄道リンク紐付けテーブル
	ITablePtr         m_ipRailwayLink;			//!< 鉄道リンクテーブル
	ITablePtr         m_ipJoinLineInfo;			//!< 乗り入れ路線テーブル
	ITablePtr         m_ipRailwayLine;			//!< 鉄道路線テーブル
	ITablePtr		  m_ipRailwayCorp;			//!< 鉄道会社テーブル
	ITablePtr		  m_ipStationInfo;			//!< 駅情報マスター
	IFeatureClassPtr  m_ipStationPoint;			//!< 駅ポイント

	sindy::CFieldMap  m_RelRailwayLinkFM;		//!< 関連テーブルのフィールドマップ
	sindy::CFieldMap  m_RailwayLinkFM;			//!< 鉄道リンク紐付けテーブルフィールドマップ
	sindy::CFieldMap  m_JoinLineInfoFM;			//!< 乗り入れ路線テーブルのフィールドマップ
	sindy::CFieldMap  m_RailwayLineFM;			//!< 鉄道路線テーブルのフィールドマップ
	sindy::CFieldMap  m_RailwayCorpFM;			//!< 鉄道会社テーブルのフィールドマップ
	sindy::CFieldMap  m_StationInfoFM;			//!< 駅情報マスターのフィールドマップ
	sindy::CFieldMap  m_StationPointFM;			//!< 駅ポイントテーブルのフィールドマップ

};
