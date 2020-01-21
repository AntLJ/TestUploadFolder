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

#if !defined(AFX_STDAFX_H__FF3610C5_66A4_49FA_B9F2_D16E91D7AFFA__INCLUDED_)
#define AFX_STDAFX_H__FF3610C5_66A4_49FA_B9F2_D16E91D7AFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#import <msxml3.dll> named_guids raw_interfaces_only
// using MSXML2::IXMLDOMElementPtr;
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#import <esriSystem.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("") exclude("OLE_HANDLE", "OLE_COLOR")
#import <esriSystemUI.olb> no_namespace named_guids raw_interfaces_only raw_native_types, rename("IProgressDialog", "_IProgressDialog") rename("ICommand", "_ICommand")
#import <esriGeometry.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("") function_mapping rename("ISegment", "_ISegment")
#import <esriDisplay.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("") function_mapping
#import <esriGeoDatabase.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#import <esriDataSourcesGDB.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("")
#import <esriDataSourcesFile.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("")
#import <esriDataSourcesRaster.olb> no_namespace named_guids raw_interfaces_only raw_native_types
#import <esriOutput.olb> no_namespace named_guids raw_interfaces_only raw_native_types
#import <esriGISClient.olb> no_namespace named_guids raw_interfaces_only raw_native_types
#import <esriCarto.olb> no_namespace named_guids high_method_prefix("_") raw_method_prefix("") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#pragma warning(pop)
#pragma warning(disable : 4290)
#pragma warning(disable : 4503)
#pragma warning(disable : 4786)
#pragma warning(disable : 4819)
#pragma warning(disable : 4231)
#pragma warning(disable : 6225)
#pragma warning(disable : 6226)
#pragma warning(disable : 6334)

#define ARCTL_NO_USE_ARCMAPUI

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

// if���̎d�l��ANSI������
#ifdef _WIN32
#ifndef for
#define for if(0);else for
#endif // for
#endif // _WIN32 //

// ���̑���`
extern const TCHAR kSourceWorkspace[];
extern const TCHAR kTestWorkspace[];

extern const TCHAR kTable1[];
extern const TCHAR kTable2[];
extern const TCHAR kTable3[];

extern const TCHAR kValue1[];
extern const TCHAR kValue2[];
extern const TCHAR kValue3[];
extern const TCHAR kShape[];

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__FF3610C5_66A4_49FA_B9F2_D16E91D7AFFA__INCLUDED_)
