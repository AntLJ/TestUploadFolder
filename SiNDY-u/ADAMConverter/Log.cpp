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

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{

		// ファイル関連
		case NgLoadCnvParam:		return _T("地域パラメータファイル読み込み失敗(%s)");
		case NgGetSubFields:		return _T("[%s]：変換元テーブル サブフィールド取得失敗");
		case NgOpenOutputFile:		return _T("出力ファイルオープン失敗(%s)");
		case NgEncode:				return _T("エンコードタイプ不正");
		case NgExistField:			return _T("[%s]：変換フィールド対応表 変換元テーブル[%s]にフィールド[%s]は存在しない");

		// 出力フォーマット関連
		case NgOpEncode:			return _T("オプション：文字コードオプションに不正値(%s)");
		case NgOpDelimiter:			return _T("オプション：デリミタオプションに不正値(%s)");
		case NgOpNewline:			return _T("オプション：改行コードオプションに不正値(%s)");
		case NgOpGeoref:			return _T("オプション：測地系オプションに不正値(%s)");
		case NgOpShapeType:			return _T("オプション：形状タイプオプションに不正値(%s)");
		case NgOpPolygonType:		return _T("オプション：ポリゴン向きオプションに不正値(%s)");


		// 変換テーブル関連
		case NgLoadConvTable:				return _T("変換テーブル 読み込み失敗(%s)");
		case DuplicateConvTableGroup:		return _T("[%s]：変換テーブル グループが重複");
		case NgConTableTab:					return _T("[%s]：変換テーブル レコードタブ数が%ldでない(%s)");
		case DuplicateConvTableAttribute:	return _T("[%s]：変換テーブル %sが重複");
		case UnknownConvTableAttribute:		return _T("[%s]：変換テーブル 不明なタグ(%s)");
		case NoneConvTableGroup:			return _T("変換テーブル グループが1つも存在しない");
		case NgMatchGroup:					return _T("変換テーブルと変換フィールド対応表でグループが一致しない(%s)");
		case NoneConvTableAttribute:		return _T("[%s]：変換テーブル %sが未設定");
		case NoneConvTableGroup2:			return _T("変換テーブル 必須項目は設定されているがグループ名が未設定");
		case NgConvTableFieldName:			return _T("[%s]：変換テーブル %s %sテーブルが存在しない");
		case UnknownSearchType:				return _T("[%s]：変換テーブル 不明な検索タイプ(%s)");
		case NgOidTableName:				return _T("[%s]：変換テーブル OIDフィールドがピリオド区切りになっていない(%s)");

		// 変換フィールド対応表関連
		case NgLoadConvField:				return _T("変換フィールド対応表 読み込み失敗(%s)");
		case DuplicateConvFieldGroup:		return _T("[%s]：変換フィールド対応表 グループが重複");
		case NoneConvFieldGroup:			return _T("変換フィールド対応表 変換フィールド対応表 グループが1つも存在しない");
		case NgConvFieldTab:				return _T("[%s]：変換フィールド対応表 レコードタブ数が%ld以上でない(%s)");
		case UnknownConvFieldAttribute:		return _T("[%s]：変換フィールド対応表 オプション名が不正(%s)");
		case NoneConvFieldSrcFieldComma:	return _T("[%s]：変換フィールド対応表 変換元フィールドにピリオドがない(%s)");
		case NgConvFieldFieldType:			return _T("[%s]：変換フィールド対応表 %sの型指定に不正値(%s)");
		case NgConvFieldLength:				return _T("[%s]：変換フィールド対応表 %sの列幅に不正値(%s)");
		case NgConvFieldNull:				return _T("[%s]：変換フィールド対応表 %sのNULL許可値に不正値(%s)");
		case DuplicateConvFieldRoundValue:	return _T("[%s]：変換フィールド対応表 %sの丸め込みオプションで1対多になる(%s %s)");
		case NgConvFieldOptionValue:		return _T("[%s]：変換フィールド対応表 %sの%s値情報が不足");
		case DuplicateRange:				return _T("[%s]：変換フィールド対応表 %sの%s範囲情報の設定範囲が重複(%s%s %s%s / %s%s %s%s)");
		case NoneRequiredRange:				return _T("[%s]：変換フィールド対応表 %sの%s範囲情報の必須項目が空");
		case NgSelectRange:					return _T("[%s]：変換フィールド対応表 %sの%s範囲情報の選択項目が不正");
		case NgConfigRange:					return _T("[%s]：変換フィールド対応表 %sの%s範囲情報の範囲設定が不正(%s%s)");
		case NoneConvFieldGroup2:			return _T("変換フィールド対応表 フィールド対応項目は設定されているがグループ名が未設定");
		case DuplicateDstField:				return _T("[%s]：変換フィールド対応表 出力フィールド名が重複(%s)");
		case NgRangeValue:					return _T("[%s]：変換フィールド対応表 %sの%s範囲情報の設定値が数字でない(%s)");
		case NgNullValue:					return _T("[%s]：変換フィールド対応表 %sの%s値情報が空文字になっている");

		// 出力データ作成
		case NgShapeType:					return _T("形状タイプ不正");
		case NgDataType:					return _T("データ型不正");
		case NgGeomType:					return _T("ジオメトリタイプ不正");
		case NgTOKYOtoJGD2000_RP:			return _T("日本測地系->世界測地系変換失敗");
		case NgCoordIntegerDigitOver:		return _T("%s値の整数桁オーバー(桁数：%ld, データ：%s)");
		case NgPointCoord:					return _T("形状不正(ポイント)");

		// 出力データチェック
		case NgCheckValidValue:				return _T("有効値外([%s.%s] %s)");
		case NgCheckValidRange:				return _T("有効範囲値外([%s.%s] %s)");
		case NgCheckErrValue:				return _T("エラー値内([%s.%s] %s)");
		case NgCheckErrRange:				return _T("エラー範囲値内([%s.%s] %s)");
		case NgCheckNull:					return _T("NULLチェックエラー([%s.%s] %s)");
		case NgCheckIntegerDigitOver:		return _T("整数桁不正([%s.%s] %s)");
		case NgCheckStringLength:			return _T("文字列数不正([%s.%s] %s)");
		case NgCheckFieldType:				return _T("フィールドタイプ不正([%s.%s] %s)");

		// ArcObjects関連
		case NgDBConnect:					return _T("DB接続失敗(%s)");
		case NgOpenTable:					return _T("[%s]：%sテーブルオープン失敗");
		case NgGetRowCount:					return _T("[%s]：レコード数取得失敗");
		case NgGetCursor:					return _T("[%s]：カーソル取得失敗");
		case NgQueryDef:					return _T("[%s]：QueryDef取得失敗");
		case NgJoinTable:					return _T("[%s]：テーブル結合設定失敗");
		case NgSetFilter:					return _T("[%s]：検索フィルタ設定失敗");
		case NgEvalute:						return _T("[%s]：Evaluate失敗");
		case NgGetFields:					return _T("[%s]：フィールド群取得失敗");
		case NgGetFieldCount:				return _T("[%s]：フィールド数取得失敗");
		case NgGetField:					return _T("[%s]：%ldフィールド目取得失敗");
		case NgGetGeomDef:					return _T("[%s]：%ldフィールド目取得失敗（形状フィールド）");
		case NgGetHasZ:						return _T("[%s]：Z値使用フラグ取得失敗");
		case NgGetHasM:						return _T("[%s]：M値使用フラグ取得失敗");
		case NgGetGeomType:					return _T("[%s]：形状タイプ取得失敗");
		case NgNextRowCount:				return _T("[%s]：件数レコード取得失敗");
		case NgGetValueCount:				return _T("[%s]：件数カラム取得失敗");
		case NgFindField:					return _T("[%s]：%s %s.%sフィールドが存在しない");
		case NgSetSubFields:				return _T("[%s]：出力対象フィールド設定失敗");
		case NgSetPostfixClause:			return _T("[%s]：PostfixClause設定失敗");
		case NgSetPrefixClause:				return _T("[%s]：PrefixClause設定失敗");

		case NgGetValue:					return _T("データ取得失敗");
		case NgQueryInterface:				return _T("%s->%s変換失敗");
		case NgGetPointCount:				return _T("構成点数取得失敗");
		case NgGetPoint:					return _T("ポイント取得失敗");
		case NgGetExteriorRingCount:		return _T("外側リング数取得失敗");
		case NgQueryExteriorRings:			return _T("外側リング取得失敗");
		case NgGetInteriorRingCount:		return _T("内側リング数取得失敗");
		case NgQueryInteriorRings:			return _T("内側リング取得失敗");
		case NgQueryCoord:					return _T("ポイントの緯度経度取得失敗");
		case NgGetZ:						return _T("Z値取得失敗");
		case NgGetM:						return _T("M値取得失敗");

		// その他
		case NgChangeType:					return _T("ChangeType失敗");

		default:							return _T("エラーです");
	}
}

CString CLog::GetErrTypeMsg(err_type::ECode errType)
{
	using namespace err_type;
	switch(errType)
	{
		case fatal:   return _T("FATAL");
		case error:   return _T("ERROR");
		case warning: return _T("WARNING");
		case info:    return _T("INFO");
		default:      return _T("UNKNOWN");
	}
}

void CLog::PrintLog1(bool bCout,
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
}

void CLog::PrintLog2(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					const CString strOID,
					...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, strOID);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	// 最終的なログ出力部分のフォーマットをここで設定
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t%s\t%s\t%ld\t%s\t"), GetErrTypeMsg(errType), strTableName, strOID, errCode, strMsg);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					const CString& strMsg,
					bool bLineBreak)
{
	if(bCout){
		if(bLineBreak)
			std::cout << CT2A(strMsg) << std::endl;
		else
			std::cout << CT2A(strMsg);
	}

	if(bCerr){
		if(bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg);
	}

	if(bRun && m_ofsRunLog.is_open()){
		if(bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg);
	}

	if(bErr && m_ofsErrLog.is_open()){
		if(bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg);
	}
}

bool CLog::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if(!ofsLogFile.is_open()){
		std::cerr << CT2A(strFilePath) << "のオープンに失敗" << std::endl;
		return false;
	}
	return true;
}

bool CLog::SetRunLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsRunLog);
}

bool CLog::SetErrLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsErrLog);
}
