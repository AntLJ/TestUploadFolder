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

// DRMASite.h: DRMASite �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMASITE_H__E1EC300E_BAB1_4F7A_B9F6_A92E3C1A6F6F__INCLUDED_)
#define AFX_DRMASITE_H__E1EC300E_BAB1_4F7A_B9F6_A92E3C1A6F6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABackGround.h"

/**
 * @brief DRMA���n�f�[�^�t�H�[�}�b�g�N���X
 */
typedef DRMABackGroundFormat DRMASiteFormat;

/**
 * @brief DRMA���n�f�[�^���R�[�h�N���X
 */
class DRMASite : public DRMABackGround
{
public:
// �\�z/����
	DRMASite(const Format& cOriginalRecord) : DRMABackGround(cOriginalRecord)
	{
	}

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMASITE_H__E1EC300E_BAB1_4F7A_B9F6_A92E3C1A6F6F__INCLUDED_)
