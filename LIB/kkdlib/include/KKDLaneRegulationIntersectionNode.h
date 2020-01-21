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

// KKDLaneRegulationIntersectionNode.h: KKDLaneRegulationIntersectionNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_)
#define AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief ���[���K���W�������_�m�[�h�f�[�^�t�H�[�}�b�g�N���X
 */
struct KKDLaneRegulationIntersectionNodeFormat
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

	struct ConnectIntersection {
		_DRMADec eMeshCode[6];
		_DRMAHex eConnectIntersectionNodeId[5];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
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
	AppertainNode eAppertainNodes[5];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectIntersectionCount[2];
	ConnectIntersection eConnectIntersections[5];

	_DRMAHex eFiller[24];

	_DRMADec eContinue[1];
};

/**
 * @brief ���[���K���W�������_�m�[�h�f�[�^���R�[�h�N���X
 */
class KKDLaneRegulationIntersectionNode : public KKDRecordT<KKDLaneRegulationIntersectionNodeFormat>
{
public:
// ��`
	typedef int Id;

	/// ���������_
	struct AppertainNode : public DRMAItemConverterTemplate<KKDLaneRegulationIntersectionNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);

		int ePrefectureCode; ///< �s���{���R�[�h
		int eStationCode; ///< ���R�[�h
		int eCrossId; ///< ���������_�ԍ�
		int eNodeMeshDirection; ///< �����m�[�h���b�V���ԍ�
		int eNodeId; ///< �����m�[�h�ԍ�
	};

	/// ���K�����W
	typedef DRMAPoint Point;

	/// �����_
	struct ConnectIntersection : public DRMAItemConverterTemplate<KKDLaneRegulationIntersectionNodeFormat::ConnectIntersection> {
		ConnectIntersection(const Format& cOriginal);

		int eMeshCode; ///< �Q�����b�V���R�[�h
		int eConnectIntersectionNodeId; ///< �ڑ������_�m�[�h�ԍ�
		int ePrefectureCode; ///< �s���{���R�[�h
		int eStationCode; ///< ���R�[�h
		int eCrossId; ///< ���������_�ԍ�
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectIntersection> ConnectIntersections;

// �\�z/����
	KKDLaneRegulationIntersectionNode(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);
protected:
// �ǉ�
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectIntersections(const Format& cOriginalRecord);

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

	int eConnectIntersectionCount; ///< �ڑ������_��
	ConnectIntersections eConnectIntersections; ///< �ڑ������_

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_)
