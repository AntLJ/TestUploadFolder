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
#include "Diff2Sindy.h"
#include "PathFinder.h"
#include "ListProcessor.h"
#include "PGFGDBImportProcessor.h"
#include "PGFGDBExportProcessor.h"
#include "PGFGDBConvertProcessor.h"
#include "PGFGDBMergeProcessor.h"
#include "MeshClipProcessor.h"
#include "LotClipProcessor.h"
#include "PPLProcessor.h"
#include "PathUtility.h"

#include <arctl/coinitializer.h>
#include <directory_util.h>
#include <TDC/sindylib_core/Workspace.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <future>

using namespace sindy;

namespace sindy {
	extern bool g_bCheckUpdatedByArcObjects;
#ifdef _DEBUG
	extern bool g_bTraceUpdate;
#endif // ifdef _DEBUG
	extern bool g_bOutputTraceUpdate;
	extern bool g_bCreateAllStoreInfo;
}

/**
 * @brief taregt を num 個に分割する
 *
 * targetの総数がnumにみたない場合は、１つずつに分割されます
 * @param target [in] 分割対象
 * @param num [in] 分割個数
 * @return 分割されたものが格納されたコンテナ
 */
template <typename T>
std::vector<std::vector<T>> split( const std::vector<T>& target, size_t num )
{
	std::vector<std::vector<T>> ret;

	if( 1 > num || target.empty() )
		return ret;

	size_t target_count = target.size();
	// 総数が分割数より少ないなら、分割数を総数に合わせる
	if( num > target_count )
		num = target_count;
	size_t split_count = target_count / num;
	std::vector<T>::const_iterator start = target.begin();
	size_t remainder_count = target_count;
	for( size_t i = 0; i < num; ++i )
	{
		if( 0 == remainder_count )
			break;

		size_t copy_count = split_count;
		if( split_count > remainder_count )
			copy_count = remainder_count;

		ret.push_back( std::vector<T>() );
		std::copy( start, start + copy_count, std::back_inserter( *ret.rbegin() ) );
		start += copy_count;
	}
	return ret;
}

// 初期化
bool CDiff2Sindy::init()
{
	g_bCheckUpdatedByArcObjects = false; // sindylibでの更新の有無はキャッシュ作成前を考慮しない
#ifdef _DEBUG
	g_bTraceUpdate = false; // レコード更新時のデバッグトレースは行わない
#endif // ifdef _DEBUG
	g_bOutputTraceUpdate = false; // レコード更新時の標準出力へのトレースは行わない
	g_bCreateAllStoreInfo = false; // エラーオブジェクトはエラーがあるもののみ

	// ADAMのカント値修正対応で必要なので追加
	// これでは困る場合はオプションで指定できるようにする必要がある
	g_bStoreShapeJudge_PointZ_Exact = true; // Store()時の形状変更判定で、Z値の許容値を判定しない

	return true;
}

// 実行
bool CDiff2Sindy::execute()
{
	// 初期化
	if( !init() )
		return false;

	// ログ表示
	outputExecInfo();

	// コンバート処理を行う（スレッド実行対応）
	if( !m_args.m_mapDefFile[s2sModeConvert].empty() )
	{
#ifndef S2S_NOUSETHREAD
		bool ret = true;
		// メッシュリストをスレッド数分に分ける
		auto listMeshes = split( m_args.m_meshes, m_args.m_thread );
		//  スレッド実行
		std::list<std::future<bool>> threads;
		for( const auto& meshes : listMeshes )
			threads.push_back( std::async( std::launch::async, [this, &meshes]{ return convertDB(meshes); } ) );
		// 終了確認
		for( auto& th : threads )
		{
			if( !th.get() )
				ret = false;
//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		if( ! ret )
			return false;
#else
		if( !convertDB(m_args.m_meshes) )
			return false;
#endif // ifndef S2S_NOUSETHREAD
	}

	// 隣接マージ処理を行う（スレッド実行非対応）
	if( !m_args.m_mapDefFile[s2sModeMerge].empty() )
	{
		if( !mergeDB(m_args.m_meshes) )
			return false;
	}

	// クリッピング処理を行う（スレッド実行非対応）
	if( !m_args.m_mapDefFile[s2sModeClip].empty() )
	{
		if( !clipDB(m_args.m_meshes) )
			return false;
	}

	return true;
}

// 複数のメッシュに区切られたテーブル、フィーチャを一つにする
bool CDiff2Sindy::convertDB( const std::vector<uh::tstring>& meshes )
{
#ifndef S2S_NOUSETHREAD
	const arctl::coinitializer aCoInitializer;
	{
		IArcGISLocalePtr ipLocal(CLSID_ArcGISLocale);
		ipLocal->SetThreadLocale();
#endif // ifndef S2S_NOUSETHREAD
		ConvertingDefinitionManager mgr;
		mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeConvert])); // コンバート実施定義クラスを作成

		if( !mgr )
			return true; // 定義がないなら終了
		// 1 メッシュ の処理クラスを作成
		std::shared_ptr<Processor<uh::tstring>> processor;

		// output,inputがフォルダかどうかで処理クラスを分ける
		if( isDirectory( m_args.m_output ) && !isDirectory( m_args.m_input ) )
			processor = std::shared_ptr<PGFGDBExportProcessor<uh::tstring>>(new PGFGDBExportProcessor<uh::tstring>( CWorkspace( m_args.m_input.c_str() ), m_args.m_output )); // TODO: 未実装
		else
			processor = std::shared_ptr<PGFGDBConvertProcessor<uh::tstring>>(
			new PGFGDBConvertProcessor<uh::tstring>( m_args.m_input, m_args.m_output, mgr, m_args));

		// ループ処理クラスを作成する
		ListProcessor<uh::tstring> listProcessor( meshes, processor );

		// ループ事前処理
		if(!listProcessor.doPreprocessing())
			return false;

		// ループ処理実行
		if(!listProcessor.doLoopProcessing())
			return false;

		// 一括処理実行
		if(!listProcessor.doBulkProcessing())
			return false;

		// ループ事後処理
		if(!listProcessor.doPostprocessing())
			return false;
#ifndef S2S_NOUSETHREAD
	}
#endif // ifndef S2S_NOUSETHREAD
	return true;
}

// 隣接マージを行う
bool CDiff2Sindy::mergeDB( const std::vector<uh::tstring>& meshes )
{
	MergingDefinitionManager mgr;
	mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeMerge])); // マージ実施定義クラスを作成

	if( !mgr )
		return true; // 定義がないなら終了

	CWorkspace outPutWorkspace(m_args.m_output.c_str());

	// 1メッシュごとのマージ処理クラスを作成
	std::shared_ptr<Processor<uh::tstring>> processor;
	processor = std::shared_ptr<PGFGDBMergeProcessor<uh::tstring>>(new PGFGDBMergeProcessor<uh::tstring>( outPutWorkspace,
																											m_args.m_meshDB,
																											m_args.m_meshLayerName,
																											m_args.m_meshTableOwner,
																											mgr,
																											m_args));

	// ループ処理クラスを作成する
	ListProcessor<uh::tstring> listProcessor( meshes, processor );

	// ループ事前処理
	if(!listProcessor.doPreprocessing())
		return false;

	// ループ処理実行
	if (!listProcessor.doLoopProcessing())
		return false;

	// ループ事後処理
	if(!listProcessor.doPostprocessing())
		return false;

	return true;
}

// クリッピングを行う
bool CDiff2Sindy::clipDB( const std::vector<uh::tstring>& meshes )
{
	ClippingDefinitionManager mgr;
	mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeClip])); // マージ実施定義クラスを作成

	if( !mgr )
		return true; // 定義がないなら終了

	CWorkspace outPutWorkspace(m_args.m_output.c_str());

	// 1メッシュごとのクリッピング処理クラスを作成
	std::shared_ptr<Processor<uh::tstring>> processor;
	switch( mgr.getClipType() )
	{
	case clipTypeLot:
		processor = std::make_shared<LotClipProcessor<uh::tstring>>( outPutWorkspace, mgr, meshes );
		break;
	case clipTypeMesh:
	default:
		processor = std::make_shared<MeshClipProcessor<uh::tstring>>( outPutWorkspace, mgr );
		break;
	}

	// ループ処理クラスを作成する
	ListProcessor<uh::tstring> listProcessor( meshes, processor );

	// ループ事前処理
	if(!listProcessor.doPreprocessing())
		return false;

	// ループ処理実行
	if (!listProcessor.doLoopProcessing())
		return false;

	// ループ事後処理
	if(!listProcessor.doPostprocessing())
		return false;

	return true;

}

// 処理開始ログを表示
void CDiff2Sindy::outputExecInfo() const
{
	using namespace std;

	wcout << L"#SINDYSTDLOG" << endl;
	wcout << L"# " << uh::toWStr( m_InternalName.GetString() )
		  << L" F:" << uh::toWStr( m_FileVersion.GetString() )
		  << L" P:" << uh::toWStr( m_ProductVersion.GetString() ) << endl;
	wcout << L"# インプットDB情報 : " << m_args.m_input << endl;
	wcout << L"# アウトプットDB情報 : " << m_args.m_output << endl;
	wcout << L"# インプット側オーナー名 : " << m_args.m_input_owner << endl;
	wcout << L"# アウトプット側オーナー名 : " << m_args.m_output_owner << endl;
	wcout << L"# メッシュ/リスト/メッシュファイル : " << m_args.m_mesh_list_str << endl;
	wcout << L"# メッシュDB : " << m_args.m_meshDB << endl;
	wcout << L"# メッシュDBオーナー名 : " << m_args.m_meshTableOwner << endl;
	wcout << L"# メッシュメッシュDB参照レイヤ名 : " << m_args.m_meshLayerName << endl;
	wcout << L"# マージ定義ファイルパス : " << m_args.m_mapDefFile[s2sModeMerge] << endl;
	wcout << L"# コンバート定義ファイルパス : " << m_args.m_mapDefFile[s2sModeConvert] << endl;
//	wcout << L"# 測地系変換定義ファイルパス : " << m_args.m_mapDefFile[s2sModeCrdcnv] << endl;
	wcout << L"# クリッピング定義ファイルパス : " << m_args.m_mapDefFile[s2sModeClip] << endl;
	wcout << L"# 実行ログファイルパス : " << m_args.m_runLog << endl;
	wcout << L"# エラーログファイルパス : " << m_args.m_errLog << endl;
	wcout << L"# 実行スレッド数 : " << m_args.m_thread << endl;
	for( const auto& val : m_args.m_values )
		wcout << L"# オプションVALUE : " << uh::toWStr( val ) << endl;
	wcout << L"# マルチパート解消 : " << ( m_args.m_dissolveMulti ? L"TRUE" : L"FALSE" ) << endl;
	wcout << L"# 開始時刻 : " << boost::lexical_cast< wstring >( boost::posix_time::second_clock::local_time() ) << endl;

	wcerr << L"#SINDYSTDLOG" << endl;
}