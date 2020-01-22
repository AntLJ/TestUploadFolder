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

// ImportBusPoint.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ImportBusPoint.h"
#include "BusPointCommon/Time.h"
#include <WinLib/VersionInfo.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::bus;

namespace 
{
	// スキーマで対応できないフィールドの定義
	const char * const k_line_idx         = "LINEIDX";
	const char * const k_name_remarks     = "NAMEREMARKS";
	const char * const k_day_time_remarks = "DAYTIMEREMARKS";
	const char * const k_dms_lat          = "DMSLAT";
	const char * const k_dms_lon          = "DMSLON";

	crd_cnv g_crd;
}

// 各種処理を実行するために最初に呼ばれる関数
bool CImportBusPoint::execute()
{
	// 各種初期化を実施
	if( !Init() ){
		return false;
	}

	// インポート結果フラグ
	bool impOK = true;

	if( m_import ){
	
		// インポート用コンテナ
		BUSPOINTMAP  busStopPoints;
		BUSMASTERMAP busLineMasters;
		BUSSEQVEC    busStopSeqs; 

		// インポート先DB情報出力
		m_ofsImp << "#DB_BUS：" << CT2CA(m_strBusDB) << endl;

		// 既存の系統番号を全て取得する
		GetExistRouteNumber();

		// 既存の停留所コードと標柱番号のペアを全て取得する
		GetExistStopAndPole();

		// 調査結果ファイル読み込み
		if( !LoadExcelData( busStopPoints, busLineMasters, busStopSeqs )){
			m_ofsImp << "#ERROR インポート処理失敗" << endl;
			return false;
		}

		// インポート処理
		if( ImportBusPoint( busStopPoints, busLineMasters, busStopSeqs ) ){
			m_ofsImp << "#インポート処理成功" << endl;
		}
		else{
			impOK = false;
			m_ofsImp << "#ERROR インポート処理失敗" << endl;
		}
	}

	if( m_autoRPoint ){

		if( impOK ){

			// 作成先・参照先DB情報出力
			m_ofsARP << "#DB_BUS：" << CT2CA(m_strBusDB) << endl;
			m_ofsARP << "#DB_ROAD：" << CT2CA(m_strRoadDB) << endl;

			// 自由形式ヘッダ出力
			m_ofsARP << "#FREESTYLELOG" << endl;
			m_ofsARP << "LAYER\tOBJECTID\tSTOP_ID\tPOLE_NO\tSTOP_NAME_KANJI\tSTOP_NAME_YOMI\tCOMMENT" << endl;

			// ルート探索用ポイント作成
			CreateRPoints();
		}
		else{
			// インポート処理がNGだった場合は、ログにその旨を記載するのみとする
			m_ofsARP << "#インポートモードでエラーがあったため、ルート探索用ポイントモードは実施しません" << endl;
		}

	}

	return true;
}

// 各種初期化
bool CImportBusPoint::Init()
{
	// 調査結果ファイルパスリストを読み込み、ファイルパスをvectorに格納
	if( m_import && !LoadList()){
		cerr << "#ERROR 調査結果ファイルリストの読み込みに失敗：" << CT2CA(m_strInFile) << endl;
		return false;
	}

	// インポートログファイルオープン
	if( m_import ){
		m_ofsImp.open( m_strImpLog );
		if( !m_ofsImp ){
			cerr << "#ERROR インポートログファイルオープンに失敗：" << CT2CA(m_strImpLog) << endl;
			return false;
		}
	}

	// ルート探索用Pログファイルオープン
	if( m_autoRPoint ){
		m_ofsARP.open( m_strARPLog );
		if( !m_ofsARP ){
			cerr << "#ERROR インポートログファイルオープンに失敗：" << CT2CA(m_strARPLog) << endl;
			return false;
		}
	}

	// インポート処理 or ルート探索用P処理
	if( m_import || m_autoRPoint ){
	
		// バス関連ワークスペースオープン
		if( ! DB::OpenWorkspace( m_strBusDB, m_ipBusWorkspace ) )
			return false;

		// バス停ポイントフィーチャクラスオープン＆フィールドマップ作成
		if( ! DB::OpenFeatureClass( bus_stop_point::kTableName, m_ipBusWorkspace, m_ipBusStopPointFC, m_busStopPointFM ) )
			return false;

		// インポート処理
		if( m_import ){
			// バス路線マスタテーブルオープン＆フィールドマップ作成
			if( ! DB::OpenTable( bus_line_master::kTableName, (IWorkspacePtr)m_ipBusWorkspace, m_ipBusLineMasterTable, m_busLineMasterFM ) )
				return false;
			
			// バス停車順管理テーブルオープン＆フィールドマップ作成
			if( ! DB::OpenTable( bus_stop_seq::kTableName, (IWorkspacePtr)m_ipBusWorkspace, m_ipBusStopSeqTable, m_busStopSeqFM ) )
				return false;

			// 事業者ファイル列名をリストに格納
			SetColumnName();
		}

		// ルート探索用ポイント処理
		if( m_autoRPoint ){
			// バスルート探索用ポイントフィーチャクラスオープン＆フィールドマップ作成
			if( ! DB::OpenFeatureClass( bus_route_point::kTableName, m_ipBusWorkspace, m_ipBusRoutePointFC, m_busRoutePointFM ) )
				return false;
		}
	}

	if( m_autoRPoint ){
		// 道路NWワークスペースオープン
		if( ! DB::OpenWorkspace( m_strRoadDB, m_ipRoadWorkspace ) )
			return false;

		// 道路NW(ROAD_LINK)フィーチャクラスオープン
		CFieldMap roadLinkFM;
		if( ! DB::OpenFeatureClass( road_link::kTableName, m_ipRoadWorkspace, m_ipRoadLinkFC, roadLinkFM ) )
			return false;
	}
	return true;
}

// 列名をリストに格納
void CImportBusPoint::SetColumnName()
{
	m_columnNameList.push_back( bus_stop_point::kPointAttrCode ); //!< 種別番号
	m_columnNameList.push_back( bus_stop_seq::kStopSeq );         //!< 順序番号
	m_columnNameList.push_back( bus_stop_point::kStopID );        //!< 停留所コード
	m_columnNameList.push_back(  bus_stop_point::kName );         //!< 停留所名
	m_columnNameList.push_back( bus_stop_point::kYomi );          //!< 停留所名よみがな
	m_columnNameList.push_back( bus_stop_point::kChangedName );   //!< 変更後停留所名
	m_columnNameList.push_back( bus_stop_point::kChangedYomi );   //!< 変更後停留所名よみがな
	m_columnNameList.push_back( k_name_remarks );                 //!< 名称変更備考
	m_columnNameList.push_back( bus_stop_point::kPoleNo );        //!< 標柱番号
	m_columnNameList.push_back( bus_stop_point::kGettingOffF );   //!< 降車専用
	m_columnNameList.push_back( bus_line_master::kRouteNumber );  //!< 系統番号
	m_columnNameList.push_back( bus_line_master::kRouteName );    //!< 系統名
	m_columnNameList.push_back( bus_line_master::kRouteDetail );  //!< 経路
	m_columnNameList.push_back( bus_line_master::kRouteRemarks ); //!< 経由地等備考
	m_columnNameList.push_back( bus_stop_seq::kDayCodes );        //!< 経路曜日
	m_columnNameList.push_back( bus_stop_seq::kTimeCodes );       //!< 経路時間帯
	m_columnNameList.push_back( k_day_time_remarks );             //!< 曜日時間備考
	m_columnNameList.push_back( k_dms_lat );                      //!< DMS緯度
	m_columnNameList.push_back( k_dms_lon );                      //!< DMS経度
}

// 調査結果ファイルリストを読み込み、ファイルパス1行ずつをリストに格納
bool CImportBusPoint::LoadList()
{
	ifstream ifs( m_strInFile );
	if( !ifs ){
		return false;
	}
	copy( std::istream_iterator<string>(ifs), 
				std::istream_iterator<string>(), 
				std::back_inserter(m_fileList) );

	return ifs.eof();
}

// BUS_LINE_MASTERから既存の系統番号（ROUTE_NUMBER）を全て取得する
void CImportBusPoint::GetExistRouteNumber()
{
	// SubFieldsセット
	CString subStr;
	subStr.Format( _T("%s"), bus_line_master::kRouteNumber );

	// サーチ
	_ICursorPtr ipCursor = m_ipBusLineMasterTable->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	_IRowPtr    ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){
		
		// 系統番号を取得し、格納する
		m_existRouteNumbers.insert( ipRow->GetValue( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteNumber ) ).lVal );
	}
}

// BUS_STOP_POINTから既存の停留所コードと標柱番号のペアを全て取得する
void CImportBusPoint::GetExistStopAndPole()
{
	// SubFieldsセット
	CString subStr;
	subStr.Format( _T("%s,%s"), bus_stop_point::kStopID, bus_stop_point::kPoleNo );

	// サーチ
	IFeatureCursorPtr ipCursor = m_ipBusStopPointFC->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	IFeaturePtr       ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){
		
		// 停留所コードと標柱番号取得
		long stopID = ipFeature->GetValue( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ) ).lVal;
		long poleNo = ipFeature->GetValue( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ) ).lVal;

		// ペアでセットに格納
		m_existStopPole.insert( make_pair( stopID, poleNo ) );
	}
}

// 調査結果ファイルから、インポート用リスト作成
bool CImportBusPoint::LoadExcelData( BUSPOINTMAP& busStopPoints, BUSMASTERMAP& busLineMasters, BUSSEQVEC& busStopSeqs )
{
	try{
		using namespace Excel;

		// 調査結果ファイルごとの処理
		for( auto& strFile : m_fileList ){

			BUSSEQVEC fileInputs;

			cerr << "調査結果ファイルオープン中 ... ";
			CExcelHelper cExcel;

			if( !cExcel.OpenReadOnly( (CString)(strFile.c_str()) )){

				cerr << "失敗" << endl;
				m_ofsImp << "#ERROR 調査結果ファイルのオープンに失敗：" << strFile << endl;
				continue;
			}
			cerr << "成功" << endl;

			// ファイル名
			CString companyName = cExcel.GetFileName().Left( cExcel.GetFileName().ReverseFind('.') );

			// 調査結果ファイル内のシートごとの処理（1枚目のシートはNum=1）
			for( int sheetNum = 1 ; sheetNum <= cExcel.GetSheetCnt()  ; ++sheetNum ){
				_WorksheetPtr ipWorkSheet = cExcel.GetSheet( sheetNum );
				if( !ipWorkSheet ){
					continue;
				}
				CString sheetName = (LPCTSTR)(ipWorkSheet->Name);
				BUSSEQVEC sheetInputs; //!< シートの一次格納用
				set<long> routeNums;   //!< 系統番号確認用
				
				// 調査結果ファイル内の1行ごとの処理（2行目からデータ行：Index=2）
				for( int lineIdx = 2 ; ; ++lineIdx ){

					CString strStartKey;

					strStartKey.Format( _T("A%d"), lineIdx );

					// [A?セル]に何か入っていればレコードがあると判断(空なら情報はここまで)
					CComVariant vaData = ipWorkSheet->Range[CComVariant(strStartKey)]->Value2;
					if( VT_EMPTY == vaData.vt || vaData.vt == VT_NULL ){
						break;
					}

					// 1行ずつシート内の情報を取得
					BUSCONTAINER busInput;
					if( !GetLineFromSheet( ipWorkSheet, lineIdx, busInput ) ){

						// 読み込み失敗した行は、エラー出力＆スキップ
						m_ofsImp << "#ERROR " << lineIdx << " 行目の読み込みに失敗：" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
						continue;
					}
					
					// 系統番号確認用にsetに格納しておく
					routeNums.insert( _ttol( busInput.at( bus_line_master::kRouteNumber ) ) );

					// 1行を1レコードとして、格納
					sheetInputs.push_back( busInput );
				}
				
				// 系統番号に揺れがあったらインポートしない（ないはずだけど）
				if( routeNums.size() != 1 ){
					m_ofsImp << "#ERROR シート内で系統番号が2つ以上存在するため、インポートしない：" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
					continue;
				}

				// 既にBUS_LINE_MASTERに存在する系統番号か確認する
				auto routeNum = *(routeNums.begin());
				if( m_existRouteNumbers.end() != m_existRouteNumbers.find( routeNum ) ){
					auto routeName =  sheetInputs.begin()->at( bus_line_master::kRouteName );
					m_ofsImp << "#INFO " << routeNum << "(" << CT2CA( routeName ) << ") は既にBUS_LINE_MASTERに存在するため、インポートしない：" << strFile << "([" << CT2CA(sheetName) << "])" << endl;
					continue;
				}

				// 問題なければインポート用のコンテナに格納する
				fileInputs.insert( fileInputs.end(), sheetInputs.begin(), sheetInputs.end() );


			}	// シートごとの処理ここまで

			// ファイルごとにインポート用リストに格納する（BUS_LINE_MASTERでファイル名(事業者名)が必要なため）
			for( auto& rec : fileInputs ){

				// レコードごとの処理

				// 停留所コード＆標柱番号のユニークを取って、バス停ポイント(BUS_STOP_POINT)へのインポートリストに格納
				BUSCONTAINER busStopPoint;
				if( SetBusStopPoint( rec, busStopPoint ) ){
					busStopPoints.insert( make_pair( make_pair( rec.at( bus_stop_point::kStopID ), rec.at( bus_stop_point::kPoleNo )), busStopPoint ));
				}

				// 系統番号＆経路のユニークを取って、BUS_LINE_MASTER(バス路線マスタ)へのインポートリストに格納
				BUSCONTAINER busLineMaster;
				if( SetBusLineMaster( rec, companyName, busLineMaster )){
					busLineMasters.insert( make_pair( make_pair( rec.at( bus_line_master::kRouteNumber ), rec.at( bus_line_master::kRouteDetail )), busLineMaster ));
				}
				// 全ての調査結果をBUS_STOP_SEQ(バス停車順管理)へのインポートリストに格納
				BUSCONTAINER busStopSeq;
				if( SetBusStopSeq( rec, busStopSeq ) ){
					busStopSeqs.push_back( busStopSeq );
				}
			}
		
			cExcel.Close();

		}	// ファイルごとの処理ここまで
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR 調査結果ファイルから、インポート用リスト作成時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// 調査結果ファイルの1行から、コンテナに読み込み
bool CImportBusPoint::GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, const long lineIdx, BUSCONTAINER& busInput )
{
	CString strStartKey, strEndKey;

	strStartKey.Format( _T("A%d"), lineIdx );
	strEndKey.Format( _T("S%d"), lineIdx );

	CComSafeArray<VARIANT> sa;
	long idx = 0;

	try
	{
		// 1行まとめて読み込んだ後、後で分解する
		CComVariant vaRecord = ipWorkSheet->Range[CComVariant(strStartKey)][CComVariant(strEndKey)]->Value2;
		if( VT_NULL == vaRecord.vt ){
			return false;
		}
		sa.Attach(vaRecord.parray);

		for( auto& columnName : m_columnNameList ){

			// 1セルずつ取り込み(座標値はDMSの文字列のため、数値列は全て整数で良い
			busInput.insert( make_pair( columnName, GetCellStr( sa, ++idx )));
		}

		sa.Detach();
	}
	catch( ... )
	{
		sa.Detach();
		return false;
	}

	return true;
}

// バス停ポイントインポート用リストへの値セット
bool CImportBusPoint::SetBusStopPoint( const BUSCONTAINER& busInput, BUSCONTAINER& busStopPoint )
{
	try{
		busStopPoint.insert( make_pair( bus_stop_point::kStopID, busInput.at( bus_stop_point::kStopID )));

		// インプットされた調査結果種別が「2」の場合：インポートの際は「1」に変更するため、ここで値変更
		// インプットされた調査結果種別が「2」以外　：インプットされた調査結果種別のまま
		busStopPoint.insert( make_pair( bus_stop_point::kPointAttrCode, 
			busInput.at( bus_stop_point::kPointAttrCode ) == _T("2")? _T("1") : busInput.at( bus_stop_point::kPointAttrCode )));

		busStopPoint.insert( make_pair( bus_stop_point::kName, busInput.at( bus_stop_point::kName )));
		busStopPoint.insert( make_pair( bus_stop_point::kYomi, busInput.at( bus_stop_point::kYomi )));
		busStopPoint.insert( make_pair( bus_stop_point::kChangedName, busInput.at( bus_stop_point::kChangedName )));
		busStopPoint.insert( make_pair( bus_stop_point::kChangedYomi, busInput.at( bus_stop_point::kChangedYomi )));
		busStopPoint.insert( make_pair( bus_stop_point::kRemarks, busInput.at( k_name_remarks )));
		busStopPoint.insert( make_pair( bus_stop_point::kPoleNo, busInput.at( bus_stop_point::kPoleNo )));
		busStopPoint.insert( make_pair( bus_stop_point::kGettingOffF, busInput.at( bus_stop_point::kGettingOffF )));
		busStopPoint.insert( make_pair( k_dms_lat, busInput.at( k_dms_lat )));
		busStopPoint.insert( make_pair( k_dms_lon, busInput.at( k_dms_lon )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス停ポイントインポート用リスト作成時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス路線マスタインポート用リストへの値セット
bool CImportBusPoint::SetBusLineMaster( const BUSCONTAINER& busInput, const CString& companyName, BUSCONTAINER& busLineMaster )
{
	try{
		busLineMaster.insert( make_pair( bus_line_master::kCompany, companyName ));
		busLineMaster.insert( make_pair( bus_line_master::kRouteNumber, busInput.at( bus_line_master::kRouteNumber )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteName, busInput.at( bus_line_master::kRouteName )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteDetail, busInput.at( bus_line_master::kRouteDetail )));
		busLineMaster.insert( make_pair( bus_line_master::kRouteRemarks, busInput.at( bus_line_master::kRouteRemarks )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス路線マスタインポート用リスト作成時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス停車順管理インポート用リストへの値セット
bool CImportBusPoint::SetBusStopSeq( const BUSCONTAINER& busInput, BUSCONTAINER& busStopSeq )
{
	try{
		busStopSeq.insert( make_pair( bus_stop_seq::kStopSeq, busInput.at( bus_stop_seq::kStopSeq )));

		// インプットされた調査結果種別が「2」の場合：インポートの際は「1」に変更するため、ここで値変更
		// インプットされた調査結果種別が「2」以外　：インポートの際は「0」に変更するため、ここで値変更
		busStopSeq.insert( make_pair( bus_stop_seq::kPassF, 
			busInput.at( bus_stop_point::kPointAttrCode ) == _T("2")? _T("1") : _T("0") ));

		busStopSeq.insert( make_pair( bus_stop_seq::kDayCodes,  busInput.at( bus_stop_seq::kDayCodes )));
		busStopSeq.insert( make_pair( bus_stop_seq::kTimeCodes, busInput.at( bus_stop_seq::kTimeCodes )));
		busStopSeq.insert( make_pair( bus_stop_seq::kRemarks,   busInput.at( k_day_time_remarks )));

		// 以下はインポート時の他テーブルからOID取得用
		busStopSeq.insert( make_pair( bus_line_master::kRouteNumber, busInput.at( bus_line_master::kRouteNumber )));
		busStopSeq.insert( make_pair( bus_line_master::kRouteDetail, busInput.at( bus_line_master::kRouteDetail )));
		busStopSeq.insert( make_pair( bus_stop_point::kStopID, busInput.at( bus_stop_point::kStopID )));
		busStopSeq.insert( make_pair( bus_stop_point::kPoleNo, busInput.at( bus_stop_point::kPoleNo )));
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス停車順管理インポート用リスト作成時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// 調査結果からSDEのバス関連テーブルにインポート
bool CImportBusPoint::ImportBusPoint( const BUSPOINTMAP& busStopPoints, const BUSMASTERMAP& busLineMasters, const BUSSEQVEC& busStopSeqs )
{
	bool bError = false;

	// 編集開始
	if( ! DB::StartEdit( m_ipBusWorkspace ) )
		return false;

	// バス停ポイントフィーチャクラス
	IFeatureCursorPtr ipBusStopPointCursor;
	m_ipBusStopPointFC->Insert( true, &ipBusStopPointCursor );

	for( auto& busStopPoint : busStopPoints ){

		long stopID = _ttol(busStopPoint.second.at( bus_stop_point::kStopID ));
		long poleNo = _ttol(busStopPoint.second.at( bus_stop_point::kPoleNo ));

		// 既にBUS_STOP_POINTに存在する停留所コードと標柱番号のペアの場合、追加しない
		if( m_existStopPole.end() != m_existStopPole.find( make_pair( stopID, poleNo ) ) ){
			m_ofsImp << "#INFO [停留所コード：" << stopID << "/標柱番号：" << poleNo << "] は既にBUS_STOP_POINTに存在するため、インポートしない(停車順のみ追加)" << endl;
			continue;
		}
		IFeatureBufferPtr ipBusStopPointBuffer;
		m_ipBusStopPointFC->CreateFeatureBuffer( &ipBusStopPointBuffer );
		if( !SetWriteBusStopPoint( busStopPoint.second, ipBusStopPointBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusStopPointCursor->InsertFeature(ipBusStopPointBuffer, &vIDVariant);

		// 成功したら停留所コードと標柱番号のペアリストに追加しておく
		m_existStopPole.insert( make_pair( stopID, poleNo ) );

	}
	ipBusStopPointCursor->Flush();


	// バス路線マスタテーブル
	_ICursorPtr ipBusLineMasterCursor;
	m_ipBusLineMasterTable->Insert( true, &ipBusLineMasterCursor );

	for( auto& busLineMaster : busLineMasters ){

		IRowBufferPtr ipBusLineMasterBuffer;
		m_ipBusLineMasterTable->CreateRowBuffer( &ipBusLineMasterBuffer );
		if( !SetWriteBusLineMaster( busLineMaster.second, ipBusLineMasterBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusLineMasterCursor->InsertRow(ipBusLineMasterBuffer, &vIDVariant);

		// 成功したら系統番号リストにも追加しておく
		long routeNumber = _ttol(busLineMaster.second.at( bus_line_master::kRouteNumber ));
		m_existRouteNumbers.insert( routeNumber );
	}
	ipBusLineMasterCursor->Flush();


	// バス停車順管理テーブル
	_ICursorPtr ipBusStopSeqCursor;
	m_ipBusStopSeqTable->Insert( true, &ipBusStopSeqCursor );

	for( auto& busStopSeq : busStopSeqs ){

		IRowBufferPtr ipBusStopSeqBuffer;
		m_ipBusStopSeqTable->CreateRowBuffer( &ipBusStopSeqBuffer );
		if( !SetWriteBusStopSeq( busStopSeq, ipBusStopSeqBuffer ) ){
			bError = true;
			continue;
		}
		CComVariant vIDVariant;
		ipBusStopSeqCursor->InsertRow(ipBusStopSeqBuffer, &vIDVariant);
	}
	ipBusStopSeqCursor->Flush();

	// 編集保存＆終了
	if( ! DB::StopEdit( m_ipBusWorkspace ) )
		return false;
	
	return (! bError);
}

// バス停ポイントインポートリストレコードから、形状・属性をFeatureBufferにセット
bool CImportBusPoint::SetWriteBusStopPoint( const BUSCONTAINER& busStopPoint, IFeatureBufferPtr& ipBusStopPointBuffer )
{
	try{
		// N35... , E139... の形でDMS緯度経度を持っているため、crd_cnvのdms型に変換する 
		CString dmsStr = busStopPoint.at( k_dms_lon );
		int first = 0, second = 0;
		first  = dmsStr.Find( _T(".") );
		second = dmsStr.Find( _T("."), first + 1 );
		dms dmsLon;
		dmsLon.deg = _ttoi( dmsStr.Mid( 1, first - 1 ));
		dmsLon.min = _ttoi( dmsStr.Mid( first + 1, second - first - 1 ));
		dmsLon.sec = _ttof( dmsStr.Mid( second + 1 ));

		dmsStr = busStopPoint.at( k_dms_lat );
		first  = dmsStr.Find( _T(".") );
		second = dmsStr.Find( _T("."), first + 1 );
		dms dmsLat;
		dmsLat.deg = _ttoi( dmsStr.Mid( 1, first - 1 ));
		dmsLat.min = _ttoi( dmsStr.Mid( first + 1, second - first - 1 ));
		dmsLat.sec = _ttof( dmsStr.Mid( second + 1 ));


		// DMS緯度経度から10進に変換後、ポイント作成
		WKSPoint point;

		point.X = g_crd.DMStoDEC( dmsLon );
		point.Y = g_crd.DMStoDEC( dmsLat );

		ISpatialReferencePtr ipSpRef;
		((IGeoDatasetPtr)m_ipBusStopPointFC)->get_SpatialReference( &ipSpRef );
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( point.X, point.Y );

		// 形状セット
		if( S_OK != ((IFeatureBufferPtr)ipBusStopPointBuffer)->putref_Shape( ipPoint )){
			m_ofsImp << "#ERROR バス停ポイントへインポートの際、形状セットに失敗（停留所コード：「" 
				<< CT2CA(busStopPoint.at( bus_stop_point::kStopID )) << "」, 標柱番号「" << CT2CA(busStopPoint.at( bus_stop_point::kPoleNo )) << "」）" << endl;
			return false;
		}

		// 属性セット
		if( S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kStopID ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPointAttrCode ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kPointAttrCode ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), CComVariant(busStopPoint.at( bus_stop_point::kName )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kYomi ), CComVariant(busStopPoint.at( bus_stop_point::kYomi )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedName ), CComVariant(busStopPoint.at( bus_stop_point::kChangedName )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kChangedYomi ), CComVariant(busStopPoint.at( bus_stop_point::kChangedYomi )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kRemarks ), CComVariant(busStopPoint.at( bus_stop_point::kRemarks )))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kPoleNo ))))
			|| S_OK != ipBusStopPointBuffer->put_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kGettingOffF ), CComVariant(_ttol(busStopPoint.at( bus_stop_point::kGettingOffF ))))
			){
	
			m_ofsImp << "#ERROR バス停ポイントへインポートの際、属性セットに失敗（停留所コード：「" 
				<< CT2CA(busStopPoint.at( bus_stop_point::kStopID )) << "」, 標柱番号「" << CT2CA(busStopPoint.at( bus_stop_point::kPoleNo )) << "」）" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス停ポイントの形状・属性セット時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス路線マスタテーブルインポートリストレコードから、属性をRowBufferにセット
bool CImportBusPoint::SetWriteBusLineMaster( const BUSCONTAINER& busLineMaster, IRowBufferPtr& ipBusLineMasterBuffer )
{
	try{
		// 属性セット
		if( S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kCompany ), CComVariant(busLineMaster.at( bus_line_master::kCompany )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteNumber ), CComVariant(_ttol(busLineMaster.at( bus_line_master::kRouteNumber ))))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteName ), CComVariant(busLineMaster.at( bus_line_master::kRouteName )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteDetail ), CComVariant(busLineMaster.at( bus_line_master::kRouteDetail )))
			|| S_OK != ipBusLineMasterBuffer->put_Value( m_busLineMasterFM.GetFieldIndex( bus_line_master::kRouteRemarks ), CComVariant(busLineMaster.at( bus_line_master::kRouteRemarks )))
			){
	
			m_ofsImp << "#ERROR バス路線マスタへインポートの際、属性セットに失敗（系統番号：「" 
				<< CT2CA(busLineMaster.at( bus_line_master::kRouteNumber )) << "」, 経路「" << CT2CA(busLineMaster.at( bus_line_master::kRouteDetail )) << "」）" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス路線マスタの属性セット時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス停車順管理テーブルインポートリストレコードから、属性をRowBufferにセット
bool CImportBusPoint::SetWriteBusStopSeq( const BUSCONTAINER& busStopSeq, IRowBufferPtr& ipBusStopSeqBuffer )
{
	try{
		// BUS_LINE_MASTER(バス路線マスタ)から、系統番号と経路が同一のレコードのOIDを取得
		long lineMasterOID = 0;

		IQueryFilterPtr ipFilter(CLSID_QueryFilter);
		CString strWhere;

		strWhere.Format( _T("%s = '%s' and %s = '%s'"), bus_line_master::kRouteNumber, busStopSeq.at( bus_line_master::kRouteNumber ), 
			bus_line_master::kRouteDetail, busStopSeq.at( bus_line_master::kRouteDetail ));
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		_ICursorPtr ipCursor;
		m_ipBusLineMasterTable->Search( ipFilter, false, &ipCursor );
		_IRowPtr	ipRow;
		if( ipCursor && ipCursor->NextRow(&ipRow) == S_OK ){
			ipRow->get_OID( &lineMasterOID );
		}
		else{
			m_ofsImp << "#ERROR バス停車順管理へインポートの際、バス路線マスタからObjectID取得失敗（系統番号：「" 
				<< CT2CA(busStopSeq.at( bus_line_master::kRouteNumber )) << "」, 経路「" << CT2CA(busStopSeq.at( bus_line_master::kRouteDetail )) << "」）" << endl;
			return false; 
		}

		// BUS_STOP_POINT(バス停ポイント)、停留所コードと標柱番号が同一のレコードのOIDを取得
		long stopPointOID = 0;
		IQueryFilterPtr ipFilter2(CLSID_QueryFilter);
		strWhere.Format( _T("%s = '%s' and %s = '%s'"), bus_stop_point::kStopID, busStopSeq.at( bus_stop_point::kStopID ), 
			bus_stop_point::kPoleNo, busStopSeq.at( bus_stop_point::kPoleNo ));
		ipFilter2->put_WhereClause( _bstr_t(strWhere) );

		IFeatureCursorPtr ipCursor2;
		m_ipBusStopPointFC->Search( ipFilter2, false, &ipCursor2 );
		IFeaturePtr	ipFeature;
		if( ipCursor2 && ipCursor2->NextFeature(&ipFeature) == S_OK ){
			ipFeature->get_OID( &stopPointOID );
		}
		else{
			m_ofsImp << "#ERROR バス停車順管理へインポートの際、バス停ポイントからObjectID取得失敗（停留所コード：「" 
				<< CT2CA(busStopSeq.at( bus_stop_point::kStopID )) << "」, 標柱番号「" << CT2CA(busStopSeq.at( bus_stop_point::kPoleNo )) << "」）" << endl;
			return false; 
		}

		// 属性セット
		if( S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusLineMasterID ), CComVariant(lineMasterOID) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kStopSeq ), CComVariant(_ttol(busStopSeq.at( bus_stop_seq::kStopSeq ))))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kBusStopPointID ), CComVariant(stopPointOID) )
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kPassF ), CComVariant(_ttol(busStopSeq.at( bus_stop_seq::kPassF ))))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kDayCodes ), CComVariant(busStopSeq.at( bus_stop_seq::kDayCodes )))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kTimeCodes ), CComVariant(busStopSeq.at( bus_stop_seq::kTimeCodes )))
			|| S_OK != ipBusStopSeqBuffer->put_Value( m_busStopSeqFM.GetFieldIndex( bus_stop_seq::kRemarks ), CComVariant(busStopSeq.at( bus_stop_seq::kRemarks )))
			){
	
			m_ofsImp << "#ERROR バス停車順管理へインポートの際、属性セットに失敗（系統番号：「" << CT2CA(busStopSeq.at( bus_line_master::kRouteNumber )) 
				<< "」, 経路「" << CT2CA(busStopSeq.at( bus_line_master::kRouteDetail )) << "」, 停留所コード「" << CT2CA(busStopSeq.at( bus_stop_point::kStopID )) 
				<< "」, 標柱番号「" << CT2CA(busStopSeq.at( bus_stop_point::kPoleNo )) << "」, 順序番号「" << CT2CA(busStopSeq.at( bus_stop_seq::kStopSeq )) << "」）" << endl;
			return false; 
		}
	}
	catch( out_of_range& e ){
		m_ofsImp << "#ERROR バス停車順管理の属性セット時に例外が発生：" << e.what() << endl;
		return false;
	}
	return true;
}

// バス停ポイントの標柱ポイントに対応するルート探索用ポイントを追加
bool CImportBusPoint::CreateRPoints()
{
	set<list<CString>> busPoints;

	// ルート探索用ポイント追加対象のバス停ポイントObjectIDリストを作成する
	if( !CreateOIDList( busPoints )){
		m_ofsARP << "#ERROR バス停ポイントから、ルート探索用ポイント追加対象の取得に失敗" << endl;
		return false;
	}

	// 編集開始
	if( ! DB::StartEdit( m_ipBusWorkspace ) )
		return false;

	// バスルート探索用ポイント作成処理
	IFeatureCursorPtr ipBusRoutePointCursor;
	m_ipBusRoutePointFC->Insert( true, &ipBusRoutePointCursor );
	
	for( auto& busPoint : busPoints ){

		IFeatureBufferPtr ipBusRoutePointBuffer;
		m_ipBusRoutePointFC->CreateFeatureBuffer( &ipBusRoutePointBuffer );

		if( !SetWriteBusRoutePoint( _ttol(*(busPoint.begin())), ipBusRoutePointBuffer ) ){
			continue;
		}
		// ログ出力
		m_ofsARP << "BUS_STOP_POINT\t";
		for( auto& field : busPoint ){
			m_ofsARP << CT2CA(field) << "\t";
		}
		m_ofsARP << "作成成功" << endl;

		CComVariant vIDVariant;
		ipBusRoutePointCursor->InsertFeature(ipBusRoutePointBuffer, &vIDVariant);
	}
	ipBusRoutePointCursor->Flush();


	// 編集保存＆終了
	if( ! DB::StopEdit( m_ipBusWorkspace ) )
		return false;
	
	return true;
}

// 作成対象のバス停ポイントObjectIDリストを作成する
bool CImportBusPoint::CreateOIDList( set<list<CString>>& busPoints )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// バス停ポイントのバスポイント種別コード = 1（停留所標柱ポイント）
	strWhere.Format( _T("%s = '%d'"), bus_stop_point::kPointAttrCode, bus_stop_point::point_attr_code::kPole );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;

	// まずはバス停ポイントから作成対象を全て取得する
	m_ipBusStopPointFC->Search(ipFilter, VARIANT_FALSE, &ipCursor);

	if( ipCursor ){
		IFeaturePtr ipFeature;

		while( ipCursor->NextFeature( &ipFeature ) == S_OK ){

			list<CString> busPoint;

			// バス停ポイントに対応するルート探索用ポイントがないか先に確認する
			long busPointOID;
			ipFeature->get_OID( &busPointOID );
			if( IsRPointTarget( busPointOID )){
				
				// ObjectIDを格納
				CString oID;
				oID.Format( _T("%d"), busPointOID );
				busPoint.push_back( oID );

				// ログ出力用に作成対象の情報を取得する
				if( !GetBusPointInfo( ipFeature, busPoint ) ){
					return false;
				}

				// ルート探索用ポイントが存在しないものだけリストに追加
				busPoints.insert( busPoint );
			}
		}
	}
	else{
		return false;
	}
	return true;
}

//ルート探索用ポイントの追加対象か調べる
bool CImportBusPoint::IsRPointTarget( const long busPointOID )
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// バス停ルート探索用ポイントから、バス停ポイントOIDを持つものが存在するか
	strWhere.Format( _T("%s = '%d'"), bus_route_point::kBusStopPointID, busPointOID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;

	m_ipBusRoutePointFC->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	IFeaturePtr ipFeature;

	// 見つかった場合は、既に対となるルート探索用ポイントが存在するため、追加対象としない
	if( ipCursor && ipCursor->NextFeature( &ipFeature ) == S_OK ){
		return false;
	}
	else{
		return true;
	}
}

// ルート探索用ポイント作成対象のバス停ポイント情報を取得する
bool CImportBusPoint::GetBusPointInfo( const IFeaturePtr& ipFeature, list<CString>& busPoint )
{
	CComVariant vaValue;
	CString     str;

	// STOP_ID
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kStopID ), &vaValue ) ){
		return false;
	}
	str.Format( _T("%d"), vaValue.lVal );
	busPoint.push_back( str );

	// POLE_NO
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kPoleNo ), &vaValue ) ){
		return false;
	}
	str.Format( _T("%d"), vaValue.lVal );
	busPoint.push_back( str );

	// STOP_NAME_KANJI
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kName ), &vaValue ) ){
		return false;
	}
	str = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
	busPoint.push_back( str );

	// STOP_NAME_YOMI
	if( S_OK != ipFeature->get_Value( m_busStopPointFM.GetFieldIndex( bus_stop_point::kYomi ), &vaValue ) ){
		return false;
	}
	str = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
	busPoint.push_back( str );

	return true;
}

// バス停ポイントのOIDから、バスルート探索用ポイント追加用FeatureBufferに形状・属性をセット
bool CImportBusPoint::SetWriteBusRoutePoint( const long busStopPointOID, IFeatureBufferPtr& ipBusRoutePointBuffer )
{
	IFeaturePtr ipFeature;
	m_ipBusStopPointFC->GetFeature( busStopPointOID, &ipFeature );
	if(ipFeature){
		IPointPtr ipPoint = GetNearestPoint( ipFeature );
		if( ipPoint ){

			// 形状セット
			if( S_OK != ((IFeatureBufferPtr)ipBusRoutePointBuffer)->putref_Shape( ipPoint )){
				m_ofsARP << "#ERROR バスルート探索用ポイントへインポートの際、形状セットに失敗（バス停ポイントOID：「" << busStopPointOID << "」）" << endl;
				return false;
			}

			// 属性セット
			if( S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kOperator ), CComVariant(CComBSTR(_T("sindy"))) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kPurpose ), CComVariant(0L) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kModifyProgName ), CComVariant(CVersion().GetInternalName()) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( ipc_table::kProgModifyDate ), CComVariant(GetCurTime()) )
				|| S_OK != ipBusRoutePointBuffer->put_Value( m_busRoutePointFM.GetFieldIndex( bus_route_point::kBusStopPointID ), CComVariant(busStopPointOID) )
			){
	
				m_ofsARP << "#ERROR バス停ポイントへインポートの際、属性セットに失敗（バス停ポイントOID：「" << busStopPointOID << "」）" << endl;
				return false;
			}
		}
		else{
			// 最近傍リンクを取得できなかった
			m_ofsARP << "#ERROR バス停ポイントから、最近傍リンク上の最短距離の点の取得失敗（OID：「" << busStopPointOID << "」"  << endl;
			return false;
		}
	}
	else{
		// 一回検索済みのOIDのため、普通は出ないエラー
		m_ofsARP << "#ERROR バス停ポイントOIDから、情報取得失敗（OID：「" << busStopPointOID << "」"  << endl;
		return false;
	}
	return true;
}

// 最近傍リンク上の最短距離の点を求める（仕様が同一のため、旧バスPJのロジックのまま）
IPointPtr CImportBusPoint::GetNearestPoint(const IFeaturePtr& ipFeature)
{
	const double METER_PARAM = 0.00001; ///< 度をメートルに変換(日本でのザックリ変換)
	const double BASE_DIST = 10.0 * METER_PARAM;	///< 最近傍リンク探索開始最小半径
	const double DELTA_DIST = 50.0 * METER_PARAM;	///< 最近傍リンク探索時の増分

	// バス停ポイントフィーチャから、ジオメトリ取得
	IGeometryPtr ipGeo;
	ipFeature->get_Shape( &ipGeo );
	IPointPtr ipPoint( ipGeo );

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	((IGeoDatasetPtr)m_ipBusStopPointFC)->get_SpatialReference( &ipSpRef );

	// バス停ポイントジオメトリから、X・Y取得
	double srcX, srcY;
	ipPoint->QueryCoords( &srcX, &srcY );

	// 道路NWから空間検索の準備
	CComBSTR	bstrFieldName;
	m_ipRoadLinkFC->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter;
	double minDist = DBL_MAX;
	double tgtX = 0, tgtY = 0;
	bool found = false;

	// 約10mから50mずつ増やして、最大110m(10,60,110)
	for( int i = 0 ; !found && i < 2 ; ++i ){

		// 正確な距離で無くても良いため、簡易に変換したメートル用の値でBuffer取得
		// 正確に行う場合は、ProjectedCoordinateSystemで測地系指定等を行い、Buffer取得が必要
		IGeometryPtr ipBuf;
		double radius = BASE_DIST + ( i * DELTA_DIST );
		((ITopologicalOperatorPtr)ipGeo)->Buffer( radius, &ipBuf );
		ipBuf->putref_SpatialReference(ipSpRef);

		ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipBuf, bstrFieldName );
		IFeatureCursorPtr ipCursor;

		// 道路リンクから空間検索
		m_ipRoadLinkFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
		if( ipCursor ){
			IFeaturePtr ipTgtFeature;
			while( ipCursor->NextFeature(&ipTgtFeature) == S_OK ){
				IGeometryPtr ipLine;
				ipTgtFeature->get_Shape( &ipLine );
				IPointPtr ipNearestPoint;
				IProximityOperatorPtr ipProximity( ipLine );
				ipProximity->ReturnNearestPoint( ipPoint, esriNoExtension, &ipNearestPoint );
				if( ipNearestPoint ){
					double nearX, nearY;
					ipNearestPoint->QueryCoords( &nearX, &nearY );
					double dist = g_crd.GetDist( srcX, srcY, nearX, nearY );

					// 同一ループ内で一番近いものを採用する
					if( minDist > dist ){
						found = true;
						minDist = dist;
						tgtX = nearX;
						tgtY = nearY;
					}
				}
			}
		}else{
			break;
		}
	}

	// 見つからなかった場合
	if( !found ){
		return NULL;
	}

	IPointPtr ipTgtGeo(CLSID_Point);
	ipTgtGeo->PutCoords( tgtX, tgtY );
	ipTgtGeo->putref_SpatialReference( ipSpRef );
	ipTgtGeo->SnapToSpatialReference();

	return ipTgtGeo;
}

