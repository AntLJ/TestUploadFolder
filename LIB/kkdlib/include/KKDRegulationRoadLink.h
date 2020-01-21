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

// KKDRegulationRoadLink.h: KKDRegulationRoadLink �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_)
#define AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief ��ʋK�����H�����N�f�[�^�t�H�[�}�b�g�N���X
 */
struct KKDRegulationRoadLinkFormat
{
// ��`
	struct IntersectionId {
		_DRMADec eMeshCode[6];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
	};

	struct IntersectionIdPair {
		IntersectionId eIntersection1Id;
		IntersectionId eIntersection2Id;
	};

	struct LinkId {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

	struct Condition {
		_DRMADec eClass[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
		_DRMADec eCarWeight[3];
		_DRMADec eCarWidth[3];
		_DRMADec eCarHeight[3];
	};

	struct NodesInMesh {
		typedef _DRMAHex NodeId[5];
		_DRMADec eMeshCode[6];
		_DRMADec eNodeCount[2];
		NodeId eNodeIds[20];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	IntersectionIdPair eId;
	LinkId eLinkId;

	_DRMADec eSequence[2];

	_DRMADec eIllNavigation[1];
	
	_DRMADec eConditionCount[2];
	Condition eConditions[3];

	_DRMADec eNodeCount[2];
	_DRMADec eMeshCount[2];
	NodesInMesh eNodesInMeshs[1];

	_DRMAHex eFiller1[2];

	_DRMADec eContinue[1];
};

/**
 * @brief ��ʋK�����H�����N�f�[�^���R�[�h�N���X
 */
class KKDRegulationRoadLink : public KKDRecordT<KKDRegulationRoadLinkFormat>
{
public:
// ��`
	typedef KKDIntersectionId IntersectionId;
	typedef KKDIntersectionIdPair IntersectionIdPair;
	typedef KKDLinkId LinkId; ///< �����N�ԍ�
	typedef KKDIntersectionIdPair Id;

	/// ��ʋK���f�[�^
	struct Condition : public DRMAItemConverterTemplate<KKDRegulationRoadLinkFormat::Condition> {
		Condition(const Format& cOriginal);

		int eClass; ///< ��ʋK����ʃR�[�h
		int eCarType; ///< �Ԏ�w��
		int eWeekday; ///< �j���w��
		int eTime; ///< ���Ԏw��i�n�A�I�����j
		int eDate; ///< �����w��i�n�A�I�����j
		int eCarWeight; ///< �d�ʎw��i0.1t�j
		int eCarWidth; ///< �ԕ��w��i0.1m�j
		int eCarHeight; ///< �ԍ��w��i0.1m�j
	};

	/// ���b�V��
	struct NodesInMesh : public DRMAItemConverterTemplate<KKDRegulationRoadLinkFormat::NodesInMesh> {
		typedef int NodeId;
		typedef std::vector<NodeId> NodeIds;

		NodesInMesh(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendNodeIds(const Format& cOriginal);

	public:
		int eMeshCode; ///< �Q�����b�V���R�[�h
		int eNodeCount; ///< ���b�V�����m�[�h��
		NodeIds eNodeIds; ///< �m�[�h�ԍ�
	};

	typedef std::vector<Condition> Conditions;
	typedef std::vector<NodesInMesh> NodesInMeshs;

// �\�z/����
	KKDRegulationRoadLink(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendConditions(const Format& cOriginalRecord);
	void appendNodesInMeshs(const Format& cOrignalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	KKDIntersectionIdPair eId; ///< �����_�ԍ��y�A
	LinkId eLinkId; ///< �����N�ԍ�

	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eIllNavigation; ///< ��ʗU���s�K��ԃR�[�h
	
	int eConditionCount; ///< �K����
	Conditions eConditions; ///< ��ʋK���f�[�^

	int eNodeCount; ///< �����N���m�[�h��
	int eMeshCount; ///< �Q�����b�V����
	NodesInMeshs eNodesInMeshs; ///< ���b�V��

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId; ///< ���Y���R�[�hID
};

#endif // !defined(AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_)
