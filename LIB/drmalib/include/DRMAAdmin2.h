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

// DRMAAdmin2.h: DRMAAdmin2 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_)
#define AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief �Ǘ��f�[�^�i���̂Q�j�t�H�[�}�b�g�N���X
 */
struct DRMAAdmin2Format
{
// ��`
	struct LayerCount {
		typedef _DRMADec Code[5];

		Code eBaseRoadNode;
		Code eBaseRoadLink;
		Code eBaseRoadLinkAttr;
		Code eBeaconLocation;
		Code eAllRoadNode;
		Code eAllRoadLink;
		Code eSite;
		Code eAdminLocation;
		Code eRailwayLocation;
		Code eFacilLocation;
		Code eFacilShape;
		Code ePlaceNameLocation;
		Code eRoadLinkRelation;
		Code eAllRoadLinkAttr;
	};

	struct MaxItemId {
		_DRMAHex eBaseRoadNode[4];
		_DRMAHex eAllRoadNode1[5];
		_DRMAHex eAllRoadNode2[5];
		_DRMADec eSite[4];
		_DRMADec eAdminLocation[4];
		_DRMADec eRailwayLocation[4];
		_DRMADec eFacil[5];
		_DRMADec ePlaceNameLocation[4];
		_DRMADec eBeacon[4];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	LayerCount eRecordCount;

	_DRMAHex eFiller1[10];

	LayerCount eItemCount;
	
	_DRMAHex eFiller2[10];

	_DRMAKanji eCopyright[23];

	MaxItemId eMaxItemId;

	_DRMAHex eFiller3[9];

};

/**
 * @brief �Ǘ��f�[�^�i���̂Q�j���R�[�h�N���X
 */
class DRMAAdmin2 : public DRMARecordT<DRMAAdmin2Format>
{
public:
// ��`
	struct LayerCount {
		int eBaseRoadNode; ///< ��{���H�m�[�h�f�[�^
		int eBaseRoadLink; ///< ��{���H�����N�f�[�^
		int eBaseRoadLinkAttr; ///< ��{���H�����N�������f�[�^
		int eBeaconLocation; ///< �r�[�R���ʒu�f�[�^
		int eAllRoadNode; ///< �S���H�m�[�h�f�[�^
		int eAllRoadLink; ///< �S���H�����N�f�[�^
		int eSite; ///< ���n�f�[�^
		int eAdminLocation; ///< �s���E�ʒu�f�[�^
		int eRailwayLocation; ///< �S���ʒu�f�[�^
		int eFacilLocation; ///< �{�ݓ��ʒu�f�[�^
		int eFacilShape; ///< �{�ݓ��`��f�[�^
		int ePlaceNameLocation; ///< �n�����\���ʒu�f�[�^
		int eRoadLinkRelation; ///< ��{���H�����N�E�S���H�����N�Ή��f�[�^
		int eAllRoadLinkAttr; ///< �S���H�����N������
	};

	struct MaxItemId {
		int eBaseRoadNode; ///< ��{���H�m�[�h�ԍ�
		int eAllRoadNode1; ///< �S���H�m�[�h�ԍ�1
		int eAllRoadNode2; ///< �S���H�m�[�h�ԍ�2
		int eSite; ///< ���n�f�[�^�A�C�e���ԍ�
		int eAdminLocation; ///< �s���E�ʒu�f�[�^�A�C�e���ԍ�
		int eRailwayLocation; ///< �S���ʒu�f�[�^�A�C�e���ԍ�
		int eFacil; ///< �{�ݓ��ԍ�
		int ePlaceNameLocation; ///< �n�����\���ʒu�f�[�^�A�C�e���ԍ�
		int eBeacon; ///< �r�[�R���A�C�e���ԍ�
	};

// �\�z�E����
	DRMAAdmin2(const Format& cOriginalRecord);

// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	LayerCount eRecordCount; ///< �f�[�^�ʃ��R�[�h��
	LayerCount eItemCount; ///< �f�[�^�ʃA�C�e������

	char eCopyright[47]; ///< ���쌠�\��

	MaxItemId eMaxItemId; ///< �g�p�ςݍő�A�C�e���ԍ�

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_)
