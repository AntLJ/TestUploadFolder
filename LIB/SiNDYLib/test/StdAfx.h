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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__5C5891A8_2228_4D2F_982B_8D05857C09B3__INCLUDED_)
#define AFX_STDAFX_H__5C5891A8_2228_4D2F_982B_8D05857C09B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C�W���̊댯�Ȋ֐��̌x�����o���Ȃ�
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996�̌x�����o���Ȃ�
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATE��_CRT_SECURE_NO_DEPRECATE��C4996���������Ȃ��I�I

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows 2000 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0500		// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT		// Windows 2000 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0500	// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE			// IE 6.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0600	// ����� IE. �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#include <stdio.h>
#include <tchar.h>

#define _CRTDBG_MAP_ALLOC // malloc�Ń������m�ۂ������̂̃��[�N���o��
///////////////////////
// MFC���g�p����ꍇ
#ifdef _AFXDLL
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif

#include <afx.h>
#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#else
///////////////////////
// MFC���g�p���Ȃ��ꍇ
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
// MFC�Ɠ������@��new�Ń������m�ۂ������̂̃��[�N���o��
// �ecpp�t�@�C���Ɉȉ��̋L�q���K�v�ł��iMFC�Ƌ��ʁj
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif // ifdef _DEBUG
#endif // ifdef _AFX_DLL

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

#include <atlbase.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#pragma warning(disable : 4278)
#pragma warning(disable : 4336)
#import<esriSystem.olb>                 raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
#import<esriSystemUI.olb>               raw_interfaces_only raw_native_types no_namespace rename("IProgressDialog", "_IProgressDialog") rename("ICommand", "_ICommand")
#import<esriGeometry.olb>               raw_interfaces_only raw_native_types no_namespace named_guids rename("ISegment", "_ISegment")
#import<esriDisplay.olb>                raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriGeoDatabase.olb>            raw_interfaces_only raw_native_types no_namespace named_guids rename("IRow", "_IRow") rename("ICursor", "_ICursor") rename("IRelation", "_IRelation")
#import<esriGeoDatabaseUI.olb>          raw_interfaces_only raw_native_types no_namespace
#import<esriDataSourcesFile.olb>        raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriDataSourcesGDB.olb>         raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriCarto.olb>                  raw_interfaces_only raw_native_types no_namespace named_guids rename("ITableDefinition", "_ITableDefinition")
#import<esriArcMapUI.olb>               raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriFramework.olb>              raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriEditor.olb>                 raw_interfaces_only raw_native_types no_namespace
#import<esriCatalog.olb>                raw_interfaces_only raw_native_types no_namespace named_guids
#import"sindyeCore.tlb"                raw_interfaces_only raw_native_types no_namespace named_guids exclude("ISiNDYSnapAgent")
#pragma warning(pop)

#define _USE_SINDYLIB_STATIC
#include <SiNDYLib.h>
#include <list>
#include <map>
#include <vector>
#include "libschema.h"
#include "TableTypeUtil.h"

//#define _RELATIONALOPERATOR_EQUALS_BUG_TEST // IRelationalOperator::Equals����r���Ɣ�r������ւ��邾���ňقȂ錋�ʂ��������o�O���Č�������ꍇ�ɃR�����g�A�E�g���邱��
#ifdef _TEST_ALL
#define _NETWORK_TEST
#define _CHECK_INFLQ
#endif // ifdef _TEST_ALL
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__5C5891A8_2228_4D2F_982B_8D05857C09B3__INCLUDED_)
