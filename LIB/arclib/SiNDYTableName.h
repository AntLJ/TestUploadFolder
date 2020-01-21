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

// SiNDYTableName.h: SiNDYTableName クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTABLENAME_H__FFE3DDBD_28A5_403F_8F8F_D356795223A7__INCLUDED_)
#define AFX_SINDYTABLENAME_H__FFE3DDBD_28A5_403F_8F8F_D356795223A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SiNDYTableName  
{
public:
	SiNDYTableName();
	SiNDYTableName(_IRow* ipRow);
	SiNDYTableName(ITable* ipTable);
	SiNDYTableName(IFeatureClass* ipFeatureClass);
	SiNDYTableName(IDataset* ipDataset);
	SiNDYTableName(IDatasetName* ipDatasetName);
	SiNDYTableName(const CString& rName);

	const CString& fullNameString() const { return m_strFullName; }
	const CString& tableNameString() const { return m_strTableName; }
	const CString& ownerNameString() const { return m_strOwnerName; }

	CString complementOwnerName(LPCTSTR lpszTableName) const;
	CString getInfFullNameString() const;
	CString getLqFullNameString() const;
	CString getLqRfFieldNameString() const;

	bool isInfTableName() const { return m_bIsInfTableName; }
	bool isLqTableName() const { return m_bIsLqTableName; }
	bool isRoadLinkTableName() const { return m_bIsRoadLinkTableName; }

	bool isSameTable(const SiNDYTableName& rTableName) const;

private:
	bool setTableName(_IRow* ipRow);
	bool setTableName(IDataset* ipDataset);
	bool setTableName(IDatasetName* ipDatasetName);
	bool setTableName(const CString& rName);

	CString m_strFullName;
	CString m_strOwnerName;
	CString m_strTableName;

	bool m_bIsInfTableName;
	bool m_bIsLqTableName;
	bool m_bIsRoadLinkTableName;
};

#endif // !defined(AFX_SINDYTABLENAME_H__FFE3DDBD_28A5_403F_8F8F_D356795223A7__INCLUDED_)
