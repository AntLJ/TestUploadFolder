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
#include "Arguments.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

#include "ParseFunction.h"
#include "AddIndex.h"

using namespace std;

enum RESULT:int
{
	SUCCESS   =0, //!< 正常終了
	ABEND     =1, //!< エラー有り異常終了
	EXCEPTION =2, //!< 例外有り異常終了
};

namespace
{
/**
 * @brief コマンドライン引数の状態と開始時刻を出力
 * @param args [in] コマンドライン引数
 */
inline void writeStart(const Arguments& args)
{
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_INPUT).GetString() << _T(" : ")
		<< args.m_db << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_CONFIG).GetString() << _T(" : ")
		<< args.m_config << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_RUNLOG).GetString() << _T(" : ")
		<< args.m_runLog << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_ERRLOG).GetString() << _T(" : ")
		<< args.m_errLog << _T("\n");

	// 開始時刻を出力
	RUNLOGGER.writeTime(AheLoadString(IDS_LOG_START_TIME).GetString());
}

} // namespace

int _tmain(int argc, _TCHAR* argv[])
{
	// ロケール関連の不具合防止
	uh::setJapaneseCharacterTypeFacet();

	// 実行結果を格納
	RESULT ret = RESULT::SUCCESS;
	try
	{
		const arctl::coinitializer aCoInitializer;
		try
		{
			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << AheLoadStringA(IDS_PARAMETER_HAVE_PROBLEM).GetString() << endl;
				return RESULT::ABEND;
			}
			// エラーログ・実行ログを開く
			if( !ERRLOGGER.Initialize( args.m_errLog ) )
			{
				cerr << AheLoadStringA(IDS_FAILED_OPEN_LOG).GetString()
					<< " : FILE[" << uh::toStr(args.m_errLog) << "]" << std::endl;
				return RESULT::ABEND;
			}
			if( !RUNLOGGER.Initialize( args.m_runLog ) )
			{
				cerr << AheLoadStringA(IDS_FAILED_OPEN_LOG).GetString()
					<< " : FILE[" << uh::toStr(args.m_runLog) << "]" << std::endl;
				return RESULT::ABEND;
			}

			// 処理開始時間をログに出力
			writeStart( args );

			index_def::INDEXINFOMAP indexInfo;
			if( !parser::xmlToIndexDef(args.m_config, indexInfo) )
				return RESULT::ABEND;

			// 以下からフォルダ/ファイル/レイヤの生成開始
			CAddIndex addIndex( args.m_db, indexInfo );
			if( !addIndex.execute() )
				return RESULT::ABEND;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		wcerr << uh::toWStr(e.what()) << endl;
		RUNLOGGER.fatal(uh::toTStr(e.what()).c_str());
		ret = RESULT::EXCEPTION;
	}
	return ret;
}
