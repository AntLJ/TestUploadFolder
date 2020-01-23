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
#include "main.h"
#include "Arguments.h"
#include "ConvRoadName.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retVal = 0;
	CString strMsg;
	try {
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		// コマンドライン引数処理
		Arguments args;
		if (!args.parse(argc, argv)) {
			retVal = 1;
		} else {
			// 本処理実施
			strMsg.Format(_T("[%s] 処理開始"), getNowTime());
			Log::GetInstance().PrintLog(true, false, true, false, strMsg);
			ConvRoadName convRoadName;
			if (!convRoadName.run(args))
				retVal = 1;
		}
	} catch (const _com_error& e) {
		Log::GetInstance().PrintLog(true, true, true, false, CString(e.ErrorMessage()));
		retVal = 2;
	} catch (const std::exception& e) {
		Log::GetInstance().PrintLog(true, true, true, false, CString(e.what()));
		retVal = 2;
	} catch (...) {
		Log::GetInstance().PrintLog(true, true, true, false, _T("予期せぬエラーが発生しました"));
		retVal = 2;
	}
	if (0 == retVal) {
		strMsg.Format(_T("[%s] 正常終了"), getNowTime());
		Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	} else {
		strMsg.Format(_T("[%s] 異常終了"), getNowTime());
		Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	}
	strMsg.Format(_T("[%s] 処理完了"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);

    return retVal;
}

