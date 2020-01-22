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
#include <atl2/io.h>
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Util.h"
#include "ImportBusPoint.h"
#include "CheckBusPoint.h"
#include "ExportBusPoint.h"

namespace po = boost::program_options;

/**
 * @brief 必要なオプション情報があるかをチェックする
 * @param vm [in] 引数のmap
 * @param option [in] オプション名称
 * @param desctiption [in] オプションの記述
 * @retval true 必要なオプションあり
 * @retval false 必要なオプションなし
 */
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption);


int _tmain(int argc, _TCHAR* argv[])
{
	try{
		const arctl::coinitializer aCoInitializer;

		try{
			// ロケール関連の不具合防止
			uh::setJapaneseCharacterTypeFacet();

			po::options_description desc("Allowed options");

			const char * const k_import          = "import";
			const char * const k_auto_rpoint     = "auto_rpoint";
			const char * const k_db_check        = "db_check";
			const char * const k_export          = "export";
			const char * const k_in_list         = "in_list";
			const char * const k_import_log      = "import_log";
			const char * const k_auto_rpoint_log = "auto_rpoint_log";
			const char * const k_db_check_log    = "db_check_log";
			const char * const k_export_log      = "export_log";
			const char * const k_bus_db          = "bus_db";
			const char * const k_road_db         = "road_db";
			const char * const k_company_list    = "company_list";
			const char * const k_export_dir      = "export_dir";

			Arguments	args;
			desc.add_options()
				(k_import,          "インポートモード(ON/OFF)")
				(k_auto_rpoint,     "ルート探索用ポイントモード(ON/OFF)")
				(k_db_check,        "DBチェックモード(ON/OFF)")
				(k_export,          "エクスポートモード(ON/OFF)")
				(k_in_list,         uh::tvalue<uh::tstring>(&args.in_list),         "[インポート][必須]調査結果ファイルリスト")
				(k_import_log,      uh::tvalue<uh::tstring>(&args.import_log),      "[インポート][必須]インポートログファイル")
				(k_auto_rpoint_log, uh::tvalue<uh::tstring>(&args.auto_rpoint_log), "[ルート探索用P][必須]ルート探索用Pログファイル")
				(k_db_check_log,    uh::tvalue<uh::tstring>(&args.db_check_log),    "[DBチェック][必須]DBチェックログファイル")
				(k_export_log,      uh::tvalue<uh::tstring>(&args.export_log),      "[エクスポート][必須]エクスポートログファイル")
				(k_bus_db,          uh::tvalue<uh::tstring>(&args.bus_db),			"[インポート/ルート探索用P/DBチェック/エクスポート][必須]バス接続用DB")
				(k_road_db,         uh::tvalue<uh::tstring>(&args.road_db),			"[ルート探索用P/DBチェック][必須]道路NW用DB")
				(k_company_list,    uh::tvalue<uh::tstring>(&args.company_list),    "[エクスポート]エクスポート対象事業者リスト(指定無し：全事業者)")
				(k_export_dir,      uh::tvalue<uh::tstring>(&args.export_dir),      "[エクスポート][必須]エクスポートディレクトリ")
				;

			if(argc == 1) {
				std::cerr << desc << std::endl;
				return 1;
			}

			// argc, argv を解析して、結果をvmに格納
			po::variables_map vm;
			store(parse_command_line(argc, argv, desc), vm);
			notify(vm);

			if( vm.count(k_import) != 0 )      { args.bImport     = true; }
			if( vm.count(k_auto_rpoint) != 0 ) { args.bAutoRPoint = true; }
			if( vm.count(k_db_check) != 0 )    { args.bDbCheck    = true; }
			if( vm.count(k_export) != 0 )      { args.bExport     = true; }

			// 必須パラメータチェック
			bool bMust = true;

			// 少なくとも一つのモードを指定する必要あり
			bMust &= ( args.bImport || args.bAutoRPoint || args.bDbCheck || args.bExport );

			// インポートモードで必須パラメータ
			if( args.bImport ){
				bMust &= check_required_option( vm, k_in_list,    "[インポート][必須]調査結果ファイルリスト");
				bMust &= check_required_option( vm, k_import_log, "[インポート][必須]インポートログファイル");
				bMust &= check_required_option( vm, k_bus_db,     "[インポート/ルート探索用P/DBチェック][必須]バス接続用DB");

			}

			// ルート探索用Pモードで必須パラメータ
			if( args.bAutoRPoint ){
				bMust &= check_required_option( vm, k_auto_rpoint_log, "[ルート探索用P][必須]ルート探索用Pログファイル");
				bMust &= check_required_option( vm, k_bus_db,          "[インポート/ルート探索用P][必須]バス接続用DB");
				bMust &= check_required_option( vm, k_road_db,         "[ルート探索用P/DBチェック][必須]道路NW用DB");
			}

			// DBチェックモードで必須パラメータ
			if( args.bDbCheck ){
				bMust &= check_required_option( vm, k_db_check_log, "[DBチェック][必須]DBチェックログファイル");
				bMust &= check_required_option( vm, k_bus_db,       "[インポート/ルート探索用P][必須]バス接続用DB");
				bMust &= check_required_option( vm, k_road_db,      "[ルート探索用P/DBチェック][必須]道路NW用DB");
			
				// DBチェックは排他処理
				if( args.bImport || args.bAutoRPoint || args.bExport ){
					std::cerr << "DBチェックモードと他モードは同時実行しないで下さい" << std::endl;
					bMust = false;
				}
			}

			// エクスポートモードで必須パラメータ
			if( args.bExport ){
				bMust &= check_required_option( vm, k_export_log,     "[エクスポート][必須]エクスポートログファイル");
				bMust &= check_required_option( vm, k_bus_db,         "[インポート/ルート探索用P][必須]バス接続用DB");
				bMust &= check_required_option( vm, k_export_dir,     "[エクスポート][必須]エクスポートディレクトリ");
			
				// エクスポート対象事業者の指定がない場合は、全事業者出力モード
				if( vm.count(k_company_list) == 0 ){
					args.bAllCompany = true;
				}
	
				// エクスポートモードは排他処理
				if( args.bImport || args.bAutoRPoint || args.bDbCheck ){
					std::cerr << "エクスポートモードと他モードは同時実行しないで下さい" << std::endl;
					bMust = false;
				}
			}

			// パラメータが正常ではない場合は、オプション表示して終了
			if( !bMust){
				std::cerr << "バスデータ統合ツール" << std::endl;
				std::cerr << desc << std::endl;
				return 1;
			}

			// 実処理
			bool bRet = false;

			// インポート＆ルート探索用ポイント
			if( args.bImport || args.bAutoRPoint ){
				CImportBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			// DBチェック
			else if( args.bDbCheck ){
				CCheckBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			// エクスポート
			else if( args.bExport ){
				CExportBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			else{
				bRet = false;
			}

			// 処理後の終了判定
			if( bRet ){
				// 正常終了
				std::cerr << "正常終了" << std::endl;
				return 0;
			}
			else{
				// 異常終了
				std::cerr << "異常終了" << std::endl;
				return 1;
			}
		}
		catch(const _com_error& e) {
			std::cerr << atl2::stringize(e) << std::endl;
			std::cerr << "異常終了" << std::endl;
		}
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "異常終了" << std::endl;
	}

	return 2;
}

// 必要なオプション情報があるかをチェックする
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption)
{
	if( vm.count(option) == 0 ){
		std::cerr << desctiption << "が未設定です。" << std::endl;
		return false;
	}
	return true;
}


