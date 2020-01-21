/**
 * @file StdAfx.h
 * @brief �W���̃V�X�e�� �C���N���[�h �t�@�C���A�܂��͎Q�Ɖ񐔂������A<br>
 *        �����܂�ύX����Ȃ�
 *        �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q����Ă��܂��B<br>
 *        �܂��A���̃t�@�C���̓v���R���p�C���w�b�_�ƂȂ邽�߁A���ׂĂ�
 *        *.cpp�t�@�C���ŃC���N���[�h����K�v������܂��B
 *
 * @author furukawa (INCREMENT P CORP.) <furukawa@mr.ipc.pioneer.co.jp>
 * $Date: 2004/02/27 05:12:16 $
 * $Id: StdAfx.h,v 1.14 2004/02/27 05:12:16 hata_k Exp $
 *
 * Copyright &copy; 2004 INCREMENT P CORP., Inc. All rights reserved.
 */

#if !defined(AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_)
#define AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_

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

// ����sindykTools��v110�Ή��ł��Ă��Ȃ������b��Ή��B�B
// ���̂Ƃ�����ƂȂ�̂�nullptr�����Ȃ̂ł���ŗ���
#if _MSC_VER == 1500
#define nullptr NULL
#endif

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#include <stdio.h>
#include <tchar.h>

#define 	_QUOTE(x)   # x
#define 	QUOTE(x)   _QUOTE(x)
#define 	__FILE__LINE__   __FILE__ "(" QUOTE(__LINE__) ") : "

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

#define OEMRESOURCE

#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlctl.h>	// IPropertyPageImpl ���g�p����̂�

#include <WinLib/arcobjects_import_rawmethod.h>
#import "tlb/sindyeCore.tlb"					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "_ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#import <msxml6.dll>

extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_AttributeSelectDlgObj = {0x5AEA755B,0xFCCA,0x41C0,{0x8C,0x3F,0x19,0xBF,0x60,0xFC,0x3C,0x3F}};

#define __SINDYE_NO_CHECK_NODE_ATTRIBUTE__	// [bug 1944] �񍳘H�̃m�[�h�ɐM���@�E�����_���̑��������Ă���ꍇ�̊m�F�͕s�v
#define _RELATIONALOPERATOR_EQUALS_BUG_FIXED
#define _NOT_USE_SDE_RELATIONSHIP

#include <list>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "define.h"
#include "charutil.h"
#include "Str2.h"
#include "sindy/libschema.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

/**
 * \mainpage
 *
 * ���̃��C�v�����́AArcObjects���g�p�����ŕ֗��Ǝv����N���X
 * �Q��֐��Q���W�߂����ƂȂ��Ă��܂��B
 *
 * ���C�u�������̂�CVS�ŊǗ�����Ă��܂��B
 * ���ɗ��R���Ȃ�����A�ŐV�ł�ArcHelper���g�p���邱�Ƃ������߂��܂��B
 * 
 * ArcHelperEx�̎擾���@�F
 *
 * �C���N���[�h�t�@�C���A���C�u�����A�֘A���C�u�����A�֘A���C�u�����C���N���[�h�t�@�C��
 * �����ׂĎ擾���Ă����K�v������܂��B
 *
 * - SiNDY-e/ArcHelperEx/include �ȉ�
 * - SiNDY-e/COMMON/ �ȉ�
 * - SiNDY-u/COMMON/lib/crd_cnv.lib
 * - SiNDY-u/crd_cnv/crd_cnv.h
 * - TOOLS/ �ȉ�
 *
 * �������́ASiNDY-e�ASiNDY-u���ۂ��ƃ`�F�b�N�A�E�g���Ă��������B
 *
 * ArcHelperEx�̗��p���@�F
 * 
 * COMMON/ArcHelperEx.h���C���N���[�h���Ă��������B
 */
#endif // !defined(AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_)
