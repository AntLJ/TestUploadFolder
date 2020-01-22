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

// AttrGouPointDlg.cpp: CAttrGouPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrGouPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���E�|�C���g�p�j
BEGIN_WINDOW_MAP(GOU_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_STATIC1)
				RCTOFIT(-1)
				RCREST(IDC_GOU_POINT_EDIT1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_GOU_POINT_EDIT6)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_GOU_POINT_EDIT2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_CHECK1)
				RCSPACE(50)
				RCTOFIT(IDC_CHECK_HIMAWARI)
				RCSPACE(50)
				RCTOFIT(IDC_GOU_POINT_CHECK2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_GOU_POINT_EDIT3)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_GOU_POINT_STATIC7)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_GOUPOINT)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))
				RCTOFIT(IDC_STATIC_S)
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					RCTOFIT(IDC_STATIC_EXIST)
					RCREST(IDC_EDIT_EXIST)
				ENDGROUP()
				RCSPACE(30)
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					RCTOFIT(IDC_STATIC_HSAG)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HS1)
						RCTOFIT(IDC_EDIT_HS2)
					ENDGROUP()
				ENDGROUP()
				RCSPACE(30)
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					RCTOFIT(IDC_STATIC_LMAG)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_LM1)
						RCTOFIT(IDC_EDIT_LM2)	
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))
				RCTOFIT(IDC_BUTTON_UNCHECK)
				RCSPACE(2)
				RCTOFIT(IDC_BUTTON_VIEWNG)
				RCSPACE(2)
				RCTOFIT(IDC_BUTTON_VIEWOK)
				RCSPACE(2)
				RCTOFIT(IDC_BUTTON_INQDOC)
				RCSPACE(2)
				RCTOFIT(IDC_BUTTON_PUBDOC)
				RCSPACE(2)
				RCTOFIT(IDC_BUTTON_NEWBUILDDOC)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,5))
				RCTOFIT(IDC_STATIC_CONFIRM)
				RCREST(IDC_COMBO_CONFIRM)
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
 * @retval CAttrGouPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrGouPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrGouPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrGouPointDlg::CAttrGouPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrGouPointDlg::CAttrGouPointDlg()
	: ATTR_BASE_CLASS(GOU_POINT_MAP, IDD_ATTRGOUPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_GOU_POINT_EDIT1 ), 
		m_wnd1( _T("EDIT"), this, IDC_GOU_POINT_EDIT2 ), 
		m_wnd2( _T("CHECKBOX"), this, IDC_GOU_POINT_CHECK1 ), 
		m_wnd3( _T("EDIT"), this, IDC_GOU_POINT_EDIT3 ),
		m_wnd4( _T("CHECKBOX"), this, IDC_GOU_POINT_CHECK2 ), 
		m_wnd5( _T("EDIT"), this, IDC_GOU_POINT_EDIT6 ),
		m_wnd6( _T("COMBO"), this, IDC_COMBO_GOUPOINT ),
		m_wnd7( _T("CHECKBOX"), this, IDC_CHECK_HIMAWARI ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd9( _T("BUTTON"), this, IDC_BUTTON_VIEWOK ),
		m_wnd10( _T("BUTTON"), this, IDC_BUTTON_VIEWNG ),
		m_wnd12( _T("BUTTON"), this, IDC_BUTTON_INQDOC ),
		m_wnd13( _T("BUTTON"), this, IDC_BUTTON_PUBDOC ),
		m_wnd14( _T("COMBO"), this, IDC_COMBO_CONFIRM ),
		m_wnd15( _T("EDIT"), this, IDC_EDIT_EXIST ),
		m_wnd16( _T("EDIT"), this, IDC_EDIT_HS1 ),
		m_wnd17( _T("EDIT"), this, IDC_EDIT_HS2 ),
		m_wnd18( _T("EDIT"), this, IDC_EDIT_LM1 ),
		m_wnd19( _T("EDIT"), this, IDC_EDIT_LM2 ),
		m_wnd20( _T("BUTTON"), this, IDC_BUTTON_UNCHECK ),
		m_wnd21( _T("BUTTON"), this, IDC_BUTTON_NEWBUILDDOC )
{
}

/** 
 * @fn HWND CAttrGouPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrGouPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrGouPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrGouPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrGouPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrGouPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( m_cArcHelper.IsStartEditing() && IsEditableFeature() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_GOU_POINT_EDIT1) );

		// PGDB���[�h�ł����DISABLE��
		if( AheIsPGDBEditMode() )
		{
			SetDlgStatus( IDC_GOU_POINT_EDIT3, FALSE );
			SetDlgStatus( IDC_GOU_POINT_CHECK2, FALSE );
		}
	}

	// �ҏW�s�ɂ��Ă���
	SetDlgStatus(IDC_GOU_POINT_EDIT6, FALSE, TRUE );
	SetDlgStatus(IDC_COMBO_GOUPOINT, FALSE );
	SetDlgStatus(IDC_GOU_POINT_EDIT2, FALSE, TRUE );
	// S�Њm�F�t�B�[���h���Ȃ��Ȃ�Disable
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( pFeatureDef )
	{
		CFieldDef* pFieldDef = pFeatureDef->GetFieldDef( GOU_POINT_TABLE_NAME, _T("PRECISION_CODE") ); // �Z����v����
		if( pFieldDef )
		{
			// �Z����v�������Z�b�g
			if( VT_BSTR == pFieldDef->m_vaValue.vt )
			{
				CString strVal( pFieldDef->m_vaValue.bstrVal );
				strVal.Trim();
				if( 8 == strVal.GetLength() )
				{
					// ���ݔ���
					CString strExist; // IDC_EDIT_EXIST�ɓ��镶����
					switch( strVal.GetAt(0) )
					{
					case _T('0'): strExist = AheLoadString( IDS_NOTHING ); break;
					case _T('1'):
						switch( strVal.GetAt(1) )
						{
						case _T('1'): strExist = AheLoadString( IDS_HAL ); break;
						case _T('0'): strExist = AheLoadString( IDS_MAD ); break;
						default: _ASSERTE(false); break;
						}
						break;
					default: _ASSERTE(false); break;
					}
					SetDlgItemText(IDC_EDIT_EXIST, strExist);
					// �ƌ`����v
					SetDlgItemText(IDC_EDIT_HS1, CString(strVal.GetAt(2)));
					SetDlgItemText(IDC_EDIT_HS2, CString(strVal.GetAt(3)) + strVal.GetAt(4));
					// ���e�͈͓���v
					SetDlgItemText(IDC_EDIT_LM1, CString(strVal.GetAt(5)));
					SetDlgItemText(IDC_EDIT_LM2, CString(strVal.GetAt(6)) + strVal.GetAt(7));
				}
			}
		}
	}

    return bRet;
}

/**
 * @fn void CAttrGouPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrGouPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���E�|�C���g�p

		// ���|�C���g
		m_cControlRel.SetControl( IDC_GOU_POINT_EDIT1,	TYPE_EDITSTRING,0,	0,	_T("GOU_NO"),		NULL, GOU_POINT_TABLE_NAME );
		// 20���Z���R�[�h
		m_cControlRel.SetControl( IDC_GOU_POINT_EDIT6,	TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),		NULL, GOU_POINT_TABLE_NAME );
		// �Ή�����ID
		m_cControlRel.SetControl( IDC_GOU_POINT_EDIT2,	TYPE_EDIT,		0,	0,	_T("BUILD_ID"),		NULL, GOU_POINT_TABLE_NAME );
		// �g�����ԍ��t���O
		m_cControlRel.SetControl( IDC_GOU_POINT_CHECK1,	TYPE_CHECKBOX,	0,	0,	_T("EXPGOUNO_F"),	NULL, GOU_POINT_TABLE_NAME );
		// ���^�C�v
		m_cControlRel.SetControl( IDC_GOU_POINT_EDIT3,	TYPE_EDIT,		0,	0,	_T("GOUTYPE"),		NULL, GOU_POINT_TABLE_NAME );
		// �D��t���O
		m_cControlRel.SetControl( IDC_GOU_POINT_CHECK2,	TYPE_CHECKBOX,	0,	0,	_T("PRIORITY_F"),	NULL, GOU_POINT_TABLE_NAME );
		// �G���A���ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_GOUPOINT,	TYPE_COMBO,		0,	0,	_T("AREACLASS_C"),	NULL, GOU_POINT_TABLE_NAME );
		// �Ђ܂��t���O
		m_cControlRel.SetControl( IDC_CHECK_HIMAWARI,	TYPE_CHECKBOX,	0,	0,	_T("HIMAWARI_F"),	NULL, GOU_POINT_TABLE_NAME, 0, 0, FOLLOW_READONLY ); // bug 6606
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, GOU_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_COMBO_CONFIRM,	TYPE_COMBO,		0,	0,	_T("CONFIRM_C"),	NULL, GOU_POINT_TABLE_NAME, 0, 0, FOLLOW_READONLY ); // �m�F�X�e�[�^�X�ibug 6606�j
		m_cControlRel.SetControl( IDC_BUTTON_VIEWOK,	TYPE_LIST );
		m_cControlRel.SetControl( IDC_BUTTON_VIEWNG,	TYPE_LIST );
		m_cControlRel.SetControl( IDC_BUTTON_INQDOC,	TYPE_LIST );
		m_cControlRel.SetControl( IDC_BUTTON_PUBDOC,	TYPE_LIST );
		m_cControlRel.SetControl( IDC_BUTTON_UNCHECK,	TYPE_LIST );
		m_cControlRel.SetControl( IDC_BUTTON_NEWBUILDDOC,TYPE_LIST );
	}
}

LRESULT CAttrGouPointDlg::OnClickRadio(WORD/* wNotifyCode*/, WORD wID, HWND/* hWndCtl*/, BOOL& bHandled)
{
	bHandled = FALSE; // OnCommand�����s�����邽��
	long lValue = -1;
	switch( wID )
	{
	case IDC_BUTTON_VIEWOK:     lValue = 7; break; // �ڎ��m�FOK
	case IDC_BUTTON_VIEWNG:     lValue = 3; break; // �ڎ��m�FNG
	case IDC_BUTTON_INQDOC:     lValue = 8; break; // ��������OK
	case IDC_BUTTON_PUBDOC:     lValue = 9; break; // ���}����OK
	case IDC_BUTTON_UNCHECK:	lValue = 0; break; // ���m�F
	case IDC_BUTTON_NEWBUILDDOC: lValue = 10; break; // �V�z����OK
	default: break;
	}
	CControlDef* pDef = m_cControlRel.GetControlDef( GetDlgItem(IDC_COMBO_CONFIRM) );
	if( pDef != NULL )
	{
		pDef->SetValue( lValue );
		pDef->SetComboBox(-1);
	}

	return 0;
}

/**
 * @fn BOOL CAttrGouPointDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrGouPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == GOU_POINT_TABLE_NAME )
		{

			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaGouNO;
			pFieldDef = it->GetFieldDef( GOU_POINT_TABLE_NAME, _T("GOU_NO") );
			vaGouNO = pFieldDef->m_vaValue;

			if( vaGouNO.vt == VT_BSTR )
			{
				USES_CONVERSION;

				CString strMessage;
				LONG lOID;
				it->GetFeature()->get_OID( &lOID );

				CString strGouNO;
				strGouNO.Format( _T("%s"), OLE2T(vaGouNO.bstrVal) );
				int GouNoByte = ::WideCharToMultiByte(CP_ACP, 0, T2CW((LPCTSTR)(strGouNO)), -1, NULL, 0, NULL, NULL);
				GouNoByte = GouNoByte -1;
				if( GouNoByte > 32  )
				{
					strMessage.Format(AheLoadString( IDS_32BYTE ), lOID );
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}
				if( strGouNO.Find( _T("*") ) >= 0 && GouNoByte != 1)
				{
					strMessage.Format( AheLoadString( IDS_MARKONLY ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}

				if( strGouNO.ReverseFind( '#' ) > 0 )
				{
					strMessage.Format( AheLoadString( IDS_AHEADSHARP ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}
			}
			else if( vaGouNO.vt == VT_NULL || vaGouNO.vt == VT_EMPTY )
			{
				// [bug7748]NULL�ł͕ۑ��ł��Ȃ��悤�ɂ���
				AttrMessageBox( AheLoadString( IDS_NULL ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}
