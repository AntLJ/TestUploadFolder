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

#if !defined(AFX_STDAFX_H__461694DE_5494_475B_82A6_09F6CA75E05F__INCLUDED_)
#define AFX_STDAFX_H__461694DE_5494_475B_82A6_09F6CA75E05F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// TODO: �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B
#include <atlbase.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#import <esriSystem.olb>			high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE", "OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriGeometry.olb>			high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment", "_ISegment")
#import <esriDisplay.olb>			raw_interfaces_only raw_native_types no_namespace
#import <esriGeoDatabase.olb>		high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow", "_IRow") rename("ICursor", "_ICursor") rename("IRelationship", "_IRelationship")
#import <esriDataSourcesFile.olb>	high_method_prefix("_"), raw_method_prefix(""), no_namespace, named_guids
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#pragma warning(pop)

#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include "Objects.h"

#define EXE_ERRORLOG(msg, data, ret)					\
	{													\
	if( data )											\
		std::cerr << msg << " : " << data << std::endl;	\
	else												\
		std::cerr << msg << std::endl;					\
	return ret;											\
	}

#include "sindy/libschema.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__461694DE_5494_475B_82A6_09F6CA75E05F__INCLUDED_)
