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

#include "stdafx.h"

#include "Properties.h"
#include "DBUtils.h"
#include <coord_converter.h>

#define ROAD_LINK_WHERE _T("roadclass_c < 400 AND alleymainte_c <> 9")
#define ROAD_NODE_WHERE _T("")
#define WALK_LINK_WHERE _T("")
#define WALK_NODE_WHERE _T("")
#define TIME_NOPASSAGE_WHERE _T("meshcode = %s")
#define WALK_NOPASSAGE_WHERE _T("meshcode = %s")
#define TIME_ONEWAY_WHERE _T("meshcode = %s")
#define WALK_ONEWAY_WHERE _T("meshcode = %s")
#define ROAD_CODE_LIST_WHERE _T("")
#define WALK_CODE_LIST_WHERE _T("")

const double MESH_BUFFER_METER = 500.0;

typedef std::map<CString, CAdapt<CComVariant>> MAP_VALUE;
typedef std::map<CString, MAP_VALUE> MAP_ROWS;
typedef std::set<MAP_VALUE> SET_ROWS;

static crd_cnv g_crdCnv;

class CDatabaseController
{
public:
	typedef enum layer
	{
		BASE_MESH,
		ROAD_LINK,
		ROAD_NODE,
		WALK_LINK,
		WALK_NODE,
		TIME_NOPASSAGE,
		WALK_NOPASSAGE,
		TIME_ONEWAY,
		WALK_ONEWAY,
		ROAD_CODE_LIST,
		WALK_CODE_LIST,
		NONE,
	} LAYER;

//private:
public:
	IWorkspacePtr m_ipRoadWorkspace;
	IWorkspacePtr m_ipWalkWorkspace;
	IWorkspacePtr m_ipRefWorkspace;
	long m_meshcodeIdx;
	IFeatureClassPtr m_ipMeshFeatureClass;

	struct databaseInfo
	{
		// 格納されているレイヤ
		LAYER eLayer;
		// フィーチャークラス
		CAdapt<ITablePtr> ipTable;
		// WHERE句のSQL
		CString sql;
		// ObjectIDをキーとしたフィーチャーのマップ(ROAD_LINK,ROAD_NODE,WALK_LINK,WALK_NODE用)
		MAP_FEATURE features;
		// メッシュ＋バッファ形状に含まれる、ObjectIDをキーとしたフィーチャーのマップ(ROAD_NODE用(WALK_NODE.ROAD_NODE_IDのチェックで使用))
		MAP_FEATURE buffer_features;
		// テーブルのレコードセット(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY用)
		SET_ROWS rows;
		// ROADCODEをキーとしたレコードのマップ(ROAD_CODE_LIST,WALK_CODE_LIST用)
		MAP_ROWS roadcode_rows;
		// フィールドインデックス
		FIELD_INDEX fieldIndex;
	};
	std::vector<databaseInfo> m_dbList;

	LAYER tmp_layer;
	LAYER tmp_buffer_layer;
	MAP_FEATURE tmp_features;
	MAP_FEATURE tmp_buffer_features;
	SET_ROWS tmp_rows;
	MAP_ROWS tmp_roadcode_rows;
	FIELD_INDEX tmp_fieldIndex;

	CString m_roadSchema;
	CString m_walkSchema;
	CString m_refSchema;

public:

	CDatabaseController(void);
	~CDatabaseController(void);

	/**
	 * @brief	DB初期化
	 * @param	roadDbConnection	[in]	道路DB接続情報
	 * @param	walkDbConnection	[in]	歩行者DB接続情報
	 * @param	refDbConnection	[in]	参照メッシュDB接続情報
	 * @param	roadSchema	[in]	道路DBスキーマ
	 * @param	walkSchema	[in]	歩行者DBスキーマ
	 * @param	refSchema	[in]	参照メッシュDBスキーマ
	 * @param	inputLayer	[in]	チェック対象レイヤ
	 * @param	errDb	[out]	エラー時のエラーDB接続情報
	 * @return	bool
	 */
	bool init(std::string roadDbConnection, std::string walkDbConnection, std::string refDbConnection, std::string roadSchema, std::string walkSchema, std::string refSchema, std::string inputLayer, std::string& errDb);

	/**
	 * @brief	取得対象のメッシュ内のフィーチャ設定(ROAD_LINK,ROAD_NODE,WALK_LINK,WALK_NODE用)
	 * @param	cMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool setArea(std::string meshcode);

	/**
	 * @brief	取得対象のメッシュ＋バッファ内のフィーチャ設定(ROAD_NODE用(WALK_NODE.ROAD_NODE_IDのチェックで使用))
	 * @param	cMeshcode	[in]	メッシュコード
	 * @param	layer	[in]	取得対象のレイヤ
	 * @return	bool
	 */
	bool setAreaBuffer(std::string meshcode, LAYER layer);

	/**
	 * @brief	フィーチャー取得
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	objectId	[in]	取得対象のオブジェクトID
	 * @param	ipFeature	[out]	フィーチャー
	 * @return	bool
	 */
	bool getFeature(LAYER layer, const long objectId, IFeaturePtr& ipFeature);

	/**
	 * @brief	メッシュ＋バッファ内のフィーチャー取得
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	objectId	[in]	取得対象のオブジェクトID
	 * @param	ipFeature	[out]	フィーチャー
	 * @return	bool
	 */
	bool getBufferFeature(LAYER layer, const long objectId, IFeaturePtr& ipFeature);

	/**
	 * @brief	取得対象のメッシュコードのレコード設定(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY用)
	 * @param	cMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool setAreaRecords(std::string meshcode);

	/**
	 * @brief	取得対象のレコード設定(ROAD_CODE_LIST,WALK_CODE_LIST用)
	 * @return	bool
	 */
	bool setRoadcode(void);

	/**
	 * @brief	フィールドインデックス取得
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	index	[out]	フィールドインデックス
	 * @return	bool
	 */
	bool getFieldIndex(LAYER layer, FIELD_INDEX& index);

	/**
	 * @brief	件数取得
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	count	[out]	件数
	 * @return	bool
	 */
	bool getRecordCount(LAYER layer,long& count);
	void clear(void);

	/**
	 * @brief	レコード検索(TIME_NOPASSAGE,WALK_NOPASSAGE,TIME_ONEWAY,WALK_ONEWAY用)
	 * @param	layer	[in]	対象のレイヤ
	 * @param	mapValue	[in]	検索対象のレコードの値map
	 * @return	bool
	 */
	bool findRow(LAYER layer, MAP_VALUE& mapValue);

	/**
	 * @brief	レコード検索(ROAD_CODE_LIST,WALK_CODE_LIST用)
	 * @param	layer	[in]	対象のレイヤ
	 * @param	key	[in]	検索対象のキー(道路の場合:路線コード,路線種別コード、歩行者の場合:路線コード)
	 * @param	mapValue	[out]	検索結果のレコードの値map
	 * @return	bool
	 */
	bool findRoadCode(LAYER layer, CString key, MAP_VALUE& mapValue);

	/**
	 * @brief	ObjectIDをキーとしたフィーチャーのマップ取得(setArea()で取得済みであれば取得済の内容を返す。未取得であれば新規に取得する)
	 * @param	cMeshcode	[in]	メッシュコード
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	MapFeatures	[out]	フィーチャーのマップ
	 * @param	FiledIndex	[out]	フィールドインデックス
	 * @return	bool
	 */
	bool getFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& MapFeatures, FIELD_INDEX& FiledIndex);

private :

	/**
	 * @brief	DB情報取得
	 * @param	ipWorkspace	[in]	ワークスペース
	 * @param	tableName	[in]	テーブル名
	 * @param	schema	[in]	スキーマ
	 * @param	dbInfo	[out]	DB情報
	 * @return	bool
	 */
	bool init_db(const IWorkspacePtr ipWorkspace , CString tableName, CString schema, databaseInfo& dbInfo );

	/**
	 * @brief	ObjectIDをキーとしたテーブルマップの生成（ジオメトリ指定）
	 * @param	meshGeometry	[in]	取得範囲ジオメトリ
	 * @param	dbInfo	[in]	取得対象のDB情報
	 * @param	features	[out]	作成したマップ
	 * @return	bool
	 */
	bool createFeatureMap(const IGeometryPtr meshGeometry, databaseInfo dbInfo, MAP_FEATURE& features);

	/**
	 * @brief	フィールドの値を取得してマップに設定
	 * @param	dbInfo	[in]	DB情報
	 * @param	ipRow	[in]	レコード
	 * @param	field	[in]	取得するフィールド名
	 * @param	bSTR	[in]	取得した値を文字列で保持するかどうか(CComVariant.vt=VT_BSTR)
	 *							使用ケース1: 文字列の場合、CComVariantではNULLの扱いが2種類あるため統一したい場合
	 *							・CComVariant.vt == VT_NULL				※レコード追加後の初期状態
	 *							・CComVariant.vt == VT_BSTR, 値 = ""	※一旦値を設定後に削除した状態
	 *							使用ケース2: VT_ENPTY、VT_NULLをどちらもNULLとして区別したくないため文字列型として扱いたい場合
	 * @param	mapValue	[out]	取得した値を設定するマップ
	 * @return	bool
	 */
	bool setValue(const databaseInfo& dbInfo, const _IRowPtr& ipRow, const CString& field, const bool bSTR, MAP_VALUE& mapValue);

	/**
	 * @brief	ObjectIDをキーとしたフィーチャーのマップ取得(新規に取得)
	 * @param	cMeshcode	[in]	メッシュコード
	 * @param	layer	[in]	取得対象のレイヤ
	 * @param	ipFeature	[out]	フィーチャーのマップ
	 * @param	filedIndex	[out]	フィールドインデックス
	 * @return	bool
	 */
	bool getNewFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& features, FIELD_INDEX& filedIndex);

	/**
	 * @brief	メッシュのフィーチャー取得
	 * @param	cMeshcode	[in]	メッシュコード
	 * @param	feasures	[out]	取得したフィーチャー
	 * @return	bool
	 */
	bool getMesh(const std::string& cMeshcode, MAP_FEATURE& feasures);

	/**
	 * @brief	メートル→座標変換
	 * @param	meter	[in]	メートル
	 * @param	meshGeometry	[in]	メッシュ形状
	 * @param	coordinate	[out]	変換後の座標
	 * @return	bool
	 */
	bool meter2Coordinate(double meter, const IGeometryPtr& meshGeometry, double& coordinate);

	/**
	 * @brief	バッファ取得
	 * @param	meshGeometry	[in]	メッシュ形状
	 * @param	ipBuffer	[out]	メッシュ＋バッファ形状
	 * @param	bufMeter	[in]	バッファ(メートル)
	 * @return	bool
	 */
	bool getBuffer(const IGeometryPtr& meshGeometry, IGeometryPtr& ipBuffer, double bufMeter = MESH_BUFFER_METER);

};

