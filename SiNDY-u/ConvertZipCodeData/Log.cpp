#include "stdafx.h"
#include "Log.h"

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
		case NgAddrDBConnectString:			return _T("住所DB接続文字列不正(%s)");
		
		// 郵便番号と住所紐付け
		case NgDuplicateRelAddr:			return _T("郵便番号に紐付く住所が重複(%s:%s)");
		case NgAddrCode:					return _T("住所コードの桁数が不正(%s:%s)");
		case NgAddr11Dir:					return _T("住所データのディレクトリにファイルが存在しない(%s)");
		case NgAddr11File:					return _T("住所データのディレクトリにaddr**.txt以外のファイルが存在する(%s)");
		case NgAddr11FileOpen:				return _T("ファイルオープン失敗(%s)");
		case NgAddr11Record:				return _T("住所データのレコードがおかしい(%s)");
		case NgAddr11Duplicate:				return _T("11桁コードが重複(%s)");
		case NgAddr11AccCode2:				return _T("住所コードが11桁で精度2(%s)");

		// 郵便番号ポリゴン
		case NgZipPriority:					return _T("優先度に対応する郵便番号が存在しない(%s)");
		case NgOverlapZipValue:				return _T("郵便番号ポリゴンのZIPCODE1,ZIPCODE2の設定がおかしい(%s,%s)");
		case NgSameZipCode:					return _T("ZIPCODE1,ZIPCODE2に同じ郵便番号が設定されている");

		// 重心
		case NgGetCentroid:					return _T("重心取得失敗");
		case NgGetCentroid2:				return _T("重心取得失敗（例外）");

		// 市区町村代表点
		case NgDuplicateAddrRec:			return _T("代表点が複数ある(%s)");
		case NgAddrCode5Digit:				return _T("住所コードが5桁でない(%s)");
		case NgZipCodeUnder00:				return _T("郵便番号の下2桁が「00」でない(%s)");

		// ArcObjects関連
		case NgDBConnect:					return _T("DB接続失敗(%s)");
		case NgOpenTable:					return _T("テーブルオープン失敗");
		case NgPutWhereClause:				return _T("クエリフィルタ設定失敗");
		case NgGetCursor:					return _T("カーソル取得失敗");
		case NgGetFields:					return _T("フィールド群取得失敗");
		case NgGetFieldCount:				return _T("フィールド数取得失敗");
		case NgGetField:					return _T("%ldフィールド目取得失敗");
		case NgGetFieldName:				return _T("フィールド名取得失敗");
		case NgGetValue:					return _T("データ取得失敗");
		case NgGetEnumDatasetName:			return _T("データセット名リスト取得失敗(%s)");
		case NgResetEnumDatasetName:		return _T("データセット名リストリセット失敗(%s)");
		case NgGetTableName:				return _T("データセット名の名称取得失敗(%s)");
		case NgDeleteTable:					return _T("テーブル削除失敗");
		case NgGetShape:					return _T("形状取得失敗");
		case NgGetOID:						return _T("OID取得失敗");
		case NgQueryInterface:				return _T("QueryInterface失敗");

		case NgPutName:						return _T("フィールド名称設定失敗(%s)");
		case NgPutType:						return _T("フィールドタイプ設定失敗(%s)");
		case NgPutEditable:					return _T("編集許可設定失敗(%s)");
		case NgPutIsNullable:				return _T("NULL許可設定失敗(%s)");
		case NgPutLength:					return _T("長さ設定失敗(%s)");
		case NgPutPrecision:				return _T("Precision設定失敗(%s)");
		case NgPutSpRef:					return _T("空間参照設定失敗");
		case NgPutGeoType:					return _T("形状タイプ設定失敗");
		case NgPutGeoDef:					return _T("GeometryDef設定失敗");
		case NgPutCoords:					return _T("ポイント設定失敗(%s)");
		case NgAddField:					return _T("フィールド追加失敗(%s)");
		case NgCreateTable:					return _T("テーブル作成失敗");

		case NgInsertCursor:				return _T("Insert用のカーソル取得失敗");
		case NgCreateRowBuffer:				return _T("レコードバッファ取得失敗");
		case NgPutValue:					return _T("属性設定失敗");
		case NgInsertRow:					return _T("レコード追加失敗");
		case NgFlush:						return _T("Flush失敗");
		case NgPutShape:					return _T("形状設定失敗");

		case NgUnion:						return _T("形状マージ失敗");

		// その他
		case NgOther:
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
