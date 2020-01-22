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

#include "stdafx.h"
#include "ErrLogger.h"
#include "TDC/sindylib_base/TableContainer.h"
#include "ArcHelperEx/AheMeshUtilArcObjects.h"
#include <sindy/libschema.h>

using namespace std;
using namespace uh;
using namespace uh::str_util;
using namespace uh::variant_util;
using namespace sindy;
using namespace sindy::schema;
using namespace err_lv;
using namespace err_type;

void CErrLogger::header()
{
	write( _T("#FREESTYLELOG") );

	// �J������ݒ�
	vector<CString> col;
	col.emplace_back( _T("FLAG") );
	col.emplace_back( _T("LAYER") );
	col.emplace_back( _T("OBJECTID") );
	col.emplace_back( _T("�m�[�h���") );
	col.emplace_back( _T("�G���[���") );
	col.emplace_back( _T("�G���[�R�[�h") );
	col.emplace_back( _T("�G���[���e") );
	col.emplace_back( _T("�m�[�h�ڑ������N��") );
	col.emplace_back( _T("Citymesh") );
	col.emplace_back( _T("�ŏI�X�V��") );
	col.emplace_back( _T("�ŏI�X�V��") );
	col.emplace_back( _T("�ŏI�X�V�v���O������") );
	col.emplace_back( _T("�ŏI�v���O�����X�V����") );
	col.emplace_back( _T("COMMENT") );
	
	write( join( col, _T("\t") ) );
}

void CErrLogger::writeErrInfo( const CRowContainer& row, const error_type errType )
{
	vector<CString> info;

	// �t���O(0�ŌŒ�)
	info.emplace_back(_T("0"));

	// ���C����
	info.emplace_back( row.GetTableName() );

	// �I�u�W�F�N�gID
	info.emplace_back( format( _T("%d"), row.GetOID() ) );

	// �m�[�h���
	info.emplace_back( row.GetDomainName( road_node::kNodeClass ) );

	// �G���[��ʁE�G���[�R�[�h�E�G���[���e
	switch(errType)
	{
#define ERROR_CODE( code, level, msg ) \
	case code: info.emplace_back( getStrErrLv(level).c_str() ); \
	info.emplace_back( format( _T("%d"), code ) ); \
	info.emplace_back( _T(#msg) ); \
	break;
#include "ErrCode.h"
#undef ERROR_CODE
		default:
			info.emplace_back( getStrErrLv(kFatal).c_str() );
			info.emplace_back( format( _T("%d"), errType ) );
			info.emplace_back( _T("���̃G���[�R�[�h�ɂ͑Ή����Ă��܂���") );
			break;
	}

	// �m�[�h�ڑ������N��
	{
		const auto& linkT = *row.GetItemTable( road_link::kTableName );
		info.emplace_back( format( _T("%d"), linkT.size() ) );
	}

	// Citymesh
	{
		IGeometryPtr ipNode( row.CRowBase::GetShapeCopy() );
		if( !ipNode )
			info.emplace_back( _T("���b�V���R�[�h���擾�ł��܂���") );

		int x = -1, y = -1;
		int cityMeshCode = AhePoint2MESH( (IPointPtr)ipNode, 64, x, y );
		if( -1 == cityMeshCode )
			info.emplace_back( _T("���b�V���R�[�h���擾�ł��܂���") );
		else
			info.emplace_back( format( _T("%d"), cityMeshCode ) );
	}

	// �ŏI�X�V��
	// NULL�̉\��������̂ŁA��������`�F�b�N
	{
		const CComVariant vaOpeName = row.GetValue( road_node::kOperator );
		const CString strOpeName( !isNullOrEmpty( vaOpeName ) ? vaOpeName : _T("<NULL>") );
		info.emplace_back( strOpeName );
	}

	// �ŏI�X�V��
	// NULL�̉\��������̂ŁA��������`�F�b�N
	{
		const CComVariant vaModDate = row.GetValue( road_node::kModifyDate );
		const CString strModDate( !isNullOrEmpty( vaModDate ) ? vaModDate : _T("<NULL>") );
		info.emplace_back( strModDate );
	}

	// �ŏI�X�V�v���O������
	// NULL�̉\��������̂ŁA��������`�F�b�N
	{
		const CComVariant vaProgName = row.GetValue( road_node::kModifyProgName );
		const CString strProgName( !isNullOrEmpty( vaProgName ) ? vaProgName : _T("<NULL>") );
		info.emplace_back( strProgName );
	}

	// �ŏI�v���O�����X�V����
	// NULL�̉\��������̂ŁA��������`�F�b�N
	{
		const CComVariant vaProgModDate = row.GetValue( road_node::kProgModifyDate );
		const CString strProgModDate( !isNullOrEmpty( vaProgModDate ) ? vaProgModDate : _T("<NULL>") );
		info.emplace_back( strProgModDate );
	}
	
	write( join( info, _T("\t") ) );
}
