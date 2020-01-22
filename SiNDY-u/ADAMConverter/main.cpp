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
#include "ADAMConverter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try {
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		// コマンドライン引数処理
		Arguments args;
		if(! args.parse(argc, argv)){
			retval = 1;
		} else {
			// 設定ファイルなどの読み込み、初期化
			CADAMConverter cADAMConverter;
			if(!cADAMConverter.init(args)){
				retval = 1;
			} else {
				// 処理実行
				if(!cADAMConverter.run()){
					retval = 1;
				}
			}
		}
	} catch(const _com_error& e) {
		CLog::GetInstance().PrintLog(true, true, true, false, CString(e.ErrorMessage()));
		retval = 2;
	} catch(const std::exception& e){
		CLog::GetInstance().PrintLog(true, true, true, false, CString(e.what()));
		retval = 2;
	} catch (...) {
		CLog::GetInstance().PrintLog(true, true, true, false, _T("予期せぬエラーが発生しました"));
		retval = 2;
	}

	CString strMsg;
	strMsg.Format(_T("End : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	if(0 == retval){
		CLog::GetInstance().PrintLog(true, false, true, false, _T("正常終了"));
	} else {
		CLog::GetInstance().PrintLog(true, false, true, false, _T("異常終了"));
	}
	return retval;
}
