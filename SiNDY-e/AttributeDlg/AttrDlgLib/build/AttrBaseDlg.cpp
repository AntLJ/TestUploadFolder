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

// AttrBaseDlg.cpp : CAttrBaseDlg �̃C���v�������e�[�V����
#include "stdafx.h"
#include "../FeatureDef.h"
#include "../AttrBaseDlg.h"

#include "ArcHelper.h"
#include "../../../LIB/WinLib/charutil.h"
#include <Uxtheme.h> // ComboBox��VisualStyle�𖳌��ɂ��邽�߂ɕK�v
#pragma comment(lib,"UxTheme.lib")
/////////////////////////////////////////////////////////////////////////////
// CAttrDefaultDlg

// �R���{�{�b�N�X�A�^�u�R���g���[����VisualStyle�𖳌��ɂ���R�[���o�b�N�֐�
BOOL CALLBACK EnumChildProc( HWND hWnd, LPARAM lParam )
{
	TCHAR className[20] = _T("");
	GetClassName( hWnd, className, 19 );
	if( 0 == lstrcmpi( _T("ComboBox"), className ) ||
		0 == lstrcmpi( _T("SysTabControl32"), className ) )
		SetWindowTheme (hWnd, TEXT (" "), TEXT (" "));

	return TRUE;
}

CAttrBaseDlg::CAttrBaseDlg( WINRECT* pWinMap, WORD resourceID )
	: m_winMgr(pWinMap), IDD(resourceID)
{
	m_bFinishInit = TRUE;

	m_hChangedBrush = CreateSolidBrush( RGB(255,168,126) );
	m_hErrorBrush = CreateSolidBrush( RGB(255,0,0) );

	//�@�R���{�{�b�N�X��Disable�̎��̔w�i�F�̐ݒ�����W�X�g������擾
	m_lComboR = 214;
	m_lComboG = 211;
	m_lComboB = 206;
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		DWORD dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboR"), &dwResult, hOpenKey ) )
			m_lComboR = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboG"), &dwResult, hOpenKey ) )
			m_lComboG = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboB"), &dwResult, hOpenKey ) )
			m_lComboB = (long)dwResult;
		RegCloseKey( hOpenKey );
	}
	m_hBrushCombo = CreateSolidBrush(RGB(m_lComboR,m_lComboG,m_lComboB));

}

LRESULT CAttrBaseDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �E�B���h�E�}�l�[�W���ɂ��Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
	
	// ComboBox��VisualStyle���K�p�����ƁA�h���b�v�_�E�����X�g�̏ꍇ��
	// �w�i�F��ύX���邱�Ƃ��ł��Ȃ��B���̂��߁A�R���{�{�b�N�X�̂�VislalStyle
	// �𖳌��ɂ���ibug 9602�j
	EnumChildWindows( m_hWnd, EnumChildProc, 0L );

	// �������I��
	m_bFinishInit = TRUE;

	return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

/**
 * �E�B���h�E�}�l�[�W���p�C�x���g�n���h��
 */
LRESULT CAttrBaseDlg::OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLASSERT(lParam);
	NMWINMGR& nmw = *(NMWINMGR*)lParam;

	return 0;
}
/**
 * �_�C�A���O�̍ő�E�ŏ��������C�x���g�n���h��
 */
LRESULT CAttrBaseDlg::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>( lParam);

	m_winMgr.GetMinMaxInfo( this, pMinMaxInfo);
	bHandled = TRUE;

	return 0;
}
/**
 * �_�C�A���O�̃T�C�Y�ύX���������Ƃ��̃C�x���g�n���h��
 */
LRESULT CAttrBaseDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT iWidth	= LOWORD(lParam);
	INT iHeight = HIWORD(lParam);

	m_winMgr.CalcLayout(iWidth, iHeight, this);
	m_winMgr.SetWindowPositions(this);

	bHandled = FALSE;

	return 0;
}
/**
 * TAB�L�[�ł̃R���g���[���ړ��A�y�і��L�[�̏㉺�ŃR���{�{�b�N�X�̃A�C�e���̕ύX�̃G�~�����[�g
 */
LRESULT CAttrBaseDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_TAB )
	{
		UINT uiShiftState = GetAsyncKeyState(VK_SHIFT);
		UINT uiControlState = GetAsyncKeyState(VK_CONTROL);

		if( (_rotl(uiControlState, 1 ) & 0x01) == 0x01 )
		{
			if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
				PostMessage( WM_NOTIFY, 0, -1 );
			else
				PostMessage( WM_NOTIFY, 0, -2 );
		}
		else
		{
			if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
				PrevDlgCtrl();
			else
				NextDlgCtrl();
		}

		bHandled = TRUE;
		return 1;
	}
	else if( wParam == VK_RETURN )
	{
		// �ҏW�J�n��Ԃ��擾
		esriEditState state;
		IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
		ipEditor->get_EditState( &state );

		if( state != esriStateNotEditing )
		{
			// ���^�[���L�[�������ꂽItem���X�V����
			CWindow wnd = GetFocus();
			INT iWndID = wnd.GetDlgCtrlID();
			m_cControlRel.ControlChanged(iWndID);

			if( !CheckReturnKeyItem(iWndID) ) {		// �G���^�[�L�[�������ꂽItem�̃`�F�b�N
				return 1;				// �G���[�E��������̏ꍇ�A�ȍ~�̏����𔲂���
			}

			InvalidateRect( NULL, FALSE );

			::PostMessage( GetParent(), WM_CLOSE, 0, -1);
			bHandled = TRUE;
			return 1;
		}
	}
	else if( wParam == VK_ESCAPE )
	{
		// ���^�[���L�[�������ꂽItem���X�V����
		CWindow wnd = GetFocus();
		INT iWndID = wnd.GetDlgCtrlID();
		if( !CheckEscKeyItem(iWndID) ){
			return 1;
		}
		::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
		bHandled = TRUE;
		return 1;			
	}

	bHandled = FALSE;	// �����I�ɐݒ肷��K�v�A��
	return 0;
}

// ���[�_���̎��ɃL�[��߂܂���
LRESULT CAttrBaseDlg::OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}

BOOL CAttrBaseDlg::IsStartEditing()
{
	esriEditState state;

	IEditorPtr ipEditor = m_cArcHelper.GetEditor();
	ipEditor->get_EditState( &state );
	if( state != esriStateEditing )
		return FALSE;
	else
		return TRUE;
}

BOOL CAttrBaseDlg::IsEditableFeature()
{
	sindyeEditableType enumEditableType = sindyeEditableNoDefine;
	CComBSTR bstrMsg;
	BOOL bEditable = FALSE;

	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( pFeatureDef != NULL )
	{
		IFeaturePtr ipFeature = pFeatureDef->GetFeature();
		IGeometryPtr ipGeometry;

		IWorkspacePtr ipWorkspace;
		ITablePtr ipTable;
		ipFeature->get_Table( &ipTable );
		IDatasetPtr ipDataset( ipTable );
		if( !m_ipRule )
			m_ipRule = GetRule();
		if( ipDataset )
			ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			_ASSERTE( ipWorkspace != NULL );
		else {
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
			if(ipWorkspaceEdit != 0) {
				ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited );
				if( vbIsBeingEdited == VARIANT_TRUE )
				{
					ipFeature->get_ShapeCopy( &ipGeometry );
					m_ipRule->IsEditableFeature( ipFeature, ipGeometry, sindyeEditAttribute, &enumEditableType, &bstrMsg );
					switch( enumEditableType )
					{
						case sindyeEditableEditable: 
							bEditable = TRUE; 
							break;
						case sindyeEditableWarnning:
							if( pFeatureDef->GetWarnDelete() )
								bEditable = TRUE;
							break;
						default:
							break;
					}
				}
			}
		}
	}
	
	return bEditable;	
}


void CAttrBaseDlg::SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList )
{
	m_pFeatureDefList = pFeatureDefList;
	m_cControlRel.SetFeatureDefList( pFeatureDefList );
	m_lFeatureDefIndex = 0;
}

BOOL CAttrBaseDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, INT view, BOOL bForce, BOOL bEditable )
{ 
	m_bFinishInit = FALSE;

	m_cControlRel.SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex ); 
	m_cControlRel.Visible( view, bForce, bEditable );

	m_lFeatureDefIndex = lFeatureIndex;

	m_bFinishInit = TRUE;
	
	return TRUE;
}

/**
 * ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 *
 * @return ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 */
CFeatureDef* CAttrBaseDlg::GetCurrentFeatureDef()
{
	_ASSERT( m_pFeatureDefList );
	_ASSERT( !m_pFeatureDefList->empty() );

	std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
	advance( it, m_lFeatureDefIndex );

	return &(*it);
}
/**
 * �ύX�����邩�ǂ����̔���
 *
 * return �ύX������ꍇ�� TRUE�A�Ȃ���� FALSE
 */
BOOL CAttrBaseDlg::Changed()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( it->Changed() ) 
			return TRUE;
	}

	return FALSE;
}

BOOL CAttrBaseDlg::SetButton( BOOL bChanged )
{
	::SendMessage( GetParent(), WM_FEATURECHANGED, NULL, (LONG)bChanged );

	return TRUE;
}

BOOL CAttrBaseDlg::CheckAllItemsError()
{
	return m_cControlRel.CheckError();
}

BOOL CAttrBaseDlg::CheckReturnKeyItem(INT nClassID)
{
	return TRUE;
}

BOOL CAttrBaseDlg::CheckEscKeyItem(INT nClassID)
{
	return TRUE;
}

BOOL CAttrBaseDlg::ErrorCheck()
{
	return TRUE;
}

/**
 * �w��G�f�B�b�g�{�b�N�X��S�I����Ԃɂ���
 *
 * @param
 *
 * @return	TRUE : ����
 */
VOID CAttrBaseDlg::SelectDlgItem(INT nClassID)
{
	CEdit	cEdit(GetDlgItem(nClassID));
	cEdit.SetSel(0,-1,TRUE);

    LPTSTR lpszString = (LPTSTR)_alloca((cEdit.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< ���������������m��
    cEdit.GetWindowText( lpszString, cEdit.GetWindowTextLength() + 1 );

	if(lstrcmpi(lpszString, _T("<NULL>")) == 0)
	{
		cEdit.SetWindowText(_T(""));
	}
}

/**
 * IME�𐧌䂷��
 * 
 * @param bIsOn		[in]	IME����t���O True : On , False : OFF
 */
void CAttrBaseDlg::SetIMEMode(BOOLEAN bIsOn)
{
	HIMC hImc;
	hImc = ::ImmGetContext( m_hWnd );
	if( hImc )
	{
		::ImmSetOpenStatus( hImc, bIsOn );
		::ImmReleaseContext( m_hWnd, hImc );
	}
}

void CAttrBaseDlg::SetComboboxList(INT nClassID)
{
	CComboBox cCombo(GetDlgItem(nClassID));
	LONG lListNum = cCombo.GetCount();

	if( lListNum >= 20 )
	{
		RECT rect;
		INT i = cCombo.GetWindowRect(&rect);
		int nHeight = cCombo.GetItemHeight(0);
		int nWinWidth = rect.right - rect.left;
		int nWinHeight = rect.bottom - rect.top + nHeight * 21;
		cCombo.SetWindowPos(NULL, 0, 0, nWinWidth, nWinHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
		cCombo.ShowWindow(SW_SHOW);
	}
}

void CAttrBaseDlg::SetDlgStatus(INT nID, BOOL bIsEnable, BOOL bIsEditBox)
{
	if( !bIsEditBox )
	{
		// �R���{�{�b�N�X�ɂ̓��[�h�I�����[�������Ȃ����߁AENABLE��DISABLE���ŃZ�b�g����K�v������
		::EnableWindow( GetDlgItem( nID ), bIsEnable);
	}
	else
	{
		if( bIsEnable )
			::SendMessage(  GetDlgItem( nID ), EM_SETREADONLY, FALSE, 0L );
		else
			::SendMessage(  GetDlgItem( nID ), EM_SETREADONLY, TRUE, 0L );
	}
}

int CAttrBaseDlg::AttrMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel )
{
	int iRet = AheMessageBox( NULL, lpcszMsg, lpcszCap, uType|MB_TASKMODAL, uLevel );
	return iRet;
}

/**
 * �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ�l���Ď�����
 * 
 * @param hWndCtl		[in]	�Ώۂ̃G�f�B�b�g�{�b�N�X�̃E�B���h�E�n���h��
 * @param nID	        [in]	�Ώۂ̃R���g���[����ID
 */
BOOL CAttrBaseDlg::CheckInputValue( HWND hWndCtl, WORD wID )
{
	if( ::IsWindow( hWndCtl ) )
	{
		CEdit edit( hWndCtl );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		// <null> �̏ꍇ���l��
		if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
		{
			CString strText( lpszText );
			int iLen = CString_GetLength2( strText );
			CString strRet;

			USES_CONVERSION;

			for( int i = 0; i < iLen; i++ )
			{
				CString str = CString_GetAt2(strText, i);
				LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

				// �s����������������������쐬
				// 1�����ڂ̓}�C�i�X������
				if( i==0 )
				{
					if( ( isasciiminus( lpstr[0] ) || isasciinumber( lpstr[0] ) ) )
						strRet += A2CT( lpstr );
				}
				// 2�����ڈȍ~�͐����̂݋���
				else
				{
					if( isasciinumber( lpstr[0] )  )
						strRet += A2CT( lpstr );
				}
			}

			// �s���������܂܂�Ă����ꍇ
			int lCheckLen = CString_GetLength2( strRet );
			if( iLen != lCheckLen )
			{
				INT dwSelStart = 0, dwSelEnd = 0;
				edit.GetSel( dwSelStart, dwSelEnd );
				edit.SetWindowText( strRet );
				// ���͈ʒu�̏C��
				INT nDiff = CString( lpszText ).GetLength() - strRet.GetLength();
				edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				return FALSE;
			}
		}
		m_cControlRel.ControlChanged( wID );
		SetButton( Changed() );
		InvalidateRect( NULL, FALSE );
	}
	return TRUE;
}

// BACKGROUND_CLASS���烋�[�����m�ۂ���
void CAttrBaseDlg::LoadBackgroundRule()
{
	// BACKGROUND_CLASS�擾
	IWorkspacePtr ipWorkspace;
	GetRule()->GetWorkspace( &ipWorkspace );
	IFeatureWorkspacePtr ipFeatureWorkspace( ipWorkspace );
	if( !ipWorkspace )
		return;

	IWorkspaceFactoryPtr ipWorkspaceFactory;
	ipWorkspace->get_WorkspaceFactory( &ipWorkspaceFactory );
	esriWorkspaceType type = esriFileSystemWorkspace;
	ipWorkspaceFactory->get_WorkspaceType( &type );
	CString strBGTableName;
	if( type == esriRemoteDatabaseWorkspace )
		strBGTableName.Format(_T("REFERENCE.%s"), REF_BACKGROUND_TABLE_NAME );	// REFERENCE.������
	else
		strBGTableName.Format(_T("%s"), REF_BACKGROUND_TABLE_NAME );	

	ITablePtr ipBackgroundTable;
	ipFeatureWorkspace->OpenTable( _bstr_t( strBGTableName ), &ipBackgroundTable );
	if( !ipBackgroundTable )
		return;

	// �w�i���C���R�[�h2�h���C������A�����̃��C���R�[�h���擾����
	IFieldPtr ipField;
	AheGetFieldByFieldName( ipBackgroundTable, _T("LAYERID"), &ipField );
	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	ICodedValueDomainPtr ipCodedValueDomain = ipDomain;
	long lLayerID = -1, lCount = -1;
	ipCodedValueDomain->get_CodeCount( &lCount );
	for( int i = 0; i < lCount; ++i )
	{
		CComBSTR bstrName;
		ipCodedValueDomain->get_Name( i, &bstrName );
		// �������g�̃��C���R�[�h��������
		if( m_strClassName.CompareNoCase( bstrName ) == 0 )
		{
			CComVariant vaValue;
			ipCodedValueDomain->get_Value( i, &vaValue );
			lLayerID = vaValue.lVal;
			break;
		}
	}
	
	// ���̊֐����ĂԍۂɁA�Ȃ��͂��͂Ȃ�
	if( lLayerID < 0 )
		return;

	// LAYERID=0�́A���ׂẴ��C���ŋ��ʂ̌��e�ԍ�
	CString strWhere;
	strWhere.Format( _T("LAYERID = 0 OR LAYERID = %ld"), lLayerID );
	_ICursorPtr ipCursor;
	ipBackgroundTable->Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE, &ipCursor );
	
	if( !ipCursor )
		return;

	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow )
	{
		BACKGROUND_RULE br = {0};
		br.lClass = AheGetAttribute( ipRow, BACKGROUND_CLASS ).lVal;
		br.strClass_Name = AheGetAttribute( ipRow, BACKGROUND_CLASS_NAME ).bstrVal;
		br.strMS_NO = AheGetAttribute( ipRow, BACKGROUND_MS_NO ).bstrVal;
		br.lLayerID = AheGetAttribute( ipRow, BACKGROUND_LAYERID ).lVal;
		m_BackgroundRule.push_back(br);
	}
	return;
}

// �R�[�h�l���猴�e�ԍ����擾����
CString CAttrBaseDlg::GetMSNOByClass(long lClass)
{
	if( m_BackgroundRule.empty() )
		LoadBackgroundRule();
	for ( auto &rule : m_BackgroundRule )
	{
		if (lClass == rule.lClass)
			return rule.strMS_NO;
	}

	return _T("");
}

// ���e�ԍ�����R�[�h�l���擾����
long CAttrBaseDlg::GetClassByMSNO(CString strMSNO)
{
	if( m_BackgroundRule.empty() )
		LoadBackgroundRule();
	for ( auto &rule : m_BackgroundRule )
	{
		if (lstrcmpi(strMSNO, rule.strMS_NO) == 0) 
			return rule.lClass;
	}

	return -1;
}
