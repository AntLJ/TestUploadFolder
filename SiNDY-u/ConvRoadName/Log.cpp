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
#include "Log.h"

CString Log::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch (errCode)
	{
		// オプション関連
		case NgDBConnect:			return _T("DB接続失敗(%s)");
		case NgOpenTable:			return _T("%sテーブルオープン失敗");
		case NgGetIndex:			return _T("%sテーブル%sフィールドのインデックス取得失敗");
		case NgSetFilter:			return _T("%sテーブルの検索フィルタ設定失敗");
		case NgGetRowCount:			return _T("%sテーブルのレコード数取得失敗");
		case NgGetCursor:			return _T("%sテーブルのカーソル取得失敗");
		case NgGetOID:				return _T("%sテーブルのOBJECTID取得失敗");
		case NgGetVal:				return _T("%s取得失敗");

		case FailOpenInputFile:		return _T("ファイルオープンに失敗しました。%s");
		case NgDictionaryFieldNum:	return _T("辞書ファイルにフィールド数が不正な行があります:%d行");
		case NgDictionaryFieldVal:	return _T("辞書ファイルにNULLフィールドが存在します:%d行");
		case NgDictionaryRecord:	return _T("辞書ファイルに分割前後で文字列が違う行が存在します：%d行");
		case DuplicateDictionary:	return _T("同文字列に対する辞書データが複数存在します。:d行");
		case DuplicateData:			return _T("レコードが重複しています");
		case NoRelData:				return _T("関連付く%sレコードが存在しない");
		case NullStrData:			return _T("表示用名称への文字列加工処理の結果、名称がNULLとなったため変換を飛ばします");

		case FailDelFile:			return _T("ファイル削除に失敗。ファイル名：%s");
		case NoInited:				return _T("出力クラスが初期化されていません");
		case FailOpenOutputFile:	return _T("出力ファイルオープンに失敗。ファイル名：%s");

		case NgSplitFunc:			return _T("定義されていない分割ファンクションが指定されています");
		case NgParenConsis:			return _T("（）の位置が名称と読みで整合が取れていません");
		case NgSpClsConsis:			return _T("分割属性が名称と読みで整合が取れていません");
		case DiffSplitCount:		return _T("名称と読みで分割数が異なります");
		case UnsymmetricalParen:	return _T("カッコが閉じていない文字列が存在します");
		case UnsymmetricalWord:		return _T("分割後の名称と読みの中に、片方のみNULLのものが存在します");

		default:					return _T("定義されていないエラーコードです");
	}
}

CString Log::GetErrTypeMsg(err_type::ECode errType)
{
	using namespace err_type;
	switch (errType)
	{
		case fatal:		return _T("FATAL");
		case error:		return _T("ERROR");
		case warning:	return _T("WARNING");
		case info:		return _T("INFO");
		default:		return _T("UNKNOWN");
	}
}

void Log::PrintLog1(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, errCode);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t\t\t%ld\t%s\t"), GetErrTypeMsg(errType), errCode, strMsg);
	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
	return;
}

void Log::PrintLog2(bool bCout,
				   	 bool bCerr,
					 bool bRun,
					 bool bErr,
					 err_type::ECode errType,
					 err_code::ECode errCode,
					 const CString& strTableName,
					 const long oid,
					 ...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, oid);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	// 最終的なログ出力部分のフォーマットをここで設定
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t%s\t%ld\t%ld\t%s\t"), GetErrTypeMsg(errType), strTableName, oid, errCode, strMsg);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
	return;
}


void Log::PrintLog(bool bCout,
	bool bCerr,
	bool bRun,
	bool bErr,
	const CString& strMsg,
	bool bLineBreak)
{
	if (bCout) {
		if (bLineBreak)
			std::cout << CT2A(strMsg) << std::endl;
		else
			std::cout << CT2A(strMsg);
	}

	if (bCerr) {
		if (bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg);
	}

	if (bRun && m_ofsRunLog.is_open()) {
		if (bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg);
	}

	if (bErr && m_ofsErrLog.is_open()) {
		if (bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg);
	}
	return;
}

bool Log::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if (!ofsLogFile.is_open()) {
		std::cerr << CT2A(strFilePath) << "のオープンに失敗" << std::endl;
		return false;
	}
	return true;
}

bool Log::SetRunLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsRunLog);
}

bool Log::SetErrLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsErrLog);
}
