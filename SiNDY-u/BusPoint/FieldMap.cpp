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

#include "StdAfx.h"
#include "FieldMap.h"

// �t�B�[���h�}�b�v�쐬
void CFieldMap::CreateFieldMap( const ITablePtr& ipTable )
{
	IFieldsPtr ipFields;
	ipTable->get_Fields( &ipFields );
	if( ipFields ){
		CreateFieldMap( ipFields );
	}
	else{
		m_mapField.clear();
	}
}

// �t�B�[���h�}�b�v�쐬
void CFieldMap::CreateFieldMap( const IFieldsPtr& ipFields )
{
	m_mapField.clear();	// ��U�폜

	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );

	for( long i = 0; i < lFieldCount; ++i ){
		CComBSTR bstr;
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstr );
		m_mapField.insert( std::pair<CString, long>(CString(bstr), i) );
	}
}
