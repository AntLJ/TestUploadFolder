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

// AttrRoadSignPointDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrRoadSignPointDlg.h"
#include <boost/assign/list_of.hpp>
#include "Util.h"

using namespace sindy::schema;
using namespace adam_v2;

#include "WinMap.h"

// �|�����C���`�掞�̐ݒ�
namespace {
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}

extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrRoadSignPointDlg();
}

CAttrRoadSignPointDlg::CAttrRoadSignPointDlg() 
	: ATTR_BASE_CLASS(ROADSIGN_POINT_MAP, IDD_ROADSIGNPOINTDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_ROADSIGN_C,      CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_ROADSIGN_C))
		(IDC_EDIT_ROADSIGN_C,       CContainedWindow(_T("EDIT"), this, IDC_EDIT_ROADSIGN_C))
		(IDC_BUTTON_SEARCH,         CContainedWindow(_T("BUTTON"), this, IDC_BUTTON_SEARCH))
		(IDC_EDIT_HORIZONTAL_ANGLE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_HORIZONTAL_ANGLE))
		(IDC_CHECK_VARIABLE_F,      CContainedWindow(_T("CHECKBOX"), this, IDC_CHECK_VARIABLE_F))
		(IDC_COMBO_SPECULATION_C,   CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C))
		(IDC_EDIT_SOURCE_ID,        CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		(IDC_EDIT_SOURCE,           CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE ))
		;

	// �����o�ϐ�������
	m_TabNum		= 0;
	m_FeatureIndex  = 0;
	m_TableIndex	= 0;
	m_RowIndex		= 0;
	m_IsForce		= false;
	m_Editable		= false;
}

LRESULT CAttrRoadSignPointDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R���g���[���̃Z�b�g
	CreateControlRelation();

	// �O���b�h������
	m_Grid.init(GetDlgItem(IDC_VSFLEXGRID_NODE));

	// GridCtrl����̃C�x���g���擾
	AtlAdviseSinkMap(this, TRUE);

	// �T�u�N���X��
	for (auto& it : m_mapSubClass) {
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	//// �^�u�R���g���[���̍쐬
	//CTabCtrl cTab( GetDlgItem( IDC_TAB_ROADSIGN ) );
	//cTab.AddItem( AheLoadString( IDS_ROADSIGN_BASE ) );
	//cTab.AddItem( AheLoadString( IDS_ROADSIGN_CLASS ) );

	return CAttrBaseDlg::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrRoadSignPointDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if( msg == EN_SETFOCUS )
	SelectDlgItem( nTargetControl );

	if (msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);
	if ((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		m_cControlRel.ControlChanged((INT)LOWORD(wParam));
		InvalidateRect(NULL, FALSE);
		SetButton(Changed());
	}

	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetRoadsignCodeToComboBox(IDC_EDIT_ROADSIGN_C, IDC_COMBO_ROADSIGN_C);
	return 0;
}

// ���H�W���R�[�h��ʃ^�u�ŕҏW�\�ɂ���ꍇ�ɔ����āA
// �ȉ��̃R�[�h�̓R�����g�A�E�g�Ƃ��Ďc���Ă���
//LRESULT CAttrRoadSignPointDlg::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
//{
	//if( IDC_TAB_ROADSIGN == wParam )
	//{
	//	// �Z�����؂�ւ��̃C�x���g��
	//	if( (((NMHDR*)lParam)->code) == TCN_SELCHANGE )
	//	{
	//		// �I�����ꂽ�^�u���擾
	//		m_ChangeType = TabCtrl_GetCurSel(((NMHDR*)lParam)->hwndFrom);

	//		m_cControlRel.Visible(m_ChangeType);
	//		ChangeWindowMgrMap( m_ChangeType );
	//		m_TabNum = m_ChangeType;				// �^�u�ʒu�ۑ�

	//		ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_FeatureIndex, m_TableIndex, m_RowIndex, m_TabNum, m_IsForce, m_Editable );
	//					
	//		switch( m_TabNum )
	//		{
	//		case EDITVIEW_ROADSIGN_KIHON:
	//			if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
	//				::SetFocus( GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE) );
	//			break;
	//		case EDITVIEW_ROADSIGN_CLASS:
	//			break;
	//		default:
	//			break;
	//		}

	//		bHandled = TRUE;
	//		return 1;
	//	}
	//}

	//if( 0 == wParam )
	//{
	//	HWND hwndTab = GetDlgItem( IDC_TAB_ROADSIGN );
	//	if( -2 == lParam || -1 == lParam )
	//	{
	//		// �R���g���[���؂�ւ�
	//		if( -1 == lParam )
	//		{	
	//			if( 1 == m_ChangeType )	
	//				m_ChangeType = 0;
	//			else
	//				m_ChangeType = m_ChangeType + 1;
	//		}
	//		else
	//		{
	//			if( 0 == m_ChangeType )
	//				m_ChangeType = 1;
	//			else
	//				m_ChangeType = m_ChangeType - 1;
	//		}
	//		::SendMessage( hwndTab, TCM_SETCURSEL, m_ChangeType, 0 );

	//		m_cControlRel.Visible( m_ChangeType );
	//		ChangeWindowMgrMap( m_ChangeType );
	//		m_TabNum = m_ChangeType;	// �^�u�̈ʒu�ۑ�
	//		SetCurrentFeatureDefIndex( m_FeatureIndex, m_TableIndex, m_RowIndex, m_IsForce, m_Editable );

	//		bHandled = TRUE;
	//		return 1;
	//	}
	//}

//}

LRESULT CAttrRoadSignPointDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_Grid.addRow();
	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_Grid.deleteSelectedRow();

	auto oids = m_Grid.getData();

	// �O���b�h�ł̍폜���f�[�^�ɔ��f
	std::vector<long> errOids;
	if (!m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		// �G���[�ɂȂ邱�Ƃ͂Ȃ��͂������O�̂���
		AheMessageBox(NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID),
			AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), MB_OK | MB_TASKMODAL, NULL);
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);

	return 0;
}

HWND CAttrRoadSignPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent, dwInitParam);
}

void CAttrRoadSignPointDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	SetFocus();

	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrRoadSignPointDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadsign_point::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_RelNodes[lOid].isChange())
			continue;

		if (!m_RelNodes[lOid].updateTable())
			return false;
	}
	return true;
}

BOOL CAttrRoadSignPointDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadsign_point::kTableName) != 0)
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID(&lOid);

		// �֘A�e�[�u�����ǂݍ��܂�Ă��Ȃ�(�c���[�ɑ��݂��邪�J����Ă��Ȃ�)�t�B�[�`���̓`�F�b�N���Ȃ�
		if (m_RelNodes.find(lOid) == m_RelNodes.end())
			continue;

		// �֘A�m�[�h�̑Ó����`�F�b�N
		if (m_RelNodes[lOid].hasError()) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid);
			AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE), MB_OK | MB_TASKMODAL, NULL);
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrRoadSignPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_TabNum, bForce, bEditable );

	m_FeatureIndex = lFeatureIndex;
	m_TableIndex = lTableIndex;
	m_RowIndex = lRowIndex;
	m_IsForce = bForce;
	m_Editable = bEditable;

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_SOURCE));
	}

	IFeaturePtr roadsign = GetCurrentFeatureDef()->GetFeature();
	if (!roadsign)
		return FALSE;

	ITablePtr roadsignTable = AheGetTable(roadsign);
	if (!roadsignTable)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(roadsignTable, rel_lane_node_roadsign_point::kTableName);
	if (!relTable)
		return FALSE;
	ITablePtr nodeTable = AheOpenSameOwnerTable(roadsignTable, lane_node::kTableName);
	if (!nodeTable)
		return FALSE;


	// �܂��֘A�e�[�u����ǂݍ���ł��Ȃ����H�W���|�C���g�Ȃ�΃f�[�^��ǂݍ���
	// �c���[�r���[�ł̐؂�ւ����쎞�̂��߂ɕK�v�ȏ���
	roadsign->get_OID(&m_CurrentOid);
	if (m_RelNodes.count(m_CurrentOid) == 0)
	{
		CComVariant vaGid = AheGetAttribute(roadsign, kGlobalID);
		if (vaGid.vt != VT_BSTR)
			return FALSE;

		RelNodeInfos relInfo(relTable, nodeTable, vaGid.bstrVal);
		if (!relInfo.read())
			return FALSE;
		m_RelNodes[m_CurrentOid] = relInfo;
	}

	// �O���b�h�փf�[�^���Z�b�g
	auto oids = m_RelNodes[m_CurrentOid].getOIDs();
	m_Grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrRoadSignPointDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_ROADSIGN_C) || (nClassID == IDC_BUTTON_SEARCH))
	{
		if (!SetRoadsignCodeToComboBox(IDC_EDIT_ROADSIGN_C, IDC_COMBO_ROADSIGN_C))
			return 0;
	}

	return 0;
}


BOOL CAttrRoadSignPointDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_RelNodes[m_CurrentOid].isChange();
	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	DrawLinkageLine();
	return 0;
}

void CAttrRoadSignPointDlg::CreateControlRelation()
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

		m_cControlRel.SetControl(IDC_COMBO_ROADSIGN_C, TYPE_COMBO, 0, 0, roadsign_point::kRoadSignC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_ROADSIGN_C, TYPE_EDIT, 0, 0);
		m_cControlRel.SetControl(IDC_COMBO_ROADSIGN_C, TYPE_COMBO, 0, 0, roadsign_point::kRoadSignC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_HORIZONTAL_ANGLE, TYPE_EDITDOUBLE, 0, 0, roadsign_point::kHorizontalAngle, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_CHECK_VARIABLE_F, TYPE_CHECKBOX, 0, 0, roadsign_point::kVariableF, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, roadsign_point::kSperculationC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, roadsign_point::kSourceID, NULL, roadsign_point::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, roadsign_point::kSource, NULL, roadsign_point::kTableName, 0, -1, FOLLOW_READONLY);
	}
}

void CAttrRoadSignPointDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void CAttrRoadSignPointDlg::OnAfterEdit(LONG Row, LONG Col)
{
	// �O���b�h�̏���ێ����Ă���f�[�^���f
	auto oids = m_Grid.getData();

	std::vector<long> errOids;
	if (!m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		//m_Grid.deleteRow(errOids);
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_CurrentOid);
		AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE), MB_OK | MB_TASKMODAL, NULL);
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
}

void CAttrRoadSignPointDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	// �E�N���b�N�ȊO�͉����������Ȃ�
	if (!(Button & 0x02)) { return; }

	// �ҏW���J�n���������Ȃ�
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) { return; }

	// �}�E�X�A�b�v���̏����̓O���b�h�R���g���[���N���X�ֈϏ�
	m_Grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void CAttrRoadSignPointDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// �ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_lane_node_roadsign_point::kTableName);
	pRelTableDef->setChanged(m_RelNodes[m_CurrentOid].isChange());

	// �{�^������
	SetButton(Changed());
}

void CAttrRoadSignPointDlg::DrawLinkageLine()
{
	// �ĕ`��C�x���g�ł��̊֐����Ăяo���Ă���̂ŁA
	// �`�F�b�N����
	if (m_RelNodes.empty())
		return;

	// ���H�W���|�C���g�̃W�I���g���擾
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPointPtr roadsignPont(geo);
	if (!roadsignPont)
		return;

	ISpatialReferencePtr sr;
	roadsignPont->get_SpatialReference(&sr);

	// �|�C���g�̌`����擾
	auto nodePoints = m_RelNodes[m_CurrentOid].getNodePoints();

	for (const auto point : nodePoints)
	{
		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->putref_SpatialReference(sr);
		drawLine->put_FromPoint(roadsignPont);
		drawLine->put_ToPoint(point);

		DrawPolyLine(drawLine, LINE_COLOR);
	}
}

void CAttrRoadSignPointDlg::DrawPolyLine(const IPolylinePtr & polyline, const DWORD & color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}

bool CAttrRoadSignPointDlg::SetRoadsignCodeToComboBox(INT editControlId, INT comboBoxId)
{
	// EditControl �ɓ��͂��ꂽ�l���擾
	CEdit edit(GetDlgItem(editControlId));
	CString strSearchCode;
	edit.GetWindowText(strSearchCode);
	if (strSearchCode.IsEmpty())
		return true;

	CComboBox combo(GetDlgItem(comboBoxId));
	CFieldDef* fieldDef = GetCurrentFeatureDef()->GetFieldDef(roadsign_point::kTableName, roadsign_point::kRoadSignC);
	const auto domainMap = fieldDef->GetDomain();
	for (const auto& domain : *domainMap)
	{
		bool isMatched = false;
		// �R�[�h�l�ƌ����������S��v���Ă��邩�ǂ���������
		if (strSearchCode.CompareNoCase(domain.code.bstrVal) == 0) {
			isMatched = true;
		}

		// "�R�[�h�l����"�ƌ���������������v���Ă��邩�ۂ�������
		if (util::partialMatch(domain.code_discription, strSearchCode)) {
			isMatched = true;
		}

		if (isMatched) {
			combo.SetCurSel(domain.idx);
			m_cControlRel.ControlChanged(comboBoxId);
			SetButton(Changed());
			// �l�ύX�ɂ��R���{�{�b�N�X�̐F�ύX��L���ɂ��邽�ߍĕ`�悳����
			InvalidateRect(NULL, FALSE);
			break;
		}
	}

	return true;
}

// ���H�W���R�[�h��ʃ^�u�ŕҏW�\�ɂ���ꍇ�ɔ����āA
// �ȉ��̃R�[�h�̓R�����g�A�E�g�Ƃ��Ďc���Ă���
//void CAttrRoadSignPointDlg::ChangeWindowMgrMap( INT nIndex )
//{
//	switch( nIndex )
//	{
//	case EDITVIEW_ROADSIGN_KIHON:	
//		ChangeCtrlEnableToKihonTab();
//		m_winMgr.ChangeMgrMap( ROADSIGN_KIHON_MAP );
//		break;
//	case EDITVIEW_ROADSIGN_CLASS:
//		ChangeCtrlEnableToKiseiTab();
//		m_winMgr.ChangeMgrMap( ROADSIGN_CLASS_MAP );
//		break;
//	default:
//		break;
//	}
//
//	// �Ĕz�u
//	m_winMgr.InitToFitSizeFromCurrent(this);
//	m_winMgr.CalcLayout(this);
//	m_winMgr.SetWindowPositions(this);
//}

// ���H�W���R�[�h��ʃ^�u�ŕҏW�\�ɂ���ꍇ�ɔ����āA
// �ȉ��̃R�[�h�̓R�����g�A�E�g�Ƃ��Ďc���Ă���
//void CAttrRoadSignPointDlg::ChangeCtrlEnableToKihonTab()
//{
//	::ShowWindow(GetDlgItem(IDC_STATIC_HORIZONTAL_ANGLE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_CHECK_VARIABLE_F), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_STATIC_REL_LANE_NODE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_VSFLEXGRID_NODE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_STATIC_SPECULATION_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_COMBO_SPECULATION_C), SW_SHOW);
//
//	::ShowWindow(GetDlgItem(IDC_STATIC_ROADSIGN_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_COMBO_ROADSIGN_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_LIST_TYPE_ICON), SW_HIDE);
//}
//
//void CAttrRoadSignPointDlg::ChangeCtrlEnableToKiseiTab()
//{
//	::ShowWindow(GetDlgItem(IDC_STATIC_HORIZONTAL_ANGLE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_CHECK_VARIABLE_F), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_STATIC_REL_LANE_NODE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_VSFLEXGRID_NODE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_STATIC_SPECULATION_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_COMBO_SPECULATION_C), SW_HIDE);
//
//	::ShowWindow(GetDlgItem(IDC_STATIC_ROADSIGN_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_COMBO_ROADSIGN_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_LIST_TYPE_ICON), SW_SHOW);
//}
