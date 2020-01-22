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

// AttrCityAdminDlg.cpp: CAttrCityAdminDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityAdminDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�s�s�n�}�s���E�p�j
BEGIN_WINDOW_MAP(CITY_ADMIN_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT3)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_CHECK1)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO1)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC4)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT4)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC5)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT5)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC7)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_CHECK2)
				RCTOFIT(-1)
				RCTOFIT(IDC_CITY_ADMIN_CHECK3)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC8)
				RCTOFIT(-1)
				RCREST(IDC_COMBOBOXEX_CITY_ADMIN)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC9)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO3)
			ENDGROUP()
		ENDGROUP()
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
 * @retval CAttrCityAdminDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityAdminDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCityAdminDlg::CAttrCityAdminDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCityAdminDlg::CAttrCityAdminDlg()
	: ATTR_BASE_CLASS(CITY_ADMIN_MAP, IDD_ATTRCITYADMINDLG),
		m_wnd0( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT2 ), 
		m_wnd1( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT3 ), 
		m_wnd2( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK1 ), 
		m_wnd3( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT4 ), 
		m_wnd4( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT5 ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_CITY_ADMIN_COMBO1 ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_CITY_ADMIN_COMBO2 ),
		m_wnd7( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK2 ),
		m_wnd8( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK3 ),
		m_wnd9( _T("COMBOBOX"), this, IDC_COMBOBOXEX_CITY_ADMIN ),
		m_wnd10( _T("COMBO"), this, IDC_CITY_ADMIN_COMBO3 ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd12( _T("EDIT"), this, IDC_EDIT_ADDRCODE2 )
{
}

/** 
 * @fn HWND CAttrCityAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCityAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityAdminDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCityAdminDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCityAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * 2�����b�V�����ׂ��Őڂ��Ă���X�΂��I������Ă���ꍇ�ɂ́A�֘A����X�΂��m�ۂ���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrCityAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_CITY_ADMIN_EDIT2) );
		// PGDB���[�h�ł����DISABLE��
		if( AheIsPGDBEditMode() )
		{
			SetDlgStatus( IDC_CITY_ADMIN_CHECK1, FALSE );
			SetDlgStatus( IDC_CITY_ADMIN_CHECK3, FALSE );
			SetDlgStatus( IDC_CITY_ADMIN_COMBO3, FALSE );
		}
		SetDlgStatus( IDC_COMBOBOXEX_CITY_ADMIN, TRUE );
	}
	else
		SetDlgStatus( IDC_COMBOBOXEX_CITY_ADMIN, FALSE );

	// [bug 7588] [e][�v�]]�����ҏW�_�C�A���O�ōs���E�̊X��F��ҏW������
	CComboBoxEx cComboCity = GetDlgItem( IDC_COMBOBOXEX_CITY_ADMIN );
	CFieldDef* pColorCode = GetCurrentFeatureDef()->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("COLORCODE") );
	long lColorCode = 0;
	if( pColorCode->m_vaValue.vt != VT_NULL )
	{
		lColorCode = pColorCode->m_vaValue.lVal;
		lColorCode -= 1;	// DB���̔ԍ�����A1�������l���R���{�{�b�N�X�̃C���f�b�N�X�l�ɂȂ邽��
	}
	else
		lColorCode = 6;		// null�l�̏ꍇ�́A�R���{�{�b�N�X�̃C���f�b�N�X�l��6

	cComboCity.SetCurSel( lColorCode );

	// [bug 8043] ADDRCODE2�t�B�[���h������΃R���g���[����\��
	bool bHasAddrCode2 = GetCurrentFeatureDef()->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE2") );
	::EnableWindow( GetDlgItem( IDC_EDIT_ADDRCODE2 ), bHasAddrCode2 ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_STATIC_ADDRCODE2 ), bHasAddrCode2 ? TRUE : FALSE );

    return bRet;
}

/**
 * @fn void CAttrCityAdminDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCityAdminDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �s�s�n�}�s���E�p
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT2,		TYPE_EDITSTRING, 	0,	0,	_T("CITYCODE"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT3,		TYPE_EDITSTRING, 	0,	0,	_T("ADDRCODE"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK1,	TYPE_CHECKBOX,		0,	0,	_T("TMPCODE_F"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO1,	TYPE_COMBO,			0,	0,	_T("ADDRCLASS_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT4,		TYPE_EDIT,			0,	0,	_T("GAIKUFUGO"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT5,		TYPE_EDITSTRING, 	0,	0,	_T("EXTGAIKUFUGO"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO2,	TYPE_COMBO, 		0,	0,	_T("AREACLASS_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK2,	TYPE_CHECKBOX,		0,	0,	_T("MANAGE_F"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK3,	TYPE_CHECKBOX,		0,	0,	_T("DETACH_F"),		NULL, CITY_ADMIN_TABLE_NAME );		
		m_cControlRel.SetControl( IDC_COMBOBOXEX_CITY_ADMIN, TYPE_OTHER,		0,	0);
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO3,	TYPE_COMBO,			0,	0,	_T("CONVTYPE_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE2,		TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE2"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),		NULL, CITY_ADMIN_TABLE_NAME );
	}
}

/**
 * @fn void CAttrCityAdminDlg::ChangeToSeaValue()
 * @brief �G���A���ʎ�ʂ��C�ɂȂ����Ƃ��ɁA�֘A��ʂ������I�ɕύX����
 */
void CAttrCityAdminDlg::ChangeToSeaValue()
{
	// �G���A���ʎ�ʃR���{�{�b�N�X�̎擾���l�擾
	CComboBox	cComboAreaClass( GetDlgItem( IDC_CITY_ADMIN_COMBO2 ) );
	int iAreaClass = cComboAreaClass.GetCurSel();

	// �G���A���ʎ�ʂ̒l���C�i�Q�j�̎�
	if( iAreaClass == 2 )
	{
		CString strCityCode = _T("00000");
		CString strAddrCode = _T("000000");
		CString strGaikuFugou = _T("");
		CString strExtGaikuFugou = _T("");

		// �s�撬���R�[�h�̒l��00000�ɃZ�b�g
		SetDlgItemText( IDC_CITY_ADMIN_EDIT2, strCityCode );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT2 );

		// �Z���R�[�h�̒l��000000�ɃZ�b�g
		SetDlgItemText( IDC_CITY_ADMIN_EDIT3, strAddrCode );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT3 );

		// �X�敄���̒l��NULL�ɃZ�b�g
		SetDlgItemText( IDC_CITY_ADMIN_EDIT4, strGaikuFugou );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT4 );

		// �X�敄���g���̒l��NULL�ɃZ�b�g
		SetDlgItemText( IDC_CITY_ADMIN_EDIT5, strExtGaikuFugou );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT5 );

		// �Z��������ʂ̒l�𒬒��ڊE�������ɃZ�b�g
		CComboBox	cComboAddrClass( GetDlgItem( IDC_CITY_ADMIN_COMBO1 ) );
		cComboAddrClass.SetCurSel( 0 );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_COMBO1 );
	}

}

/**
 * @fn BOOL CAttrCityAdminDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɃG���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrCityAdminDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CITY_ADMIN_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			LONG lCityCode, lAddrCode, lAddrClass_c, lAreaClass_c, lGaikuFugo, lOID;
			CComBSTR	bstrCityCode, bstrAddrCode, bstrGaikuFugo, bstrExtGaikuFugo;
			CString		strDummy;
			BOOL		bIsGaikuNull = FALSE;
			BOOL		bIsExtGaikuNull = FALSE;

			// �s�撬���R�[�h�擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("CITYCODE") );
			bstrCityCode = pFieldDef->m_vaValue.bstrVal;
			strDummy = bstrCityCode.m_str;
			lCityCode = _ttol( strDummy );

			// �Z���R�[�h�擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE") );
			bstrAddrCode = pFieldDef->m_vaValue.bstrVal;
			strDummy = bstrAddrCode.m_str;
			lAddrCode = _tcstol(strDummy, NULL, 16);

			// �Z��������ʎ擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCLASS_C") );
			lAddrClass_c = pFieldDef->m_vaValue.lVal;

			// �X�敄���擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("GAIKUFUGO") );
			if( pFieldDef->m_vaValue.vt == VT_NULL )
				bIsGaikuNull = TRUE;
			else
				lGaikuFugo = pFieldDef->m_vaValue.lVal;

			// �g���X�敄���擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("EXTGAIKUFUGO") );
			if( pFieldDef->m_vaValue.vt == VT_NULL )
				bIsExtGaikuNull = TRUE;
			else
			{
				bstrExtGaikuFugo = pFieldDef->m_vaValue.bstrVal;

				CString strExGaikuFugo;
				strExGaikuFugo.Format( _T("%s"), CString(bstrExtGaikuFugo) );

				if( strExGaikuFugo.Find( _T(" ") ) != -1 )
				{
					AttrMessageBox( AheLoadString( IDS_UNNECCESARYSPACE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}

				USES_CONVERSION;
				LONG lStrNum = strExGaikuFugo.GetLength();
				BOOL bIsExistWord = FALSE;
				BOOL bIsExistNum = FALSE;
				for(INT i = 0; i < lStrNum; i++ )
				{
					CString str = CString_GetAt2(strExGaikuFugo, i);
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );
					if ( isasciinumber( lpstr[0] ) )
						bIsExistNum = TRUE;
					else
						bIsExistWord = TRUE;
				}
				if( bIsExistNum && !bIsExistWord )
				{
					AttrMessageBox( AheLoadString( IDS_ONLYHALFWIDTHNUMBER ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}
			}

			// �G���A���ʎ�ʎ擾
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("AREACLASS_C") );
			lAreaClass_c = pFieldDef->m_vaValue.lVal;

			it->GetFeature()->get_OID(&lOID);


			// �s�撬���R�[�h�̌����`�F�b�N
			if( bstrCityCode.Length() != 5 )
			{
				AttrMessageBox( AheLoadString( IDS_CITYCODE5FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			// �Z���R�[�h�̌����`�F�b�N
			if( bstrAddrCode.Length() != 6 )
			{
				AttrMessageBox( AheLoadString( IDS_ADDRCODE6FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			// [bug 8042] ADDRCODE2�`�F�b�N
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE2") );
			if( pFieldDef )
			{
				CComVariant vaAddrCode2 = pFieldDef->m_vaValue;
				// [bug 8189] VT_NULL�͏��O
				if( VT_BSTR == vaAddrCode2.vt )
				{
					CString strAddrCode2(vaAddrCode2);
					if( strAddrCode2.GetLength() != 6 )
					{
						AttrMessageBox( AheLoadString( IDS_ADDRCODE26FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
						return FALSE;
					}
				}
			}

			// �G���A���ʎ�ʂ��C�̎�
			if( lAreaClass_c == 2 )
			{
				// �s�撬���R�[�h�̃`�F�b�N
				if( lCityCode != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CITYCODE00000 ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

					return FALSE;
				}

				// �Z���R�[�h�̃`�F�b�N
				if( lAddrCode != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_ADDRCODE000000 ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �G���A���ʎ�ʂ��C�̎��ɂ́A�K���Z��������ʂ͒����ڊE������
				if( lAddrClass_c != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_ADDRCLASSUNSPECIFY ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �X�敄���̃`�F�b�N
				if( !bIsGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_TOWNBLOCKNULL ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �g���X�敄���̃`�F�b�N
				if( !bIsExtGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_EXTOWNBLOCKNULL ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
			// �G���A���ʎ�ʂ��C�ȊO�̎�
			else
			{
				// �s�撬���R�[�h�̃`�F�b�N
				if( lCityCode == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTCITYCODE00000 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �s�撬���R�[�h�̃`�F�b�N
				if( lCityCode > 47999 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CITYCODERANGE ),lOID );
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �Z���R�[�h�̃`�F�b�N
				if( lAddrCode == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTADDRCODE000000 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// �Z��������ʂ̃`�F�b�N
				if( lAddrClass_c == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTADDRCLASSUNSPECIFY ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}

			
			// �X�敄���̃`�F�b�N	
			if( lAddrClass_c == 0 || lAddrClass_c == 3 )
			{
				// �X�敄����NULL�łȂ��Ƃ����Ȃ�
				if( !bIsGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_TOWNBLOCKNULL2 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
			else
			{
				// �X�敄����NULL�ł͂����Ȃ�
				if( ( bIsGaikuNull && bIsExtGaikuNull ) || ( !bIsGaikuNull && !bIsExtGaikuNull ) )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_INPUTTOWNBLOCKOREXTOWNBLOCK ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}
