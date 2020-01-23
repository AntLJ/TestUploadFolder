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

// RNSConsoleApp.cpp: RNSConsoleApp クラスのインプリメンテーション
//
// $Id: RNSConsoleApp.cpp,v 1.1 2003/05/16 09:40:02 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include "RNSConsoleApp.h"
#include "RNSMessage.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNSConsoleApp::RNSConsoleApp(const char* cAppCaption) :
	mAlreadyRun(false)
{
	setAppCaption(cAppCaption);
}

RNSConsoleApp::~RNSConsoleApp()
{

}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief	引数を設定する
 *
 * @param	argc		[in]	引数の数
 * @param	argv		[in]	引数
 * @retval	true	正常に設定できた
 * @retval	false	定義されていないオプションが引数に含まれている
 */
bool RNSConsoleApp::setArgs(int argc, char* argv[])
{
	// アプリケーション名設定
	setAppName(argv[0]);

	// オプション設定
	const char* pOptionSimbols = optionSimbols();
	if(pOptionSimbols) {
		mOptionSet.regestOptionSimbols(pOptionSimbols);
	}

	return mOptionSet.setOption(argc-1, argv+1, &mArgList);
}

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief	処理開始
 *
 * @note	複数回呼び出すとstd::logic_error()を発生させる。
 *
 * @retval	0	成功
 * @retval	!0	失敗
 */
int RNSConsoleApp::run()
{
	using namespace rns_message;

	// 複数回呼び出し対応
	if(mAlreadyRun)
		throw std::logic_error(RNS_DEBUG_STRING("\"run\" method 2 times called"));
	mAlreadyRun = true;

	// usage出力
	if(isNeedPrintUsage()) {
		printUsage(std::cerr);
		return 0;
	}

	// 本処理呼び出し
	int aResult = callEntrust();

	if(aResult == 0) {
		std::cerr << appCaption().c_str() << getMsg(iSuccess) << std::endl;
	}
	else {
		std::cerr << appCaption().c_str() << getMsg(eFailure) << std::endl;
	}

	return aResult;
}

/**
 * @brief	本処理呼び出し
 *
 * try構文内でpreEntrust()、entrust()、epiEntrust()を呼び出す。\n
 * preEntrust()やepiEntrust()がfalseを返した場合、またはstd::exception()から派生された例外を捕らえた場合、-1を返して処理を中断する。\n
 *
 * @retval	0	成功
 * @retval	!0	失敗
 */
int RNSConsoleApp::callEntrust()
{
	int aResult = 0;

	try {
		if(! preEntrust())
			return -1;

		aResult = entrust();

		if(! epiEntrust())
			return -1;
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		aResult = -1;
	}

	return aResult;
}

//////////////////////////////////////////////////////////////////////
// オーバーライドされる処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief	usage出力
 *
 * @param	cStream	[in]	usageを出力するストリーム
 */
void RNSConsoleApp::printUsage(std::ostream& cStream)
{
	cStream << appCaption().c_str() << "(" << appName().c_str() << ")" << std::endl;
}
