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

// DRMAFacilShape.h: DRMAFacilShape �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_)
#define AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief �{�ݓ��`��t�H�[�}�b�g�N���X
 */
struct DRMAFacilShapeFormat
{
// ��`
	struct Point {
		_DRMADec eClass[1];
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];

	_DRMADec eMeshCode[6];
	_DRMADec eId[5];

	_DRMADec eSequence[2];
	_DRMADec eBlockNo[1];

	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMAHex eFiller2[5];

	_DRMAHex eContinue[1];
};

/**
 * @brief �{�ݓ��`��f�[�^���R�[�h�N���X
 */
class DRMAFacilShape : public DRMARecordT<DRMAFacilShapeFormat>
{
public:
// ��`
	typedef int Id;
	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMAFacilShapeFormat::Point>
	{
		Point(const Format& cOriginal);

		int eClass;
	};

	typedef std::vector<Point> Points;

// �\�z/����
	DRMAFacilShape(const Format& cOrignalRecord);

// �ǉ�
	void append(const Format& cOrignalRecord);

protected:
// �ǉ�
	void appendPoints(const Format& cOrignalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	
	int eMeshCode; ///< �Q�����b�V���R�[�h
	int eId; ///< �{�ݓ��ԍ�

	int eSequence; ///< �A�C�e�������R�[�h�ԍ�
	int eBlockNo; ///< ����{�ݓ����A�C�e���ԍ�

	int ePointCount; ///< ��ԓ_����
	Points ePoints; ///< ��ԓ_

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_)
