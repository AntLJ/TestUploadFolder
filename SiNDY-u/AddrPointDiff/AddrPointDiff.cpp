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

// ������
bool CAddrPointDiff::init()
{
	cout << (CT2A)uh::str_util::format(_T("������")) << endl;

	// SiNDY�i�Z���j
	if( !initCollection(m_addrFinder, m_args.m_ipc_addr_db.c_str(), city_admin::kTableName) )
		return false;

	// SiNDY�i�w�i�j
	if( !initCollection(m_bgFinder, m_args.m_ipc_bg_db.c_str(), building::kTableName) )
		return false;

	// ���ԃt�@�C���Z��
	if( !initCollection(m_midAddrPointFinder, m_args.m_mid_addr_db.c_str(), m_args.m_mid_addr_layer.c_str()) )
		return false;

		// ���s���O�I�[�v��
	if( !RUNLOGGER.Initialize( (uh::tstring)m_args.m_run_log) )
	{
		cerr << (CT2A)uh::str_util::format(_T("FATAL\t���s���O���J���܂���:%s"), m_args.m_run_log) << endl;
		return false;
	}

	// ���s���O�փI�v�V��������������
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
	// �����Ώۂ̃|�C���g�Q�i�}���`�|�C���g�j�ɐڂ���BUILDING���擾
	auto query = AheInitQueryFilter
					(
						(IQueryFilterPtr)AheInitSpatialFilter( nullptr, targetGeo ),
						nullptr
					);
	buildingT._Select(query, false);

	// BUILDING�̌`���Geospace�ƌ`ID�̃y�A�𐶐�
	vector<pair<IGeometryPtr, CString>> buildings;

	long geoIdIdx = buildingT.FindField(building::kGeospaceID);
	for(const auto& bld : buildingT)
	{
		auto& row = *CAST_ROWC(bld);
		buildings.emplace_back(row.CRowBase::GetShapeCopy(), row.GetValue(geoIdIdx, CString()));
	}

	// �Ώۂ̏Z���|�C���g�����ABUILDING��ɑ��݂��邩�m�F
	for(auto& rec : addrPointRecs)
	{
		// ���Ɍ��������݃t���O�������Ă���ꍇ�̓X�L�b�v
		if( rec.isInBuilding )
			continue;

		for(const auto& bld : buildings)
		{
			// �擾����BUILDING�Ɛڂ���ꍇ�A���������݃t���O�𗧂āA
			// ���̌�����Geospace�ƌ`ID���i�[����
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
	// ���̊֐����̏��������\������

	// BUILDING�ɑ΂��鑮�������p�ɁAGeospace�ƌ`ID�̃��X�g�𐶐�
	// �܂��Ώۂ�Geospace�ƌ`ID�ɕR�t���Z���|�C���g���������邽�߂�map�������Đ���
	set<CStringA> geoIdList;
	map<CString, vector<AddrPointRec*>> pointRecs;
	for(auto& point : addrPointRecs)
	{
		if( point.geoBldId.IsEmpty() )
			continue;

		geoIdList.emplace(point.geoBldId);
		pointRecs[point.geoBldId].emplace_back(&point);
	}

	// Geospace�ƌ`ID�����������ɂ��Č����i1000�����ɕ����Č����j
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

		// BUILDING����擾����������Geospace�ƌ`ID�ƈ�v����
		// �Z���|�C���g�ɑ΂��A���������݃t���O�𗧂Ă�
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

	// �ΏۃW�I���g���̐ڂ���CITY_ADMIN�̃I�u�W�F�N�g���擾
	auto query = AheInitQueryFilter
			(
				(IQueryFilterPtr)AheInitSpatialFilter( nullptr, targetGeo),
				nullptr
			);
	cityAdminT._Select(query, false);

	// �擾�����I�u�W�F�N�g�̂����A�s�s�n�}�G���A���iaddrclass_c=1�j�̌`���1�̌`��Ƀ}�[�W
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
		RUNLOGGER.Error(_T("CITY_ADMIN(�s�s�n�}�G���A��)�̌`��}�[�W�Ɏ��s"));
		return false;
	}

	if( !ipMargePolygon )
	{
		RUNLOGGER.Error(_T("�}�[�W����CITY_ADMIN(�s�s�n�}�G���A��)�̌`�󂪕s��"));
		return false;
	}

	// �s�s�n�}�G���A���O�𔻒肷��Z���|�C���g���A���[�v
	IRelationalOperatorPtr iRelOp( ipMargePolygon );
	for(auto& rec : addrPointRecs)
	{
		// �}�[�W����CITY_ADMIN�i�s�s�n�}�G���A���̌`��j�Ƀ|�C���g���܂܂�邩����
		// �܂܂��F�s�s�n�}�G���A���A�܂܂�Ȃ��F�s�s�n�}�G���A�O
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
	case AddrPointType::GouPoint: // ���|�C���g���琶��
		{
			// ���ԍ����́u#�v�͍폜����
			CString gouNo = targetRow.GetValue(gou_point::kGouNo, CString());
			gouNo.Replace(_T("#"), _T(""));

			bool extGouNo = targetRow.GetValue(gou_point::kExpGouNo, false);
		
			if( !gaikuFugo.IsEmpty() ) // �X�敄���ɒl������ꍇ
			{
				address = ( !extGouNo ) ? 
							uh::str_util::format(_T("%s%s%s"), gaikuFugo, SEPARATOR_HALF_HYPHEN, gouNo) :
							gouNo;
			}
			else if( !extGaikuFugo.IsEmpty() ) // �g���X�敄���ɒl������ꍇ
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
	case AddrPointType::CsAddrPoint: // CS�|�C���g���琶��
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
	case AddrPointType::GsAddrPoint: // ���ԃt�@�C���Z�����琶��
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
	// �ǉ��Ώۂ̏Z���|�C���g���A���[�v
	for(auto& point : addrPointRecs)
	{
		addrPointSets[point.addrcode].setAddrPointRecs(point);

		// �����Ώۂ�CITY_ADMIN��addrcode2���ݒ肳��Ă���ꍇ�A
		// addrcode2�̏Z��11���R�[�h�����Z���|�C���g������
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

	// �Z��������ʂ��u0�v�܂��́A�Z���R�[�h���u999999�v�͏��O
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

		// ���ԍ����u*�v�͏��O����
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

// ���s
bool CAddrPointDiff::execute()
{
	bool ret = true;

	// ������
	if( !init() )
		return false;

	auto& cityAdminT = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminT.SetAutoCache(sindyContainerAutoCacheNone);

	auto& buildingT = *m_bgFinder.FindTable(building::kTableName);
	buildingT.SetAutoCache(sindyContainerAutoCacheNone);

	RUNLOGGER.Info(_T("�s�撬���R�[�h���X�g�Ǎ���\n"), true);

	auto& citycodeList = uh::text_util::ToTStrList(m_args.m_citycode_list.c_str());

	for(const auto& citycode : citycodeList)
	{
		// �ǂݍ��񂾎s�撬���R�[�h����̏ꍇ�A�X�L�b�v
		if( citycode.empty() )
			continue;

		set<CString> addrcode11List;
		map<CString, AddrPointRecSet> urbanAreaAddrPoints;
		map<CString, AddrPointRecSet> nonUrbanAreaAddrPoints;

		map<CString, AddrPointRecSet> urbanAreaMidAddrPoints;
		map<CString, AddrPointRecSet> nonUrbanAreaMidAddrPoints;
	
		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("���|�C���g�擾(%s)\n"), citycode.c_str()), true);

		// ���|�C���g�����r�ΏۂƂȂ�Z���|�C���g���擾
		getAddrPointsFromGouPoint(
						citycode.c_str(),
						cityAdminT,
						buildingT,
						addrcode11List,
						urbanAreaAddrPoints,
						nonUrbanAreaAddrPoints
					);

		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("CS�|�C���g�擾(%s)\n"), citycode.c_str()),  true);

		// CS�|�C���g�����r�ΏۂƂȂ�Z���|�C���g���擾
		ret &= getAddrPointsFromCsPoint(
						citycode.c_str(),
						cityAdminT,
						buildingT,
						addrcode11List,
						urbanAreaAddrPoints,
						nonUrbanAreaAddrPoints
					);

		RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("���ԃt�@�C���Z���|�C���g�擾(%s)\n"), citycode.c_str()), true);

		// ���ԃt�@�C���Z���|�C���g�����r�ΏۂƂȂ�Z���|�C���g���擾
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
			// �G���[���O�I�[�v��
			if( !ERRLOGGER.Initialize( (uh::tstring)errlog ) )
			{
				RUNLOGGER.Error((uh::tstring)uh::str_util::format(_T("FATAL\t�G���[���O���J���܂���:%s\n"), errlog), true);
				ERRLOGGER.Finalize();
				ret = false;
				continue;
			}

			// �w�b�_������
			ERRLOGGER.Log(_T("#%s FILEVERSION %s\n"), ver.GetInternalName(), ver.GetFileVersion());
			ERRLOGGER.Log(_T("%s\n"), uh::str_util::join(ERRLOG_HEADER_LIST, _T("\t")));

			RUNLOGGER.Info((uh::tstring)uh::str_util::format(_T("�����擾����(%s)\n"), addrcode11), true);

			// �s�s�n�}�G���A���̏Z���|�C���g���m�ɔ�r
			diffUrbanAreaPoints(addrcode11,
									urbanAreaAddrPoints,
									urbanAreaMidAddrPoints);

			// �s�s�n�}�G���A�O�̏Z���|�C���g���m�ɔ�r
			diffNonUrbanAreaPoints(addrcode11,
									nonUrbanAreaAddrPoints,
									nonUrbanAreaMidAddrPoints);

			// �~�ς����G���[���O�ɏ�����
			writeErrLog();

			// �t�@�C���N���[�Y
			ERRLOGGER.Finalize();

		}
	}

	return ret;
}



void CAddrPointDiff::writeErrLog()
{
	// �o�͑O�Ƀ\�[�g
	sort(errInfoRecs.begin(), errInfoRecs.end());

	// ������
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
	// �Z���|�C���g�Z�b�g�iiPC�Z���j����Ώۂ̏Z��11���R�[�h�ɕR�t���|�C���g�Q���擾
	AddrPointRecsByCode ipcAddrPoints;
	if( 0 != urbanAreaAddrPoints.count( addrcode11 ) )
		urbanAreaAddrPoints[addrcode11].getAddrPointRecs(ipcAddrPoints);

	// �Z���|�C���g�Z�b�g�i���ԃt�@�C���Z���j����Ώۂ̏Z��11���R�[�h�ɕR�t���|�C���g�Q���擾
	AddrPointRecsByCode midAddrPoints;
	if( 0 != urbanAreaMidAddrPoints.count ( addrcode11 ) )
		urbanAreaMidAddrPoints[addrcode11].getAddrPointRecs(midAddrPoints);

	// iPC�Z���|�C���g�̐��������[�v
	for(auto& ipcPoints : ipcAddrPoints)
	{
		for(auto& ipcPoint : ipcPoints.second)
		{
			// iPC�Z���|�C���g��������ɑ��݂��Ȃ��ꍇ
			if( !ipcPoint.isInBuilding )
			{
				errInfoRecs.emplace_back(ipcPoint, AddrPointRec(), _T("5"));
				continue;
			}

			// iPC�Z���|�C���g��������ɑ��݂��邪�A
			// ����Geospace�ƌ`ID�������ԃt�@�C���̏Z���|�C���g�����݂��Ȃ�
			// �܂��́AGeopsace�ƌ`ID�l�������ꍇ
			if( 0 == midAddrPoints.count( ipcPoint.geoBldId ) || ipcPoint.geoBldId.IsEmpty() )
			{
				errInfoRecs.emplace_back(ipcPoint, AddrPointRec(), _T("3"));
				continue;
			}

			// ����Geospace�ƌ`ID�������ԃt�@�C���Z���|�C���g�̐��������[�v
			for(auto& midPoint : midAddrPoints[ipcPoint.geoBldId])
			{
				// iPC�Z���|�C���g�ƒ��ԃt�@�C���Z���|�C���g������Z��
				if( ipcPoint == midPoint )
				{
					midPoint.addrExistAttr = AddrExistAttr::ExistSameAddrInArea;
					errInfoRecs.emplace_back(ipcPoint, midPoint, _T("1"), m_args.m_mid_addr_layer.c_str());
					break;
				}

				// iPC�Z���|�C���g�ƒ��ԃt�@�C���Z���|�C���g������Z���ł͂Ȃ�
				midPoint.isExistOtherPointInSameBld = true;
				errInfoRecs.emplace_back(ipcPoint, midPoint, _T("4"), m_args.m_mid_addr_layer.c_str());
			}
		}
	}

	// ���ԃt�@�C���Z���|�C���g�̐��������[�v
	for(auto& midPoints : midAddrPoints)
	{
		for(auto& midPoint : midPoints.second)
		{
			// ���ꌚ��������Z���|�C���g��iPC�Z���|�C���g�ɑ��݂���ꍇ�́A�X�L�b�v
			if( AddrExistAttr::ExistSameAddrInArea == midPoint.addrExistAttr )
				continue;

			// ���ꌚ�����ɈقȂ�Z��������iPC�Z���|�C���g�����݂���ꍇ�́A�X�L�b�v
			if( AddrExistAttr::NoExist == midPoint.addrExistAttr && midPoint.isExistOtherPointInSameBld )
				continue;

			// �c��͒��ԃt�@�C���Z���|�C���g���������ɑ��݂��邪�A���̌�������iPC�Z���͑��݂��Ȃ����A
			// ���ԃt�@�C���Z���|�C���g���������ɑ��݂��Ȃ��ꍇ
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
		// iPC�Z���|�C���g�ɑ΂��A1���ȏ�臒l�ȓ��̒��ԃt�@�C���Z���|�C���g�����݂��邩
		bool isExistInAreaTotal = false;
		double minDist = DBL_MAX;
		vector<AddrPointWithSortKey> outMidPointRecs;
		map<AddrExistAttr, vector<AddrPointWithSortKey>> midPointRecs;

		for(auto& midPoint : midAddrPoints)
		{
			// 2�_�Ԃ̋������Z�o
			double dist = _tstof(uh::str_util::ToString(
									AheGetMeterLength((IPointPtr)ipcPoint.geo, (IPointPtr)midPoint.geo),"%.2f"));
			if( minDist > dist ) minDist = dist;

			// 臒l�����ȓ��ɑ��݂��邩
			bool isExistInArea = (dist <= _tstof(m_args.m_point_dist.c_str()));
			// ����Z����
			bool isExistSameAddr = ( ipcPoint == midPoint );

			AddrExistAttr attr = AddrExistAttr::NoExist;

			if( isExistSameAddr && isExistInArea ) // 臒l����������Z���|�C���g
				attr = AddrExistAttr::ExistSameAddrInArea;
			else if( isExistInArea )	// 臒l�������̏Z���|�C���g
				attr = AddrExistAttr::ExistOtherAddrInArea;
			else if( isExistSameAddr )	// 臒l�����O�̓���Z���|�C���g
				attr = AddrExistAttr::ExistSameAddrOutArea;
			else
				attr = AddrExistAttr::NoExist;

			midPointRecs[attr].emplace_back(&midPoint, midPoint.getAddressSort(dist, false));
		}

		// �����p�^�[�������肷��
		PointExistAttrPattern decidePattern = decideDiffPattern(midPointRecs, outMidPointRecs);
		
		// �����̒��ԃt�@�C���Z���|�C���g�̒�����A���O�ɏo�͂���|�C���g��1�ɍi��ہA
		// iPC�Z��/���ԃt�@�C���Z���|�C���g�Ԃ̋������l�����邩�ǂ���
		bool isConsiderDist = true;

		// �u臒l�������ɓ���Z���|�C���g����v
		// �u臒l�������ɏZ���|�C���g�͂Ȃ����A臒l�����O�ɂ͓���Z���|�C���g����v�̏ꍇ�A
		// ���ԃt�@�C���Z���|�C���g�̏Z�����ݎ�ʂ�ύX����
		if( AddrExistAttr::ExistSameAddrInArea == decidePattern.first ||
				AddrExistAttr::ExistSameAddrOutArea == decidePattern.first )
		{
			for(auto& rec : outMidPointRecs)
				(*rec.first).addrExistAttr = decidePattern.first;

			// �����ꂩ�̏ꍇ�͋������l�����Ȃ��̂ŁA���łɂ����Ńt���O���I�t�ɂ���
			isConsiderDist = false;
		}
	
		// ���肵�������p�^�[���ŁAiPC�Z���|�C���g�̏Z�����ݎ�ʂ��ύX����
		ipcPoint.addrExistAttr = decidePattern.first;
		AddrPointWithSortKey ipcPointWithSk = std::make_pair(&ipcPoint, ipcPoint.getAddressSort(minDist, true));

		// �o�͑Ώی��̒��ԃt�@�C���Z���̃|�C���g�̒�����A1���R�[�h��I��
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
	// �����p�^�[���̔���́A�����Ŋi�[���ꂽ�����Ŋm�F���邽�߁A���̏����͏d�v
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

	// ���[�N�X�y�[�X�ڑ�
	if(!workspace.Connect(conn))
	{
		return false;
	}

	// �e�[�u���I�[�v��
	table = workspace.OpenSameOwnersTable(tableName);
	if(!table)
	{
		return false;
	}

	return true;
}
