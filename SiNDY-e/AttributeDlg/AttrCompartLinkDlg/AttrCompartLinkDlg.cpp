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

// AttrCompartLinkDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrCompartLinkDlg.h"
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/registry_util.h>
#include "CompartMap.h"	// ���������N�_�C�A���O�̃E�B���h�E�z�u�}�b�v

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLink =REGPATH_SINDY_E_CHILDDLG  _T("COMPART_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrCompartLinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCompartLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCompartLinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCompartLinkDlg::CAttrCompartLinkDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCompartLinkDlg::CAttrCompartLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_COMPARTLINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// TODO[SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_COMPART_LINE_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_COMPART_LINE_C ) )
		( IDC_COMBO_COMPART_STYLE_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_COMPART_STYLE_C ) )
		( IDC_COMBO_SLOWDOWN_LABEL_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SLOWDOWN_LABEL_C) )
		( IDC_CHECK_SLOWDOWN_LABEL_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SLOWDOWN_LABEL_F ) )
		( IDC_CHECK_INSTALLATION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_INSTALLATION_F ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		( IDC_TAB_COMPARTATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_COMPARTATTR ));
}

/** 
 * @fn HWND CAttrCompartLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCompartLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrCompartLinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCompartLinkDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCompartLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCompartLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
		// TODO[SiNDYChildDlg] �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// ���X�g�r���[������������
	initLaneLink();

	// ���X�g�{�b�N�X�̓��e��\��
	if ( !setLaneLink() )
		return 0;

	createGrid();

	// �O���b�h�̕ҏW�\�t�B�[���h��ݒ�
	readReg();

	return bRet;
}

/**
 * @fn void CAttrCompartLinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCompartLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// TODO[SiNDYChildDlg] ControlRel�Ƀt�B�[���h���ƃe�[�u������o�^
		
		// ����ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_COMPART_LINE_C, TYPE_COMBO, 0, 0, compart_link::kCompartLineC, 0, compart_link::kTableName );
		// �����ݒu�l���R�[�h
		m_cControlRel.SetControl( IDC_COMBO_COMPART_STYLE_C, TYPE_COMBO, 0, 0, compart_link::kCompartStyleC, 0, compart_link::kTableName );
		// �����\���t���O
		m_cControlRel.SetControl(IDC_COMBO_SLOWDOWN_LABEL_C, TYPE_COMBO, 0, 0, compart_link::kSlowdownLabelC, 0, compart_link::kTableName );
		// �ݒu���t���O
		m_cControlRel.SetControl( IDC_CHECK_INSTALLATION_F, TYPE_CHECKBOX, 0, 0, compart_link::kInstallationF, 0, compart_link::kTableName );
		// �����R�[�h
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, compart_link::kSperculationC, 0, compart_link::kTableName );
		// �񋟌�ID
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, compart_link::kSourceID, 0, compart_link::kTableName, 0, -1, FOLLOW_READONLY);
		// Static Source
		m_cControlRel.SetControl( IDC_STATIC_VAR, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, EDITVIEW_KIHON, EDITVIEW_NODE, ipc_table::kSource, 0, compart_link::kTableName );

		// ����ʃR�[�h
		m_cControlRel.SetControl( IDC_STATIC_COMPART_LINE_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �����ݒu�l���R�[�h
		m_cControlRel.SetControl( IDC_STATIC_COMPART_STYLE_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �����W���R�[�h
		m_cControlRel.SetControl( IDC_STATIC_SLOWDOWN_LABEL_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �����R�[�h
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,		    TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �񋟌�ID
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// �Ԑ������N
		m_cControlRel.SetControl( IDC_STATIC_LANE_LINK,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// List
		m_cControlRel.SetControl( IDC_LIST_LANE_LINK,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);		
		// Grid Info
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
		// Grid
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);

		// ���䂷��K�v�̂Ȃ��R���g���[��
		//m_cControlRel.SetControl( IDC_BUTTON_SEARCH,	TYPE_BUTTON,	0,	0 );
	}
}

// �Ԑ������N���擾
bool CAttrCompartLinkDlg::getLaneLink()
{
	// �֘A�e�[�u��������������N��GID�ɕR�Â����R�[�h���擾
	IFeaturePtr currentFeature = GetCurrentFeatureDef()->GetFeature();

	// ���������N�|�Ԑ������N�֘A�e�[�u���̃t�B�[�`���N���X���擾
	ITablePtr iRelCompartLinkLaneLinkT = AheOpenSameOwnerTable(
		(ITablePtr)AheGetFeatureClass(currentFeature), rel_compart_link_lane_link::kTableName);
	if(!iRelCompartLinkLaneLinkT) return false;

	// ���ݑI������Ă���I�u�W�F�N�g��GID���擾����

	// 2016/10/03 GID����̏ꍇ���l������Ă��Ȃ��׏C��
	CComVariant gid =  AheGetAttribute(currentFeature, updown_link::kGlobalID);
	if (gid.vt == VT_NULL)
		return true;

	CString lGID (gid);
	
	// Where����쐬
	CString strWhere( uh::str_util::format(_T("%s='%s'"), rel_compart_link_lane_link::kCompartLinkGID, lGID ) );

	_ICursorPtr iRelCompartLinkLaneLinkCursor;
	iRelCompartLinkLaneLinkT->Search( AheInitQueryFilter( nullptr, nullptr, strWhere),
															VARIANT_FALSE,
															&iRelCompartLinkLaneLinkCursor );

	_IRowPtr relRow;
	std::list<std::string> aLaneLinkGIDs;  // �R�Â��Ă���Ԑ������N��GID
	while( ( S_OK == iRelCompartLinkLaneLinkCursor->NextRow( &relRow ) ) && relRow )
	{
		long sequence = AheGetAttribute( relRow,rel_compart_link_lane_link::kSequence).lVal;
		// �����o�ϐ��Ɋi�[

		// �d�����Ă�����̂��������珜�O(���ʂ͖����͂�)
		bool doContinue(false);
		for ( const auto& tmp : m_relCompartLinkLaneLink)
		{		
			if( AheGetAttribute(tmp.second, rel_compart_link_lane_link::kLaneLinkGID) ==  
				AheGetAttribute(relRow, rel_compart_link_lane_link::kLaneLinkGID)
			  )
			{
				doContinue = true;
				break;
			}
		}

		if ( doContinue )
			continue;
		
		m_relCompartLinkLaneLink.insert(std::make_pair(sequence,relRow));
		// ���łɕR�Â��Ă���Ԑ������N��GID���擾���Ă���
		aLaneLinkGIDs.push_back( uh::toStr(AheGetAttribute( relRow, rel_compart_link_lane_link::kLaneLinkGID).bstrVal ) );
	}

	// IN���GID����R�Â��Ă���Ԑ������N�����ׂĎ擾
	IFeatureClassPtr iLaneLinkFC = AheOpenSameOwnerTable(
		AheGetTable(GetCurrentFeatureDef()->GetFeature()), lane_link::kTableName);
	if(!iLaneLinkFC) return false;

	uh::ElementsStrings gidStrings = uh::enumerate_elements<std::list<std::string>>( aLaneLinkGIDs, 1000, "'" );

	// 1000���ƂɃI�u�W�F�N�g���擾
	for( const auto gidStr : gidStrings )
	{
		CString whereClause = uh::str_util::format( _T("%s IN (%s)"), lane_link::kGlobalID, uh::toTStr(gidStr).c_str() );

		IFeatureCursorPtr iLaneLinkCursor;
		iLaneLinkFC->Search( AheInitQueryFilter( nullptr, nullptr, whereClause), VARIANT_FALSE, &iLaneLinkCursor );

		IFeaturePtr laneLinkFeature;
		while( ( S_OK == iLaneLinkCursor->NextFeature( &laneLinkFeature ) ) && laneLinkFeature )
		{
			long OID(0);
			laneLinkFeature->get_OID(&OID);

			// �����o�ϐ��ɎԐ������N�̌`����Z�b�g
			m_laneLinkGeo[OID] = AheGetShape(laneLinkFeature);
			// GID-OID�̕ϊ��\���쐬
			m_laneLinkGidOid[AheGetAttribute(laneLinkFeature, lane_link::kGlobalID)] = OID;
		}
	}

	return true;
}

// �Ԑ������N�����������X�g�{�b�N�X�ɕ\��
bool CAttrCompartLinkDlg::setLaneLink()
{
	// �Ԑ������N���擾
	if (!getLaneLink())
		return false;

	CListViewCtrl lvCtrl(GetDlgItem(IDC_LIST_LANE_LINK));
	lvCtrl.DeleteAllItems();
	int itemCount = lvCtrl.GetItemCount();
	for( const auto& relRec : m_relCompartLinkLaneLink )
	{
		if (!relRec.second)
			continue;

		int index = 0;
		CString laneLinkGID( AheGetAttribute(relRec.second, rel_compart_link_lane_link::kLaneLinkGID) );

		if ( m_laneLinkGidOid.find(laneLinkGID) == m_laneLinkGidOid.end() )
			continue;

		lvCtrl.AddItem( itemCount, 0, uh::str_util::ToString(m_laneLinkGidOid[laneLinkGID]) );
		++itemCount;
	}
	return true;
}

// ���X�g�r���[������������
void CAttrCompartLinkDlg::initListView()
{
	const UINT COLUMN_MASK = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;  //!< ��̗L�������o�t���O
	const UINT MAX_SIZE = 265;								 //!< ��̍ő�T�C�Y(���{���͂ǂ�������Ƃ��Ă�����)

	CListViewCtrl lvCtrl(GetDlgItem(IDC_LIST_LANE_LINK));

	// �A�C�e����������
	lvCtrl.DeleteAllItems();
	// �J�������������
	long lHeaderCount = lvCtrl.GetHeader().GetItemCount();
	for(int i=0; i<lHeaderCount; ++i)
		lvCtrl.DeleteColumn(0);

	lvCtrl.AddColumn( _T("Object ID"), 0, -1, COLUMN_MASK, LVCFMT_LEFT );
	lvCtrl.SetColumnWidth(0, MAX_SIZE);

	// ���X�g�r���[�̃X�^�C�����w��
	// �I�����ꂽ�Ƃ��ɍs�S�̂�I����Ԃɂ��A�O���b�h��������
	DWORD word = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ;
	lvCtrl.SetExtendedListViewStyle( word , word );
}

// ���X�g�r���[������������
void CAttrCompartLinkDlg::initLaneLink()
{
	m_laneLinkGeo.clear();
	m_laneLinkGidOid.clear();
	m_ipSelectedLine = nullptr;
	m_relCompartLinkLaneLink.clear();
}

// �Ԑ������N���X�g���N���b�N�����Ƃ��̏���
LRESULT CAttrCompartLinkDlg::OnClickSearchResult(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CListViewCtrl lv( GetDlgItem(IDC_LIST_LANE_LINK) );
	// �I�𒆂̃��R�[�h�̃I�u�W�F�N�gID���擾
	int nCurIdx = lv.GetSelectedIndex();
	if( -1 == nCurIdx ) return 0;

	TCHAR buffer[256];
	lv.GetItemText(nCurIdx, 0,  buffer, sizeof(buffer) / sizeof(TCHAR));
	CString strOID(buffer);
	if(strOID.IsEmpty()) return 0;

	// �����o�ϐ��ɃZ�b�g
	m_ipSelectedLine = m_laneLinkGeo[_ttoi(strOID)];

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}

// �ĕ`��
LRESULT CAttrCompartLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �ҏW����Lane_LINK�̌`���ΐF�ŕ`�悷��
	if(m_ipSelectedLine)
		AheDrawPolyline(m_cArcHelper.GetDisplay(), m_ipSelectedLine, RGB(0,255,50), 5, esriSLSSolid, TRUE, TRUE);

	highlightLine();
	return 0;
}

// �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
void CAttrCompartLinkDlg::ChangeWindowMgrMap( int iIndex )
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
void CAttrCompartLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable)	return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_compart_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// �n�߂ă_�C�A���O�ɕ\�������t�B�[�`���ł���΁A�m�[�h��O���b�h��o�^
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_compart_node::kObjectID)
			.addField(nq_compart_node::kSequence)
			.addField(nq_compart_node::kWidth)
			.addField(nq_compart_node::kCompartNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// �O���b�h������
	m_nqDataGridMap[m_currentOid].init();

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	CComVariant linkGid = AheGetAttribute(ipFeature, compart_link::kGlobalID);
	if (linkGid.vt == VT_NULL) return;

	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_compart_node::kCompartLinkGID, linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrCompartLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
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
void CAttrCompartLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
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
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqCompartNodeGrid, X, Y );
	}
}

// After Edit
void CAttrCompartLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// �֘A�e�[�u���̍X�V���̍X�V
void CAttrCompartLinkDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

// �E�N���b�N����� <�폜> �|�b�v�A�b�v
void CAttrCompartLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
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
			nq_compart_node::kObjectID, bstrValue);

		CString objectID = bstrValue.m_str;

		// �ȉ���if���́A�V�K���R�[�h�̏ꍇ�AOID��RowData�ɓ���ĊǗ����邽�߂̏����ł���
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_compart_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}		

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);

		m_nqDataGridMap[m_currentOid].highlightChangedCell();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// �{�^������
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
		pRelTableDef->setChanged(true);

		// �{�^������
		SetButton(true);
	}
}

// Update Grid Data
bool CAttrCompartLinkDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), compart_link::kTableName) != 0)	continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged()) continue;

		if (!m_nqDataGridMap[oid].store()) return false;
	}
	return true;
}

// Check Data Changes
bool CAttrCompartLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrCompartLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// �_�C�A���O�ŕ\�����Ă�����������N�t�B�[�`�����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","���������NGID"���擾����
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNodeT = AheOpenSameOwnerTable(ipTable, compart_node::kTableName);
	if (!ipNodeT) return;

	CString nodeGid, nodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(nodeGid, nodeGidNext, nq_compart_node::kSequence, nq_compart_node::kCompartNodeGID);

	//   �������ꂽ�m�[�h��̃m�[�hGID���玟�̃m�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nodeGid);
	_ICursorPtr ipNodeCur;
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr ipNodeFeature;
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nodeGidNext);
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipToPoint);
}

// �O���b�h�őI�����Ă���m�[�h��̃��C�����n�C���C�g����
void CAttrCompartLinkDlg::highlightLine()
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
LRESULT CAttrCompartLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// �m�[�h��̃W�����v,���S�ֈړ�
void CAttrCompartLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
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
void CAttrCompartLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID �t�B�[���h�͎w�肳��Ă��ύX�s��
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_compart_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
