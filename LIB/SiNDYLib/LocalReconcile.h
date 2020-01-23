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

/**
 * @file LocalReconcile.h
 * @brief <b>���[�J�����R���T�C����`�t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _LOCALRECONCILE_H_
#define _LOCALRECONCILE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {

/**
 * @brief <b>���[�J�����R���T�C�������s����</b>\n
 * �n����郏�[�N�X�y�[�X�͕ҏW�o�[�W�����i�q�o�[�W�����j�ł���A
 * ���ҏW���I���iStopEditing�j���Ă���K�v������܂��B
 * �܂��A���[�J�����R���T�C�����s��ҏW�o�[�W�����͍폜���܂���̂ŁA
 * �폜����K�v������܂��B
 * @param ipWorkspace	[in]			�ҏW�o�[�W����
 * @param ipTrackCancel	[in]	�L�����Z���g���b�J�[�iNULL OK�j
 * @param hr			[out]			HRESULT
 * @param nRetryMax		[in,optional]	���g���C�ő吔�i�f�t�H���g�F-1 �c �������j
 * @param nRetryWait	[in,optional]	���g���C���̃E�F�C�g�i�f�t�H���g�F0ms�j
 * @retval sindyErr_EditConflictDetected �R���t���N�g��������
 * @retval sindyErr_NoErr ���[�J�����R���T�C������
 * @retval ��L�ȊO �G���[
 */
errorcode::sindyErrCode SINDYLIB_API LocalReconcile( IWorkspace* ipWorkspace, ITrackCancel* ipTrackCancel, HRESULT& hr, int bRetryMax = -1, int nRetryWait = 0 );

} // sindy

#endif // ifndef _LOCALRECONCILE_H_
