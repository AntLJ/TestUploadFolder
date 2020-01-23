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
 * @file sindycomutil.h
 * @brief COM���[�e�B���e�B�[��`
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _SINDYCOMUTIL_H_
#define _SINDYCOMUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "util.h"

#define QUERYINTERFACE(x,i,p) \
	{ \
		if( x == NULL ) \
			ERRORLOG(sindyErr_COMInterfaceIsNull); \
		else { \
			if( FAILED( x->QueryInterface( i, p ) ) ) \
				ERRORLOG(sindyErr_COMQueryInterfaceFailed); \
		} \
	}

/**
 * @def INVOKEMETHOD(x)
 * �C���^�[�t�F�[�X���\�b�h���s�����S�ɍs���}�N���ł��B
 * ���̃}�N���́ACComWrapper,CComPtr,CComQIPtr���p�����Ă���ꍇ�ɂ̂ݗL���Ȃ̂ŁA�X�}�[�g�|�C���^
 * �̃��\�b�h�̃��b�p�[����������Ƃ��Ɏg�p���Ă��������B
 * �܂��AHRESULT�͒�`�ς݂ł��̂ŁAHRESULT���K�v�ȏꍇ��hr���Q�Ƃ��Ă��������B
 *
 * <h3>��F</h3>
 *  IHoge::get_Name()���Ăяo�������ꍇ�F
 *  @code INVOKEMETHOD( get_Name( &bstrName ) ); @endcode
 *
 * @param x		[in]	���s���\�b�h
 */
#define INVOKEMETHOD(x) \
	HRESULT hr = -1; \
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing ) \
	{ \
		hr = p->x; \
		LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_COMFunctionFailed ); \
	} \

#define QUERYINVOKEMETHOD(t,i,x) \
	HRESULT hr = -1; \
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing ) \
	{ \
		t * p2 = NULL; \
		LOGASSERTE_IF( SUCCEEDED( hr = p->QueryInterface( i, (void**)&p2 ) ), sindyErr_COMQueryInterfaceFailed ) \
		{ \
			LOGASSERTE_IF( SUCCEEDED( hr = p2->x ), sindyErr_COMFunctionFailed ); \
			p2->Release(); \
		} \
	}

#define _INVOKEMETHOD2 INVOKEMETHOD
/**
 * @brief �C���^�[�t�F�[�X���\�b�h���s�����S�ɍs���}�N���ł��B
 * ���̃}�N���́A�Ăяo�����\�b�h�̈�
 * ������ŁA�����COM�C���^�[�t�F�[�X�|�C���^�ł���K�v������܂��B
 * �����ŃG���[�R�[�h�����^�[�����܂����A���̏ꍇ�͏��sindyErr_NoErr
 * ����sindyErr_COMFunctionFailed�ł��̂ŁA��҂��󂯎�����ꍇ��
 * �K��IErrorInfo�ŃG���[���e���m�F���Ă��������B
 *
 * @see INVOKEMETHOD
 *
 * <h3>��F</h3>
 * IHoge::Exec( IFuga* pFuga )���Ăяo�������ꍇ�F
 * @code INVOKEMETHOD3( pFuga, Exec ); @endcode
 *
 * @param arg	[in]	����
 * @param func	[in]	���s���\�b�h
 */
#define INVOKEMETHOD3(arg,func) \
	if( (arg) == NULL ) return sindyErr_COMInterfaceIsNull; \
	_INVOKEMETHOD2( func( arg ) ); \
	if( SUCCEEDED( hr ) ) \
		return sindyErr_NoErr; \
	else { \
		SASSERT( hr ); \
		return sindyErr_COMFunctionFailed; \
	}

/**
 * @brief �C���^�[�t�F�[�X���\�b�h���s�����S�ɍs���}�N���ł��B
 * ���̃}�N���́A::INVOKEMETHOD3�Ɩw�ǈꏏ�ł����Aarg��NULL�ł�
 * �x�����o���܂���BNULL OK�Ȉ����̂Ƃ��̂ݎg�p���Ă��������B
 *
 * @see INVOKEMETHOD3
 *
 * <h3>��F</h3>
 * IHoge::Exec( IFuga* pFuga )���Ăяo�������ꍇ�F
 * @code INVOKEMETHOD3NULLOK( pFuga, Exec ); @endcode
 *
 * @param arg	[in]	����
 * @param func	[in]	���s���\�b�h
 */
#define INVOKEMETHOD3NULLOK(arg,func) \
	_INVOKEMETHOD2( func( arg ) ); \
	if( SUCCEEDED( hr ) ) \
		return sindyErr_NoErr; \
	else { \
		SASSERT( hr ); \
		return sindyErr_COMFunctionFailed; \
	} 

#endif // _SINDYCOMUTIL_H_
