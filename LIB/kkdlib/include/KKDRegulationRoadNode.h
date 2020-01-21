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

// KKDRegulationRoadNode.h: KKDRegulationRoadNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_)
#define AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDVersion.h"
#include "KKDRecord.h"

/**
 * @brief ��ʋK�����H�m�[�h�f�[�^�t�H�[�}�b�g�N���X
 */
struct KKDRegulationRoadNodeFormat
{
// ��`
	struct AppertainNode {
		_DRMADec eMeshDirection[1];
		_DRMAHex eId[5];
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

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[15];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[30];
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
		Name eIntersectionName;
		_DRMADec eRegulationCount[2];
		IntersectionRegulation eIntersectionRegulations[2];
#if KKD_VERSION >= 101
		_DRMADec eIntersectionNameKanjiResearchLevel[1];
		_DRMADec eIntersectionNameKanaResearchLevel[1];
		_DRMADec eIntersectionResearchDate[4];
#endif // KKD_VERSION >= 101
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMADec ePrefectureCode[2];
	_DRMADec eStationCode[4];
	_DRMADec eCrossId[5];

	_DRMADec eSignal[1];

	_DRMADec eSequence[2];

	_DRMAHex eId[5];

	_DRMADec eAppertainNodeCount[2];
	AppertainNode eAppertainNodes[5];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[1];

#if KKD_VERSION >= 101
	_DRMAHex eFiller[5];
#else
	_DRMAHex eFiller[11];
#endif // KKD_VERSION >= 101

	_DRMADec eContinue[1];
};

/**
 * @brief ��ʋK�����H�m�[�h�f�[�^���R�[�h�N���X
 */
class KKDRegulationRoadNode : public KKDRecordT<KKDRegulationRoadNodeFormat>
{
public:
	typedef int Id;

	/// �����m�[�h
	struct AppertainNode : public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);
		AppertainNode(int nMeshDirection, int nId);

		bool operator < (const AppertainNode& rNode) const;

		int eMeshDirection; ///< �����m�[�h�Y�����b�V���R�[�h
		int eId; ///< �����m�[�h�ԍ�
	};

	typedef DRMAPoint Point ;
	typedef KKDIntersectionId IntersectionId;

	/// �����_����
	struct Name {
		int eKanjiLength; ///< �����_����
		char eKanji[31]; ///< �����_���̊���
		int eKanaLength; ///< �J�i������
		char eKana[31]; ///< �����_���̃J�i
	};

	/// �����_�ʍs�K��
	struct IntersectionRegulation : public KKDIntersectionRegulation, public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::IntersectionRegulation>{
		IntersectionRegulation(const Format& cOriginal);
	};

	/// �ڑ������N
	struct ConnectLink : public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::ConnectLink> {
		typedef std::vector<IntersectionRegulation> IntersectionRegulations;
		ConnectLink(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendIntersectionRegulations(const Format& cOriginal);

	public:
		int eConnectNodeMeshDirection; ///< �ڑ��m�[�h�Y�����b�V���R�[�h
		int eConnectNodeId; ///< �ڑ��m�[�h�ԍ�
		IntersectionId eConnectIntersectionId; ///< �ڑ������_�ԍ�
		Name eIntersectionName; ///< �����_����
		int eRegulationCount; ///< �K����
		IntersectionRegulations eIntersectionRegulations; ///< �����_�ʍs�K��
#if KKD_VERSION >= 101
		int eIntersectionNameKanjiResearchLevel; ///< �����_���̊����������x��
		int eIntersectionNameKanaResearchLevel; ///< �����_���̃J�i�������x��
		int eIntersectionResearchDate; ///< �����_���̒����N��
#endif // KKD_VERSION >= 101
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectLink> ConnectLinks;

// �\�z/����
	KKDRegulationRoadNode(const Format& cOriginalRecord);

	void append(const Format& cOriginalRecord);
protected:
// �ǉ�
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectLinks(const Format& cOriginalRecord);

public:
//�v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	int ePrefectureCode; ///< �s���{���R�[�h
	int eStationCode; ///< ���R�[�h
	int eCrossId; ///< ���������_�ԍ�

	int eSignal; ///< �M���@�L���R�[�h

	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eId; ///< �m�[�h�ԍ�

	int eAppertainNodeCount; ///< ��\�m�[�h�����m�[�h��
	AppertainNodes eAppertainNodes; ///< �����m�[�h

	Point ePoint; ///< ���K�����W

	int eClass; ///< �����_�m�[�h��ʃR�[�h

	int eConnectLinkCount; ///< �ڑ������N�{��
	ConnectLinks eConnectLinks; ///< �ڑ������N

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_)
