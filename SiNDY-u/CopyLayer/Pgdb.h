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

// Pgdb.h: Pgdb クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PGDB_H__2EA6C417_EF3A_48D9_884C_9231F236F13C__INCLUDED_)
#define AFX_PGDB_H__2EA6C417_EF3A_48D9_884C_9231F236F13C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Pgdb  
{
public:
	bool execCopyLayer(LONG cMeshCode);
	bool init(LPCTSTR cSrcDir, LPCTSTR cTgtDir, LPCTSTR cSuffix, bool cForceMode);
	Pgdb();
	virtual ~Pgdb();

protected:
	LONG m_OperationIndex[2];
	LONG m_DateIndex[2];
	LONG m_MachineIndex[2];
	LONG m_UserIndex[2];
	LONG m_ConnectIndex[2];
	LONG m_ClassIndex[2];
	LONG m_NameIndex[2];
	IWorkspacePtr fnOpenLocalDatabase(LPCTSTR cBasePath);
	LONG m_DataIndex;
	LONG m_MeshCode;
	bool m_ForceMode;
	CString m_TgtDir;
	CString m_SrcDir;
	ITablePtr fnOpenExportInfoTable(IWorkspacePtr ipWorkspace, int cMode);
	bool fnSetExportInfo(ITablePtr ipSrcTbl, ITablePtr ipTgtTbl, int cIndex);
	_IRowPtr fnGetRow(ITablePtr ipTable, LPCTSTR cLayerName);
	bool fnCopyInfo(_IRowPtr ipTgtRow, _IRowPtr ipSrcRow);
};

#endif // !defined(AFX_PGDB_H__2EA6C417_EF3A_48D9_884C_9231F236F13C__INCLUDED_)
