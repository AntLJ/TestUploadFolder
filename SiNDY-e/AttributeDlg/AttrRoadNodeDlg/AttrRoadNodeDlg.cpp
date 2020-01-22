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

// AttrRoadNodeDlg.cpp: CAttrRoadNodeDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrRoadNodeDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�m�[�h�p�j
BEGIN_WINDOW_MAP(ROADNODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_WORKYEAR_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_WORKYEAR_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NODEKINDS_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_NODEKINDS_COMBO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SIGNAL_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SIGNAL)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KANJI_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_KANJI_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_KANA_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_KANA_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_HEIGHTFLAG_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_HEIGHTFLAG_COMBO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_HEIGHT_STATIC)
				RCTOFIT(-1)
				RCREST(IDC_HEIGHT_EDIT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PARKINGID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PARKINGID)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrRoadNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrRoadNodeDlg();
}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrRoadNodeDlg::CAttrRoadNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏������A�t�H���g�쐬�����Ă����܂��B
 */
CAttrRoadNodeDlg::CAttrRoadNodeDlg()
	: ATTR_BASE_CLASS(ROADNODE_MAP, IDD_ATTRROADNODEDLG),
		m_wnd0( _T("EDIT"), this, IDC_WORKYEAR_EDIT ), 
		m_wnd1( _T("COMBOBOX"), this, IDC_NODEKINDS_COMBO ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_SIGNAL ), 
		m_wnd3( _T("EDIT"), this, IDC_KANJI_EDIT ), 
		m_wnd4( _T("EDIT"), this, IDC_KANA_EDIT ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_HEIGHTFLAG_COMBO ), 
		m_wnd6( _T("EDIT"), this, IDC_HEIGHT_EDIT ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_PARKINGID ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE )
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
}

/** 
 * @fn HWND CAttrRoadNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrRoadNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrRoadNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrRoadNodeDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrRoadNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrRoadNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_WORKYEAR_EDIT) );
	}

	SetToolTip(IDC_KANJI_EDIT);
	SetToolTip(IDC_KANA_EDIT);

	return bRet;
}

/**
 * @fn void CAttrRoadNodeDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrRoadNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���H�m�[�h�p
		m_cControlRel.SetControl( IDC_WORKYEAR_EDIT,	TYPE_EDIT,		0,	0,	_T("WORK_YEAR"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_NODEKINDS_COMBO,	TYPE_COMBO,		0,	0,	_T("NODECLASS_C"),	NULL, NODE_TABLE_NAME , NULL, -1, FOLLOW_READONLY );
		m_cControlRel.SetControl( IDC_COMBO_SIGNAL,		TYPE_COMBO,		0,	0,	_T("SIGNAL_C"),		NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KANJI_EDIT,		TYPE_EDITSTRING,0,	0,	_T("NAME_KANJI"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_KANA_EDIT,		TYPE_EDITSTRING,0,	0,	_T("NAME_YOMI"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_HEIGHTFLAG_COMBO,	TYPE_COMBO,		0,	0,	_T("HEIGHT_C"),		NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_HEIGHT_EDIT,		TYPE_EDIT,		0,	0,	_T("HEIGHT"),		NULL, NODE_TABLE_NAME, _T("LINK_ID"), IDC_HEIGHTFLAG_COMBO, FOLLOW_VALUEIS1 );
		m_cControlRel.SetControl( IDC_EDIT_PARKINGID,	TYPE_EDIT,		0,	0,	_T("PARKING_ID"),	NULL, NODE_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, NODE_TABLE_NAME );
	}
}

/**
 * @fn void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
 * @brief �c�[���`�b�v���쐬���܂��B
 *
 * @note �쐬����c�[���`�b�v�͑����ɕύX��������Ȃ����߁A�����ςȂ��Ŗ��Ȃ�
 *
 * @param nTargetItem	[in]	�c�[���`�b�v���쐬����R���g���[���A�C�e����ID
*/
void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
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
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}

/**
 * @fn BOOL CAttrRoadNodeDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɓ��H�m�[�h�̃G���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrRoadNodeDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == NODE_TABLE_NAME )
		{

			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( NODE_TABLE_NAME, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( NODE_TABLE_NAME, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERRORKANA ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERRORKANA ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

