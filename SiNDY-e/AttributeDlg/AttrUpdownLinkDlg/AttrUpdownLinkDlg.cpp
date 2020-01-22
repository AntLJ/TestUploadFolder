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

// AttrUpdownLinkDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrUpdownLinkDlg.h"
#include <Map.h>
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/registry_util.h>
#include "UpdownMap.h"	// �㉺�������N�_�C�A���O�̃E�B���h�E�z�u�}�b�v

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

const CString CAttrUpdownLinkDlg::UNDEF_NAME = _T("���ݒ�");
const long CAttrUpdownLinkDlg::UNDEF_OID = 0;
const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLink =REGPATH_SINDY_E_CHILDDLG  _T("UPDOWN_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrUpdownLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrUpdownLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrUpdownLinkDlg::CAttrUpdownLinkDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrUpdownLinkDlg::CAttrUpdownLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_UPDOWNLINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// [SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_UPDOWN_GROUP_ID,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_UPDOWN_GROUP_ID ) )
		( IDC_EDIT_GROUP_C,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_GROUP_C ) )
		( IDC_EDIT_LANE_COUNT,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_COUNT ) )
		( IDC_EDIT_RIGHT_CHANGE,CContainedWindow(  _T("EDIT"), this, IDC_EDIT_RIGHT_CHANGE ) )
		( IDC_EDIT_LEFT_CHANGE,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LEFT_CHANGE ) )

		( IDC_COMBO_MAX_LEGAL_SPEED_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MAX_LEGAL_SPEED_C ) )
		( IDC_COMBO_MIN_LEGAL_SPEED_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MIN_LEGAL_SPEED_C ) )
		( IDC_COMBO_TRAVEL_DIRECTION_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_TRAVEL_DIRECTION_C ) )
		( IDC_COMBO_ROAD_STRUCTURE_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ROAD_STRUCTURE_C ) )
		( IDC_COMBO_ONEWAY_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ONEWAY_C ) )

		( IDC_CHECK_RUBBING_F,	CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RUBBING_F ) )
		( IDC_CHECK_TUNNEL_F,	CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL_F ) )

		( IDC_COMBO_SPECULATION_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID,		CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) )
		( IDC_EDIT_SOURCE,		CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		
		( IDC_TAB_UPDOWNATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_UPDOWNATTR ));
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
 * @fn HWND CAttrUpdownLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrUpdownLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrUpdownLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrUpdownLinkDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrUpdownLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrUpdownLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
		::SetFocus( GetDlgItem(IDC_EDIT_LANE_COUNT) );
	}
	
		// ����������
	init();

	// ���́E�ǂݗp�̃c�[���`�b�v�쐬
	//
	UpdateGroupC();
	UpdateUnDefOID();

	createGrid();
			
	// �O���b�h�̕ҏW�\�t�B�[���h��ݒ�
	readReg();

	return bRet;
}

/**
 * @fn void CAttrUpdownLinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrUpdownLinkDlg::CreateControlRelation()
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

		m_cControlRel.SetControl( IDC_EDIT_LANE_COUNT,			TYPE_EDIT,			0, 0,	updown_link::kLaneCount,			0,	updown_link::kTableName );	
		m_cControlRel.SetControl( IDC_EDIT_RIGHT_CHANGE,		TYPE_EDIT,			0, 0,	updown_link::kRightChange,			0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_EDIT_LEFT_CHANGE,			TYPE_EDIT,			0, 0,	updown_link::kLeftChange,			0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_COMBO_MAX_LEGAL_SPEED_C,	TYPE_COMBO,			0, 0,	updown_link::kMaxLegalSpeedC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_MIN_LEGAL_SPEED_C,	TYPE_COMBO,			0, 0,	updown_link::kMinLegalSpeedC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_TRAVEL_DIRECTION_C,	TYPE_COMBO,			0, 0,	updown_link::kTravelDirectionC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_ROAD_STRUCTURE_C,	TYPE_COMBO,			0, 0,	updown_link::kRoadStructureC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY_C,			TYPE_COMBO,			0, 0,	updown_link::kOneWayC,				0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_CHECK_RUBBING_F,			TYPE_CHECKBOX,		0, 0,	updown_link::kRubbingF,				0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL_F,			TYPE_CHECKBOX,		0, 0,	updown_link::kTunnelF,				0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			0, 0,	updown_link::kSperculationC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,			TYPE_EDITSTRING,	0, 0,	updown_link::kSourceID,				0,	updown_link::kTableName, 0, -1, FOLLOW_READONLY);
				
		// Static Source
		m_cControlRel.SetControl( IDC_STATIC_VAR, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);

		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDIT,			EDITVIEW_KIHON, EDITVIEW_NODE,	updown_link::kSource,				0,	updown_link::kTableName );

		// ���䂷��K�v�̂Ȃ��R���g���[��
		m_cControlRel.SetControl( IDC_EDIT_UPDOWN_GROUP_ID,		TYPE_EDIT,			0, 0,	updown_link::kUpdoenGroupID,		0,	updown_link::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_EDIT_GROUP_C,				TYPE_EDIT,			0, 0,	0,			0, 0 , 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
	
		
		// �㉺���O���[�vOID
		m_cControlRel.SetControl( IDC_STATIC_UPDOWN_GROUP_ID,   TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �㉺���O���[�v����
		m_cControlRel.SetControl( IDC_STATIC_GROUP_C,           TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �Ԑ���
		m_cControlRel.SetControl( IDC_STATIC_LANE_COUNT,        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �E�������Ԑ���
		m_cControlRel.SetControl( IDC_STATIC_RIGHT_CHANGE,      TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ���������Ԑ���
		m_cControlRel.SetControl( IDC_STATIC_LEFT_CHANGE,       TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �������x�R�[�h(�ő�)
		m_cControlRel.SetControl( IDC_STATIC_MAX_LEGAL_SPEED_C, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �������x�R�[�h(�ŏ�)
		m_cControlRel.SetControl( IDC_STATIC_MIN_LEGAL_SPEED_C, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �i�s������ʃR�[�h
		m_cControlRel.SetControl( IDC_STATIC_TRAVEL_DIRECTION_C,TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ���H�\����ʃR�[�h
		m_cControlRel.SetControl( IDC_STATIC_ROAD_STRUCTURE_C,  TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ����ʍs��ʃR�[�h
		m_cControlRel.SetControl( IDC_STATIC_ONEWAY_C,          TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,     TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �񋟌�ID
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,         TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ATTR_GROUP
		m_cControlRel.SetControl( IDC_STATIC_ATTR_GROUP,        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// PECULATION_C_GROUP
		m_cControlRel.SetControl( IDC_STATIC_PECULATION_C_GROUP,TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
				
		// Grid Info
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
		// Grid
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
	
	}
}
/**
 * �G���[�`�F�b�N
 * @TODO	���́E�ǂ݂̕Е���NULL�������ꍇ�̂݃`�F�b�N
 *			���`�F�b�N��v������
 */
BOOL CAttrUpdownLinkDlg::ErrorCheck()
{
	// ���̏���
	return TRUE;
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
 * @TODO	�Ԑ����f���ł��g���Ă���̂ŋ��ʉ�������
*/
void CAttrUpdownLinkDlg::SetToolTip(INT nTargetItem)
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


void CAttrUpdownLinkDlg::UpdateGroupC()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//�O���[�v��ʃR�[�h�擾
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipUpdownLinkGroup = AheOpenSameOwnerTable(ipTable, updown_link_group::kTableName);
	if(!ipUpdownLinkGroup)
	{
		return;
	}

	//�㉺���O���[�vOID���瑮���R�[�h�l�擾
	long updoenGroupID = AheGetAttribute(ipFeature, updown_link::kUpdoenGroupID).lVal;
	CString where_clause;
	where_clause.Format(_T("%s=%d"), updown_link_group::kObjectID, updoenGroupID);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipUpdownLinkGroup->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	long groupC = 1;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		groupC = AheGetAttribute(ipRow, updown_link_group::kGroupC).lVal;
	}
	//�O���[�v��ʃR�[���瑮���l���̎擾
	if(!SetDomain())
	{
		return;
	}
	CString str;
	if(updoenGroupID == UNDEF_OID)
	{
		str = UNDEF_NAME;
	}
	else
	{
		str = m_segAttrDomain[groupC];
	}

	//�R���g���[���X�V
	CEdit(GetDlgItem(IDC_EDIT_GROUP_C)).SetWindowText(str);
}

bool CAttrUpdownLinkDlg::SetDomain(bool update/* = false*/)
{
	//�X�V���Ȃ��ꍇ
	if(!update)
	{
		//�ݒ�ς�(��łȂ����OK�Ԃ�)
		if(!m_segAttrDomain.empty())
		{
			return true;
		}
	}

	m_segAttrDomain.clear();

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if(!ipFeature)
	{
		return false;
	}

	//�㉺�������N�O���[�v�e�[�u���ڑ�
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return false;
	}
	ITablePtr ipUpdownLinkGroup = AheOpenSameOwnerTable(ipTable, updown_link_group::kTableName);
	if(!ipUpdownLinkGroup)
	{
		return false;
	}

	//�h���C���l�擾
	IDomainPtr ipDomain;
	long index = -1;
	ipUpdownLinkGroup->FindField(const_cast<BSTR>( updown_link_group::kGroupC), &index);
	IFieldsPtr ipFields;
	ipUpdownLinkGroup->get_Fields(&ipFields);
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
			m_segAttrDomain[val.lVal] = CString(name);
		}
	}

	if(m_segAttrDomain.empty())
	{
		return false;
	}

	return true;
}

void CAttrUpdownLinkDlg::UpdateUnDefOID()
{
	//OID��UNDEF_OID�Ȃ�\�����X�V
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
		return;

	//�Z�O�����gID
	if(AheGetAttribute(ipFeature, updown_link::kUpdoenGroupID).lVal == UNDEF_OID)
	{
		//�R���g���[���X�V
		CEdit(GetDlgItem(IDC_EDIT_SEGMENT)).SetWindowText(UNDEF_NAME);
	}
}

// ����������
void CAttrUpdownLinkDlg::init()
{
	_IRowPtr currentRow = GetCurrentFeatureDef()->GetFeature();
	long OID = AheGetOID( currentRow );
	
	BOOL isEnableWindow(TRUE);
	if(m_GIDMap.find(OID) == m_GIDMap.end())
	{
		// 2016/09/13 GID����̏ꍇ���l������Ă��Ȃ��׏C��
		CComVariant gid =  AheGetAttribute(currentRow, updown_link::kGlobalID);
		if(gid.vt != VT_NULL)
		{
			m_GIDMap[OID] = AheGetAttribute(currentRow, updown_link::kGlobalID);
		}
		else
		{
			// GID����̏ꍇ�ꍇ�͕R�t���@�\���@�\�����Ȃ�
			MessageBox( AheLoadString( IDS_NOT_USE_RELATION ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONINFORMATION );
			isEnableWindow = FALSE;
		}
	}


	if(m_relRoadLinkOrgs.find(OID) == m_relRoadLinkOrgs.end())
		m_relRoadLinkOrgs[OID] = std::map<long, _IRowPtr>();

	if(m_relRoadLinkLists.find(OID) == m_relRoadLinkLists.end())
		m_relRoadLinkLists[OID] = InvolvedRoadLinkMap();

	if(m_searchedRoadLinkLists.find(OID) == m_searchedRoadLinkLists.end())
		m_searchedRoadLinkLists[OID] = InvolvedRoadLinkMap();

	// ���H�����N�����R�[�h�̃h���C�����擾���Ă���
	m_directionCDomain = getDomainCodeValue( m_cArcHelper.GetFocusMap(),
		                                   rel_road_link_updown_link::kTableName,
		                                   rel_road_link_updown_link::kRoadLinkDirectionC);
}

// �ҏW�Ώۂ̓��H�����N�|�㉺�������N�֘A���R�[�h(ORG)��ԋp����
std::map<long, _IRowPtr>& CAttrUpdownLinkDlg::getCurrentRelRoadLinkOrg()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_relRoadLinkOrgs[OID];
}

// �ҏW�Ώۂ̏㉺�������N�ɕR�t���Ă��铹�H�����N��ԋp����
InvolvedRoadLinkMap& CAttrUpdownLinkDlg::getCurrentRelRoadLinkList()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_relRoadLinkLists[OID];
}

// �ҏW�Ώۂ̎��ӌ������s�������H�����N��ԋp����
InvolvedRoadLinkMap& CAttrUpdownLinkDlg::getCurrentSearchedList()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_searchedRoadLinkLists[OID];
}

// �ĕ`��
LRESULT CAttrUpdownLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	highlightLine();
	return 0;
}

// ���H�����N���擾
IFeatureClassPtr CAttrUpdownLinkDlg::getRoadLinkFC()
{
	if( m_roadLinkFeatureClass )
		return m_roadLinkFeatureClass;

	m_roadLinkFeatureClass = getSameTable(road_link::kTableName, nullptr);

	return m_roadLinkFeatureClass;
}

// TOC�ɂ���w�肵���e�[�u������ITable���擾
ITablePtr CAttrUpdownLinkDlg::getSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner)
{
	// TOC�ɂ���w�肵���e�[�u������ITable���擾
	CMap cMap =  m_cArcHelper.GetFocusMap();

	ITablePtr ret = AheGetTable( cMap.FindLayer( nullptr, lpcszOwner, lpcszName ) );

	return ret;
}


// ���H�����N�|�㉺�������N�֘A���R�[�h�̍X�V
bool CAttrUpdownLinkDlg::Update()
{
	using namespace sindy::schema::adam_v2::rel_road_link_updown_link;

	// �X�V�Ώۂ̊֘A�e�[�u���擾���Ă���
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	ITablePtr ipRelUpdownLink = AheOpenSameOwnerTable(AheGetTable(ipFeature), rel_road_link_updown_link::kTableName);

	for (const auto& orgRelMap: m_relRoadLinkOrgs)
	{
		long updownLinkOid = orgRelMap.first;

		InvolvedRoadLinkMap chgRelLinks = m_relRoadLinkLists.at(updownLinkOid);

		for (auto& chgRelLink : chgRelLinks)
		{
			// �ύX������΍X�V
			if ( orgRelMap.second.find(chgRelLink.first) != orgRelMap.second.end() )
			{
				if( isChangeDirectionC(chgRelLink.first) )
				{
					_IRowPtr org =  orgRelMap.second.at(chgRelLink.first);

					if( !AheSetModifyData( org, sindyUpdateTypeProperty ))
						return false;

					org->put_Value( AheFindField( AheGetFields( org ), kRoadLinkDirectionC ),
									CComVariant( chgRelLink.second.m_directionC ) );
					org->Store();
				}
			}
			// ���X�̊֘A�e�[�u���ɑ��݂��Ȃ��ꍇ�͐V�K�ǉ�
			else
			{
				_IRowPtr row = AheCreateRow( ipRelUpdownLink );
				if( !row )
					return false;

				// �����t�^
				if( !AheSetModifyData( row, sindyUpdateTypeCreate ) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kUpdownLinkGID,  m_GIDMap[updownLinkOid]) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kRoadLinkID, chgRelLink.first) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kRoadLinkDirectionC, (long)chgRelLink.second.m_directionC)
					)
					return false;

				row->Store();
			}
		}
		
		// �폜���ꂽ���H�����N�|�㉺�������N�֘A���R�[�h��������
		std::list<long> deleteList;
		for (const auto& orgRelLink : orgRelMap.second)
		{
			// ���X�g�{�b�N�X����Ȃ��Ȃ��Ă��܂��Ă�����͍̂폜���ꂽ�Ɣ��f
			if ( chgRelLinks.find(orgRelLink.first) == chgRelLinks.end() )
				deleteList.push_back(orgRelLink.first);
		}

		// �폜����
		for( const auto& deleteOid : deleteList )
		{
			_IRowPtr row = orgRelMap.second.at(deleteOid);
			row->Delete();
			row->Store();
		}
	}

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), updown_link::kTableName) != 0) continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged()) continue;

		if (!m_nqDataGridMap[oid].store()) return false;
	}

	return true;
}

// ���H�����N�|�㉺�������N�֘A���R�[�h�ɕύX����������
bool CAttrUpdownLinkDlg::isChangeRefTbl()
{
	// �悸�A������v���邩�m�F
	if( getCurrentRelRoadLinkOrg().size() != getCurrentRelRoadLinkList().size() )
		return true;

	// ���H�����N��OID�A���H�����N�����R�[�h�ɕύX�����邩�m�F
	for( const auto& relRec : getCurrentRelRoadLinkList() )
	{
		if(getCurrentRelRoadLinkOrg().find(relRec.first) == getCurrentRelRoadLinkOrg().end() ) 
			return true;
	}
	return false;
}

// ���H�����N�����R�[�h�ɕύX����������
bool CAttrUpdownLinkDlg::isChangeDirectionC(long oid)
{
	// �V�K�ɒǉ����ꂽ���R�[�h�͏��true
	if (getCurrentRelRoadLinkOrg().find(oid) == getCurrentRelRoadLinkOrg().end())
		return true;
	
	_IRowPtr org = getCurrentRelRoadLinkOrg().at(oid);

	int srcC  = AheGetAttribute( org, rel_road_link_updown_link::kRoadLinkDirectionC).intVal;
	int dstC  = getCurrentRelRoadLinkList().at(oid).m_directionC;
	
	return srcC != dstC;
}

// �R�[�h�l�h���C�����擾����
std::map<CString,long> CAttrUpdownLinkDlg::getDomainCodeValue( IMap* ipMap ,LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName )
{
	std::map<CString,long> mapCodeValue;
	// �R�[�h�l�h���C�����擾����
	IDomainPtr ipDomain;
	if( AheGetDomainByFieldName( ipMap, lpcszTableName, lpcszFieldName, &ipDomain ) )
	{
		ICodedValueDomainPtr ipCVDomain( ipDomain );
		if( ipCVDomain )
		{
			long lCount = 0;
			ipCVDomain->get_CodeCount( &lCount );
			for( int i = 0; i < lCount; ++i )
			{
				CComBSTR bstrValue;
				CComVariant val;
				ipCVDomain->get_Name( i, &bstrValue );
				ipCVDomain->get_Value( i, &val );
				mapCodeValue.insert( std::pair<CString,long>( CString(bstrValue), val.intVal ) );
			}
		}
	}
	return mapCodeValue;
}

// �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
void CAttrUpdownLinkDlg::ChangeWindowMgrMap( int iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_KIHON:		// ��{�����^�u
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_NODE:		// �m�[�h�񑮐��^�u
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
void CAttrUpdownLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable)	return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_updown_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// �n�߂ă_�C�A���O�ɕ\�������t�B�[�`���ł���΁A�m�[�h��O���b�h��o�^
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_updown_node::kObjectID)
			.addField(nq_updown_node::kSequence)
			.addField(nq_updown_node::kWidth)
			.addField(nq_updown_node::kUpdownNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// �O���b�h������
	m_nqDataGridMap[m_currentOid].init();

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	CComVariant linkGid = AheGetAttribute(ipFeature, updown_link::kGlobalID);
	if (linkGid.vt == VT_NULL) return;

	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_updown_node::kUpdownLinkGID, linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrUpdownLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
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
void CAttrUpdownLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
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
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqUpdownNodeGrid, X, Y );
	}
}

// After Edit
void CAttrUpdownLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// �֘A�e�[�u���̍X�V���̍X�V
void CAttrUpdownLinkDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

// �E�N���b�N����� <�폜> �|�b�v�A�b�v
void CAttrUpdownLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
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
			nq_updown_node::kObjectID, bstrValue);

		CString objectID = bstrValue.m_str;

		// �ȉ���if���́A�V�K���R�[�h�̏ꍇ�AOID��RowData�ɓ���ĊǗ����邽�߂̏����ł���
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_updown_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}		

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// �{�^������
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
		pRelTableDef->setChanged(true);

		// �{�^������
		SetButton(true);
	}
}

// Check Data Changes
bool CAttrUpdownLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrUpdownLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// �_�C�A���O�ŕ\�����Ă���㉺�������N�t�B�[�`�����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","�㉺�������NGID"���擾����
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNodeT = AheOpenSameOwnerTable(ipTable, updown_node::kTableName);
	if (!ipNodeT) return;

	CString nodeGid, nodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(nodeGid, nodeGidNext, nq_updown_node::kSequence, nq_updown_node::kUpdownNodeGID);

	//   �������ꂽ�m�[�h��̃m�[�hGID���玟�̃m�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s'"), updown_node::kGlobalID, nodeGid);
	_ICursorPtr ipNodeCur;
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr ipNodeFeature;
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), updown_node::kGlobalID, nodeGidNext);
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipToPoint);
}

// �O���b�h�őI�����Ă���m�[�h��̃��C�����n�C���C�g����
void CAttrUpdownLinkDlg::highlightLine()
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

// Tear down connection points
LRESULT CAttrUpdownLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// �m�[�h��̃W�����v,���S�ֈړ�
void CAttrUpdownLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
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
void CAttrUpdownLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID �t�B�[���h�͎w�肳��Ă��ύX�͕s��
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_updown_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
