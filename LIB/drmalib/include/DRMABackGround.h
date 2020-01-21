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

// DRMABackGround.h: DRMABackGround �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_)
#define AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief �w�i�n�f�[�^�t�H�[�}�b�g�N���X
 */
struct DRMABackGroundFormat
{
// ��`
	struct Point {
		_DRMADec eClass[1];
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	
	_DRMAHex eFiller1[7];

	_DRMADec eId[4];
	_DRMADec eSequence[2];
	_DRMADec eClass[1];

	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMAHex eFiller2[5];

	_DRMAHex eContinue[1];
};

/**
 * @brief �w�i�n�f�[�^���R�[�h�N���X
 */
class DRMABackGround : public DRMARecordT<DRMABackGroundFormat>
{
public:
// ��`
	typedef int Id;

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMABackGroundFormat::Point>
	{
		Point(const Format& cOriginal);

		int eClass;
	};

	typedef std::vector<Point> Points;

// �\�z/����
	DRMABackGround(const Format& cOrignalRecord);

// �ǉ�
	void append(const Format& cOrignalRecord);

protected:
// �ǉ�
	void appendPoints(const Format& cOrignalRecord);

public:
// �v���p�e�B
//	int eRecordId;
	
	int eId;
	int eSequence;
	int eClass;

	int ePointCount;
	Points ePoints;

	int eContinue;
};


#endif // !defined(AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_)
