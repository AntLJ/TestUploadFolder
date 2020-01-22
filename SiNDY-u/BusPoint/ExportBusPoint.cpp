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
#include "ExportBusPoint.h"
#include "BusPointCommon/Time.h"
#include "BusPointCommon/CommonFunc.h"
#include <sindy/workspace.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

// 各種処理を実行するために最初に呼ばれる関数
bool CExportBusPoint::execute()
{
	// 各種初期化を実施
	if( !Init() ){
		return false;
	}

	// 出力対象事業者リスト作成
	if( m_allCompany ){
		// 全事業者出力モードの場合
		if( !CreateCompanyList() ){
			cerr << "#ERROR 路線マスタから全事業者リスト作成失敗" << endl;
			return false;
		}
	}
	else{
		// 出力事業者名リスト指定の場合
		if( !LoadList() ){
			cerr << "#ERROR 事業者名リストの読み込み失敗" << endl;
			return false;
		}
	}

	// チェック先・参照先DB情報出力
	m_ofsExp << "#DB_BUS：" << CT2CA(m_strBusDB) << endl;

	// 開始時間
	m_ofsExp << "#開始時間：" << CT2CA(GetCurTime()) << endl;

	for( auto& companyName : m_companySet ){

		m_ofsExp << "#出力事業者名：" << CT2CA(companyName) << endl;

		// バス停経路情報出力ファイルオープン（<指定Dir>\バス停・バス経路情報（事業社名）yyyymmdd.csv）
		CString strDetailFile;
		strDetailFile.Format( _T("%s\\バス停・バス経路情報（%s）%s.csv"), m_strExpDir, companyName, GetCurDay() );
		m_ofsDetailFile.open( strDetailFile );
		if( !m_ofsDetailFile ){
			cerr << "#ERROR バス停経路情報出力ファイルオープンに失敗：" << CT2CA(strDetailFile) << endl;
			return false;
		}

		// バス停留所名異表記情報出力ファイル（<指定Dir>\バス停留所名異表記情報（事業社名）yyyymmdd.csv）
		CString strChangedFile;
		strChangedFile.Format( _T("%s\\バス停留所名異表記情報（%s）%s.csv"), m_strExpDir, companyName, GetCurDay() );
		m_ofsChangedFile.open( strChangedFile );
		if( !m_ofsChangedFile ){
			cerr << "#ERROR バス停留所名異表記情報出力ファイルオープンに失敗：" << CT2CA(strChangedFile) << endl;
			return false;
		}

		// バス停経路情報出力ファイルヘッダ出力
		m_ofsDetailFile << "番号,種別番号,順序番号,停留所コード,停留所名,停留所名よみがな,標柱番号,系統番号,"
			<< "系統名,経路,発停留所名,着停留所名,経由,経路曜日,経路時間帯,緯度,経度,備考" << endl;

		// バス停留所名異表記情報出力ファイルヘッダ出力
		m_ofsChangedFile << "番号,停留所コード,停留所名,停留所名よみがな,標柱番号,異表記名,異表記よみがな,備考" << endl;

		// 出力
		Export( companyName );

		// バス停経路情報出力ファイルクローズ
		m_ofsDetailFile.close();

		// バス停留所名異表記情報出力ファイルクローズ
		m_ofsChangedFile.close();
	}

	// 終了時間
	m_ofsExp << "#終了時間：" << CT2CA(GetCurTime()) << endl;

	return true;
}

// 各種初期化
bool CExportBusPoint::Init()
{
	// DBチェックログファイルオープン
	m_ofsExp.open( m_strExpLog );
	if( !m_ofsExp ){
		cerr << "#ERROR エクスポートログファイルオープンに失敗：" << CT2CA(m_strExpLog) << endl;
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

	return true;
}

// バス路線マスタから全事業者リストを作成する
bool CExportBusPoint::CreateCompanyList()
{
	// バス路線マスタから全件取得
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( NULL, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipLineRow;
	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextRow(&ipLineRow) ){

		// フィーチャのNULLチェック
		if( !ipLineRow ){
			continue;
		}

		// 事業者名を取得
		CComVariant vaValue;
		ipLineRow->get_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), &vaValue );
		if( !consider_as_null( vaValue ) ){
			m_companySet.insert( vaValue.bstrVal );
		}
	}

	if( m_companySet.size() > 0 ){
		return true;
	}
	else{
		return false;
	}
}

// 出力対象事業者リストを読み込み、事業者名1行ずつをリストに格納
bool CExportBusPoint::LoadList()
{
	ifstream ifs( m_strCompanyList );
	if( !ifs ){
		return false;
	}

	string str;
	while( getline( ifs, str )){
		CString cstr = str.c_str();
	
		if( !cstr.IsEmpty() ){
			m_companySet.insert( cstr );
		}
	}

	ifs.close();

	return true;
}

// 出力メイン関数
void CExportBusPoint::Export( const CString& companyName )
{
	// バス路線マスタから、
	// ・該当事業者の情報を系統番号をキーに、ObjectID・系統名・経路・経由地等備考を格納（経路情報出力用）
	// ・ObjectIDをセットに格納（異表記名出力用）
	LINEMASTERS lineMasters;
	OIDSET      lineOIDs;
	GetLineMaster( companyName, lineMasters, lineOIDs );

	// バス停経路情報出力
	ExportDetail( lineMasters );

	// バス停留所名異表記情報出力
	ExportChanged( lineOIDs );
}

// バス路線マスタから該当事業者の情報を取得
void CExportBusPoint::GetLineMaster( const CString& companyName, LINEMASTERS& lineMasters, OIDSET& lineOIDs )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// 事業者名
	strWhere.Format( _T("%s = '%s'"), bus_line_master::kCompany, companyName );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 検索
	_IRowPtr    ipRow;
	_ICursorPtr ipCursor;
	m_ipBusLineMasterTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	
	// オブジェクトごとの処理
	while( ipCursor && S_OK == ipCursor->NextRow(&ipRow) ){
		
		// フィーチャのNULLチェック
		if( !ipRow ){
			m_ofsExp << "#ERROR バス路線マスタからフィーチャ取得失敗" << endl;
			continue;
		}

		// 属性を取得
		CComVariant vaValue;
		MAPATTR     mapAttr;
		for( auto& field : m_busLineMasterFM.GetFieldMap() )
		{
			ipRow->get_Value( field.second, &vaValue );
			mapAttr[field.first] = vaValue;
		}

		// ObjectID
		long oID = 0;
		ipRow->get_OID( &oID );

		// 後でキーとなるフィールドのNULLチェック(念のため)
		if( consider_as_null( mapAttr[bus_line_master::kRouteNumber] ) 
			|| consider_as_null( mapAttr[bus_line_master::kRouteDetail] )
			|| 0 == oID )
		{
			m_ofsExp << "#ERROR バス路線マスタの系統番号or経路orObjectIDが空" << endl;
			continue;
		}

		// 系統番号をキーに、ObjectID・系統名・経路・経由地等備考を格納（経路情報出力用）
		lineMasters[mapAttr[bus_line_master::kRouteNumber].lVal].push_back( make_pair(
			make_pair( oID, mapAttr[bus_line_master::kRouteName].bstrVal ),
			make_pair( mapAttr[bus_line_master::kRouteDetail].bstrVal, mapAttr[bus_line_master::kRouteRemarks].bstrVal )));

		// ObjectIDをセットに格納（異表記名出力用）
		lineOIDs.insert( oID );
	}
}

// バス停経路情報出力
void CExportBusPoint::ExportDetail( LINEMASTERS& lineMasters )
{
	// 出力用行番号
	long lineIdx = 0;

	// 系統番号ごとの処理
	for( auto& lineMaster : lineMasters ){
		// lineMaster→first:系統番号, second( first:( first:ObjectID, second:系統名 ), second:( first:経路, second:経由地等備考 ))
	
		long routeNum = lineMaster.first; //!< 系統番号
		
		// 経路の昇順でソートする
		sort( lineMaster.second.begin(), lineMaster.second.end(), lessDetail() );

		// 経路ごとの処理
		for( auto& details : lineMaster.second ){
			// routeDetails→first:( first:ObjectID, second:系統名 ), second:( first:経路, second:経由地等備考 )

			CString routeName    = details.first.second;  //!< 系統名
			CString routeDetail  = details.second.first;  //!< 経路
			CString routeRemarks = details.second.second; //!< 経由地等備考
			long    lineOID      = details.first.first;   //!< 路線マスタのObjectID

			long    count = 0;

			// バス路線マスタのObjectIDから、紐づく停車順管理を取得
			_ICursorPtr ipSeqCursor = GetStopSeqs( lineOID, bus_stop_seq::kBusLineMasterID, count );
			_IRowPtr    ipSeqRow;

			// 同一経路の停車順格納用
			STOPSEQS stopSeqs;

			// 停車順ごとの処理
			while( ipSeqCursor && S_OK == ipSeqCursor->NextRow(&ipSeqRow) ){
		
				// フィーチャのNULLチェック
				if( !ipSeqRow ){
					m_ofsExp << "#ERROR バス停車順からフィーチャ取得失敗" << endl;
					continue;
				}

				// 停車順の属性を取得
				CComVariant vaSeqValue;
				MAPATTR     mapSeqAttr;
				for( auto& field : m_busStopSeqFM.GetFieldMap() )
				{
					ipSeqRow->get_Value( field.second, &vaSeqValue );
					mapSeqAttr[field.first] = vaSeqValue;
				}

				// 順序番号でソートして出力するため、同一経路の停車順を順序番号をキーに属性をmapに格納
				// （順序番号をキーにするが、順序番号は重複するため、multimap）
				stopSeqs.insert( make_pair( mapSeqAttr[bus_stop_seq::kStopSeq].lVal, mapSeqAttr ) );
			}

			// 経路内で停車順が一つも存在しない場合はスキップ
			if( stopSeqs.size() < 1 ){
				continue;
			}

			// 発停留所名取得(経路内の停車順順序番号が1の(変更後)停留所名)
			CString startStopName = GetStopName( stopSeqs.begin()->second );

			// 着停留所名取得(経路内の停車順順序番号が最大の(変更後)停留所名)
			CString endStopName   = GetStopName( stopSeqs.rbegin()->second );

			for( auto& stopSeq : stopSeqs ){ 

				// バス停ポイントIDのNULLチェック(念のため)
				if( consider_as_null( stopSeq.second[bus_stop_seq::kBusStopPointID] )){
					m_ofsExp << "#ERROR バス停車順からバス停ポイントID取得失敗" << endl;
					continue;
				}

				// 停車順のバス停ポイントIDから、バス停ポイントを取得
				IFeaturePtr ipStopFeature = GetStopPoint( stopSeq.second[bus_stop_seq::kBusStopPointID].lVal, count );

				// 取得したバス停ポイントの処理
				if( ipStopFeature ){

					// バス停ポイントの属性を取得
					CComVariant vaStopValue;
					MAPATTR     mapStopAttr;
					for( auto& field : m_busStopPointFM.GetFieldMap() )
					{
						ipStopFeature->get_Value( field.second, &vaStopValue );
						mapStopAttr[field.first] = vaStopValue;
					}

					// バス停ポイントのObjectID取得
					long stopID = 0;
					ipStopFeature->get_OID( &stopID );

					// バス停ポイントのObjectIDから、ルート探索用ポイントを取得
					IFeaturePtr ipRouteFeature = GetRoutePoint( stopID, count );
					
					// 取得したルート探索用ポイントの処理
					if( ipRouteFeature ){

						// ルート探索用ポイントの形状から、1/256秒緯度・経度を取得
						int routeLat256 = 0, routeLon256 = 0;
						Get256LatLon( ipRouteFeature, routeLat256, routeLon256 );

						// ルート探索用ポイントのObjectID取得
						long routeID = 0;
						ipRouteFeature->get_OID( &routeID );


						// // バス停経路情報出力ファイルにルート探索用ポイント1行出力
						if( !ExportDetailLine( ++lineIdx, routeLat256, routeLon256, routeNum, routeName, routeDetail, routeRemarks, startStopName, endStopName, stopSeq.second, mapStopAttr, true )){
							m_ofsExp << "#ERROR ルート探索用ポイント1行出力失敗（番号：" << lineIdx << "、ObjectID：" << routeID << "）" << endl;
						}
					}

					// バス停ポイントの形状から、1/256秒緯度・経度を取得
					int stopLat256 = 0, stopLon256 = 0; 
					Get256LatLon( ipStopFeature, stopLat256, stopLon256 );


					// バス停経路情報出力ファイルにバス停ポイント1行出力
					if( !ExportDetailLine( ++lineIdx, stopLat256, stopLon256, routeNum, routeName, routeDetail, routeRemarks, startStopName, endStopName, stopSeq.second, mapStopAttr )){
						m_ofsExp << "#ERROR バス停ポイント1行出力失敗（番号：" << lineIdx << "、ObjectID：" << stopID << "）" << endl;
					}
				}
				else{
					// バス停ポイント取得失敗
					if( !ipStopFeature ){
						m_ofsExp << "#ERROR 停車順のバス停ポイントIDからバス停ポイント取得失敗" << endl;
						continue;
					}
				}
			}
		}

	}
}

// バス停留所名異表記情報出力
void CExportBusPoint::ExportChanged( const OIDSET& lineOIDs )
{
	// 出力用行番号
	long lineIdx = 0;

	long count = 0;

	// バス停ポイント格納用
	STOPPOINTS stopPoints;

	// 該当事業者名を持つ路線マスタのObjectIDごとの処理
	for( auto& lineOID : lineOIDs ){

		// バス路線マスタのObjectIDから、紐づく停車順管理を取得
		_ICursorPtr ipSeqCursor = GetStopSeqs( lineOID, bus_stop_seq::kBusLineMasterID, count );
		_IRowPtr    ipSeqRow;

		// 停車順ごとの処理
		while( ipSeqCursor && S_OK == ipSeqCursor->NextRow(&ipSeqRow) ){
		
			// フィーチャのNULLチェック
			if( !ipSeqRow ){
				m_ofsExp << "#ERROR バス停車順からフィーチャ取得失敗" << endl;
				continue;
			}

			// 停車順のバス停ポイントIDを取得
			CComVariant vaSeqValue;
			ipSeqRow->get_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusStopPointID ), &vaSeqValue );

			// 停車順のバス停ポイントIDからバス停ポイントを取得
			IFeaturePtr ipStopFeature = GetStopPoint( vaSeqValue.lVal, count );

			// 取得したバス停ポイントの処理
			if( ipStopFeature ){

				// バス停ポイントの属性を取得
				CComVariant vaStopValue;
				MAPATTR     mapStopAttr;
				for( auto& field : m_busStopPointFM.GetFieldMap() )
				{
					ipStopFeature->get_Value( field.second, &vaStopValue );
					mapStopAttr[field.first] = vaStopValue;
				}

				// ソート出力のため、停留所コードを第一キー、標柱番号を第二キーとして、バス停ポイントの属性をmapに格納
				stopPoints[mapStopAttr[bus_stop_point::kStopID].lVal].insert( 
					make_pair( mapStopAttr[bus_stop_point::kPoleNo].lVal, mapStopAttr ));
			}
		}
	}

	// 停留所コードごとのバス停ポイント処理
	for( auto& stopPoint : stopPoints ){

		// 標柱番号ごとのバス停ポイント処理
		for( auto& stopAttr : stopPoint.second ){

			// 変更後停留所名称がNULL以外が出力対象
			if( !consider_as_null( stopAttr.second[bus_stop_point::kChangedName] )){

				ExportChangedLine( ++lineIdx, stopAttr.second );
			}
		}
	}
}

// 該当停車順と紐づくバス停ポイントの(変更後)停留所名取得
CString CExportBusPoint::GetStopName( const MAPATTR& mapSeqAttr )
{
	try{
		// 停車順のバス停ポイントIDから、バス停ポイントを取得
		long stopCount = 0;
		IFeaturePtr ipStopFeature = GetStopPoint( (mapSeqAttr.at( bus_stop_seq::kBusStopPointID )).lVal, stopCount );
	
		if( ipStopFeature ){
			CComVariant vaValue;
	
			// BUS_STOP_SEQと紐づくバス停ポイントの
			//   CHANGED_STOP_NAME_KANJI != NULL：CHANGED_STOP_NAME_KANJI
			//   CHANGED_STOP_NAME_KANJI  = NULL：STOP_NAME_KANJI 
			ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedName ), &vaValue );

			if( !consider_as_null( vaValue ) ){
				return vaValue.bstrVal;
			}
			else{
				ipStopFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), &vaValue );
				if( !consider_as_null( vaValue ) ){
					return vaValue.bstrVal;
				}
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR バス停ポイントの停留所名取得時に例外が発生：" << e.what() << endl;
		return _T("");
	}

	// バス停ポイントが見つからない or 名称がどちらもNULLは空文字
	return _T("");
}

// フィーチャから、1/256秒緯度・経度を取得
void CExportBusPoint::Get256LatLon( const IFeaturePtr& ipFeature, int& lat256, int& lon256 )
{
	static crd_cnv crdCnv;

	IGeometryPtr ipGeo;
	ipFeature->get_ShapeCopy( &ipGeo );
	IPointPtr ipPoint( ipGeo );

	double x = 0, y = 0;
	ipPoint->QueryCoords( &x, &y );

	crdCnv.LLtoLL256( x, y, &lon256, &lat256 );
}

// バス停経路情報出力ファイルに1行出力
bool CExportBusPoint::ExportDetailLine( const long lineIdx, const int lat256, const int lon256, const long routeNum, const CString& routeName, const CString& routeDetail, const CString& routeRemarks, const CString& startStopName, const CString& endStopName, const MAPATTR& mapSeqAttr, const MAPATTR& mapStopAttr, bool routeFlag /* = false */ )
{
	try{
		// 【種別番号】
		// ルート探索用ポイント：「0」で固定
		// バス停ポイント（BUS_STOP_SEQ.PASS_F=1の場合）：「2」(通過停留所ポイント)
		//               （それ以外の場合）             ：BUS_STOP_POINT.POINT_ATTR_CODEの値
		long attrCode = 0;
		if( !routeFlag ){
			attrCode = ((mapSeqAttr.at( bus_stop_seq::kPassF )).lVal == 1)?  2 : (mapStopAttr.at( bus_stop_point::kPointAttrCode )).lVal;
		}

		// 【停留所名】
		// BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI != NULL：BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI
		// BUS_STOP_POINT.CHANGED_STOP_NAME_KANJI  = NULL：BUS_STOP_POINT.STOP_NAME_KANJI
		CString stopName = !consider_as_null(mapStopAttr.at( bus_stop_point::kChangedName ))? 
				(mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal : (mapStopAttr.at( bus_stop_point::kName )).bstrVal;

		// 【停留所名よみがな】
		// BUS_STOP_POINT.CHANGED_STOP_NAME_YOMI != NULL：BUS_STOP_POINT.CHANGED_STOP_NAME_YOMI
		// BUS_STOP_POINT.CHANGED_STOP_NAME_YOMI  = NULL：BUS_STOP_POINT.STOP_NAME_YOMI
		CString stopYomi = !consider_as_null(mapStopAttr.at( bus_stop_point::kChangedYomi ))? 
				(mapStopAttr.at( bus_stop_point::kChangedYomi )).bstrVal : (mapStopAttr.at( bus_stop_point::kYomi )).bstrVal;

		// 【経路曜日】
		CString dayCodes = !consider_as_null( mapSeqAttr.at( bus_stop_seq::kDayCodes ))? 
				(mapSeqAttr.at( bus_stop_seq::kDayCodes )).bstrVal : _T("");

		// 【経路時間帯】
		CString timeCodes = !consider_as_null( mapSeqAttr.at( bus_stop_seq::kTimeCodes ))?
				(mapSeqAttr.at( bus_stop_seq::kTimeCodes )).bstrVal : _T("");

		// 【備考】
		// 次の3つの内容を出力する。複数ある時は";"(セミコロン)で区切る。
		// 1.BUS_STOP_SEQ.REMARKSに入力されていればその内容をそのまま出力。
		// 2.停留所名称をBUS_STOP_POINT.CHANGED_STOP_NAME_KANJIから取得した場合は「名称変更([STOP_NAME_KANJI]->[CHANGED_STOP_NAME_KANJI])」と出力
		// 3.BUS_STOP_POINT.GETTING_OFF_ONLY=1なら"降車専用"と出力
		CString etcRemarks;
		if( !consider_as_null( mapSeqAttr.at( bus_stop_seq::kRemarks ))){
			etcRemarks = mapSeqAttr.at( bus_stop_seq::kRemarks );
		}
		if( !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedName ))){
			
			// 1が入力されている場合は、「 ; 」入力後に名称変更情報を追加
			etcRemarks.AppendFormat( _T("%s名称変更(%s->%s)"), 
				etcRemarks.IsEmpty()? _T("") : _T(" ; "),
				(mapStopAttr.at( bus_stop_point::kName )).bstrVal,
				(mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal );
		}
		if( 1 == (mapStopAttr.at( bus_stop_point::kGettingOffF )).lVal ){

			// 1と2のどちらかでも入力されている場合は、「 ; 」入力後に「降車専用」を追加
			etcRemarks.AppendFormat( _T("%s降車専用"), etcRemarks.IsEmpty()? _T("") : _T(" ; ") );
		}

		// 1行出力
		m_ofsDetailFile 
			<< lineIdx << ","                                         //!< 行番号
			<< attrCode << ","                                        //!< 種別番号
			<< (mapSeqAttr.at( bus_stop_seq::kStopSeq )).lVal << ","  //!< 順序番号
			<< (mapStopAttr.at( bus_stop_point::kStopID)).lVal << "," //!< 停留所コード
			<< CT2CA(stopName) << ","                                 //!< 停留所名
			<< CT2CA(stopYomi) << ","                                 //!< 停留所名よみがな
			<< (mapStopAttr.at( bus_stop_point::kPoleNo)).lVal << "," //!< 標柱番号
			<< routeNum << ","                                        //!< 系統番号
			<< CT2CA(routeName) << ","                                //!< 系統名
			<< CT2CA(routeDetail) << ","                              //!< 経路
			<< CT2CA(startStopName) << ","                            //!< 発停留所名
			<< CT2CA(endStopName) << ","                              //!< 着停留所名
			<< CT2CA(routeRemarks) << ","                             //!< 経由
			<< CT2CA(dayCodes) << ","                                 //!< 経路曜日
			<< CT2CA(timeCodes)  << ","                               //!< 経路時間帯
			<< lat256 << ","                                          //!< 1/256秒緯度
			<< lon256 << ","                                          //!< 1/256秒経度
			<< CT2CA(etcRemarks)                                      //!< 備考
			<< endl;
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR バス停経路情報出力ファイルに1行出力時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス停留所名異表記情報出力ファイルに1行出力
bool CExportBusPoint::ExportChangedLine( const long lineIdx, const MAPATTR& mapStopAttr )
{
	try{
		CString name        = !consider_as_null( mapStopAttr.at( bus_stop_point::kName ))?        (mapStopAttr.at( bus_stop_point::kName )).bstrVal        : _T("");
		CString yomi        = !consider_as_null( mapStopAttr.at( bus_stop_point::kYomi ))?        (mapStopAttr.at( bus_stop_point::kYomi )).bstrVal        : _T("");
		CString changedName = !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedName ))? (mapStopAttr.at( bus_stop_point::kChangedName )).bstrVal : _T("");
		CString changedYomi = !consider_as_null( mapStopAttr.at( bus_stop_point::kChangedYomi ))? (mapStopAttr.at( bus_stop_point::kChangedYomi )).bstrVal : _T("");
		CString remarks     = !consider_as_null( mapStopAttr.at( bus_stop_point::kRemarks ))?     (mapStopAttr.at( bus_stop_point::kRemarks )).bstrVal     : _T("");

		// 1行出力
		m_ofsChangedFile 
			<< lineIdx << ","                                          //!< 行番号
			<< (mapStopAttr.at( bus_stop_point::kStopID)).lVal << ","  //!< 停留所コード
			<< CT2CA(name) << ","                                      //!< 停留所名
			<< CT2CA(yomi) << ","                                      //!< 停留所名よみがな
			<< (mapStopAttr.at( bus_stop_point::kPoleNo )).lVal << "," //!< 標柱番号
			<< CT2CA(changedName) << ","                               //!< 異表記名
			<< CT2CA(changedYomi) << ","                               //!< 異表記名よみがな
			<< CT2CA(remarks)                                          //!< 備考
			<< endl;
	}
	catch( out_of_range& e ){
		m_ofsExp << "#ERROR バス停留所名異表記情報出力ファイルに1行出力時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// 指定フィールドとObjectIDに対応する停車順管理テーブルを取得
_ICursorPtr CExportBusPoint::GetStopSeqs( const long oID, const CString& field, long& count )
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

// ルート探索用ポイントor停車順に対応するバス停ポイントを取得
IFeaturePtr CExportBusPoint::GetStopPoint( const long stopID, long& count )
{
	count = 0;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// (バス停ポイント)OBJECTID = (ルート探索用ポイントor停車順)バス停ポイントID
	strWhere.Format( _T("OBJECTID = %d"), stopID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	// 個数カウント
	m_ipBusStopPointFC->FeatureCount( ipFilter, &count );

	// 1個以外はNULLを返す
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

// バス停ポイントに対応するルート探索用ポイントを取得
IFeaturePtr CExportBusPoint::GetRoutePoint( const long oID, long& count )
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
