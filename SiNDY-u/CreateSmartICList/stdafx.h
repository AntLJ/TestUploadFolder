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

#include <iostream>
#include <fstream>

#include <set>
#include <map>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids, rename("IProgressDialog", "_IProgressDialog"), rename("ICommand", "_ICommand")
#import <esriGeometry.olb>	high_method_prefix("_") raw_method_prefix("")  no_namespace named_guids, rename("ISegment", "_ISegment")
#import <esriDisplay.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix(""), no_namespace, named_guids, rename("IRow", "_IRow"), rename("ICursor", "_ICursor")
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")

#include "sindy/libschema.h"
