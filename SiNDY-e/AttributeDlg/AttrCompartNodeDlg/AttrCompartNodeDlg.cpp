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

// AttrCompartNodeDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrCompartNodeDlg.h"
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
 * @retval CAttrCompartNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCompartNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCompartNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCompartNodeDlg::CAttrCompartNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCompartNodeDlg::CAttrCompartNodeDlg() 
	: ATTR_BASE_CLASS(COMPART_NODE_MAP, IDD_COMPARTNODEDLG),
	  m_currentOid(0)
{
	// TODO[SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODECLASS_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_NODECLASS_C ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_STATIC_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) );
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

void CAttrCompartNodeDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// �O���b�h�񂲂Ƃ̕ҏW�ېݒ�̓O���b�h�N���X���ֈϏ�
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

void CAttrCompartNodeDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

void CAttrCompartNodeDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);
	updateChanged();
}

/**
 * @fn HWND CAttrCompartNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCompartNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrCompartNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCompartNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrCompartNodeDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), compart_node::kTableName) != 0)
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
 * @fn BOOL CAttrCompartNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCompartNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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

BOOL CAttrCompartNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

/**
 * @fn void CAttrCompartNodeDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCompartNodeDlg::CreateControlRelation()
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
		m_cControlRel.SetControl( IDC_COMBO_NODECLASS_C, TYPE_COMBO, 0, 0, compart_node::kNodeClassC, 0, compart_node::kTableName );
		// �����R�[�h
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, compart_node::kSperculationC, 0, compart_node::kTableName );
		// �񋟌�ID
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, compart_node::kSourceID, 0, compart_node::kTableName, 0, -1, FOLLOW_READONLY);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, compart_node::kTableName );
	}
}

void CAttrCompartNodeDlg::createGrid()
{
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	if (!feature)
		return;

	ITablePtr nodeTable;
	feature->get_Table(&nodeTable);
	if (!nodeTable) {
		return;
	}
	ITablePtr nqTable = AheOpenSameOwnerTable(nodeTable, nq_compart_node::kTableName);
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
		builder.addField(nq_compart_node::kObjectID)
			.addField(nq_compart_node::kSequence)
			.addField(nq_compart_node::kWidth);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[oid] = nqGrid;
	}
	
	// �O���b�h������
	m_currentOid = oid;
	m_nqDataGridMap[m_currentOid].init();

	// �O���b�h�̕ҏW�\�t�B�[���h��ݒ�
	const static std::set<CString> editableFields = boost::assign::list_of(CString(nq_compart_node::kWidth));
	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	// �I�[�t���O�������Ă�����̂̓O���b�h�֕\�����Ȃ�
	CComVariant compartNodeGid = AheGetAttribute(feature, compart_node::kGlobalID);
	if (compartNodeGid.vt == VT_NULL) {
		return;
	}
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_compart_node::kCompartNodeGID, compartNodeGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

void CAttrCompartNodeDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

void CAttrCompartNodeDlg::highlightLine()
{
	if (m_nqDataGridMap.empty())
		return;

	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h���擾
	_IRowPtr nqRow = m_nqDataGridMap[m_currentOid].getSelectedRow();
	// �_�C�A���O�ŕ\�����Ă�������m�[�h�t�B�[�`�����擾
	IFeaturePtr compartNodeFeature = GetCurrentFeatureDef()->GetFeature();
	if (!compartNodeFeature)
		return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","���������NGID"���擾����
	ITablePtr compartNodeT;
	compartNodeFeature->get_Table(&compartNodeT);
	if (!compartNodeT)
		return;
	ITablePtr nqCompartNodeT = AheOpenSameOwnerTable(compartNodeT, nq_compart_node::kTableName);
	if (!nqCompartNodeT)
		return;

	CComVariant compartLinkGid = AheGetAttribute(nqRow, nq_compart_node::kCompartLinkGID);
	if (compartLinkGid.vt == VT_NULL)
		return;
	long sequence = AheGetAttribute(nqRow, nq_compart_node::kSequence).lVal;


	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h�̎��̃m�[�h���擾����
	//   �m�[�h�񂩂玟�̃m�[�h�ɂ����郌�R�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s' and %s=%d"),
		nq_compart_node::kCompartLinkGID, compartLinkGid.bstrVal,
		nq_compart_node::kSequence, sequence + 1);

	_ICursorPtr nqCur;
	if (FAILED(nqCompartNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &nqCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	_IRowPtr nqNextRow;
	if (nqCur->NextRow(&nqNextRow) != S_OK)
		return;
	if (!nqNextRow)
		return;
	CComVariant nextCompartNodeGid = AheGetAttribute(nqNextRow, nq_compart_node::kCompartNodeGID);
	if (nextCompartNodeGid.vt == VT_NULL)
		return;

	//   �������ꂽ�m�[�h��̋����m�[�hGID���玟�̃m�[�h������
	whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nextCompartNodeGid.bstrVal);
	_ICursorPtr compartNodeCur;
	if (FAILED(compartNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &compartNodeCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr compartNodeNextFeature;
	if (IFeatureCursorPtr(compartNodeCur)->NextFeature(&compartNodeNextFeature) != S_OK)
		return;
	if (!compartNodeNextFeature)
		return;


	// �_�C�A���O�ŕ\�����Ă�������m�[�h�Ǝ��̋����m�[�h�����ԃ��C�����쐬����
	IGeometryPtr fromPt, toPt;
	compartNodeFeature->get_ShapeCopy(&fromPt);
	compartNodeNextFeature->get_ShapeCopy(&toPt);

	ISpatialReferencePtr sr;
	fromPt->get_SpatialReference(&sr);

	IPolylinePtr drawLine(CLSID_Polyline);
	drawLine->putref_SpatialReference(sr);
	drawLine->put_FromPoint(IPointPtr(fromPt));
	drawLine->put_ToPoint(IPointPtr(toPt));

	// ���C���`��
	AheDrawPolyline(m_cArcHelper.GetDisplay(), drawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);
}
