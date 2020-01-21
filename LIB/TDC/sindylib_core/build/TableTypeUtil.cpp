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
 * @file TableTypeUtil.cpp
 * @brief <b>�e�[�u���^�C�v�g�p���[�e�B���e�B�����t�@�C��</b>\n
 * ��`�̍ۂɁAFeatureTableType.h��ǂݍ���Ń}�N���o�R�Œ�`���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "sindymacroutil.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

/**
 * @brief <b>�e�[�u���^�C�v����e�[�u�����ɕϊ�����</b>\n
 * @return �e�[�u����
 */
LPCTSTR AheTableType2TableName( sindyTableType::ECode emCode )
{
	switch( emCode )
	{
#ifndef _UNICODE
#define FEATURETABLE_TYPE(name_space,table_name,class_name) case sindyTableType::name_space: return #table_name; break;
#else
#define FEATURETABLE_TYPE(name_space,table_name,class_name) case sindyTableType::name_space: return L#table_name; break;
#endif // _UNICODE
#include "FeatureTableType.h"
#undef FEATURETABLE_TYPE
		default:
			ERRORLOG(sindyErr_UnknownTableType);
			SASSERT(false);
			return NULL;
	}
}

static std::map<CString, sindyTableType::ECode> g_mapTableName2TableType;

// �e�[�u��������e�[�u���^�C�v�ɕϊ�����
sindyTableType::ECode AheTableName2TableType( LPCTSTR lpcszTableName )
{
	// ����̂ݏ�����
	if( g_mapTableName2TableType.empty() )
	{
#ifndef _UNICODE
#define FEATURETABLE_TYPE(name_space,table_name,class_name) g_mapTableName2TableType.insert(std::map<CString, sindyTableType::ECode>::value_type(#table_name, sindyTableType::name_space) );
#else
#define FEATURETABLE_TYPE(name_space,table_name,class_name) g_mapTableName2TableType.insert( std::map<CString, sindyTableType::ECode>::value_type(L#table_name, sindyTableType::name_space) );
#endif // _UNICODE
#include "FeatureTableType.h"
#undef FEATURETABLE_TYPE
	}

	// �啶���ɕϊ�
	CString strTableName( lpcszTableName );
	strTableName.MakeUpper();

	std::map<CString, sindyTableType::ECode>::const_iterator it = g_mapTableName2TableType.find( strTableName );
	if( it != g_mapTableName2TableType.end() )
		return it->second;
	else {
		return sindyTableType::other; // unknown�͖��������Aother�͖��o�^�Ƃ���
	}
}

// �e�[�u���^�C�v��LQ�n���ǂ���
bool AheIsLQTableType( sindyTableType::ECode emCode )
{
	switch( emCode )
	{
		case sindyTableType::lq_byway:
		case sindyTableType::lq_dirguide:
		case sindyTableType::lq_guide:
		case sindyTableType::lq_internavi:
		case sindyTableType::lq_lane:
		case sindyTableType::lq_route:
		case sindyTableType::lq_turnreg:
		case sindyTableType::lq_uturn:
		case sindyTableType::lq_vics:
		case sindyTableType::lq_ipcvics:
		case sindyTableType::lq_intersection: return true;
		default: break;
	}

	return false;
}

} // sindy
