#include "stdafx.h"
#include "CheckTableAndMaster.h"

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTableAndMasterのエラーコードは002xxxとする
	const CString DoesNotExistInMasterAdr11 = _T("002001");
	const CString DoesNotExistInMasterAdr5 = _T("002002");
}

/**
 * @brief 住所コードのセットに差があるかどうかを調べる
 * @param masterRecords [in] 住所マスタの住所コード
 * @param addrCodes [in] 差分を調べたい住所コード
 * @param diffCodes [in,out] 住所コードの差分
 * @param fieldName [in] 住所マスタの住所コードフィールド名
 * @retval true 差分なし
 * @retval false 差分あり
 */
bool HasAddrDiff(
	addr::CJusyoRecordSet& masterRecords,
	const std::set<CString>& addrCodes,
	std::set<CString>& diffCodes,
	const CString& fieldName)
{
	// 必要な分だけCJusyoRecordからstd::setへ格納
	std::set<CString> masterAddrCodes;
	masterRecords.begin();
	while (!masterRecords.end())
	{
		masterAddrCodes.insert(masterRecords.GetStringValue(fieldName));
		masterRecords.next();
	}

	// addrCodesにあってmasterにないものをdiffCodesに入れる
	std::set_difference(
		addrCodes.begin(), addrCodes.end(),
		masterAddrCodes.begin(), masterAddrCodes.end(),
		std::inserter(diffCodes, diffCodes.end()));

	// diffCodesが空(差分なし)ならtrue
	return (diffCodes.empty());
}

CheckTableAndMaster::CheckTableAndMaster()
{
}

CheckTableAndMaster::~CheckTableAndMaster()
{
}

bool CheckTableAndMaster::Init()
{
	// 住所マスタ取得
	if (!m_jusyoMasterHelper.Connect(m_args.m_jusyoMaster.c_str()))
	{
		RUNLOGGER.Error(_T("住所マスタ接続に失敗"));
		return false;
	}

	// 加除マスタ取得
	if (!m_kajyoMasterHelper.Connect(m_args.m_kajyoMaster.c_str()))
	{
		RUNLOGGER.Error(_T("加除マスタ接続に失敗"));
		return false;
	}

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

bool CheckTableAndMaster::Check()
{
	RUNLOGGER.Info(_T("住所紐付けテーブルと住所マスタの整合チェック実行…\n"));
	std::cout << "住所紐付けテーブルと住所マスタの整合チェック実行…" << std::endl;

	RUNLOGGER.Info(_T("11桁チェック中…\n"));
	std::cout << "11桁チェック中…" << std::endl;
	bool result = CheckExistAddrCode();

	RUNLOGGER.Info(_T("5桁チェック中…\n"));
	std::cout << "5桁チェック中…" << std::endl;
	result &= CheckExistCityCode();

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;
	return result;
}

bool CheckTableAndMaster::CheckExistAddrCode()
{
	// [チェック] 紐付けテーブルの住所コード(11桁)が住所マスタ、加除マスタ上に存在しているか確認
	// 紐付けテーブルから11桁取得

	CString whereClause;
	whereClause.Format(_T("%s <> %ld"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("カーソル取得失敗"));
		return false;
	}

	_IRowPtr ipRow;
	std::set<CString> addrCodes;
	std::map<CString, std::vector<long>> addrCodeIds;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		addrCodes.emplace(CString(addrCode.bstrVal).Left(11));
		addrCodeIds[CString(addrCode.bstrVal).Left(11)].emplace_back(oid);
	}

	// 住所マスタにいるものは除外
	CString queryString;
	queryString.Append(_T("SELECT (ken_code || shi_code || oaza_code || aza_code) as ADDRCODE "));
	queryString.Append(_T("FROM   areacode_master "));
	addr::CJusyoRecordSet masterRecords;
	if (!m_jusyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("住所マスタ取得失敗\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("住所マスタ取得失敗\n"));
		return false;
	}

	std::set<CString> diffMaster;
	if (HasAddrDiff(masterRecords, addrCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// 残りものが加除マスタにいることを確認
	addrCodes = diffMaster;
	diffMaster.clear();
	if (!m_kajyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("加除マスタ取得失敗\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("加除マスタ取得失敗\n"));
		return false;
	}

	if (HasAddrDiff(masterRecords, addrCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// 最後まで残ってるやつはエラー
	for (auto& addrcode : diffMaster)
	{
		for (auto& oid : addrCodeIds[addrcode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInMasterAdr11,
				_T("住所コード（11/16/20桁）がマスタに存在しない"),
				addrcode
			);
		}
	}
	return false;
}

bool CheckTableAndMaster::CheckExistCityCode()
{
	// [チェック] 紐付けテーブルの住所コード(5桁)が住所マスタ上に存在し、かつ生存しているか確認
	// 紐付けテーブルから5桁取得

	CString whereClause;
	whereClause.Format(_T("%s = %ld"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::Used5digit);

	_ICursorPtr ipCursor;
	auto query = AheInitQueryFilter(nullptr, nullptr, whereClause);
	if (S_OK != m_ipRelAddrTable->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		RUNLOGGER.Error(_T("カーソル取得失敗"));
		return false;
	}

	_IRowPtr ipRow;
	std::set<CString> cityCodes;
	std::map<CString, std::vector<long>> cityCodeIds;
	long idxAddrCode = m_relAddrTableFieldMap.FindField(rel_addr::kAddrCode);
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
	{
		long oid = 0;
		ipRow->get_OID(&oid);
		CComVariant addrCode;
		ipRow->get_Value(idxAddrCode, &addrCode);
		cityCodes.emplace(CString(addrCode.bstrVal));
		cityCodeIds[CString(addrCode.bstrVal)].emplace_back(oid);
	}

	// 住所マスタにいるものは除外
	CString queryString;
	queryString.Append(_T("SELECT distinct (new_ken_code || new_shi_code) as ADDRCODE "));
	queryString.Append(_T("FROM   areacode_master "));
	queryString.Append(_T("WHERE  haishi_date = '000000' "));
	queryString.Append(_T("AND    new_oaza_code = '000' AND new_aza_code = '000' "));
	queryString.Append(_T("AND    new_shi_code != '000' "));
	addr::CJusyoRecordSet masterRecords;
	if (!m_jusyoMasterHelper.Select(queryString, masterRecords))
	{
		RUNLOGGER.Error(_T("住所マスタ取得失敗\n"));
		return false;
	}
	if (0 == masterRecords.GetRecordCount())
	{
		RUNLOGGER.Error(_T("住所マスタ取得失敗\n"));
		return false;
	}

	std::set<CString> diffMaster;
	if (HasAddrDiff(masterRecords, cityCodes, diffMaster, _T("ADDRCODE")))
		return true;

	// 最後まで残ってるやつはエラー
	for (auto& citycode : diffMaster)
	{
		for (auto& oid : cityCodeIds[citycode])
		{
			ERRLOGGER.writeStdLog(
				rel_addr::kTableName20,
				oid,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::DoesNotExistInMasterAdr5,
				_T("住所コード（5桁）がマスタに存在しない"),
				citycode
			);
		}
	}
	return false;
}
