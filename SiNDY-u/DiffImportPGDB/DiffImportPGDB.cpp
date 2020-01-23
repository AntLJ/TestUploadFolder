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

// DiffImportPGDB.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
/*
 開発当初に以下の要件を設けました。
１．極力少ないツールと簡単な設定で実行できること
２．基本的にはバージョン対応のものに対してのみ実行可能
　　（特別なオプション等を付けることで、非対応の場合も実行可能に）
３．内部で作業バージョンを作成し、そこに対して更新すること
４．既存の手法と同じ結果になること
*/

#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include "Reflect.h"
#include "TimeLog.h"
#include <TDC/useful_headers/flush_scope_guard.h>

namespace {
	CString gConnect = _T("");			///< サーバ接続文字列
	CString gSuffix = _T("");			///< VDB時代の拡張子（処理セットの単位）
	CString gWorkVersion = _T("");		///< 作業用子バージョン
}

/** 
 * @brief 環境変数から接続情報の取得と作業用子バージョン名を決める
 *
 */ 
bool GetConnectInfo(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ANY"));
	if(aEnv != NULL){
		gConnect = aEnv;
	}else{
#ifdef _DEBUG
		//gConnect = _T("TRIAL/TRIAL/TRIAL.ETC_DIFFIMPORT_TEST/5151/Coral2");
		gConnect = _T("D:\\test\\DiffImportPGDB\\新フロー2\\Dst_import_empty_f.gdb");
#else
		_ftprintf(stderr, _T("環境変数「DB_ANY」が設定されていません。\n"));
		return false;
#endif // _DEBUG
	}
	aEnv = _tgetenv(_T("WORK_VERSION"));
	if(aEnv != NULL){
		gWorkVersion = aEnv;
	}else{
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);
		//メンテで消えるようにしようとしたが、それでは中断機能が無駄になるので「_for_post」追加
		gWorkVersion.Format(_T("DiffImportPGDB_%04d/%02d/%02d %02d:%02d:%02d_for_post"), 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	return true;
}

/** 
 * @brief 作業用子バージョンの作成又は使いまわし後、実際のバージョン名を返す
 *
 * @retval 作業用の子バージョン名（そのまま接続文字列として使用可能）
 */ 
CString CreateWorkVersion(void)
{
	// 接続
	IVersionPtr ipCurrentVersion = CReflect::connectSDE( gConnect );
	if(ipCurrentVersion){
		IVersionPtr ipChildVersion;
		((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion(CComBSTR(gWorkVersion), &ipChildVersion);
		if(ipChildVersion){
			_ftprintf(stderr, _T("#作業用子バージョンは既存のものを使います,%s\n"), static_cast<LPCTSTR>(gWorkVersion));
		}else{
			HRESULT hr = ipCurrentVersion->CreateVersion(CComBSTR(gWorkVersion),&ipChildVersion);
			if(ipChildVersion == NULL){
				_ftprintf(stderr, _T("#作業用子バージョン作成失敗,%s(hr=%d)\n"), static_cast<LPCTSTR>(gWorkVersion), hr);
				return _T("");
			}
		}
		return sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();
	}
	return _T("");
}

/** 
 * @brief 実行部本体
 *
 */ 
void Execute( const CArguments& arg )
{
	CString aWorkVersion;
	if( !arg.m_exceptSdeMode )
	{
		// 子バージョンを作成(作成したら一旦切断)
		aWorkVersion = CreateWorkVersion();
	}
	else
	{
		CString aExt = gConnect.Right(4);
		if( 0 != aExt.Compare(_T(".mdb")) && 0 != aExt.Compare(_T(".gdb")) )
		{
			_tprintf(_T("接続先はPGDBまたはFGDBを指定してください\n"));
			_ftprintf(stderr, _T("#接続先はPGDBまたはFGDBを指定してください\n"));
			return;
		}
		// SDE以外にインポートモードなら接続文字列そのまま
		aWorkVersion = gConnect;
	}

	if( !aWorkVersion.IsEmpty() )
	{
		_tprintf(_T("作業バージョンは[%s]です\n"), static_cast<LPCTSTR>(aWorkVersion));
		_ftprintf(stderr, _T("#作業バージョンは[%s]です\n"), static_cast<LPCTSTR>(aWorkVersion));
		bool aContinue = true;
		CReflect aReflect;
		while(aContinue)
		{
			// 中断できるようにループ
			if( !aReflect.init( aWorkVersion, arg ) )
				break;

			aContinue = aReflect.execReflect( arg );
			if( !aContinue )
				break;

			while( 1 )
			{
				// 大文字の「Q」又は「S」以外は受け付けない（CTRL+Cは効いてしまう）
				_tprintf(_T("「S」キーで再開します。「Q」キーで終了します。\n"));
				int aKey = _getch();
				if(aKey == 'Q')
				{
					aContinue = false;
					break;
				}
				if(aKey == 'S')
				{
					break;
				}
			}
		}
	}
}


/**
 * @brief メイン関数。
 *
 * @param argc	[in]	コマンドラインのトークン数（？）
 * @param argv	[in]	コマンドラインのトークン列（？）
 *
 * @retval 0		正常終了（実はきっちり守られてはいない）
 * @retval 0以外	異常終了
 */
int _tmain(int argc, _TCHAR* argv[])
{
	// スコープを抜ける時に強制的にflushする
	uh::flush_scope_guard fsg;

	::setlocale(LC_ALL, "Japanese");
	const arctl::coinitializer coinit;

	try
	{
		CArguments arg;
		if( !arg.parse( argc, argv ) )
		{
			return 1;
		}

		if( GetConnectInfo() )
		{
			Execute( arg );
			// logに終了時刻を出力して閉じる
			// ちなみにopenはExecute内でやっている
			arg.m_logFile.CloseLogFile();
		}
	}
	catch( const std::exception& e )
	{
		printf( "ERROR,%s\n", e.what() );
		fprintf( stderr, "#ERROR,%s\n", e.what() );
		return 1;
	}
	return 0;
}

