#include "stdafx.h"
#include "CheckUtil.h"
#include "CheckPolygonAndAddr.h"
#include "TDC/sindylib_base/GeometryBase.h"
#include "TDC/useful_headers/enumerate_elements.h"
#include <boost/assign.hpp>

using namespace sindy;
using namespace sindy::schema;
using namespace addr;


namespace ErrorCode
{
	// CheckPolygonAndAddrのエラーコードは006xxxとする
	const CString CityAdminDoesNotExist = _T("006001");
	const CString LessOverlapZipCodeArea = _T("006002");
	const CString LessOverlapCityAdmin = _T("006003");
	const CString AddressPointDoesNotExist = _T("006004");
	const CString ZipCodeAreaDoesNotExist = _T("006005");
}

/**
 * @brief 都市地図行政界ポリゴンと郵便番号ポリゴンの重なりをチェックする(チェック本体)
 * @param admin [in] 都市地図行政会ポリゴンデータ
 * @param zipResult [in] 行政界に対応する郵便番号ポリゴン群
 * @param rate [in] 重なり率判定に使用する閾値
 * @retval true 郵便番号ポリゴンに対する行政界の重なり率が閾値以上
 * @retval false 郵便番号ポリゴンに対する行政界の重なり率が閾値未満
 */
bool CheckDupliRate(
	const sindy::CSPContainerBase& admin,
	const sindy::CIndexedContainer::IteratorContainer& zipResult,
	int rate
)
{
	// CreateCacheしてないので、ここでCreateCacheする
	// しないとGetShapeでGeometryがとれない
	// CreateCacheなしでもGetShapeCopyならGeometry取れるが、GetShapeのほうがずっと早い
	CAST_ROWC(admin)->CreateCache();
	IGeometryPtr ipAdminShape = (IGeometry*)(*CAST_ROWC(admin)->GetShape());
	CAST_ROWC(admin)->CRowBase::DeleteCache(); // メモリ消費抑えるため、すぐDeleteCacheする
	for (const auto& zip : zipResult)
	{
		CAST_ROWC(*zip)->CreateCache();
		const IGeometryPtr ipZipShape = (IGeometry*)(*CAST_ROWC(*zip)->GetShape());
		CAST_ROWC(*zip)->CRowBase::DeleteCache();

		IGeometryPtr ipIntersectGeometry;
		ITopologicalOperatorPtr ipTopo(ipZipShape);
		if (FAILED(ipTopo->Intersect(ipAdminShape, esriGeometry2Dimension, &ipIntersectGeometry)))
			continue;

		if (!ipIntersectGeometry)
			continue;

		double adminArea = 0, intersectArea = 0;
		((IAreaPtr)ipAdminShape)->get_Area(&adminArea);
		((IAreaPtr)ipIntersectGeometry)->get_Area(&intersectArea);

		// 行政界ポリゴンが閾値以上の割合で郵便番号ポリゴンと重なっていればOK
		if (intersectArea / adminArea * 100 >= rate)
			return true;
	}
	return false;
}

/**
 * @brief 都市地図行政界ポリゴンと郵便番号ポリゴンの重なりをチェックする(チェック準備/結果取得)
 * @param addrZipMap [in] 住所コードと郵便番号の対応表
 * @param zipAreaTbl [in] 郵便番号ポリゴンデータ
 * @param cityAdminTbl [in, out] 都市地図行政界データ
 * @param rate [in] 重なり率判定に使用する閾値
 * @param errorInfo [in] チェックの結果エラーだったものの情報(OID、郵便番号、住所コード)
 */
void CheckDupliRate(
	const std::map<CString, CString>& addrZipMap,
	const sindy::CTableContainer& zipAreaTbl,
	const sindy::CTableContainer& cityAdminTbl,
	int rate,
	std::map<long, std::pair<CString, CString>>& zipErrorInfo,
	std::map<long, std::pair<CString, CString>>& addrErrorInfo
)
{
	for (const auto& addrZip : addrZipMap)
	{
		// 郵便番号ポリゴンを取ってみる
		auto& zipResult1 = zipAreaTbl.finds_by_key(zipcode_area::kZipCode1, CComVariant(addrZip.second));
		auto& zipResult2 = zipAreaTbl.finds_by_key(zipcode_area::kZipCode2, CComVariant(addrZip.second));

		sindy::CIndexedContainer::IteratorContainer zipResult;
		zipResult.insert(zipResult1.begin(), zipResult1.end());
		zipResult.insert(zipResult2.begin(), zipResult2.end());

		if (zipResult.empty())
			continue;

		// 行政界取ってみる
		// まず住所コード1で検索
		static const sindy::KeyList key =
			boost::assign::list_of(CString(city_admin::kCityCode))(CString(city_admin::kAddrCode));
		sindy::ValueList val;
		val.push_back(CComVariant(addrZip.first.Left(5)));
		val.push_back(CComVariant(addrZip.first.Mid(5, 6)));
		auto adminResult1 = cityAdminTbl.finds_by_key(key, val);

		// 住所コード2で検索
		static const sindy::KeyList key2 =
			boost::assign::list_of(CString(city_admin::kCityCode))(CString(city_admin::kAddrCode2));
		auto adminResult2 = cityAdminTbl.finds_by_key(key2, val);

		sindy::CIndexedContainer::IteratorContainer adminResult;
		adminResult.insert(adminResult1.begin(), adminResult1.end());
		adminResult.insert(adminResult2.begin(), adminResult2.end());

		// 郵便番号ポリゴンがあるのに行政界がない
		if (adminResult.empty())
		{
			for (auto& zip : zipResult)
			{
				ERRLOGGER.writeStdLog(
					zipcode_area::kTableName,
					CAST_ROWC(*zip)->GetOID(),
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::CityAdminDoesNotExist,
					_T("郵便番号ポリゴンに対応する行政界が１つもない"),
					uh::str_util::format(_T("%s\t%s"), addrZip.second, addrZip.first)
				);
			}
			continue;
		}

		// 郵便番号ポリゴンからみて行政界と重なっているかどうか
		for (const auto& zip : zipResult)
		{
			CAST_ROWC(*zip)->CreateCache();
			IGeometryPtr ipZipShape = (IGeometry*)(*CAST_ROWC(*zip)->GetShape());
			CAST_ROWC(*zip)->CRowBase::DeleteCache();

			bool judge = false;
			for (const auto& admin : adminResult)
			{
				CAST_ROWC(*admin)->CreateCache();
				IGeometryPtr ipAdminShape = (IGeometry*)(*CAST_ROWC(*admin)->GetShape());

				VARIANT_BOOL disjoint = VARIANT_FALSE;
				((IRelationalOperatorPtr)ipZipShape)->Disjoint(ipAdminShape, &disjoint);

				if (!disjoint)
				{
					judge = true;
					break;
				}
			}

			if (!judge)
			{
				zipErrorInfo[CAST_ROWC(*zip)->GetOID()] =
					std::make_pair(addrZip.second, addrZip.first);
			}
		}


		// 行政界からみて郵便番号ポリゴンと重なっているかどうか
		for (const auto& admin : adminResult)
		{
			if (!CheckDupliRate(*admin, zipResult, rate))
			{
				addrErrorInfo[CAST_ROWC(*admin)->GetOID()] =
					std::make_pair(addrZip.second, addrZip.first);
			}
		}
	}
}

CheckPolygonAndAddr::CheckPolygonAndAddr()
{
}

CheckPolygonAndAddr::~CheckPolygonAndAddr()
{
}

bool CheckPolygonAndAddr::Init()
{
	// SDE(郵便番号ポリゴン)接続
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("郵便番号ポリゴンDB接続先への接続に失敗"));
		return false;
	}
	IFeatureClassPtr ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);

	// ファインダ初期化
	CModel zipModel;
	m_relAddrFinder.InitCollection((ITablePtr)ipZipCodeArea, zipModel);

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

bool CheckPolygonAndAddr::Check()
{
	RUNLOGGER.Info(_T("ポリゴンと住所データの整合チェック実行…\n"));
	std::cout << "ポリゴンと住所データの整合チェック実行…" << std::endl;

	// 住所データからみて郵便番号ポリゴンがあるかどうか
	bool result = true;
	RUNLOGGER.Info(_T("11桁チェック中…\n"));
	std::cout << "11桁チェック中…" << std::endl;
	result &= CheckContainCityAdmin();

	RUNLOGGER.Info(_T("16, 20桁チェック中…\n"));
	std::cout << "16, 20桁チェック中…" << std::endl;
	result &= CheckContainAddrPoint();

	RUNLOGGER.Info(_T("チェック終了\n"));
	std::cout << "チェック終了" << std::endl;
	return result;
}

bool CheckPolygonAndAddr::CheckContainCityAdmin()
{
	// 紐付けテーブルから11桁の住所コードに対応する郵便番号を取得する
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// 郵便番号ポリゴン
	auto& zipAreaTbl = *m_relAddrFinder.FindTable(zipcode_area::kTableName);
	zipAreaTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// 行政界ポリゴン
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// メモリの都合上、都道府県ごとにチェック
	bool hasError = false;
	for (int i = 1; i <= 47; ++i)
	{
		RUNLOGGER.Log(_T("都道府県コード: %02d のチェック中\n"), i);
		auto prefCode = uh::str_util::formatA(_T("%02d"), i).GetString();
		std::cout << "都道府県コード: " << prefCode << "のチェック中" << std::endl;

		auto query = AheInitQueryFilter(
			nullptr, nullptr,
			_T("%s NOT IN (%ld, %ld, %ld) AND %s LIKE '%02d%%%%'"),
			rel_addr::kModifyStatus,
			rel_addr::ModifyStatusCode::UnUsed,
			rel_addr::ModifyStatusCode::Obsolete,
			rel_addr::ModifyStatusCode::Used5digit,
			rel_addr::kAddrCode, i);
		relAddrTbl._Select(query, false);

		std::set<std::string> zipCodes;
		std::map<CString, CString> addrZipMap;
		for (const auto& relAddrRec : relAddrTbl)
		{
			auto& row = *CAST_ROWC(relAddrRec);
			CString addrcode = row.GetValue(rel_addr::kAddrCode, CString());
			if (11 != addrcode.GetLength())
				continue;

			CString zipcode = row.GetValue(rel_addr::kZipCode, CString());

			addrZipMap[addrcode] = zipcode;
			zipCodes.insert(CStringA(zipcode).GetString());
		}
		relAddrTbl.clear();

		// 行政界ポリゴンを取得する
		CString strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%'"), city_admin::kCityCode, i);
		cityAdminTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

		// 郵便番号500件ずつチェック
		std::map<long, std::pair<CString, CString>> zipErrorInfo;
		std::map<long, std::pair<CString, CString>> addrErrorInfo;
		auto inClauses = uh::enumerate_elements(zipCodes, 500, "'");
		for (auto& inClause : inClauses)
		{
			// 郵便番号でSelect
			strWhere = uh::str_util::format(
				_T("%s IN (%s) OR %s IN (%s)"),
				zipcode_area::kZipCode1, CString(inClause.c_str()),
				zipcode_area::kZipCode2, CString(inClause.c_str()));
			zipAreaTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

			// 重なりをチェック
			int rate = _ttoi(m_args.m_dupliRate.c_str());
			CheckDupliRate(addrZipMap, zipAreaTbl, cityAdminTbl, rate, zipErrorInfo, addrErrorInfo);
			zipAreaTbl.clear();
		}

		for (auto& err : zipErrorInfo)
		{
			hasError = true;
			ERRLOGGER.writeStdLog(
				zipcode_area::kTableName,
				err.first,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::LessOverlapZipCodeArea,
				_T("郵便番号ポリゴンに対応する行政界が見つからない"),
				uh::str_util::format(_T("%s\t%s"), err.second.second, err.second.first)
			);
		}

		for (auto& err : addrErrorInfo)
		{
			hasError = true;
			ERRLOGGER.writeStdLog(
				city_admin::kTableName,
				err.first,
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::LessOverlapCityAdmin,
				_T("郵便番号ポリゴンに対して行政界の重なり率が小さい"),
				uh::str_util::format(_T("%s\t%s"), err.second.second, err.second.first)
			);
		}
		cityAdminTbl.clear();
	}

	return !hasError;
}

bool CheckPolygonAndAddr::CheckContainAddrPoint()
{
	CheckUtil util(m_addrRecHelper);

	// SiNDY住所データ
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& gouPointTbl = *m_addrFinder.FindTable(gou_point::kTableName);
	gouPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& csPointTbl = *m_addrFinder.FindTable(cs_addr_point::kTableName);
	csPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// CSは少ないので一旦全国分取ってしまう
	std::map<CString, AddrRecs> csAddrRecsMap;
	m_addrRecHelper.makeAdrRecordsFromCs(csPointTbl, _T(""), csAddrRecsMap);
	csPointTbl.clear();

	// 住所紐付けテーブル
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& zipCodeAreaTbl = *m_relAddrFinder.FindTable(zipcode_area::kTableName);
	zipCodeAreaTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto query = AheInitQueryFilter(
		nullptr, nullptr,
		_T("%s NOT IN (%ld, %ld, %ld)"),
		rel_addr::kModifyStatus,
		rel_addr::ModifyStatusCode::UnUsed,
		rel_addr::ModifyStatusCode::Obsolete,
		rel_addr::ModifyStatusCode::Used5digit);
	relAddrTbl._Select(query, false);

	// 郵便番号と住所コードのペアを作る。住所コードは16桁、20桁のもののみ。
	std::map<CString, std::vector<sindy::CSPRowContainer>> zipAddrMap;
	for (const auto& relAddrRec : relAddrTbl)
	{
		sindy::CSPRowContainer row = CAST_ROWC(relAddrRec);
		auto addrcode = row->GetValue(rel_addr::kAddrCode, CString());
		if (11 == addrcode.GetLength())
			continue;

		auto zipcode = row->GetValue(rel_addr::kZipCode, CString());
		zipAddrMap[zipcode].push_back(row);
	}

	// 11桁コード+名称(地番、号)の一覧を作成
	bool hasError = false;
	for (auto& zipAddr : zipAddrMap)
	{
		std::cout << "郵便番号： " << CStringA(zipAddr.first).GetString() << std::endl;

		std::map<CString, AddrRecPairs> relAddrMap;
		for (auto& addrRec : zipAddr.second)
		{
			// 名称(地番、号)取得
			AddrRec tgtAddrRec(addrRec->GetValue(rel_addr::kAddrCode, CString()), nullptr);
			if (!m_addrRecHelper.makeAddrNames(tgtAddrRec))
			{
				ERRLOGGER.Log(_T("#文字付き住所テーブルから名称取得に失敗\t%s\n"), tgtAddrRec.getJusyocode());
				hasError = true;
				continue;
			}

			relAddrMap[tgtAddrRec.getJusyocode()].push_back(std::make_pair(tgtAddrRec, *addrRec));
		}

		if (hasError)
		{
			RUNLOGGER.Error(_T("11桁コード＋名称の取得にエラーがあります。(エラーログ参照)\n"));
		}

		// チェック
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
				//csAddrRecs = csAddrRecsMap[relAddrRecPairs.first];
				csAddrRecs = csAddrRecsMap[relAddrRecPairs.first.Left(11)];
			}

			// 存在判定
			for (auto& relAddrRec : relAddrRecPairs.second)
			{
				AddrRecs existRecs;
				bool exist = util.CheckAddressDataExistance(
					adminAddrRecs, csAddrRecs, gouPointTbl, relAddrRec.first, existRecs);

				if (!exist)
				{
					// ここに入ってくるということは紐付けテーブルの更新がちゃんとされてない
					// table_addrのチェックでやってるので、ここでは無視
					continue;
				}

				// 郵便番号ポリゴンを取る
				auto& whereClause = uh::str_util::format(
					_T("%s = '%s' OR %s = '%s'"),
					zipcode_area::kZipCode1,
					zipAddr.first,
					zipcode_area::kZipCode2,
					zipAddr.first);
				zipCodeAreaTbl._Select(AheInitQueryFilter(nullptr, nullptr, whereClause), false);

				// 郵便番号ポリゴンからみて住所がいるかどうか
				hasError &= ZipToAddrCheck(zipCodeAreaTbl, existRecs,
					zipAddr.first, relAddrRec.first.getJusyocode());

				// 住所からみて郵便番号ポリゴンがいるかどうか
				hasError &= AddrToZipCheck(zipCodeAreaTbl, existRecs,
					zipAddr.first, relAddrRec.first.getJusyocode());

				zipCodeAreaTbl.clear();
			}
		}
	}

	return !hasError;
}

bool CheckPolygonAndAddr::ZipToAddrCheck(
	const sindy::CTableContainer& zipCodeAreaTbl,
	const AddrRecs& existRecs,
	const CString& zipCode,
	const CString& addrCode)
{
	bool hasError = false;
	for (auto& zipArea : zipCodeAreaTbl)
	{
		CAST_ROWC(zipArea)->CreateCache();
		IGeometryPtr ipZipAreaGeometry = (IGeometry*)(*CAST_ROWC(zipArea)->GetShape());
		CAST_ROWC(zipArea)->CRowBase::DeleteCache();

		bool judge = false;
		for (const auto& rec : existRecs)
		{
			// 号・CSポイント、行政界ポリゴンが郵便番号ポリゴンに内包されていればOK
			auto existGeometry = rec.getGeometry();
			auto geometryType = esriGeometryNull;
			existGeometry->get_GeometryType(&geometryType);

			VARIANT_BOOL disjoint = VARIANT_FALSE;
			(IRelationalOperatorPtr(ipZipAreaGeometry))->Disjoint(existGeometry, &disjoint);

			if (!disjoint)
			{
				judge = true;
				break;
			}
		}

		if (!judge)
		{
			hasError = true;
			ERRLOGGER.writeStdLog(
				zipcode_area::kTableName,
				CAST_ROWC(zipArea)->GetOID(),
				_T(""),
				_T(""),
				_T("ERROR"),
				ErrorCode::AddressPointDoesNotExist,
				_T("郵便番号ポリゴンに対応する住所データがない"),
				uh::str_util::format(_T("%s\t%s"), zipCode, addrCode)
			);
		}
	}
	return !hasError;
}

bool CheckPolygonAndAddr::AddrToZipCheck(
	const sindy::CTableContainer& zipCodeAreaTbl,
	const AddrRecs& existRecs,
	const CString& zipCode,
	const CString& addrCode)
{
	bool hasError = false;
	for (const auto& rec : existRecs)
	{
		// 号・CSポイント、行政界ポリゴンが郵便番号ポリゴンに内包されていればOK
		auto existGeometry = rec.getGeometry();
		auto geometryType = esriGeometryNull;
		existGeometry->get_GeometryType(&geometryType);

		bool judge = false;
		switch (geometryType)
		{
		case esriGeometryPolygon:
		{
			for (auto& zipArea : zipCodeAreaTbl)
			{
				CAST_ROWC(zipArea)->CreateCache();
				IGeometryPtr ipZipAreaGeometry = (IGeometry*)(*CAST_ROWC(zipArea)->GetShape());
				CAST_ROWC(zipArea)->CRowBase::DeleteCache();
				IGeometryPtr ipIntersectGeometry;
				ITopologicalOperatorPtr ipTopo(ipZipAreaGeometry);
				ipTopo->Intersect(existGeometry, esriGeometry2Dimension,
					&ipIntersectGeometry);

				if (!ipIntersectGeometry)
					continue;

				double adminArea = 0, intersectArea = 0;
				((IAreaPtr)existGeometry)->get_Area(&adminArea);
				((IAreaPtr)ipIntersectGeometry)->get_Area(&intersectArea);

				if (intersectArea / adminArea * 100 >= _ttol(m_args.m_dupliRate.c_str()))
				{
					judge = true;
					break;
				}
			}

			if (!judge)
			{
				// ループ中のrecに該当するCITY_ADMINのOIDが取りたいが、
				// AddrRecからはOID取れないので、代表点計算してログから飛べるようにする。
				auto point =  m_repPointCalculator.CalculateRepWKSPoint(existGeometry, rpc::OnSurface);
				hasError = true;
				ERRLOGGER.writeStdLog(
					city_admin::kTableName,
					0, // OIDは0として出力する
					uh::str_util::format(_T("%.20lf"), point.X),
					uh::str_util::format(_T("%.20lf"), point.Y),
					_T("ERROR"),
					ErrorCode::LessOverlapCityAdmin,
					_T("郵便番号ポリゴンに対して行政界の重なり率が小さい"),
					uh::str_util::format(_T("%s\t%s"), zipCode, addrCode)
				);
			}
			break;
		}
		case esriGeometryPoint:
		{
			// ポイントとの整合
			for (auto& zipArea : zipCodeAreaTbl)
			{
				CAST_ROWC(zipArea)->CreateCache();
				IGeometryPtr ipZipAreaGeometry = (IGeometry*)(*CAST_ROWC(zipArea)->GetShape());
				CAST_ROWC(zipArea)->CRowBase::DeleteCache();
				VARIANT_BOOL within = VARIANT_FALSE;
				IRelationalOperatorPtr ipRel(existGeometry);
				if (FAILED(ipRel->Within(ipZipAreaGeometry, &within)))
					continue;

				if (within)
				{
					judge = true;
					break;
				}
			}

			if (!judge)
			{
				hasError = true;
				// 号かCSかはわからないので、ログは座標値で飛べるようにする
				IPointPtr ipPoint(existGeometry);
				double x = 0, y = 0;
				ipPoint->QueryCoords(&x, &y);

				ERRLOGGER.writeStdLog(
					_T("GOU/CS"),
					0, // OIDは0として出力する
					uh::str_util::format(_T("%.20lf"), x),
					uh::str_util::format(_T("%.20lf"), y),
					_T("ERROR"),
					ErrorCode::ZipCodeAreaDoesNotExist,
					_T("住所ポイントに対応する郵便番号ポリゴンがない"),
					uh::str_util::format(_T("%s\t%s"), zipCode, addrCode)
				);
			}
			break;
		}
		default:
			break;
		}
	}
	return !hasError;
}

