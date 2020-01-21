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

#define WIN32_LEAN_AND_MEAN // Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�

#include <atlbase.h>

#import <msxml3.dll> named_guids raw_interfaces_only
using MSXML2::IXMLDOMElementPtr;
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#import "../../ArcObjects/esriCore.olb" high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE", "OLE_COLOR")
#pragma warning(pop)

#pragma warning(disable : 4786)
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/field/index.h>

// ���̑���`
extern const TCHAR kTmpWorkspace[];

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__5C5891A8_2228_4D2F_982B_8D05857C09B3__INCLUDED_)
