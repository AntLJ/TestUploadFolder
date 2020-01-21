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

// INRoadLinkTraceCondition.h: INRoadLinkTraceCondition �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INROADLINKTRACECONDITION_H__206779E5_7942_417D_B97C_1DD60B17001C__INCLUDED_)
#define AFX_INROADLINKTRACECONDITION_H__206779E5_7942_417D_B97C_1DD60B17001C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INRoadInfoSet.h"

class INRoadLinkTraceCondition  
{
public:
	INRoadLinkTraceCondition();
	virtual ~INRoadLinkTraceCondition();

	virtual bool exit(INROADINFO *cInfo, IN2Node *cNode) = 0;
	virtual bool trace(INROADINFO *cInfo, IN2Link *cLink, IN2Node *cNode) = 0;

	virtual void push_back(INROADINFO *cInfo, IN2Link *cLink)
	{
		ePassedLink.insert(LinkID(cInfo->eMesh, cLink->eID1, cLink->eID2));
		eLinkList.push_back(LinkID(cInfo->eMesh, cLink->eID1, cLink->eID2));
	}
	virtual void pop_back() { eLinkList.pop_back(); }

	virtual bool Passable(INROADINFO *cInfo, IN2Link *cLink, IN2Node *cNode);
	virtual bool TurnCorner(INROADINFO *cInfo, IN2Link *cLink);

public:
	struct LinkID
	{
		long		eMesh;
		long		eFrom;
		long		eTo;
		LinkID():eMesh(0),eFrom(0),eTo(0){}
		LinkID(long m, long f, long t):eMesh(m),eFrom(f),eTo(t){}
		virtual ~LinkID(){}
	};

	struct lessLinkID : public std::binary_function<LinkID,LinkID,bool>
	{
		bool operator ()(const LinkID &cLinkID1, const LinkID &cLinkID2) const
		{
			return (cLinkID1.eMesh != cLinkID2.eMesh) ? cLinkID1.eMesh < cLinkID2.eMesh : (cLinkID1.eFrom != cLinkID2.eFrom) ? cLinkID1.eFrom < cLinkID2.eFrom : cLinkID1.eTo < cLinkID2.eTo;
		}
	};

	std::set<LinkID,lessLinkID> ePassedLink;	// �����p
	std::vector<LinkID> eLinkList;
};

/**
 *	IN���H�̒ǐՂ����{����
 *	@param	INROADINFO					*cRoadInfo			���H�f�[�^�Z�b�g(in)
 *	@param	IN2Node						*cNode				���H�m�[�h(in)
 *	@param	INRoadLinkTraceCondition	*cTraceCondition	�ǐՏ���(in)
 *	@param	INRoadInfoSet				*cRoadInfoSet		�J�����g�ȊO�̃t�@�C���Z�b�g(in)
 *	@return	bool		true = ���B�ł����Afalse = ���B�ł��Ȃ�����
 */
extern bool road_trace(INROADINFO *cRoadInfo, IN2Node *cNode, INRoadLinkTraceCondition *cTraceCondition, INRoadInfoSet *cRoadInfoSet = 0);

#endif // !defined(AFX_INROADLINKTRACECONDITION_H__206779E5_7942_417D_B97C_1DD60B17001C__INCLUDED_)
