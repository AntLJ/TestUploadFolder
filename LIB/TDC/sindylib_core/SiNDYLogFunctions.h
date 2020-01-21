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
 * @file SiNDYLogFunctions.h
 * @brief <b>SiNDY�Ńn���h�����O�\�ȃ��O�̃��[�e�B���e�B�֐��Q��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SINDYLOGFUNCTIONS_H_
#define _SINDYLOGFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {
enum sindyLogFormat {
	sindyLogFormat_Common,			//!< ���O�n���h���W���t�H�[�}�b�g�i�J���}��؂�j
	sindyLogFormat_Freestyle,		//!< ���R�`�����O�i�J���}��؂�j
	sindyLogFormat_CommonTab,		//!< ���O�n���h���W���t�H�[�}�b�g�i�^�u��؂�j
	sindyLogFormat_FreestyleTab,	//!< ���R�`�����O�i�^�u��؂�j
};

enum sindyLogLevel {
	sindyLogLevel_INFO,			//!< ���
	sindyLogLevel_WARN,			//!< �x��
	sindyLogLevel_ERR,			//!< �G���[
};

/**
 * @brief ���O�n���h���c�[���p�̃w�b�_���o�͂���
 *
 * �t���[�X�^�C�����O�̏ꍇ�̃t�H�[�}�b�g�͈ȉ��̂Ƃ���ł��B<br>
 * @code
 *   FLAG,LAYER,OBJECTID,MESHCODE,X,Y,LATITUDE,LONGITUDE,SCALE,COMMENT{�G���[���x��},COMMENT{�G���[�R�[�h},COMMENT{�G���[���b�Z�[�W},COMMENT{���l},COMMENT{�t���[�R�����g}
 * @endcode
 * �o�͕����R�[�h��SJIS�ł��B
 *
 * @param out				[in]			�X�g���[��
 * @param emFormat			[in]			�t�H�[�}�b�g�`��
 */
void SINDYLIB_API SINDYLOGHEADER( FILE* out, sindyLogFormat emFormat );

/**
 * @brief ���O�n���h���c�[���Ή��̃t�H�[�}�b�g�`���Ń��O���o�͂���
 *
 * �G���[�R�[�h�A�G���[�������sindyErrCode����擾���܂��B<br>
 * �W���`���̏ꍇ�A�Ō�̕�����͏��P�Ƃ��ďo�͂���܂��B<br>
 * �t���[�X�^�C���̏ꍇ�̃t�H�[�}�b�g��SINDYLOGHEADER���Q�Ƃ��Ă��������B<br>
 * �o�͕����R�[�h��SJIS�ł��B
 *
 * @param out				[in]			�X�g���[��
 * @param emFormat			[in]			�t�H�[�}�b�g�`��
 * @param lpcszTableName	[in]			�e�[�u����
 * @param lOID				[in]			�I�u�W�F�N�gID
 * @param scale				[in]			�X�P�[��
 * @param dX				[in]			��\X���W
 * @param dY				[in]			��\Y���W
 * @param emErrLevel		[in]			�G���[���x��
 * @param emErr				[in]			�G���[�R�[�h
 * @param console			[in]			�W���o�͂ɏo�����ǂ���
 * @param dbg				[in]			�f�o�b�O�E�B���h�E�ɏo�͂��邩�ǂ���
 * @param lpcszFormat		[in,optional]	���P
 */
void SINDYLIB_API SINDYCOMMONLOG2V( FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat, va_list args );
void SINDYLIB_API SINDYCOMMONLOG2(  FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat = NULL, ... );
void SINDYLIB_API SINDYCOMMONLOG(   FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, LPCTSTR lpcszFormat = NULL, ... );

} // sindy

#endif // _SINDYLOGFUNCTIONS_H_
