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
#include "ErrorMsg.h"

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
		case NgOpenFile:          return _T("ファイルオープン失敗");
		case NgOpenWorkspace:     return _T("ワークスペース接続失敗");
		case NgOpenTable:         return _T("テーブル取得失敗");
		case NgGetFields:         return _T("フィールドリスト取得失敗");
		case NgGetField:          return _T("フィールド取得失敗");
		case NgGetRow:            return _T("レコード取得失敗");
		case NgGetRowBuffer:      return _T("レコードバッファ取得失敗");
		case NgGetShape:          return _T("形状取得失敗");
		//case NgGetDomain:         return _T("ドメイン取得失敗");
		//case NgGetDomainField:    return _T("ドメインフィールド取得失敗");
		//case NgGetFieldMap:       return _T("フィールドマップ取得失敗");
		case NgGetCursor:         return _T("カーソル取得失敗");
		case NgGetAttr:           return _T("属性取得失敗");
		//case NgGetDomainValue:    return _T("ドメイン値取得失敗");
		//case NgGetDataset:        return _T("データセット取得失敗");
		case NgGetFieldType:      return _T("フィールドタイプ取得失敗");
		case NgGetFieldIndex:     return _T("フィールドインデックス取得失敗");
		case NgGetClipShape:      return _T("クリップ形状取得失敗");
		case NgGetGeomType:       return _T("ジオメトリタイプ取得失敗");
		//case NgGetPointCol:       return _T("ポイントコレクション取得失敗");
		case NgGetVertexCount:    return _T("構成点数取得失敗");
		//case NgGetFieldLength:    return _T("フィールド長取得失敗");
		case NgGetFieldPrecision: return _T("精度取得失敗");

		case NgCreateDir:         return _T("ディレクトリ作成失敗");
		case NgMeshCodeDigit:     return _T("メッシュコードの桁数が合っていない");
		case NgFieldType:         return _T("フィールドタイプが変換対象外");
		case NgCreateClone:       return _T("クローン作成失敗");
		case NgGeomType:          return _T("ジオメトリタイプが未対応");
		case NgIntersect:         return _T("クリッピング失敗");

		case NgMoveFile:          return _T("ファイル移動失敗");
		case NgCreateTable:       return _T("テーブル作成失敗");

		case NgPutRowAttr:        return _T("レコード属性付与失敗");
		case NgPutRowShape:       return _T("レコード形状付与失敗");
		case NgPutFieldAttr:      return _T("フィールド属性与失敗");
		case NgPutWhere:          return _T("Where句指定失敗");
		//case NgPutSubFields:      return _T("SubFields指定失敗");
		case NgPutSpRel:          return _T("形状検索方法設定失敗");
		case NgPutRefGeom:        return _T("検索形状設定失敗");

		//case NgFGDB:              return _T("FGDBでない");
		//case NgStartEdit:         return _T("編集開始失敗");
		//case NgAbortEdit:         return _T("編集破棄失敗");
		//case NgStopEdit:          return _T("編集終了失敗");
		//case NgStore:             return _T("Store処理失敗");
		case NgFlush:             return _T("Flush処理失敗");

		case NgAttrFormatEssField:            return _T("必須属性（変換元と変換先）が指定されていない");
		case NgAttrFormatOrgFieldEmpty:       return _T("変換元フィールド名が空");
		case NgAttrFormatDstFieldEmpty:       return _T("変換先フィールド名が空");
		case NgAttrFormatOrgFieldExceptNewOp: return _T("変換元フィールド名に新規作成フィールドオプション以外が指定されている");
		case NgAttrFormatOrgFieldName:        return _T("変換元フィールドが存在しない");
		case NgAttrFormatDstFieldSetOp:       return _T("変換先フィールドにオプションが指定されている");
		case NgAttrFormatOrgFieldDuplicate:   return _T("変換元フィールドが重複している");
		case NgAttrFormatDstFieldDuplicate:   return _T("変換先フィールドが重複している");
		case NgAttrFormatNewOp:               return _T("新規フィールドオプションは変換元フィールド名（第1フィールド）のみに使用可能");
		case NgAttrFormatNoOp:                return _T("オプション指定なしは変換先フィールド名（第2フィールド）のみ許容");
		case NgAttrFormatUnknownOp:           return _T("設定オプション不明");
		case NgAttrFormatValue:               return _T("設定値のフォーマット不正");
		case NgAttrFormatValueAps:            return _T("設定値のフォーマット不正（「'」が不正）");
		case NgAttrFormatFieldInfo:           return _T("フィールド設定のフォーマット不正");
		case NgAttrFormatUnknownFieldInfo:    return _T("フィールド設定の設定項目が不正");
		case NgAttrFormatFieldInfoDuplicate:  return _T("フィールド設定の設定項目が重複");
		case NgAttrFormatRound:               return _T("丸め込み設定のフォーマット不正");
		case NgAttrFormatRoundAps:            return _T("丸め込み設定のフォーマット不正（「'」不正）");
		case NgAttrFormatRoundDuplicate:      return _T("丸め込み設定の設定項目が重複");
		case NgAttrFormatDefaultDupicate:     return _T("初期値設定の設定項目が重複");
		case NgAttrFormatAttrAps:             return _T("属性切り出し設定のフォーマット不正（「'」不正）");
		case NgAttrFormatAttrDuplicate:       return _T("属性切り出し設定が重複");
		case NgAttrFormatNewDefault:          return _T("新規作成フィールドに初期値設定がされていない");
		case NgAttrFormatNewFieldValue:       return _T("新規作成フィールドに属性値設定がされている");
		case NgAttrFormatNewRound:            return _T("新規作成フィールドに丸め込み設定がされている");
		case NgAttrFormatExistDefault:        return _T("既存フィールドに初期値設定がされている");
		case NgAttrFormatExistAttr:           return _T("既存フィールドに属性切り出し値設定がされている");
		case NgAttrFormatNewFieldInfoType:    return _T("新規作成フィールドにフィールド設定のTYPEが指定がされていない");
		case NgAttrFormatExistFieldInfoType:  return _T("既存フィールドにフィールド設定のTYPEが指定がされている");
		case NgAttrFormatFieldInfoType:       return _T("フィールド設定のTYPE設定値が不正");
		case NgAttrFormatFieldInfoTypeValue:  return _T("フィールド設定で型にあった属性が指定されていない");
		case NgAttrFormatFieldInfoValue:      return _T("フィールド設定で属性値が不正");
		case NgAttrFormatFieldInfoScale:      return _T("フィールド設定でSCALE設定値が不正");
		case NgAttrFormatFieldInfoPrecision:  return _T("フィールド設定でPRECISION設定値が不正");
		case NgAttrFormatFieldInfoLength:     return _T("フィールド設定でLENGTH設定値が不正");

		case NgAddRow:            return _T("レコード追加失敗");
		case NgAddField:          return _T("フィールド追加失敗");
		//case NgAddAttr:           return _T("属性追加失敗");
		//case NgAddDomain:         return _T("ドメイン追加失敗");

		//case NgUpdRow:            return _T("レコード更新失敗");
		//case NgUpdField:          return _T("フィールド更新失敗");
		//case NgUpdAttr:           return _T("属性更新失敗");
		//case NgUpdDomain:         return _T("ドメイン更新失敗");

		//case NgDelRow:            return _T("レコード削除失敗");
		//case NgDelField:          return _T("フィールド削除失敗");
		//case NgDelAttr:           return _T("属性削除失敗");
		//ase NgDelDomain:         return _T("ドメイン削除失敗");
		case NgDelFile:           return _T("ファイル削除失敗");

		//case GetManyRow:          return _T("複数レコード取得");
		case ExistField:          return _T("既存フィールド");
		case ChangePrecision:     return _T("精度設定変更");
		

		default:                  return _T("エラーです");
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
		default:      return _T("UNKOWN");
	}
}


void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeatureClassPtr ipFeatureClass)
{
	CComBSTR bstrName;
	IDatasetPtr(ipFeatureClass)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), 0, strOtherMsg);

}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					ITablePtr ipTable)
{
	CComBSTR bstrName;
	IDatasetPtr(ipTable)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), 0, strOtherMsg);

}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeatureClassPtr ipFeatureClass,
					long lOID)
{
	CComBSTR bstrName;
	IDatasetPtr(ipFeatureClass)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), lOID, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					ITablePtr ipTable,
					long lOID)
{
	CComBSTR bstrName;
	IDatasetPtr(ipTable)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), lOID, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeaturePtr ipFeature)
{
	long lOID = 0;
	ipFeature->get_OID(&lOID);
	IObjectClassPtr ipClass;
	ipFeature->get_Class(&ipClass);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, strOtherMsg, IFeatureClassPtr(ipClass), lOID);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					_IRowPtr ipRow)
{
	long lOID = 0;
	ipRow->get_OID(&lOID);
	ITablePtr ipTable;
	ipRow->get_Table(&ipTable);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, strOtherMsg, ipTable, lOID);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg)
{
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, _T(""), 0, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					long lOID,
					const CString& strOtherMsg)
{
	// 最終的なログ出力部分のフォーマットをここで設定
	CString strPrintMsg;
	if(lOID > 0)
		strPrintMsg.Format(_T("%s\t%s\t%ld\t%ld\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, lOID, errCode, GetErrCodeMsg(errCode), strOtherMsg);
	else
		strPrintMsg.Format(_T("%s\t%s\t\t%ld\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, errCode, GetErrCodeMsg(errCode), strOtherMsg);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					const CString& strMsg)
{
	if(bCout)
		std::cout << CT2A(strMsg) << std::endl;

	if(bCerr)
		std::cerr << CT2A(strMsg) << std::endl;

	if(bRun)
		m_ofsRunLog << CT2A(strMsg) << std::endl;

	if(bErr)
		m_ofsErrLog << CT2A(strMsg) << std::endl;
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
