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

// SiNDYTablesBuffer.cpp: SiNDYTablesBuffer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiNDYTablesBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SiNDYTablesBuffer.cpp: SiNDYTablesBuffer クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SiNDYTablesBuffer::SiNDYTablesBuffer()
{
}

SiNDYTablesBuffer::SiNDYTablesBuffer(IFeatureWorkspacePtr ipWorkspace) :
m_ipFeatureWorkspace(ipWorkspace)
{
}

SiNDYTablesBuffer::~SiNDYTablesBuffer()
{

}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void SiNDYTablesBuffer::setWorkspace(IFeatureWorkspacePtr ipWorkspace)
{
	m_cTableMap.clear();
	m_ipFeatureWorkspace = ipWorkspace;
}

ITablePtr SiNDYTablesBuffer::getTable(LPCTSTR lpszTableName)
{
	return getTable(CString(lpszTableName));
}

/**
 * @brief テーブルを取得する
 *
 * @note フィーチャクラスの名称を指定した場合、フィーチャクラスを取得する。
 * @note 一度取得したテーブル（フィーチャクラス）はインスタンスにキャッシュされる。
 *
 * @param strTableName [in] テーブル名
 * @return 指定したテーブル名を持つテーブル。取得できなかった場合NULL。
 */
ITablePtr SiNDYTablesBuffer::getTable(CString strTableName)
{
	if(m_ipFeatureWorkspace == 0)
		return 0;

	ITablePtr ipTable;

	// 大文字で整列している
	strTableName.MakeUpper();
	TableMap::const_iterator it = m_cTableMap.find(strTableName);

	// キャッシュを検索
	if(it != m_cTableMap.end()) {
		// キャッシュされていたらそれを
		ipTable =it->second;
	}
	else {
		// されていなかったら、新しく開く
		IFeatureClassPtr ipFeatureClass;
		if(m_ipFeatureWorkspace->OpenFeatureClass(CComBSTR(strTableName), &ipFeatureClass) == S_OK) {
			ipTable = ipFeatureClass;
		}
		else {
			m_ipFeatureWorkspace->OpenTable(CComBSTR(strTableName), &ipTable);
		}
		m_cTableMap[strTableName] = ipTable;
	}

	return ipTable;
}

ITablePtr SiNDYTablesBuffer::getTable(const TableName& rTableName)
{
	return getTable(rTableName.fullNameString());
}
