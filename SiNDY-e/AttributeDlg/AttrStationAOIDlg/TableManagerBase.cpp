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
#include "TableManagerBase.h"

#include <TDC/useful_headers/str_util.h>
#include <boost/assign/list_of.hpp>
#include <sindy/schema.h>

using namespace boost::assign;
using namespace sindy::schema;

void CTableManagerBase::Join(const CString& strMain, const CString& strSub, const CString& strMainKey, const CString& strSubKey)
{
	//TODO:���S�̂��߂Ɋ󂪔�������悤�Ȃ�ǉ����Ȃ��Ƃ�������������
	m_tableSearchedKey[strSub] = strSubKey;
	m_joinMap[strMain] = std::make_pair(strSub,strMainKey);

	auto checkList = list_of (strMain) (strSub);
	// �w��e�[�u���̃t�B�[���h�}�b�v���쐬����Ă��邩���肵�A�쐬����Ă��Ȃ��Ȃ�쐬����
	for( auto& elem : checkList )
	{
		if( m_FieldMaps.find(elem)!=m_FieldMaps.end() )
			continue;
		//�V�o�Ȃ�t�B�[���h�}�b�v����
		sindy::CFieldMap fm;
		createFieldMap( elem, fm);
		m_FieldMaps[elem] = fm;
	}
}

AOI_MATRIX CTableManagerBase::GetValue( const CString& key, const CString& fieldName, const CString& strTable )
{
	ITablePtr ipTable = AheOpenSameOwnerTable( m_ipTable, strTable );
	if( !ipTable )
		return AOI_MATRIX();

	CString strWhere, strSubField;
	strWhere.Format(_T("%s=%s"), fieldName, key);
	setStrSubFields( strSubField, strTable, fieldName );

	_ICursorPtr ipCursor;
	if( FAILED(ipTable->Search( AheInitQueryFilter( nullptr, strSubField, strWhere ), VARIANT_TRUE, &ipCursor)) )
	{
		return AOI_MATRIX();
	}

	return getValue(ipCursor, strTable);
}

void CTableManagerBase::SetReturnData( const std::list<std::pair<CString,CString>>& dataList )
{
	m_dataList = dataList;
	// �T�u�t�B�[���h���ݒ�
	m_subFields.clear();
	for( const auto& elem : m_dataList )
	{
		m_subFields[elem.first].push_back( elem.second );
	}
}

AOI_MATRIX CTableManagerBase::getValue(  _ICursorPtr& ipCursor, const CString& strTable )
{
	CSearchList tempList;
	loadValue( ipCursor, strTable, tempList);
	AOI_MATRIX reList;

	// �q������Ȃ��A���g�̗v�f���Ȃ��Ȃ�A�����͂����Ŏ~�߂�
	if( tempList.m_childList.empty() && tempList.m_attrList.empty() )
		return reList;

	searchList2ListList( tempList, reList );

	// ���X�g�̒�����m_dataList�ƈ�v���Ȃ�==�֘A�f�[�^�������Ă��Ȃ��f�[�^�Ȃ̂ŁA����
	for( auto it = reList.begin(); it!=reList.end(); ++it )
	{
		if(it->size()!=m_dataList.size())
			it = reList.erase(it);
	}

	return reList;
}

void CTableManagerBase::loadValue( _ICursorPtr& ipCursor, const CString& strTable, CSearchList& parent )
{
	/// ���Ɍ�������e�[�u�������݂��邩�ۂ��̃t���O
	bool isNextExist = m_joinMap.find( strTable ) != m_joinMap.end();
	/// �������ʂƂȂ�f�[�^�����݂������ۂ��̃t���O
	bool isExistData = false;

	std::list<CString> keyValues;
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow)==S_OK && ipRow )
	{
		// �f�[�^����x�ł��ǂݎ�����Ȃ�A�t���O��ύX
		isExistData = true;

		parent.m_childList.push_back(CSearchList());
		// ���̍s�̑����擾
		getRowValue( ipRow, strTable, (*parent.m_childList.rbegin()).m_attrList );

		if( !isNextExist )
			continue;

		//���̃e�[�u������������̂ɕK�v�ȃf�[�^�̎擾
		CComVariant value = AheGetAttribute( ipRow, m_FieldMaps[strTable].FindField( m_joinMap[strTable].second ) );
		// �f�[�^�擾�Ɏ��s�������̂͏���
		if( value.vt == VT_EMPTY )
			continue;
		//������������ۂ̃L�[�ƂȂ�l���擾
		CString strValue = uh::str_util::ToString( value );
		keyValues.push_back(strValue);
	}
	if( !isNextExist || !isExistData )
		return ;

	const CString& strNextTable = m_joinMap[strTable].first;
	const CString& strKeyField = m_tableSearchedKey[strNextTable];

	ITablePtr ipTable = AheOpenSameOwnerTable( m_ipTable, strNextTable );
	if( !ipTable )
		return ;

	auto it = parent.m_childList.begin();
	for( auto& key : keyValues )
	{
		CString strWhere, strSubField;
		strWhere.Format(_T("%s=%s"), strKeyField, key);
		setStrSubFields( strSubField, strNextTable );

		_ICursorPtr ipRelCursor;
		if( FAILED(ipTable->Search( AheInitQueryFilter( nullptr, strSubField, strWhere ), VARIANT_TRUE, &ipRelCursor)) )
		{
			return ;
		}
		loadValue(ipRelCursor, strNextTable, *it );
		++it;
	}
}

void CTableManagerBase::getRowValue( _IRowPtr& ipRow, const CString& strTable, AOI_LINE& addList )
{
	FIELDNAMES& list = m_subFields[strTable];
	for( auto& elem : list )
	{
		CComVariant value = AheGetAttribute( ipRow, m_FieldMaps[strTable].FindField(elem) );
		if( value.vt != VT_EMPTY )
			addList.push_back( uh::str_util::ToString(value) );
	}
}

// �T�u�e�[�u���̃t�B�[���h���Z�b�g
void CTableManagerBase::setStrSubFields( CString& strSubField, const CString& strTable, const CString& keyField )
{
	// �����������p��set(�d�����������߂Ɏg�p)
	std::set<CString> whereSet;

	// ���̃e�[�u�������������ۂɕK�v�ƂȂ�t�B�[���h������ꍇ�́A�����ǂݏo��
	if( !m_tableSearchedKey[strTable].IsEmpty() )
		whereSet.insert(m_tableSearchedKey[strTable]);

	// ���̃e�[�u������������ۂɕK�v�ƂȂ�t�B�[���h������ꍇ�́A�����ǂݏo��
	if( !m_joinMap[strTable].second.IsEmpty() )
		whereSet.insert(m_joinMap[strTable].second);

	// ��������ۂ̃L�[�ƂȂ�t�B�[���h��ݒ�
	if( !keyField.IsEmpty() )
		whereSet.insert(keyField);

	// ���̑��T�u�t�B�[���h��ݒ�
	for( auto& elem : m_subFields[strTable] )
		whereSet.insert( elem );

	// �i�Ȃ��͂������j�����ƂȂ���̂��Ȃ��Ȃ珈���͂����ŏI����
	if( whereSet.empty() )
		return;

	auto it = whereSet.begin();

	// �n�߂̈�������ʂɏ���
	strSubField.Format( _T("%s"), *it );

	for( ; it!= whereSet.end(); ++it )
	{
		strSubField.AppendFormat( _T(",%s"), *it );
	}
}

// SearchList��AOI_MATRIX�̌`���ɕϊ�(�ċA)
void CTableManagerBase::searchList2ListList( CSearchList& ops, AOI_MATRIX& lList ) const 
{
	if( ops.m_childList.empty() )
	{
		lList.push_back( ops.m_attrList );
	}

	bool firstFlag = true;
	for( auto& child : ops.m_childList )
	{
		AOI_MATRIX childDataList;
		searchList2ListList( child, childDataList );
		firstFlag = false;
		for( auto& elem : childDataList )
		{
			lList.push_back( ops.m_attrList );
			//.merge�ĂԂƏ��ԓ���ւ��... merge�͏��Ԉێ����Ă���Ȃ��H
			//.splice�Ŗ����ɘA�����Ă��ʖ�...�d���Ȃ��̂ň�����
			for( auto& elem2 : elem )
				lList.rbegin()->push_back( elem2 );
		}
	}
}

// �t�B�[���h�}�b�v���쐬
bool CTableManagerBase::createFieldMap( const CString& strTable, sindy::CFieldMap& fieldMap )
{
	ITablePtr ipTable =  AheOpenSameOwnerTable( m_ipTable, strTable );
	if( !ipTable )
		return false;

	IFieldsPtr ipFields;
	ipTable->get_Fields( &ipFields );
	if( !ipFields )
		return false;

	fieldMap.CreateFieldMap( ipFields );
	return true;
}
