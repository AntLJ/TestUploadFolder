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

#pragma once

#include "AddrCount.h"
#include <AddrLib/AreaCodeHelper.h>

using namespace addr;

/**
 * @class	CompareAddrCount
 * @brief	住所データ件数比較クラス
 */
class CompareAddrCount
{

public:

	CompareAddrCount(void) :
	  mCharF(false), mTarget(kTgtAll) {}
	~CompareAddrCount(void) {}

	/**
	 * @brief	使用方法の表示
	 */
	void PrintUsage();

	/**
	 * @brief	初期化
	 * @param	cArgc	[in]	引数の数
	 * @param	cArgv	[in]	引数リスト
	 * @return	成功したか否か
	 */
	bool Init(int cArgc, _TCHAR* cArgv[]);

	/**
	 * @brief	処理の実行
	 * @return	成功したか否か
	 */
	bool Execute();

private:

	bool		mCharF;			///< カウントの対象を文字付き住所とするかどうか
	ETarget		mTarget;		///< カウント対象指定方法
	ofstream	mLog;			///< ログファイルストリーム
	CString		mLogFilename;	///< ログファイル名
	CString		mTgtOpt;		///< 範囲指定文字列（範囲 or リストファイル or 5桁単体）

	CString		mDBOld;			///< DB接続文字列(旧)
	CString		mDBNew;			///< DB接続文字列(新)

	CString		mFCGouPoint;	///< 号ポイントフィーチャクラス名
	CString		mFCCityAdmin;	///< 都市地図行政界フィーチャクラス名
	CString		mFCCSAddrPoint;	///< CSポイントフィーチャクラス名

	CString		mJDBOld;		///< 住所DB接続文字列(旧)
	CString		mJDBNew;		///< 住所DB接続文字列(新)

	CAreaCodeHelper	mACHelperOld;	///< 住所マスタアクセスクラス(旧)
	CAreaCodeHelper	mACHelperNew;	///< 住所マスタアクセスクラス(新)

	AddrCount	mAddrCountOld;	///< 住所データカウンタ(旧)
	AddrCount	mAddrCountNew;	///< 住所データカウンタ(新)

	/**
	 * @brief	引数の解析
	 * @param	cArgc	[in]	引数の数
	 * @param	cArgv	[in]	引数リスト
	 * @return	成功したか否か
	 */
	bool fnAnalyzeArg(int cArgc, _TCHAR *cArgv[]);

	/**
	 * @brief	環境変数の取得
	 * @return	成功したか否か
	 */
	bool fnGetEnvVar();

	/**
	 * @brief	各フィーチャクラスの取得
	 * @param	cDB				[in]	DB接続文字列
	 * @param	cGouPoint		[out]	号ポイント・フィーチャクラス
	 * @param	cCityAdmin		[out]	都市地図行政界・フィーチャクラス
	 * @param	cCSAddrPoint	[out]	CSポイント・フィーチャクラス
	 * @return	成功したか否か
	 */
	bool fnGetFeatureClasses(const CString& cDB, IFeatureClassPtr& cGouPoint, IFeatureClassPtr& cCityAdmin, IFeatureClassPtr& cCSAddrPoint);

	/**
	 * @brief	住所マスタからの施行年月・廃止年月の取得
	 * @param	cAddrCode		[in]	住所コード
	 * @param	cShikoDate		[out]	施行年月
	 * @param	cHaishiDate		[out]	廃止年月
	 * @return	成功したか否か
	 */
	bool fnGetShikoHaishiDate(const CString& cAddrCode, CString& cShikoDate, CString& cHaishiDate); 
};
