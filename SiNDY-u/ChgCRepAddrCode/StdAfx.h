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

#if !defined(AFX_STDAFX_H__3447A067_32B6_42B9_8B4F_E1E1608D787F__INCLUDED_)
#define AFX_STDAFX_H__3447A067_32B6_42B9_8B4F_E1E1608D787F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "atlbase.h"
using namespace std;	

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
//#import "c:\ArcGIS\arcexe82\Bin\esriCore.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids , exclude ("OLE_COLOR", "OLE_HANDLE")
#import "c:\Program Files\ArcGIS\com\esriSystem.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriGeometry.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriGeoDatabase.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriDataSourcesGDB.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
//#import "c:\Program Files\ArcGIS\com\esriDataSourcesFile.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#pragma warning(pop)

#define WIN32_LEAN_AND_MEAN // Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�

#include <stdio.h>

// TODO: �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B
#include <conio.h>
#include <map>
#include <set>
#include "Str.h"	// CString �N���X�iMFC �ƈꏏ�{���j

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__3447A067_32B6_42B9_8B4F_E1E1608D787F__INCLUDED_)
