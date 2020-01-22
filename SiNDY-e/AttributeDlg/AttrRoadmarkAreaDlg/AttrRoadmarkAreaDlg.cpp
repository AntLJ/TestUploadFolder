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
#include "AttrRoadmarkAreaDlg.h"
#include <boost/assign/list_of.hpp>
#include "Util.h"

using namespace sindy::schema;
using namespace adam_v2;

#include "WinMap.h"

// �|�����C���`�掞�̐ݒ�
namespace {
	const COLORREF Color_Guideline = RGB(0xC0, 0xC0, 0x80);
	const COLORREF Color_LinkHighlight = RGB(0xD7, 0x1D, 0x3B);
}

extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrRoadmarkAreaDlg();
}

CAttrRoadmarkAreaDlg::CAttrRoadmarkAreaDlg() 
	: ATTR_BASE_CLASS(ROADMARK_AREA_MAP, IDD_ROADMARKAREADLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_ROADMARK_C,      CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_ROADMARK_C))
		(IDC_EDIT_DESCRIPTION,      CContainedWindow(_T("EDIT"), this, IDC_EDIT_DESCRIPTION))
		(IDC_COMBO_SPECULATION_C,   CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C))
		(IDC_EDIT_SOURCE_ID,        CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		(IDC_EDIT_SOURCE,           CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE ))
		;
}

LRESULT CAttrRoadmarkAreaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R���g���[���̃Z�b�g
	CreateControlRelation();

	// �O���b�h������
	m_grid.init(GetDlgItem(IDC_VSFLEXGRID_LANE_LINK));

	// GridCtrl����̃C�x���g���擾
	AtlAdviseSinkMap(this, TRUE);

	// �T�u�N���X��
	for (auto& it : m_mapSubClass) {
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	return ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrRoadmarkAreaDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAttrRoadmarkAreaDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_grid.addRow();
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_grid.deleteSelectedRow();

	auto oids = m_grid.getData();

	// �O���b�h�ł̍폜���f�[�^�ɔ��f
	std::vector<long> errOids;
	if (!m_relLinks[m_currentOid].reloadData(oids, errOids))
	{
		// �G���[�ɂȂ邱�Ƃ͂Ȃ��͂������O�̂���
		AheMessageBox(NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID),
			AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), MB_OK | MB_TASKMODAL, NULL);
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);

	return 0;
}

HWND CAttrRoadmarkAreaDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent, dwInitParam);
}

void CAttrRoadmarkAreaDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	SetFocus();

	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrRoadmarkAreaDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadmark_area::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_relLinks[lOid].isChange())
			continue;

		if (!m_relLinks[lOid].updateTable())
			return false;
	}
	return true;
}

BOOL CAttrRoadmarkAreaDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadmark_area::kTableName) != 0)
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID(&lOid);

		// �֘A�e�[�u�����ǂݍ��܂�Ă��Ȃ�(�c���[�ɑ��݂��邪�J����Ă��Ȃ�)�t�B�[�`���̓`�F�b�N���Ȃ�
		if (m_relLinks.count(lOid) == 0)
			continue;

		// �֘A�����N�̑Ó����`�F�b�N
		if (m_relLinks[lOid].hasError()) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid);
			AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_LINK), MB_OK | MB_TASKMODAL, NULL);
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrRoadmarkAreaDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0,  bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_SOURCE));
	}

	IFeaturePtr roadmark = GetCurrentFeatureDef()->GetFeature();
	if (!roadmark)
		return FALSE;

	ITablePtr roadmarkT = AheGetTable(roadmark);
	if (!roadmarkT)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(roadmarkT, rel_roadmark_area_lane_link::kTableName);
	if (!relTable)
		return FALSE;
	ITablePtr linkTable = AheOpenSameOwnerTable(roadmarkT, lane_link::kTableName);
	if (!linkTable)
		return FALSE;


	// �܂��֘A�e�[�u����ǂݍ���ł��Ȃ����H�W���G���A�Ȃ�΃f�[�^��ǂݍ���
	// �c���[�r���[�ł̐؂�ւ����쎞�̂��߂ɕK�v�ȏ���
	roadmark->get_OID(&m_currentOid);
	if (m_relLinks.count(m_currentOid) == 0)
	{
		CComVariant vaGid = AheGetAttribute(roadmark, kGlobalID);
		if (vaGid.vt != VT_BSTR)
			return FALSE;

		RelLinkInfos relInfo(relTable, linkTable, vaGid.bstrVal);
		if (!relInfo.read())
			return FALSE;
		m_relLinks[m_currentOid] = relInfo;
	}

	// �O���b�h�փf�[�^���Z�b�g
	auto oids = m_relLinks[m_currentOid].getOIDs();
	m_grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrRoadmarkAreaDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_relLinks[m_currentOid].isChange();
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	DrawLinkageLine();
	return 0;
}

void CAttrRoadmarkAreaDlg::CreateControlRelation()
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
		m_cControlRel.SetControl(IDC_COMBO_ROADMARK_C, TYPE_COMBO, 0, 0, roadmark_area::kRoadMarkC, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_DESCRIPTION, TYPE_EDITSTRING, 0, 0, roadmark_area::kDescription, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, roadmark_area::kSperculationC, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, roadmark_area::kSourceID, NULL, roadmark_area::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, roadmark_area::kSource, NULL, roadmark_area::kTableName);
	}
}

void CAttrRoadmarkAreaDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void CAttrRoadmarkAreaDlg::OnAfterEdit(LONG Row, LONG Col)
{
	// �O���b�h�̏���ێ����Ă���f�[�^���f
	auto oids = m_grid.getData();

	std::vector<long> errOids;
	if (!m_relLinks[m_currentOid].reloadData(oids, errOids))
	{
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_currentOid);
		AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_LINK), MB_OK | MB_TASKMODAL, NULL);
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
}

void CAttrRoadmarkAreaDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	// �E�N���b�N�ȊO�͉����������Ȃ�
	if (!(Button & 0x02)) { return; }

	// �ҏW���J�n���������Ȃ�
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) { return; }

	// �}�E�X�A�b�v���̏����̓O���b�h�R���g���[���N���X�ֈϏ�
	m_grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void CAttrRoadmarkAreaDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// �ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_roadmark_area_lane_link::kTableName);
	pRelTableDef->setChanged(m_relLinks[m_currentOid].isChange());

	// �{�^������
	SetButton(Changed());
}

void CAttrRoadmarkAreaDlg::DrawLinkageLine()
{
	// �ĕ`��C�x���g�ł��̊֐����Ăяo���Ă���̂ŁA
	// �`�F�b�N����
	if (m_relLinks.empty())
		return;

	// ���H�W���G���A�̃W�I���g���擾
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPolygonPtr roadmarkPolygon(geo);
	if (!roadmarkPolygon)
		return;

	ISpatialReferencePtr sr;
	roadmarkPolygon->get_SpatialReference(&sr);

	// �R�Â������N�̌`��Q���擾
	auto linkPolylines = m_relLinks[m_currentOid].getLinkPolylines();

	for (const auto polyline : linkPolylines)
	{
		IPointPtr fromPoint, toPoint;
		if (!util::getNearestPoint(roadmarkPolygon, polyline, fromPoint, toPoint))
			continue;

		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->putref_SpatialReference(sr);
		drawLine->put_FromPoint(fromPoint);
		drawLine->put_ToPoint(toPoint);

		DrawPolyLine(drawLine, Color_Guideline);
		DrawPolyLine(polyline, Color_LinkHighlight);
	}
}

void CAttrRoadmarkAreaDlg::DrawPolyLine(const IPolylinePtr & polyline, const DWORD & color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}
