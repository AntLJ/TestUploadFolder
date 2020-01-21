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
 * @file TableBase.cpp
 * @brief CTableBase�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "type_convert.h"
#include "sindycomutil.h"
#include "sindymacroutil.h"
#include "TableTypeUtil.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "NameString.h"
#include "RowBase.h"
#include "TableBase.h"
#ifdef _DEBUG
#include "GeometryBase.h"
#endif // ifdef _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR RETURN_ONLY_FIRST_RECORD[] = _T("%s : �x���F�����̃��R�[�h��������܂������ACTableBase::GetRow()�͍ŏ��Ɍ����������R�[�h�̂ݕԂ��܂��B\n");
#else
const static _TCHAR RETURN_ONLY_FIRST_RECORD[] = _T("%s : Warning : A number of record were found, but CTableBase::GetRow() returns only the record found first. \n");
#endif	// SINDY_FOR_ENGLISH

namespace sindy {
using namespace errorcode;

CTableBase& CTableBase::operator =( const CTableBase& obj )
{
	SetObject( obj.GetTableType(), (ITable*)obj );
	// �X�L�[�}���R�s�[����ہA�R�s�[���ɃL���b�V�����Ȃ���
	// �����I�ɃR�s�[���A�R�s�[�旼���ŃL���b�V�������Ȃ�
	// ��΂Ȃ炸�A�K�w�I�ȃ��f���̏ꍇ�͒v���I�ȑ��x�ቺ��
	// �����炷
	// �Ȃ̂ŁA�����ŃL���b�V��������Ă��܂�
	obj.GetFieldMap();
	obj.GetNameString();
	CSchemaSupport::operator=(obj);
	return *this;
}

// IClass�C���^�[�t�F�[�X
IUIDPtr CTableBase::GetCLSID() const
{
	IUIDPtr ipUID;
	INVOKEMETHOD( get_CLSID( &ipUID ) );

	return ipUID;
}

long CTableBase::FindField( LPCTSTR Name ) const
{
	return GetFieldMap()->FindField( Name ); 
}

IFieldsPtr CTableBase::GetFields() const
{
	IFieldsPtr ipFields;
	INVOKEMETHOD( get_Fields( &ipFields ) );

	return ipFields;
}

bool CTableBase::HasOID() const
{
	VARIANT_BOOL vbHasOID = VARIANT_FALSE;
	INVOKEMETHOD( get_HasOID( &vbHasOID ) );

	return VB2bool(vbHasOID);
}

// ITable�C���^�[�t�F�[�X
CRowBase& CTableBase::CreateRow( CRowBase& cRow )
{
	_IRowPtr ipRow;
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		SUCCEEDED_IF( p->CreateRow( &ipRow ), sindyErr_AOCreateRowFailed, _T("TABLE[%s]"), GetTableName() )
		{
			cRow.SetObject( ipRow, GetTableType(), true );

			// �f�t�H���g�l�t�^
			cRow.SetDefaultValue();

			// �V�K�쐬���������t�^
			cRow.SetCreateAutoValue();
		}
	}
	return cRow;
}

CRowBase& CTableBase::CreateDummyRow( CRowBase& cRow )
{
	CSPFieldMap cFields;
	CSPTableNameString cNames;
	if( (_IRow*)cRow )
	{
		cFields = cRow.GetFieldMap();
		cNames = cRow.GetNameString();
	}

	if ( ! cFields ) {	// �Ȃ��Ƃ��͎����̂��g�p
		cFields = GetFieldMap();
		cNames = GetNameString();
	}

	// ���̂��Ȃ��̂ŃL���b�V����������Ă���
	cRow.CreateCache( false, cFields, cNames );

	// �f�t�H���g�l�t�^
	cRow.SetDefaultValue();

	// �V�K�쐬���������t�^
	cRow.SetCreateAutoValue();

	return cRow;
}

sindyErrCode CTableBase::DeleteSearchedRows( IQueryFilter* QueryFilter )
{
	INVOKEMETHOD3( QueryFilter, DeleteSearchedRows );
}

_IRowPtr CTableBase::GetRow( long OID ) const
{
	_IRowPtr ipRow;
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		p->GetRow( OID, &ipRow ); // ���݂��Ȃ��ꍇHRESULT��S_OK����Ȃ��Ȃ邪�A����͂��������̂ŃG���[�n���h�����O���Ȃ�
	}

	return ipRow;
}

_IRowPtr CTableBase::GetRow( IQueryFilter* QueryFilter ) const
{
	_IRowPtr ipRet;	// �Ԃ�l

	_ICursorPtr ipCursor = Search( QueryFilter, false );
	LOGASSERTE_IF( NULL != ipCursor, sindyErr_SearchFailed )
	{
		_IRowPtr ipRow;
		while( S_OK == ipCursor->NextRow( &ipRow ) )
		{
			if( NULL == ipRet )
				ipRet = ipRow;
			else {
				TRACEMESSAGE( RETURN_ONLY_FIRST_RECORD, __FILE__LINE__ );
				break;
			}
		}
	}

	return ipRet;
}

_ICursorPtr CTableBase::GetRows( const VARIANT& oids, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	INVOKEMETHOD( GetRows( oids, bool2VB(Recycling), &ipCursor ) );

	return ipCursor;
}

long CTableBase::RowCount( IQueryFilter* QueryFilter ) const
{
	long lCount = -1;
	INVOKEMETHOD( RowCount( QueryFilter, &lCount ) );

	return lCount;
}

#ifdef _DEBUG
#ifdef _USE_SINDYLIB_STATIC
void _QueryFilter_Trace( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, bool bRecycling );
#else
void _QueryFilter_Trace( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, bool bRecycling )
{
	if( ipFilter )
	{
		// �N�G���t�B���^�̃g���[�X
		CComBSTR bstrSubFields, bstrWhere;
		ipFilter->get_SubFields( &bstrSubFields );
		ipFilter->get_WhereClause( &bstrWhere );
		CString strWhere(bstrWhere);
		if( strWhere.GetLength() > 80 )
			strWhere = strWhere.Left(80) + _T("...");

		ISpatialFilterPtr ipSpFilter( ipFilter );
		if( NULL != ipSpFilter )
		{
			VARIANT_BOOL vbSpatialFilterOwnsGeometry = VARIANT_FALSE;
			IGeometryPtr ipGeom;
			CComBSTR bstrGeomField, bstrSpatialRelDesc;
			esriSearchOrder emOrder = esriSearchOrderSpatial;
			esriSpatialRelEnum emSpatialRel = esriSpatialRelIntersects;

			ipSpFilter->get_FilterOwnsGeometry( &vbSpatialFilterOwnsGeometry );
			ipSpFilter->get_Geometry( &ipGeom );
			ipSpFilter->get_GeometryField( &bstrGeomField );
			ipSpFilter->get_SearchOrder( &emOrder );
			ipSpFilter->get_SpatialRel( &emSpatialRel );
			ipSpFilter->get_SpatialRelDescription( &bstrSpatialRelDesc );
			ATLTRACE(_T("����[%s]�i���T�C�N���F%s�j�F�N�G���t�B���^�FSubFields[%s] Where[%s] SFOG[%s] GEOMF[%s] DESC[%s] ORDER[%s] REL[%d]\n"), lpcszTableName, ( bRecycling ) ? _T("����") : _T("���Ȃ�"), OLE2T(bstrSubFields), strWhere, ( vbSpatialFilterOwnsGeometry ) ? _T("TRUE") : _T("FALSE"), COLE2CT(bstrGeomField), COLE2CT(bstrSpatialRelDesc), ( emOrder == esriSearchOrderSpatial ) ? _T("SPATIAL") : _T("ATTRIBUTE"), emSpatialRel );
			CGeometryBase::Trace( ipGeom, _T("                      �F�`��") );
			IAreaPtr ipArea( ipGeom );
			if( ipArea )
			{
				double dArea = 0;
				ipArea->get_Area( &dArea );
				ATLTRACE( _T("                      �F�ʐ�[%.15lf]\n"), dArea );
			}
		}
		else {
			ATLTRACE(_T("����[%s]�i���T�C�N���F%s�j�F�N�G���t�B���^�FSubFields[%s] Where[%s]\n"), lpcszTableName, ( bRecycling ) ? _T("����") : _T("���Ȃ�"), COLE2CT(bstrSubFields), strWhere );
		}
	}
}
#endif // ifdef _USE_SINDYLIB_STATIC
#else
#define _QueryFilter_Trace ((void)0);
#endif // ifdef _DEBUG

_ICursorPtr CTableBase::Search( IQueryFilter* QueryFilter, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	_QueryFilter_Trace( GetTableName(), QueryFilter, Recycling );
	SUCCEEDED_IF( p->Search( QueryFilter, bool2VB(Recycling), &ipCursor ), sindyErr_AOFeatureSearchError, NULL )
	{
	}

	return ipCursor;
}
void CTableBase::SetObject( sindyTableType::ECode emTableType, ITable *lp)
{
	SetObject( lp );
	m_emTableType = emTableType;	// SetObject( ITable* lp )������m_emTableType������������̂Ō�ŃZ�b�g����K�v������
}

void CTableBase::SetObject( ITable* lp, const CSPFieldMap& cFields/* = CSPFieldMap()*/, const CSPTableNameString& cNames/* = CSPTableNameString()*/ )
{
	AtlComPtrAssign( (IUnknown**)&p, lp );
	
	SetFieldMap( cFields );
	SetNameString( cNames );
	SetTableType( sindyTableType::unknown );
}

// �t�B�[���h�Ή��\�̎Q�Ƃ�Ԃ�
CSPFieldMap CTableBase::GetFieldMap() const
{
	if( ! CSchemaSupport::GetFieldMap() )
		return CSchemaSupport::GetFieldMap( GetFields() );

	return CSchemaSupport::GetFieldMap();
}

// �e�[�u�����Ȃǂ��Ǘ����郁���o�̋��L�R�s�[��Ԃ�
CSPTableNameString CTableBase::GetNameString() const
{
	if( ! CSchemaSupport::GetNameString() )
		return CSchemaSupport::GetNameString( (ITable*)*this );

	return CSchemaSupport::GetNameString();
}

// �������[�N�X�y�[�X�ɑ��݂���e�[�u�����I�[�v������
ITablePtr CTableBase::OpenTable( LPCTSTR lpcszTableName ) const
{
	ITablePtr ipTable;
	IFeatureWorkspacePtr ipWorkspace( GetWorkspace() );
	if( NULL != ipWorkspace )
	{
		USES_CONVERSION;

		BSTR bstrTableName = T2BSTR(lpcszTableName);
		ipWorkspace->OpenTable( bstrTableName, &ipTable );
		::SysFreeString(bstrTableName);
	}
	return ipTable;
}

// �������[�N�X�y�[�X�ɑ��݂���e�[�u�����I�[�v������
ITablePtr CTableBase::OpenTable( LPCTSTR lpcszOwnerName, sindyTableType::ECode emTableType ) const
{
	CString strTableDotOwnerName;
	if( lpcszOwnerName && 0 < lstrlen(lpcszOwnerName) )
		strTableDotOwnerName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( emTableType ) );
	else
		strTableDotOwnerName = AheTableType2TableName( emTableType );

	return OpenTable( strTableDotOwnerName );
}

// �������[�N�X�y�[�X�ɑ��݂��铯���I�[�i�[���̃e�[�u�����I�[�v������
ITablePtr CTableBase::OpenSameOwnerTable( LPCTSTR lpcszTableName ) const
{
	CString strTableName(lpcszTableName);

	// �I�[�i�[���t���̃e�[�u�����̏ꍇ
	if( _tcschr( lpcszTableName, _T('.') ) )
	{
		// �I�[�i�[�������
		int nPeriodIndex = strTableName.Find( _T('.'), 0 );
		strTableName = strTableName.Right( strTableName.GetLength() - nPeriodIndex - 1 );
	}

	if( GetNameString()->HasOwnerName() )
		return OpenTable( CString(GetOwnerName()) + _T(".") + strTableName );
	else
		return OpenTable( strTableName );
}

// �������[�N�X�y�[�X�ɑ��݂��铯���I�[�i�[���̃e�[�u�����I�[�v������
ITablePtr CTableBase::OpenSameOwnerTable( sindyTableType::ECode emTableType ) const
{
	return OpenTable( GetOwnerName(), emTableType );
}

// ���[�N�X�y�[�X���擾����
IWorkspacePtr CTableBase::GetWorkspace() const
{
	IWorkspacePtr ipWorkspace;
	IDatasetPtr ipDataset( p );
	if( NULL != ipDataset )
		ipDataset->get_Workspace( &ipWorkspace );

	return ipWorkspace;
}

// OID���X�g�Ō�������
_ICursorPtr CTableBase::GetRows( const std::list<long>& fids, bool Recycling, LPCTSTR lpcszRelIDFieldName ) const
{
	_ICursorPtr ipRet; // �Ԃ�l

	// �n���ꂽID���SQL�p�̕�����ɕϊ�
	std::list<CString> strIDs;
	IDs2Str( fids, strIDs );

	// �֐���̐����ɂ��A����������ƂȂ�ꍇ�̓G���[�Ƃ���
	LOGASSERTE_IF( 1 >= strIDs.size(), sindyErr_ArgLimitOver )
		ipRet = Search( AheInitQueryFilter( NULL, NULL, _T("%s IN(%s)"), lpcszRelIDFieldName, *strIDs.begin() ), Recycling );

	return ipRet;
}

} // sindy
