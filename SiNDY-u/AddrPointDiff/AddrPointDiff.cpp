#include "stdafx.h"
#include "AddrPointDiff.h"
#include "Logger.h"
#include "Utility.h"
#include "Arguments.h"
#include "AddrPointRec.h"
#include <math.h>

#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheMeshUtilArcObjects.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <WinLib/type_convert.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy;
using namespace schema;
using namespace sindy::errorcode;

bool CAddrPointDiff::initCollection(CTableFinder& finder, const CString& conn, const CString& table)
{
	CModel model;
	ITablePtr ipTbl;
	if(!openTable(conn, table, ipTbl))
	{
		return false;
	}

	finder.InitCollection(ipTbl, model);

	return true;
}

// 初期化
bool CAddrPointDiff::init()
{
	cout << (CT2A)uh::str_util::format(_T("初期化")) << endl;

	// SiNDY（住所）
	if( !initCollection(m_addrFinder, m_args.m_ipc_addr_db.c_str(), city_admin::kTableName) )
		return false;

	// SiNDY（背景）
	if( !initCollection(m_bgFinder, m_args.m_ipc_bg_db.c_str(), building::kTableName) )
		return false;

	// 中間ファイル住所
	if( !initCollection(m_midAddrPointFinder, m_args.m_mid_addr_db.c_str(), m_args.m_mid_addr_layer.c_str()) )
		return false;

		// 実行ログオープン
	if( !RUNLOGGER.Initialize( (uh::tstring)m_args.m_run_log) )
	{
		cerr << (CT2A)uh::str_util::format(_T("FATAL\t実行ログが開けません:%s"), m_args.m_run_log) << endl;
		return false;
	}

	// 実行ログへオプション情報を書込み
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_ipc_addr_db, m_args.m_ipc_addr_db.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_ipc_bg_db, m_args.m_ipc_bg_db.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_mid_addr_db, m_args.m_mid_addr_db.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_mid_addr_layer, m_args.m_mid_addr_layer.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_citycode_list, m_args.m_citycode_list.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_point_dist, m_args.m_point_dist.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_run_log, m_args.m_run_log.c_str()), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_err_log, m_args.m_err_log.c_str()), false);
	RUNLOGGER.Log(_T("\n"), false);

	return true;
}

void CAddrPointDiff::checkInBuilding(
		CTableContainer& buildingT,
		AddrPointRecs& addrPointRecs,
		const IGeometryPtr& targetGeo)
{
	// 検索対象のポイント群（マルチポイント）に接するBUILDINGを取得
	auto query = AheInitQueryFilter
					(
						(IQueryFilterPtr)AheInitSpatialFilter( nullptr, targetGeo ),
						nullptr
					);
	buildingT._Select(query, false);

	// BUILDINGの形状とGeospace家形IDのペアを生成
	vector<pair<IGeometryPtr, CString>> buildings;

	long geoIdIdx = buildingT.FindField(building::kGeospaceID);
	for(const auto& bld : buildingT)
	{
		auto& row = *CAST_ROWC(bld);
		buildings.emplace_back(row.CRowBase::GetShapeCopy(), row.GetValue(geoIdIdx, CString()));
	}

	// 対象の住所ポイント数分、BUILDING上に存在するか確認
	for(auto& rec : addrPointRecs)
	{
		// 既に建物内存在フラグが立っている場合はスキップ
		if( rec.isInBuilding )
			continue;

		for(const auto& bld : buildings)
		{
			// 取得したBUILDINGと接する場合、建物内存在フラグを立て、
			// その建物のGeospace家形IDを格納する
			IRelationalOperatorPtr iRelOp( bld.first );
			if( VB2bool(iRelOp->_Contains(rec.geo)) )
			{
				rec.isInBuilding = true;
				rec.geoBldId = bld.second;
				break;
			}
		}
	}

	buildingT.clear();
}

void CAddrPointDiff::checkInBuilding(
		CTableContainer& buildingT,
		AddrPointRecs& addrPointRecs)
{
	// この関数内の処理が結構かかる

	// BUILDINGに対する属性検索用に、Geospace家形IDのリストを生成
	// また対象のGeospace家形IDに紐付く住所ポイントを検索するためのmapも併せて生成
	set<CStringA> geoIdList;
	map<CString, vector<AddrPointRec*>> pointRecs;
	for(auto& point : addrPointRecs)
	{
		if( point.geoBldId.IsEmpty() )
			continue;

		geoIdList.emplace(point.geoBldId);
		pointRecs[point.geoBldId].emplace_back(&point);
	}

	// Geospace家形IDを検索条件にして検索（1000件毎に分けて検索）
	for(const auto& element : uh::enumerate_elements(geoIdList, 1000, "'", ","))
	{
		auto query = AheInitQueryFilter(
			nullptr, 
			nullptr, 
			_T("%s in (%s)"), 
			building::kGeospaceID, (CString)element.c_str()
		);
		buildingT._Select(query, false);
	}

	long geoIdIdx = buildingT.FindField(building::kGeospaceID);
	for(const auto& bld : buildingT)
	{
		auto& row = *CAST_ROWC(bld);
		CString geoId = row.GetValue(geoIdIdx, CString());

		// BUILDINGから取得した建物のGeospace家形IDと一致する
		// 住所ポイントに対し、建物内存在フラグを立てる
		if( 0 == pointRecs.count(geoId) )
			continue;

		for(auto& point : pointRecs[geoId])
			point->isInBuilding = true;
	}
	buildingT.clear();
}

bool CAddrPointDiff::assortAddrPointsByArea(
		CTableContainer& cityAdminT,
		const AddrPointRecs& addrPointRecs,
		const IGeometryPtr& targetGeo,
		map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
		map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints)
{
	cityAdminT.clear();

	// 対象ジオメトリの接するCITY_ADMINのオブジェクトを取得
	auto query = AheInitQueryFilter
			(
				(IQueryFilterPtr)AheInitSpatialFilter( nullptr, targetGeo),
				nullptr
			);
	cityAdminT._Select(query, false);

	// 取得したオブジェクトのうち、都市地図エリア内（addrclass_c=1）の形状を1つの形状にマージ
	IGeometryBagPtr ipMargeGeoBag(CLSID_GeometryBag);
	ipMargeGeoBag->PutRefSpatialReference(AheGetSpatialReference((ITable*)cityAdminT));
	IGeometryCollectionPtr ipMargeGeoCollection(ipMargeGeoBag);

	for(auto& ca : cityAdminT)
	{
		auto& row = *CAST_ROWC(ca);
		
		if(city_admin::area_class::kInArea == row.GetValue(city_admin::kAreaClass, 0L))
			ipMargeGeoCollection->AddGeometry(row.CRowBase::GetShapeCopy());
	}

	IPolygon4Ptr ipMargePolygon(CLSID_Polygon);
	ipMargePolygon->PutRefSpatialReference(AheGetSpatialReference((ITable*)cityAdminT));
	IEnumGeometryPtr ipEnumMargeGeo(ipMargeGeoBag);
	
	if( FAILED(((ITopologicalOperatorPtr)ipMargePolygon)->ConstructUnion(ipEnumMargeGeo)) )
	{
		RUNLOGGER.Error(_T("CITY_ADMIN(都市地図エリア内)の形状マージに失敗"));
		return false;
	}

	if( !ipMargePolygon )
	{
		RUNLOGGER.Error(_T("マージしたCITY_ADMIN(都市地図エリア内)の形状が不正"));
		return false;
	}

	// 都市地図エリア内外を判定する住所ポイント分、ループ
	IRelationalOperatorPtr iRelOp( ipMargePolygon );
	for(auto& rec : addrPointRecs)
	{
		// マージしたCITY_ADMIN（都市地図エリア内の形状）にポイントが含まれるか判定
		// 含まれる：都市地図エリア内、含まれない：都市地図エリア外
		if( VB2bool(iRelOp->_Contains(rec.geo)) )
			urbanAreaAddrPoints[rec.addrcode].setAddrPointRecs(rec);
		else
			nonUrbanAreaAddrPoints[rec.addrcode].setAddrPointRecs(rec);
	}

	cityAdminT.clear();
	return true;
}

void CAddrPointDiff::makeAddrPoint(
		AddrPointType type,
		const CString& addrcode11,
		const CRowContainer& targetRow,
		const CString& gaikuFugo,
		const CString& extGaikuFugo,
		int addrClass,
		AddrPointRecs& addrPointRecs)
{
	static const std::vector<CString> addressFields = boost::assign::list_of
		( mid_addr_schema::addr_point::kAddress1 )
		( mid_addr_schema::addr_point::kAddress2 )
		( mid_addr_schema::addr_point::kAddress3 );

	CString geoId;
	CString address;

	switch (type)
	{
	case AddrPointType::GouPoint: // 号ポイントから生成
		{
			// 号番号内の「#」は削除する
			CString gouNo = targetRow.GetValue(gou_point::kGouNo, CString());
			gouNo.Replace(_T("#"), _T(""));

			bool extGouNo = targetRow.GetValue(gou_point::kExpGouNo, false);
		
			if( !gaikuFugo.IsEmpty() ) // 街区符号に値がある場合
			{
				address = ( !extGouNo ) ? 
							uh::str_util::format(_T("%s%s%s"), gaikuFugo, SEPARATOR_HALF_HYPHEN, gouNo) :
							gouNo;
			}
			else if( !extGaikuFugo.IsEmpty() ) // 拡張街区符号に値がある場合
			{
				if( !extGouNo )
					address = ( addrClass == city_admin::addr_class::kResidenceIndicationMaintenance ) ? 
								uh::str_util::format(_T("%s%s%s"), extGaikuFugo, SEPARATOR_HALF_HYPHEN, gouNo) :
								uh::str_util::format(_T("%s%s"), extGaikuFugo, gouNo);
				else
					address = gouNo;
			}
			else
				address = gouNo;
		}
		break;
	case AddrPointType::CsAddrPoint: // CSポイントから生成
		{
			CString addrcode = targetRow.GetValue(cs_addr_point::kAddrCode, CString());

			vector<CString> valueList;
			int chibancode = _tstol(addrcode.Mid(11, 5));
			int jukyocode = _tstol(addrcode.Mid(16, 4));

			if( 0 != chibancode ) valueList.emplace_back( uh::str_util::ToString(chibancode) );
			if( 0 != jukyocode ) valueList.emplace_back( uh::str_util::ToString(jukyocode) );

			address = uh::str_util::join( valueList, SEPARATOR_HALF_HYPHEN );
		}
		break;
	case AddrPointType::GsAddrPoint: // 中間ファイル住所から生成
		{
			vector<CString> valueList;
			for(const auto& field : addressFields)
			{
				CString value = replaceStr(targetRow.GetValue(field, CString()));
				if( !value.IsEmpty() )
					valueList.emplace_back(value);
			}

			address = uh::str_util::join(valueList, SEPARATOR_HALF_HYPHEN);
			geoId = replaceStr(targetRow.GetValue(mid_addr_schema::addr_point::kGeoBFid, CString()));
		}
		break;
	}

	AddrPointRec rec(type, targetRow.GetOID(), addrcode11, address, targetRow.CRowBase::GetShapeCopy(), geoId);
	addrPointRecs.push_back(rec);
}

void CAddrPointDiff::makeAddrPoint(
		AddrPointType type,
		const CString& addrcode11,
		const CRowContainer& targetRow,
		AddrPointRecs& addrPointRecs)
{
	makeAddrPoint(
			type,
			addrcode11,
			targetRow,
			_T(""),
			_T(""),
			0,
			addrPointRecs
		);
}

void CAddrPointDiff::setAddrPointRecs(
				const AddrPointRecs& addrPointRecs,
				const CString& addrcode2,
				map<CString, AddrPointRecSet>& addrPointSets
			)
{
	// 追加対象の住所ポイント分、ループ
	for(auto& point : addrPointRecs)
	{
		addrPointSets[point.addrcode].setAddrPointRecs(point);

		// 処理対象のCITY_ADMINにaddrcode2も設定されている場合、
		// addrcode2の住所11桁コードを持つ住所ポイントも生成
		if( _T("000000") != addrcode2.Mid(5, 6) )
		{
			AddrPointRec point2(point);
			point2.addrcode = addrcode2;
			addrPointSets[point2.addrcode].setAddrPointRecs(point2);
		}
	}
}

void CAddrPointDiff::getAddrPointsFromGouPoint(
				const CString& citycode,
				CTableContainer& cityAdminT,
				CTableContainer& buildingT,
				set<CString>& addrcode11List,
				map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			)
{
	auto& gouPointT = *m_addrFinder.FindTable(gou_point::kTableName);
	gouPointT.SetAutoCache(sindyContainerAutoCacheNone);

	// 住所整備種別が「0」または、住所コードが「999999」は除外
	auto query = AheInitQueryFilter(
		nullptr, 
		nullptr, 
		_T("%s = '%s' and %s <> %ld and %s <> '%s'"), 
			city_admin::kCityCode, citycode,
			city_admin::kAddrClass, city_admin::addr_class::kAzaUndeveloped,
			city_admin::kAddrCode, _T("999999")
	);
	cityAdminT._Select(query, false);

	for(const auto& ca : cityAdminT)
	{
		const auto& caRow = *CAST_ROWC(ca);

		CString addrcode = caRow.GetValue(city_admin::kAddrCode, CString());
		int addrClass = caRow.GetValue(city_admin::kAddrClass, 0L);
		int areaClass = caRow.GetValue(city_admin::kAreaClass, 0L);
		CString gaikuFugo = caRow.GetValue(city_admin::kGaikuFugo, CString());
		CString extGaikuFugo = caRow.GetValue(city_admin::kExtGaikuFugo, CString());
		CString citycode = caRow.GetValue(city_admin::kCityCode, CString());
		CString addrcode2 = caRow.GetValue(city_admin::kAddrCode2, CString());
		IGeometryPtr geo = caRow.CRowBase::GetShapeCopy();

		// 号番号が「*」は除外する
		auto query = AheInitQueryFilter
				(
					(IQueryFilterPtr)AheInitSpatialFilter( nullptr, geo ),
					nullptr,
					_T("%s <> '*'"), gou_point::kGouNo
				);
		gouPointT._Select(query, false);

		AddrPointRecs tmpUrbanAddrPoints;
		AddrPointRecs tmpNonUrbanAddrPoints;

		CString addrcode11 = citycode + addrcode;
		CString addrcode11sec = citycode + addrcode2;
		if( !gouPointT.empty() )
		{
			addrcode11List.emplace(addrcode11);
			if( _T("000000") != addrcode2 )
				addrcode11List.emplace(addrcode11sec);
		}

		for(const auto& gou : gouPointT)
		{
			auto& gouRow = *CAST_ROWC(gou);
			makeAddrPoint(
				AddrPointType::GouPoint,
				addrcode11,
				gouRow,
				gaikuFugo,
				extGaikuFugo,
				addrClass,
				( city_admin::area_class::kInArea == areaClass ) ? tmpUrbanAddrPoints : tmpNonUrbanAddrPoints);
		}
		gouPointT.clear();

		checkInBuilding(buildingT, tmpUrbanAddrPoints, geo);
		setAddrPointRecs(tmpUrbanAddrPoints, addrcode11sec, urbanAreaAddrPoints);
		setAddrPointRecs(tmpNonUrbanAddrPoints, addrcode11sec, nonUrbanAreaAddrPoints);
	}
	cityAdminT.clear();

}

bool CAddrPointDiff::getAddrPointsFromCsPoint(
				const CString& citycode,
				CTableContainer& cityAdminT,
				CTableContainer& buildingT,
				set<CString>& addrcode11List,
				map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			)
{
	bool ret = true;

	auto& csAddrPointT = *m_addrFinder.FindTable(cs_addr_point::kTableName);
	csAddrPointT.SetAutoCache(sindyContainerAutoCacheNoDetach);

	auto query = AheInitQueryFilter(
		nullptr, 
		nullptr, 
		_T("%s like '%s%%'"), cs_addr_point::kAddrCode, citycode
	);
	csAddrPointT._Select(query, false);

	AddrPointRecs tmpCsAddrPoints;
	IMultipointPtr ipMultiPoint(CLSID_Multipoint);
	ipMultiPoint->PutRefSpatialReference(AheGetSpatialReference((ITable*)csAddrPointT));
	for(const auto& cp : csAddrPointT)
	{
		auto& row = *CAST_ROWC(cp);

		CString addrcode = row.GetValue(cs_addr_point::kAddrCode, CString());
		addrcode11List.emplace(addrcode.Mid(0, 11));

		makeAddrPoint(AddrPointType::CsAddrPoint, addrcode.Mid(0, 11), row, tmpCsAddrPoints);
		IGeometryPtr geo = row.CRowBase::GetShapeCopy();
		((IPointCollectionPtr)ipMultiPoint)->_AddPoint((IPointPtr)geo);
	}
	csAddrPointT.clear();

	checkInBuilding(buildingT, tmpCsAddrPoints, ipMultiPoint);
	ret &= assortAddrPointsByArea(cityAdminT, tmpCsAddrPoints, ipMultiPoint, urbanAreaAddrPoints, nonUrbanAreaAddrPoints);

	return ret;

}

bool CAddrPointDiff::getAddrPointsFromMidAddrPoint(
				const CString& citycode,
				CTableContainer& cityAdminT,
				CTableContainer& buildingT,
				set<CString>& addrcode11List,
				map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			)
{
	bool ret = true;

	auto& gsAddrPointT = *m_midAddrPointFinder.FindTable(m_args.m_mid_addr_layer.c_str());
	gsAddrPointT.SetAutoCache(sindyContainerAutoCacheNone);

	auto query = AheInitQueryFilter(
		nullptr, 
		nullptr, 
		_T("%s like '%s%%'"), mid_addr_schema::addr_point::kPlaceCd, citycode
	);
	gsAddrPointT._Select(query, false);

	AddrPointRecs tmpGsAddrPoints;
	IMultipointPtr ipMultiPointGs(CLSID_Multipoint);
	ipMultiPointGs->putref_SpatialReference(AheGetSpatialReference((ITable*)gsAddrPointT));

	for(const auto& gs : gsAddrPointT)
	{
		auto& row = *CAST_ROWC(gs);

		CString placecode = row.GetValue(mid_addr_schema::addr_point::kPlaceCd, CString());
		addrcode11List.emplace(placecode);

		makeAddrPoint(AddrPointType::GsAddrPoint, placecode, row, tmpGsAddrPoints);
		IGeometryPtr geo = row.CRowBase::GetShapeCopy();
		((IPointCollectionPtr)ipMultiPointGs)->_AddPoint((IPointPtr)geo);
	}
	gsAddrPointT.clear();

	checkInBuilding(buildingT, tmpGsAddrPoints);
	ret &= assortAddrPointsByArea(cityAdminT, tmpGsAddrPoints, ipMultiPointGs, urbanAreaAddrPoints, nonUrbanAreaAddrPoints);

	return ret;
}

// 実行
bool CAddrPointDiff::execute()
{
	bool ret = true;

	// 初期化
	if( !init() )
		return false;

	auto& cityAdminT = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminT.SetAutoCache(sindyContainerAutoCacheNone);

	auto& buildingT = *m_bgFinder.FindTable(building::kTableName);
	buildingT.SetAutoCache(sindyContainerAutoCacheNone);

	RUNLOGGER.Info(_T("市区町村コードリスト読込み\n"), true);

	auto& citycodeList = uh::text_util::ToTStrList(m_args.m_citycode_list.c_str());

	for(const auto& citycode : citycodeList)
	{
		// 読み込んだ市区町村コードが空の場合、スキップ
		if( citycode.empty() )
			continue;

		set<CString> addrcode11List;
		map<CString, AddrPointRecSet> urbanAreaAddrPoints;
		map<CString, AddrPointRecSet> nonUrbanAreaAddrPoints;

		map<CString, AddrPointRecSet> urbanAreaMidAddrPoints;
		map<CString, AddrPointRecSet> nonUrbanAreaMidAddrPoints;
	
		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("号ポイント取得(%s)\n"), citycode.c_str()), true);

		// 号ポイントから比較対象となる住所ポイントを取得
		getAddrPointsFromGouPoint(
						citycode.c_str(),
						cityAdminT,
						buildingT,
						addrcode11List,
						urbanAreaAddrPoints,
						nonUrbanAreaAddrPoints
					);

		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("CSポイント取得(%s)\n"), citycode.c_str()),  true);

		// CSポイントから比較対象となる住所ポイントを取得
		ret &= getAddrPointsFromCsPoint(
						citycode.c_str(),
						cityAdminT,
						buildingT,
						addrcode11List,
						urbanAreaAddrPoints,
						nonUrbanAreaAddrPoints
					);

		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("中間ファイル住所ポイント取得(%s)\n"), citycode.c_str()), true);

		// 中間ファイル住所ポイントから比較対象となる住所ポイントを取得
		ret &= getAddrPointsFromMidAddrPoint(
						citycode.c_str(),
						cityAdminT,
						buildingT,
						addrcode11List,
						urbanAreaMidAddrPoints,
						nonUrbanAreaMidAddrPoints
					);

		CVersion ver;
		for(const auto& addrcode11 : addrcode11List)
		{
			CString errlog = uh::str_util::format(_T("%s\\err_%s.log"),
													m_args.m_err_log.c_str(), addrcode11);
			// エラーログオープン
			if( !ERRLOGGER.Initialize( (uh::tstring)errlog ) )
			{
				RUNLOGGER.Error((uh::tstring)uh::str_util::format(_T("FATAL\tエラーログが開けません:%s\n"), errlog), true);
				ERRLOGGER.Finalize();
				ret = false;
				continue;
			}

			// ヘッダ書込み
			ERRLOGGER.Log(_T("#%s FILEVERSION %s\n"), ver.GetInternalName(), ver.GetFileVersion());
			ERRLOGGER.Log(_T("%s\n"), uh::str_util::join(ERRLOG_HEADER_LIST, _T("\t")));

			RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("差分取得処理(%s)\n"), addrcode11), true);

			// 都市地図エリア内の住所ポイント同士に比較
			diffUrbanAreaPoints(addrcode11,
									urbanAreaAddrPoints,
									urbanAreaMidAddrPoints);

			// 都市地図エリア外の住所ポイント同士に比較
			diffNonUrbanAreaPoints(addrcode11,
									nonUrbanAreaAddrPoints,
									nonUrbanAreaMidAddrPoints);

			// 蓄積したエラーログに書込み
			writeErrLog();

			// ファイルクローズ
			ERRLOGGER.Finalize();

		}
	}

	return ret;
}



void CAddrPointDiff::writeErrLog()
{
	// 出力前にソート
	sort(errInfoRecs.begin(), errInfoRecs.end());

	// 書込み
	for(const auto& rec : errInfoRecs)
	{
		CString a = rec.getMessageStr();
		ERRLOGGER.Log(_T("%s\n"), rec.getMessageStr());
	}
	errInfoRecs.clear();
}


void CAddrPointDiff::diffUrbanAreaPoints(
			const CString& addrcode11,
			map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
			map<CString, AddrPointRecSet>& urbanAreaMidAddrPoints
			)
{
	// 住所ポイントセット（iPC住所）から対象の住所11桁コードに紐付くポイント群を取得
	AddrPointRecsByCode ipcAddrPoints;
	if( 0 != urbanAreaAddrPoints.count( addrcode11 ) )
		urbanAreaAddrPoints[addrcode11].getAddrPointRecs(ipcAddrPoints);

	// 住所ポイントセット（中間ファイル住所）から対象の住所11桁コードに紐付くポイント群を取得
	AddrPointRecsByCode midAddrPoints;
	if( 0 != urbanAreaMidAddrPoints.count ( addrcode11 ) )
		urbanAreaMidAddrPoints[addrcode11].getAddrPointRecs(midAddrPoints);

	// iPC住所ポイントの数だけループ
	for(auto& ipcPoints : ipcAddrPoints)
	{
		for(auto& ipcPoint : ipcPoints.second)
		{
			// iPC住所ポイントが建物上に存在しない場合
			if( !ipcPoint.isInBuilding )
			{
				errInfoRecs.emplace_back(ipcPoint, AddrPointRec(), _T("5"));
				continue;
			}

			// iPC住所ポイントが建物上に存在するが、
			// 同一Geospace家形IDを持つ中間ファイルの住所ポイントが存在しない
			// または、Geopsace家形ID値が無い場合
			if( 0 == midAddrPoints.count( ipcPoint.geoBldId ) || ipcPoint.geoBldId.IsEmpty() )
			{
				errInfoRecs.emplace_back(ipcPoint, AddrPointRec(), _T("3"));
				continue;
			}

			// 同一Geospace家形IDを持つ中間ファイル住所ポイントの数だけループ
			for(auto& midPoint : midAddrPoints[ipcPoint.geoBldId])
			{
				// iPC住所ポイントと中間ファイル住所ポイントが同一住所
				if( ipcPoint == midPoint )
				{
					midPoint.addrExistAttr = AddrExistAttr::ExistSameAddrInArea;
					errInfoRecs.emplace_back(ipcPoint, midPoint, _T("1"), m_args.m_mid_addr_layer.c_str());
					break;
				}

				// iPC住所ポイントと中間ファイル住所ポイントが同一住所ではない
				midPoint.isExistOtherPointInSameBld = true;
				errInfoRecs.emplace_back(ipcPoint, midPoint, _T("4"), m_args.m_mid_addr_layer.c_str());
			}
		}
	}

	// 中間ファイル住所ポイントの数だけループ
	for(auto& midPoints : midAddrPoints)
	{
		for(auto& midPoint : midPoints.second)
		{
			// 同一建物内同一住所ポイントがiPC住所ポイントに存在する場合は、スキップ
			if( AddrExistAttr::ExistSameAddrInArea == midPoint.addrExistAttr )
				continue;

			// 同一建物内に異なる住所を持つiPC住所ポイントが存在する場合は、スキップ
			if( AddrExistAttr::NoExist == midPoint.addrExistAttr && midPoint.isExistOtherPointInSameBld )
				continue;

			// 残りは中間ファイル住所ポイントが建物内に存在するが、その建物内にiPC住所は存在しないか、
			// 中間ファイル住所ポイントが建物内に存在しない場合
			errInfoRecs.emplace_back(
							AddrPointRec(),
							midPoint,
							( midPoint.isInBuilding ) ? _T("2"): _T("6"),
							m_args.m_mid_addr_layer.c_str()
						);
		}
	}
}

void CAddrPointDiff::diffNonUrbanAreaPoints(
			const CString& addrcode11,
			map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints,
			map<CString, AddrPointRecSet>& nonUrbanAreaMidAddrPoints
			)
{
	AddrPointRecs ipcAddrPoints;
	if( 0 != nonUrbanAreaAddrPoints.count( addrcode11 ) )
		nonUrbanAreaAddrPoints[addrcode11].getAddrPointRecs(ipcAddrPoints);

	AddrPointRecs midAddrPoints;
	if( 0 != nonUrbanAreaMidAddrPoints.count ( addrcode11 ) )
		nonUrbanAreaMidAddrPoints[addrcode11].getAddrPointRecs(midAddrPoints);

	for(auto& ipcPoint : ipcAddrPoints)
	{
		// iPC住所ポイントに対し、1件以上閾値以内の中間ファイル住所ポイントが存在するか
		bool isExistInAreaTotal = false;
		double minDist = DBL_MAX;
		vector<AddrPointWithSortKey> outMidPointRecs;
		map<AddrExistAttr, vector<AddrPointWithSortKey>> midPointRecs;

		for(auto& midPoint : midAddrPoints)
		{
			// 2点間の距離を算出
			double dist = _tstof(uh::str_util::ToString(
									AheGetMeterLength((IPointPtr)ipcPoint.geo, (IPointPtr)midPoint.geo),"%.2f"));
			if( minDist > dist ) minDist = dist;

			// 閾値距離以内に存在するか
			bool isExistInArea = (dist <= _tstof(m_args.m_point_dist.c_str()));
			// 同一住所か
			bool isExistSameAddr = ( ipcPoint == midPoint );

			AddrExistAttr attr = AddrExistAttr::NoExist;

			if( isExistSameAddr && isExistInArea ) // 閾値距離内同一住所ポイント
				attr = AddrExistAttr::ExistSameAddrInArea;
			else if( isExistInArea )	// 閾値距離内の住所ポイント
				attr = AddrExistAttr::ExistOtherAddrInArea;
			else if( isExistSameAddr )	// 閾値距離外の同一住所ポイント
				attr = AddrExistAttr::ExistSameAddrOutArea;
			else
				attr = AddrExistAttr::NoExist;

			midPointRecs[attr].emplace_back(&midPoint, midPoint.getAddressSort(dist, false));
		}

		// 差分パターンを決定する
		PointExistAttrPattern decidePattern = decideDiffPattern(midPointRecs, outMidPointRecs);
		
		// 複数の中間ファイル住所ポイントの中から、ログに出力するポイントを1つに絞る際、
		// iPC住所/中間ファイル住所ポイント間の距離を考慮するかどうか
		bool isConsiderDist = true;

		// 「閾値距離内に同一住所ポイントあり」
		// 「閾値距離内に住所ポイントはないが、閾値距離外には同一住所ポイントあり」の場合、
		// 中間ファイル住所ポイントの住所存在種別を変更する
		if( AddrExistAttr::ExistSameAddrInArea == decidePattern.first ||
				AddrExistAttr::ExistSameAddrOutArea == decidePattern.first )
		{
			for(auto& rec : outMidPointRecs)
				(*rec.first).addrExistAttr = decidePattern.first;

			// いずれかの場合は距離を考慮しないので、ついでにここでフラグをオフにする
			isConsiderDist = false;
		}
	
		// 決定した差分パターンで、iPC住所ポイントの住所存在種別も変更する
		ipcPoint.addrExistAttr = decidePattern.first;
		AddrPointWithSortKey ipcPointWithSk = std::make_pair(&ipcPoint, ipcPoint.getAddressSort(minDist, true));

		// 出力対象候補の中間ファイル住所のポイントの中から、1レコードを選定
		auto res = ( !outMidPointRecs.empty() ) ?
						*(selectNearestAddress(outMidPointRecs, ipcPointWithSk, isConsiderDist).first) : AddrPointRec();

		errInfoRecs.emplace_back(
				ipcPoint,
				res,
				decidePattern.second,
				m_args.m_mid_addr_layer.c_str()
			);
	}

	for(auto& point : midAddrPoints)
	{
		if( AddrExistAttr::ExistSameAddrInArea == point.addrExistAttr )
			continue;

		if( AddrExistAttr::ExistSameAddrOutArea == point.addrExistAttr )
			continue;

		errInfoRecs.emplace_back(
						AddrPointRec(),
						point,
						_T("11"),
						m_args.m_mid_addr_layer.c_str()
					);
	}
}

PointExistAttrPattern CAddrPointDiff::decideDiffPattern(
		map<AddrExistAttr, vector<AddrPointWithSortKey>>& midPointRecs,
		vector<AddrPointWithSortKey>& outMidPointRecs)
{
	// 差分パターンの判定は、ここで格納された順序で確認するため、この順序は重要
	static const vector<PointExistAttrPattern> AddrExistAttr = boost::assign::pair_list_of
		(AddrExistAttr::ExistSameAddrInArea, _T("7"))
		(AddrExistAttr::ExistOtherAddrInArea, _T("8"))
		(AddrExistAttr::ExistSameAddrOutArea, _T("9"))
		(AddrExistAttr::NoExist, _T("10"));

	PointExistAttrPattern decideAttr;
	for(const auto& attr : AddrExistAttr)
	{
		if( 0 != midPointRecs.count(attr.first) )
		{
			decideAttr = attr;
			outMidPointRecs = midPointRecs[attr.first];
			return decideAttr;;
		}
	}
	return make_pair(AddrExistAttr::NoExist, _T("10"));
}

bool CAddrPointDiff::openTable(const CString& conn, const CString& tableName, ITablePtr& table)
{
	CWorkspace workspace;

	// ワークスペース接続
	if(!workspace.Connect(conn))
	{
		return false;
	}

	// テーブルオープン
	table = workspace.OpenSameOwnersTable(tableName);
	if(!table)
	{
		return false;
	}

	return true;
}
