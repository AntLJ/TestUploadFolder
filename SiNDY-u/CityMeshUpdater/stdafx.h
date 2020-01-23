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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B
#ifndef WINVER				// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0501		// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT		// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0501	// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_WINDOWS 0x0410 // ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE			// IE 6.0 �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define _WIN32_IE 0x0600	// ����� IE. �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

#include <atlbase.h>
#include <atlstr.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B
#define _ATL_TMP_NO_CSTRING

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>

#include <atlcrack.h>
#include <atlmisc.h>
//#include <atlctrls.h>
#include <atlddx.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable : 4146 4192)
#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("IRelationship", "_IRelationship")
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesFile.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriDataSourcesRaster.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor")
#import <esriOutput.olb> raw_interfaces_only no_namespace named_guids
#import <esriGISClient.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriGeoDatabaseExtensions.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor")
#import <esriCarto.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("UINT_PTR")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" high_method_prefix("_") raw_method_prefix("") no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#pragma warning(pop)
#pragma warning(disable : 4503 4786 6334)

#include "LocalDefine.h"
#include "Output.h"

extern COutput g_cOutput;
