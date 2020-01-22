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

// AttrCrosswalkLineDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include <boost/assign/list_of.hpp>

#include "AttrCrosswalkLineDlg.h"
#include "WinMap.h"


// �|�����C���`�掞�̐ݒ�
namespace{
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}


extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCrosswalkLineDlg();
}

CAttrCrosswalkLineDlg::CAttrCrosswalkLineDlg() 
	: CAttrBaseDlg(CROSSWALK_LINE_MAP,
	  IDD_ATTR_CROSSWALK_LINE_DIALOG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_WIDTH,  CContainedWindow(  _T("EDIT"), this, IDC_EDIT_WIDTH ) );
		( IDC_COMBO_SPECULATION_C,  CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) );
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) );
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

CAttrCrosswalkLineDlg::~CAttrCrosswalkLineDlg() 
{}

LRESULT CAttrCrosswalkLineDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �R���g���[���̃Z�b�g
	CreateControlRelation();

	// �O���b�h������
	m_Grid.init(GetDlgItem(IDC_VSFLEXGRID_NODE));

	// GridCtrl����̃C�x���g���擾
	AtlAdviseSinkMap(this, TRUE);

	// �T�u�N���X��
	for( auto& it : m_mapSubClass){
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}

	return CAttrBaseDlg::OnInitDialog( uMsg, wParam, lParam, bHandled );
}

LRESULT CAttrCrosswalkLineDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
	INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

	// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
	if( msg == EN_SETFOCUS )
	SelectDlgItem( nTargetControl );

	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		InvalidateRect( NULL, FALSE );
		SetButton( Changed() );
	}

	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//�n�}�ĕ`��
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_Grid.addRow();
	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_Grid.deleteSelectedRow();

	auto oids = m_Grid.getData();

	// �O���b�h�ł̍폜���f�[�^�ɔ��f
	std::vector<long> errOids;
	if( ! m_RelNodes[m_CurrentOid].reloadData(oids, errOids) )
	{
		// �G���[�ɂȂ邱�Ƃ͂Ȃ��͂������O�̂���
		AheMessageBox( NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), 
		               AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID ), MB_OK|MB_TASKMODAL, NULL );
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	
	return 0;
}

HWND CAttrCrosswalkLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return CAttrBaseDlg::Create(hWndParent,dwInitParam);
}

void CAttrCrosswalkLineDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	SetFocus(); 

	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

bool CAttrCrosswalkLineDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), crosswalk_line::kTableName ) != 0 )
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID( &lOid );

		if( ! m_RelNodes[lOid].isChange() )
			continue;

		if( ! m_RelNodes[lOid].updateTable() )
			return false;
	}
	return true;
}

BOOL CAttrCrosswalkLineDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), crosswalk_line::kTableName ) != 0 )
			continue;

		// OID �擾
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// �֘A�e�[�u�����ǂݍ��܂�Ă��Ȃ�(�c���[�ɑ��݂��邪�J����Ă��Ȃ�)�t�B�[�`���̓`�F�b�N���Ȃ�
		if( m_RelNodes.find(lOid) == m_RelNodes.end() )
			continue;

		// �֘A�m�[�h�̑Ó����`�F�b�N
		if( m_RelNodes[lOid].hasError() ) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid); 
			AheMessageBox( NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE ), MB_OK|MB_TASKMODAL, NULL );
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrCrosswalkLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = CAttrBaseDlg::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	IFeaturePtr crosswalkFeature = GetCurrentFeatureDef()->GetFeature();
	if (! crosswalkFeature)
		return FALSE;

	ITablePtr cwTable = AheGetTable(crosswalkFeature);
	if(! cwTable)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(cwTable, rel_lane_node_crosswalk_line::kTableName);
	if(! relTable)
		return FALSE;
	ITablePtr nodeTable = AheOpenSameOwnerTable(cwTable, lane_node::kTableName);
	if(! nodeTable )
		return FALSE;


	// �܂��֘A�e�[�u����ǂݍ���ł��Ȃ����f�����Ȃ�΃f�[�^��ǂݍ���
	// �c���[�r���[�ł̐؂�ւ����쎞�̂��߂ɕK�v�ȏ���
	crosswalkFeature->get_OID(&m_CurrentOid);
	if( m_RelNodes.count( m_CurrentOid ) == 0 )
	{
		CComVariant vaGid = AheGetAttribute(crosswalkFeature, kGlobalID);
		if( vaGid.vt != VT_BSTR )
			return FALSE;

		RelNodeInfos relInfo(relTable, nodeTable, vaGid.bstrVal);
		if(! relInfo.read())
			return FALSE;
		m_RelNodes[m_CurrentOid] = relInfo;
	}

	// �O���b�h�փf�[�^���Z�b�g
	auto oids = m_RelNodes[m_CurrentOid].getOIDs();
	m_Grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return bRet;
}

BOOL CAttrCrosswalkLineDlg::Changed()
{
	return CAttrBaseDlg::Changed() || m_RelNodes[m_CurrentOid].isChange();
}

LRESULT CAttrCrosswalkLineDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DrawLinkageLine();

	return 0;
}

void CAttrCrosswalkLineDlg::CreateControlRelation()
{
	using namespace sindy::schema::adam_v2;

	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		m_cControlRel.SetControl(IDC_EDIT_WIDTH,          TYPE_EDITDOUBLE,  0, 0, crosswalk_line::kWidth,        NULL, crosswalk_line::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO,       0, 0, crosswalk_line::kSperculationC, NULL, crosswalk_line::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID,      TYPE_EDITSTRING,  0, 0, crosswalk_line::kSourceID,      NULL, crosswalk_line::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE,         TYPE_EDITSTRING,  0, 0, crosswalk_line::kSource,        NULL, crosswalk_line::kTableName);
	}
}

void __stdcall CAttrCrosswalkLineDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{
	// �ҏW���J�n���̓O���b�h�̕ҏW�s��
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void __stdcall CAttrCrosswalkLineDlg::OnAfterEdit(LONG Row, LONG Col)
{ 
	// �O���b�h�̏���ێ����Ă���f�[�^���f
	auto oids = m_Grid.getData();
	
	std::vector<long> errOids;
	if(! m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		//m_Grid.deleteRow(errOids);
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_CurrentOid); 
		AheMessageBox( NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE ), MB_OK|MB_TASKMODAL, NULL );
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
}

void __stdcall CAttrCrosswalkLineDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	using namespace sindy::schema::adam_v2;

	// �E�N���b�N�ȊO�͉����������Ȃ�
	if (! (Button & 0x02)) { return; }

	// �ҏW���J�n���������Ȃ�
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()){ return; }

	// �}�E�X�A�b�v���̏����̓O���b�h�R���g���[���N���X�ֈϏ�
	m_Grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void __stdcall CAttrCrosswalkLineDlg::OnCellChangedGrid()
{
	//DrawSelectedNode();
}

void CAttrCrosswalkLineDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// �ύX��ԍX�V
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_lane_node_crosswalk_line::kTableName);
	pRelTableDef->setChanged( m_RelNodes[m_CurrentOid].isChange() );

	// �{�^������
	SetButton( Changed() );
}

void CAttrCrosswalkLineDlg::DrawLinkageLine()
{
	// �ĕ`��C�x���g�ł��̊֐����Ăяo���Ă���̂ŁA
	// �`�F�b�N����
	if( m_RelNodes.empty() )
		return;

	// ���f�������C���̃W�I���g���擾
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPolylinePtr cwPolyline(geo);
	if(! cwPolyline)
		return;

	// �|�C���g�̌`����擾
	auto nodePoints = m_RelNodes[m_CurrentOid].getNodePoints();

	for( auto point : nodePoints )
	{
		// ���C����ň�ԋ߂��_���擾���A���C�����쐬
		// �ŋߖT�_���擾
		IPointPtr nearestPoint;
	    if( FAILED((( IProximityOperatorPtr)cwPolyline)->ReturnNearestPoint( point, esriSegmentExtension::esriNoExtension, &nearestPoint )))
			continue;

		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->put_FromPoint(nearestPoint);
		drawLine->put_ToPoint(point);

		DrawPolyLine(drawLine, LINE_COLOR);
	}
}

void CAttrCrosswalkLineDlg::DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}
