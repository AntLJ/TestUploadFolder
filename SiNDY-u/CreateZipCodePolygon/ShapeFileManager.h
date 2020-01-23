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
#include "ZipCodeDataContainer.h"

class ShapeFileManager
{
public:
	ShapeFileManager(void){}
	~ShapeFileManager(void){}

	/**
	 * @brief ワークスペースのオープン
	 * @param workspaceName [in] ワークスペース名(ディレクトリパス)
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool OpenWorkspace(const CString& workspaceName);

	/**
	 * @brief フィーチャクラス作成
	 * @param fileName [in] シェープファイル名(=フィーチャクラス名)
	 * @param fields [in] フィーチャクラスのフィールド情報
	 * @retval 作成したフィーチャクラス(失敗したらnullptr)
	 */
	IFeatureClassPtr CreateFeatureClass(
		const CString& fileName, IFieldsEditPtr& fields);

	/**
	 * @brief フィーチャクラス作成
	 * @param fileName [in] シェープファイル名(=テーブル名)
	 * @param fields [in] テーブルのフィールド情報
	 * @retval 作成したフィーチャクラス(失敗したらnullptr)
	 */
	ITablePtr CreateTable(const CString& fileName, IFieldsEditPtr& fields);

	/**
	 * @brief シェープフィールドの作成
	 * @param fields [in,out] フィールド情報
	 * @param spatialRef [in] 空間参照
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool CreateShapeField(
		IFieldsEditPtr& fields, const ISpatialReferencePtr& spatialRef);

private:
	IWorkspacePtr m_outWorkspace; //!< ワークスペース
};

