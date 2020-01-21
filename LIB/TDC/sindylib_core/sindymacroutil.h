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
 * @file util.h
 * @brief ���[�e�B���e�B�[�֐��̒�`
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

extern bool g_bTRACEMESSAGE_STDOUT; //!< TRACEMESSAGE��W���o�͂ɏo�����ǂ����i�f�t�H���g�Ftrue�j

/**
 * @brief <b>�g���[�X���o��</b>\n
 * �f�o�b�O���[�h�̎��̓f�o�b�O�E�B���h�E�ƃX�g���[���ɁA
 * �����[�X���[�h�̎��̓X�g���[���݂̂Ƀg���[�X���o�͂��܂��B
 * �o�̓X�g���[���ɂ�sindy::g_Tracelog���g�p����̂ŁA
 * �A�v���P�[�V�����N�����ɃZ�b�g���Ă��������B
 * @param lpcszFormat	[in]	�o�͕�����t�H�[�}�b�g
 */
void SINDYLIB_API TRACEMESSAGE( LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>�g���[�X���o��</b>\n
 * �f�o�b�O���[�h�̎��̓f�o�b�O�E�B���h�E�ƃX�g���[���ɁA
 * �����[�X���[�h�̎��̓X�g���[���݂̂Ƀg���[�X���o�͂��܂��B
 * @param lpcszFormat	[in]	�o�͕�����t�H�[�}�b�g
 */
void SINDYLIB_API TRACEMESSAGE( FILE* out, LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>�g���[�X���o��</b>\n
 * �f�o�b�O���[�h�̎��̓f�o�b�O�E�B���h�E�ƃX�g���[���ƃv���O���X�ɁA
 * �����[�X���[�h�̎��̓X�g���[���ƃv���O���X�݂̂Ƀg���[�X���o�͂��܂��B
 * @param ipProgress	[in]	�v���O���X�R���|�[�l���g
 * @param lStep			[in]	�X�e�b�v��
 * @param bIsPercent	[in]	�X�e�b�v�����p�[�Z���e�[�W���ǂ���
 * @param bAddMsg		[in]	�v���O���X�Ƀ��b�Z�[�W��ǒǋL����ꍇ��true		
 * @param lpcszFormat	[in]	�o�͕�����t�H�[�}�b�g
 */
void SINDYLIB_API TRACEMESSAGE( IProgressor* ipProgress, long lStep, bool bIsPercent, bool bAddMsg, LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>�g���[�X���o��</b>\n
 * �t�@�C���ɏo�͂���ꍇ�̂݁A�u# yyyy/mm/dd hh:mm:ss �v���w�b�_�Ƃ��čs���ɂ��܂�
 * @param lpcszFormat	[in]			�o�͕�����t�H�[�}�b�g
 * @param args			[in,optional]	�ϒ��������X�g�i�f�t�H���g�FNULL�j
 * @param out			[in,optional]	�t�@�C���n���h���i�f�t�H���g�FNULL�j
 * @param bOutDebugger	[in,optional]	�f�o�b�O�ł̏ꍇ�ɃA�E�g�v�b�g�E�B���h�E�ɏo�͂���ꍇ��true�i�f�t�H���g�Ffalse�j
 * @param bOutStdOut	[in,optional]	�W���o�͂ɏo�͂���ꍇ��true�i�f�t�H���g�Ffalse�j
 * @param ipProgress	[in,optional]	�v���O���X�R���|�[�l���g�i�f�t�H���g�FNULL�j
 * @param bStep			[in,optional]	�����ɃX�e�b�v����Ȃ�true�i�f�t�H���g�Ffalse�j
 * @param bUseStepValue	[in,optinoal]	step���X�e�b�v�l�Ƃ��Ďg�p����Ȃ�true�i�f�t�H���g�Ffalse�j
 * @param bIsPercent	[in,optional]	step���p�[�Z���e�[�W�Ȃ�true�i�f�t�H���g�Ffalse�j
 * @param step			[in,optional]	�X�e�b�v�l���̓p�[�Z���e�[�W�i�f�t�H���g�F0�j
 * @param bAddMsg		[in,optional]	�v���O���X�Ƀ��b�Z�[�W��ǋL����ꍇ��true		
 */
void SINDYLIB_API TraceMessageV( 
				   LPCTSTR lpcszFormat, 
				   va_list args = NULL, 
				   FILE* out = NULL, 
				   bool bOutDebugger = false,
				   bool bOutStdOut = false,
				   IProgressor* ipProgress = NULL, 
				   bool bStep = false,
				   bool bUseStepValue = false,
				   bool bIsPercent = false,
				   long step = 0,
				   bool bAddMsg = false);

/**
 * @brief <b>�G���[���O�ɏo�͂��Ă��烊�^�[������</b>\n
 * sindyErrCode�����^�[������ꍇ�́A���̃}�N�����g�p����ƃ��O��
 * �����Ă����̂ŕ֗��ł��B
 * @param x	[in]	sindyErrCode
 */
#define ERR_RETURN(x) \
	{ \
		_ERRORLOG(x,__func__); \
		return x; \
	}

#ifdef _DEBUG
#define RETURN_IF(expr,err) \
	{ \
		if(expr) { \
			do { \
				if( 1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { \
					ERRORLOG(err); \
					_CrtDbgBreak(); \
				} \
			} while (0); \
			return (err); \
		} \
	}
#else
#define RETURN_IF(expr,err) \
	{ \
		if(expr) { \
			ERRORLOG(err); \
			return (err); \
		} \
	}
#endif

#define FUNCTRACE(isstart) TRACEMESSAGE( _T("%s : %s\n"), __func__, ( isstart ) ? _T("START") : _T("END") );
#define FUNCTRACE2(isstart,errcode) TRACEMESSAGE( _T("%s : %s %s\n"), __func__, ( isstart ) ? _T("START") : _T("END"), SINDYERRCODE2STRING(errcode) );

/**
 * @brief <b>�A�T�[�g�؂�ւ��p�}�N��</b>\n
 * ���b�Z�[�W�{�b�N�X�`���A���O�`���Ȃǂ̐؂�ւ��̂��߂�
 * �}�N���ł��B���C�u�������ł͕K��SASSERT���g�p���Ă��������B
 * ���b�Z�[�W�{�b�N�X�`���̎��ɔ��莮��\���������ꍇ�́A
 * SASSERTE���g�p���Ă��������B
 */
#ifdef _DEBUG
#define SASSERT(expr) \
	do { if (!(expr) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) _CrtDbgBreak(); } while (0)
#else
#define SASSERT(expr) ((void)0)
#endif // ifdef _DEBUG

/**
 * @brief <b>�A�T�[�g�؂�ւ��p�}�N��</b>\n
 * ���b�Z�[�W�{�b�N�X�`���A���O�`���Ȃǂ̐؂�ւ��̂��߂�
 * �}�N���ł��B���C�u�������ł͕K��SASSERT���g�p���Ă��������B\n
 */
#ifdef _DEBUG
#define SASSERTE(expr) \
	do { if (!(expr) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr))) _CrtDbgBreak(); } while (0)
#else
#define SASSERTE(expr) SASSERT(expr)
#endif // ifdef _DEBUG

/**
 * @brief <b>���O��ASSERT���o���}�N��</b>\n
 * ���b�Z�[�W�{�b�N�X�`���̎��ɔ��莮��\���������ꍇ�́A
 * LOGASSERTE���g�p���Ă��������B
 */
#ifdef _DEBUG
#define LOGASSERT(expr,err) \
do { if (!(expr)) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { ERRORLOG(err); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERT(expr,err) do { if(!(expr)) ERRORLOG(err); } while (0)
#endif // ifdef _DEBUG

/**
 * @brief <b>���O��ASSERT���o���}�N��</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTE(expr,err) \
do { if (!(expr)) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) { ERRORLOG(err); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERTE(expr,err) LOGASSERT(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>FAILED����̏ꍇ��HRESULT���݂̃��O��ASSERT���o���}�N��</b>\n
 * if( SUCCEEDED() )�̑���Ɏg�p���邱�Ƃ��ł��܂��B\n
 * ��������FAILED�̏ꍇ�ɍs�����������O�o�͂�ASSERTE�̏ꍇ�݂̂ɗL���ł�
 * �̂ŁAelse�����L�q���ăG���[�������s���K�v������܂���B
 * 
 * e.x.
 * SUCCEEDED_IF( ipCurve->get_Length( &dLen ), sindyErr_GeometryFunctionFailed, _T("%s"), _T("���s������") )
 * {
 *   ...
 * }
 */
#ifdef _DEBUG
#define SUCCEEDED_IF(expr,err,format,...) \
	if(FAILED(expr)) \
	{ \
		do { \
			if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) \
			{ \
				_ERRORHRLOG((err),(expr),(format), __VA_ARGS__); \
				_CrtDbgBreak(); \
			} \
		} \
		while(0); \
	} else
#else
#define SUCCEEDED_IF(expr,err,format,...) \
	if(FAILED(expr)) \
	{ \
		_ERRORHRLOG((err),(expr),(format), __VA_ARGS__); \
	} else
#endif // ifdef _DEBUG

/**
 * @brief <b>���O��ASSERT���o���}�N��</b>\n
 * ���̃}�N����HRESULT��p�ł��Bexpr�ɂ�HRESULT���w�肵�Ă��������B
 */
#ifdef _DEBUG
#define LOGASSERTHR(expr,err) \
do { if (FAILED((expr))) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { ERRORHRLOG((err),(expr)); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERTHR(expr,err) do { if(FAILED((expr))) ERRORHRLOG((err),(expr)); } while (0)
#endif // ifdef _DEBUG
#ifdef _DEBUG
#define LOGASSERTEHR(expr,err) \
do { if (FAILED((expr))) { ERRORHRLOG((err),(expr)); if( 1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr) ) _CrtDbgBreak(); } } while (0)
#else
#define LOGASSERTEHR(expr,err) LOGASSERTHR(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERT���o���}�N��</b>\n
 * �A�T�[�g�_�C�A���O�̒��ɏ��������o�������ꍇ��LOGASSERTE_IF��
 * �g�p���Ă��������B
 */
#ifdef _DEBUG
#define ASSERT_IF(expr) if( !(expr) ) { \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define ASSERT_IF(expr) if( ! (expr) ) {} else
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERT���o���}�N��</b>\n
 */
#ifdef _DEBUG
#define ASSERTE_IF(expr) if( !(expr) ) { \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define ASSERTE_IF(expr) ASSERT_IF(expr)
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERTE�ƃ��O���o���}�N��</b>\n
 * �A�T�[�g�_�C�A���O�̒��ɏ��������o�������ꍇ��LOGASSERTE_IF��
 * �g�p���Ă��������B
 */
#ifdef _DEBUG
#define LOGASSERT_IF(expr,err) if( !(expr) ) { \
	ERRORLOG(err); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERT_IF(expr,err) if( !(expr) ) { ERRORLOG(err); } else
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERTE�ƃ��O���o���}�N��</b>\n
 * �A�T�[�g�_�C�A���O�̒��ɏ��������o�������ꍇ��LOGASSERTE_IF��
 * �g�p���Ă��������B
 */
#ifdef _DEBUG
#define LOGASSERTM_IF(expr,err,format,...) if( !(expr) ) { \
	_ERRORLOG((err),(format),__VA_ARGS__); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTM_IF(expr,err,format,...) if( !(expr) ) { _ERRORLOG((err),(format),__VA_ARGS__); } else
#endif // ifdef _DEBUG


/**
 * @brief <b>�����ɂ�����Ȃ����ASSERT�ƃ��O���o���}�N��</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTE_IF(expr,err) if( !(expr) ) { \
	ERRORLOG(err); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTE_IF(expr,err) LOGASSERT_IF(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERT�ƃ��O���o���}�N��</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTEM_IF(expr,err,format,...) if( !(expr) ) { \
	_ERRORLOG(err,(format),__VA_ARGS__); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTEM_IF(expr,err,format,...) LOGASSERTM_IF(expr,err,format,__VA_ARGS__)
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERTE�ƃ��O���o���}�N��</b>\n
 * emErr�ɃG���[�R�[�h���Z�b�g���܂��BemErr�͌Ăяo�����Œ�`����
 * �K�v������܂��B\n
 * �A�T�[�g�_�C�A���O�̒��ɏ��������o�������ꍇ��LOGASSERTEERR_IF��
 * �g�p���Ă��������B
 */
#ifdef _DEBUG
#define LOGASSERTERR_IF(expr,err) if( !(expr) ) { \
	emErr = err; \
	ERRORLOG(emErr); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTERR_IF(expr,err) if( !(expr) ) { emErr = err; ERRORLOG(err); } else
#endif // ifdef _DEBUG

/**
 * @brief <b>�����ɂ�����Ȃ����ASSERT�ƃ��O���o���}�N��</b>\n
 * emErr�ɃG���[�R�[�h���Z�b�g���܂��BemErr�͌Ăяo�����Œ�`����
 * �K�v������܂��B
 */
#ifdef _DEBUG
#define LOGASSERTEERR_IF(expr,err) if( !(expr) ) { \
	emErr = err; \
	ERRORLOG(emErr); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTEERR_IF(expr,err) LOGASSERTERR_IF(expr,err)
#endif // ifdef _DEBUG

#if defined(_USE_SINDYLIB_STATIC) || defined(SINDYLIB_EXPORTS) || defined(_LIB)
/**
 * @brief <b>COM�G���[���擾����֐�</b>\n
 * ���̊֐���CString��Ԃ����߁ADLL�łł͔���J���\�b�h�ɂȂ�܂�
 */
CString GETCOMERRORSTRING();
#endif // if defined(_USE_SINDYLIB_STATIC) || defined(SINDYLIB_EXPORTS) || defined(_LIB)

#endif // _UTIL_H_
