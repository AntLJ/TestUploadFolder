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

// AttrSubwayNodeDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrSubwayNodeDlg.h"
#include "globalfunc.h"
#include <boost/assign/list_of.hpp>
#include "Map.h"
#include <TDC/useful_headers/str_util.h>

using namespace sindy::schema;

// TODO[SiNDYChildDlg] �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(NODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NODECLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CODECLASS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_STATIONNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STATIONNAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINENAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINENAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_STATIONNUM)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STATIONNUM)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DUMMY)
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_STATIONLIST)
					RCTOFIT(-1)
					RCREST(IDC_LIST_STATION)
				ENDGROUP()
				BEGINROWS(WRCT_REST, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_LINELIST)
					RCTOFIT(-1)
					RCREST(IDC_LIST_LINE)
				ENDGROUP()
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_DUMMY2)
					RCREST(-1)
					RCTOFIT(IDC_BUTTON_APPLY)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()


BEGIN_WINDOW_MAP(LINK_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINENAME2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINENAME2)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrSubwayNodeDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrSubwayNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSubwayNodeDlg::CAttrSubwayNodeDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrSubwayNodeDlg::CAttrSubwayNodeDlg() 
	: ATTR_BASE_CLASS(NODE_MAP, IDD_SUBWAYNODEDLG)
{
	// TODO[SiNDYChildDlg] �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
		( IDC_EDIT_STATIONNUM, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_STATIONNUM ) );
		( IDC_COMBO_CODECLASS, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_CODECLASS ) );
}

/** 
 * @fn HWND CAttrSubwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrSubwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrSubwayNodeDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrSubwayNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

CTargetItems& CAttrSubwayNodeDlg::GetConnectedLink(IFeaturePtr nodePtr)
{
	if( !m_connectedLink.empty() )
		return m_connectedLink;

	AheGetConnectedNetworkLink( nodePtr, m_connectedLink );

	// �V�K�쐬���͐ڑ������N������������Ȃ�
	if( m_connectedLink.empty() )
	{
		// �c���[�ɂ͕K�������N��1��������͂�
		long node_id = AheGetOID( nodePtr );
		for( auto& feature : *m_pFeatureDefList )
		{
			if(0 == _tcsicmp( sj::subway_link::kTableName, feature.GetTableName() ))
			{
				m_connectedLink.push_back( CTargetItem( feature.GetFeature() ) );
				break;
			}
		}
	}
	return m_connectedLink;
}

IFeaturePtr CAttrSubwayNodeDlg::GetCorrespNode( long node_id, const IFeaturePtr& linkPtr )
{
	CString to_or_from = 
		( AheGetAttribute( linkPtr, sj::subway_link::kToNodeID ).lVal == node_id ) ?
			sj::subway_link::kFromNodeID : sj::subway_link::kToNodeID;

	// �����̃m�[�h������
	IQueryFilterPtr query = AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), kObjectID, AheGetAttribute( linkPtr, to_or_from ).lVal );
	IFeatureCursorPtr ipCur;
	AheGetNetworkNodeFeatureClass(linkPtr)->Search( query, VARIANT_FALSE, &ipCur );
	IFeaturePtr another;
	ipCur->NextFeature( &another );

	// �V�K�쐬�̏ꍇSerch()�Ɉ���������Ȃ�
	// FGDB�ł͈���������悤�����AIRow.Store()����Ă���K�v������|��
	// �w���v�ɋL�ڂ���Ă���̂Œ��Ӂibug 10103�j
	// ToDo: IFeatureClass.GetFeature()�ł���
	if( !another )
	{
		// �c���[�ɂ͕K����΂̃m�[�h������͂�
		for( auto& feature : *m_pFeatureDefList )
		{
			if(
				0 == _tcsicmp( sj::subway_node::kTableName, feature.GetTableName() ) &&
				node_id != feature.GetRowOID() )
			{
				another = feature.GetFeature();
				break;
			}
		}
	}

	return another;
}


void CAttrSubwayNodeDlg::ChangeWindowMap( bool bLink )
{
	m_winMgr.ChangeMgrMap( bLink ? LINK_MAP : NODE_MAP );
	for( auto& r: LINK_MAP )
	{
		::ShowWindow( GetDlgItem( r.GetID() ), bLink ? SW_SHOW : SW_HIDE );
	}
	for( auto& r: NODE_MAP )
	{
		::ShowWindow( GetDlgItem( r.GetID() ), bLink ? SW_HIDE : SW_SHOW );
	}
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}


/**
 * @fn BOOL CAttrSubwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A���e�ԍ��̒l�����������āA���[������擾���Ă���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrSubwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	bool enable = ( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable );
	
	// �ҏW�J�n���ĂȂ���ΐG�点�Ȃ�
	SetDlgStatus( IDC_BUTTON_APPLY, enable ? TRUE : FALSE );
	SetDlgStatus( IDC_LIST_LINE, enable ? TRUE : FALSE );
	SetDlgStatus( IDC_LIST_STATION, enable ? TRUE : FALSE );
	// �w���ƘH������ReadOnly��
	SetDlgStatus( IDC_EDIT_STATIONNAME, FALSE );
	SetDlgStatus( IDC_EDIT_LINENAME, FALSE );
	SetDlgStatus( IDC_EDIT_LINENAME2, FALSE );
	
	// �\�������t�B�[�`��
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	// �wPOI
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipPoi = cMap.FindLayer( nullptr, nullptr, station_point::kTableName );
	IFeatureClassPtr ipPoiClass = AheGetFeatureClass( ipPoi );
	
	// ������H���e�[�u���m��
	// �����N�̏ꍇ�́���return����Ă��܂��̂ŁA�K��������O
	m_joinLineT = AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, join_line_info::kTableName );

	if( sj::subway_link::kTableName == m_layerName )
	{
		// �H�����擾
		long id = AheGetAttribute( ipFeature, sj::subway_link::kRailwayLineID ).lVal;
		_IRowPtr row;
		AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, railway_line::kTableName )->GetRow( id, &row );

		// �G�f�B�b�g�{�b�N�X�ɔ��f
		CEdit edit = GetDlgItem(IDC_EDIT_LINENAME2);
		edit.SetWindowText( CString(AheGetAttribute( row, railway_line::kName )) );

		// �����N�p�̃_�C�A���O�ɂ���
		ChangeWindowMap(true);
		return bRet;
	}
	
	// ������H��ID�擾
	long join_line_id = AheGetAttribute( ipFeature, sj::subway_node::kJoinLineInfo ).lVal;

	// �}�s��m�[�h�͘M�点�Ȃ�
	if( sj::subway_node::node_class::kOutline == AheGetAttribute( ipFeature, sj::subway_node::kNodeClassC ).lVal )
	{
		m_cControlRel.Visible( 0, TRUE, FALSE );
	}

	m_connectedLink.clear();

	// �w���E�H�����I�����X�g�{�b�N�X������
	m_serchLists.reflesh( ipFeature, 100, ipPoiClass );

	// �m�[�h���V�K�̏ꍇ��0
	if( join_line_id < 1 )
	{
		// �K������͂�
		auto& link = *GetConnectedLink( ipFeature ).begin();

		// ���ӂ̉wPOI���������H��ID���擾
		join_line_id = m_serchLists.GetJoinLineInfoIDFromRailWayLine( 
			link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal );
	}

	// ���X�g�I�����āu�K�p�v�{�^�����G�~�����[�g�i�G�f�B�b�g�{�b�N�X�ɔ��f�j
	m_serchLists.select( join_line_id );
	CButton( GetDlgItem(IDC_BUTTON_APPLY) ).Click();
	
	// �E�B���h�E�}�b�v�Ĕz�u
	ChangeWindowMap(false);
	
	if( enable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_STATIONNUM) );
	}
	return bRet;
}

void CAttrSubwayNodeDlg::SetFeatureClassName(LPCTSTR lpcszFeatureClassName)
{
	m_layerName = lpcszFeatureClassName;
}

/**
 * @fn void CAttrSubwayNodeDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrSubwayNodeDlg::CreateControlRelation()
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
		// �m�[�h���
		m_cControlRel.SetControl( IDC_COMBO_CODECLASS, TYPE_COMBO, 0, 0, sj::subway_node::kNodeClassC, 0, sj::subway_node::kTableName );
		// �w�ԍ�
		m_cControlRel.SetControl( IDC_EDIT_STATIONNUM, TYPE_EDITSTRING, 0, 0, sj::subway_node::kStationNumber, 0, sj::subway_node::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, sj::subway_node::kTableName );
	}

	// �����p���X�g�{�b�N�X�R���g���[��������
	m_serchLists.init( GetDlgItem(IDC_LIST_STATION), GetDlgItem(IDC_LIST_LINE) );
}

// �Ή�����H��ID����������
long CAttrSubwayNodeDlg::SearchLineID( const IFeaturePtr& subway_node )
{
	long join_info_id = AheGetAttribute( subway_node, sj::subway_node::kJoinLineInfo ).lVal;
	IQueryFilterPtr query = AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), kObjectID, join_info_id );
	
	_IRowPtr row;
	_ICursorPtr ipCur;

	long line_id = -1;
	m_joinLineT->Search( query, VARIANT_FALSE, &ipCur );
	if( S_OK == ipCur->NextRow( &row ) && row )
	{
		line_id = AheGetAttribute( row, join_line_info::kLineID ).lVal;
	}

	return line_id;
}

long GetLineID( const ITablePtr& joinLineT, long line_info )
{
	// JOIN_LINE_INFO ���� LINEID�擾
	_IRowPtr joinLine;
	joinLineT->GetRow( line_info, &joinLine );
	return AheGetAttribute( joinLine, join_line_info::kLineID ).lVal;
}

bool CAttrSubwayNodeDlg::CheckNode( const IFeaturePtr& node, long railway_id, CTargetItems& allLinks )
{
	for( auto& feature : GetConnectedLink(node) )
	{
		// ���[�m�[�h
		IFeaturePtr another = GetCorrespNode( AheGetOID(node), feature.GetFeature() );

		// �ڑ������N�擾
		CTargetItems links;
		AheGetConnectedNetworkLink( another, links );
		if( !links.empty() )
		{
			for( const auto& link : links)
			{
				//long id = link.GetAttribute( kObjectID ).lVal;
				//long railway = link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal;
				//OutputDebugString( uh::str_util::format( _T("[OID]:%d [RailwayID]:%d\n"), id, railway ) );
				if( railway_id != link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_CONNECT_DIFFERENT_LINE), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
		}
		else // �V�K�����N
		{
			if( railway_id < 0 ) // �����ł͘H������ł��Ȃ��i�w�ȊO�j
			{
				// ���[�̃m�[�h�𒲂ׂĂ����߂Ȃ�_��
				railway_id = SearchLineID( another );
				if( railway_id < 0 )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_UNKNOWN_LINE), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
			else
			{
				// ���[�̘H���ƈ���Ă���_��
				if( railway_id != SearchLineID( another ) )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_DIFFERENT_LINE_ID), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
		}

		allLinks.push_back( feature );
	}
	return true;
}

// OK��������ɌĂ΂��i�I�[�o�[���C�h�j
BOOL CAttrSubwayNodeDlg::ErrorCheck()
{
	IFeaturePtr nodePtr;
	long railway_id = 0;
	CTargetItems links; // �ڑ������N�Q
	// ������GetCurrentFeatureDef()���Ă��]�݂̂��͓̂����Ȃ����Ƃ�����̂Ŏ��O�Ńm�[�h��T��
	for( auto& def : *m_pFeatureDefList )
	{
		// GetTableName()��LPCTSTR�A�X�L�[�}��`��_TCHAR�Ȃ̂� == ��r�ł��Ȃ��I�I�I�I
		if( 0 == _tcsicmp( def.GetTableName(), sj::subway_node::kTableName ) )
		{
			nodePtr = def.GetFeature();
			const auto& lineInfo = def.GetFieldDef( sj::subway_node::kTableName, sj::subway_node::kJoinLineInfo );
			railway_id = GetLineID( m_joinLineT, lineInfo->m_vaValue.lVal );

			//OutputDebugString( uh::str_util::format( _T("[%s]: %d [line_info] %d [railway_id] %d\n"), def.GetTableName(), AheGetOID(nodePtr), l, railway_id ));
			CTargetItems linkstemp;
			if( !CheckNode( nodePtr, railway_id, linkstemp ) )
			{
				return False;
			}

			for( auto& link : linkstemp )
			{
				link.SetAttribute( sj::subway_link::kRailwayLineID, railway_id );
				links.push_back( link );
			}
		}
	}
	if( !nodePtr )
		return FALSE;
	
	// �����N�ɕύX����������X�g�A���Ă���
	for( auto& link : links )
	{
		link.Store();
	}

	return TRUE;
}

// ���X�g�{�b�N�X�I��ύX
LRESULT CAttrSubwayNodeDlg::OnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch( wID )
	{
	case IDC_LIST_STATION:
		m_serchLists.StationSelChange();
		break;
	case IDC_LIST_LINE:
		m_serchLists.LineSelChange();
		break;
	default:
		break;
	}

	return 0;
}

// �K�p�{�^������
LRESULT CAttrSubwayNodeDlg::OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// �w��
	CEdit edit = GetDlgItem(IDC_EDIT_STATIONNAME);
	edit.SetWindowText( m_serchLists.selectedStation );

	// �H����
	edit = GetDlgItem(IDC_EDIT_LINENAME);
	edit.SetWindowText( m_serchLists.selectedLine );

	// �ς���Ă��瑮���ύX
	long join_line_info = m_serchLists.GetJoinLineInfoID();
	if( join_line_info < 0 )
		return 0;

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if( join_line_info != AheGetAttribute(ipFeature, sj::subway_node::kJoinLineInfo).lVal )
	{
		// FieldDef::SetNewValue()���Ȃ��ƃR���g���[����ŕύX�������ƂɂȂ�Ȃ�
		GetCurrentFeatureDef()->GetFieldDef( sj::subway_node::kTableName, sj::subway_node::kJoinLineInfo )->SetNewValue( join_line_info, m_hWnd );
		AheSetAttribute( ipFeature, sj::subway_node::kJoinLineInfo, join_line_info );
		SetButton( TRUE );
	}
	
	return 0;
}
