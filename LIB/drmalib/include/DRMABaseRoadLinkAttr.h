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

// DRMABaseRoadLinkAttr.h: DRMABaseRoadLinkAttr �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_)
#define AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief ��{���H�����N�������t�H�[�}�b�g�N���X
 */
struct DRMABaseRoadLinkAttrFormat
{
// ��`
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Attr {
		struct Point {
			_DRMADec eX[5];
			_DRMADec eY[5];
		};

		struct Name {
			_DRMADec eKanjiLength[2];
			_DRMAKanji eKanji[10];
			_DRMADec eKanaLength[2];
			_DRMAKana eKana[20];
		};

		_DRMADec eClass[2];
		_DRMADec eDispLevel[1];
		_DRMADec eBeginPointNo[3];
		_DRMADec ePreBegin[1];
		_DRMADec eEndPointNo[3];
		_DRMADec eEpiEnd[1];
		_DRMADec eLength[5];
		
		// �������̕\���Q�l�ʒu���K�����W
		Point ePoint;
		
		// ��������
		Name eName;
	};

	struct Attr2 {
		_DRMADec eHeavyNoPassage[1];
		_DRMADec eFacilManager[4];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	LinkId eId;

	_DRMAHex eFiller1[2];

	_DRMADec eSequence[2];

	_DRMADec eAttrCount[2];
	Attr eAttrs[3];
	Attr2 eAttr2s[3];

	_DRMAHex eFiller2[14];

	_DRMADec eContinue[1];
};

/**
 * @brief ��{���H�����N���������R�[�h�N���X
 */
class DRMABaseRoadLinkAttr : public DRMARecordT<DRMABaseRoadLinkAttrFormat>
{
public:
// ��`
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct Attr : public DRMAItemConverterTemplate<DRMABaseRoadLinkAttrFormat::Attr> {
		typedef DRMAPoint Point;
		typedef DRMAShortName Name;

		Attr(const Format& cOriginal);

		int eClass; ///< ������ʃR�[�h
		int eDispLevel; ///< �\�����x���Q�l�R�[�h
		int eBeginPointNo; ///< �n�_�⊮�_�ԍ�
		int ePreBegin; ///< �n�_���ڑ��L���R�[�h
		int eEndPointNo; ///< �I�_�⊮�_�ԍ�
		int eEpiEnd; ///< �I�_���ڑ��L���R�[�h
		int eLength; ///< ��������
		
		// �������̕\���Q�l�ʒu���K�����W
		Point ePoint;
		
		// ��������
		Name eName;
	};

	struct Attr2 : public DRMAItemConverterTemplate<DRMABaseRoadLinkAttrFormat::Attr2> {
		Attr2(const Format& cOriginal);

		int eHeavyNoPassage; ///< ��^�Ԓʍs�s�R�[�h
		int eFacilManager; ///< 
	};

	typedef std::vector<Attr> Attrs;
	typedef std::vector<Attr2> Attr2s;

// �\�z/����
	DRMABaseRoadLinkAttr(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendAttrs(const Format& cOriginalRecord);
	void appendAttr2s(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	LinkId eId; ///< �����N�ԍ�
	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eAttrCount; ///< �����N����������
	Attrs eAttrs; ///< �����N������
	Attr2s eAttr2s; ///< �����N������'

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_)
