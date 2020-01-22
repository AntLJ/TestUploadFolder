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
#include "RelStationSiteData.h"

// STL
#include <list>

// SiNDYLib
#include <FieldMap.h>



/**
 * @brief 駅ポリゴン紐付きテーブルに対しての処理を行うためのヘルパークラス
 */
class CRelationTableHelper
{
public:

	CRelationTableHelper() : m_Oid(-1), m_LayerNo(-1) {}
	~CRelationTableHelper(){}

	/**
	 * @brief 初期化処理
	 * @note  最初に本関数を必ず呼び出す必要あり
	 * @param[in] ipRow     この引数と同じオーナーのテーブルを開く
	 * @retval true  初期化成功
	 * @retval false 初期化失敗
	 */
	bool init( const _IRowPtr& ipRow );

	/**
	 * @brief 駅ポリゴン紐付きテーブルの読み込み
	 * @param [out] cData 読み込んだデータ 
	 * @retval true  読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool readRelStationSite( CRelStationSiteData& cData );

	/**
	 * @brief 駅名から駅ポリゴン紐付きテーブルを読み込む
	 * @param [in]  strStaName    読み込み対象の駅名
	 * @param [out] cData         読み込んだ鉄道路線データ
	 * @retval true  読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool readRelStationSiteByStationName(const CString& strStationName, CRelStationSiteData& cData);

	/**
	 * @brief 駅ポリゴン紐付きテーブルへデータ追加
	 * @param  [in]   cData   追加するレコード群
	 * @retval true   追加成功
	 * @retval false  追加失敗
	 */
	bool insertData( const CRelStationSiteData& cData ) const;

	/**
	 * @brief 駅ポリゴン紐付きテーブルのデータ削除
	 * @param [in]   cData  削除するレコード群
	 * @retval true  削除成功
	 * @retval false 削除失敗
	 */
	bool deleteData( const CRelStationSiteData& cData ) const;

	/**
	 * @brief   グループIDマスタから駅名を取得する
	 * @param   [in]   lGroupId  グループID
	 * @return  駅名
	 */
	CString getStationName( long lGroupId ) const;


private:

	/**
	 * @brief   駅ポリゴン紐付きテーブルの削除条件文作成
	 * @param   [in]     cData      削除対象の鉄道路線OID群
	 * @return  削除条件文
	 */
	CString getDeleteWhereClause( const CRelStationSiteData& cData ) const;

	/**
	 * @brief 駅ポリゴン紐付きテーブルのデータを駅IDから取得する
	 * @param [in]   lStationId   駅ID
	 * @param [out]  cData        読み込んだデータ 
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool readRelStationSiteByStationId( long lStationId, CRelStationSiteData& cData );


public:

	/**
	 * @brief レイヤ番号を取得する
	 * @param [in]   ipRow
	 * @retval -1以外 取得成功
	 * @retval -1     取得失敗
	 */
	static int getLayerNo( const _IRowPtr& ipRow );

	/**
	 * @brief レイヤ番号を取得する
	 * @param [in]   レイヤ名
	 * @retval -1以外 取得成功
	 * @retval -1     取得失敗
	 */
	static int getLayerNo( const CString& strLayerName );


private:

	long	m_Oid;								//!< 関連元レコードのOID
	int     m_LayerNo;							//!< レイヤー番号

	ITablePtr         m_ipRelStationSite;		//!< 駅ポリゴン紐付きテーブル
	ITablePtr         m_ipGroupIdMaster;		//!< グループIDマスタ

	sindy::CFieldMap  m_ipRelStationSiteFM;		//!< 駅ポリゴン紐付きテーブルのフィールドマップ
	sindy::CFieldMap  m_ipGroupIdMasterFM;		//!< グループIDマスタのフィールドマップ

};
