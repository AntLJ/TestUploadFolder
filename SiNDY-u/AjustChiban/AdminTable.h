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

// AdminTable.h: AdminTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINTABLE_H__DAAFC3E1_C70A_4A3A_8A04_9AC1BC94C90D__INCLUDED_)
#define AFX_ADMINTABLE_H__DAAFC3E1_C70A_4A3A_8A04_9AC1BC94C90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef std::set<CString> CODE_SET;
class AdminTable  
{
public:
	bool next(CString&);
	bool init(IWorkspacePtr ipWorkspace, LPCTSTR cGyosei, LPCTSTR cWhere);
	AdminTable();
	virtual ~AdminTable();
private:
	CODE_SET m_CityCodeSet;
	CODE_SET::iterator m_CurrentCode;
};

#endif // !defined(AFX_ADMINTABLE_H__DAAFC3E1_C70A_4A3A_8A04_9AC1BC94C90D__INCLUDED_)
