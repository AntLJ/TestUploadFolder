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
 * @file FieldMap.cpp
 * @brief CFieldMap�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "FieldMap.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// CFieldMap���R�s�[����
void CopyFieldMap( CFieldMap& dest, const CFieldMap& src )
{
	dest.m_mapFieldIndex = src.m_mapFieldIndex;
	dest.m_vecFieldName = src.m_vecFieldName;
	dest.m_lShapeIndex = src.m_lShapeIndex;
	dest.m_lOIDIndex = src.m_lOIDIndex;
}

sindyErrCode CFieldMap::CreateFieldMap( IFields* ipFields )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	clear();	// ������

	if( ipFields != NULL )
	{
		long lFieldCount = 0;
		IFieldPtr ipField;

		ipFields->get_FieldCount( &lFieldCount );
		for( long i = 0; i < lFieldCount; ++i )
		{
			_FIELD stField;
			VARIANT_BOOL vbIsEditable = VARIANT_FALSE;

			ipFields->get_Field( i, &ipField );
			if( ipField )
			{
				CComBSTR bstrName;
				// OBJECTID�ASHPAE�̃t�B�[���h�ԍ����擾
				ipField->get_Type( &stField.type );
				if( esriFieldTypeOID == stField.type )
					m_lOIDIndex = i;
				else if( esriFieldTypeGeometry == stField.type )
					m_lShapeIndex = i;

				// �ҏW�s�����擾
				ipField->get_Editable( &vbIsEditable );
				stField.editable = VB2bool(vbIsEditable);

				// NULL OK�����擾
				ipField->get_IsNullable( &vbIsEditable );
				stField.nullable = VB2bool(vbIsEditable);

				// �t�B�[���h�������擾
				ipField->get_Name( &bstrName );
				stField.name = bstrName;
				stField.name.MakeUpper();

				// �t�B�[���h����ID�ŏI��邩�ǂ���
				// �_�~�[Row���쐬����ۂ�NULL�Z�b�g
				// �ΏۊO�ɂ��邽�߂ɂ��炩���ߔ���
				stField.isid = ( stField.name.Right(2) == _T("ID") );

				// �G�C���A�X�������擾
				bstrName.Empty();
				ipField->get_AliasName( &bstrName );
				stField.alias = bstrName;

				// �f�t�H���g�l���擾
				ipField->get_DefaultValue( &stField.dvalue );
				// �f�t�H���g�l���t�B�[���h�^�C�v�ƈقȂ�ꍇ������̂ŁA
				// �����I�ɕύX����
				if( VT_R8 == stField.dvalue.vt && esriFieldTypeInteger == stField.type )
					stField.dvalue.ChangeType(VT_I4);

				// �t�B�[���h�������擾
				long lLen = 0;
				ipField->get_Length( &lLen );
				stField.length = lLen;

				// ���������擾
				stField.precision = 0;
				ipField->get_Precision( &stField.precision );

				// �����_�ȉ����������擾
				stField.scale = 0;
				ipField->get_Scale( &stField.scale );

				// �R�[�h�l�E�����W�h���C�������擾
				IDomainPtr ipDomain;
				ipField->get_Domain( &ipDomain );
				ICodedValueDomainPtr ipCDomain( ipDomain );
				IRangeDomainPtr ipRDomain( ipDomain );
				if( ipCDomain )
				{
					long lCodeCount = 0;
					ipCDomain->get_CodeCount( &lCodeCount );
					for( long i = 0; i < lCodeCount; ++i )
					{
						CComBSTR bstrName;
						CComVariant vaValue;
						ipCDomain->get_Name( i, &bstrName );
						ipCDomain->get_Value( i, &vaValue );
						vaValue.ChangeType(VT_I4);
						stField.domain[CString(bstrName)] = vaValue.lVal;
						stField.rdomain[vaValue.lVal] = CString(bstrName);
					}
				}
				else if( ipRDomain )
				{
					ipRDomain->get_MinValue( &stField.range.first );
					ipRDomain->get_MaxValue( &stField.range.second );
				}
			}
			m_mapFieldIndex[stField.name] = i;
			m_vecFieldName.push_back( stField );
		}
	}
	else
		emErr = sindyErr_COMInterfaceIsNull;

	return emErr;
}

// �t�B�[���h�̕t���������擾����
const CFieldMap::_FIELD& CFieldMap::GetFieldAttr( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index];

	// ������̂�h�����߂ɁA�Ȃ��ꍇ�͈�ԍŌ��Ԃ�
	return *m_vecFieldName.rbegin();
}

// �t�B�[���h�C���f�b�N�X����t�B�[���h�����擾����
LPCTSTR CFieldMap::GetName( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].name;
	return NULL;
}

// �t�B�[���h�C���f�b�N�X����t�B�[���h�G�C���A�X�����擾����
LPCTSTR CFieldMap::GetAliasName( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].alias;
	return NULL;
}

const VARIANT& CFieldMap::GetDefaultValue( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].dvalue;
	return vtMissing;
}

void CFieldMap::clear()
{
	m_mapFieldIndex.clear();
	m_vecFieldName.clear();
	m_lOIDIndex = -1;
	m_lShapeIndex = -1;
}

bool CFieldMap::IsEditable( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].editable;

	return false;
}

// �t�B�[���h��NULL OK���ǂ������`�F�b�N����
bool CFieldMap::IsNullable( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].nullable;

	return false;
}

bool CFieldMap::IsSiNDYEditable(long lIndex, bool bForCopy/*=false*/) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
	{
		if ( IsEditable( lIndex ) ) {
			CString strFieldName = m_vecFieldName[lIndex].name;

			INT iPos = strFieldName.ReverseFind('.');
			if( iPos > 0 ) strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );

			bool bRet = true;

			if ( IsShapeField(lIndex) ) bRet = false;
			else if ( IsOIDField(lIndex) ) bRet = false;
			//if(      lstrcmp( strFieldName, _T("FID")            ) == 0 ) bRet = false; // FID
			//else if( lstrcmp( strFieldName, _T("SHAPE")          ) == 0 ) bRet = false; // Shape
			//else if( lstrcmp( strFieldName, _T("AREA")           ) == 0 ) bRet = false; // AREA
			//else if( lstrcmp( strFieldName, _T("SHAPE_LENGTH")   ) == 0 ) bRet = false; // Shape_length
			//else if( lstrcmp( strFieldName, _T("OBJECTID")       ) == 0 ) bRet = false; // OBJECTID
			else if( lstrcmp( strFieldName, _T("ENABLED")        ) == 0 ) bRet = false; // Enabled
			else if( lstrcmp( strFieldName, schema::ipc_table::kOperator ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kPurpose ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kModifyDate ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kUpdateType ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kProgModifyDate ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kModifyProgName ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kUserClaim ) == 0 ) bRet = false;
			else if( ( ! bForCopy ) && lstrcmp( strFieldName, _T("FIELDSURVEY_F")  ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("FROM_NODE_ID")   ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("TO_NODE_ID")     ) == 0 ) bRet = false;
			else if( strFieldName.Right( 3 ).CompareNoCase( _T("_RF")   ) == 0 ) bRet = false;
			else if( strFieldName.Right( 5 ).CompareNoCase( _T("_LQRF") ) == 0 ) bRet = false;
			else if( strFieldName.Left(  3 ).CompareNoCase( _T("TMP")   ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("NODECLASS_C") ) == 0 ) bRet = false;

			return bRet;
		}
	}

	return false;
}

// �t�B�[���h�^�C�v���擾����
esriFieldType CFieldMap::GetFieldType( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].type;

	return (esriFieldType)-1;
}

// �R�[�h�l�h���C���Ή��\���擾����
const std::map<CString,long>& CFieldMap::GetDomain( LPCTSTR lpcszFieldName ) const
{
	const_iterator it = begin();
	std::advance( it, FindField( lpcszFieldName ) );
	return it->domain;
}

// �t�B�[���h������t�B�[���h�C���f�b�N�X�ԍ����擾����
long CFieldMap::_FindField( LPCTSTR lpcszFieldName, bool bDebugTrace ) const
{
	CString strFieldName( lpcszFieldName );	// �啶���ɕϊ�����
	strFieldName.MakeUpper();

	std::map<CString,long>::const_iterator it = m_mapFieldIndex.find( strFieldName );
	if( it != m_mapFieldIndex.end() )
		return it->second;
	else if( bDebugTrace )
	{
#ifdef DEBUG
		_ASSERTE( it != m_mapFieldIndex.end() );
		// �Ή��\�̒��g��\��
		TRACEMESSAGE( _T("%s%s\n"), _T("DEBUG: CFieldMap �C���f�b�N�X�ԍ��擾�G���[�F"), strFieldName );
		TRACEMESSAGE( _T("%s\n"), _T("DEBUG: ------------------------------------") );
		for( it = m_mapFieldIndex.begin(); it != m_mapFieldIndex.end(); ++it )
			TRACEMESSAGE( _T("DEBUG: [%s]=[%d]\n"), it->first, it->second );
#endif // DEBUG			
	}
	return -1;
}

} // sindy
