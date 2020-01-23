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

// SindyLinkQueue.h: SindyLinkQueue �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLINKQUEUE_H__4E160C85_5340_4790_BB9D_476A31B69FFD__INCLUDED_)
#define AFX_SINDYLINKQUEUE_H__4E160C85_5340_4790_BB9D_476A31B69FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyInfTable.h"

class SindyLinkQueue : public SindyInfTable  
{
public:
	SindyLinkQueue(SindyDBConnection *cDBConnection, const std::string &cInfName, const std::string &cLqName);
	virtual ~SindyLinkQueue();

	virtual HRESULT LoadByMeshCode(long cMeshCode, bool cStart = true);
	virtual HRESULT LoadByLinkID(const std::set<long> &cIdList);

	// ipInfRow���������N��̎擾
	virtual HRESULT SearchLinkQueue(_IRowPtr ipInfRow, GDBOBJLIST *cLqList);

	// aLinkID���܂�INF���擾����ꍇ �� cStartLink == false
	// aLinkID���擪�ł���INF���擾����ꍇ �� cStartLink == true
	virtual HRESULT SearchByLinkID(long cLinkID, bool cStartLink, GDBOBJLIST *cInfList);

protected:
	SindyLqTable mLqTable;
};

#endif // !defined(AFX_SINDYLINKQUEUE_H__4E160C85_5340_4790_BB9D_476A31B69FFD__INCLUDED_)
