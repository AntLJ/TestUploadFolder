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

// AttrStationPolygonDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrStationPolygonDlg.h"

// boost
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(STATION_POLYGON_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_POLYGON_CLASS)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_POLYGON_CLASS)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_STATION_INFO)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 25)
			RCREST(IDC_GRID_STATION_INFO)
		ENDGROUP()
		RCSPACE(15)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_SEARCH_STATION)
			RCSPACE(12)
			RCTOFIT(IDC_STATIC_STATION_NAME)
			RCSPACE(8)
			RCTOFIT(IDC_EDIT_STATION_NAME)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_SEARCH_STATION)
			RCREST(-1)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_GRID_SEARCH_STATION)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_BUTTON_APPLY)
		ENDGROUP()
		BEGINCOLS(WRCT_PCT, 0, 45)
			RCREST(-1)
		ENDGROUP()
		RCSPACE(5)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrStationPolygonDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrStationPolygonDlg();
}



/// CAttrStationPolygonDlg �����o�֐�


// �R���X�g���N�^
CAttrStationPolygonDlg::CAttrStationPolygonDlg() 
	: ATTR_BASE_CLASS(STATION_POLYGON_MAP, IDD_STATIONPOLYGONDLG),
	  m_CurrentOid(0L),
	  m_bEditable(false)
{
	 // �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_POLYGON_CLASS, CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_POLYGON_CLASS ) )
		( IDC_BUTTON_APPLY       , CContainedWindow(  _T("BUTTON")   , this, IDC_BUTTON_APPLY ) )
		;
}



// �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂��
LRESULT CAttrStationPolygonDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

	// �R���g���[��
	CreateControlRelation();
	// �T�u�N���X��
	for( auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}

	// �V���N�C�x���g�̃A�h�o�C�Y
	AtlAdviseSinkMap(this, true);

	return 1;
}


// WM_COMMAND �p�C�x���g�n���h��
LRESULT CAttrStationPolygonDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( ! m_bFinishInit ) return 0;

	// ���상�b�Z�[�W�擾
	INT msg = HIWORD(wParam);						
	// ����ΏۃR���g���[���擾
	INT	nTargetControl = (INT)LOWORD(wParam);

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );
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


// �u�K�p�v�{�^�����������̏���
LRESULT CAttrStationPolygonDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �O���b�h�Ŏw�肵�����R�[�h���R���e�i�ɒǉ�
	getCurrentStationInfo()->addCheckedData();

	// �O���b�h�̕\�����X�V
	getCurrentStationInfo()->updateStationInfoGrid();
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	updateChanged();

	return 0;
}


// �w���̌����{�^�����������̏���
LRESULT CAttrStationPolygonDlg::OnClickStationSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �G�f�B�b�g�{�b�N�X�̕�����̎擾/has
	CWindow edit( GetDlgItem( IDC_EDIT_STATION_NAME ) );

	// �l���擾
	CString strStationName;
	edit.GetWindowText(strStationName);

	/// �w���Ō��� + �O���b�h�X�V
	getCurrentStationInfo()->searchStationName(strStationName);
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	return 0;
}


// �O���b�h�E�N���b�N�̃��j���[����u�폜�v�{�^���������ꂽ�Ƃ��̏���
LRESULT CAttrStationPolygonDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �f�[�^�폜
	getCurrentStationInfo()->deleteSelectedRow();

	// �O���b�h�X�V
	getCurrentStationInfo()->updateStationInfoGrid();
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	// �ύX��Ԃ��X�V
	updateChanged();

	return 0;
}


// [�w��񃊃X�g�O���b�h]�ҏW�O�̏���
void __stdcall CAttrStationPolygonDlg::OnStationInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	getCurrentStationInfo()->OnStationInfoGridBeforeEdit( Row, Col, Cancel);
}


// [�w��񃊃X�g�O���b�h]�}�E�X�A�b�v���̏���
void __stdcall CAttrStationPolygonDlg::OnStationInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	getCurrentStationInfo()->OnStationInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_STATION_INFO_GRID, m_bEditable );
}


// [�w��񌟍��O���b�h]�ҏW�O�̏���
void __stdcall CAttrStationPolygonDlg::OnStationSearchBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	getCurrentStationInfo()->OnSearchStationInfoBeforeEdit( Row, Col, Cancel, m_bEditable);
}


// �q�_�C�A���O���쐬����
HWND CAttrStationPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// �e��G���[�`�F�b�N��ɌĂяo����� 
bool CAttrStationPolygonDlg::Update()
{
	// �c���[�ŕ\������Ă��邷�ׂẴt�B�[�`�����X�V
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), station_detail::kTableName )   != 0 &&
		    _tcsicmp( featureDef.GetTableName(), midzoom_platform::kTableName ) != 0 &&
		    _tcsicmp( featureDef.GetTableName(), highzoom_station::kTableName ) != 0 &&
		    _tcsicmp( featureDef.GetTableName(), midzoom_station::kTableName )  != 0 &&
		    _tcsicmp( featureDef.GetTableName(), lowzoom_platform::kTableName ) != 0 )
		{
			continue;
		}

		// �X�V�t�B�[�`���̃��C���ԍ���OID���擾
		_IRowPtr ipRow = featureDef.GetFeature();
		ipRow->get_OID( &m_CurrentOid );
		m_CurrentLayerNo = CRelationTableHelper::getLayerNo( ipRow );

		// �֘A�e�[�u�����������Ă��Ȃ���Ή������Ȃ�
		if( ! hasStationInfo( m_CurrentLayerNo, m_CurrentOid ) )
			continue;

		// �֘A�e�[�u���ɕύX���Ȃ���Ή������Ȃ�
		if( ! getCurrentStationInfo()->isChange() )
			continue;

		// �֘A�e�[�u�����̍X�V
		if( ! getCurrentStationInfo()->updateTable() )
			return false;
	}
	return true;
}


// �쐬�����q�_�C�A���O���폜���܂��B
void CAttrStationPolygonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}


// ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n��
BOOL CAttrStationPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// STATION_DETAIL �ȊO�Ȃ�΁A�|���S����ʂ̐ݒ蕔���͔�\���ɂ���
	if( m_strClassName != station_detail::kTableName )
	{
		CWindow cComboBox( GetDlgItem( IDC_COMBO_POLYGON_CLASS ) );
		cComboBox.ShowWindow( SW_HIDE );
		CWindow cText( GetDlgItem( IDC_STATIC_POLYGON_CLASS ) );
		cText.ShowWindow( SW_HIDE );
	}

	// ���Y�t�B�[�`���̃��C���ԍ���OID���擾
	m_CurrentLayerNo = CRelationTableHelper::getLayerNo( m_strClassName );
	GetCurrentFeatureDef()->GetFeature()->get_OID( &m_CurrentOid );
	
	// ���Y�t�B�[�`���̕\�����n�߂ĂȂ�֘A�e�[�u���̏���o�^����
	if( ! hasStationInfo( m_CurrentLayerNo, m_CurrentOid ) )
	{
		// �w���̏�����
		SPStationInfo cStationInfo = std::make_shared<CStationInfoController>();
		if( ! cStationInfo->init( GetDlgItem(IDC_GRID_STATION_INFO), GetDlgItem(IDC_GRID_SEARCH_STATION), GetCurrentFeatureDef()->GetFeature() ))
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		// �w���̓o�^
		m_StationInfoList.insert( std::make_pair(StaObjKey( m_CurrentLayerNo, m_CurrentOid ), cStationInfo ));
	}


	// �ҏW�ېݒ�
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	m_bEditable = ( state != esriStateNotEditing );


	// ���Y�t�B�[�`���̘H�����X�g���\��
	getCurrentStationInfo()->updateStationInfoGrid();


	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_POLYGON_CLASS) );
	}

	return bRet;
}


// �f�[�^�ɕύX����������
BOOL CAttrStationPolygonDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || getCurrentStationInfo()->isChange();
}


// �q�_�C�A���O�����ۂɍ쐬
void CAttrStationPolygonDlg::CreateControlRelation()
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

		// �ڍ׃|���S���̂�
		if( m_strClassName == station_detail::kTableName )
		{
			// �|���S�����
			m_cControlRel.SetControl( IDC_COMBO_POLYGON_CLASS, TYPE_COMBO, 0, 0, station_detail::kPolygonType_C, 0, station_detail::kTableName );
		}
	}
}


// �{�^����Enable/Disable�̍X�V�A�֘A�e�[�u���̍X�V���̍X�V
void CAttrStationPolygonDlg::updateChanged()
{
	// �֘A�e�[�u���̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_station_site::kTableName);
	pRelTableDef->setChanged( getCurrentStationInfo()->isChange());

	// �{�^������
	SetButton( Changed() );
}


// ���ݕ\�����̃t�B�[�`���̉w��񃌃R�[�h��Ԃ�
CAttrStationPolygonDlg::SPStationInfo CAttrStationPolygonDlg::getCurrentStationInfo()
{
	auto iter = m_StationInfoList.find( StaObjKey( m_CurrentLayerNo, m_CurrentOid ) );
	_ASSERTE( iter != m_StationInfoList.end() );
	_ASSERTE( iter->second != nullptr );
	
	return iter->second;
}


// �w��̃��C���ԍ�&���C����OID�̃��R�[�h�����ɂ����Ă��邩�ۂ�
bool CAttrStationPolygonDlg::hasStationInfo( long lLayerNo, long lLayerOid) const
{
	auto iter = m_StationInfoList.find( StaObjKey( m_CurrentLayerNo, m_CurrentOid ) );
	return ( iter != m_StationInfoList.end() );
}
