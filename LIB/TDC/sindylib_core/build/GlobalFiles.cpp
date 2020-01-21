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
 * @file �O���[�o���ϐ����̃t�@�C��
 * @brief DLL���ŃO���[�o���Ɏg�p����ϐ��̎��̂��`���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "GlobalFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	// �G���[���O�t�@�C���p
	TCHAR g_strErrlog[_MAX_PATH] = _T("C:\\Temp\\error.log");	//!< �G���[���O�t�@�C����
	FILE* g_Errlog = NULL;										//!< �G���[���O�t�@�C���n���h��

	// �g���[�X���O�p
	TCHAR g_strTracelog[_MAX_PATH] = _T("C:\\Temp\\trace.log");	//!< �g���[�X���O�t�@�C����
	FILE* g_Tracelog = NULL;									//!< �g���[�X���O�t�@�C���n���h��
} // sindy
