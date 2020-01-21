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

// KKDLoad.h: KKDLoad �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

/**
 * @file
 * @brief KKD�f�[�^�����ǂݍ��݃N���X��`�w�b�_�t�@�C��
 */
#if !defined(AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_)
#define AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KKDCodeCommunity.h"
#include "KKDLoadProtocol.h"
#include "KKDLayer.h"

/**
 * @brief KKD�f�[�^�����ǂݍ��݃N���X
 */
template <
	typename TRegulationRoadNodeLayer,
	typename TRegulationRoadLinkLayer,
	typename TLaneRegulationRoadNodeLayer,
	typename TLaneRegulationRoadLinkLayer,
	typename TLaneRegulationIntersectionNodeLayer
>
class KKDLoadT : public KKDLoadProtocol  
{
public:
// �擾
	typedef TRegulationRoadNodeLayer KKDRegulationRoadNodeLayer;
	typedef TRegulationRoadLinkLayer KKDRegulationRoadLinkLayer;
	typedef TLaneRegulationRoadNodeLayer KKDLaneRegulationRoadNodeLayer;
	typedef TLaneRegulationRoadLinkLayer KKDLaneRegulationRoadLinkLayer;
	typedef TLaneRegulationIntersectionNodeLayer KKDLaneRegulationIntersectionNodeLayer;

	const KKDAdminLayer& adminLayer() const { return mAdminLayer; }
	const KKDRegulationRoadNodeLayer& regulationRoadNodeLayer() const { return mRegulationRoadNodeLayer; }
	const KKDRegulationRoadLinkLayer& regulationRoadLinkLayer() const { return mRegulationRoadLinkLayer; }
	const KKDLaneRegulationRoadNodeLayer& laneRegulationRoadNodeLayer() const { return mLaneRegulationRoadNodeLayer; }
	const KKDLaneRegulationRoadLinkLayer& laneRegulationRoadLinkLayer() const { return mLaneRegulationRoadLinkLayer; }
	const KKDLaneRegulationIntersectionNodeLayer& laneRegulationIntersectionNodeLayer() const { return mLaneRegulationIntersectionNodeLayer; }

protected:
// �v���g�R���̎���
	bool append(int cRecordId, const char* cOriginalRecord)
	{
		// ���R�[�hID�ŏ�����U�蕪��
		switch(cRecordId) {
		case kkd::kAdminId: // �Ǘ��f�[�^
			mAdminLayer.append(reinterpret_cast<const KKDAdminFormat&>(*cOriginalRecord));
			break;
		case kkd::kRegulationRoadNodeId: // ��ʋK�����H�m�[�h�f�[�^
			mRegulationRoadNodeLayer.append(reinterpret_cast<const KKDRegulationRoadNodeFormat&>(*cOriginalRecord));
			break;
		case kkd::kRegulationRoadLinkId: // ��ʋK�����H�����N�f�[�^
			mRegulationRoadLinkLayer.append(reinterpret_cast<const KKDRegulationRoadLinkFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationRoadNodeId: // ���[���K�����H�m�[�h�f�[�^
			mLaneRegulationRoadNodeLayer.append(reinterpret_cast<const KKDLaneRegulationRoadNodeFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationRoadLinkId: // ���[���K�����H�����N�f�[�^
			mLaneRegulationRoadLinkLayer.append(reinterpret_cast<const KKDLaneRegulationRoadLinkFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationIntersectionNodeId: // ���[���K���W�������_�m�[�h�f�[�^
			mLaneRegulationIntersectionNodeLayer.append(reinterpret_cast<const KKDLaneRegulationIntersectionNodeFormat&>(*cOriginalRecord));
			break;
		default:
			return false;
		}

		return true;
	}

	void clear()
	{
		mAdminLayer.clear();
		mRegulationRoadNodeLayer.clear();
		mRegulationRoadLinkLayer.clear();
		mLaneRegulationRoadNodeLayer.clear();
		mLaneRegulationRoadLinkLayer.clear();
		mLaneRegulationIntersectionNodeLayer.clear();
	}

protected:
// �v���p�e�B
	KKDAdminLayer mAdminLayer;
	KKDRegulationRoadNodeLayer mRegulationRoadNodeLayer;
	KKDRegulationRoadLinkLayer mRegulationRoadLinkLayer;
	KKDLaneRegulationRoadNodeLayer mLaneRegulationRoadNodeLayer;
	KKDLaneRegulationRoadLinkLayer mLaneRegulationRoadLinkLayer;
	KKDLaneRegulationIntersectionNodeLayer mLaneRegulationIntersectionNodeLayer;
};

typedef KKDLoadT<
	DRMAContinualLayerTemplate<KKDRegulationRoadNode>,
	DRMAContinualLayerTemplate<KKDRegulationRoadLink>,
	DRMAContinualLayerTemplate<KKDLaneRegulationRoadNode>,
	DRMAContinualLayerTemplate<KKDLaneRegulationRoadLink>,
	DRMAContinualLayerTemplate<KKDLaneRegulationIntersectionNode>
> KKDLoad;

/**
 * @example example.cpp
 *
 * D�h���C�u�̃��[�g�f�B���N�g�������ɑ��݂���ukkd�v�f�B���N�g�����533945��KKD�f�[�^��ǂݍ��݁A
 * �S��ʋK�����H�m�[�h�̊e������o�͂���B
 */

#endif // !defined(AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_)
