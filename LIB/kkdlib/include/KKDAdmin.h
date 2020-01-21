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

// KKDAdmin.h: KKDAdmin �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_)
#define AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KKDRecord.h"

/**
 * @brief �Ǘ��f�[�^�t�H�[�}�b�g�N���X
 */
struct KKDAdminFormat
{
// ��`
	struct LayerDate {
		typedef _DRMADec Date[6];
		Date eRegulationRoadNode;
		Date eRegulationRoadLink;
		Date eLaneRegulationRoadNode;
		Date eLaneRegulationRoadLink;
		Date eLaneRegulationIntersectionNode;
	};

	struct LayerCount {
		typedef _DRMADec Count[5];
		Count eRegulationRoadNode;
		Count eRegulationRoadLink;
		Count eLaneRegulationRoadNode;
		Count eLaneRegulationRoadLink;
		Count eLaneRegulationIntersectionNode;
	
	};

	struct MaxItemId {
		typedef _DRMAHex Id[5];
		Id eBaseRoadNode;
		Id eAllRoadNode1;
		Id eAllRoadNode2;
		Id eOtherNode1;
		Id eOtherNode2;
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	_DRMADec eMeshCode[6];
	_DRMADec eEditionNo[4];
	
	_DRMADec eDrmaEditionNo[4];

	LayerDate eUpdateDate;
	LayerDate eModDate;
	LayerCount eRecordCount;
	LayerCount eItemCount;

	_DRMAKanji eCopyright[25];

	MaxItemId eMaxItemId;

	_DRMAHex eFiller1[55];
};

/**
 * @brief �Ǘ��f�[�^���R�[�h�N���X
 */
class KKDAdmin : public KKDRecordT<KKDAdminFormat>
{
public:
// ��`
	struct LayerDate {
		typedef int Date;
		Date eRegulationRoadNode; ///< ��ʋK�����H�m�[�h�f�[�^
		Date eRegulationRoadLink; ///< ��ʋK�����H�����N�f�[�^
		Date eLaneRegulationRoadNode; ///< ���[���K�����H�m�[�h�f�[�^
		Date eLaneRegulationRoadLink; ///< ���[���K�����H�����N�f�[�^
		Date eLaneRegulationIntersectionNode; ///< ���[���K���W�������_�m�[�h�f�[�^
	};

	struct LayerCount {
		typedef int Count;
		Count eRegulationRoadNode; ///< ��ʋK�����H�m�[�h�f�[�^
		Count eRegulationRoadLink; ///< ��ʋK�����H�����N�f�[�^
		Count eLaneRegulationRoadNode; ///< ���[���K�����H�m�[�h�f�[�^
		Count eLaneRegulationRoadLink; ///< ���[���K�����H�����N�f�[�^
		Count eLaneRegulationIntersectionNode; ///< ���[���K���W�������_�m�[�h�f�[�^
	
	};

	struct MaxItemId {
		typedef int Id;
		Id eBaseRoadNode; ///< ��{���H�m�[�h�ԍ�
		Id eAllRoadNode1; ///< �S���H�m�[�h�ԍ�(1)
		Id eAllRoadNode2; ///< �S���H�m�[�h�ԍ�(2)
		Id eOtherNode1; ///< ���̑��m�[�h�ԍ�(1)
		Id eOtherNode2; ///< ���̑��m�[�h�ԍ�(2)
	};
// �\�z/����
	KKDAdmin(const Format& cOriginalRecord);


// �t�H�[�}�b�g
//	int eRecordId; ///< ���R�[�hID
	int eMeshCode; ///< �Q�����b�V���R�[�h
	int eEditionNo; ///< �Ŕԍ�
	
	int eDrmaEditionNo; ///< �g�pDRMA�f�[�^�x�[�X�Ŕԍ�

	LayerDate eUpdateDate; ///< �f�[�^�ʊ�X�V�N����
	LayerDate eModDate; ///< �f�[�^�ʏC���N����
	LayerCount eRecordCount; ///< �f�[�^�ʃ��R�[�h��
	LayerCount eItemCount; ///< �f�[�^�ʃA�C�e������

	char eCopyright[51]; ///< ���쌠�\��

	MaxItemId eMaxItemId; ///< �g�p�ςݍő�A�C�e���ԍ�

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_)
