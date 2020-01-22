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

#include "stdafx.h"
#include "AttrWalkLinkDlg.h"
#include "LayoutMap.h"
#include "Map.h"
#include <WinLib/type_convert.h>

using namespace sindy;
using namespace sindy::schema;

/**
 * @brief �R���g���[���w�i�F
 */
namespace ctrl_color{
	const COLORREF Modified = RGB(255,168,126);
}

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrWalkLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrWalkLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrWalkLinkDlg::CAttrWalkLinkDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrWalkLinkDlg::CAttrWalkLinkDlg() 
	: ATTR_BASE_CLASS(WALKLINK_KIHON_MAP, IDD_ATTRWALKLINKDLG)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_WALKCLASS,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_WALKCLASS ) )
		( IDC_EDIT_ROADCODE,      CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ROADCODE ) )
		( IDC_BUTTON_SEARCHROADCODE, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHROADCODE ) )
		( IDC_CHECK_AUTOWALK,     CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_AUTOWALK ) )
		( IDC_CHECK_STAIR,        CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_STAIR ) )
		( IDC_CHECK_SLOPE,        CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SLOPE ) )
		( IDC_CHECK_ESCALATOR,    CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_ESCALATOR ) )
		( IDC_CHECK_ARCADE,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_ARCADE ) )
		( IDC_CHECK_OPEN_AREA,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_OPEN_AREA ) )
		( IDC_CHECK_TUNNEL,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL ) )
		( IDC_CHECK_SCRAMBLE,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SCRAMBLE ) )
		( IDC_EDIT_FLOORLEVEL,    CContainedWindow(  _T("EDIT"), this, IDC_EDIT_FLOORLEVEL ) )
		( IDC_EDIT_PLATFORM,    CContainedWindow(  _T("EDIT"), this, IDC_EDIT_PLATFORM ) )
		( IDC_COMBO_FLOORMOVE,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_FLOORMOVE ) )
		( IDC_COMBO_MATERIAL,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_MATERIAL ) )
		( IDC_BUTTON_SEARCH,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCH ) )
		( IDC_EDIT_SEARCHRANGE,        CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SEARCHRANGE ) )
		( IDC_LIST_ROADSEARCH,        CContainedWindow(  _T("EDIT"), this, IDC_LIST_ROADSEARCH ) )
		( IDC_BUTTON_ROAD_REMOVE,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_ROAD_REMOVE ) )
		( IDC_BUTTON_ROAD_ACCEPT,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_ROAD_ACCEPT ) )
		( IDC_LIST_ROAD,        CContainedWindow(  _T("EDIT"), this, IDC_LIST_ROAD ) )
		( IDC_COMBO_ONEWAY,       CContainedWindow(  _T("COMBO"), this, IDC_COMBO_ONEWAY ) )
		( IDC_COMBO_NOPASSAGE,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_NOPASSAGE ) )
		( IDC_EDIT_SOURCE,        CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_TAB_WALKLINK,       CContainedWindow(  _T("TAB"), this, IDC_TAB_WALKLINK ) );

	// �����o�ϐ�������
	m_lTabNum		= 0;
	m_iChangeType	= 0;
	m_lFeatureIndex = 0;
	m_lTableIndex	= 0;
	m_lRowIndex		= 0;
	m_bForce		= false;
	m_bEditable		= false;
	m_bIsLinkOnMesh = false;
}

/** 
 * @fn HWND CAttrWalkLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrWalkLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrWalkLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrWalkLinkDlg::Delete()
{
	// �n�C���C�g����
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

void CAttrWalkLinkDlg::ClearFeatureDefs()
{
	// �R�t���Ă铹�H��j���ibug 12472�j
	m_buddyRoads.ClearLinkMap();
	ATTR_BASE_CLASS::ClearFeatureDefs();
};

void CAttrWalkLinkDlg::SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList )
{
	// ����GetCurrentFeatureDef()�����\��������̂ŁA��ɃZ�b�g���Ă����K�v������
	ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList );

	// �R�t���Ă铹�H���擾
	m_buddyRoads.Init( pFeatureDefList, m_cArcHelper );
}

/**
 * @fn BOOL CAttrWalkLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * ��ʂƒʋւ̃R���{�{�b�N�X��DISABLE�ɂ��܂��B
 * ���s�҃����N��2�����b�V�����ׂ��ł���΁A�Ή����郊���N���擾���܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */

BOOL CAttrWalkLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// ���H���̂��o��
	SetRoadName(IDC_EDIT_ROADCODE, IDC_STATIC_ROADNAME2);

	// [bug 5552] 2�����b�V�����E�����ׂ��ł�����s�҃����N�̏ꍇ�͔��Α��̃����N������������
	m_cOverMeshLinks.clear();
	m_bIsLinkOnMesh = AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), GetCurrentFeatureDef()->GetFeature(), m_cOverMeshLinks );

	// ��ʂƒʋւ͕ҏW�ł��Ȃ��悤��DISABLE��
	SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
	SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable && (m_lTabNum == EDITVIEW_WALK_KIHON) )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_WALKCLASS) );
	}

	// �����l
	m_cProperty.Init();
	GetDlgItem(IDC_EDIT_SEARCHRANGE).SetWindowText( uh::str_util::ToString(m_cProperty.GetSearchRange()) );

	// �_�E���R���o�[�g�̂��߂̃R���g���[���\���E��\��
	SetControlState();
	InitBuddyRinks();

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrWalkLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �S�^�u����
		m_cControlRel.SetControl( IDC_STATIC_VAR,	TYPE_OTHER,			EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		m_cControlRel.SetControl( IDC_TAB_WALKLINK,	TYPE_OTHER,			EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,	TYPE_EDITSTRING,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI,  walk_link::kSource,		NULL, walk_link::kTableName );

		// ��{�^�u
		// ���s�ҕ\�����
		m_cControlRel.SetControl( IDC_STATIC_WALKCLASS,	TYPE_OTHER,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_WALKCLASS,	TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kWalkClass,	NULL, walk_link::kTableName );
		// �H���R�[�h
		m_cControlRel.SetControl( IDC_STATIC_ROADCODE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_ROADCODE,		TYPE_EDIT,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kWalkCode,	NULL, walk_link::kTableName );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCHROADCODE,TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME2,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		// �I�[�g�E�H�[�N
		m_cControlRel.SetControl( IDC_CHECK_AUTOWALK,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kAutoWalkFlag,	NULL, walk_link::kTableName );
		// �K�i�t���O
		m_cControlRel.SetControl( IDC_CHECK_STAIR,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kStairFlag,	NULL, walk_link::kTableName );
		// �X���[�v
		m_cControlRel.SetControl( IDC_CHECK_SLOPE,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kSlopeFlag,	NULL, walk_link::kTableName );
		// �G�X�J���[�^
		m_cControlRel.SetControl( IDC_CHECK_ESCALATOR,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kEscalatorFlag,	NULL, walk_link::kTableName );
		// �����t��
		m_cControlRel.SetControl( IDC_CHECK_ARCADE,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kArcadeFlag,	NULL, walk_link::kTableName );
		// �L��ibug 11190�j
		m_cControlRel.SetControl( IDC_CHECK_OPEN_AREA,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kOpenAreaFlag,	NULL, walk_link::kTableName );
		// �g���l���ibug 11292�j
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kTunnelFlag,	NULL, walk_link::kTableName );
		// �X�N�����u�������_
		m_cControlRel.SetControl( IDC_CHECK_SCRAMBLE,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kScrambleFlag,	NULL, walk_link::kTableName );
		// �K�w���x��
		m_cControlRel.SetControl( IDC_STATIC_FLOORLEVEL,	TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_FLOORLEVEL,		TYPE_EDITFLOAT,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kFloorLevel,	NULL, walk_link::kTableName );
		// �̂�Δԍ�
		m_cControlRel.SetControl( IDC_STATIC_PLATFORM,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_PLATFORM,		TYPE_EDIT,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kPlatformNumber,NULL, walk_link::kTableName );
		// �K�w�ړ����
		m_cControlRel.SetControl( IDC_STATIC_FLOORMOVE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_FLOORMOVE,		TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kFloorMoveClass,	NULL, walk_link::kTableName );
		// �H�ʑf��
		m_cControlRel.SetControl( IDC_STATIC_MATERIAL,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_MATERIAL,		TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kRoadSurface,	NULL, walk_link::kTableName );
		// ���H�����N
		m_cControlRel.SetControl( IDC_STATIC_ROADID,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_LIST_ROAD,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_LIST_ROADSEARCH,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_SEARCHRANG,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_SEARCHRANGE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_ROAD_ACCEPT,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_ROAD_REMOVE,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		
		// �K���^�u
		std::list<CString> array;
		// ����ʍs
		m_cControlRel.SetControl( IDC_STATIC_ONEWAYGROUP,		TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERONEWAY,		TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY,				TYPE_COMBO,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, walk_link::kOneway, CHECK_DISPOTHER, walk_link::kTableName );
		array.clear();
		array.push_back( walk_oneway::kLinkDir );
		array.push_back( walk_oneway::kStartMonth );
		array.push_back( walk_oneway::kStartDay );
		array.push_back( walk_oneway::kStartHour );
		array.push_back( walk_oneway::kStartMin );
		array.push_back( walk_oneway::kEndMonth );
		array.push_back( walk_oneway::kEndDay );
		array.push_back( walk_oneway::kEndHour );
		array.push_back( walk_oneway::kEndMin );
		array.push_back( walk_oneway::kDayOfWeek );
		m_cControlRel.SetControl( IDC_GRID_ONEWAY,				TYPE_OWTIMEREG,	EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, array, NULL, walk_oneway::kTableName, walk_oneway::kLinkID, IDC_COMBO_ONEWAY, FOLLOW_VALUE );

		// �ʍs�~��
		m_cControlRel.SetControl( IDC_STATIC_NOPASSAGEGROUP,	TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERNOPASSAGE,	TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_COMBO_NOPASSAGE,			TYPE_COMBO,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI,  walk_link::kNoPassage, CHECK_DISPOTHER, walk_link::kTableName );
		array.clear();
		array.push_back( walk_nopassage::kStartMonth );
		array.push_back( walk_nopassage::kStartDay );
		array.push_back( walk_nopassage::kStartHour );
		array.push_back( walk_nopassage::kStartMin );
		array.push_back( walk_nopassage::kEndMonth );
		array.push_back( walk_nopassage::kEndDay );
		array.push_back( walk_nopassage::kEndHour );
		array.push_back( walk_nopassage::kEndMin );
		array.push_back( walk_nopassage::kDayOfWeek );
		m_cControlRel.SetControl( IDC_GRID_NOPASSAGE,			TYPE_TIMEREG,	EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, array, NULL, walk_nopassage::kTableName, walk_nopassage::kLinkID, IDC_COMBO_NOPASSAGE, FOLLOW_VALUE );
	}
}

//�_�E���R���o�[�g�̃R���g���[���̕\���E��\���𔻒f����
void CAttrWalkLinkDlg::SetControlState()
{
	// ���s�҃_�E���R���o�[�g�Ɣėp�ł̕\���̐؂�ւ�
	int nCmdShow = SW_HIDE;
	// WINMGR�ɔC���A���̊֐��ōŏI�I�ɕ\���E��\�����肷�邽��
	// �^�u�ʒu�𔻒�ɂ����(��������Ȃ��ƋK���̕\���̎��Ɉȉ��Ŕ���������{�^�u�̂��̂��\������Ă��܂��j
	if( m_lTabNum == EDITVIEW_WALK_KIHON && AheIsDownconvert( GetCurrentFeatureDef()->GetFeature() ) == walk_type::type_class::kGeneral )
		nCmdShow = SW_SHOW;

	::ShowWindow( GetDlgItem( IDC_STATIC_ROADID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_ROAD ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_ROAD_ACCEPT ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_ROAD_REMOVE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_ROADSEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCHRANG ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_SEARCHRANGE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCH ), nCmdShow );
}

// �E�C���h�E�}�b�v�ύX
void CAttrWalkLinkDlg::ChangeWindowMgrMap( INT nIndex )
{
	switch( nIndex )
	{
	case EDITVIEW_WALK_KIHON:	
		m_winMgr.ChangeMgrMap( WALKLINK_KIHON_MAP );
		break;
	case EDITVIEW_WALK_KISEI:
		m_winMgr.ChangeMgrMap( WALKLINK_KISEI_MAP );
		break;
	default:
		break;
	}

	// �Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}

// [bug 5552] 2�����b�V�����E�����ׂ��ł�����s�҃����N�̏ꍇ�͔��Α��̃����N������������
void CAttrWalkLinkDlg::SyncChangedAttrToOverMeshLink( INT nId )
{
	if( m_bIsLinkOnMesh )
	{
		// �ύX�̂������R���g���[��
		CControlDef *pCtrlFeatureDef = m_cControlRel.GetControlDef( GetDlgItem(nId) );
		if( pCtrlFeatureDef )
		{
			// �R���g���[������l���擾
			CComVariant vaVal;
			pCtrlFeatureDef->GetControlValue( vaVal );

			// �I�����ꂽ�����N��2�����b�V�����ׂ��Ōq�����Ă郊���N�̃��X�g
			for( CTargetItems::const_iterator itOverMeshLink = m_cOverMeshLinks.begin(); itOverMeshLink != m_cOverMeshLinks.end(); itOverMeshLink++ )
			{
				// 2�����b�V�����ׂ������N�́A�K�������_�C�A���O�ŏE���Ă���͂��Ȃ̂ŁA
				// �����_�C�A���O�������Ă���t�B�[�`���̃��X�g�����v������̂�T��
				for( std::list<CFeatureDef>::iterator itFeatures = m_pFeatureDefList->begin(); itFeatures != m_pFeatureDefList->end(); itFeatures++ )
				{
					if( AheIsSameFeature( itOverMeshLink->GetFeature(), itFeatures->GetFeature() ) )
					{
						// �����_�C�A���O�ŕێ����Ă��郊���N�ɕύX�𔽉f����
						CFieldDef *pFieldDef = itFeatures->GetFieldDef( pCtrlFeatureDef->GetTableName(), pCtrlFeatureDef->GetFieldName() );
						if( pFieldDef )
						{
							pFieldDef->SetNewValue( vaVal, m_hAttributeDlgWnd );
						}
					}
				}
			}
		}
	}
}

//  �H���������s���A�H������\������
void CAttrWalkLinkDlg::SetRoadName(INT nCodeID, INT nLabelID )
{
	// �������r���Ŏ��s�����ꍇ�ɂ́A�󕶎���ɂ���
	// (����Ȋ��Ő�������Ȃ�A���������s���邱�Ƃ͂Ȃ��̂Ń��b�Z�[�W�\�L�Ȃǂ̃T�|�[�g�͓��ɂ��Ȃ�)
	GetDlgItem(nLabelID).SetWindowText( _T("") );

	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(GetCurrentFeatureDef()->GetFeature()), walk_code_list::kTableName);
	if( !ipTable ) return;

	// �H���ԍ��R�[�h���G�f�B�b�g�{�b�N�X����擾
	CString strWalkCode;
	GetDlgItem( nCodeID ).GetWindowText( strWalkCode );

	// �擾�����H���ԍ����e�[�u�����猟��
	static CString strSubFields = uh::str_util::format( _T("%s,%s"),
		walk_code_list::kNameKanji, walk_code_list::kNameYomi  );
	
	_ICursorPtr ipCursor;
	ipTable->Search( 
		AheInitQueryFilter( nullptr, strSubFields, _T("%s=%s"), walk_code_list::kWalkCode, strWalkCode),
		VARIANT_FALSE, &ipCursor );

	if( !ipCursor )
		return;

	// ��������郌�R�[�h�͈���A������Ȃ��͂� �G���[�`�F�b�N����K�v������܂�
	_IRowPtr ipRow;
	CString strKanjiName, strKanjiYomi;
	if( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		strKanjiName = AheGetAttribute( ipRow, walk_code_list::kNameKanji );

		// NAME_YOMI�t�B�[���h��NULL���Ȃ̂ŁANULL���ǂ������肷��
		CComVariant varKanjiYomi = AheGetAttribute( ipRow, walk_code_list::kNameYomi );
		// ������E���l�������Ă������́ACString��=operator����肭�������Ă����
		// VARIANT��NULL�̂Ƃ��ɂ�=operator���ŗ�����̂ŁANULL��\�������������
		strKanjiYomi = (varKanjiYomi.vt==VT_NULL)? _T("<null>") : varKanjiYomi;
	}
	// �������ʂ̕������H�����ɏ�������
	CString strText = uh::str_util::format( _T("%s(%s)"), strKanjiName, strKanjiYomi );

	GetDlgItem(nLabelID).SetWindowText( strText );
}

// ���X�g�{�b�N�X�X�V
void UpdateListBox( HWND listBox, RoadList& roads )
{
	CListBox lb(listBox);
	lb.ResetContent();
	for( auto& road : roads )
	{
		// �폜����Ă���͖̂���
		if( road.m_deleteF )
			continue;

		int idx = lb.AddString( uh::str_util::ToString( road.m_roadID ) );
		// ���X�g�{�b�N�X�̃A�C�e���ɊǗ��N���X��R�Â��Ă���
		lb.SetItemDataPtr( idx, &road );
	}
}

// ���X�g�{�b�N�X�I���A�C�e���폜
void RemoveSelectItem( HWND listBox )
{
	CListBox lb( listBox );
	int num = lb.GetCount();
	for( long i=0; i<num; ++i )
	{
		if( !lb.GetSel(i) )
			continue;

		// �R�t���Ă������o�f�B�������o���č폜�t���O�𗧂ĂĂ���
		auto buddy = (CBuddyRoad*)lb.GetItemDataPtr(i);
		buddy->m_deleteF = true;
	}
}

// ���H�R�t����������
void CAttrWalkLinkDlg::InitBuddyRinks()
{
	// ���X�g�{�b�N�X�Ɋi�[
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads(walkID) );
	UpdateListBox(GetDlgItem(IDC_LIST_ROADSEARCH), m_buddyRoads.GetSearchedRoads());
	// �n�C���C�g���Ă���(bug 11462)
	BOOL bHandled;
	OnRefresh( 0, 0, 0, bHandled );
}

// [����] �{�^��
LRESULT CAttrWalkLinkDlg::OnClickedArroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �����͈͎擾
	CString strRang;
	GetDlgItem( IDC_EDIT_SEARCHRANGE ).GetWindowText( strRang );
	long rang = _ttol( strRang );
	if( rang == 0 )
		return 0;

	// ���ӌ������ă��X�g�{�b�N�X�ɕ\��
	auto walkLink = GetCurrentFeatureDef()->GetFeature();
	UpdateListBox( GetDlgItem( IDC_LIST_ROADSEARCH ), m_buddyRoads.ArroundSearch( walkLink, rang ) );

	return 0;
}

// [<] �{�^��
LRESULT CAttrWalkLinkDlg::OnClickedAddBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ���[���`�F�b�N�ibug 11410�j
	VARIANT_BOOL vb = VARIANT_FALSE;
	m_cArcHelper.GetRule()->IsCreatableLayer( CComBSTR(link_relation::kTableName), &vb );
	if( !vb )
	{
		MessageBox( AheLoadString( IDS_UNCREATIVE_LAYER ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONERROR );
		return 0;
	}

	// �I���s���폜
	RemoveSelectItem( GetDlgItem( IDC_LIST_ROADSEARCH ) );

	// �����ς݂���R�t�������N�Ɉړ�
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	m_buddyRoads.AddFromSearchedRoads( walkID );

	// ���X�g�{�b�N�X�X�V
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads( walkID ) );
	UpdateListBox( GetDlgItem( IDC_LIST_ROADSEARCH ), m_buddyRoads.GetSearchedRoads() );
	
	// �����N�����[�V�����t���O
	SetLinkRF();

	return 0;
}

// [>] �{�^��
LRESULT CAttrWalkLinkDlg::OnClickedRemoveBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ���[���`�F�b�N�ibug 11410�j
	VARIANT_BOOL vb = VARIANT_FALSE;
	m_cArcHelper.GetRule()->IsDeletableLayer( CComBSTR(link_relation::kTableName), &vb );
	if( !vb )
	{
		MessageBox( AheLoadString( IDS_UNDELETABLE_LAYER ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONERROR );
		return 0;
	}

	// �I���s���폜
	RemoveSelectItem( GetDlgItem( IDC_LIST_ROAD ) );

	// ���X�g�{�b�N�X�X�V
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads( walkID ) );

	// �����N�����[�V�����t���O
	SetLinkRF();

	return 0;
}

// �����N�R�t���e�[�u���t���O�ݒ�
void CAttrWalkLinkDlg::SetLinkRF()
{
	auto walkLink = GetCurrentFeatureDef()->GetFeature();
	long walkID = AheGetOID( walkLink );
	BOOL buddyChange = bool2BOOL( m_buddyRoads.IsChanged(walkID) );

	// FieldDef��m_bChanged���ł����グ�邱�Ƃɂ���Đe�_�C�A���O���x��
	// ����ɂ���āA�c���[�̕ύX�F�𐧌䂵�Ă��ꂽ��AOK���Update()�̏��������s�����悤�ɂȂ�
	// �t�B�[���h�͂Ȃ�ł��ǂ��A�A�A�Ǝv��
	auto fieldDef = GetCurrentFeatureDef()->GetFieldDef( walk_link::kTableName, walk_link::kModifyDate );
	fieldDef->m_bChanged = buddyChange;
	
	// �����N�����[�V�����t���O��ݒ�
	if( buddyChange )
	{
		AheSetAttribute( walkLink, walk_link::kLinkRelationRF, m_buddyRoads.HasBuddy( walkID ) ? 1L : 0L );
	}

	// �uOK�v�{�^���ۂ�WALK_LINK���g�̑����ύX���l������K�v������
	SetButton( ATTR_BASE_CLASS::Changed() || buddyChange);
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
}

// ���X�g�{�b�N�X�w�i�F
LRESULT CAttrWalkLinkDlg::OnCtlColorListBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::GetDlgCtrlID( (HWND)lParam ) != IDC_LIST_ROAD )
		return 0;

	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	if( m_buddyRoads.IsChanged(walkID) )
	{
		static auto brush = CreateSolidBrush( ctrl_color::Modified );
		HDC hdc = (HDC)wParam;
		// �w�i���߁iTRANSPARENT�j�ɂ���ƁA�����s�G�f�B�b�g�{�b�N�X��BackSpace
		// ���������ɕ`�悪���������Ȃ�i������������Ȃ��j
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, ctrl_color::Modified );
		return (DWORD)brush;
	}
	return 0;
}

// ���X�g�s�_�u���N���b�N
LRESULT CAttrWalkLinkDlg::OnListDblClk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (!m_cArcHelper.IsStartEditing())
	{
		return 0;
	}
	CButton btn;
	switch (wID)
	{
	case IDC_LIST_ROAD: 
		btn.Attach( GetDlgItem(IDC_BUTTON_ROAD_REMOVE) ); break;
	case IDC_LIST_ROADSEARCH:
		btn.Attach( GetDlgItem(IDC_BUTTON_ROAD_ACCEPT) ); break;
	default:
		return 0;
	}

	btn.Click();

	return 0;
}

// �I���s���ς����
LRESULT CAttrWalkLinkDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	OnRefresh( 0, 0, 0, bHandled );
	return 0;
}

// �ĕ`��
LRESULT CAttrWalkLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	auto highliteLink = [&]( UINT listId, COLORREF color, bool selectedOnly )
	{
		CListBox lb( GetDlgItem( listId ) );
		int num = lb.GetCount();
		for( long i=0; i<num; ++i )
		{
			if( selectedOnly && !lb.GetSel(i) )
				continue;

			auto buddy = (CBuddyRoad*)lb.GetItemDataPtr(i);
			AheDrawPolyline(m_cArcHelper.GetDisplay(), buddy->m_geom, color, 5, esriSLSSolid, TRUE, TRUE);
		}
	};
	
	// Buddy�͏�Ƀn�C���C�g�Ń��X�g�I��������ʂ̐F�ibug 11462�j
	highliteLink( IDC_LIST_ROAD, RGB(0,0,255), false );
	highliteLink( IDC_LIST_ROAD, RGB(255,0,255), true );

	// �����̂ق��̓��X�g�I������Ă���̂���
	highliteLink( IDC_LIST_ROADSEARCH, RGB(0,255,0), true );

	return 0;
}

// OK�{�^���������ꂽ��ɌĂяo�����
BOOL CAttrWalkLinkDlg::ErrorCheck()
{
	// TODO: �`�F�b�N���������炿���Ƃǂ����ɕ����悤

	// �K�w�ړ���ʐ������`�F�b�N�ibug 11284�j
	auto FloorMoveChk = [](CFeatureDef& featureDef){
		using namespace walk_link;

		// �K�i�ƃG�X�J���[�^�́u�㉺�Ȃ��v��NG
		bool isStair = (featureDef.GetFieldDef( kTableName, kStairFlag )->m_vaValue.lVal != 0);
		bool isEsclator = (featureDef.GetFieldDef( kTableName, kEscalatorFlag )->m_vaValue.lVal != 0);
		bool isNone = (featureDef.GetFieldDef( kTableName, kFloorMoveClass )->m_vaValue.lVal == floor_move_class::kNone );

		return !( (isStair && isNone) || (isEsclator && isNone) );
	};

	// �G���[�_�C�A���O�\��
	auto ErrMsg = [&]( UINT stringID ){
		MessageBox( AheLoadString( stringID ), AheLoadString( IDS_CHECK_LOGIC ), MB_OK | MB_ICONERROR );
		return FALSE;
	};

	// �c���[�ɂ���S�Ă̕��s�҃����N�t�B�[�`�����Ώ�
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), walk_link::kTableName ) != 0 )
			continue;

		if( !FloorMoveChk( featureDef ) )
			return ErrMsg( IDS_FLOORMOVE_INTEGRATION );

	}

	UINT stringID;
	if( !m_buddyRoads.CheckLogic( stringID ) )
		return ErrMsg( stringID );

	return TRUE;
}

// �O���e�[�u���ۑ��p
bool CAttrWalkLinkDlg::Update()
{
	return m_buddyRoads.UpdateLinkRelTable();
}
