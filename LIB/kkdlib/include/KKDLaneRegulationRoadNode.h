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

// KKDLaneRegulationRoadNode.h: KKDLaneRegulationRoadNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_)
#define AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief ���[���K�����H�m�[�h�f�[�^�t�H�[�}�b�g�N���X
 */
struct KKDLaneRegulationRoadNodeFormat
{
// ��`
	struct AppertainNode {
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
		_DRMADec eNodeMeshDirection[1];
		_DRMAHex eNodeId[5];
	};

	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct IntersectionId {
		_DRMADec eMeshCode[6];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
	};

	struct UTurnRegulation {
		_DRMADec eClass[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
	};

	struct IntersectionRegulation {
		_DRMAHex eOppositeNodeMeshDirection[1];
		_DRMAHex eOppositeNodeId[5];
		IntersectionId eOppositeIntersectionId;
		_DRMADec eRegulationLane[1];
		_DRMADec ePassage[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
	};

	struct ConnectLink {
		_DRMADec eConnectNodeMeshDirection[1];
		_DRMAHex eConnectNodeId[5];
		IntersectionId eConnectIntersectionId;
		UTurnRegulation eUTurnRegulation;
		_DRMADec eRegulationCount[2];
		_DRMADec eLaneCount[1];
		_DRMADec eApproachLaneCount[1];
		IntersectionRegulation eIntersectionRegulations[2];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMADec ePrefectureCode[2];
	_DRMADec eStationCode[4];
	_DRMADec eCrossId[5];

	_DRMADec eFiller1[1];

	_DRMADec eSequence[2];

	_DRMAHex eId[5];

	_DRMADec eAppertainNodeCount[2];
	AppertainNode eAppertainNodes[4];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectLinkCount[2];
	ConnectLink eConnectLinks[1];

	_DRMAHex eFiller[13];

	_DRMADec eContinue[1];
};

/**
 * @brief ���[���K�����H�m�[�h�f�[�^���R�[�h�N���X
 */
class KKDLaneRegulationRoadNode : public KKDRecordT<KKDLaneRegulationRoadNodeFormat>
{
public:
// ��`
	typedef int Id;

	/// ���������_
	struct AppertainNode : public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);

		bool operator < (const AppertainNode& rNode) const;

		int ePrefectureCode; ///< �s���{���R�[�h
		int eStationCode; ///< ���R�[�h
		int eCrossId; ///< ���������_�ԍ�
		int eNodeMeshDirection; ///< �����m�[�h���b�V���ԍ�
		int eNodeId; ///< �����m�[�h�ԍ�
	};

	/// ���K�����W
	typedef DRMAPoint Point;

	/// �����_�ԍ�
	typedef KKDIntersectionId IntersectionId;

	/// �t�^�[���K���L��
	struct UTurnRegulation {
		int eClass; ///< �K���R�[�h
		int eCarType; ///< �Ԏ�w��
		int eWeekday; ///< �j���w��
		int eTime; ///< ���Ԏw��
		int eDate; ///< �����w��
	};

	/// �����_�ʍs�K��
	struct IntersectionRegulation : public KKDIntersectionRegulation, public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::IntersectionRegulation> {
		IntersectionRegulation(const Format& cOriginal);
	};

	/// �ڑ������N
	struct ConnectLink : public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::ConnectLink> {
		typedef std::vector<IntersectionRegulation> IntersectionRegulations;
		ConnectLink(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendIntersectionRegulations(const Format& cOriginal);

	public:
		int eConnectNodeMeshDirection; ///< �ڑ��m�[�h�Y�����b�V���R�[�h
		int eConnectNodeId; ///< �ڑ��m�[�h�ԍ�
		IntersectionId eConnectIntersectionId; ///< �ڑ������_�ԍ�
		UTurnRegulation eUTurnRegulation; ///< �t�^�[���K���L��
		int eRegulationCount; ///< �K����
		int eLaneCount; ///< ���[�����i�������[�����j
		int eApproachLaneCount; ///< ���߃��[����
		IntersectionRegulations eIntersectionRegulations; ///< �ڑ������N
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectLink> ConnectLinks;

// �\�z/����
	KKDLaneRegulationRoadNode(const Format& cOriginalRecord);

	void append(const Format& cOriginalRecord);
protected:
// �ǉ�
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectLinks(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	int ePrefectureCode; ///< �s���{���R�[�h
	int eStationCode; ///< ���R�[�h
	int eCrossId; ///< ���������_�ԍ�

	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eId; ///< �m�[�h�ԍ�

	int eAppertainNodeCount; ///< ��\�����_�����m�[�h��
	AppertainNodes eAppertainNodes; ///< �����m�[�h

	Point ePoint; ///< ���K�����W

	int eClass; ///< �����_�m�[�h��ʃR�[�h

	int eConnectLinkCount; ///< �ڑ������N�{��
	ConnectLinks eConnectLinks; ///< �ڑ������N

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_)
