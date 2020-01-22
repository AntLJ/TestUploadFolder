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
#include "RelationTableManager.h"

#include <TDC/useful_headers/str_util.h>
#include <TDC/sindylib_base/TableContainer.h>

void CRelationTableManager::Initialize( const CString& strTable, const CString& strKey, const FIELDNAMES& subFields, _IRowPtr& ipRow )
{
	m_KeyFieldName = strKey;
	m_subFields = subFields;

	// �I������Ă���t�B�[�`���[���擾
	if( !ipRow )
		return;

	// �t�B�[�`���[����e�[�u�����擾
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return;

	m_ipTable = AheOpenSameOwnerTable( ipCurrentTable, strTable );
	if( !m_ipTable )
		return ;

	// �t�B�[���h�}�b�v�̍쐬
	IFieldsPtr ipFields;
	m_ipTable->get_Fields( &ipFields );
	m_RelTableFM.CreateFieldMap( ipFields );
	return;
}

// �֘A�e�[�u���Ƀf�[�^��ǉ����鏈��
bool CRelationTableManager::AddData( long lOID, const AOI_MATRIX& lList ) const
{
	if( lList.empty() ) return true;

	_ICursorPtr ipCursor;
	if( FAILED( m_ipTable->Insert( VARIANT_TRUE, &ipCursor ) ) )
		return false;
	IRowBufferPtr ipRowBuffer;
	if( FAILED( m_ipTable->CreateRowBuffer( &ipRowBuffer ) ) )
		return false;

	for( auto& value : lList )
	{
		if( FAILED(ipRowBuffer->put_Value( m_RelTableFM.FindField( m_KeyFieldName ), CComVariant(lOID)) ) ||
		   !putValues(ipRowBuffer, value) )
			return false;

		CComVariant newRowData;
		if( FAILED( ipCursor->InsertRow(ipRowBuffer, &newRowData) ) )
			return false;
	}
	if( FAILED(ipCursor->Flush()) )
		return false;
	return true;
}

// �֘A�e�[�u���Ƀf�[�^��ǉ����鏈��
bool CRelationTableManager::AddData( long lOID, const AOI_LINE& lList ) const
{
	AOI_MATRIX tempList;
	for( auto& elem : lList )
	{
		AOI_LINE addList;
		addList.push_back( elem );
		tempList.push_back(addList);
	}
	return AddData( lOID, tempList );
}

// �֘A�e�[�u������f�[�^���폜���鏈��
bool CRelationTableManager::DeleteData( long lOID, const DELETELIST& lList ) const
{
	if( lList.empty() ) return true;

	DELETELISTS targetValues;
	// DeleteData�Ŏg�p�ł���悤�ɐ��`
	for( auto& elem : lList )
	{
		std::list<CString> addList;
		addList.push_back( elem );
		targetValues.push_back( addList );
	}

	return DeleteData( lOID, targetValues );
}

// �֘A�e�[�u������f�[�^���폜���鏈��
bool CRelationTableManager::DeleteData( long lOID, const DELETELISTS& lList ) const
{
	if( lList.empty() ) return true;

	/// ���������ɓn��������
	CString strWhere = makeDeleteWhereClause( lOID, lList );

	// �����ɓ��ěƂ܂�s���폜
	if( FAILED(m_ipTable->DeleteSearchedRows( AheInitQueryFilter( nullptr, nullptr, strWhere ) )) )
		return false;

	return true;
}

// �֘A�e�[�u������L�[�t�B�[���h��OID����v����s�̈�v�f���擾
AOI_COLUMN CRelationTableManager::GetRelationData( long lOID, const CString& strField ) const
{
	AOI_COLUMN retList;
	// (�ŏI�I�ɂ͕Ԃ�l�Ƃ��ĕԂ���)GetRelationData�Ɉ����n�����߂ɁA�v�f��strField��ǉ�
	retList.push_back( strField );

	// m_KeyFieldName��OID�ł���s��strField�̒l���擾
	auto tempList = GetRelationData( lOID, retList );

	// �Ԃ�l�Ƃ��ĕԂ����߂ɐ��`
	// AOI_MATRIX��AOI_COLUMN�ɒ���
	retList.clear();
	for( auto& columns : tempList )
	{
		// �v�f���͂ǂ��1�Ȃ̂ŁA1�������o��
		retList.push_back( *columns.begin() );
	}
	return retList;
}

// �֘A�e�[�u������OID�Ɋ֘A����s��JOIN_LINE_INFO_ID��S�Ď擾
AOI_MATRIX CRelationTableManager::GetRelationData( long lOID, const FIELDNAMES& strFields ) const
{
	AOI_MATRIX retList;

	_ICursorPtr ipCursor;
	CString strSubFields, strWhereClause;

	// �T�u�t�B�[���h��ݒ�
	strSubFields.Format( _T("%s"), m_KeyFieldName );
	for( auto& elem : strFields )
		strSubFields.AppendFormat( _T(",%s"), elem );

	// ������ݒ�
	strWhereClause.Format( _T("%s=%ld"), m_KeyFieldName, lOID );
	if( FAILED(m_ipTable->Search( AheInitQueryFilter( nullptr, strSubFields, strWhereClause ), VARIANT_TRUE, &ipCursor)) )
		return retList;

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow)==S_OK && ipRow )
	{
		AOI_LINE addList;
		for( auto& elem : strFields )
		{
			CComVariant value = AheGetAttribute( ipRow, m_RelTableFM.FindField( elem ) );
			if( value.vt!=VT_INT && value.vt!=VT_I4 && value.vt!=VT_I2 )
				continue;
			addList.push_back( uh::str_util::ToString(value) );
		}
		retList.push_back( addList );
	}
	return retList;
}

// IRowBufferPtr�ɒl������
bool CRelationTableManager::putValues( IRowBufferPtr& ipRowBuffer, const AOI_LINE& columns ) const
{
	// �}������f�[�^�ƃf�[�^�}����̃T�u�t�B�[���h�̐�����v���Ȃ��ꍇ�́A���s�Ƃ���
	if( m_subFields.size() != columns.size() )
		return false;

	auto it = columns.begin();
	for( auto& elem : m_subFields )
	{
		if( FAILED(ipRowBuffer->put_Value( m_RelTableFM.FindField(elem), CComVariant(*it))) )
		{
			return false;
		}
		++it;
	}
	return true;
}

//  DB��̃f�[�^���폜����ۂ̏������𐶐�
CString CRelationTableManager::makeDeleteWhereClause( long lOID, const DELETELISTS& lList ) const
{
	CString strWhereClause;
	strWhereClause.Format( _T("%s=%ld"), m_KeyFieldName, lOID );

	int index = 0;
	for( auto& strField : m_subFields )
	{
		//"%s in ({���̕���})"���쐬���邽�߂̃��X�g���쐬
		std::list<CString> targetValues;
		for( auto& elem : lList )
		{
			auto it = elem.begin();
			std::advance( it, index );
			if( it!=elem.end() )
				targetValues.push_back(*it);
		}
		// ��=�����̎w��̎d�������������̂ŁA�������ɂ͑����Ȃ�
		if( targetValues.empty() )
			continue;

		// �ȉ��A"in (...)"�̕����̐��`
		strWhereClause.AppendFormat(_T(" and %s in ("), strField);

		// IN�Ŏg�p�ł��镶����𐶐�
		std::list<CString> inList;
		sindy::CTableBase::IDs2Str( targetValues, inList );

		// ��ȏ�v�f�͂Ȃ����̂Ƃ��ď�������
		// TODO:1000�ȏ�̗v�f����������\��������ꍇ�́A�S�̂��������ĉ��C�̂���
		for( const auto& elem : inList )
		{
			strWhereClause.AppendFormat( _T("%s"), elem );
		}

		// �Ō�̂�������ǉ� �ŏI�I�� %s=%ld and %s IN ('%ld','%ld') �̂悤�ɂȂ�͂��B
		strWhereClause.Append(_T(")"));

		++index;
	}
	return strWhereClause;
}
