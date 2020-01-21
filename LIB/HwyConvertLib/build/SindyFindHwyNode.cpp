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

// SindyFindHwyNode.cpp: SindyFindHwyNode �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SindyFindHwyNode.h"


SindyFindHwyNode::SindyFindHwyNode(SindyHighwayInfo *cHwyInfo) :
SindyHighwayTrace(),
mpHwyInfo(cHwyInfo),
mRoadCode(0),
mRoadSeq(0)
{
	set_direction(sindy::rnw::relation::back);
}

SindyFindHwyNode::~SindyFindHwyNode()
{

}

HRESULT SindyFindHwyNode::exit(SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit)
{
	HRESULT hr;
	const char *func = "SindyFindHwyNode::exit";

	*cExit = false;

	IGeometryPtr ipGeometry = ipNode->GetShapeCopy();

	// �n�C�E�F�C�m�[�h�̌���
	GDBPNTRANGE aRange;
	if (FAILED(hr = mpHwyInfo->eHighwayNode.SearchByPoint((IPointPtr)ipGeometry, &aRange))) return hr;

	for (GDBPNTITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipHwyNode = aIter->second;

		// ����|�C���g�łȂ���΂Ȃ�Ȃ�
		CComVariant vaInOut;
		if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::INOUT_C, &vaInOut))) return hr;
		vaInOut.ChangeType(VT_I4);
		if (vaInOut.lVal != sindy::highway_node::inout_exit) continue;

		// �H���R�[�h�ƘH�����V�[�P���X�̃`�F�b�N
		// �H���R�[�h�܂��͘H�����V�[�P���X�̂ǂ��炩���Z�b�g����Ă�����A
		// �`�F�b�N�Ɏg�p���邽�߃n�C�E�F�C�m�[�h�̘H���R�[�h�E�H�����V�[�P���X���擾
		long aRoadCode = 0, aRoadSeq = 0;
		if (mRoadCode != 0 || mRoadSeq != 0)
		{
			if (FAILED(hr = mpHwyInfo->GetFacilCode(ipHwyNode, &aRoadCode, &aRoadSeq))) return hr;
		}
		bool aResult = true;
		// �H���R�[�h���Z�b�g����Ă���΁A�n�C�E�F�C�m�[�h�̘H���R�[�h�͂���ƈ�v���Ȃ���΂Ȃ�Ȃ�
		if (mRoadCode != 0)
		{
			if (aRoadCode != mRoadCode)
			{
				aResult = false;
			}
		}
		// �H�����V�[�P���X���Z�b�g����Ă���΁A�n�C�E�F�C�m�[�h�̘H�����V�[�P���X�͂���ƈ�v���Ȃ���΂Ȃ�Ȃ�
		if (mRoadSeq != 0)
		{
			if (aRoadSeq != mRoadSeq)
			{
				aResult = false;
			}
		}

		// �����𖞂����n�C�E�F�C�m�[�h������������L��
		if (aResult)
		{
			mFoundHwyNodeArray.push_back(ipHwyNode);
//			mHwyNode = ipHwyNode;
//			*cExit = true;
//			return S_OK;
		}
	}

	return S_OK;
}
