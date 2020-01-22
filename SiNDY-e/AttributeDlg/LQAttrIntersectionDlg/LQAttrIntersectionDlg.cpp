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

// LQAttrIntersectionDlg.cpp: CLQAttrIntersectionDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrIntersectionDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�i�������ʌ����_���̗p�j
BEGIN_WINDOW_MAP(INTERSECTION_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			BEGINROWS(WRCT_REST, IDC_STATIC_INTERSECTION, RCMARGINS(10,5))	// �i�������ʌ����_�O���[�v
				RCSPACE(15)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))		// ��������
					RCTOFIT(IDC_STATIC_KANJI)
					RCREST(IDC_EDIT_KANJI)
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,5))		// �J�i����
					RCTOFIT(IDC_STATIC_YOMI)
					RCREST(IDC_EDIT_YOMI)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))		// �R�s�[�{�^��
			RCTOFIT(IDC_BUTTON_BASE_COPY)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			BEGINROWS(WRCT_REST, IDC_STATIC_BASE_INTERSECTION, RCMARGINS(10,5))	// ��\�����_�O���[�v
				RCSPACE(15)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))		// ��������
					RCTOFIT(IDC_STATIC_BASE_KANJI)
					RCREST(IDC_EDIT_BASE_KANJI)
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,5))		// �J�i����
					RCTOFIT(IDC_STATIC_BASE_YOMI)
					RCREST(IDC_EDIT_BASE_YOMI)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
		ENDGROUP()
		RCSPACE(7)
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CLQAttrIntersectionDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrIntersectionDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////
/** 
 * @fn CLQAttrIntersectionDlg::CLQAttrIntersectionDlg()
 * @brief �R���X�g���N�^
 */
CLQAttrIntersectionDlg::CLQAttrIntersectionDlg() 
	: CLQAttrCommonBaseDlg(INTERSECTION_MAP, IDD_LQATTRINTERSECTIONDLG),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndSourceKanji( _T("EDIT"), this, IDC_EDIT_KANJI ),
	m_wndSourceYomi( _T("EDIT"), this, IDC_EDIT_YOMI ),
	m_wndSourceBaseKanji( _T("EDIT"), this, IDC_EDIT_BASE_KANJI ),
	m_wndSourceBaseYomi( _T("EDIT"), this, IDC_EDIT_BASE_YOMI ),
	m_wndSourceCopyBtn( _T("BUTTON"), this, IDC_BUTTON_BASE_COPY ) // �^�u�I�[�_�[���������������̂ŁAXXX_BASE_XXX�ɂ��Ă��ǉ� (bug9458)
{
	m_hTipNameKanji = NULL;
	m_hTipNameYomi = NULL;
	m_hTipBaseNameKanji = NULL;
	m_hTipBaseNameYomi = NULL;
}
/** 
 * @fn HWND CLQAttrIntersectionDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrIntersectionDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrIntersectionDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrIntersectionDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrIntersectionDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CLQAttrIntersectionDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	else
		::SendMessage( GetDlgItem(IDC_EDIT_SOURCE), WM_KILLFOCUS, NULL, NULL );

	SetBaseIntersectionInfo();	// ��\�����_���̂̃Z�b�g

	SetToolTip( IDC_EDIT_KANJI );
	SetToolTip( IDC_EDIT_YOMI );
	SetToolTip( IDC_EDIT_BASE_KANJI );
	SetToolTip( IDC_EDIT_BASE_YOMI );
 
	return bRet;
}

/**
 * @fn void CLQAttrIntersectionDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrIntersectionDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),		NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_KANJI,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_EDIT_YOMI,	TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI"),	NULL, strInfoTableName );
		m_cControlRel.SetControl( IDC_BUTTON_BASE_COPY,		TYPE_BUTTON,	0,	0 );
	}
}
BOOL CLQAttrIntersectionDlg::SetCtrlColorChange(HWND hTargetWND)
{
	HWND hWnd1,hWnd2;

	hWnd1 = CWindow( GetDlgItem( IDC_EDIT_KANJI ) );
	hWnd2 = CWindow( GetDlgItem( IDC_EDIT_YOMI ) );
	if( hWnd1 == hTargetWND || hWnd2 == hTargetWND )
		return TRUE;

	return FALSE;
}
/**
 * @fn BOOL CLQAttrIntersectionDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɃG���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CLQAttrIntersectionDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();
		if( strTableName == strInfTableName )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( strTableName, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( strTableName, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_KANAERROR ), lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_KANAERROR ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

IFeatureClassPtr CLQAttrIntersectionDlg::GetRoadLinkWithLQ( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOC�ɂ��邷�ׂẴW�I�t�B�[�`�����C�����擾���ă��[�v�񂵂��ړ��Ă̓��H�����N�擾����
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// ���H�����N���A�I�[�i���������łȂ��Ƃ����Ȃ�
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadFeatureClass;
}

IFeaturePtr CLQAttrIntersectionDlg::GetRoadFeature( CLQRowDef* pRowDef, LONG lOID )
{
	IFeaturePtr ipFeature;
	if( lOID > 0 )
	{
		// LQ�Ɠ������[�N�X�y�[�X�̓��H�����N�t�B�[�`���N���X���擾
		IFeatureClassPtr ipRoadClass = GetRoadLinkWithLQ( pRowDef );
		// ���������N�t�B�[�`���擾
		// �ҏW�J�n�O�͕ҏW�L���b�V�����Ƃ�Ȃ��̂ŁA�S���������ق����͂₢�̂ŕҏW�J�n�O�J�n��ŏꍇ�킯
		if( IsStartEditing() )
		{
			if( ipRoadClass )
				ipRoadClass->GetFeature( lOID, &ipFeature );
		}
		else{
			// ���H�����N�e�[�u������N�G���������ĒT��
			IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("TO_NODE_ID,FROM_NODE_ID,SHAPE"), _T("OBJECTID=%ld"), lOID );

			if( ipFilter )
			{
				IFeatureCursorPtr ipCursor;
				ipRoadClass->Search( ipFilter, VARIANT_FALSE, &ipCursor );
				ipCursor->NextFeature( &ipFeature );
			}
		}
	}

	return ipFeature;	
}
IFeatureClassPtr CLQAttrIntersectionDlg::GetRoadNodeFeatureClass( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadNodeFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOC�ɂ��邷�ׂẴW�I�t�B�[�`�����C�����擾���ă��[�v�񂵂��ړ��Ă̓��H�m�[�h�擾����
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// ���H�m�[�h���A�I�[�i���������łȂ��Ƃ����Ȃ�
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( NODE_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadNodeFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadNodeFeatureClass;
}
void CLQAttrIntersectionDlg::SetBaseIntersectionInfo()
{
	CLQRowDef* pLQRowDef = GetCurrentFeatureDef();
	IFeatureClassPtr ipFeatureClassNode = GetRoadNodeFeatureClass( pLQRowDef );
	if( pLQRowDef && ipFeatureClassNode )
	{
		LONG lDirC = 0;
		LONG lLinkID = 0;
		// �����N��̍Ōナ���N��ID�ƌ������擾����
		if( GetTargetLinkAndDir( pLQRowDef, lDirC, lLinkID ) )
		{
			LONG lNodeID = 0;
			IFeaturePtr ipLinkFeature = GetRoadFeature( pLQRowDef, lLinkID );
			IFieldsPtr ipFields;
			long lidx = 0;
			ipLinkFeature->get_Fields( &ipFields );
			CComVariant cVal;
			if( lDirC == 1 )
			{
				ipFields->FindField( _T("TO_NODE_ID"), &lidx );
			}
			else
			{
				ipFields->FindField( _T("FROM_NODE_ID"), &lidx );
			}
			ipLinkFeature->get_Value( lidx, &cVal );
			lNodeID = cVal.lVal;

			IFeaturePtr ipNodeFeature;
			ipFeatureClassNode->GetFeature( lNodeID, &ipNodeFeature );
			ipFields = NULL;
			ipNodeFeature->get_Fields( &ipFields );
			CString strVal;
			// ���H�m�[�h�̑�\�����_��������
			ipFields->FindField( _T("NAME_KANJI"), &lidx );
			ipNodeFeature->get_Value( lidx, &cVal );
			strVal = cVal.bstrVal;
			CEdit cEdit_Kanji( GetDlgItem(IDC_EDIT_BASE_KANJI) );
			cEdit_Kanji.SetWindowTextW( strVal );

			// ���H�m�[�h�̑�\�����_�J�i����
			ipFields->FindField( _T("NAME_YOMI"), &lidx );
			ipNodeFeature->get_Value( lidx, &cVal );
			strVal = cVal.bstrVal;
			CEdit cEdit_Yomi( GetDlgItem(IDC_EDIT_BASE_YOMI) );
			cEdit_Yomi.SetWindowTextW( strVal );
		}
	}
}
bool CLQAttrIntersectionDlg::GetTargetLinkAndDir(CLQRowDef* pRowDef, LONG &lDirC, LONG &lLinkID)
{
	LONG OID = pRowDef->GetRowOID();
	CRelationalTableDef* pRelTableDef = pRowDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), LASTLINK_F_FIELD ) == 1 )
		{
			lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
			lDirC = GetLongValue( &(*itLQRow), LINKDIR_C_FIELD );

			return true;
		}
	}
	return false;
}
LONG CLQAttrIntersectionDlg::GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName )
{
	LONG lVal = -1;

	if( pRowDef )
	{
		const CFieldDef* pFieldDef = pRowDef->GetFieldDef( lpcszFieldName );
		if( pFieldDef != NULL )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
				lVal = pFieldDef->m_vaValue.lVal;
		}
	}
	return lVal;
}

LRESULT CLQAttrIntersectionDlg::OnBnClickedButtonBaseCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CLQRowDef* pLQRowDef = GetCurrentFeatureDef();
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueIntersection, sindyeLoadQueueInfoInfoTableName );

	TCHAR cBufKanji[MAX_PATH];
	CEdit cEdit_BaseKanji( GetDlgItem(IDC_EDIT_BASE_KANJI) );
	cEdit_BaseKanji.GetWindowTextW( cBufKanji, MAX_PATH );

	TCHAR cBufYomi[MAX_PATH];
	CEdit cEdit_BaseYomi( GetDlgItem(IDC_EDIT_BASE_YOMI) );
	cEdit_BaseYomi.GetWindowTextW( cBufYomi, MAX_PATH );

	CEdit cEdit_Kanji( GetDlgItem(IDC_EDIT_KANJI) );
	cEdit_Kanji.SetWindowTextW( cBufKanji );

	CEdit cEdit_Yomi( GetDlgItem(IDC_EDIT_YOMI) );
	cEdit_Yomi.SetWindowTextW( cBufYomi );

	_IRowPtr ipRow = pLQRowDef->GetFeature();
	if( ipRow )
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lidx = 0;
		ipFields->FindField( _T("NAME_KANJI"), &lidx );
		CComVariant cValKanji( cBufKanji );
		ipRow->put_Value( lidx, cValKanji );

		ipFields->FindField( _T("NAME_YOMI"), &lidx );
		CComVariant cValYomi( cBufYomi );
		ipRow->put_Value( lidx, cValYomi );
	}

	return 0;
}

void CLQAttrIntersectionDlg::SetToolTip(INT nTargetItem, BOOL bIsChange)
{
	// ToolInfo�\���� 
	static TOOLINFO ti;
	if( !bIsChange )
	{
		// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		HWND hTmpWnd = NULL;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				hTmpWnd = m_hTipNameKanji;
				break;
			case IDC_EDIT_YOMI:
				hTmpWnd = m_hTipNameYomi;
				break;
			case IDC_EDIT_BASE_KANJI:
				hTmpWnd = m_hTipBaseNameKanji;
				break;
			case IDC_EDIT_BASE_YOMI:
				hTmpWnd = m_hTipBaseNameYomi;
				break;
			default:
				break;
		}
		if( hTmpWnd )
		{
			InitToolTip( nTargetItem, &ti );
			::SendMessage( hTmpWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ti );
			ti.lpszText = lpszText;

			::SendMessage( hTmpWnd, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
			return;
		}

		// �܂��c�[���`�b�v�쐬����Ă��Ȃ�������쐬����i��ԍŏ��̂ݓ����Ă���j
		HWND hToolTip;
		hToolTip =
			CreateWindowEx(
			0,				//�g���E�B���h�E�X�^�C��
			TOOLTIPS_CLASS,	//�N���X�l�[��
			NULL,			//�E�B���h�E�l�[��
			TTS_ALWAYSTIP,	// �E�B���h�E�X�^�C��
			CW_USEDEFAULT,	// �w���W
			CW_USEDEFAULT,	// �x���W
			CW_USEDEFAULT,	// ��
			CW_USEDEFAULT,	// ����
			GetDlgItem(nTargetItem),// �e�E�B���h�E�̃n���h��
			NULL,			// ���j���[�n���h��
			NULL,			// �C���X�^���X�n���h��
			NULL);			// WM_CREATE�f�[�^

		InitToolTip( nTargetItem, &ti );

		ti.lpszText = lpszText;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				m_hTipNameKanji = hToolTip;
				break;
			case IDC_EDIT_YOMI:
				m_hTipNameYomi = hToolTip;
				break;
			case IDC_EDIT_BASE_KANJI:
				m_hTipBaseNameKanji = hToolTip;
				break;
			case IDC_EDIT_BASE_YOMI:
				m_hTipBaseNameYomi = hToolTip;
				break;
			default:
				break;
		}
		hTmpWnd = hToolTip;
		::SendMessage( hTmpWnd, TTM_ADDTOOL , 0 , (LPARAM)&ti);
		::SendMessage( hTmpWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

		return;
	}
	else
	{
		// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		HWND hTmpWnd = NULL;
		switch( nTargetItem )
		{
			case IDC_EDIT_KANJI:
				hTmpWnd = m_hTipNameKanji;
				break;
			case IDC_EDIT_YOMI:
				hTmpWnd = m_hTipNameYomi;
				break;
			case IDC_EDIT_BASE_KANJI:
				hTmpWnd = m_hTipBaseNameKanji;
				break;
			case IDC_EDIT_BASE_YOMI:
				hTmpWnd = m_hTipBaseNameYomi;
				break;
			default:
				break;
		}
		if( hTmpWnd )
		{
			InitToolTip( nTargetItem, &ti );
			::SendMessage( hTmpWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ti );
			ti.lpszText = lpszText;

			::SendMessage( hTmpWnd, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
			return;
		}
	}
}

void CLQAttrIntersectionDlg::InitToolTip(INT nTargetItem, TOOLINFO *ti)
{
	ZeroMemory(&(*ti) , sizeof(TOOLINFO)) ; 
	(*ti).cbSize = sizeof(TOOLINFO) ; 
	(*ti).uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	(*ti).hwnd = GetDlgItem(nTargetItem);
	(*ti).uId = (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;
}
