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

// DRMABeaconLocation.h: DRMABeaconLocation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_)
#define AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief �r�[�R���ʒu�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMABeaconLocationFormat
{
// ��`
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[30];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[60];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMADec eItemNo[4];
	_DRMADec eId[8];
	_DRMADec eClass[2];
	_DRMADec eApplication[2];

	Point ePoint;

	_DRMAHex eFiller1[2];

	LinkId eLinkId;
	_DRMADec eLinkDir[1];
	Name eName;

	_DRMAHex eFiller2[93];
};

/**
 * @brief �r�[�R���ʒu�f�[�^���R�[�h�N���X
 */
class DRMABeaconLocation : public DRMARecordT<DRMABeaconLocationFormat>
{
public:
// ��`
	typedef DRMAPoint Point;
	typedef DRMABaseLinkId LinkId;
	typedef DRMALongName Name;
	typedef int Id;

// �\�z/����
	DRMABeaconLocation(const Format& cOriginalRecord);

// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	int eItemNo; ///< �r�[�R���A�C�e���ԍ�
	int eId; ///< ���M�r�[�R���ԍ�
	int eClass; ///< �r�[�R����ʃR�[�h
	int eApplication; ///< �r�[�R���p�r�ʃR�[�h

	Point ePoint; ///< ���K�����W

	LinkId eLinkId; ///< �ݒu�����N�ԍ�
	int eLinkDir; ///< �Ή������N�����R�[�h
	Name eName; ///< �r�[�R���ݒu�n�_��

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_)
