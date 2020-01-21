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

// DRMAPlaceNameLocation.h: DRMAPlaceNameLocation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_)
#define AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief �n�����\���ʒu�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMAPlaceNameLocationFormat
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

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMAHex eFiller1[1];

	_DRMADec eId[4];
	
	_DRMAHex eFiller2[4];

	_DRMADec eClass[2];
	_DRMADec eAdminCode[5];

	_DRMAHex eFiller3[1];

	_DRMADec eDispLevel[1];
	_DRMADec eAngle[3];

	Point ePoint;
	ShortName eShortName;
	LongName eLongName;

	_DRMAHex eFiller[55];
};

/**
 * @brief �n�����\���ʒu�f�[�^���R�[�h�N���X
 */
class DRMAPlaceNameLocation : public DRMARecordT<DRMAPlaceNameLocationFormat>
{
public:
// ��`
	typedef DRMAPoint Point;
	typedef DRMAShortName ShortName;
	typedef DRMALongName LongName;
	typedef int Id;

// �\�z/����
	DRMAPlaceNameLocation(const Format& cOriginalRecord);

// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID

	int eId; ///< �A�C�e���ԍ�
	
	int eClass; ///< �n������ʃR�[�h
	int eAdminCode; ///< �s�����R�[�h

	int eDispLevel; ///< �\�����x���Q�l�R�[�h
	int eAngle; ///< �n�����\���Q�l�p�x

	Point ePoint; ///< �n�����\���Q�l�ʒu���K�����W
	ShortName eShortName; ///< �\������
	LongName eLongName; ///< ��������

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_)
