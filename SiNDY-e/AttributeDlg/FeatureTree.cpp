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

/*! \file FeatureTree.cpp
\brief CFeatureTree �N���X�̃C���v�������e�[�V����
\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
\date 2002-02-01
*/
#include "stdafx.h"
#include "FeatureTree.h"
//#include "resource.h"       // ���C�� �V���{��

// �R���X�g���N�^
CFeatureTree::CFeatureTree()
{
	m_pFeatureDefList = NULL;
	m_pLQDefList = NULL;
	m_arrayHilitFeatures.CreateInstance( CLSID_Set );
	m_bIsLineHilight = FALSE;
	m_bIsVertexHilight = FALSE;	
	m_bIsLQAttribute = false;
	m_bIsRButtonClick = false;
}

// �f�X�g���N�^
CFeatureTree::~CFeatureTree()
{
	Clear();
}

// ArcHelper���Z�b�g���ď�����
void CFeatureTree::Init( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );
	m_cArcHelper.Init( (IApplicationPtr)ipApp );
}

// �n�C���C�g�폜�p
void CFeatureTree::Clear()
{
	m_arrayHilitFeatures->RemoveAll();
	m_ipHilitPoint = NULL;
	m_pFeatureDefList = NULL;
	m_pLQDefList = NULL;
	m_ipHilightGeometry = NULL;
}

// �t�B�[�`����`�N���X�̃��X�g���c���[�ɃZ�b�g����
void CFeatureTree::SetFeatures( std::list<CFeatureDef>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	m_pFeatureDefList = pFeatureDefList;
	m_bIsLQAttribute = false;

	Add( m_pFeatureDefList );
}

// �����N���`�N���X�̃��X�g���c���[�ɃZ�b�g����
void CFeatureTree::SetLQs( std::list<CLQRowDef>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	m_pLQDefList = pFeatureDefList;
	m_bIsLQAttribute = true;

	AddLQ( m_pLQDefList );
}

// �c���[�ŕ\������Ă�����̂�_ISet�ɂ���ĕԂ�
void CFeatureTree::GetSelectedFeatures( _ISet** ipSet )
{
	long lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;
	GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );
	
	if( lFeatureIndex < 0 ) 
		return;

	_ISetPtr ipRetSet( CLSID_Set );
	if( !m_bIsLQAttribute )
	{
		std::list<CFeatureDef>::iterator itFeatDefList = m_pFeatureDefList->begin();
		if( m_pFeatureDefList->size() - 1 < (UINT)lFeatureIndex ) 
		{
			(*ipSet) = ipRetSet;
			(*ipSet)->AddRef();
			return;
		}
		advance( itFeatDefList, lFeatureIndex );
		
		if( lFeatureIndex >= 0 && ( lTableIndex == -1 && lRowIndex == -1 ) )
			ipRetSet->Add( itFeatDefList->GetFeature() );
		else if( lTableIndex >= 0 && lRowIndex >= 0 ) 
		{
			std::list<CRelationalTableDef>* pRelTableDefList = itFeatDefList->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, lTableIndex );
			
			std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
			advance( itRowDefList, lRowIndex );
			
			ipRetSet->Add( itRowDefList->GetRow() );
		}
		(*ipSet) = ipRetSet;
		(*ipSet)->AddRef();
	}
	else
	{
		std::list<CLQRowDef>::iterator itFeatDefList = m_pLQDefList->begin();
		if( m_pLQDefList->size() - 1 < (UINT)lFeatureIndex ) 
		{
			(*ipSet) = ipRetSet;
			(*ipSet)->AddRef();
			return;
		}
		advance( itFeatDefList, lFeatureIndex );
		
		if( lFeatureIndex >= 0 && ( lTableIndex == -1 && lRowIndex == -1 ) )
			ipRetSet->Add( itFeatDefList->GetFeature() );
		else if( lTableIndex >= 0 && lRowIndex >= 0 ) 
		{
			std::list<CRelationalTableDef>* pRelTableDefList = itFeatDefList->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, lTableIndex );
			
			std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
			advance( itRowDefList, lRowIndex );
			
			ipRetSet->Add( itRowDefList->GetRow() );
		}
		(*ipSet) = ipRetSet;
		(*ipSet)->AddRef();
	}
}

// ���ݑI������Ă���A�C�e������C���f�b�N�X�ԍ����擾����
void CFeatureTree::GetSelectedFeatures( long& lFeatureIndex, long& lTableIndex, long& lRowIndex )
{
	if( !::IsWindow( m_hWnd ) )
		return;
	
	HTREEITEM hSelectedItem = GetSelectedItem();
	if( !hSelectedItem )
		return;
	DWORD dwSelected = GetItemData( hSelectedItem );
	
	// �t�B�[�`��
	if( dwSelected & 0x4000000 )
	{
		// �t�B�[�`���̃C���f�b�N�X�ԍ��擾
		lFeatureIndex = (LONG)(dwSelected ^ 0x4000000);
		lTableIndex = lRowIndex = -1;
	}
	// ���R�[�h
	else if( dwSelected & 0x1000000 )
	{
		// �e�[�u���̃C���f�b�N�X�ԍ��擾
		HTREEITEM hTableItem = GetParentItem( hSelectedItem );
		ATLASSERT( ( GetItemData( hTableItem ) & 0x2000000 ) != 0x0 );
		lTableIndex = (LONG)( GetItemData( hTableItem ) ^ 0x2000000 );
		// �t�B�[�`���̃C���f�b�N�X�ԍ��擾
		HTREEITEM hFeatItem = GetParentItem( hTableItem );
		ATLASSERT( ( GetItemData( hFeatItem ) & 0x4000000 ) != 0x0 );
		lFeatureIndex = (LONG)( GetItemData( hFeatItem ) ^ 0x4000000 );
		// ���R�[�h�̃C���f�b�N�X�ԍ��擾
		lRowIndex = (LONG)( dwSelected ^ 0x1000000 );
	}
}

// �A�C�e�����c���[�ɓo�^����
HTREEITEM CFeatureTree::AddItem( bool bIsRoot, HTREEITEM lpItem, LPCTSTR lptstrNodeName )
{
	TV_INSERTSTRUCT curTreeItem;
	
	if( bIsRoot )
		curTreeItem.hParent = NULL;
	else
		curTreeItem.hParent = lpItem;

	curTreeItem.hInsertAfter = TVI_LAST;
	curTreeItem.item.pszText = (LPTSTR)lptstrNodeName;
	curTreeItem.item.mask = TVIF_TEXT;
	
	HTREEITEM htItem = InsertItem(&curTreeItem);
	
	return htItem;
}

// �O���e�[�u�����c���[�ɓo�^����
void CFeatureTree::AddOuterTable( HTREEITEM& item, CRelationalTableDef* pRelTableDef, LONG lTableIndex )
{
	if( !pRelTableDef )
	{
		item = NULL;
		return;
	}

	// �O���e�[�u������o�^
	HTREEITEM itemTable = AddItem( NULL, item, pRelTableDef->GetTableName() );
	SetItemData( itemTable, (DWORD)0x2000000 | (DWORD)lTableIndex );
	
	LONG lRowCount = 0;
	for( std::list<CRowDef>::iterator it = pRelTableDef->begin(); it != pRelTableDef->end(); it++, lRowCount++ )
	{
		// �O�����R�[�h�̕\���ɂ� OBJECTID ���g�p
		CString strOID;
		strOID.Format( _T("%d"), it->GetRowOID() );
		HTREEITEM itemRow = AddItem( FALSE, itemTable, strOID );
		SetItemData( itemRow, (DWORD)0x1000000 | (DWORD)lRowCount );
	}
}

// �t�B�[�`�����c���[�ɓo�^����
void CFeatureTree::Add( std::list<CFeatureDef>* pFeatureDef )
{
	USES_CONVERSION;
	
	LONG lFeatCount = 0;
	BOOL bIsOver = FALSE;
	LONG lAddItemCount = 0;
	for( std::list<CFeatureDef>::iterator it = pFeatureDef->begin(); it != pFeatureDef->end(); it++, lFeatCount++ )
	{
		HTREEITEM pLayerNode = NULL, pFeatureNode = NULL, pRowNode = NULL, pVertexNode = NULL;
		IFeaturePtr ipFeature = it->GetFeature();
		
		// �t�B�[�`����TOC�ɕ\������Ă��郌�C�������擾
		CComBSTR bstrLayerName;
		GetLayerName( ipFeature, bstrLayerName );
		
		// OID���擾
		long lObjectID = -1;
		ipFeature->get_OID( &lObjectID );
		CString strFieldValue;
		strFieldValue.Format(_T("%d"), lObjectID );
		
		// ���ɓo�^����Ă��郌�C�������ǂ����`�F�b�N
		pLayerNode = SearchLayerName( bstrLayerName );
		// �܂��o�^����Ă��Ȃ����C�����̏ꍇ�͐V�K�Ƀ��C����ǉ�����
		if( !pLayerNode ) 
		{
			pLayerNode = AddItem( TRUE, NULL, OLE2T(bstrLayerName) );                // �e��o�^
			lAddItemCount++;
			SetItemData( pLayerNode, (DWORD)0x8000000 );                                        // ���C����HSB1�r�b�g�ڂ�ON
		}
		pFeatureNode = AddItem( FALSE, pLayerNode, strFieldValue );                // �q���Ƃ��ēo�^
		lAddItemCount++;

		// �A�C�e���̃f�[�^�Ƃ��ăt�B�[�`�����X�g�̃C���f�b�N�X�ԍ���ێ�
		SetItemData( pFeatureNode, (DWORD)0x4000000 | (DWORD)lFeatCount );        // �t�B�[�`����HSB2�r�b�g�ڂ�ON
		CComBSTR bstrTmp;
		GetItemText( pLayerNode, (BSTR&)bstrTmp );
		
		// �I����Ԃɂ��邩�ǂ���
		if(  lFeatCount == 0 )
			SelectItem( pFeatureNode );
		
		// ���_���c���[�ɒǉ�
		HTREEITEM pVertex;
		LONG lCount = 0;
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		IPointCollectionPtr ipCol( ipGeom );
		if( !ipCol ) {
			IPointPtr ipGeomPoint;
			ipGeomPoint = ipGeom; 
			if( ipGeomPoint == NULL )
				return;

			CString strVertex;
			strVertex.Format( _T("%d�F%s"), 0, getXYZAsString(ipGeomPoint));
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			lAddItemCount++;
			SetItemData( pVertexNode, (DWORD)0x800000 | (DWORD)lFeatCount );
			pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
			lAddItemCount++;
			// �A�C�e���̃f�[�^�Ƃ��č\���_�̃C���f�b�N�X�ԍ���ێ��i�t�B�[�`���̏ꍇ�Ƌ�ʂ��邽�߁j
			SetItemData( pVertex, (DWORD)0x400000 );
		} else {	
			// �O���e�[�u����o�^����
			std::list<CRelationalTableDef>* pRelTableDefList = it->GetRelationalTableDefList();
			LONG lTableIndex = 0;
			for( std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin(); itRelTableDefList != pRelTableDefList->end(); itRelTableDefList++, lTableIndex++ )
			{
				AddOuterTable( pFeatureNode, &(*itRelTableDefList), lTableIndex );
			}
			ipCol->get_PointCount( &lCount );
						
			if( !bIsOver )
			{
				if( lAddItemCount + lCount > 10000 )
				{
					AheMessageBox( NULL, AheLoadString( IDS_TOOMENYITEM ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL );
					bIsOver = TRUE;
				}
			}
			if( !bIsOver )
			{
				LONG        lVertexMax = 2000;
				if(lCount < lVertexMax)
				{
					pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
					lAddItemCount++;
					SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
					for( LONG i = 0; i < lCount; i++ ) {
						IPointPtr ipPoint;
						ipCol->get_Point( i, &ipPoint );

						CString strVertex;
						strVertex.Format(_T("%d�F%s"), i, getXYZAsString(ipPoint));
						pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
						lAddItemCount++;
						// �A�C�e���̃f�[�^�Ƃ��č\���_�̃C���f�b�N�X�ԍ���ێ��i�t�B�[�`���̏ꍇ�Ƌ�ʂ��邽�߁j
						SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
					}
				}
				else{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CHECKDISPMENYVERTEX ) ,OLE2T(bstrLayerName),lObjectID,lCount,lVertexMax);
					int iRet = AheMessageBox( NULL, strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO|MB_TASKMODAL|MB_DEFBUTTON2 );
					
					if( iRet == IDYES )
					{
						pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
						lAddItemCount++;
						SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
						for( LONG i = 0; i < lCount; i++ ) {
							IPointPtr ipPoint = NULL;
							ipCol->get_Point( i, &ipPoint );

							CString strVertex;
							strVertex.Format(_T("%d�F%s"), i, getXYZAsString(ipPoint));
							pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
							lAddItemCount++;
							// �A�C�e���̃f�[�^�Ƃ��č\���_�̃C���f�b�N�X�ԍ���ێ��i�t�B�[�`���̏ꍇ�Ƌ�ʂ��邽�߁j
							SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
						}
					}
				}
			}
		}
	}
    // �`��i��ԍŏ��̎������ׂ̈Ɂc�j
    HilitSelectedFeatures();
}

// �����N����c���[�ɓo�^����
void CFeatureTree::AddLQ( std::list<CLQRowDef>* pFeatureDef )
{
	if( !pFeatureDef )
		return;

	CString strFieldValue;
	HTREEITEM hSelected = NULL;

	USES_CONVERSION;

	LONG lFeatCount = 0;
	for( std::list<CLQRowDef>::iterator it = pFeatureDef->begin(); it != pFeatureDef->end(); it++, lFeatCount++ )
	{
		HTREEITEM pLayerNode = NULL, pFeatureNode = NULL, pRowNode = NULL, pVertexNode = NULL;
		_IRowPtr ipRow = it->GetFeature();

		if( !ipRow )
		{
			ATLASSERT( ipRow != NULL );
			return;
		}

		long lOID = -1;
		ipRow->get_OID( &lOID );
		strFieldValue.Format(_T("%d"), lOID );

		// �����N��̃I�[�i����\�����Ȃ��Ă͂����Ȃ�����
		CString strLayerName = AheGetFeatureClassOwnerName( ipRow );
		if( strLayerName.IsEmpty() == FALSE )
			strLayerName += _T(".");
		strLayerName += it->GetDispName();

		// ���ɓo�^����Ă��郌�C�������ǂ����`�F�b�N
		pLayerNode = SearchLayerName( (LPCTSTR)strLayerName );
		// �܂��o�^����Ă��Ȃ����C�����̏ꍇ�͐V�K�Ƀ��C����ǉ�����
		if( !pLayerNode ) 
		{
			pLayerNode = AddItem( TRUE, NULL, strLayerName );		// �e��o�^
			SetItemData( pLayerNode, (DWORD)0x8000000 );					// ���C����HSB1�r�b�g�ڂ�ON
		}
		pFeatureNode = AddItem( FALSE, pLayerNode, strFieldValue );		// �q���Ƃ��ēo�^
		// �A�C�e���̃f�[�^�Ƃ��ăt�B�[�`�����X�g�̃C���f�b�N�X�ԍ���ێ�
		SetItemData( pFeatureNode, (DWORD)0x4000000 | (DWORD)lFeatCount );	// �t�B�[�`����HSB2�r�b�g�ڂ�ON

		// �I����Ԃɂ��邩�ǂ���
		if(  lFeatCount == 0 || it->GetDefault() )
		{
			SelectItem( pFeatureNode );
			hSelected = pFeatureNode;
		}

		// ���_���c���[�ɒǉ�
		HTREEITEM pVertex;
		LONG lCount = 0;
		IGeometryPtr ipGeom = it->GetShape();
		IPointCollectionPtr ipCol( ipGeom );
		if( ipCol == NULL ) {
			IPointPtr ipGeomPoint;
			ipGeomPoint = ipGeom; 
			if( !ipGeomPoint )
				return;
			CString strVertex;
			DOUBLE x = 0.0, y = 0.0;
			ipGeomPoint->QueryCoords( &x, &y );
			char szXPos[ 128];
			_gcvt( x, 100, szXPos);
			char szYPos[ 128];
			_gcvt( y, 100, szYPos);
			USES_CONVERSION;
			strVertex.Format( _T("%d�FX=%s Y=%s"), 0, A2T( szXPos), A2T( szYPos));
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			SetItemData( pVertexNode, (DWORD)0x800000 | (DWORD)lFeatCount );
			pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
			// �A�C�e���̃f�[�^�Ƃ��č\���_�̃C���f�b�N�X�ԍ���ێ��i�t�B�[�`���̏ꍇ�Ƌ�ʂ��邽�߁j
			SetItemData( pVertex, (DWORD)0x400000 );
		} else {
			// �O���e�[�u����o�^����
			std::list<CRelationalTableDef>* pRelTableDefList = it->GetRelationalTableDefList();
			LONG lTableIndex = 0;
			for( std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin(); itRelTableDefList != pRelTableDefList->end(); itRelTableDefList++, lTableIndex++ )
			{
				AddOuterTable( pFeatureNode, &(*itRelTableDefList), lTableIndex );
			}
			ipCol->get_PointCount( &lCount );
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
			for( LONG i = 0; i < lCount; i++ ) {
				IPointPtr ipPoint = NULL;
				ipCol->get_Point( i, &ipPoint );
				CString strVertex;
				DOUBLE x = 0.0, y = 0.0;
				ipPoint->QueryCoords( &x, &y );
				char szXPos[ 128];
				_gcvt( x, 100, szXPos);
				char szYPos[ 128];
				_gcvt( y, 100, szYPos);
				USES_CONVERSION;
				strVertex.Format( _T("%d�FX=%s Y=%s"), i, A2T( szXPos), A2T( szYPos));
				pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
				// �A�C�e���̃f�[�^�Ƃ��č\���_�̃C���f�b�N�X�ԍ���ێ��i�t�B�[�`���̏ꍇ�Ƌ�ʂ��邽�߁j
				SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
			}
		}
	}
	// �I�����ꂽ�t�B�[�`�����n�C���C�g����	
	if( hSelected )
		HilitSelectedLQ( GetLQRowDef( hSelected )->GetShape() );
}

// IFeature����TOC�ɕ\������Ă��郌�C�������擾
void CFeatureTree::GetLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName )
{
	// [Bug 5358] [�v�]] ����\�[�X�̃��C��������TOC�ɂ���Ƃ��͑I�����C�������o���ė~�����ɑΉ�
	// ���C����I�����Ă���ꍇ�ɂ́AGetSelectedLayerName���疼�O�����B
	GetSelectedLayerName(ipFeature, bstrLayerName);

	// �������Ȃ�������i�O���[�v���C���Ƃ��I�𒆂�������A�f�[�^�Z�b�g��I�����Ă����ꍇ�j����΂��ăt�B�[�`�����烌�C�������擾
	if( !bstrLayerName )
	{
		// ���ׂẴt�B�[�`�����C���Ń��[�v���܂킵�đΏۂ̃��C����������TOC�̃��C�������擾����
		std::list<CAdapt<ILayerPtr> > listLayers = AheFindLayers( m_cArcHelper.GetMap(), ipFeature );
		// ��{�I��1�̂͂�(��������ꍇ�����邪�A���̏ꍇ�͍ŏ��Ɍ��������ق��ł��܂�Ȃ�)
		for( std::list<CAdapt<ILayerPtr> >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
		{
			(it->m_T)->get_Name( &bstrLayerName );
			break;
		}
	}
}

// �I�����C�����疼�O���擾
void CFeatureTree::GetSelectedLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName)
{
	// ������NULL�ł���\���͂��邾�낤���H�ꉞ�`�F�b�N
	if( !ipFeature )
		return;
	// IFeature����t�B�[�`���N���X�擾���Ă���
	IObjectClassPtr ipClass;
	ipFeature->get_Class( &ipClass );

	// ���C���I�����ăt�B�[�`����I�����Ă���A�����ɂ���܂łɁA���C���̑I�����ς��Ȃ��Ƃ����O��
	IMxDocumentPtr ipMxDoc = m_cArcHelper.GetDocument();
	IContentsViewPtr ipContentsView;
	ipMxDoc->get_ContentsView( 0, &ipContentsView );
	// �I���A�C�e�����擾
	CComVariant vaSelectedItem;
	ipContentsView->get_SelectedItem( &vaSelectedItem );
	if( VT_UNKNOWN == vaSelectedItem.vt)
	{
		_ISetPtr ipSet( vaSelectedItem );
		// �������C���I�����Ă���Ƃ�
		if( ipSet )
		{
			IUnknownPtr ipUnk;
			ipSet->Reset();
			while( ipSet->Next( &ipUnk ) == S_OK )
			{
				IGeoFeatureLayerPtr ipGeoFeatureLayer( ipUnk );
				// �W�I�t�B�[�`�����C���̂ݖ��O�����
				if( ipGeoFeatureLayer )
				{
					IFeatureClassPtr ipFeatureClass;
					ipGeoFeatureLayer->get_FeatureClass( &ipFeatureClass );
					// �t�B�[�`���N���X�ƃ��[�N�X�y�[�X�̃|�C���^�Ŕ�r����
					if( (IUnknown*)(IUnknownPtr)ipClass == (IUnknown*)(IUnknownPtr)ipFeatureClass )
					{
						// TOC�ɕ\������Ă��郌�C�������擾����
						ipGeoFeatureLayer->get_Name( &bstrLayerName );
						break;
					}
				}
			}
		}
		// �ЂƂ������C���I�����Ă��鎞
		else 
		{
			IGeoFeatureLayerPtr ipGeoFeatureLayer( vaSelectedItem );
			// �W�I�t�B�[�`�����C���̂�
			if( ipGeoFeatureLayer )
			{
				// [Bug 5471]TOC�I������SplitTool�œ��H�����N�𕪊�����ƃ��C���\����TOC�őI���������̂����łȂ��ւ̏C��
				// �O���c�[���iSplitTool�j����Ă΂��ꍇ�ɂ́ATOC�őI�����Ă��郌�C���ȊO���\�����邱�Ƃ�����̂�Y��Ă�orz
				// �c���[�ɕ\������t�B�[�`����TOC�őI�����Ă���t�B�[�`���Ɠ����t�B�[�`���N���X���ǂ������ׂ�K�v������
				// �����łȂ�������A�Ăяo������GetLayerName�ł���΂��đΏۂ̃��C���������Ă��
				IFeatureClassPtr ipFeatureClass;
				ipGeoFeatureLayer->get_FeatureClass( &ipFeatureClass );
				// �t�B�[�`���N���X�ƃ��[�N�X�y�[�X�̃|�C���^�Ŕ�r����
				if( (IUnknown*)(IUnknownPtr)ipClass == (IUnknown*)(IUnknownPtr)ipFeatureClass )
					ipGeoFeatureLayer->get_Name( &bstrLayerName );
			}
		}
	}
}


// �������C���������A�C�e����T��
HTREEITEM CFeatureTree::SearchLayerName( CComBSTR lpcszLayerName )
{
	if( lpcszLayerName.Length() == 0) 
		return NULL;

	HTREEITEM itemFirst;
	if( !m_bIsLQAttribute )
		itemFirst = GetFirstVisibleItem();
	else
		itemFirst = GetRootItem();

	if( !itemFirst )
		return NULL;
	if( IsSameText( lpcszLayerName, itemFirst ) )
		return itemFirst;
	
	HTREEITEM itemNext = GetNextItem( itemFirst, TVGN_NEXT );
	while( itemNext != NULL ) {
		if( IsSameText( lpcszLayerName, itemNext ) )
			return itemNext;
		itemNext = GetNextItem( itemNext, TVGN_NEXT );
	}
	return NULL;
}

// ���C������OBJECTID����A�C�e����T��
HTREEITEM CFeatureTree::SearchOID( LPCTSTR lpcszLayerName, LONG lOID )
{
	HTREEITEM hLayerItem = SearchLayerName( lpcszLayerName );
	HTREEITEM hFeatureItem = GetChildItem( hLayerItem );
	LONG lObjectID = -1;

	if( !hFeatureItem ) 
		return NULL;

	CLQRowDef* pLQRowDef = GetLQRowDef( hFeatureItem );
	if( !pLQRowDef )
	{
		ATLASSERT( pLQRowDef != NULL );
		hFeatureItem = NULL;
	}
	else {
		_IRowPtr ipLQRow = pLQRowDef->GetFeature();
		ipLQRow->get_OID( &lObjectID );
		while( lOID != lObjectID )
		{
			hFeatureItem = GetNextSiblingItem( hFeatureItem );

			if( !hFeatureItem )
				break;
			ipLQRow = GetLQRowDef( hFeatureItem )->GetFeature();
			ipLQRow->get_OID( &lObjectID );
		}
	}

	return hFeatureItem;
}

// �n���ꂽ���C�����ƃc���[�̃e�L�X�g���������ǂ����̔���
BOOL CFeatureTree::IsSameText( CComBSTR bstrLayerName, HTREEITEM item )
{
	if( bstrLayerName.Length() == 0 || !item )
		return FALSE;

	USES_CONVERSION;
	
	if( !m_bIsLQAttribute )
	{
		BSTR bstrCurrentLayerName = NULL;
		
		GetItemText( item, (BSTR&)bstrCurrentLayerName );
		if( lstrcmp( OLE2T( bstrLayerName ), OLE2T( bstrCurrentLayerName ) ) == 0 )
		{
			if( bstrCurrentLayerName != NULL )
				::SysFreeString( bstrCurrentLayerName );
			return TRUE;
		}
		else
		{
			if( bstrCurrentLayerName != NULL )
				::SysFreeString( bstrCurrentLayerName );
			return FALSE;
		}
	}
	else
	{
		BSTR bstrCurrentLayerName = NULL;
		GetItemText( item, (BSTR&)bstrCurrentLayerName );
		BOOL bRet = ( lstrcmp( OLE2T( bstrLayerName ), OLE2T( bstrCurrentLayerName ) ) == 0 );
		return bRet;
	}
}

// �c���[�R���g���[����̃A�C�e���̎�ނ𔻒肷��
INT CFeatureTree::ItemKind( HTREEITEM item )
{
	if( item == NULL )
		return NOKINDS;

	DWORD dwData = GetItemData( item );
	
	// ���C��
	if( dwData & 0x8000000 )
		return LAYERITEM;
	// �t�B�[�`��
	else if( dwData & 0x4000000 )
		return FEATUREITEM;
	// �e�[�u��
	else if( dwData & 0x2000000 )
		return OUTERTABLEITEM;
	// ���R�[�h
	else if( dwData & 0x1000000 )
		return OUTERROWITEM;
	// �\���_
	else if( dwData & 0x400000 )
		return FEATUREPOINTITEM;
	
	return NOKINDS;
}

// �I������Ă��郊���N����n�C���C�g����
void CFeatureTree::HilitSelectedLQ( IGeometryPtr ipGeom,  BOOL bRefresh/* = FALSE */ )
{
	IScreenDisplayPtr ipScreen = m_cArcHelper.GetScreenDisplay();

	// �n�C���C�g�F�̐ݒ�擾
	int nRed = 0, nGreen = 0, nBlue = 0;
	_bstr_t strXPath = _T("settings/setting[@name='LQAttributeTool' and @type='polyline_main']/color");
	bool bUseMainSetting = false;
	bUseMainSetting = GetHighlightColor( strXPath, nRed, nGreen, nBlue );

	if( !bRefresh ) {
		// �Â��t�B�[�`������
		if( m_ipHilightGeometry )
			AheDrawArrowline( ipScreen, m_ipHilightGeometry, bUseMainSetting? RGB( nRed, nGreen, nBlue ) : _COLOR_LQFLASH, FALSE, TRUE );
	}

	// �n�C���C�g�`��̏I�_���擾
	IPolylinePtr ipLQLine( ipGeom );
	IPointPtr ipLQToPoint;
	ipLQLine->get_ToPoint( &ipLQToPoint );
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );

	// ��ʓ��ɏI�_���܂܂�Ă��邩�H
	IEnvelopePtr ipEnv;
	m_cArcHelper.GetActiveView()->get_Extent( &ipEnv );
	IRelationalOperatorPtr ipRel( ipLQToPoint );
	VARIANT_BOOL vaIsWithin = VARIANT_FALSE;
	ipRel->Within( ipEnv, &vaIsWithin );
	
	// �܂܂�ĂȂ��Ȃ�ʂ̐ݒ�����K�v������̂Ńn�C���C�g�F�Ď擾
	bool bUseSubSetting = false;
	int nRedSub = 0, nGreenSub = 0, nBlueSub = 0;
	if( !vaIsWithin )
	{
		strXPath = _T("settings/setting[@name='LQAttributeTool' and @type='polyline_sub']/color");
		bUseSubSetting = GetHighlightColor( strXPath, nRedSub, nGreenSub, nBlueSub );
	}

	// ��ʋ�`�Ńn�C���C�g�`����J�b�g(bug9405)
	ITopologicalOperatorPtr ipTopo( AheGetClone( ipLQLine ) );
	// Intersect���Ǝ��Ȍ���������ꍇ�Ɍ��������Ń��C�������������̂�Clip���g��
	ipTopo->Clip( ipEnv );
	
	// �n�C���C�g�`�󂪃}���`�p�[�g�ɂȂ��Ă��܂������͂ǂ����󂪏o��悤�ɂ���(bug9405)
	IGeometryCollectionPtr ipGeomCol( AheSetKnownSimple( ipTopo ) );
	if ( ipGeomCol )
	{
		long lPartCount = -1;
		ipGeomCol->get_GeometryCount( &lPartCount );

		for( int i = 0; i < lPartCount; ++i )
		{
			IGeometryCollectionPtr ipNewGeomCol( CLSID_Polyline );
			IPolylinePtr ipPolyline( ipNewGeomCol );
			ipPolyline->putref_SpatialReference( ipSpRef );

			IGeometryPtr ipPartGeom;
			ipGeomCol->get_Geometry( i, &ipPartGeom );
			
			IPathPtr ipPath( ipPartGeom );
			ipNewGeomCol->AddGeometry( ipPath );
			// �I�_����ʓ��ɂȂ���΃n�C���C�g�͉��F�A��ʓ��ɂ���ΐ��F(bug9405)
			AheDrawArrowline( ipScreen, ipPolyline, vaIsWithin? (bUseMainSetting? RGB( nRed, nGreen, nBlue ) : _COLOR_LQFLASH) : (bUseSubSetting? RGB( nRedSub, nGreenSub, nBlueSub ) : _COLOR_LQFLASH_YELLOW), FALSE, TRUE);
		}
	}

	if( !bRefresh ) {
		m_ipHilightGeometry = ipGeom; // ���X�̌`�������Ă���
		/// �S�~�������̂ɂ� esriViewForeground
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	}
	
	InvalidateRect( NULL, FALSE );
}

// �I������Ă���t�B�[�`�����n�C���C�g����
void CFeatureTree::HilitSelectedFeatures( BOOL bRefresh/* = FALSE */ )
{
	IFeaturePtr ipFeature;
	IScreenDisplayPtr ipScreen = m_cArcHelper.GetScreenDisplay();
	if( !bRefresh ) {
		// �Â��t�B�[�`������
		m_arrayHilitFeatures->Reset();
		IUnknownPtr ipUnk;
		while( m_arrayHilitFeatures->Next( &ipUnk ) == S_OK ) 
		{
			ipFeature = ipUnk;
			ATLASSERT( ipFeature != NULL );
			// ���H�����N�̕����ɖ�������
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
			{
				BOOL bFixFlow = FALSE;
				IGeometryPtr ipRoadGeom;
				ipFeature->get_ShapeCopy( &ipRoadGeom );
				AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
				// ���C���̉����n�C���C�g���邩�ǂ���
				if( m_bIsLineHilight == FALSE )
					AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
			}
			// �|�C���g�t�B�[�`���̏ꍇ
			else if( kinds == sindyePointFeature && kinds == sindyeNodeFeature)
				AheDrawFeatureXOR( ipScreen, ipFeature, 0, FALSE );
			// �܂�|���S���̏ꍇ
			else
			{
				IGeometryPtr ipGeom;
				ipFeature->get_Shape( &ipGeom );
				IPolygonPtr ipPolygon = ipGeom;
				AheDrawPolygonXOR( ipScreen, ipPolygon );
			}
		}
		
		// array ����
		m_arrayHilitFeatures->RemoveAll();
		
		// �V�����t�B�[�`���`��
		_ISetPtr ipSet;
		GetSelectedFeatures( &ipSet );
		if( !ipSet )
			return;
		
		ipSet->Reset();
		while( ipSet->Next( &ipUnk ) == S_OK ) {
			ipFeature = ipUnk;
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( ipFeature != NULL ) {
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
				{
					BOOL bFixFlow = FALSE;
					IGeometryPtr ipRoadGeom;
					ipFeature->get_ShapeCopy( &ipRoadGeom );
					AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
				}
				
				// �t�B�[�`���̎�����n�C���C�g�\��
				if( kinds != sindyePointFeature && kinds != sindyeNodeFeature)
				{
					if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
					{
						if( m_bIsLineHilight == FALSE )
							AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
					}
					// �܂�|���S���̏ꍇ
					else
					{
						IGeometryPtr ipGeom;
						ipFeature->get_Shape( &ipGeom );
						IPolygonPtr ipPolygon = ipGeom;
						AheDrawPolygonXOR( ipScreen, ipPolygon );
					}
				}

				// �\���_���n�C���C�g
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
				{
					if( !m_bIsVertexHilight )
						AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
				}
				else
					AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
				
				m_arrayHilitFeatures->Add( ipFeature );        //!< Array �ɕۑ�
			}
			ipUnk = NULL;
		}
		/// �S�~�������̂ɂ� esriViewForeground
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
	} else {
		IUnknownPtr ipUnk;
		m_arrayHilitFeatures->Reset();
		while( m_arrayHilitFeatures->Next( &ipUnk ) == S_OK ) 
		{
			ipFeature = ipUnk;
			ATLASSERT( ipFeature != NULL );
			// ���H�����N�̕����ɖ�������
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
			{
				BOOL bFixFlow = FALSE;
				IGeometryPtr ipRoadGeom;
				ipFeature->get_ShapeCopy( &ipRoadGeom );
				AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
			}
			// �t�B�[�`���̎�����n�C���C�g�\��
			if( kinds != sindyePointFeature && kinds != sindyeNodeFeature)
			{
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
				{
					if( !m_bIsLineHilight )
						AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
				}
				// �܂�|���S���̏ꍇ
				else
				{
					IGeometryPtr ipGeom;
					ipFeature->get_Shape( &ipGeom );
					IPolygonPtr ipPolygon = ipGeom;
					AheDrawPolygonXOR( ipScreen, ipPolygon );
				}
			}

			// �\���_���n�C���C�g
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
			{
				if( !m_bIsVertexHilight )
					AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
			}
			else
				AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );

			ipUnk = NULL;
		}
	}
	if( m_ipHilitPoint != NULL )
		AheDrawPoint( ipScreen, m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );
	
	InvalidateRect( NULL, FALSE );
}

// �}�E�X�{�^���̃N���b�N����������֐�
LONG CFeatureTree::OnMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick )
{
	HTREEITEM hItem;
	CPoint point;
	UINT nHF;
	
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	INT nFlags = (INT)wParam;
	hItem = HitTest(point, &nHF);
	
	if( nHF & TVHT_ONITEMBUTTON ) {
		Expand( hItem, TVE_TOGGLE );
		
		return 1;
	} else if( nHF & TVHT_ONITEMLABEL ) {
		INT iKind = ItemKind( hItem );
		// ���C���̃A�C�e���Ȃ牽�����Ȃ�
		if( iKind == FEATUREITEM ) {
			// �I����Ԃ�ύX����
			SelectItem( hItem );
			SetFocus();
			
			if( m_bIsRButtonClick )
				ShowMenu( uMsg, wParam, lParam, bHandled );

			// �I�����ꂽ�t�B�[�`�����n�C���C�g����
			HilitSelectedFeatures();
			
			// �N���b�N���ꂽ�A�C�e���̎�ނ�ʒm
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		} else if( iKind == FEATUREPOINTITEM ) {
			std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
			advance( it, (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( hItem ) ) ) ) );
			IFeaturePtr ipFeature = it->GetFeature();
			
			m_ipHilitPoint = AheGetPoint( ipFeature, (LONG)( 0x400000 ^ GetItemData( hItem ) ) );
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );
			
			// �N���b�N���ꂽ�\���_�����t�B�[�`�������ݑI������Ă���t�B�[�`���ƈႤ�ꍇ�ɂ�
			// �I���t�B�[�`����ύX����
			_ISetPtr ipSet;
			IUnknownPtr ipUnk;
			GetSelectedFeatures( &ipSet );
			if( ipSet )
			{
				ipSet->Reset();
				ipSet->Next( &ipUnk );
			}
			IFeaturePtr ipSelectedFeature = ipUnk;
			
			if( ipFeature != ipSelectedFeature )
			{
				SelectItem( GetParentItem( GetParentItem( hItem ) ) );
				SetFocus();
				
				// �I�����ꂽ�t�B�[�`���̃n�C���C�g
				HilitSelectedFeatures();
				// �t�B�[�`���̑I�����ύX���ꂽ���Ƃ�ʒm
				return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
			}
			
			if( bDblClick )
				m_cArcHelper.SetCenter( m_ipHilitPoint );
			// �S�~�������̂ɂ� esriViewForeground
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		} else if( iKind == OUTERROWITEM ) {
			// �I����Ԃ�ύX����
			SelectItem( hItem );
			SetFocus();
			
			// �N���b�N���ꂽ�A�C�e���̎�ނ�ʒm
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		}
	}
	return 0;
}

// �����N���p�}�E�X�{�^���̃N���b�N����������
LONG CFeatureTree::OnLQMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick )
{
	HTREEITEM hItem;
	CPoint point;
	UINT nHF;

	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	INT nFlags = (INT)wParam;
	hItem = HitTest(point, &nHF);

	if( nHF & TVHT_ONITEMBUTTON ) {
		Expand( hItem, TVE_TOGGLE );

		return 1;
	} else if( nHF & TVHT_ONITEMLABEL ) {
		INT iKind = ItemKind( hItem );
		// ���C���̃A�C�e���Ȃ牽�����Ȃ�
		if( iKind == FEATUREITEM ) {
			// �I����Ԃ�ύX����
			SelectItem( hItem );
			SetFocus();

			if( m_bIsRButtonClick )
				ShowMenu( uMsg, wParam, lParam, bHandled );
			
			m_ipHilitPoint = NULL;
			// �I�����ꂽ�t�B�[�`�����n�C���C�g����
			HilitSelectedLQ(GetLQRowDef( hItem )->GetShape());
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
			
			// �N���b�N���ꂽ�A�C�e���̎�ނ�ʒm
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		} else if( iKind == FEATUREPOINTITEM ) {
			std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
			advance( it, (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( hItem ) ) ) ) );
			
			((IPointCollectionPtr)it->GetShape())->get_Point( (LONG)( 0x400000 ^ GetItemData( hItem ) ), &m_ipHilitPoint );
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );

			// �N���b�N���ꂽ�\���_�����t�B�[�`�������ݑI������Ă���t�B�[�`���ƈႤ�ꍇ�ɂ�
			// �I���t�B�[�`����ύX����
			_ISetPtr ipSet;
			IUnknownPtr ipUnk;
			GetSelectedFeatures( &ipSet );

			// Bug 1375�b��Ή� by ���V
			// ipSet��NULL�̂܂܂̏ꍇ�����邽��
			if( ipSet )
			{
				ipSet->Reset();
				ipSet->Next( &ipUnk );
			}
			_IRowPtr ipSelectedFeature = ipUnk;
			if( it->GetFeature() != ipSelectedFeature )
			{
				SelectItem( GetParentItem( GetParentItem( hItem ) ) );
				SetFocus();

				// �I�����ꂽ�t�B�[�`���̃n�C���C�g
				HilitSelectedLQ(it->GetShape());
				// �t�B�[�`���̑I�����ύX���ꂽ���Ƃ�ʒm
				return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
			}

			if( bDblClick )
				m_cArcHelper.SetCenter( m_ipHilitPoint );
			// �S�~�������̂ɂ� esriViewForeground
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		} else if( iKind == OUTERROWITEM ) {
			// �I����Ԃ�ύX����
			SelectItem( hItem );
			SetFocus();

			// �N���b�N���ꂽ�A�C�e���̎�ނ�ʒm
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		}
	}
	return 0;
}

// �c���[��ŉE�N���b�N���������Ƃ��ɃR�s�[�̃��j���[���o���܂�
BOOL CFeatureTree::ShowMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ���j���[�ǂݍ���
	HMENU hMenu = ::LoadMenu( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU));
	if ( !hMenu )
		return FALSE;
	// �|�b�v�A�b�v���j���[�擾
	HMENU hPopup = ::GetSubMenu( hMenu, 0);
	if ( !hPopup )
		return FALSE;
	
	// �R���e�L�X�g���j���[�̐e�ƂȂ�}�b�v��Window�n���h���擾
	HWND hWnd;
	IApplicationPtr piApp = m_cArcHelper.GetApp();
	piApp->get_hWnd( reinterpret_cast<UINT*>( &hWnd));
	
	// �J�[�\���ʒu�擾
	POINT stPos;
	::GetCursorPos( &stPos);
	
	// Popup���j���[�\��
	int nResult = ::TrackPopupMenu( hPopup, TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, stPos.x, stPos.y, 0, hWnd, NULL);
	
	switch( nResult )
	{
        // �����R�s�[
	case ID_OBJECTID_COPY:
		HTREEITEM hItem = GetSelectedItem();
		BSTR strTree = NULL;
		GetItemText( hItem, strTree );
		CComBSTR bstrTree;
		bstrTree.Attach(strTree);
		
		WriteClipBoard(bstrTree);
		::SysFreeString( strTree );
		break;
	}
	
	return TRUE;
}

// HTREEITEM �\���̂��� CFeatureDef ����肵�ĕԂ�
CFeatureDef* CFeatureTree::GetFeatureDef( HTREEITEM item )
{
	LONG l = (LONG)( 0x4000000 ^ GetItemData( item ) );
	LONG lListCount = m_pFeatureDefList->size();
	if( lListCount < (l - 1) ) return NULL;        // ���X�g�̃T�C�Y�`�F�b�N
	std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
	advance( it, l );
	
	return &(*it);
}

// HTREEITEM �\���̂��� CRowDef ����肵�ĕԂ�
CRowDef* CFeatureTree::GetRowDef( HTREEITEM item )
{
	if( m_bIsLQAttribute == false )
	{
		LONG l = (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( item ) ) ) );
		LONG lListCount = m_pFeatureDefList->size();
		if( lListCount < (l - 1) ) return NULL;        // ���X�g�̃T�C�Y�`�F�b�N
		std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
		advance( it, l );
		
		l = (LONG)( 0x2000000 ^ GetItemData( GetParentItem( item ) ) );
		LONG lRelationCount = it->GetRelationalTableDefList()->size();
		if( lRelationCount < (l - 1) ) return NULL; // ���X�g�̃T�C�Y�`�F�b�N
		std::list<CRelationalTableDef>::iterator itRelTableDefList = it->GetRelationalTableDefList()->begin();
		advance( itRelTableDefList, l );
		
		l = (LONG)( 0x1000000 ^ GetItemData( item ) );
		LONG lRelDefCount = itRelTableDefList->size();
		if( lRelDefCount < (l - 1) ) return NULL; // ���X�g�̃T�C�Y�`�F�b�N
		std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
		advance( itRowDefList, l );
		
		return &(*itRowDefList);
	}
	else
	{
		LONG l = (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( item ) ) ) );
		LONG lListCount = m_pLQDefList->size();
		if( lListCount < (l - 1) ) return NULL;	// ���X�g�̃T�C�Y�`�F�b�N
		std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
		advance( it, l );

		l = (LONG)( 0x2000000 ^ GetItemData( GetParentItem( item ) ) );
		LONG lRelationCount = it->GetRelationalTableDefList()->size();
		if( lRelationCount < (l - 1) ) return NULL; // ���X�g�̃T�C�Y�`�F�b�N
		std::list<CRelationalTableDef>::iterator itRelTableDefList = it->GetRelationalTableDefList()->begin();
		advance( itRelTableDefList, l );

		l = (LONG)( 0x1000000 ^ GetItemData( item ) );
		LONG lRelDefCount = itRelTableDefList->size();
		if( lRelDefCount < (l - 1) ) return NULL; // ���X�g�̃T�C�Y�`�F�b�N
		std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
		advance( itRowDefList, l );

		return &(*itRowDefList);
	}
}

// HTREEITEM �\���̂��� CLQRowDef ����肵�ĕԂ�
CLQRowDef* CFeatureTree::GetLQRowDef( HTREEITEM item )
{
	if( item == NULL ) return NULL;

	LONG l = (LONG)( 0x4000000 ^ GetItemData( item ) );
	LONG lListCount = m_pLQDefList->size();
	if( lListCount < (l - 1) || l < 0 ) return NULL;	// ���X�g�̃T�C�Y�`�F�b�N
	std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
	advance( it, l );

	return &(*it);
}
// �N���b�v�{�[�h�ɑI������OID���R�s�[���܂�
void CFeatureTree::WriteClipBoard(CComBSTR bstrTree)
{
    USES_CONVERSION;
    LPTSTR str;
    HGLOBAL hGlobal;
    int iLength = 0;
    CString lpStr = OLE2T( bstrTree );
    iLength = bstrTree.Length();
    if (iLength > 1024)
        iLength = 1024;
    hGlobal = ::GlobalAlloc(GHND, 2*( iLength + 1 ));
    if (!hGlobal)
        return;

	/*
	�O���[�o���������̈�̃��b�N
 
	�O���[�o���������I�u�W�F�N�g�����b�N���A
	�������u���b�N�̍ŏ���1�o�C�g�ւ̃|�C���^��Ԃ��B
	*/
    if( !( str = (LPTSTR)::GlobalLock(hGlobal)) )
	{
		GlobalFree(hGlobal);
		return;
	}

	::lstrcpy(str,lpStr);

	/*
	�O���[�o���������̈�̃��b�N����
	�@
	�w�肳�ꂽ�O���[�o���������I�u�W�F�N�g��������A
	���̃n���h���𖳌��ɂ���B
	*/
    ::GlobalUnlock(hGlobal);

	/*
	�N���b�v�{�[�h���J��

	�N���b�v�{�[�h���J���A
	���̃A�v���P�[�V�������N���b�v�{�[�h�̓��e��ύX�ł��Ȃ��悤�ɂ���B
	���̃E�B���h�E�����ɃN���b�v�{�[�h���J���Ă���ꍇ�A
	OpenClipboard�֐��͎��s����B
	OpenClipboard�̌Ăяo���ɐ������邽�тɁA
	�A�v���P�[�V������CloseClipboard�֐����Ăяo���ׂ��B
	*/
	if( !( ::OpenClipboard(NULL) ) )
	{
		GlobalFree(hGlobal);
		return;
	}

	/*
	�N���b�v�{�[�h����ɂ���
	
	�N���b�v�{�[�h����ɂ��A
	�N���b�v�{�[�h���̃f�[�^�̃n���h�����������B
	�����ɁA�N���b�v�{�[�h���J�����E�B���h�E�ɁA
	�N���b�v�{�[�h�̏��L����^����B
	*/
    if( !( ::EmptyClipboard() ) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}

	/*
	�N���b�v�{�[�h�ɁA�w�肳�ꂽ�f�[�^�`���Ńf�[�^���i�[����B
	�f�[�^���i�[����ɂ́A
	�Ăяo�����̃E�B���h�E���N���b�v�{�[�h�̌��݂̃I�[�i�[�ł��邱�ƁA
	�����OpenClipboard�֐����g���ăN���b�v�{�[�h���J���Ă������Ƃ��K�v�B
	
	SetClipboardData�ŃN���b�v�{�[�h�ɓn�����������I�u�W�F�N�g�́A
	����ȍ~�g�p�ł��Ȃ��Ȃ�B
	(����āA�K�v�Ȃ�ʂɃo�b�t�@���m�ۂ��Ă����ɕۑ����Ă���)�B
	
	SetClipboardData�ŃN���b�v�{�[�h�ւ̐ݒ肪���������ꍇ�A
	�O���[�o���������n���h����Windows�̊Ǘ��ɂȂ�̂ŁA
	�A�v���P�[�V��������GlobalFree()���Ă͂Ȃ�Ȃ�(��΂�)�B
	 
	SetClipboardData�����s������́A
	�������u���b�N��Windows�̂��̂ƂȂ�̂ŁA
	���(GlobalFree)������A
	���b�N(GlobalLock�����ςȂ�)�����܂܂ł����Ă͂����Ȃ��B
�@�@ */
    if( !::SetClipboardData(CF_UNICODETEXT, hGlobal) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}

	/*
	�N���b�v�{�[�h�̃N���[�Y

	CloseClipboard�֐����Ăяo���ăN���b�v�{�[�h�����ƁA
	���̃A�v���P�[�V�������N���b�v�{�[�h�ɃA�N�Z�X�ł���悤�ɂȂ�B
	CloseClipboard���Ăяo������́A
	�N���b�v�{�[�h�ɃI�u�W�F�N�g��z�u���Ȃ����ƁB
�@�@ */
    ::CloseClipboard();

	return;
}

#include <TDC/useful_headers/str_util.h>

CString CFeatureTree::getXYZAsString(IPointPtr point) const
{
	double x = 0.0, y = 0.0, z = 0.0;
	point->QueryCoords(&x, &y);

	VARIANT_BOOL ZAware = VARIANT_FALSE;
	IZAwarePtr(point)->get_ZAware(&ZAware);
	if (ZAware)
		point->get_Z(&z);

	auto d2cstr = [](double d)->CString {
		char c[128];
		_gcvt(d, 100, c);
		return CString(c);
	};
	CString strVertex;
	strVertex.Format(_T("X=%s Y=%s Z=%s"), d2cstr(x), d2cstr(y), d2cstr(z));

	return strVertex;
}

/////////////////////////////////////////////////////////////////////////////
//
// �c���[�r���[�C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////

// ���N���b�N�C�x���g
LRESULT CFeatureTree::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// [Bug 5490]�c���[�\�����ɃS�~���c��o�O�@�������I����܂ŕ`����Ƃ߂�
	SetRedraw( FALSE );
    m_bIsRButtonClick = FALSE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, FALSE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, FALSE );

	SetRedraw( TRUE );
    return lResult;
}

// �E�N���b�N�C�x���g
LRESULT CFeatureTree::OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    m_bIsRButtonClick = TRUE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, FALSE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, FALSE );

    return lResult;
}

// �_�u���N���b�N�C�x���g
LRESULT CFeatureTree::OnLButtonDoubleClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    m_bIsRButtonClick = FALSE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, TRUE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, TRUE );

    return lResult;
}

// �ĕ`��C�x���g
LRESULT CFeatureTree::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( !m_bIsLQAttribute )
		HilitSelectedFeatures( TRUE );
	else
	{
		if( m_ipHilitPoint )
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );

		HilitSelectedLQ( m_ipHilightGeometry, TRUE );
	}
    return 0;
}
// �����N��n�C���C�g�̃C�x���g
// �����N��I���_�C�A���O���炵���Ă΂�Ȃ��O��
LRESULT CFeatureTree::OnSetCurrentFeature( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( !wParam || lParam <= 0 )
		return 0;

	HTREEITEM hItem = SearchOID( (LPCTSTR)wParam, (LONG)lParam );

	if( !hItem )
		return 0;

	// �I����Ԃ�ύX����
	SelectItem( hItem );
	SetFocus();
		
	if( m_bIsLQAttribute )
	{
		HilitSelectedLQ( GetLQRowDef( hItem )->GetShape() );
	}

	// �N���b�N���ꂽ�A�C�e���̎�ނ�ʒm
	return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
}
