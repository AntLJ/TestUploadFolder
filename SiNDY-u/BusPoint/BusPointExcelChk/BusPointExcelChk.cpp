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
#include "BusPointExcelChk.h"
#include <TDC/useful_headers/directory_util.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy::schema::bus;

// スキーマで対応できないフィールドの定義
const char * const k_line_idx = "LINEIDX";
const char * const k_name_remarks = "NAMEREMARKS";
const char * const k_day_time_remarks = "DAYTIMEREMARKS";
const char * const k_dms_lat = "DMSLAT";
const char * const k_dms_lon = "DMSLON";

// 各種処理を実行するために最初に呼ばれる関数
bool CBusPointExcelChk::execute()
{
	// 各種初期化を実施
	if( !Init() ){
		return false;
	}

	// 調査結果ファイル読み込み先コンテナ（ファイル名・シート名で階層キー）
	BUSFILEMAP busInputs;

	// 調査結果ファイル読み込み
	LoadExcelData( busInputs );

	// 調査結果ファイルのチェック
	if( !InputCheck( busInputs ) ){
		m_ofsChk << "#ERROR 調査結果ファイルに一つ以上エラーが含まれます。" << endl;
	}

	return true;
}

// 各種初期化
bool CBusPointExcelChk::Init()
{
	// 調査結果ファイルリストを読み込み、vectorに格納
	if( !LoadList() ){
		cerr << "#ERROR 調査結果ファイルリストの読み込みに失敗：" << CT2CA(m_strInFile) << endl;
		return false;
	}

	// チェックログファイルオープン
	m_ofsChk.open( m_strChkLog );
	if( !m_ofsChk ){
		cerr << "#ERROR チェックログファイルオープンに失敗：" << CT2CA(m_strChkLog) << endl;
		return false;
	}


	// 列名をリストに格納
	SetColumnName();

	return true;
}

// 各種初期化
void CBusPointExcelChk::SetColumnName()
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
	m_columnNameList.push_back( k_dms_lat );                      //!< 1/256秒緯度
	m_columnNameList.push_back( k_dms_lon );                      //!< 1/256秒経度
}

// 調査結果ファイルリストを読み込み、ファイルパス1行ずつをリストに格納
bool CBusPointExcelChk::LoadList()
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

// 調査結果ファイルから、レコード情報をファイル名・シート名で階層キーのコンテナに格納
void CBusPointExcelChk::LoadExcelData( BUSFILEMAP& busInputs )
{
	using namespace Excel;

	// 調査結果ファイルごとの処理
	for( auto& strFile : m_fileList ){

		cerr << "調査結果ファイルオープン中 ... ";
		CExcelHelper cExcel;

		if( !cExcel.OpenReadOnly( (CString)(strFile.c_str()) )){

			cerr << "失敗" << endl;
			m_ofsChk << "#ERROR 調査結果ファイルのオープンに失敗：" << strFile << endl;
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

			// シート名
			CString sheetName = (LPCTSTR)(ipWorkSheet->GetName());
			BUSCONTVEC sheetBusInputs;

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
					m_ofsChk << "#ERROR " << lineIdx << " 行目の読み込みに失敗：" << strFile << endl;
					continue;
				}
				
				// ログ出力用に冗長だけど、レコードにシート名の情報も持たせておく
				busInput.insert( make_pair( _T("SHEET_NAME"), sheetName ));

				// 1行を1レコードとして、格納
				sheetBusInputs.push_back( busInput );
			}

			// ファイル名をキーに1シートを1レコードとして、格納
			busInputs[companyName].push_back( make_pair( sheetName, sheetBusInputs ));
	
		}	// シートごとの処理ここまで

		cExcel.Close();

	}	// ファイルごとの処理ここまで
}



// 調査結果ファイルの1行から、コンテナに読み込み
bool CBusPointExcelChk::GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, long lineIdx, BUSCONTAINER& busInput )
{
	CString strStartKey, strEndKey;

	CString strLineIdx;
	strLineIdx.Format( _T("%d"), lineIdx );

	strStartKey.Format( _T("A%d"), lineIdx );
	strEndKey.Format( _T("S%d"), lineIdx );

	CComSafeArray<VARIANT> sa;
	long idx = 0;

	try
	{

		// ログに出力用に何行目という情報もコンテナに持たせておく
		busInput.insert( make_pair( k_line_idx, strLineIdx )); //!< 行数

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

// 調査結果ファイルチェック
bool CBusPointExcelChk::InputCheck( const BUSFILEMAP& busInputs )
{
	bool chkOK= true;

	try{
		// ファイルごとの処理
		for( auto& file : busInputs ){
			// ファイル内チェックで使用するための格納用コンテナ
			map<CCPAIR, BUSCONTVEC>	fileSameStop, fileSameCoord;

			CString fileName = file.first;
			m_ofsChk << "#ファイル：" << CT2CA(fileName) << endl;

			// シートごとの処理
			for( auto& sheet : file.second ){

				// 空シートの場合はスキップ
				if( sheet.second.empty() ){
					continue;
				}

				CString sheetName = sheet.first;
				m_ofsChk << " #シート[" << CT2CA(sheetName) << "]" << endl;

				// 単一レコードチェック
				m_ofsChk << "  #単一レコードチェック" << endl;

				// レコードごとの処理
				for( auto& rec : sheet.second ){
				
					list<CString> lineErrMsgs;
					if( !CheckByLine( rec, lineErrMsgs ) ){
					
						// レコード単位で1つでもエラーがあった場合は、NGとする
						chkOK = false;

						// エラーメッセージの分だけエラー出力
						for( auto& errMsg : lineErrMsgs ){
							m_ofsChk << "   #ERROR " << CT2CA(rec.at(k_line_idx)) << " 行目：" << CT2CA(errMsg) << endl;
						}
						m_ofsChk.flush();
					}

					// 同一停留所でのフィールド不整合チェック用に別コンテナに格納する（停留所コードと標柱番号がキー）
					fileSameStop[make_pair(rec.at(bus_stop_point::kStopID), rec.at(bus_stop_point::kPoleNo))].push_back( rec );
	
					// 同一緯度経度で停留所コードor標柱番号用に別コンテナに格納する（緯度と経度がキー）
					fileSameCoord[make_pair(rec.at(k_dms_lat), rec.at(k_dms_lon))].push_back( rec );

				} // レコードごとの処理ここまで

				// シート内チェック
				m_ofsChk << "  #シート内チェック" << endl;

				list<CString> sheetErrMsgs;
				if( !CheckBySheet( sheet, sheetErrMsgs ) ){
	
					// シート単位で1つでもエラーがあった場合は、NGとする
					chkOK = false;

					// エラーメッセージの分だけエラー出力
					for( auto& errMsg : sheetErrMsgs ){
						m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
					}
					m_ofsChk.flush();
				}
			} // シートごとの処理ここまで
			

			// ファイル内チェック（同一停留所でのフィールド不整合チェック）
			m_ofsChk << " #ファイル内チェック" << endl;

			list<CString> fileErrMsgs;
			if( !CheckByFileSameStop( fileSameStop, fileErrMsgs ) ){
	
				// ファイル単位で1つでもエラーがあった場合は、NGとする
				chkOK = false;

				// エラーメッセージの分だけエラー出力
				for( auto& errMsg : fileErrMsgs ){
					m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
				}
				m_ofsChk.flush();
			}

			// ファイル内チェック（同一緯度経度で停留所コードor標柱番号チェック用）
			list<CString> fileErrMsgs2;
			if( !CheckByFileSameCoord( fileSameCoord, fileErrMsgs2 ) ){
	
				// ファイル単位で1つでもエラーがあった場合は、NGとする
				chkOK = false;

				// エラーメッセージの分だけエラー出力
				for( auto& errMsg : fileErrMsgs2 ){
					m_ofsChk << "   #ERROR " <<  CT2CA(errMsg) << endl;
				}
				m_ofsChk.flush();
			}

		} // ファイルごとの処理ここまで

		m_ofsChk.flush();
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR Excelファイルチェック中に未登録キーの例外が発生：" << e.what() <<endl;
		return false;
	}

	return chkOK;
}

// 単一レコード内チェック
bool CBusPointExcelChk::CheckByLine( const BUSCONTAINER& busInput, list<CString>& errMsgs )
{
	try{
		CString errMsg;

		// -- 【NULL不許可フィールド】ここから --
		// 標柱番号
		if( busInput.at(bus_stop_point::kPoleNo).IsEmpty() ){
			errMsg.Format( _T("[標柱番号]がNULL") );
			errMsgs.push_back( errMsg );
		}
		// 経路
		if( busInput.at(bus_line_master::kRouteDetail).IsEmpty() ){
			errMsg.Format( _T("[経路]がNULL") );
			errMsgs.push_back( errMsg );
		}
		// 系統番号
		if( busInput.at(bus_line_master::kRouteNumber).IsEmpty() ){
			errMsg.Format( _T("[系統番号]がNULL") );
			errMsgs.push_back( errMsg );
		}
		// 緯度
		if( busInput.at(k_dms_lat).IsEmpty() || _T("N") != busInput.at(k_dms_lat).Left(1) ){
			errMsg.Format( _T("[緯度]がNULL or フォーマット違反") );
			errMsgs.push_back( errMsg );
		}
		// 経度
		if( busInput.at(k_dms_lon).IsEmpty() || _T("E") != busInput.at(k_dms_lon).Left(1) ){
			errMsg.Format( _T("[経度]がNULL or フォーマット違反") );
			errMsgs.push_back( errMsg );
		}
		// -- 【NULL不許可フィールド】ここまで --


		// -- 【指定サイズオーバー】ここから --
		// 順序番号（5桁）
		if( 5 < busInput.at(bus_stop_seq::kStopSeq).GetLength() ){
			errMsg.Format( _T("[順序番号]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 停留所コード（7桁）
		if( 7 < busInput.at(bus_stop_point::kStopID).GetLength() ){
			errMsg.Format( _T("[停留所コード]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 停留所名（200バイト）
		if( 200 < CStringA(busInput.at(bus_stop_point::kName)).GetLength() ){
			errMsg.Format( _T("[停留所名]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 停留所よみがな（200バイト）
		if( 200 < CStringA(busInput.at(bus_stop_point::kYomi)).GetLength() ){
			errMsg.Format( _T("[停留所よみがな]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 変更後停留所名（200バイト）
		if( 200 < CStringA(busInput.at(bus_stop_point::kChangedName)).GetLength() ){
			errMsg.Format( _T("[変更後停留所名]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 変更後停留所よみがな（200バイト）
		if( 200 < CStringA(busInput.at(bus_stop_point::kChangedYomi)).GetLength() ){
			errMsg.Format( _T("[変更後停留所名よみがな]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 名称変更備考（2000バイト）
		if( 2000 < CStringA(busInput.at(k_name_remarks)).GetLength() ){
			errMsg.Format( _T("[名称変更備考]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 標柱番号（2桁）
		if( 2 < busInput.at(bus_stop_point::kPoleNo).GetLength() ){
			errMsg.Format( _T("[標柱番号]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 系統番号（7桁）
		if( 7 < busInput.at(bus_line_master::kRouteNumber).GetLength() ){
			errMsg.Format( _T("[系統番号]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 系統名（200バイト）
		if( 200 < CStringA(busInput.at(bus_line_master::kRouteName)).GetLength() ){
			errMsg.Format( _T("[系統名]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 経路（200バイト）
		if( 200 < CStringA(busInput.at(bus_line_master::kRouteDetail)).GetLength() ){
			errMsg.Format( _T("[経路]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 経由（経由地等備考）（2000バイト）
		if( 2000 < CStringA(busInput.at(bus_line_master::kRouteRemarks)).GetLength() ){
			errMsg.Format( _T("[経由（経由地等備考）]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// 曜日時間備考（2000バイト）
		if( 2000 < CStringA(busInput.at(k_day_time_remarks)).GetLength() ){
			errMsg.Format( _T("[曜日時間備考]のサイズがオーバー") );
			errMsgs.push_back( errMsg );
		}
		// -- 【指定サイズオーバー】ここまで --


		// -- 【半角数字以外の文字がある】ここから --
		boost::wregex	regOnlyNum(L"^[0-9]+$");	// 半角数字のみの正規表現
		// 順序番号
		if( !busInput.at(bus_stop_seq::kStopSeq).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kStopSeq), regOnlyNum )){
			errMsg.Format( _T("[順序番号]に半角数字以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 停留所コード
		if( !busInput.at(bus_stop_point::kStopID).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_point::kStopID), regOnlyNum )){
			errMsg.Format( _T("[停留所コード]に半角数字以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 標柱番号
		if( !busInput.at(bus_stop_point::kPoleNo).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_point::kPoleNo), regOnlyNum )){
			errMsg.Format( _T("[標柱番号]に半角数字以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 系統番号
		if( !busInput.at(bus_line_master::kRouteNumber).IsEmpty() && !IsOKRegEx( busInput.at(bus_line_master::kRouteNumber), regOnlyNum )){
			errMsg.Format( _T("[系統番号]に半角数字以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// -- 【半角数字以外の文字がある】ここまで --


		// -- 【指定された数字と記号以外の文字がある】ここから --
		// 種別番号（0 or 1）
		if( !busInput.at(bus_stop_point::kPointAttrCode).IsEmpty() && ( _T("0") != busInput.at(bus_stop_point::kPointAttrCode) && _T("1") != busInput.at(bus_stop_point::kPointAttrCode) )){
			errMsg.Format( _T("[種別番号]に 0 or 1 以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 降車専用（1）
		if( !busInput.at(bus_stop_point::kGettingOffF).IsEmpty() && _T("1") != busInput.at(bus_stop_point::kGettingOffF) ){
			errMsg.Format( _T("[降車専用]に 1 以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 経路曜日（半角数字 or 半角セミコロン）
		boost::wregex	regOnlyNumScolon(L"^[0-9;]+$");	// 半角数字+半角セミコロンのみの正規表現
		if( !busInput.at(bus_stop_seq::kDayCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kDayCodes), regOnlyNumScolon )){
			errMsg.Format( _T("[経路曜日]に半角数字と半角セミコロン以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// 経路時間帯（半角数字 or 半角コロン or 半角セミコロン or 半角ハイフン）
		boost::wregex	regOnlyNumColonScolon(L"^[0-9;:\-]+$");	// 半角数字+半角セミコロン+半角コロン+半角ハイフンのみの正規表現
		if( !busInput.at(bus_stop_seq::kTimeCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kTimeCodes), regOnlyNumColonScolon )){
			errMsg.Format( _T("[経路時間帯]に半角数字,半角コロン,半角セミコロン,半角スペース以外の文字がある") );
			errMsgs.push_back( errMsg );
		}
		// -- 【指定された数字と記号以外の文字がある】ここまで --
	

		// -- 【その他単一レコードチェック】ここから --
		// 経路曜日と経路時間帯の条件数が異なる
		boost::wregex	regOnlyScolon(L";");
		if( GetRegExCnt( busInput.at(bus_stop_seq::kDayCodes), regOnlyScolon ) != GetRegExCnt( busInput.at(bus_stop_seq::kTimeCodes), regOnlyScolon )){
			errMsg.Format( _T("[経路曜日]と[経路時間帯]の条件数が異なる") );
			errMsgs.push_back( errMsg );
		}
		// 経路時間帯として、あり得ない時刻がある（4:00～27:59）
		// [bug 10474] 許容値を4:00～27:59 に変更
		boost::wregex	regOKTime(L"^(( |;)*(([4-9]|1[0-9]|2[0-7]):[0-5][0-9]-([4-9]|1[0-9]|2[0-7]):[0-5][0-9])*( |;)*)*$");	// [4:00～27:59]-[4:00～27:59]と ;（複数区切り）
		if( !busInput.at(bus_stop_seq::kTimeCodes).IsEmpty() && !IsOKRegEx( busInput.at(bus_stop_seq::kTimeCodes), regOKTime )){
			errMsg.Format( _T("[経路時間帯]として、あり得ない時刻となっている") );
			errMsgs.push_back( errMsg );
		}
		// 経路曜日が99(その他)なのに、経路曜日備考に記載がない
		if( _T("99") == busInput.at(bus_stop_seq::kDayCodes) && busInput.at(k_day_time_remarks).IsEmpty() ){
			errMsg.Format( _T("[経路曜日]が99(その他)なのに、[曜日時間備考]に記載がない") );
			errMsgs.push_back( errMsg );
		}
		// (変更後停留所名/変更後停留所名よみがな/名称変更備考）のどこか1つもしくは2つが空欄
		if( !(( busInput.at(bus_stop_point::kChangedName).IsEmpty() == busInput.at(bus_stop_point::kChangedYomi).IsEmpty() ) && ( busInput.at(bus_stop_point::kChangedYomi).IsEmpty() == busInput.at(k_name_remarks).IsEmpty() ))){
			errMsg.Format( _T("[変更後停留所名],[変更後停留所名よみがな],[名称変更備考]どこか1つもしくは2つが空") );
			errMsgs.push_back( errMsg );
		}
		// -- 【その他単一レコードチェック】ここまで --


		if( errMsgs.empty() ){
			return true;
		}
		else{
			return false;
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR 単体レコードチェック中に未登録キーの例外が発生：" << e.what() << endl;
		return false;
	}
}

// シート内チェック
bool CBusPointExcelChk::CheckBySheet( const BUSSHEETPAIR& sBusInputs, list<CString>& errMsgs )
{
	try{
		map<pair<CCPAIR, CString>, set<long>> seqNumMap; //!< 「同一の系統、経路で、順序番号に欠番(飛び)がある」確認用
		map<CCPAIRPAIR, set<CString>>         stopIDMap; //!< 「同一の系統、経路、順序番号で、停留所コードが異なる」確認用
		map<CString, set<CString>>            routeMap;  //!< 「系統番号が同一で系統名が異なる」確認用
	
		CString errMsg;

		for( auto& chk : sBusInputs.second ){
			// ペア(ペア(系統番号, 系統名), 経路)をキーに、順序番号を格納
			seqNumMap[make_pair( make_pair( chk.at(bus_line_master::kRouteNumber), chk.at(bus_line_master::kRouteName) ), chk.at(bus_line_master::kRouteDetail))].insert( _ttol( chk.at(bus_stop_seq::kStopSeq) ));

			// ペア(ペア(系統番号, 系統名), ペア(経路, 順序番号))をキーに、停留所コードを格納
			stopIDMap[make_pair( make_pair( chk.at(bus_line_master::kRouteNumber), chk.at(bus_line_master::kRouteName) ), make_pair( chk.at(bus_line_master::kRouteDetail), chk.at(bus_stop_seq::kStopSeq) ))].insert( chk.at(bus_stop_point::kStopID) );

			// 系統番号をキーに、系統名を格納
			routeMap[chk.at(bus_line_master::kRouteNumber)].insert( chk.at(bus_line_master::kRouteName) );
		}

		// 同一の系統、経路で、順序番号に欠番(飛び)がないかチェック
		for( auto& seqNums : seqNumMap ){

			// 空の場合スキップ
			if( seqNums.second.empty() ){
				continue;
			}

			for( int i = 1 ; i < *(seqNums.second.rbegin()) ; ++i ){

				// 1～最大値まで、シーケンスが存在するか確認する
				if( seqNums.second.end() == seqNums.second.find( i ) ){

					// 同一の系統、経路で、順序番号に欠番(飛び)がある
					errMsg.Format( _T("[系統番号(%s)],[系統名(%s)],[経路(%s)]で、[順序番号]に欠番(飛び)がある（「%d」）"), 
						seqNums.first.first.first, seqNums.first.first.second, seqNums.first.second, i );
					errMsgs.push_back( errMsg );
				}
			}
		}

		// 同一の系統、経路、順序番号で、停留所コードが異ならないかチェック
		for( auto& stopIDs : stopIDMap ){

			if( 1 < stopIDs.second.size() ){

				// 2つ以上格納されている場合は、エラー
				for( auto& stopID : stopIDs.second ){

					// 同一の系統、経路、順序番号で、停留所コードが異なる
					errMsg.Format( _T("[系統番号(%s)],[系統名(%s)],[経路(%s)],[順序番号(%s)]が同一で、[停留所コード]が異なる（「%s」）"), 
						stopIDs.first.first.first, stopIDs.first.first.second, stopIDs.first.second.first, stopIDs.first.second.second, stopID );
					errMsgs.push_back( errMsg );
				}
			}
		}

		// 系統番号が同一で系統名が異ならないかチェック
		for( auto& routes : routeMap ){
	
			if( 1 < routes.second.size() ){

				// 2つ以上格納されている場合は、エラー
				for( auto& route : routes.second ){

					// 系統番号が同一で系統名が異なる
					errMsg.Format( _T("[系統番号(%s)]が同一で、[系統名]が異なる（「%s」）"), 
						routes.first, route );
					errMsgs.push_back( errMsg );
				}
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR シート内チェック中に未登録キーの例外が発生：" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}

// ファイル内チェック（同一停留所でのフィールド不整合）
bool CBusPointExcelChk::CheckByFileSameStop( const map<CCPAIR, BUSCONTVEC>& sBusInputs, list<CString>& errMsgs )
{
	try{
		CString errMsg;
	
		for( auto& sameStop : sBusInputs ){
		
			CString strStopKanji    = sameStop.second.begin()->at(bus_stop_point::kName);
			CString strStopYomi     = sameStop.second.begin()->at(bus_stop_point::kYomi);
			CString strChgStopKanji = sameStop.second.begin()->at(bus_stop_point::kChangedName);
			CString strChgStopYomi  = sameStop.second.begin()->at(bus_stop_point::kChangedYomi);
			CString strNameRemarks  = sameStop.second.begin()->at(k_name_remarks);
			CString strGetOffOnly   = sameStop.second.begin()->at(bus_stop_point::kGettingOffF);
			CString strLat          = sameStop.second.begin()->at(k_dms_lat);
			CString strLon          = sameStop.second.begin()->at(k_dms_lon);
			CString strSheetName    = sameStop.second.begin()->at(_T("SHEET_NAME"));

			for( auto& chk : sameStop.second ){

				// 停留所コードと標柱番号が同じ停留所で、
				// 停留所名/停留所名よみがな/変更後停留所名/変更後停留所名よみがな/名称変更備考/降車専用/緯度/経度
				// のいずれかが異なる

				// 停留所名
				if( strStopKanji != chk.at(bus_stop_point::kName) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[停留所名]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strStopKanji, strSheetName,
						chk.at(bus_stop_point::kName), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 停留所名よみがな
				if( strStopYomi != chk.at(bus_stop_point::kYomi) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[停留所名よみがな]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strStopYomi, strSheetName,
						chk.at(bus_stop_point::kYomi), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 変更後停留所名
				if( strChgStopKanji != chk.at(bus_stop_point::kChangedName) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[変更後停留所名]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strChgStopKanji, strSheetName,
						chk.at(bus_stop_point::kChangedName), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 変更後停留所名よみがな
				if( strChgStopYomi != chk.at(bus_stop_point::kChangedYomi) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[変更後停留所名よみがな]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strChgStopYomi, strSheetName,
						chk.at(bus_stop_point::kChangedYomi), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 名称変更備考
				if( strNameRemarks != chk.at(k_name_remarks) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[名称変更備考]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strNameRemarks, strSheetName,
						chk.at(k_name_remarks), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 降車専用
				if( strGetOffOnly != chk.at(bus_stop_point::kGettingOffF) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[降車専用]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strGetOffOnly, strSheetName,
						chk.at(bus_stop_point::kGettingOffF), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 緯度
				if( strLat != chk.at(k_dms_lat) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[緯度]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strLat, strSheetName,
						chk.at(k_dms_lat), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 経度
				if( strLon != chk.at(k_dms_lon) ){
					errMsg.Format( _T("[停留所コード(%s)],[標柱番号(%s)]が同一で、[経度]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameStop.first.first, sameStop.first.second, strLon, strSheetName,
						chk.at(k_dms_lon), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}

				strStopKanji    = chk.at(bus_stop_point::kName);
				strStopYomi     = chk.at(bus_stop_point::kYomi);
				strChgStopKanji = chk.at(bus_stop_point::kChangedName);
				strChgStopYomi  = chk.at(bus_stop_point::kChangedYomi);
				strNameRemarks  = chk.at(k_name_remarks);
				strGetOffOnly   = chk.at(bus_stop_point::kGettingOffF);
				strLat          = chk.at(k_dms_lat);
				strLon          = chk.at(k_dms_lon);
				strSheetName    = chk.at(_T("SHEET_NAME"));
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR ファイル内チェック中に未登録キーの例外が発生：" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}

// ファイル内チェック（同一緯度経度で停留所コードor標柱番号確認）
bool CBusPointExcelChk::CheckByFileSameCoord( const map<CCPAIR, BUSCONTVEC>& sBusInputs, list<CString>& errMsgs )
{
	try{
		CString errMsg;
	
		for( auto& sameLL : sBusInputs ){

			CString strStopID    = sameLL.second.begin()->at(bus_stop_point::kStopID);
			CString strPoleNum   = sameLL.second.begin()->at(bus_stop_point::kPoleNo);
			CString strSheetName = sameLL.second.begin()->at(_T("SHEET_NAME"));

			for( auto& chk : sameLL.second ){

				// 緯度経度が同じ停留所で、(停留所コード/標柱番号)のいずれかが異なる

				// 停留所コード
				if( strStopID != chk.at(bus_stop_point::kStopID) ){
					errMsg.Format( _T("[緯度(%s)],[経度(%s)]が同一で、[停留所コード]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameLL.first.first, sameLL.first.second, strStopID, strSheetName,
						chk.at(bus_stop_point::kStopID), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}
				// 標柱番号
				if( strPoleNum != chk.at(bus_stop_point::kPoleNo) ){
					errMsg.Format( _T("[緯度(%s)],[経度(%s)]が同一で、[標柱番号]が異なる（「%s」([%s]シート) と「%s」([%s]シート) ）"), 
						sameLL.first.first, sameLL.first.second, strPoleNum, strSheetName,
						chk.at(bus_stop_point::kPoleNo), chk.at(_T("SHEET_NAME")) );
					errMsgs.push_back( errMsg );
				}

				strStopID    = chk.at(bus_stop_point::kStopID);
				strPoleNum   = chk.at(bus_stop_point::kPoleNo);
				strSheetName = chk.at(_T("SHEET_NAME"));
			}
		}
	}
	catch( out_of_range& e ){
		m_ofsChk << "#ERROR ファイル内チェック中に未登録キーの例外が発生：" << e.what() << endl;
		return false;
	}

	if( errMsgs.empty() ){
		return true;
	}
	else{
		return false;
	}
}
