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

// AttrSegmentAttrDlg.cpp: CAttrSegmentAttrDlg �N���X�̃C���v�������e�[�V����

#include "stdafx.h"
#include "AttrSegmentAttrDlg.h"

using namespace sindy::schema;

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�����N�������p�j
BEGIN_WINDOW_MAP(SEGMENT_ATTR_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_CHECK1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT3)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC4)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_COMBO1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC5)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT4)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT5)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrSegmentAttrDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSegmentAttrDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSegmentAttrDlg::CAttrSegmentAttrDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrSegmentAttrDlg::CAttrSegmentAttrDlg()
	: ATTR_BASE_CLASS(SEGMENT_ATTR_MAP, IDD_ATTRSEGMENTATTRDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_SEGMENT_ATTR_CHECK1, CContainedWindow( _T("CHECKBOX"), this, IDC_SEGMENT_ATTR_CHECK1))
		(IDC_SEGMENT_ATTR_EDIT2,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT2 ))
		(IDC_SEGMENT_ATTR_EDIT3,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT3 ))
		(IDC_SEGMENT_ATTR_COMBO1, CContainedWindow( _T("COMBO"),    this, IDC_SEGMENT_ATTR_COMBO1 ))
		(IDC_SEGMENT_ATTR_EDIT4,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT4 ))
		(IDC_SEGMENT_ATTR_EDIT5,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT5 ))
		(IDC_EDIT_SOURCE,         CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SOURCE ));
}

/** 
 * @fn HWND CAttrSegmentAttrDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrSegmentAttrDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSegmentAttrDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrSegmentAttrDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrSegmentAttrDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * 2�����b�V�����ׂ��Őڂ��Ă��郊���N���������I������Ă���ꍇ�ɂ́A�֘A���郊���N���������m�ۂ���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrSegmentAttrDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 2�����b�V�����E�̂��ߒǉ�
	m_pAttrLinkOnMeshList.clear();

	// 2�����b�V�����E�ׂ��ł��邩�ǂ����t���O�̏�����
	m_bIsRoadOnMesh = FALSE;

	IFeaturePtr	ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cTargetItems;
	cTargetItems.clear();

	// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă��郊���N���������擾
	if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), ipFeature, cTargetItems ) == TRUE )
	{
		// 2�����b�V�����ׂ��ł���̂Ńt���O�ύX
		m_bIsRoadOnMesh = TRUE;

		// �����̎��
		long attrCode = AheGetAttribute( GetCurrentFeatureDef()->GetFeature(), segment_attr::kLinkAttr ).lVal;

		// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
		for( const auto& onMesh : cTargetItems )
		{
			IFeaturePtr ipFeatureOnMesh = onMesh.GetFeature();

			// ������ʂ̂��̂�������ɂ��Ȃ��ibug 11641�j
			if( attrCode != AheGetAttribute(ipFeatureOnMesh, segment_attr::kLinkAttr).lVal )
				continue;

			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pAttrLinkOnMeshList.push_back(featDef);
		}
	}

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_SEGMENT_ATTR_COMBO1) );
	}

	// ����NAME�t�B�[���h���Ȃ��Ȃ�G�f�B�b�g�{�b�N�X���͔�\���ɂ���
	AdjustLayout();

	// �������Ӓn�_�ȊO�Ȃ�NAME�t�B�[���h�͕ҏW�s��
	SetNameFieldEditable();

	return bRet;
}

/**
 * @fn void CAttrSegmentAttrDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrSegmentAttrDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// �����N�������p

		// ���n�����t���O
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_CHECK1, TYPE_CHECKBOX, 0, 0, segment_attr::kFieldSurvey,NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY  );
		// ��ƔN�x
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT2, TYPE_EDITSTRING, 0, 0, segment_attr::kWorkYear, NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY  );
		// �Ή������NID
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT3, TYPE_EDIT, 0, 0, segment_attr::kLinkID, NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY );
		// �����N�����
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_COMBO1, TYPE_COMBO, 0, 0, segment_attr::kLinkAttr, NULL, segment_attr::kTableName );
		// �㉺�֌W
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT4, TYPE_EDIT, 0, 0, segment_attr::kUnderOver, NULL, segment_attr::kTableName );
		// ����
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT5, TYPE_EDITSTRING, 0, 0, segment_attr::kName, NULL, segment_attr::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, segment_attr::kSource, NULL, segment_attr::kTableName );
	}
}



void CAttrSegmentAttrDlg::SetLinkAttrComboBox()
{
	// �R���{�{�b�N�X�擾
	CComboBox	cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );

	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return;

	// SEGMENTATTR_TABLE_NAME�擾
	ITablePtr			ipTable;
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = segment_attr::kTableName;
	else
		strOwnerName.AppendFormat( _T(".%s"), segment_attr::kTableName );

	ipTable = AheOpenTableByTableName ( ipWorkspace, strOwnerName );

	if( !ipTable ) {
		AttrMessageBox( AheLoadString( IDS_FAILTOGETTABLE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// �����NID�擾
	CFieldDef	*cFieldDef = GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kLinkID );
	if( !cFieldDef )
		return;
	LONG lLink_ID = cFieldDef->m_vaValue.lVal;
	// �e�[�u�����猟��
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	_ICursorPtr ipCursor;
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), segment_attr::kLinkID, lLink_ID );
	strSubFields.Format(_T("OBJECTID,Shape"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	if( !ipCursor )
		return;

	// Geometry�擾
	IFeaturePtr	ipFeature;
	IGeometryPtr ipCurrentGeom;
	ipFeature = GetCurrentFeatureDef()->GetFeature();
	ipFeature->get_ShapeCopy( &ipCurrentGeom );
	LONG lCurrentOID;
	ipFeature->get_OID( &lCurrentOID );

	_IRowPtr ipRow;
    while( ipCursor->NextRow( &ipRow ) == S_OK )
    {
        if( !ipRow ) 
			break;

		IFeaturePtr ipFeatureOnSameLink = ipRow;
		LONG lOIDOnSameLink = 0;
		ipFeatureOnSameLink->get_OID( &lOIDOnSameLink );

		if( lCurrentOID == lOIDOnSameLink )
			continue;

		IGeometryPtr ipGeomOnSameLink;
		ipFeatureOnSameLink->get_ShapeCopy( &ipGeomOnSameLink );

		// �I�[�o�[���b�v���Ă��郊���N�������̃����N��ʂ��R���{�{�b�N�X����͂���
		if( AheIsOverlap2( ipCurrentGeom, ipGeomOnSameLink ) )
		{
			IFieldsPtr ipFields;
			ipFeatureOnSameLink->get_Fields( &ipFields );

			LONG	lIndex = 0;
			CComVariant	vaValue;
			ipFields->FindField( CComBSTR( segment_attr::kLinkAttr ), &lIndex );
			ipFeatureOnSameLink->get_Value( lIndex, &vaValue );

			// ���ݕێ����Ă��郊�X�g�̒��ɑΏۂ̃����N������������΁A����ƌ�������
			// �����Ȃ���΁ACursor����E���Ă������̂ł悢
			for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
			{
				CString strTableName = (*it).GetTableName();
				CFieldDef* cFieldDefOID = (*it).GetFieldDef( segment_attr::kTableName, _T("OBJECTID") );
				// [Bug 5875]�I�[�o�[���b�v���Ă��郊���N��������I������Ɨ�����o�O�Ή�
				// �����N�������ȊO������ł���ꍇ�ɗ����Ă��܂��Ă���
				if( cFieldDefOID )
				{
					long lOID = cFieldDefOID->m_vaValue.lVal;
					if( strTableName == segment_attr::kTableName && lOID == lOIDOnSameLink )
					{
						// ���݂̒l�ƒu��������
						CFieldDef* cFieldDef = (*it).GetFieldDef( segment_attr::kTableName, segment_attr::kLinkAttr );
						vaValue = cFieldDef->m_vaValue;
					}
				}
			}

			LONG lLinkAttr_c = vaValue.lVal;
			LONG lCount = cCombo.GetCount();

			for( INT i = 0 ; i < lCount; i++ )
			{
				LONG lSetItemData = cCombo.GetItemData( i );

				if( lSetItemData == lLinkAttr_c )
					cCombo.DeleteString( i );
			}
		}
	}
}

/**
 * @fn BOOL CAttrSegmentAttrDlg::CheckReturnKeyItem(INT nClassID)
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	INT		nClassID	[i]		���^�[���L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ���^�[���L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrSegmentAttrDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
	// �ȉ��̏������s��
	if( m_bIsRoadOnMesh )
	{
		// �Ƃ肠�����A���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v���s��
		for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
			CFeatureDef *pFeatureDef = NULL;
			for( std::list<CFeatureDef>::iterator iAttr = m_pAttrLinkOnMeshList.begin(); iAttr != m_pAttrLinkOnMeshList.end(); iAttr++ )
			{
				if( iAttr->GetFeature() == it->GetFeature() )
				{
					pFeatureDef = &(*it);
					break;
				}
			}

			if( pFeatureDef )
			{
				// ���ׂẴt�B�[���h�l���`�F�b�N
				for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
				{
					CString strFieldName = itField->m_strFieldName;

					// �ȉ����e���R�s�[����
					if( 0 == strFieldName.CompareNoCase( segment_attr::kLinkAttr ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kUnderOver ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kName ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kSource ) )
					{
						// ��L��if���̏����ȊO�̓R�s�[����
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( segment_attr::kTableName, itField->m_strFieldName );
						VARIANT vaValue;
						pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
						if( itField->m_vaValue != vaValue )
						{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = TRUE;
						}
						else{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = FALSE;
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CAttrSegmentAttrDlg::ErrorCheck()
{
	// ��������NAME�t�B�[���h�Ȃ��Ȃ牽�����Ȃ�(ASEAN�Ƃ�)
	if( !GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return TRUE;

	// �u�������Ӓn�_�v�ȊO�����̂������Ă��Ă͂����Ȃ�
	// �����I�ɂ͑��̎�ʂł����̎��悤�ɂȂ邩��

	// �R���{�{�b�N�X�擾
	CComboBox cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );
	int selectedIndex = cCombo.GetCurSel();
	DWORD selectedValue = cCombo.GetItemData( selectedIndex );
	if( (long)selectedValue == sindy::schema::segment_attr::link_attr::kFlooding )
	{
		return TRUE;
	}

	// ���̂�NULL��OK
	CString name;
	GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ).GetWindowText( name );
	if( name.IsEmpty() )
	{
		return TRUE;
	}

	CComBSTR selectedText;
	cCombo.GetLBTextBSTR( selectedIndex, (BSTR&)selectedText );
	CString errorMessage;
	errorMessage.Format( AheLoadString( IDS_NAMENOTNULL ), selectedText );
	AheMessageBox( errorMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH );

	return FALSE;
}

void CAttrSegmentAttrDlg::SetNameFieldEditable()
{
	// ��������NAME�t�B�[���h�Ȃ��Ȃ牽�����Ȃ�(ASEAN�Ƃ�)
	if( !GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return;

	if( IsNameFieldEditableAttr() )
	{
		::EnableWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), TRUE );
	}
	else
	{
		::EnableWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), FALSE );
	}
}

bool CAttrSegmentAttrDlg::IsNameFieldEditableAttr()
{
	// �R���{�{�b�N�X�擾
	CComboBox cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );
	int selectIndex = cCombo.GetCurSel();
	DWORD selectedValue = cCombo.GetItemData( selectIndex );

	// �������Ӓn�_�ȊO�Ȃ�NAME�t�B�[���h�͕ҏW�s��
	// NAME�t�B�[���h�ҏW�̎�ʂ͏����I�ɑ����邩��
	if( (long)selectedValue == sindy::schema::segment_attr::link_attr::kFlooding )
	{
		return true;
	}

	return false;
}

void CAttrSegmentAttrDlg::AdjustLayout()
{
	WINRECT *map = NULL; // WinMgr����̃R���g���[���擾�p

	// NAME�t�B�[���h�����邩�ǂ���
	if( GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return;

	// �t�B�[���h���Ȃ����̓G�f�B�b�g�{�b�N�X�ƃX�^�e�B�b�N�R���g���[���͔�\��
	::ShowWindow( GetDlgItem( IDC_SEGMENT_ATTR_STATIC6 ), SW_HIDE );
	::ShowWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), SW_HIDE );
	map = m_winMgr.FindRect( IDC_SEGMENT_ATTR_EDIT5 );

	if( map )
	{
		// �Ĕz�u
		m_winMgr.InitToFitSizeFromCurrent(this);
		m_winMgr.CalcLayout(this);
		m_winMgr.SetWindowPositions(this);	
	}
}
