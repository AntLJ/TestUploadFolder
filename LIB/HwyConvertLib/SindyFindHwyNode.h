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

// SindyFindHwyNode.h: SindyFindHwyNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
/**
 * @file HwyConvertLib�J���ɔ����ASindyHighwayCheck�̃\�[�X��SindyHighwayConvert�ł��g���Ă����̂��������邽�߁A
 *       ���̋��L������SindyHighwayCheck����HwyConvertLib�Ɉڊǂ����B
 */

#if !defined(AFX_SINDYFINDHWYNODE_H__3544C431_37C6_42C8_A48D_A8B05E8143E1__INCLUDED_)
#define AFX_SINDYFINDHWYNODE_H__3544C431_37C6_42C8_A48D_A8B05E8143E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayTrace.h"
#include <vector>

typedef std::vector<IFeaturePtr> HwyNodeArray;

class SindyFindHwyNode : public SindyHighwayTrace  
{
public:
	SindyFindHwyNode(SindyHighwayInfo *cHwyInfo);
	virtual ~SindyFindHwyNode();

	virtual void set_facil_code(long cRoadCode, long cRoadSeq) { mRoadCode = cRoadCode; mRoadSeq = cRoadSeq; }

	virtual HRESULT exit(SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit);

	const HwyNodeArray& getFoundHwyNodeArray(void) { return mFoundHwyNodeArray; }

protected:
	SindyHighwayInfo *mpHwyInfo;
	long mRoadCode;
	long mRoadSeq;
	HwyNodeArray mFoundHwyNodeArray;		// �ǐՏI�_�̃n�C�E�F�C�m�[�h�̏W��
};

#endif // !defined(AFX_SINDYFINDHWYNODE_H__3544C431_37C6_42C8_A48D_A8B05E8143E1__INCLUDED_)
