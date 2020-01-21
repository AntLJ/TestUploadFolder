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

// DRMABaseRoadNode.h: DRMABaseRoadNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_)
#define AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief ��{���H�m�[�h�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMABaseRoadNodeFormat
{
// ��`
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ConnectLink {
		_DRMAHex eNodeId[4];
		_DRMADec ePassageCode[8];
		_DRMADec eAngle[3];
	};

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[10];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[20];
	};

	struct ConnectFerry {
		_DRMADec eJointMeshCode[6];
		_DRMAHex eJointNodeId[4];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	_DRMAHex eId[4];
	_DRMADec eSequence[2];

	Point ePoint;
	
	_DRMADec eAltitude[3];
	_DRMADec eClass[1];

	_DRMADec eJointMeshCode[6];
	_DRMAHex eJointNodeId[4];

	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[8];

	Name eName;
	_DRMADec eConnectFerryCount[2];
	ConnectFerry eConnectFerries[5];

	_DRMAHex eFiller[6];

	_DRMAHex eContinue[1];
};

/**
 * @brief ��{���H�m�[�h�f�[�^���R�[�h�N���X
 */
class DRMABaseRoadNode : public DRMARecordT<DRMABaseRoadNodeFormat>
{
public:
// ��`
	typedef DRMAPoint Point;
	typedef DRMAShortName Name;
	typedef int Id;

	struct ConnectLink : public DRMAItemConverterTemplate<DRMABaseRoadNodeFormat::ConnectLink> {
		ConnectLink(const Format& cOriginal);

		int eNodeId; ///< �ڑ��m�[�h�ԍ�
		int ePassageCode; ///< �����_�ʍs�R�[�h
		int eAngle; ///< �ڑ��p�x�i�x�j
	};

	struct ConnectFerry  : public DRMAItemConverterTemplate<DRMABaseRoadNodeFormat::ConnectFerry> {
		ConnectFerry(const Format& cOriginal);

		int eJointMeshCode; ///< �q�H�ڑ��Q�����b�V���R�[�h
		int eJointNodeId; ///< �q�H�ڑ��m�[�h�ԍ�
	};

	typedef std::vector<ConnectLink> ConnectLinks;
	typedef std::vector<ConnectFerry> ConnectFerries;

// �\�z/����
	DRMABaseRoadNode(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendConnectLinks(const Format& cOriginalRecord);
	void appendConnectFerries(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	int eId; ///< �m�[�h�ԍ�
	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	Point ePoint; ///< ���K�����W

	int eAltitude; ///< �W��
	int eClass; ///< �m�[�h��ʃR�[�h

	int eJointMeshCode; ///< �אڂQ�����b�V���R�[�h
	int eJointNodeId; ///< �אڂQ�����b�V���ڍ��m�[�h�ԍ�

	int eConnectLinkCount; ///< �ڑ������N�{��
	ConnectLinks eConnectLinks; ///< �ڑ������N

	Name eName; ///< �����_����
	int eConnectFerryCount; ///< �t�F���[�ڑ��q�H����
	ConnectFerries eConnectFerries; ///< �t�F���[�ڑ��q�H
	
	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_)
