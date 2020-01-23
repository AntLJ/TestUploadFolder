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

// DBConnect.cpp: CDBConnect �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBConnect.h"

#include "..\..\LIB\WinLib\CStringTokenizer.h"

/**
 * @class	CDBConnect
 */

// �t�B�[�`���N���X�擾
IFeatureClassPtr CDBConnect::GetFeatureClassByName( LPCTSTR lpszName )
{
	IFeatureClassPtr ipFeatureClass;

	HRESULT hr = ((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass( CComBSTR(lpszName), &ipFeatureClass );

	return ( SUCCEEDED(hr)? ipFeatureClass : NULL );
}

// �Q�ƃe�[�u���擾
ITablePtr CDBConnect::GetTableByName( LPCTSTR lpszName )
{
	ITablePtr ipTable;

	HRESULT hr = ((IFeatureWorkspacePtr)m_ipWorkspace)->OpenTable(CComBSTR(lpszName), &ipTable);

	return ( SUCCEEDED(hr)? ipTable : NULL );
}

// �h���C���}�b�v�擾�֐�
DOMAINMAP CDBConnect::GetDomainMap( LPCTSTR lpszTableName, LPCTSTR lpszFieldName )
{
	int i;
	DOMAINMAP map;

	// �e�[�u���擾
	ITablePtr ipTable;
	ipTable = GetTableByName( lpszTableName );
	if( NULL == ipTable )
		return map;

	// �t�B�[���h������t�B�[���hID�擾
	LONG lFieldID;
	if( FAILED(ipTable->FindField( CComBSTR(lpszFieldName), &lFieldID )) )
		return map;

	// �Ƃ肠���������I�u�W�F�N�g���擾���āA�h���C�����擾����
	_IRowPtr ipRow;
	for( i=1;i<INT_MAX;i++ )
	{
		// �擾�ł����烋�[�v������
		ipTable->GetRow( i, &ipRow );
		if( ipRow != NULL )
			break;
	}

	if( NULL == ipRow )
		return map;

	// IField�擾����
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	IFieldPtr ipField;
	ipFields->get_Field( lFieldID, &ipField );

	// IDomain�擾���āAmap�𐶐�
	IDomainPtr ipDomain;
	if( FAILED(ipField->get_Domain( &ipDomain )) )
		return map;

	ICodedValueDomainPtr ipCVDomain(ipDomain);
	if( NULL == ipCVDomain )
		return map;

	LONG lDomainCount;
	ipCVDomain->get_CodeCount( &lDomainCount );

	USES_CONVERSION;
	for( i=0;i<lDomainCount;i++)
	{
		CComVariant vaValue;
		BSTR bstr;
		ipCVDomain->get_Value( i, &vaValue );
		ipCVDomain->get_Name( i, &bstr );


		map.insert( pair<LONG, std::string>(vaValue.lVal, OLE2A(bstr)) );
		::SysFreeString(bstr);
	}

	return map;
}

/**
 * @class	CSDEConnect
 */

// SDE�ڑ�
bool CSDEConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx /* = NULL */ )
{	
	CStringTokenizer cTokenizer( lpszDBProp, _T("/"), TOKEN_RET_EMPTY_ALL );

	if( 5 != cTokenizer.CountTokens() )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );

	m_vaUser     = cTokenizer.GetNextToken();
	m_vaPass     = cTokenizer.GetNextToken();
	m_vaVersion  = cTokenizer.GetNextToken();
	m_vaInstance = cTokenizer.GetNextToken();
	m_vaServer   = cTokenizer.GetNextToken();

	// �v���p�e�B�Z�b�g
	USES_CONVERSION;
	ipPropertySet->SetProperty( T2BSTR(_T("USER")), m_vaUser );
	ipPropertySet->SetProperty( T2BSTR(_T("PASSWORD")), m_vaPass );
	ipPropertySet->SetProperty( T2BSTR(_T("INSTANCE")), m_vaInstance );
	ipPropertySet->SetProperty( T2BSTR(_T("SERVER")), m_vaServer );
	ipPropertySet->SetProperty( T2BSTR(_T("VERSION")), m_vaVersion );

	if( ipWorkspaceFactory->Open(ipPropertySet, NULL, &m_ipWorkspace) != S_OK )
		return false;

	return true;
}

/**
 * @class	CPGDBConnect
 */

// �ڑ�
bool CPGDBConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEz /* = NULL */ )
{
	if( NULL == lpszDBProp )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	if( FAILED(ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszDBProp), 0, &m_ipWorkspace)) )
		return false;

	m_strFile = lpszDBProp;

	return true;
}

/**
 * @class	CShapeConnect
 */

// �ڑ�
bool CShapeConnect::Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx /* = NULL */ )
{
	if( NULL == lpszDBProp )
		return false;

	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );
	IPropertySetPtr ipPropertySet( CLSID_PropertySet );
	ipPropertySet->SetProperty( CComBSTR(_T("DATABASE")), CComVariant(lpszDBProp) );

	if( FAILED(ipWorkspaceFactory->Open(ipPropertySet, 0, &m_ipWorkspace)) )
		return false;

	m_strDir = lpszDBProp;

	return true;
}

//////////////////////////////////////////////////////////
//	���ʊ֐�
//////////////////////////////////////////////////////////

// PGDB���[�h?
bool isPGDBMode()
{
	return (_tgetenv(_T("PGDB_MODE")) != NULL)? true : false;
}

// ��ԁE��������
bool SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, esriSpatialRelEnum spatialRel, 
				    LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
	}

	// SubFields�w�肳��Ă�����A�Z�b�g����
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// ����
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	return ( (ppFeatureCursor != NULL) ? true : false );
}

// ��ԁE��������(�����̂ݔ�)
LONG SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
	}

	/// �������ʂ� 0 �Ȃ烊�^�[��
	LONG lHitCount = 0;
	return (SUCCEEDED(ipFeatureClass->FeatureCount(ipFilter, &lHitCount)) ? lHitCount : 0 );
}


// ��ԁE��������(�������擾��)
LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause != NULL ){
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
	}

	// SubFields�w�肳��Ă�����A�Z�b�g����
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// �������ʂ� 0 �Ȃ烊�^�[��
	LONG lHitCount = 0;
	ipFeatureClass->FeatureCount( ipFilter, &lHitCount );
	if( lHitCount < 1 ) {
		return 0;
	}

	/// ����
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	if( *ppFeatureCursor == NULL )
		return 0;
			
	return lHitCount;
}

// ��������
bool SelectByAttr( ITablePtr ipTable, _ICursor** ppCursor, LPCSTR lpszWhereClause, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

	// SubFields�w�肳��Ă�����A�Z�b�g����
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// ����
	return SUCCEEDED(ipTable->Search(ipFilter, VARIANT_FALSE, ppCursor));
}

// ��������(�����̂�)
LONG SelectByAttrOnlyCount( ITablePtr ipTable, LPCSTR lpszWhereClause )
{
	/// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g

	/// ����
	LONG lCount = 0;
	return ( SUCCEEDED(ipTable->RowCount(ipFilter, &lCount)) ? lCount : 0 );
}

// ��������(�������ꏏ��)
LONG SelectByAttrWithCount( ITablePtr ipTable, _ICursor** ppCursor, LPCSTR lpszWhereClause, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause == NULL )
		return false;
	ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

	// SubFields�w�肳��Ă�����A�Z�b�g����
	if( lpszSubFields != NULL )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	// �����擾
	LONG lHitCount = 0;
	ipTable->RowCount( ipFilter, &lHitCount );
	if( lHitCount < 1 )
		return 0;

	/// ����
	return ( SUCCEEDED(ipTable->Search(ipFilter, VARIANT_FALSE, ppCursor)) ? lHitCount : 0 );
}
