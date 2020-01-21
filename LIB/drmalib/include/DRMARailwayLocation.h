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

// DRMARailwayLocation.h: DRMARailwayLocation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMARAILWAYLOCATION_H__1804490F_9A32_4EFD_9E67_7E0EBC47D030__INCLUDED_)
#define AFX_DRMARAILWAYLOCATION_H__1804490F_9A32_4EFD_9E67_7E0EBC47D030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABackGround.h"

/**
 * @brief DRMA�S���ʒu�f�[�^�t�H�[�}�b�g�N���X
 */
typedef DRMABackGroundFormat DRMARailwayLocationFormat;

/**
 * @brief DRMA�S���ʒu�f�[�^���R�[�h�N���X
 */
class DRMARailwayLocation: public DRMABackGround
{
public:
// �\�z/����
	DRMARailwayLocation(const Format& cOriginalRecord) : DRMABackGround(cOriginalRecord)
	{
	}

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMARAILWAYLOCATION_H__1804490F_9A32_4EFD_9E67_7E0EBC47D030__INCLUDED_)
