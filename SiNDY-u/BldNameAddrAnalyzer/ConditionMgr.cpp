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
#include "ConditionMgr.h"
#include "Logger.h"
#include "globalfunc.h"

#include <WinLib/ADOBase.h>


using namespace sindy;


void Condition::add( const CString& key, const CComVariant& val )
{
	check( key, val );
	keys_.push_back(key);
	values_.push_back(val);
}

void Condition::check( const CString& key, const CComVariant& val )
{
	using namespace sindy::schema::sj::buildingname_src_point;
	using namespace uh::str_util;

	// SOURCETYPE_C �� 1, 2 �ȊO��NG
	if( key.CompareNoCase( kSourceType ) == 0 )
	{
		switch (val.lVal)
		{
		case source_type::kImmovables:       // �s���Y���
		case source_type::kFirePreventive:   // �h�ΑΏە����
			break;
		default:
			throw std::runtime_error( formatA( _T("�����G���[: %s=%ld"), key, val.lVal ) );
			break;
		}
	}
}

CString Condition::getQuery( IWorkspacePtr work ) const
{
	std::vector<CString> conditions;

	auto itVal = values_.cbegin();
	for( const auto& key : keys_ )
	{
		CString condition;
		switch( itVal->vt )
		{
		case VT_DATE:
			condition = getDateWhere( work, key, *itVal );
			break;
		case VT_BSTR:
			condition = uh::str_util::format( _T("%s='%s'"), key, uh::str_util::ToString(*itVal) );
			break;
		default:
			condition = uh::str_util::format( _T("%s=%s"), key, uh::str_util::ToString(*itVal) );
			break;
		}

		conditions.push_back( condition );
		++itVal;
	}

	return uh::str_util::join( conditions, _T(" AND ") );
}

void ConditionMgr::init( const CString& filePath )
{
	RUNLOGGER.Info( uh::str_util::format( _T( "�t�@�C���ǂݍ���...�F %s" ), filePath), true );

	CADOBase ado;
	
	if( !ado.connect( filePath ) )
	{
		std::string msg = "���̓t�@�C���I�[�v���G���[�F " + CStringA(filePath);
		throw std::runtime_error( msg );
	}
	
	boost::filesystem::path fullPath = filePath.GetString();
	CStringA query = "SELECT * FROM " + CStringA(fullPath.filename().c_str());
	CADOCursor cur = ado.search( query );
	if( S_OK != cur.err() )
	{
		std::string msg = "���̓t�@�C���ǂݍ��݃G���[�F " + CStringA(cur.GetErrorMessage());
		throw std::runtime_error( msg );
	}

	// CSV �w�b�_�̃L�[�t�B�[���h�ƁA�e�s�̒l���擾
	auto fields = cur.fields();
	for( auto& record : cur )
	{
		Condition condition;
		long idx = -1;
		for( const auto field : fields )
		{
			++idx; // continue�΍�Ő�ɃC���N�������g

			CComVariant val = record[idx];
			if( uh::variant_util::isNullOrEmpty( val ) )
				continue;

			condition.add( field.name, val );
		}
		add( condition ); // 1�s����ǉ�
	}

	RUNLOGGER.Info( _T( "OK.\n" ), true );
}
