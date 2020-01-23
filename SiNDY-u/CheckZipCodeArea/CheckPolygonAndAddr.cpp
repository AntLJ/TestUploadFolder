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
	// CheckPolygonAndAddr�̃G���[�R�[�h��006xxx�Ƃ���
	const CString CityAdminDoesNotExist = _T("006001");
	const CString LessOverlapZipCodeArea = _T("006002");
	const CString LessOverlapCityAdmin = _T("006003");
	const CString AddressPointDoesNotExist = _T("006004");
	const CString ZipCodeAreaDoesNotExist = _T("006005");
}

/**
 * @brief �s�s�n�}�s���E�|���S���ƗX�֔ԍ��|���S���̏d�Ȃ���`�F�b�N����(�`�F�b�N�{��)
 * @param admin [in] �s�s�n�}�s����|���S���f�[�^
 * @param zipResult [in] �s���E�ɑΉ�����X�֔ԍ��|���S���Q
 * @param rate [in] �d�Ȃ藦����Ɏg�p����臒l
 * @retval true �X�֔ԍ��|���S���ɑ΂���s���E�̏d�Ȃ藦��臒l�ȏ�
 * @retval false �X�֔ԍ��|���S���ɑ΂���s���E�̏d�Ȃ藦��臒l����
 */
bool CheckDupliRate(
	const sindy::CSPContainerBase& admin,
	const sindy::CIndexedContainer::IteratorContainer& zipResult,
	int rate
)
{
	// CreateCache���ĂȂ��̂ŁA������CreateCache����
	// ���Ȃ���GetShape��Geometry���Ƃ�Ȃ�
	// CreateCache�Ȃ��ł�GetShapeCopy�Ȃ�Geometry���邪�AGetShape�̂ق��������Ƒ���
	CAST_ROWC(admin)->CreateCache();
	IGeometryPtr ipAdminShape = (IGeometry*)(*CAST_ROWC(admin)->GetShape());
	CAST_ROWC(admin)->CRowBase::DeleteCache(); // ����������}���邽�߁A����DeleteCache����
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

		// �s���E�|���S����臒l�ȏ�̊����ŗX�֔ԍ��|���S���Əd�Ȃ��Ă����OK
		if (intersectArea / adminArea * 100 >= rate)
			return true;
	}
	return false;
}

/**
 * @brief �s�s�n�}�s���E�|���S���ƗX�֔ԍ��|���S���̏d�Ȃ���`�F�b�N����(�`�F�b�N����/���ʎ擾)
 * @param addrZipMap [in] �Z���R�[�h�ƗX�֔ԍ��̑Ή��\
 * @param zipAreaTbl [in] �X�֔ԍ��|���S���f�[�^
 * @param cityAdminTbl [in, out] �s�s�n�}�s���E�f�[�^
 * @param rate [in] �d�Ȃ藦����Ɏg�p����臒l
 * @param errorInfo [in] �`�F�b�N�̌��ʃG���[���������̂̏��(OID�A�X�֔ԍ��A�Z���R�[�h)
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
		// �X�֔ԍ��|���S��������Ă݂�
		auto& zipResult1 = zipAreaTbl.finds_by_key(zipcode_area::kZipCode1, CComVariant(addrZip.second));
		auto& zipResult2 = zipAreaTbl.finds_by_key(zipcode_area::kZipCode2, CComVariant(addrZip.second));

		sindy::CIndexedContainer::IteratorContainer zipResult;
		zipResult.insert(zipResult1.begin(), zipResult1.end());
		zipResult.insert(zipResult2.begin(), zipResult2.end());

		if (zipResult.empty())
			continue;

		// �s���E����Ă݂�
		// �܂��Z���R�[�h1�Ō���
		static const sindy::KeyList key =
			boost::assign::list_of(CString(city_admin::kCityCode))(CString(city_admin::kAddrCode));
		sindy::ValueList val;
		val.push_back(CComVariant(addrZip.first.Left(5)));
		val.push_back(CComVariant(addrZip.first.Mid(5, 6)));
		auto adminResult1 = cityAdminTbl.finds_by_key(key, val);

		// �Z���R�[�h2�Ō���
		static const sindy::KeyList key2 =
			boost::assign::list_of(CString(city_admin::kCityCode))(CString(city_admin::kAddrCode2));
		auto adminResult2 = cityAdminTbl.finds_by_key(key2, val);

		sindy::CIndexedContainer::IteratorContainer adminResult;
		adminResult.insert(adminResult1.begin(), adminResult1.end());
		adminResult.insert(adminResult2.begin(), adminResult2.end());

		// �X�֔ԍ��|���S��������̂ɍs���E���Ȃ�
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
					_T("�X�֔ԍ��|���S���ɑΉ�����s���E���P���Ȃ�"),
					uh::str_util::format(_T("%s\t%s"), addrZip.second, addrZip.first)
				);
			}
			continue;
		}

		// �X�֔ԍ��|���S������݂čs���E�Əd�Ȃ��Ă��邩�ǂ���
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


		// �s���E����݂ėX�֔ԍ��|���S���Əd�Ȃ��Ă��邩�ǂ���
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
	// SDE(�X�֔ԍ��|���S��)�ڑ�
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S��DB�ڑ���ւ̐ڑ��Ɏ��s"));
		return false;
	}
	IFeatureClassPtr ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);

	// �t�@�C���_������
	CModel zipModel;
	m_relAddrFinder.InitCollection((ITablePtr)ipZipCodeArea, zipModel);

	// SDE(�s���E�A��)�ڑ�
	sindy::CWorkspace addrWorkspace;
	if (!addrWorkspace.Connect(m_args.m_addrDb.c_str()))
	{
		RUNLOGGER.Error(_T("�Z���f�[�^DB�ڑ���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �Z���f�[�^(�s���E�A��)�t�@�C���_������
	CModel addrModel;
	m_addrFinder.InitCollection(addrWorkspace.OpenTable(city_admin::kTableName), addrModel);

	// �����t���Z���R�[�h�ϊ��e�[�u���t�@�C���ڑ�
	if (!m_addrRecHelper.connectCharConvFile(m_args.m_charConv.c_str()))
	{
		RUNLOGGER.Error(_T("�����t���ϊ��e�[�u���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	// �����t���Z���R�[�h�ϊ����e�[�u���t�@�C���ڑ�
	if (!m_addrRecHelper.connectCharConvTmpFile(m_args.m_charConvTmp.c_str()))
	{
		RUNLOGGER.Error(_T("�����t���ϊ����e�[�u���ւ̐ڑ��Ɏ��s"));
		return false;
	}

	return true;
}

bool CheckPolygonAndAddr::Check()
{
	RUNLOGGER.Info(_T("�|���S���ƏZ���f�[�^�̐����`�F�b�N���s�c\n"));
	std::cout << "�|���S���ƏZ���f�[�^�̐����`�F�b�N���s�c" << std::endl;

	// �Z���f�[�^����݂ėX�֔ԍ��|���S�������邩�ǂ���
	bool result = true;
	RUNLOGGER.Info(_T("11���`�F�b�N���c\n"));
	std::cout << "11���`�F�b�N���c" << std::endl;
	result &= CheckContainCityAdmin();

	RUNLOGGER.Info(_T("16, 20���`�F�b�N���c\n"));
	std::cout << "16, 20���`�F�b�N���c" << std::endl;
	result &= CheckContainAddrPoint();

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;
	return result;
}

bool CheckPolygonAndAddr::CheckContainCityAdmin()
{
	// �R�t���e�[�u������11���̏Z���R�[�h�ɑΉ�����X�֔ԍ����擾����
	auto& relAddrTbl = *m_relAddrFinder.FindTable(rel_addr::kTableName20);
	relAddrTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// �X�֔ԍ��|���S��
	auto& zipAreaTbl = *m_relAddrFinder.FindTable(zipcode_area::kTableName);
	zipAreaTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// �s���E�|���S��
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// �������̓s����A�s���{�����ƂɃ`�F�b�N
	bool hasError = false;
	for (int i = 1; i <= 47; ++i)
	{
		RUNLOGGER.Log(_T("�s���{���R�[�h: %02d �̃`�F�b�N��\n"), i);
		auto prefCode = uh::str_util::formatA(_T("%02d"), i).GetString();
		std::cout << "�s���{���R�[�h: " << prefCode << "�̃`�F�b�N��" << std::endl;

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

		// �s���E�|���S�����擾����
		CString strWhere = uh::str_util::format(_T("%s LIKE '%02d%%%%'"), city_admin::kCityCode, i);
		cityAdminTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

		// �X�֔ԍ�500�����`�F�b�N
		std::map<long, std::pair<CString, CString>> zipErrorInfo;
		std::map<long, std::pair<CString, CString>> addrErrorInfo;
		auto inClauses = uh::enumerate_elements(zipCodes, 500, "'");
		for (auto& inClause : inClauses)
		{
			// �X�֔ԍ���Select
			strWhere = uh::str_util::format(
				_T("%s IN (%s) OR %s IN (%s)"),
				zipcode_area::kZipCode1, CString(inClause.c_str()),
				zipcode_area::kZipCode2, CString(inClause.c_str()));
			zipAreaTbl._Select(AheInitQueryFilter(nullptr, nullptr, strWhere), false);

			// �d�Ȃ���`�F�b�N
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
				_T("�X�֔ԍ��|���S���ɑΉ�����s���E��������Ȃ�"),
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
				_T("�X�֔ԍ��|���S���ɑ΂��čs���E�̏d�Ȃ藦��������"),
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

	// SiNDY�Z���f�[�^
	auto& cityAdminTbl = *m_addrFinder.FindTable(city_admin::kTableName);
	cityAdminTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& gouPointTbl = *m_addrFinder.FindTable(gou_point::kTableName);
	gouPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	auto& csPointTbl = *m_addrFinder.FindTable(cs_addr_point::kTableName);
	csPointTbl.SetAutoCache(sindy::sindyContainerAutoCacheNone);

	// CS�͏��Ȃ��̂ň�U�S��������Ă��܂�
	std::map<CString, AddrRecs> csAddrRecsMap;
	m_addrRecHelper.makeAdrRecordsFromCs(csPointTbl, _T(""), csAddrRecsMap);
	csPointTbl.clear();

	// �Z���R�t���e�[�u��
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

	// �X�֔ԍ��ƏZ���R�[�h�̃y�A�����B�Z���R�[�h��16���A20���̂��̂̂݁B
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

	// 11���R�[�h+����(�n�ԁA��)�̈ꗗ���쐬
	bool hasError = false;
	for (auto& zipAddr : zipAddrMap)
	{
		std::cout << "�X�֔ԍ��F " << CStringA(zipAddr.first).GetString() << std::endl;

		std::map<CString, AddrRecPairs> relAddrMap;
		for (auto& addrRec : zipAddr.second)
		{
			// ����(�n�ԁA��)�擾
			AddrRec tgtAddrRec(addrRec->GetValue(rel_addr::kAddrCode, CString()), nullptr);
			if (!m_addrRecHelper.makeAddrNames(tgtAddrRec))
			{
				ERRLOGGER.Log(_T("#�����t���Z���e�[�u�����疼�̎擾�Ɏ��s\t%s\n"), tgtAddrRec.getJusyocode());
				hasError = true;
				continue;
			}

			relAddrMap[tgtAddrRec.getJusyocode()].push_back(std::make_pair(tgtAddrRec, *addrRec));
		}

		if (hasError)
		{
			RUNLOGGER.Error(_T("11���R�[�h�{���̂̎擾�ɃG���[������܂��B(�G���[���O�Q��)\n"));
		}

		// �`�F�b�N
		for (auto & relAddrRecPairs : relAddrMap)
		{
			std::cout << "�Z���R�[�h: " << CStringA(relAddrRecPairs.first).GetString() << " ������" << std::endl;

			// 11���R�[�h����s���E�擾
			AddrRecPairs adminAddrRecs;
			m_addrRecHelper.getCityAdminRecs(cityAdminTbl, relAddrRecPairs.first, adminAddrRecs);
			cityAdminTbl.clear();

			// 11���R�[�h����CS�|�C���g�擾
			AddrRecs csAddrRecs;
			if (0 != csAddrRecsMap.count(relAddrRecPairs.first.Left(11)))
			{
				//csAddrRecs = csAddrRecsMap[relAddrRecPairs.first];
				csAddrRecs = csAddrRecsMap[relAddrRecPairs.first.Left(11)];
			}

			// ���ݔ���
			for (auto& relAddrRec : relAddrRecPairs.second)
			{
				AddrRecs existRecs;
				bool exist = util.CheckAddressDataExistance(
					adminAddrRecs, csAddrRecs, gouPointTbl, relAddrRec.first, existRecs);

				if (!exist)
				{
					// �����ɓ����Ă���Ƃ������Ƃ͕R�t���e�[�u���̍X�V�������Ƃ���ĂȂ�
					// table_addr�̃`�F�b�N�ł���Ă�̂ŁA�����ł͖���
					continue;
				}

				// �X�֔ԍ��|���S�������
				auto& whereClause = uh::str_util::format(
					_T("%s = '%s' OR %s = '%s'"),
					zipcode_area::kZipCode1,
					zipAddr.first,
					zipcode_area::kZipCode2,
					zipAddr.first);
				zipCodeAreaTbl._Select(AheInitQueryFilter(nullptr, nullptr, whereClause), false);

				// �X�֔ԍ��|���S������݂ďZ�������邩�ǂ���
				hasError &= ZipToAddrCheck(zipCodeAreaTbl, existRecs,
					zipAddr.first, relAddrRec.first.getJusyocode());

				// �Z������݂ėX�֔ԍ��|���S�������邩�ǂ���
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
			// ���ECS�|�C���g�A�s���E�|���S�����X�֔ԍ��|���S���ɓ����Ă����OK
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
				_T("�X�֔ԍ��|���S���ɑΉ�����Z���f�[�^���Ȃ�"),
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
		// ���ECS�|�C���g�A�s���E�|���S�����X�֔ԍ��|���S���ɓ����Ă����OK
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
				// ���[�v����rec�ɊY������CITY_ADMIN��OID����肽�����A
				// AddrRec�����OID���Ȃ��̂ŁA��\�_�v�Z���ă��O�����ׂ�悤�ɂ���B
				auto point =  m_repPointCalculator.CalculateRepWKSPoint(existGeometry, rpc::OnSurface);
				hasError = true;
				ERRLOGGER.writeStdLog(
					city_admin::kTableName,
					0, // OID��0�Ƃ��ďo�͂���
					uh::str_util::format(_T("%.20lf"), point.X),
					uh::str_util::format(_T("%.20lf"), point.Y),
					_T("ERROR"),
					ErrorCode::LessOverlapCityAdmin,
					_T("�X�֔ԍ��|���S���ɑ΂��čs���E�̏d�Ȃ藦��������"),
					uh::str_util::format(_T("%s\t%s"), zipCode, addrCode)
				);
			}
			break;
		}
		case esriGeometryPoint:
		{
			// �|�C���g�Ƃ̐���
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
				// ����CS���͂킩��Ȃ��̂ŁA���O�͍��W�l�Ŕ�ׂ�悤�ɂ���
				IPointPtr ipPoint(existGeometry);
				double x = 0, y = 0;
				ipPoint->QueryCoords(&x, &y);

				ERRLOGGER.writeStdLog(
					_T("GOU/CS"),
					0, // OID��0�Ƃ��ďo�͂���
					uh::str_util::format(_T("%.20lf"), x),
					uh::str_util::format(_T("%.20lf"), y),
					_T("ERROR"),
					ErrorCode::ZipCodeAreaDoesNotExist,
					_T("�Z���|�C���g�ɑΉ�����X�֔ԍ��|���S�����Ȃ�"),
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

