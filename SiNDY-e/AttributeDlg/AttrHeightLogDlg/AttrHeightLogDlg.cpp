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

// AttrHeightLogDlg.cpp : DLL �G�N�X�|�[�g�̎����ł��B


#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "resource.h"
#include "AttrHeightLogDlg.h"
#include "winlib/winutil.h"

BEGIN_WINDOW_MAP(HEIGHT_LOG_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_Z10)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_Z10)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ZORIG)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_ZORIG)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_RUNWAY)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_RUNWAY)
				ENDGROUP()
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(-1 )
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_UPDOWN)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_UPDOWN)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ONOFF)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_ONOFF)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ROADNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_ROADNAME)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_SPOTNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_SPOTNAME)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_MASTERNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_MASTERNAME)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_FILENAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_FILENAME)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_CONTAIN)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_CONTAIN)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_MEASURE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_MEASURE)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_DATE)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_DATE)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_REST, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_REMARKS)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_REMARKS)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrHeightLogDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHeightLogDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightLogDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHeightLogDlg::CAttrHeightLogDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrHeightLogDlg::CAttrHeightLogDlg()
	: ATTR_BASE_CLASS(HEIGHT_LOG_MAP, IDD_ATTRHEIGHTLOGDLG)
{
}

/** 
 * @fn HWND CAttrHeightLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrHeightLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHeightLogDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrHeightLogDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrHeightLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrHeightLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	SetDlgStatus( IDC_COMBO_RUNWAY, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_ROADNAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_SPOTNAME, FALSE, TRUE );
	SetDlgStatus( IDC_COMBO_UPDOWN, FALSE, FALSE );
	SetDlgStatus( IDC_COMBO_ONOFF, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_MASTERNAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_FILENAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_CONTAIN, FALSE, TRUE );
	SetDlgStatus( IDC_COMBO_MEASURE, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_DATE, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_REMARKS, FALSE, TRUE );
	SetMasterValue();
	return bRet;
}

/**
 * @fn void CAttrHeightLogDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrHeightLogDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_EDIT_Z10,		TYPE_EDITSTRING, 	0,	0,	_T("Z_10"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_EDIT_ZORIG,	TYPE_EDITSTRING, 	0,	0,	_T("Z_ORIG"),	NULL, m_strFeatureClassName );
	}
}

void CAttrHeightLogDlg::SetMasterValue()
{
	_IRowPtr ipRow = GetCurrentFeatureDef()->GetFeature();
	// �Ȃ��ł��郏�[�N�X�y�[�X�̃e�[�u�����擾����
	if( ipRow )
	{
		IWorkspacePtr ipWorkspace = AheGetWorkspace( ipRow );
		ITablePtr ipTable = AheOpenTableByTableName( ipWorkspace, _T("HEIGHT_LOG_MASTER") );
		if( ipTable )
		{
			_IRowPtr ipMasterRow;
			ipTable->GetRow( GetCurrentFeatureDef()->GetFieldDef( m_strFeatureClassName, _T("MASTER_ID") )->m_vaValue.lVal, &ipMasterRow );
			if( ipMasterRow )
			{
				SetValueToCombo( IDC_COMBO_RUNWAY, GetDisplayValue( ipMasterRow, _T("RUNWAY_C") ) );
				SetValueToEdit( IDC_EDIT_ROADNAME, GetDisplayValue( ipMasterRow, _T("ROAD_NAME") ) );
				SetValueToEdit( IDC_EDIT_SPOTNAME, GetDisplayValue( ipMasterRow, _T("SPOT_NAME") ) );
				SetValueToCombo( IDC_COMBO_UPDOWN, GetDisplayValue( ipMasterRow, _T("UP_DOWN_C") ) );
				SetValueToCombo( IDC_COMBO_ONOFF, GetDisplayValue( ipMasterRow, _T("ON_OFF_C") ) );
				SetValueToEdit( IDC_EDIT_MASTERNAME, GetDisplayValue( ipMasterRow, _T("MASTER_NAME") ) );
				SetValueToEdit( IDC_EDIT_FILENAME, GetDisplayValue( ipMasterRow, _T("FILENAME") ) );
				SetValueToEdit( IDC_EDIT_CONTAIN, GetDisplayValue( ipMasterRow, _T("CONTAIN") ) );
				SetValueToCombo( IDC_COMBO_MEASURE, GetDisplayValue( ipMasterRow, _T("MEASURE_C") ) );
				SetValueToEdit( IDC_EDIT_DATE, GetDisplayValue( ipMasterRow, _T("DELIVERY_DATE") ) );
				SetValueToEdit( IDC_EDIT_REMARKS, GetDisplayValue( ipMasterRow, _T("REMARKS") ) );
			}
		}
	}
}

void CAttrHeightLogDlg::SetValueToCombo( int nID, CString strValue )
{
	CComboBox cCombo = GetDlgItem( nID );
	cCombo.ResetContent();
	cCombo.SetCurSel( cCombo.AddString( strValue ) );
}

void CAttrHeightLogDlg::SetValueToEdit( int nID, CString strValue )
{
	CEdit cEdit = GetDlgItem( nID );
	cEdit.SetWindowText( strValue );
}

CString CAttrHeightLogDlg::GetDisplayValue( _IRowPtr ipRow, CString strFieldName )
{
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	long lIndex = 0;
	ipFields->FindField( _bstr_t( strFieldName ), &lIndex );
	CComVariant vaValue;
	ipRow->get_Value( lIndex, &vaValue );

	IFieldPtr ipField;
	ipFields->get_Field( lIndex, &ipField );
	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	CString strValue;
	if( ipDomain )
	{
		ICodedValueDomainPtr ipCodedValueDomain = ipDomain;
		long lCount = 0;
		ipCodedValueDomain->get_CodeCount( &lCount );
		for( int i = 0; i < lCount; ++i )
		{
			CComVariant vaCodeValue;
			ipCodedValueDomain->get_Value( i, &vaCodeValue );
			if( vaCodeValue.lVal == vaValue.lVal )
			{
				CComBSTR bstrValue;
				ipCodedValueDomain->get_Name( i, &bstrValue );
				strValue = bstrValue;
				break;
			}
		}
	}
	else
	{
		if( vaValue.vt == VT_BSTR )
			strValue = vaValue.bstrVal;
		else if( vaValue.vt == VT_DATE )
		{
			vaValue.ChangeType( VT_BSTR );
			strValue = vaValue.bstrVal;
		}
	}
	return strValue;
}
