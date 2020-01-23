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

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

#include <atlbase.h>
#include <atlstr.h>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#import <esriSystem.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriOutput.olb> raw_interfaces_only no_namespace named_guids
#import <esriGeoDatabase.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("ItemInfo","_ItemInfo") rename("IRelationship","_IRelationship")
#import <esriCarto.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("PropertyInfo", "_PropertyInfo")  rename("ITableDefinition", "_ITableDefinition" )
#pragma warning(pop)
#import <esriDataSourcesFile.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <list>
#include <set>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <float.h>
#include <libarchelper.h>
#include <coord_converter.h>

#define foreach BOOST_FOREACH
