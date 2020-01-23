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

/**
 *	@file main.cpp
 *	@brief エントリポイント
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */
// main.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <exception>
#include <arctl/coinitializer.h>
#include <atl2/io.h>
#include "CreateLinkPatch.h"
#include "GlobalFunction.h"


int main(int argc, char* argv[])
{
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;	// Arc9.3.1対応時にtryスコープ内に移動

		CreateLinkPatch aApp;
		aApp.init( argc, argv );

		CString aStartTime;
		GlobalFunction::getTime( aStartTime );
		std::cout << "Start at " << static_cast<const char*>(aStartTime) << std::endl;

		aApp.run();

		CString aEndTime;
		GlobalFunction::getTime( aEndTime );
		std::cout << "End at " << static_cast<const char*>(aEndTime) << std::endl;
	}
	catch (std::exception& e) 
	{
		//e.what()に付与したフラグ"R1F:"が存在すれば、プログラム作成者の認識しているエラーなのでreturn 1 を返す。
		//R1FはRETURN1FLAGの略
		CString ErrMess = e.what();
		int flagcheck = strncmp(ErrMess,"R1F:",4);

		if(flagcheck == 0)
		{
			//付与したフラグ部分の削除
			ErrMess = ErrMess.Right(ErrMess.GetLength()-ErrMess.Find(_T(":"))-1);
			
			std::cerr << CT2CA(ErrMess) << std::endl;
			std::cerr << "異常終了" << std::endl;
			return 1;
		}
		else
		{
			//フラグがなければ想定外のエラーなので例外での異常終了とし、return 2 を返す。
			std::cerr << e.what() << std::endl;
			std::cerr << "例外が発生" << std::endl;
			std::cerr << "異常終了(例外)" << std::endl;
			return 2;
		}
	}
	catch (_com_error& e) {
		std::cerr << e << std::endl;
		std::cerr << "例外が発生" << std::endl;
		std::cerr << "異常終了(例外)" << std::endl;
		return 2;
	}
	std::cerr << "正常終了" << std::endl;
	return 0;
}
