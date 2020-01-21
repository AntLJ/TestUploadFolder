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
#include "NameString.h"
#include "FieldMap.h"
#include "TableTypeUtil.h"
#include "ErrorCode.h"
#include "SchemaSupport.h"

using namespace sindy;
using namespace sindy::errorcode;


#ifndef SINDY_FOR_ENGLISH
const static _TCHAR TABLENAME_STR_IS_NOT_INITIALIZED[] = _T("CTableNameStringが初期化されていません");
const static _TCHAR FIELDMAP_IS_NOT_INITIALIZED[]      = _T("CFieldMapが初期化されていません");
#else
const static _TCHAR TABLENAME_STR_IS_NOT_INITIALIZED[] = _T("CTableNameString is not initialized.");
const static _TCHAR FIELDMAP_IS_NOT_INITIALIZED[]      = _T("CFieldMap is not initialized.");
#endif	// SINDY_FOR_ENGLISH

CSchemaSupport::CSchemaSupport() : m_emTableType(sindyTableType::unknown)
{
}

CSchemaSupport::~CSchemaSupport()
{
}

CSchemaSupport::CSchemaSupport( const CSchemaSupport& obj ) :
	m_emTableType(obj.m_emTableType),
	m_spNames(obj.m_spNames),
	m_spFields(obj.m_spFields)
{
}

CSchemaSupport::CSchemaSupport( sindyTableType::ECode emType, CSPTableNameString spNames, CSPFieldMap spFields ) :
	m_emTableType( emType ),
	m_spNames( spNames ),
	m_spFields( spFields )
{
}

const CSchemaSupport& CSchemaSupport::operator=( const CSchemaSupport& obj )
{
	m_emTableType = obj.m_emTableType;
	m_spNames = obj.m_spNames;
	m_spFields = obj.m_spFields;
	return *this;
}

CSPFieldMap CSchemaSupport::GetFieldMap( IFields* ipFields ) const
{
	m_spFields.reset( new CFieldMap( ipFields ) );
	return m_spFields;
}

CSPTableNameString CSchemaSupport::GetNameString( ITable* ipTable ) const
{
	m_spNames.reset( new CTableNameString( ipTable ) );
#ifdef _DEBUG
	if( m_spNames.get()->IsSDE() )
	{
		ATLTRACE(_T("%s : CTableNameString cache created : %s.%s[%s]%s@%s:%s\n"), 
			_T("CSPTableNameString CSchemaSupport::GetNameString( ITable* ipTable ) const"),
			m_spNames.get()->GetOwnerName(),
			m_spNames.get()->GetTableName(),
			m_spNames.get()->GetVersionName(),
			m_spNames.get()->GetUserName(),
			m_spNames.get()->GetServerName(),
			m_spNames.get()->GetInstanceName()
			);
	}
	else {
		ATLTRACE(_T("%s : CTableNameString cache created : %s\\\\%s\n"),
			_T("CSPTableNameString CSchemaSupport::GetNameString( ITable* ipTable ) const"),
			m_spNames.get()->GetTableName(),
			m_spNames.get()->GetPathName()
			);
	}
#endif // ifdef _DEBUG

	return m_spNames;
}

sindyTableType::ECode CSchemaSupport::GetTableType() const
{
	if( sindyTableType::unknown == m_emTableType )
	{
		if( GetNameString() )
			m_emTableType = AheTableName2TableType( GetNameString()->GetTableName() );
	}
	return m_emTableType;
}

// テーブル名を返す
LPCTSTR CSchemaSupport::GetTableName() const
{
	CSPTableNameString spNames( GetNameString() );
	if( ! spNames )
		return NULL; // ダミーコンテナとして使いたい場合にASSERTが邪魔なので、警告は出さない

	return spNames->GetTableName();
}

// テーブルオーナー名を取得する
LPCTSTR CSchemaSupport::GetOwnerName() const
{
	CSPTableNameString spNames( GetNameString() );
	LOGASSERTEM_IF( spNames, sindyErr_ClassMemberMissing, TABLENAME_STR_IS_NOT_INITIALIZED )
	{
		return spNames->GetOwnerName();
	}
	return NULL;
}

//オーナー名付きテーブル名を取得する
LPCTSTR CSchemaSupport::GetOwnerTableName() const
{
	CSPTableNameString spNames( GetNameString() );
	LOGASSERTEM_IF( spNames, sindyErr_ClassMemberMissing, TABLENAME_STR_IS_NOT_INITIALIZED )
	{
		return spNames->GetOwnerTableName();
	}
	return NULL;
}

// OBJECTIDのフィールドインデックス番号を取得する
long CSchemaSupport::GetOIDFieldIndex() const
{
	CSPFieldMap cFields( GetFieldMap() );
	LOGASSERTEM_IF( cFields, sindyErr_ClassMemberMissing, FIELDMAP_IS_NOT_INITIALIZED )
	{
		return cFields->GetOIDIndex();
	}
	return -1;
}

// フィールド名からインデックス番号を取得する
long CSchemaSupport::FindField( LPCTSTR lpcszFieldName ) const
{
	CSPFieldMap spFields( GetFieldMap() );
	LOGASSERTEM_IF( spFields, sindyErr_ClassMemberMissing, FIELDMAP_IS_NOT_INITIALIZED )
	{
		return spFields->FindField( lpcszFieldName );
	}
	return -1;
}
