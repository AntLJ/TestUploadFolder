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

// AttrLaneLinkDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrLaneLinkDlg.h"
#include <boost/assign/list_of.hpp>
#include "LaneMap.h"	// �Ԑ������N�_�C�A���O�̃E�B���h�E�z�u�}�b�v
#include <TDC/useful_headers/registry_util.h>

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace sindy::schema::category;

const CString CAttrLaneLinkDlg::UNDEF_NAME = _T("���ݒ�");
const long CAttrLaneLinkDlg::UNDEF_OID = 0;
const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLaneLink =REGPATH_SINDY_E_CHILDDLG  _T("LANE_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");


/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrLaneLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrLaneLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrLaneLinkDlg::CAttrLaneLinkDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrLaneLinkDlg::CAttrLaneLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_LANELINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// [SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_FROM_NODE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_FROM_NODE_ID ) )
		( IDC_EDIT_TO_NODE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_TO_NODE_ID ) )
		( IDC_EDIT_LANE_GROUP_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_GROUP_ID ) )
		( IDC_EDIT_LANE_GROUP_C, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_GROUP_C ) )
		( IDC_COMBO_ROAD_STRUCTURE_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ROAD_STRUCTURE_C ) )
		( IDC_EDIT_LANE_NUMBER, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_NUMBER ) )
		( IDC_EDIT_LANE_COUNT, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_COUNT ) )
		( IDC_COMBO_MAX_LEGAL_SPEED_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MAX_LEGAL_SPEED_C ) )
		( IDC_COMBO_MIN_LEGAL_SPEED_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MIN_LEGAL_SPEED_C ) )
		( IDC_COMBO_TRAVEL_DIRECTION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_TRAVEL_DIRECTION_C ) )
		( IDC_CHECK_LEFT_CHANGE_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_LEFT_CHANGE_F ) )
		( IDC_CHECK_RIGHT_CHANGE_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RIGHT_CHANGE_F ) )
		( IDC_CHECK_LEFT_PROTRUSION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_LEFT_PROTRUSION_F ) )
		( IDC_CHECK_RIGHT_PROTRUSION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RIGHT_PROTRUSION_F ) )
		( IDC_CHECK_RUBBING_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RUBBING_F ) )
		( IDC_CHECK_TUNNEL_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL_F ) )
		( IDC_EDIT_ROAD_LINK_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ROAD_LINK_ID ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) )

		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ))
		( IDC_TAB_LANEATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_LANEATTR ));

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
 * @fn HWND CAttrLaneLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrLaneLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrLaneLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrLaneLinkDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrLaneLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrLaneLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_iChangeType, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// [SiNDYChildDlg] �ŏ��Ƀt�H�[�J�X�����R���g���[��
		// ���́E�ǂ݂�NULL�Ȃ̂ŁA�Ԑ��ԍ����ŏ��ɂ��Ă���
		//::SetFocus( GetDlgItem(IDC_EDIT_LANE_NUMBER) );
	}

	UpdateLaneGroup();
	GetRoadLinkID();
	UpdateUnDefOID();
	createGrid();

	// �O���b�h�̕ҏW�\�t�B�[���h��ݒ�
	readReg();	

	return bRet;
}

/**
 * @fn void CAttrLaneLinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrLaneLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// [SiNDYChildDlg] ControlRel�Ƀt�B�[���h���ƃe�[�u������o�^
		m_cControlRel.SetControl( IDC_TAB_LANEATTR,		TYPE_OTHER,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_NODE );
		m_cControlRel.SetControl( IDC_EDIT_FROM_NODE_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kFromNodeID,			0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_EDIT_TO_NODE_ID,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kToNodeID,			0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_EDIT_LANE_GROUP_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneGroupID,		0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_EDIT_LANE_GROUP_C,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	0,								0,	0,					   0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_COMBO_ROAD_STRUCTURE_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRoadStructureC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_LANE_NUMBER,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneNumber,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_LANE_COUNT,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneCount,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_MAX_LEGAL_SPEED_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kMaxLegalSpeedC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_MIN_LEGAL_SPEED_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kMinLegalSpeedC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_TRAVEL_DIRECTION_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kTravelDirectionC,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_LEFT_CHANGE_F,		TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLeftChangeF,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RIGHT_CHANGE_F,		TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRightChangeF,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_LEFT_PROTRUSION_F,  TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLeftProtrusionF,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RIGHT_PROTRUSION_F,	TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRightProtrusionF,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RUBBING_F,			TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRubbingF,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL_F,			TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kTunnelF,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_ROAD_LINK_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	0,								0,	0,					   0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kSperculationC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,			TYPE_EDITSTRING,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kSourceID,			0,  lane_link::kTableName,  0, -1, FOLLOW_READONLY); //�\���݂̂̂��ߏ�ɓǂݎ���p
		
		m_cControlRel.SetControl( IDC_STATIC_FROM_NODE_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_TO_NODE_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_GROUP_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_GROUP_C,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_ROAD_STRUCTURE_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_NUMBER,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_COUNT,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_MAX_LEGAL_SPEED_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_MIN_LEGAL_SPEED_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_TRAVEL_DIRECTION_C,	TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_CHANGE_F,			    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_PROTRUSION_F,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_ROAD_LINK_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,			    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_BASE_GROUP,		    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_EXTENSION_GROUP,	    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_VAR,			        TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
	
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_LANE_NODE, EDITVIEW_LANE_NODE);
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_LANE_NODE, EDITVIEW_LANE_NODE);
	
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_NODE,	lane_link::kSource,				0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);
		
	}
}

/**
 * @fn void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
 * @brief �c�[���`�b�v���쐬���܂��B
 *
 * @note �쐬����c�[���`�b�v�͑����ɕύX��������Ȃ����߁A�����ςȂ��Ŗ��Ȃ�
 *
 * @param nTargetItem	[in]	�c�[���`�b�v���쐬����R���g���[���A�C�e����ID
 * 
 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg ���Q�l
*/
void CAttrLaneLinkDlg::SetToolTip(INT nTargetItem)
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
 * �G���[�`�F�b�N
 * @TODO	
 *			
 */
BOOL CAttrLaneLinkDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == lane_link::kTableName )
		{
			//�G���[����
		}
	}

	return TRUE;
}

void CAttrLaneLinkDlg::UpdateLaneGroup()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//�Z�O�����g�����擾
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, lane_link_group::kTableName);
	if(!ipGroupTable)
	{
		return;
	}

	//�Ԑ��O���[�vID����R�[�h�l�擾
	long lane_group_oid = AheGetAttribute(ipFeature, lane_link::kLaneGroupID).lVal;
	CString where_clause;
	where_clause.Format(_T("%s=%d"), lane_link_group::kObjectID, lane_group_oid);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipGroupTable->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	long lane_group = 1;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		lane_group = AheGetAttribute(ipRow, lane_link_group::kGroupC).lVal;
	}
	//�R�[�h�l���疼�̎擾
	if(!SetDomain())
	{
		return;
	}
	CString str;
	if(lane_group_oid == UNDEF_OID)
	{
		str = UNDEF_NAME;
	}
	else
	{
		str = m_laneGroupDomain[lane_group];
	}

	//�R���g���[���X�V
	CEdit(GetDlgItem(IDC_EDIT_LANE_GROUP_C)).SetWindowText(str);
}

void CAttrLaneLinkDlg::GetRoadLinkID()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, rel_road_link_lane_link::kTableName);
	if(!ipGroupTable)
	{
		return;
	}

	//�O���[�o��ID�Ɠ��H�����NGID����v���郌�R�[�h���擾
	CString grobal_id;
	grobal_id.Format(_T("%s"),AheGetAttribute(ipFeature, nq_lane_node::kGlobalID).lVal);
	CString where_clause;
	where_clause.Format(_T("%s='%s'"), rel_road_link_lane_link::kLaneLinkGID, grobal_id);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipGroupTable->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	CString road_link_oid;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		road_link_oid.Format(_T("%d"),AheGetAttribute(ipRow, rel_road_link_lane_link::kRoadLinkID).lVal);
	}

	//�R���g���[���X�V
	CEdit(GetDlgItem(IDC_EDIT_ROAD_LINK_ID)).SetWindowText(road_link_oid);
}

bool CAttrLaneLinkDlg::SetDomain(bool update/* = false*/)
{
	//�X�V���Ȃ��ꍇ
	if(!update)
	{
		//�ݒ�ς�(��łȂ����OK�Ԃ�)
		if(!m_laneGroupDomain.empty())
		{
			return true;
		}
	}

	m_laneGroupDomain.clear();

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if(!ipFeature)
	{
		return false;
	}

	//�Ԑ��O���[�v�e�[�u���ڑ�
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return false;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, lane_link_group::kTableName);
	if(!ipGroupTable)
	{
		return false;
	}

	//�h���C���l�擾
	IDomainPtr ipDomain;
	long index = -1;
	ipGroupTable->FindField(const_cast<BSTR>(lane_link_group::kGroupC), &index);
	IFieldsPtr ipFields;
	ipGroupTable->get_Fields(&ipFields);
	if(!ipFields)
	{
		return false;
	}
	IFieldPtr ipField;
	ipFields->get_Field(index, &ipField);
	if(!ipField)
	{
		return false;
	}

	ipField->get_Domain(&ipDomain);
	if (!ipDomain)
	{
		return false;
	}
	ICodedValueDomainPtr ipCodeDomain(ipDomain);
	if (!ipCodeDomain)
	{
		return false;
	}

	LONG n = 0;
	ipCodeDomain->get_CodeCount(&n);
	for (long i=0 ; i<n ; i++)
	{
		BSTR name;
		CComVariant val;

		HRESULT hr1 = ipCodeDomain->get_Name (i, &name);
		HRESULT hr2 = ipCodeDomain->get_Value(i, &val);
		if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
		{
			m_laneGroupDomain[val.lVal] = CString(name);
		}
	}

	if(m_laneGroupDomain.empty())
	{
		return false;
	}

	return true;
}

void CAttrLaneLinkDlg::UpdateUnDefOID()
{
	//OID��UNDEF_OID�Ȃ�\�����X�V
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//�Ԑ��O���[�vID
	if(AheGetAttribute(ipFeature, lane_link::kLaneGroupID).lVal == UNDEF_OID)
	{
		//�R���g���[���X�V
		CEdit(GetDlgItem(IDC_EDIT_LANE_GROUP_C)).SetWindowText(UNDEF_NAME);
	}
}

// �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
void CAttrLaneLinkDlg::ChangeWindowMgrMap( int iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_LANE_KIHON:		// ��{�����^�u
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_LANE_NODE:		// �m�[�h�񑮐��^�u
		m_winMgr.ChangeMgrMap( NODEMAP );
		break;	
	default:
		break;
	}
	// �Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}

// �O���b�h�𐶐�����
void CAttrLaneLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_lane_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// �n�߂ă_�C�A���O�ɕ\�������t�B�[�`���ł���΁A�m�[�h��O���b�h��o�^
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_lane_node::kObjectID)
			.addField(nq_lane_node::kSequence)
			.addField(nq_lane_node::kWidth)
			.addField(nq_lane_node::kLinearElement)
			.addField(nq_lane_node::kCurvature)
			.addField(nq_lane_node::kCurvatureRadius)
			.addField(nq_lane_node::kCurvatureChangeRate)
			.addField(nq_lane_node::kClothoidParameter)
			.addField(nq_lane_node::kLongitudinalSlope)
			.addField(nq_lane_node::kCrossSlope)
			.addField(nq_lane_node::kLaneNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// �O���b�h������
	m_nqDataGridMap[m_currentOid].init();

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	CComVariant laneLinkGid = AheGetAttribute(ipFeature, lane_link::kGlobalID);
	if (laneLinkGid.vt == VT_NULL) return;
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_lane_node::kLaneLinkGID, laneLinkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(laneLinkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrLaneLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// �O���b�h�񂲂Ƃ̕ҏW�ېݒ�̓O���b�h�N���X���ֈϏ�
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

// MouseUp Event on Grid
void CAttrLaneLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_gridSelect = false;
	// �N���b�N
	if ((!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) && Button == 1 )
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
	}
	else if(IsEditableFeature() && m_cArcHelper.IsStartEditing() && (Button == 1 || Button == 2))
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);	
		
		// �E�N���b�N�Ȃ烁�j���[���o��
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqLaneNodeGrid, X, Y );
	}
}

// After Edit
void CAttrLaneLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// �֘A�e�[�u���̍X�V���̍X�V
void CAttrLaneLinkDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

// �E�N���b�N����� <�폜> �|�b�v�A�b�v
void CAttrLaneLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
{
	long selectedRow = ipGrid->GetSelectedRow(0);

	// �J�[�\���ʒu�擾
	POINT stPos = {0,0};
	::GetCursorPos( &stPos);

	CMenu menu;
	menu.LoadMenuW( IDR_MENU );
	CMenu popupMenu = menu.GetSubMenu(0);

	if(selectedRow <= 0) popupMenu.EnableMenuItem(ID_DEL, MF_GRAYED |MF_BYCOMMAND |MF_DISABLED);

	// Popup���j���[�\��
	int nResult = popupMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, stPos.x, stPos.y, m_hWnd, 0);

	if(nResult == ID_DEL && MessageBox(_T("�폜���܂����H"),_T("�m�F"), MB_OKCANCEL) == IDOK)
	{
		CComBSTR bstrValue;
		ipGrid->get_TextMatrix(_variant_t(selectedRow), _variant_t(0L), &bstrValue);
		
		CString whereClause = uh::str_util::format(_T("%s=%s"),
			nq_lane_node::kObjectID, bstrValue);
		CString objectID = bstrValue.m_str;
		
		// �ȉ���if���́A�V�K���R�[�h�̏ꍇ�AOID��RowData�ɓ���ĊǗ����邽�߂̏����ł���
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_lane_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);
		m_nqDataGridMap[m_currentOid].highlightChangedCell();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// �{�^������
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
		pRelTableDef->setChanged(true);

		// �{�^������
		SetButton(true);
	}
}

// Update Grid Data
bool CAttrLaneLinkDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), lane_link::kTableName) != 0) continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged())	continue;

		if (!m_nqDataGridMap[oid].store())	return false;
	}
	return true;
}

// Check Data Changes
bool CAttrLaneLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrLaneLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// �_�C�A���O�ŕ\�����Ă���Ԑ������N�t�B�[�`�����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","�Ԑ������NGID"���擾����
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipLaneNodeT = AheOpenSameOwnerTable(ipTable, lane_node::kTableName);
	if (!ipLaneNodeT) return;

	CString laneNodeGid, laneNodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(laneNodeGid, laneNodeGidNext, nq_lane_node::kSequence, nq_lane_node::kLaneNodeGID);

	//   �������ꂽ�m�[�h��̎Ԑ��m�[�hGID���玟�̃m�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, laneNodeGid);

	_ICursorPtr ipLaneNodeCur;
	if (FAILED(ipLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipLaneNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr ipLaneNodeFeature;
	if (IFeatureCursorPtr(ipLaneNodeCur)->NextFeature(&ipLaneNodeFeature) != S_OK) return;
	if (!ipLaneNodeFeature) return;

	ipLaneNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, laneNodeGidNext);
	if (FAILED(ipLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipLaneNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	if (IFeatureCursorPtr(ipLaneNodeCur)->NextFeature(&ipLaneNodeFeature) != S_OK) return;
	if (!ipLaneNodeFeature) return;

	ipLaneNodeFeature->get_ShapeCopy(&ipToPoint);
}

// �O���b�h�őI�����Ă���m�[�h��̃��C�����n�C���C�g����
void CAttrLaneLinkDlg::highlightLine()
{
	if (m_nqDataGridMap.empty()) return;

	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h�̎��̃m�[�h���擾����
	//   �m�[�h�񂩂玟�̃m�[�h�ɂ����郌�R�[�h������
	IGeometryPtr ipFromPoint, ipToPoint;
	getSequenceGeometry(ipFromPoint, ipToPoint);
	
	if(!ipFromPoint) return;

	ISpatialReferencePtr ipSpatialReference;
	ipFromPoint->get_SpatialReference(&ipSpatialReference);

	IPolylinePtr ipDrawLine(CLSID_Polyline);
	ipDrawLine->putref_SpatialReference(ipSpatialReference);
	ipDrawLine->put_FromPoint(IPointPtr(ipFromPoint));
	ipDrawLine->put_ToPoint(IPointPtr(ipToPoint));

	moveToCenter(ipFromPoint, ipToPoint);

	// ���C���`��i�`�撆�̃��C���͏���)
	AheDrawPolyline(m_cArcHelper.GetDisplay(), ipDrawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);

}

// �ĕ`��C�x���g
LRESULT CAttrLaneLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	highlightLine();
	return 0;
}

// Tear down connection points
LRESULT CAttrLaneLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// �m�[�h��̃W�����v,���S�ֈړ�
void CAttrLaneLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
{
	// ���S�ֈړ�
	IActiveViewPtr ipActiveView = m_cArcHelper.GetActiveView();
	
	if (ipActiveView && m_gridSelect && ipFromPoint) {
		m_gridSelect = false;
		IEnvelopePtr ipEnvelope;
		
		// Point�̍��W
		double fromX(0), fromY(0), toX(0), toY(0);

		IPointPtr(ipFromPoint)->QueryCoords(&fromX, &fromY);
		if(ipToPoint)
		{
			IPointPtr(ipToPoint)->QueryCoords(&toX, &toY);
		}
		else
		{
			toX = fromX;
			toY = fromY;
		}

		ISpatialReferencePtr ipSpatialRefer;
		ipFromPoint->get_SpatialReference(&ipSpatialRefer);

		IPointPtr ipPoint( CLSID_Point );
		ipPoint->putref_SpatialReference(ipSpatialRefer);
		ipPoint->PutCoords( (fromX + toX)/2, (fromY + toY)/2 );

		ipActiveView->get_Extent( &ipEnvelope );
		ipEnvelope->CenterAt(ipPoint);
		ipActiveView->put_Extent( ipEnvelope );

		ipActiveView->Refresh();		
	}
}

// ���W�X�g������f�[�^��ǂݍ�
void CAttrLaneLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLaneLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID �t�B�[���h�͎w�肳��Ă��ύX�s��
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_lane_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
