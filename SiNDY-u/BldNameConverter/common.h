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

// ツール名
const LPCTSTR TOOLNAME = _T("BldNameConverter");

// 引数
const LPCSTR INI		= "ini";
const LPCSTR PROCLOG	= "proc_log";
const LPCSTR ERRLOG		= "err_log";
const LPCSTR TEST		= "test_mode";

// 設定ファイル項目
const LPCSTR BASE_DB		= "BASE_DB";
const LPCSTR MAP_DB			= "MAP_DB";
const LPCSTR MESH_DB		= "MESH_DB";
const LPCSTR BLD_EXC_LIST	= "BLD_EXC_LIST";
const LPCSTR MESH_LIST		= "MESH_LIST";
const LPCSTR OUTPUT_DIR		= "OUTPUT_DIR";

/**
 * @brief 緯度経度情報
 */
struct LONLAT
{
	double e_lon;	//!< 経度
	double e_lat;	//!< 緯度

	bool operator < (const LONLAT& lonlat) const {
		bool retVal = false;
		if (e_lon != lonlat.e_lon) {
			if (e_lon < lonlat.e_lon)
				retVal = true;
		} else if (e_lat != lonlat.e_lat) {
			if (e_lat < lonlat.e_lat)
				retVal = true;
		}
		return retVal;
	}

	LONLAT() : e_lon(0.0), e_lat(0.0)
	{
	}
};

/**
 * @brief 建物ビル名称リリースデータレコード情報
 */
struct RECORDINFO
{
	long			e_bldID;		//!< 建物ポリゴンID
	CString			e_bldName;		//!< 建物ビル名称
	IGeometryPtr	e_ipGeometry;	//!< 建物ビル名称ポイント・ジオメトリ（検証用Shape出力用）

	RECORDINFO() : e_bldID(-1)
	{
	}

	RECORDINFO(long bldID, CString bldName, IGeometryPtr ipGeometry) :
		e_bldID(bldID), e_bldName(bldName), e_ipGeometry(ipGeometry)
	{
	}
};

/**
 * @brief 建物ポリゴンIDをキーとしたリリースデータレコード情報のマップ
 */
typedef std::map<long, RECORDINFO> IDRecordMap;

/**
 * @brief IDをキーとしたフィーチャのマップ
 */
typedef std::map<long, IFeaturePtr> IDFeatureMap;

/**
 * @brief フィールド名をキーとしたフィールドインデックスのマップ
 */
typedef std::map<CString, long> FieldIndexMap;

// 共通関数
namespace common_proc
{
	/**
	 * @brief  ワークスペース取得関数
	 * @param  connectInfo [in] ワークスペースへのパス(DB名)
	 * @param  ipWorkspace [out] ワークスペース	
	 * @retval true 成功
	 * @retval false 失敗	
	 */
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace);

	/**
	 * @brief  テーブルオープン関数
	 * @param  tableName [in] テーブル名
	 * @param  ipWorkspace [in] ワークスペース
	 * @param  ipTable [out] テーブル
	 * @retval ture 成功
	 * @retval false 失敗
	 */
	bool GetTable(LPCTSTR tableName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipTable);

	/**
	 * @brief  GeometryDef取得関数
	 * @param  featureClassName [in] フィーチャクラス名
	 * @param  ipFeatureClass [in] フィーチャクラス
	 * @param  ipGeoDef [out] GeometryDef
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetGeometryDef(LPCTSTR featureClassName, const IFeatureClassPtr& ipFeatureClass, IGeometryDefPtr &ipGeoDef);

	/**
	 * @brief  フィールドインデックス取得関数
	 * @param  tableName [in] テーブル名
	 * @param  ipTable [in] テーブル
	 * @param  indeces [out] インデックス情報
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetFieldIndeces(LPCTSTR tableName, const ITablePtr& ipTable, FieldIndexMap* indeces);

	/**
	 * @brief  フィーチャカーソル取得関数（空間検索）
	 * @param  tableName [in] テーブル名
	 * @param  ipFeatureClass [in] フィーチャクラス	
	 * @param  ipGeometry [in] 基準となる形状
	 * @param  spRel [in] リレーション
	 * @param  ipFeatureCursor [out] フィーチャカーソル
	 * @param  whereClause [in] Where句
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetRelFeatureCursor(LPCTSTR tableName, const IFeatureClassPtr& ipFeatureClass, const IGeometryPtr& ipGeometry, const esriSpatialRelEnum spRel, IFeatureCursorPtr& ipFeatureCursor, CString whereClause = _T(""));

	/**
	 * @brief  フィールド値取得関数
	 * @param  ipRow [in] IRowPtr
	 * @param  talbleName [in] テーブル名(エラー出力用)
	 * @param  fieldName [in] フィールド名
	 * @param  indeces [in] フィールドインデックス情報
	 * @param  val [out]　取得した値
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetValue(const _IRowPtr& ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const FieldIndexMap& indeces, CComVariant* val);

	/**
	 * @brief  オブジェクトのエクステントの最大・最小座標取得関数
	 * @param  ipGeometry [in] ジオメトリ
	 * @param  minLonLat [out] エクステントの最小座標
	 * @param  maxLonLat [out] エクステントの最大座標
	 * @retval true 成功	
	 * @retval false 失敗
	 */
	bool GetMinMax(const IGeometryPtr ipGeometry, LONLAT* minLonLat, LONLAT* maxLonLat);

};
