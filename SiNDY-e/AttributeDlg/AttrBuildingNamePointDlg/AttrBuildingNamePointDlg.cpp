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

// AttrBuildingNamePointDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include <WinLib/libwinlib.h>
#include "libschema.h"
#include <TDC/common_check/NameYomiChk.h>
#include <TDC/common_check/libcommoncheck.h>
#include "AttrBuildingNamePointDlg.h"
#include "Map.h"
#include <WinLib/IMEStatus.h>


using namespace sindy::schema;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
		BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
			RCSPACE(20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BUILDINGOID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_BUILDINGOID)
				RCTOFIT(IDC_BUTTON_SEARCHOID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAMESEPARATED)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_NAMESEPARATED)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_YOMISEPARATED)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_YOMISEPARATED)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS1C)
				RCTOFIT(-1)
				RCSPACE(2)
				RCREST(IDC_COMBO_BLDGCLASS1C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS2C)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_COMBO_BLDGCLASS2C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS3C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_BLDGCLASS3C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FLOORS)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FLOORS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCETYPEC)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SOURCETYPEC)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SRCOID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_SRCOID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYDATE)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_SURVEYDATE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ANYPLACEID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_ANYPLACEID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRIORITYC)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_COMBO_PRIORITYC)
				RCTOFIT(IDC_BUTTON_PRIORITYC)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MATCHPER)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_MATCHPER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SRCPOINTLIST)
				ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_LIST_SRCPOINT)
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
				RCSPACE(3)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

	// �f�ރ|�C���g�֘A���̃��X�g��`
	LVCOLUMN CAttrBuildingNamePointDlg::m_ColumnInfo[] = {
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN0_SZ, BNP_SRC_COLUMN0, 0, 0, 0, 0 },
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN1_SZ, BNP_SRC_COLUMN1, 0, 0, 0, 0 },
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN2_SZ, BNP_SRC_COLUMN2, 0, 0, 0, 0 }
	};

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrBuildingNamePointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrBuildingNamePointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////


// �R���X�g���N�^
CAttrBuildingNamePointDlg::CAttrBuildingNamePointDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_ATTRBUILDINGNAMEPOINTDLG), m_bShowSrcInfo(false)
{

	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_BUILDINGOID,  CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_BUILDINGOID ) )
		( IDC_BUTTON_SEARCHOID,  CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHOID ) )
		( IDC_EDIT_NAME,         CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_NAME ) )
		( IDC_EDIT_NAMESEPARATED,CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_NAMESEPARATED ) )
		( IDC_EDIT_YOMISEPARATED,CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_YOMISEPARATED ) )
		( IDC_COMBO_BLDGCLASS1C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS1C ) )
		( IDC_COMBO_BLDGCLASS2C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS2C ) )
		( IDC_COMBO_BLDGCLASS3C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS3C ) )
		( IDC_EDIT_FLOORS,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_FLOORS ) )
		( IDC_COMBO_SOURCETYPEC, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_SOURCETYPEC ) )
		( IDC_EDIT_SRCOID,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SRCOID ) )
		( IDC_EDIT_SURVEYDATE,   CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SURVEYDATE ) )
		( IDC_EDIT_ANYPLACEID,   CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_ANYPLACEID ) )
		( IDC_COMBO_PRIORITYC,   CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_PRIORITYC ) )
		( IDC_BUTTON_PRIORITYC,  CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_PRIORITYC ) )
		( IDC_EDIT_MATCHPER,     CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_MATCHPER ) )
		( IDC_LIST_SRCPOINT,     CContainedWindow(  _T("EDIT"),   this, IDC_LIST_SRCPOINT ) )
		( IDC_EDIT_SOURCE,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SOURCE ) );

}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

// �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
LRESULT CAttrBuildingNamePointDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R���g���[��
	CreateControlRelation();
	// �T�u�N���X��
	for( auto& it : m_mapSubClass)
		it.second.SubclassWindow( GetDlgItem( it.first ) );

	return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
}


// WM_COMMAND �p�C�x���g�n���h��
LRESULT CAttrBuildingNamePointDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

	if( msg == EN_SETFOCUS ){
		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		SelectDlgItem( nTargetControl );

		CONVERSIONMODE selectedMode = NOCHANGE;
		switch( nTargetControl ){
			// ���̂܂��͖��́i�����j�̎��͑S�p�Ђ炪�ȓ��͂ɂ���
			case IDC_EDIT_NAME :
			case IDC_EDIT_NAMESEPARATED :
				selectedMode = HIRAGANA;
				break;
			// ���~�i�����j�̏ꍇ�͑S�p�J�^�J�i���͂ɂ���
			case IDC_EDIT_YOMISEPARATED:
				selectedMode = KATAKANA_FULL;
				break;
			// �K���̎��͔��p�p�������͂ɂ���
			case IDC_EDIT_FLOORS :
				selectedMode = ALPHA_HALF;
				break;
			default :
				break;
		}
		CIMEStatus imeStatus(m_hWnd);
		imeStatus.setIMEConversionStatus(selectedMode);
	}

	// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
		// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}

	return 0;
}


// string�^�p�C�x���g�n���h��
LRESULT CAttrBuildingNamePointDlg::OnEnUpdateEditString(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !::IsWindow( hWndCtl ) ) return 0;
	
	CEdit edit( hWndCtl );
	CString strText;
	edit.GetWindowText( strText );

	if( wID == IDC_EDIT_NAME && strText == _T(" ") )
	{
		// ���̂�NULL�s���̂��ߐV�K�쐬���ɃX�y�[�X�����͂���邪�A�s�v�Ȃ��߃X�y�[�X���폜����
		edit.SetWindowText( NULL_VALUE );
	}
	else if( strText != NULL_VALUE )
	{
		CString strChange;

		// wID���Ƃ̌ʏ���
		if( wID == IDC_EDIT_NAME )
		{
			int nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszChange;
			lpszChange = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) * 2 );	// 2�{�̗̈���m��
			LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, strText, nTextLen, lpszChange, nTextLen * 2 );
			strChange = lpszChange;
		}
		else if( wID == IDC_EDIT_SURVEYDATE )
		{
			strChange = AheConvertToDateStr( strText );
		}

		// �G�f�B�b�g�{�b�N�X�̕ύX
		if( strText != strChange )
		{
			int dwSelStart = 0, dwSelEnd = 0;
			edit.GetSel( dwSelStart, dwSelEnd );
			edit.SetWindowText( strChange );
			// ���͈ʒu�̏C��
			int nDiff = CString_GetLength2( strText ) - CString_GetLength2( strChange );
			edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
		}
	}

	m_cControlRel.ControlChanged( wID );
	SetButton( Changed() );
	InvalidateRect( NULL, FALSE );

	return 0;
}


// �q�_�C�A���O���쐬���܂��B
HWND CAttrBuildingNamePointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// �쐬�����q�_�C�A���O���폜���܂��B
void CAttrBuildingNamePointDlg::Delete()
{
	// �_�C�A���O����鎞�ɑS�p�Ђ炪�ȓ��͂ɂ���
	CIMEStatus imeStatus(m_hWnd);
	imeStatus.setIMEConversionStatus(HIRAGANA);

	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}


// ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
BOOL CAttrBuildingNamePointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_NAME) );
	}
	
	if( !m_bShowSrcInfo )
	{
		OutputBuildingSrcInfo(); // �����r�����̃|�C���g�ɕR�Â������r�����̑f�ރ|�C���g�̏������X�g�o�͂���
		m_bShowSrcInfo = true;
	}

	SetToolTip( IDC_EDIT_NAME );
	return bRet;
}


// �����r�����̃|�C���g�ɕR�Â������r�����̑f�ރ|�C���g�̏������X�g�o�͂���
void CAttrBuildingNamePointDlg::OutputBuildingSrcInfo()
{

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	long lBuildingNameOID = AheGetOID( ipFeature );
	IFeatureClassPtr ipBuildingName = AheGetFeatureClass( ipFeature );

	// �����r�����̕R�t���e�[�u���A�����r�����̑f�ރ|�C���g�擾
	ITablePtr ipRelBuildTable = AheOpenSameOwnerTable( (ITablePtr)ipBuildingName,  sj::rel_buildingname::kTableName );
	if( !ipRelBuildTable )   return;
	IFeatureClassPtr ipBuildSrcFC = AheOpenSameOwnerTable( (ITablePtr)ipBuildingName,  sj::buildingname_src_point::kTableName );
	if( !ipBuildSrcFC )   return;

	// �����r�����̃|�C���g::OID���猚���r�����̕R�t���e�[�u���̊Y���s���擾
	CString strSubField;
	strSubField.Format( _T("%s"), sj::rel_buildingname::kSrcOID );
	CString strWhereClause;
	strWhereClause.Format( _T("%s = %ld"), sj::rel_buildingname::kBuildingNameOID, lBuildingNameOID );
	_ICursorPtr ipRelBuildCursor;
	ipRelBuildTable->Search( AheInitQueryFilter(NULL, strSubField, strWhereClause ), VARIANT_TRUE, &ipRelBuildCursor );

	CListViewCtrl lvSrcPoint = GetDlgItem( IDC_LIST_SRCPOINT );
	_IRowPtr ipRelBuildRow;
	while( ipRelBuildCursor->NextRow( &ipRelBuildRow ) == S_OK && ipRelBuildRow )
	{
		// �����r�����̕R�t���e�[�u��::�Q�Ƒf��OID���猚���r�����̑f�ރ|�C���g�̏������X�g�֏o��
		long lSrcOID = AheGetAttribute( ipRelBuildRow, sj::rel_buildingname::kSrcOID ).lVal;

		IFeaturePtr ipBuildSrcFeature;
		ipBuildSrcFC->GetFeature( lSrcOID, &ipBuildSrcFeature );
		if( !ipBuildSrcFeature )
			continue;
		
		int nItemCount = lvSrcPoint.GetItemCount();
		CString strBuildSrcOID;
		strBuildSrcOID.Format( _T("%d"), AheGetOID( ipBuildSrcFeature ) );
		lvSrcPoint.InsertItem( nItemCount, strBuildSrcOID );
		lvSrcPoint.SetItemText( nItemCount, list_col::building_src::Name, CString( AheGetAttribute( ipBuildSrcFeature, sj::buildingname_src_point::kName ) ) );

		// �o�T�R�[�h�̓R�[�h�l�h���C���̕�����ŏo��
		CString strDomainName;
		CComVariant vaValue;
		AheGetDomainNameAndValue( ipBuildSrcFeature, sj::buildingname_src_point::kSourceType, strDomainName, vaValue );
		lvSrcPoint.SetItemText( nItemCount, list_col::building_src::SrcCode, strDomainName );
	}


}


// �q�_�C�A���O�����ۂɍ쐬���܂��B
void CAttrBuildingNamePointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �����|���S��OID
		m_cControlRel.SetControl( IDC_EDIT_BUILDINGOID, TYPE_EDIT, 0, 0, sj::buildingname_point::kBuildingOID, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCHOID, TYPE_BUTTON, 0, 0 );
		// ����
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kName, NULL, sj::buildingname_point::kTableName );
		// ���́i�����j
		m_cControlRel.SetControl( IDC_EDIT_NAMESEPARATED, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kNameSeparated, NULL, sj::buildingname_point::kTableName );
		// ���~�i�����j
		m_cControlRel.SetControl( IDC_EDIT_YOMISEPARATED, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kYomiSeparated, NULL, sj::buildingname_point::kTableName );
		// ������ʃR�[�h1
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS1C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass1, NULL, sj::buildingname_point::kTableName );
		// ������ʃR�[�h2
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS2C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass2, NULL, sj::buildingname_point::kTableName );
		// ������ʃR�[�h3
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS3C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass3, NULL, sj::buildingname_point::kTableName );
		// �K��
		m_cControlRel.SetControl( IDC_EDIT_FLOORS, TYPE_EDIT, 0, 0, sj::buildingname_point::kFloors, NULL, sj::buildingname_point::kTableName );
		// �o�T�R�[�h
		m_cControlRel.SetControl( IDC_COMBO_SOURCETYPEC, TYPE_COMBO, 0, 0, sj::buildingname_point::kSourceType, NULL, sj::buildingname_point::kTableName );
		// �Q�Ƒf��OID
		m_cControlRel.SetControl( IDC_EDIT_SRCOID, TYPE_EDIT, 0, 0, sj::buildingname_point::kSrcOID, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );
		// ������
		m_cControlRel.SetControl( IDC_EDIT_SURVEYDATE, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kSurveyDate, NULL, sj::buildingname_point::kTableName );
		// AnyPlace ID
		m_cControlRel.SetControl( IDC_EDIT_ANYPLACEID, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kAnyPlaceID, NULL, sj::buildingname_point::kTableName );
		// �M���x�R�[�h
		m_cControlRel.SetControl( IDC_COMBO_PRIORITYC, TYPE_COMBO, 0, 0, sj::buildingname_point::kPriority, NULL, sj::buildingname_point::kTableName );
		m_cControlRel.SetControl( IDC_BUTTON_PRIORITYC, TYPE_BUTTON, 0, 0 );
		// �}�b�`���O��
		m_cControlRel.SetControl( IDC_EDIT_MATCHPER, TYPE_EDIT, 0, 0, sj::buildingname_point::kMatchPer, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );

		// �����r�����̑f�ރ|�C���g��OID�E���́E�o�T�R�[�h�̃��X�g
		CListViewCtrl lvSrcList = GetDlgItem( IDC_LIST_SRCPOINT );
		for(int i = 0; i < list_col::building_src::ColumnNum; ++i)
			lvSrcList.InsertColumn( i, &m_ColumnInfo[i] );

		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, NULL, sj::buildingname_point::kTableName );

	}
}


// �c�[���`�b�v���쐬���܂��B
void CAttrBuildingNamePointDlg::SetToolTip(INT nTargetItem)
{
	
	CEdit edit( GetDlgItem(nTargetItem) );

	// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	// �c�[���`�b�v�����쐬
	CToolInfo tiEdit(
		TTF_IDISHWND | TTF_SUBCLASS,
		edit,
		(UINT_PTR)edit.m_hWnd,
		CRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
		lpszText );

	// �c�[���`�b�v�R���g���[���쐬
	CToolTipCtrl tooltip;
	tooltip.Create( m_hWnd );
	tooltip.Activate( TRUE );
	tooltip.AddTool( tiEdit );

}


// �u�����|���S��OID�擾�v�{�^�����������ꍇ�̏���
LRESULT CAttrBuildingNamePointDlg::OnBnClickedButtonSearchoid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �^�ʒu�����ɂ��錚���|���S����OID���擾

	// �\�����Ă��錚���r������
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipBuildingNameGeom = AheGetShape( ipFeature );

	// �����|���S��
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipBuilding = cMap.FindLayer( nullptr, nullptr, building::kTableName );
	if( !ipBuilding )
	{
		CString strMsg;
		strMsg.Format( AheLoadString( IDS_ERRORGETBYTOC ), building::kTableName );
		AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		return 0;
	}
	IFeatureClassPtr ipBuildingClass = AheGetFeatureClass( ipBuilding );
	
	// �����r�����̂̒����ɑ��݂��錚���|���S�����擾
	CString subField;
	subField.Format( _T("%s"), ipc_feature::kObjectID );
	IQueryFilterPtr ipQuery = AheInitQueryFilter( AheInitSpatialFilter( nullptr, ipBuildingNameGeom ), subField, nullptr );

	long lFeatureCount = 0;
	 ipBuildingClass->FeatureCount( ipQuery, &lFeatureCount );
	if( lFeatureCount > 1 ) // �����|���S���͈�ӏ��ɕ������݂��Ȃ��O��
	{
		ATLASSERT(FALSE);
		return 0;
	}

	IFeatureCursorPtr ipCursor;
	ipBuildingClass->Search( ipQuery, VARIANT_TRUE, &ipCursor );
	IFeaturePtr ipBuildingFeature;
	if( S_OK != ipCursor->NextFeature( &ipBuildingFeature ) || !ipBuildingFeature ){
		AttrMessageBox( AheLoadString( IDS_ERRORGETBUILDOID ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		return 0;
	}

	long lBuildingOID = AheGetOID( ipBuildingFeature );
	SetDlgItemInt( IDC_EDIT_BUILDINGOID, lBuildingOID, FALSE );

	return 0;
}


// �u��������(1)���́v�{�^�����������ꍇ�̏���
LRESULT CAttrBuildingNamePointDlg::OnBnClickedButtonPriorityc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComboBox cWAnnoClass( GetDlgItem( IDC_COMBO_PRIORITYC ) );
	cWAnnoClass.SetCurSel( sj::buildingname_point::priority_code::kEdit );

	m_cControlRel.ControlChanged( IDC_COMBO_PRIORITYC );
	InvalidateRect( NULL, FALSE );
	// �ύX���ꂽ�ꍇ��OK�{�^���� Enable ��
	SetButton( Changed() );

	return 0;
}


// �G���[�`�F�b�N
BOOL CAttrBuildingNamePointDlg::ErrorCheck()
{
	for( auto& it : *m_pFeatureDefList )
	{
		CString strTableName = it.GetTableName();
		if( strTableName == sj::buildingname_point::kTableName )
		{
			// BUILDING_OID��0�̂܂܁i�������Ɍ����|���S�����Ȃ��j�̏ꍇ�͍쐬�����Ȃ�
			CFieldDef* pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kBuildingOID );
			CComVariant vaBuildingOid = pFieldDef->m_vaValue;

			if( vaBuildingOid.lVal == 0 )
			{
				AttrMessageBox( AheLoadString( IDS_ERRORNOBUILDOID ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			CString errMsg;
			// �S�p�p�C�v��������NAME_SEPARATED��NAME�Ɠ���ł��邩�iNULL�̏ꍇ�̓`�F�b�N���Ȃ��j
			pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kNameSeparated );
			CFieldDef* pFieldDefName = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kName );
			if(!checkNameSeparated(pFieldDef->m_vaValue, pFieldDefName->m_vaValue))
				errMsg += _T("�u�b�v������������(����)�����̂ƈ�v���Ă��܂���B\n");

			// YOMI_SEPARATED���S�p�J�^�J�i����ёS�p�p�C�v�݂̂ł��邩�iNULL�̏ꍇ�̓`�F�b�N���Ȃ��j
			pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kYomiSeparated );
			if(!checkYomiSeparated(pFieldDef->m_vaValue))
				errMsg += _T("���~(����)�͑S�p�J�^�J�i����сu�b�v�̂ݓ��͉\�ł��B\n");

			if(!errMsg.IsEmpty()){
				AttrMessageBox( errMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}
		}
	}

	return TRUE;
}


// �S�p�p�C�v��������NAME_SEPARATED��NAME�Ɠ���ł��邩�m�F
bool CAttrBuildingNamePointDlg::checkNameSeparated(const CComVariant& vaNameSep, const CComVariant& vaName){
	CString strNameSeparated = uh::str_util::ToString(vaNameSep);
	if(strNameSeparated.IsEmpty()) // ��̏ꍇ�̓`�F�b�N���Ȃ�
		return true;

	// �S�p�p�C�v���������Ė��̂ƈ�v�����OK
	strNameSeparated.Replace(_T("�b"),_T(""));
	if(strNameSeparated.Compare(uh::str_util::ToString(vaName)) == 0)
		return true;

	return false;
}


// YOMI_SEPARATED���S�p�J�^�J�i����ёS�p�p�C�v�݂̂ł��邩�m�F
bool CAttrBuildingNamePointDlg::checkYomiSeparated(const CComVariant& vaVal){
	std::string strYomiSeparated = CT2A(uh::str_util::ToString(vaVal));
	if(strYomiSeparated.empty()) // ��̏ꍇ�̓`�F�b�N���Ȃ�
		return true;

	// �S�p�p�C�v���������đS�p�J�^�J�i�݂̂ł����OK
	if(CheckYomiCharacter(strYomiSeparated, "�b"))
		return true;

	return false;
}

