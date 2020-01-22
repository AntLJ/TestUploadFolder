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
#include "CheckBusPoint.h"
#include "BusPointCommon/RegexChk.h"
#include "BusPointCommon/CommonFunc.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <sindy/workspace.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

// バス停ポイントとルート探索用ポイントの距離閾値（以上だとERROR）
const long k_stop_to_route_dist = 20; //!< 20m

// バス停標柱ポイント同士の距離閾値（以下だとWARNING）
const long k_pole_to_pole_dist = 5;   //!< 5m

// ルート探索用ポイントと道路リンクの距離閾値（以上だとERROR）
const double k_route_to_nw_dist = 0.5;  //!< 50cm

// 各種処理を実行するために最初に呼ばれる関数
bool CCheckBusPoint::execute()
{
	// 各種初期化を実施
	if( !Init() ){
		return false;
	}

	// チェック先・参照先DB情報出力
	m_ofsChk << "#DB_BUS：" << CT2CA(m_strBusDB) << endl;
	m_ofsChk << "#DB_ROAD：" << CT2CA(m_strRoadDB) << endl;

	// 自由形式ヘッダ出力
	m_ofsChk << "#FREESTYLELOG" << endl;
	m_ofsChk << "LAYER\tOBJECTID\tERR_LEVEL\tERR_MESSAGE\tCOMPANY\tROUTE_NAME\tROUTE_DETAIL\tSTOP_SEQ\tFLAG\tCOMMENT" << endl;

	// バス停ポイントチェック
	CheckStopPoint();

	// バスルート探索用ポイントチェック
	CheckRoutePoint();

	// バス路線マスタチェック
	CheckLineMaster();
	
	// バス停車順管理チェック
	CheckStopSeq();

	return true;
}

// 各種初期化
bool CCheckBusPoint::Init()
{
	// DBチェックログファイルオープン
	m_ofsChk.open( m_strChkLog );
	if( !m_ofsChk ){
		cerr << "#ERROR DBチェックログファイルオープンに失敗：" << CT2CA(m_strChkLog) << endl;
		return false;
	}

	// バス関連ワークスペースオープン
	m_ipBusWorkspace = sindy::create_workspace( m_strBusDB );

	if( ! m_ipBusWorkspace ){
		cerr << "#ERROR バスサーバへの接続に失敗 : " << CT2CA(m_strBusDB) << endl;
		return false;
	}

	// バス停ポイントフィーチャクラスオープン
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenFeatureClass( _bstr_t(bus_stop_point::kTableName), &m_ipBusStopPointFC );
	if( ! m_ipBusStopPointFC ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA(bus_stop_point::kTableName) << endl;
		return false;
	}
	// バス停ポイントフィールドマップ作成
	m_busStopPointFM.CreateFieldMap( (ITablePtr)m_ipBusStopPointFC );

	// バス路線マスタテーブルオープン
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenTable( _bstr_t(bus_line_master::kTableName), &m_ipBusLineMasterTable );
	if( ! m_ipBusLineMasterTable ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA(bus_line_master::kTableName) << endl;
		return false;
	}
	// バス路線マスタテーブルフィールドマップ作成
	m_busLineMasterFM.CreateFieldMap( m_ipBusLineMasterTable );

	// バス停車順管理テーブルオープン
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenTable( _bstr_t(bus_stop_seq::kTableName), &m_ipBusStopSeqTable );
	if( ! m_ipBusStopSeqTable ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA(bus_stop_seq::kTableName) << endl;
		return false;
	}
	// バス停車順管理テーブルフィールドマップ作成
	m_busStopSeqFM.CreateFieldMap( m_ipBusStopSeqTable );

	// バスルート探索用ポイントフィーチャクラスオープン
	((IFeatureWorkspacePtr)m_ipBusWorkspace)->OpenFeatureClass( _bstr_t(bus_route_point::kTableName), &m_ipBusRoutePointFC );
	if( ! m_ipBusRoutePointFC ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA(bus_route_point::kTableName) << endl;
		return false;
	}
	// バスルート探索用ポイントフィールドマップ作成
	m_busRoutePointFM.CreateFieldMap( (ITablePtr)m_ipBusRoutePointFC );


	// 道路NWワークスペースオープン
	m_ipRoadWorkspace = sindy::create_workspace( m_strRoadDB );

	if( ! m_ipRoadWorkspace ){
		cerr << "#ERROR バスサーバへの接続に失敗 : " << CT2CA(m_strRoadDB) << endl;
		return false;
	}

	// 道路NW(ROAD_LINK)フィーチャクラスオープン
	((IFeatureWorkspacePtr)m_ipRoadWorkspace)->OpenFeatureClass( _bstr_t(road_link::kTableName), &m_ipRoadLinkFC );
	if( ! m_ipRoadLinkFC ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA(road_link::kTableName) << endl;
		return false;
	}
	return true;
}

// バス停ポイントチェック
void CCheckBusPoint::CheckStopPoint()
{
	// バス停ポイントから全件取得
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( NULL, VARIANT_FALSE, &ipCursor );

	// バス停ポイント同士の属性比較用
	BUSSTOPP busStopPoints;

	IFeaturePtr ipStopFeature;
	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextFeature(&ipStopFeature) ){

		// フィーチャのNULLチェック
		if( !ipStopFeature ){
			m_ofsChk << "#ERROR バス停ポイントチェック時にフィーチャ取得失敗" << endl;
			continue;
		}

		// 属性を取得しておく
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busStopPointFM.GetFieldMap() )
		{
			ipStopFeature->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipStopFeature->get_OID( &oID );

		// ポイント種別ごとのチェック
		bus_stop_point::point_attr_code::ECode pAttrCode = (bus_stop_point::point_attr_code::ECode)(mapAttr[bus_stop_point::kPointAttrCode].lVal);

		switch( pAttrCode )
		{

		// 標柱ポイントのみ
		case bus_stop_point::point_attr_code::kPole: 
			{
			
			// バス停ポイント自体（標柱ポイント）
			if( consider_as_null( mapAttr[bus_stop_point::kPoleNo] )){
				// 【停留所標柱ポイントの標柱番号が空白である】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t停留所標柱ポイントの標柱番号が空白である\t\t\t\t\t0\t" << endl;
			}

			// 対応するルート探索用ポイント関連
			long routePointCnt = 0;
			IFeaturePtr ipRouteFeature = GetRoutePoint( oID, routePointCnt );
			if( 0 == routePointCnt ){
				// 【停留所標柱ポイントに対応する停留所ルート探索用ポイントがない】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t停留所標柱ポイントに対応する停留所ルート探索用ポイントがない\t\t\t\t\t0\t" << endl;
			}
			else if( 1 < routePointCnt ){
				// 【停留所標柱ポイントに対応する停留所ルート探索用ポイント2つ以上ある】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t停留所標柱ポイントに対応する停留所ルート探索用ポイント2つ以上ある\t\t\t\t\t0\t" << endl;
			}
			else{
				// 停留所ポイントとルート探索用ポイントの距離を測る
				double dist = GetDistFeature( ipStopFeature, ipRouteFeature );
				if( k_stop_to_route_dist < dist ){
					// 【停留所標柱ポイントに対応するルート探索用ポイントが離れすぎている(閾値：20m)(WARNING)】
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t停留所標柱ポイントに対応するルート探索用ポイントが離れすぎている(閾値：" << k_stop_to_route_dist << "m)\t\t\t\t\t0\t" << endl;
				}
			}

			long stopPointCount = 0;
			IFeatureCursorPtr ipNearCursor = GetNearStopPoints( ipStopFeature, stopPointCount );
			if( 0 < stopPointCount ){
				// バス停ポイントが5m以内に存在する場合は、標柱ポイントの数を数える
				if( 0 < CountPolePoint( ipNearCursor ) ){
					// 【別の停留所標柱ポイントが5m以内にある(WARNING)】
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t別の停留所標柱ポイントが" << k_pole_to_pole_dist << "m以内にある\t\t\t\t\t0\t" << endl;
				}
			}
		
			// 対応する停車順を取得
			long seqCnt = 0;
			set<CString> companys;
			_IRowPtr ipSeqRow;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			while( ipSeqCursor && S_OK == ipSeqCursor->NextRow( &ipSeqRow )){

				// フィーチャのNULLチェック
				if( !ipSeqRow ){
					continue;
				}

				CComVariant vaSeqValue;
				ipSeqRow->get_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusLineMasterID ), &vaSeqValue );

				if( !consider_as_null( vaSeqValue )){
					// 対応する路線情報管理を取得	
					_IRowPtr ipLineRow = GetLineMaster( vaSeqValue.lVal );
					if( ipLineRow ){

						// 路線情報管理テーブルの事業者名取得
						CComVariant vaLineValue;
						ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), &vaLineValue );
						if( !consider_as_null( vaLineValue ) && VT_BSTR == vaLineValue.vt){
							companys.insert( vaLineValue.bstrVal );
						}
					}
				}
			}

			// 事業者名が二つ以上取得された場合
			if( 1 < companys.size() ){
				// 【同一の停留所が異なる事業者の路線で使用されている】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t同一の停留所が異なる事業者の路線で使用されている\t\t\t\t\t0\t" << endl;
			}
			}
			break;

		// 国土数値情報ポイントのみ
		case bus_stop_point::point_attr_code::kNationalLand: 
			{

			// 対応する停車順を取得
			long seqCnt = 0;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			if( 0 < seqCnt ){
				// 国土数値情報ポイントに対応している停車順がある
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t国土数値情報ポイントに対応している停車順がある\t\t\t\t\t0\t" << endl;
			}
			}
			break;


		// 経路中間ポイントのみ
		case bus_stop_point::point_attr_code::kRoute: 
			{

			// 【経路中間ポイントにポイント種別以外の属性が入力されている】
			for( const auto& val : mapAttr )
			{
				if( val.first == bus_stop_point::kPointAttrCode ){
					continue;
				}
				if( !consider_as_null( val.second ) ){
					m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t経路中間ポイントにポイント種別以外の属性が入力されている\t\t\t\t\t0\t" << endl;
					break;
				}
			}
			}
			break;
		}

		// 標柱ポイントと国土数値情報ポイント
		if( bus_stop_point::point_attr_code::kPole == pAttrCode || bus_stop_point::point_attr_code::kNationalLand == pAttrCode ){
	
			// ポイント自体
			if( consider_as_null( mapAttr[bus_stop_point::kName] )){
				// 【停留所標柱ポイント、国土数値情報ポイントの停留所名が空白である】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t停留所標柱ポイント、国土数値情報ポイントの停留所名が空白である\t\t\t\t\t0\t" << endl;
			}

			if( consider_as_null( mapAttr[bus_stop_point::kYomi] )){
				// 【停留所標柱ポイント、国土数値情報ポイントの停留所名が空白である】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t停留所標柱ポイント、国土数値情報ポイントの停留所名よみがなが空白である\t\t\t\t\t0\t" << endl;
			}
		}

		// 標柱ポイントと経路中間ポイント
		if( bus_stop_point::point_attr_code::kPole == pAttrCode || bus_stop_point::point_attr_code::kRoute == pAttrCode ){

			// 対応する停車順を取得
			long seqCnt = 0;
			_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusStopPointID, seqCnt );
			if( 0 == seqCnt ){
				// 【停留所標柱ポイント、経路中間ポイントに対応する停車順がない(WARNING)】
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tWARNING\t停留所標柱ポイント、経路中間ポイントに対応する停車順がない\t\t\t\t\t0\t" << endl;
			}
		}


		// 以下は全てのポイント（標柱ポイント・国土数値情報ポイント・経路中間ポイント）

		// 【(変更後停留所名/変更後停留所名よみ/名称変更備考）のうち、1つまたは2つが空欄になっている】
		if( consider_as_null( mapAttr[ bus_stop_point::kChangedName ] ) == 
			( consider_as_null( mapAttr[ bus_stop_point::kChangedYomi ] ) &&
			consider_as_null( mapAttr[ bus_stop_point::kRemarks ] ) ) ){
			// OK
		}
		else{
			// ERROR
			m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << oID << "\tERROR\t(変更後停留所名/変更後停留所名よみ/名称変更備考）のうち、1つまたは2つが空欄になっている\t\t\t\t\t0\t" << endl;
		}

		// 停留所コードと標柱番号が両方同一のバス停ポイント確認の準備で、二つをキーにして、vectorにOIDをつめる
		busStopPoints[make_pair( mapAttr[bus_stop_point::kStopID].lVal, mapAttr[bus_stop_point::kPoleNo].lVal )].push_back( oID );

	} // オブジェクトごとの処理ここまで


	// バス停ポイント同士の確認（属性）
	for( auto& busStopPoint : busStopPoints ){
		if( busStopPoint.second.size() > 1 ){
			// 【停留所コードと標柱番号が両方同一のバス停ポイントがある】
			for( auto& errOID : busStopPoint.second ){
				m_ofsChk << CT2CA(bus_stop_point::kTableName) << "\t" << errOID << "\tERROR\t停留所コードと標柱番号が両方同一のバス停ポイントがある\t\t\t\t\t0\t" << endl;
			}
		}
	}
}

// バスルート探索用ポイントチェック
void CCheckBusPoint::CheckRoutePoint()
{
	// バスルート探索用ポイントから全件取得
	IFeatureCursorPtr ipCursor;
	m_ipBusRoutePointFC->Search( NULL, VARIANT_FALSE, &ipCursor );

	IFeaturePtr ipRouteFeature;
	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextFeature(&ipRouteFeature) ){

		// フィーチャのNULLチェック
		if( !ipRouteFeature ){
			m_ofsChk << "#ERROR バス停ルート探索用ポイントチェック時にフィーチャ取得失敗" << endl;
			continue;
		}

		// ObjectID
		long oID = 0;
		ipRouteFeature->get_OID( &oID );

		// バス停ポイントID
		CComVariant vaValue;
		ipRouteFeature->get_Value( m_busRoutePointFM.GetFieldIndex( bus_route_point::kBusStopPointID ), &vaValue );
		long stopID = vaValue.lVal;

		// 【ルート探索用ポイントに対応する停留所ポイントがない】
		long count = 0;
		GetStopPoint( stopID, count );
		if( 0 == count ){
			m_ofsChk << CT2CA(bus_route_point::kTableName) << "\t" << oID << "\tERROR\tルート探索用ポイントに対応する停留所ポイントがない\t\t\t\t\t0\t" << endl;
		}
		
		// 【ルート探索用ポイントが道路リンクの近傍にない(閾値：50cm)】
		if( !GetNearRoadLink( ipRouteFeature )){
			m_ofsChk << CT2CA(bus_route_point::kTableName) << "\t" << oID << "\tERROR\tルート探索用ポイントが道路リンクの近傍にない(閾値：" << k_route_to_nw_dist << "m)\t\t\t\t\t0\t" << endl;
		}
	}
}

// バス路線マスタチェック
void CCheckBusPoint::CheckLineMaster()
{
	// バス路線マスタから全件取得
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	SAMELINEMASTER sameLineMaster; //!< 事業者名、系統番号、経路が同一がないか確認用map(左記3つをキーに、OID・路線名・経路)
	SAMELINENUM    sameLineNum;	//!< 同一系統番号で、系統名が異なるものの確認用map(系統番号をキーに、
	
	_IRowPtr ipLineRow;
	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextRow(&ipLineRow) ){

		// フィーチャのNULLチェック
		if( !ipLineRow ){
			m_ofsChk << "#ERROR バス路線マスタチェック時にフィーチャ取得失敗" << endl;
			continue;
		}

		// 属性を取得しておく
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busLineMasterFM.GetFieldMap() )
		{
			ipLineRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipLineRow->get_OID( &oID );

		// 対応する停車順を取得
		long seqCnt = 0;
		_ICursorPtr ipSeqCursor = GetStopSeqs( oID, bus_stop_seq::kBusLineMasterID, seqCnt );
		if( 0 == seqCnt ){
			// 【バス路線に対応する停車順がない】
			m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << oID << "\tERROR\tバス路線に対応する停車順がない\t" 
				<< CT2CA(mapAttr[bus_line_master::kCompany].bstrVal) << "\t" << CT2CA(mapAttr[bus_line_master::kRouteName].bstrVal)
				 << "\t" << CT2CA(mapAttr[bus_line_master::kRouteDetail].bstrVal) << "\t\t0\t" << endl;
		}

		// 事業者名、系統番号、経路が同一の路線情報確認の準備で、三つをキーにして、vectorに系統名とOIDをつめる
		sameLineMaster[make_pair( mapAttr[bus_line_master::kRouteNumber].lVal, 
			make_pair( mapAttr[bus_line_master::kCompany].bstrVal, mapAttr[bus_line_master::kRouteDetail].bstrVal ))].push_back(
			make_pair( mapAttr[bus_line_master::kRouteName].bstrVal, oID ));

		// 系統番号が同一で、系統名が異なる路線情報確認の準備で、系統番号をキーにして、vectorに事業者名・OID・系統名・経路をつめる
		sameLineNum[mapAttr[bus_line_master::kRouteNumber].lVal].push_back( make_pair( make_pair( mapAttr[bus_line_master::kCompany].bstrVal, oID ), 
			make_pair( mapAttr[bus_line_master::kRouteName].bstrVal, mapAttr[bus_line_master::kRouteDetail].bstrVal )));
	}

	// バス路線マスタ同士の確認
	CheckLineMasterAttrs( sameLineMaster, sameLineNum );
}

// バス路線マスタ同士の確認
void CCheckBusPoint::CheckLineMasterAttrs( const SAMELINEMASTER& sameLineMaster, const SAMELINENUM& sameLineNum )
{
	// バス路線マスタ同士の確認(事業者名、系統番号、経路が同一？)
	for( auto& lineMaster : sameLineMaster ){
		if( 1 < lineMaster.second.size() ){
			// 【事業者名、系統番号、経路が同一の路線情報がある】
			for( auto& errOID : lineMaster.second ){
				m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << errOID.second << "\tERROR\t事業者名、系統番号、経路が同一の路線情報がある\t" 
					<< CT2CA(lineMaster.first.second.first) << "\t" << CT2CA(errOID.first) << "\t" << CT2CA(lineMaster.first.second.second) 
					<< "\t\t0\t" << endl;
			}
		}
	}

	// バス路線マスタ同士の確認(系統番号が同一、系統名が異なる？)
	for( auto& lineNum : sameLineNum ){
		set<CString> routeNameSet;
		for( auto& routeName : lineNum.second ){
			// 系統名を格納
			routeNameSet.insert( routeName.second.first);
		}
		// 同一系統番号で系統名が2個以上
		if( 1 < routeNameSet.size() ){
			for( auto& errOIDs : lineNum.second ){
				// 【系統番号が同一で、系統名が異なる路線情報がある】
				m_ofsChk << CT2CA(bus_line_master::kTableName) << "\t" << errOIDs.first.second << "\tERROR\t系統番号が同一で、系統名が異なる路線情報がある\t"
					<< CT2CA(errOIDs.first.first) << "\t" << CT2CA(errOIDs.second.first) << "\t" << CT2CA(errOIDs.second.second) << "\t\t0\t" << endl;
			}
		}
	}
}

// バス停車順管理チェック
void CCheckBusPoint::CheckStopSeq()
{
	// バス停車順から全件取得
	_ICursorPtr ipCursor;
	m_ipBusStopSeqTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	// 同一路線の停車順確認用
	SAMESEQ sameSeq;

	_IRowPtr ipSeqRow;

	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextRow(&ipSeqRow) ){

		// フィーチャのNULLチェック
		if( !ipSeqRow ){
			m_ofsChk << "#ERROR バス停車順管理チェック時にフィーチャ取得失敗" << endl;
			continue;
		}

		// 属性を取得しておく
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busStopSeqFM.GetFieldMap() )
		{
			ipSeqRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipSeqRow->get_OID( &oID );

		// ログ出力用に路線マスタの系統名と経路と事業者名、自身のシーケンスを取得しておく
		_IRowPtr ipLineRow = GetLineMaster( mapAttr[bus_stop_seq::kBusLineMasterID].lVal );
		CComVariant vaLineValue;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteName), &vaLineValue );
		CString routeName   = vaLineValue.bstrVal;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteDetail), &vaLineValue );
		CString routeDetail = vaLineValue.bstrVal; 
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany), &vaLineValue );
		CString company = vaLineValue.bstrVal;
		long stopSeq = mapAttr[bus_stop_seq::kStopSeq].lVal;

		// 同一路線の停車順確認の準備で、路線IDをキーに、順序番号・バス停ポイントID・ObjectID・事業者名・系統名・経路をVectorに格納
		sameSeq[mapAttr[bus_stop_seq::kBusLineMasterID].lVal].push_back( make_pair( 
			make_pair( mapAttr[bus_stop_seq::kStopSeq].lVal, make_pair( mapAttr[bus_stop_seq::kBusStopPointID].lVal, oID )), 
			make_pair( company, make_pair( routeName, routeDetail ))));

		// 【対応するバス停ポイントがない】
		long count = 0;
		GetStopPoint( mapAttr[bus_stop_seq::kBusStopPointID].lVal, count );
		if( 0 == count ){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t対応するバス停ポイントがない\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// 【指定された数字と記号以外の値が入力されている(経路曜日)】
		boost::wregex	regOnlyNumScolon(L"^([1-6;]|99)+$");	// (経路曜日(DAY_CODES)：1～6および99の半角数字と、半角セミコロン(;)のみの正規表現
		if( !consider_as_null( mapAttr[bus_stop_seq::kDayCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kDayCodes].bstrVal, regOnlyNumScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t指定された数字と記号以外の値が入力されている(経路曜日)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}
		
		// 【指定された数字と記号以外の値が入力されている(経路時間帯)】
		boost::wregex	regOnlyNumColonScolon(L"^[0-9;:\-]+$");	// 半角数字+半角セミコロン+半角コロン+半角ハイフンのみの正規表現
		if( !consider_as_null( mapAttr[bus_stop_seq::kTimeCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOnlyNumColonScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t指定された数字と記号以外の値が入力されている(経路時間帯)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// 【経路時間帯としてあり得ない時刻が入力されている(4:00から27:59の間以外)】
		// [Bug 10475]チェック内容(経路時間帯）の変更 により、[0:00～23:59]から修正
		boost::wregex	regOKTime(L"^(( |;)*(([4-9]|[1][0-9]|[2][0-7]):[0-5][0-9]-([4-9]|[1][0-9]|[2][0-7]):[0-5][0-9])*( |;)*)*$");	// [4:00～27:59]-[4:00～27:59]と ;（複数区切り）
		if( !consider_as_null( mapAttr[bus_stop_seq::kTimeCodes] ) && !IsOKRegEx( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOKTime )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t経路時間帯としてあり得ない時刻が入力されている(4:00から27:59の間以外)\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// 【経路曜日と時間帯の条件数が異なる】
		boost::wregex	regOnlyScolon(L";");
		if( GetRegExCnt( mapAttr[bus_stop_seq::kDayCodes].bstrVal, regOnlyScolon ) != GetRegExCnt( mapAttr[bus_stop_seq::kTimeCodes].bstrVal, regOnlyScolon )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t経路曜日と時間帯の条件数が異なる\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}

		// 【経路曜日が99(その他)なのに、曜日時間備考に記載がない】
		CString dayCodes = mapAttr[bus_stop_seq::kDayCodes].bstrVal;
		if( _T("99") == dayCodes && consider_as_null( mapAttr[bus_stop_seq::kRemarks] )){
			m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << oID << "\tERROR\t経路曜日が99(その他)なのに、曜日時間備考に記載がない\t"
				<< CT2CA(company) << "\t" << CT2CA(routeName) << "\t" << CT2CA(routeDetail) << "\t" << stopSeq << "\t0\t" << endl;
		}
	}

	// 同一路線の停車順確認
	CheckSameSeqs( sameSeq );

}

// 同一路線の停車順確認
void CCheckBusPoint::CheckSameSeqs( const SAMESEQ& sameSeq )
{
	// lineMaster→first:路線ID, second( first:(first:stopSeq, second:(first:BusStopPointID, second:oID)), second:(first:company, second:(first:routeName, second:routeDetail)))
	for( auto& lineMaster : sameSeq ){

		SAMELINESEQ sameLineMap;

		for( auto& sameLine : lineMaster.second ){

			// 停車順をキーに、バス停IDとObjectIDと事業者と系統名と経路をvectorに格納
			sameLineMap[sameLine.first.first].push_back( make_pair( make_pair( sameLine.first.second.first, sameLine.first.second.second ), 
				make_pair( sameLine.second.first, make_pair( sameLine.second.second.first, sameLine.second.second.second ))));
		}

		// 同一路線のシーケンス確認
		for( int i = 1 ; i < sameLineMap.rbegin()->first ; ++i ){

			// 1～最大値まで、シーケンスが存在するか確認する
			if( sameLineMap.end() == sameLineMap.find( i ) ){

				// 1がない
				if( i == 1 ){
					// 【同一路線の停車順が1から始まらない】
					m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << (sameLineMap.begin()->second).begin()->first.second
						<< "\tERROR\t同一路線の停車順が1から始まらない\t" << CT2CA((sameLineMap.begin()->second).begin()->second.first)
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.first) 
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.second) << "\t" << i << "\t0\t" << endl;
				}
				// それ以外の欠番
				else{
					// 【同一路線の停車順に欠番がある】
					m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << (sameLineMap.begin()->second).begin()->first.second 
						<< "\tERROR\t同一路線の停車順に欠番がある\t" << CT2CA((sameLineMap.begin()->second).begin()->second.first)
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.first) 
						<< "\t" << CT2CA((sameLineMap.begin()->second).begin()->second.second.second) << "\t" << i << "\t0\t" << endl;
				}
			}
		}

		// seqChk→first:停車順, second( first( first:バス停ID, second:ObjectID ), second( first:事業者名, second:(first:系統名, second:経路 )))) 
		// 重複シーケンスの場合、バス停ポイントの停留所コード(STOP_ID)を確認
		for( auto& seqChk : sameLineMap ){

			// シーケンスが重複している場合
			if( 1 < seqChk.second.size() ){

				set<long> stopIDs;

				// バス停ポイントの停留所コード(STOP_ID)を確認する
				for( auto& stopPointID : seqChk.second ){

					// バス停車順のバス停ポイントID(BUS_STOP_POINT_ID)から、バス停ポイントを取得し、停留所コード(STOP_ID)を格納する
					long count = 0;
					IFeaturePtr ipStopFeature = GetStopPoint( stopPointID.first.first, count );
					if( ipStopFeature ){
						CComVariant vaValue;
						ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), &vaValue );
						stopIDs.insert( vaValue.lVal );
					}
				}
				
				// 同一路線、同一シーケンスで、バス停ポイントID(STOP_ID)が異なる場合
				if( 1 < stopIDs.size() ){
							
					for( auto& errOIDs : seqChk.second ){

						// 【同一路線の停車順で、同一停留所コード以外のバス停で順序番号が重複している】
						m_ofsChk << CT2CA(bus_stop_seq::kTableName) << "\t" << errOIDs.first.second 
							<< "\tERROR\t同一路線の停車順で、同一停留所コード以外のバス停で順序番号が重複している\t" 
							<< CT2CA(errOIDs.second.first) << "\t" << CT2CA(errOIDs.second.second.first) << "\t" 
							<< CT2CA(errOIDs.second.second.second) << "\t" << seqChk.first << "\t0\t" << endl;
					}
				}
			}
		}
	}
}

// バス停ポイントに対応するルート探索用ポイントを取得
IFeaturePtr CCheckBusPoint::GetRoutePoint( const long& oID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// バス停ポイントID
	strWhere.Format( _T("%s = %d"), bus_route_point::kBusStopPointID, oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 個数カウント
	m_ipBusRoutePointFC->FeatureCount( ipFilter, &count );

	// 1個以外はNULLを返す
	if( count != 1 ){
		return nullptr;
	}

	// 検索
	IFeaturePtr ipFeature;
	IFeatureCursorPtr ipCursor;
	m_ipBusRoutePointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		ipCursor->NextFeature( &ipFeature );
		return ipFeature;
	}
	else{
		return nullptr;
	}
}

// フィーチャ間の距離(m)を測定
double CCheckBusPoint::GetDistFeature( const IFeaturePtr& ipSrcFeature, const IFeaturePtr& ipDestFeature )
{
	static crd_cnv crdCnv;

	double srcX = 0, srcY = 0, destX = 0, destY = 0;
	IGeometryPtr ipSrcGeo, ipDestGeo;
	ipSrcFeature->get_ShapeCopy( &ipSrcGeo );
	ipDestFeature->get_ShapeCopy( &ipDestGeo );
	IPointPtr ipSrcPoint( ipSrcGeo ), ipDestPoint( ipDestGeo );
	ipSrcPoint->QueryCoords( &srcX, &srcY );
	ipDestPoint->QueryCoords( &destX, &destY );

	// crd_cnvの距離測定結果を返す(m)
	return crdCnv.GetDist( srcX, srcY, destX, destY );
}

// 指定フィールドとObjectIDに対応する停車順管理テーブルを取得
_ICursorPtr CCheckBusPoint::GetStopSeqs( const long& oID, const CString& field, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// バス停ポイントID or バス路線マスタID
	strWhere.Format( _T("%s = %d"), field, oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 個数カウント
	m_ipBusStopSeqTable->RowCount( ipFilter, &count );

	// 0個はNULLを返す
	if( count == 0 ){
		return nullptr;
	}

	// 検索
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusStopSeqTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	
	return ipCursor;
}

// 停車順管理テーブルの路線情報IDから路線情報管理テーブルを取得
_IRowPtr CCheckBusPoint::GetLineMaster( const long& lineID )
{
	long count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (路線情報管理テーブル)OBJECTID = (停車順管理テーブル)路線情報ID
	strWhere.Format( _T("OBJECTID = %d"), lineID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 個数カウント
	m_ipBusLineMasterTable->RowCount( ipFilter, &count );

	// 0個はNULLを返す
	if( count == 0 ){
		return nullptr;
	}

	// 検索
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		ipCursor->NextRow( &ipRow );
		return ipRow;
	}
	else{
		return nullptr;
	}
}

// バス停ポイントの指定近傍のバス停ポイントを取得
IFeatureCursorPtr CCheckBusPoint::GetNearStopPoints( const IFeaturePtr& ipFeature, long& count )
{
	count = 0;

	// 指定近傍のBufferを作成
	IGeometryPtr ipGeo, ipBuf;
	ipFeature->get_ShapeCopy( &ipGeo );

	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;

	// 東京
	ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan9, &ipProjectedCoordinateSystem);
	ipGeo->Project(ipProjectedCoordinateSystem);

	((ITopologicalOperatorPtr)ipGeo)->Buffer( k_pole_to_pole_dist, &ipBuf );
	ISpatialFilterPtr ipFilter( AheInitSpatialFilter( NULL, ipBuf ));

	// 自分自身を除外するWhere句
	long oID = 0;
	ipFeature->get_OID( &oID );
	CString strWhere;
	strWhere.Format( _T("OBJECTID <> %d"), oID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 0個はNULLを返す
	if( count == 0 ){
		return nullptr;
	}

	// 指定近傍範囲のバス停ポイントを検索
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );

	return ipCursor;
}

// バス停ポイントのカーソルから、標柱ポイント数をカウント
long CCheckBusPoint::CountPolePoint( const IFeatureCursorPtr& ipCursor )
{
	long count = 0;
	IFeaturePtr ipFeature;
	while( ipCursor && S_OK == ipCursor->NextFeature( &ipFeature )){

		// フィーチャのNULLチェック
		if( !ipFeature ){
			continue;
		}

		// 標柱ポイントかどうか確認
		CComVariant vaValue;
		ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPointAttrCode ), &vaValue );

		if( bus_stop_point::point_attr_code::kPole == vaValue.lVal ){
			
			// 見つかった個数をカウント
			++count;
		}
	}
	return count;
}

// ルート探索用ポイントor停車順に対応するバス停ポイントを取得
IFeaturePtr CCheckBusPoint::GetStopPoint( const long& stopID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (バス停ポイント)OBJECTID = (ルート探索用ポイントor停車順)バス停ポイントID
	strWhere.Format( _T("OBJECTID = %d"), stopID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 個数カウント
	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 0個はNULLを返す
	if( count != 1 ){
		return nullptr;
	}

	// 検索
	IFeatureCursorPtr ipCursor;
	m_ipBusStopPointFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( ipCursor ){
		IFeaturePtr ipFeature;
		ipCursor->NextFeature( &ipFeature ); 
		return ipFeature;
	}
	else{
		return nullptr;
	}
}

// ルート探索用ポイントの指定近傍の道路リンクを取得
IFeaturePtr CCheckBusPoint::GetNearRoadLink( const IFeaturePtr& ipFeature )
{
	long count = 0;

	// 指定近傍のBufferを作成
	IGeometryPtr ipGeo, ipBuf;
	ipFeature->get_ShapeCopy( &ipGeo );

	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;

	// 東京
	ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan9, &ipProjectedCoordinateSystem);
	ipGeo->Project(ipProjectedCoordinateSystem);

	((ITopologicalOperatorPtr)ipGeo)->Buffer( k_route_to_nw_dist, &ipBuf );
	ISpatialFilterPtr ipFilter( AheInitSpatialFilter( NULL, ipBuf ));

	// 指定近傍範囲の道路リンクをカウント
	m_ipRoadLinkFC->FeatureCount( ipFilter, &count );

	// 0個はNULLを返す
	if( count == 0 ){
		return nullptr;
	}

	// 指定近傍範囲の道路リンクを検索
	IFeatureCursorPtr ipCursor;
	m_ipRoadLinkFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );

	if( ipCursor ){
		IFeaturePtr ipRetFeature;
		ipCursor->NextFeature( &ipRetFeature );
		return ipRetFeature;
	}
	else{
		return nullptr;
	}
}


