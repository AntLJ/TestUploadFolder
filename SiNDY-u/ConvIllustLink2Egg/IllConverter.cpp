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
#include "IllConverter.h"
#include "LogSys.h"

#include <sindy/schema/egg.h>

using namespace egg::schema;

IllConverter::IllConverter(std::shared_ptr<PgDataBaseEx> orgDataBase,
						   std::shared_ptr<PgDataBaseEx> tgtDataBase, 
						   LPCTSTR orgSchema, 
						   LPCTSTR tgtSchema)
	:m_orgDataBase(orgDataBase),m_tgtDataBase(tgtDataBase)
{
	m_illustLink.Format(_T("%s.%s"), orgSchema, sindy::schema::illust_link::kTableName);
	m_roadLink.Format(_T("%s.%s"), orgSchema, sindy::schema::road_link::kTableName);
	m_infIllustLink.Format(_T("%s.%s"), tgtSchema, ::inf_illust::kTableName);
	m_lqIllustLink.Format(_T("%s.%s"), tgtSchema, ::lq_illust::kTableName);
	m_infFields.Format(_T("%s, %s, %s"), ::inf_illust::kObjectId, ::inf_illust::kIllust, ::inf_illust::kFileName);
	m_lqFields.Format(_T("%s, %s, %s, %s"), ::lq_illust::kInfId, ::lq_illust::kLinkId, ::lq_illust::kLinkDir, ::lq_illust::kSequence);
}

IllConverter::~IllConverter(void)
{
}

bool IllConverter::Convert(const std::vector<IllSetRecord>& illSetInfo, const std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo)
{
	CString errMsg;
	bool retVal = true;
	CString procMsg;
	// 変換元テーブル存在チェック
	if (! m_orgDataBase->exists(ns::Database::TABLE, CT2CA(m_illustLink))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("変換元DBにテーブルが存在しません"), m_illustLink);
		return false;
	}
	// 出力テーブル作成
	LogSys::GetInstance().WriteProcLog(true, true, _T("出力テーブル作成\n"));
	if (! CreateIllTables())
		return false;
	//イラスト処理情報設定ファイルのレコード単位で処理を実施
	for (const auto& illSet :illSetInfo) {
		procMsg.Format(_T("%s = %s, %s = %s\n"),  sindy::schema::illust_link::kIllustClass, illSet.e_illCls, sindy::schema::illust_link::kCustomerCode, illSet.e_dstCode);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// 対象レコード群取得
		ns::RowsPtr rows;
		if (! GetIllLinkRows(illSet.e_illCls, illSet.e_dstCode, rows)) {
			retVal = false;
			continue;
		}
		std::vector<ns::RowPtr> tgtRows;
		ns::RowPtr row;
		while (row = rows->next()) {
			tgtRows.push_back(row);
		}
		// 対象レコードが0の場合は、エラーとする。(コード値ドメイン外の値がイラスト処理情報設定ファイルで指定されている可能性があるため)
		if (!tgtRows.size()) {
			errMsg.Format(_T("%s = %s, %s = %sの条件に関連付くリンクが存在しません。処理情報設定ファイルに問題がが無いか確認してください。\n"), sindy::schema::illust_link::kIllustClass, illSet.e_illCls, sindy::schema::illust_link::kCustomerCode, illSet.e_dstCode);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			retVal = false;
			continue;
		}
		// イラストリンクID⇔イラスト画像名対応表取得
		ClassCode classCode(illSet.e_illCls, illSet.e_dstCode);
		std::map<ClassCode, std::map<OID, IllFileNames>>::const_iterator itr = inputListInfo.find(classCode);
		if (inputListInfo.end() == itr) {
			errMsg.Format(_T("%s = %d, %s = %d　のイラスト画像名対応表が存在しません"), sindy::schema::illust_link::kIllustClass, classCode.e_illCls, sindy::schema::illust_link::kCustomerCode, classCode.e_dstCode);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::illust_link::kTableName);
			retVal = false;
			continue;
		}
		// 変換処理
		std::map<OID,InfIllInfo> infIllInfos;
		std::map<LqIllKey, LqIllInfo> lqIllInfos;
		if (! m_illCooker.Cook(m_orgDataBase, 
							   tgtRows,
							   itr->second,
							   m_roadLink,
							   infIllInfos,
							   lqIllInfos)) {
			retVal = false;
			continue;			   
		}
		// レコード情報→テキスト
		std::string infCopyValues, lqCopyValues;
		Info2Str(infIllInfos, lqIllInfos, infCopyValues, lqCopyValues);
		// インポート処理
		if (! importValues(CT2CA(m_infIllustLink), CT2CA(m_infFields), infCopyValues)) {
			retVal = false;
			continue;
		}
		if (! importValues(CT2CA(m_lqIllustLink), CT2CA(m_lqFields), lqCopyValues)) {
			retVal = false;
			continue;
		}
	}
	return retVal;
}
bool IllConverter::CreateIllTables()
{
	// テーブル存在チェック(既に祖納する場合は削除する。
	if (m_tgtDataBase->exists(ns::Database::TABLE, CT2CA(m_infIllustLink))) {
		if (! DeleteTable(m_tgtDataBase, m_infIllustLink))
			return false;
	}
	if (m_tgtDataBase->exists(ns::Database::TABLE, CT2CA(m_lqIllustLink))) {
		if (! DeleteTable(m_tgtDataBase, m_lqIllustLink))
			return false;
	}
	// SQL作成
	// フィールド情報
	CString infFields, lqFields;
	infFields.Format(_T("%s integer NOT NULL, %s integer NOT NULL, %s varchar(10) NOT NULL"), ::inf_illust::kObjectId, ::inf_illust::kIllust, ::inf_illust::kFileName);
	lqFields.Format(_T("%s integer NOT NULL, %s integer NOT NULL, %s integer NOT NULL, %s integer NOT NULL"), ::lq_illust::kInfId, ::lq_illust::kLinkId, ::lq_illust::kLinkDir, ::lq_illust::kSequence);
	// Create文作成
	CString makeInf, makeLq;
	makeInf.Format(_T("CREATE TABLE %s ( %s, CONSTRAINT %s_pkey PRIMARY KEY (%s) )"), m_infIllustLink, infFields, ::inf_illust::kTableName, ::inf_illust::kObjectId);
	makeLq.Format(_T("CREATE TABLE %s ( %s, CONSTRAINT %s_pkey PRIMARY KEY (%s,\"%s\") )"), m_lqIllustLink, lqFields, ::lq_illust::kTableName, ::lq_illust::kInfId, ::lq_illust::kSequence);
	// SQL実行
	if (!m_tgtDataBase->executeCommand(CT2CA(makeInf))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("テーブル作成に失敗しました"), m_infIllustLink);
		return false;
	}
	if (!m_tgtDataBase->executeCommand(CT2CA(makeLq))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("テーブル作成に失敗しました"), m_lqIllustLink);
		return false;
	}
	// index付与
	CString makeLqIndex;
	makeLqIndex.Format(_T("CREATE INDEX %s_%s ON %s (%s)"), ::lq_illust::kTableName, ::lq_illust::kLinkId, m_lqIllustLink, ::lq_illust::kLinkId);
	// SQL実行
	if (! m_tgtDataBase->executeCommand(CT2CA(makeLqIndex))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("インデックス付与に失敗しました"), m_lqIllustLink);
		return false;
	}
	return true;
}

bool IllConverter::DeleteTable(std::shared_ptr<PgDataBaseEx> dataBase, LPCTSTR tableName)
{
	CString deleteSql;
	deleteSql.Format(_T("DROP TABLE %s;"), tableName);
	if (!dataBase->executeCommand(CT2CA(deleteSql))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("既存テーブル削除に失敗しました"), tableName);
		return false;
	}
	return true;
}


bool IllConverter::GetIllLinkRows(LPCTSTR illClass, LPCTSTR dstCode, ns::RowsPtr& rows)
{
	// ILLUST_LINK取得SQL作成
	// 条件：illust_class_c=X AND customer_c=X
	CString getIllLink;
	getIllLink.Format(_T("SELECT * FROM %s WHERE %s = %s AND %s = %s"), m_illustLink, sindy::schema::illust_link::kIllustClass, illClass, sindy::schema::illust_link::kCustomerCode, dstCode);
	// rows取得
	rows = m_orgDataBase->execute(CT2CA(getIllLink));
	if (! rows) {
		CString errMsg;
		errMsg.Format(_T("データ抽出失敗:%s=%s and %s=%s"), sindy::schema::illust_link::kIllustClass, illClass, sindy::schema::illust_link::kCustomerCode, dstCode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::illust_link::kTableName);
		return false;
	}
	return true;
}


void IllConverter::Info2Str(const std::map<OID, InfIllInfo>& infIllInfos,
						   const std::map<LqIllKey, LqIllInfo>& lqIllInfos,
						   std::string& infCopyValues,
						   std::string& lqCopyValues)
{
	infCopyValues.clear();
	lqCopyValues.clear();
	for (const auto& infInfo : infIllInfos) {
		CString temp;
		temp.Format(_T("%d\t%d\t%s\n"), infInfo.first, infInfo.second.e_illustCode, infInfo.second.e_fileName);
		infCopyValues += CT2CA(temp);
	}
	for (const auto& lqInfo : lqIllInfos) {
		CString temp;
		temp.Format(_T("%d\t%d\t%d\t%d\n"), lqInfo.first.e_infId, lqInfo.second.e_linkId, lqInfo.second.e_linkDir, lqInfo.first.e_Seq);
		lqCopyValues += CT2CA(temp);
	}
}

bool IllConverter::importValues(LPCSTR tableName, LPCSTR fields, const std::string& importValues)
{
	// コピー開始
	if (! m_tgtDataBase->StartCopy(tableName, fields)) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	// コピー実施
	if (! m_tgtDataBase->copyData(importValues.c_str(), importValues.size())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	// コピー完了
	if (! m_tgtDataBase->EndCopy()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	return true;
}
