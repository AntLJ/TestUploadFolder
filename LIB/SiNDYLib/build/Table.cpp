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
 * @file Table.cpp
 * @brief CTable�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "Table.h"
#include "sindycomutil.h"
#include "util.h"
#include "TableTypeUtil.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "CreatableLayers.h"

#ifdef _DEBUG
#include "Geometry.h"
#endif // ifdef _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR GETROW_RETURN_FIRST_RECORD  = _T("%s : Warning : A number of record were found, but CTable::GetRow() returns only the record found first. \n");
#else
	LPCTSTR GETROW_RETURN_FIRST_RECORD  = _T("%s : �x���F�����̃��R�[�h��������܂������ACTable::GetRow()�͍ŏ��Ɍ����������R�[�h�̂ݕԂ��܂��B\n;");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

// IClass�C���^�[�t�F�[�X
IUIDPtr CTable::GetCLSID() const
{
	IUIDPtr ipUID;
	INVOKEMETHOD( get_CLSID( &ipUID ) );

	return ipUID;
}

long CTable::FindField( LPCTSTR Name ) const
{
	long lIndex = -1;
	if( ! Name ) return sindyErr_ArgIsNull;
	BSTR bstrName = T2BSTR(Name);
	INVOKEMETHOD( FindField( bstrName, &lIndex ) );
	SysFreeString(bstrName);

	return lIndex;
}

IFieldsPtr CTable::GetFields() const
{
	IFieldsPtr ipFields;
	INVOKEMETHOD( get_Fields( &ipFields ) );

	return ipFields;
}

bool CTable::HasOID() const
{
	VARIANT_BOOL vbHasOID = VARIANT_FALSE;
	INVOKEMETHOD( get_HasOID( &vbHasOID ) );

	return VB2bool(vbHasOID);
}

// ITable�C���^�[�t�F�[�X
CRow& CTable::CreateRow( CRow& cRow )
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

CRow& CTable::CreateDummyRow( CRow& cRow )
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

sindyErrCode CTable::DeleteSearchedRows( IQueryFilter* QueryFilter )
{
	// ���C�A�E�g�r���[�ɐ؂�ւ����GeometryBag�ł̌����Ɏ��s����Ή�
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	INVOKEMETHOD3( QueryFilter, DeleteSearchedRows );

	// ���ɖ߂��Ă���
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );
}

_IRowPtr CTable::GetRow( long OID ) const
{
	_IRowPtr ipRow;
	INVOKEMETHOD( GetRow( OID, &ipRow ) );

	return ipRow;
}

_IRowPtr CTable::GetRow( IQueryFilter* QueryFilter ) const
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
				TRACEMESSAGE(GETROW_RETURN_FIRST_RECORD, __FILE__LINE__ );
				break;
			}
		}
	}

	return ipRet;
}

_ICursorPtr CTable::GetRows( const VARIANT& oids, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	INVOKEMETHOD( GetRows( oids, bool2VB(Recycling), &ipCursor ) );

	return ipCursor;
}

long CTable::RowCount( IQueryFilter* QueryFilter ) const
{
	// ���C�A�E�g�r���[�ɐ؂�ւ����GeometryBag�ł̌����Ɏ��s����Ή�
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	long lCount = -1;
	INVOKEMETHOD( RowCount( QueryFilter, &lCount ) );

	// ���ɖ߂��Ă���
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );

	return lCount;
}

#ifdef _DEBUG
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
			CGeometry::Trace( ipGeom, _T("                      �F�`��") );
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
#else
#define _QueryFilter_Trace ((void)0);
#endif // ifdef _DEBUG

_ICursorPtr CTable::Search( IQueryFilter* QueryFilter, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	_QueryFilter_Trace( GetTableName(), QueryFilter, Recycling );

	// ���C�A�E�g�r���[�ɐ؂�ւ����GeometryBag�ł̌����Ɏ��s����Ή�
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	SUCCEEDED_IF( p->Search( QueryFilter, bool2VB(Recycling), &ipCursor ), sindyErr_AOFeatureSearchError, NULL )
	{
	}

	// ���ɖ߂��Ă���
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );

	return ipCursor;
}
void CTable::SetObject( sindyTableType::ECode emTableType, ITable *lp)
{
	SetObject( lp );
	m_emTableType = emTableType;	// SetObject( ITable* lp )������m_emTableType������������̂Ō�ŃZ�b�g����K�v������
}

void CTable::SetObject( ITable* lp, const CSPFieldMap& cFields/* = CSPFieldMap()*/, const CSPTableNameString& cNames/* = CSPTableNameString()*/ )
{
	AtlComPtrAssign( (IUnknown**)&p, lp );
	
	m_emTableType = sindyTableType::unknown;
	m_spFields = cFields;
	m_spNameString = cNames;
}

// �t�B�[���h�Ή��\�̎Q�Ƃ�Ԃ�
CSPFieldMap CTable::GetFieldMap() const
{
	if( ! m_spFields )
		m_spFields.reset( new CFieldMap( GetFields() ) );

	return m_spFields;
}

// �e�[�u�����Ȃǂ��Ǘ����郁���o�̋��L�R�s�[��Ԃ�
CSPTableNameString CTable::GetNameString() const
{
	if( ! m_spNameString )
		m_spNameString.reset( new CTableNameString( (ITable*)*this ) );

	return m_spNameString;
}

// �������[�N�X�y�[�X�ɑ��݂���e�[�u�����I�[�v������
ITablePtr CTable::OpenTable( LPCTSTR lpcszTableName ) const
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
ITablePtr CTable::OpenTable( LPCTSTR lpcszOwnerName, sindyTableType::ECode emTableType ) const
{
	CString strTableDotOwnerName;
	if( lpcszOwnerName && 0 < lstrlen(lpcszOwnerName) )
		strTableDotOwnerName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( emTableType ) );
	else
		strTableDotOwnerName = AheTableType2TableName( emTableType );

	return OpenTable( strTableDotOwnerName );
}

// �������[�N�X�y�[�X�ɑ��݂��铯���I�[�i�[���̃e�[�u�����I�[�v������
ITablePtr CTable::OpenSameOwnerTable( LPCTSTR lpcszTableName ) const
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
ITablePtr CTable::OpenSameOwnerTable( sindyTableType::ECode emTableType ) const
{
	return OpenTable( GetOwnerName(), emTableType );
}

// ���[�N�X�y�[�X���擾����
IWorkspacePtr CTable::GetWorkspace() const
{
	IWorkspacePtr ipWorkspace;
	IDatasetPtr ipDataset( p );
	if( NULL != ipDataset )
		ipDataset->get_Workspace( &ipWorkspace );

	return ipWorkspace;
}

/*// OID���X�g�Ō�������
_ICursorPtr CTable::GetRows( const std::list<long>& fids, bool Recycling ) const
{
	SAFEARRAYBOUND bound = { fids.size(), 0 };
	SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
	LONG* pArray;
	SafeArrayAccessData( pSa, (void**)&pArray );

	long i = 0;
	for( std::list<long>::const_iterator it = fids.begin(); it != fids.end(); ++it, ++i )
		pArray[i] = *it;

	SafeArrayUnaccessData( pSa );
	VARIANT vaArray;
	VariantInit( &vaArray );
	vaArray.vt = VT_ARRAY | VT_I4;
	vaArray.parray = pSa;

	_ICursorPtr ipCursor = GetRows( vaArray, Recycling );
	VariantClear( &vaArray );

	return ipCursor;
}*/

// OID���X�g�Ō�������
_ICursorPtr CTable::GetRows( const std::list<long>& fids, bool Recycling, LPCTSTR lpcszRelIDFieldName ) const
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

sindyTableType::ECode CTable::GetTableType() const
{
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		if( sindyTableType::unknown == m_emTableType )
			m_emTableType = AheTableName2TableType( GetNameString()->GetTableName() );
	}

	return m_emTableType;
}

// TODO:����C������Ƃ��ɁA���̊֐��폜��������Ă�������
// Ver8.0.1�ł́A�܂��Ƃ肠�����c���Ă����܂��B
// ��ԎQ�ƍĐݒ�
ISpatialReferencePtr CTable::resetSpRef(IQueryFilter* QueryFilter, WKSEnvelope& wksenv) const
{
	// 
	ISpatialReferencePtr ipSpRef;
	ISpatialFilterPtr ipSpFilter( QueryFilter );

	if ( ipSpFilter )
	{
		IGeometryPtr ipGeom;
		ipSpFilter->get_Geometry( &ipGeom );

		if ( ipGeom )
		{
			esriGeometryType emtype = esriGeometryAny;
			ipGeom->get_GeometryType( &emtype );

			if ( esriGeometryBag == emtype )	// GeometryBag�̎�����
			{
				IGeometryPtr ipGeom_Shift( (IGeometryPtr)AheCreateClone( ipGeom ) );

				// ��ԎQ�ƂƂ���...(��ԎQ�Ƃ̃R�s�[�ł͂��߂Ȃ�)
				ipGeom_Shift->get_SpatialReference( &ipSpRef );
				if ( ipSpRef )
				{
					ipSpRef->GetDomain( &wksenv.XMin, &wksenv.XMax, &wksenv.YMin, &wksenv.YMax );

					// �V�t�g����...
					double dbShift = 156.25f;
					ipSpRef->SetDomain( wksenv.XMin - dbShift, wksenv.XMax - dbShift, wksenv.YMin - dbShift, wksenv.YMax - dbShift );

					// �Z�b�g���Ȃ���
					CGeometry cGeom( ipGeom_Shift );
					for ( CGeometry::part_iterator itPart = cGeom.part_begin(); itPart != cGeom.part_end(); ++itPart )
					{
						if ( (*itPart) )
							(*itPart)->putref_SpatialReference( ipSpRef );
					}

					ipGeom_Shift->putref_SpatialReference( ipSpRef );
					ipSpFilter->putref_Geometry( ipGeom_Shift );
				}
			}
		}
	}

	return ipSpRef;
}

} // sindy
