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

// DRMAFacilLocation.h: DRMAFacilLocation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_)
#define AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief �{�ݓ��ʒu�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMAFacilLocationFormat
{
// ��`
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ShortName {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[10];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[20];
	};

	struct LongName {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[30];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[60];
	};

	struct AdjoinBaseNode {
		_DRMADec eMeshCode[6];
		_DRMAHex eId[4];
	};

	struct AdjoinBaseLink {
		struct LinkId {
			_DRMAHex eNode1Id[4];
			_DRMAHex eNode2Id[4];
		};

		_DRMADec eMeshCode[6];
		LinkId eId;
		_DRMADec eLinkDir[1];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	_DRMADec eId[5];
	_DRMADec eClass[6];
	_DRMADec eAdminCode[5];
	_DRMADec eShapeCount[1];

	_DRMAHex eFiller1[4];

	Point ePoint;

	ShortName eShortName;
	LongName eLongName;

	_DRMADec eAdjoinBaseNodeCount[1];
	AdjoinBaseNode eAdjoinBaseNodes[2];

	_DRMADec eAdjoinBaseLinkCount[1];
	AdjoinBaseLink eAdjoinBaseLinks[2];

	_DRMAHex eFiller2[3];
};

/**
 * @brief �{�ݓ��ʒu�f�[�^���R�[�h�N���X
 */
class DRMAFacilLocation : public DRMARecordT<DRMAFacilLocationFormat>
{
public:
// ��`
	typedef DRMAPoint Point;
	typedef DRMAShortName ShortName;
	typedef DRMALongName LongName;
	typedef int Id;

	struct AdjoinBaseNode {
		int eMeshCode; ///< �Q�����b�V���R�[�h
		int eId; ///< �m�[�h�ԍ�
	};

	struct AdjoinBaseLink {
		typedef DRMABaseLinkId LinkId;

		int eMeshCode; ///< �Q�����b�V���R�[�h
		LinkId eId; ///< �����N�ԍ�
		int eLinkDir; ///< �Ή������N����
	};

// �\�z/����
	DRMAFacilLocation(const Format& cOriginalRecord);

// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	int eId; ///< �{�ݓ��ԍ�
	int eClass; ///< �{�ݓ���ʃR�[�h
	int eAdminCode; ///< �s�����R�[�h
	int eShapeCount; ///< �`��f�[�^�A�C�e������

	Point ePoint; ///< ���K�����W

	ShortName eShortName; ///< �\������
	LongName eLongName; ///< ��������

	int eAdjoinBaseNodeCount; ///< �אڊ�{���H�m�[�h����
	AdjoinBaseNode eAdjoinBaseNodes[2]; ///< �אڊ�{���H�m�[�h

	int eAdjoinBaseLinkCount; ///< �אڊ�{���H�����N����
	AdjoinBaseLink eAdjoinBaseLinks[2]; ///< �אڊ�{���H�����N

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_)
