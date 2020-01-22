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
 * @brief 鉄道ノード紐付きテーブルに対しての処理を行うためのヘルパークラス
 * @note  鉄道ノード紐月テーブルからの関連先の乗り入れ路線テーブルの値の取得処理、<br>
 *        及び、乗り入れ路線テーブルのFKから他テーブルの値を取得する処理も本クラスで実装
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
	 * @brief 鉄道ノード紐付けテーブルの読み込み
	 * @param [out] cData 読み込んだデータ 
	 */
	bool read( CRelTableData& cData );

	/**
	 * @brief 乗り入れ路線テーブルの読み込み
	 * @param [in]  lId    読み込み対象のSTATION_ID
	 * @param [out] cData  読み込んだ駅情報データ
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readJoinLineInfo(long lStaPtOid, CRelTableData& cData);

	/**
	 * @brief 鉄道ノード紐付けテーブルへデータ追加
	 * @param  [in]   cData   追加するレコード群
	 * @retval true   追加成功
	 * @retval false  追加失敗
	 */
	bool addData( const CRelTableData& cData ) const;

	/**
	 * @brief 鉄道ノード紐付けテーブルのデータ削除
	 * @param [in]   IDs   削除対象のJOIN_LINE_INFO_ID群
	 * @retval true  削除成功
	 * @retval false 削除失敗
	 */
	bool deleteData( const std::vector<long>& IDs ) const;

	/**
	 * @brief 鉄道ノード紐付けテーブルの駅番号を更新
	 * @param [in]   cData  駅番号変更対象レコード群
	 * @retval true  変更成功
	 * @retval false 変更失敗
	 */
	bool changeStationNumber( const CRelTableData& cData ) const;

	/**
	 * @brief 鉄道ノード紐付けテーブルの削除条件分作成
	 * @param   [in]     cData      削除対象のJOIN_LINE_INFO_ID群
	 * @retval  削除条件文
	 */
	CString getDeleteWhereClause( const std::vector<long>& IDs ) const;

	/**
	 * @brief  駅ポイントのフィーチャを取得
	 * @param  [in]    lId     フィーチャ取得対象のJOIN_LINE_INFO_ID
	 * @retval return  駅ポイントのフィーチャ
	 */
	IFeaturePtr getStationPointFeature( long lId );


private:

	/**
	 * @brief  路線IDから路線名を取得する
	 * @param  駅名を取得できなければ、空文字列を返す
	 * @param  [in]  lLineId  路線ID
	 * @return 路線名 
	 */
	CString getRailwayLineName( long lLineId );

	/**
	 * @brief  駅情報IDから駅名を取得する
	 * @param  駅情報マスターから駅情報を取得する
	 * @param  [in]  lInfoId 駅情報ID
	 * @return 駅名 
	 */
	CString getStaionName( long lInfoOId );	


private:

	long	m_Oid;								//!< 関連元レコードのOID

	ITablePtr         m_ipRelRailwayNode;		//!< 関連テーブル
	ITablePtr         m_ipJoinLineInfo;			//!< 乗り入れ路線テーブル
	ITablePtr         m_ipRailwayLine;			//!< 鉄道路線テーブル
	ITablePtr		  m_ipStationInfo;			//!< 駅情報マスター
	IFeatureClassPtr  m_ipStationPoint;			//!< 駅ポイント

	sindy::CFieldMap  m_RelRailwayNodeFM;		//!< 関連テーブルのフィールドマップ
	sindy::CFieldMap  m_JoinLineInfoFM;			//!< 乗り入れ路線テーブルのフィールドマップ
	sindy::CFieldMap  m_RailwayLineFM;			//!< 乗り入れ路線テーブルのフィールドマップ
	sindy::CFieldMap  m_StationInfoFM;			//!< 乗り入れ路線テーブルのフィールドマップ
	sindy::CFieldMap  m_StationPointFM;			//!< 駅ポイントテーブルのフィールドマップ

};
