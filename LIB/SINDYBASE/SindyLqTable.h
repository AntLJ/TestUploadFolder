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

// SindyLqTable.h: SindyLqTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLQTABLE_H__F8FA9737_F9D1_4CAF_A00B_A4286DDD6FD8__INCLUDED_)
#define AFX_SINDYLQTABLE_H__F8FA9737_F9D1_4CAF_A00B_A4286DDD6FD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"
#include "SindyTools.h"

typedef std::map<long,GDBOBJLIST>		GDBLQLIST;
typedef GDBLQLIST::iterator				GDBLQITER;
typedef std::pair<long,GDBOBJLIST>		GDBLQSET;

class SindyLqTable : public SindyTable  
{
public:
	SindyLqTable(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyLqTable();

	virtual HRESULT Load();
	virtual HRESULT LoadByMeshCode(long cMeshCode, bool cStart = true);
	virtual HRESULT LoadByInfID(const std::set<long> &cInfID);
	virtual HRESULT SearchByInfID(long cInfID, GDBOBJLIST *cList);

	// 指定IDのリンクが該当するINF_IDの取得
	// cStartLink == true  : 指定IDのリンクが先頭リンクとなっているもの
	// cStartLink == false : 指定IDのリンクが含まれていればどれでもＯＫ
	virtual HRESULT SearchByLinkID(long cLinkID, bool cStartLink, std::set<long> *cInfID);

	// 指定メッシュに先頭リンクを含むものを検索する(cStart == true)
	// リンクが指定メッシュ内に含まれているものを検索する(cStart == false)
	virtual HRESULT SearchByMeshCode(long cMeshCode, bool cStart, std::set<long> *cInfID);

protected:
	GDBLQLIST mLinkQueue;
	GDB2OBJLIST mLinkList;	// リンクIDによるリスト

protected:
	virtual HRESULT Store(_ICursorPtr ipCursor);
	virtual HRESULT Store(const std::set<long> &cList);
};

#endif // !defined(AFX_SINDYLQTABLE_H__F8FA9737_F9D1_4CAF_A00B_A4286DDD6FD8__INCLUDED_)
