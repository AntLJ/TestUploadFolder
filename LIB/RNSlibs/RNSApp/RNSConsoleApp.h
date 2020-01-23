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

// RNSConsoleApp.h: RNSConsoleApp クラスのインターフェイス
//
// $Id: RNSConsoleApp.h,v 1.2 2003/07/14 01:21:01 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_)
#define AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <iostream>

#include "RNSOptionSet.h"

/**
 * @brief	コンソールアプリケーションクラス
 */
class RNSConsoleApp  
{
public:
// 構築/消滅
	RNSConsoleApp(const char* cAppCaption);
	virtual ~RNSConsoleApp();

// 設定
	bool setArgs(int argc, char* argv[]);

// 取得
	const std::string&	appCaption() const { return mAppCaption; }
	const std::string&	appName() const	{ return mAppName; }
	const RNSOptionSet&	optionSet() const { return mOptionSet; }
	const RNSArgList&	argList() const { return mArgList; }

// 処理
	int run();

protected:
// オーバーライドされる設定
	virtual const char* optionSimbols() { return 0; }

// オーバーライドされる判定
	virtual bool		isNeedPrintUsage() { return false; }

// オーバーライドされる処理
	virtual void		printUsage(std::ostream& cStream);
	virtual int			callEntrust();
	virtual bool		preEntrust(){return true;}
	virtual int			entrust() = 0;
	virtual bool		epiEntrust(){return true;}

private:
// 設定
	void setAppCaption(const char* cAppCaption) { mAppCaption = cAppCaption; }
	void setAppName(const char* cAppName)		{ mAppName = RNSPlant::fileNameOf(cAppName); }

// 処理

// プロパティ
	std::string		mAppCaption;	///< アプリケーション名
	std::string		mAppName;		///< アプリケーション実行ファイル名
	RNSOptionSet	mOptionSet;		///< 規定オプション一覧
	RNSArgList		mArgList;		///< 規定オプション外引数一覧

	bool			mAlreadyRun;	///< 実行開始済みフラグ
};

/** @example example_RNSConsoleApp.cpp
 * 通常、RNSConsoleAppクラス自体のインスタンスを作成することはない。\n
 * 例ファイルにおいてRNSConsoleAppのインスタンスが作成されている
 */

#endif // !defined(AFX_RNSCONSOLEAPP_H__832C4327_E892_46FC_B182_999FC8EA742D__INCLUDED_)
