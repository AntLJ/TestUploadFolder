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

 AttrCSDBDlg.cpp : CAttrCSDBDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCSDBDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(DEFAULTMAP)
	BEGINROWS(WRCT_REST,0,0)
		RCREST(IDC_GRIDCTRL)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrCSDBDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCSDBDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCSDBDlg::CAttrCSDBDlg()
 * @brief �R���X�g���N�^
 *
 * �Ȃɂ����܂���
 */
CAttrCSDBDlg::CAttrCSDBDlg() 
	: ATTR_BASE_CLASS(DEFAULTMAP, IDD_CSDBDLG)
{
	m_lPosition = -1;
}
/**
 * �_�C�A���O�̃T�C�Y�ύX���������Ƃ��̃C�x���g�n���h��
 */
LRESULT CAttrCSDBDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RefreshGrid();
	bHandled = FALSE;

	return 0;
}

/** 
 * @fn HWND CAttrCSDBDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCSDBDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��ATTR_BASE_CLASS�ɔC���܂�
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCSDBDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCSDBDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CAttrCSDBDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCSDBDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	m_spGrid->put_Redraw( flexRDNone );
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	SetHiddenRow();			// [bug 7714] [e][�v�]]null�̍��ڂ��\���ɂ��ė~����

	long lCols = -1, lRows = -1;
	m_spGrid->get_Cols( &lCols );
	m_spGrid->put_ExtendLastCol( TRUE );
	m_spGrid->put_AutoSizeMode( flexAutoSizeColWidth );
	m_spGrid->AutoSize( 0, lCols -1 );
	m_spGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
	m_spGrid->AutoSize( 0, lCols -1 );

	m_spGrid->put_Ellipsis( flexEllipsisEnd );
	m_spGrid->put_Redraw( (RedrawSettings)VARIANT_TRUE );

	// GridEditMode����Ȃ���BaseDlg�Œe�����̂ł����ōĐݒ�
	m_spGrid->put_Editable(flexEDKbdMouse);

	return bRet;
}

/**
 * @fn void CAttrCSDBDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCSDBDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �W���O���b�h
		m_cControlRel.SetControl( IDC_GRIDCTRL,	TYPE_FEATUREGRID, 0, 0 );
	}
}

void CAttrCSDBDlg::SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList )
{ 
	// �Z�b�g�����FeatureDef�i�W�I���g���݂̂̃t�B�[�`���j������肵��
	// �Ή����鑮����ʃe�[�u��������������Ă��Ēu��������
	m_lstDmyFeatureDef.clear();
	for( std::list<CFeatureDef>::iterator it=pFeatureDefList->begin(); it!=pFeatureDefList->end(); ++it )
	{
		IFeaturePtr ipFeature = it->GetFeature();
		IFieldsPtr ipFields;
		ipFeature->get_Fields( &ipFields );
		long lIndex = 0;

		ITablePtr ipTable;
		ipFeature->get_Table( &ipTable );
		if( !ipTable )
			continue;

		IDatasetPtr ipDataSet( ipTable );
		if( !ipDataSet )
			continue;

		CComBSTR cComBSTR;
		ipDataSet->get_Name( &cComBSTR );
		CString strFeatureName( cComBSTR );
		long ldot = strFeatureName.ReverseFind( '.' );
		if( ldot != -1 )
			strFeatureName = strFeatureName.Mid( ldot+1 );

		// CSDB��NAVI�̏ꍇ�ŏ����𕪂���
		CString strTableName;
		if( strFeatureName.CompareNoCase( _T("CSDB") ) == 0 )
			strTableName.Format( _T("ATTR_CSDB") );
		else if( strFeatureName.CompareNoCase( _T("NAVI") ) == 0 )
			strTableName.Format( _T("ATTR_NAVI") );

		ipFields->FindField( CComBSTR("ATTRTBLID"), &lIndex );

		// CSDB�̃t�B�[�`���N���X�̎����������e�[�u����ǂ݂ɂ���
		if( lIndex > -1)
		{
			CComVariant vaVal;
			ipFeature->get_Value( lIndex, &vaVal );

			// �����e�[�u�����J����ID�ɑΉ����鑮�����擾
			ITablePtr ipTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), strTableName );
			if( !ipTable )
			{
				CString msg;
				msg.Format( AheLoadString( IDS_NOTFINDTABLE ), strTableName );
				AttrMessageBox( msg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				m_lstDmyFeatureDef = *pFeatureDefList;
				break;
			}
			_IRowPtr ipAttrRow;
			ipTable->GetRow( vaVal.lVal, &ipAttrRow );

			CFeatureDef featDef;
			featDef.clear();
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipAttrRow );

			// ���̊֐��𔲂��Ă����̂�ێ����Ă��Ȃ��Ƃ����Ȃ��̂Ń����o�ϐ����g��
			m_lstDmyFeatureDef.push_back( featDef );
		}
	}

	ATTR_BASE_CLASS::SetFeatureDefList( &m_lstDmyFeatureDef ); 
}

void CAttrCSDBDlg::SetAliasOrField( BOOL bAliasOrField )
{
	for( std::list<CFeatureDef>::iterator it = m_lstDmyFeatureDef.begin(); it != m_lstDmyFeatureDef.end(); ++it )
	{
		// ��ɃG�C���A�X�\��
		m_cControlRel.SetAliasOrField( FALSE );
	}
}

void CAttrCSDBDlg::CreateAliasMap( _IRowPtr ipRow )
{
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	long lFields = 0;
	ipFields->get_FieldCount( &lFields );
	for( int i=0; i<lFields; ++i)
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName, bstrAlias;
		ipField->get_Name( &bstrFieldName );
		ipField->get_AliasName( &bstrAlias );
		m_mapDB2Alias[ CString(bstrFieldName) ] = CString( bstrAlias );
		m_mapAlias2DB[ CString(bstrAlias) ] = CString( bstrFieldName );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// �O���b�h�R���g���[���p
//
/////////////////////////////////////////////////////////////////////////////

HRESULT CAttrCSDBDlg::CreateActiveXControls(UINT nID)
{
	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*) ::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = GetWindow(GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData; 
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							MapDialogRect(&rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(m_hWnd, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								static WCHAR pwchLicenseKey[] =
								{
									0x004F,	0x0030,	0x0045,	0x0054,	0x0054,	0x0043,	
									0x0037,	0x0034,	0x0037
								};
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, /*bstrLicKey*/CComBSTR(pwchLicenseKey));
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}

// [bug 7714] [e][�v�]]null�̍��ڂ��\���ɂ��ė~����
void CAttrCSDBDlg::SetHiddenRow()
{
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( !pFeatureDef )
		return;

	_IRowPtr ipCurrent = pFeatureDef->GetFeature();
	if( !ipCurrent )
		return;

	ITablePtr ipCurrentTable;
	ipCurrent->get_Table( &ipCurrentTable );
	if( !ipCurrentTable )
		return;

	IDatasetPtr ipDataSet( ipCurrentTable );
	if( !ipDataSet )
		return;

	CComBSTR cComBSTR;
	ipDataSet->get_Name( &cComBSTR );
	CString strFeatureName( cComBSTR );
	long ldot = strFeatureName.ReverseFind( '.' );
	if( ldot != -1 )
		strFeatureName = strFeatureName.Mid( ldot+1 );

	// CSDB��NAVI�̏ꍇ�ŏ����𕪂���
	long lChangeCase = 0;
	if( strFeatureName.CompareNoCase( _T("ATTR_CSDB") ) == 0 )
		lChangeCase = 0;		// CSDB�̏ꍇ 0
	else if( strFeatureName.CompareNoCase( _T("ATTR_NAVI") ) == 0 )
		lChangeCase = 1;		// NAVI�̏ꍇ 1
	else
		return;

	// �ʒu�ԍ�
	for( long i=1; i<=10; ++i )
	{
		std::list<CString> strList;
		CString strLOC;

		strLOC.Format( _T("LOC%02d_NAME"), i );
		strList.push_back( strLOC );
		strLOC.Format( _T("LOC%02d_LAT"), i );
		strList.push_back( strLOC );
		strLOC.Format( _T("LOC%02d_LONG"), i );
		strList.push_back( strLOC );

		_SetHiddenRow( ipCurrent, strList );

	}
	// �񎟒���02 or �񓚁E����
	for( long i=1; i<=10; ++i )
	{
		std::list<CString> strList;
		CString strINQ;

		switch( lChangeCase )
		{
			case 0:	// ATTR_CSDB
			default:
				strINQ.Format( _T("INQ2_%02dFCT"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ2_%02dDTL"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ2_%02dTGT"), i );
				strList.push_back( strINQ );
				break;
			case 1: // ATTR_NAVI
				strINQ.Format( _T("INQ_%02dFCT"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ_%02dDTL"), i );
				strList.push_back( strINQ );
				strINQ.Format( _T("INQ_%02dTGT"), i );
				strList.push_back( strINQ );
				break;
		}

		_SetHiddenRow( ipCurrent, strList );

	}
}

void CAttrCSDBDlg::_SetHiddenRow(_IRowPtr ipRow, std::list<CString>& strList)
{
	if( !ipRow )	return;

	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	if( !ipTable )	return;

	std::vector<long> vecHidden;
	// ���X�g���t�B�[���h�l�����ׂ�null�̎��̂�
	for( std::list<CString>::iterator it = strList.begin(); it != strList.end(); ++it )
	{
		long lidx = 0;
		if( FAILED( ipTable->FindField( CComBSTR( (*it) ), &lidx ) ) )
			return;

		CComVariant cVal;
		if( FAILED( ipRow->get_Value( lidx, &cVal ) ) )
			return;
		
		if( cVal.vt != VT_NULL )
			return;

		vecHidden.push_back( lidx+1 );
	}

	// �O���b�h�̍s���\���ɂ���
	for( std::vector<long>::iterator it = vecHidden.begin(); it != vecHidden.end(); ++it )
		m_spGrid->put_RowHidden( (*it), VARIANT_TRUE );
}
