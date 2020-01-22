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

// AttrRailwayNodeDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrRailwayNodeDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(NODE_MAP)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_STATIC_NODE_C)
				RCTOFIT(IDC_COMBO_NODE_C)
				//RCREST(-1)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_STATIC_RAILWAY_LIST)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_PCT, 0, 30)
				RCREST(IDC_GRID_LINE_INFO)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCPERCENT(IDC_STATIC_STATION_LIST, 30)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_PCT, 0, 40)
				RCREST(IDC_GRID_AROUND_STA)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_BUTTON_APPLY)
				//RCREST(-1)
			ENDGROUP()
			RCSPACE(5)
		ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrRailwayNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg2 * __stdcall CreateDlg()
{
	return new CAttrRailwayNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrRailwayNodeDlg::CAttrRailwayNodeDlg() 
	: ATTR_BASE_CLASS(NODE_MAP, IDD_RAILWAYNODEDLG),
	  m_CurrentOid(-1)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODE_C, CContainedWindow(  _T("COMBO") , this, IDC_COMBO_NODE_C ) ),
		( IDC_BUTTON_APPLY, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_APPLY ) )
		;
}


/**
 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
 */
LRESULT CAttrRailwayNodeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CAttrRailwayNodeDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}

	return 0;
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
HWND CAttrRailwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// �u�K�p�v�{�^���������ꂽ���̏���
LRESULT CAttrRailwayNodeDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_LineInfoList[m_CurrentOid].addCheckedRecord();
	updateChanged();

	return 0;
}


// �O���b�h�E�N���b�N�̃��j���[����u�폜�v�{�^���������ꂽ�Ƃ��̏���
LRESULT CAttrRailwayNodeDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_LineInfoList[m_CurrentOid].deleteSelectedRow();
	updateChanged();

	return 0;
}


// �H�����X�g�O���b�h�ҏW�O�̏���
void __stdcall CAttrRailwayNodeDlg::OnLineInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridBeforeEdit(Row, Col, Cancel);
}


// �H�����X�g�O���b�h�ҏW��̏���
void __stdcall CAttrRailwayNodeDlg::OnLineInfoAfterEdit(LONG Row, LONG Col)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridAfterEdit(Row, Col);
	updateChanged();
}


// �H�����X�g�O���b�h�}�E�X�A�b�v���̏���
void __stdcall CAttrRailwayNodeDlg::OnLineInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_LINE_INFO_GRID);
}


// ���Ӊw���X�g�O���b�h�ҏW�O�̏���
void __stdcall CAttrRailwayNodeDlg::OnAroundStaBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_LineInfoList[m_CurrentOid].OnAroundStaBeforeEdit(Row, Col, Cancel);
}


// OK�{�^���������ꂽ��ɌĂяo�����
BOOL CAttrRailwayNodeDlg::ErrorCheck()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_node::kTableName ) != 0 )
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );
		
		// �֘A�e�[�u�����ǂݍ��܂�Ă��Ȃ�(�c���[�ɑ��݂��邪�J����Ă��Ȃ�)�t�B�[�`����
		// �֘A�e�[�u����ǂݍ���ł���`�F�b�N����
		if( m_LineInfoList.find(lOid) == m_LineInfoList.end() )
		{
			CLineInfoController cLICtlr;
			cLICtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_AROUND_STA), GetCurrentFeatureDef()->GetFeature(), m_cArcHelper );
			m_LineInfoList.insert(std::make_pair(lOid, cLICtlr));
		}

		for( auto fieldDef : featureDef )
		{
			if( fieldDef.m_strFieldName != sj::railway_node::kNodeClass_C )
				continue;

			// �m�[�h��ʂƊ֘A�e�[�u���̐������`�F�b�N
			if( ! checkNodeClass( m_LineInfoList[lOid], fieldDef.m_vaValue , lOid) )
				return FALSE;
		}
	}

	return TRUE;
}


// �e��G���[�`�F�b�N��ɌĂяo�����
bool CAttrRailwayNodeDlg::Update()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_node::kTableName ) != 0 )
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// �֘A�e�[�u���ɕύX������΍X�V
		if( ! m_LineInfoList[lOid].isChange() )
			continue;

		if( ! m_LineInfoList[lOid].updateTable() )
			return false;
	}
	return true;
}


// �m�[�h��ʂƊ֘A�e�[�u���̐������`�F�b�N
bool CAttrRailwayNodeDlg::checkNodeClass( const CLineInfoController& cLineInfoCntr, const CComVariant& vaNodeType, long lOid )
{
	// �m�[�h��ʂƊ֘A�e�[�u���̐������`�F�b�N
	if( vaNodeType.lVal == sj::railway_node::node_class::kStation )
	{
		if( cLineInfoCntr.isEnptyRelData() )
		{	
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_LINEINFO_IS_NOT_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	else
	{
		if( ! cLineInfoCntr.isEnptyRelData() )
		{
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_LINEINFO_IS_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	return true;
}


/**
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * @warning  �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrRailwayNodeDlg::Delete()
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
BOOL CAttrRailwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
	if( m_LineInfoList.find( m_CurrentOid ) == m_LineInfoList.end() )
	{
		CLineInfoController cLineInfoGrid;
		// �H�����X�g�O���b�h�̏�����
		if( ! cLineInfoGrid.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_AROUND_STA), 
							GetCurrentFeatureDef()->GetFeature(), m_cArcHelper ) )
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		m_LineInfoList.insert( std::make_pair(m_CurrentOid, cLineInfoGrid ));
	}

	// �O���b�h�̕ҏW�ۂ͌ʂɐ���
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	if( state == esriStateNotEditing )
		m_LineInfoList[m_CurrentOid].disable();

	// �I�����Ă���t�B�[�`���[�̌`����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	// ���ӂ̉w�����������ă��X�g�r���[�֕\��
	m_LineInfoList[m_CurrentOid].searchStationInfo(ipFeature, ipLayer);
	m_LineInfoList[m_CurrentOid].updateLineInfoGrid();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_BUTTON_APPLY) );
	}

	// �ŏ��Ƀt�H�[�J�X�����R���g���[��
	::SetFocus( GetDlgItem(IDC_COMBO_NODE_C) );

	return bRet;
}


// �f�[�^�ɕύX����������
BOOL CAttrRailwayNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_LineInfoList[m_CurrentOid].isChange();
}


/**
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrRailwayNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �m�[�h���
		m_cControlRel.SetControl( IDC_COMBO_NODE_C, TYPE_COMBO, 0, 0, railway_node::kNodeClass_C, 0, railway_node::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, railway_node::kTableName );
		// �K�p�{�^��
		m_cControlRel.SetControl( IDC_BUTTON_APPLY,	TYPE_BUTTON, 	0, 0 );
	}
}


/**
* @brief TOC����w�背�C�����擾
* @param layerName [in]     TOC����擾���郌�C����
* @param stCount   [in,out] TOC��ɑ��݂��郌�C�����̐����擾
* @param ipMap     [in]     IMapPtr
*/
ILayerPtr CAttrRailwayNodeDlg::getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOC����layerName���擾
	CMap cMap( ipMap );
	// TOC���layerName�̐����J�E���g
	stCount = 0;
	ILayerPtr ipLayer;
	for( auto& it : cMap )
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
void CAttrRailwayNodeDlg::updateChanged()
{
	// �֘A�e�[�u���̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(sj::rel_railway_node::kTableName);
	pRelTableDef->setChanged( m_LineInfoList[m_CurrentOid].isChange());

	// �{�^������
	SetButton( Changed() );
}
