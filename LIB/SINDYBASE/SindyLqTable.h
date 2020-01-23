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

// SindyLqTable.h: SindyLqTable �N���X�̃C���^�[�t�F�C�X
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

	// �w��ID�̃����N���Y������INF_ID�̎擾
	// cStartLink == true  : �w��ID�̃����N���擪�����N�ƂȂ��Ă������
	// cStartLink == false : �w��ID�̃����N���܂܂�Ă���΂ǂ�ł��n�j
	virtual HRESULT SearchByLinkID(long cLinkID, bool cStartLink, std::set<long> *cInfID);

	// �w�胁�b�V���ɐ擪�����N���܂ނ��̂���������(cStart == true)
	// �����N���w�胁�b�V�����Ɋ܂܂�Ă�����̂���������(cStart == false)
	virtual HRESULT SearchByMeshCode(long cMeshCode, bool cStart, std::set<long> *cInfID);

protected:
	GDBLQLIST mLinkQueue;
	GDB2OBJLIST mLinkList;	// �����NID�ɂ�郊�X�g

protected:
	virtual HRESULT Store(_ICursorPtr ipCursor);
	virtual HRESULT Store(const std::set<long> &cList);
};

#endif // !defined(AFX_SINDYLQTABLE_H__F8FA9737_F9D1_4CAF_A00B_A4286DDD6FD8__INCLUDED_)
