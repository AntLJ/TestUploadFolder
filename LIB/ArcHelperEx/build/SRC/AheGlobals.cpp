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
* @file AheGlobals.cpp
*
* @brief Arc Helper �O���[�o���֐������t�@�C��
*/
#include "stdafx.h"

#include "AheGlobals.h"
#include "AheGeometryOp.h"
#include "AheLayerFunctions.h"
#include "WKSHelper.h"
#ifdef DEBUG
#include "AheDraw.h"
#endif
#include "Map.h"
#include "AheNetwork.h"
#include "GlobalFunctions.h"
#include "AheBugEvasion.h"
#include "type_convert.h"
#include <boost/assign/list_of.hpp>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy::schema;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR WARN_NODE_LAYER_NOT_FOUND               = _T("�m�[�h���C���𔭌��ł��܂���ł����B\n���̂��߁A����ȍ~�̑���Ő������������Ȃ�\��������܂��B");
	LPCTSTR NO_NODE_LAYER                           = _T("�m�[�h���C��������܂���");
	LPCTSTR ERROR_FAIL_SET_SHAPE_TO_FEATURE         = _T("�t�B�[�`���ւ̌`�󔽉f�Ɏ��s���܂���\n");
	LPCTSTR ERROR_DOMAIN_SET_EXCEPT_NUMERICAL_VALUE = _T("���l�ȊO�̒l�ɑ΂��ăR�[�h�l�h���C�����ݒ肳��Ă��܂�");
	LPCTSTR ERROR_KEY_EVENT_EXCEPTION_OCCURRED      = _T("�L�[�C�x���g�ł̈ړ����ɗ�O���������܂����FCODE[%d] SHIFT[%d]");
	LPCTSTR ZOOM_IN_OUT_SCROLL                      = _T("�g��k���X�N���[��");
#else
	LPCTSTR WARN_NODE_LAYER_NOT_FOUND               = _T("Node layer was not found. \n For this reason, the consistency may occur in subsequent operations.");
	LPCTSTR NO_NODE_LAYER                           = _T("There is no node layer.");
	LPCTSTR ERROR_FAIL_SET_SHAPE_TO_FEATURE         = _T("Failed to set shape to feature.\n");
	LPCTSTR ERROR_DOMAIN_SET_EXCEPT_NUMERICAL_VALUE = _T("A coded value domain is set for a value except the numerical value");
	LPCTSTR ERROR_KEY_EVENT_EXCEPTION_OCCURRED      = _T("An exception occurred during the movement by the key event�FCODE[%d] SHIFT[%d]");
	LPCTSTR ZOOM_IN_OUT_SCROLL                      = _T("Zoom in/out and scroll");
#endif // SINDY_FOR_ENGLISH
}

/////////////////////////////////////////////////////////////////////////////
//
// �ݒ�l����
//
/////////////////////////////////////////////////////////////////////////////
UINT AheGetSelectTolerance( LPCTSTR lpcszInifilePath)
{
	LPCTSTR lpcsz;
	TCHAR szInifilePath[ MAX_PATH];
	if( NULL == lpcszInifilePath)
	{
		AheGetInifilePath( szInifilePath, MAX_PATH);
		lpcsz = szInifilePath;
	}
	else
	{
		lpcsz = lpcszInifilePath;
	}
	UINT unTolerance = ::GetPrivateProfileInt( TEXT( "Tools"), TEXT( "Tolerance"), 8, lpcsz);

	return max( min( unTolerance, 100), 1);
}

int AheGetInifilePath( LPTSTR lpszPath, int nSize)
{
	TCHAR  szOpenRegistry[MAX_PATH];
	lstrcpy( szOpenRegistry, SOFTWARE_KEY);
	lstrcat( szOpenRegistry, INSTALL_KEY);
	lstrcat( szOpenRegistry, INSTALLROOT_KEY);
	HKEY hOpenKey = EmxCreateRegKey( szOpenRegistry, HKEY_LOCAL_MACHINE);
    if( hOpenKey)
    {
		CString cStrPath;
		BOOL blResult = EmxGetRegString( PATH_KEY, cStrPath.GetBuffer( MAX_PATH), MAX_PATH, hOpenKey);
		cStrPath.ReleaseBuffer();
        if( FALSE != blResult)
		{
			CString cStrFile;
			blResult = EmxGetRegString( INIFILEPATH_KEY, cStrFile.GetBuffer( MAX_PATH), MAX_PATH, hOpenKey);
			cStrFile.ReleaseBuffer();

			if( FALSE != blResult)
			{
				if( TEXT( '\\') != cStrPath.Right( 1))
				{
					cStrPath += TEXT( '\\');
				}
				cStrPath += cStrFile;

				if( nSize > cStrPath.GetLength())
				{
					lstrcpyn( lpszPath, cStrPath, nSize);
				}
				return cStrPath.GetLength();
			}
		}
    }
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//
// ���[�N�X�y�[�X�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
// ���[�N�X�y�[�X���擾����
IWorkspacePtr AheGetWorkspace( IUnknown* ipUnk )
{
	IWorkspacePtr ipWorkspace( ipUnk );
	if( ipWorkspace )
		return ipWorkspace;

	IFeatureLayerPtr ipLayer( ipUnk );
	IDatasetPtr ipDataset( ipUnk );
	_IRowPtr ipRow( ipUnk );
	if( ipRow )
	{
		ITablePtr ipTable;
		ipRow->get_Table( &ipTable );
		ipDataset = ipTable;
	}
	else if( ipLayer )
	{
		IFeatureClassPtr ipClass;
		ipLayer->get_FeatureClass( &ipClass );
		ipDataset = ipClass;
	}

	if( ipDataset )
		IDataset_getWorkspace( ipDataset, &ipWorkspace );

	return ipWorkspace;

}

// �����[�g���[�N�X�y�[�X���ǂ����𒲂ׂ�
bool AheIsRemoteWorkspace( IWorkspace* ipWorkspace )
{
	CComBSTR bstrPath;
	if( ipWorkspace )
		ipWorkspace->get_PathName( &bstrPath );

	return ( 0 < bstrPath.Length() ) ? false : true;
}

//////////////////////////////////////////////////////////////////////
//
// ����Ɋւ���֐�
//
//////////////////////////////////////////////////////////////////////
// ���H���C�����ǂ����̔���
BOOL AheIsRoadLayer( ILayerPtr piLayer)
{
	return AheCompareLayerName( piLayer, ROAD_TABLE_NAME);
}

// �m�[�h���C�����ǂ����̔���
BOOL AheIsNodeLayer( ILayerPtr piLayer)
{
	return AheCompareLayerName( piLayer, NODE_TABLE_NAME);
}

// ���H�N���X���ǂ����̔���
BOOL AheIsRoadClass( IFeatureClassPtr piFeatureClass)
{
	return AheCompareFeatureName( piFeatureClass, ROAD_TABLE_NAME);
}

// �m�[�h�N���X���ǂ����̔���
BOOL AheIsNodeClass( IFeatureClassPtr piFeatureClass)
{
	return AheCompareFeatureName( piFeatureClass, NODE_TABLE_NAME);
}

// �m�[�h�t�B�[�`�����ǂ����̔���
BOOL AheIsNodeFeature( IMapPtr piMap, IFeaturePtr piFeature)
{
	IFeatureClassPtr piFeatureClass;
	if( AheGetFeatureClass( piMap, piFeature, &piFeatureClass))
	{
		return AheCompareFeatureName( piFeatureClass, NODE_TABLE_NAME);
	}
	return FALSE;
}

// ���H�t�B�[�`�����ǂ����̔���
BOOL AheIsRoadFeature( IMapPtr piMap, IFeaturePtr piFeature)
{
	IFeatureClassPtr piFeatureClass;
	if( AheGetFeatureClass( piMap, piFeature, &piFeatureClass))
	{
		return AheCompareFeatureName( piFeatureClass, ROAD_TABLE_NAME);
	}
	return FALSE;
}

BOOL AheIsChukiLayer( LPCTSTR lpcszFeatureClassName )
{
	if( 0 != lstrcmpi( lpcszFeatureClassName, _T("CITY_DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("CITY_ANNOTATION"))     &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("B_SC4DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("B_SC3DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("B_SC2DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("B_SC1DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("BASE_ANNOTATION"))     &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("M_SC4DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("M_SC3DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("M_SC2DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("M_SC1DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("MIDDLE_ANNOTATION"))   &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("T_SC4DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("T_SC3DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("T_SC2DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("T_SC1DISP_LINE"))      &&
		0 != lstrcmpi( lpcszFeatureClassName, _T("TOP_ANNOTATION")) )
		return FALSE;
	return TRUE;
}

bool AheIsPoiLayer(const CString& layerName)
{
	using namespace sindy::schema::sindyk;
	static const std::set<CString> layers = boost::assign::list_of
		( CString(poi_point::kTableName) )
		( CString(poi_sub_point::kTableName) )
		( CString(poi_point_org::kTableName) )
		( CString(poi_sub_point_org::kTableName) );

	return ( layers.end() != layers.find( layerName ) );
}

// �m�[�h���C�����擾
ILayerPtr AheGetNodeLayer( IMapPtr piMap, BOOL bEditableOnly/* = FALSE*/ )
{
	ILayerPtr ipRetLayer = NULL;

	if( AheGetLayerByName( piMap, NODE_TABLE_NAME, &ipRetLayer, _FIND_TABLE, bEditableOnly ) == TRUE ) return ipRetLayer;

	// �x��
	::MessageBox( ::GetActiveWindow(), 
		WARN_NODE_LAYER_NOT_FOUND,
		NO_NODE_LAYER, MB_OK );

	return NULL;
}

// ���H���C�����擾
ILayerPtr AheGetRoadLayer( IMapPtr piMap, BOOL bEditableOnly/* = FALSE*/ )
{
	ILayerPtr ipRetLayer = NULL;

	if( AheGetLayerByName( piMap, ROAD_TABLE_NAME, &ipRetLayer, _FIND_TABLE, bEditableOnly ) == TRUE ) return ipRetLayer;

	return NULL;
}

// �m�[�h�N���X���擾
IFeatureClassPtr AheGetNodeClass( IMapPtr piMap, BOOL bEditableOnly/* = FALSE*/ )
{
	IFeatureLayerPtr piFeatureLayer( AheGetNodeLayer( piMap, bEditableOnly ) );
	IFeatureClassPtr piFeatureClass = NULL;

	if( piFeatureLayer == NULL ) return NULL;
	piFeatureLayer->get_FeatureClass( &piFeatureClass );

	return piFeatureClass;
}

// ���H�N���X���擾
IFeatureClassPtr AheGetRoadClass( IMapPtr piMap, BOOL bEditableOnly/* = FALSE*/ )
{
	IFeatureLayerPtr piFeatureLayer( AheGetRoadLayer( piMap, bEditableOnly ) );
	IFeatureClassPtr piFeatureClass = NULL;

	if( piFeatureLayer == NULL ) return NULL;
	piFeatureLayer->get_FeatureClass( &piFeatureClass );

	return piFeatureClass;
}

// �t�B�[�`�����\������Ă��郌�C���ɑ����Ă��邩�ǂ����𒲂ׂ�
BOOL AheIsVisibleFeature( IMap* ipMap, IFeature* ipFeature )
{
	if( ! ipMap || ! ipFeature )
		return FALSE;

	IMapLayersPtr ipMapLayers( ipMap );

	// �t�B�[�`�����������郌�C���i�\������Ă�����́j�擾
	std::list<CAdapt<ILayerPtr> > listLayers = AheFindLayers( ipMap, ipFeature, true );

	for( std::list<CAdapt<ILayerPtr> >::const_iterator it = listLayers.begin(); it != listLayers.end(); ++it )
	{
		// ���̃t�B�[�`�����{���ɕ`��ł��邩�ǂ����������_���ɖ₢���킹
		IFeatureRendererPtr ipRenderer;
		if( SUCCEEDED( ((IGeoFeatureLayerPtr)it->m_T)->get_Renderer( &ipRenderer ) ) )
		{
			ISymbolPtr ipSymbol;
			if( SUCCEEDED( ipRenderer->get_SymbolByFeature( ipFeature, &ipSymbol ) ) && ipSymbol != NULL )
				return TRUE;
		}
	}
	return FALSE;
}

// �t�B�[�`���A�e�[�u�����������郌�C�����f�[�^�t���[�����猟������
std::list<CAdapt<ILayerPtr> > AheFindLayers( IMap* ipMap, IUnknown* ipUnk, bool bVisibleOnly/* = false*/, bool bEditableOnly/* = false*/ )
{
	std::list<CAdapt<ILayerPtr> > listLayers; // �Ԃ�l

	IFeaturePtr ipFeature( ipUnk );
	ITablePtr ipTable( ipUnk );

	if( ! ipMap || ( ipFeature == NULL && ipTable == NULL ) )
		return listLayers;

	if( ipFeature != NULL )
		ipFeature->get_Table( &ipTable );

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( LayerTypeFeatureLayer, bEditableOnly, bVisibleOnly ); it != cLayers.end(); ++it )
	{
		IFeatureClassPtr ipFeatClass;
		if( SUCCEEDED( ((IFeatureLayerPtr)*it)->get_FeatureClass( &ipFeatClass ) ) )
		{
			if( (IUnknown*)(IUnknownPtr)ipTable == (IUnknown*)(IUnknownPtr)ipFeatClass )
				listLayers.push_back( *it );
		}
	}
	return listLayers;
}

// ���C�����\������Ă��邩�ǂ������`�F�b�N����
BOOL AheIsVisibleLayer( IMapLayers* ipMap, ILayer* ipLayer )
{
	if( ! ( ipMap && ipLayer ) )
		return FALSE;

	VARIANT_BOOL vbVisible = VARIANT_FALSE;
	ipMap->IsLayerVisible( ipLayer, &vbVisible );

	return vbVisible ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////
//
//
// 
//////////////////////////////////////////////////////////////////////
BOOL AheGetDatasetForTableName( IMapPtr piMap, IUnknownPtr ipUnk, IDataset** ipDataset )
{
	IFeatureClassPtr ipFeatureClass = NULL;

	if( AheGetFeatureClass( piMap, ipUnk, &ipFeatureClass ) == FALSE ) return FALSE;

	IDatasetPtr ipRetDataset = ipFeatureClass;
	if( ipRetDataset != NULL ) {
		(*ipDataset) = ipRetDataset;
		(*ipDataset)->AddRef();

		return TRUE;
	}

	return FALSE;
}

BOOL AheCompareFeatureName( IFeatureClassPtr piFeatureClass, LPCTSTR lpcszLayerName )
{
	if( piFeatureClass == NULL || lpcszLayerName == NULL ) return FALSE;

	BOOL blResult = FALSE;
	IDatasetPtr piDataset;
	if( SUCCEEDED( piFeatureClass->QueryInterface( &piDataset)))
	{
		BSTR bstrLayerName = NULL;
		piDataset->get_Name( &bstrLayerName);
		if( NULL != bstrLayerName)
		{
			USES_CONVERSION;
			LPCTSTR lpcsz = OLE2T( bstrLayerName);
			// �e�[�u������ ****.**** �̏ꍇ�A�ŏ��� ****. �͍폜
			for( int nIndex = 0; nIndex < lstrlen( lpcsz); nIndex++)
			{
				if( TEXT('.') == lpcsz[ nIndex])
				{
					lpcsz = &( lpcsz[ nIndex+1]);
					break;
				}
			}
//			ATLTRACE(_T("CArcHelperBase::IsSameLayerName : ��r���F%s ��r��F%s\n"), lpcszLayerName , lpcsz );
			blResult = ( 0 == lstrcmpi( lpcsz, lpcszLayerName)) ? TRUE : FALSE;

			::SysFreeString( bstrLayerName);
		}
	}

	return blResult;
}

BOOL AheCompareLayerName( ILayerPtr piLayer, LPCTSTR lpcszLayerName ) // TODO: SketchTool�̂�
{
	BOOL blResult = FALSE;

	IFeatureLayerPtr piFeatureLayer;
	if( SUCCEEDED( piLayer->QueryInterface( &piFeatureLayer)))
	{
		IFeatureClassPtr piFeatureClass;
		if( SUCCEEDED( piFeatureLayer->get_FeatureClass( &piFeatureClass)))
		{
			blResult = AheCompareFeatureName( piFeatureClass, lpcszLayerName);
		}
	}

	return blResult;
}
BOOL AheGetFieldByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IField** ipRetField )
{
	ITablePtr ipTable = NULL;

	// �e�[�u���擾
	if( AheGetTableByTableName( piMap, lpszTableName, &ipTable ) == FALSE ) return FALSE;
	return AheGetFieldByFieldName( ipTable, lpszFieldName, ipRetField );
}

BOOL AheGetFieldByFieldName( ITablePtr ipTable, LPCTSTR lpszFieldName, IField** ipRetField )
{
	IFieldsPtr ipFields = NULL;
	IFieldPtr ipField = NULL;
	LONG lFieldCount = 0;
	CComBSTR bstrFieldName;

	if( ipTable == NULL ) return FALSE;

	USES_CONVERSION;

	ipTable->get_Fields( &ipFields );

	ipFields->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; i++ ) {
		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrFieldName );

		if( lstrcmpi( OLE2T( bstrFieldName ), lpszFieldName ) == 0 ) {
			/// �������O�����t�B�[���h����������
			(*ipRetField) = ipField;
			(*ipRetField)->AddRef();

			return TRUE;
		} else {
			ipField = NULL;
		}
	}
	
	return FALSE;
}

BOOL AheRowIsChanged( _IRowPtr ipRow )
{
	IRowChangesPtr ipRowChanges( ipRow );
	if( ipRowChanges == NULL ) return FALSE;

	IFieldsPtr ipFields;
	LONG lFieldCount = -1;

	ipRow->get_Fields( &ipFields );
	ipFields->get_FieldCount( &lFieldCount );
	
	for( LONG i = 0; i < lFieldCount; i++ )
	{
		VARIANT_BOOL vaIsChanged = VARIANT_FALSE;
		ipRowChanges->get_ValueChanged( i, &vaIsChanged );
		if( vaIsChanged == VARIANT_TRUE )
			return TRUE;
	}

	return FALSE;
}

BOOL AheShapeIsChanged( IFeaturePtr ipFeature )
{
	IFeatureChangesPtr ipChange( ipFeature );
	if( ! ipChange )
		return FALSE;

	VARIANT_BOOL vbChanged = VARIANT_FALSE;
	ipChange->get_ShapeChanged( &vbChanged );
	return VB2BOOL( vbChanged );
}

BOOL AheIsSameFeature( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2 )
{
	LONG OID1 = 0, OID2 = 0;
	VARIANT_BOOL vbHasOID1 = VARIANT_FALSE, vbHasOID2 = VARIANT_FALSE;

	if( ipFeature1 == NULL || ipFeature2 == NULL ) return FALSE;

	ipFeature1->get_HasOID( &vbHasOID1 );
	ipFeature2->get_HasOID( &vbHasOID2 );

	if( ( vbHasOID1 == VARIANT_FALSE ) || 
		( vbHasOID2 == VARIANT_FALSE ) ) {

		return FALSE;
	}

	ipFeature1->get_OID( &OID1 );
	ipFeature2->get_OID( &OID2 );

	if( OID1 == OID2 ) return TRUE;

	return FALSE;
}

BOOL AheIsSameField( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2, BSTR bstrFieldName )
{
	IFieldsPtr ipFields1 = NULL, ipFields2 = NULL;
	LONG lIndex1 = -1, lIndex2 = -1;
	VARIANT va1, va2;

	ipFeature1->get_Fields( &ipFields1 );
	ipFeature2->get_Fields( &ipFields2 );

	ipFields1->FindField( bstrFieldName, &lIndex1 );
	ipFields2->FindField( bstrFieldName, &lIndex2 );

	if( lIndex1 < 0 || lIndex2 < 0 ) return FALSE;

	ipFeature1->get_Value( lIndex1, &va1 );
	ipFeature2->get_Value( lIndex2, &va2 );

	CComVariant pVa1( va1 );

	return ( pVa1 == va2 );
}

BOOL AheIsSameAttributes( _IRowPtr ipRow1, _IRowPtr ipRow2 )
{
	using namespace sindy::schema;

	// �قȂ�t�B�[�`���N���X��������false
	if( AheGetFeatureClassName( ipRow1 ).Compare( AheGetFeatureClassName( ipRow2 ) ) != 0 )
		return FALSE;

	IFieldsPtr ipFields1, ipFields2;
	LONG lFieldCount = -1;

	ipRow1->get_Fields( &ipFields1 );
	ipRow2->get_Fields( &ipFields2 );

	// �����e�[�u���ɏ������邩
	bool bIsSameTable = ( (ITable*)AheGetTable(ipRow1) == (ITable*)AheGetTable(ipRow2) );

	ipFields1->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; i++ )
	{
		// SiNDY-e �I�ɕҏW�\�ȃt�B�[���h�̂ݔ�r
		IFieldPtr ipField;
		ipFields1->get_Field( i, &ipField );
		if( !AheIsEditableField( ipField ) )
			continue;
		
		CComBSTR bstrFieldName1;
		ipField->get_Name( &bstrFieldName1 );

		// ����ɂ����Ȃ��t�B�[���h�͏��O�ibug 9780�j
		if( !bIsSameTable )
		{
			CComBSTR bstrFieldName2;
			long lIndex = -1;
			ipFields2->FindField( bstrFieldName2, &lIndex );
			if( lIndex < 0 )
				continue;
		}
		
		// SiNDY-c�ŗL�̃t�B�[���h�͏��O�ibug 9779�j
		CString strFieldName((BSTR)bstrFieldName1);
		if( AheIsSindycField(strFieldName) )
			continue;
		
		// �ȉ��̃t�B�[���h�͏��O
		if( strFieldName == road_link::kSource ||
			strFieldName == road_link::kAdminCode ||
			strFieldName == inf_uturn::kSourceName )
			continue;

		CComVariant vaValue1, vaValue2;

		ipRow1->get_Value( i, &vaValue1 );
		ipRow2->get_Value( i, &vaValue2 );

		if( vaValue1 != vaValue2 ) 
		{
#ifdef DEBUG
			CComBSTR bstrFieldNameTmp;
			ipField->get_AliasName( &bstrFieldNameTmp );
			ATLTRACE(_T("AheIsSameAttributes() : %s �t�B�[���h�̒l���قȂ�܂�\n"), CString(bstrFieldNameTmp) );
#endif // DEBUG
			return FALSE;
		}
	}

	return TRUE;
}

BOOL AheIsEditableField( IFieldPtr ipField, bool bForCopy/* = FALSE*/ )
{
	BOOL bRet = FALSE;

	if( ipField != NULL ) 
	{
		VARIANT_BOOL vaIsEditable = VARIANT_FALSE;

		ipField->get_Editable( &vaIsEditable );
		if( vaIsEditable == VARIANT_TRUE ) 
		{
			CComBSTR bstrFieldName;

			ipField->get_Name( &bstrFieldName );
			CString strFieldName( bstrFieldName );
			INT iPos = strFieldName.ReverseFind('.');
			if( iPos > 0 ) strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );
			strFieldName.MakeUpper();	// �S�đ啶���ɒu��
		
			if(      lstrcmp( strFieldName, _T("FID")            ) == 0 ) bRet = FALSE; // FID
			else if( lstrcmp( strFieldName, _T("SHAPE")          ) == 0 ) bRet = FALSE; // Shape
			else if( lstrcmp( strFieldName, _T("AREA")           ) == 0 ) bRet = FALSE; // AREA
			else if( lstrcmp( strFieldName, _T("SHAPE_LENGTH")   ) == 0 ) bRet = FALSE; // Shape_length
			else if( lstrcmp( strFieldName, _T("OBJECTID")       ) == 0 ) bRet = FALSE; // OBJECTID
			else if( lstrcmp( strFieldName, _T("ENABLED")        ) == 0 ) bRet = FALSE; // Enabled
			else if( lstrcmp( strFieldName, _T("OPERATOR")       ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("PURPOSE_C")      ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("MODIFYDATE")     ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("UPDATETYPE_C")   ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("PROGMODIFYDATE") ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("MODIFYPROGNAME") ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("USERCLAIM_F")    ) == 0 ) bRet = FALSE;
			else if( ( ! bForCopy ) && lstrcmp( strFieldName, _T("FIELDSURVEY_F")  ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("FROM_NODE_ID")   ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("TO_NODE_ID")     ) == 0 ) bRet = FALSE;
			else if( strFieldName.Right( 3 ).CompareNoCase( _T("_RF")   ) == 0 ) bRet = FALSE;
			else if( strFieldName.Right( 5 ).CompareNoCase( _T("_LQRF") ) == 0 ) bRet = FALSE;
//			else if( bForCopy && 0 == lstrcmp( strFieldName, _T("
/*			else if( bExept_ID == FALSE && strFieldName.Right( 3 ).CompareNoCase( _T("_ID")   ) == 0 ) 
			{
				if( bIsCopyPaste == TRUE )
				{
					if( lstrcmp( strFieldName, _T("VICS_LINK_ID") ) == 0 )
						bRet = TRUE;
				}
				else
					bRet = FALSE;
			}*/
			else if( strFieldName.Left(  3 ).CompareNoCase( _T("TMP")   ) == 0 ) bRet = FALSE;
			else if( lstrcmp( strFieldName, _T("NODECLASS_C") ) == 0 ) bRet = FALSE;
/*			{
				if( bIsCopyPaste == TRUE )
					bRet = FALSE;
			}*/
			else                                                          bRet = TRUE;

		} 
	}

	return bRet;
}

sindyeFeatureKinds AheGetFeatureKinds( IFeaturePtr ipFeature )
{
/*	HRESULT hr;
	IObjectClassPtr ipClass = NULL;
	IGeometryPtr ipGeom = NULL;
	IPolylinePtr ipPolyline = NULL;
	IPolygonPtr ipPolygon = NULL;
	IPointPtr ipPoint = NULL;
	BSTR bstrName;

	USES_CONVERSION;

	ipFeature->get_Class( &ipClass );
	ipClass->get_AliasName( &bstrName );

	// ���C���̃G�C���A�X�����ׂē��肷��
	// ���C�����Ɋ��҂���G�C���A�X���������Ă���K�v������܂�
	if( lstrcmp( OLE2CT( bstrName ), ALIAS_NODE_NAME ) == 0 ) return sindyeNodeFeature;
	if( lstrcmp( OLE2CT( bstrName ), ALIAS_ROAD_NAME ) == 0 ) return sindyeRoadFeature;
*/
	CString strClassName = AheGetFeatureClassName( ipFeature );
	if( AheIsNetworkNode( ipFeature ) ) return sindyeNodeFeature;
	if( AheIsNetworkLink( ipFeature ) ) return sindyeRoadFeature;

	// �|�����C���܂��̓|���S���ł��邩
	IPolylinePtr ipPolyline;
	IPolygonPtr ipPolygon;
	IPointPtr ipPoint;
	IGeometryPtr ipGeom;
	HRESULT hr;

	ipFeature->get_Shape( &ipGeom );
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPolyline ) ) ) return sindyeLineFeature;
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPolygon  ) ) ) return sindyePolygonFeature;
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPoint    ) ) ) return sindyePointFeature;

	// �߂�ǂ������̂łƂ肠����
	return sindyeUnknownFeature;
}
#if 0
sindyeFeatureKinds AheGetFeatureKinds(IMapPtr piMap, IFeaturePtr ipFeature )
{
/*	HRESULT hr;
	IObjectClassPtr ipClass = NULL;
	IGeometryPtr ipGeom = NULL;
	IPolylinePtr ipPolyline = NULL;
	IPolygonPtr ipPolygon = NULL;
	IPointPtr ipPoint = NULL;
	CComBSTR cComBSTR;
	CString cStrFindTemp;

	USES_CONVERSION;

	// �e�[�u�����̎擾
	AheGetLayerName( piMap, ipFeature, &cComBSTR, _FIND_TABLE );
	cStrFindTemp = cComBSTR;
	int nFind = cStrFindTemp.Find( TEXT( '.'));
	if ( 0 <= nFind ) {
		cStrFindTemp = cStrFindTemp.Mid( nFind + 1);
	}
	cStrFindTemp.MakeUpper();
	// �w�蕶����Ƃ̏ƍ�
	if( 0 <= cStrFindTemp.Find( NODE_TABLE_NAME ) )  return sindyeNodeFeature;
	if( 0 <= cStrFindTemp.Find( ROAD_TABLE_NAME ) )  return sindyeRoadFeature;

	// �|�����C���܂��̓|���S���ł��邩
	ipFeature->get_Shape( &ipGeom );
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPolyline ) ) ) return sindyeLineFeature;
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPolygon  ) ) ) return sindyePolygonFeature;
	if( SUCCEEDED( hr = ipGeom->QueryInterface( &ipPoint    ) ) ) return sindyePointFeature;

	// �߂�ǂ������̂łƂ肠����
	return sindyeUnknownFeature;
*/
	ATLTRACE(_T("AheGetFeatureKinds() : AheGetFeatureKinds( IFeaturePtr ipFeature ) ���g�p���Ă�������\n"));
	return AheGetFeatureKinds( ipFeature );
}
#endif // if 0
DOUBLE AheGetXYDomain1UnitLength( ISpatialReferencePtr ipSpRef )
{
	if( ipSpRef == NULL ) return -1.0;

	DOUBLE dX = 0.0f, dXMin = 0.0f, dXMax = 0.0f, dYMin = 0.0f, dYMax = 0.0f;

	ipSpRef->GetDomain( &dXMin, &dXMax, &dYMin, &dYMax );
	dX = dXMax - dXMin;

	if( dX == 0.0 )
	{
		ATLASSERT( dX != 0.0 );
		return 0.0;
	}

	return (DOUBLE)( (DOUBLE)dX / pow( 2.0, AheIsHighPrecision(ipSpRef) ? 52.0 : 31.0 ) );
}

ISpatialReferencePtr AheGetMinimumSpatialReference( ISpatialReferencePtr ipSpRef1, ISpatialReferencePtr ipSpRef2 )
{
	if( ipSpRef1 == NULL && ipSpRef2 == NULL ) return NULL;
	if( ipSpRef1 == NULL && ipSpRef2 != NULL ) return ipSpRef2;
	else                                       return ipSpRef1;

	DOUBLE dXMax1 = 0.0f, dXMin1 = 0.0f, dYMax1 = 0.0f, dYMin1 = 0.0f, dX1 = 0.0f;
	DOUBLE dXMax2 = 0.0f, dXMin2 = 0.0f, dYMax2 = 0.0f, dYMin2 = 0.0f, dX2 = 0.0f;

	ipSpRef1->GetDomain( &dXMin1, &dXMax1, &dYMin1, &dYMax1 );
	dX1 = dXMax1 - dXMin1;
	ipSpRef2->GetDomain( &dXMin2, &dXMax2, &dYMin2, &dYMax2 );
	dX2 = dXMax2 - dXMin2;

	if( dX1 < dX2 )
		return ipSpRef1;
	else
		return ipSpRef2;
}

/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[���h�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
// IFields���擾����
IFieldsPtr AheGetFields( IUnknown* ipUnk )
{
	if( ! ipUnk )
		return IFieldsPtr();

	IFieldsPtr ipFields( ipUnk );
	if( ipFields )
		return ipFields;

	_IRowPtr ipRow( ipUnk );
	if( ipRow )
	{
		ipRow->get_Fields( &ipFields );
		return ipFields;
	}

	ITablePtr ipTable( ipUnk );
	if( ipTable )
	{
		ipTable->get_Fields( &ipFields );
		return ipFields;
	}

	_ICursorPtr ipCursor( ipUnk );
	if( ipCursor )
	{
		ipCursor->get_Fields( &ipFields );
		return ipFields;
	}

	return ipFields;
}

// �t�B�[���h���Ńt�B�[���h�C���f�b�N�X���擾����
long AheFindField( IFields* ipFields, LPCTSTR lpcszFieldName )
{
	if( ! ( ipFields && lpcszFieldName ) )
	{
		_ASSERTE( ipFields && lpcszFieldName );
		return -1;
	}

	// FindField���Ƒ啶�����������l������K�v���o�Ă��邽�߁A
	// ���ʂ������[�v���܂킷���Ƃŉ��
	long lFieldCount = -1;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; ++i )
	{
		IFieldPtr ipField;
		CComBSTR bstrName;

		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrName );

		// �e�[�u�������ɂ��Ή�����
		CString strFieldName( bstrName );
		INT iPos = strFieldName.ReverseFind('.');
		if( iPos > 0 ) 
			strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );

		if( 0 == lstrcmpi( lpcszFieldName, strFieldName ) )
			return i;
	}
	return -1;
}

bool AheHasZ(IFieldPtr ipField)
{
	IGeometryDefPtr ipGeometryDef;
	ipField->get_GeometryDef(&ipGeometryDef);

	if(!ipGeometryDef)
		return false;

	VARIANT_BOOL hasZ = VARIANT_FALSE;
	ipGeometryDef->get_HasZ(&hasZ);

	return hasZ;
}

bool AheHasZ(IFeatureClassPtr ipFeatureClass)
{
	// Shape�t�B�[���h�̃C���f�b�N�X���擾����
	CComBSTR vaShapeField;
	ipFeatureClass->get_ShapeFieldName(&vaShapeField);
	long fieldIndex = -1;
	ipFeatureClass->FindField(vaShapeField, &fieldIndex);
	if(fieldIndex < 0)
		return false;

	// Shape�t�B�[���h���擾����
	IFieldsPtr ipFields;
	ipFeatureClass->get_Fields(&ipFields);
	IFieldPtr ipField;
	ipFields->get_Field(fieldIndex, &ipField);

	if(!ipField)
		return false;

	return AheHasZ(ipField);
}

bool AheGeoHasZ(IGeometryPtr ipGeometry)
{
	IZAwarePtr ipZAware(ipGeometry);
	if(! ipZAware)
		return false;

	VARIANT_BOOL hasZ = VARIANT_FALSE;
	ipZAware->get_ZAware(&hasZ);

	return hasZ == VARIANT_TRUE;
}

bool AheSetZeroToZ(IGeometryPtr ipGeometry)
{
	VARIANT_BOOL zaware = VARIANT_FALSE;
	IZAwarePtr(ipGeometry)->get_ZAware(&zaware);

	// Z�l������Ȃ牽�����Ȃ�
	if(zaware)
		return false;

	// TODO:MultiPoint�Ή����K�v�BIZ��Polyline��Polygon�ł����C���X�^���X���Ȃ��BPoint�����t�H���[���Ă��Ў藎��
	IZAwarePtr(ipGeometry)->put_ZAware(VARIANT_TRUE);
	IZPtr ipZ(ipGeometry);
	if(ipZ){
		ipZ->SetConstantZ(0);
		return true;
	}
	IPointPtr ipPoint(ipGeometry);
	if(ipPoint){
		ipPoint->put_Z(0);
		return true;
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[�`���̑����Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
BOOL AheSetAttribute( IRowBufferPtr ipRow, long lIndex, const VARIANT& vaValue )
{
	if( ! ( ipRow && 0 <= lIndex && vaValue.vt != VT_ERROR && vaValue.vt != VT_EMPTY ) )
	{
		_ASSERTE( ipRow && 0 <= lIndex && vaValue.vt != VT_ERROR && vaValue.vt != VT_EMPTY );
		return FALSE;
	}

	// ��x�l���Ƃ��Ă݂āA EMPTY �ł͂Ȃ����Ƃ��m�F����i�����ōi���Ă��邩������Ȃ��̂Łj
	CComVariant vaTmp;
	ipRow->get_Value( lIndex, &vaTmp );
	if( vaTmp.vt == VT_EMPTY || vaTmp.vt == VT_ERROR )
		_ASSERTE( vaTmp.vt != VT_EMPTY && vaTmp.vt != VT_ERROR );	// �ꉞ ASSERT �̂݁B�����ɗ��Ă͂����Ȃ�

	return ( SUCCEEDED(ipRow->put_Value( lIndex, vaValue ) ) ) ? TRUE : FALSE;
}

BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, const VARIANT& vaValue )
{
	// �����`�F�b�N
	if( ! lpcszFieldName )
	{
		_ASSERTE( lpcszFieldName );
		return FALSE;
	}

	return AheSetAttribute( ipRow, AheFindField( AheGetFields( ipRow ), lpcszFieldName ), vaValue );
}

CComVariant AheGetAttribute( IRowBufferPtr ipRow, long lIndex )
{
	CComVariant vaValue;

	// �����`�F�b�N
	if( ! ( ipRow && 0 <= lIndex ) )
	{
		//_ASSERTE( ipRow && 0 <= lIndex ); ������
		return vaValue;
	}

	if( FAILED( ipRow->get_Value( lIndex, &vaValue ) ) )
		_ASSERTE( SUCCEEDED( ipRow->get_Value( lIndex, &vaValue ) ) );

	return vaValue;
}

CComVariant AheGetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName )
{
	// �����`�F�b�N
	if( ! lpcszFieldName )
	{
		_ASSERTE( lpcszFieldName );
		return CComVariant();
	}

	return AheGetAttribute( ipRow, AheFindField( AheGetFields( ipRow ), lpcszFieldName ) );
}

BOOL AheCopyAttribute( IRowBufferPtr ipSourceRow, IRowBufferPtr ipDestRow, BOOL bIsSplitMode/* = FALSE*/ )
{
	// �����`�F�b�N
	if( !(ipSourceRow && ipDestRow) ) return FALSE;

	BOOL bRet = TRUE;

	IFieldsPtr ipSourceFields, ipDestFields;
	LONG lFieldCount = 0;

	// �����R�s�[
	ipSourceRow->get_Fields( &ipSourceFields );
	ipDestRow->get_Fields( &ipDestFields );

	ipSourceFields->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; ++i )
	{
		VARIANT_BOOL vaEditable = VARIANT_FALSE;
		esriFieldType type = (esriFieldType)-1;
		CComBSTR bstrSourceFieldName;
		IFieldPtr ipSourceField;

		ipSourceFields->get_Field( i, &ipSourceField );
		ipSourceField->get_Type( &type );
		ipSourceField->get_Editable( &vaEditable );
		ipSourceField->get_Name( &bstrSourceFieldName );

		CString strSourceFieldName( bstrSourceFieldName );
		if( type != esriFieldTypeOID && 
			type != esriFieldTypeGeometry && 
			vaEditable == VARIANT_TRUE && 
			0 != strSourceFieldName.Left(4).CompareNoCase(_T("ORG_")) && // [bug 5224] ORG_*�̓R�s�[�ΏۂƂ��Ȃ�
			0 != strSourceFieldName.Right(4).CompareNoCase(_T("_GID")) && // [bug 5224] *_GID�̓R�s�[�ΏۂƂ��Ȃ�
			0 != strSourceFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) && // [bug 5224] *_GID�̓R�s�[�ΏۂƂ��Ȃ�
			!(bIsSplitMode && 0 == strSourceFieldName.CompareNoCase( building::kGeospaceID )) && // [bug 10063] �������[�h�̏ꍇ��GEOSPACE_ID�̓R�s�[���Ȃ�
			( AheIsEditableField( ipSourceField, TRUE ) || 
			bIsSplitMode && 0 == strSourceFieldName.CompareNoCase( ipc_table::kUserClaim ) ) )	// [bug 4043] �������[�h�̏ꍇ��USERCLAIM_F���R�s�[
		{
			CComVariant vaSourceValue(_T(""));
			LONG lDestFieldIndex = -1;

			ipSourceRow->get_Value( i, &vaSourceValue );
			ipDestFields->FindField( bstrSourceFieldName, &lDestFieldIndex );

			if( lDestFieldIndex < 0 )
			{
				ATLASSERT( FALSE );	// �Ȃ��킯���Ȃ�
				bRet = FALSE;
			} else {
				if( ipDestRow->put_Value( lDestFieldIndex, vaSourceValue ) != S_OK )
				{
					ATLASSERT( FALSE );
					bRet = FALSE;
				}
			}
		}
	}

	return bRet;
}
#ifndef __BUILD_SINDYRULE__
BOOL AheSetModifyData( IRowBufferPtr piRow, sindyUpdateType emUpdateType, LPSYSTEMTIME lpSystemTime)
{
	// �����`�F�b�N
	if( ! piRow )
	{
		_ASSERTE( piRow ); // ����ȁI�I
		return FALSE;
	}

	long lIndex = -1;
	IFieldsPtr ipFields( AheGetFields( piRow ) );

	// �C���������X�V
	if( 0 <= ( lIndex = AheFindField( ipFields, MODIFYDATE_FIELD ) ) )
	{
		CComVariant cComModifyDate = AheGetNow( lpSystemTime );
		if( ! AheSetAttribute( piRow, lIndex, cComModifyDate ) ) 
			return FALSE;
	}

	ISiNDYRulePtr ipRule( CLSID_SiNDYRuleObj );
	if( ipRule )
	{
		// �C���ҏ��X�V
		if( 0 <= ( lIndex = AheFindField( ipFields, OPERATOR_FIELD ) ) )
		{
			CComBSTR bstrOperator;
			ipRule->get_OperatorName( &bstrOperator );
			CComVariant vaOperator( bstrOperator );
			if( ! AheSetAttribute( piRow, lIndex, vaOperator ) )
				return FALSE;
		}

		// ��ƖړI���X�V
		if( 0 <= ( lIndex = AheFindField( ipFields, PURPOSE_C_FIELD ) ) )
		{
			CComBSTR bstrPurpose;
			LONG lID = -1;
			ipRule->GetOperatingPurpose( &lID, &bstrPurpose );
			CComVariant vaPurpose( lID );
			if( ! AheSetAttribute( piRow, lIndex, vaPurpose ) )
				return FALSE;
		}

		// ���������t�^
		CComBSTR bstrAutoAttrGiving;
		ISiNDYRule2Ptr ipRule2( ipRule );
		if( NULL != ipRule2 )
			ipRule2->GetAutoAttrGiving( &bstrAutoAttrGiving );

		std::list<CString> listAutoAttrGiving;
		AheSepStr2listString( (CString)bstrAutoAttrGiving, _T("|"), listAutoAttrGiving );
		for( std::list<CString>::iterator it = listAutoAttrGiving.begin(); it != listAutoAttrGiving.end(); it++ )
		{
			CString strAutoAttrGiving = *it;
			int iEqualPos = strAutoAttrGiving.Find( _T("=") );
			CString strFieldName = strAutoAttrGiving.Left( iEqualPos );
			CString strRight = strAutoAttrGiving.Right( strAutoAttrGiving.GetLength() - iEqualPos - 1 );
			CComVariant vaValue;
			CString strStr;
			bool bIsOK = false;
			if( strRight.Find( _T("\'") ) == 0 )
			{
				strStr = strRight.Right( strRight.GetLength() - 1 );
				if( strStr.Find( _T("\'") ) == strStr.GetLength() - 1 )
				{
					strStr = strStr.Left( strStr.GetLength() - 1 );
					vaValue = strStr;
					bIsOK = true;
				}
			}
			else if( strRight == _T("NULL") )
			{
				vaValue.vt = VT_NULL;
				vaValue = NULL;
				bIsOK = true;
			}
			else
			{
				long lValue = _ttol( strRight );
				CString strCheck;
				strCheck.Format( _T("%d"), lValue );
				if( strCheck == strRight )
				{
					vaValue = lValue;
					bIsOK = true;
				}
			}
			if( bIsOK )
			{
				if( ! AheSetAttribute( piRow, strFieldName, vaValue ) )
				{
					ATLTRACE(_T("���������t�^���� %s �̐ݒ�Ɏ��s���܂���\n"), strFieldName );
				}
			}
		}
	}

	// �X�V�^�C�v���X�V
	if( 0 <= ( lIndex = AheFindField( ipFields, UPDATETYPE_CFIELD ) ) )
	{
		if( ! AheSetAttribute( piRow, lIndex, CComVariant((LONG)emUpdateType) ) )
			return FALSE;
	}

	// SiNDY-c�p�X�V�^�C�v���X�V�ibug 5201�j
	// ���[�h�Ɋւ�炸����΍X�V�ibug 5777�j
	if( 0 <= ( lIndex = AheFindField( ipFields, UPDATE_CFIELD ) ) )
	{
		long lUpdateCode = 0;
		switch( emUpdateType )
		{
			case sindyUpdateTypeNoUpdate:	lUpdateCode = 0; break; // �C���Ȃ�/������
			case sindyUpdateTypeCreate:		lUpdateCode = 3; break; // �V�K����/�`��A�������C��
			case sindyUpdateTypeShape:		lUpdateCode = 1; break; // �`����C��
			case sindyUpdateTypeProperty:	lUpdateCode = 2; break; // �������C��
			case sindyUpdateTypeDelete:
			default:						lUpdateCode = -1; break; // �Ή�������̂��Ȃ����A�܂��������c
		}
		// ���Ƃ��Ƃ̑����Ƃ�OR����������̂ōX�V�i�~�ό^�j
		CComVariant vaOrgUpdateCode = AheGetAttribute( piRow, lIndex );
		if( VT_EMPTY != vaOrgUpdateCode.vt )
		{
			if( VT_I4 == vaOrgUpdateCode.vt )
				lUpdateCode |= vaOrgUpdateCode.lVal;
			AheSetAttribute( piRow, lIndex, CComVariant(lUpdateCode) );
		}
	}

	return TRUE;
}
#endif //__BUILD_SINDYRULE__

BOOL AheSetDefaultData( IRowBufferPtr ipRow )
{
	IFieldsPtr ipFields;
	LONG lFieldCount = -1;

	if( ipRow == NULL ) 
	{
		_ASSERTE( ipRow != NULL );	// �����ɂ���ȁI�I
		return FALSE;
	}

	ipRow->get_Fields( &ipFields );
	ipFields->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		VARIANT_BOOL vaIsEditable = VARIANT_FALSE;

		ipFields->get_Field( i, &ipField );
		ipField->get_Editable( &vaIsEditable );
		// �ҏW�\�t�B�[���h�̂�
		if( vaIsEditable == VARIANT_TRUE )
		{
			CComVariant vaDefaultValue;
			ipField->get_DefaultValue( &vaDefaultValue );
			ipRow->put_Value( i, vaDefaultValue );
		}
	}

	return TRUE;
}

_IRowPtr AheCreateRow( IUnknownPtr ipUnk, IGeometryPtr ipGeom/* = NULL */, BOOL bSetDefaultData/* = TRUE */ )
{
	_IRowPtr ipRow( ipUnk );
	ILayerPtr ipLayer( ipUnk );
	IDatasetPtr ipDataset( ipUnk );

	// IDataset �����o��
	if( ipRow != NULL )
	{
		ITablePtr ipTable;
		ipRow->get_Table( &ipTable );
		ipDataset = ipTable;
	}
	else if( ipLayer != NULL )
	{
		IFeatureLayerPtr ipFeatureLayer( ipLayer );
		if( ipFeatureLayer != NULL )
		{
			IFeatureClassPtr ipFeatureClass;
			ipFeatureLayer->get_FeatureClass( &ipFeatureClass );
			ipDataset = ipFeatureClass;
		}
	}

	if( ipDataset == NULL ) return NULL;

	IFeatureClassPtr ipFeatureClass( ipDataset );
	if( ipFeatureClass != NULL )
	{
		IFeaturePtr ipFeature;
		if( FAILED( ipFeatureClass->CreateFeature( &ipFeature ) ) )
		{
			_ASSERTE( ipFeature != NULL );
			AheTraceErrorMessage( _T("AheCreateRow") );
			return NULL;
		}
		if( bSetDefaultData == TRUE ) AheSetDefaultData( ipFeature );

/*		IGeoDatasetPtr ipGeoDataset( ipFeatureClass );
		if( ipGeoDataset != NULL )
		{
			// �n���Q�Ƃ����킹�Ȃ��Ƃ����iArcGIS 8.2 ����H�j
			ISpatialReferencePtr ipSpRef;
			ipGeoDataset->get_SpatialReference( &ipSpRef );

			if( ipGeom != NULL )
			{
				IClonePtr ipClone;
				((IClonePtr)ipGeom)->Clone( &ipClone );
				IGeometryPtr ipGeomCopy( ipClone );
				if( ipGeomCopy != NULL )
				{
					ipGeomCopy->putref_SpatialReference( ipSpRef );
					ipGeomCopy->SnapToSpatialReference();
					if( FAILED(ipFeature->putref_Shape( ipGeomCopy ) ) ) 
					{
						AheTraceErrorMessage( _T("AheCreateRow") );
						return NULL;
					}
				} else
					return NULL;
			}
		}
		else 
			return NULL;
*/
		if( ipGeom != NULL )
		{
			if(AheHasZ(ipFeatureClass))
				AheSetZeroToZ(ipGeom);

			if( AheSetShape( ipFeature, ipGeom ) == FALSE )
			{
				AheMessageBox( ERROR_FAIL_SET_SHAPE_TO_FEATURE, _T("AheSetShape()"), MB_OK|MB_ICONSTOP );
				return NULL;
			}
		}

		return ipFeature;
	}
	else {
		ITablePtr ipTable( ipDataset );
		if( ipTable == NULL ) return NULL;

		if( FAILED( ipTable->CreateRow( &ipRow ) ) )
		{
			_ASSERTE( FALSE );
			AheTraceErrorMessage( _T("AheCreateRow()") );
		}
		else {
			if( bSetDefaultData == TRUE ) AheSetDefaultData( ipRow );
		}

		return ipRow;
	}
}

IRowBufferPtr AheCreateRowBuffer( IUnknownPtr ipUnk, IGeometryPtr ipGeom/* = NULL */, BOOL bSetDefaultData/* = TRUE */ )
{
	_IRowPtr ipRow( ipUnk );
	ILayerPtr ipLayer( ipUnk );
	IDatasetPtr ipDataset( ipUnk );

	// IDataset �����o��
	if( ipRow != NULL )
	{
		ITablePtr ipTable;
		ipRow->get_Table( &ipTable );
		ipDataset = ipTable;
	}
	else if( ipLayer != NULL )
	{
		IFeatureLayerPtr ipFeatureLayer( ipLayer );
		if( ipFeatureLayer != NULL )
		{
			IFeatureClassPtr ipFeatureClass;
			ipFeatureLayer->get_FeatureClass( &ipFeatureClass );
			ipDataset = ipFeatureClass;
		}
	}

	if( ipDataset == NULL )
	{
		_ASSERTE( ipDataset != NULL );	// �����ɗ��Ă͂����Ȃ�
		return NULL;
	}

	IFeatureClassPtr ipFeatureClass( ipDataset );
	if( ipFeatureClass != NULL )
	{
		IFeatureBufferPtr ipFeature;
		if( FAILED( ipFeatureClass->CreateFeatureBuffer( &ipFeature ) ) )
		{
			_ASSERTE( ipFeature != NULL );
			AheTraceErrorMessage( _T("AheCreateRowBuffer") );
			return NULL;
		}
		if( bSetDefaultData == TRUE ) AheSetDefaultData( ipFeature );

		if( ipGeom != NULL )
		{
			if( AheSetShape( ipFeature, ipGeom ) == FALSE )
			{
				AheMessageBox( ERROR_FAIL_SET_SHAPE_TO_FEATURE, _T("AheSetShape()"), MB_OK|MB_ICONSTOP );
				return NULL;
			}
		}

		return ipFeature;
	}
	else {
		IRowBufferPtr ipNewRow;
		ITablePtr ipTable( ipDataset );
		if( ipTable == NULL ) return NULL;

		ipTable->CreateRowBuffer( &ipNewRow );
		if( bSetDefaultData == TRUE ) AheSetDefaultData( ipNewRow );

		return ipNewRow;
	}
}
BOOL AheSetShape( IFeatureBufferPtr ipFeature, IGeometryPtr ipGeometry )
{
	if( ipFeature == NULL || ipGeometry == NULL ) return FALSE;

	//AheTraceGeometry( ipGeometry, __FILE__LINE__ );
	if( SUCCEEDED( ipFeature->putref_Shape( (IGeometryPtr)(AheGetClone(ipGeometry)) ) ) )
	{
		// �`�󂪕ς���Ă��Ȃ����ǂ����`�F�b�N
		esriGeometryType type;
		IGeometryPtr ipPuttedShape;

		ipGeometry->get_GeometryType( &type );
		ipFeature->get_Shape( &ipPuttedShape );
		switch( type )
		{
			case esriGeometryPoint:
			{
				IPointPtr ipPoint( ipGeometry ), ipPuttedPoint( ipPuttedShape );
				DOUBLE dx, dy;

				ipPoint->QueryCoords( &dx, &dy );
				ipPuttedPoint->PutCoords( dx, dy );

				if(AheGeoHasZ(ipPoint) && AheGeoHasZ(ipPuttedPoint)){
					double dZ(0.0);
					ipPoint->get_Z(&dZ);
					ipPuttedPoint->put_Z(dZ);
				}

				break;
			}
			case esriGeometryPolyline:
			case esriGeometryPolygon:
			{
				ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipGeometry ) );
				IPointCollectionPtr ipPointCol( ipGeometry );
				IPointCollectionPtr ipPuttedPointCol( ipPuttedShape );
				if( ipPointCol != NULL && ipPuttedPointCol != NULL )
				{
					IPointPtr ipPoint( CLSID_Point ), ipPuttedPoint( CLSID_Point );
					ipPoint->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
					ipPuttedPoint->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
					LONG lPointCount = -1, lPuttedPointCount = -1;

					ipPointCol->get_PointCount( &lPointCount );
					ipPuttedPointCol->get_PointCount( &lPuttedPointCount );
					if( lPointCount == lPuttedPointCount )
					{
						for( LONG i = 0; i < lPointCount; i++ )
						{
							DOUBLE dx, dy, dxx, dyy;

							// ������
							ipPoint->PutCoords( 0, 0 );
							ipPuttedPoint->PutCoords( 0, 0 );
							// �������̂��� QueryPoint ���g�p
							ipPointCol->QueryPoint( i, ipPoint );
							ipPuttedPointCol->QueryPoint( i, ipPuttedPoint );
							ipPoint->QueryCoords( &dx, &dy );
							ipPuttedPoint->QueryCoords( &dxx, &dyy );
							// IRelationalOperator::IsEquals() ���Ɣ����ȋ����͊ۂ߂���I�I
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
							if( ! AheIsEqual(ipPoint, ipPuttedPoint) )
#else
							if( dx != dxx || dy != dyy )
#endif
							{
								ATLTRACE(_T("WARNING : AheSetShape() �قȂ�_���C�����܂��� %d �_��\n"), i );
								ipPuttedPointCol->UpdatePoint( i, ipPoint );
							}
						}
					}
					else {
						_ASSERTE( lPointCount == lPuttedPointCount );
						ATLTRACE(_T("ERROR : AheSetShape() : IFeature::putref_Shape() ���Ƀ|�C���g�����ύX���ꂽ���߁A�`��C���ł��܂���B\n") );
						return FALSE;
					}
				}
				else {
					_ASSERTE( ipPointCol != NULL && ipPuttedPointCol != NULL );
					return FALSE;
				}
				break;
			}
			default:
				break;
		}
	}
	else {
		AheMessageBox( GetActiveWindow(), AheGetErrorMessage(), _T("AheSetShape()"), MB_OK|MB_ICONWARNING );
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
// �f�o�b�O�p�֐�
//
/////////////////////////////////////////////////////////////////////////////
void AheTraceRow( IRowBufferPtr ipRow, LPCTSTR lpcszMessage )
{
#ifdef DEBUG	
	if( ipRow == NULL ) return;

	IFieldsPtr ipFields;
	CString strName = AheGetFeatureClassName( ipRow );
	LONG lFieldCount = -1;
	ipRow->get_Fields( &ipFields );
	ipFields->get_FieldCount( &lFieldCount );
	ATLTRACE(_T("%s ----------------------------------\n"), lpcszMessage );
	ATLTRACE(_T("%s �e�[�u�����F%s\n"), lpcszMessage, strName );
	for( LONG i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName, bstrFieldAliasName;
		ipField->get_AliasName( &bstrFieldAliasName );
		ipField->get_Name( &bstrFieldName );
		CComVariant vaValue;
		ipRow->get_Value( i, &vaValue );
		if( CSTR_EQUAL == CompareStringW( LOCALE_INVARIANT, NORM_IGNORECASE, bstrFieldName, -1, L"SHAPE", -1 ) ) continue;

		CString str;
		if( vaValue.vt == VT_NULL )
			str = NULL_VALUE;
		else if( vaValue.vt == VT_EMPTY )
			str = EMPTY_VALUE;
		else {
			vaValue.ChangeType( VT_BSTR );
			str = vaValue.bstrVal;
		}
		ATLTRACE(_T("%s �t�B�[���h�F%s�i%s�j �l�F%s\n"), lpcszMessage, COLE2CT(bstrFieldAliasName), COLE2CT(bstrFieldName), str );
	}
	IFeaturePtr ipFeature( ipRow );
	if( ipFeature != NULL )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		AheTraceGeometry( ipGeom, lpcszMessage );
	}
#endif
}
/////////////////////////////////////////////////////////////////////////////
//
// ���W�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
DOUBLE AheConvertPixelsToMapUnits( IDisplayPtr piScreenDisplay, double dPixelUnits )
{
	DOUBLE dRealWorldDisplayExtent = 0.0, dSizeOfOnePixel = 0.0;
	INT    iPixelExtent = 0;
	RECT   pRect;
	IEnvelopePtr				ipEnv = NULL;
	IDisplayTransformationPtr	ipTrans = NULL;
	
	piScreenDisplay->get_DisplayTransformation( &ipTrans );

	ipTrans->get_DeviceFrame( &pRect );
	iPixelExtent = pRect.right - pRect.left;

	ipTrans->get_FittedBounds( &ipEnv );
	ipEnv->get_Width( &dRealWorldDisplayExtent );

	return ( ( dRealWorldDisplayExtent * dPixelUnits) / static_cast<double>( iPixelExtent));
	/*
	dSizeOfOnePixel = dRealWorldDisplayExtent / (DOUBLE)iPixelExtent;
	
	return dPixelUnits * dSizeOfOnePixel;*/
}

void AheQueryMapPoint( IDisplayPtr piScreenDisplay, const LONG X, const LONG Y, IPoint** ipPoint)
{
	IDisplayTransformationPtr ipTrans = NULL;

	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->ToMapPoint( X, Y, ipPoint );
//	(*ipPoint)->AddRef();
}

BOOL AheURISiNDYAdrs2LonLat(CString strURI, LonLat& lTmp)
{
	/// ������

	return FALSE;
}

BOOL AheURISiNDYAdrs2Adrs(CString strURI, AdrsScale& AdrsTmp)
{
	USES_CONVERSION;

	CString 	strTmp, strCompWord, strValue;
	INT			iCnt = 0;
	LPTSTR 		lpszTmp;

	// URI��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 22 );
	while( strURI.CompareNoCase( _T("") ) != 0 ) {
		INT 		iCount = 0, iValue = 0;
		CString		szTmp;

		// &�ʒu�擾
		iCount = strURI.Find( _T("&") );
		if( iCount < 0 )
			strTmp = strURI;
		else
			strTmp = strURI.Left( iCount );

		// =�ʒu�擾
		iValue = strTmp.Find( _T("=") );
		if( iValue < 0 )
			return FALSE;

		// ���͂��ꂽ������擾
		strCompWord = strTmp.Left( iValue );
		// ���͂��ꂽ�l�擾
		strValue = strTmp.Right( strTmp.GetLength() - iValue - 1 );

		if( strCompWord.CompareNoCase( _T("address") ) == 0 ) {
			if ( iCnt != 0 )
				return FALSE;
			iCnt++;

			AdrsTmp.Adrs = strValue;
		} else if( strCompWord.CompareNoCase( _T("scale") ) == 0 ) {
			if ( iCnt != 1 )
				return FALSE;

			if ( AheGetKeywordValue( strTmp, strValue, _T("scale=0123456789."), szTmp, 0 ) == FALSE )
				return FALSE;

			AdrsTmp.Scale = _tcstod( (LPCTSTR)szTmp, &lpszTmp );
		} else if( strCompWord.CompareNoCase( _T("") ) == 0 ) {
			return FALSE;
		} else {
			return FALSE;
		}

		if( iCount < 0 )
			break;
		strURI = strURI.Right( strURI.GetLength() - iCount - 1 );
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
// �R�[�h�l�h���C���Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
BOOL AheCodedValueDomainName2VARIANT( ICodedValueDomainPtr ipCodedValueDomain, LPCTSTR lpszCodedValueDomainName, VARIANT& va )
{
	USES_CONVERSION;

	LONG lDomainCount   = 0L;
	BSTR bstrDomainName = NULL;

	va.vt = VT_NULL; // ������

	ipCodedValueDomain->get_CodeCount( &lDomainCount );

	for( LONG i = 0; i < lDomainCount; i++ ) {
		ipCodedValueDomain->get_Name( i, &bstrDomainName );	// �R�[�h�l�h���C�����̎擾

		if( lstrcmpi( lpszCodedValueDomainName, OLE2T( bstrDomainName ) ) == 0 )
		{
			ipCodedValueDomain->get_Value( i, &va );

			return TRUE;
		}
	}
	// ������Ȃ�����
	ATLTRACE("AheCodedValueDomainName2VARIANT() : �R�[�h�l�h���C���̖��̂�������܂���ł��� : %s\n", lpszCodedValueDomainName );

	return FALSE;
}

BOOL AheVARIANT2CodedValueDomainName( ICodedValueDomainPtr ipCodedValueDomain, VARIANT va, CString& lpszCodedValueDomainName )
{
	if( ipCodedValueDomain == NULL ) return FALSE;
	USES_CONVERSION;

	VARIANT vaDomainValue;
	LONG lDomainCount   = 0L;
	LONG lIndex         = -1;	// �����ݒ�
	BSTR bstrDomainName = NULL;

	// ������
	vaDomainValue.vt = VT_NULL;

	ipCodedValueDomain->get_CodeCount( &lDomainCount );

	for( LONG i = 0; i < lDomainCount; i++ ) {
		ipCodedValueDomain->get_Value( i, &vaDomainValue );

		switch ( vaDomainValue.vt ) {
			case VT_I2:	if( va.iVal   == vaDomainValue.iVal   ) lIndex = i; break;
			case VT_I4:	if( va.lVal   == vaDomainValue.lVal   ) lIndex = i; break;
			case VT_R4: if( va.fltVal == vaDomainValue.fltVal ) lIndex = i; break;
			case VT_R8: if( va.dblVal == vaDomainValue.dblVal ) lIndex = i; break;
			default:
				::MessageBox( ::GetActiveWindow(), 
					ERROR_DOMAIN_SET_EXCEPT_NUMERICAL_VALUE, 
					_T("CAttrDlg::GetCodeNameByValue()"), MB_ICONEXCLAMATION );
				break;
		}
		if( lIndex >= 0 ) break;
	}
	// ������Ȃ������ꍇ
	if( lIndex < 0 ) {
// �R�[�h�l�h���C�����ݒ肳��Ă���A���� NULL OK �ł����݂̒l�� NULL �̏ꍇ������̂�
// �����̃��b�Z�[�W�{�b�N�X�͊����ĊO��
//		::MessageBox( ::GetActiveWindow(), 
//			_T("��������R�[�h�l�h���C����������܂���ł���"),
//			_T("CAttrDlg::GetCodeNameByValue()"), MB_ICONEXCLAMATION );
		ATLTRACE(_T("CAttrDlg::GetValueByCodeName : ��������R�[�h�l�h���C����������܂���\n"));

		return FALSE;
	}
	// ���������ꍇ
	else {
		ipCodedValueDomain->get_Name( lIndex, &bstrDomainName );
		lpszCodedValueDomainName = OLE2T( bstrDomainName );
	}

	return TRUE;
}

BOOL AheGetDomainNameAndValue( IUnknownPtr ipUnk, LPCTSTR lpszFieldName, CString& strDomainName, VARIANT& vaValue )
{
	IRowBufferPtr ipRowBuffer( ipUnk );
	IFieldsPtr ipFields = NULL;
	IFieldPtr ipField = NULL;
	IDomainPtr ipDomain = NULL;
	ICodedValueDomainPtr ipCodedValueDomain = NULL;

	LONG lIndex = 0;

	if( ipRowBuffer == NULL ) return FALSE;

	ipRowBuffer->get_Fields( &ipFields );
	ipFields->FindField( CComBSTR( lpszFieldName ), &lIndex );
	if(lIndex < 0) return FALSE;	//!< ����ȃt�B�[���h�͂Ȃ��A�ƌ����Ă���̂ŏI�����܂��傤
	ipRowBuffer->get_Value( lIndex, &vaValue );
	if( vaValue.vt == VT_EMPTY ) return FALSE;	//!< �擾�ł��Ȃ��ꍇ�� VT_EMPTY �̂�
	ipFields->get_Field( lIndex, &ipField );
	ipField->get_Domain( &ipDomain );
	if( ipDomain == NULL ) return TRUE;	//!< �l�͎擾�ł��Ă���̂� OK
	ipCodedValueDomain = ipDomain;
	if( ipCodedValueDomain != NULL ) 
		AheVARIANT2CodedValueDomainName( ipCodedValueDomain, vaValue, strDomainName );
	
	return TRUE;
}

CComVariant AheGetDomainName( IUnknownPtr ipUnk, LPCTSTR lpszFieldName )
{
	CComVariant value;
	CString domainName;
	if(!AheGetDomainNameAndValue(ipUnk, lpszFieldName, domainName, value))
		return value;
	return domainName;
}

BOOL AheGetDomainByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IDomain** ipRetDomain )
{
	IFieldPtr ipField = NULL;
	IDomainPtr ipDomain = NULL;

	if( AheGetFieldByFieldName( piMap, lpszTableName, lpszFieldName, &ipField ) == FALSE ) return FALSE;
	ipField->get_Domain( &ipDomain );

	if( ipDomain != NULL ) {
		(*ipRetDomain) = ipDomain;
		(*ipRetDomain)->AddRef();

		return TRUE;
	}
	
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//
// ����Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
BOOL AheIsEndNode( IFeaturePtr ipNodeFeature )
{
	IObjectClassPtr ipObjectClass;
	IFieldsPtr ipFields;
	IFieldPtr ipField;
	
	long lCount = 0;
	BOOL bFlg = FALSE;
	CComVariant vValue, vIsValid;
	CComBSTR sName;

	USES_CONVERSION;

	/// �V���{�����[�_�Ȃ� NODE_CLASS ���Q�Ƃ����O��ŏ�����Ă��܂�

	/// �^����ꂽ Feature ����t�B�[���h�����擾
	ipNodeFeature->get_Class(&ipObjectClass);

	if (ipObjectClass) {
		ipObjectClass->get_Fields(&ipFields);

		if (ipFields) {
			/// �t�B�[���h�����擾
			ipFields->get_FieldCount(&lCount);

			for( LONG i = 0; i < lCount; i++ ) {
				ipFields->get_Field(i, &ipField);

				if (ipField) {
					ipField->get_Name(&sName);

					///  NODE_CLASS ��̒l���Q�ł��邩�`�F�b�N
					if ( sName == _T("NODE_CLASS") )
					{
						vValue.lVal = 2;

						ipField->CheckValue(vValue, &vIsValid.boolVal);

						if( vIsValid.boolVal )
							bFlg = TRUE;
						break;
					}
				}
			}
		}
	}

	return bFlg;
}
/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[�`���I���Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
IQueryFilterPtr AheGetSpatialFilter( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/ )
{
	IGeometryPtr ipSearchGeomCopy( AheGetClone( ipGeom ) );
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );

	if( ipFilter != NULL && ipSearchGeomCopy != NULL && ipFeatureClass != NULL )
	{
		// �t�B�[�`���N���X�̋�ԎQ�Ǝ擾
		IGeoDatasetPtr ipDataset( ipFeatureClass );
		ISpatialReferencePtr ipClassSpRef;
		ipDataset->get_SpatialReference( &ipClassSpRef );
/*		// �����G���A�ƃt�B�[�`���N���X�̋�ԎQ�Ƃ���v������ <- �Ӗ��Ȃ��݂����c
		AheSnapToSpatialReference( ipSearchGeomCopy, ipClassSpRef ); <- ���ƁA�ӂɃX�i�b�v�������̃|�C���g�Ō�������Ƃ��Ȃǂɂ��܂�̂ŃR�����g�A�E�g�I�I*/

		CComBSTR bstrFieldName;
		// �N�G���t�B���^�쐬
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		//AheTraceGeometry( ipSearchGeomCopy, _T("AheGetSpatialFilter()") );
		ipFilter->putref_Geometry( ipSearchGeomCopy );
		IGeometryPtr ipGeomTmp;
		ipFilter->get_Geometry( &ipGeomTmp );
		//AheTraceGeometry( ipGeomTmp, _T("AheGetSpatialFilter()") );
		ipFilter->put_SpatialRel( spatialRel );
		if( lpcszSubFields != NULL )
			ipFilter->put_SubFields( CComBSTR( lpcszSubFields ) );
		if( lpcszWhereClause != NULL )
			ipFilter->put_WhereClause( CComBSTR( lpcszWhereClause ) );
/*		// �o�͎��̋�ԎQ�Ƃ����킹�邽�߂Ƀt�B�[�`���N���X�̋�ԎQ�Ɛݒ� <- �����Ȃ��݂����c
		if( ipClassSpRef != NULL )
			ipFilter->putref_OutputSpatialReference( bstrFieldName, ipClassSpRef );*/
	}

	return ipFilter;
}
LONG AheSelectByShapeAndGetCount( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, IFeatureCursor**  ipFeatureCursor, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bRecycling/* = FALSE*/ )
{
	// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( AheGetSpatialFilter( ipGeom, ipFeatureClass, spatialRel, lpcszSubFields, lpcszWhereClause ) );

	if( ipFeatureCursor == NULL || ipFeatureClass == NULL || ipFilter == NULL )
		return NULL;

	// �J�E���g�擾
	LONG lCount = -1;
	if( FAILED( ipFeatureClass->FeatureCount( ipFilter, &lCount ) ) )
		AheTraceErrorMessage( _T("AheSelectByShape") );
	// ����
	IFeatureCursorPtr pIFeatureCursor;

	if( FAILED( ipFeatureClass->Search( ipFilter, ( bRecycling == TRUE ? VARIANT_TRUE : VARIANT_FALSE ), &pIFeatureCursor ) ) )
	{
		(*ipFeatureCursor) = NULL;
		AheTraceErrorMessage( _T("AheSelectByShape") );
	} else 
		(*ipFeatureCursor) = pIFeatureCursor;

	if( (*ipFeatureCursor) != NULL )
		(*ipFeatureCursor)->AddRef();

	return lCount;
}
IFeatureCursorPtr AheSelectByShape( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bRecycling/* = FALSE*/ )
{
	// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( AheGetSpatialFilter( ipGeom, ipFeatureClass, spatialRel, lpcszSubFields, lpcszWhereClause ) );

	if(ipFeatureClass == NULL || ipFilter == NULL )
		return NULL;

	// ����
	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED( ipFeatureClass->Search( ipFilter, ( bRecycling == TRUE ? VARIANT_TRUE : VARIANT_FALSE ), &ipFeatureCursor ) ) )
		AheTraceErrorMessage( _T("AheSelectByShape") );

	return ipFeatureCursor;
}
IFeatureCursorPtr AheSelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel/* = esriSpatialRelIntersects*/, LPCTSTR lpcszSubFields/* = NULL*/, LPCTSTR lpcszWhereClause/* = NULL*/, BOOL bRecycling/* = FALSE*/ )
{
	return AheSelectByShape( ipGeom, AheGetFeatureClass( ipLayer ), spatialRel, lpcszSubFields, lpcszWhereClause, bRecycling );
}
/////////////////////////////////////////////////////////////////////////////
//
// �Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
void AheGetFeatureVertex( IFeaturePtr ipFeature, IPointCollection** ipRetCol )
{
	// TODO: ToMultipoint�Ƃ����֐��ł��ق����悢
	if( ! ( ipFeature && ipRetCol ) )
	{
		_ASSERTE( ipFeature && ipRetCol );
		return;
	}
	*ipRetCol = NULL; // ������

	// �\�z

	IGeometryPtr ipFeatGeom;
	ipFeature->get_Shape( &ipFeatGeom );

	IPointCollectionPtr piPointCollection;
	if( SUCCEEDED( piPointCollection.CreateInstance( CLSID_Multipoint)))
	{
		((IGeometryPtr)piPointCollection)->putref_SpatialReference( AheGetSpatialReference( ipFeatGeom ) ); // ��ԎQ�Ƃ�t�^
		(*ipRetCol) = piPointCollection;
		(*ipRetCol)->AddRef();

		IPointCollectionPtr ipCol;
		IGeometryPtr ipVertexGeom;
		LONG lPointCount = 0;

		IPolylinePtr ipPolyline;
		IPolygonPtr ipPolygon;
		IPointPtr ipPoint;
		// �_��擾
		if(SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPolyline, (void**)&ipPolyline ) ) )
			ipCol = ipPolyline;
		else if( SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPolygon, (void**)&ipPolygon ) ) )
			ipCol = ipPolygon;
		else if( SUCCEEDED( ipFeatGeom->QueryInterface( IID_IPoint, (void**)&ipPoint ) ) ) {
			// IPoint �� IPointCollecion �̃C���^�[�t�F�[�X�������Ă��Ȃ��̂ŕʂɓ���Ă��
			(*ipRetCol)->AddPoint( ipPoint );
			ipCol = ipPoint;
			return;
		}
		else
			return;	// �|�C���g�A�|�����C���A�|���S���ȊO�͂Ƃ肠�����m��܂���
		// �_��R�s�[
		ipCol->get_PointCount( &lPointCount );
		for( LONG j = 0; j < lPointCount; j++ ) {
			ipCol->get_Point( j, &ipPoint );
			(*ipRetCol)->AddPoint( ipPoint );
		}
	}
}
esriGeometryHitPartType AheTestFeatureGeometryHit( IFeaturePtr piFeature, IPointPtr piTestPoint, double dbTorelance, IPoint** piPoint, double* pdbHitDist, LONG* plPartIndex, LONG* plVertexIndex, esriGeometryHitPartType testType/* = esriGeometryPartVertex|esriGeometryPartBoundary*/ )
{
	IGeometryPtr piGeom;

	piFeature->get_ShapeCopy( &piGeom);

	ATLASSERT( piGeom != NULL );

	return AheTestGeometryHit( piGeom, piTestPoint, dbTorelance, piPoint, pdbHitDist, plPartIndex, plVertexIndex, testType );
}

esriGeometryHitPartType AheTestGeometryHit( IGeometryPtr ipGeom, IPointPtr ipTestPoint, double dwTorelance, IPoint** ipPoint, double* pdwHitDist, LONG* plHitPartIndex, LONG* plHitSegmentIndex, esriGeometryHitPartType testType/* = esriGeometryPartVertex|esriGeometryPartBoundary*/ )
{
	if( ipGeom == NULL )
	{
		_ASSERTE( ipGeom != NULL );
		return esriGeometryPartNone;
	}

	ISpatialReferencePtr 	ipSpRef( AheGetSpatialReference( ipGeom ) );
	IPointPtr 				ipRetPoint;
	IGeometryPtr			ipTestGeom;

	// �W�I���g���̌^�𒲂ׂ�Ring��Path�Ȃ炻�ꂼ��Polygon��Polyline�ɕϊ����Ă��
	// TODO: ToPolyline�AToPolygon������3�����Z�q��OK
	IGeometryCollectionPtr ipGeomCol;
	esriGeometryType geomType;
	ipGeom->get_GeometryType( &geomType );

	switch( geomType )
	{
		case esriGeometryPath: ipGeomCol.CreateInstance( CLSID_Polyline ); break;
		case esriGeometryRing: ipGeomCol.CreateInstance( CLSID_Polygon  ); break;
		default: break;
	}
	if( ipGeomCol )
		((IGeometryPtr)ipGeomCol)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^

	if( geomType == esriGeometryPath || geomType == esriGeometryRing )
	{
		ipGeomCol->AddGeometry( ipGeom );
		ipTestGeom = ipGeomCol;
	}
	else
		ipTestGeom = ipGeom;

	IHitTestPtr	ipHitTest( ipTestGeom );
	if ( ! ipHitTest )
	{
		esriGeometryType emTestGeomType = esriGeometryAny;
		ipTestGeom->get_GeometryType( &emTestGeomType );
		ATLTRACE( _T("%sAheTestGeometryHit() : IHitTest�ŃT�|�[�g����Ă��Ȃ��^��������܂���FesriGeometryType=%d\n"), __FILE__LINE__, emTestGeomType );
		ATLASSERT(FALSE);
		return esriGeometryPartNone;
	}

	if ( SUCCEEDED( ipRetPoint.CreateInstance( CLSID_Point ) ) ) 
	{
		ipRetPoint->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^

		double 						dwHitDist = -1.0f;
		LONG 						lHitPartIndex = -1, lHitSegmentIndex = -1;
		VARIANT_BOOL 				vbRightSide = VARIANT_FALSE;
		VARIANT_BOOL 				vbHit       = VARIANT_FALSE;

		for( INT i = 0; i < 3; i++ )
		{
			esriGeometryHitPartType type;
			switch( i )
			{
				// �D�揇��
				case 0: type = esriGeometryPartEndpoint; break;
				case 1: type = esriGeometryPartVertex; break;
				case 2: type = esriGeometryPartBoundary; break;
				default: type = esriGeometryPartNone; break;
			}

			if( ( testType & type ) != 0 && 
				SUCCEEDED( ipHitTest->HitTest( ipTestPoint, dwTorelance, type, ipRetPoint, &dwHitDist, &lHitPartIndex, &lHitSegmentIndex, &vbRightSide, &vbHit ) ) ) 
			{
				if( vbHit )
				{
					if( ipPoint )
					{
						// HitTest�ŕԂ��Ă���|�C���g�̋�ԎQ�Ƃ͕K����炵���̂ŁA�e�X�g�`��̋�ԎQ�Ƃ������ŃZ�b�g����ibug 4321�j
						// TODO: ��鎞�ɃZ�b�g���Ă��΂���Ȃ��񂶂�Ȃ��́H�H
						ipRetPoint->putref_SpatialReference( ipSpRef );

						(*ipPoint) = ipRetPoint;
						(*ipPoint)->AddRef();
					}
					if( pdwHitDist )		(*pdwHitDist)			= dwHitDist;
					if( plHitPartIndex )	(*plHitPartIndex)		= lHitPartIndex;
					if( plHitSegmentIndex )	(*plHitSegmentIndex)	= lHitSegmentIndex;

					return type;
				}
			}
		}
	}
	return esriGeometryPartNone;
}

LONG AheHitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance )
{
	IPointPtr ipHitPoint = NULL, ipRetPoint( CLSID_Point );
	ipRetPoint->putref_SpatialReference( AheGetSpatialReference( ipFeature ) ); // ��ԎQ�ƕt�^
	DOUBLE dHitDist = -1.0f;
	LONG lHitPartIndex = -1, lHitSegmentIndex = -1;

	// �q�b�g�e�X�g
	if( AheTestFeatureGeometryHit( ipFeature, ipPoint, dTorelance, &ipHitPoint, &dHitDist, &lHitPartIndex, &lHitSegmentIndex, esriGeometryPartVertex ) == esriGeometryPartVertex )
		return lHitSegmentIndex;
	else
		return -1;
}
/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[�`���ҏW�Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////

/*_ISetPtr AheSplitLineByPoint( IFeaturePtr ipLineFeature, IGeometryPtr ipSplitPointGeom)
{
	_ISetPtr ipSet = NULL;
	IFeatureEditPtr ipFeatureEdit( ipLineFeature);

	/// �X�v���b�g
	ipFeatureEdit->Split( ipSplitPointGeom, &ipSet);

	return ipSet;
}*/

_ISetPtr AheSplitLineByPoints( IFeaturePtr piLineFeature, IPointCollectionPtr piSplitPoipnts)
{
	long lCount;
	piSplitPoipnts->get_PointCount( &lCount);

	if( 0 >= lCount)return NULL;

	_ISetPtr piReslut;
	if( SUCCEEDED( piReslut.CreateInstance( CLSID_Set)))
	{
		_ISetPtr piSetBase;
		if( SUCCEEDED( piSetBase.CreateInstance( CLSID_Set)))
		{
			piSetBase->Add( piLineFeature);

			IFeaturePtr piFeature;
			IPointPtr piPoint;

			IRelationalOperatorPtr piRelation;
			IGeometryPtr piTestGeom;
			VARIANT_BOOL stBool;
			IUnknownPtr piUnknown;
			IFeatureEditPtr piEditFeature;
			for( long lIndex = 0; lIndex < lCount; lIndex++)
			{
				if( SUCCEEDED( piSplitPoipnts->get_Point( lIndex, &piPoint)))
				{
					piSetBase->Reset();
					while( S_OK == piSetBase->Next( &piUnknown))
					{
						piFeature = piUnknown;

						piFeature->get_Shape( &piTestGeom);

						piRelation = piTestGeom;

						if( SUCCEEDED( piRelation->Disjoint( piPoint, &stBool)))
						{
							if( VARIANT_FALSE == stBool)
							{
								// �X�v���b�g�ΏۂƂȂ�t�B�[�`�����A���ʃ��X�g���폜
								piSetBase->Remove( piUnknown);
								break;
							}
						}
						piFeature = NULL;
					}
					if( NULL == piFeature)continue;

					piEditFeature = piFeature;

					_ISetPtr piSetWork1 = NULL;
					if( SUCCEEDED( piEditFeature->Split( piPoint, &piSetWork1)))
					{
						// ���ʃ��X�g�֊i�[
						piSetWork1->Reset();
						while( S_OK == piSetWork1->Next( &piUnknown))
						{
							piSetBase->Add( piUnknown);
						}
						ATLTRACE( TEXT( "split\n"));
					}
					else
					{
						// �{���ł���΂����ւ��邱�Ƃ͂Ȃ����ADB��PC��̃W�I���g�����x�̍�����
						// �G���[����������P�[�X���l�����邪�A���̏ꍇ�X�v���b�g�p����D�悳
						// ����B
						ATLASSERT(FALSE);
						piSetBase->Add( piFeature);
						ATLTRACE( TEXT( "split NG\n"));
						continue;
					}
				}
			}

			piReslut = piSetBase;

			/*
			if( NULL != piSetBase)
			{
				// ���ʃ��X�g�֊i�[
				piSetBase->Reset();
				while( S_OK == piSetBase->Next( &piUnknown))
				{
					piReslut->Add( piUnknown);
				}
			}*/
		}
	}
	return piReslut;
}
void AheFeature2Polygon( IFeaturePtr ipFeature, double dbBufferSize, IPolygon** ipPolygon )
{
	IGeometryPtr ipGeom;

	ipFeature->get_Shape( &ipGeom );
	AheGeometry2Polygon( ipGeom, dbBufferSize, ipPolygon );
}
IPolygonPtr AheGeometry2Polygon( IGeometryPtr ipGeom, double dbBufferSize, IPolygon** ipPolygon )
{
	IPolygonPtr ipRet; // �Ԃ�l
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeom ) ); // �R�s�[�쐬

	// ��W�I���g������Ȃ����ǂ����`�F�b�N
	if ( ipGeomCopy ) {
		VARIANT_BOOL vbIsEmpty = VARIANT_FALSE;

		ipGeomCopy->get_IsEmpty( &vbIsEmpty );

		// ��W�I���g������Ȃ����ǂ����`�F�b�N
		if ( !vbIsEmpty ) {
			ISpatialReferencePtr ipSpRef;

			ipGeomCopy->get_SpatialReference( &ipSpRef );
			double dbBuffer = dbBufferSize, dMinLength = 0.0f;

			if ( ipSpRef )
				dMinLength = AheGetXYDomain1UnitLength( ipSpRef );

			// �o�b�t�@�T�C�Y���K�؂��ǂ����`�F�b�N
			if ( dMinLength > dbBuffer )
				dbBuffer = dMinLength;

			esriGeometryType emType = esriGeometryAny;

			ipGeomCopy->get_GeometryType( &emType );

			// �|�C���g�̎��́AEnvelope�ɕϊ����Ă���|���S����
			if ( emType == esriGeometryPoint ) {
				IEnvelopePtr ipEnv;

				ipGeomCopy->get_Envelope( &ipEnv );
				if ( ipEnv )
					ipEnv->Expand( dbBuffer, dbBuffer, VARIANT_FALSE );

				ipGeomCopy = ipEnv;
			}

			if ( ipGeomCopy ) {
				ipGeomCopy->get_GeometryType( &emType );

				switch ( emType ) {
					case esriGeometryEnvelope:
						ipRet = AheEnvelope2Polygon( ipGeomCopy );
						break;
					case esriGeometryPolygon:
						if ( dbBufferSize == 0.0f ) {
							ipRet = ipGeomCopy;
							break;
						}
					case esriGeometryPolyline:
						{
							ITopologicalOperatorPtr ipTopo( ipGeomCopy );

							if ( ipTopo ) {
GEOMETRY2POLYGON:
								IGeometryPtr ipBuffer;

								try {
									ipTopo->Buffer( dbBuffer, &ipBuffer );
								} catch(...) {
									goto GEOMETRY2POLYGON;
								}

								ipRet = ipBuffer;
							}
						}
						break;
					case esriGeometryBag:
						{
							IEnumGeometryPtr ipEnumGeom( ipGeomCopy );
							IGeometryPtr ipGeomPart;
							IPolygonPtr ipPoly;

							ipEnumGeom->Reset();
							while( S_OK == ipEnumGeom->Next( &ipGeomPart ) ) {
								IPolygonPtr ipPolygonPart = AheGeometry2Polygon( ipGeomPart, dbBuffer );

								if ( !ipPoly )
									ipPoly = ipPolygonPart;
								else {
									ITopologicalOperatorPtr ipTopo = ipPoly;
									IGeometryPtr ipResultGeom;

									if ( ipTopo )
										ipTopo->Union( ipPolygonPart, &ipResultGeom );

									if ( ipResultGeom )
										ipPoly = ipResultGeom;
								}
							}

							ipRet = ipPoly;
						}
						break;
					default:
						AheTraceGeometry( ipGeom, _T("AheGeometry2Polygon : ���T�|�[�g�`��") );
						_ASSERTE(false); // �T�|�[�g���ĂȂ�
						break;
				}
			}
		}
	}

	if ( ipRet ) {
		AheForceSimplify( ipRet );

		if ( ipPolygon ) {
			(*ipPolygon) = ipRet;
			if ( (*ipPolygon) )
				(*ipPolygon)->AddRef();
		}
	}

	return ipRet;	
}

LONG AheGetFieldMap( IFeaturePtr piFeature, FIELDMAP& cNameMap, FIELDMAP& cAliasMap)
{
	LONG lFields = 0;
	IObjectClassPtr piObjClass;
	if( SUCCEEDED( piFeature->get_Class( &piObjClass)))
	{
		IFieldsPtr piFields;
		if( SUCCEEDED( piObjClass->get_Fields( &piFields)))
		{
			USES_CONVERSION;
			CComBSTR cComBSTR;
			IFieldPtr piField;

			piFields->get_FieldCount( &lFields);
			for( LONG lIndex = 0; lIndex < lFields; lIndex++)
			{
				if( SUCCEEDED( piFields->get_Field( lIndex, &piField)))
				{
					piField->get_Name( &cComBSTR);
					cNameMap[ OLE2T( cComBSTR)] = lIndex;

					piField->get_AliasName( &cComBSTR);
					cAliasMap[ OLE2T( cComBSTR)] = lIndex;
				}
			}
		}
	}
	return lFields;
}

BOOL AheGetEnvelope( IPointPtr ipPoint, IDisplayPtr piScrnDisp, LONG pixel, IEnvelope** ppiEnvelope)
{
	return AheGetEnvelope( ipPoint, AheConvertPixelsToMapUnits( piScrnDisp, pixel), ppiEnvelope);
}

BOOL AheGetEnvelope( IPointPtr ipPoint, DOUBLE dTolerance, IEnvelope** ppiEnvelope)
{
	if( dTolerance > 0 )
	{
		IEnvelopePtr ipEnv = NULL;
		if( SUCCEEDED( ipPoint->get_Envelope( &ipEnv)))
		{
			DOUBLE dRadius = dTolerance;
			ipEnv->put_Height( dRadius );
			ipEnv->put_Width( dRadius );
			ipEnv->CenterAt( ipPoint );

			( *ppiEnvelope) = ipEnv;
			( *ppiEnvelope)->AddRef();
		}
		AheTraceGeometry( (IGeometryPtr)ipEnv, _T("AheGetEnvelope()") );
		return TRUE;
	}

	return FALSE;
}

int AheMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel)
{
	return AheMessageBox( ::GetActiveWindow(), lpcszMsg, lpcszCap, uType, uLevel);
}

int AheMessageBox( HWND hWnd, LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel)
{
	UINT unRegLevel = WARNLEVEL_NONE;

	TCHAR  szOpenRegistry[MAX_PATH];
	lstrcpy( szOpenRegistry, SOFTWARE_KEY);
	lstrcat( szOpenRegistry, INSTALL_KEY);
	lstrcat( szOpenRegistry, WARNING_SECTION);
	HKEY hOpenKey = EmxCreateRegKey( szOpenRegistry, HKEY_LOCAL_MACHINE);
    if( hOpenKey)
    {
		DWORD  uResult;
        if( FALSE != EmxGetRegDword( LEVEL_KEY, &uResult, hOpenKey))
		{
			unRegLevel = static_cast<UINT>( uResult);
		}

		// �K�v�Ȃ��Ȃ����i�K�ŕK���N���[�Y���邱�Ɓi���Ȃ��ƃ��������[�N�j
		RegCloseKey( hOpenKey );	
    }

	if( 0 >= ( static_cast<int>( unRegLevel) - static_cast<int>( uLevel)))
	{
		return ::MessageBox( hWnd, lpcszMsg, lpcszCap, uType);
	}

    return IDCANCEL;
}
// ZooMoveTool�̃L�[�ړ��g��k������
BOOL AheZooMoveKeyDownProc( IActiveViewPtr piActView, LONG lKeyCode, LONG lShift, BOOL* pblHandled)
{
	*pblHandled = TRUE;

	BOOL blReslut = TRUE;

	try {

	switch( lKeyCode)
	{
	// �X�y�[�X�L�[�ŁA�J�[�\�������}�b�v�̃Z���^�[�ֈړ�����
	case VK_SPACE:
		blReslut = ::AheMoveCursor2Center( piActView);
		break;

	//�@NumPAD���Œn�}�̈ړ��������B+CTRL�ň��ʈړ��A�ʏ�͔ŉ�ʈړ�
	case 'Z':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD1:
		::AheShiftMap( piActView, ( 2 == lShift) ? -1 : -2, ( 2 == lShift) ? -1 : -2);
		break;
	case 'S':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD2:
		::AheShiftMap( piActView, 0, ( 2 == lShift) ? -1 : -2);
		break;
	case 'C':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD3:
		::AheShiftMap( piActView, ( 2 == lShift) ? 1 : 2, ( 2 == lShift) ? -1 : -2);
		break;
	case 'A':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD4:
		::AheShiftMap( piActView, ( 2 == lShift) ? -1 : -2, 0);
		break;
	case 'D':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD6:
		::AheShiftMap( piActView, ( 2 == lShift) ? 1 : 2, 0);
		break;
	case 'Q':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD7:
		::AheShiftMap( piActView, ( 2 == lShift) ? -1 : -2, ( 2 == lShift) ? 1 : 2);
		break;
	case 'W':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD8:
		::AheShiftMap( piActView, 0, ( 2 == lShift) ? 1 : 2);
		break;
	case 'E':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD9:
		::AheShiftMap( piActView, ( 2 == lShift) ? 1 : 2, ( 2 == lShift) ? 1 : 2);
		break;
	//�@NumPAD��5�͌���J�[�\���ʒu����ʂ̃Z���^�[�ֈړ�����
	case 'X':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_NUMPAD5:
		{
			::AheMoveCenterToMousePointer( piActView, false );
			::AheMoveCursor2Center( piActView);
			AheRefresh(piActView);
		}
		break;

	//�@NumPAD��+-��ZoomUP/DOWN
	case '2':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_ADD:
		{
			::AheMoveCenterToMousePointer( piActView );
			::AheMoveCursor2Center( piActView);
			::AheZoomUp( piActView);
			AheRefresh(piActView);
		}
		break;
	case '3':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
	case VK_SUBTRACT:
		::AheZoomDown( piActView);
		AheRefresh(piActView);
		break;
	//�@NumPAD��.�őS�̕\�����s��
/*	case VK_DECIMAL:
		{
			IEnvelopePtr iEnv;

			piActView->get_FullExtent( &iEnv);
			piActView->put_Extent( iEnv);
			piActView->Refresh();
		}
		break;
*/	//�@NumPAD��/*�ōő�ŏ���Zoom
	case VK_MULTIPLY:
		{
			IMapPtr iMap;

			piActView->get_FocusMap( &iMap);
			iMap->put_MapScale( static_cast<double>( 1562));
			AheRefresh(piActView);
		}
		break;
	case VK_DIVIDE:
		{
			IMapPtr iMap;

			piActView->get_FocusMap( &iMap);
			iMap->put_MapScale( static_cast<double>( 3200000));
			AheRefresh(piActView);
		}
		break;
	// 1�L�[�ōĕ`��
	case '1':
		if( 0 != lShift)
		{
			*pblHandled = FALSE;
			break;
		}
		AheRefresh(piActView);
		break;
	default:
		pblHandled = FALSE;
		break;
	}

	} catch(...) {
		CString strMsg;
		strMsg.Format(ERROR_KEY_EVENT_EXCEPTION_OCCURRED, lKeyCode, lShift );
		MessageBox( GetActiveWindow(), strMsg, ZOOM_IN_OUT_SCROLL, MB_OK|MB_ICONWARNING );
	}

	return blReslut;
}
// �}�E�X�|�C���^�̈ʒu�����S�ɂ���悤�ɃX�N���[������
BOOL AheMoveCenterToMousePointer( IActiveViewPtr piActiveView, bool bSuppressEvents )
{
	if( !piActiveView )
		return false;

	// �}�E�X�|�C���^�̃f�o�C�X���W�擾
	IScreenDisplayPtr ipDisp;
	piActiveView->get_ScreenDisplay( &ipDisp );
	if( !ipDisp )
		return false;

	POINT stPoint = { 0, 0 };
	HWND hWnd = 0;
	::GetCursorPos( &stPoint );
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ScreenToClient( hWnd, &stPoint );

	// �n�}���W�ɕϊ�
	IPointPtr ipPoint;
	IDisplayTransformationPtr ipDispTrans;
	ipDisp->get_DisplayTransformation( &ipDispTrans );
	if( !ipDispTrans )
		return false;

	ipDispTrans->ToMapPoint( stPoint.x, stPoint.y, &ipPoint );

	// ���S�ֈړ�
	IEnvelopePtr ipEnv;
	piActiveView->get_Extent( &ipEnv );
	ipEnv->CenterAt( ipPoint );

	// [Bug 3772]�g�傪�����Ȃ��Ȃ�o�O�ւ̑Ή�
	// �}�b�v�̃T�C�Y�͕ς��Ȃ��̂ɁA���W�ړ��O��Envelope�����W�ړ����
	// Envelope�ɐݒ肷��ƃX�P�[�����ς�邱�Ƃ�����Ƃ������
	// ��U�C�x���g���Ƃ߂āAVisibleBounds�ŗ̈��ݒ肷��
	// [Bug 5495]�������A�C�x���g���Ƃ߂Ă��܂��ƕ`��̕ύX�͂����Ă��A�����I�ɂ͑O�̍��W�ʒu�ł��邽��
	// MoveCenterToMousePointer�̌�ɃC�x���g�𔭍s���鏈�����Ȃ��ꍇ�ɂ́A�C�x���g�Ƃ߂Ȃ����Ƃɂ���
	// �ꉞ���̏����ł���΁A[Bug 3772]�Ɨ����͂ł���
	IDisplayTransformationPtr ipTrans;
	ipDisp->get_DisplayTransformation( &ipTrans );
	if( !ipTrans )
		return false;

	// �C�x���g�~�߂�O�Ɍ��݂̃C�x���g��Ԃ��擾
	VARIANT_BOOL vaSurressEvents = VARIANT_FALSE;
	ipTrans->get_SuppressEvents( &vaSurressEvents );

	if( bSuppressEvents )
		ipTrans->put_SuppressEvents( VARIANT_TRUE );// �C�x���g�~�߂�
	ipTrans->put_VisibleBounds( ipEnv );
	if( bSuppressEvents )
	{
		// ���łɒN�����C�x���g�~�߂Ă�����A�C�x���g�Ƃ߂Ă͂����Ȃ��̂ŁA�����Ń`�F�b�N
		if( vaSurressEvents == VARIANT_FALSE )
			ipTrans->put_SuppressEvents( VARIANT_FALSE );// �C�x���g�Đڑ�
	}
	return true;
}

BOOL AheMoveCursor2Center( IActiveViewPtr piActiveView)
{
	if( !piActiveView )
		return FALSE;

	// �}�b�v�̍��W�T�C�Y�擾
	IScreenDisplayPtr ipDisp;
	piActiveView->get_ScreenDisplay( &ipDisp );
	if( !ipDisp )
		return FALSE;

	IDisplayTransformationPtr ipTrans;
	ipDisp->get_DisplayTransformation( &ipTrans );
	if( !ipTrans )
		return FALSE;

	RECT rect;
	ipTrans->get_DeviceFrame( &rect );

	// �}�b�v�̒��S���擾
	POINT stPoint;
	stPoint.x = (rect.left + rect.right )/2;
	stPoint.y = (rect.bottom + rect.top)/2;

	// �N���C�A���g�̈�̍��W����X�N���[�����W�ɕϊ�
	HWND hWnd;
	ipDisp->get_hWnd( reinterpret_cast<OLE_HANDLE*>( &hWnd ) );
	::ClientToScreen( hWnd, &stPoint );

	// �}�E�X�J�[�\���ړ�
	::SetCursorPos( stPoint.x, stPoint.y );
	return TRUE;
}

BOOL AheShiftMap( IActiveViewPtr piActView, int nXStep, int nYStep)
{
	IEnvelopePtr piEnv;
	if( SUCCEEDED( piActView->get_Extent( &piEnv)))
	{
		double dbMin;
		double dbMax;
		double dbWork;

		if( 0 != nXStep)
		{
			piEnv->get_Width( &dbWork);
			piEnv->get_XMax( &dbMax);
			piEnv->get_XMin( &dbMin);
			dbMax += ( dbWork / static_cast<double>( nXStep));
			dbMin += ( dbWork / static_cast<double>( nXStep));
			piEnv->put_XMax( max( dbMax, dbMin));
			piEnv->put_XMin( min( dbMax, dbMin));
		}

		if( 0 != nYStep)
		{
			piEnv->get_Height( &dbWork);
			piEnv->get_YMax( &dbMax);
			piEnv->get_YMin( &dbMin);
			dbMax += ( dbWork / static_cast<double>( nYStep));
			dbMin += ( dbWork / static_cast<double>( nYStep));
			piEnv->put_YMax( max( dbMax, dbMin));
			piEnv->put_YMin( min( dbMax, dbMin));
		}

		if( SUCCEEDED( piActView->put_Extent( piEnv)))
		{
			AheRefresh(piActView);

			return TRUE;
		}
	}

	return FALSE;
}

void AheZoomUp( IActiveViewPtr piActiveView)
{
	double dbCurrent;

	IMapPtr piMap;
	piActiveView->get_FocusMap( &piMap);
	piMap->get_MapScale( &dbCurrent);

	LONG lScale = static_cast<LONG>( floor( dbCurrent));
	dbCurrent = ( ( dbCurrent - static_cast<double>( lScale)) >= static_cast<double>( 0.5)) ? lScale + 1 : lScale;

	double dbScaleBase = 6400000L;
	for( int nZM = 0; nZM <= 18; nZM++)
	{
		double dbScale = floor( dbScaleBase / pow( 2, static_cast<double>( nZM)));
		if( dbScale < dbCurrent)
		{
			piMap->put_MapScale( dbScale);
			break;
		}
	}
}

// MapFAN��ZM�l�ɑ������g�嗦�ł̏k������
void AheZoomDown( IActiveViewPtr piActiveView)
{
	double dbCurrent;

	IMapPtr piMap;
	piActiveView->get_FocusMap( &piMap);
	piMap->get_MapScale( &dbCurrent);

	LONG lScale = static_cast<LONG>( floor( dbCurrent));
	dbCurrent = ( ( dbCurrent - static_cast<double>( lScale)) >= static_cast<double>( 0.5)) ? lScale + 1 : lScale;

	double dbScaleBase = 6400000L;
	for( int nZM = 18; nZM >= 0; nZM--)
	{
		double dbScale = floor( dbScaleBase / pow( 2, static_cast<double>( nZM)));
		if( dbScale > dbCurrent)
		{
			piActiveView->get_FocusMap( &piMap);
			piMap->put_MapScale( dbScale);
			break;
		}
	}
}

// �ĕ`�惂�[�h���擾
bool AheGetRefreshMode()
{
	static BOOL bRefreshMode = -1;
	if( -1 == bRefreshMode )
	{
		CRegKey key;
		DWORD dwRefreshMode = 0;
		if( ERROR_SUCCESS == key.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == key.QueryDWORDValue( _T("RefreshMode"), dwRefreshMode ) )
				bRefreshMode = ( 1 == dwRefreshMode ) ? TRUE : FALSE;
		}
		if( -1 == bRefreshMode )
			bRefreshMode = FALSE;	// [bug 8437]�L�[���Ȃ��ꍇ�͂�����}���ɂ��Ȃ��i�f�t�H���g�j
		// TODO:AheZooMoveFunctions�ɂ����l�̂��̂�����̂ŁA���̏C���ł͂ǂ��炩�Ƀ\�[�X�Ƀ}�[�W���A�Е��폜���邱��
	}
	return (bRefreshMode) ? true : false;
}

// ������}���ĕ`��
void AheRefresh(IActiveViewPtr piActView, IEnvelopePtr ipEnv/*=NULL*/)
{
	if ( piActView )
	{
		if ( AheGetRefreshMode() )
		{
			IEnvelopePtr ipEnvTmp( ipEnv );
			if ( ! ipEnvTmp )
				piActView->get_Extent( &ipEnvTmp );

			IScreenDisplayPtr ipScreen;
			piActView->get_ScreenDisplay( &ipScreen );

			if ( ipScreen )
			{
				IDisplayTransformationPtr ipTrans;
				ipScreen->get_DisplayTransformation( &ipTrans );

				if ( ipTrans )
				{
					ITrackCancelPtr ipCancel;
					ipScreen->get_CancelTracker( &ipCancel );

					HDC hdcCacheMemDC = NULL;
					ipScreen->get_CacheMemDC( 0, (OLE_HANDLE*)&hdcCacheMemDC );

					HWND hWnd = NULL;
					ipScreen->get_hWnd( (OLE_HANDLE*)&hWnd );
					HDC hdcScreen = ::GetDC(hWnd);

					RECT rRect={0};
					ipTrans->get_DeviceFrame( &rRect );
					const int nWidth = rRect.right - rRect.left;
					const int nHeight = rRect.bottom - rRect.top;

					// ��ƗpHDC�𔒂�
					::BitBlt( hdcCacheMemDC, rRect.left, rRect.top, nWidth, nHeight, NULL, 0, 0, WHITENESS );
					// ��ƗpHDC�ɕ`��
					piActView->Output( (OLE_HANDLE)hdcCacheMemDC, 0, &rRect, ipEnvTmp, ipCancel );
					// ���C���ɕ`��
					::BitBlt( hdcScreen, rRect.left, rRect.top, nWidth, nHeight, hdcCacheMemDC, rRect.left, rRect.top, SRCCOPY );
					// �����̈��ݒ�
					ipScreen->Invalidate( ipEnvTmp, VARIANT_TRUE, esriViewBackground );
					// ���t���b�V��
					piActView->PartialRefresh( esriViewForeground, NULL, ipEnvTmp );

					::ReleaseDC(hWnd, hdcScreen);
				}
			}
		}
		else
			piActView->Refresh();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// ���W�X�g���̊�{�A�N�Z�X�֐�
//
/////////////////////////////////////////////////////////////////////////////
HKEY EmxCreateRegKey( LPCTSTR lpszKey, HKEY hKey )
{
    DWORD  dwDisposition;
    HKEY  hOpenedKey;

    if(RegCreateKeyEx( hKey, lpszKey, 0, _T(""), REG_OPTION_NON_VOLATILE, 
        KEY_ALL_ACCESS, NULL, &hOpenedKey, &dwDisposition) != ERROR_SUCCESS)
            hOpenedKey = NULL;
    //  ���s������NULL��Ԃ�
    return hOpenedKey;
}

/////////////////////////////////////////////////////////////////////////////
//  DWORD�l�̏�������

BOOL EmxSetRegDword( LPCTSTR lpszValueName, DWORD dwData, HKEY hKey )
{
    DWORD    dwBuffer = dwData;

    if( RegSetValueEx( hKey, lpszValueName, 0, REG_DWORD, 
        (LPBYTE)&dwBuffer, sizeof(DWORD)) != ERROR_SUCCESS )
            return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//  DWORD�l�̎擾

BOOL EmxGetRegDword( LPCTSTR lpszValueName, DWORD* lpdwBuff, HKEY hKey )
{
    DWORD    dwType;
    DWORD    dwBufferSize = sizeof(DWORD);

    //ASSERT(m_hKey);
    if( RegQueryValueEx( hKey, lpszValueName, 0, &dwType,
        (LPBYTE)lpdwBuff, &dwBufferSize) != ERROR_SUCCESS )
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//  ������̏�������

BOOL EmxSetRegString( LPCTSTR lpszValueName, LPCTSTR lpcszString, HKEY hKey )
{
    DWORD    dwBufferSize = sizeof(TCHAR)*(lstrlen(lpcszString) + 1);

    if( RegSetValueEx( hKey, lpszValueName, 0, REG_SZ,
        (LPBYTE)(LPCTSTR)lpcszString, dwBufferSize) != ERROR_SUCCESS )
            return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//  ������̎擾
BOOL EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, DWORD nSize, HKEY hKey )
{
    DWORD    dwType;
    DWORD    dwBufferSize = nSize;

    if( RegQueryValueEx( hKey, lpszValueName, 0, &dwType,
        (LPBYTE)lpReturnedString, &dwBufferSize) != ERROR_SUCCESS )
            return FALSE;

    return TRUE;
}

BOOL EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, HKEY hKey )
{
    DWORD    dwType;
    DWORD    dwBufferSize = 0;

    if( RegQueryValueEx( hKey, lpszValueName, 0, &dwType,
        (LPBYTE)NULL, &dwBufferSize) != ERROR_SUCCESS )
            return FALSE;
    if( RegQueryValueEx( hKey, lpszValueName, 0, &dwType,
        (LPBYTE)lpReturnedString, &dwBufferSize) != ERROR_SUCCESS )
            return FALSE;

    return TRUE;
}

IPointCollectionPtr AheRemovePoint( IFeaturePtr ipFeature, LONG lIndex )
{
	IGeometryPtr ipGeom = NULL;
	IPointCollectionPtr ipCol = NULL;

	ipFeature->get_ShapeCopy( &ipGeom );
	AheGeometry2PointCollection( ipGeom, &ipCol );

	if( ipCol == NULL )
		return NULL;
	ipCol->RemovePoints( lIndex, 1 );

	return ipCol;
}
