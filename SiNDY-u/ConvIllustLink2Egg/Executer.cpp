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

#include "LogSys.h"
#include "common.h"
#include "TransParam.h"
#include "PgDataBaseEx.h"
#include "IllConverter.h"

#include <WinLib/CStringTokenizer.h>

/**
 *	@brief	イラストファイル名対応表取得関数
 *	@param	inputListPath	[in] イラストファイル名対応表パス
 *	@param	inputListInfo	[out] イラストファイル名対応表内容格納コンテナ
 *	@return	true 成功
 *	@return false 失敗
 */
bool get_input_list(LPCTSTR inputListPath, std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo)
{
	CString errMsg;
	inputListInfo.clear();
	// ファイルオープン
	std::ifstream ifs(inputListPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%sの読み込みに失敗しました"), inputListPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	long count = 0;
	while (std::getline(ifs, temp)) {
		++count;
		CString line = CA2CT(temp.c_str());
		line.Trim();
		// レコードが空 or 先頭が#の場合はcontinue
		if (line.IsEmpty() || _T("#") == line.Left(1))
			continue;
		// タブで分割
		CStringTokenizer cTokenizer(line, _T("\t"), TOKEN_RET_EMPTY_ALL);
		if (ILLFILEFNUM != cTokenizer.CountTokens()) {
			errMsg.Format(_T("%sのフォーマットが不正です。:%d行"), inputListPath, count);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		// 各属性取得
		// フォーマット：
		// イラスト種別コード\t仕向け先コード\tイラストOBJECTID\t元イラスト名\tリリース用イラスト名
		CString illCls = cTokenizer.GetNextToken();
		CString dstCode = cTokenizer.GetNextToken();
		ClassCode classCode(illCls, dstCode);
		OID illId = _ttol(cTokenizer.GetNextToken());
		IllFileNames illFileNames;
		illFileNames.e_orgIllName = cTokenizer.GetNextToken();
		illFileNames.e_tgtIllName = cTokenizer.GetNextToken();
		// コンテナへの格納
		std::map<ClassCode, std::map<OID, IllFileNames>>::iterator clsCodeItr = inputListInfo.find(classCode);
		if (inputListInfo.end() == clsCodeItr) {
			std::map<OID, IllFileNames> id2names;
			id2names.insert(std::pair<OID, IllFileNames>(illId, illFileNames));
			inputListInfo.insert(std::pair<ClassCode, std::map<OID, IllFileNames>>(classCode, id2names));
		} else {
			std::map<OID, IllFileNames>::_Pairib pib;
			pib = clsCodeItr->second.insert(std::pair<OID, IllFileNames>(illId, illFileNames));
			if (! pib.second) {
				errMsg.Format(_T("一つのobjectidに対して複数のリリース用イラストファイル名が関連付いています。：対象イラストid：%d"), illId);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
				return false;
			}
		}
		
	}
	return true;
}

/**
 *	@brief	イラスト処理情報設定ファイル取得関数
 *	@param	illSetPath	[in] イラスト処理情報設定ファイルパス
 *	@param	illSetInfo	[out] イラスト処理情報設定ファイル内容格納コンテナ
 *	@return true 成功
 *	@return false 失敗
 */
bool get_ill_set(LPCTSTR illSetPath, std::vector<IllSetRecord>& illSetInfo)
{
	CString errMsg;
	illSetInfo.clear();
	// ファイルオープン
	std::ifstream ifs(illSetPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%sの読み込みに失敗しました"), illSetPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	long count = 0;
	while (std::getline(ifs, temp)) {
		++count;
		CString line = CA2CT(temp.c_str());
		// レコードが空 or 先頭が#の場合はcontinue
		if (line.IsEmpty() || _T("#") == line.Left(1) )
			continue;
		// タブで分割
		CStringTokenizer cTokenizer(line, _T("\t"), TOKEN_RET_EMPTY_ALL);
		if (ILLSETFNUM != cTokenizer.CountTokens()) {
			errMsg.Format(_T("%sのフォーマットが不正です。:%d行"), illSetPath, count);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		IllSetRecord illSetRecord;
		// 各属性取得
		// フォーマット
		// イラスト種別コード\t仕向け先コード\tプレフィックス\t拡張子\tファイル命名規則\t格納ディレクトリ
		illSetRecord.e_illCls = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_dstCode = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_prefix = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_suffix = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_nameRule = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_illDir = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_excCond = CString(cTokenizer.GetNextToken()).Trim();
		// コンテナへ挿入
		illSetInfo.push_back(illSetRecord);
	}
	return true;
}

bool exec(TransParam &transParam)
{
	CString errMsg;
	// イラスト処理情報設定ファイル読み込み
	LogSys::GetInstance().WriteProcLog(true, true, _T("イラスト処理情報設定ファイル読み込み\n"));
	std::vector<IllSetRecord> illSetInfo;
	if (! get_ill_set(transParam.GetIllSetFile(), illSetInfo))
		return false;
	// イラストファイル名対応表読み込み
	LogSys::GetInstance().WriteProcLog(true, true, _T("イラストファイル名対応表読み込み\n"));
	std::map<ClassCode, std::map<OID, IllFileNames>> inputListInfo;
	if (! get_input_list(transParam.GetInputList(), inputListInfo))
		return false;
	// DB接続
	LogSys::GetInstance().WriteProcLog(true, true, _T("DB接続\n"));
	std::shared_ptr<PgDataBaseEx> orgDataBase(new PgDataBaseEx(CT2CA(transParam.GetRepDB())));
	std::shared_ptr<PgDataBaseEx> tgtDataBase(new PgDataBaseEx(CT2CA(transParam.GetEggDB())));
	if (! orgDataBase->connected()) {
		errMsg.Format(_T("DB接続に失敗しました：%s"), transParam.GetRepDB());
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	if (! tgtDataBase->connected()) {
		errMsg.Format(_T("DB接続に失敗しました:%s"), transParam.GetEggDB());
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 変換
	LogSys::GetInstance().WriteProcLog(true, true, _T("変換開始\n"));
	IllConverter illConverter(orgDataBase, 
							  tgtDataBase, 
							  transParam.GetRepSchema(), 
							  transParam.GetEggSchema());
	if (! illConverter.Convert(illSetInfo, inputListInfo))
		return false;
	LogSys::GetInstance().WriteProcLog(true, true, _T("変換完了\n"));

	return true;
}
