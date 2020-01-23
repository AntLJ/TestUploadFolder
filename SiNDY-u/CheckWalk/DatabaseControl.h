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

#include <TDC/sindylib_core/FieldMap.h>
#include <sindy/workspace/name.h>
#include <sindy/schema.h>

/**
 * @brief データベース
 */
enum database
{
	road,
	walk,
	station,
	poi,
	underground_area,
	mesh,
};

/**
 * @brief テーブル
 * @warinig 先頭の識別子の値を0以外にするか、末尾のtable_sizeの位置を変えると
 * OpenTables()の処理が失敗するので、変更しないこと。
 */
enum table
{
	c_road_link = 0,
	c_road_node,
	c_segment_attr,
	c_walk_link,
	c_walk_node,
	c_height_node,
	c_link_relation,
	c_walk_code_list,
	c_walk_nopassage,
	c_walk_oneway,
	c_walk_type,
	c_station_gate,
	c_poi_point_org,
	c_underground_area,
	c_base_mesh,
	c_city_mesh,
	c_scramble_area,
	c_pedxing,
	table_size
};

/**
 * @class	CDatabaseControl
 * @brief	DB接続管理クラス
 */
class CDatabaseControl
{
public:

	CDatabaseControl(void);
	virtual ~CDatabaseControl(void){}

	/**
	 * @brief 初期化
	 * @param  roadDB    [in] 道路DB接続文字列
	 * @param  walkDB    [in] 歩行者DB接続文字列
	 * @param  stationDB [in] 駅DB接続文字列
	 * @param  poiDB     [in] POIDB接続文字列
	 * @param  underDB   [in] 地下街DB接続文字列
	 * @param  meshDB    [in] メッシュDB接続文字列
	 * @retval 空文字 成功
	 * @retval 空文字以外 接続に失敗したDB名
	 */
	CString Init(
		const CString& roadDB,
		const CString& walkDB,
		const CString& stationDB,
		const CString& poiDB,
		const CString& underDB,
		const CString& meshDB);

	/**
	 * @brief	テーブル群のオープン
	 * @retval	空文字	成功
	 * @retval	空文字以外	オープンに失敗したテーブル名
	 */
	CString OpenTables();

	/**
	 * @brief	ワークスペースの取得
	 * @param	value	[in]	取得対象のEnum値
	 * @param	ipWorkspace	[out]	ワークスペース
	 * @retval	true	成功
	 * @retval	false 失敗
	 */
	bool GetWorkspace(const database value, IWorkspacePtr& ipWorkspace);

	/**
	 * @brief	テーブルの取得
	 * @param	value	[in]	取得対象のEnum値
	 * @param	ipTable	[out]	テーブル
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetTable(const table value, ITablePtr& ipTable);

	/**
	 * @brief	フィールドマップの取得
	 * @param	value	[in]	取得対象のEnum値
	 * @param	fieldMap	[out]	フィールドマップ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetFieldMap(const table value, sindy::CFieldMap& fieldMap);

	/**
	 * @brief データタイプの追加
	 * @param [in] dataType データタイプ	 
	 */
	void setDataType(sindy::schema::walk_type::type_class::ECode dataType);

private:
	/**
	 * @brief	ワークスペースオープン
	 * @param	name	[in]	接続名
	 * @param	ipWorkspace	[out]	ワークスペース
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	テーブル初期設定
	 * @param	workspaceValue	[in]	ワークスペース
	 * @param	tableValue	[in]	オープン対象のテーブル
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool InitialaizeTable( table tableValue);

	/**
	 * @brief	テーブルオープン
	 * @param	name	[in]	フィーチャクラス名
	 * @param	ipWorkspace	[in]	オープンするためのワークスペース
	 * @param	ipTable	[out] 取得するテーブル
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @brief	フィールドマップ作成
	 * @param	ipTable	[in]	作成対象のテーブル
	 * @param	fieldMap	[out]	取得するフィールドマップ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool CreateFiledMap( const ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	/**
	 * @brief	引き数に指定されたEnum値の文字列取得
	 * @param	value	[in]	取得対象のEnum値
	 * @return	文字列	対象がなかった場合""(空文字)
	 */
	CString GetEnumString(const table value);

private:
	/**
	 * @brief ワークスペースのMap
	 */
	std::map<database, IWorkspacePtr> m_workspaceMap;
	/**
	 * @brief フィーチャークラスのMap
	 */
	std::map<table, IFeatureClassPtr> m_featureClassMap;
	/**
	 * @brief テーブルのMap
	 */
	std::map<table, ITablePtr> m_tableMap;
	/**
	 * @brief フィールドマップのMap
	 */
	std::map<table, sindy::CFieldMap> m_fieldMap;
	/**
	 * @brief	DBとデータベースのMap
	 */
	std::map<table, database> m_tableDatabasePairMap;

	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};

