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
#include "SearchListCtrls.h"
#include "globalfunc.h"
#include "AheLayerFunctions.h"
#include "TDC/useful_headers/str_util.h"

using namespace sindy::schema;

CSearchListCtrls::CSearchListCtrls(void)
{
}


CSearchListCtrls::~CSearchListCtrls(void)
{
}

void CSearchListCtrls::init( HWND station_list, HWND line_list )
{
	m_lvStation.Detach();
	m_lvStation.Attach( station_list );
	m_lvLine.Detach();
	m_lvLine.Attach( line_list );
}

// ���X�g�{�b�N�X�̒��g���X�V����
void CSearchListCtrls::reflesh( IFeaturePtr ipNodeFeature, double dRange, IFeatureClassPtr ipPoiClass )
{
	// �֘A�e�[�u���擾
	ITablePtr joinLineT = AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, join_line_info::kTableName );
	ITablePtr stationInfoT = AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, station_info::kTableName );
	ITablePtr railwayLineT = AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, railway_line::kTableName );
	
	IGeometryPtr ipGeom;
	ipNodeFeature->get_ShapeCopy( &ipGeom );

	m_JoinLineInfos.clear();
	for( long id : GetNeighboringStationPoint(ipGeom, dRange, ipPoiClass) )
	{
		IQueryDefPtr ipQueryDef;
		((IFeatureWorkspacePtr)AheGetWorkspace(ipPoiClass))->CreateQueryDef( &ipQueryDef );

		CString strJoinLineT = AheGetOwnerTableName( joinLineT );
		CString strStationInfoT = AheGetOwnerTableName( stationInfoT );
		CString strRailwyLineT = AheGetOwnerTableName( railwayLineT );

		ipQueryDef->put_Tables( CComBSTR(uh::str_util::format( _T("%s,%s,%s"), strJoinLineT, strStationInfoT, strRailwyLineT )) );

		CString strSub;
		strSub.Format(_T("%s.%s,%s.%s,%s.%s,%s.%s"), 
			strStationInfoT, station_info::kName,    // �w��
			strRailwyLineT, railway_line::kName,     // �H����
			strRailwyLineT, kObjectID,               // RAIL_WAY_INFO ID
			strJoinLineT, kObjectID);                // JOIN_LINE_INFO ID
		ipQueryDef->put_SubFields( CComBSTR(strSub) );

		CString strWhere = uh::str_util::format(_T("%s.%s=%ld AND %s.%s=%s.%s AND %s.%s=%s.%s"),
			strJoinLineT, join_line_info::kStationID, id,                           // JOIN_LINE_INFO.STATIONID = STATION_POINT.OBJECTID
			strJoinLineT, join_line_info::kInfoID, strStationInfoT, kObjectID,      // JOIN_LINE_INFO.INFOID = STATION_INFO.OBJECTID
			strJoinLineT, join_line_info::kLineID, strRailwyLineT, kObjectID);      // JOIN_LINE_INFO.LINDID = RAILWAY_LINE.OBJECTID
		ipQueryDef->put_WhereClause(CComBSTR(strWhere));

		_ICursorPtr ipCur;
		ipQueryDef->Evaluate( &ipCur );
		std::list<JoinLineInfo> lst_infos;
		_IRowPtr ipRow;
		while( S_OK == ipCur->NextRow( &ipRow ) )
		{
			JoinLineInfo info(
				AheGetAttribute( ipRow, 3 ).lVal,
				AheGetAttribute( ipRow, 2 ).lVal,
				AheGetAttribute( ipRow, 0L ),
				AheGetAttribute( ipRow, 1 ) );

			lst_infos.push_back( info );
		}
		if( !lst_infos.empty() )
			m_JoinLineInfos[id] = lst_infos;
	}

	m_lvStation.ResetContent();
	for( const auto& infos : m_JoinLineInfos )
	{
		for( const auto& info : infos.second )
		{
			// �w�������j�[�N�ɂȂ�悤�ɒǉ�
			if( LB_ERR == m_lvStation.FindString( -1, info.station_name ) )
				m_lvStation.AddString(info.station_name);
		}
	}

	// �ŏ��̂��̂�I�����Ă���
	if( !m_JoinLineInfos.empty() )
		select( m_JoinLineInfos.begin()->second.begin()->oid );
}

// ���X�g�{�b�N�X�̍��ڂ�I�����āA������_�C�A���O�ɒʒm����
void SelectString( CListBox& lb, const CString& item )
{
	lb.SelectString( 0, item );
	lb.GetParent().SendMessage( WM_COMMAND, MAKEWPARAM( lb.GetDlgCtrlID(),  LBN_SELCHANGE), 0); 
}

// ���X�g�{�b�N�X��I������
void CSearchListCtrls::select( long join_line_info_id )
{
	for(const auto& infos : m_JoinLineInfos )
	{
		// ������H��ID�Ō���
		auto& it = std::find_if( infos.second.begin(), infos.second.end(), 
			[&]( const JoinLineInfo& info )
			{
				return (info.oid == join_line_info_id );
			});

		// ����������Ή�����w���ƘH�������擾
		if( it != infos.second.end() )
		{
			// ���X�g�{�b�N�X����T���đI������
			SelectString( m_lvStation, it->station_name );
			SelectString( m_lvLine, it->line_name );
			return;
		}
	}
}

// ���X�g�{�b�N�X�̑I����Ԃ��������H��ID�擾
long CSearchListCtrls::GetJoinLineInfoID()
{
	long id = -1;
	for(const auto& infos : m_JoinLineInfos )
	{
		auto& it = std::find_if( infos.second.begin(), infos.second.end(), 
			[&]( const JoinLineInfo& info )
			{
				return (info.line_name == selectedLine && info.station_name == selectedStation );
			});

		if( it != infos.second.end() )
		{
			id = it->oid;
			break;
		}
	}
	return id;
}

// �S���H��ID���������H��ID�擾
long CSearchListCtrls::GetJoinLineInfoIDFromRailWayLine( long railway_line_id )
{
	long id = -1;
	for(const auto& infos : m_JoinLineInfos )
	{
		auto& it = std::find_if( infos.second.begin(), infos.second.end(), 
			[&]( const JoinLineInfo& info )
			{
				return (info.railway_line_id == railway_line_id);
			});

		if( it != infos.second.end() )
		{
			id = it->oid;
			break;
		}
	}
	return id;
}

// ������H��ID����S���H��ID���擾
long CSearchListCtrls::GetRailWayLineIDFromJoinLineInfo( long join_line_info )
{
	long id = -1;
	for(const auto& infos : m_JoinLineInfos )
	{
		auto& it = std::find_if( infos.second.begin(), infos.second.end(), 
			[&]( const JoinLineInfo& info )
			{
				return (info.oid == join_line_info);
			});

		if( it != infos.second.end() )
		{
			id = it->railway_line_id;
			break;
		}
	}
	return id;
}

// �w���̑I�����ύX���ꂽ
void CSearchListCtrls::StationSelChange()
{
	CComBSTR text; 
	m_lvStation.GetTextBSTR( m_lvStation.GetCurSel(), (BSTR&)text );
	selectedStation = text;
	
	// �H�����̃��X�g�{�b�N�X���X�V����
	m_lvLine.ResetContent();
	for( const auto& infos : m_JoinLineInfos )
	{
		for( const auto& info : infos.second )
		{
			if( selectedStation == info.station_name)
				m_lvLine.AddString(info.line_name);
		}
	}
}

// �H�����̑I�����ύX���ꂽ
void CSearchListCtrls::LineSelChange()
{
	CComBSTR text; 
	m_lvLine.GetTextBSTR( m_lvLine.GetCurSel(), (BSTR&)text );
	selectedLine = text;
}
