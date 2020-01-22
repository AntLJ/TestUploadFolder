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

// AttrWalkNodeDlg.cpp : DLL �G�N�X�|�[�g�̎����ł��B


#include "stdafx.h"
#include "AttrWalkNodeDlg.h"
#include <boost/assign/list_of.hpp>
#include "Map.h"
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema;

/// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(WALKNODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			// �m�[�h���
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WALKNODECLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WALKNODECLASS)
			ENDGROUP()
			RCSPACE(10)

			// ���H�m�[�hID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADNODEID)
				RCTOFIT(IDC_EDIT_ROADNODEID)
				RCTOFIT(IDC_STATIC_CROSSINGNAME)
				RCREST(IDC_STATIC_CROSSINGNAME2)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHROADNODE)
				RCTOFIT(IDC_DELETE_ROADNODEID)
			ENDGROUP()
			RCSPACE(10)

			// �w�o����ID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REFID)
				RCTOFIT(IDC_EDIT_STATIONGATE)
				RCTOFIT(IDC_STATIC_GATESTATION)
				RCREST(IDC_STATIC_STATIONGATE_NAME)
				RCTOFIT(IDC_BUTTON_SEARCHSTATIONGATE)
				RCTOFIT(IDC_DELETE_STATIONGATEID)
			ENDGROUP()
			RCSPACE(10)

			// �n���X�o����ID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_UNDERGROUNDGATENODEID)
				RCTOFIT(IDC_EDIT_UNDERGROUNDGATE)
				RCTOFIT(IDC_STATIC_UNDERGROUNDGATE)
				RCREST(IDC_STATIC_UNDERGROUNDGATE_NAME)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHUNDERGROUNDGATE)
				RCTOFIT(IDC_DELETE_UNDERGROUNDGATEID)
			ENDGROUP()
			RCSPACE(10)

			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCSPACE(50)
					RCTOFIT(IDC_BUTTON_INSERT)
				ENDGROUP()
				BEGINROWS(WRCT_REST, 0, RCMARGINS(2,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_SEARCH_RESULT_LABEL)
						RCTOFIT(IDC_STATIC_SEARCH)
						RCFIXED(IDC_EDIT_SEARCH,60)
						RCTOFIT(IDC_STATIC_SEARCH2)
					ENDGROUP()
					RCTOFIT(-1)
					RCSPACE(3)
					RCREST(IDC_LIST_RESULT)
				ENDGROUP()
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

namespace
{
_TCHAR NULL_STRING[] = _T("(null)"); //! ���̂���̏ꍇ�ɕ\�����镶����

} // namespace

/// ���X�g�r���[�̊e�J�����̏����`
LVCOLUMN CAttrWalkNodeDlg::m_cResultColInfo[] =
{
	{LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 50, _T("�h�c"), 0, 0, 0, 0 },
	{LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 100, _T("Unknown"), 0, 0, 0, 0 }
};

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrWalkNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrWalkNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrWalkNodeDlg::CAttrWalkNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrWalkNodeDlg::CAttrWalkNodeDlg() 
	: ATTR_BASE_CLASS(WALKNODE_MAP, IDD_ATTRWALKNODEDLG)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_WALKNODECLASS,			CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_WALKNODECLASS ) )
		( IDC_EDIT_ROADNODEID,				CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_ROADNODEID ) )
		( IDC_BUTTON_SEARCHROADNODE,		CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHROADNODE ) )
		( IDC_EDIT_STATIONGATE,				CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_STATIONGATE ) )
		( IDC_EDIT_UNDERGROUNDGATE,			CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_UNDERGROUNDGATE ) )
		( IDC_EDIT_SEARCH,					CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SEARCH ) )
		( IDC_BUTTON_SEARCHSTATIONGATE,		CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHSTATIONGATE ) )
		( IDC_BUTTON_SEARCHUNDERGROUNDGATE,	CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHUNDERGROUNDGATE ) )
		( IDC_LIST_RESULT,					CContainedWindow(  _T("LIST"),   this, IDC_LIST_RESULT ) )
		( IDC_BUTTON_INSERT,				CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_INSERT ) )
		( IDC_EDIT_SOURCE,					CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SOURCE ) )
		( IDC_DELETE_ROADNODEID,			CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_ROADNODEID ) )
		( IDC_DELETE_STATIONGATEID,			CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_STATIONGATEID ) );
		( IDC_DELETE_UNDERGROUNDGATEID,		CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_UNDERGROUNDGATEID ) );
}

/** 
 * @fn HWND CAttrWalkNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrWalkNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrWalkNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrWalkNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrWalkNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * ���e�ԍ��̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */

BOOL CAttrWalkNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema;

    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���͂���Ă���֘AID�ɍ��킹�ĕ\�����閼�̂�ύX
	SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME );
	SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2 );
	SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME );

	// �s���ȓ��͂����o���邽�߂̗v�f��������
	m_searchedLayer[GetCurrentFeatureDef()->GetFeature()] = nullptr;
	m_errors[GetCurrentFeatureDef()->GetFeature()].Reset();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_WALKNODECLASS) );

		// �R���g���[���̗L���E�����𐧌䂷��
		// �m�[�h��ʂɂ���Ċe�t�B�[���h�̕ҏW�ۂ��قȂ�
		SetControlState(true);
	}
	else
		SetControlState(false);

	return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrWalkNodeDlg::CreateControlRelation()
{
	using namespace  sindy::schema;

	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// �������
		m_cControlRel.SetControl( IDC_COMBO_WALKNODECLASS,	TYPE_COMBO,	0,	0,  walk_node::kNodeClass,	NULL, walk_node::kTableName );
		// ���H�m�[�h�Ή�ID
		m_cControlRel.SetControl( IDC_EDIT_ROADNODEID,		TYPE_EDIT,	0,	0,  walk_node::kRoadNodeID,	NULL, walk_node::kTableName );
		// �w�o�����|�C���gID
		m_cControlRel.SetControl( IDC_EDIT_STATIONGATE,		TYPE_EDIT,	0,	0,  walk_node::kStationGatePointID,	NULL, walk_node::kTableName );
		// [bug 11559][e][SJ���s��]���s�҃m�[�h�ƒn���X�o����POI�̕R�Â����o����悤�ɂ��Ăق���
		// �n���X�o�����|�C���gID
		m_cControlRel.SetControl( IDC_EDIT_UNDERGROUNDGATE,	TYPE_EDIT,	0,	0,  walk_node::kUndergroundGatePointID,	NULL, walk_node::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,  walk_node::kSource,		NULL, walk_node::kTableName );
	}
}

/**
 * @fn void CAttrBaseRailwayDlg::SetDispButton()
 * @brief �R���g���[���̗L���E�����𔻒f����
 *
 */
void CAttrWalkNodeDlg::SetControlState(bool bEditable)
{
	// ��U�A�S�ẴR���g���[����FALSE�ɂ���
	::EnableWindow( GetDlgItem( IDC_COMBO_WALKNODECLASS ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_INSERT ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_SEARCH ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_LIST_RESULT ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_SOURCE ), FALSE );

	// ���X�g�r���[�̃A�C�e���E�J���������ׂč폜
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	lv.DeleteAllItems();
	lv.DeleteColumn(0);
	lv.DeleteColumn(0);

	// ���s�҃_�E���R���o�[�g�Ɣėp�ł̕\���̐؂�ւ�
	int nCmdShow = SW_HIDE;
	if( AheIsDownconvert( GetCurrentFeatureDef()->GetFeature() ) == walk_type::type_class::kGeneral )
		nCmdShow = SW_SHOW;

	::ShowWindow( GetDlgItem( IDC_STATIC_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_CROSSINGNAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_CROSSINGNAME2 ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_REFID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_GATESTATION ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_STATIONGATE_NAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATENODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATE_NAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_INSERT ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_SEARCH_RESULT_LABEL ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_SEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCH2 ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_RESULT ), nCmdShow );

	// �s���ȓ��̓f�[�^������΃G���[�t���O�𗧂Ă�
	CheckInvaliedValue();

	// �ҏW�\��ԂłȂ��Ȃ�ȉ��̏����͍s��Ȃ�
	if( !bEditable ) return;

	// �n���X�o�����Ή�ID�t�B�[���h�����݂��邩�`�F�b�N
	bool existUndergroundGatePointIDField = CheckExistField( walk_node::kTableName, walk_node::kUndergroundGatePointID );

	// ���\�[�X�ƃR���{�{�b�N�X�͑S�Ă̎�ʂŕύX�\
	::EnableWindow( GetDlgItem( IDC_COMBO_WALKNODECLASS ), TRUE );
	::EnableWindow( GetDlgItem(IDC_EDIT_SOURCE), TRUE );
	// ���������Ȃ�ǂ̎�ʂł��\
	::EnableWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), TRUE );
	::EnableWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), TRUE );
	// �t�B�[���h�����݂����ꍇ�̂݉\
	if( existUndergroundGatePointIDField )
		::EnableWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), TRUE );

	// ���݂̃m�[�h��ʂ��擾
	CComboBox combo = GetDlgItem(IDC_COMBO_WALKNODECLASS);
	DWORD_PTR nodeClass = combo.GetItemData( combo.GetCurSel() );

	// �ȉ��A��ʌŗL�̃R���g���[��������
	// �m�[�h��ʂ�����̂��̂Ȃ�A�C���T�[�g�͋����Ȃ�
	switch(nodeClass)
	{
	// TODO: �����ʂŃG�f�B�b�g�n�̃R���g���[���̓��͋������Ȃ��Ȃ�A�����Ɏ�ʂ�ǉ�����
	case walk_node::node_class::kOutline: // �}�s��m�[�h
		break;
	default:
		// ���X�g�\���E�C���T�[�g�E�����_�����֘A�͏�L��ʈȊO�Ȃ���͉\
		::EnableWindow( GetDlgItem( IDC_BUTTON_INSERT ), TRUE );
		::EnableWindow( GetDlgItem( IDC_EDIT_SEARCH ), TRUE );
		::EnableWindow( GetDlgItem( IDC_LIST_RESULT ), TRUE );
		::EnableWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), TRUE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), TRUE );

		// �X�Ɍ������X�g�r���[�̍쐬�̎d���́A��ʂŕ�����
		switch(nodeClass)
		{
		case walk_node::node_class::kGate:   /// �w�o�����m�[�h
			// �w�o�����Ȃ�w�o�����p�̃��X�g�r���[���쐬���A�����{�^�����̃R���g���[��������
			::EnableWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), TRUE );
			::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), TRUE );
			// Msg �w�o��������, �w�o�������ӌ�������
			CreateListView(AheLoadString(IDS_STATIONGATECOLUMN), AheLoadString(IDS_STATIONGATERESULT));
			break;
		// [bug 11559][e][SJ���s��]���s�҃m�[�h�ƒn���X�o����POI�̕R�Â����o����悤�ɂ��Ăق���
		case walk_node::node_class::kUndergroundGate:   /// �n���X�o�����m�[�h
			// �Ώۃt�B�[���h�����݂���ꍇ�̂݉\
			if( existUndergroundGatePointIDField )
			{
				// �n���X�o�����Ȃ�n���X�o�����p�̃��X�g�r���[���쐬���A�����{�^�����̃R���g���[��������
				::EnableWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), TRUE );
				::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), TRUE );
				// Msg �n���X�o��������, �n���X�o�������ӌ�������
				CreateListView(AheLoadString(IDS_UNDERGROUNDGATECOLUMN), AheLoadString(IDS_UNDERGROUNDGATERESULT));
				break;
			}
			// �Ώۃt�B�[���h�����݂��Ȃ��ꍇ��default����
		default:
			// ��{�I�Ɍ����_�����p�̃��X�g�r���[���쐬����
			// Msg �����_����, �����_���ӌ�������
			CreateListView(AheLoadString(IDS_CROSSNODECOLUMN), AheLoadString(IDS_CROSSRESULT));
			break;
		}
		break;
	}
}

BOOL CAttrWalkNodeDlg::ErrorCheck()
{
	for(auto& elem : m_errors )
	{
		// �G���[���Ȃ��Ȃ牽�����Ȃ�
		if(!elem.second.IsError()) continue;

		// �G���[������Ȃ�G���[����\��
		CString errMsg; //!< �\������G���[���b�Z�[�W���i�[

		// �I�u�W�F�N�gID�ƒ�^����i�[
		// Msg:%s [%ld] �ɉ��L�̃G���[������܂��B
		errMsg.Format(AheLoadString( IDS_ERRORID ), ipc_feature::kObjectID, AheGetOID(elem.first));

		// ���͂��ꂽ�G���[�̈ꗗ���擾
		auto errorList = elem.second.GetStringList();
		for(auto& msg : errorList)
		{
			// ���s���ĒǋL
			errMsg.Append( _T("\n")+msg );
		}

		// ���`�����G���[����\��
		AttrMessageBox( errMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return FALSE;
	}
	return TRUE;
}

IFeatureClassPtr CAttrWalkNodeDlg::GetFeatureClass( LPCTSTR fcName , bool popupMsgTOCHide/*=true*/ )
{
	bool bCancel = false;
	IFeatureClassPtr ipFC = GetSameGroupFeatureClass(fcName, GetCurrentFeatureDef()->GetFeature(), true, bCancel, popupMsgTOCHide);
	// �L�����Z�����ꂽ�ꍇ�͓��Ƀ��b�Z�[�W�͕\�����Ȃ�
	if( bCancel ) return ipFC;
	if( popupMsgTOCHide && !ipFC )
	{
		CString strMsg;
		// Msg TOC��%s���Ȃ����ߊ֘A�t�����ł��܂���B
		strMsg.Format(AheLoadString( IDS_NOROADNODECLASS ), fcName);
		AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
	}
	return ipFC;
}

IFeatureClassPtr CAttrWalkNodeDlg::GetSameGroupFeatureClass( LPCTSTR targetName, const IFeaturePtr& ipSameGroupFeature, bool bMsgDisp, bool& bCancel, bool popupMsgTOCHide/*=true*/ )
{
	bCancel = false; //!< ���������[�U�[�̓��͂ɂ���Ē��f���ꂽ��true�ɂ���

	bool bSameGroup = false; //!< �����O���[�v���ɑ��݂��邩�ۂ�

	// �����̋N�_�Ƃ���t�B�[�`���N���X�̃��[�N�X�y�[�X�Ȃǂ��擾
	IFeatureClassPtr ipGroupClass = AheGetFeatureClass( ipSameGroupFeature );
	IWorkspacePtr ipWorkspace = AheGetWorkspace( ipGroupClass );
	CString strOwnerName = AheGetFeatureClassOwnerName( ipGroupClass );
	CString strSameGroupFCName = AheGetFeatureClassName( ipSameGroupFeature );

	// TOC���猟���̋N�_�Ƃ��郌�C�����擾
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipSameGroupLayer = cMap.FindLayer( ipWorkspace, strOwnerName, strSameGroupFCName );
	if( !ipSameGroupLayer )
		return nullptr;

	// �����̋N�_�ƂȂ郌�C�������[����Ă���e�̃O���[�v���C�����擾����
	ICompositeLayerPtr ipWalkCompositeLayer = cMap.GetParentCompositeLayer( ipSameGroupLayer );

	std::list<CAdapt<ILayerPtr>> targetLayers; //!< �O���[�v�����猟�����ꂽ�Ώۃt�B�[�`���N���X���i�[

	// �N�_���O���[�v���C���ɓ����Ă��Ȃ��ꍇ�ɂ́ATOC�̏ォ��T�����čŏ��Ɍ�������
	// ���C�����擾�ΏۂƂ���
	if( !ipWalkCompositeLayer )
	{
		targetLayers = std::move(cMap.FindLayers( nullptr, nullptr, targetName ));
	}
	// �N�_���O���[�v���C���̒��ɂ���΁A���̃O���[�v���C�����ɂ���Ώۃt�B�[�`�����擾�Ώ�
	else
	{
		long lLayerCount = 0;
		ipWalkCompositeLayer->get_Count( &lLayerCount );
		for( int i = 0; i < lLayerCount; ++i )
		{
			ILayerPtr ipCheckLayer;
			ipWalkCompositeLayer->get_Layer( i, &ipCheckLayer );
			CString strTargetFC = AheGetFeatureClassName( ipCheckLayer );
			if( 0 == strTargetFC.CompareNoCase( targetName ) )
				targetLayers.push_back(ipCheckLayer);
		}

		// �O���[�v���ɑΏۃt�B�[�`����������Ȃ�������TOC�ォ�猟������
		if( targetLayers.empty() )
			targetLayers = std::move(cMap.FindLayers( nullptr, nullptr, targetName ));
		// �O���[�v������擾�ł��Ă�����t���O�𗧂ĂĂ���
		else
			bSameGroup = true;
	}

	// TOC��ɑΏۃt�B�[�`����������Ȃ�������I��
	if( targetLayers.empty() )
		return nullptr;

	ILayerPtr ipTargetLayer; //!< �ŏI�I�ɕԂ����C�����i�[

	// �Ώۂ̓��A�\������Ă��郌�C���̐����J�E���g
	int visibleLayerCount = 0;
	bool bInitVisible = true; //!< �\������Ă���t�B�[�`������ł����������ۂ��̃t���O
	for( auto& elem : targetLayers )
	{
		VARIANT_BOOL bVisible = VARIANT_FALSE;
		elem->get_Visible(&bVisible);
		if(bVisible)
		{
			++visibleLayerCount;
			if(bInitVisible)
			{
				ipTargetLayer = elem.m_T;
				bInitVisible = false;
			}
		}
	}

	bool bMessage = false; //!< ���b�Z�[�W�\���t���O
	// �\�����C���̐��ɂ���ď�����ς���
	switch(visibleLayerCount)
	{
	case 0: // ��\�����C���������݂��Ȃ�
		// [bug 11557][e][SJ���s��]���s�҃m�[�h�\�����Ɍx�����o���Ȃ��łق���
		if(bMsgDisp && popupMsgTOCHide )
		{
			CString strMsg;

			// �O���[�v���̃��C�����ۂ��Ŏ኱���b�Z�[�W��ύX
			// ...�d�����镔�����������A�p�ꉻ���l����ƕ����͂��Ȃ������u�����͊y
			if(bSameGroup)
				// Msg �O���[�v����%s���S�Ĕ�\���ł��B\n��\�����C����ID�̊֘A�t�����s���܂����H
				strMsg.Format(AheLoadString(IDS_INVISIBLEGROUP), targetName);
			else
				// Msg TOC���%s���S�Ĕ�\���ł��B\n��\�����C����ID�̊֘A�t�����s���܂����H
				strMsg.Format(AheLoadString(IDS_INVISIBLETOC), targetName);

			// No���I�����ꂽ�珈���𒆒f
			if(IDNO==AttrMessageBox(strMsg, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL))
			{
				bCancel = true;
				return nullptr;
			}
		}
		// ��\���̓��A�ŏ��Ɍ������ꂽ���̂�ΏۂƂ���
		ipTargetLayer = targetLayers.begin()->m_T;
		// ��₪�����������Ȃ�A���b�Z�[�W���o��
		if(targetLayers.size()>1)
			bMessage = true;
		break;
	case 1: // �\�����C������Ȃ�ipTargetLayer�Ɋi�[����Ă�����̂����̂܂ܑΏۂƂ���
		break;
	default: // �\�����C���������Ȃ烁�b�Z�[�W���o��
		bMessage = true;
		break;
	}

	// ��₪�������������ꍇ�̓��b�Z�[�W���o��
	if( bMessage && bMsgDisp )
	{
		CString strMsg;

		// �O���[�v���̃��C�����ۂ��Ŏ኱���b�Z�[�W��ύX
		if(bSameGroup)
			// Msg �O���[�v���ɕ���(%d��)��%s�������������߁A��ԏ��߂Ɍ����������L���֘A�t���̑ΏۂƂ��܂��B
			strMsg.Format(AheLoadString(IDS_FINDPLURALINSAMEGROUP), targetLayers.size(), targetName);
		else
			// Msg ����(%d��)��%s�������������߁A��ԏ��߂Ɍ����������L���֘A�t���̑ΏۂƂ��܂��B
			strMsg.Format(AheLoadString(IDS_FINDPLURALTOC), targetLayers.size(), targetName);

		// �ȉ��A�I�[�i�[��.�t�B�[�`���N���X�����\��
		strMsg.AppendFormat(_T("\n"));
		CString ownerName = AheGetFeatureClassOwnerName(ipTargetLayer);
		if(!ownerName.IsEmpty())
			strMsg.AppendFormat(_T("%s."), ownerName);
		strMsg.AppendFormat(_T("%s"), targetName);

		AttrMessageBox(strMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
	}
	return AheGetFeatureClass(ipTargetLayer);
}

void CAttrWalkNodeDlg::SetIDName(LPCTSTR fcName, INT nCodeID, INT nLabelID )
{
	// ���̏����ł�TOC��\���̃G���[���b�Z�[�W���o�͂��Ȃ�
	IFeatureClassPtr ipFC = GetFeatureClass(fcName, false );
	if( !ipFC ) return;

	// ��������R�[�h���G�f�B�b�g�{�b�N�X����擾
	CString strID;
	GetDlgItem( nCodeID ).GetWindowText( strID );
	// �������͂���Ă��Ȃ��Ȃ�󕶎�����Z�b�g
	if( strID.IsEmpty() )
	{
		GetDlgItem(nLabelID).SetWindowText( _T("") );
		return;
	}

	IFeaturePtr ipFeature;
	ipFC->GetFeature( _ttol( strID ), &ipFeature );
	// ����OID�����t�B�[�`�������݂��Ȃ��Ȃ�"���݂��Ȃ�OID"���Z�b�g
	if( !ipFeature )
	{
		// Msg ���݂��Ȃ�OID
		GetDlgItem(nLabelID).SetWindowText( AheLoadString(IDS_NONEXISTID) );
		return;
	}

	// ���̂Ɠǂ݂��擾
	LONG lNameIndex = -1, lYomiIndex = -1;
	IFieldsPtr ipFields;
	ipFeature->get_Fields( &ipFields );
	if(0==CString(fcName).CompareNoCase(road_node::kTableName))
	{
		ipFields->FindField( CComBSTR(road_node::kNameKanji), &lNameIndex );
		ipFields->FindField( CComBSTR(road_node::kNameYomi), &lYomiIndex );
	}
	else if(0==CString(fcName).CompareNoCase(station_gate_point::kTableName))
	{
		ipFields->FindField( CComBSTR(station_gate_point::kName), &lNameIndex );
		ipFields->FindField( CComBSTR(station_gate_point::kYomi), &lYomiIndex );
	}
	else if(0==CString(fcName).CompareNoCase(sindyk::poi_point_org::kTableName))
	{
		ipFields->FindField( CComBSTR(sindyk::poi_point_org::kName), &lNameIndex );
		ipFields->FindField( CComBSTR(sindyk::poi_point_org::kYomi), &lYomiIndex );

		// POI_POINT_ORG�̏ꍇ�R���e���c�R�[�h���n���X�o�����|�C���g�łȂ��ꍇ
		// "�n���X�o�����|�C���g�ȊO��OID"���Z�b�g
		CComVariant varContentsCode = AheGetAttribute(ipFeature, sindyk::poi_point_org::kContentsCode);
		if( UNDERGROUND_GATE_CONTENTS_CODE != varContentsCode.lVal )
		{
			GetDlgItem(nLabelID).SetWindowText( AheLoadString(IDS_NO_UNDERGROUNDGATE_CONTENTS) );
			return;
		}
	}

	// ���̂��擾����
	CComVariant varKanjiName = AheGetAttribute(ipFeature, lNameIndex);
	CComVariant varKanjiYomi = AheGetAttribute(ipFeature, lYomiIndex);
	// �\���p�ɖ��̂Ɠǂ݂��擾
	CString strKnajiName = gf::VariantToCString( varKanjiName, NULL_STRING );
	CString strKanjiYomi = gf::VariantToCString( varKanjiYomi, NULL_STRING );

	// �������ʂ̕�����𖼏̃t�B�[���h�ɏo��
	GetDlgItem(nLabelID).SetWindowText(
		uh::str_util::format(_T("%s(%s)"), strKnajiName, strKanjiYomi) );
}

void CAttrWalkNodeDlg::InsertToEditBox(int nCurIdx)
{
	IFeatureClassPtr& ipFC = m_searchedLayer[GetCurrentFeatureDef()->GetFeature()];
	// nullptr=���X�g�r���[�ɉ����\������Ă��Ȃ���ԂȂ̂ŁA�������Ȃ�
	if(!ipFC) return;

	// ipFC���擾�ł����Ȃ�t�B�[�`���N���X�����擾
	CString fcName = AheGetFeatureClassName(ipFC);

	// ���X�g�r���[�ŕ\�����Ă���t�B�[�`���N���X�ɂ���ďo�͐��ύX
	INT nEditID = 0; //!< ID�̏o�͐�ƂȂ�G�f�B�b�g�{�b�N�X��CtrlID
	INT nNameID = 0; //!< ���̂̏o�͐�ƂȂ�X�^�e�B�b�N�e�L�X�g��CtrlID
	// STATION_GATE_POINT���H
	if(0==fcName.CompareNoCase(station_gate_point::kTableName))
	{
		nEditID = IDC_EDIT_STATIONGATE;
		nNameID = IDC_STATIC_STATIONGATE_NAME;
	}
	// [bug 11559][e][SJ���s��]���s�҃m�[�h�ƒn���X�o����POI�̕R�Â����o����悤�ɂ��Ăق���
	// UNDERGROUND_GATE_POINT���H
	else if(0==fcName.CompareNoCase( sindyk::poi_point_org::kTableName ))
	{
		nEditID = IDC_EDIT_UNDERGROUNDGATE;
		nNameID = IDC_STATIC_UNDERGROUNDGATE_NAME;
	}
	// ROAD_NODE���H
	else if(0==fcName.CompareNoCase(road_node::kTableName))
	{
		nEditID = IDC_EDIT_ROADNODEID;
		nNameID = IDC_STATIC_CROSSINGNAME2;
	}
	else // ����ȊO�̏ꍇ�͑Ή����Ă��Ȃ��̂ŉ������Ȃ�
	{
		// TODO:�����ɗ���Ƃ������Ƃ́A�i�����_�ł̎d�l����l����Ɓj��������̉��C�r��ł���Ƃ�������
		// ���C���e�ɍ��킹�ď�L��������K�؂ɕύX����K�v������
		// ���C������ł���Ȃ�A�����ɓ��邱�Ƃ��Ȃ��̂ŁA�c�[���g�p�җp�̃G���[���͏o�͂��Ȃ�
		// (�]�v�ȉp�ꉻ�Ώۂ𑝂₵�����Ȃ�)
		ASSERT(false);
		return;
	}

	// ���łɊ֘A�t�����Ă���m�[�hID���擾
	CString nowID;
	GetDlgItem( nEditID ).GetWindowText( nowID );

	// ���X�g����ID���擾
	CString strID;
	CListViewCtrl lvResult( GetDlgItem(IDC_LIST_RESULT) );
	lvResult.GetItemText( nCurIdx, 0, strID );

	// ���łɊ֘A�t�����Ă���ID�ɑ΂��� ���͂��ꂽ�Ȃ牽�����Ȃ�
	// ���̏��������Ȃ��ƒl���ς���Ă��Ȃ��̂ɑ����ύX�������ɂȂ�
	if( 0==strID.Compare(nowID) ) return;

	// ���̂��擾
	CString strName;
	lvResult.GetItemText( nCurIdx, 1, strName );

	// �G�f�B�b�g�{�b�N�X�ƃX�^�e�B�b�N�e�L�X�g�ɏ�������
	SetDlgItemText( nEditID, strID );
	SetDlgItemText( nNameID, strName );

	// �s���ȓ��̓f�[�^������΃G���[�t���O�𗧂Ă�
	CheckInvaliedValue();
}

void CAttrWalkNodeDlg::ReplaceListViewNameColumn(UINT colNameID, UINT resultID)
{
	// �J�����񖼂�؂�ς���
	// �Q�l�Fhttps://msdn.microsoft.com/ja-jp/library/b11bety8.aspx
	// pszText�ɂ́ASetColumn���Ăяo���܂Ő������Ă��镶�����n���K�v������
	// �Ⴆ�΁Aconst_cast<LPTSTR>(AheLoadString(IDC_HOGE));�̂悤�Ȃ��͓̂n���Ȃ��̂Œ���
	LVCOLUMN nameColumn = m_cResultColInfo[1];
	nameColumn.mask = LVCF_TEXT; // �e�L�X�g�̏�񂾂��ύX����̂Ń}�X�N��������
	CString strColumnName = AheLoadString(colNameID);
	nameColumn.pszText = const_cast<LPTSTR>(strColumnName.GetString());

	// GetColumn�̂Ƃ���cchTextMax�����o�ϐ���ݒ肷��K�v�����邪�ASetColumn�̂Ƃ��͖��������
	// (pszText��null�ŏI��镶����(������null-terminated string)�ł��邱�Ƃ��O��)
	CListViewCtrl( GetDlgItem(IDC_LIST_RESULT) ).SetColumn(1, &nameColumn);

	// �������ʕ\�����؂�ւ���
	SetDlgItemText(IDC_SEARCH_RESULT_LABEL, AheLoadString(resultID));
}

void CAttrWalkNodeDlg::SearchAroundPoint(LPCTSTR fcName, LPCTSTR nameFieldName, LPCTSTR yomiFieldName, LPCTSTR whereClause/*=NULL*/ )
{
	// �G���[���b�Z�[�W�͊֐����ŏo��
	IFeatureClassPtr ipFC = GetFeatureClass(fcName);
	if( !ipFC ) return;

	// �G�f�B�b�g�{�b�N�X���猟���͈͂��擾
	CString rangeValue;
	GetDlgItem( IDC_EDIT_SEARCH ).GetWindowText(rangeValue);

	long meter = 0;
	if(!rangeValue.IsEmpty()) // ��łȂ��Ȃ琔�l�ɕϊ�
		meter = _ttol( rangeValue );
	if(meter < 0) meter = 0;

	// ���� meter ���[�g���ȓ��̃t�B�[�`��������
	IFeatureCursorPtr ipCur = gf::AroundSearch( (IFeaturePtr)(GetCurrentFeatureDef()->GetFeature()), ipFC, meter, whereClause );

	// �l���擾����t�B�[���h�̃C���f�b�N�X�ԍ����擾���Ă���
	IFieldsPtr ipFields;
	ipFC->get_Fields( &ipFields );
	LONG lKanjiNameIndex = -1, lKanjiYomiIndex = -1;
	ipFields->FindField( CComBSTR(nameFieldName), &lKanjiNameIndex );
	ipFields->FindField( CComBSTR(yomiFieldName), &lKanjiYomiIndex );

	// �����v�Z�p�ɁA�ҏW���̃t�B�[�`���̌`����擾
	IGeometryPtr ipCurrentGeom;
	((IFeaturePtr)(GetCurrentFeatureDef()->GetFeature()))->get_ShapeCopy(&ipCurrentGeom);
	IPointPtr ipCurrentPoint(ipCurrentGeom);

	// �\�[�g�p��multimap��p��
	// �������ŕ��בւ��邽�߁Adouble��key�Ƃ��� <����,<�I�u�W�F�N�gID,����(�ǂ�)>>
	// �������̂��̂ɂ��Ă͌����������ɂȂ�
	std::multimap<double,std::pair<long,CString>> sortMap;
	IFeaturePtr ipFeature;
	while( S_OK == ipCur->NextFeature(&ipFeature) && ipFeature )
	{
		// ���̂��擾
		CComVariant varKanji = AheGetAttribute(ipFeature, lKanjiNameIndex);
		// VARIANT�^��CString�ɕϊ�
		CString strKanjiName = gf::VariantToCString( varKanji, NULL_STRING );

		// ���̂��擾�ł��Ȃ����́E���̂���̂��̂͑Ή��t���̑Ώۂɂ͂Ȃ蓾�Ȃ��̂Ń��X�g�ɒǉ����Ȃ�
		// (�w�o�������NULL�s�������A�����_�m�[�h�ɂ͕K���������̂����݂��Ȃ��̂ł����Œe��)
		if( 0==strKanjiName.CompareNoCase(NULL_STRING) || strKanjiName.IsEmpty() )
			continue;

		// �`����擾���A�Ώۃt�B�[�`������̋������擾����
		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy(&ipGeom);
		IPointPtr ipPoint(ipGeom);

		// �ҏW���̃t�B�[�`������̋������擾(�������Ƀ\�[�g���邽�߂Ɏg�p)
		double dRange = AheGetMeterLength(ipPoint, ipCurrentPoint);
		// TODO: gf::AroundSearch()���C�����ꂽ�ꍇ�A���킹�ďC������
		// ����A�������ʂɂ͎w��͈͊O�̃t�B�[�`�����܂܂�Ă��܂����߁A�����Ŏw��͈͊O�̂��̂͏��O����
		if( dRange > meter ) continue;

		// �ǂ݂��擾
		CComVariant varYomi = AheGetAttribute(ipFeature, lKanjiYomiIndex);
		// ���X�g��ɕ\������ǂ݂��擾
		CString strYomiName = gf::VariantToCString( varYomi, NULL_STRING );
		// ���X�g�\���p�ɖ��́E�ǂ݂𐮌`
		CString strListYomi = uh::str_util::format(_T("%s(%s)"), strKanjiName, strYomiName);
		// �}���`�}�b�v�֑}��
		sortMap.insert( std::make_pair(dRange, std::make_pair(AheGetOID( ipFeature ), strListYomi)));
	}

	// �}�b�v�Ɋi�[�������̂����X�g�r���[�Ɋi�[
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	int count = 0; //!< ���X�g�r���[�̍s�����L�^(���X�g�r���[�̖����ɒǉ�����̂ɕK�v)
	lv.DeleteAllItems();
	for(const auto& elem : sortMap)
	{
		// ID��ɂ̓I�u�W�F�N�gID��\��
		lv.AddItem( count, 0, uh::str_util::ToString(elem.second.first) );
		// ���O��ɂ�{��������(�ǂ�)}�̂悤�ɕ\������
		lv.AddItem( count, 1, elem.second.second );
		++count;
	}

	// List��ɕ\������Ă���A�C�e�����ǂ��̃t�B�[�`���[�N���X�̂��̂Ȃ̂���ێ����Ă���
	m_searchedLayer[GetCurrentFeatureDef()->GetFeature()] = ipFC;

	if(!sortMap.empty())
	{
		// ��Ԏn�߂Ɍ����������̂̓G�f�B�b�g�{�b�N�X�ɓ��͂���
		InsertToEditBox(0);
	}
}

void CAttrWalkNodeDlg::BlinkPointFeature()
{
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	// �I�𒆂̃��R�[�h�̃I�u�W�F�N�gID���擾
	int nCurIdx = lv.GetSelectedIndex();
	if( -1 == nCurIdx ) return;

	// �����{�^���������ꂽ�Ƃ��ɎQ�Ƃ����t�B�[�`���N���X�̎Q�Ƃ�ێ�
	const IFeatureClassPtr& ipSearchedFC = m_searchedLayer[GetCurrentFeatureDef()->GetFeature()];

	// �����{�^���������ꂽ�Ƃ��ɎQ�Ƃ������̂�TOC����擾�ł������ǂ����𔻒�
	CString targetFCName = AheGetFeatureClassName(ipSearchedFC);

	// �G���[���b�Z�[�W�͊֐����ŏo��
	IFeatureClassPtr ipNowTargetFC = GetFeatureClass(targetFCName);
	if( !ipNowTargetFC ) return;

	if(ipSearchedFC!=ipNowTargetFC)
	{
		// Msg TOC�̍\�����ύX����Ă��܂��B��������蒼���Ă��������B
		AttrMessageBox( AheLoadString(IDS_ONEMORESEARCH), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	CString strID;
	lv.GetItemText( nCurIdx, 0, strID );

	// �N���b�N���ꂽ�t�B�[�`����_�ł�����
	gf::BlinkFeature( m_cArcHelper.GetScreenDisplay(), _ttol(strID), ipSearchedFC, 80, 3 );
}

void CAttrWalkNodeDlg::CreateListView(LPCTSTR nameColName, LPCTSTR resultStr)
{
	// ���X�g�r���[�̃J��������X�^�e�B�b�N�ȕ��������ʂɍ��킹�ĕύX
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	for( int i=0; i<m_ListViewColumnSize; ++i )
		lv.InsertColumn( i, &m_cResultColInfo[i] );
	lv.SetExtendedListViewStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	// LVCOLUMN��LPTSTR���������o�Ɏ��ĂȂ����߁ALPCTSTR��n���ꍇ�ɂ�LPTSTR��cast����K�v������
	// ListView_SetColumn���ł͕ύX����Ȃ����Ƃ����炩�Ȃ̂ŁAconst_cast����LPTSTR�^�ɕϊ����Ă���
	LVCOLUMN column = {LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 120, const_cast<LPTSTR>(nameColName), 0, 0, 0, 0 };
	ListView_SetColumn(GetDlgItem(IDC_LIST_RESULT), 1, &column);
	SetDlgItemText(IDC_SEARCH_RESULT_LABEL, resultStr);
}

void CAttrWalkNodeDlg::CheckInvaliedValue()
{
	// �I������Ă���m�[�h��ʂ��擾
	CComboBox combo( GetDlgItem(IDC_COMBO_WALKNODECLASS) );
	DWORD_PTR nodeClass = combo.GetItemData( combo.GetCurSel() );

	// [bug 5954]�m�[�h��ʂ��u�w�o�����m�[�h�v����ύX�ƂȂ����ꍇ
	// ...����啪�ς��܂���

	// �ێ����Ă���G���[����x���Z�b�g����
	m_errors[GetCurrentFeatureDef()->GetFeature()].Reset();

	// �w�o������Ή�ID�R���g���[���Ɋւ���`�F�b�N
	CheckStationGateCtrl(nodeClass);

	// [bug 11559][e][SJ���s��]���s�҃m�[�h�ƒn���X�o����POI�̕R�Â����o����悤�ɂ��Ăق���
	// �n���X�o�����Ή�ID�R���g���[���Ɋւ���`�F�b�N
	CheckUndergroundGateCtrl(nodeClass);

	// ���H�m�[�h�Ή�ID�R���g���[���Ɋւ���`�F�b�N
	CheckCrossNodeCtrl(nodeClass);
}

void CAttrWalkNodeDlg::CheckStationGateCtrl( DWORD_PTR nodeClass )
{
	CheckCommonGateCtrl( nodeClass, IDC_EDIT_STATIONGATE );
}

void CAttrWalkNodeDlg::CheckUndergroundGateCtrl( DWORD_PTR nodeClass )
{
	CheckCommonGateCtrl( nodeClass, IDC_EDIT_UNDERGROUNDGATE );
}

void CAttrWalkNodeDlg::CheckCommonGateCtrl( DWORD_PTR nodeClass, INT nID )
{
	using namespace sindy_error::walk_node;

	// �eID���̐ݒ�
	int checkNodeClass = 0;
	ErrorCode noneInputError = ErrorCode::STATIONGATE_NONE_INPUT_ERROR;
	ErrorCode inputError = ErrorCode::STATIONGATE_INPUT_ERROR;
	switch (nID)
	{
	case IDC_EDIT_STATIONGATE:
		checkNodeClass = walk_node::node_class::kGate;
		noneInputError = ErrorCode::STATIONGATE_NONE_INPUT_ERROR;
		inputError=ErrorCode::STATIONGATE_INPUT_ERROR;
		break;
	case IDC_EDIT_UNDERGROUNDGATE:
		checkNodeClass = walk_node::node_class::kUndergroundGate;
		noneInputError = ErrorCode::UNDERGROUNDGATE_NONE_INPUT_ERROR;
		inputError=ErrorCode::UNDERGROUNDGATE_INPUT_ERROR;
		break;
	default:
		// CheckCtrl�n�ȊO�Ŏg�p���ꂸ�AIDC_EDIT_STATIONGATE/IDC_EDIT_UNDERGROUNDGATE�ȊO��
		// �g�p����Ȃ��̂ŁA���̂܂�return
		return;
	}

	// �`�F�b�N����
	CControlDef* pTargetCtrl = m_cControlRel.GetControlDef( GetDlgItem(nID) );
	CComVariant vaValue;
	pTargetCtrl->GetControlValue( vaValue );
	if( checkNodeClass == nodeClass )
	{
		// ID�������Ȃ��Ƃ����Ȃ��̂ɓ��͂��Ȃ���΃G���[
		pTargetCtrl->m_bErrored = ( ( vaValue.vt == VT_NULL ) ) ? TRUE : FALSE;
		if(pTargetCtrl->m_bErrored)
		{
			// �G���[�Ȃ�G���[�R�[�h���v�b�V��
			m_errors[GetCurrentFeatureDef()->GetFeature()].Push(noneInputError);
		}
		return;
	}

	// ID�������Ă͂����Ȃ��̂ɓ��͂�����΃G���[
	pTargetCtrl->m_bErrored = ( ( vaValue.vt != VT_NULL ) ) ? TRUE : FALSE;
	if(pTargetCtrl->m_bErrored)
	{
		// �G���[�Ȃ�G���[�R�[�h���v�b�V��
		m_errors[GetCurrentFeatureDef()->GetFeature()].Push(inputError);
	}
}

void CAttrWalkNodeDlg::CheckCrossNodeCtrl(DWORD_PTR nodeClass)
{
	using namespace sindy_error::walk_node;

	CControlDef* pTargetCtrl = m_cControlRel.GetControlDef( GetDlgItem(IDC_EDIT_ROADNODEID) );
	CComVariant vaValue;
	pTargetCtrl->GetControlValue( vaValue );
	// �����ʂ̏ꍇ�͓��͂������Ȃ��̂ŃG���[
	switch(nodeClass)
	{
	case walk_node::node_class::kOutline:
		// ID�������Ă͂����Ȃ��̂ɓ��͂�����΃G���[
		pTargetCtrl->m_bErrored = ( ( vaValue.vt != VT_NULL ) ) ? TRUE : FALSE;
		if(pTargetCtrl->m_bErrored)
		{
			// �G���[�Ȃ�G���[�R�[�h���v�b�V��
			m_errors[GetCurrentFeatureDef()->GetFeature()].Push(ErrorCode::ROADNODEID_INPUT_ERROR);
		}
		break;
	default: // ���̑��̎�ʂł̓G���[�ɂ͂��Ȃ�
		pTargetCtrl->m_bErrored = FALSE;
		break;
	}
}

bool CAttrWalkNodeDlg::CheckExistField( LPCTSTR fcName, LPCTSTR fieldName )
{
	IFeatureClassPtr ipFC = GetFeatureClass( fcName, false );
	long fieldIndex = -1;
	ipFC->FindField( CComBSTR(walk_node::kUndergroundGatePointID), &fieldIndex);
	if( fieldIndex == -1 )
		return false;
	return true;
}
