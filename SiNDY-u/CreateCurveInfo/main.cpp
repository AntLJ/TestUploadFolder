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

// CreateCurveInfo.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <arctl/coinitializer.h>
#include <boost/program_options.hpp>
#include "CurveInfoCreator.h"
#include "OutputOptions.h"

namespace po = boost::program_options;

using namespace adlib;

/**
 * @brief コマンドライン引数の解析と変換処理の開始
 * @param[in]	vm	コマンドライン引数管理オブジェクト
 * @return	全ての処理が正常に終了したか否か
 **/
bool process( po::variables_map& vm )
{
	using namespace curve_info;

	// ロケール関連の不具合防止
	setJapaneseLocaleWithCNumpunctFacet();

	// 出力オプションの設定
	OutputOptions aOutputOptions(
		vm["max_roc_for_region_deletion"].as<long>(),
		vm["max_roc_for_region_deletion_high"].as<long>(),
		vm["max_roc_of_section"].as<long>(),
		vm["max_roc_of_section_high"].as<long>(),
		vm["curve_certify_angle"].as<long>(),
		vm["straight_angle"].as<double>(),
		vm["roc_point_interval"].as<long>(),
		vm["fitting_point_interval"].as<long>(),
		vm["fitting_threshold"].as<long>(),
		vm["section_min_angle"].as<double>()
	);

	if (vm.count("query_where_clause") != 0)
	{
		aOutputOptions.m_QueryWhereClause = vm["query_where_clause"].as<tstring>().c_str();
	}
	aOutputOptions.m_CreateEachMesh = (vm.count("create_each_mesh") != 0);
	aOutputOptions.m_SHP05DirConfig = (vm.count("shp05_dir_config") != 0);
	aOutputOptions.m_OutputVerificationLog = (vm.count("verification_logfile") != 0);
	aOutputOptions.m_LogHandlingProcess = (vm.count("log_handling_process") != 0);
	aOutputOptions.m_LogFitEvaluatingProcess = (vm.count("log_fit_evaluating_process") != 0);

	using namespace boost::filesystem;

	// 変換クラスの作成と実行
	curve_info::CurveInfoCreator aCurveInfoCreator( aOutputOptions );
	aCurveInfoCreator.init(
		vm["road_db"].as<tstring>(),
		vm["mesh_db"].as<tstring>(),
		path(vm["meshlist"].as<tstring>().c_str()),
		path(vm["output_dir"].as<tstring>().c_str()),
		path(vm["logfile"].as<tstring>().c_str()),
		aOutputOptions.m_OutputVerificationLog ? path(vm["verification_logfile"].as<tstring>().c_str()) : path()
		);
	aCurveInfoCreator.convert();

	return true;
}

/**
 * @brief エントリポイント
 */
int _tmain(int argc, _TCHAR* argv[])
{
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;

		try
		{
			// TODO: tstringに対応
			// TODO: "road_db"などと書くのを１回だけにする
			// TODO: 接続の自動リトライに対応
			// TODO: usageの改行部分がプロンプト上で文字化けするのを修正
			// TODO: 全体的に、ポインタだとNULLかどうかのチェックが面倒なので、std::tr1::reference_wrapperを使うなどして参照にできるところは変えたい。
			po::options_description desc("How to use");
			desc.add_options()
				("road_db,R",							po::wvalue<std::wstring>(), "[必須] 道路データベース")
				("mesh_db,M",							po::wvalue<std::wstring>(),	"[必須] ２次メッシュデータベース")
				("meshlist,m",							po::wvalue<std::wstring>(), "[必須] 出力対象２次メッシュコードのメッシュリストファイルパス or 対象２次メッシュコード ")
				("output_dir,o",						po::wvalue<std::wstring>(), "[必須] Shapeファイル出力ディレクトリパス")
				("logfile,l",							po::wvalue<std::wstring>(), "[必須] ログファイルパス ")
				("max_roc_for_region_deletion,r",		po::value<long>(),			"[必須] 領域削除のための最大曲率半径（低速域用）（m）")
				("max_roc_for_region_deletion_high,t",	po::value<long>(),			"[必須] 領域削除のための最大曲率半径（高速域用）（m）")
				("max_roc_of_section,w",				po::value<long>(),			"[必須] 区間の最大曲率半径（低速域用）（m）")
				("max_roc_of_section_high,u",			po::value<long>(),			"[必須] 区間の最大曲率半径（高速域用）（m）")
				("straight_angle,s",					po::value<double>(),		"[必須] 直線と見なす角度（度）")				// TODO: デフォルト値を記載
				("roc_point_interval,i",				po::value<long>(),			"[必須] 曲率半径を求める点の間隔（m）")				// TODO: デフォルト値を記載
				("curve_certify_angle,c",				po::value<long>(),			"[必須] カーブ認定角度（度）")				// TODO: デフォルト値を記載
				("fitting_point_interval,F",			po::value<long>(),			"[必須] 円弧がベジエ曲線にフィットするかを評価する点の間隔（m）")				// TODO: デフォルト値を記載
				("fitting_threshold,f",					po::value<long>(),			"[必須] 円弧がベジエ曲線にフィットすると見なす閾値（m）")				// TODO: デフォルト値を記載
				("section_min_angle,n",					po::value<double>(),		"[必須] 区間に対応する円弧の最小角度（度）")
				("query_where_clause,q",				po::wvalue<std::wstring>(), "[任意] 対象リンク指定クエリ") // bug 9617で追加
				("create_each_mesh,e",												"[任意] メッシュごとにShapeを作成する") // bug 9292で追加
				("shp05_dir_config,d",												"[任意] SHP05のディレクトリ構成で出力する") // bug 9704で追加
				("verification_logfile,v",				po::wvalue<std::wstring>(), "[任意] 検証用ログファイル")
				("log_handling_process,p",											"[任意] 処理過程をログファイルに出力する")
				("log_fit_evaluating_process,a",									"[任意] フィット評価過程をログファイルに出力する")	// 情報量が非常に多いので、--log_handling_processとは別にしておく
				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			// 必須パラメータのチェック
			if (vm.count("road_db") == 0 ||
				vm.count("mesh_db") == 0 ||
				vm.count("meshlist") == 0 ||
				vm.count("output_dir") == 0 ||
				vm.count("logfile") == 0 ||
				vm.count("max_roc_for_region_deletion") == 0 ||
				vm.count("max_roc_for_region_deletion_high") == 0 ||
				vm.count("max_roc_of_section") == 0 ||
				vm.count("max_roc_of_section_high") == 0 ||
				vm.count("straight_angle") == 0 ||
				vm.count("curve_certify_angle") == 0 ||
				vm.count("roc_point_interval") == 0 ||
				vm.count("fitting_point_interval") == 0 ||
				vm.count("fitting_threshold") == 0 ||
				vm.count("section_min_angle") == 0
				)
			{
				// TODO: 何が足りないかを通知
				std::cerr << desc << std::endl;
				return 0;
			}

			aIsSuccess = process(vm);
		}
		catch (_com_error& e)
		{
			aIsSuccess = false;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		aIsSuccess = false;
	}

	std::cerr << std::endl << ( aIsSuccess ? "正常終了" : "異常終了" ) << std::endl;

	return aIsSuccess ? 0 : 1;
}

