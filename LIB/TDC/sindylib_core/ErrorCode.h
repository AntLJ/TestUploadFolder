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
 * @file ErrorCode.h
 * @brief SiNDY�Ŏg�p�����G���[�R�[�h���[�e�B���e�B�֐���`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _ERRORCODE_H_
#define _ERRORCODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "WinLib/macro_util.h"

namespace sindy {

	enum sindyErrLevel {
		sindyErrLevel_INFO,			//!< ���
		sindyErrLevel_WARN,			//!< �x��
		sindyErrLevel_ERR,			//!< �G���[
	};

	namespace errorcode {
		/**
		 * @enum sindyErrCode
		 * @brief SiNDY�G���[�R�[�h��`
		 */
		enum sindyErrCode {
#define ERROR_CODE(id,...) id,
#include "ErrorCodeBase.h"
#undef ERROR_CODE
		};

		/**
		 * @brief <b>sindyErrCode�𕶎���ɕϊ�����</b>\n
		 * @param code	[in]	sindyErrCode
		 * @return �G���[������
		 */
		LPCTSTR SINDYLIB_API SINDYERRCODE2STRING( sindyErrCode code );

		/**
		 * @brief <b>sindyErrCode���G���[���O�ɏo�͂���</b>\n
		 * sindyErr_NoErr�ȊO�ɔ��肳�ꂽ�Ƃ��ɂ��̊֐����Ă�ŉ������B\n
		 * �G���[�o�͐�p�̃G���[���O�t�@�C���ig_Errlog�j�ɏo�͂��܂��B
		 * g_Errlog�́A�f�t�H���g�ł�./error.log�ƂȂ�܂��B
		 * �o�͐��ς������ꍇ�́A�v���O�����̐擪��g_Errlog�̃n���h��
		 * �������ŃI�[�v������K�v������܂��B
		 * \n\n
		 * �G���[���O�o�͗�F\n
		 * 
		 * 
		 * @param code			[in]	�G���[�R�[�h
		 * @param lpcszFormat	[in]	�o�͕�����t�H�[�}�b�g
		 */
		void SINDYLIB_API _ERRORLOG( sindyErrCode code, LPCTSTR lpcszFormat, ...);

		/**
		 * @brief <b>sindyErrCode��HRESULT�l���G���[���O�ɏo�͂���</b>\n
		 * ���̊֐���sindyErrCode�̑���HRESULT�l���T�|�[�g���܂��B\n\n
		 * sindyErr_NoErr�ȊO�ɔ��肳�ꂽ�Ƃ��ɂ��̊֐����Ă�ŉ������B\n
		 * �G���[�o�͐�p�̃G���[���O�t�@�C���ig_Errlog�j�ɏo�͂��܂��B
		 * g_Errlog�́A�f�t�H���g�ł�./error.log�ƂȂ�܂��B
		 * �o�͐��ς������ꍇ�́A�v���O�����̐擪��g_Errlog�̃n���h��
		 * �������ŃI�[�v������K�v������܂��B
		 * \n\n
		 * �G���[���O�o�͗�F\n
		 * 
		 * 
		 * @param code			[in]	�G���[�R�[�h
		 * @param hr			[in]	HRESULT�l
		 * @param lpcszFormat	[in]	�o�͕�����t�H�[�}�b�g
		 */
		void SINDYLIB_API _ERRORHRLOG( sindyErrCode code, HRESULT hr, LPCTSTR lpcszFormat, ...);

		/**
		 * @brief <b>sindyErrCode��HRESULT�l���G���[���O�ɏo�͂���</b>\n
		 * ���̊֐��́AsindyErrCode��HRESULT���T�|�[�g���܂��B\n
		 * \n\n
		 * �G���[���O�o�͗�F\n
		 * 
		 * 
		 * @param code			[in]			�G���[�R�[�h
		 * @param lpcszFormat	[in]			�o�͕�����t�H�[�}�b�g
		 * @param args			[in,optional]	�ϒ��������X�g�i�f�t�H���g�FNULL�j
		 * @param out			[in,optional]	�t�@�C���n���h���i�f�t�H���g�FNULL�j\nNULL�̏ꍇ�̓t�@�C���o�͂��܂���B
		 * @param hr			[in,optional]	HRESULT�l�i�f�t�H���g�FS_OK�j
		 */
		void SINDYLIB_API ErrorlogV( sindyErrCode code, LPCTSTR lpcszFormat, va_list args = NULL, FILE* out = NULL, HRESULT hr = S_OK );

		#define ERRORLOG(x) _ERRORLOG(x,_T("%s%s S%d: "),__FILE__LINE__,__func__,x)
		#define ERRORHRLOG(x,hr) _ERRORLOG(x,_T("%s%s S%d: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,hr)
#ifdef _UNICODE
		#define ERRORLOG2(x,mes) _ERRORLOG(x,_T("%s%s S%d�i%s�j: "),__FILE__LINE__,__func__,x,L#mes)
		#define ERRORHRLOG2(x,hr,mes) _ERRORLOG(x,_T("%s%s S%d�i%s�j: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,L#mes,hr)
#else
		#define ERRORLOG2(x,mes) _ERRORLOG(x,_T("%s%s S%d�i%s�j: "),__FILE__LINE__,__func__,x,#mes)
		#define ERRORHRLOG2(x,hr,mes) _ERRORLOG(x,_T("%s%s S%d�i%s�j: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,#mes,hr)
#endif // ifdef _UNICODE

	} // errorcode
} // sindy

#endif // _ERRORCODE_H_
