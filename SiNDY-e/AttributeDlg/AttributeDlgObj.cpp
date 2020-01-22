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

/*! \file AttributeDlgObj.cpp
	\brief CAttributeDlgObj �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2001-09-07
*/
#include "stdafx.h"
#include "AttributeDlg.h"
#include "AttributeDlgObj.h"
#include <math.h>
#include "charutil.h"
#ifdef _USE_CHILDDLG_SHORTCUT
#include "shlobj.h"
_COM_SMARTPTR_TYPEDEF(IShellLink, __uuidof(IShellLink));
#endif // ifdef _USE_CHILDDLG_SHORTCUT
#include "type_convert.h"
#include "local_define.h"

using namespace sindy::schema;

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_DISABLECONTROL  = RegisterWindowMessage(_T("WM_DISABLECONTROL"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

typedef CExportDlg* __stdcall CreateDlg();
typedef CExportDlg* __stdcall CreateDlgByName( LPCTSTR lpcszTableName );

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�x�[�X�p�j
BEGIN_WINDOW_MAP(BASEMAP)
	BEGINROWS(WRCT_REST,0,0)
		BEGINCOLS(WRCT_REST,0,0)
			BEGINROWS(WRCT_TOFIT,0,RCMARGINS(0,0))
				RCREST(IDC_FEATTREE)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					RCTOFIT(IDC_BUTTON_GRIDMODE)
					RCTOFIT(IDC_BUTTON_ALIASMODE)
				ENDGROUP()
			ENDGROUP()
			RCFIXED(ID_WIN_SIZEBAR,4)
			BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
				RCREST(IDC_STATIC_BASE)
#ifdef _FOR_ENGLISH		// [bug 11183]�ʏ�̏ꍇ�A���̕\���͗v��Ȃ�
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,4))
					RCSPACE(4)
					BEGINCOLS(WRCT_PCT, 0, 50 )
						RCREST(IDC_STATIC_OFFICIALNAME)
					ENDGROUP()
					RCSPACE(4)
					BEGINCOLS(WRCT_PCT, 0, 50 )
						RCREST(IDC_STATIC_ALTERNATIVENAME)
					ENDGROUP()
					RCSPACE(4)
				ENDGROUP()
#endif
			ENDGROUP()
		ENDGROUP()
		RCSPACE(4)
		RCTOFIT(-1)
		BEGINCOLS(WRCT_TOFIT,0,4)
			RCTOFIT(IDC_BUTTON_DELETEWARN)
			RCREST(IDC_STATIC_MESSAGE)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_CONFIRMATION)
			RCSPACE(4)
			RCTOFIT(IDOK)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_CLOSE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

#ifdef _USE_CHILDDLG_SHORTCUT
HMODULE LoadShellLinkLibrary( LPCTSTR lpcszFileName )
{
	HMODULE hMod = NULL;

	HRESULT hr = 0;
	if( lpcszFileName )
	{ 
		CString strFileName( lpcszFileName );
		// �V���[�g�J�b�g�����邩�ǂ������Ɋm�F����
		IPersistFilePtr ipFile;
		if( SUCCEEDED( ipFile.CreateInstance( CLSID_ShellLink ) ) )
		{
			// �V���[�g�J�b�g�t�@�C���͂ǂ�Ȗ��O�ɂ��Ă��K���u.lnk�v�����ɂ��炵���i�Ȃ񂶂Ⴛ���I�I�j
			// �Ⴆ�΁A�G�N�X�v���[����ŁuCITY_DISP_LINE.dll�v�Ƃ����t�@�C�����̃V���[�g�J�b�g������Ȃ�A����́uCITY_DISP_LINE.dll.lnk�v����Ȃ��ƃI�[�v���ł��Ȃ�
			if( SUCCEEDED( hr = ipFile->Load( CComBSTR(strFileName + _T(".lnk")), STGM_READ | STGM_SHARE_DENY_WRITE ) ) )
			{
				IShellLinkPtr ipLink;
				if( SUCCEEDED( ipFile->QueryInterface( IID_IShellLink, (void**)&ipLink ) ) )
				{
					TCHAR strShortPath[MAX_PATH + 1], strPath[MAX_PATH + 1];
					WIN32_FIND_DATA wfd = {0};
					if( SUCCEEDED( ipLink->GetPath( strShortPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_SHORTPATH ) ) )
					{
						DWORD dwRet = GetLongPathName( strShortPath, strPath, MAX_PATH );
						if( 0 < dwRet && dwRet < MAX_PATH )
						{
							strFileName = strPath;
						}
					}
				}
			}
		}
		hMod = ::LoadLibrary( strFileName );
	}

	return hMod;
}
#endif // #ifdef _USE_CHILDDLG_SHORTCUT


// �_�C�A���O������������C�x���g�n���h��
LRESULT CAttributeDlgObj::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//[Bug 4694]smw�Ń_�C�A���O�쐬�����ƁA�J�[�\���n���h����NULL�ƂȂ��Ă��܂��̂ŁA
	//�Ƃ肠�����A�J�[�\�����蓖�ĂĂ����B�{���Ȃ��smw���Őݒ肳��Ă����ׂ�
	SetClassLongPtr(m_hWnd, GCLP_HCURSOR, LONG_PTR( LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))));
	m_bClosedDialog = FALSE;
	// �c���[�T�u�N���X��
	m_cTreeCtrl.SubclassWindow( GetDlgItem( IDC_FEATTREE ) );
	m_cTreeCtrl.ModifyStyle( NULL, TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS );

	// �T�u�N���X��
	m_cTree.SubclassWindow( GetDlgItem( IDC_FEATTREE ) );
	m_cBtn1.SubclassWindow( GetDlgItem( IDC_BUTTON_GRIDMODE ) );
	m_cBtn2.SubclassWindow( GetDlgItem( IDC_BUTTON_ALIASMODE ) );
	m_cBtnClose.SubclassWindow( GetDlgItem( IDC_BUTTON_CLOSE ) );
	m_cBtnSave.SubclassWindow( GetDlgItem( IDOK ) );
	m_cBtnDelWarn.SubclassWindow( GetDlgItem( IDC_BUTTON_DELETEWARN ) );
	m_cBtnConfirmation.SubclassWindow( GetDlgItem( IDC_BUTTON_CONFIRMATION ) );

	// �V����WinMgr�Ŏg�������ς�����H
	m_wndSizerBar.Create( WS_CHILD|WS_VISIBLE|SS_NOTIFY, *this, m_winMgr, ID_WIN_SIZEBAR );

	// �{�^���Ƀr�b�g�}�b�v���蓖��
	CButton cAliasButton = GetDlgItem( IDC_BUTTON_ALIASMODE );
	CButton cGridButton  = GetDlgItem( IDC_BUTTON_GRIDMODE  );
	cAliasButton.SetBitmap( (m_bIsDBNameMode == TRUE ) ? m_hDBNameModeBmp : m_hAliasModeBmp  );
	cGridButton.SetBitmap(  (m_bIsGridMode   == TRUE ) ? m_hGridModeBmp   : m_hCommonModeBmp );

	// �E�B���h�E�}�l�[�W���ɂ��Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	// �_�C�A���O�T�C�Y�ύX
	// ���̌n�R���g���[�����ŏc����+15���܂�
	if( m_cDlgRect.Width() > 0 && m_cDlgRect.Height() > 0 )
		MoveWindow( &m_cDlgRect );
	else
		// [bug 11183]���̕\���̗L���ɂ��A�����𒲐�����
#ifdef _FOR_ENGLISH
		ResizeClient( 550, 400, FALSE );
#else
		ResizeClient( 550, 370, FALSE );
#endif

	// ���[�_���̏ꍇ�̂�
	if( m_bIsModal )
	{
		// �c���[�Ƀt�B�[�`����o�^�i�\�������O�ɓo�^����K�v�A���j
		if( !m_bIsLQAttribute )
			m_cTreeCtrl.SetFeatures( &m_listFeatureDef );
		else
		{
			m_cTreeCtrl.SetLQs( &m_listLQRowDef );

			for( std::list<CLQRowDef>::iterator i = m_listLQRowDef.begin(); i != m_listLQRowDef.end(); i++ )
			{
				// �t�B�[�`���N���X���܂��̓I�u�W�F�N�g�N���X���̎擾
				// �����Ŏ擾�������O����A���C����.dll��ǂݍ��ނ��ƂɂȂ�܂��B
				CString strTableName;
				_IRowPtr ipRow = i->GetFeature();
				ITablePtr ipTable;
				IObjectClassPtr ipObjectClass;

				ipRow->get_Table( &ipTable );
				ipObjectClass = ipTable;
				if( ipObjectClass )
				{
					CComBSTR bstrTableName;
					CString	strTemp;
					ipObjectClass->get_AliasName( &bstrTableName );
					strTemp = bstrTableName;
					strTableName = strTemp.Right( strTemp.GetLength() - strTemp.Find( _T(".") ) - 1 ).MakeUpper();
				}

				std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( strTableName );
				if( itDlg == m_mapDlg.end() )
				{
					// �t�B�[�`���̎�ނɂ���ĕ\���`�Ԃ�ς���
					CExportDlg* cExportDlg = NULL;

					// ���C���ʂ̎q�_�C�A���O���擾
					cExportDlg = SetChildDlg(strTableName);
					if( !cExportDlg )	// �G�C���A�X�ő��݂��Ȃ� [bug 7516]
					{
						strTableName = AheGetFeatureClassName( ipRow );  // �e�[�u�������擾
						itDlg = m_mapDlg.find( strTableName );
						if( itDlg == m_mapDlg.end() )
						{
							cExportDlg = SetChildDlg(strTableName);
						}
					}
					if( cExportDlg )
					{
						cExportDlg->Create( m_hWnd );
						cExportDlg->SetFeatureDefList( &m_listLQRowDef );
						m_mapDlg[strTableName] = cExportDlg;
					}
				}
			}
		}
		// �_�C�A���O�Ƀt�B�[�`����o�^�im_listFeatureDef ���Z�b�g���ꂽ��j
		SetSelectedFeaturesToDlg();
	}

	m_bIsEnding = FALSE;

	// �L�����Z���{�^���t���O
	m_bIsCancel = FALSE;

	// �V�K�쐬�̎��͕K��sindyNotApplyRuleEnable�œn�����
	// ���̂Ƃ��̓f�t�H���g��OK�{�^����������悤�ɂȂ�Ȃ�
	// �Ƃ����Ȃ�
	if( sindyeNotApplyRuleEnable == m_sAttributeEditType )
		SetButton( TRUE );

	// �ҏW�J�n��Ԃ��擾
	esriEditState state = esriStateNotEditing;
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	ipEditor->get_EditState( &state );

	if( state != esriStateEditing )
		::SetFocus( GetDlgItem(IDC_BUTTON_CLOSE) );

	LoadProperty();

	// [bug 11183]���ʂ̏ꍇ�A���̕\���͗v��Ȃ�
#ifdef _FOR_ENGLISH
	static const std::vector<UINT> useFontEdits = boost::assign::list_of
		( IDC_STATIC_OFFICIALNAME )
		( IDC_STATIC_ALTERNATIVENAME );

	for( const auto& ctrl : useFontEdits )
	{
		SetLocalFont( GetDlgItem( ctrl ) );
	}
#else
	CStatic cStaticOfficial( GetDlgItem( IDC_STATIC_OFFICIALNAME ) );
	CStatic cStaticAlternative( GetDlgItem( IDC_STATIC_ALTERNATIVENAME ) );

	cStaticOfficial.ShowWindow(SW_HIDE);
	cStaticAlternative.ShowWindow(SW_HIDE);
#endif

	return 0;
}

// �L�[�������ꂽ�Ƃ��p�C�x���g�n���h��
LRESULT CAttributeDlgObj::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_TAB )
	{
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��
		UINT uiKeyState = GetAsyncKeyState(VK_SHIFT);
		if( (_rotl(uiKeyState, 1 ) & 0x01) == 0x01 )
			PrevDlgCtrl();
		else
			NextDlgCtrl();

		bHandled = TRUE;
		return 1;
	}
	else if( wParam == VK_RETURN )
	{
		CWindow	wnd = GetFocus();
		INT iWndID = wnd.GetDlgCtrlID();

		if ( iWndID == IDC_BUTTON_CLOSE )
		{
			m_bIsCancel = TRUE;
			// �L�����Z���{�^�����������Ɠ�������
			CloseDlg( FALSE );
			bHandled = TRUE;
			return 1;
		}
		else
		{
			CloseDlg( TRUE );
			bHandled = TRUE;
			return 1;
		}
	}
	else if( wParam == VK_ESCAPE )
	{
		m_bIsCancel = TRUE;
		// �L�����Z���{�^�����������Ɠ�������
		CloseDlg( FALSE );
		bHandled = TRUE;
		return 1;
	}

	bHandled = FALSE;
	return 0;
}

// �E�B���h�E�}�l�[�W���p�C�x���g�n���h��
LRESULT CAttributeDlgObj::OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bIsEnding == TRUE ) return 0;	// �I���������Ȃ�S�ď����L�����Z��

	NMWINMGR& nmw = *(NMWINMGR*)lParam;

	if (nmw.code==NMWINMGR::GET_SIZEINFO) 
	{
		if (wParam==IDC_FEATTREE) 
		{
			CWindow wndTree = GetDlgItem(IDC_FEATTREE);
			CRect rect;
			wndTree.GetWindowRect(&rect);
			nmw.sizeinfo.szDesired = rect.Size();

			bHandled = TRUE; // handled--important!
			return -1;
		}
	} 
	else if( nmw.code == NMWINMGR::SIZEBAR_MOVED) 
	{
		m_winMgr.MoveRect( wParam, nmw.sizebar.ptMoved, this );
		m_winMgr.SetWindowPositions(this);

		ResizeClientDlg();

		bHandled = TRUE;
		return -1;
	}

	return 0;
}

// �_�C�A���O�̍ő�E�ŏ��������C�x���g�n���h��
LRESULT CAttributeDlgObj::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>( lParam);

	m_winMgr.GetMinMaxInfo( this, pMinMaxInfo);
	bHandled = TRUE;
	return 0;
}

// �_�C�A���O�̃T�C�Y�ύX���������Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT iWidth	= LOWORD(lParam);
	INT iHeight = HIWORD(lParam);

	m_winMgr.CalcLayout(iWidth, iHeight, this);
	m_winMgr.SetWindowPositions(this);
	ResizeClientDlg();

	bHandled = TRUE;
	return 0;
}

// * �_�C�A���O�����ۂ̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( lParam == -1 )
		CloseDlg( TRUE );			// �ۑ��I��
	else
		CloseDlg( FALSE );			// �j���I��

	return 0;
}

// �uOK�v�{�^���������ꂽ�Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CloseDlg( TRUE );				// �ۑ��I��
	return 0;
}

// �L�����Z���v�{�^���������ꂽ�Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnCloseClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bIsCancel = TRUE;
	CloseDlg( FALSE );				// �j���I��
	return 0;
}

// �c���[�r���[�Ńt�B�[�`���I�����ύX���ꂽ���̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( lParam == CFeatureTree::FEATUREITEM || lParam == CFeatureTree::OUTERROWITEM )
		SetSelectedFeaturesToDlg();

	return 0;
}

// WM_NOTIFY �p�C�x���g�n���h��
LRESULT CAttributeDlgObj::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bIsEnding ) return 0;	// �I���������Ȃ�S�ď����L�����Z��

	 if( wParam == IDC_FEATTREE ) {
		NMTVCUSTOMDRAW*  lpCustomDraw = (NMTVCUSTOMDRAW*)lParam;
		if( lpCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT )
		{
			//  �A�C�e���̕`��ł���悤�ɂ���
			bHandled = TRUE;
			return CDRF_NOTIFYITEMDRAW;
		}
		if( lpCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )
		{
			if( !m_bIsLQAttribute )
			{
				UINT pFlags = 0;
				CRect rect = lpCustomDraw->nmcd.rc;
				HTREEITEM item = m_cTreeCtrl.HitTest( rect.TopLeft(), &pFlags );
				if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::FEATUREITEM ) {
					CFeatureDef* pFeatureDef = m_cTreeCtrl.GetFeatureDef( item );
					if( pFeatureDef )
					{
						if( pFeatureDef->Changed() )
						{
							//  �e�L�X�g�F�ݒ�
							lpCustomDraw->clrText = RGB(0,0,0);
							//  �w�i�F�ݒ�
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				} else if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::OUTERROWITEM ) {
					CRowDef* pRowDef = m_cTreeCtrl.GetRowDef( item );
					if( pRowDef )
					{
						if( pRowDef->Changed() )
						{
							//  �e�L�X�g�F�ݒ�
							lpCustomDraw->clrText = RGB(0,0,0);
							//  �w�i�F�ݒ�
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				}
			}
			else
			{
				UINT pFlags = 0;
				CRect rect = lpCustomDraw->nmcd.rc;
				HTREEITEM item = m_cTreeCtrl.HitTest( rect.TopLeft(), &pFlags );
				if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::FEATUREITEM ) {
					CLQRowDef* pLQRowDef = m_cTreeCtrl.GetLQRowDef( item );
					if( pLQRowDef )
					{
						if( pLQRowDef->Changed() )
						{
							//  �e�L�X�g�F�ݒ�
							lpCustomDraw->clrText = RGB(0,0,0);
							//  �w�i�F�ݒ�
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
						if( pLQRowDef->m_bError )
						{
							//  �e�L�X�g�F�ݒ�
							lpCustomDraw->clrText = RGB(0,0,0);
							//  �w�i�F�ݒ�
							lpCustomDraw->clrTextBk = RGB(255,0,0);
						}
					}
				} else if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::OUTERROWITEM ) {
					CRowDef* pRowDef = m_cTreeCtrl.GetRowDef( item );
					if( pRowDef )
					{
						if( pRowDef->Changed() )
						{
							//  �e�L�X�g�F�ݒ�
							lpCustomDraw->clrText = RGB(0,0,0);
							//  �w�i�F�ݒ�
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				}
			}
		}
	}

	return 0;
}

// �t�B�[�`���ɕύX���������Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnChangedFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �ύX�������Ă��Ȃ��ꍇ��OK�{�^����DISABLE�ɂȂ��Ă��܂����ߏ���
	SetButton( (BOOL)lParam );

	return 0;
}

// �G�C���A�X�ƃt�B�[���h���̐؂�ւ��{�^���������ꂽ�Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnClickedAlias_or_Field(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_bIsGridMode)
	{
		m_bIsDBNameMode = ( m_bIsDBNameMode == TRUE ) ? FALSE : TRUE;
		CButton cAliasButton = GetDlgItem( IDC_BUTTON_ALIASMODE );
		cAliasButton.SetBitmap( (m_bIsDBNameMode == TRUE ) ? m_hDBNameModeBmp : m_hAliasModeBmp );
		if( m_cExportDlg )
		{
			m_cExportDlg->SetAliasOrField( m_bIsDBNameMode );
			SetSelectedFeaturesToDlg();
		}
	}
	return 0;
}

// �R�����R���g���[���\���ƃO���b�h�\����؂�ւ���{�^���̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnClickedCommon_Or_Grid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bIsGridMode = ( m_bIsGridMode == TRUE ) ? FALSE : TRUE;
	CButton cGridButton  = GetDlgItem( IDC_BUTTON_GRIDMODE  );
	cGridButton.SetBitmap( (m_bIsGridMode == TRUE ) ? m_hGridModeBmp : m_hCommonModeBmp );
	SetSelectedFeaturesToDlg();

	return 0;
}

// ���[�_���̎��ɃL�[��߂܂���
LRESULT CAttributeDlgObj::OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}

// �x�������{�^�����������Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnClickedDeleteWarn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// �����Z�b�g����Ă��Ȃ�����
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
	{
		ATLASSERT(FALSE);
		return 0;
	}

	if( (ULONG)lFeatureIndex < m_listFeatureDef.size() )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );
		it->SetWarnDelete();	// �x����Ԃ�����
		SetSelectedFeaturesToDlg();
	}

	if( (ULONG)lFeatureIndex < m_listLQRowDef.size() )
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );
		it->SetWarnDelete();	// �x����Ԃ�����
		SetSelectedFeaturesToDlg();
	}
	return 0;
}

// �m�F�{�^�����������Ƃ��̃C�x���g�n���h��
LRESULT CAttributeDlgObj::OnClickedConfirmation(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// �����Z�b�g����Ă��Ȃ�����
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
	{
		ATLASSERT(FALSE);
		return 0;
	}

	_IRowPtr ipRow;
	if( (ULONG)lFeatureIndex < m_listFeatureDef.size() )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );
		ipRow = it->GetFeature();
	}
	if( (ULONG)lFeatureIndex < m_listLQRowDef.size() )
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );
		ipRow = it->GetFeature();
	}

	if( ipRow )
	{
		if( !m_cVerifyLog.AppendVerifyLog( m_cArcHelper.GetMap(), ipRow ) )
			AheMessageBox( NULL, AheLoadString( IDS_FAILTOWRITECHECKED ),  AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_NONE );
	}

	return 0;
}

// �I������Ă���t�B�[�`����ύX�˗����󂯂�C�x���g�n���h��
LRESULT CAttributeDlgObj::OnSetCurrentFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return SendMessage( m_cTreeCtrl, uMsg, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents �C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeDlgObj::ContentsChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ContentsCleared()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemAdded(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemDeleted(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemReordered(VARIANT Item, LONG toIndex)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::SelectionChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ViewRefreshed(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
{
	switch (phase)
	{
		case esriViewNone:
			break;
		case esriViewBackground:
			break;
		case esriViewGeography:
			break;
		case esriViewGeoSelection:
			break;
		case esriViewGraphics:
			break;
		case esriViewGraphicSelection:
			break;
		case esriViewForeground:
			if( !m_bIsEnding )
			{
				if( m_cExportDlg )
					m_cExportDlg->SendMessage( WM_REFRESHFEATURE, NULL, NULL );

				::SendMessage( m_cTreeCtrl, WM_REFRESHFEATURES, NULL, NULL );
			}
			break;
		default:
			break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterItemDraw(SHORT Index, IDisplay * Display, esriDrawPhase phase)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::FocusMapChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::SpatialReferenceChanged()
{
  return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
// IEditEvents �C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeDlgObj::OnSelectionChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCurrentLayerChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCurrentTaskChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnSketchModified()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnSketchFinished()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterDrawSketch(IDisplay * pDpy)
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnStartEditing()
{
	// �_�C�A���O���J���Ă����ԂŁA�ҏW�̊J�n������ƃo�[�W�������قȂ邽�߂ɁA
	// �ҏW�������Ă��A���̓��e�����f����Ȃ��̂ŁA�_�C�A���O�͕��邱�Ƃɂ���
	CloseDlg( FALSE );
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnStopEditing(VARIANT_BOOL Save)
{
	// �ҏW���ɕҏW�I�����ꂽ��_�C�A���O�I������
	// -> �Ȃ��Ȃ�ҏW�I������ƃc�[���������I�Ɏg�p�ł��Ȃ��Ȃ�iArcMap�̂����j����
	//    �܂��A�����ɗ��鎞�_�Ŋ��ɏI���������n�܂��Ă���炵���A�t�B�[�`���̕ۑ����o���Ȃ��iIEditEvents2�ŏo���邪�A�����Ă��Ȃ��j
	//    �]���āA�S�Ă͂����ă_�C�A���O�I��
	CloseDlg( FALSE );

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnConflictsDetected()
{
	m_lstUpdate.clear();

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnUndo()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnRedo()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCreateFeature(IObject * obj)
{
	// �����_�C�A���O�ł̓t�B�[�`��������邱�Ƃ͂Ȃ����A�O�̂���
	// �쐬���ꂽ�t�B�[�`�����m��
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("CreateFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
STDMETHODIMP CAttributeDlgObj::OnChangeFeature(IObject * obj)
{
	// �ύX���������t�B�[�`�����m��
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("ChangeFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
STDMETHODIMP CAttributeDlgObj::OnDeleteFeature(IObject * obj)
{
	// �����_�C�A���O�ł̓t�B�[�`�����폜����邱�Ƃ͂Ȃ����A�O�̂���
	// �폜���ꂽ�t�B�[�`�����m��
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("DeleteFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// IAttributeDlgObj �C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////

// �����o�ϐ�������������
STDMETHODIMP CAttributeDlgObj::Init(IDispatch *hook)
{
	IDispatchPtr ipDisp( hook );
	m_cArcHelper.Init( ipDisp );
	m_cTreeCtrl.Init( m_cArcHelper.GetApp() );

	// �O���b�h�\�����R�����R���g���[���\�����̃t���O�������i�����ݒ�̓R�����R���g���[���\���j
	m_bIsGridMode = FALSE;
	// �t�B�[���h���\�����G�C���A�X�\�����̃t���O�������i�����ݒ�̓G�C���A�X�\���j
	m_bIsDBNameMode = FALSE;

	// �q�_�C�A���O�̃^�u�ʒu�̏�����
	m_lRoadTabNumber = 0;
	m_lAccessTabNumber = 0;
	m_lHwyNodeTabNumber = 0;
	m_lDRMATabNumber = 0;

	// �L�����Z���{�^���t���O
	m_bIsCancel = FALSE;
	// �ύX�t���O
	m_bIsChanged = FALSE;
	// �_�C�A���O�ɓ����Ă���ŏ��̃t�B�[�`���t���O
	m_bIsFirst = TRUE;
	// ���[�_�������[�h���X���̃t���O
	m_bIsModal = FALSE;
	// ���o�����N�����E���Ă���t���O
	m_lMode = 0;
	// �t�B�[�`���ĕ`��͈�
	m_lRefreshBuf = 5;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]���̃`�b�v�p

	m_bIsLineHilight   = false;
	m_bIsVertexHilight = false;

	return S_OK;
}

// �t�B�[�`�����_�C�A���O�ɃZ�b�g����C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::SetItem(_IRow* pRow, _ISet* pSet, IGeometry* pGeometry, VARIANT_BOOL IsCreated )
{
	// [Bug4090]�Ŕ��o�������Ƃ����A�G�f�B�b�g�^�C�v�����܂܂�SetItem�ŏ��������Ă�������ǂ��A
	// ����͕s�K�؂������Bput_EditType�̌��SetItem������Ă���c�[���������āA���̂Ƃ��ɏ����l��
	// ���ǂ���Ă��܂��Ă��邱�Ƃ����������߁AInit�ł��悤�ɂ���
	m_sAttributeEditType = sindyeApplyRule;
	m_lstUpdate.clear();

	// �t�B�[�`���̏ꍇ
	if( !pSet )
	{
		m_listLQ.clear();
		m_bIsLQAttribute = FALSE;
		AddFeatures( pRow );

		// �ŏ��ɑI���������̂��_�C�A���O�ɏo�����߂����ŃI�u�W�F�N�gID���擾���Ă���
		if( m_bIsFirst )
		{
			pRow->get_OID( &m_lFirstOID );
			m_bIsFirst = FALSE;
		}

		// �t�B�[�`���N���X�擾
		IFeatureClassPtr ipFeatureClass;
		if( AheGetFeatureClass( m_cArcHelper.GetMap(), pRow, &ipFeatureClass))
		{
			CTargetItems cTargetItems;
			cTargetItems.clear();
			// ���H�����N���ǂ����H
			// [bug 5552] ���s�҃����N�ɂ��Ή�
			if( AheIsNetworkLink( ipFeatureClass ) )
			{
				// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă��铹�H���擾
				if( AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
			// �����N�������̏ꍇ
			else if( AheCompareFeatureName( ipFeatureClass, SEGMENTATTR_TABLE_NAME) )
			{
				// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă��铹�H���擾
				if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
			// �X�΂̏ꍇ
			else if( AheCompareFeatureName( ipFeatureClass, GRAD_TABLE_NAME) )
			{
				// 2�����b�V�����ׂ��ł��邩�ǂ������`�F�b�N���A�ׂ��ł���ΐڑ����Ă��铹�H���擾
				if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// �I�����ꂽ�A2�����b�V���֘A�̓��H��o�^����
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
		}
	}
	// �����N��̏ꍇ
	else
	{
		m_cFeatureItems.clear();
		m_bIsLQAttribute = TRUE;
		AddLQs( pRow, pSet, pGeometry, ( IsCreated == VARIANT_TRUE ? TRUE : FALSE ) );
	}

	return S_OK;
}

// ���[�h���X�_�C�A���O�ŃZ���N�V�������̃t�B�[�`����\������C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::DoModeless()
{
	// �����N�񑮐��_�C�A���O�̏ꍇ�ɂ́A�����N�񂪃Z�b�g���ꂽ���ǂ������ׂ�K�v������
	// ���ꂪ�Ȃ��ƁA�����N�񑮐��_�C�A���O��\�����Ă����ԂŁA����Ƀ����N���I�����悤�Ƃ���
	// �I���ł��Ȃ������ꍇ�A�c���[�\�����������Ă��܂����߁B
	if( m_bIsLQAttribute == TRUE )
	{
		if( m_listLQ.size() < 1 )
			return S_OK;
	}

	// �_�C�A���O�쐬
	if( !IsWindow() )
		Create( m_cArcHelper.GetHWND() );

	// �R�l�N�V�����ڑ�
	if( m_dwViewCookie == 0 )
		ConnectToEvents();

	// �ʏ�̃t�B�[�`���ƃ����N��Ƃŏꍇ�킯����
	if( !m_bIsLQAttribute )
	{
		if( !SetFeatures( m_cFeatureItems ) )
			return S_OK;
	}
	else
	{
		if( !SetLQs( m_listLQ ) )
			return S_OK;
	}

	ShowWindow( SW_SHOW );
	m_bIsModal = FALSE;
	// �_�C�A���O�ɓn���I�������ɏ��������Ă����Ȃ��Ƃ����Ȃ�
	m_cFeatureItems.clear();
	m_listLQ.clear();

	return S_OK;
}

// SetFeature() �ŃZ�b�g���ꂽ�t�B�[�`�������[�_���_�C�A���O�ŕ\������
STDMETHODIMP CAttributeDlgObj::DoModal(VARIANT_BOOL* IsChanged)
{
	ConnectToEvents();

	m_bIsModal = TRUE;
	// �ʏ�̃t�B�[�`���ƃ����N��Ƃŏꍇ�킯����
	if( !m_bIsLQAttribute )
	{
		if( !SetFeatures( m_cFeatureItems ) )
			return S_OK;
	}
	else
	{
		if( !SetLQs( m_listLQ ) )
			return S_OK;
	}
	// �_�C�A���O�ɓn���I�������ɏ��������Ă����Ȃ��Ƃ����Ȃ�
	m_cFeatureItems.clear();
	CAxDialogImpl<CAttributeDlgObj>::DoModal();
	m_bIsModal = FALSE;
	if( m_bIsChanged )
		*IsChanged = ( m_bIsChanged == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	if( m_bIsCancel )
		return S_FALSE;
	else
		return S_OK;
}

// �Ăяo�����Ƃ̃c�[������_�C�A���O�����p�̃C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::Close()
{
	ConnectToEvents( FALSE );

	// �c���[�����R���N�V����������
	m_cTreeCtrl.Clear();

	// �_�C�A���O���X�g��S�Ĕj������
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* pTest = it->second;
		pTest->Delete();
	}

	for( std::map<CString, HMODULE>::iterator itModule = m_mapModule.begin(); itModule != m_mapModule.end(); itModule++ )
	{
		HMODULE hModule = itModule->second;
		FreeLibrary( hModule );
	}

	m_mapDlg.clear();
	m_mapModule.clear();
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	// �o�^�p�t�B�[�`���̃N���A
	m_cFeatureItems.clear();
	m_listLQ.clear();
	// �t�B�[�`�����X�g�N���A
	m_listFeatureDef.clear();
	m_listLQRowDef.clear();
	
	// �ĕ`�悵�Ă��݂�����
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	// �_�C�A���O������
	if( IsWindow() )
	{
		if( m_bIsModal )
			EndDialog(0);		// ���[�_���̏ꍇ
		else
			DestroyWindow();	// ���[�h���X�̏ꍇ
	}

	m_bClosedDialog = TRUE;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]���̃`�b�v�p
	// ����ȂƂ���ŁA�t���O���������Ă����E�E�E
	m_bIsFirst = TRUE;
	WriteProperty();

	return S_OK;
}

STDMETHODIMP CAttributeDlgObj::SetMode(long lMode)
{
	m_lMode = lMode;
	return S_OK;
}

STDMETHODIMP CAttributeDlgObj::GetActiveItem(_IRow **pRow)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;
	// �c���[�ŉ���I�����Ă��邩���擾
	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// �����Z�b�g����Ă��Ȃ�����
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
		return S_FALSE;

	// �I�����Ă���t�B�[�`����iterator���擾
	if( !m_bIsLQAttribute )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );

		// �I�𒆂̃t�B�[�`���擾
		IFeaturePtr ipFeature;
		ipFeature = it->GetFeature();

		if( ipFeature != NULL )
		{
			*pRow = ipFeature;
			(*pRow)->AddRef();

		}else
			return S_FALSE;
	}
	else
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );

		// �I�𒆂̃t�B�[�`���擾
		_IRowPtr ipRow;
		ipRow = it->GetFeature();

		if( ipRow )
		{
			*pRow = ipRow;
			(*pRow)->AddRef();

		}else
			return S_FALSE;
	}

	return S_OK;
}

// ���݂̃I�y���[�V�������[�h��Ԃ��C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::get_DoOperation(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = BOOL2VB(m_bDoOperation);
	
	return S_OK;
}

// �I�y���[�V������Ԃ��Z�b�g����C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::put_DoOperation(VARIANT_BOOL newVal)
{
	m_bDoOperation = VB2BOOL(newVal);
	return S_OK;
}

// ���݂̕ҏW���[����Ԃ��C���^�[�t�F�C�X
STDMETHODIMP CAttributeDlgObj::get_EditType(sindyeAttributeEditType *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = m_sAttributeEditType;
	return S_OK;
}

// �����ҏW�_�C�A���O�ŁA���[���ɏ]���ă_�C�A���O��OK�{�^����������悤�ɂ��邩�ǂ�����ݒ肷��
STDMETHODIMP CAttributeDlgObj::put_EditType(sindyeAttributeEditType newVal)
{
	m_sAttributeEditType = newVal;
	return S_OK;
}

// �g�p���Ȃ�
STDMETHODIMP CAttributeDlgObj::put_AutoMode(VARIANT_BOOL newVal)
{
	return S_OK;
}

// �R���X�g���N�^
CAttributeDlgObj::CAttributeDlgObj() : m_winMgr(BASEMAP), m_dwViewCookie(0), m_dwEditCookie(0), m_cDlgRect( -1, -1, -1, -1 ),
	m_cTree( WC_TREEVIEW, this, IDC_FEATTREE ),
	m_cBtn1( _T("BUTTON"), this, IDC_BUTTON_GRIDMODE ),
	m_cBtn2( _T("BUTTON"), this, IDC_BUTTON_ALIASMODE ),
	m_cBtnClose( _T("BUTTON"), this, IDC_BUTTON_CLOSE ),
	m_cBtnSave( _T("BUTTON"), this, IDOK ),
	m_cBtnDelWarn( _T("BUTTON"), this, IDC_BUTTON_DELETEWARN ),
	m_cBtnConfirmation( _T("BUTTON"), this, IDC_BUTTON_CONFIRMATION )
{
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	m_cFeatureItems.clear();
	m_listLQ.clear();
}

// �t�B�[�`���������o�ϐ��im_cFeatureItems�j�ɒǉ�����
void CAttributeDlgObj::AddFeatures( IFeaturePtr ipFeature )
{
	m_cFeatureItems.push_back( ipFeature );
	long lSize = m_cFeatureItems.size();
}

// �����N��������o�ϐ��im_listLQ�j�ɒǉ�����
void CAttributeDlgObj::AddLQs( _IRowPtr ipLQRow, _ISetPtr ipLQRowSet, IGeometryPtr ipLQGeom, BOOL bIsCreated )
{
	LQROWDEF def;

	def.obj = ipLQRow;
	def.shp = ipLQGeom;
	def.deflt = bIsCreated;

	def.flg = m_cLQInfo.GetLoadQueueType( AheGetFeatureClassName( ipLQRow ), sindyeLoadQueueInfoInfoTableName );
	def.name = m_cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)def.flg, sindyeLoadQueueInfoAliasName );

	// LQ ��o�^
	ipLQRowSet->Reset();
	IUnknownPtr ipUnk;
	while( SUCCEEDED( ipLQRowSet->Next( &ipUnk ) ) )
	{
		if( ipUnk == NULL ) break;

		ROWDEF lqdef;
		lqdef.obj = ipUnk;

		def.lqlist.push_back( lqdef );
	}
	m_listLQ.push_back( def );
}

// m_cFeatureItems �Ɋi�[�����t�B�[�`�����c���[�ɕ\������
BOOL CAttributeDlgObj::SetFeatures( CTargetItems cFeatureSet )
{
	// ���[�h���X�̏ꍇ�ɂ�������K�v�͂Ȃ����[�_���̏ꍇ�͑��̃t�B�[�`���[�G��Ȃ��̂ł����ɓ���Ȃ��ł悵
	if( !m_bIsModal )
	{
		// ���ݕҏW���̃t�B�[�`��������΁A�ۑ����Ă���
		m_bIsSetFeature = TRUE;
		if( !StoreFeature() )
		{
			// �ۑ��Ɏ��s������o�^�p�t�B�[�`���̃N���A
			m_cFeatureItems.clear();

			m_bIsSetFeature = FALSE;
			return FALSE;
		}
		m_bIsSetFeature = FALSE;

		// �����̂��̂�S�ăN���A
		if( IsWindow() )
			m_cTreeCtrl.DeleteAllItems();

		// ���ݕ\�����Ă���_�C�A���O������΁A����� CCrontrolRel �� CFeatureDef �̊֌W���N���A
		if( m_cExportDlg != NULL )
			m_cExportDlg->ClearFeatureDefs();

		m_listFeatureDef.clear();
	}

	if( cFeatureSet.size() == 0 )
		return FALSE;

	// �t�B�[�`����`�N���X�̃��X�g���쐬
	for( CTargetItems::iterator i = cFeatureSet.begin(); i != cFeatureSet.end(); i++ )
	{
		IFeaturePtr ipFeature = i->GetFeature();

		CFeatureDef featDef;
		featDef.clear();
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetFeature( ipFeature );

		// �t�B�[�`�����ҏW�\���ǂ������ׂ�
		ISiNDYRulePtr		ipRule;
		ipRule.CreateInstance( CLSID_SiNDYRuleObj );
		
		IWorkspacePtr ipWorkspace;
		ITablePtr ipTable;
		ipFeature->get_Table( &ipTable );
		IDatasetPtr ipDataset( ipTable );
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
					if( !ipRule )
						ipRule.CreateInstance( CLSID_SiNDYRuleObj );

					IGeometryPtr ipGeometry;
					ipFeature->get_ShapeCopy( &ipGeometry );

					ipRule->IsEditableFeature( ipFeature, ipGeometry, sindyeEditAttribute, &(featDef.m_sindyeEditableType), &(featDef.m_bstrIsEditableFeature) );

					if ( featDef.m_sindyeEditableType == sindyeEditableEditable )
						featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANEDITFEATURE );
				}
				else {
					featDef.m_sindyeEditableType = sindyeEditableNoEditable;
					featDef.m_bstrIsEditableFeature = AheLoadString( IDS_DONTSTARTEDITING );
				}
			}
			else {
				featDef.m_sindyeEditableType = sindyeEditableNoEditable;
				featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
			}
		}

		IDatasetEditInfoPtr ipDatasetEditInfo;
		ipDatasetEditInfo = ipDataset;
		VARIANT_BOOL vaCanEdit = VARIANT_FALSE;
		if( ipDatasetEditInfo )
			ipDatasetEditInfo->get_CanEdit( &vaCanEdit );
		if( vaCanEdit == VARIANT_FALSE )
		{
			featDef.m_sindyeEditableType = sindyeEditableNoEditable;
			featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
		}
		m_listFeatureDef.push_back( featDef );
	}

	cFeatureSet.clear();

	// �ŏ��ɃN���b�N�������̂�擪�ɏo�����߂̏���
	std::list<CFeatureDef>::iterator itfirst = m_listFeatureDef.begin();
	IFeaturePtr ipFeature = itfirst->GetFeature();

	LONG lOID = 0;
	ipFeature->get_OID( &lOID );
	if( lOID != m_lFirstOID )
	{
		std::list<CFeatureDef> reverse;
		reverse.clear();
		// ���X�g�����ւ���
		// �܂��A���X�g�ɍŏ��̃t�B�[�`��������Ă���
		for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
		{
			ipFeature = it->GetFeature();
			ipFeature->get_OID( &lOID );

			if( lOID == m_lFirstOID )
			{
				reverse.push_front(*it);
				break;
			}
		}
		// �I�������A�ŏ��̃t�B�[�`���ȊO������
		for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ )
		{
			ipFeature = i->GetFeature();
			ipFeature->get_OID( &lOID );

			if( lOID != m_lFirstOID )
				reverse.push_back(*i);
		}

		// m_listFeatureDef��u��������
		m_listFeatureDef = reverse;
	}

	// ���[�_���i�V�K�쐬�����L�ҏW�j�̎�
	if( m_bIsModal )
	{
		// ����̃_�C�A���O����ɏo�Ă���悤�ɂ���
		static std::set<CString> prev_layer = boost::assign::list_of
			(building::kTableName)          // �ƌ`�͊K������
			(sj::subway_node::kTableName);  // �n���S�m�[�h�̓����N������

		// �Ώۂ̃��C��������ɂ�����t���ɂ���
		// ���C����3�ȏ゠������ǂ����񂾂Ƃ����b�����邪�A�Ƃ肠�����ƌ`�ƒn���SNW���f���Ɍ������b�Ȃ̂ł܂��������B�B
		if( prev_layer.end() != prev_layer.find(AheGetFeatureClassName( m_listFeatureDef.rbegin()->GetFeature() )) )
			std::reverse(m_listFeatureDef.begin(), m_listFeatureDef.end());
	}
	// ���[�h���X�̎�
	else
	{
		// �c���[�Ƀt�B�[�`����o�^�i�\�������O�ɓo�^����K�v�A���j
		m_cTreeCtrl.SetFeatures( &m_listFeatureDef );
		// �_�C�A���O�Ƀt�B�[�`����o�^�im_listFeatureDef ���Z�b�g���ꂽ��j
		SetSelectedFeaturesToDlg();
	}
	return TRUE;
}

// m_listLQ �Ɋi�[�����t�B�[�`�����c���[�ɕ\������
BOOL CAttributeDlgObj::SetLQs( std::list<LQROWDEF>& listRowDef )
{
	// ���[�h���X�̏ꍇ�ɂ�������K�v�͂Ȃ����[�_���̏ꍇ�͑��̃t�B�[�`���[�G��Ȃ��̂ł����ɓ���Ȃ��ł悵
	if( !m_bIsModal )
	{
		// ���ݕҏW���̃t�B�[�`��������΁A�ۑ����Ă���
		m_bIsSetFeature = TRUE;
		if( !LQStoreFeature() )
		{
			// �ۑ��Ɏ��s������o�^�p�t�B�[�`���̃N���A
			m_listLQ.clear();
			m_bIsSetFeature = FALSE;
			return FALSE;
		}
		m_bIsSetFeature = FALSE;

		// �����̂��̂�S�ăN���A
		if( IsWindow() == TRUE )
			m_cTreeCtrl.DeleteAllItems();

		// ���ݕ\�����Ă���_�C�A���O������΁A����� CCrontrolRel �� CFeatureDef �̊֌W���N���A
		if( m_cExportDlg != NULL )
			m_cExportDlg->ClearFeatureDefs();

		m_listLQRowDef.clear();
	}
	
	if( listRowDef.size() < 1 )
		return FALSE;

	// �t�B�[�`����`�N���X�̃��X�g���쐬
	CLoadQueueInfo cLQInfo;
	for( std::list<LQROWDEF>::iterator it = listRowDef.begin(); it != listRowDef.end(); it++ )
	{
		if( !it->obj ) continue;

		CLQRowDef featDef;
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetLQRow( it->obj, it->shp, (sindyeRoadQueueType)it->flg, it->name, it->deflt );
		// �ҏW�s���L�����Ă���
		// INF�̏������郏�[�N�X�y�[�X���ҏW�J�n����Ă��Ȃ���΂��߂Ȃ悤�ɂ���
		IWorkspacePtr ipWorkspace;
		ITablePtr ipInfTable;
		it->obj->get_Table( &ipInfTable );
		IDatasetPtr ipDataset( ipInfTable );
		if( ipDataset )
			ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			_ASSERTE( ipWorkspace != NULL );
		else {
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
			ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited );
			if( vbIsBeingEdited )
			{
				if( !m_ipRule )
					m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );

				m_ipRule->IsEditableFeature( it->obj, featDef.GetShape(), sindyeEditAttribute, &(featDef.m_sindyeEditableType), &(featDef.m_bstrIsEditableFeature) );

				if ( featDef.m_sindyeEditableType == sindyeEditableEditable )
					featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANEDITFEATURE );
			}
			else {
				featDef.m_sindyeEditableType = sindyeEditableNoEditable;
				featDef.m_bstrIsEditableFeature = AheLoadString( IDS_DONTSTARTEDITING );
			}
		}
		IDatasetEditInfoPtr ipDatasetEditInfo;
		ipDatasetEditInfo = ipDataset;
		VARIANT_BOOL vaCanEdit = VARIANT_FALSE;
		if( ipDatasetEditInfo )
			ipDatasetEditInfo->get_CanEdit( &vaCanEdit );
		if( !vaCanEdit )
		{
			featDef.m_sindyeEditableType = sindyeEditableNoEditable;
			featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
		}

		m_listLQRowDef.push_back( featDef );

		// LQ ���O�����R�[�h�Ƃ��Ė������o�^
		LONG lOID = -1;
		ITablePtr ipTable;
		CRelationalTableDef cRelTableDef;

		it->obj->get_OID( &lOID );
		for( std::list<ROWDEF>::iterator itLQs = it->lqlist.begin(); itLQs != it->lqlist.end(); itLQs++ )
		{
			CRowDef rowDef;
			IFieldsPtr ipFields;
			IFieldPtr ipField;
			LONG lFieldCount = -1;
			CString strTableName = AheGetFeatureClassName( itLQs->obj );

			if( !ipTable )
				itLQs->obj->get_Table( &ipTable );
			rowDef.SetRow( itLQs->obj );
			rowDef.SetTable( ipTable );

			itLQs->obj->get_Fields( &ipFields );
			ipFields->get_FieldCount( &lFieldCount );
			for( LONG i = 0; i < lFieldCount; i++ ) 
			{
				CComBSTR bstrFieldName = 0;
				ipFields->get_Field( i, &ipField );
				ipField->get_Name( &bstrFieldName );

				CFieldDef fieldDef;

				fieldDef.SetFieldDef( strTableName, OLE2T( bstrFieldName ) );
				fieldDef.SetRowBuffer( i, ipField, itLQs->obj );
				rowDef.push_back( fieldDef );
			}
			cRelTableDef.push_back( rowDef );
		}
		cRelTableDef.SetTable( ipTable, AheGetFeatureClassName( ipTable ), lOID, INF_ID_FIELD, FALSE, TRUE );
		m_listLQRowDef.rbegin()->GetRelationalTableDefList()->push_back( cRelTableDef );
	}

	listRowDef.clear();	// �����Ă���

	// ���[�_���i�V�K�쐬�j�̎�
	if( m_bIsModal )
	{
		// VICS�̐V�K�쐬�̎��ɗ��������쐬�����ꍇ�ɁA�ŏ��ɍ쐬����������VICS��\�����邽�߂ɁA�ȉ��̏������s��
		BOOL	bReverse = FALSE;
		std::list<CLQRowDef> reverse;
		reverse.clear();

		LONG lFeatCount = 0;
		for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++, lFeatCount++ )
		{
			_IRowPtr	ipRow = it->GetFeature();

			CString 	strLayerName;
			strLayerName = AheGetLayerName( m_cArcHelper.GetMap(), ipRow, _FIND_TABLE );

			// VICS���Q�������ꍇ
			if( (strLayerName.Find( _T("VICS")) != -1 && lFeatCount == 1) || (strLayerName.Find( _T("INTERNAVI")) != -1 && lFeatCount == 1)  )
				bReverse = TRUE;
		}
		// �t�ɓ����
		if( bReverse )
		{
			// ���X�g���t���ɂ����
			for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++ )
				reverse.push_front(*it);

			// ����ŁA�Ⴂ�ԍ�����ɗ������ƂɂȂ�
			m_listLQRowDef = reverse;
		}
	}

	// ���[�_������Ȃ��ꍇ�̂�
	if( !m_bIsModal )
	{
		// ���ʈē����������ꍇ�ɂ́A�ŏ��ɃG���[�`�F�b�N�������āA�s�������Ȃ����ǂ�������ׂ�
		for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++ )
		{
			_IRowPtr	ipRow = it->GetFeature();

			CString 	strLayerName;
			strLayerName = AheGetLayerName( m_cArcHelper.GetMap(), ipRow, _FIND_TABLE );

			if( strLayerName.Find(_T("INF_DIRGUIDE") ) != -1 )
			{

				CExportDlg* cExportDlg;

				std::map<CString, CExportDlg*>::iterator itDir = m_mapDlg.find(  _T("INF_DIRGUIDE")  );
				if( itDir == m_mapDlg.end() )
				{
					cExportDlg = SetChildDlg( _T("INF_DIRGUIDE") );
					cExportDlg->Create( m_hWnd );
					m_mapDlg[_T("INF_DIRGUIDE")] = cExportDlg;

				}
				else
					cExportDlg = itDir->second;

				cExportDlg->SendMessage( NULL, true, 0 );
				cExportDlg->SetFeatureDefList( &m_listLQRowDef );
				cExportDlg->ErrorCheck();
				cExportDlg->SendMessage( NULL, false, 0 );
				break;
			}
		}

		// �c���[�Ƀt�B�[�`����o�^�i�\�������O�ɓo�^����K�v�A���j
		m_cTreeCtrl.SetLQs( &m_listLQRowDef );
		// �_�C�A���O�Ƀt�B�[�`����o�^�im_listFeatureDef ���Z�b�g���ꂽ��j
		SetSelectedFeaturesToDlg();
	}

	return TRUE;
}

// �c���[�őI��ł���t�B�[�`���̎q�_�C�A���O���쐬���A�_�C�A���O�Ɏ��ۂɕ\������֐�
BOOL CAttributeDlgObj::SetSelectedFeaturesToDlg()
{
	BOOL bEditable = TRUE;

	IUnknownPtr ipUnk;
	IRowBufferPtr ipRowBuffer;

	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	// �c���[�ŉ���I�����Ă��邩���擾
	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );
	m_cTreeCtrl.m_bIsLineHilight = m_bIsLineHilight;
	m_cTreeCtrl.m_bIsVertexHilight = m_bIsVertexHilight;

	// �����Z�b�g����Ă��Ȃ�����
	if( lFeatureIndex < 0 )
		return FALSE;

	// �I�𒆂̃t�B�[�`���擾
	IFeaturePtr ipFeature;
	_IRowPtr ipRow;

	CComBSTR bstrIsEditableFeature;
	sindyeEditableType sindyEditable = sindyeEditableNoDefine;

	// �I������Ă���t�B�[�`����iterator���擾���A���𔲂��o��
	// LQ�n�Ƃ̏ꍇ����������
	if( ! m_bIsLQAttribute )
	{
		// �I�����Ă���t�B�[�`����iterator���擾
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		if( m_listFeatureDef.size() > (ULONG)lFeatureIndex )
		{
			advance( it, lFeatureIndex );
			// �V�K�쐬�̏ꍇ�ɍ쐬���ꂽ�t�B�[�`�������[���ɂ�����炸�����I�Ƀ��[���Ő��䂳��Ă���e��R���g���[����ҏW�\��Ԃɂ���
			if( m_sAttributeEditType == sindyeNotApplyRuleEnable )
			{
				// [Bug 5770]�V�K�쐬�̏ꍇ�ɂ́A�x���͂Ȃ��������̂Ƃ��Ĉ���Ȃ��Ǝq�_�C�A���O���̍ŏ��̃t�H�[�J�X�Z�b�g�ŁA
				// �t�H�[�J�X��������Ȃ��Ȃ��Ă��܂��̂ŁA�����Ōx���͂����Ă���
				it->SetWarnDelete();
				it->m_sindyeEditableType = sindyeEditableEditable;
			}
			bstrIsEditableFeature = it->m_bstrIsEditableFeature;
			sindyEditable = it->m_sindyeEditableType;
			// �ҏW�x���̏ꍇ�̓t���O���݂ĉ�������Ă��邩�ǂ������f
			if( sindyeEditableWarnning == sindyEditable && it->GetWarnDelete() )
			{
				sindyEditable = sindyeEditableEditable;
				bstrIsEditableFeature = _T("");
			}
		}

		if( lTableIndex < 0 && lRowIndex < 0 )
			ipFeature = it->GetFeature();
		else {
			// �t�B�[�`���ɕt�����Ă���e�[�u���֘A�i����ʍs��ʍs�֎~�̃e�[�u���j�̑������擾���邽��
			// �I�����Ă���t�B�[�`���̊֘A�e�[�u�����擾
			std::list<CRelationalTableDef>* pRelTableList = it->GetRelationalTableDefList();
			if( pRelTableList )
			{
				std::list<CRelationalTableDef>::iterator itRelTableList = pRelTableList->begin();
				if( pRelTableList->size() > (ULONG)lTableIndex )
				{
					advance( itRelTableList, lTableIndex );
					std::list<CRowDef>::iterator itRowDef = itRelTableList->begin();
					if( itRelTableList->size() > (ULONG)lRowIndex )
					{
						advance( itRowDef, lRowIndex );
						ipFeature = itRowDef->GetRow();
						ipRow = itRowDef->GetRow();
					}
				}
			}
		}
	}
	else {
		// �I�����Ă���t�B�[�`����iterator���擾
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		if( m_listLQRowDef.size() > (ULONG)lFeatureIndex )
		{
			advance( it, lFeatureIndex );

			// �V�K�쐬�̏ꍇ�ɍ쐬���ꂽ�t�B�[�`�������[���ɂ�����炸�����I�Ƀ��[���Ő��䂳��Ă���e��R���g���[����ҏW�\��Ԃɂ���
			if( m_sAttributeEditType == sindyeNotApplyRuleEnable )
			{
				// [Bug 5770]�V�K�쐬�̏ꍇ�ɂ́A�x���͂Ȃ��������̂Ƃ��Ĉ���Ȃ��Ǝq�_�C�A���O���̍ŏ��̃t�H�[�J�X�Z�b�g�ŁA
				// �t�H�[�J�X��������Ȃ��Ȃ��Ă��܂��̂ŁA�����Ōx���͂����Ă���
				it->SetWarnDelete();
				it->m_sindyeEditableType = sindyeEditableEditable;
			}
			bstrIsEditableFeature = it->m_bstrIsEditableFeature;
			sindyEditable = it->m_sindyeEditableType;
			// [Bug 5727]�����N�񂪃��[�j���O�̎��ɕҏW�{�^���������Ă��ҏW�ł��Ȃ��o�O
			// �ҏW�x���̏ꍇ�̓t���O���݂ĉ�������Ă��邩�ǂ������f
			if( sindyeEditableWarnning == sindyEditable && it->GetWarnDelete() )
			{
				sindyEditable = sindyeEditableEditable;
				bstrIsEditableFeature = _T("");
			}

			if( lTableIndex < 0 && lRowIndex < 0 )
			{
				ipRow = it->GetFeature();
				IGeometryPtr ipGeom = it->GetShape();
				if( ipGeom != NULL )
				{
					// �����o�b�t�@�����O����
					DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
					IPolygonPtr ipPolygon;
					AheGeometry2Polygon( ipGeom, dBufferSize, &ipPolygon );
					// [Bug 4883]�s���Ȍ`���I�����Ă��܂����ꍇ�ɗ������肪����̂ŁANULL�`�F�b�N���Ă���
					if( ipPolygon )
						ipPolygon->get_Envelope( &m_ipSelectFeatEnv );
				}
			}
			else {
				// �t�B�[�`���ɕt�����Ă���e�[�u���֘A�iLQ�e�[�u���j�̑������擾���邽��
				// �I�����Ă���t�B�[�`���̊֘A�e�[�u�����擾
				std::list<CRelationalTableDef>* pRelTableList = it->GetRelationalTableDefList();
				if( pRelTableList )
				{
					std::list<CRelationalTableDef>::iterator itRelTableList = pRelTableList->begin();
					if( pRelTableList->size() > (ULONG)lTableIndex )
					{
						advance( itRelTableList, lTableIndex );
						std::list<CRowDef>::iterator itRowDef = itRelTableList->begin();
						if( itRelTableList->size() > (ULONG)lRowIndex )
						{
							advance( itRowDef, lRowIndex );
							ipFeature = itRowDef->GetRow();
							ipRow = itRowDef->GetRow();
						}
					}
				}
			}
		}
	}
	if( ! ( ipFeature || ipRow ) )
	{
		MessageBox( AheLoadString( IDS_CANTSPECIFY ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING );
		return FALSE;
	}

	// �t�B�[�`���N���X���܂��̓I�u�W�F�N�g�N���X���̎擾
	// �����Ŏ擾�������O����A���C����.dll��ǂݍ��ނ��ƂɂȂ�܂��B
	CString strTableName;
	if( ! m_bIsLQAttribute )
		strTableName = AheGetFeatureClassName( ipFeature );
	else
	{
		ITablePtr ipTable;
		ipRow->get_Table( &ipTable );
		CComBSTR bstrTableName;
		((IObjectClassPtr)ipTable)->get_AliasName( &bstrTableName );
		CString strTemp( bstrTableName );
		strTableName = strTemp.Right( strTemp.GetLength() - strTemp.Find( _T(".") ) - 1 ).MakeUpper();
	}

	
	// ���e�m�F�{�^����UPDATETYPE_CFIELD������ꍇ�ɂ̂݉�����悤�ɂ��Ă������߁A�\������t�B�[�`����UPDATETYPE_CFIELD�����邩
	// �ǂ����m�F���āAENABLE�ɂ��邩DISABLE�ɂ��邩���߂�
	if( ipFeature )
	{
		IFieldsPtr ipFields;
		ipFeature->get_Fields( &ipFields );
		long lFieldIndex = -1;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFieldIndex );
		::EnableWindow( GetDlgItem(IDC_BUTTON_CONFIRMATION), ( lFieldIndex == -1 ) ? FALSE : TRUE );
	}
	else if( ipRow )
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lFieldIndex = -1;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFieldIndex );
		::EnableWindow( GetDlgItem(IDC_BUTTON_CONFIRMATION), ( lFieldIndex == -1 ) ? FALSE : TRUE );
	}
#ifdef _FOR_ENGLISH		// [bug 11183]�ʏ�̏ꍇ�A���̕\���͗v��Ȃ�
	// ���̂�\������
	DisplayObjectName( ipFeature );
#endif

	// �t�B�[�`���̎�ނɂ���ĕ\���`�Ԃ�ς���
	CExportDlg* cExportDlg = NULL;

	// �R�����R���g���[���\���̏ꍇ
	if( ! m_bIsGridMode )
	{
		// ���C���ʂ̎q�_�C�A���O���擾
		cExportDlg = SetChildDlg(strTableName);
		if( m_bIsLQAttribute && !cExportDlg )  // �G�C���A�X�ő��݂��Ȃ� [bug 7516]
		{
			strTableName = AheGetFeatureClassName( ipRow );
			cExportDlg = SetChildDlg( strTableName );
		}
	}
	
	// GRID�\���̏ꍇ
	// �ǂ�ł��Ȃ�������f�t�H���g�̃O���b�h��\��
	bool bIsDefaultDlg = FALSE;
	if( ! cExportDlg )
	{
		if( ! m_bIsLQAttribute )
		{
			std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( _T("DEFAULT") );

			if( it == m_mapDlg.end() )
			{
				if( m_strInstallPath.IsEmpty() )
				{
					// SiNDY-e�C���X�g�[�����[�g�p�X�擾
					CInifileHelper	cIniFile;
					cIniFile.GetInstallRoot( m_strInstallPath );
				}

				CString strModuleName;
				strModuleName += m_strInstallPath + _T("ChildDlgDLL\\DEFAULT.dll");

#ifdef _USE_CHILDDLG_SHORTCUT
				HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
				HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

				if( hModule )
				{
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");

					cExportDlg = pCreateTest();
					cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );
					m_mapModule[_T("DEFAULT")] = hModule;
				}
			}
			else
				cExportDlg = it->second;
		}
		else {
			std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( _T("LQDEFAULT") );
			if( it == m_mapDlg.end() )
			{
				if( m_strInstallPath.IsEmpty() )
				{
					// SiNDY-e�C���X�g�[�����[�g�p�X�擾
					CInifileHelper	cIniFile;
					cIniFile.GetInstallRoot( m_strInstallPath );
				}

				CString strModuleName;
				strModuleName += m_strInstallPath + _T("ChildDlgDLL\\LQDEFAULT.dll");

#ifdef _USE_CHILDDLG_SHORTCUT
				HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
				HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

				if( hModule )
				{
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");

					cExportDlg = pCreateTest();
					cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );

					m_mapModule[_T("LQDEFAULT")] = hModule;
				}
			}
			else
				cExportDlg = it->second;
		}

		bIsDefaultDlg = TRUE;
	}

	// �����܂ł��āA�_�C�A���O�Ȃ�������A�����Ă悵�A���肦�Ȃ����ǁE�E�E
	if( ! cExportDlg )
	{
		ATLASSERT( cExportDlg != NULL );
		return FALSE;
	}

	// ���łɃ_�C�A���O���\������Ă�����A�V�������̂�\�����Ȃ��Ă͂����Ȃ��̂ŁA�\������Ă���_�C�A���O�쐬
	if( m_hCurrentWnd )
		::ShowWindow( m_hCurrentWnd, SW_HIDE );

	// �I�����ꂽ�q�_�C�A���O�쐬
	if( ! bIsDefaultDlg )
	{
		std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( strTableName );
		if( itDlg == m_mapDlg.end() )
		{
			m_hCurrentWnd =	cExportDlg->Create( m_hWnd, (LPARAM)(LPCTSTR)strTableName );
			m_mapDlg[strTableName] = cExportDlg;
		}
		else
			m_hCurrentWnd = cExportDlg->GetDlg();
	}
	else {
		if( ! m_bIsLQAttribute )
		{
			std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( _T("DEFAULT") );
			if( itDlg == m_mapDlg.end() )
			{
				m_hCurrentWnd =	cExportDlg->Create( m_hWnd );
				m_mapDlg[_T("DEFAULT")] = cExportDlg;
			}
			else
				m_hCurrentWnd = cExportDlg->GetDlg();
		}
		else
		{
			std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( _T("LQDEFAULT") );
			if( itDlg == m_mapDlg.end() )
			{
				m_hCurrentWnd =	cExportDlg->Create( m_hWnd );
				m_mapDlg[_T("LQDEFAULT")] = cExportDlg;
			}
			else
				m_hCurrentWnd = cExportDlg->GetDlg();
		}
	}

	// �G�C���A�X�\�����ADB�\����������
	cExportDlg->SetAliasOrField( m_bIsDBNameMode );

	// �q�_�C�A���O�\��
	::ShowWindow( m_hCurrentWnd, SW_SHOW );

	// �q�_�C�A���O�Ƀt�B�[�`���̃��X�g��n��
	if( m_bIsLQAttribute )
		cExportDlg->SetFeatureDefList( &m_listLQRowDef );
	else
		cExportDlg->SetFeatureDefList( &m_listFeatureDef );

	if( strTableName.Find(_T("INF_DIRGUIDE")) != -1 || strTableName.Find(_T("INF_LANE")) != -1)
		cExportDlg->SendMessage( WM_DISABLECONTROL, m_lMode, 0 );

	// �ҏW�\���C���������łȂ����̃`�F�b�N
	// �X�^�e�B�b�N�R���g���[���Ƀt�B�[�`���̕ҏW�s��\������
	CComBSTR	 bstrMsg( bstrIsEditableFeature );
	switch( m_sAttributeEditType )
	{
		case sindyeNotApplyRuleEnable:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), FALSE );
			bEditable = TRUE;
			break;
		case sindyeNotApplyRuleDisable:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), FALSE );
			bEditable = FALSE;
			break;
		default:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), ( sindyeEditableEditable == sindyEditable ) ? FALSE : TRUE );
			bEditable = ( sindyeEditableEditable == sindyEditable ) ? TRUE : FALSE;
			break;
	}
	// �����ҏW�c�[���Œ��L��I�������ꍇ�ɂ͕ҏW�s�Ƃ���
	if( AheIsChukiLayer( strTableName ) && !m_bIsModal )
		bEditable = FALSE;

	// �ҏW��ԕ\�����Ƀt�B�[�`���̏�Ԃ�\���i�q�_�C�A���O���ŏ㏑�����邱�Ƃ�����̂Ő�Ɂj
	::SetWindowText( GetDlgItem(IDC_STATIC_MESSAGE), OLE2T(bstrMsg) );

	// �q�_�C�A���O�̊e��R���g���[�����Z�b�e�B���O
	cExportDlg->SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, TRUE, bEditable );

	// �����o�ϐ��ɓo�^���Ă���
	m_lFeatureDefIndex = lFeatureIndex;
	m_cExportDlg = cExportDlg;

	SetButton( Changed() );

#ifdef _FOR_ENGLISH		// [bug 11183]�ʏ�̏ꍇ�A���̕\�����v��Ȃ��̂ŁA�����̃`�b�v���\�����Ȃ�
	InitToolTip(IDC_STATIC_OFFICIALNAME);
	InitToolTip(IDC_STATIC_ALTERNATIVENAME);
#endif
	InitToolTip(IDC_STATIC_MESSAGE);

	// �_�C�A���O�T�C�Y�̐ݒ�
	ResizeClientDlg();

	// ���ݑI�����Ă���t�B�[�`����Envelope���Ƃ��Ă����āA�_�C�A���O�������Ƃ��ɁA�I�����Ă���t�B�[�`���̎��肾���ĕ`�悷��
	if( ipFeature )
	{
		// �����o�b�t�@�����O����
		sindyeFeatureKinds kinds = AheGetFeatureKinds(ipFeature);
		IPolygonPtr ipPolygon;
		// �|���S���̏ꍇ�̓o�b�t�@�����O���Ȃ�
		if( kinds == sindyePolygonFeature )
		{
			IGeometryPtr ipRefreshGeom;
			ipFeature->get_Shape( &ipRefreshGeom );
			ipPolygon = ipRefreshGeom;
		}
		else
		{
			DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
			AheFeature2Polygon( ipFeature, dBufferSize, &ipPolygon );
		}
		// [Bug 4883]�s���Ȍ`���I�����Ă��܂����ꍇ�ɗ������肪����̂ŁANULL�`�F�b�N���Ă���
		if( ipPolygon )
			ipPolygon->get_Envelope( &m_ipSelectFeatEnv );
	}

	// ����ȂƂ���ŁA�t���O���������Ă����E�E�E
	m_bIsFirst = TRUE;

	return TRUE;
}

// �t�B�[�`���̕ۑ����Ǘ�����֐�
BOOL CAttributeDlgObj::StoreFeature()
{
	// �ύX���������ꍇ�ۑ�
	// Sketch�c�[�������΂�Ă���ꍇ�ɂ͐V�K�쐬�ő������Ȃɂ��ҏW����Ă��Ȃ��ꍇ������̂ŁA����ł�����Ȃ��Ă͂����Ȃ�
	// sindyeNotApplyRuleEnable��Sketch���炵����΂�Ȃ��̂�
	if( Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
	{
		// �ύX�t���O�𗧂Ă�
		m_bIsChanged = TRUE;

		if( ErrorCheck() )
		{
			// �t�B�[�`���̐؂�ւ����������Ƃ��ɂ̓_�C�A���O���o���āA�ۑ����邩�������˂�
			// �ύX�����������ǂ��������Ă����Ȃ��ƁA�V�K�쐬�����΂ꂽ�Ƃ��ɁA�ŏ��ɉ��̃��b�Z�[�W���J���Ă��܂�
			if(	m_bIsSetFeature && Changed() )
			{
				int iRet = AheMessageBox( NULL, AheLoadString( IDS_CHECKSAVE ), AheLoadString( IDS_CAPTION ), MB_YESNO|MB_ICONEXCLAMATION|MB_TASKMODAL );
				if( iRet == IDNO )
					return TRUE;
			}

			esriEditState State;
			m_cArcHelper.GetEditor()->get_EditState( &State );
			// �����_�C�A���O�ŕҏW�J�n�𐧌䂷�邩�ǂ���
			if( m_bDoOperation && State == esriStateEditing)
			{
				// �ҏW�J�n
				ATLTRACE(_T("�X�V�Ώ� %d\n"), m_lstUpdate.size());
				m_lstUpdate.clear();
				m_cArcHelper.StartEditOperation();
			}

			IEnvelopePtr ipEnv;						// �ĕ`��̈�ێ��p
			// �I�𒆂̃t�B�[�`���Ń��[�v���܂킵�A�ҏW�̂������Ă���t�B�[�`�����݂��A���̃t�B�[�`����Envelop���Ƃ�
			// �����āA�ҏW���������Ă���΁A�ۑ�����
			ISiNDYModifyCheckPtr	ipModifyCheck;
			if( !ipModifyCheck )
				ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );

			bool bIsAnnotation = false;
			std::list<CFeatureDef> listChangedLinkandNode;
			for( std::list<CFeatureDef>::iterator itDef = m_listFeatureDef.begin(); itDef != m_listFeatureDef.end(); ++itDef ) 
			{
				IFeaturePtr ipFeature(itDef->GetFeature());
				CString strLayerName = AheGetFeatureClassName( ipFeature );
				if( itDef->Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
				{
					// �t�B�[���h�̃`�F�b�N
					if( !CheckValue( &(*itDef) ) )
					{
						// �����ŃG���[�ɂȂ����ꍇ�ɂ̓A�{�[�g���Ȃ��Ƃ����Ȃ�
						if( m_bDoOperation && State == esriStateEditing )
							m_cArcHelper.AbortEditOperation();	// �ҏW�I��
						return false;
					}
					// �����ɕҏW����Ă��܂����ꍇ������̂ŁA�ŏI�I�ɂ�����Modify�`�F�b�N������
					if( ipModifyCheck )
					{
						VARIANT_BOOL vbModified = VARIANT_FALSE;
						CComBSTR bstrMsg;
						if( SUCCEEDED( ipModifyCheck->IsModified( ipFeature, &vbModified, &bstrMsg ) ) )
						{
							if( vbModified == VARIANT_TRUE )
							{
								AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
								AheMessageBox( NULL, AheLoadString( IDS_CLOSEANDNOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

								if( m_bDoOperation && State == esriStateEditing )
									m_cArcHelper.AbortEditOperation();	// �ҏW�I��
								return TRUE;
							}
						}
					}

					// �ۑ�
					itDef->Store( m_bDoOperation );

					// ���L���C���͒��L�c�[�����œƎ��ɍĕ`�悩����̂ŁA�ĕ`��͈͎擾��l�b�g���[�N�`�F�b�N���Ȃ�
					if( AheIsChukiLayer( strLayerName ) )
					{
						// �����ɓ����Ă���̂́A���L�c�[�����炵�����肦�Ȃ�
						bIsAnnotation = true;
						continue;
					}
					//	[Bug 5370]�ύX����Ă��Ȃ����H�����N�̃m�[�h��ʂ�ύX���Ă͂����Ȃ��΍�
					// �ۑ������Changed�t���O������FALSE�ɂȂ��Ă��܂��̂ŁA�����ł��炽�Ɋm�ۂ��Ă���
					// �ύX���������������N�Ɋ֘A����m�[�h�ƁA�ύX�����������m�[�h�݂̂ł悢�͂�
					if( AheIsNetwork( ipFeature ) )
						listChangedLinkandNode.push_back( *itDef );

					
					// �ĕ`��͈͂��擾
					if( ipFeature )
					{
						sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
						IPolygonPtr ipPolygon;
						// �|���S���̏ꍇ�̓o�b�t�@�����O���Ȃ�
						if( kinds == sindyePolygonFeature )
						{
							IGeometryPtr ipRefreshGeom;
							ipFeature->get_Shape( &ipRefreshGeom );
							ipPolygon = ipRefreshGeom;
						}
						else
						{
							// �����o�b�t�@�����O����
							DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 15 );
							AheFeature2Polygon( ipFeature, dBufferSize, &ipPolygon );
						}
						// [Bug 4883]�s���Ȍ`���I�����Ă��܂����ꍇ�ɗ������肪����̂ŁANULL�`�F�b�N���Ă���
						if( ipPolygon )
						{
							IEnvelopePtr ipFeatEnv;
							ipPolygon->get_Envelope( &ipFeatEnv );
							if( !ipEnv )
								ipEnv = ipFeatEnv;
							else
								ipEnv->Union( ipFeatEnv );
						}
					}
				}
			}

			// [Bug 5370]�ύX����Ă��Ȃ����H�����N�̃m�[�h��ʂ�ύX���Ă͂����Ȃ��΍�
			// �ύX�̂������������N�Ɋւ���m�[�h�ƕύX���������m�[�h�̃m�[�h��ʂ̍Đݒ�
			for( std::list<CFeatureDef>::iterator it = listChangedLinkandNode.begin(); it != listChangedLinkandNode.end(); it++ ) 
			{
				// �ۑ�������̂����H�����N�̏ꍇ�A�m�[�h��ʎ����؊������邽�߂Ƀ`�F�b�N����B
				// [Bug 5370]�ύX����Ă��Ȃ����H�����N�̃m�[�h��ʂ�ύX���Ă͂����Ȃ����߁A�ύX���������Ă��铹�H�����N�����ׂ�
				if( AheIsNetworkLink( (IFeaturePtr)it->GetFeature() ) )
				{
					// [Bug 5389]���s��NW�Ή��F�l�b�g���[�N�����N�ɂ��Ă���m�[�h���擾
					CTargetItems	cNodeItems;
					AheGetConnectedNetworkNode( it->GetFeature(), cNodeItems );
					for(CTargetItems::iterator itNode = cNodeItems.begin(); itNode != cNodeItems.end(); ++itNode )
					{
						// �����ɕҏW����Ă��܂����ꍇ������̂ŁA�ŏI�I�ɂ�����Modify�`�F�b�N������
						if( ipModifyCheck )
						{
							VARIANT_BOOL vbModified = VARIANT_FALSE;
							CComBSTR bstrMsg;
							if( SUCCEEDED( ipModifyCheck->IsModified( itNode->GetFeature(), &vbModified, &bstrMsg ) ) )
							{
								if( vbModified == VARIANT_TRUE )
								{
									AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
									AheMessageBox( NULL, AheLoadString( IDS_CONFLICTNODE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

									if( m_bDoOperation == TRUE && State == esriStateEditing )
										m_cArcHelper.AbortEditOperation();	// �ҏW�I��
									return TRUE;
								}
							}
						}
						StoreRoadNord( itNode->GetFeature() );
					}
				}
				else if( AheIsNetworkNode( (IFeaturePtr)it->GetFeature() )  )
					StoreRoadNord( it->GetFeature() );
			}

			// �����_�C�A���O���ł̑����X�V
			if( ! Update() )
			{
				if( m_bDoOperation && State == esriStateEditing )
					m_cArcHelper.AbortEditOperation();
				AheMessageBox( NULL, AheLoadString( IDS_UPDATE_FAILED ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
				return FALSE;
			}

			// �ҏW�I��
			if( m_bDoOperation && State == esriStateEditing )
			{
				m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) );	// �ҏW�I��
				FinalModifyCheck();
			}

			// ���L�̍ĕ`��͒��L���œƎ��ɂ���Ă���̂ŁA�����_�C�A���O�ł͍s��Ȃ�
			if( !bIsAnnotation )
				m_cArcHelper.GetActiveView()->PartialRefresh( (esriViewDrawPhase)( esriViewGeography|esriViewGraphics ), NULL, ipEnv );
		}
		else
		{
			m_bIsEnding = FALSE;
			return FALSE;
		}
	}
	return TRUE;
}

// �m�[�h��ʂ̍Đݒ���s���֐�
bool CAttributeDlgObj::StoreRoadNord( IFeaturePtr ipNodeFeature )
{
	// �m�[�h��ʂ��擾
	IFieldsPtr		ipNodeFields;
	ipNodeFeature->get_Fields(&ipNodeFields);
	long			lNodeClass = 0;
	ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
	CComVariant		vNodeClass;
	ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
	sindyeNodeClass	sNodeClass = sindyeNodeClass(vNodeClass.lVal);

	// [Bug 5389]���s��NW�Ή��F�l�b�g���[�N�m�[�h�ɂ��Ă��邷�ׂẴ����N���擾
	CTargetItems	cRoadItems;
	AheGetConnectedNetworkLink( ipNodeFeature, cRoadItems );

	for ( CTargetItems::iterator itRoad = cRoadItems.begin(); itRoad != cRoadItems.end(); ++itRoad ) {
		ATLTRACE(_T("%d\n"), (*itRoad).GetOID());
	}

	// �m�[�h��ʂ������ݒ�
	BOOL	bIsChanged = FALSE;
	if( AheResetNodeAttribute( ipNodeFeature, cRoadItems, &bIsChanged, TRUE ) == TRUE )
	{
		// �m�[�h��ʕύX����Ă�����
		if( bIsChanged ) {
			LONG lOID = 0;
			ipNodeFeature->get_OID(&lOID);
			ATLTRACE(_T("%d\n"), lOID);
			ipNodeFeature->Store();
		}
	}
	return ( bIsChanged ) ? true : false;
}

// �����N��̕ۑ����Ǘ�����֐�
BOOL CAttributeDlgObj::LQStoreFeature()
{
	BOOL bStore = TRUE;
	if( Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable)
	{
		if( ErrorCheck() )
		{
			// �t�B�[�`���̐؂�ւ����������Ƃ��ɂ̓_�C�A���O���o���āA�ۑ����邩�������˂�
			// �ύX�����������ǂ��������Ă����Ȃ��ƁA�V�K�쐬�����΂ꂽ�Ƃ��ɁA�ŏ��ɉ��̃��b�Z�[�W���J���Ă��܂�
			if(	m_bIsSetFeature && Changed())
			{
				int iRet = AheMessageBox( NULL, AheLoadString( IDS_CHECKSAVE ), AheLoadString( IDS_CAPTION ), MB_YESNO|MB_ICONEXCLAMATION|MB_TASKMODAL );
				if( iRet == IDNO )
					return TRUE;
			}

			esriEditState State;
			m_cArcHelper.GetEditor()->get_EditState( &State );
			if( m_bDoOperation && State == esriStateEditing )
			{
				// �ҏW�J�n
				ATLTRACE(_T("�X�V�Ώ� %d\n"), m_lstUpdate.size());
				m_lstUpdate.clear();
				m_cArcHelper.StartEditOperation();
			}

			// �ۑ�
			IEnvelopePtr ipEnv;						// �ĕ`��̈�ێ��p
			ISiNDYModifyCheckPtr	ipModifyCheck;
			for( std::list<CLQRowDef>::iterator i = m_listLQRowDef.begin(); i != m_listLQRowDef.end(); i++ ) 
			{
				if( i->Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
				{
					CString strLayerName = i->GetTableName();
					// �t�B�[���h�̃`�F�b�N
					if( !LQCheckValue( &(*i) ) )
					{
						// �����ŃG���[�ɂȂ����ꍇ�ɂ̓A�{�[�g���Ȃ��Ƃ����Ȃ�
						if( m_bDoOperation == TRUE && State == esriStateEditing )
							m_cArcHelper.AbortEditOperation();	// �ҏW�I��
						return false;
					}

					_IRowPtr ipModifyCheckRow( i->GetFeature() );
					// �����ɕҏW����Ă��܂����ꍇ������̂ŁA�ŏI�I�ɂ�����Modify�`�F�b�N������
					if( !ipModifyCheck )
						ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );
					if( ipModifyCheck )
					{
						VARIANT_BOOL vbModified = VARIANT_FALSE;
						CComBSTR bstrMsg;
						if( SUCCEEDED( ipModifyCheck->IsModified( ipModifyCheckRow, &vbModified, &bstrMsg ) ) )
						{
							if( vbModified )
							{
								AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
								AheMessageBox( NULL,  AheLoadString( IDS_CLOSEANDNOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

								if( m_bDoOperation && State == esriStateEditing )
									m_cArcHelper.AbortEditOperation();	// �ҏW�I��
								return TRUE;
							}
						}
					}

					// �ۑ�
					i->Store( m_bDoOperation );

					// �ĕ`��͈͎擾
					_IRowPtr ipRow(i->GetFeature());
					if( ipRow )
					{
						IGeometryPtr ipGeom = i->GetShape();
						// �����o�b�t�@�����O����
						DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
						IPolygonPtr ipPolygon;
						AheGeometry2Polygon( ipGeom, dBufferSize, &ipPolygon );
						// [Bug 4883]�s���Ȍ`���I�����Ă��܂����ꍇ�ɗ������肪����̂ŁANULL�`�F�b�N�����Ă���
						if( ipPolygon )
						{
							IEnvelopePtr ipFeatEnv;
							ipPolygon->get_Envelope( &ipFeatEnv );
							if( !ipEnv )
								ipEnv = ipFeatEnv;
							else
								ipEnv->Union( ipFeatEnv );
						}
					}
				}
			}
			// �ҏW�I��
			if( m_bDoOperation && State == esriStateEditing )
			{
				m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) );	// �ҏW�I��
				FinalModifyCheck();
			}

			// �ύX�����������C���̂ݍĕ`��
			if( ipEnv )
				m_cArcHelper.GetActiveView()->PartialRefresh( (esriViewDrawPhase)(esriViewGeography|esriViewGraphics), NULL, ipEnv );
		}
		else
		{
			m_bIsEnding = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}

// �_�C�A���O����鎞�̏���
BOOL CAttributeDlgObj::CloseDlg( BOOL bStore )
{
	m_bIsEnding = TRUE;	// �I�������J�n

	// �_�C�A���O�̈ʒu���L��
	GetWindowRect( &m_cDlgRect );

	// �ۑ����邩�ǂ���
	if( bStore )	// OK�{�^�����������Ƃ�
	{	// �����������Εۑ�����i���ɕύX�ȂǂȂ��ꍇ�͕ۑ����Ȃ��j
		if( !m_bIsLQAttribute )
		{
			if( !StoreFeature() )
				return FALSE;
		}
		else
		{	// �����������Εۑ�����i���ɕύX�ȂǂȂ��ꍇ�͕ۑ����Ȃ��j
			if( !LQStoreFeature() )
				return FALSE;
		}
	}
	else					// �L�����Z���{�^�����������Ƃ�
	{
		// �j������ۂ̃_�C�A���O��\������i���ɕύX�ȂǂȂ��ꍇ�͏o���Ȃ��j
		if( Changed() )
			m_bIsChanged = TRUE;			// �ύX�t���O�𗧂Ă�
		m_bIsCancel = TRUE;
	}

	ConnectToEvents( FALSE );

	// �c���[�����R���N�V����������
	m_cTreeCtrl.Clear();
	m_cTreeCtrl.DeleteAllItems();

	// �_�C�A���O���X�g��S�Ĕj������
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* cExportDlg = it->second;
		CString strLayerName = it->first;

		// �^�u�ʒu���o���Ă���
		if( strLayerName == _T("ROAD_LINK") )
			m_lRoadTabNumber = cExportDlg->GetTabNum();
		else if( strLayerName.Find(_T("DRMA_LINK")) != -1 )
			m_lDRMATabNumber = cExportDlg->GetTabNum();
		else if( strLayerName == _T("ACCESS_POINT") )
			m_lAccessTabNumber = cExportDlg->GetTabNum();
		else if( strLayerName.Find(_T("VICS")) != -1 )
			cExportDlg->SendMessage( WM_REFRESHFEATURE, 1, 0);
		else if( strLayerName.Find( _T("HIGHWAY_NODE") ) != -1 )
		{
			m_lHwyNodeTabNumber = cExportDlg->GetTabNum();	// [Bug6570] ���������[�����p�Ƀ^�u��ǉ������̂ŏC��
			cExportDlg->SendMessage( WM_REFRESHFEATURE, 1, 0);
		}
		cExportDlg->ClearFeatureDefs();
		cExportDlg->Delete();
	}

	for( std::map<CString, HMODULE>::iterator itModule = m_mapModule.begin(); itModule != m_mapModule.end(); itModule++ )
	{
		HMODULE hModule = itModule->second;
		FreeLibrary( hModule );
	}
	m_mapDlg.clear();
	m_mapModule.clear();
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	// �o�^�p�t�B�[�`���̃N���A
	m_cFeatureItems.clear();
	m_listLQ.clear();
	// �t�B�[�`�����X�g�N���A
	m_listFeatureDef.clear();
	m_listLQRowDef.clear();

	// �K�����������ďI�����Ă���
	m_lMode = 0;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]���̃`�b�v�p

	// �ĕ`�悵�Ă��݂�����
	if( m_ipSelectFeatEnv )
	{
		DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), m_lRefreshBuf );
		m_ipSelectFeatEnv->Expand(  dBufferSize, dBufferSize, VARIANT_FALSE );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, m_ipSelectFeatEnv );
		m_ipSelectFeatEnv = NULL;
	}

	// �_�C�A���O������
	if( !m_bIsModal )
		DestroyWindow();	// ���[�h���X�̏ꍇ
	else
		EndDialog(0);		// ���[�_���̏ꍇ

	m_bClosedDialog = TRUE;
	m_lstUpdate.clear();

	// ����ȂƂ���ŁA�t���O���������Ă����E�E�E
	m_bIsFirst = TRUE;
	WriteProperty();

	return TRUE;
}

// OK�{�^���̕\����Ԃ�ύX����
void CAttributeDlgObj::SetButton( BOOL bEdited )
{
	esriEditState state = esriStateNotEditing;
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	ipEditor->get_EditState( &state );

	// �ҏW�J�n����Ă��āA�V�K�쐬�Ȃǃ��[���ɏ]��Ȃ��ꍇ�ɂ�OK�{�^���������Ԃɂ��Ă����Ȃ���΁A
	// �ύX���|�����Ă��Ȃ��ꍇ�ɁAOK�{�^�������Ȃ��Ȃ��Ă��܂�
	if( m_sAttributeEditType == sindyeNotApplyRuleEnable && state == esriStateEditing)
		bEdited = TRUE;

	if( state != esriStateEditing )	// �ҏW�J�n���ꂽ�Ƃ��̃f�[�^�t���[�����A�N�e�B�u�f�[�^�t���[���ł���ꍇ�̂݉�����悤�ɂ���
		bEdited = FALSE;

	::EnableWindow( GetDlgItem( IDOK ), bEdited );

	// �c���[�R���g���[���̃A�C�e���̐F�������^�C�~���O�ŕς���K�v������̂ōĕ`��
	m_cTreeCtrl.Invalidate( FALSE );
}

// �_�C�A���O�����T�C�Y����Ƃ��̊֐�
void CAttributeDlgObj::ResizeClientDlg()
{
	if( ::IsWindow(m_hCurrentWnd) )
	{
		CRect rect;
		::GetWindowRect( GetDlgItem(IDC_STATIC_BASE), rect );
		ScreenToClient( rect );
		// [Bug 5490]�c���[�\�����ɃS�~���c��o�O
		HDWP hDWP = BeginDeferWindowPos( 1 );
		hDWP = ::DeferWindowPos( hDWP, m_hCurrentWnd, HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		EndDeferWindowPos( hDWP );
	}
}

// ���ێ����Ă���t�B�[�`���ɕύX�����邩�ǂ���
BOOL CAttributeDlgObj::Changed()
{
	// �ҏW����Ă��邩�ǂ����m�F
	BOOL bChanged = FALSE;
	for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
	{
		if( it->Changed() ) 
			return TRUE;
	}

	for( std::list<CLQRowDef>::iterator itLQ = m_listLQRowDef.begin(); itLQ != m_listLQRowDef.end(); itLQ++ )
	{
		if( itLQ->Changed() ) 
			return TRUE;
	}

	return FALSE;
}

// �C�x���g�ɐڑ�����֐�
BOOL CAttributeDlgObj::ConnectToEvents( BOOL bConnect/* = TRUE */ )
{
	BOOL bRet = FALSE;

	IEditorPtr ipEdit( m_cArcHelper.GetEditor() );
	if( bConnect == TRUE )
	{
		// IActiveViewEvents �ɐڑ����܂�
		if( m_dwViewCookie != 0 )
		{
			ATLASSERT(FALSE);
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : �ڑ��N�b�L�[�� 0 �ł͂���܂��񂪁A�����I�ɐڑ����܂�\n"));
		}
		if( FAILED( AtlAdvise(m_cArcHelper.GetActiveView(), this->GetUnknown(), IID_IActiveViewEvents, &m_dwViewCookie) ) )
		{
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : ActiveViewEvents �ւ̐ڑ������s���܂���\n"));
			bRet = FALSE;
		}

		// IEditEvents �ɐڑ����܂�
		if( m_dwEditCookie != 0 )
		{
			ATLASSERT(FALSE);
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : �ڑ��N�b�L�[�� 0 �ł͂���܂��񂪁A�����I�ɐڑ����܂�\n"));
		}
		if( FAILED( AtlAdvise(ipEdit, this->GetUnknown(), IID_IEditEvents, &m_dwEditCookie) ) )
		{
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : EditEvents �ւ̐ڑ������s���܂���\n"));
			bRet = FALSE;
		}
	}
	else {
		// �C�x���g�ڑ�����
		if( m_dwViewCookie != 0 )
		{
			if( FAILED( AtlUnadvise( m_cArcHelper.GetActiveView(), IID_IActiveViewEvents, m_dwViewCookie) ) )
			{
				ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : ActiveViewEvents �ւ̐ڑ����������s���܂���\n"));
				bRet = FALSE;
			} else
				m_dwViewCookie = 0;
		}

		if( m_dwEditCookie != 0 )
		{
			if( FAILED( AtlUnadvise( ipEdit, IID_IEditEvents, m_dwEditCookie) ) )
			{
				ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : EditEvents �ւ̐ڑ����������s���܂���\n"));
				m_dwEditCookie = 0;
				bRet = FALSE;
			} else
				m_dwEditCookie = 0;
		}
	}

	return bRet;
}

// �q�_�C�A���O�������Ă���֐�
CExportDlg* CAttributeDlgObj::SetChildDlg(CString strClassName)
{
	CExportDlg* cExportDlg = NULL;

	CString strDRMAClassName;
	int iDRMALINKClassName = -1,iDRMAINLINEClassName = -1,iDRMAINPOINTClassName = -1,iDRMANODEClassName = -1,iVICSClassName = -1,iBAnnoName = -1, iMAnnoName = -1,iTAnnoName = -1,iAnnotation = -1,iDispline = -1;

	// DRMA�����N���ǂ������ׂ�
	iDRMALINKClassName = strClassName.Find(_T("DRMA_LINK"));
	if( iDRMALINKClassName == -1 )
		iDRMALINKClassName = strClassName.Find(_T("DRMADIFF_LINK"));

	// DRMA�m�[�h���ǂ������ׂ�
	iDRMANODEClassName = strClassName.Find(_T("DRMA_NODE"));
	if( iDRMANODEClassName == -1)
		iDRMANODEClassName = strClassName.Find(_T("DRMADIFF_NODE"));

	// DRMA�����N���������ǂ������ׂ�
	iDRMAINLINEClassName = strClassName.Find(_T("DRMA_INLINE"));

	// DRMA�����N���|�C���g���ǂ������ׂ�
	iDRMAINPOINTClassName = strClassName.Find(_T("DRMA_INPOINT"));

	// VICS���ǂ������ׂ�
	iVICSClassName = -1;
	if( m_bIsLQAttribute == FALSE )
		iVICSClassName = strClassName.Find(_T("VICS"));

	std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( strClassName );
	if( it == m_mapDlg.end() )
	{
		if( m_strInstallPath.IsEmpty() == TRUE )
		{
			CInifileHelper	cIniFile;
			// SiNDY-e�C���X�g�[�����[�g�p�X�擾
			cIniFile.GetInstallRoot( m_strInstallPath );
		}

		// DRMA�����N�Ȃ�A�N���X�l�[�����擾���ADRMA_LINK.DLL���Ăяo��
		if( iDRMALINKClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_LINK");
		}
		// DRMA�m�[�h�Ȃ�A�N���X�l�[�����擾���ADRMA_NODE.DLL���Ăяo���Ă���
		else if( iDRMANODEClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_NODE");
		}
		// DRMA�����N�������Ȃ�A�N���X�l�[�����擾���ADRMA_INLINE.DLL���Ăяo���Ă���
		else if( iDRMAINLINEClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_INLINE");
		}
		// DRMA�����N���|�C���g�Ȃ�A�N���X�l�[�����擾���ADRMA_INLINE.DLL���Ăяo���Ă���
		else if( iDRMAINPOINTClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_INPOINT");
		}
		// VICS�Ȃ�A�N���X�l�[�����擾���AVICS.DLL���Ăяo���Ă���
		else if( iVICSClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("VICS");
		}

		CString strModuleName = strClassName;
		strModuleName += _T(".dll");
		strModuleName = m_strInstallPath + _T("ChildDlgDLL\\") + strModuleName;

#ifdef _USE_CHILDDLG_SHORTCUT
		HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
		HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

		if( hModule != NULL )
		{
			if( ! cExportDlg )
			{
				CreateDlgByName *pCreateTest = (CreateDlgByName *)GetProcAddress(hModule,"CreateDlgByName"); // �q�_�C�A���O�����ŌĂяo��
				if( pCreateTest )
					cExportDlg = pCreateTest( strClassName );
				else {
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");
					cExportDlg = pCreateTest();
				}
			}
			else
				_ASSERTE( ! cExportDlg );

			cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );

			// �^�u�ʒu���Z�b�g
			if( strClassName == _T("ROAD_LINK") )
				cExportDlg->SetTabNum(m_lRoadTabNumber);
			else if( strClassName == _T("DRMA_LINK") )
				cExportDlg->SetTabNum(m_lDRMATabNumber);
			else if( strClassName == _T("ACCESS_POINT") )
				cExportDlg->SetTabNum(m_lAccessTabNumber);

			m_cExportDlg = cExportDlg;

			if( strClassName == _T("HIGHWAY_NODE") )
			{
				cExportDlg->SetTabNum(m_lHwyNodeTabNumber);	// [Bug6570] ���������[�����p�Ƀ^�u��ǉ������̂ŏC��
			}

			if( iDRMALINKClassName != -1 || iDRMANODEClassName != -1 || iDRMAINLINEClassName != -1 || 
				iDRMAINPOINTClassName != -1 || iVICSClassName != -1 || iBAnnoName != -1 || iMAnnoName != -1 || iTAnnoName != -1 )
				m_mapModule[strDRMAClassName] = hModule;
			else
				m_mapModule[strClassName] = hModule;
		}
	}
	else
		cExportDlg = it->second;

	// VICS�n�q�_�C�A���O�����ŕK�v�������̂őS�Ă̎q�_�C�A���O�ŌĂԂ悤�C��
	if( cExportDlg != NULL )
		cExportDlg->SetFeatureClassName( strDRMAClassName.IsEmpty() ? strClassName : strDRMAClassName );

	return cExportDlg;
}

// �e��t�B�[�`���̃`�F�b�N
BOOL CAttributeDlgObj::ErrorCheck()
{
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* cExportDlg = it->second;
		if( cExportDlg->ErrorCheck() == FALSE )
			return FALSE;
	}
	return TRUE;
}


// �`�F�b�N��̃f�[�^�X�V����
bool CAttributeDlgObj::Update()
{
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg2* cExportDlg2 = dynamic_cast<CExportDlg2*>(it->second);
		if( ! cExportDlg2 )
			continue;

		if( ! cExportDlg2->Update() )
			return false;
	}
	return true;
}


// �����_�C�A���O�֘A�������W�X�g���ɏ�������
void CAttributeDlgObj::WriteProperty()
{
	// ������Ԃ̎��ɂ̓t���O������������Ă��Ȃ��̂ŁA�����Ƀt���O��FALSE�ɂ��Ă���
	if( m_bIsGridEditMode != TRUE && m_bIsGridEditMode != FALSE)
	{
		CString strRegOpenKey;
		strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
		if ( hOpenKey ) 
		{
			m_bIsGridEditMode = FALSE;
			EmxSetRegDword( _T("GridEditMode"), (DWORD)m_bIsGridEditMode, hOpenKey );
			RegCloseKey( hOpenKey );
		}
	}
}

// �����_�C�A���O�֘A�������W�X�g������ǂݎ��
void CAttributeDlgObj::LoadProperty()
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = NULL;
	hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		DWORD dwResult = 0;
		if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) )
			m_bIsGridEditMode = (BOOL)dwResult;
		if( EmxGetRegDword( _T("IsLineHilight"), &dwResult, hOpenKey ) )
			m_bIsLineHilight   = ((BOOL)dwResult)? true : false;
		if( EmxGetRegDword( _T("IsVertexHilight"), &dwResult, hOpenKey ) )
			m_bIsVertexHilight = ((BOOL)dwResult)? true : false;

		RegCloseKey( hOpenKey );
	}

	// �t�B�[�`���̍ĕ`��͈́i�f�t�H���g�F5Map�P�ʁj
	strRegOpenKey.Format( REGCOMMONOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		if( !EmxGetRegDword( _T("RefreshBuffer"), &m_lRefreshBuf, hOpenKey ) )
			m_lRefreshBuf = 5;
		RegCloseKey( hOpenKey );
	}
}

// �c�[���`�b�v�̏������E�X�V�̊֐�
void CAttributeDlgObj::InitToolTip(const int nID)
{
	// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
	CStatic cStatic( GetDlgItem(nID) );
	INT nTextLen = cStatic.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	cStatic.GetWindowText( lpszText, nTextLen );

	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ;
	// TTM_ADDTOOL���b�Z�[�W�𐳏�ɑ��邽�߂̂��܂��Ȃ��ibug 3934�j
	// c.f. http://ameblo.jp/blueskyame/entry-10398978729.html
	ti.cbSize = sizeof(TOOLINFO) - sizeof(void*) ;
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ;
	ti.hwnd = GetDlgItem(nID);
	ti.uId = (UINT)GetDlgItem(nID).m_hWnd;
	CString str = lpszText;
	str.Remove( _T('\n') );
	lpszText = (LPTSTR)(LPCTSTR)str;
	ti.lpszText = lpszText;

	if( IDC_STATIC_MESSAGE == nID )
	{
		if( m_hMessageWND == NULL )
		{
			// �c�[���`�b�v�쐬
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
				GetDlgItem(nID),// �e�E�B���h�E�̃n���h��
				NULL,			// ���j���[�n���h��
				NULL,			// �C���X�^���X�n���h��
				NULL);			// WM_CREATE�f�[�^
			m_hMessageWND = hToolTip;
			::SendMessage( m_hMessageWND, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWND, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
			::SendMessage( m_hMessageWND, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);
		}
		else
		{
			::SendMessage( m_hMessageWND, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWND, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
			::SendMessage( m_hMessageWND, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
		}
	}

	// [bug 11183]���̃`�b�v�p
	if( IDC_STATIC_ALTERNATIVENAME == nID || IDC_STATIC_OFFICIALNAME == nID )
	{
		if( m_hMessageWNDEnglish == NULL )
		{
			if( !m_OfficialNameFont )
				m_OfficialNameFont.CreatePointFont( 140, AheLoadString(IDS_FONT) );

			// �c�[���`�b�v�쐬
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
				GetDlgItem(nID),// �e�E�B���h�E�̃n���h��
				NULL,			// ���j���[�n���h��
				NULL,			// �C���X�^���X�n���h��
				NULL);			// WM_CREATE�f�[�^
			m_hMessageWNDEnglish = hToolTip;
			::SendMessage( m_hMessageWNDEnglish, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWNDEnglish, WM_SETFONT , (WPARAM)(HFONT)m_OfficialNameFont , TRUE);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)600);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);
		}
		else
		{
			::SendMessage( m_hMessageWNDEnglish, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWNDEnglish, WM_SETFONT , (WPARAM)(HFONT)m_OfficialNameFont , TRUE);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)600);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
		}
	}
}

// ���f�B�t�@�C�`�F�b�N������֐�
void CAttributeDlgObj::FinalModifyCheck()
{
	VARIANT vt;
	_IRowPtr ipRow;
	sindyUpdateType ut = sindyUpdateTypeNoUpdate;
	sindyeEditType et = sindyeEditCreate;

	ATLTRACE(_T("�X�V�Ώ� %d\n"), m_lstUpdate.size());

	for ( std::list<CAdapt<_IRowPtr> >::iterator itItem = m_lstUpdate.begin(); itItem != m_lstUpdate.end(); itItem++ ) {
		/// �Ō�̃I�y���[�V�������擾
		ipRow = (*itItem).m_T;
		vt = AheGetAttribute(ipRow, UPDATETYPE_CFIELD);

	#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("%s %d %d\n"), AheGetFeatureClassName(ipRow), lOID, vt.lVal);
	#endif
		if ( vt.vt != VT_NULL ) {
			ut = static_cast<sindyUpdateType>(vt.lVal);

			switch ( ut ) {
			case sindyUpdateTypeNoUpdate:
				//!< �ڍs������
				break;
			case sindyUpdateTypeCreate:
				et = sindyeEditCreate;
				break;
			case sindyUpdateTypeDelete:
				et = sindyeEditDelete;
				break;
			case sindyUpdateTypeShape:
				et = sindyeEditMove;
				break;
			case sindyUpdateTypeProperty:
				et = sindyeEditAttribute;
				break;
			}

			if ( ut != sindyUpdateTypeNoUpdate ) {
				ISiNDYModifyCheckPtr	ipModifyCheck;
				if( ipModifyCheck == NULL )
					ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );

				if( ipModifyCheck != NULL )
				{
					VARIANT_BOOL vbModified = VARIANT_FALSE;
					CComBSTR bstrMsg;
					if( SUCCEEDED( ipModifyCheck->IsModified( ipRow, &vbModified, &bstrMsg ) ) )
					{
						if( vbModified == VARIANT_TRUE )
						{
							try {
								IOperationStackPtr ipOpStack;

								if( SUCCEEDED( ((IMxDocumentPtr)m_cArcHelper.GetDocument())->get_OperationStack( &ipOpStack ) ) ) {
									IWorkspacePtr	ipEditWorkspace;

									m_cArcHelper.GetEditor()->get_EditWorkspace( &ipEditWorkspace );
									IWorkspaceEditPtr ipWorkspaceEdit( ipEditWorkspace );

									if( SUCCEEDED( ipWorkspaceEdit->UndoEditOperation() ) ) {
										LONG lOpCount = -1;

										if( SUCCEEDED( ipOpStack->get_Count( &lOpCount ) ) ) {
											ipOpStack->Remove(lOpCount - 1);
										}
									}
								}
							} catch (...) {
								AheMessageBox(NULL, AheLoadString( IDS_FAILTOUNDO ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
							}

							break;
						}
					}
				}
			}
		}
	}
}

// �ݒ肳��Ă��鑮����DB�I�ȃG���[���Ȃ����ǂ����𒲂ׂ�֐�
BOOL CAttributeDlgObj::CheckValue(CFeatureDef *pFeatureDef)
{
	long lOID = 0;
	_IRowPtr ipRow( pFeatureDef->GetFeature() );
	ipRow->get_OID( &lOID );
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	// PGDB�ł�esriFieldTypeInteger�̌����`�F�b�N���ł��Ȃ��̂ŁA���ʂ���K�v������
	CString strFeatureClassName = AheGetFeatureClassName( ipRow );
	ITablePtr ipTable = AheGetFeatureClass( ipRow );
	esriWorkspaceType WorkspaceType = esriRemoteDatabaseWorkspace;
	CComBSTR bstrName;
	if( ipTable )
	{
		IDatasetPtr ipDataset = ipTable;
		if( ipDataset )
		{
			ipDataset->get_Name( &bstrName );
			IWorkspacePtr ipWorkspace;
			ipDataset->get_Workspace( &ipWorkspace );
			ipWorkspace->get_Type( &WorkspaceType );
		}
	}
	bool bIsPGDB = false;
	if( WorkspaceType != esriRemoteDatabaseWorkspace )
		bIsPGDB = true;

	// ���ׂẴt�B�[���h�`�F�b�N
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName;
		ipField->get_Name( &bstrFieldName );
		// ���݂̒l���擾
		CFieldDef *cFieldDef = pFeatureDef->GetFieldDef( strFeatureClassName, bstrFieldName );
		CComVariant vaValue = cFieldDef->m_vaValue;
		if( !CheckField( ipField, vaValue, lOID, bstrName, bIsPGDB ) )
			return false;
	}
	return true;
}

// �����N��̑�����DB�I�ȃG���[���Ȃ����ǂ����𒲂ׂ�֐�
BOOL CAttributeDlgObj::LQCheckValue(CLQRowDef *pLQRowDef)
{
	long lOID = 0;
	_IRowPtr ipRow( pLQRowDef->GetFeature() );
	ipRow->get_OID( &lOID );
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	// PGDB�ł�esriFieldTypeInteger�̌����`�F�b�N���ł��Ȃ��̂ŁA���ʂ���K�v������
	CString strFeatureClassName = AheGetFeatureClassName( ipRow );
	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	esriWorkspaceType WorkspaceType = esriRemoteDatabaseWorkspace;
	CComBSTR bstrName;
	if( ipTable )
	{
		IDatasetPtr ipDataset = ipTable;
		if( ipDataset )
		{
			ipDataset->get_Name( &bstrName );
			IWorkspacePtr ipWorkspace;
			ipDataset->get_Workspace( &ipWorkspace );
			ipWorkspace->get_Type( &WorkspaceType );
		}
	}
	bool bIsPGDB = false;
	if( WorkspaceType != esriRemoteDatabaseWorkspace )
		bIsPGDB = true;

	// ���ׂẴt�B�[���h�`�F�b�N
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName;
		ipField->get_Name( &bstrFieldName );
		// ���݂̒l���擾
		CFieldDef *cFieldDef = pLQRowDef->GetFieldDef( strFeatureClassName, bstrFieldName );
		CComVariant vaValue = cFieldDef->m_vaValue;
		if( !CheckField( ipField, vaValue, lOID, bstrName, bIsPGDB ) )
			return false;
	}
	return true;
}

// �{���Ȃ�IField::CheckValue�����΂���ł����̂�������Ȃ����A�G���[���b�Z�[�W�������K�v������̂ŁA���傤���Ȃ�
// ���܂��܂ȃp�^�[�����`�F�b�N����
bool CAttributeDlgObj::CheckField( IFieldPtr ipField, CComVariant vaValue, long lOID, CComBSTR bstrName, bool bIsPGDB )
{
	CComBSTR bstrFieldName;
	ipField->get_Name( &bstrFieldName );
	// �t�B�[���h�^�C�v�ɂ���ĕ�����
	esriFieldType type = esriFieldTypeSmallInteger;
	ipField->get_Type( &type );

	// NULL�l�������ꍇ�ɂ�NULLOK���ǂ����𒲂ׂ�
	if( vaValue.vt == VT_NULL )
	{
		VARIANT_BOOL bIsNullable;
		ipField->get_IsNullable( &bIsNullable );
		if( bIsNullable )
			return true;
		else
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_NOTPERMITNULLVALUE ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	// �����W�h���C���������Ă���ꍇ�ɂ́A�h���C�����ɒl�������Ă��邩�ǂ������`�F�b�N
	IDomainPtr ipDomain = NULL;
	IRangeDomainPtr ipRangeDomain = NULL;
	ipField->get_Domain( &ipDomain );
	ipRangeDomain = ipDomain;
	if( ipRangeDomain != NULL )
	{
		CComVariant vaMax, vaMin;
		ipRangeDomain->get_MaxValue( &vaMax );
		ipRangeDomain->get_MinValue( &vaMin );
		// VT_R8�łȂ����Ԃ��Ă���̂ŁAChangeType�ł��킹�Ă���
		// �����Ȃǂ͓����Ă��Ȃ��Ƃ����O���
		vaMax.ChangeType( vaValue.vt );
		vaMin.ChangeType( vaValue.vt );
		if( vaMax < vaValue || vaMin > vaValue )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGEDOMAIN ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	// esriFieldTypeInteger�܂���esriFieldTypeSingle�̏ꍇ
	if( type == esriFieldTypeInteger || type == esriFieldTypeSingle)
	{
		// PGDB�̏ꍇ�ɂ̓`�F�b�N�ł��Ȃ��̂Ő���Ƃ��ĕԂ�
		if( bIsPGDB )
			return true;

		// ���̒l�ɂ��Ă���
		double dCheckValue = 0;
		if( type == esriFieldTypeInteger )
			dCheckValue = abs(vaValue.lVal);
		else if( type == esriFieldTypeSingle )
			dCheckValue = abs(vaValue.fltVal);		// double�Ŏ󂯎���Ă����Ȃ�

		// �S�������擾
		long lPrecision = -1;
		ipField->get_Precision( &lPrecision );
		// �����_�ȉ��̌���
		long lScale = -1;
		ipField->get_Scale( &lScale );
		// �ő�̒l���쐬
		// FLOAT�̏ꍇ�ɂ͑S�������S�A�����_�ȉ��������P�ƂȂ��Ă���ꍇ�ɂ�
		// �ő�l��9999�ł͂Ȃ��A999.9�܂ł̓��͂ƂȂ�A�����_�ȉ��������Q�̏ꍇ�ɂ́A99.99���ő�l�ƂȂ�
		double dLimitValue = 1;
		for( int i = 0; i < lPrecision - lScale; i++ )
			dLimitValue  = dLimitValue * 10;

		if( dLimitValue - dCheckValue <= 0 )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGE ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}

		if( type == esriFieldTypeSingle )
		{
			float dCheckScale = abs(vaValue.fltVal);
			for( int i = 0; i < lScale; i++ )
				dCheckScale = dCheckScale * 10.0;
			// �܂������_�ȉ��̒l����������A�����Ȃ�
			double temp;
			if( modf(dCheckScale, &temp) != 0 )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_DECIMALPOINT ) ,bstrName, lOID, bstrFieldName, lScale );
				AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
				return false;
			}
		}
	}
	else if( type == esriFieldTypeString )
	{
		long lLength = 0;
		ipField->get_Length( &lLength );
		CString strValue = vaValue.bstrVal;

		// [Bug 5418]�Ƃ͒��ڊ֌W�Ȃ����ACString::GetLength�ŕ������Ƃ��Ă��܂��Ă��āA�������̃`�F�b�N�ɂȂ��Ă��܂��Ă����̂ŁA
		// �o�C�g���ł̃`�F�b�N�ɏC���@
		// UNICODE������CString::GetLength() * sizeof( TCHAR )���ƁA���ǁA������*2�ƂȂ��āA�~�����l�������Ȃ��B
		// �Ⴆ��"��a1"�Ƃ���������ɑ΂���6�o�C�g���Ԃ��Ă��Ă��܂��B���p������1�o�C�g�Ƃ����4�o�C�g�Ƃ݂Ȃ������B
		// WideCharToMultiByte��ANSI�ɕϊ��iANSI�ł͔��p������1�o�C�g�A�S�p��2�o�C�g�j���āA�����-1�����Ă�����ƁA���߂�o�C�g���ɂȂ�
		INT nByte = ::WideCharToMultiByte(CP_ACP, 0, T2CW((LPCTSTR)(strValue)), -1, NULL, 0, NULL, NULL) - 1;
		if( nByte > lLength )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGE ),bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	return true;
}

#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/namelib/OfficialNameTable.h>
#include <TDC/namelib/OfficialNameRow.h>
#include <TDC/namelib/AlternativeNameTable.h>
#include <TDC/namelib/AlternativeNameRow.h>
#include <TDC/namelib/TranslationTable.h>
#include <TDC/namelib/TranslationRow.h>
#include <TDC/namelib/name_globals.h>
using namespace sindy;

// �����p�̃t�@�C���_�[�̏������֐�
bool CAttributeDlgObj::InitCollection( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	IWorkspacePtr ipWorkspace( AheGetWorkspace( ipFeature ) );
	esriWorkspaceType type = esriWorkspaceType::esriFileSystemWorkspace;
	ipWorkspace->get_Type( &type );
	CString strTableName;
	if( type == esriWorkspaceType::esriRemoteDatabaseWorkspace )
	{
		CString strOwnerName = AheGetOwnerDotClassName( ipFeature );
		strTableName.Format( _T("%s.%s"), strOwnerName.Left( strOwnerName.Find( _T(".") ) ), official_name::kTableName );
	}
	else
		strTableName = official_name::kTableName;

	ITablePtr ipOfficialName = AheOpenTableByTableName( ipWorkspace, strTableName );
	if( ipOfficialName )
	{
		// �K��������
		m_cFinder.ClearCollection();
		m_cFinder.InitCollection( ipOfficialName, GetNameModel() );
		return true;
	}
	return false;
}

// �t�B�[�`���̖���ID���擾����
long CAttributeDlgObj::GetNameID( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	IFieldsPtr ipFields;
	ipFeature->get_Fields( &ipFields );
	sindy::CFieldMap cFieldMap( ipFields );
	long lIndex = cFieldMap.FindField( sindy::schema::global::name_class::kNameID );
	if( lIndex == -1 )
		return NO_NAME_ID_FIELD;

	CComVariant vaValue;
	ipFeature->get_Value( lIndex, &vaValue );
	if( vaValue.vt != VT_I4 )
		return NO_NAME_ID;
	return vaValue.lVal;
}

// ���̂�\������֐�
bool CAttributeDlgObj::DisplayObjectName( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	// �R���g���[��������
	CStatic cStaticOfficial( GetDlgItem( IDC_STATIC_OFFICIALNAME ) );
	CStatic cStaticAlternative( GetDlgItem( IDC_STATIC_ALTERNATIVENAME ) );
	cStaticOfficial.SetWindowText( _T("") );
	cStaticAlternative.SetWindowText( _T("") );

	if( !ipFeature )
		return false;

	// ������
	if( !InitCollection( ipFeature ) )
		return false;

	// ����ID�擾���Ă݂�
	long lNameID = GetNameID( ipFeature );
	if( lNameID <= 0 )
		return false;

	// ����ID�������猟������
	CSPOfficialNameTable spOfficialNameTable = CAST_OFFICIALTABLE( m_cFinder.FindTable( official_name::kTableName ) );
	if( !spOfficialNameTable || !(ITable*)*spOfficialNameTable )
		return false;

	// ���̌���
	spOfficialNameTable->Select( lNameID );

	// �Ȃ��͂��͂Ȃ��񂾂�
	if( !spOfficialNameTable->empty() )
	{
		// �����܂ł��ꂽ�疼�̂����Ă�
		CSPOfficialNameRow spONRow = CAST_OFFICIALROW( *spOfficialNameTable->begin() );
		cStaticOfficial.SetWindowText( spONRow->GetValue( spOfficialNameTable->GetFieldMap()->FindField( official_name::kName ) ).bstrVal );

		CSPTableContainer spANTable = spONRow->GetItemTable( alternative_name::kTableName );
		if( spANTable && (ITable*)*spANTable && !spANTable->empty() )
		{
			// Alternative�͕����R�Â��ꍇ�����邪�A1�ڂ����I��
			CSPAlternativeNameRow spANRow = CAST_ALTERNATIVEROW( *spANTable->begin() );
			cStaticAlternative.SetWindowText( spANRow->GetValue( spANTable->GetFieldMap()->FindField( alternative_name::kName ) ).bstrVal );
		}
	}
	else
		ATLASSERT( false );

	return true;
}

// [bug 11183]�\���p�t�H���g�̐ݒ�
void CAttributeDlgObj::SetLocalFont( CWindow& wnd )
{
	static CFont font;
	if( font.IsNull() )
	{
		// �f�t�H���g�F MS UI �S�V�b�N 14pt
		CString fontName = uh::reg_util::read( reg_key::LocalFontName, reg_key::path, _T("MS UI Gothic") );
		long fontSize = uh::reg_util::read( reg_key::LocalFontSize, reg_key::path, 14 );
		font.CreatePointFont( fontSize*10, fontName );
	}

	wnd.SetFont( font );
}
