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

/**
 * @file RoadNetwork.cpp
 * @brief <b>CRoadNetworkクラス実装ファイル</b>\n
 * @author 
 * @version $Id$
 */
#include "stdafx.h"
#include <boost/foreach.hpp>
#include "RoadNetwork.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "GeometryRule.h"
#include "TableTypeUtil.h"
#include "AheNetwork.h"
#include <typeinfo>
#include "RuleModel.h"
#include "AheLayerFunctions.h"
#include "Mesh.h"
#include "meshutil.h"
#include "define.h"
#include "AheGeometryOp.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define foreach BOOST_FOREACH
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ );


namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR SHAPE_POINT_TOUCHING = _T("%d - %d 構成点どうしが接触しています");
	LPCTSTR SEGMENT_ARE_CLOSE    = _T("%ld - %ld 同一ノードに接続する線分が近接しています（正規化座標距離 %.1lf）");
	LPCTSTR ANGLE_LESS_THAN_20   = _T("%ld - %ld 2叉路の接続角が20°以下です（%d°）");
#else
	LPCTSTR SHAPE_POINT_TOUCHING = _T("%d-%d shape points are touching");
	LPCTSTR SEGMENT_ARE_CLOSE    = _T("%ld-%ld Line segments that are connected to the same node are close together (Normalized coordinate distance %.1lf)");
	LPCTSTR ANGLE_LESS_THAN_20   = _T("%ld - %ld Connection angle of forked path is less than 20(%d degree)");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
using namespace errorcode;

// 2次メッシュ矩形で検索するかどうかのフラグ
// 
// 2次メッシュでネットワークを検索する場合は検索矩形内
// で関連フィーチャも完結するため、処理が簡略化されます。
// バッチ処理等で2次メッシュ単位で処理をする際にはこの
// フラグをONにすることで検索の際にかなり高速化されます。
bool g_bNetworkSearchBy2ndMesh = false;

CSPRow RowClassFactory( _IRow* ipRow, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames )
{
	CSPRow cRet; // 戻り値
	LPCTSTR lpcszTableName = cNames->GetTableName();
	if( AheIsNetworkNode( lpcszTableName ) )
		cRet.reset( new CRelNode( ipRow, emType, false, cFields, cNames ) );
	else if( AheIsNetworkLink( lpcszTableName ) )
		cRet.reset( new CRelRoad( ipRow, emType, false, cFields, cNames ) );
	else if( (IFeaturePtr)ipRow )
		cRet.reset( new CFeature( ipRow, emType, false, cFields, cNames ) );
	else
		cRet.reset( new CRow( ipRow, emType, false, cFields, cNames ) );

	return cRet;
}

// 形状で検索する
sindyErrCode CRoadNetwork::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/ )
{
	// ジオメトリがNULLでも検索できるが、ここではあえて
	// エラーとすることで予期しない全件検索を回避する
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	sindyErrCode emErr = _Select( ipGeom, bRecycling, bCreateCache, cErrs, bInit );

	// 周辺再検索（リンク端点を用いる bug 6157）
	IGeometryCollectionPtr ipGeomColl( CLSID_GeometryBag );

	((IGeometryPtr)ipGeomColl)->putref_SpatialReference( GetSpatialReference() );

	// 道路の両端を取得
	sindyTableType::ECode emNetworkLinkTableType = GetTableType();
	for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
	{
		ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );
		ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
	}

	CGeometry cSearchGeom = (IGeometryPtr)ipGeomColl;

	if ( (IGeometry*)cSearchGeom && !cSearchGeom.IsEmpty() )
		emErr = _Select( cSearchGeom, bRecycling, bCreateCache, cErrs, false, false );

	return emErr;
}

// 形状で検索する
sindyErrCode CRoadNetwork::_Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	return _Select( AheInitSpatialFilter( NULL, ipGeom ), bRecycling, bCreateCache, cErrs, bInit, bGetRelated );
}
sindyErrCode CRoadNetwork::_Select( IQueryFilter* ipFilter, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	// ベースオブジェクトの検索
	sindyErrCode emErr = CFeatureContainer::Select( ipFilter, typeid(CRelRoad&), bRecycling, bCreateCache, cErrs, bInit );
	sindyTableType::ECode emNetworkLinkTableType = GetTableType();

	unsigned int meshcode = 0; // 2次メッシュ単位で検索するときのみ使用
	if( g_bNetworkSearchBy2ndMesh )
	{
		// メッシュコードを調べる
		IGeometryPtr ipGeom;
		((ISpatialFilterPtr)ipFilter)->get_Geometry( &ipGeom );
		IPointPtr ipPoint;
		((IAreaPtr)ipGeom)->get_Centroid( &ipPoint );
		WKSPoint center = {0,0};
		int x = 0, y = 0;
		ipPoint->QueryCoords( &center.X, &center.Y );
		meshcode = LL2MESH( center.X, center.Y, 2, x, y );

		for(rows_iterator i=begin(sindy::sindyTableType::road_link);i!=end(sindy::sindyTableType::road_link);i++){
			ISpatialReferencePtr srp;
			IGeometryPtr ipShape=(*i)->GetShapeCopy();
			ipShape->get_SpatialReference(&srp);
			if (!AheIsContain(ipGeom,ipShape ,srp)){
				erase(*i);
			}
		}
	}

	// リンク列を検索する
	for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf )
	{
		// IPCVICSが存在しないモデルもあるので、それは飛ばす
		long index_lqrf = GetFieldMap()->HasField( itInf->get<2>() );
		if( 0 > index_lqrf )
			continue;

		sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
		sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());
		//////////////////////////////////////////
		// INF IDの収集
		//////////////////////////////////////////
		std::set<long> listInfIDs; // 取得対象INFのID列
		if( g_bNetworkSearchBy2ndMesh )
		{
			// 2次メッシュ矩形の場合はLQをメッシュコードで検索する
			// これにより、道路のIDを使用したLQの検索を代替できる（bug 7301）
			CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
			LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
			{
				_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, _T("*"), _T("%s=%u"), schema::link_queue::kMeshCode, meshcode ), true ) ); // INFのIDを取るだけなので捨てる
				// 取得したLQからINFのIDを取得する
				long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
				LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
				{
					_IRowPtr ipRow;
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( ! ipRow )
							break;
						CComVariant vaInfID(-1L);
						ipRow->get_Value( lInfIDIndex, &vaInfID );
						listInfIDs.insert( vaInfID.lVal );
					}
				}
			}
		}
		else {
			// 関連する道路IDを収集する
			std::list<long> listRoadIDs;
			for( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				if( (*itRoad)->GetBoolValue( index_lqrf, false ) )	// 関連フラグが true かどうか
					listRoadIDs.push_back( (*itRoad)->GetOID() );
			}

			if( ! listRoadIDs.empty() )
			{
				// LQ取得
				CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
				LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
				{
					// 収集した道路IDに関連するLQを取得
					// ここで検索できたLQは捨てる（INFのIDリスト作成のみに使用）
					std::list<CString> listIDsString;
					cLQTable.IDs2Str( listRoadIDs, listIDsString );
					for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
					{
						// 検索
						_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, schema::link_queue::kInfID, _T("%s IN (%s)"), schema::link_queue::kLinkID, *itIDsString ), true ) );	// とりあえずINFのIDをとるので、LQは捨てる

						// 取得したLQからINFのIDを取得する
						long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
						LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
						{
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								if( ipRow == NULL ) break;

								CComVariant vaInfID(-1L);
								ipRow->get_Value( lInfIDIndex, &vaInfID );

								listInfIDs.insert( vaInfID.lVal );
							}
						}
					}
				}
			}
		}
		// 既にリストに存在する場合はその分を差し引く
		foreach( const CLinkQueue& lq, m_mapLinkQueue[emInfCode] )
			listInfIDs.erase( lq.GetOID() );
//		for( std::list<CLinkQueue>::const_iterator it = m_mapLinkQueue[emInfCode].begin(), itEnd = m_mapLinkQueue[emInfCode].end(); it != itEnd; ++it )
//			listInfIDs.remove( it->GetOID() );

		if( listInfIDs.empty() )
			continue;

		//////////////////////////////////////////
		// INFの検索
		//////////////////////////////////////////
		// CLinkQueueクラスの構造上、必ず先にINFを入れる必要がある
		// 関連するINFを検索する
		CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
		LOGASSERTEERR_IF( cInfTable != NULL, sindyErr_TableNotFound )
		{
			// 共有オブジェクトの取得
			CSPTableNameString spInfNameString( cInfTable.GetNameString() );
			CSPFieldMap spInfFieldMap( cInfTable.GetFieldMap() );

			// INF検索
			_ICursorPtr ipInfCursor( cInfTable.GetRows( listInfIDs, bRecycling ) );
			LOGASSERTEERR_IF( NULL != ipInfCursor, sindyErr_SearchFailed )
			{
				// INF格納
				_IRowPtr ipRow;
				while( S_OK == ipInfCursor->NextRow( &ipRow ) )
				{
					if( ipRow == NULL ) break;

					linkq_iterator itLinkQueue;
					if( g_bNetworkSearchBy2ndMesh ) // 2次メッシュでの検索時はlinkq_findしない（多分する必要はないと思う）
						itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow,spInfFieldMap,spInfNameString) );
					else {
						long lOID = -1;
						ipRow->get_OID( &lOID );

						itLinkQueue = linkq_find( emInfCode, lOID );
						if ( itLinkQueue == linkq_end(emInfCode) )
							itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow,spInfFieldMap,spInfNameString) );
						else
							(*itLinkQueue) = ipRow;
					}
					// リサイクルする場合はキャッシュ作成
					if( bRecycling )
						itLinkQueue->CreateCache( true, spInfFieldMap, spInfNameString );
					else if( bCreateCache )
						itLinkQueue->CreateCache( false, spInfFieldMap, spInfNameString );
				}
			}
			ipInfCursor = NULL;
		}
		//////////////////////////////////////////
		// LQ の検索
		//////////////////////////////////////////
		CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
		LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
		{
			// 取得したINFIDに関連するLQを取得する
			std::list<CString> listIDsString;
			cLQTable.IDs2Str( listInfIDs, listIDsString, 500 );
			// 共有オブジェクトの取得
			CSPTableNameString spLQNameString( cLQTable.GetNameString() );
			CSPFieldMap spLQFieldMap( cLQTable.GetFieldMap() );
			long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
			// LQをpush_backする先はINFの中だが、その際にlinkq_findが走る。
			// これは全てのINFをなめているので非常に効率が悪い。
			// なので、ここで事前に対応表を作成しておく（bug 7301）
			std::map<long,CLinkQueue*> mapINF;
			foreach( CLinkQueue& inf, m_mapLinkQueue[emInfCode] )
				mapINF[inf.GetOID()] = &inf;
			for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
			{
				// 検索
				_ICursorPtr ipCursor( cLQTable.Search( AheInitQueryFilter( NULL, NULL, _T("%s IN (%s)"), schema::link_queue::kInfID, *itIDsString ), bRecycling ) );

				// LQをINFIDで振り分け
				LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
				{
					_IRowPtr ipRow;
					while( S_OK == ipCursor->NextRow( &ipRow ) )
					{
						if( ipRow == NULL ) break;

						CComVariant vaInfID( -1L );
						ipRow->get_Value( lInfIDIndex, &vaInfID );

//						linkq_iterator itLinkQueue = linkq_find( emInfCode, vaInfID.lVal );
//						itLinkQueue->push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );
						CLinkQueue& inf = *mapINF[vaInfID.lVal];
						inf.push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );

						// リサイクルする場合はキャッシュ作成
						if( bRecycling || bCreateCache )
							inf.rbegin()->CreateCache( ( bRecycling ) ? true : false, spLQFieldMap, spLQNameString );
					}
					ipCursor = NULL;
				}
			}
		}
	}

	// 関連するノードを検索する
	CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );
	if( g_bNetworkSearchBy2ndMesh )
	{
		// 2次メッシュ矩形での検索なら、何もリンクIDで引いてくる必要はない（bug 7301）
		CTable cClass( OpenSameOwnerTable( strNetworkNodeTableName ) );
		_ICursorPtr ipCursor( cClass.Search( ipFilter, bRecycling ) );
		LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
		{
			Add( ipCursor, typeid(CRelNode&), emNetworkNodeTableType, cClass.GetFieldMap(), cClass.GetNameString(), bRecycling, bCreateCache, cErrs, false );
		}
	}
	else {
		if( ! empty(emNetworkLinkTableType) )
		{
			// FROM、TOからノードID列作成
			std::list<long> listNodeIDs;
			for( const_rows_iterator itRoad = begin(emNetworkLinkTableType); itRoad != end(emNetworkLinkTableType); ++itRoad )
			{
				listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ); // TODO: FROM、TOフィールド名はルールに定義していないが、良い？？
				listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );
			}

			listNodeIDs.sort();
			listNodeIDs.unique();
			// 検索
			CFeatureContainer::Select( strNetworkNodeTableName, listNodeIDs, typeid(CRelNode&), bRecycling, bCreateCache, cErrs, false ); // 初期化は最初にしてあるので最後はfalse
		}
	}
	// 関連オブジェクトはネットワークリンクのIDで検索するのでID列を作っておく
	// TODO: ただし、ID関連があるもののみ。将来形状のみの関連のものができたときは、そのときに考えよう
	std::list<long> listRoadIDs;
	for( const_rows_iterator itRoad = begin(emNetworkLinkTableType); itRoad != end(emNetworkLinkTableType); ++itRoad )
		listRoadIDs.push_back( (*itRoad)->GetOID() );

	// 内包オブジェクトの検索（ただしID関連があるもののみ）
	if( g_bNetworkSearchBy2ndMesh )
	{
		for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
			itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
		{
			if( it->second.IsEmpty() ) // ID関連があるもののみ
				continue;

			// 2次メッシュ矩形での検索なら、何もリンクIDで引いてくる必要はない（bug 7301）
			CTable cClass( OpenSameOwnerTable( it->first ) );
			_ICursorPtr ipCursor( cClass.Search( ipFilter, bRecycling ) );
			LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
			{
				Add( ipCursor, typeid(CFeature&), AheTableName2TableType(it->first), cClass.GetFieldMap(), cClass.GetNameString(), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() );
			}
		}
	}
	else {
		if( ! listRoadIDs.empty() )
		{
			for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
				itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
			{
				if( ! it->second.IsEmpty() ) // ID関連があるもののみ
				{
					// 検索
					CFeatureContainer::Select( it->first, listRoadIDs, typeid(CFeature&), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() ); // 初期化は最初にしてあるので最後はfalse
				}
			}
		}
	}
	// 外部テーブルの検索
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(GetTableName()),
		itEnd = g_cRuleRelationTable.end(GetTableName()); it != itEnd; ++it )
	{
		// *_RFフラグが付いているリンクOIDの収集
		std::list<long> listHasRFOID;
		long index = GetFieldMap()->FindField(it->get<0>()); // *_RF
		for( const_rows_iterator itLink = begin(emNetworkLinkTableType), itLinkEnd = end(emNetworkLinkTableType); itLink != itLinkEnd; ++itLink )
		{
			// TODO : RAILWAY_LINKは*_RFフラグを持っていないため、決め打ちで対応。（関連テーブルを削除するため）
			// 本来は関連フラグフィールド名が空だったらコピーしないとするべき。対応時はRAILWAL_NODEの方も忘れずに対応のこと。
			// そもそも、"関連テーブルをもつフィーチャクラスは関連フィールドを持つ"という前提でライブラリは構成されているので、
			// 関連テーブルをもつフィールドは関連フラグをもつようにモデル変更するべき
			if( 0 == (CString(GetTableName()).CompareNoCase(sindy::schema::sj::railway_link::kTableName)) ||
				1 == (*itLink)->GetLongValue( index, -1 ) )
				listHasRFOID.push_back( (*itLink)->GetOID() );
		}
		// 検索
		CFeatureContainer::Select( it->get<1>(), listHasRFOID, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), GetTableName() ); // 初期化は最初にしてあるので最後はfalse
	}

	if( bGetRelated )
	{
		CGeometry cSearchGeom;

		// 2次メッシュ上ノードの時は接続してる道路で再検索をかける
		CContainer cNodeFeatures; // 再検索フィーチャ
		for( const_rows_iterator it = begin(emNetworkNodeTableType), itEnd = end(emNetworkNodeTableType); it != itEnd; ++it )
		{
			CRelNode* pNode = static_cast<CRelNode*>(it->get());
			if ( pNode->IsBorderNode() ) {
/* 道路リンクのIDでノードを検索しているので、道路があるかどうかを調べる必要なし
				long lOID = (*it)->GetOID();

				for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad ) {
					if ( (*itRoad)->GetLongValueByFieldName( schema::road_link::kFromNodeID, -1 ) == lOID || (*itRoad)->GetLongValueByFieldName( schema::road_link::kToNodeID, -1 ) == lOID ) */
						cNodeFeatures.push_back( *it, false );
//				}
			}
		}

		// 再検索フィーチャがあれば、そのフィーチャの形状で検索をかける。
		if ( !cNodeFeatures.empty() )
		{
			IGeometryCollectionPtr ipGeomColl( CLSID_GeometryBag );

			((IGeometryPtr)ipGeomColl)->putref_SpatialReference( GetSpatialReference() );

			// 再検索の前の道路の両端を取得
			for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );
				ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
			}

			// 2次メッシュ上ノードの形状で再検索
			IGeometryPtr ipNodeGeom = cNodeFeatures.GetShape();
			ipNodeGeom->putref_SpatialReference( GetSpatialReference() );
			emErr = _Select( ipNodeGeom, bRecycling, bCreateCache, cErrs, false, false );

			// 再検索で取得した道路の形状を取得
			for ( const_rows_iterator itRoad = begin(emNetworkLinkTableType), itRoadEnd = end(emNetworkLinkTableType); itRoad != itRoadEnd; ++itRoad )
			{
				// 2次メッシュ上ノードに接続している道路の端点を取得
				for ( const_rows_iterator itNode = cNodeFeatures.begin(emNetworkNodeTableType),
					itNodeEnd = cNodeFeatures.end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
				{
					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() )
						ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint(0) );

					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() )
						ipGeomColl->AddGeometry( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ) );
				}
			}

			// 再検索によって2次メッシュ上ノードが新たに追加されたら、もう一度検索しなければいけないので。
			CContainer cReNodeFeatures; // 再々検索フィーチャ

			for( const_rows_iterator it = begin(emNetworkNodeTableType), itEnd = end(emNetworkNodeTableType); it != itEnd; ++it )
			{
				CRelNode* pNode = static_cast<CRelNode*>(it->get());

				if ( pNode->IsBorderNode() ) {
					if ( ! cNodeFeatures.find( *(*it) ) )
						cReNodeFeatures.push_back( *it );
				}
			}

			if ( !cReNodeFeatures.empty() )
				emErr = _Select( cReNodeFeatures.GetShape(), bRecycling, bCreateCache, NULL, false, true );

			cSearchGeom = (IGeometryPtr)ipGeomColl;

			// 周囲を検索？
			if ( (IGeometry*)cSearchGeom && !cSearchGeom.IsEmpty() )
				emErr = _Select( cSearchGeom, bRecycling, bCreateCache, NULL, false, false );	// 周辺再検索（リンク端点を用いる bug 6157）
		}

		// ネットワークノードに対する外部テーブルの検索
		for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(strNetworkNodeTableName),
			itEnd = g_cRuleRelationTable.end(strNetworkNodeTableName); it != itEnd; ++it )
		{
			// *_RFフラグが付いているノードOIDの収集
			std::list<long> listHasRFOID;
			long index = GetFieldMap()->FindField(it->get<0>()); // *_RF
			for( const_rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
			{
				// TODO : RAILWAY_NODEは*_RFフラグを持っていないため、決め打ちで対応。（関連テーブルを削除するため）
				// 本来は関連フラグフィールド名が空だったらコピーしないとするべき。対応時はRAILWAL_LINKの方も忘れずに対応のこと。
				// そもそも、"関連テーブルをもつフィーチャクラスは関連フィールドを持つ"という前提でライブラリは構成されているので、
				// 関連テーブルをもつフィールドは関連フラグをもつようにモデル変更するべき
				if( 0 == (strNetworkNodeTableName.CompareNoCase(sindy::schema::sj::railway_node::kTableName)) ||
					1 == (*itNode)->GetLongValue( index, -1 ) )
					listHasRFOID.push_back( (*itNode)->GetOID() );
			}
			// 検索
			CFeatureContainer::Select( it->get<1>(), listHasRFOID, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), strNetworkNodeTableName ); // 初期化は最初にしてあるので最後はfalse
		}
	}

	return emErr;
}

// 属性ダイアログ専用
sindyErrCode CRoadNetwork::SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /* = true*/ )
{
	return _SelectByObject( ipRow, bRecycling, bCreateCache, bInit );
}

sindyErrCode CRoadNetwork::_SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /* = true*/, bool bGetRelated /* = true*/ )
{
	// 引数チェック
	RETURN_IF( ! ipRow, sindyErr_ArgIsNull )

	CRow cRow( ipRow );

	// 接続プロパティが同じかどうかを確認
	RETURN_IF( ! ( *GetNameString() == *cRow.GetNameString() ), sindyErr_ArgLimitOver )

	// ネットワーク関連オブジェクトかどうか検査
	// TODO: CFeatureContainerに移植する場合はちゃんとルールを見て
	// どの関連を適用すべきかを調べる必要がある
	RETURN_IF( ! AheIsRelateWithNetwork( cRow.GetTableName() ), sindyErr_ArgLimitOver )

	// 初期化
	if( bInit )
		clear();

	// 関連元締めのネットワークを検索してコンテナに格納
	CContainer cLinkContainer;
	// TODO: LINK_IDを直接指定しているが、CFeatureContainerに移植する場合は
	// ちゃんとルールからたどってフィールド名を取得する必要がある
	// 
	// ネットワークリンクの場合はそのまま、
	// ネットワークノードの場合はOBJECTIDをFROM、TOで保持しているものを検索、
	// 内包される側の場合は外部キーがOBJECTIDになるものを検索、（TODO: ID関連がないものは形状で引く必要がある）
	// TODO: 外部レコードの場合も外部キーがOBJECTIDになるものを検索、
	// TODO: リンク列の場合は…
	IQueryFilterPtr ipFilter;
	if( g_cRuleModelNetwork.IsNetworkLink( cRow.GetTableName() ) )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("OBJECTID=%d"), cRow.GetOID() );
	else if( g_cRuleModelNetwork.IsNetworkNode( cRow.GetTableName() ) )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("FROM_NODE_ID=%d OR TO_NODE_ID=%d") ,cRow.GetOID(), cRow.GetOID() );
	else if( g_cRuleRelationshipContain.find_value( cRow.GetTableName() ) != g_cRuleRelationshipContain.end() )
		ipFilter = AheInitQueryFilter( ipFilter, NULL, _T("OBJECTID=%d") ,cRow.GetLongValueByFieldName(_T("LINK_ID"), -1) ); // TODO: ちゃんとルールから引くべき。それと、ID関連がないものについて考慮されていない

	return _Select( ipFilter, bRecycling, bCreateCache, NULL, bInit, bGetRelated );
}


// メンバをクリアする
void CRoadNetwork::clear()
{
	CFeatureContainer::clear();
	m_mapLinkQueue.clear();
}

// キャッシュされているかどうかをチェックする
bool CRoadNetwork::IsCached() const
{
	if( CFeatureContainer::IsCached() )
	{
		for( std::map<sindyTableType::ECode, std::list<CLinkQueue> >::const_iterator it = m_mapLinkQueue.begin(); it != m_mapLinkQueue.end(); ++it )
		{
			for( std::list<CLinkQueue>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( ! it2->IsCached() )
					return false;
			}
		}
		return true;
	}
	return false;
}

// 与えられた形状との交差部分で分割処理を行う
errorcode::sindyErrCode CRoadNetwork::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			// Splitした後に分割点を追加すると、実際に分割した点とずれている可能性があるのでおかしくなってしまうので、
			// 先に分割点を追加しておく。
			CString strTableName( pSource->GetTableName() );

			if( ! AheIsNetworkNode( strTableName ) ) {
				if ( AheIsNetworkLink( strTableName ) ) {
					// 内包関係を分割
					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						if ( (*itRoad) == pSource ) {
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								CContainer cContains;

								for( const_rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
									cContains.push_back( (*itSeg) );

								// 内包関係に分割点を追加
								for ( CGeometry::const_point_iterator itPt = cGeom.point_begin(), itPtEnd = cGeom.point_end(); itPt != itPtEnd; ++itPt ) {
									for ( CContainer::rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg ) {
										CGeometry cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
										CGeometry* pSegGeom = (*itSeg)->GetShape();

										cPtGeom.PutRefSpatialReference( pSegGeom->GetSpatialReference() );
										// 分割点がセグメント上にあるけど構成点がない時は追加
										if ( AhePointIsOnGeometry( (IGeometry*)(*pSegGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pSegGeom->IntersectPoints( cPtGeom ) )
											pSegGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );
									}
								}

								// 内包関係を分割
								for ( CContainer::rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg )
									_Split( (*itSeg), cGeom, cErrObjects );
							}

							break;
						}
					}
				} else {
					// 分割点を道路と他の内包関係に追加
					// 所属する道路を検索
					CSPRow pRoad;

					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						// 内包関係のもの
						for( rows_iterator itSeg = (*itRoad)->begin(pSource->GetTableType()),
							itSetEnd = (*itRoad)->end(pSource->GetTableType());
							itSeg != itSetEnd; ++itSeg )
						{
							if ( (*itSeg) == pSource ) {
								pRoad = (*itRoad);
								break;
							}
						}

						if ( pRoad )
							break;
					}

					if ( pRoad ) {
						// 分割点を道路に追加
						for ( CGeometry::const_point_iterator itPt = cGeom.point_begin(), itPtEnd = cGeom.point_end(); itPt != itPtEnd; ++itPt ) {
							CGeometry cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
							CGeometry* pRoadGeom = pRoad->GetShape();

							cPtGeom.PutRefSpatialReference( pRoadGeom->GetSpatialReference() );
							// 分割点がセグメント上にあるけど構成点がない時は追加
							if ( AhePointIsOnGeometry( (IGeometry*)(*pRoadGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pRoadGeom->IntersectPoints( cPtGeom ) )
								pRoadGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );

							// 内包関係に分割点を追加
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								for( rows_iterator itSeg = pRoad->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = pRoad->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
								{
									if ( (*itSeg) != pSource ) {
										cPtGeom = (IGeometryPtr)AheCreateClone( (IGeometry*)*itPt );
										CGeometry* pSegGeom = (*itSeg)->GetShape();

										cPtGeom.PutRefSpatialReference( pSegGeom->GetSpatialReference() );
										// 分割点がセグメント上にあるけど構成点がない時は追加
										if ( AhePointIsOnGeometry( (IGeometry*)(*pSegGeom), (IPointPtr)(IGeometry*)cPtGeom ) && ! pSegGeom->IntersectPoints( cPtGeom ) )
											pSegGeom->AddVertex( (IPointPtr)(IGeometry*)cPtGeom );
									}
								}
							}
						}
					}
				}
			}

			// 分割
			_Split( pSource, cGeom, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 与えられた形状との交差部分で分割処理を行う
errorcode::sindyErrCode CRoadNetwork::_Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			CString strTableName( pSource->GetTableName() );

			if( ! AheIsNetworkNode( strTableName ) ) {
				if ( AheIsNetworkLink( strTableName ) ) {
					CGeometry cSplittedGeoms, cSplittedPoints;

					// Split
					CPoint cFromP( pFeature->GetShape()->GetPoint(0) ); // 始点をコピー（bug 7300）
					if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
						if ( (IGeometry*)cSplittedGeoms ) {
							if ( cSplittedGeoms.GetPartCount() > 0 ) {
								CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
								sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );

								for ( rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
									if ( (*itRoad) == pSource ) {
										CContainer cNewNodes;	// ノード種別の再設定で使用
										CContainer cNewRoads;	// リンク列の分割で使用
										CTable cNodeTable( (*begin(emNetworkNodeTableType))->GetTable() );

										// 分割形状を割り当てる
										for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
											CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

											if ( (IGeometry*)cNewGeom ) {
												CSPRow pRoad;
												WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];
												((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );
												// 分割前の始点を持つものをID引継ぎ対象とする（bug 7300）
												if( cFromP.Equals( point_wks[0] ) )
												{
													// 現在の中身と入れ替える
													((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
													pRoad = pSource;
												} 
												else {
													// フィーチャ作成
													pRoad.reset( new CRelRoad( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

													CTable( pSource->GetTable() ).CreateRow( *pRoad );

													// 属性をコピー
													pSource->CopyTo( *pRoad, true, false, false, false );

													// 形状をセット
													((IPointCollectionPtr)(IGeometry*)*pRoad->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

													// 外部テーブル関係関連付け
													for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
														itOuterEnd = g_cRuleRelationTable.end(GetTableName());
														itOuter != itOuterEnd; ++itOuter )
													{
														for( rows_iterator itOneway = pSource->begin(AheTableName2TableType(itOuter->get<1>())),
															itOnewayEnd = pSource->end(AheTableName2TableType(itOuter->get<1>()));
															itOneway != itOnewayEnd; ++itOneway )
														{
															// 作成
															CSPRow cNew;

															cNew.reset( new CRow( NULL, (*itOneway)->GetTableType(), true, (*itOneway)->GetFieldMap(), (*itOneway)->GetNameString() ) );

															CTable cTable( (*itOneway)->GetTable() );

															cTable.CreateRow( *cNew.get() );

															// 属性をコピー
															(*itOneway)->CopyTo( *cNew.get(), true, false, false, false );

															cNew->SetLongValueByFieldName( itOuter->get<2>(), pRoad->GetOID() );

															// 道路に格納
															pRoad->push_back( cNew );
														}
													}

													// 道路を格納
													push_back( pRoad );
													cNewRoads.push_back( pRoad );
												}
												delete [] point_wks;
												point_wks = NULL;

												// ノードが存在しているかチェック
												bool bHasFrom = false, bHasTo = false;
												CGeometry cFront = cNewGeom.GetPoint( 0 );
												CGeometry cBack = cNewGeom.GetPoint( cNewGeom.GetPointCount() - 1 );
												CSPRow pFromNode, pToNode;

												for ( const_rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType);
													itNode != itNodeEnd; ++itNode )
												{
													if ( (*itNode)->GetShape()->Equals( cFront ) ) {
														pFromNode = (*itNode);
														bHasFrom = true;
													}

													if ( (*itNode)->GetShape()->Equals( cBack ) ) {
														pToNode = (*itNode);
														bHasTo = true;
													}
												}

												if ( ! bHasFrom ) {	// FROM側のノードがない？
													// ノード作成
													CSPRow cNewNode;

													cNewNode.reset( new CRelNode( NULL, emNetworkNodeTableType, true ) );

													cNodeTable.CreateRow( *cNewNode.get() );
													cNewNode->SetShape( cFront );	// 形状セット

													// キャッシュ作成
													cNewNode->CreateCache();

													if ( pRoad->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) != cNewNode->GetOID() )
														// 道路のFROM_NODE_IDセット
														pRoad->SetLongValueByFieldName(schema::road_link::kFromNodeID, cNewNode->GetOID() );

													// 追加
													push_back( cNewNode );
													cNewNodes.push_back( cNewNode );
												} else {
													// ある場合でも、NODE_IDが合っていない時があるので
													if ( pRoad->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) != pFromNode->GetOID() )
														// 道路のFROM_NODE_IDセット
														pRoad->SetLongValueByFieldName(schema::road_link::kFromNodeID, pFromNode->GetOID() );
												}

												if ( ! bHasTo ) {	// TO側のノードがない？
													// ノード作成
													CSPRow cNewNode;

													cNewNode.reset( new CRelNode( NULL, emNetworkNodeTableType, true ) );

													cNodeTable.CreateRow( *cNewNode.get() );
													cNewNode->SetShape( cBack );	// 形状セット

													// キャッシュ作成
													cNewNode->CreateCache();

													if ( pRoad->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) != cNewNode->GetOID() )
														// 道路のFROM_NODE_IDセット
														pRoad->SetLongValueByFieldName(schema::road_link::kToNodeID, cNewNode->GetOID() );

													// 追加
													push_back( cNewNode );
													cNewNodes.push_back( cNewNode );
												} else {
													// ある場合でも、NODE_IDが合っていない時があるので
													if ( pRoad->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) != pToNode->GetOID() )
														// 道路のTO_NODE_IDセット
														pRoad->SetLongValueByFieldName(schema::road_link::kToNodeID, pToNode->GetOID() );
												}
											}
										}

										// ノード種別の再設定
										for ( CContainer::rows_iterator itNode = cNewNodes.begin(emNetworkNodeTableType), itNodeEnd = cNewNodes.end(emNetworkNodeTableType); itNode != itNodeEnd; ++itNode )
											// ノード種別の再設定
											ResetNodeAttribute( (*itNode) );

										// リンク列の分割
										for ( linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
											// INF分
											for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
												if ( itLinkq->HasRoad( *pSource.get() ) ) {	// INFのLQに道路がある？
													if ( sindyErr_NoErr != itLinkq->Split( *pSource.get(), GetTableType(), cNewRoads, cErrObjects ) )
														cErrObjects.push_back( CErrorObject( *itLinkq ) );
												}
											}
										}

										break;
									}
								}

								// 内包関係関連付け
								for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
									itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
									itContain != itContainEnd; ++itContain )
								{
									CContainer cContains;

									for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
										// 変更があった内包関係取得
										for( rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
											itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
											itSeg != itSetEnd; ++itSeg )
										{
											schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

											if ( (*itSeg)->GetLongValueByFieldName( itContain->second, -1 ) == pSource->GetOID() ||
												( (*itSeg)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) )
												cContains.push_back( (*itSeg) );
										}
									}

									// 関連付け
									for ( CContainer::const_rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), itSegEnd = cContains.end(AheTableName2TableType(itContain->first)); itSeg != itSegEnd; ++itSeg ) {
										// [Bug 6076]道路リンクのセグメント削除時にリンク内属性が削除されない場合がある
										// 削除されるもの以外の関連付けを変更するようにしないと、所属がないものがでてきてしまう。
										if ( ! (*itSeg)->IsDeleted() ) {
											for ( rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
												if ( (*itRoad)->GetShape()->Contains( (*itSeg)->GetShapeCopy() ) || (*itRoad)->GetShape()->Equals( (*itSeg)->GetShapeCopy() ) ) {
													// LINK_ID設定
													(*itSeg)->SetLongValueByFieldName( itContain->second, (*itRoad)->GetOID() );

													// 道路に格納
													(*itRoad)->push_back( (*itSeg) );
												} else
													// 所属する道路が変わったので除外
													(*itRoad)->erase( (*itSeg) );
											}
										}
									}
								}
								// いらなくなったノードを削除（端点ノード）
								
								
								std::vector<long> listNode;
								std::set<long> listFromTo;
								for ( rows_iterator it = begin(emNetworkNodeTableType); it != end(emNetworkNodeTableType); ++it ) {
									listNode.push_back( (*it)->GetOID() );
								}
								
								for ( rows_iterator it = begin(GetTableType()); it != end(GetTableType()); ++it ) {
									listFromTo.insert( (*it)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1));
									listFromTo.insert( (*it)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1));
								}
								std::vector<long> listDelete;
								//std::set_difference(listNode.begin(), listNode.end(),listFromTo.begin(), listFromTo.end(), back_inserter(listDelete));
								for(std::vector<long>::iterator it=listNode.begin();it!=listNode.end();it++){
									if(listFromTo.find(*it)==listFromTo.end())
										listDelete.push_back(*it);
								}

								for ( rows_iterator it = begin(emNetworkNodeTableType); it != end(emNetworkNodeTableType); ++it ) {
									long oid = (*it)->GetOID();
									for( std::vector<long>::iterator itOID = listDelete.begin();itOID!=listDelete.end();++itOID){
										if(oid==*itOID){
											(*it)->Delete();
											listDelete.erase(itOID);
											break;
										}
									}
								}
								
								
								
								// いらなくなったノードを削除（端点ノード）
								/*
								std::cerr << size(emNetworkNodeTableType) << std::endl;
								for ( rows_iterator itNode = begin(emNetworkNodeTableType); itNode != end(emNetworkNodeTableType); ++itNode ) {
									bool bIsFind = false;

									for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
										if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) == (*itNode)->GetOID() ||
											(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) == (*itNode)->GetOID() ) {
											bIsFind = true;
											break;
										}
									}

									if ( ! bIsFind )
										(*itNode)->Delete();
								}
								
								*/
							}
						}
					}
				} else {
					// 分割点を道路と他の内包関係のものに追加
					// 所属する道路を検索
					CSPRow pRoad;

					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						// 内包関係のもの
						for( rows_iterator itSeg = (*itRoad)->begin(pSource->GetTableType()),
							itSetEnd = (*itRoad)->end(pSource->GetTableType());
							itSeg != itSetEnd; ++itSeg )
						{
							if ( (*itSeg) == pSource ) {
								pRoad = (*itRoad);
								break;
							}
						}

						if ( pRoad )
							break;
					}

					if ( pRoad ) {
						CGeometry cSplittedGeoms, cSplittedPoints;

						// Split
						if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
							if ( (IGeometry*)cSplittedGeoms ) {
								if ( cSplittedGeoms.GetPartCount() > 0 ) {
									// 所属する道路に作成フィーチャを格納
									CTable cTable( pSource->GetTable() );

									// 分割形状を割り当てる
									for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
										CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

										if ( (IGeometry*)cNewGeom ) {
											WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

											((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

											if ( itPart == cSplittedGeoms.part_begin() ) {
												// 現在の中身と入れ替える
												((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
											} else {
												// フィーチャ作成
												CSPRow cNew;

												cNew.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

												cTable.CreateRow( *cNew.get() );

												// 属性をコピー
												pSource->CopyTo( *cNew.get(), true, false, false, false );

												// 形状をセット
												((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

												// 所属する道路に追加
												pRoad->push_back( cNew );
											}
											delete [] point_wks;
											point_wks = NULL;
										}
									}
								} else {
									// ないことはありえないはず…
									cErrObjects.push_back( CErrorObject( *pSource.get() ) );
								}
							}
						}
					}
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 外部テーブル関係を削除
void DeleteRelation( CSPRow& row, const CString& table )
{
	// itOuter <0>:親のテーブル名 <1>:削除対象のテーブル名 <2>:<1>のテーブルでキーとなるフィールド名
	for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(table), 
		itOuterEnd = g_cRuleRelationTable.end(table);
		itOuter != itOuterEnd; ++itOuter )
	{
		for( CContainer::rows_iterator itOuterObj = row->begin(AheTableName2TableType(itOuter->get<1>())),
			itOuterObjEnd = row->end(AheTableName2TableType(itOuter->get<1>()));
			itOuterObj != itOuterObjEnd; ++itOuterObj )
		{
			// 再帰でさらに関連づいているものも全て消す
			DeleteRelation( (*itOuterObj), itOuter->get<1>() );
			(*itOuterObj)->Delete();
		}
	}
}
errorcode::sindyErrCode CRoadNetwork::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		if( *pSource->GetNameString() == *pDest->GetNameString() && AheIsNetworkLink( pSource->GetTableName() ) )	// 同じタイプ？
		{
			CString strNetworkNodeFC = AheGetNetworkNodeFeatureClassName(GetTableName());
			sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(strNetworkNodeFC); // ネットワークノードタイプ

			// マージ先道路にある規制＆関連フィーチャのLINK_IDを変更
			//const_rows_iterator itRoad = find( GetTableType(), pDest->GetOID() );

			//if ( itRoad != end(GetTableType()) ) {
				// 内包関係
				for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()), 
					itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
					itContain != itContainEnd; ++itContain )
				{
					CContainer cContains;

					for( rows_iterator itContainFeat = pDest->begin(AheTableName2TableType(itContain->first)), 
						itContainFeatEnd = pDest->end(AheTableName2TableType(itContain->first));
						itContainFeat != itContainFeatEnd; ++itContainFeat )
					{
						if( sindyErr_NoErr != (*itContainFeat)->SetLongValueByFieldName( itContain->second, pSource->GetOID() ) )
							cErrObjects.push_back( CErrorObject( *itContainFeat->get() ) );

						cContains.push_back( (*itContainFeat) );
					}

					// [Bug 5949] 非交差処理設定がなくなった
					// 所属を変更していないと内包関係フィーチャの構成点削除ができないので。
					// 所属する道路が変わったのでマージ元から除外しておく
					for ( CContainer::const_rows_iterator itSeg = cContains.begin(AheTableName2TableType(itContain->first)), 
						itSegEnd = cContains.end(AheTableName2TableType(itContain->first));
						itSeg != itSegEnd; ++itSeg )
					{
						// マージ先に所属変更
						pSource->push_back( (*itSeg) );

						pDest->erase( (*itSeg) );
					}
				}
				// 外部テーブル関係を削除
				DeleteRelation( pDest, GetTableName() );
			//}

			if ( cErrObjects.empty() ) {
				// From_Node_ID / To_Node_IDの変更
				long nSourceFromID = pSource->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1);
				long nSourceToID = pSource->GetLongValueByFieldName(schema::road_link::kToNodeID, -1);
				long nDestFromID = pDest->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1);
				long nDestToID = pDest->GetLongValueByFieldName(schema::road_link::kToNodeID, -1);
				bool bIsReverse = false;	// 形状を反転するかどうか
				long nDelNodeID = -1;	// 削除ノードID
				long nChkNodeID = -1;	// 属性チェックノードID

				if ( nSourceFromID == nDestFromID ) {			// T---FF---T
					nDelNodeID = nSourceFromID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kFromNodeID, nDestToID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceFromID;
					bIsReverse = true;
				} else if ( nSourceFromID == nDestToID ) {	// T---FT---F
					nDelNodeID = nSourceFromID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kFromNodeID, nDestFromID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceFromID;
				} else if ( nSourceToID == nDestFromID ) {	// F---TF---T
					nDelNodeID = nSourceToID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kToNodeID, nDestToID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceToID;
				} else if ( nSourceToID == nDestToID ) {		// F---TT---F
					nDelNodeID = nSourceToID;

					if ( sindyErr_NoErr != pSource->SetLongValueByFieldName(schema::road_link::kToNodeID, nDestFromID) )
						cErrObjects.push_back( CErrorObject( (*pSource) ) );

					nChkNodeID = nSourceToID;
					bIsReverse = true;
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					cErrObjects.push_back( CErrorObject( (*pDest) ) );
				}

				if ( cErrObjects.empty() ) {
					CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

					// 形状のマージ
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// ノードの削除
						rows_iterator itNode = find( emNetworkNodeTableType, nDelNodeID );

						if ( itNode != end(emNetworkNodeTableType) )
						{
							(*itNode)->Delete();
							DeleteRelation( *itNode, strNetworkNodeFC );
						}

						if ( cErrObjects.empty() ) {
							// リンク列のマージ
							for ( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
								bool bEdit = false;

								// INF分
								for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
									if ( ! itLinkq->IsDeleted() ) {
										itLinkq->Merge( (*pSource), (*pDest), bIsReverse, GetTableType(), *this, cErrObjects );

										// マージ元にLQがあればLQRFのフラグを立てる
										for ( CLinkQueue::iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ ) {
											if ( (*itLQ).GetLinkId() == pSource->GetOID() ) {
												// マージ元のLQRFのフラグを立てる
												for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
													if ( itLinkqs->first == AheTableName2TableType(itInf->get<0>()) ) {
														if ( sindyErr_NoErr != pSource->SetBoolValueByFieldName(itInf->get<2>(), true) )
															cErrObjects.push_back( CErrorObject( (*pSource) ) );
														break;
													}
												}

												break;
											}
										}

										bEdit = true;
									}
								}

								if ( bEdit )
									ResetRoadLQRF( itLinkqs->first, cErrObjects );	// 道路のLQRF再設定
							}

							if ( cErrObjects.empty() ) {
								// マージ先道路の削除
								pDest->Delete();

								if ( cErrObjects.empty() ) {
									// ノード種別の再設定
									itNode = find( emNetworkNodeTableType, nChkNodeID );

									if ( itNode != end(emNetworkNodeTableType) )
										ResetNodeAttribute( (*itNode) );
								}
							}
						}
					}
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly/*=false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if ( pFeature ) {
			CGeometry cIntersect = pFeature->GetShape()->IntersectPoints( cGeom );

			if( (IGeometry*)cIntersect && ! cIntersect.IsEmpty() )
			{ 
				if( AheIsNetworkLink( pRow->GetTableName() ) )
				{
					// 内包関係のもの
					for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
						if ( (*itRoad) == pRow ) {
							for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
								itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
								itContain != itContainEnd; ++itContain )
							{
								for( rows_iterator itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
									itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
									itSeg != itSetEnd; ++itSeg )
								{
									if ( sindyErr_NoErr != Erase( *itSeg, cIntersect, cErrObjects ) )
										break;
								}
							}

							break;
						}
					}

					// 始終点以外の点を削除
					CGeometry cFront = pFeature->GetShape()->GetPoint( 0 );
					CGeometry cBack = pFeature->GetShape()->GetPoint( pFeature->GetShape()->GetPointCount() - 1 );
					bool bHasFront = false, bHasBack = false;

					for ( CGeometry::point_iterator it = cIntersect.point_begin(); it != cIntersect.point_end(); ++it ) {
						if ( cFront.Equals( *it ) )
							bHasFront = true;
						else if ( cBack.Equals( *it ) )
							bHasBack = true;
						else
							pFeature->GetShape()->EraseVertex( *it );
					}

					if ( ! bHasFront && ! bHasBack ) {
						if ( pFeature->GetShape()->GetPointCount() <= 1 )
							Delete( pRow, cErrObjects );
					} else {
						CGeometry cBefore( (IGeometryPtr)AheCreateClone( pFeature->GetShapeCopy() ) );	// 分割される前の形状保持
						CGeometry cFirst, cEnd;
						long nCnt = pFeature->GetShape()->GetPointCount() - 1, i = 1;

						for ( CGeometry::point_iterator it = pFeature->GetShape()->point_begin(); it != pFeature->GetShape()->point_end(); ++it, ++i ) {
							if ( bHasFront ) {
								if ( ! (IGeometry*)cFirst && i == 2 )
									cFirst = (IGeometryPtr)AheCreateClone( *it );	// 始点の次の点
							}

							if ( bHasBack ) {
								if ( ! (IGeometry*)cEnd && nCnt == i )
									cEnd = (IGeometryPtr)AheCreateClone( *it );	// 終点の前の点
							}
						}

						// 削除される始終点側のノードが2次メッシュノードなら、
						// 反対側の道路の始終点も削除する。
						CListLong listNodeIDs;

						if ( bHasFront )
							listNodeIDs.push_back( pFeature->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) );

						if ( bHasBack )
							listNodeIDs.push_back( pFeature->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) );

						std::map< long, CContainer > mapRoads;	// map<ノードID,list<道路>>

						for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
							itRoad != itRoadEnd; ++itRoad )
						{
							if( pRow != *itRoad )
							{
								mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1)].push_back( *itRoad );
								mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1)].push_back( *itRoad );
							}
						}

						// 2次メッシュノード抽出
						sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName())); // ノードテーブルタイプ
						CContainer cEdgeNodes;

						for ( CListLong::const_iterator itIDs = listNodeIDs.begin(), itIDsEnd = listNodeIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
							rows_iterator itNode = end(emNetworkNodeTableType);

							if ( end(emNetworkNodeTableType) != (itNode = find( emNetworkNodeTableType, (*itIDs) )) ) {
								CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

								// 2次メッシュ上ノードに接続している道路の削除
								if ( pNode->IsBorderNode() ) {
									if ( mapRoads.find( pNode->GetOID() ) != mapRoads.end() )
										cEdgeNodes.push_back( *itNode );
								}
							}
						}

						if ( bHasFront ) {
							// 始点の次の点で分割
							if ( sindyErr_NoErr == Split( pRow, cFirst, cErrObjects ) ) {
								CContainer cRoads;

								// 分割点をもつものを抽出(2個しかないはず)
								for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetShape()->IntersectPoints( cFirst ) )
											cRoads.push_back( (*itRoad) );
									}
								}

								// 分割される前の形状の始点をもつものがオリジナルかチェック
								// 作成された側ならオリジナルと入れ替え
								bool bIsSwap = false;
								CSPRow cOrgRoad, cNewRoad;

								for( const_rows_iterator itRoad = cRoads.begin(GetTableType()), itRoadEnd = cRoads.end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									if ( (*itRoad)->GetShape()->GetPoint( 0 ).Equals( cBefore.GetPoint( 0 ) ) ) {
										schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

										if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kCreated ) {
											cOrgRoad = (*itRoad);
											bIsSwap = true;
										} else
											cNewRoad = (*itRoad);
									} else
										cNewRoad = (*itRoad);
								}

								if ( cNewRoad ) {
									if ( bIsSwap )
										// 中身を入れ替え
										swap( cOrgRoad, cNewRoad );

									// 分割でできた道路を削除
									CContainer listexclusions;

									listexclusions.push_back( cNewRoad );

									_Delete( cNewRoad, listexclusions, false, cErrObjects );
								} else {
									// 分割でできなかった時（構成点が2点しかない時）は、オリジナルを削除
									CContainer listexclusions;

									listexclusions.push_back( pRow );

									_Delete( pRow, listexclusions, false, cErrObjects );
								}
							}
						}

						if ( bHasBack ) {
							// 終点の前の点で分割
							if ( sindyErr_NoErr == Split( pRow, cEnd, cErrObjects ) ) {
								CContainer cRoads;

								// 分割点をもつものを抽出(2個しかないはず)
								for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetShape()->IntersectPoints( cEnd ) )
											cRoads.push_back( (*itRoad) );
									}
								}

								// 分割される前の形状の始点をもつものがオリジナルかチェック
								// 作成された側ならオリジナルと入れ替え
								bool bIsSwap = false;
								CSPRow cOrgRoad, cNewRoad;

								for( const_rows_iterator itRoad = cRoads.begin(GetTableType()), itRoadEnd = cRoads.end(GetTableType());
									itRoad != itRoadEnd; ++itRoad )
								{
									if ( (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 ).Equals( cBefore.GetPoint( cBefore.GetPointCount() - 1 ) ) ) {
										schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

										if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kCreated ) {
											cOrgRoad = (*itRoad);
											bIsSwap = true;
										} else
											cNewRoad = (*itRoad);
									} else
										cNewRoad = (*itRoad);
								}

								if ( cNewRoad ) {
									if ( bIsSwap )
										// 中身を入れ替え
										swap( cOrgRoad, cNewRoad );

									// 分割でできた道路を削除
									CContainer listexclusions;

									listexclusions.push_back( cNewRoad );

									_Delete( cNewRoad, listexclusions, false, cErrObjects );
								} else {
									// 分割でできなかった時（構成点が2点しかない時）は、オリジナルを削除
									CContainer listexclusions;

									listexclusions.push_back( pRow );

									_Delete( pRow, listexclusions, false, cErrObjects );
								}
							}
						}

						// 2次メッシュノードに接続する反対側の道路の削除
						for ( CContainer::rows_iterator itNode = cEdgeNodes.begin(emNetworkNodeTableType); itNode != cEdgeNodes.end(emNetworkNodeTableType); ++itNode ) {
							for ( CContainer::rows_iterator itRoad = mapRoads[(*itNode)->GetOID()].begin(GetTableType()); itRoad != mapRoads[(*itNode)->GetOID()].end(GetTableType()); ++itRoad ) {
								if ( ! (*itRoad)->Changed() )
									Erase( *itRoad, cIntersect, cErrObjects );
							}
						}
					}
				}
				// AheIsNetwork()以外
				else {
					// 内包関係ものの構成点削除
					// マルチポイントの場合もあるので、分解して削除
					for( long i=0; i<cIntersect.GetPointCount(); ++i )
						deleteContainVertex( pRow, (CGeometry)cIntersect.GetPoint(i), cErrObjects );
				}
			}
		}
		else
			cErrObjects.push_back( CErrorObject( (*pRow) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

bool CRoadNetwork::isOnNodePoint( const CGeometry& geom, bool onlyMeshBorder /*= false*/ )
{
	// 抱えてるノードを検索
	sindyTableType::ECode nodeType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName()));
	CSPRow onBorderNode;
	for( auto itNode = begin(nodeType), itEnd = end(nodeType);
		 itNode != itEnd; ++itNode )
	{
		CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

		// 二次メッシュ境界限定
		if( onlyMeshBorder && !pNode->IsBorderNode() )
			continue;

		// geomは（マルチ）ポイント前提
		if( !geom.Disjoint(*pNode->GetShape()) )
			return true;
	}

	return false;
}


void CRoadNetwork::deleteContainVertex( CSPRow& containRel, const CGeometry& delVertex, CErrorObjects& cErrObjects )
{
	CFeature* pFeature = dynamic_cast<CFeature*>( containRel.get() );

	if( !pFeature || pFeature->IsDeleted() )
		return;

    // 既に削除されている場合もある
	if( delVertex.Disjoint( (IGeometry*)(*pFeature->GetShape()) ) )
		return;

	// 自分の構成点を削除
	// 構成点削除の結果、自分自身がなくなったかどうかはCFeature::Erase()でうまいことやってるのでここでは気にしない
	pFeature->Erase( delVertex, cErrObjects );

	// 親となるリンクからも削除
	deleteSharedVertex( getParentLink( containRel ), delVertex );

	// 自分自身が消えたか、2次メッシュ上の構成点が消されたら、反対側も消す
	if( pFeature->IsDeleted() || isOnNodePoint( delVertex, true ) )
	{
		// 反対側は削除前の形状で探す
		CSPRow opposite = getContainOppositeMesh( containRel, true );
		if( opposite )
		{
			opposite->Trace();
			// 反対側は全削除
			deleteContainRelation( opposite );
		}
	}
}

void CRoadNetwork::deleteContainRelation( CSPRow& containLine )
{
	if( containLine->IsDeleted() )
		return;

	// とりあえず自分を消す
	containLine->Delete();

	// 2次メッシュ境界だったら反対側も
	CSPRow opposite = getContainOppositeMesh( containLine );
	if( opposite )
		deleteContainRelation( opposite);
}

void CRoadNetwork::deleteSharedVertex( CSPRow& link, const CGeometry& vertices )
{
	if( !link || link->IsDeleted() )
		return;

	// overlap の構成点と一致する target の構成点を削除するラムダ式
	auto deleteVertex = [&]( CSPRow& target, const CGeometry& overlap )
	{
		if( target->IsDeleted() )
			return;

		target->Trace();

		long pointNum = overlap.GetPointCount();
		for( long i=0; i<pointNum; ++i)
		{
			CPoint vertex = overlap.GetPoint(i);
			
			// 構成点だけを対象にするのでノード（リンクの始終点）は除外
			// また、内包ものがノードで接する場合、片方だけ消したい場合もあるので他のは消さない
			// 消したい方の削除と2次メッシュ境界云々は呼び出し側（deleteContainVertex）で面倒を見る
			if( isOnNodePoint( (CGeometry)vertex ) )
				continue;

			// 構成点削除
			target->GetShape()->EraseVertex( vertex );
		}
	};

	// 道路リンクの共有構成点削除
	deleteVertex( link, vertices );

	// 内包関係のものからも全て削除
	for( auto itContain = g_cRuleRelationshipContain.begin(GetTableName()),
		itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
		itContain != itContainEnd; ++itContain )
	{
		for( auto itSeg = link->begin(AheTableName2TableType(itContain->first)),
			itSetEnd = link->end(AheTableName2TableType(itContain->first));
			itSeg != itSetEnd; ++itSeg )
		{
			deleteVertex( *itSeg, vertices );

			// 共有構成点として削除された結果自分が消えることもあるので、
			// その場合は二次メッシュを考慮しなければならない
			if( (*itSeg)->GetShape()->GetPointCount() == 1 )
				(*itSeg)->Delete();

			if( !(*itSeg)->IsDeleted() )
				continue;

			CSPRow opposite = getContainOppositeMesh( *itSeg, true );
			if( opposite )
				deleteContainRelation(opposite);
		}
	}
}

CSPRow CRoadNetwork::getParentLink( const CSPRow& containRel )
{
	// 親検索
	CSPRow pRoad;

	for ( auto itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
		// 内包関係のもの
		for( auto itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			for( auto itSeg = (*itRoad)->begin(AheTableName2TableType(itContain->first)),
				itSetEnd = (*itRoad)->end(AheTableName2TableType(itContain->first));
				itSeg != itSetEnd; ++itSeg )
			{
				if ( containRel == *itSeg ) {
					pRoad = *itRoad;
					break;
				}
			}
		}
		if ( pRoad )
			break;
	}

	return pRoad;
}

CSPRow CRoadNetwork::getContainOppositeMesh( const CSPRow& row, bool useOrgShape /*= false*/  )
{
	// 取得対象の形状
	const CGeometry geom = useOrgShape ? CGeometry(row->GetShapeCopy( false, true )) : *row->GetShape();

	// 始終点を取得
	CGeometry fromP = geom.GetPoint(0);
	CGeometry toP = geom.GetPoint( geom.GetPointCount()-1 );

	// 抱えてるノードを検索
	sindyTableType::ECode nodeType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName()));
	CSPRow onBorderNode;
	for( auto itNode = begin(nodeType), itEnd = end(nodeType);
		 itNode != itEnd; ++itNode )
	{
		CRelNode* pNode = static_cast<CRelNode*>(itNode->get());
		if( !pNode->IsBorderNode() )
			continue;

		// 接している２次メッシュ境界ノードがあれば取得
		if( fromP.Equals(*pNode->GetShape()) || toP.Equals(*pNode->GetShape()) )
		{
			onBorderNode = *itNode;
			break;
		}
	}

	// 2次メッシュ境界と無縁ならnullのまま返して終了
	if( !onBorderNode )
		return 	onBorderNode;


	// 親のリンク検索
	CSPRow parentLink = getParentLink( row );

	// 反対側のリンクを探す
	CSPRow oppositeLink;
	for( auto itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
		 itRoad != itRoadEnd; ++itRoad )
	{
		const CGeometry* roadGeom = (*itRoad)->GetShape();
		// 2次メッシュノードに接するもののうち
		if( onBorderNode->GetShape()->Disjoint( *roadGeom ) )
			continue;

		// 自分自身じゃないのがお目当て（反対側）の相手
		if( *itRoad != parentLink )
		{
			// 相手は必ず1つだけのはずなので、見つけたら抜ける
			oppositeLink = *itRoad;
			break;
		}
	}

	// 既に消えている場合もある
	if( !oppositeLink )
		return oppositeLink;

	// 同じ種類の内包ものを探す
	CSPRow retRow;
	auto tblType = row->GetTableType();
	for( auto itSeg = oppositeLink->begin(tblType), itSetEnd = oppositeLink->end(tblType);
		 itSeg != itSetEnd; ++itSeg )
	{
		// 2次メッシュに接していないものも含まれる（bug 10557）
		if( (*itSeg)->GetShape()->Disjoint( geom ) )
			continue;

		// リンク内属性は重なっている場合があるので種別まで見る必要がある（bug 11689）
		if( (*itSeg)->GetTableType() == sindyTableType::segment_attr )
		{
			if( (*itSeg)->GetLongValueByFieldName( schema::segment_attr::kLinkAttr, -1L ) == 
				row->GetLongValueByFieldName( schema::segment_attr::kLinkAttr, -1L ) )
				retRow = *itSeg;
		}
		else
			retRow = *itSeg;
	}
	return retRow;
}

errorcode::sindyErrCode CRoadNetwork::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CContainer listexclusions;

		listexclusions.push_back( pRow );

		emErr = _Delete( pRow, listexclusions, true, cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::_Delete( CSPRow pRow, CContainer& listexclusions, bool bDelEdgeNode, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if( pFeature )
		{
			CString strTableName( pRow->GetTableName() );
			if( AheIsNetworkLink( strTableName ) )
			{
				// 削除リンクにある関連オブジェクトの削除
				//rows_iterator itRoad = find( GetTableType(), pRow->GetOID() );

				//if ( itRoad != end(GetTableType()) )
				//{
					// 内包関係のもの
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						for( rows_iterator itSeg = pRow->begin(AheTableName2TableType(itContain->first)),
							itSegEnd = pRow->end(AheTableName2TableType(itContain->first));
							itSeg != itSegEnd; ++itSeg )
							(*itSeg)->Delete();
					}

					// 外部テーブル関係を削除
					DeleteRelation( pRow, GetTableName() );
				//}

				// リンク列の削除
				if ( sindyErr_NoErr != _DeleteLQ( pRow, cErrObjects ) )
					cErrObjects.push_back( (*pRow) );

				// 道路の削除
				pRow->Delete();

				// FROM、TOからノードID列作成
				CListLong listNodeIDs;

				listNodeIDs.push_back( pRow->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) );
				listNodeIDs.push_back( pRow->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) );

				std::map< long, std::list<CSPRow> > mapRoads;	// map<ノードID,list<道路>>

				for( rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
					itRoad != itRoadEnd; ++itRoad )
				{
					if( pRow != *itRoad )
					{
						mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1)].push_back( *itRoad );
						mapRoads[(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1)].push_back( *itRoad );
					}
				}

				sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(g_cRuleModelNetwork.GetPairName(GetTableName())); // ノードテーブルタイプ

				if ( bDelEdgeNode ) {
					for ( CListLong::const_iterator itIDs = listNodeIDs.begin(), itIDsEnd = listNodeIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
						rows_iterator itNode = end(emNetworkNodeTableType);

						if ( end(emNetworkNodeTableType) != (itNode = find( emNetworkNodeTableType, (*itIDs) )) ) {
							CRelNode* pNode = static_cast<CRelNode*>(itNode->get());

							// 2次メッシュ上ノードに接続している道路の削除
							if ( pNode->IsBorderNode() ) {
								if ( mapRoads.find( pNode->GetOID() ) != mapRoads.end() ) {
									for ( std::list<CSPRow>::const_iterator itRoad = mapRoads[pNode->GetOID()].begin(); itRoad != mapRoads[pNode->GetOID()].end(); ++itRoad ) {
										if ( ! listexclusions.find( *(*itRoad).get() ) ) {
											// 道路の削除
											listexclusions.push_back( (*itRoad) );
											if ( sindyErr_NoErr != _Delete( (*itRoad), listexclusions, true, cErrObjects ) )
												break;
										}
									}
								}
							}
						}
					}
				}

				// ノードの削除
				for ( rows_iterator itNode = begin(emNetworkNodeTableType), itNodeEnd = end(emNetworkNodeTableType);
					itNode != itNodeEnd; ++itNode )
				{
					long nConNum = mapRoads[(*itNode)->GetOID()].size(), nDelConNum = 0;

					// 接続している道路が１本もない時は削除
					for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
						itRoad != itRoadEnd; ++itRoad )
					{
						if( pRow != *itRoad ) {
							if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() || 
								(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() ) {
								schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

								if ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted )
									++nDelConNum;
							}
						}
					}

					if ( nDelConNum == nConNum ) {
						// 削除
						if ( sindyErr_NoErr != _Delete( *itNode, listexclusions, true, cErrObjects ) )
							break;
					} else {
						// 削除した道路に関係するノードの種別変更
						if ( pRow->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == (*itNode)->GetOID() || pRow->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == (*itNode)->GetOID() )
							ResetNodeAttribute( *itNode );
					}
				}
			}
			else if( AheIsNetworkNode( strTableName ) )
			{
				// 接続している道路削除
				for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
					itRoad != itRoadEnd; ++itRoad )
				{
					if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1) == pRow->GetOID() || 
						(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1) == pRow->GetOID() ) {
						schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

						if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							if ( sindyErr_NoErr != _Delete( *itRoad, listexclusions, true, cErrObjects ) )
								break;
						}
					}
				}

				// 外部テーブル関係を削除
				DeleteRelation( pRow, strTableName );

				pRow->Delete();
			}
			else {
				// 内包関係のものの場合
				if( AheIsRelateWithNetwork( strTableName ) )
				{
					deleteContainRelation( pRow );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 編集内容を保存する
errorcode::sindyErrCode CRoadNetwork::Store( CErrorObjects& cErrObjects )
{
	// ネットワークモデルと内包関係モデル
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRoad = it->second.begin(); itRoad != it->second.end(); ++itRoad )
			(*itRoad)->Store( cErrObjects );
	}
	// リンク列
	for( linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		for( linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue )
			itLinkQueue->Store( cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

// 整合性をチェックする
errorcode::sindyErrCode CRoadNetwork::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType(AheGetNetworkNodeFeatureClassName(GetTableName())); // ノードテーブルタイプ
	CListLong listNodeIDs;	// ノードチェック用

	// ネットワークリンク主体の複合チェック
	std::map<long,std::vector<CSPRow>> mapRoadsByNodeID; // ノードIDをキーとした道路リスト（2叉路角度チェックで使用）
	for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType());
		itRoad != itRoadEnd; ++itRoad )
	{
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		(*itRoad)->Changed( &emUpdateType );

		// 削除されていないもののみチェック
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			// ネットワークリンクの単体チェック
			boost::shared_ptr<CRelRoad> spRoad( boost::static_pointer_cast<CRelRoad>(*itRoad) );
			const CRelRoad& cRoad( *spRoad );
			cRoad.CheckLogic( cRule, cErrObjects );

			// FROM_NODE_IDとTO_NODE_IDを取得
			long nFromNodeID = cRoad.GetFromNodeID();
			long nToNodeID = cRoad.GetToNodeID();
			mapRoadsByNodeID[nFromNodeID].push_back( spRoad );
			mapRoadsByNodeID[nToNodeID].push_back( spRoad );

			// ネットワークリンクとネットワークノードの複合チェック
			// ・道路リンクの端点の座標と、FROM、TOと関連づいているノードの座標が一致しているか
			const_rows_iterator itFromNode = find( emNetworkNodeTableType, nFromNodeID );
			const_rows_iterator itToNode   = find( emNetworkNodeTableType, nToNodeID   );
			if( end(emNetworkNodeTableType) == itFromNode || end(emNetworkNodeTableType) == itToNode ||	// ノードの存在
				( ! ( (*itFromNode)->GetShape()->Equals( cRoad.GetFromPoint() ) &&					// FROM
					  (*itToNode)->GetShape()->Equals( cRoad.GetToPoint() ) ) ) )					// TO
			{
				cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadFromToPointIsEqualNodePoint, sindyErrLevel_ERR ) ) );
				//obj.SetErrorMessage( _T("%s"), strMsg ); <- 詳細はあとで書こう
			}

			// ノードチェック用に接続ノードを格納
			if ( end(emNetworkNodeTableType) != itFromNode )
				listNodeIDs.push_back( (*itFromNode)->GetOID() );
			if ( end(emNetworkNodeTableType) != itToNode )
				listNodeIDs.push_back( (*itFromNode)->GetOID() );

			// ネットワークリンクループチェック
			// [Bug 6242]道路リンクがループ形状になる場合でもノードを削除できてしまう
			// 同じFROM_NODE_ID/TO_NODE_IDを持つリンクは存在してはいけない
			const_rows_iterator itRoad2 = itRoad;
			++itRoad2;
			IPointCollectionPtr ipGeom1( (IGeometry*)*cRoad.GetShape() );
			for( ; itRoad2 != itRoadEnd; ++itRoad2 )
			{
				// FROM_NODE_IDとTO_NODE_IDを取得
				boost::shared_ptr<CRelRoad> spRoad2( boost::static_pointer_cast<CRelRoad>(*itRoad2) );
				const CRelRoad& cRoad2( *spRoad2 );
				long nFromNodeID2 = cRoad2.GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
				long nToNodeID2 = cRoad2.GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

				if ( (nFromNodeID == nFromNodeID2 && nToNodeID == nToNodeID2) ||
					 (nFromNodeID == nToNodeID2 && nToNodeID == nFromNodeID2) )
					cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_NetworkLinkLoop, sindyErrLevel_ERR, NULL, -1, -1, _T("%d - %d"), cRoad.GetOID(), cRoad2.GetOID() ) ) );
				// [CHECK] [ROADCHECK 02320] 構成点同士が接触しているかどうか（bug 8463）
				IPointPtr ipTouch;
				IPointCollectionPtr ipGeom2( (IGeometry*)*cRoad2.GetShape() );
				if( AheIsTouchVertex( ipGeom1, ipGeom2, ipTouch ) )
					cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_CheckReturnErr, sindyErrLevel_ERR, (IGeometryPtr)ipTouch, -1, -1, SHAPE_POINT_TOUCHING, cRoad.GetOID(), cRoad2.GetOID() ) ) );

				// [CHECK] [ROADCHECK 02322] 同一ノードに接続する線分が近接している（bug 8463）
				// 歩道リンクではチェックしない(歩行者Phase2対応)
				if( GetTableType()==sindyTableType::walk_link || GetTableType()==sindyTableType::lane_link ) continue;

				if( nFromNodeID == nFromNodeID2 || nFromNodeID == nToNodeID2 ||
					nToNodeID   == nFromNodeID2 || nToNodeID   == nToNodeID2 )
				{
					double dLen = min( AheGetConnectSegmentCrack( ipGeom1, ipGeom2 ), AheGetConnectSegmentCrack( ipGeom1, ipGeom2 ) );
					if( 20.0 >= dLen )
					{
						IGeometryPtr ipErrGeom;
						if( nFromNodeID == nFromNodeID2 || nFromNodeID == nToNodeID2 )
							ipErrGeom = (IPoint*)cRoad.GetFromPoint();
						else
							ipErrGeom = (IPoint*)cRoad.GetToPoint();
						cErrObjects.push_back( CErrorObject( cRoad, CErrorInfo( sindyErr_CheckReturnWarning, sindyErrLevel_WARN, ipErrGeom, -1, -1, SEGMENT_ARE_CLOSE, cRoad.GetOID(), cRoad2.GetOID(), dLen ) ) );
					}
				}
			}
		}
	}
	// [CHECK] 二叉路角度チェック（bug 8463 ROADCHECK 02195）
	typedef std::pair<const long,std::vector<CSPRow>> PAIR_ROADS_BY_NODEID;
	// 二叉路角度チェック
	// 鉄道リンクでは警告とし、鉄道リンク以外ではエラーとする
	// TODO ネットワークリンク別のチェックとして実装すべきでは？
	bool bRailWayChk = (GetTableType()==sindyTableType::railway_link);

	foreach( const PAIR_ROADS_BY_NODEID& _pair_roads_by_nodeid, mapRoadsByNodeID )
	{
		if( 2 != _pair_roads_by_nodeid.second.size() ) // 二叉路のみ
			continue;

		const_rows_iterator itNode = find( emNetworkNodeTableType, _pair_roads_by_nodeid.first );
		_ASSERT( itNode != end( emNetworkNodeTableType ) ); // 必ずある
		// 変更がある場合のみチェックする
		// 変更がないリンクの場合、すべての接続リンクがそろっていない可能性あり（bug 8463 #14）
		const CRow& road1 = *_pair_roads_by_nodeid.second[0];
		const CRow& road2 = *_pair_roads_by_nodeid.second[1];
		if( (*itNode)->Changed() || road1.Changed() || road2.Changed() )
		{
			double dAngle = AheGetAngle( (ICurvePtr)(IGeometry*)*road1.GetShape(), (ICurvePtr)(IGeometry*)*road2.GetShape() );
			if( -20.0 <= dAngle && dAngle <= 20.0 )
				cErrObjects.push_back( CErrorObject( road1, CErrorInfo( sindyErr_CheckReturnErr,
				(bRailWayChk? sindyErrLevel_WARN:sindyErrLevel_ERR),
				NULL, -1, -1,
				ANGLE_LESS_THAN_20, road1.GetOID(), road2.GetOID(), (int)dAngle ) ) );
		}
	}
	// 道路ノード主体の複合チェック
	foreach( const boost::shared_ptr<CRow>& spNode, std::make_pair( begin(emNetworkNodeTableType), end(emNetworkNodeTableType) ) )
	{
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		// ノード種別が適切かどうか
		// 道路リンク同士の接続角度のチェック

		// 接続している道路が編集された or 編集したもの＆削除されていないもののみチェック
		if ( listNodeIDs.find( spNode->GetOID() ) != listNodeIDs.end() || (spNode->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted) )
			spNode->CheckLogic( /*g_cRule.CreateLogicRule( itNode->second, */cRule/* )*/, cErrObjects );
	}

	// 編集したリンク列に関係する道路を取得(LQRFフラグチェック用)
	std::map<CString, std::list<long>> mapRoadLQRFs;

	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		// ***_LQRF取得
		CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName());

		for ( CRuleLinkQueue::const_value_iterator itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			if ( it->first == AheTableName2TableType(itInf->get<0>()) )
				break;
		}

		// リンク列分ループ
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( (*itLinkQueue).Changed( &emUpdateType ) ) {
				for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ ) {
					const_rows_iterator itRoad = find( GetTableType(), (*itLQ).GetLinkId() );	// LINK_IDの道路を取得

					// 道路が存在している and LQが削除された？
					if ( itRoad != end(GetTableType()) && ( (*itLQ).Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted ) ) {
						// 道路削除されていない？
						if ( ! (*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							if ( (*itRoad)->GetBoolValueByFieldName( itInf->get<2>(), false ) ) // 関連フラグが true かどうか
								mapRoadLQRFs[itInf->get<2>()].push_back( (*itRoad)->GetOID() );
						}
					}
				}
			}
		}
	}

	// リンク列の整合性と道路のLQRFの整合性チェック
	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		// ***_LQRF取得
		CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName());

		for ( CRuleLinkQueue::const_value_iterator itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			if ( it->first == AheTableName2TableType(itInf->get<0>()) )
				break;
		}

		std::list<long> listIDs = mapRoadLQRFs[itInf->get<2>()];

		listIDs.sort();
		listIDs.unique();

		// リンク列分ループ
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// 編集しているもの＆削除以外
			if ( (*itLinkQueue).Changed( &emUpdateType ) ) {
				if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
					itLinkQueue->CheckLogic( cRule, cErrObjects );	// リンク列の整合性チェック

					CGeometry cBasePoint;	// LINKDIR_Cチェック用

					for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ ) {
						(*itLQ).Changed( &emUpdateType );

						// 削除以外
						if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
							const_rows_iterator itRoad = find( GetTableType(), (*itLQ).GetLinkId() );	// LINK_IDの道路を取得

							// 道路が存在してない or 削除された？
							if ( itRoad == end(GetTableType()) || ( (*itRoad)->Changed( &emUpdateType ) && emUpdateType == schema::ipc_table::update_type::kDeleted ) )
								cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueNotRoad, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLinkID ) ) );
							else {
								// LQRFフラグがついているかどうか
								if( ! (*itRoad)->GetBoolValueByFieldName( itInf->get<2>(), false ) ) // 関連フラグが true かどうか
									cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadBadLQRF, sindyErrLevel_ERR, NULL, 0, -1, itInf->get<2>() ) ) );

								// LQRF群から削除
								listIDs.erase( std::remove( listIDs.begin(), listIDs.end(), (*itLQ).GetLinkId() ), listIDs.end() );

								// 方向チェック
								CGeometry cPoint;

								// Fromポイント取得
								if ( (*itLQ).GetLinkDir() == 1 )
									cPoint = (*itRoad)->GetShape()->GetPoint( 0 );
								else
									cPoint = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );

								if ( (IGeometry*)cBasePoint ) {
									// From/Toが一致しなかったらLINKDIR_Cがおかしい
									if ( ! cPoint.Equals( cBasePoint ) )
										cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueBadDirection, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLinkDir ) ) );
								}

								// Toポイント取得
								if ( (*itLQ).GetLinkDir() == 1 )
									cBasePoint = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );
								else
									cBasePoint = (*itRoad)->GetShape()->GetPoint( 0 );
							}
						}
					}
				}
			} else {
				// LQRF群から削除
				for ( CLinkQueue::const_iterator itLQ = (*itLinkQueue).begin(); itLQ != (*itLinkQueue).end(); ++itLQ )
					listIDs.erase( std::remove( listIDs.begin(), listIDs.end(), (*itLQ).GetLinkId() ), listIDs.end() );
			}
		}

		mapRoadLQRFs[itInf->get<2>()].swap( listIDs );
	}

	// LQRF群に道路のOIDが残ってたらおかしい
	for ( std::map<CString, std::list<long>>::const_iterator itLQRF = mapRoadLQRFs.begin(), itLQRFEnd = mapRoadLQRFs.end(); itLQRF != itLQRFEnd; ++itLQRF ) {
		for ( std::list<long>::const_iterator itID = itLQRF->second.begin(), itIDEnd = itLQRF->second.end(); itID != itIDEnd; ++itID ) {
			const_rows_iterator itRoad = find( GetTableType(), (*itID) );	// LINK_IDの道路を取得

			cErrObjects.push_back( CErrorObject( *itRoad->get(), CErrorInfo( sindyErr_RoadBadLQRF, sindyErrLevel_ERR, NULL, 0, -1, itLQRF->first ) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CRoadNetwork::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckModify( cErrObjects );
	}

	// リンク列
	for( const_linkqs_iterator it = linkqs_begin(); it != linkqs_end(); ++it )
	{
		for( const_linkq_iterator itLinkQueue = it->second.begin(); itLinkQueue != it->second.end(); ++itLinkQueue )
			itLinkQueue->CheckModify( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

// 登録されているフィーチャがあればそのフィーチャのポインタを返します
const CRow* CRoadNetwork::FindObject( _IRow* ipRow ) const
{
	if( ! ipRow )
		return NULL;

	CRow* pRet = NULL;	// 返り値

	sindyTableType::ECode emType = AheTableName2TableType(AheGetFeatureClassName( ipRow ));
	const_rows_iterator it = find( emType, AheGetOID(ipRow) );
	if( it != end(emType) )
		pRet = it->get();

	return pRet;
} 

errorcode::sindyErrCode CRoadNetwork::SelectByRoadFromLinkqs(const CListLong& listRoadIDs, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/)
{
	// 格納されている道路群
	sindyErrCode emErr  = sindyErr_NoErr;

	// [Bug 5873] 検索する道路が2次メッシュ上ノードに接続している時は、反対側の道路も検索対象にする。
	CListLong listRoadIDsTmp( listRoadIDs );

	_SelectByRoadFromLinkqs( listRoadIDs, listRoadIDsTmp );

	std::list<long> listSearchIDs;

	// リンク列分
	for( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs )
	{
		// INF分
		for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq )
		{
			(*itLinkq).sort();

			// 道路に関連するリンク列のみ
			bool bIsRoadRel = false;

			for ( std::list<long>::const_iterator itIds = listRoadIDsTmp.begin(), itIdsEnd = listRoadIDsTmp.end(); itIds != itIdsEnd; ++ itIds ) {
				if ( (*itLinkq).HasRoad( *find(GetTableType(), (*itIds) )->get() ) ) {
					bIsRoadRel = true;
					break;
				}
			}

			if ( bIsRoadRel ) {
				// 格納されていない道路を検索
				// LQ分
				for ( CLinkQueue::const_iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ )
				{
					// LQの道路が取得済みでなければ追加
					if ( find( GetTableType(), (*itLQ).GetLinkId() ) == end(GetTableType()) )
						listSearchIDs.push_back( (*itLQ).GetLinkId() );
				}
			}
		}
	}

	listSearchIDs.sort();
	listSearchIDs.unique();

	if ( ! listSearchIDs.empty() ) {
		// 道路検索
		CFeatureContainer::Select( GetTableName(), listSearchIDs, typeid(CRelRoad&), bRecycling, bCreateCache, cErrs, false ); // 初期化は最初にしてあるので最後はfalse

		// CheckLogicで整合性のチェックをするので、関連するものも追加検索
		// ノード検索(追加検索した道路に関連するノード)
		CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
		if( ! empty(GetTableType()) )
		{
			// FROM、TOからノードID列作成
			std::list<long> listNodeIDs;
			for( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad )
			{
				if ( std::find( listSearchIDs.begin(), listSearchIDs.end(), (*itRoad)->GetOID() ) != listSearchIDs.end() ) {
					listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ); // TODO: FROM、TOフィールド名はルールに定義していないが、良い？？
					listNodeIDs.push_back( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );
				}
			}

			listNodeIDs.sort();
			listNodeIDs.unique();

			// 検索
			CFeatureContainer::Select( strNetworkNodeTableName, listNodeIDs, typeid(CRelNode&), bRecycling, bCreateCache, cErrs, false ); // 初期化は最初にしてあるので最後はfalse
		}

		// 関連オブジェクトはネットワークリンクののIDで検索するのでID列を作っておく
		// TODO: ただし、ID関連があるもののみ。将来形状のみの関連のものができたときは、そのときに考えよう
		// 内包オブジェクトの検索（ただしID関連があるもののみ）
		if( ! listSearchIDs.empty() )
		{
			for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), 
				itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
			{
				if( ! it->second.IsEmpty() ) // ID関連があるもののみ
				{
					// 検索
					CFeatureContainer::Select( it->first, listSearchIDs, typeid(CFeature&), bRecycling, bCreateCache, cErrs, false, it->second, GetTableName() ); // 初期化は最初にしてあるので最後はfalse
				}
			}
		}
		// 外部テーブルの検索
		if( ! listSearchIDs.empty() )
		{
			for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(GetTableName()),
				itEnd = g_cRuleRelationTable.end(GetTableName()); it != itEnd; ++it )
			{
				// 検索
				CFeatureContainer::Select( it->get<1>(), listSearchIDs, typeid(CRow&), bRecycling, bCreateCache, cErrs, false, it->get<2>(), GetTableName() ); // 初期化は最初にしてあるので最後はfalse
			}
		}

		// リンク列を検索する
		for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf )
		{
			sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
			sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());

			// LQ取得
			std::list<long> listInfIDs;

			CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );
			LOGASSERTEERR_IF( cLQTable != NULL, sindyErr_TableNotFound )
			{
				// 収集した道路IDに関連するLQを取得
				// ここで検索できたLQは捨てる（INFのIDリスト作成のみに使用）
				std::list<CString> listIDsString;
				cLQTable.IDs2Str( listSearchIDs, listIDsString );
				for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
				{
					// WHERE句作成
					CString strWhere;
					strWhere.Format(_T("%s IN (%s)"), schema::link_queue::kLinkID, *itIDsString );

					// 検索
					_ICursorPtr ipCursor = cLQTable.Search( AheInitQueryFilter( NULL, schema::link_queue::kInfID, strWhere ), true );	// とりあえずINFのIDをとるので、LQは捨てる

					// 取得したLQからINFのIDを取得する
					long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
					LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
					{
						_IRowPtr ipRow;
						while( S_OK == ipCursor->NextRow( &ipRow ) )
						{
							if( ipRow == NULL ) break;

							CComVariant vaInfID(-1L);
							ipRow->get_Value( lInfIDIndex, &vaInfID );

							listInfIDs.push_back( vaInfID.lVal );
						}
						ipCursor = NULL;
					}
				}
				listInfIDs.sort();
				listInfIDs.unique();

				// 既にリストに存在する場合はその分を差し引く
				for( std::list<CLinkQueue>::const_iterator it = m_mapLinkQueue[emInfCode].begin(); it != m_mapLinkQueue[emInfCode].end(); ++it )
					listInfIDs.remove( it->GetOID() );

				// CLinkQueueクラスの構造上、必ず先にINFを入れる必要がある
				// 関連するINFを検索する
				if( ! listInfIDs.empty() )
				{
					CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
					LOGASSERTEERR_IF( cInfTable != NULL, sindyErr_TableNotFound )
					{
						// 共有オブジェクトの取得
						CSPTableNameString spInfNameString( cInfTable.GetNameString() );
						CSPFieldMap spInfFieldMap( cInfTable.GetFieldMap() );

						// 検索
						_ICursorPtr ipCursor = cInfTable.GetRows( listInfIDs, bRecycling );

						// 格納
						LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
						{
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								if( ipRow == NULL ) break;

								long lOID = -1;
								ipRow->get_OID( &lOID );

								linkq_iterator itLinkQueue = linkq_find( emInfCode, lOID );
								if ( itLinkQueue == linkq_end(emInfCode) )
									itLinkQueue = m_mapLinkQueue[emInfCode].insert( m_mapLinkQueue[emInfCode].end(), CLinkQueue(emInfCode,ipRow) );
								else
									(*itLinkQueue) = ipRow;

								// リサイクルする場合はキャッシュ作成
								if( bRecycling )
									itLinkQueue->CreateCache( true, spInfFieldMap, spInfNameString );
								else if( bCreateCache )
									itLinkQueue->CreateCache( false, spInfFieldMap, spInfNameString );
							}
						}
						ipCursor = NULL;
					}
				}

				// 取得したINFIDに関連するLQを取得する
				listIDsString.clear();
				cLQTable.IDs2Str( listInfIDs, listIDsString );
				for( std::list<CString>::const_iterator itIDsString = listIDsString.begin(); itIDsString != listIDsString.end(); ++itIDsString )
				{
					// WHERE句作成
					CString strWhere;
					strWhere.Format(_T("%s IN (%s)"), schema::link_queue::kInfID, *itIDsString );

					// 共有オブジェクトの取得
					CSPTableNameString spLQNameString( cLQTable.GetNameString() );
					CSPFieldMap spLQFieldMap( cLQTable.GetFieldMap() );

					// 検索
					_ICursorPtr ipCursor = cLQTable.Search( AheInitQueryFilter( NULL, NULL, strWhere ), bRecycling );

					// LQをINFIDで振り分け
					long lInfIDIndex = cLQTable.FindField( schema::link_queue::kInfID );
					LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
					{
						_IRowPtr ipRow;
						while( S_OK == ipCursor->NextRow( &ipRow ) )
						{
							if( ipRow == NULL ) break;

							CComVariant vaInfID( -1L );
							ipRow->get_Value( lInfIDIndex, &vaInfID );

							linkq_iterator itLinkQueue = linkq_find( emInfCode, vaInfID.lVal );
							itLinkQueue->push_back( emLqCode, ipRow, spLQFieldMap, spLQNameString );

							// リサイクルする場合はキャッシュ作成
							if( bRecycling )
								itLinkQueue->rbegin()->CreateCache( true, spLQFieldMap, spLQNameString );
							else if( bCreateCache )
								itLinkQueue->rbegin()->CreateCache( false, spLQFieldMap, spLQNameString );
						}
						ipCursor = NULL;
					}
				}
			}
		}
	}

	return emErr;
}

void CRoadNetwork::_SelectByRoadFromLinkqs(const CListLong& listRoadIDs, CListLong& listSearchRoads )
{
	// [Bug 5873]
	// 検索する道路が2次メッシュ上ノードに接続している時は、反対側の道路も検索対象にする。
	CString strNetworkNodeTableName( AheGetNetworkNodeFeatureClassName( GetTableName() ) );
	sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( strNetworkNodeTableName );
	CListLong listBorderNodeIDs;	// 2次メッシュ上ノード群

	// 検索する道路が2次メッシュ上ノードに接続しているなら、ノードIDを格納
	for ( CListLong::const_iterator itIDs = listRoadIDs.begin(), itIDsEnd = listRoadIDs.end(); itIDs != itIDsEnd; ++itIDs ) {
		CContainer::const_rows_iterator itRoad = find( GetTableType(), (*itIDs) );

		if ( itRoad != end( GetTableType() ) ) {
			CContainer::const_rows_iterator itFrom = find( emNetworkNodeTableType, (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) );
			CContainer::const_rows_iterator itTo = find( emNetworkNodeTableType, (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) );

			if ( itFrom != end(emNetworkNodeTableType) ) {
				CRelNode* pNode = static_cast<CRelNode*>(itFrom->get());

				if ( pNode->IsBorderNode() )
					listBorderNodeIDs.push_back( (*itFrom)->GetOID() );
			}

			if ( itTo != end(emNetworkNodeTableType) ) {
				CRelNode* pNode = static_cast<CRelNode*>(itTo->get());

				if ( pNode->IsBorderNode() )
					listBorderNodeIDs.push_back( (*itTo)->GetOID() );
			}
		}
	}

	listBorderNodeIDs.sort();
	listBorderNodeIDs.unique();

	// 2次メッシュ上ノードに接続している道路を検索リストに追加
	CListLong listRoadIDsTmp;

	for ( CContainer::const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); 
		itRoad != itRoadEnd; ++itRoad ) {

		long OID = (*itRoad)->GetOID();

		if ( listBorderNodeIDs.find( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) ) != listBorderNodeIDs.end() ) {
			if ( listSearchRoads.find( OID ) == listSearchRoads.end() )	// 格納済み？
				listRoadIDsTmp.push_back( (*itRoad)->GetOID() );
		}

		if ( listBorderNodeIDs.find( (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) ) != listBorderNodeIDs.end() ) {
			if ( listSearchRoads.find( OID ) == listSearchRoads.end() )	// 格納済み？
				listRoadIDsTmp.push_back( (*itRoad)->GetOID() );
		}
	}

	listRoadIDsTmp.sort();
	listRoadIDsTmp.unique();

	if ( ! listRoadIDsTmp.empty() ) {	// 追加検索道路群がある？
		listSearchRoads += listRoadIDsTmp;

		// 再帰検索
		_SelectByRoadFromLinkqs( listRoadIDsTmp, listSearchRoads );
	}
}

errorcode::sindyErrCode CRoadNetwork::swap( CSPRow pSource, CSPRow pDest )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		std::map<sindyTableType::ECode,bool> mapContains;

		// 内包関係の変更の前処理
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			bool bOrg = pSource->empty(AheTableName2TableType(itContain->first));
			bool bNew = pDest->empty(AheTableName2TableType(itContain->first));

			if ( ! bOrg && bNew )
				mapContains[AheTableName2TableType(itContain->first)] = true;
			else 
				mapContains[AheTableName2TableType(itContain->first)] = false;
		}

		// 外部テーブルの変更の前処理
		for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
			itOuterEnd = g_cRuleRelationTable.end(GetTableName());
			itOuter != itOuterEnd; ++itOuter )
		{
			bool bOrg = pSource->empty(AheTableName2TableType(itOuter->get<1>()));
			bool bNew = pDest->empty(AheTableName2TableType(itOuter->get<1>()));

			if ( ! bOrg && bNew )
				mapContains[AheTableName2TableType(itOuter->get<1>())] = true;
			else 
				mapContains[AheTableName2TableType(itOuter->get<1>())] = false;
		}

		// 入れ替え
		pSource->swap( *pDest.get(), true, true, false, true );

		// 内包関係の変更
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
		{
			// オリジナル側にあって新規側にない時、swapで入れ替えてしまっているので、入れ直し
			if ( mapContains[AheTableName2TableType(itContain->first)] ) {
				for( const_rows_iterator itSeg = pDest->begin(AheTableName2TableType(itContain->first)),
					itSetEnd = pDest->end(AheTableName2TableType(itContain->first));
					itSeg != itSetEnd; ++itSeg )
				{
					(*itSeg)->SetLongValueByFieldName(itContain->second, pSource->GetOID());
					pSource->push_back( *itSeg );
				}

				for( const_rows_iterator itSeg = pSource->begin(AheTableName2TableType(itContain->first)),
					itSetEnd = pSource->end(AheTableName2TableType(itContain->first));
					itSeg != itSetEnd; ++itSeg )
				{
					pDest->erase( *itSeg );
				}
			}
		}

		// 外部テーブルの変更
		for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()),
			itOuterEnd = g_cRuleRelationTable.end(GetTableName());
			itOuter != itOuterEnd; ++itOuter )
		{
			// オリジナル側にあって新規側にない時、swapで入れ替えてしまっているので、入れ直し
			if ( mapContains[AheTableName2TableType(itOuter->get<1>())] ) {
				for( const_rows_iterator itOneway = pDest->begin(AheTableName2TableType(itOuter->get<1>())),
					itOnewayEnd = pDest->end(AheTableName2TableType(itOuter->get<1>()));
					itOneway != itOnewayEnd; ++itOneway )
				{
					(*itOneway)->SetLongValueByFieldName(itOuter->get<2>(), pSource->GetOID());
					pSource->push_back( *itOneway );
				}

				for( const_rows_iterator itOneway = pSource->begin(AheTableName2TableType(itOuter->get<1>())),
					itOnewayEnd = pSource->end(AheTableName2TableType(itOuter->get<1>()));
					itOneway != itOnewayEnd; ++itOneway )
				{
					pDest->erase( *itOneway );
				}
			}
		}

		// リンク列のLQのIDを変更
		for ( linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs ) {
			// INF分
			for ( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
				if ( itLinkq->HasRoad( *pSource.get() ) || itLinkq->HasRoad( *pDest.get() ) ) {	// INFのLQに道路がある？
					link_queue::CLQ cOrgLQ, cNewLQ;
					bool bHasOrg = false, bHasNew = false;

					for ( CLinkQueue::iterator itLQ = itLinkq->begin(); itLQ != itLinkq->end(); ++itLQ ) {
						if ( pSource->GetOID() == (*itLQ).GetLinkId() ) {
							cOrgLQ = (*itLQ);
							bHasOrg = true;
						}

						if ( pDest->GetOID() == (*itLQ).GetLinkId() ) {
							cNewLQ = (*itLQ);
							bHasNew = true;
						}
					}

					if ( bHasOrg )
						cOrgLQ.SetLinkId( pDest->GetOID() );

					if ( bHasNew )
						cNewLQ.SetLinkId( pSource->GetOID() );
				}
			}
		}
	}

	return sindyErr_NoErr;
}

errorcode::sindyErrCode CRoadNetwork::_DeleteLQ( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		std::map<sindyTableType::ECode, std::list<CLinkQueue> > mapLinkqs;

		// 削除する道路に関連するリンク列を取得
		for( const_linkqs_iterator itLinkqs = linkqs_begin(); itLinkqs != linkqs_end(); ++itLinkqs )
		{
			// INF分
			for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq )
			{
				if ( itLinkq->HasRoad(*pRow) ) {	// INFのLQに道路がある？
					CLinkQueue cNewLinkq;

					cNewLinkq.SetObject( NULL, itLinkqs->first, true, (*itLinkq).GetFieldMap(), (*itLinkq).GetNameString() );

					// LQ削除
					itLinkq->DeleteLQ( *pRow, cNewLinkq, cErrObjects );

					if ( !cNewLinkq.empty_lq() )	// 分割されたLQがある？
						mapLinkqs[itLinkqs->first].insert( mapLinkqs[itLinkqs->first].end(), cNewLinkq );
				}
			}
		}

		for ( const_linkqs_iterator itLinkqs = mapLinkqs.begin(); itLinkqs != mapLinkqs.end(); ++itLinkqs ) {
			for ( const_linkq_iterator itLinkq = itLinkqs->second.begin(); itLinkq != itLinkqs->second.end(); ++itLinkq )
				m_mapLinkQueue[itLinkqs->first].insert( m_mapLinkQueue[itLinkqs->first].end(), (*itLinkq) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::DeleteLinkq( CLinkQueue& cDelLinkq, CErrorObjects& cErrObjects )
{
	if ( sindyErr_NoErr == cDelLinkq.Delete( cErrObjects ) )	// INF削除
		ResetRoadLQRF( cDelLinkq.GetTableType(), cErrObjects );	// 道路のLQRF再設定

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::ResetRoadLQRF(sindyTableType::ECode emLinkqType, CErrorObjects& cErrObjects)
{
	for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
		if ( emLinkqType == AheTableName2TableType(itInf->get<0>()) ) {	// 削除するリンク列と同じ種別？
			// 道路のLQRFを0に
			for( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad )
			{
				schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

				if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// 削除していない道路？
					if ( (*itRoad)->GetBoolValueByFieldName(itInf->get<2>(), false) ) {	// フラグが立っている？
						// 他にリンク列があるかチェック
						bool bHasLinkq = false;

						// INF分
						for ( const_linkq_iterator itLinkq = linkq_begin(emLinkqType); itLinkq != linkq_end(emLinkqType); ++itLinkq ) {
							schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

							if( !(*itLinkq).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// 削除していないリンク列？
								// LQ分
								for ( CLinkQueue::const_iterator itLQ = (*itLinkq).begin(); itLQ != (*itLinkq).end(); ++itLQ ) {
									schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

									if( !(*itLQ).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
										if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
											bHasLinkq = true;
											break;
										}
									}
								}

								if ( bHasLinkq )
									break;
							}
						}

						if ( ! bHasLinkq ) {	// 他にLQがない？
							// LQRFを０に
							if ( sindyErr_NoErr != (*itRoad)->SetBoolValueByFieldName(itInf->get<2>(), false) )
								cErrObjects.push_back( CErrorObject( *itRoad->get() ) );
						}
					}
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CRoadNetwork::DeleteLQ( CLinkQueue* pLinkq, const link_queue::CLQ& cLQ, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pLinkq, sindyErr_ArgIsNull )
	{
		long OID = cLQ.GetLinkId();

		for( rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( ! (*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {	// 削除していない道路？
				if ( (*itRoad)->GetOID() == OID ) {
					CLinkQueue cNewLinkq;
					std::map<sindyTableType::ECode, std::list<CLinkQueue> > mapLinkqs;

					cNewLinkq.SetObject( NULL, pLinkq->GetTableType(), true, pLinkq->GetFieldMap(), pLinkq->GetNameString() );

					// LQ削除
					pLinkq->DeleteLQ( *(*itRoad).get(), cNewLinkq, cErrObjects );

					if ( !cNewLinkq.empty_lq() )	// 分割されたLQがある？
						mapLinkqs[pLinkq->GetTableType()].insert( mapLinkqs[pLinkq->GetTableType()].end(), cNewLinkq );

					for ( const_linkqs_iterator itLinkqs = mapLinkqs.begin(); itLinkqs != mapLinkqs.end(); ++itLinkqs ) {
						for ( const_linkq_iterator itLinkq = itLinkqs->second.begin(); itLinkq != itLinkqs->second.end(); ++itLinkq )
							m_mapLinkQueue[itLinkqs->first].insert( m_mapLinkQueue[itLinkqs->first].end(), (*itLinkq) );
					}

					break;
				}
			}
		}

		ResetRoadLQRF( pLinkq->GetTableType(), cErrObjects );	// 道路のLQRF再設定
	}

	return cErrObjects.GetDefaultErrCode();
}

void CRoadNetwork::MaterializationLinkq()
{
	// 実体化
	for ( CRoadNetwork::const_linkqs_iterator itLinkqs = linkqs_begin(), itLinkqsEnd = linkqs_end(); itLinkqs != itLinkqsEnd; ++itLinkqs ) {
		for ( CRuleLinkQueue::const_value_iterator itInf = g_cRuleLinkQueue.begin(GetTableName()), itInfEnd = g_cRuleLinkQueue.end(GetTableName()); itInf != itInfEnd; ++itInf ) {
			sindyTableType::ECode emInfCode = AheTableName2TableType(itInf->get<0>());
			sindyTableType::ECode emLqCode = AheTableName2TableType(itInf->get<1>());

			if ( itLinkqs->first == emInfCode ) {
				CTable cInfTable( emInfCode, OpenSameOwnerTable( emInfCode ) );
				CTable cLQTable( emLqCode, OpenSameOwnerTable( emLqCode ) );

				for( linkq_iterator itLinkq = linkq_begin(itLinkqs->first); itLinkq != linkq_end(itLinkqs->first); ++itLinkq ) {
					if ( ! (_IRow*)(*itLinkq) && ! (*itLinkq).IsDeleted() )	// 実体がなくて削除されないもの
						itLinkq->Materialization( cInfTable, cLQTable );
				}

				break;
			}
		}
	}
}

bool CRoadNetwork::ResetNodeAttribute( CSPRow cRow, bool* bIsChanged/* = NULL*/ ) // TODO: CRowとCContainerを引数にとる外部関数にすること
{
	bool bHasNodeAttr = false, bDeleteNodeAttr = false;

	if ( bIsChanged )
		*bIsChanged = false;

	sindyTableType::ECode emNodeType( AheTableName2TableType( AheGetNetworkNodeFeatureClassName( GetTableName() ) ) );

	long sNodeclass = -1;
	switch ( AheGetNetWorkType( cRow->GetTableName() ) )
	{
	case sindyeNetworkRoad:
		sNodeclass = (long)GetModifyRoadNodeKind( cRow, &bHasNodeAttr );
		break;
	case sindyeNetworkWalk:
		sNodeclass = (long)GetModifyWalkNodeKind( cRow );
		break;
	case sindyeNetworkSubway:
		sNodeclass = (long)GetModifySubwayNodeKind( cRow );
		break;
	case sindyeNetworkRailway:
		sNodeclass = (long)GetModifyRailwayNodeKind( cRow );
		break;
	case sindyeNotNetwork:
	default:
		return false;
		break;
	}

	// ノードの種別が異なる場合のみ変更
	if ( bDeleteNodeAttr || sNodeclass != cRow->GetLongValueByFieldName( schema::road_node::kNodeClass, -1 ) )
	{
		cRow->SetLongValueByFieldName( schema::road_node::kNodeClass, sNodeclass );
		if ( bIsChanged )
			*bIsChanged = true;	// 変更された

		// 既存ノードの場合のみ、属性変更の履歴書き込み
		//if( bSetModifyData )
		//	AheSetModifyData( ipNodeFeature, sindyUpdateTypeProperty );
	}

	return true;
}

schema::road_node::node_class::ECode CRoadNetwork::GetModifyRoadNodeKind( const CSPRow& cRow, bool* pbHasNodeAttr ) const // TODO: CRowとContainerを引数に取る外部関数にすること
{
	// TODO : ほぼこぴぺなので、ちゃんと書き直すこと★

	// 初期化
	if ( pbHasNodeAttr )
		(*pbHasNodeAttr) = false;

	// ノード種別には、大まかに分けて3種類ある。
	// 
	// ・2次メッシュ区画線上にある場合の種別
	//   ・2次メッシュ区画線上PECノード
	//   ・2次メッシュ区画線上ノード
	// ・編集者が意図的に付与した種別
	//   ・料金所ノード
	//   ・ハイウェイ関係ノード
	//   ・ダミー施設ノード（TF用）
	//   ・同一リンクID防止ノード
	// ・接続道路リンクとノードの属性のみで決まる場合の種別
	//   ・端点ノード <- PECリンクが接続していた場合も端点ノードでいいんだっけ？
	//   ・PECリンク接続ノード
	//   ・交差点ノード
	//   ・属性変化点ノード
	//   ・属性付きノード
	//   ・意味なしノード
	//   ・ノード列基点ノード（使用してない？）
	//
	// ルール：
	// 1. 上記の3グループでは上の方が優先順位が高い
	// 2. 一番目のグループ内の種別は所属するグループより下のグループの種別に
	//    変更されることはない
	// 3. 二番目のグループ内の種別は編集者によって下のグループの種別に変更さ
	//    れない限り下のグループの種別に自動的に変更されることはない
	// 4. 一番目と三番目は自動付与できるが、二番目はユーザ付与であり、死守す
	//    る必要がある
	// 
	// 従って、自動付与の際に以前の属性を気にする必要があるのは二番目のグループ
	// に所属する属性のみであり、それ以外は現在の属性のみで決めてしまってよい。
	// 
	// アルゴリズム的には、
	// ・メッシュ区画線上かどうか確認
	//   ・区画線上ならreturn
	// ・前の種別が編集者が意図的に付与した種別かどうか確認
	//   ・意図的に付与した種別なら変更なしでreturn
	// ・それ以外は接続状況と属性で判断

	//////////////////////////////////////////
	// 前処理
	//////////////////////////////////////////
	// 接続している道路を取得
	std::list<road_link::CRoadLink*> listConnectRoads;

	for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad ) {
		if ( (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 ) == cRow->GetOID() ||
			(*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 ) == cRow->GetOID() ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
				listConnectRoads.push_back( dynamic_cast<road_link::CRoadLink*>(itRoad->get()) );
		}
	}

	// 接続リンク数チェック	
	// 道路リンクが2本以下で、かつノードに信号機フラグ及び交差点名称が入っている場合はフラグを立てる
	if ( listConnectRoads.size() <= 0 ) 
		return (schema::road_node::node_class::ECode)-1;
	else if ( listConnectRoads.size() < 3 ) {
		const road_node::CRoadNode* pRoadNode( dynamic_cast<const road_node::CRoadNode*>(cRow.get()) );

		if ( pRoadNode->IsExistAttrNode() ) {
			if ( pbHasNodeAttr )
				(*pbHasNodeAttr) = true;
		}
	}

	//////////////////////////////////////////
	// 2次メッシュ区画線上かどうか判定
	//////////////////////////////////////////
	CMesh mesh;

	mesh.SetSpatialReference( cRow->GetShape()->GetSpatialReference() );	// ノードの SpatialReference をセットしてやる

	std::list<UINT> listMeshCode;

	if ( mesh.IsTouches( cRow->GetShapeCopy(), listMeshCode ) ) {
		// メッシュ区切りのPGDBでは、ノードが2次メッシュノードでも片方にしか道路リンクがない
		// そのために必ずエラーになってしまうので、レジストリを見て道路の本数を無視するかどうか
		// を決める
		if ( AheIsPGDBEditMode() ) {
			if ( 1 == listConnectRoads.size() ) {
				if ( listConnectRoads.front()->IsPECLink() )
					return schema::road_node::node_class::kPecEdge;
				else
					return schema::road_node::node_class::kEdge;
			}
		} else {
			if ( 2 == listConnectRoads.size() ) {
				road_link::CRoadLink* pRoad = listConnectRoads.front();
				std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

				if ( pRoad->IsSameAttribute( *(*it2) ) ) {
					// 両方ともPECリンクなら2次メッシュ区画辺上PECノード、そうでなければ2次メッシュ区画辺上ノード
					if ( pRoad->IsPECLink() && (*it2)->IsPECLink() )
						return schema::road_node::node_class::kPecEdge;
					else
						return schema::road_node::node_class::kEdge;
				} else {
					// 2次メッシュ境界線に接しているのに接続する道路リンクの種別が異なる！！
					_ASSERTE( false );
					return (schema::road_node::node_class::ECode)-1;
				}
			} else {
				// 2次メッシュ境界線に接しているのに接続する道路が2本ではない！！
				_ASSERTE( false );
				return (schema::road_node::node_class::ECode)-1;
			}
		}
	}

	//////////////////////////////////////////
	// 既存の種別が意図的に付与された種別かどうかを判定
	//////////////////////////////////////////
	// 元のノード種別を取得
	schema::road_node::node_class::ECode sNodeClass = (schema::road_node::node_class::ECode)cRow->GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );

	switch ( sNodeClass ) {
		case schema::road_node::node_class::kDummyFacil:
		case schema::road_node::node_class::kTollbooth:
		case schema::road_node::node_class::kHighway:
		case schema::road_node::node_class::kLQBase: return sNodeClass; break;
		default: break;
	}

	//////////////////////////////////////////
	// 接続状況と属性で判定
	//////////////////////////////////////////
	sNodeClass = (schema::road_node::node_class::ECode)0;	// 意味なしノード

	if ( listConnectRoads.size() == 1 )	// 一本の場合は端点ノード
		sNodeClass = schema::road_node::node_class::kTerminal;
    else {	// 二本以上の場合
		bool bIsNormalRoad = false, bIsPECRoad = false;

		for ( std::list<road_link::CRoadLink*>::const_iterator it = listConnectRoads.begin(), itEnd = listConnectRoads.end(); it != itEnd; ++it ) {
			if ( (*it)->IsPECLink() )
				bIsPECRoad = true;
			else
				bIsNormalRoad = true;
		}

		// 全てPECリンクならPECノード
		// 一つでもPECリンクがあるならPECリンク接続ノード
		// 残された道路リンクが3本以上でPECリンクがないなら交差点ノード
		sNodeClass = ( ! bIsPECRoad ) ? schema::road_node::node_class::kCross : ( ( bIsNormalRoad ) ? schema::road_node::node_class::kParking : schema::road_node::node_class::kPec );

		// 三本以上の場合はここで終了
		// 二本の場合で、且つ上記判定でsindyeNodeIntersectionの場合はさらに調べる
		if ( listConnectRoads.size() == 2 && schema::road_node::node_class::kCross == sNodeClass ) {
			road_link::CRoadLink* pRoad = listConnectRoads.front();
			std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

			// 接続する道路リンクが異なる属性を持つのであれば属性変化点
			if ( !pRoad->IsSameAttribute( *(*it2) ) )
				sNodeClass = schema::road_node::node_class::kAlter;
			// ノードの交差点・高さ属性をみて、あれば属性付きノード
			else if ( pbHasNodeAttr && *pbHasNodeAttr )
				sNodeClass = schema::road_node::node_class::kAttr;
			// 接続する道路で、ターゲット以外のノードIDをFROM・TOに持つものが
			// あれば同一リンクID防止ノードにする
			// なければ意味無しノード
			else {
				// ターゲット以外のノードIDを収集する
				long lNodeOID = cRow->GetOID(), lOtherNodeID1 = -1, lOtherNodeID2 = -1; // ターゲット以外のノードID

				for ( std::list<road_link::CRoadLink*>::const_iterator it = listConnectRoads.begin(), itEnd = listConnectRoads.end(); it != itEnd; ++it ) {
					long lFromID = (*it)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
					long lToID = (*it)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

					if ( -1 == lOtherNodeID1 )
						lOtherNodeID1 = ( lNodeOID == lFromID ) ? lToID : lFromID;
					else
						lOtherNodeID2 = ( lNodeOID == lFromID ) ? lToID : lFromID;
				}

				// ターゲット以外のノードIDを持つものが渡されているかどうかチェック
				bool bFind = false; // 見つかったかどうかのフラグ

				for ( const_rows_iterator itRoad = begin(GetTableType()); itRoad != end(GetTableType()); ++itRoad ) {
					long lFromID = (*itRoad)->GetLongValueByFieldName(schema::road_link::kFromNodeID, -1 );
					long lToID = (*itRoad)->GetLongValueByFieldName(schema::road_link::kToNodeID, -1 );

					schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

					if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
						if( ( lOtherNodeID1 == lFromID && lOtherNodeID2 == lToID ) ||
							( lOtherNodeID2 == lFromID && lOtherNodeID1 == lToID ) )
						{
							bFind = true;
							break;
						}
					}
				}

				sNodeClass = ( bFind ) ? schema::road_node::node_class::kEvasion : (schema::road_node::node_class::ECode)0;
			}
		}
	}

	return sNodeClass;
}

bool CRoadNetwork::OnMeshEdgeNode( const CSPRow& cRow, std::list<road_link::CRoadLink*>& listConnectRoads, bool& hasErr ) const
{
	hasErr = false;
	if ( ! cRow )
	{
		hasErr = true;
		return false;
	}

	// 実際に接続されている道路を特定する
	for ( const_rows_iterator itRoad = begin(GetTableType()), itRoadEnd = end(GetTableType()); itRoad != itRoadEnd; ++itRoad ) {
		if ( (*itRoad)->GetLongValueByFieldName(schema::walk_link::kFromNodeID, -1 ) == cRow->GetOID() ||
			(*itRoad)->GetLongValueByFieldName(schema::walk_link::kToNodeID, -1 ) == cRow->GetOID() ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( !(*itRoad)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
				listConnectRoads.push_back( dynamic_cast<road_link::CRoadLink*>(itRoad->get()) );
		}
	}

	// 接続リンク数チェック	
	if ( listConnectRoads.empty() ) 
	{
		hasErr = true;
		return false;
	}

	//////////////////////////////////////////
	// 2次メッシュ区画線上かどうか判定
	//////////////////////////////////////////
	CMesh mesh;

	mesh.SetSpatialReference( cRow->GetShape()->GetSpatialReference() );	// ノードの SpatialReference をセットしてやる

	std::list<UINT> listMeshCode;

	if ( mesh.IsTouches( cRow->GetShapeCopy(), listMeshCode ) ) {
		// メッシュ区切りのPGDBでは、ノードが2次メッシュノードでも片方にしか道路リンクがない
		// そのために必ずエラーになってしまうので、レジストリを見て道路の本数を無視するかどうか
		// を決める
		if ( AheIsPGDBEditMode() ) {
			if ( 1 == listConnectRoads.size() )
			{
				return true;
			}
		} else {
			if ( 2 == listConnectRoads.size() ) {
				road_link::CRoadLink* pRoad = listConnectRoads.front();
				std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

				if ( pRoad->IsSameAttribute( *(*it2) ) ) {
					return true;
				} else {
					// 2次メッシュ境界線に接しているのに接続する道路リンクの種別が異なる！！
					_ASSERTE( false );
					hasErr = true;
				}
			} else {
				// 2次メッシュ境界線に接しているのに接続する道路が2本ではない！！
				_ASSERTE( false );
				hasErr = true;
			}
		}
	}
	return false;
}

schema::walk_node::node_class::ECode CRoadNetwork::GetModifyWalkNodeKind( const CSPRow& cRow ) const
{
	// TODO : ほぼこぴぺなので、ちゃんと書き直すこと★

	if ( ! cRow )
		return (schema::walk_node::node_class::ECode)-1;
	
	
	//////////////////////////////////////////
	// 2次メッシュ区画線上かどうか判定
	//////////////////////////////////////////
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads; // 実際に接続されているリンク
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) )
	{
		return hasErr ? (schema::walk_node::node_class::ECode)-1 : schema::walk_node::node_class::kOutline;
	}

	//////////////////////////////////////////
	// 既存の種別が意図的に付与された種別かどうかを判定
	//////////////////////////////////////////
	// 元のノード種別を取得
	schema::walk_node::node_class::ECode sNodeClass = (schema::walk_node::node_class::ECode)cRow->GetLongValueByFieldName( schema::walk_node::kNodeClass, -1 );

	switch ( sNodeClass ) {
		case schema::walk_node::node_class::kRoadNW:
		case schema::walk_node::node_class::kGate:
		case schema::walk_node::node_class::kUndergroundGate:
		case schema::walk_node::node_class::kElevator:
			return sNodeClass;
			break;
		default: break;
	}

	//////////////////////////////////////////
	// 接続状況と属性で判定
	//////////////////////////////////////////
	sNodeClass = schema::walk_node::node_class::kPublic;
	// 一本の場合は端点ノード
	if( listConnectRoads.size() == 1 )
		sNodeClass = schema::walk_node::node_class::kTerminal;
	// 二本以上の場合
    else
	{
		// 三本以上の場合はここで終了
		// 二本の場合はさらに調べる
		if( listConnectRoads.size() == 2 )
		{
			road_link::CRoadLink* pRoad = listConnectRoads.front();
			std::list<road_link::CRoadLink*>::iterator it2 = listConnectRoads.begin(); ++it2;

			// 接続する道路リンクが異なる属性を持つのであれば属性変化点
			if ( !pRoad->IsSameAttribute( *(*it2) ) )
				sNodeClass = schema::walk_node::node_class::kAlter;
		}
	}

	return sNodeClass;
}

schema::sj::subway_node::node_class::ECode CRoadNetwork::GetModifySubwayNodeKind( const CSPRow& cRow ) const
{
	using namespace schema::sj::subway_node::node_class;
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads;
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) && !hasErr)
	{
		return kOutline;
	}
	return hasErr ? (ECode)-1 : kStation;
}

sindy::schema::sj::railway_node::node_class::ECode CRoadNetwork::GetModifyRailwayNodeKind( const CSPRow& cRow ) const
{
	using namespace schema::sj::railway_node::node_class;
	bool hasErr = false;
	std::list<road_link::CRoadLink*> listConnectRoads;
	if( OnMeshEdgeNode( cRow, listConnectRoads, hasErr ) && !hasErr )
		return kOutline;

	// エラーがあったならエラー用コードを返す
	if(hasErr) return (ECode)-1;

	// 駅POIノードが設定されているのであれば属性は変更しない
	// 元のノード種別を取得
	ECode sNodeClass = (ECode)cRow->GetLongValueByFieldName( schema::sj::railway_node::kNodeClass_C, -1 );
	if( sNodeClass == kStation )
		return kStation;

	// 一本の場合は端点ノード
	if( listConnectRoads.size() == 1 )
		return kTerminal;

	// 三本以上の場合は分岐ノード
	if( listConnectRoads.size() >= 3 )
		return kBranch;

	// 二本の場合は接続されているリンクの属性によって種別を変える
	road_link::CRoadLink* pRoad = listConnectRoads.front();
	std::list<road_link::CRoadLink*>::iterator itRoad2 = listConnectRoads.begin();
	std::advance(itRoad2, 1);

	// 異なる属性を持つのであれば属性変化点
	//（情報ソースや自動入力されるような属性は比較対象から除く）
	if( !pRoad->IsSameAttribute( *(*itRoad2) ) )
		return kAlter;

	// 同じ属性を持つのであれば機能なしノード
	return kNone;
}

}// sindy
