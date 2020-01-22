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

// AttrExTPPointDlg.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "AttrExTPPointDlg.h"


/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i2�����b�V���p�j
BEGIN_WINDOW_MAP(EXTP_POINT_MAP)
	BEGINROWS(WRCT_TOFIT,0,0)
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_ADDRCODE)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDRCODE)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_DUMMY1)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDRNUM1)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR1)
			RCSPACE(10)
			RCTOFIT(IDC_STATIC_ADDRNUM2)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR2)
			RCSPACE(10)
			RCTOFIT(IDC_STATIC_ADDRNUM3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDR3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_DUMMY2)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_ADDRNUM4)
			RCTOFIT(-1)
			RCREST(IDC_STATIC_ADDR4)
		ENDGROUP()
		RCSPACE(3)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_DUMMY3)
			RCTOFIT(-1)
			RCTOFIT(IDC_EDIT_ADDR1)
			RCSPACE(10)
			RCTOFIT(IDC_EDIT_ADDR2)
			RCSPACE(10)
			RCTOFIT(IDC_EDIT_ADDR3)
			RCTOFIT(-1)
			RCTOFIT(IDC_STATIC_DUMMY4)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDR4)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_ADDRNAME)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_ADDRNAME)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_KATAGAKI)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_KATAGAKI)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_KEISAI)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_KEISAI)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CHECK_ADDRGP)
			RCSPACE(30)
			RCTOFIT(IDC_STATIC_INFOSRC)
			RCTOFIT(-1)
//			RCREST(IDC_COMBO_INFOSRC)
			RCREST(IDC_EDIT_INFOSRC)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_POINTSTAT)
			RCTOFIT(-1)
			RCREST(IDC_COMBO_POINTSTAT)
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
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
 * @retval CAttrExTPPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlgByName( LPCTSTR lpcszTableName )
{
	CExportDlg* pDlg = NULL;
	pDlg = new CAttrExTPPointDlg();
	if( pDlg )
		pDlg->SetFeatureClassName( lpcszTableName );

	return pDlg;
}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrExTPPointDlg::CAttrExTPPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrExTPPointDlg::CAttrExTPPointDlg()
	: ATTR_BASE_CLASS(EXTP_POINT_MAP, IDD_ATTREXTPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADDRNAME ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_KATAGAKI ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KEISAI ),
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_POINTSTAT ),
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_ADDRGP ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_INFOSRC ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_ADDR1 ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_ADDR2 ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_ADDR3 ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_ADDR4 )
{
	// �t�H���g�쐬
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,    // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,       // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,     // �o�͐��x
		CLIP_DEFAULT_PRECIS,    // �N���b�v���x
		DEFAULT_QUALITY,        // �i��
		FIXED_PITCH | FF_DONTCARE,// �s�b�`
		AheLoadString(IDS_FONT));   // �t�H���g���

	m_hBrush2 = NULL;
}

/** 
 * @fn HWND CAttrExTPPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrExTPPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrExTPPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrExTPPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrExTPPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrExTPPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_POINTSTAT) );
	}

	// �ҏW�s�ɐݒ肵�Ă���
	SetDlgStatus(IDC_EDIT_ADDR1, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR2, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR3, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDR4, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRCODE, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_ADDRNAME, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KATAGAKI, FALSE, TRUE );
	SetDlgStatus(IDC_EDIT_KEISAI, FALSE, TRUE );
	SetDlgStatus(IDC_CHECK_ADDRGP, FALSE);
	SetDlgStatus(IDC_EDIT_INFOSRC, FALSE,TRUE );

	DevideAddrCode();

	SetToolTip(IDC_EDIT_ADDRNAME);
	SetToolTip(IDC_EDIT_KATAGAKI);
	SetToolTip(IDC_EDIT_KEISAI);

    return bRet;
}

/**
 * @fn void CAttrExTPPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrExTPPointDlg::CreateControlRelation()
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

		// 20���Z���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),		NULL, m_strFeatureClassName );
		// �Z����
		m_cControlRel.SetControl( IDC_EDIT_ADDRNAME,	TYPE_EDITSTRING,0,	0,	_T("ADDRNAME"),		NULL, m_strFeatureClassName );
		// ��������
		m_cControlRel.SetControl( IDC_EDIT_KATAGAKI,	TYPE_EDITSTRING,0,	0,	_T("KATAGAKINAME"),	NULL, m_strFeatureClassName );
		// �f�ږ���
		m_cControlRel.SetControl( IDC_EDIT_KEISAI,		TYPE_EDITSTRING,0,	0,	_T("KEISAINAME"),	NULL, m_strFeatureClassName );
		// �������ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_POINTSTAT,	TYPE_COMBO,		0,	0,	_T("POINTSTAT_C"),	NULL, m_strFeatureClassName );
		// ���|�C���g���t���O
		m_cControlRel.SetControl( IDC_CHECK_ADDRGP,		TYPE_CHECKBOX,	0,	0,	_T("ADDPOINT_F"),	NULL, m_strFeatureClassName );
		// �񋟌�
		// �h���C�����ݒ肳��Ă��邯��ǂ��A���l�ŏo���Ăق����Ƃ̂��ƂȂ̂ŁA������EDITBOX��
		m_cControlRel.SetControl( IDC_EDIT_INFOSRC,		TYPE_EDIT,		0,	0,	_T("INFOSRC_C"),	NULL, m_strFeatureClassName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, m_strFeatureClassName );
	}
}


/**
 * @fn void CAttrExTPPointDlg::DevideAddrCode()
 * @brief 20���Z���R�[�h�𕪉����A���₷���悤�ɕϊ����܂��B
 */
void CAttrExTPPointDlg::DevideAddrCode()
{
	CEdit cAddrEdit( GetDlgItem( IDC_EDIT_ADDRCODE ) );
	CEdit cAddrEdit1( GetDlgItem( IDC_EDIT_ADDR1 ) );
	CEdit cAddrEdit2( GetDlgItem( IDC_EDIT_ADDR2 ) );
	CEdit cAddrEdit3( GetDlgItem( IDC_EDIT_ADDR3 ) );
	CEdit cAddrEdit4( GetDlgItem( IDC_EDIT_ADDR4 ) );

	// ��ΐ�����20���̂悤�Ȃ̂ŁA�Œ�Ŏ擾
	LPTSTR lpszAddrCode = (LPTSTR)_alloca(21*sizeof(TCHAR));
	cAddrEdit.GetWindowText( lpszAddrCode, 21 );
	CString strAddrCode = (LPCTSTR)lpszAddrCode;

	CString strAddr1,strAddr2,strAddr3,strAddr4;
	strAddr1 = strAddrCode.Mid(0,5);
	strAddr2 = strAddrCode.Mid(5,6);
	strAddr3 = strAddrCode.Mid(11,5);
	strAddr4 = strAddrCode.Mid(16,4);

	// �B�X�敄���ƇC�Z���ԍ��͐����̂悤�Ɉ����A00500�Ƃ��ł�������
	// 500�Ȃǂƕ\�������悤�ɂ���
	long lAddr3 = _ttol( (LPTSTR)(LPCTSTR)strAddr3 );
	long lAddr4 = _ttol( (LPTSTR)(LPCTSTR)strAddr4 );
	strAddr3.Format(_T("%d"),lAddr3 );
	strAddr4.Format(_T("%d"),lAddr4 );

	cAddrEdit1.SetWindowText( strAddr1 );
	cAddrEdit2.SetWindowText( strAddr2 );
	cAddrEdit3.SetWindowText( strAddr3 );
	cAddrEdit4.SetWindowText( strAddr4 );
}

/**
 * @fn void CAttrExTPPointDlg::SetToolTip(INT nTargetItem)
 * @brief �c�[���`�b�v���쐬���܂��B
 *
 * @note �쐬����c�[���`�b�v�͑����ɕύX��������Ȃ����߁A�����ςȂ��Ŗ��Ȃ�
 *
 * @param nTargetItem	[in]	�c�[���`�b�v���쐬����R���g���[���A�C�e����ID
*/
void CAttrExTPPointDlg::SetToolTip(INT nTargetItem)
{
	// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
	CEdit edit( GetDlgItem(nTargetItem) );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

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

	// ToolInfo�\���� 
	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ; 
	ti.cbSize = sizeof(TOOLINFO) ; 
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}
