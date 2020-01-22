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

#include "stdafx.h"
#include "LayerInfoManager.h"


bool LayerInfoManager::setFeatureClassInfo(
		const IFeatureWorkspacePtr& workspace,
		const CString& tableName
	)
{
	// テーブル名設定
	m_tableName = tableName;

	IFieldsPtr fields;
	if( m_isFeatureClass ) 	// フィーチャクラスの場合
	{
		if( FAILED(workspace->OpenFeatureClass(CComBSTR(tableName), &m_featureClass)) )
			return false;
		
		if( FAILED(m_featureClass->get_Fields(&fields)) )
			return false;
	}
	else // テーブルの場合
	{
		if( FAILED(workspace->OpenTable(CComBSTR(tableName), &m_table)) )
			return false;

		if( FAILED(m_table->get_Fields(&fields)) )
			return false;
	}

	// FieldMap取得
	m_fieldMap = sindy::CFieldMap(fields);

	return true;
}

