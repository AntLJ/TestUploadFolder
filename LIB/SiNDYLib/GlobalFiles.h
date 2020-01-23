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
 * @file GlobalFiles.h
 * @brief <b>�O���[�o���t�@�C���ϐ���`�t�@�C��</b>\n
 * DLL���ŃO���[�o���Ɏg�p����ϐ����`���܂��B
 * �f�[�^�Z�O�����g�Œ�`���Ă���킯�ł͂Ȃ��̂ł�����
 * ��`���ꂽ�ϐ��̓��e�͑S�ăv���Z�X�ʂɂȂ�܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _GLOBALFILES_H_
#define _GLOBALFILES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

namespace sindy {
	// �G���[���O�p
	extern SINDYLIB_API TCHAR	g_strErrlog[];		//!< �G���[���O�t�@�C����
	extern SINDYLIB_API FILE*	g_Errlog;			//!< �G���[���O�t�@�C���n���h��
	// �g���[�X���O�p
	extern SINDYLIB_API TCHAR g_strTracelog[];		//!< �g���[�X���O�t�@�C����
	extern SINDYLIB_API FILE* g_Tracelog;			//!< �g���[�X���O�t�@�C���n���h��
} // sindy

#endif // _GLOBALFILES_H_
