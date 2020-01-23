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

#define WALK_ID 100000000

/**
 * @brief OBJECTIDとDIVIDでユニークにさせるための構造体
 */
struct UNIQID{
	UNIQID(long lOID, long lDivID){
		this->eID = lOID;
		this->eDivID = lDivID;
	}
	UNIQID(){
		this->eID = 0;
		this->eDivID = 0;
	}
	long eID;
	long eDivID;
	bool operator<(const UNIQID uniqID) const {
		return (this->eID != uniqID.eID) ? (this->eID < uniqID.eID) : (this->eDivID < uniqID.eDivID);
	}
	bool operator==(const UNIQID uniqID) const {
		return (this->eID == uniqID.eID && this->eDivID == uniqID.eDivID);
	}
	bool operator!=(const UNIQID uniqID) const {
		return (this->eID != uniqID.eID || this->eDivID != uniqID.eDivID);
	}
};

/**
 * @brief 経緯度＋ノードID確保用の構造体
 */
struct LONLAT{
	double eLon;
	double eLat;
	UNIQID eNodeID;

	bool operator<(const LONLAT LonLat) const {
		return (this->eLon != LonLat.eLon) ? (this->eLon < LonLat.eLon) : (this->eLat < LonLat.eLat);
	}
	bool operator==(const LONLAT LonLat) const {
		return (this->eLon == LonLat.eLon && this->eLat == LonLat.eLat);
	}
	bool operator!=(const LONLAT LonLat) const {
		return (this->eLon != LonLat.eLon || this->eLat != LonLat.eLat);
	}
};

// チェックモード
namespace check_mode {
	enum ECode {
		kNone        =  0, //!< 未設定
		kSegmentAttr =  1, //!< リンク内属性分割
		kWalkNode    =  2, //!< 歩行者ノード分割
		kHeightNode  =  3, //!< 高さノード分割

		kIntegrity   = 99, //!< RWL,RWNのNW整合チェックモード（非公開）
	};
}

/*
 * @brief 形状の緯度経度情報取得関数
 * @param strTableName [in] 取得対象形状のテーブル名
 * @param ipGeom       [in] 取得対象の形状
 * @param vecLonLat    [out] 取得した緯度経度情報
 * @return true 成功
 * @return false 失敗
 */
bool GetLonLatData(const CString& strTableName, IGeometryPtr ipGeom, std::vector<LONLAT>& vecLonLat);

/*
 * @brief ワークスペース取得関数
 * @param strConnectInfo [in] ワークスペースへのパス
 * @param ipWorkspace    [out] 取得したワークスペース
 * @return true 成功
 * @return false 失敗
 */
bool GetWorkspace(const CString& strConnectInfo, IWorkspacePtr& ipWrokspace); 

/*
 * @breif フィーチャクラス取得関数
 * @param strTableName   [in] レイヤ名
 * @param ipWorkspace    [in] ワークスペース
 * @param ipFeatureClass [out] 取得したフィーチャクラス
 * @return true 成功
 * @return false 失敗
 */
bool GetFeatureClass(const CString& strTableName, IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass);

/*
 * @breif テーブル取得関数
 * @param strTableName   [in] レイヤ名
 * @param ipWorkspace    [in] ワークスペース
 * @param ipTableClass   [out] 取得したテーブル
 * @return true 成功
 * @return false 失敗
 */
bool GetTable(const CString& strTableName, IWorkspacePtr ipWorkspace, ITablePtr& ipTable);

/*
 * @brief フィールド数取得関数
 * @param strTableName   [in] レイヤ名(エラー出力用)
 * @param ipFields       [in] フィールド群
 * @param lCount         [out] 取得したフィールド数
 * @return true 成功
 * @return false 失敗
 */
bool GetFieldCount(const CString& strTableName, IFieldsPtr ipFields, long& lCount);

/*
 * @brief フィールド群取得関数
 * @param strTableName   [in] レイヤ名(エラー出力用)
 * @param ipFeatureClsas [in] フィーチャクラス
 * @param ipFields       [out] 取得したフィールド群
 * @return true 成功
 * @return false 失敗
 */
bool GetFields(const CString& strTableName, IFeatureClassPtr ipFeatureClass, IFieldsPtr& ipFields);

/*
 * @brief フィールド群取得関数
 * @param strTableName   [in] レイヤ名(エラー出力用)
 * @param ipTable        [in] フィーチャクラス
 * @param ipFields       [out] 取得したフィールド群
 * @return true 成功
 * @return false 失敗
 */
bool GetFields(const CString& strTableName, ITablePtr ipTable, IFieldsPtr& ipFields);

/*
 * @brief フィールド取得関数
 * @param strTableName   [in] レイヤ名(エラー出力用)
 * @param ipFields       [in] フィールド群
 * @param lIndex         [in] フィールドインデックス
 * @param ipField        [out] 取得したフィールド
 * @return true 成功
 * @return false 失敗
 */
bool GetField(const CString& strTableName, IFieldsPtr ipFields, long lIndex, IFieldPtr& ipField);

/*
 * @brief 空間フィルタ取得関数
 * @param ipSpGeom         [in] 空間検索形状
 * @param ipSpFilter       [out] 取得した空間フィルタ
 * @return true 成功
 * @return false 失敗
 */
bool GetSpatialFilter(IGeometryPtr ipSpGeom, ISpatialFilterPtr& ipSpFilter);

/*
 * @brief フィーチャカーソル取得関数
 * @param strTableName     [in] レイヤ名(エラー出力用)
 * @param ipFeatureClass   [in] フィーチャクラス
 * @param ipSpFilter       [in] 空間フィルタ
 * @param ipFeatureCursor  [out] 取得したフィーチャカーソル
 * @return true 成功
 * @return false 失敗
 */
bool GetFeatureCursor(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, IFeatureCursorPtr& ipFeatureCursor);

/*
 * @brief カーソル取得関数
 * @param strTableName     [in] レイヤ名(エラー出力用)
 * @param ipTable          [in] テーブル
 * @param ipQFilter        [in] 属性フィルタ
 * @param ipCursor         [out] 取得したカーソル
 * @return true 成功
 * @return false 失敗
 */
bool GetCursor(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, _ICursorPtr& ipCursor);

/*
 * @brief フィーチャ数取得関数
 * @param strTableName     [in] レイヤ名(エラー出力用)
 * @param ipFeatureClass   [in] フィーチャクラス
 * @param ipSpFilter       [in] 空間フィルタ
 * @param lCount           [out] 取得したフィーチャ数
 * @return true 成功
 * @return false 失敗
 */
bool GetFeatureCount(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialFilterPtr ipSpFilter, long& lCount);

/*
 * @brief レコード数取得関数
 * @param strTableName     [in] レイヤ名(エラー出力用)
 * @param ipFeatureClass   [in] フィーチャクラス
 * @param ipQFilter        [in] 属性フィルタ
 * @param lCount           [out] 取得したレコード数
 * @return true 成功
 * @return false 失敗
 */
bool GetRowCount(const CString& strTableName, ITablePtr ipTable, IQueryFilterPtr ipQFilter, long& lCount);

/*
 * @brief フィールド値取得関数
 * @param strTableName [in] レイヤ名(エラー出力用)
 * @param strFieldName [in] フィールド名
 * @param ipFeature    [in] フィーチャ
 * @param lIndex       [in] フィールドインデックス
 * @param vaValue      [out] 取得した値
 * @return true 成功
 * @return false 失敗
 */
bool GetValue(const CString& strTableName, const CString& strFieldName, IFeaturePtr ipFeature, long lIndex, CComVariant& val);

/*
 * @brief フィールド値取得関数
 * @param strTableName [in] レイヤ名(エラー出力用)
 * @param strFieldName [in] フィールド名
 * @param ipRow        [in] レコード
 * @param lIndex       [in] フィールドインデックス
 * @param vaValue      [out] 取得した値
 * @return true 成功
 * @return false 失敗
 */
bool GetValue(const CString& strTableName, const CString& strFieldName, _IRowPtr ipRow, long lIndex, CComVariant& val);

/*
 * @brief 形状取得関数
 * @param strTableName [in] レイヤ名(エラー出力用)
 * @param ipFeature    [in] フィーチャ
 * @param ipGeometry   [out] 取得した形状
 * @return true 成功
 * @return false 失敗
 */
bool GetGeometry(const CString& strTableName, const IFeaturePtr ipFeature, IGeometryPtr &ipGeometry);

/*
 * @brief ポイント数取得関数
 * @param strTableName [in] レイヤ名(エラー出力用)
 * @param ipPointCol   [in] ポイントコレクション
 * @param lCount       [out] ポイント数
 * @return true 成功
 * @return false 失敗
 */
bool GetPointCount(const CString& strTableName, IPointCollectionPtr ipPointCol, long& lCount);

/*
 * @brief WKSポイント群取得関数
 * @param strTableName [in] レイヤ名(エラー出力用)
 * @param ipPointCol   [in] ポイントコレクション
 * @param lSize        [in] ポイント数
 * @param vecWKSPoint  [out] 取得したWKSポイント群
 * @return true 成功
 * @return false 失敗
 */
bool GetQueryWKSPoints(const CString& strTableName, IPointCollectionPtr ipPointCol, long lSize, std::vector<WKSPoint>& vecWKSPoint);

/*
 * @brief メッシュ形状取得関数
 * @param strMeshCode   [in] メッシュコード
 * @param ipMeshClass   [in] メッシュクラス
 * @param ipMeshGeom    [out] 取得したメッシュ形状
 * @return true 成功
 * @return false 失敗
 */
bool GetMeshGeom(const CString& strMeshCode, IFeatureClassPtr ipMeshClass, IGeometryPtr& ipMeshGeom);
