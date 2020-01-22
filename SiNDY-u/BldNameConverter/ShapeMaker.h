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
#include "LogSys.h"
#include "common.h"

/**
 * Shapeファイル作成クラス
 */
class ShapeMaker
{
public:
	ShapeMaker(const CString& outRootDir, bool isTest, const IGeometryDefPtr ipGeometryDef);
	~ShapeMaker(void);

	/**
	 * @brief  Shape変換
	 * @param  cnvInfo [in] 出力情報
	 * @param  cityMesh [in] 処理対象メッシュ
	 * @retval true 正常終了
	 * @retval false 異常終了
	 */
	bool makeShape(const IDRecordMap& cnvInfo, long cityMesh);

private:
	/**
	 * @brief  出力ディレクトリ作成
	 * @param  cityMesh [in] 都市地図メッシュ
	 * @param  shpName [in] Shapeファイル名
	 * @param  outputDir [out] 出力ディレクトリ
	 * @retval true 正常終了
	 * @retval false 異常終了
	 */
	bool makeDirAndDelFile(long cityMesh, LPCTSTR shpName, CString* outputDir);

	/**
	 * @brief  出力dbf作成
	 * @param  dbfName [in] dbfファイル名
	 * @param  ipWorkspace [in] ワークスペース
	 * @param  ipTable [out] テーブル
	 * @retval ture 成功	
	 * @retval false 失敗
	 */
	bool makeVacuityDbf(LPCTSTR dbfName, const IWorkspacePtr ipWorkspace, ITablePtr& ipTable);

	/**
	 * @brief  出力Shape作成
	 * @param  shpName [in] shapeファイル名
	 * @param  ipWorkspace [in] ワークスペース
	 * @param  ipFeatureClass [in] フィーチャクラス
	 * @retval true 成功
	 * @retval false 失敗
	 *
	 */
	bool makeVacuityShape(LPCTSTR shpName, const IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass);

	/**
	 * @brief  属性フィールド群作成関数
	 * @parma  fileName [in] shape(dbf)ファイル名
	 * @param  ipFields [in] IFieldsEditPtr
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeAttrFields(LPCTSTR fileName, IFieldsEditPtr& ipFileds);

	/**
	 * @brief  形状フィールド作成関数
	 * @param  shpFileName [in] 作成Shapeファイル名
	 * @param  fieldName [in] フィールド名
	 * @param  fieldType [in] フィールドタイプ
	 * @param  ipGeoField [out] 作成したフィールドオブジェクト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeShapeField(LPCTSTR shpFileName, LPCTSTR fieldName, esriFieldType fieldType, IFieldPtr &ipGeoField);

	/**
	 * @brief  属性フィールド作成関数
	 * @param  shpName [in] 作成Shape名
	 * @param  fieldName [in] フィールド名
	 * @param  fieldType [in] フィールドタイプ
	 * @param  editalbe [in] 編集可否
	 * @param  nullable [in] NULL許可
	 * @param  fieldLength [in] フィールド長
	 * @param  ipField [out] 作成フィールド
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeAttrField(LPCTSTR shpName, LPCTSTR fieldName, esriFieldType fieldType, VARIANT_BOOL editable, VARIANT_BOOL nullable, long fieldLength, IFieldPtr &ipField);

	/**
	 * @brief  レコード作成関数
	 * @param  dbfName [in] dbf名
	 * @param  cnvInfo [in] 作成レコード情報
	 * @param  ipTable [out] テーブル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool importRecord(LPCTSTR dbfName, const IDRecordMap& cnvInfo, ITablePtr& ipTable);

	/**
	 * @brief  レコード作成関数
	 * @param  shpName [in] Shape名
	 * @param  cnvInfo [in] 作成レコード情報
	 * @param  ipFeatureClass [in] フィーチャクラス
	 * @retval true 成功
	 * @retval false 失敗
	 */
 	bool importRecord(LPCTSTR shpName, const IDRecordMap& cnvInfo, IFeatureClassPtr& ipFeatureClass);

private:

	CString m_outRootDir;	//!< 出力先ディレクトリ名
	bool m_isTest;			//!< テストモード
	const IGeometryDefPtr m_ipGeometryDef;	//!< 出力するShapeファイルのジオメトリ定義
};
