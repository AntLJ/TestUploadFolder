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

// AttrBorderNodeDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrBorderNodeDlg.h"
#include <boost/assign/list_of.hpp>

#include "WinMap.h"

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

namespace {
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrBorderNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrBorderNodeDlg();
}

CAttrBorderNodeDlg::CAttrBorderNodeDlg() 
	: ATTR_BASE_CLASS(BORDER_NODE_MAP, IDD_BORDERNODEDLG),
	  m_currentOid(0)
{
	// TODO[SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODECLASS_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_NODECLASS_C ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_STATIC_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) );
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}


LRESULT CAttrBorderNodeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R���g���[��
	CreateControlRelation();

	// �O���b�h�R���g���[������̃C�x���g���擾
	AtlAdviseSinkMap(this, TRUE);

	// �T�u�N���X��
	for (auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	return ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrBorderNodeDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if (!m_bFinishInit) return 0;

	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if (msg == EN_SETFOCUS)
		SelectDlgItem(nTargetControl);
	// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
	if (msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);

	// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
	if ((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
		m_cControlRel.ControlChanged((INT)LOWORD(wParam));
		// �_�C�A���O���X�V�̈�ɒǉ����܂�
		InvalidateRect(NULL, FALSE);
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton(Changed());
	}

	return 0;
}

LRESULT CAttrBorderNodeDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrBorderNodeDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	highlightLine();
	return 0;
}

void CAttrBorderNodeDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// �O���b�h�񂲂Ƃ̕ҏW�ېݒ�̓O���b�h�N���X���ֈϏ�
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

void CAttrBorderNodeDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

void CAttrBorderNodeDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);
	updateChanged();
}

/**
 * @fn HWND CAttrBorderNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBorderNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrBorderNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBorderNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrBorderNodeDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), border_node::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_nqDataGridMap[lOid].isChanged())
			continue;

		if (!m_nqDataGridMap[lOid].store())
			return false;
	}
	return true;
}

/**
 * @fn BOOL CAttrBorderNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBorderNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	createGrid();

	// �n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrBorderNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

/**
 * @fn void CAttrBorderNodeDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBorderNodeDlg::CreateControlRelation()
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

		// �m�[�h��ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_NODECLASS_C, TYPE_COMBO, 0, 0, border_node::kNodeClassC, 0, border_node::kTableName );
		// �����R�[�h
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, border_node::kSperculationC, 0, border_node::kTableName );
		// �񋟌�ID
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, border_node::kSourceID, 0, border_node::kTableName, 0, -1, FOLLOW_READONLY);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, border_node::kTableName );
	}
}

void CAttrBorderNodeDlg::createGrid()
{
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	if (!feature)
		return;

	ITablePtr nodeTable;
	feature->get_Table(&nodeTable);
	if (!nodeTable) {
		return;
	}
	ITablePtr nqTable = AheOpenSameOwnerTable(nodeTable, nq_border_node::kTableName);
	if (!nqTable) {
		return;
	}

	long oid(0);
	feature->get_OID(&oid);

	// �n�߂ă_�C�A���O�ɕ\�������t�B�[�`���ł���΁A�m�[�h��O���b�h��o�^
	if (m_nqDataGridMap.count(oid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(nqTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NQ));
		builder.addField(nq_border_node::kObjectID)
			.addField(nq_border_node::kSequence);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[oid] = nqGrid;
	}
	
	// �O���b�h������
	m_currentOid = oid;
	m_nqDataGridMap[m_currentOid].init();

	// �O���b�h�̕ҏW�\�t�B�[���h�͐ݒ肵�Ȃ�

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	// �I�[�t���O�������Ă�����̂̓O���b�h�֕\�����Ȃ�
	CComVariant borderNodeGid = AheGetAttribute(feature, border_node::kGlobalID);
	if (borderNodeGid.vt == VT_NULL) {
		return;
	}
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_border_node::kBorderNodeGID, borderNodeGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

void CAttrBorderNodeDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_border_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

void CAttrBorderNodeDlg::highlightLine()
{
	if (m_nqDataGridMap.empty())
		return;

	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h���擾
	_IRowPtr nqRow = m_nqDataGridMap[m_currentOid].getSelectedRow();
	// �_�C�A���O�ŕ\�����Ă��铹�H���E���m�[�h�t�B�[�`�����擾
	IFeaturePtr borderNodeFeature = GetCurrentFeatureDef()->GetFeature();
	if (!borderNodeFeature)
		return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","���H���E�������NGID"���擾����
	ITablePtr borderNodeT;
	borderNodeFeature->get_Table(&borderNodeT);
	if (!borderNodeT)
		return;
	ITablePtr nqBorderNodeT = AheOpenSameOwnerTable(borderNodeT, nq_border_node::kTableName);
	if (!nqBorderNodeT)
		return;

	CComVariant borderLinkGid = AheGetAttribute(nqRow, nq_border_node::kBorderLinkGID);
	if (borderLinkGid.vt == VT_NULL)
		return;
	long sequence = AheGetAttribute(nqRow, nq_border_node::kSequence).lVal;


	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h�̎��̃m�[�h���擾����
	//   �m�[�h�񂩂玟�̃m�[�h�ɂ����郌�R�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s' and %s=%d"),
		nq_border_node::kBorderLinkGID, borderLinkGid.bstrVal,
		nq_border_node::kSequence, sequence + 1);

	_ICursorPtr nqCur;
	if (FAILED(nqBorderNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &nqCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	_IRowPtr nqNextRow;
	if (nqCur->NextRow(&nqNextRow) != S_OK)
		return;
	if (!nqNextRow)
		return;
	CComVariant nextBorderNodeGid = AheGetAttribute(nqNextRow, nq_border_node::kBorderNodeGID);
	if (nextBorderNodeGid.vt == VT_NULL)
		return;

	//   �������ꂽ�m�[�h��̓��H���E���m�[�hGID���玟�̃m�[�h������
	whereClause = uh::str_util::format(_T("%s='%s'"), border_node::kGlobalID, nextBorderNodeGid.bstrVal);
	_ICursorPtr borderNodeCur;
	if (FAILED(borderNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &borderNodeCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr borderNodeNextFeature;
	if (IFeatureCursorPtr(borderNodeCur)->NextFeature(&borderNodeNextFeature) != S_OK)
		return;
	if (!borderNodeNextFeature)
		return;


	// �_�C�A���O�ŕ\�����Ă��铹�H���E���m�[�h�Ǝ��̓��H���E���m�[�h�����ԃ��C�����쐬����
	IGeometryPtr fromPt, toPt;
	borderNodeFeature->get_ShapeCopy(&fromPt);
	borderNodeNextFeature->get_ShapeCopy(&toPt);

	ISpatialReferencePtr sr;
	fromPt->get_SpatialReference(&sr);

	IPolylinePtr drawLine(CLSID_Polyline);
	drawLine->putref_SpatialReference(sr);
	drawLine->put_FromPoint(IPointPtr(fromPt));
	drawLine->put_ToPoint(IPointPtr(toPt));

	// ���C���`��
	AheDrawPolyline(m_cArcHelper.GetDisplay(), drawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);
}
