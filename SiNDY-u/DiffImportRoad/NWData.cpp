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
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/workspace.h>
#include <schema.h>
#define _USE_SINDYLIB_STATIC
#include <libsindylibcore.h>
#include <libsindylibbase.h>
#include "NWData.h"
#include "global.h"

using namespace std;
using namespace sindy;
using namespace sindy::schema;


ITablePtr CNWData::GetTable( const CString& strTableName ) const
{
	map<CString,TableData>::const_iterator it = m_mapTableData.find( strTableName );
	if( m_mapTableData.end() == it )
		return NULL;
	else
		return it->second.ipTable;
}
CFieldMap CNWData::GetFieldMap( const CString& strTableName ) const
{
	map<CString,TableData>::const_iterator it = m_mapTableData.find( strTableName );
	if( m_mapTableData.end() == it )
		return NULL;
	else
		return it->second.cFieldMap;
}
_IRowPtr CNWData::GetRow( const CString& strTableName, long lOID ) const
{
	map<CString,TableData>::const_iterator it = m_mapTableData.find( strTableName );
	if( m_mapTableData.end() == it )
		return NULL;
	map<long,CAdapt<_IRowPtr>>::const_iterator it2 = it->second.mapRow.find( lOID );
	if( it->second.mapRow.end() == it2 )
		return NULL;
	else
		return it2->second.m_T;
}


long CNWData::GetPGDBOIDFromOrgOID( const CString& strTableName, long lOrgOID ) const
{
	map<CString,TableData>::const_iterator it = m_mapTableData.find( strTableName );
	if( m_mapTableData.end() == it )
		return -1;
	map<long,long>::const_iterator it2 = it->second.mapOrgID2PGDBID.find( lOrgOID );
	if( it->second.mapOrgID2PGDBID.end() == it2 )
		return -1;
	else
		return it2->second;
}
long CNWData::GetOrgOIDFromPGDBOID( const CString& strTableName, long lPGDBOID ) const
{
	map<CString,TableData>::const_iterator it = m_mapTableData.find( strTableName );
	if( m_mapTableData.end() == it )
		return -1;
	map<long,long>::const_iterator it2 = it->second.mapPGDBID2OrgID.find( lPGDBOID );
	if( it->second.mapPGDBID2OrgID.end() == it2 )
		return -1;
	else
		return it2->second;
}

bool CNWData::Init_SDE( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, const map<CString,set<CString>>& exceptFieldMap )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CNWData::Init_SDE") );
#endif

	// 初期化
	m_ipFWorkspace = ipFWorkspace;

	for( const auto& tablename : mapTableMap )
	{
		// テーブル、フィールドマップ、IRow群取得
		if( !setTable( tablename.first, tablename.second ) || !setFieldMap( tablename.first, exceptFieldMap ) )
		{
			return false;
		}
		// フィーチャクラスの場合は空間参照を取得する
		if( !m_ipSpRef && FEATURECLASS == tablename.second )
		{
			ITablePtr ipTable = GetTable( tablename.first );
			if( !ipTable )
			{
				// あるでしょ
				_ASSERT(0);
				return false;
			}
			IGeoDatasetPtr(ipTable)->get_SpatialReference( &m_ipSpRef );
			if( !m_ipSpRef )
			{
				// あるでしょ
				_ASSERT(0);
				return false;
			}
		}
	}
	// 空間参照が取得できない場合はNG
	if( !m_ipSpRef )
		return false;

	return true;
}

bool CNWData::Init( const map<CString,DATATYPE>& mapTableMap, const IFeatureWorkspacePtr& ipFWorkspace, long lMeshCode, const IGeometryPtr& ipMeshGeom, const map<CString,set<CString>>& exceptFieldMap )
{
	bool bRet = true;

#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CNWData::Init") );
#endif

	// 初期化
	m_ipFWorkspace = ipFWorkspace;
	m_lMeshCode = lMeshCode;
	m_ipMeshGeom = ipMeshGeom;

	for( const auto& tablename : mapTableMap )
	{
		// テーブル、フィールドマップ、IRow群取得
		bool bTableFieldMap = ( setTable( tablename.first, tablename.second ) && setFieldMap( tablename.first, exceptFieldMap ) );
		bool bSetData = false;
		switch( tablename.second )
		{
		case FEATURECLASS:
			bSetData = setFeatureClassData( tablename.first, ipMeshGeom );
			break;
		case REL_LINKTABLE:
		case REL_OTHERTABLE:
		case INF:
		case LQ:
		case TABLE:
		default:
			bSetData = setTableData( tablename.first, NULL, true );
			break;
		}

		
		if( !bTableFieldMap || !bSetData )
		{
			bool bRet = false;
			break;
		}

		// 道路リンクの場合
		if( 0 == _tcsicmp( tablename.first, road_link::kTableName ) )
		{
			// リンクとノードの関連性を取得
			if( !setLinkNodeRelation() )
			{
				bool bRet = false;
				break;
			}
		}
		// 道路ノードの場合
		if( 0 == _tcsicmp( tablename.first, road_node::kTableName ) )
		{
			// メッシュ境界ノードを取得
			if( !setMeshNode() )
			{
				bool bRet = false;
				break;
			}
		}

		// INFの場合
		if( IsInfTable( tablename.first ) )
		{
			// 特になし
		}

		// LQの場合
		if( IsLqTable( tablename.first ) )
		{
			// リンクとINF,LQの関連性を取得
			if( !setInfLq( tablename.first, lMeshCode ) )
			{
				bool bRet = false;
				break;
			}
		}
	}

	// メッシュ内のリンクに対応するデータだけ抽出
	for( const auto& tablename : mapTableMap )
	{
		// リンク内属性、傾斜、一方通行、通行規制、簡易傾斜の場合
		if( IsAGOPSTable( tablename.first ) )
		{
			// リンクとの関連性を取得
			if( !setLinkAGOPSRelation( tablename.first ) )
			{
				bool bRet = false;
				break;
			}
		}
	}
	// 車種別はそのまま
	setVehicleRegTypeRelation(vehicle_type::kTableName);

	return bRet;
}

bool CNWData::setTable( const CString& strTableName, const DATATYPE dataType )
{
	ITablePtr ipTable;
	if( S_OK == m_ipFWorkspace->OpenTable( CComBSTR(strTableName), &ipTable ) && ipTable )
	{
		m_mapTableData[strTableName].dataType = dataType;
		m_mapTableData[strTableName].strTableName = strTableName;
		m_mapTableData[strTableName].ipTable = ipTable;
		return true;
	}
	else
	{
		_ASSERT(0);
		return false;
	}
}

bool CNWData::setFieldMap( const CString& strTableName, const map<CString,set<CString>>& exceptFieldMap )
{
	IFieldsPtr ipFields;
	m_mapTableData[strTableName].ipTable->get_Fields( &ipFields );
	if( !ipFields )
	{
		_ASSERT(0);
		return false;
	}

	CFieldMap cFieldMap( ipFields );
	m_mapTableData[strTableName].cFieldMap = cFieldMap;

	const auto& tableElement = exceptFieldMap.find(CString(strTableName).MakeUpper());
	if(tableElement == exceptFieldMap.end())
		return true;

	const auto& exceptSet = tableElement->second;  
	set<CString> foundFieldSet;

	for(const auto& element : cFieldMap)
	{
		CString fieldName = CString(element.name).MakeUpper();
		if(exceptSet.find(fieldName) != exceptSet.end())
		{
			long index = cFieldMap.FindField(element.name);
			m_mapTableData[strTableName].exceptFieldIndexSet.insert(index);
			foundFieldSet.insert(fieldName);
		}
	}
	bool succeeded_f = true;
	for(const auto& element : exceptSet)
	{
		if(foundFieldSet.find(element) == foundFieldSet.end())
		{
			cerr << CT2A(strTableName) << "\tにはインポート除外対象フィールド\t" << CT2A(element) << "\tが存在しない" << endl;
			succeeded_f = false;
		}
	}

	return succeeded_f;
}

bool CNWData::setTableData( const CString& strTableName, const IQueryFilterPtr& ipQFilter, bool bInMesh )
{
	_ICursorPtr ipCursor;
	if( SUCCEEDED( m_mapTableData[strTableName].ipTable->Search( ipQFilter, VARIANT_FALSE, &ipCursor ) ) && ipCursor )
	{
		_IRowPtr ipRow;
		while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
		{
			long lOID = ipRow->GetOID();
			m_mapTableData[strTableName].mapRow[lOID] = ipRow;
			m_mapTableData[strTableName].mapInMeshData[lOID] = bInMesh;
			CComVariant vaOrgID;
			ipRow->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( category::sindyc_table::kOrgOBJID ), &vaOrgID );
			if( vaOrgID.lVal > 0 )
			{
				m_mapTableData[strTableName].mapOrgID2PGDBID[vaOrgID.lVal] = lOID;
				m_mapTableData[strTableName].mapPGDBID2OrgID[lOID] = vaOrgID.lVal;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CNWData::setFeatureClassData( const CString& strTableName, const IGeometryPtr& ipMeshGeom )
{
	// まずは9メッシュ全体を取得
	if( !setTableData( strTableName, NULL, false ) )
		return false;

	ISpatialFilterPtr ipSpFil( CLSID_SpatialFilter );
	ipSpFil->putref_Geometry( ipMeshGeom );
	// 道路ノードの場合はintersects, それ以外はcontains
	if( 0 ==_tcsicmp( strTableName, road_node::kTableName) )
		ipSpFil->put_SpatialRel( esriSpatialRelIntersects );
	else
		ipSpFil->put_SpatialRel( esriSpatialRelContains );

	// 指定メッシュ分を取得（冗長だなー。。。）
	if( !setTableData( strTableName, ipSpFil, true ) )
		return false;

	return true;
}


bool CNWData::setLinkNodeRelation()
{
	for( const auto& link : m_mapTableData[road_link::kTableName].mapRow )
	{
		// FromToリンク取得
		CComVariant vaFrom, vaTo;
		link.second.m_T->get_Value(m_mapTableData[road_link::kTableName].cFieldMap.FindField( road_link::kFromNodeID ), &vaFrom );
		link.second.m_T->get_Value(m_mapTableData[road_link::kTableName].cFieldMap.FindField( road_link::kToNodeID ), &vaTo );

		// リンクとノードの関連
		m_mapLink2FromToNode[link.first].first = vaFrom.lVal;
		m_mapLink2FromToNode[link.first].second = vaTo.lVal;

		// ノードとリンクの関連
		m_mapNode2Link[vaFrom.lVal].insert( link.first );
		m_mapNode2Link[vaTo.lVal].insert( link.first );
	}

	return true;
}

bool CNWData::setLinkAGOPSRelation( const CString& strTableName )
{
	for( const auto& agops : m_mapTableData[strTableName].mapRow )
	{
		// 対応するリンクID取得
		CComVariant vaLinkID;
		agops.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( category::link_relational_table::kLinkID ), &vaLinkID );

		// メッシュ内リンクでない場合はチェックする
		if( !m_mapTableData[road_link::kTableName].mapInMeshData[vaLinkID.lVal] )
			m_mapTableData[strTableName].mapInMeshData[agops.first] = false;

		// [リンク内属性、傾斜、一方通行、通行規制、一通/通禁車種別規制]とリンクの関連
		m_mapTableData[strTableName].mapAGOPS2Link[agops.first] = vaLinkID.lVal;

		// リンクと[リンク内属性、傾斜、一方通行、通行規制、一通/通禁車種別規制]の関連
		m_mapTableData[strTableName].mapLink2AGOPS[vaLinkID.lVal].insert( agops.first );
	}

	return true;
}

bool CNWData::setVehicleRegTypeRelation( const CString& strTableName )
{
	for( const auto& regType : m_mapTableData[strTableName].mapRow )
	{
		// 対応する一通/通禁車種別規制のID取得
		CComVariant vaRegID;
		regType.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( vehicle_type::kRegID ), &vaRegID );

		// メッシュ内リンクでない場合はチェックする(ここまでに一通/通禁車種別規制のチェックで情報は充分あるはず)
		if( !m_mapTableData[vehicle_reg::kTableName].mapInMeshData[vaRegID.lVal] )
			m_mapTableData[strTableName].mapInMeshData[regType.first] = false;

		// 車種タイプと一通/通禁車種別規制の関連
		m_mapTableData[strTableName].mapType2VehicleReg[regType.first] = vaRegID.lVal;

		// 一通/通禁車種別規制と車種タイプの関連
		m_mapTableData[strTableName].mapVehicleReg2Type[vaRegID.lVal].insert( regType.first );
	}

	return true;
}

bool CNWData::setInfLq( const CString& strTableName, long lMeshCode )
{
	CString strInfName = _T("INF_") + strTableName.Right( strTableName.GetLength() - 3 );

	for( const auto& lq : m_mapTableData[strTableName].mapRow )
	{
		// 対応するメッシュ、リンクID、リンク方向、シーケンス、ラストフラグ、INFID（LQの共通フィールドだから変わらないよね。。）
		CComVariant vaMeshCode, vaLinkID, vaLinkDir, vaSeq, vaLast, vaInfID;
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kMeshCode), &vaMeshCode );
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kLinkID), &vaLinkID );
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kLinkDir), &vaLinkDir );
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kSequence), &vaSeq );
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kLastLink), &vaLast );
		lq.second.m_T->get_Value( m_mapTableData[strTableName].cFieldMap.FindField( link_queue::kInfID), &vaInfID );

		LqData cLqData;
		cLqData.lInfID = vaInfID.lVal;
		cLqData.lMeshCode = vaMeshCode.lVal;
		cLqData.lLinkID = vaLinkID.lVal;
		cLqData.lLinkDir = vaLinkDir.lVal;
		cLqData.lSeq = vaSeq.lVal;
		cLqData.lLast = vaLast.lVal;

		// InfとLQのマップ
		m_mapTableData[strInfName].mapInfData[vaInfID.lVal].insert( cLqData );

		// Infとリンクのマップ
		m_mapTableData[strInfName].mapInf2Link[vaInfID.lVal].insert( vaLinkID.lVal );

		// リンクとInfのマップ
		m_mapTableData[strInfName].mapLink2Inf[vaLinkID.lVal].insert( vaInfID.lVal );

		// 指定メッシュ内のリンク列か（メッシュをまたぐリンク列やメッシュ外のリンク列はfalse）
		if( lMeshCode != vaMeshCode.lVal )
			m_mapTableData[strInfName].mapInMeshData[vaInfID.lVal] = false;
	}

	return true;
}

bool CNWData::setMeshNode()
{
	for( const auto& node : m_mapTableData[road_node::kTableName].mapRow )
	{
		CComVariant vaNodeClass;
		node.second.m_T->get_Value( m_mapTableData[road_node::kTableName].cFieldMap.FindField( road_node::kNodeClass ), &vaNodeClass );

		// メッシュ境界＋PECメッシュ境界の場合
		if( road_node::node_class::kEdge == vaNodeClass.lVal || road_node::node_class::kPecEdge == vaNodeClass.lVal )
			m_setMeshNode.insert( node.first );
	}

	return true;
}
