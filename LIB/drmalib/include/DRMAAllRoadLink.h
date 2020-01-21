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

// DRMAAllRoadLink.h: DRMAAllRoadLink �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_)
#define AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief �S���H�����N�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMAAllRoadLinkFormat
{
// ��`
	struct LinkId {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

	struct BaseLinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
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
	_DRMADec eRoadClass[1];
	_DRMADec eAdminCode[5];
	_DRMADec eLength[5];
	_DRMADec eWidth[1];
	_DRMADec eLane[1];
	_DRMADec eRegulation[1];
	_DRMADec eRegulationCond[1];
	BaseLinkId eBaseLinkId;
	
	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMADec eAttr[1];

	_DRMAHex eFiller1[3];

	_DRMAHex eContinue[1];
};

/**
 * @brief �S���H�����N�f�[�^���R�[�h�N���X
 */
class DRMAAllRoadLink : public DRMARecordT<DRMAAllRoadLinkFormat>
{
public:
// ��`
	typedef DRMAAllLinkId LinkId;
	typedef DRMABaseLinkId BaseLinkId;
	typedef LinkId Id;

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMAAllRoadLinkFormat::Point> {
		Point(const Format& cOriginal);
	};

	typedef std::vector<Point> Points;

// �\�z/����
	DRMAAllRoadLink(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendPoints(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	LinkId eId; ///< �����N�ԍ�
	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eManager; ///< �Ǘ��҃R�[�h
	int eRoadClass; ///< ���H��ʃR�[�h
	int eAdminCode; ///< �s�����R�[�h
	int eLength; ///< �����N���i�v�Z���j�im�j
	int	eWidth; ///< ���H�����敪�R�[�h
	int	eLane; ///< �Ԑ����R�[�h
	int	eRegulation; ///< ��ʋK����ʃR�[�h
	int	eRegulationCond; ///< ��ʋK��������ʃR�[�h

	BaseLinkId eBaseLinkId; ///< �Ή���{���H�����N�ԍ�

	int	ePointCount; ///< �⊮�_����
	Points ePoints; ///< �⊮�_���K�����W

	int eAttr; ///< �����N�������L���R�[�h

	int	eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_)
