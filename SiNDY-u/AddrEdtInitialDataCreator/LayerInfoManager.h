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

#include <sindy/workspace.h>
#include <TDC/sindylib_core/FieldMap.h>

/**
 * @brief   レイヤ情報管理クラス
 */
class LayerInfoManager
{
public:

	/**
	 * @brief   コンストラクタ
	 */
	LayerInfoManager() {}

	/**
	 * @brief   コンストラクタ
	 * @param   isFeatureClass [in]  フィーチャかどうか（true：フィーチャ、false：テーブル）
	 */
	LayerInfoManager(bool isFeatureClass) : m_isFeatureClass(isFeatureClass) {}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~LayerInfoManager() {}

	/**
	 * @brief   接続情報設定
	 * @param   workspace [in]  ワークスペース
	 * @param   tableName [in]  テーブル名
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setFeatureClassInfo(
			const IFeatureWorkspacePtr& workspace,
			const CString& tableName
		);

	/**
	 * @brief   フィールドマップ取得
	 * @return  フィールドマップ
	 */
	sindy::CFieldMap LayerInfoManager::getFieldMap() const { return m_fieldMap; }

	/**
	 * @brief   テーブル名取得
	 * @return  テーブル名
	 */
	CString getTableName() const { return m_tableName; }

	/**
	 * @brief   フィーチャクラス取得
	 * @return  フィーチャクラス
	 */
	IFeatureClassPtr getFeatureClass() const { return m_featureClass; }

	/**
	 * @brief   テーブル取得
	 * @return  テーブル
	 */
	ITablePtr getTable() const { return ( ( m_isFeatureClass) ? (ITablePtr)m_featureClass : m_table ); }

private:

	IFeatureClassPtr m_featureClass;   //!< フィーチャクラス
	ITablePtr m_table;                 //!< テーブル
	sindy::CFieldMap m_fieldMap;       //!< フィールドマップ
	bool m_isFeatureClass;             //!< フィーチャかどうか（true：フィーチャ、false：テーブル）
	CString m_tableName;               //!< テーブル名
};

