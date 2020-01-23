#include "stdafx.h"
#include "CheckTable.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTableのエラーコードは001xxxとする
	const CString DuplicationOfAddrCode = _T("001001");
	const CString IllegalModifyStatus = _T("001002");
}

CheckTable::CheckTable()
{
}

CheckTable::~CheckTable()
{
}

bool CheckTable::Init()
{
	RUNLOGGER.Info(_T("住所紐付けテーブルチェック初期化…\n"));

	// SDE(郵便番号ポリゴン)接続
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンDB接続先への接続に失敗\n"));
		return false;
	}

	// 紐付けテーブル取得
	m_ipRelAddrTable = zipWorkspace.OpenTable(rel_addr::kTableName20);
	if (!m_ipRelAddrTable)
	{
		RUNLOGGER.Error(_T("住所紐付けテーブル取得失敗\n"));
		return false;
	}

	// フィールドマップ生成
	IFieldsPtr ipFields;
	m_ipRelAddrTable->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("住所紐付けテーブルフィールド対応表取得失敗\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_relAddrTableFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("住所紐付けテーブルフィールド対応表取得失敗\n"));
		return false;
	}

	return true;
}

bool CheckTable::Check()
{
	RUNLOGGER.Info(_T("住所紐付けテーブルチェック実行…\n"));
	std::cout << "住所紐付けテーブルチェック実行…" << std::endl;
	bool result = true;

	RUNLOGGER.Info(_T("住所紐付けテーブル重複チェック\n"));
	std::cout << "住所紐付けテーブル重複チェック" << std::endl;
	result &= CheckAddrCodeDuplication();

	RUNLOGGER.Info(_T("住所紐付けテーブル整備状況コードチェック\n"));
	std::cout << "住所紐付けテーブル整備状況コードチェック" << std::endl;
	result &= CheckModifyStatusCode();

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;

	return result;
}


bool CheckTable::CheckAddrCodeDuplication()
{
	// [チェック1] テーブルの住所コードに重複がないことを確認する
	// 整備状況「廃止」は除外
	auto whereClause = uh::str_util::format(
		_T("%s <> %ld"), rel_addr::kModifyStatus, rel_addr::ModifyStatusCode::Obsolete);
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	_ICursorPtr ipCursor;
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("カーソル取得失敗\n"));
		return false;
	}

	// とりあえず全部取ってくる
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	std::map<CString, std::pair<long, std::vector<long>>> addrCodes; // key:住所コード、val:重複カウント、oid
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		CString strAddrCode(addrCode);
		if (strAddrCode.IsEmpty())
			continue;

		++addrCodes[strAddrCode].first;
		long oid = 0;
		ipRow->get_OID(&oid);
		addrCodes[strAddrCode].second.push_back(oid);
	}

	bool hasError = false;
	for (const auto& dat : addrCodes)
	{
		if (1 < dat.second.first)
		{
			// エラー出力
			for (auto oid : dat.second.second)
			{
				ERRLOGGER.writeStdLog(
					rel_addr::kTableName20,
					oid,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DuplicationOfAddrCode,
					_T("住所コード重複"),
					dat.first
				);
			}
			hasError = true;
		}
	}
	return !hasError;
}

bool CheckTable::CheckModifyStatusCode()
{
	// [チェック2] 整備状況コードが１，２，３，４であることを確認する
	// １，２，３，４以外で検索してみる
	CString whereClause;
	whereClause.Format(_T("%s not in (%d, %d, %d, %d)"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Obsolete,
		rel_addr::ModifyStatusCode::UnUsed,
		rel_addr::ModifyStatusCode::Used11digit,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("カーソル取得失敗\n"));
		return false;
	}

	// 見つかったやつは全部エラー
	bool hasError = false;
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	long idxModifyStatus = m_relAddrTableFieldMap.FindField(rel_addr::kModifyStatus);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrcode, modifyStatusCode;
		ipRow->get_Value(idxAddrCode, &addrcode);
		ipRow->get_Value(idxModifyStatus, &modifyStatusCode);
		ERRLOGGER.writeStdLog(
			rel_addr::kTableName20,
			oid,
			_T(""),
			_T(""),
			_T("ERROR"),
			ErrorCode::IllegalModifyStatus,
			_T("不正な整備状況コード"),
			uh::str_util::format(_T("%s\t%d"), addrcode.bstrVal, modifyStatusCode.lVal)
		);
		hasError = true;
	}
	return !hasError;
};