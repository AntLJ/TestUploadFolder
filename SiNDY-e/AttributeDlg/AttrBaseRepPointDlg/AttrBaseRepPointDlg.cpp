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

// AttrBaseRepPointDlg.cpp: CAttrBaseRepPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseRepPointDlg.h"
//#include "icrsint.h"
//#import "C:\Program files\Common Files\System\Ado\msado15.dll" rename_namespace("MSXML")  rename("EOF", "ADOEOF")

//#import "c:\winnt\system32\msxml.dll" 
//using namespace MSXML;

#include "stdio.h"
#include "io.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���k��\�_�p�j
BEGIN_WINDOW_MAP(BASE_REP_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADMINNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADMINNAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE)
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
 * @retval CAttrBaseRepPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseRepPointDlg::CAttrBaseRepPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrBaseRepPointDlg::CAttrBaseRepPointDlg()
	: ATTR_BASE_CLASS(BASE_REP_POINT_MAP, IDD_ATTRBASEREPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADMINNAME ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_ZIPCODE ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
	m_bDoAutoInput = FALSE;
}

/** 
 * @fn HWND CAttrBaseRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBaseRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseRepPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBaseRepPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBaseRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_EDIT_ADDRCODE) );
	}

	// [bug 8045] �t�B�[���h������΃R���g���[����\��
	bool bHasZipCode = GetCurrentFeatureDef()->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ZIPCODE") );
	::EnableWindow( GetDlgItem( IDC_EDIT_ZIPCODE ), bHasZipCode ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_STATIC_ZIPCODE ), bHasZipCode ? TRUE : FALSE );


    return bRet;
}

/**
 * @fn void CAttrBaseRepPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBaseRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���k��\�_�p
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),	NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ADMINNAME,	TYPE_EDITSTRING,0,	0,	_T("ADMINNAME"),NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ZIPCODE,		TYPE_EDITSTRING,0,	0,	_T("ZIPCODE"),NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),	NULL, BASE_REP_POINT_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrBaseRepPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == BASE_REP_POINT_TABLE_NAME )
		{

			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode, vaAdminName;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADDRCODE") );
			vaAddrCode = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADMINNAME") );
			vaAdminName = pFieldDef->m_vaValue;

			USES_CONVERSION;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode, strAddminName;
			strAddrCode.Format( _T("%s"), OLE2T(vaAddrCode.bstrVal) );
			strAddminName.Format( _T("%s"), OLE2T(vaAdminName.bstrVal) );


			if( strAddrCode.GetLength() != 11 )
			{
				strMessage.Format( AheLoadString( IDS_NOT11FIGURES ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddrCode.Find( _T(" ") ) != -1 )
			{
				strMessage.Format(AheLoadString( IDS_NONEEDSPACE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddminName.Find( _T(" ") ) != -1 )
			{
				strMessage.Format( AheLoadString( IDS_NOPOINTNAME ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			// [bug 8045] ZIPCODE�`�F�b�N
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ZIPCODE") );
			if( pFieldDef )
			{
				CComVariant vaZipCode = pFieldDef->m_vaValue;
				CString strZipCode;
				// [bug 8188] VT_NULL�͏��O
				if( VT_BSTR == vaZipCode.vt )
				{
					strZipCode = vaZipCode;
					CString strZipErr;
					if( strZipCode.GetLength() != 7 )
					{
						strZipErr = AheLoadString( IDS_NOT7FIGURES );
					}
					if( strZipCode.SpanIncluding( _T("1234567890")) != strZipCode )
					{
						strZipErr += AheLoadString( IDS_EXCEPTNUMBER );
					}
					if( !strZipErr.IsEmpty() )
					{
						strMessage.Format(AheLoadString( IDS_ERRPOSTCODE ),lOID);
						AttrMessageBox( strMessage + strZipErr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::ReadDBPathFromReg()
 * @brief ���W�X�g���L�["ChukiMaster"�̒l�iDB�t�@�C���̃t���p�X�j��ǂݍ���
 *
 * @retval �L�[�������TURUE�A�������FALSE
 */
BOOL CAttrBaseRepPointDlg::ReadDBPathFromReg()
{
	// ���W�X�g������t�@�C���p�X�ǂݍ���
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if( hOpenKey )
	{
		// 
		TCHAR szFileName[1024];
		if( EmxGetRegString( _T("ChukiMaster"), (LPTSTR)szFileName, hOpenKey ) == TRUE )
		{
			m_strTableFileName = (CString)szFileName;
			RegCloseKey( hOpenKey );
			return TRUE;
		}
		// �L�[��������Ȃ������ꍇ�AClose���Ă��甲����
		RegCloseKey( hOpenKey );
	}
	return FALSE;
}


/**
 * @fn BOOL CAttrBaseRepPointDlg::SearchNameFromFile( const CString strAddCode, CString &strAddName )
 * @brief �A�N�Z�X��DB�t�@�C��(mdb)�ɐڑ�����11���R�[�h�ɑΉ�����Z���̖��̂��擾����
 *
 * @param const CString strAddCode	[in]	11���R�[�h
 * @param CString &strAddName	    [out]	�Ή��Z��
 *
 * @retval �Ή��f�[�^�����������ꍇ��TURE�A�������FALSE
 */
BOOL CAttrBaseRepPointDlg::SearchNameFromFile( const CString strAddCode, CString &strAddName )
{

	_ConnectionPtr pConnect;   // Connection�I�u�W�F�N�g
	_CommandPtr pCommand;      // Command�I�u�W�F�N�g
	_RecordsetPtr pRecoedset;  // Recordset�I�u�W�F�N�g

	try {
		// COM�̏���������
		::CoInitialize(NULL);

		// ADO�I�u�W�F�N�g�̐���
		pConnect.CreateInstance(__uuidof(Connection));
		pCommand.CreateInstance(__uuidof(Command));
		pRecoedset.CreateInstance(__uuidof(Recordset));

		// �f�[�^�x�[�X�ւ̐ڑ�
		CString strConnect;
		strConnect.Format( _T( "Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s"), m_strTableFileName );
		pConnect->Open( _bstr_t(strConnect), L"", L"", adConnectUnspecified );

		// SQL���̎��s
		CString strSqlQuery;
		strSqlQuery.Format( _T("SELECT * FROM Chuki_master_all where �Z���R�[�h = \"%s\" "), strAddCode ); 
		pRecoedset = pConnect->Execute( _bstr_t(strSqlQuery), NULL, NULL );

		// EOF�܂���BOF��������s��v
		if( pRecoedset->GetADOEOF() || pRecoedset->GetBOF() )
			return FALSE;
		// �l��HIT�����Ƃ��͕Ԃ�
		else{
			// ������"���L"��DB�����p�ꉻ���ꂽ��ł������낤
			_variant_t vaValue = pRecoedset->GetCollect( "���L" );
			strAddName = vaValue.bstrVal;
		}

	}
	catch(_com_error &e)
	{
		MessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}
