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

// SiNDYTablesBuffer.h: SiNDYTablesBuffer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTABLESBUFFER_H__A16DFF1C_C079_4EA0_B535_5AD25C1B45E8__INCLUDED_)
#define AFX_SINDYTABLESBUFFER_H__A16DFF1C_C079_4EA0_B535_5AD25C1B45E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "SiNDYTableFinder.h"
#include "SiNDYTableName.h"

class SiNDYTablesBuffer : public SiNDYTableFinder
{
public:
	typedef std::map<CString, ITablePtr> TableMap;
	typedef SiNDYTableName TableName;

// 構築/消滅
	SiNDYTablesBuffer();
	SiNDYTablesBuffer(IFeatureWorkspacePtr ipWorkspace);
	virtual ~SiNDYTablesBuffer();

// 設定/取得
	void setWorkspace(IFeatureWorkspacePtr ipWorkspace);

	ITablePtr getTable(CString strTableName);
	ITablePtr getTable(const TableName& rTableName);

// 純粋仮想関数の解決
	virtual ITablePtr getTable(LPCTSTR lpszTableName);

// 仮想関数のオーバーライド
	virtual IWorkspacePtr getWorkspace() const { return m_ipFeatureWorkspace; }

private:
	IFeatureWorkspacePtr m_ipFeatureWorkspace;
	TableMap m_cTableMap;
};

#endif // !defined(AFX_SINDYTABLESBUFFER_H__A16DFF1C_C079_4EA0_B535_5AD25C1B45E8__INCLUDED_)
