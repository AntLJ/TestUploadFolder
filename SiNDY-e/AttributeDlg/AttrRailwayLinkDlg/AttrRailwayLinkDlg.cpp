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

// AttrRailwayLinkDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrRailwayLinkDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(LINK_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_CLASS)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_RAILWAY_CLASS)
			RCSPACE(30)
			RCTOFIT(IDC_UNDERGROUND_F)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_DISPSCALE)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_DISPSCALE)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_LIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 25)
			RCREST(IDC_GRID_LINE_INFO)
		ENDGROUP()
		RCSPACE(15)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_SEARCH)
			RCSPACE(8)
			RCTOFIT(IDC_STATIC_STATION_NAME)
			RCSPACE(4)
			RCTOFIT(IDC_EDIT_STATION_SEARCH)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_STATION_SEARCH)
			RCSPACE(4)
			RCREST(0)
			RCTOFIT(IDC_BUTTON_AROUND_SEARCH)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_GRID_SEARCH_RESULT)
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
 * @retval CAttrRailwayLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg2 * __stdcall CreateDlg()
{
	return new CAttrRailwayLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrRailwayLinkDlg::CAttrRailwayLinkDlg() 
	: ATTR_BASE_CLASS(LINK_MAP, IDD_RAILWAYLINKDLG),
	  m_CurrentOid(-1),
	  m_bIsLinkOnMesh(false),
	  m_bEditable(false)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_RAILWAY_CLASS, CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_RAILWAY_CLASS ) ),
		( IDC_COMBO_DISPSCALE    , CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_DISPSCALE ) ),
		( IDC_UNDERGROUND_F      , CContainedWindow(  _T("CHECKBOX") , this, IDC_UNDERGROUND_F ) ),
		( IDC_BUTTON_APPLY       , CContainedWindow(  _T("BUTTON")   , this, IDC_BUTTON_APPLY ) )
		;
}

/**
 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
 */
LRESULT CAttrRailwayLinkDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

/**
 * WM_COMMAND �p�C�x���g�n���h��
 *
 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
 */
LRESULT CAttrRailwayLinkDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( !m_bFinishInit ) return 0;

	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

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
		// 2�����b�V�����E���ׂ��Ŕ��Α��̐ڑ������N���X�V
		syncLinkAttrOnMesh();
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}

	return 0;
}


// �u�K�p�v�{�^���������ꂽ���̏���
LRESULT CAttrRailwayLinkDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_RailwayLineList[m_CurrentOid].addCheckedData();
	syncRelTableOnMesh();

	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();
	updateChanged();

	return 0;
}


// �w���Ō����{�^���������ꂽ�Ƃ��̏���
LRESULT CAttrRailwayLinkDlg::OnClickStaSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �G�f�B�b�g�{�b�N�X�̕�����̎擾
	CWindow edit( GetDlgItem( IDC_EDIT_STATION_SEARCH ) );

	// �l���擾
	CString strStationName;
	edit.GetWindowText(strStationName);

	// �������[�h�ύX
	m_RailwayLineList[m_CurrentOid].setSearchMode(search_railway_grid::kStarionNameSearch);
	// �w���Ō��� + �O���b�h�X�V
	m_RailwayLineList[m_CurrentOid].searchStaName(strStationName);
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	return 0;
}


// ���Ӊw�̘H�������{�^���������ꂽ�Ƃ��̏���
LRESULT CAttrRailwayLinkDlg::OnClickAroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �������[�h�ύX
	m_RailwayLineList[m_CurrentOid].setSearchMode(search_railway_grid::kAroundSearch);
	// �O���b�h�X�V
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	return 0;

}


// �O���b�h�E�N���b�N�̃��j���[����u�폜�v�{�^���������ꂽ�Ƃ��̏���
LRESULT CAttrRailwayLinkDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_RailwayLineList[m_CurrentOid].deleteSelectedRow();
	syncRelTableOnMesh();

	// �O���b�h�X�V
	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	updateChanged();

	return 0;
}


// �ĕ`��
LRESULT CAttrRailwayLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �n�C���C�g
	m_RailwayLineList[m_CurrentOid].highlightFeatures();

	return 0;
}


// �H�����X�g�O���b�h�ҏW�O�̏���
void __stdcall CAttrRailwayLinkDlg::OnLineInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_RailwayLineList[m_CurrentOid].OnRailwayLineGridBeforeEdit( Row, Col, Cancel);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


// �H�����X�g�O���b�h�}�E�X�A�b�v���̏���
void __stdcall CAttrRailwayLinkDlg::OnLineInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	m_RailwayLineList[m_CurrentOid].OnLineInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_LINE_INFO_GRID, m_bEditable );

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


// �H�������O���b�h�ҏW�O�̏���
void __stdcall CAttrRailwayLinkDlg::OnRailwaySearchBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_RailwayLineList[m_CurrentOid].OnSearchRailwayBeforeEdit( Row, Col, Cancel, m_bEditable);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


/** 
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrRailwayLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// OK�{�^���������ꂽ��ɌĂяo�����
BOOL CAttrRailwayLinkDlg::ErrorCheck()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_link::kTableName ) != 0 )
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// �֘A�e�[�u�����ǂݍ��܂�Ă��Ȃ�(�c���[�ɑ��݂��邪�J����Ă��Ȃ�)�t�B�[�`����
		// �֘A�e�[�u����ǂݍ���ł���`�F�b�N����
		if( m_RailwayLineList.find(lOid) == m_RailwayLineList.end() )
		{
			CRailwayLineController cRLCtlr;
			cRLCtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), GetCurrentFeatureDef()->GetFeature(), m_cArcHelper );
			m_RailwayLineList.insert(std::make_pair(lOid, cRLCtlr));
		}

		for( const auto& fieldDef : featureDef )
		{
			if( fieldDef.m_strFieldName != sj::railway_link::kRailwayClass_C )
				continue;

			// �m�[�h��ʂƊ֘A�e�[�u���̐������`�F�b�N
			if( ! checkLinkClass( m_RailwayLineList[lOid], fieldDef.m_vaValue , lOid) )
				return FALSE;
		}
	}

	return TRUE;
}


// �e��G���[�`�F�b�N��ɌĂяo����� 
bool CAttrRailwayLinkDlg::Update()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_link::kTableName ) != 0 )
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// �֘A�e�[�u���ɕύX������΍X�V
		if( ! m_RailwayLineList[lOid].isChange() )
			continue;

		if( ! m_RailwayLineList[lOid].updateTable() )
			return false;
	}
	return true;
}


/**
 * @fn void CAttrRailwayLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrRailwayLinkDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}


/**
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
BOOL CAttrRailwayLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// TOC����STATION_POINT���擾
	// 2�ȏ㑶�݂���Ȃ�G���[
	int stCount = 0;
	ILayerPtr ipLayer = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	if( stCount >= 2 )
	{
		AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_HAS_TWO_STATION_POINT ), AheLoadString( IDS_ERR_FAILED_SPECIFIC_STATION_POINT ), MB_OK|MB_TASKMODAL, NULL );
		return 0;
	}

	GetCurrentFeatureDef()->GetFeature()->get_OID( &m_CurrentOid );
	// ���Y�t�B�[�`���̕\�������߂ĂȂ�֘A�e�[�u���O���b�h�̏���o�^����
	if( m_RailwayLineList.find( m_CurrentOid ) == m_RailwayLineList.end() )
	{
		CRailwayLineController cLineInfoGrid;
		// �H�����X�g�O���b�h�̏�����
		if( ! cLineInfoGrid.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), 
							GetCurrentFeatureDef()->GetFeature(), m_cArcHelper ))
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		m_RailwayLineList.insert( std::make_pair(m_CurrentOid, cLineInfoGrid ));
	}

	// 2�����b�V�����ׂ��ł���S�������N���擾
	m_LinkOnMeshList.clear();
	m_bIsLinkOnMesh = getLinkOnMeshFeatureDefs(m_LinkOnMeshList);

	// �ҏW�ېݒ�
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	m_bEditable = ( state != esriStateNotEditing );

	// �I�����Ă���t�B�[�`���[�̌`����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	// ���ӂ̉w�����������ă��X�g�r���[�֕\��
	m_RailwayLineList[m_CurrentOid].searchStationInfo(ipFeature, ipLayer);
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();
	// ���Y�t�B�[�`���̘H�����X�g���\��
	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_RAILWAY_CLASS) );
	}

	return bRet;
}


// �f�[�^�ɕύX����������
BOOL CAttrRailwayLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_RailwayLineList[m_CurrentOid].isChange();
}


/**
 * @fn void CAttrRailwayLinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrRailwayLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( ! m_cControlRel.empty() )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �S�����
		m_cControlRel.SetControl( IDC_COMBO_RAILWAY_CLASS, TYPE_COMBO, 0, 0, railway_link::kRailwayClass_C, 0, railway_link::kTableName );
		// �\���X�P�[�����
		m_cControlRel.SetControl( IDC_COMBO_DISPSCALE, TYPE_COMBO, 0, 0, railway_link::kDispScale_C, 0, railway_link::kTableName );
		// �n���t���O
		m_cControlRel.SetControl( IDC_UNDERGROUND_F, TYPE_CHECKBOX, 0, 0, railway_link::kUnderGround_F, 0, railway_link::kTableName );
		// �K�p�{�^��
		m_cControlRel.SetControl( IDC_BUTTON_APPLY,	TYPE_BUTTON, 	0,	0 );
	}
}


// TOC����w�背�C�����擾
ILayerPtr CAttrRailwayLinkDlg::getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOC����layerName���擾
	CMap cMap( ipMap );
	// TOC���layerName�̐����J�E���g
	stCount = 0;
	ILayerPtr ipLayer;
	for( const auto& it : cMap )
	{
		if( AheGetFeatureClassName( it ).Compare( layerName ) == 0 )
		{
			ipLayer = it;
			++stCount;
		}
	}
	return ipLayer;
}


// �{�^����Enable/Disable�̍X�V�A�֘A�e�[�u���̍X�V���̍X�V
void CAttrRailwayLinkDlg::updateChanged()
{
	// �֘A�e�[�u���̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(sj::rel_railway_link::kTableName);
	pRelTableDef->setChanged( m_RailwayLineList[m_CurrentOid].isChange());

	// �{�^������
	SetButton( Changed() );
}


// �S�������N��ʂƊ֘A�e�[�u���̐������`�F�b�N
bool CAttrRailwayLinkDlg::checkLinkClass(const CRailwayLineController& cRailLineCntr, const CComVariant& vaLinkType, long lOid)
{
	using namespace sj::railway_link;

	if( vaLinkType.lVal == link_class::kDivideByPoi )
	{
		if( cRailLineCntr.isEmptyRelData() )
		{	
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_RAILWAYLINE_IS_NOT_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	// �S���������͘H�������蓖�Ă��Ȃ�
	else if( vaLinkType.lVal == link_class::kSideTrack )
	{
		if( ! cRailLineCntr.isEmptyRelData() )
		{
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_RAILWAYLINE_IS_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	return true;
}


// ���b�V�����E���ׂ��t�B�[�`���̑������R�s�[����  
void CAttrRailwayLinkDlg::syncLinkAttrOnMesh()
{
	using namespace sj::railway_link;

	if ( ! m_bIsLinkOnMesh )
		return;

	// ���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v
	for( auto iter = m_pFeatureDefList->begin(); iter != m_pFeatureDefList->end(); ++iter )
	{
		IFeaturePtr temp;
		long lOid = -1;
		temp = iter->GetFeature();
		temp->get_OID( &lOid );


		// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
		CFeatureDef* pFeatureDef = nullptr;
		for( auto feature = m_LinkOnMeshList.begin(); feature != m_LinkOnMeshList.end(); ++feature )
		{
			long lMeshOid = -1;
			feature->GetFeature()->get_OID( &lMeshOid );
			if( lOid == lMeshOid )
			{
				pFeatureDef = &(*iter);
				break;
			}
		}
		// ���b�V���ׂ��S�������N���擾�ł��Ȃ���΁A�������Ȃ�
		if( ! pFeatureDef )
			continue;


		// ���ׂẴt�B�[���h�Ń��[�v
		for( auto field = GetCurrentFeatureDef()->begin(); field != GetCurrentFeatureDef()->end(); ++field )
		{
			CString strFieldName = field->m_strFieldName;

			// �R�s�[�Ώۂ͓S����ʁA�X�P�[���\����ʁA�n���t���O�̂�
			if( strFieldName != kRailwayClass_C && strFieldName != kDispScale_C && strFieldName != kUnderGround_F )
				continue;

			// �Ώۃt�B�[���h�̑������R�s�[
			CFieldDef *pFieldDef = pFeatureDef->GetFieldDef( kTableName, field->m_strFieldName );
			CComVariant vaValue;
			pFeatureDef->GetFeature()->get_Value( field->m_lFieldIndex, &vaValue );
			if( field->m_vaValue != vaValue )
			{
				pFieldDef->m_vaValue = field->m_vaValue;
				pFieldDef->m_bChanged = TRUE;
			}
			else{
				pFieldDef->m_vaValue = field->m_vaValue;
				pFieldDef->m_bChanged = FALSE;
			}
		}
	}
}


// ���b�V�����E���ׂ��t�B�[�`���̊֘A�e�[�u���𓯊�����
void CAttrRailwayLinkDlg::syncRelTableOnMesh()
{
	if ( ! m_bIsLinkOnMesh )
		return;

	// ���ݕێ����Ă��郊�X�g���ׂĂɂ����ă��[�v
	for( auto iter = m_pFeatureDefList->begin(); iter != m_pFeatureDefList->end(); ++iter )
	{
		IFeaturePtr temp;
		long lOid = -1;
		temp = iter->GetFeature();
		temp->get_OID( &lOid );

		// ���ݑI�𒆂�2�����b�V�����ׂ��ł��铹�H�����N�Ɋ֘A���Ă��郊�X�g�Ƃ̏ƍ�
		CFeatureDef* pFeatureDef = nullptr;
		for( auto feature = m_LinkOnMeshList.begin(); feature != m_LinkOnMeshList.end(); ++feature )
		{
			long lMeshOid = -1;
			feature->GetFeature()->get_OID( &lMeshOid );
			if( lOid == lMeshOid )
			{
				pFeatureDef = &(*iter);
				break;
			}
		}
		// ���b�V���ׂ��S�������N���擾�ł��Ȃ���΁A�������Ȃ�
		if( ! pFeatureDef )
			continue;

		// �R�s�[���OID�擾
		long lTargetOid = -1;
		pFeatureDef->GetFeature()->get_OID( &lTargetOid );

		// �R�s�[����OID�擾
		long lCurrentOid = -1;
		GetCurrentFeatureDef()->GetFeature()->get_OID( &lCurrentOid );

		// �֘A�e�[�u���̓���
		m_RailwayLineList[lCurrentOid].syncRailwayLineData(m_RailwayLineList[lTargetOid]);
		// �֘A�e�[�u���̕ύX��ʒm
		CRelationalTableDef* pRelTableDef = pFeatureDef->GetRelationalTableDef(sj::rel_railway_link::kTableName);
		pRelTableDef->setChanged( m_RailwayLineList[lTargetOid].isChange());
	}
}


// ���b�V�����E���ׂ��t�B�[�`���Q���擾����
bool CAttrRailwayLinkDlg::getLinkOnMeshFeatureDefs( std::list<CFeatureDef>& cMeshCrossedLinks )
{
	// ���ݑI�𒆂̃t�B�[�`���擾
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cMeshCrossedLink;

	// ���b�V�����ׂ��ł����̓S�������N���擾����
	if( AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), ipCurrentFeature, cMeshCrossedLink ) )
	{
		for( auto& iter = cMeshCrossedLink.begin(); iter != cMeshCrossedLink.end(); ++iter )
		{
			// ���b�V�����ׂ�����̓S�������N�̑������i�[
			IFeaturePtr ipFeatureOnMesh = iter->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			cMeshCrossedLinks.push_back(featDef);

			// ���b�V�����ׂ�����̓S�������N�R�t���e�[�u�����擾
			CRailwayLineController cLineInfoCtlr;
			cLineInfoCtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), 
				_IRowPtr(ipFeatureOnMesh), 
				m_cArcHelper );
			long lOid = -1;
			ipFeatureOnMesh->get_OID( &lOid );
			m_RailwayLineList.insert( std::make_pair(lOid, cLineInfoCtlr ));
		}
	}

	return ! cMeshCrossedLinks.empty();
}
