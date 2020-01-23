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
#include "CorrectHighwayText.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ErrorObject2.h>
#include <RowContainer.h>
#include <TableContainer.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/scope_guard.h>
#include <boost/assign.hpp>

using namespace std;
using namespace sindy;

// 実行
bool CorrectHighwayText::execute()
{
	using namespace sindy::schema;
	// 初期化
	if (!init())
		return false;

	// 編集開始
	if (!startEdit())
	{
		RUNLOGGER.Error(_T("編集開始に失敗しました"));
		RUNLOGGER.writeExitLog(false);
		return false;
	}
	// HIGHWAY_TEXTの整合合わせ処理
	if (!correctHwyText())
	{
		stopEdit(false);
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	// 編集終了
	if (!stopEdit(true))
	{
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	RUNLOGGER.writeExitLog(true);
	return true;
}

// 初期化
bool CorrectHighwayText::init()
{
	if(!RUNLOGGER.Initialize(m_args))
	{
		cerr << "実行ログファイルを開けませんでした\t" << uh::toStr(m_args.m_runlog.c_str()) << endl;
		return false;
	}

	if (!m_tgtWork.Connect(m_args.m_target.c_str()))
	{
		cerr << "処理対象接続先を開けませんでした\t" << uh::toStr(m_args.m_target.c_str()) << endl;
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	return true;
}

// 編集開始
bool CorrectHighwayText::startEdit()
{
	// FGDB・PGDBなど
	if (!(m_tgtWork.GetNameString()->IsSDE()))
	{
		return (errorcode::sindyErrCode::sindyErr_NoErr == m_tgtWork.StartEditing(false));
	}

	// SDEの場合
	// バージョン対応の接続先のみOKとする
	{
		IVersionedWorkspacePtr ipVerWork((IWorkspace*)m_tgtWork);
		if (!ipVerWork)
		{
			RUNLOGGER.Error(_T("接続先がバージョン対応ではありません"));
			return false;
		}
		IVersionPtr ipVersion((IWorkspace*)m_tgtWork);
		CComBSTR bstr;
		ipVersion->get_VersionName(&bstr);
		CString strVerName = COLE2T(bstr);
		if (strVerName.Find(_T("DEFAULT")) != -1)
		{
			RUNLOGGER.Error(_T("接続先がDEFAULTバージョンです。"));
			return false;
		}

		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StartEditing(false))
		{
			return false;
		}
		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StartEditOperation())
		{
			m_tgtWork.StopEditing(false);
			return false;
		}
	}
	return true;
}

// 編集終了
bool CorrectHighwayText::stopEdit(bool bSave)
{
	if (m_tgtWork.GetNameString()->IsSDE())
	{
		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StopEditOperation())
		{
			RUNLOGGER.Error(_T("編集セッション終了に失敗しました"));
			bSave = false;
		}
	}
	if (!bSave)
		RUNLOGGER.Info(_T("編集を破棄します"));

	if (errorcode::sindyErr_NoErr != m_tgtWork.StopEditing(bSave))
	{
		RUNLOGGER.Error(_T("編集終了に失敗しました"));
		return false;
	}
	return true;
}

// 編集セッションを破棄する
bool CorrectHighwayText::abortEditOperation()
{
	if (m_tgtWork.GetNameString()->IsSDE() && errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.AbortEditOperation())
		return false;
	return true;
}

// HIGHWAY_TEXTの整合合わせ処理
bool CorrectHighwayText::correctHwyText()
{
	using namespace sindy::schema;
	// HIGHWAY_TEXTを開く
	CTableFinder finder;
	if (!initTableFinder(highway_text::kTableName, finder))
	{
		return false;
	}
	CSPTableContainer spHwyT(finder.FindTable(CString(highway_text::kTableName)));

	// 不正なPOINT_IDの値を修正する
	if (!correctIllegalPointID(spHwyT))
		return false;

	// IN句生成のため、mapのキーをvectorに格納
	std::vector<long> pointIDs;
	for (const auto& pointID : m_illegalPointIDs)
		pointIDs.emplace_back(pointID.first);

	// 不正なPOINT_IDと同じ値のPOINT_IDが存在するか
	// 存在する場合は、更新を行うとPOINT_IDに関連するフィールドの整合が合わなくなるので更新しない
	if (existBeforeEditPointID(spHwyT, pointIDs))
		return false;

	// 不正なPOINT_IDと同じ値を持っている関連フィールド群を修正する
	if (!correctRelPointIDFields(spHwyT, pointIDs))
		return false;

	return true;
}

// テーブルファインダ初期化
bool CorrectHighwayText::initTableFinder(const CString& table, CTableFinder& finder)
{
	CModel model;
	ITablePtr ipTable(m_tgtWork.OpenTable(table));
	if (!ipTable)
	{
		CString error = uh::str_util::format(_T("%sを開けませんでした"), table);
		RUNLOGGER.Error(uh::toTStr(error.GetString()));
		return false;
	}
	if (m_tgtWork.GetNameString()->IsSDE())
	{
		IVersionedObjectPtr ipVersionedObjectPtr(ipTable);
		VARIANT_BOOL isVersionRayer = VARIANT_FALSE;
		ipVersionedObjectPtr->get_IsRegisteredAsVersioned(&isVersionRayer);
		if (!isVersionRayer)
		{
			CString error = uh::str_util::format(_T("%sがバージョン対応レイヤではありません"), table);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			return false;
		}
	}

	finder.InitCollection(ipTable, model);
	return true;
}

// 不正なPOINT_IDの値を修正する
bool CorrectHighwayText::correctIllegalPointID(CSPTableContainer& spHwyTextT)
{
	using namespace sindy::schema;
	uh::scope_guard guard([&spHwyTextT] { spHwyTextT->clear(); });

	// OBJECTIDとPOINT_IDの値が異なるオブジェクトは不正なデータ
	CString whereClause = uh::str_util::format(_T("%s<>%s"), kObjectID, highway_text::kPointID);
	spHwyTextT->clear();

	IQueryFilterPtr ipQuery(AheInitQueryFilter(nullptr, _T(""), whereClause));
	spHwyTextT->_Select(ipQuery, false, false);
	spHwyTextT->CreateCache();

	bool good = true;
	for (auto& con : *spHwyTextT)
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		long oid = row->GetOID();
		long beforePointID = row->GetValue(highway_text::kPointID, -1L);
		if (-1 == beforePointID)
		{
			// POINT_IDの値を取得できなければエラー
			CString error = uh::str_util::format(_T("%s[%ld]の%sの値を取得できませんでした"), kObjectID, oid, highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}

		// 不正なPOINT_IDと同じ値を持っている関連フィールドの値を更新するために、値をキャッシュしておく
		if (!m_illegalPointIDs.insert(std::make_pair(beforePointID, oid)).second)
		{
			// 既に同じPOINT_IDの値を処理していた場合、データとしておかしいのでエラー
			CString error = uh::str_util::format(_T("%s[%ld]と%s[%ld]が同じ%sの値を持っています"), kObjectID, oid, kObjectID, m_illegalPointIDs[beforePointID], highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}
		// OBJECTIDの値で更新する。
		if (errorcode::sindyErr_NoErr != row->SetValue(highway_text::kPointID, oid))
		{
			CString error = uh::str_util::format(_T("%s[%ld]の%sの更新に失敗しました"), kObjectID, oid, highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}
		CString info = uh::str_util::format(_T("%s[%ld]の%sの値を更新します。%ld -> %ld"),
			kObjectID, oid, highway_text::kPointID, beforePointID, oid);
		RUNLOGGER.Info(uh::toTStr(info.GetString()));

	}
	
	// 保存
	if (!good || !store(spHwyTextT))
		return false;

	return true;
}

bool CorrectHighwayText::existBeforeEditPointID(CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs)
{
	using namespace sindy::schema::highway_text;
	bool isExist = false;
	std::list<long> listPIDs(pointIDs.begin(), pointIDs.end());
	spHwyTextT->clear();
	spHwyTextT->_Select(listPIDs, kPointID, true, false);
	// 検索されたレコードは確認が必要
	for (auto& con : *spHwyTextT)
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		long pid = row->GetValue(kPointID, -1L);
		CString error = uh::str_util::format(_T("不正な%sの値と同じ値を持っているレコードが存在します。%s[%ld]の%s[%ld]"),
			kPointID, kObjectID, row->GetOID(), kPointID, pid);
		RUNLOGGER.Error(uh::toTStr(error.GetString()));
		isExist = true;
	}
	spHwyTextT->clear();
	return isExist;
}

// 不正なPOINT_IDと同じ値を持っている関連フィールド群を修正する
bool CorrectHighwayText::correctRelPointIDFields(CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs)
{
	using namespace sindy::schema::highway_text;
	// POINT_IDと関連のあるフィールド群
	std::vector<CString> targetFields = boost::assign::list_of
		(kNextID)
		(kPrevID)
		(kParentID)
		(kForeID1)
		(kForeID2)
		(kForeID3)
		(kForeID4)
		(kBackID1)
		(kBackID2)
		(kBackID3)
		(kBackID4)
		;

	bool good = true;
	std::list<long> listPIDs(pointIDs.begin(), pointIDs.end());
	// フィールド単位でループ
	for (const auto& field : targetFields)
	{
		// POINT_ID群単位でループ
		spHwyTextT->clear();
		spHwyTextT->_Select(listPIDs, field, false, false);
		for (auto& con : *spHwyTextT)
		{
			CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
			// GetValue()する値はWHERE句の条件なので、値が入っているかのチェックは省略
			long fieldVal = row->GetValue(field, -1L);
			// m_illegalPointIDsのキーを基に検索してるので、対応する値は必ずある
			long setVal = m_illegalPointIDs[fieldVal];

			// 関連フィールドの値を更新
			if (errorcode::sindyErr_NoErr != row->SetValue(field, setVal))
			{
				CString error = uh::str_util::format(_T("%s[%ld]の%sの更新に失敗しました"), kObjectID, row->GetOID(), field);
				RUNLOGGER.Error(uh::toTStr(error.GetString()));
				good = false;
				continue;
			}
			CString info = uh::str_util::format(_T("%s[%ld]の%sの値を更新します。%ld -> %ld"),
				kObjectID, row->GetOID(), field, fieldVal, setVal);
			RUNLOGGER.Info(uh::toTStr(info.GetString()));
		}
		// 保存
		if (!store(spHwyTextT))
			good = false;

	}

	spHwyTextT->clear();

	return good;
}

bool CorrectHighwayText::store(sindy::CSPTableContainer& spHwyTextT)
{
	// 編集保存
	CErrorObjectsBase errs;
	if (errorcode::sindyErr_NoErr != spHwyTextT->Store(errs))
	{
		RUNLOGGER.Error(_T("編集保存に失敗しました。"));
		return false;
	}
	return true;
}
