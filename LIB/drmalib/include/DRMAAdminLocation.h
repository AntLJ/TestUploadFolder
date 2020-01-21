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

// DRMAAdminLocation.h: DRMAAdminLocation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_)
#define AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABackGround.h"

/**
 * @brief DRMA�s���E�ʒu�f�[�^�t�H�[�}�b�g�N���X
 */
typedef DRMABackGroundFormat DRMAAdminLocationFormat;

/**
 * @brief DRMA�s���E�ʒu�f�[�^���R�[�h�N���X
 */
class DRMAAdminLocation : public DRMABackGround
{
public:
// �\�z/����
	DRMAAdminLocation(const Format& cOriginalRecord) : DRMABackGround(cOriginalRecord)
	{
	}

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_)
