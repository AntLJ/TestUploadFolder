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

// ConvIllustLink2Egg.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <WinLib/VersionInfo.h>
#include <comdef.h>

#include "LogSys.h"
#include "TransParam.h"

extern bool exec(TransParam &TransParam);

void proc_info_write(TransParam& transParam)
{
	// ツール情報の表示
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s VERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion());
	LogSys::GetInstance().WriteProcLog(true, false, toolInfo);
	// オプションの表示
	LogSys::GetInstance().WriteProcLog(true, false, CA2CT(transParam.GetOptions().c_str()));
}

bool init_log (TransParam& transParam)
{
	if (! LogSys::GetInstance().SetProcLog(transParam.GetRunLog())) {
		std::cerr << "ERROR\t" << "実行ログの作成に失敗しました\t" << CT2A(transParam.GetRunLog()) << std::endl;
		return false;
	}
	if (! LogSys::GetInstance().SetErrLog(transParam.GetErrLog())) {
		std::cerr << "ERROR\t" << "エラーログの作成に失敗しまいした\t" << CT2A(transParam.GetErrLog()) << std::endl;
		return false;
	}
	LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
	LogSys::GetInstance().WriteErrLog(false, false, _T("# フラグ\tレイヤ名\tオブジェクトID\tエラーレベル\tエラーメッセージ\n"));
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		// 引数取得 ログの設定
		TransParam transParam;
		if (! transParam.parse(argc, argv) ||! init_log(transParam)) {
			retval = 1;
		} else {
			// オプションの表示
			proc_info_write(transParam);
			// 処理実行
			if (! exec(transParam))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		retval = 2;
	} catch (...) {
		std::cerr << "予期せぬエラーが発生しました" << std::endl;
		retval = 2;
	}
	if (retval == 0)
		LogSys::GetInstance().WriteProcLog(true, true, _T("正常終了\n"));
	else
		LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
	return retval;
}

