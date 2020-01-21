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
 * @file LayerFunctions.cpp
 * @brief <b>ArcObjects�O���[�o���w���p�[�֐������t�@�C��</b>\n
 * ���̃t�@�C���ɂ́A��Ƀ��C���n�̃O���[�o���֐�����������Ă��܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "Map.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//namespace sindy {

// �e�[�u�������������ǂ������`�F�b�N����
bool _AheIsSameTableName( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2 )
{
	// �ǂ��炩���I�[�i�[�������̏ꍇ�̓I�[�i�[����]���Ώۂ���O��
	LPCTSTR lpcszTableName1Tmp = _tcsstr( lpcszTableName1, _T(".") );
	LPCTSTR lpcszTableName2Tmp = _tcsstr( lpcszTableName2, _T(".") );
	if( NULL == lpcszTableName2Tmp || NULL == lpcszTableName1Tmp )
	{
		lpcszTableName1Tmp = ( NULL != lpcszTableName1Tmp ) ? lpcszTableName1Tmp + 1 : lpcszTableName1;
		lpcszTableName2Tmp = ( NULL != lpcszTableName2Tmp ) ? lpcszTableName2Tmp + 1 : lpcszTableName2;
	}

	return ( lstrcmpi( lpcszTableName1Tmp, lpcszTableName2Tmp ) == 0 ) ? true : false;
}

// ���C������e�[�u�����擾����
ITablePtr AheLayer2Table( ILayer* ipLayer )
{
	if( ipLayer != NULL )
	{
		IFeatureLayerPtr ipFeatureLayer( ipLayer );
		if( ipFeatureLayer != NULL )
		{
			IFeatureClassPtr ipFeatureClass;
			ipFeatureLayer->get_FeatureClass( &ipFeatureClass );
			if( ipFeatureClass != NULL )
				return ipFeatureClass;
		}
	}
	return NULL;
}

// �e�[�u�����ҏW�\���ǂ������`�F�b�N����
BOOL AheIsEditableTable( ITable* ipTable )
{
	if( ! ipTable )
		return FALSE;

	// �ҏW�\���ǂ���
	VARIANT_BOOL vbCanEdit = VARIANT_FALSE;
	IDatasetEditInfoPtr ipDatasetEditInfo( ipTable );
	if( ipDatasetEditInfo )
		ipDatasetEditInfo->get_CanEdit( &vbCanEdit );
	if( ! vbCanEdit )
		return FALSE;

	// �ҏW�J�n����Ă��邩�ǂ���
	VARIANT_BOOL vbBeingEdited = VARIANT_FALSE;
	IDatasetEditPtr ipDatasetEdit( ipTable );
	if( ipDatasetEdit )
		ipDatasetEdit->IsBeingEdited( &vbBeingEdited );
	if( ! vbBeingEdited )
		return FALSE;

	// �ڑ����[�U���ƃe�[�u���I�[�i�[�����قȂ�ꍇ��
	// �ҏW�\�Ƃ݂Ȃ��Ȃ��iSiNDY�S�ʂ̃��[���j
	CString strConnUserName( AheGetConnectUserName( ipTable ) ); // �ڑ����[�U��
	if( 0 != AheGetFeatureClassOwnerName( ipTable ).CompareNoCase( AheGetConnectUserName( ipTable ) ) )
		return FALSE;

	return TRUE;
}

// �e�[�u�������������ǂ������`�F�b�N����
bool /*SINDYLIB_API*/ AheIsEqualTable( ITable* ipTable1, ITable* ipTable2 )
{
	return ( AheGetOwnerTableName( ipTable1 ).Compare( AheGetOwnerTableName( ipTable2 ) ) == 0 ) ? true : false;
}

//////////////////////////////////////////////////////////////////////
//
// ���C�����E�e�[�u�����擾�Ɋւ���֐�
//
//////////////////////////////////////////////////////////////////////
CString AheGetConnectUserName( IUnknownPtr ipUnk )
{
	IDatabaseConnectionInfoPtr ipConn( AheGetWorkspace( ipUnk ) );
	if( ! ipConn )
		return CString(_T(""));

	CComBSTR bstrUser;
	ipConn->get_ConnectedUser( &bstrUser );

	return CString(bstrUser);
}

CString AheGetOwnerDotClassName( IUnknownPtr ipUnk )
{
	IDatasetPtr ipDataset( AheGetTable( ipUnk ) );
	BSTR bstrName = 0;

	if( ipDataset == NULL ) return CString(_T(""));

	ipDataset->get_Name( &bstrName );
	CString strLayerName( bstrName );
	::SysFreeString(bstrName);

	// �啶������
	strLayerName.MakeUpper();

	return strLayerName;
}

CString AheGetFeatureClassName( IUnknownPtr ipUnk )
{
	CString strLayerName = AheGetOwnerDotClassName( ipUnk );

	INT iPos = strLayerName.ReverseFind('.');
	if( iPos >= 0 ) strLayerName = strLayerName.Right( strLayerName.GetLength() - iPos - 1 );

	return strLayerName;
}

CString AheGetFeatureClassOwnerName( IUnknownPtr ipUnk )
{
	CString strOwnerName = AheGetOwnerDotClassName( ipUnk );

	INT iPos = strOwnerName.ReverseFind('.');
	if( iPos > 0 ) 
		strOwnerName = strOwnerName.Left( iPos );
	else
		strOwnerName = _T("");

	return strOwnerName;
}

BOOL AheGetLayerTableName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName )
{
	if( ipUnk == NULL || bstrLayerName == NULL ) return FALSE;

	CComBSTR bstrLayerName2 = AheGetFeatureClassName( ipUnk );
	(*bstrLayerName) = bstrLayerName2.Detach();

	return TRUE;
}

BOOL AheGetLayerAliasName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName )
{
	IFeaturePtr ipFeature( ipUnk );
	IFeatureClassPtr ipFeatureClass = NULL;

	// ipUnk == IFeature �̎��̓V���[�g�J�b�g
	if( ipFeature != NULL ) {
		IObjectClassPtr ipObjectClass = NULL;
		ipObjectClass->get_AliasName( bstrLayerName );

		return TRUE;
	} 
	else if( AheGetFeatureClass( piMap, ipUnk, &ipFeatureClass ) == TRUE ) {
		ipFeatureClass->get_AliasName( bstrLayerName );
		
		return TRUE;
	}

	return FALSE;
}

CString AheGetLayerAliasName( IUnknownPtr ipUnk )
{
	CComBSTR bstrLayerName = 0;

	IFeatureClassPtr ipFeatureClass = AheGetFeatureClass( ipUnk );
	if( ipFeatureClass != NULL )
		ipFeatureClass->get_AliasName( &bstrLayerName );
		
	return CString( bstrLayerName );
}

BOOL AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrMXDName )
{
	IFeaturePtr ipFeature( ipUnk );
	IFeatureClassPtr ipFeatureClass( ipUnk );
	ILayerPtr ipLayer( ipUnk );

	BOOL bIsGet = FALSE;
	if( ipFeature != NULL || ipFeatureClass != NULL ) {
		CComBSTR bstrName;
		USES_CONVERSION;

		// �擾�����e�[�u�����擾
		IDatasetPtr ipDataset;
		if( ipFeature != NULL )
		{
			ITablePtr ipTable;
			ipFeature->get_Table( &ipTable );
			ipDataset = ipTable;
		}
		else if( ipFeatureClass != NULL )
		{
			IFeatureLayerPtr ipFeatureLayer;
			ipFeatureLayer = ipFeatureClass;
			if( ipFeatureLayer != NULL )
			{
				IFeatureClassPtr ipClass;
				ipFeatureLayer->get_FeatureClass( &ipClass );
				ipDataset = ipClass;
			}
		}

		if( ipDataset == NULL )
			return FALSE;

		ipDataset->get_Name( &bstrName );

		CMap cLayers( piMap );
		for( CMap::const_iterator it = cLayers.begin(); it != cLayers.end(); ++it )
		{
			// [bug 1765] �Ԃт�����ȃ��C����get_Workspace����Ɨ�����i���񍐁j�̂�
			// �����͌����郌�C���Ɍ���
			VARIANT_BOOL vaValid = VARIANT_FALSE;
			it->get_Valid( &vaValid );
			if( vaValid )
			{
				IFeatureClassPtr ipSelectFeatureClass;
				IFeatureLayerPtr ipFeatureLayer;
				ipFeatureLayer = *it;
				if( ipFeatureLayer != NULL )
					ipFeatureLayer->get_FeatureClass( &ipSelectFeatureClass );

				IDatasetPtr	ipSelectDataset;
				ipSelectDataset = ipSelectFeatureClass;

				if( ipSelectDataset == NULL )
					continue;

				CComBSTR bstrLayerName;
				ipSelectDataset->get_Name( &bstrLayerName );

				if( lstrcmpiW( bstrName, bstrLayerName ) == 0 )
					return SUCCEEDED( it->get_Name( bstrMXDName ) );
			}
		}
	}
	else
		return SUCCEEDED( ipLayer->get_Name( bstrMXDName ));

	return FALSE;
}

CString AheGetLayerMXDName( IMapPtr ipMap, IUnknownPtr ipUnk )
{
	CComBSTR bstrLayerName = 0;

	ILayerPtr ipLayer( ipUnk );
	if( ipLayer == NULL )
		ipLayer	= AheGetLayerByTableName( ipMap, AheGetFeatureClassName( ipUnk ) );

	if( ipLayer != NULL )
		ipLayer->get_Name( &bstrLayerName );

	return CString( bstrLayerName );
}

BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrName, FINDTYPE type )
{
	switch( type ) {
		case _FIND_TABLE: // �e�[�u�������擾
			return AheGetLayerTableName( piMap, ipUnk, bstrName );
			break;
		case _FIND_ALIAS: // �G�C���A�X�����擾
			return AheGetLayerAliasName( piMap, ipUnk, bstrName );
			break;
		case _FIND_MXDNAME: // MXD�t�@�C���ɕۑ�����Ă��郌�C�������擾
			return AheGetLayerMXDName( piMap, ipUnk, bstrName );
			break;
		default:
			return FALSE;
			break;
	}

	return FALSE;
}

BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, LPCTSTR& lpszName, FINDTYPE type )
{
	BSTR bstrName = NULL;

	USES_CONVERSION;

	if( AheGetLayerName( piMap, ipUnk, &bstrName, type ) == FALSE ) return FALSE;
	lpszName = OLE2T( bstrName );

	return TRUE;
}

CString AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, FINDTYPE type )
{
	BSTR bstrName = NULL;

	USES_CONVERSION;

	if( AheGetLayerName( piMap, ipUnk, &bstrName, type ) == FALSE ) return CString(_T(""));

	return CString(OLE2T( bstrName ));
}

//////////////////////////////////////////////////////////////////////
//
// ���C���E�e�[�u���E�t�B�[�`���N���X�擾�Ɋւ���֐�
//
//////////////////////////////////////////////////////////////////////
ILayerPtr AheGetLayerByTableName( IMapPtr ipMap, LPCTSTR lpcszLayerName, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetLayerByName( ipMap, lpcszLayerName, _FIND_TABLE, bEditableOnly );
}
BOOL AheGetLayerByTableName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_TABLE, bEditableOnly );
}

BOOL AheGetLayerByAliasName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_ALIAS);
}

ILayerPtr AheGetLayerByAliasName( IMapPtr ipMap, LPCTSTR lpszLayerName )
{
	return AheGetLayerByName( ipMap, lpszLayerName, _FIND_ALIAS );
}

BOOL AheGetLayerByMXDName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_MXDNAME);
}

ILayerPtr AheGetLayerByMXDName( IMapPtr ipMap, LPCTSTR lpszLayerName )
{
	return AheGetLayerByName( ipMap, lpszLayerName, _FIND_MXDNAME );
}

BOOL AheGetLayerByName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, FINDTYPE type, BOOL bEditableOnly/* = FALSE*/, BOOL bFullName/* = FALSE*/ )
{
	ILayerPtr ipLayer = AheGetLayerByName( piMap, lpszLayerName, type, bEditableOnly, bFullName );
	if( ipLayer != NULL )
	{
		(*ipRetLayer) = ipLayer;
		(*ipRetLayer)->AddRef();

		return TRUE;
	}

	return FALSE;
}

ILayerPtr AheGetLayerByName( IMapPtr ipMap, LPCTSTR lpszLayerName, FINDTYPE type/* = _FIND_TABLE*/, BOOL bEditableOnly/* = FALSE*/, BOOL bFullName/* = FALSE*/ )
{
	if( ipMap == NULL || lpszLayerName == NULL )
		return NULL;

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( bEditableOnly ? LayerTypeFeatureLayer : LayerTypeAny, bEditableOnly ? true : false ); it != cLayers.end(); ++it )
	{
		// [bug 1765] �Ԃт�����ȃ��C����get_Workspace����Ɨ�����i���񍐁j�̂�
		// �����͌����郌�C���Ɍ���
		VARIANT_BOOL vaValid = VARIANT_FALSE;
		it->get_Valid( &vaValid );
		if( vaValid )
		{
			CString strLayerName;
			switch( type ) 
			{
			case _FIND_TABLE:	//!< ���C���̃e�[�u�����Ō����������ꍇ
				strLayerName = ( bFullName ) ? AheGetOwnerDotClassName( *it ) : AheGetFeatureClassName( *it );
				break;
			case _FIND_ALIAS:	//!< ���C���̃G�C���A�X���Ō����������ꍇ
				strLayerName = AheGetLayerAliasName( *it );
				break;
			case _FIND_MXDNAME:	//!< MXD�t�@�C���ɕۑ�����Ă��郌�C�����Ō����������ꍇ
				strLayerName = AheGetLayerMXDName( ipMap, *it );
				break;
			default:
				return NULL;
				break;
			}
			if( strLayerName.CompareNoCase( lpszLayerName ) == 0 ) 
				return *it;
		}
	}

	return NULL;
}

bool _AheGetSelectedLayer( ICompositeLayerPtr ipCompositeLayer, ILayer** ipSelectLayer )
{
	// �����`�F�b�N
	if( ( ! ipCompositeLayer ) || ( ! ipSelectLayer ) )
		return false;

	// �Ԃ�l������
	*ipSelectLayer = NULL;

	ILayerPtr ipRet; // �����������C��
	long lEditLayerNum = 0; // �����������C����
	long lCompositeLayerNum = 0; // �O���[�v���C�������C����

	if( SUCCEEDED( ipCompositeLayer->get_Count( &lCompositeLayerNum ) ) )
	{
		for( long i = 0; i < lCompositeLayerNum; ++i )
		{
			ILayerPtr ipLayer;
			if( SUCCEEDED( ipCompositeLayer->get_Layer( i, &ipLayer ) ) && ipLayer )
			{
				// �t�B�[�`�����C���łȂ������疳��
				// �����O���[�v���C�����������Ƃ��Ă��������Ă��܂��B
				IFeatureLayerPtr ipFeatureLayer( ipLayer );
				if( ipFeatureLayer )
				{
					// �t�B�[�`���N���X�Ƃ�Ȃ������胏�[�N�X�y�[�X�Ƃ�Ȃ������疳��
					IFeatureClassPtr ipFeatureClass;
					if( SUCCEEDED( ipFeatureLayer->get_FeatureClass( &ipFeatureClass ) ) && ipFeatureClass )
					{
						IDatasetPtr ipDataset( ipFeatureClass );
						if( ipDataset )
						{
							IWorkspacePtr ipWorkspace( AheGetWorkspace( ipDataset ) );
							if( ipWorkspace )
							{
								// �ҏW�J�n����Ă��郏�[�N�X�y�[�X���ǂ������ׂ�
								VARIANT_BOOL vbIsBeginingEdit = VARIANT_FALSE;
								if( SUCCEEDED( ((IWorkspaceEditPtr)ipWorkspace)->IsBeingEdited( &vbIsBeginingEdit ) ) && vbIsBeginingEdit )
								{
									// �ҏW���邱�Ƃ��ł���f�[�^�Z�b�g�����ׂ�
									VARIANT_BOOL vbIsCanEdit = VARIANT_FALSE;
									IDatasetEditInfoPtr ipDatasetEditInfo( ipDataset );
									if( ipDatasetEditInfo )
									{
										if( SUCCEEDED( ipDatasetEditInfo->get_CanEdit( &vbIsCanEdit ) ) && vbIsCanEdit )
										{
											// �ҏW�J�n����Ă��郏�[�N�X�y�[�X���A�ҏW�\���C����
											// ����ꍇ�ɂ̂ݕҏW���ł��邱�Ƃɂ���
											if( vbIsBeginingEdit && vbIsCanEdit )
											{
												// �����\�[�X�̃��C����������A�������������Ƃɂ��Ȃ��ibug 5170�j
												bool bFindSameSourceLayer = false;
												if( 0 < lEditLayerNum )
												{
													IFeatureClassPtr ipFeatureClassTmp;
													((IFeatureLayerPtr)ipRet)->get_FeatureClass( &ipFeatureClassTmp );
													if( (IUnknown*)ipFeatureClass == (IUnknown*)ipFeatureClassTmp )
														bFindSameSourceLayer = true;
												}
												if( ! bFindSameSourceLayer )
												{
													++lEditLayerNum;
													ipRet = ipLayer;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			// ��������ꍇ�ɂ͒��f
			if( 1 < lEditLayerNum )
				break;
		}
	}
	// �Y��������̂���݂̂̏ꍇ�̂݃��C����Ԃ�
	if( 1 == lEditLayerNum )
	{
		*ipSelectLayer = ipRet;
		if( *ipSelectLayer )
			(*ipSelectLayer)->AddRef();
	}
	
	return ( 1 == lEditLayerNum ) ? true : false;
}

BOOL AheGetSelectedLayer( IMxDocumentPtr piMxDoc, ILayer** ipRetLayer, bool bIsEditable /* = true */)
{
	IContentsViewPtr piContentsView;
	if( SUCCEEDED( piMxDoc->get_ContentsView(0, &piContentsView)))
	{
		CComVariant vTmp;
		if( SUCCEEDED( piContentsView->get_SelectedItem( &vTmp)))
		{
			if( VT_UNKNOWN == vTmp.vt)
			{
				ILayerPtr ipLayer;
				if( SUCCEEDED( vTmp.punkVal->QueryInterface(&ipLayer)))
				{
					if( bIsEditable )
					{
						// �O���[�v���C���I������Ă���ꍇ�ɂ́A���̂Ȃ��̕ҏW�J�n����Ă��郌�C�����擾���Ă���
						// ���̃O���[�v���C���̒��ɕ����̕ҏW�\���C��������ꍇ�ɂ́A�I�����Ă���O���[�v���C��ipLayer��Ԃ����Ƃɂ��܂��B
						ICompositeLayerPtr ipCompositeLayer( ipLayer );
						if( ipCompositeLayer != NULL )
							_AheGetSelectedLayer( ipCompositeLayer, ipRetLayer );
					}

					if( *ipRetLayer == NULL )
					{
						( *ipRetLayer) = ipLayer;
						( *ipRetLayer)->AddRef();
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// AheGetTableByTableName�A_AhGetTableByTableNameFromCompositeLayer���ʂŎg�p����T�u���[�`���i����J�j
void _AheGetTableByTableName( ITablePtr& ipTable, ITable* ipTableTmp, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly )
{
	// �����`�F�b�N
	if( ipTableTmp == NULL ) return;

	if( _AheIsSameTableName( AheGetOwnerTableName( ipTableTmp ), lpcszOwnerTableName ) )
	{
		// �ҏW�\���ǂ������`�F�b�N
		if( ! bEditableOnly || AheIsEditableTable( ipTableTmp ) )
		{
			if( ipTable == NULL )
			{
				ipTable = ipTableTmp;
				++nFindCount;
			}
			else {
				// ���ɂ���ꍇ�͓������ǂ����`�F�b�N
				if( ! AheIsEqualTable( ipTable, ipTableTmp ) )
					++nFindCount;	// �J�E���^�̂ݑ����A�Ԃ��e�[�u���͍ŏ��Ɍ��������̂̂�
			}
		}
	}
}

// �O���[�v���C���̒�����e�[�u�����擾����i����J�j
ITablePtr _AhGetTableByTableNameFromCompositeLayer( ICompositeLayer* ipCompLayer, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true )
{
	// ������
	nFindCount = 0;

	ITablePtr ipTable;
	if( ipCompLayer != NULL && lpcszOwnerTableName != NULL )
	{
		long lCount = 0;
		ipCompLayer->get_Count( &lCount );
		for( long i = 0; i < lCount; ++i )
		{
			ILayerPtr ipLayer;
			VARIANT_BOOL vbIsValid = VARIANT_FALSE;

			ipCompLayer->get_Layer( i, &ipLayer );
			if( ipLayer != NULL )
			{
				// [bug 1765] �Ԃт�����ȃ��C����get_Workspace����Ɨ�����i���񍐁j�̂�
				// �����͌����郌�C���Ɍ���
				ipLayer->get_Valid( &vbIsValid );
				if( vbIsValid )
				{
					if( (ICompositeLayerPtr)ipLayer != NULL )
					{
						int nFindCountTmp = 0;
						ITablePtr ipTableTmp = _AhGetTableByTableNameFromCompositeLayer( (ICompositeLayerPtr)ipLayer, lpcszOwnerTableName, nFindCountTmp, bEditableOnly );
						if( ipTable == NULL )
						{
							ipTable = ipTableTmp;
							nFindCount = nFindCountTmp;
						}
						else if( ! AheIsEqualTable( ipTable, ipTableTmp ) )
							nFindCount += nFindCountTmp;
					}
					else {
						if( AheIsEqualTable( ipTable, AheLayer2Table( ipLayer ) ) )
							++nFindCount;
					}

					// ���ʃT�u���[�`��
					_AheGetTableByTableName( ipTable, AheLayer2Table( ipLayer ), lpcszOwnerTableName, nFindCount, bEditableOnly );
				}
			}
		}
	}
	else
		_ASSERTE( ipCompLayer != NULL && lpcszOwnerTableName != NULL );
		
	return ipTable;
}

// �f�[�^�t���[���ɓǂݍ��܂�Ă���e�[�u���E�t�B�[�`���N���X���擾����
ITablePtr AheGetTableByTableName2( IMap* ipMap, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly/* = true*/ )
{
	// ������
	nFindCount = 0;
	ITablePtr ipTable;

	if( ipMap != NULL && lpcszOwnerTableName != NULL )
	{
		long lCount = 0;
		ipMap->get_LayerCount( &lCount );
		for( long i = 0; i < lCount; ++i )
		{
			ILayerPtr ipLayer;
			VARIANT_BOOL vbIsValid = VARIANT_FALSE;

			ipMap->get_Layer( i, &ipLayer );
			if( ipLayer != NULL )
			{
				// [bug 1765] �Ԃт�����ȃ��C����get_Workspace����Ɨ�����i���񍐁j�̂�
				// �����͌����郌�C���Ɍ���
				ipLayer->get_Valid( &vbIsValid );
				if( vbIsValid )
				{
					if( (ICompositeLayerPtr)ipLayer != NULL )
					{
						int nFindCountTmp = 0;
						ITablePtr ipTableTmp = _AhGetTableByTableNameFromCompositeLayer( (ICompositeLayerPtr)ipLayer, lpcszOwnerTableName, nFindCountTmp, bEditableOnly );
					}
					else
						// ���ʃT�u���[�`��
						_AheGetTableByTableName( ipTable, AheLayer2Table(ipLayer), lpcszOwnerTableName, nFindCount, bEditableOnly );
				}
			}
		}
	}
	else
		_ASSERTE( ipMap != NULL && lpcszOwnerTableName != NULL );

	return ipTable;
}

BOOL AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, ITable** ipRetTable, BOOL bReference/* = FALSE*/, BOOL bEditableOnly /* = FALSE */ )
{
	if( ipMap == NULL || lpszTableName == NULL || ipRetTable == NULL ) return FALSE;

	ITablePtr ipTable = AheGetTableByTableName( ipMap, lpszTableName, bReference, bEditableOnly );
	if( ipTable != NULL )
	{
		*ipRetTable = ipTable;
		(*ipRetTable)->AddRef();

		return TRUE;
	}

	return FALSE;
}
BOOL _AheIsAlreadyCheckedWorkspace( IWorkspacePtr ipWorkspace, const std::list<CString>& listCheck, CString& strDBString )
{
	USES_CONVERSION;

	if( ipWorkspace == NULL ) return TRUE;
	if( listCheck.size() < 1 ) return FALSE;

	CComBSTR bstrWorkspacePathName;

	// ���ɒ��ׂ����[�N�X�y�[�X���ǂ����`�F�b�N
	ipWorkspace->get_PathName( &bstrWorkspacePathName );			// SDE ����Ȃ��ꍇ
	if( bstrWorkspacePathName.Length() == 0 )
	{
		IPropertySetPtr ipConnProp;
		ipWorkspace->get_ConnectionProperties( &ipConnProp );
		if( ipConnProp != NULL )
		{
			// �R�l�N�V�����v���p�e�B�ŃT�[�o����������̂��̗p
			CComVariant vaServer, vaUser, vaVersion;
			ipConnProp->GetProperty( CComBSTR(_T("SERVER")), &vaServer );
			ipConnProp->GetProperty( CComBSTR(_T("USER")), &vaUser );
			ipConnProp->GetProperty( CComBSTR(_T("VERSION")), &vaVersion );

			if( vaServer.vt != VT_BSTR ) { vaServer.vt = VT_BSTR; vaServer.bstrVal = ::SysAllocString( L"NODEFINE" ); }
			if( vaUser.vt != VT_BSTR) {  vaUser.vt = VT_BSTR; vaUser.bstrVal = ::SysAllocString( L"NODEFINE" ); }
			if( vaVersion.vt != VT_BSTR) {  vaVersion.vt = VT_BSTR; vaVersion.bstrVal = ::SysAllocString( L"NODEFINE" ); }

			strDBString.Format(_T("%s:%s:%s"), OLE2T(vaServer.bstrVal), OLE2T(vaUser.bstrVal), OLE2T(vaVersion.bstrVal) );
		}
	}
	else
		strDBString = bstrWorkspacePathName;

	if( ! strDBString.IsEmpty() )
	{
		BOOL bAlreadyCheckedWorkspace = FALSE;
		for( std::list<CString>::const_iterator it = listCheck.begin(); it != listCheck.end(); it++ )
		{
			if( it->CompareNoCase( strDBString ) == 0 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

ITablePtr AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, BOOL bReference/* = FALSE*/, BOOL bEditableOnly/* = FALSE */ )
{
	if( ipMap == NULL || lpszTableName == NULL )
		return NULL;

	ITablePtr ipTable;
	std::list<CString> listWorkspacePathName;

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( bEditableOnly ? LayerTypeFeatureLayer : LayerTypeAny, bEditableOnly ? true : false ); it != cLayers.end(); ++it )
	{
		if( AheGetFeatureClassName( *it ).CompareNoCase( lpszTableName ) != 0 )
		{
			IWorkspacePtr ipWorkspace( AheGetWorkspace( *it ) );

			if( ! ipWorkspace )
				continue;

			
			// ���ɒ��ׂ����[�N�X�y�[�X���ǂ����`�F�b�N
			CString strDBString;
			if( ! _AheIsAlreadyCheckedWorkspace( ipWorkspace, listWorkspacePathName, strDBString ) )
			{
				ipTable = AheOpenTableByTableName( ipWorkspace, lpszTableName, bReference );
				if( ipTable != NULL )
					return ipTable;
				else
					listWorkspacePathName.push_back( strDBString );
			}
		}
		else
			return AheGetFeatureClass( *it );
	}

	return NULL;
}

ITablePtr AheOpenTableByTableName( IWorkspacePtr ipWorkspace, LPCTSTR lpszTableName, BOOL bReference/* = FALSE*/ )
{
	// �����`�F�b�N
	if( ipWorkspace == NULL || lpszTableName == NULL ) 
	{
		_ASSERTE( ipWorkspace != NULL && lpszTableName != NULL );
		return NULL;
	}
	IFeatureWorkspacePtr ipFeatWorkspace( ipWorkspace );
	if( ipFeatWorkspace == NULL )
	{
		_ASSERTE( ipFeatWorkspace != NULL );
		return NULL;
	}

	// �e�[�u��������
	IDatabaseConnectionInfoPtr ipDBConInfo( ipWorkspace );
	CComBSTR bstrTableName;

	if( bReference == FALSE || IDatabaseConnectionInfoPtr( ipWorkspace ) == NULL )
		bstrTableName = lpszTableName;
	else {
		CComBSTR bstrRefTableName( _T("REFERENCE.") );
		if( FAILED( bstrRefTableName.Append( lpszTableName ) ) )
			_ASSERTE(false);
		bstrTableName = bstrRefTableName;
	}

	// �e�[�u���I�[�v��
	ITablePtr ipTable;
	if( FAILED( ipFeatWorkspace->OpenTable( bstrTableName, &ipTable ) ) )
		AheTraceErrorMessage( _T("AheOpenTableByTableName") );

	return ipTable;
}

// �����I�[�i�[�̃e�[�u�����I�[�v������
ITablePtr AheOpenSameOwnerTable( IWorkspace* ipWork, ITable* ipTable, LPCTSTR lpcszTableName )
{
	ITablePtr ipRet; // �Ԃ�l

	// �����`�F�b�N
	if( ! ( ipTable && lpcszTableName ) )
		return ipRet;

	CString strTableName(lpcszTableName);

	// �I�[�i�[���t���̃e�[�u�����̏ꍇ
	if( _tcschr( lpcszTableName, _T('.') ) )
	{
		// �I�[�i�[�������
		int nPeriodIndex = strTableName.Find( _T('.'), 0 );
		strTableName = strTableName.Right( strTableName.GetLength() - nPeriodIndex - 1 );
	}

	CString strOwnerName( AheGetFeatureClassOwnerName( ipTable ) );
	if( ! strOwnerName.IsEmpty() )
		return AheOpenTableByTableName( ipWork, strOwnerName + _T(".") + strTableName );
	else
		return AheOpenTableByTableName( ipWork, strTableName );
}

ITablePtr AheOpenSameOwnerTable( ITable* ipTable, LPCTSTR lpcszTableName )
{
	return AheOpenSameOwnerTable( AheGetWorkspace( ipTable ), ipTable, lpcszTableName );
}

ITablePtr AheGetReferenceTable( IMapPtr ipMap, LPCTSTR lpcszTableName )
{
	return AheGetTableByTableName( ipMap, lpcszTableName, TRUE );
}

BOOL AheGetFeatureClass( IMapPtr/* piMap*/, IUnknownPtr ipUnk, IFeatureClass** ipFeatureClass )
{
	IFeatureClassPtr ipRet( AheGetFeatureClass( ipUnk ) );
	if( ipRet != NULL )
	{
		(*ipFeatureClass) = ipRet;
		(*ipFeatureClass)->AddRef();
		return TRUE;
	}
	return FALSE;
}

ITablePtr AheGetTable( IUnknownPtr ipUnk )
{
	ITablePtr ipRet; // �߂�l

	// FeatureLayer��ITable�̃C���^�[�t�F�[�X�������Ă���̂�
	// ��ɔ��ʂ���K�v������
	IFeatureLayerPtr ipLayer( ipUnk );
	if( ipLayer )
	{
		IFeatureClassPtr ipClass;
		ipLayer->get_FeatureClass( &ipClass );
		ipRet = ipClass;
	}
	else {
		// �s����AITable �I�u�W�F�N�g�����̂܂ܗ��邱�Ƃ�����
		ipRet = ipUnk;
		if( ! ipRet )
		{
			_IRowPtr ipRow( ipUnk );

			// ipUnk == IRow �̏ꍇ
			if( ipRow ) 
				ipRow->get_Table( &ipRet );
		}
	}

	return ipRet;
}

//} // sindy
