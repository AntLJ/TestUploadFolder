#include "stdafx.h"
#include "CheckUtil.h"
#include "CheckTableAndAddr.h"

using namespace addr;
using namespace sindy;
using namespace sindy::schema;

namespace ErrorCode
{
	// CheckTableAndAddrのエラーコードは003xxxとする
	const CString DoesNotExistInAddressData = _T("003001");
	const CString DoesNotExistInRelAddr = _T("003002");
}

CheckTableAndAddr::CheckTableAndAddr()
{
}

CheckTableAndAddr::~CheckTableAndAddr()
{
}

bool CheckTableAndAddr::Init()
{
	// SDE(郵便番号ポリゴン)接続
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンDB接続先への接続に失敗"));
		return false;
	}

	// 紐付けテーブルファインダ初期化
	CModel zipModel;
	m_relAddrFinder.InitCollection(zipWorkspace.OpenTable(rel_addr::kTableName20), zipModel);

	// SDE(行政界、号)接続
	sindy::CWorkspace addrWorkspace;
	if (!addrWorkspace.Connect(m_args.m_addrDb.c_str()))
	{
		RUNLOGGER.Error(_T("住所データDB接続先への接続に失敗"));
		return false;
	}

	// 住所データ(行政界、号)ファインダ初期化
	CModel addrModel;
	m_addrFinder.InitCollection(addrWorkspace.OpenTable(city_admin::kTableName), addrModel);

	// 文字付き住所コード変換テーブルファイル接続
	if (!m_addrRecHelper.connectCharConvFile(m_args.m_charConv.c_str()))
	{
		RUNLOGGER.Error(_T("文字付き変換テーブルへの接続に失敗"));
		return false;
	}

	// 文字付き住所コード変換仮テーブルファイル接続
	if (!m_addrRecHelper.connectCharConvTmpFile(m_args.m_charConvTmp.c_str()))
	{
		RUNLOGGER.Error(_T("文字付き変換仮テーブルへの接続に失敗"));
		return false;
	}

	return true;
}


bool CheckTableAndAddr::Check()
{
	RUNLOGGER.Info(_T("住所紐付けテーブルと住所データの整合チェック実行…\n"));
	std::cout << "住所紐付けテーブルと住所データの整合チェック実行…" << std::endl;

	bool result = true;
	RUNLOGGER.Info(_T("テーブルから住所データのチェック\n"));
	std::cout << "テーブルから住所データのチェック" << std::endl;
	result &= CheckTableToAddr();

	RUNLOGGER.Info(_T("行政界からテーブルのチェック\n"));
	std::cout << "行政界からテーブルのチェック" << std::endl;
	result &= CheckCityAdminToTable();

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;
	return result;
}

bool CheckTableAndAddr::CheckTableToAddr()
{
	CheckUtil util(m_addrRecHelper);

	// SiNDY住所データ
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& gouPointTbl = *m_addrFinder.FindTable(gou_point::kTableName);
	gouPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& csPointTbl = *m_addrFinder.FindTable(cs_addr_point::kTableName);
	csPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// 住所紐付けテーブル
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto query = AheInitQueryFilter(
		nullptr, nullptr,
		_T("%s NOT IN (%ld, %ld, %ld)"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::UnUsed,
		rel_addr::ModifyStatusCode::Obsolete,
		rel_addr::ModifyStatusCode::Used5digit);
	relAddrTbl._Select(query, false);

	// 11桁コード+名称(地番、号)の一覧を作成
	// 取れないものは仕方ないので、取れたものだけでチェック
	std::map<CString, AddrRecPairs> relAddrMap;
	if (!util.MakeAddrYomiList(relAddrTbl, relAddrMap))
	{
		RUNLOGGER.Error(_T("11桁コード＋名称の取得にエラーがあります。(エラーログ参照)\n"));
	}

	// CSは少ないので一旦全国分取ってしまう
	std::map<CString, AddrRecs> csAddrRecsMap;
	m_addrRecHelper.makeAdrRecordsFromCs(csPointTbl, _T(""), csAddrRecsMap);
	csPointTbl.clear();

	// 11桁ごとに処理
	for (auto & relAddrRecPairs : relAddrMap)
	{
		std::cout << "住所コード: " << CStringA(relAddrRecPairs.first).GetString() << " 処理中" << std::endl;

		// 11桁コードから行政界取得
		AddrRecPairs adminAddrRecs;
		m_addrRecHelper.getCityAdminRecs(cityAdminTbl, relAddrRecPairs.first, adminAddrRecs);
		cityAdminTbl.clear();

		// 11桁コードからCSポイント取得
		AddrRecs csAddrRecs;
		if (0 != csAddrRecsMap.count(relAddrRecPairs.first.Left(11)))
		{
			csAddrRecs = csAddrRecsMap[relAddrRecPairs.first.Left(11)];
		}

		// 存在判定
		for (auto& relAddrRec : relAddrRecPairs.second)
		{
			bool exist = util.CheckAddressDataExistance(
				adminAddrRecs, csAddrRecs, gouPointTbl, relAddrRec.first);

			if (!exist)
			{
				ERRLOGGER.writeStdLog(
					rel_addr::kTableName20,
					relAddrRec.second.GetOID(),
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotExistInAddressData,
					_T("住所コードに対応する行政界、号、CSが存在しない"),
					relAddrRec.first.getJusyocode()
				);
			}
		}
	}

	return true;
}

bool CheckTableAndAddr::CheckCityAdminToTable()
{
	// SiNDY住所データ
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// 住所紐付けテーブル
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	bool hasError = false;
	for (int i = 1; i <= 47; ++i)
	{
		RUNLOGGER.Log(_T("都道府県コード: %02d のチェック中\n"), i);
		auto prefCode = uh::str_util::formatA(_T("%02d"), i).GetString();
		std::cout << "都道府県コード: " << prefCode << "のチェック中" << std::endl;

		CString strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%'"), city_admin::kCityCode, i);
		cityAdminTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

		std::map<CString, std::vector<long>> addrCodeMap; // key:11桁コード val:OID群
		for (const auto& cityAdmin : cityAdminTbl)
		{
			auto& row = *CAST_ROWC(cityAdmin);
			CString citycode = row.GetValue(city_admin::kCityCode, CString());
			CString addrcode1 = row.GetValue(city_admin::kAddrCode, CString());
			CString addrcode2 = row.GetValue(city_admin::kAddrCode2, CString());

			// ADDRCODEが000000と999999は除外 (海とか自衛隊敷地とかで郵便番号は持たない)
			if (0 != addrcode1.Compare(_T("000000")) && 0 != addrcode1.Compare(_T("999999")))
			{
				addrCodeMap[citycode + addrcode1].push_back(row.GetOID());
			}

			// ADDRCODE2も同じ(999999はいないはずだが)
			if (0 != addrcode2.Compare(_T("000000")) && 0 != addrcode2.Compare(_T("999999")))
			{
				addrCodeMap[citycode + addrcode2].push_back(row.GetOID());
			}
		}
		cityAdminTbl.clear();

		// 紐付けテーブルも絞り込み
		strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%' AND %s <> %ld"),
			rel_addr::kAddrCode, i,
			rel_addr::kModifyStatus,
			rel_addr::ModifyStatusCode::Obsolete);
		relAddrTbl._Select(AheInitQueryFilter(nullptr, rel_addr::kAddrCode, strWhere), false);
		static const long idxAddrCode = relAddrTbl.FindField(rel_addr::kAddrCode); // index取っておくとちょっと速くなる
		std::set<CString> relAddrCodeSet;
		for (auto rec : relAddrTbl)
		{
			auto relAddrCode = CAST_ROWC(rec)->GetValue(idxAddrCode, CString());
			relAddrCodeSet.emplace(relAddrCode.Left(11));
		}
		relAddrTbl.clear();

		// 前方11桁が一致するレコードがあればOK
		for (const auto& addrCode : addrCodeMap)
		{
			const auto& result = relAddrCodeSet.find(addrCode.first);
			if (result != relAddrCodeSet.end())
				continue;

			hasError = true;
			for (auto oid : addrCode.second)
			{
				ERRLOGGER.writeStdLog(
					city_admin::kTableName,
					oid,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotExistInRelAddr,
					_T("行政界にあるが紐付けテーブルに存在しない"),
					addrCode.first
				);
			}
		}
	}
	return !hasError;
}
