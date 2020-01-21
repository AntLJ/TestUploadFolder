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

// DRMAAdmin1.h: DRMAAdmin1 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_)
#define AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief �Ǘ��f�[�^�i���̂P�j�t�H�[�}�b�g�N���X
 */
struct DRMAAdmin1Format
{
// ��`
	struct LayerDate {
		typedef _DRMADec Code[6];

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

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	_DRMADec eMeshCode[6];
	_DRMADec eEditionNo[4];
	
	_DRMADec eScale[5];
	_DRMAKanji eMeshName[10];
	_DRMADec eRevisionYear[2];
	_DRMADec eModYear[2];

	_DRMADec eBias[3];
	
	_DRMADec eBottomDistance[5];
	_DRMADec eRightDistance[5];
	_DRMADec eTopDistance[5];
	_DRMADec eLeftDistance[5];

	LayerDate eUpdateDate;

	_DRMAHex eFiller1[12];

	LayerDate eModDate;

	_DRMADec eLastModDate[6];

	_DRMAHex eFiller2[6];
};

/**
 * @brief �Ǘ��f�[�^�i���̂P�j���R�[�h�N���X
 */
class DRMAAdmin1 : public DRMARecordT<DRMAAdmin1Format> {
public:
// ��`
	struct LayerDate {
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

// �\�z/����
	DRMAAdmin1(const Format& cOriginalRecord);

// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	int eMeshCode; ///< �Q�����b�V���R�[�h
	int eEditionNo; ///< �Ŕԍ�
	
	int eScale; ///< �k��
	char eMeshName[21]; ///< �}��
	int eRevisionYear; ///< �����N�x
	int eModYear; ///< �C���N�x

	int eBias; ///< �n���C�Ίp�i���΁j
	
	int eBottomDistance; ///< ��扺��
	int eRightDistance; ///< ���E��
	int eTopDistance; ///< �����
	int eLeftDistance; ///< ��捶��

	LayerDate eUpdateDate; 	///< �f�[�^�ʊ�E�X�V�N����
	LayerDate eModDate;	///< �f�[�^�ʏC���N����

	int eLastModDate; ///< �ŏI�C���N����

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_)
