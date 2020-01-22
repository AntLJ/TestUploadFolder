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

// AttrLaneNodeDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrLaneNodeDlg.h"
#include <boost/assign/list_of.hpp>

#include "WinMap.h"

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace sindy::schema::category;

namespace {
	const CString UNDEF_NAME = _T("���ݒ�");
	const long UNDEF_OID = 0;
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}


/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrLaneNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrLaneNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrLaneNodeDlg::CAttrLaneNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrLaneNodeDlg::CAttrLaneNodeDlg() 
	: ATTR_BASE_CLASS(LANE_NODE_MAP, IDD_LANENODEDLG),
	m_currentOid(0)
{
	// [SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODECLASS_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_NODECLASS_C ) )
		( IDC_COMBO_POSITION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_POSITION_C ) )
		( IDC_CHECK_STOP_POINT_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_STOP_POINT_F ) )
		( IDC_CHECK_COVERING_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_COVERING_F ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID ) )

		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );

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

LRESULT CAttrLaneNodeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAttrLaneNodeDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAttrLaneNodeDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrLaneNodeDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	highlightLine();
	return 0;
}

LRESULT CAttrLaneNodeDlg::OnUpdateCrossName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (::IsWindow(hWndCtl))
	{
		CEdit edit(hWndCtl);
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca(nTextLen * sizeof(TCHAR));
		edit.GetWindowText(lpszText, nTextLen);

		// <null> �̏ꍇ���l��
		if (lstrcmp(lpszText, NULL_VALUE) != 0)
		{
			CString strName = AheConvertToIntersectionName(lpszText, TRUE);

			if (strName.Compare(lpszText) != 0)
			{
				INT dwSelStart = 0, dwSelEnd = 0;
				edit.GetSel(dwSelStart, dwSelEnd);
				edit.SetWindowText(strName);
				// ���͈ʒu�̏C��
				INT nDiff = CString_GetLength2(CString(lpszText)) - CString_GetLength2(strName);
				edit.SetSel(dwSelStart - nDiff, dwSelEnd - nDiff);
			}
		}
		m_cControlRel.ControlChanged(wID);
		SetButton(Changed());
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}

LRESULT CAttrLaneNodeDlg::OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (::IsWindow(hWndCtl))
	{
		CEdit edit(hWndCtl);
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca(nTextLen * sizeof(TCHAR));
		edit.GetWindowText(lpszText, nTextLen);


		// <null> �̏ꍇ���l��
		if (lstrcmp(lpszText, NULL_VALUE) != 0)
		{
			CString strYomi = AheConvertToIntersectionName(lpszText, FALSE);
			if (strYomi.Compare(lpszText) != 0)
			{
				INT nSelStart = 0, nSelEnd = 0;
				edit.GetSel(nSelStart, nSelEnd);
				edit.SetWindowText(strYomi);
				// ���͈ʒu�̏C��
				INT nDiff = CString_GetLength2(CString(lpszText)) - CString_GetLength2(strYomi);
				edit.SetSel(nSelStart - nDiff, nSelEnd - nDiff);
			}
		}
		m_cControlRel.ControlChanged(wID);
		SetButton(Changed());
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}

void CAttrLaneNodeDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// �O���b�h�񂲂Ƃ̕ҏW�ېݒ�̓O���b�h�N���X���ֈϏ�
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

void CAttrLaneNodeDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

void CAttrLaneNodeDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);
	updateChanged();
}

/** 
 * @fn HWND CAttrLaneNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrLaneNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrLaneNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrLaneNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrLaneNodeDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), lane_node::kTableName) != 0)
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
 * @fn BOOL CAttrLaneNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrLaneNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// [SiNDYChildDlg] �ŏ��Ƀt�H�[�J�X�����R���g���[��
		// ���́E�ǂ݂�NULL�Ȃ̂ŁA�Ԑ��ԍ����ŏ��ɂ��Ă���
		//::SetFocus( GetDlgItem(IDC_EDIT_LANE_NUMBER) );
	}

	createGrid();

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrLaneNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

/**
 * @fn void CAttrLaneNodeDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrLaneNodeDlg::CreateControlRelation()
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
		m_cControlRel.SetControl( IDC_COMBO_NODECLASS_C,		TYPE_COMBO,			0, 0,	lane_node::kNodeClassC,			0,	lane_node::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
		m_cControlRel.SetControl( IDC_COMBO_POSITION_C,			TYPE_COMBO,			0, 0,	lane_node::kPositionC,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_STOP_POINT_F,		TYPE_CHECKBOX,		0, 0,	lane_node::kStopPointF,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_COVERING_F,			TYPE_CHECKBOX,		0, 0,	lane_node::kCoveringF,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			0, 0,	lane_node::kSperculationC,		0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,           TYPE_EDITSTRING,	0, 0,	lane_node::kSourceID,			0,	lane_node::kTableName,  0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p

		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,	0, 0,	lane_node::kSource,			0,	lane_node::kTableName, 0, -1, FOLLOW_READONLY);	//�\���݂̂̂��ߏ�ɓǂݎ���p
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
void CAttrLaneNodeDlg::SetToolTip(INT nTargetItem)
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
 */
BOOL CAttrLaneNodeDlg::ErrorCheck()
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

void CAttrLaneNodeDlg::createGrid()
{
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	if (!feature)
		return;

	ITablePtr table;
	feature->get_Table(&table);
	if (!table)
		return;

	ITablePtr nqTable = AheOpenSameOwnerTable(table, nq_lane_node::kTableName);
	if (!nqTable)
		return;

	long oid(0);
	feature->get_OID(&oid);

	// �n�߂ă_�C�A���O�ɕ\�������t�B�[�`���ł���΁A�m�[�h��O���b�h��o�^
	if (m_nqDataGridMap.count(oid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(nqTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NQ));
		builder.addField(nq_lane_node::kObjectID)
			.addField(nq_lane_node::kSequence)
			.addField(nq_lane_node::kWidth)
			.addField(nq_lane_node::kLinearElement)
			.addField(nq_lane_node::kCurvature)
			.addField(nq_lane_node::kCurvatureRadius)
			.addField(nq_lane_node::kCurvatureChangeRate)
			.addField(nq_lane_node::kClothoidParameter)
			.addField(nq_lane_node::kLongitudinalSlope)
			.addField(nq_lane_node::kCrossSlope);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[oid] = nqGrid;
	}

	// �O���b�h������
	m_currentOid = oid;
	m_nqDataGridMap[m_currentOid].init();

	// �O���b�h�̕ҏW�\�t�B�[���h��ݒ�
	const static std::set<CString> editableFields = boost::assign::list_of(CString(nq_lane_node::kWidth));
	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);

	// �m�[�h��f�[�^���O���b�h�֓ǂݍ���
	// �I�[�t���O�������Ă�����̂̓O���b�h�֕\�����Ȃ�
	CComVariant laneNodeGid = AheGetAttribute(feature, lane_node::kGlobalID);
	if (laneNodeGid.vt == VT_NULL) {
		return;
	}
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_lane_node::kLaneNodeGID, laneNodeGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

void CAttrLaneNodeDlg::updateChanged()
{
	// �֘A�e�[�u��(�m�[�h��)�̕ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// �{�^������
	SetButton(Changed());
}

void CAttrLaneNodeDlg::highlightLine()
{
	if (m_nqDataGridMap.empty())
		return;

	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h���擾
	_IRowPtr nqRow = m_nqDataGridMap[m_currentOid].getSelectedRow();
	if (!nqRow)
		return;

	// �_�C�A���O�ŕ\�����Ă���Ԑ��m�[�h�t�B�[�`�����擾
	IFeaturePtr laneNodeFeature = GetCurrentFeatureDef()->GetFeature();
	if (!laneNodeFeature)
		return;

	// �O���b�h�őI���̃m�[�h�񃌃R�[�h����A"����","�Ԑ������NGID"���擾����
	ITablePtr laneNodeT;
	laneNodeFeature->get_Table(&laneNodeT);
	if (!laneNodeT)
		return;
	ITablePtr nqLaneNodeT = AheOpenSameOwnerTable(laneNodeT, nq_lane_node::kTableName);
	if (!nqLaneNodeT)
		return;

	CComVariant laneLinkGid = AheGetAttribute(nqRow, nq_lane_node::kLaneLinkGID);
	if (laneLinkGid.vt == VT_NULL)
		return;
	long sequence = AheGetAttribute(nqRow, nq_lane_node::kSequence).lVal;


	// �O���b�h�I���s�̃m�[�h�񃌃R�[�h�̎��̃m�[�h���擾����
	//   �m�[�h�񂩂玟�̃m�[�h�ɂ����郌�R�[�h������
	CString whereClause = uh::str_util::format(_T("%s='%s' and %s=%d"),
		nq_lane_node::kLaneLinkGID, laneLinkGid.bstrVal,
		nq_lane_node::kSequence, sequence + 1);
	
	_ICursorPtr nqCur;
	if (FAILED(nqLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &nqCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	_IRowPtr nqNextRow;
	if (nqCur->NextRow(&nqNextRow) != S_OK)
		return;
	if (!nqNextRow)
		return;
	CComVariant nextLaneNodeGid = AheGetAttribute(nqNextRow, nq_lane_node::kLaneNodeGID);
	if (nextLaneNodeGid.vt == VT_NULL)
		return;

	//   �������ꂽ�m�[�h��̎Ԑ��m�[�hGID���玟�̃m�[�h������
	whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, nextLaneNodeGid.bstrVal);
	_ICursorPtr laneNodeCur;
	if (FAILED(laneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &laneNodeCur)))
		return;
	//   1���R�[�h������������Ȃ��͂��Ȃ̂ŁA���̑O��ŏ�������
	IFeaturePtr laneNodeNextFeature;
	if (IFeatureCursorPtr(laneNodeCur)->NextFeature(&laneNodeNextFeature) != S_OK)
		return;
	if (!laneNodeNextFeature)
		return;


	// �_�C�A���O�ŕ\�����Ă���Ԑ��m�[�h�Ǝ��̎Ԑ��m�[�h�����ԃ��C�����쐬����
	IGeometryPtr fromPt, toPt;
	laneNodeFeature->get_ShapeCopy(&fromPt);
	laneNodeNextFeature->get_ShapeCopy(&toPt);

	ISpatialReferencePtr sr;
	fromPt->get_SpatialReference(&sr);

	IPolylinePtr drawLine(CLSID_Polyline);
	drawLine->putref_SpatialReference(sr);
	drawLine->put_FromPoint(IPointPtr(fromPt));
	drawLine->put_ToPoint(IPointPtr(toPt));

	// ���C���`��i�`�撆�̃��C���͏���)
	AheDrawPolyline(m_cArcHelper.GetDisplay(), drawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);
}
