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

#include <boost/assign/list_of.hpp>
#include "RoadLinkTable.h"
#include "RowContainer.h"
#include "GeometryBase.h"
#include "ClassFactory.h"

namespace sindy {

using namespace schema;

CSPTableContainer _TableClassFactory2( const CTableFinder& cFinder, ITablePtr& ipTable )
{
	CSPTableContainer cTable; // 戻り値

	CString strTableName( AheGetFeatureClassName( ipTable ) );
	strTableName.MakeUpper(); // 大文字に

	if( strTableName == road_link::kTableName )
		cTable.reset( new CRoadLinkTable( ipTable, cFinder ) );
	else
		cTable.reset( new CTableContainer( ipTable, cFinder ) );

	return cTable;
}

// ライブラリを初期化する
void init_sindynlib()
{
	TableClassFactory = _TableClassFactory2;
}

// ライブラリの後始末をする
void uninit_sindynlib()
{
}

// 道路ネットワーク用のモデルを作成する
void GetSindynModel( CModel& model )
{
	model.Add( road_link::kTableName, kObjectID, NULL, NULL );
	BOOST_FOREACH( LPCTSTR lpcszLQ, linkqueuetype2lqtablename( linkqueue_type::kAll ) )
	{
		model.Add( lpcszLQ,					link_queue::kLinkID,	road_link::kTableName,	kObjectID				);
		model.Add( lq2inf(lpcszLQ),			kObjectID,				lpcszLQ,				link_queue::kInfID		);
	}
}

/** 
 * @biref linkqueue_type から LQテーブル名のリストに変換する
 */
std::list<LPCTSTR> linkqueuetype2lqtablename( linkqueue_type::ECode emCode )
{
	std::list<LPCTSTR> ret; // 戻り値
	for( int i = linkqueue_type::kLRTurn; i <= linkqueue_type::kAll; i = i*2 )
	{
		LPCTSTR lpcszName = NULL;
		switch( (linkqueue_type::ECode)(i & emCode) )
		{
#define LINK_QUEUE(inf,lq,type) case type: lpcszName = lq; break;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
		default: break;
		}
		if( lpcszName )
			ret.push_back( lpcszName );
	}
	return ret;
}

/**
 * @brief LQテーブル名からINFテーブル名に変換する
 */
LPCTSTR lq2inf( LPCTSTR lpcszLQ )
{
#define LINK_QUEUE(inf,lq,type) if( 0 == lstrcmpi( lpcszLQ, lq ) ) return inf;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
	assert( false );
	return NULL;
}

/**
 * @brief INFテーブル名からLQテーブル名に変換する
 */
LPCTSTR inf2lq( LPCTSTR lpcszInf )
{
#define LINK_QUEUE(inf,lq,type) if( 0 == lstrcmpi( lpcszInf, inf ) ) return lq;
#include "define_linkqueue.h"
#define LINK_QUEUE(inf,lq,type)
	assert( false );
	return NULL;
}

// sort用プレディケート（LQのシーケンス順にソート）
bool CRoadLinkTable::LessLQSequence::operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const
{
	if( ! ( c1 && c2 ) )
		return false;

	return CAST_ROW(c1)->GetValue( link_queue::kSequence, -1L ) 
		< CAST_ROW(c2)->GetValue( link_queue::kSequence, -1L );
}

// 道路リンクのOBJECTIDで関連オブジェクトを検索する
void CRoadLinkTable::Select( const std::list<long>& listOID, bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/, bool/* as_start = false*/, long/* start_node_id = 0*/ )
{
	if( ! IsConnected() )
		return;

	// 道路リンクの検索
	// モデル上子供としているものもここで一緒に検索される
	__super::Select( listOID, kObjectID, detach, unique );

	// 足りていないLQの検索
	SelectLQs( detach, unique, emType );
}

void CRoadLinkTable::SelectLQs( bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ )
{
	BOOST_FOREACH( LPCTSTR lpcszLQTable, linkqueuetype2lqtablename( emType ) )
	{
		std::list<long> listInfID; // 取得済みINF OID
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INFテーブル名

		// 格納用INFテーブルクラス取得
		// テーブルファインダに入っているコンテナに格納する
		// 本当はINF検索時に入れてしまってもいいのだが、とりあえず
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // トップレベルにあるINF用コンテナ
		// 検索済みINFを収集及びトップレベルのコンテナに格納
		BOOST_FOREACH( CSPContainerBase& con_link, GetContainer() )
		{
			CRowContainer& link( *CAST_ROW(con_link) );
			BOOST_FOREACH( CSPContainerBase& con_lq, *link.GetItemTable( lpcszLQTable ) )
			{
				CRowContainer& lq( *CAST_ROW(con_lq) );
				BOOST_FOREACH( CSPContainerBase& con_inf, *lq.GetItemTable( lpcszInfTable ) )
				{
					cInfTable.push_back( con_inf, true ); // 重複しないように格納
					listInfID.push_back( CAST_ROW(con_inf)->GetOID() ); // OID取得
				}
			}
		}
		listInfID.sort();
		listInfID.unique();

		// INFに関連するLQを検索
		CSPTableContainer spLqTable( FindTable( lpcszLQTable )->CreateEmptyClone() );
		CTableContainer& cLqTable( *spLqTable );
		cLqTable._Select( listInfID, link_queue::kInfID, detach, unique );
		// 検索したLQをそれぞれのINFに振り分け
		BOOST_FOREACH( CSPContainerBase& con, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
		{
			CRowContainer& lq( *CAST_ROW(con) );
			CContainerBase::iterator itInf = cInfTable.find_by_key( kObjectID, lq.GetValue( link_queue::kInfID ) );
			if( cInfTable.end() == itInf )
			{
				// ここに来るということは、LQに対応するINFがDB上に存在しないか
				// LQの子供としてINFが登録されていないか
				// -> TODO: モデル上LQの子供としてINFが登録されていない場合にはここの処理を飛ばす等したほうがよい
				assert( cInfTable.end() != itInf );
				continue; // しょうがないので飛ばす
			}
			CRowContainer& inf( *CAST_ROW(*itInf) );
			CSPContainerBase con_inf_lq( inf.GetItemTable( lpcszLQTable ) ); // トップレベルのINFが抱えているLQのコンテナ
			// モデル上INFの子供としてLQが登録されていないとGetItemTableできない
			// 本来なら一時的にモデルを変更して検索すれば済む話なのだが、面倒なので
			// 無い場合はここでコンテナを追加する
			if( ! con_inf_lq )
			{
				inf.push_back( CSPTableContainer( FastTableClassFactory( GetFinder(), lpcszLQTable ) ) );
				con_inf_lq = *inf.rbegin();
			}
			CAST_ROW(*itInf)->GetItemTable( lpcszLQTable )->push_back( con, unique );
		}
		// 追加したLQをソート
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
			CAST_ROW(con_inf)->GetItemTable( lpcszLQTable )->sort( LessLQSequence() );
	}
}

// LQ、INFの形状を補完する
void CRoadLinkTable::SelectLQShape( linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ )
{
	// 道路リンクのIDを収集する
	std::list<LPCTSTR> listLQName( linkqueuetype2lqtablename(emType) ); // LQテーブル名
	std::list<long> listLinkID; // 道路のOID（検索用）
	BOOST_FOREACH( LPCTSTR lpcszLQTable, listLQName )
	{
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INFテーブル名
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // トップレベルにあるINF用コンテナ
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
		{
			CRowContainer& inf( *CAST_ROW(con_inf) );
			CTableContainer& cLqTable( *inf.GetItemTable( lpcszLQTable ) );
			BOOST_FOREACH( CSPContainerBase& con_lq, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
				listLinkID.push_back( CAST_ROW(con_lq)->GetValue( link_queue::kLinkID, -1L ) );
		}
	}
	listLinkID.sort();
	listLinkID.unique();

	// 道路を検索する
	CSPTableContainer linkT( CreateEmptyClone() );
	linkT->_Select( listLinkID, kObjectID, _T("OBJECTID,SHAPE,SHAPE.LEN"), false, false ); // 形状だけ
	// 形状取得
	std::map<long,IGeometryPtr> mapLinkShape; // key: 道路OID value: 道路形状
	BOOST_FOREACH( CSPContainerBase& con_link, std::make_pair( linkT->begin(), linkT->end() ) )
	{
		CRowContainer& link( *CAST_ROW(con_link) );
		mapLinkShape[link.GetOID()] = link.CRowBase::GetShapeCopy();
	}

	// 道路形状をLQに割り当て
	BOOST_FOREACH( LPCTSTR lpcszLQTable, listLQName )
	{
		LPCTSTR lpcszInfTable = lq2inf(lpcszLQTable); // INFテーブル名
		CTableContainer& cInfTable( *FindTable( lpcszInfTable ) ); // トップレベルにあるINF用コンテナ
		BOOST_FOREACH( CSPContainerBase& con_inf, std::make_pair( cInfTable.begin(), cInfTable.end() ) )
		{
			IGeometryPtr inf_shape; // リンク列全体の形状
			CRowContainer& inf( *CAST_ROW(con_inf) );
			CTableContainer& cLqTable( *inf.GetItemTable( lpcszLQTable ) );
			BOOST_FOREACH( CSPContainerBase& con_lq, std::make_pair( cLqTable.begin(), cLqTable.end() ) )
			{
				CRowContainer& lq( *CAST_ROW(con_lq) );
				// LQに形状セット
				lq.SetShape( mapLinkShape[lq.GetValue( link_queue::kLinkID, -1L )] );
				// INF用の形状作成
				if( ! inf_shape )
				{
					inf_shape = lq.CRowBase::GetShapeCopy();
					// AheUnionNetworkがbase側の方向であわせるので、
					// 方向を順方向にしておく
					if( lq.GetValue( link_queue::kLinkDir, -1L ) == link_queue::link_dir::kReverse )
						IPolylinePtr(inf_shape)->ReverseOrientation();
				}
				else {
					CGeometryBase& geom( *lq.GetShape() );
					inf_shape = AheUnionNetwork( inf_shape, (IGeometry*)geom, geom.GetSpatialReference()  );
				}
			}
			// INFに形状セット
			inf.SetShape( inf_shape );
		}
	}
}

} // sindy
