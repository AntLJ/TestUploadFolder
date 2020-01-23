#include "stdafx.h"
#include "CheckPolygonAndTable.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckPolygonAndTableのエラーコードは005xxxとする
	const CString DoesNotExistInRelAddr = _T("005001");
	const CString DoesNotExistInZipArea = _T("005002");
}

CheckPolygonAndTable::CheckPolygonAndTable()
{
}

CheckPolygonAndTable::~CheckPolygonAndTable()
{
}

bool CheckPolygonAndTable::Init()
{
	// SDE(郵便番号ポリゴン)接続
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンDB接続先への接続に失敗"));
		return false;
	}

	// 紐付けテーブル取得
	m_ipRelAddrTable = zipWorkspace.OpenTable(rel_addr::kTableName20);
	if (!m_ipRelAddrTable)
	{
		RUNLOGGER.Error(_T("住所紐付けテーブル取得失敗"));
		return false;
	}

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

	// 郵便番号ポリゴン取得
	m_ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);
	if (!m_ipZipCodeArea)
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィーチャクラス取得失敗\n"));
		return false;
	}

	m_ipZipCodeArea->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィールド対応表取得失敗\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_zipCodeAreaFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンフィールド対応表取得失敗\n"));
		return false;
	}

	return true;
}

bool CheckPolygonAndTable::Check()
{
	RUNLOGGER.Info(_T("ポリゴンと紐付けテーブルの整合チェック実行…\n"));
	std::cout << "ポリゴンと紐付けテーブルの整合チェック実行…" << std::endl;

	bool result = CheckZipCode();

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;

	return result;
}

bool CheckPolygonAndTable::CheckZipCode()
{
	// [チェック] 郵便番号ポリゴンの持つ郵便番号と紐付けテーブルの持つ郵便番号の整合チェック
	// 郵便番号ポリゴンの持つ郵便番号を取得
	IFeatureCursorPtr ipCursor;
	auto subFields = uh::str_util::format(
		_T("%s, %s, %s"),
		zipcode_area::kObjectID, zipcode_area::kZipCode1, zipcode_area::kZipCode2);
	auto query = AheInitQueryFilter(nullptr, subFields);
	if ( S_OK != m_ipZipCodeArea->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンカーソル取得失敗\n"));
		return false;
	}

	long idxZipCode1 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode1);
	long idxZipCode2 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode2);
	std::set<CString> polygonZipCodes;
	std::map<CString, std::vector<long>> polygonZipOids; // 郵便番号とOID対応表(ログ用)
	IFeaturePtr ipFeature;
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long oid = 0;
		ipFeature->get_OID(&oid);

		CComVariant zipcode1, zipcode2;
		ipFeature->get_Value(idxZipCode1, &zipcode1);
		ipFeature->get_Value(idxZipCode2, &zipcode2);

		polygonZipCodes.insert(zipcode1.bstrVal);
		polygonZipOids[zipcode1.bstrVal].push_back(oid);
		if (!((CString)zipcode2.bstrVal).IsEmpty())
		{
			polygonZipCodes.insert(zipcode2.bstrVal);
			polygonZipOids[zipcode2.bstrVal].push_back(oid);
		}
	}

	// 紐付けテーブルの持つ郵便番号を取得
	subFields = uh::str_util::format(
		_T("%s, %s"),
		ipc_table::kObjectID, rel_addr::kZipCode);
	auto whereClause = uh::str_util::format(_T("%s = %ld"),
		rel_addr::kModifyStatus, rel_addr::ModifyStatusCode::Used11digit);
	query = AheInitQueryFilter(nullptr, subFields, whereClause);
	_ICursorPtr ipTbCursor;
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipTbCursor) || !ipTbCursor)
	{
		RUNLOGGER.Error(_T("住所紐付けテーブルカーソル取得失敗\n"));
		return false;
	}

	std::set<CString> tableZipCodes;
	std::map<CString, std::vector<long>> tableZipOids; // 郵便番号とOID対応表(ログ用)
	_IRowPtr ipRow;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kZipCode);
	while (S_OK == ipTbCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);

		CComVariant zipcode;
		ipRow->get_Value(idxAddrCode, &zipcode);
		tableZipCodes.insert(zipcode.bstrVal);
		tableZipOids[zipcode.bstrVal].push_back(oid);
	}

	// 突合
	std::set<CString> polygonDiff;
	std::set<CString> tableDiff;
	std::set_difference(
		polygonZipCodes.begin(), polygonZipCodes.end(),
		tableZipCodes.begin(), tableZipCodes.end(),
		std::inserter(polygonDiff, polygonDiff.end()));
	std::set_difference(
		tableZipCodes.begin(), tableZipCodes.end(),
		polygonZipCodes.begin(), polygonZipCodes.end(),
		std::inserter(tableDiff, tableDiff.end()));

	// エラー判定
	bool hasError = false;
	if (!polygonDiff.empty() || !tableDiff.empty())
		hasError = true;

	// エラー出力
	for (auto& zipcode : polygonDiff)
	{
		for (auto& oid : polygonZipOids[zipcode])
		{
			ERRLOGGER.writeStdLog(
				zipcode_area::kTableName,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInRelAddr,
				_T("郵便番号ポリゴンにあるが紐付けテーブルに存在しない郵便番号"),
				zipcode
			);
		}
	}

	for (auto& zipcode : tableDiff)
	{
		for (auto& oid : tableZipOids[zipcode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInZipArea,
				_T("紐付けテーブルにあるが郵便番号ポリゴンに存在しない郵便番号"),
				zipcode
			);
		}
	}

	return !hasError;
};