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

// DRMAAllRoadNode.h: DRMAAllRoadNode �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_)
#define AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief �S���H�m�[�h�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMAAllRoadNodeFormat
{
// ��`
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ConnectLink {
		_DRMAHex eNodeId[5];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMAHex eId[5];
	Point ePoint;
	_DRMADec eClass[1];
	_DRMADec eJointMeshCode[6];
	_DRMAHex eJointNodeId[5];
	
	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[8];

	_DRMAHex eFiller[186];
};

/**
 * @brief �S���H�m�[�h�f�[�^���R�[�h�N���X
 */
class DRMAAllRoadNode : public DRMARecordT<DRMAAllRoadNodeFormat>
{
public:
// ��`
	typedef DRMAPoint Point;
	typedef int Id;

	struct ConnectLink : public DRMAItemConverterTemplate<DRMAAllRoadNodeFormat::ConnectLink> {
		ConnectLink(const Format& cOriginal);

		int eNodeId;
	};

	typedef std::vector<ConnectLink> ConnectLinks;

// �\�z/����
	DRMAAllRoadNode(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendConnectLinks(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	int eId; ///< �m�[�h�ԍ�
	Point ePoint; ///< ���K�����W
	int eClass; ///< �m�[�h��ʃR�[�h
	int eJointMeshCode; ///< �אڂQ�����b�V���R�[�h
	int eJointNodeId; ///< �אڂQ�����b�V���ڍ��m�[�h�ԍ�
	
	int eConnectLinkCount; ///< �ڑ������N�{��
	ConnectLinks eConnectLinks; ///< �ڑ��m�[�h�ԍ�

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_)
