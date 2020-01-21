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

// DRMABaseRoadLink.h: DRMABaseRoadLink �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_)
#define AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief ��{���H�����N�t�H�[�}�b�g�N���X
 */
struct DRMABaseRoadLinkFormat
{
// ��`
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Road {
		_DRMADec eRoadClass[1];
		_DRMADec eRoadNo[4];
		_DRMADec eKubun[1];
	};

	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	LinkId eId;
	_DRMADec eSequence[2];

	_DRMADec eManager[1];
	Road eMain;

	_DRMADec eDuplicateCount[2];
	Road eDuplicates[3];

	_DRMADec eAdminCode[5];
	_DRMADec eLength[5];
	_DRMADec eClass[1];
	_DRMADec eBeacon[1];

	_DRMADec eCarOnly[1];
	_DRMADec eToll[1];
	_DRMADec ePassage[1];
	_DRMADec eWeatherRegulation[1];
	_DRMADec eCarWeight[1];
	_DRMADec eCarHeight[1];
	_DRMADec eCarWidth[1];
	
	_DRMADec eWidth[1];
	_DRMADec eLane[1];
	_DRMADec eRoadWidth[3];
	_DRMADec eMinRoadWidth[3];
	_DRMADec eMedianWidth[3];
	_DRMADec eMedianLength[5];
	
	_DRMADec eTrafic[4];
	_DRMADec eTripSpeed[4];

	_DRMADec eRegulation[1];
	_DRMADec eRegulationCond[1];
	_DRMADec eNoPassing[1];
	_DRMADec eSpeedRegulation[1];
	
	_DRMADec eSpetialVehicle[1];
	_DRMADec eAttr[1];
	_DRMADec ePointCount[3];
	Point ePoints[16];

	_DRMAHex eFiller1[2];

	_DRMADec eShapeSource[1];
	_DRMADec eDesignate[1];

	_DRMAHex eContinue[1];
};

/**
 * @brief ��{���H�����N���R�[�h�N���X
 */
class DRMABaseRoadLink : public DRMARecordT<DRMABaseRoadLinkFormat>
{
public:
// ��`
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct Road : public DRMAItemConverterTemplate<DRMABaseRoadLinkFormat::Road> {
		Road();
		Road(const Format& cOriginal);

		int eRoadClass; ///< ���H��ʃR�[�h
		int eRoadNo; ///< �H���ԍ�
		int eKubun; ///< ��E�]���H�敪�R�[�h
	};

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMABaseRoadLinkFormat::Point> {
		Point(const Format& cOriginal);
	};

	typedef std::vector<Road> Duplicates;
	typedef std::vector<Point> Points;

// �\�z/����
	DRMABaseRoadLink(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendDuplicates(const Format& cOriginalRecord);
	void appendPoints(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	LinkId eId; ///< �����N�ԍ�
	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eManager; ///< �Ǘ��҃R�[�h
	Road eMain; ///< ��H��

	int eDuplicateCount; ///< �d�p�H������
	Duplicates eDuplicates; ///< �d�p�H��

	int eAdminCode; ///< �s�����R�[�h
	int eLength; ///< �����N���i�v�Z���j�im�j
	int eClass; ///< �����N��ʃR�[�h
	int eBeacon; ///< �r�[�R���L���R�[�h

	int	eCarOnly; ///< �����Ԑ�p���H�R�[�h
	int	eToll; ///< �L�����H�R�[�h
	int	ePassage; ///< �����N�ʍs�A�s�R�[�h
	int	eWeatherRegulation; ///< �ُ�C�ێ��ʍs�K����Ԏ�ʃR�[�h
	int	eCarWeight; ///< �ԗ��d�ʐ����L���R�[�h
	int	eCarHeight; ///< �ԗ����������L���R�[�h
	int	eCarWidth; ///< �ԗ��������L���R�[�h
	
	int	eWidth; ///< ���H�����敪�R�[�h
	int	eLane; ///< �Ԑ����R�[�h
	int	eRoadWidth; ///< �ԓ������i0.1m�j
	int	eMinRoadWidth; ///< �ŏ��ԓ��������i0.1m�j
	int	eMedianWidth; ///< �����ѕ����i0.1m�j
	int	eMedianLength; ///< �����ѐݒu�����im�j
	
	int	eTrafic; ///< 12���Ԍ�ʗʁi100��j
	int	eTripSpeed; ///< ���s���x�i�s�[�N���j
	
	int	eRegulation; ///< ��ʋK����ʃR�[�h
	int	eRegulationCond; ///< ��ʋK��������ʃR�[�h
	int	eNoPassing; ///< �ǉz�֎~�R�[�h
	int	eSpeedRegulation; ///< �K�����x�R�[�h
	
	int	eSpetialVehicle; ///< ���Ԓʍs�V�X�e���ΏۃR�[�h
	int	eAttr; ///< �����N�������L���R�[�h
	int	ePointCount; ///< �⊮�_����
	Points ePoints; ///< �⊮�_���K�����W

	int	eShapeSource; ///< �`��f�[�^�擾�����R�[�h
	int	eDesignate; ///< ��ʍ����E�w���ԊY���R�[�h

	int	eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_)
