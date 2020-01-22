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
 * @brief フィールド取得関数
 * @param strTableName   [in] レイヤ名(エラー出力用)
 * @param strFieldName   [in] フィールド名
 * @param ipFields       [in] フィールド群
 * @param lIndex         [out] 取得したフィールドインデックス
 * @return true 成功
 * @return false 失敗
 */
bool GetFieldIndex(const CString& strTableName, const CString& strFieldName, IFieldsPtr ipFields, long& lIndex);

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

/*
 * @brief ワークスペース取得関数
 * @param strTableName      [in] テーブル名（エラー出力用）
 * @param ipFeatureClass    [in] 空間参照取得元
 * @param ipSpRef           [out] 取得した空間参照
 * @return true 成功
 * @return false 失敗
 */
bool GetSpatialReference(const CString& strTableName, IFeatureClassPtr ipFeatureClass, ISpatialReferencePtr& ipSpRef);

/*
 * @brief ワークスペース取得関数
 * @param strTableName      [in] テーブル名（エラー出力用）
 * @param ipFeatureClass    [in] フィーチャ取得元
 * @param lOID              [in] 取得対象のオブジェクトID
 * @param ipFeature         [out] 取得したフィーチャ
 * @return true 成功
 * @return false 失敗
 */
bool GetFeature(const CString& strTableName, IFeatureClassPtr ipFeatureClass, long lOID, IFeaturePtr& ipFeature);

/*
 * @brief 空間参照付与関数
 * @param strTableName      [in] テーブル名（エラー出力用）
 * @param ipSpRef           [in] 空間参照
 * @param ipGeom            [in,out] 付与対象形状
 * @return true 成功
 * @return false 失敗
 */
bool SetSpatialReference(const CString& strTableName, ISpatialReferencePtr ipSpRef, IGeometryPtr& ipGeom);

/*
 * @brief OID取得関数
 * @param strTableName      [in] テーブル名（エラー出力用）
 * @param ipFeature         [in] 取得対象フィーチャ
 * @param lOID              [out] 取得したOID
 * @return true 成功
 * @return false 失敗
 */
bool GetOID(const CString& strTableName, IFeaturePtr ipFeature, long& lOID);
