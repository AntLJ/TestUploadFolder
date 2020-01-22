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
#include "BldNameImporter.h"
#include "LogSys.h"
#include "common.h"
#include "ctime.h"

using namespace sindy::schema::sj;

BldNameImporter::BldNameImporter(
	const std::vector<long>& srcPriListPrior,
	const std::vector<long>& srcPriListMatch,
	const std::vector<CString>& poiPriList,
	const std::vector<CString>& supPoiPriList,
	const std::vector<StrPair>& replaceList,
	const std::set<long>& bldExcList,
	const std::map<long, long>& condList,
	const std::map<CString, long>& bldSrcIndeces,
	const std::map<CString, long>& bldAddInfoIndeces,
	const std::map<CString, long>& bldIndeces,
	const std::map<CString, long>& twnpageIndeces,
	const std::map<CString, long>& poiPtOrgIndeces,
	const std::map<CString, long>& bldNamePtIndeces,
	const std::map<CString, long>& relBldNameIndeces,
	const IFeatureClassPtr& ipBldSrcPt,
	const ITablePtr& ipBldAddInfo,
	const IFeatureClassPtr& ipBld,
	const IFeatureClassPtr& ipCityAdm,
	const IFeatureClassPtr& ipTwnpage,
	const IFeatureClassPtr& ipPoiPtOrg,
	const IFeatureClassPtr& ipBldNamePt,
	const ITablePtr& ipRelBldName,
	bool isUpdate,
	bool isSurvey,
	SurveyPointManager& surveyPtMgr
	)
	: m_bldExcList(bldExcList), m_bldIndeces(bldIndeces), m_twnpageIndeces(twnpageIndeces), m_poiPtOrgIndeces(poiPtOrgIndeces),
	m_bldSrcIndeces(bldSrcIndeces), m_bldAddInfoIndeces(bldAddInfoIndeces), m_bldNamePtIndeces(bldNamePtIndeces), m_relBldNameIndeces(relBldNameIndeces),
	m_ipBldSrcPt(ipBldSrcPt), m_ipBldAddInfo(ipBldAddInfo), m_ipBld(ipBld), m_ipCityAdm(ipCityAdm),
	m_ipTwnpage(ipTwnpage), m_ipPoiPtOrg(ipPoiPtOrg), m_ipBldNamePt(ipBldNamePt), m_ipRelBldName(ipRelBldName),
	m_isUpdate(isUpdate), m_isSuvey(isSurvey), m_surveyPtMgr( surveyPtMgr )
{
	m_matchingProcessor = new MatchingProcessor(srcPriListPrior, srcPriListMatch, poiPriList, supPoiPriList, replaceList, condList, bldSrcIndeces, poiPtOrgIndeces, m_mapAddrCodeCounter, isSurvey, surveyPtMgr);
}

BldNameImporter::~BldNameImporter(void)
{
	delete m_matchingProcessor;
}

bool BldNameImporter::Init()
{
	// �o�T��ʃR�[�h�l���X�g���擾
	if (! ::common_proc::GetCodedValues(m_ipBldSrcPt, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, m_srcTypeCodedValues))
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("�o�T��ʃR�[�h�l���X�g�̍쐬�Ɏ��s"), ::buildingname_src_point::kTableName); 
		return false;
	}

	// �u�h�ΑΏە����v�|�C���g�E20���Z���R�[�h�J�E���g���X�g�̍쐬
	// Where��ݒ�
	CString strWhere;
	strWhere.Format(_T("%s = %d"), ::buildingname_src_point::kSourceType, ::buildingname_src_point::source_type::kFirePreventive);
	IFeatureCursorPtr ipCurSrcPt;
	if (! ::common_proc::SearchByWhereClause(m_ipBldSrcPt, ipCurSrcPt, strWhere, true))
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("20���Z���R�[�h�J�E���g���X�g�̍쐬�Ɏ��s"), ::buildingname_src_point::kTableName); 
		return false;
	}
	IFeaturePtr ipSrcPt;
	while (SUCCEEDED(ipCurSrcPt->NextFeature(&ipSrcPt)) && ipSrcPt)
	{
		// 20���Z���R�[�h���擾
		CComVariant addrCode;
		if (! ::common_proc::GetValue((_IRowPtr)ipSrcPt, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrCode, m_bldSrcIndeces, &addrCode))
		{
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("20���Z���R�[�h�J�E���g���X�g�̍쐬�Ɏ��s"), ::buildingname_src_point::kTableName); 
			return false;
		}
		CString strAddrCode(addrCode.bstrVal);
		// ���Y20���Z���R�[�h�̃J�E���^��+1
		if (!strAddrCode.IsEmpty()) ++m_mapAddrCodeCounter[strAddrCode];
	}

	return true;
}

bool BldNameImporter::Import(const CString& addrCode)
{
	// �C���|�[�g�p�e�탌�R�[�h�����N���A
	m_bldNamePtInfos.clear();
	m_relBldNameInfos.clear();
	m_deleteBldNamePtIDs.clear();

	// 5���R�[�h���Ώۍs���E�擾���A�}�[�W�`��擾
	std::cout << CT2A(addrCode) << "\t���Y�s�撬���s���E�}�[�W��" << std::endl;
	IGeometryPtr tgtAdmGeometry;
	if (! GetTgtCityAdms(addrCode, tgtAdmGeometry))
		return false;
	if (!tgtAdmGeometry) {
		CString errMsg;
		errMsg.Format(_T("�����Ώۍs���E�����݂��܂���B5���R�[�h�F%s"), addrCode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, errMsg, sindy::schema::city_admin::kTableName);
		return false;
	}

	// �s���E��intersect���錚����overlap���錚����oid���X�g�擾
	std::cout << CT2A(addrCode) << "\t���Y�s�撬���������|���S���擾��" << std::endl;
	IDFeatureMap intersectBldFeatures;
	std::set<long> overlapBldIDs;
	if (! GetTgtBldInfos(tgtAdmGeometry, &intersectBldFeatures, &overlapBldIDs))
		return false;

	// �������J��Ԃ����ADB�X�V���擾
	long i = 0;
	for (const auto& bldFeatureInfo : intersectBldFeatures) {
		if (0 == i % 100)
			std::cout << CT2A(addrCode) << "\t���̃|�C���g������ [ " << std::setw(6) << i << " / " << std::setw(6) << intersectBldFeatures.size() << " ]\r";
		++i;
		if( !GetImportRecordInfos(bldFeatureInfo.first, bldFeatureInfo.second.m_T, overlapBldIDs))
			return false;
		// DB�X�V
		if (! DBUpdate(false))
			return false;
	}
	std::cout << CT2A(addrCode) << "\t���̃|�C���g������ [ " << std::setw(6) << i << " / " << std::setw(6) << intersectBldFeatures.size() << " ]" << std::endl;

	// DB�X�V�i�S�����f�j
	if (! DBUpdate(true))
		return false;

	// �����|�C���g���X�g�͍s���E���̏����̍Ō�ŏo�͂���B
	if (m_isSuvey) {
		std::cout << CT2A(addrCode) << "\t�����Ώۃ|�C���g���X�g�o�͒�" << std::endl;
		m_surveyPtMgr.output();
	}

	return true;
}

bool BldNameImporter::GetTgtCityAdms(const CString& addrCode, IGeometryPtr& tgtAdmGeometry)
{
	CString errMsg;

	// Where��쐬
	CString whereClause;
	whereClause.Format(_T("%s = '%s'"), sindy::schema::city_admin::kCityCode, addrCode);
	// Cursor�擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByWhereClause(m_ipCityAdm, ipFeatureCursor, whereClause))
		return false;

	// �W�I���g���o�b�O�����i��ԎQ�ƕt�^�j
	IGeometryCollectionPtr ipGeomColl(CLSID_GeometryBag);
	ISpatialReferencePtr ipSpRef;
	if(FAILED(IGeoDatasetPtr(m_ipCityAdm)->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̎擾�Ɏ��s���܂���"), sindy::schema::city_admin::kTableName);
		return false;
	}
	if(FAILED(IGeometryPtr(ipGeomColl)->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̕t�^(GeometryBag)�Ɏ��s���܂���"), sindy::schema::city_admin::kTableName);
		return false;
	}

	// �t�B�[�`���擾
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid�擾
		long oid = -1;
		ipFeature->get_OID(&oid);
		// geometry�擾
		IGeometryPtr ipGeometry;
		if (FAILED(ipFeature->get_Shape(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), sindy::schema::city_admin::kTableName, oid);
			return false;
		}
		if (FAILED(ipGeomColl->AddGeometry(ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̒ǉ��Ɏ��s���܂���"), sindy::schema::city_admin::kTableName, oid);
			return false;
		}
	}

	// geometry�����݂��Ȃ������ꍇ�ANULL��n���ďI��
	long countGeom = 0;
	IEnumGeometryPtr(ipGeomColl)->get_Count(&countGeom);
	if (0 == countGeom) {
		tgtAdmGeometry = nullptr;
		return true;
	}

	// geometries�}�[�W
	IPolygonPtr ipUnion(CLSID_Polygon);
	if(FAILED(ipUnion->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̕t�^(Polygon)�Ɏ��s���܂���"), sindy::schema::city_admin::kTableName);
		return false;
	}
	if (FAILED(ITopologicalOperatorPtr(ipUnion)->ConstructUnion(IEnumGeometryPtr(ipGeomColl)))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̓����Ɏ��s���܂���"), sindy::schema::city_admin::kTableName);
		return false;
	}
	tgtAdmGeometry = ipUnion;

	return true;
}

bool BldNameImporter::GetTgtBldInfos(const IGeometryPtr& ipAdmGeometry, IDFeatureMap* intersectBldFeatures, std::set<long>* overlapBldIDs)
{
	CString errMsg;

	// overlap�����p��IRelationalOperator������
	IRelationalOperatorPtr ipRel(ipAdmGeometry);

	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipBld, ipFeatureCursor, ipAdmGeometry, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid�擾
		long oid = -1;
		ipFeature->get_OID(&oid);
		// geometry�̎擾
		IGeometryPtr ipGeometry;
		if (FAILED(ipFeature->get_Shape(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
			return false;
		}
		// �s���E��contains���Ă��邩���ׂ�
		VARIANT_BOOL containsChk = VARIANT_FALSE;
		if (FAILED(ipRel->Contains(ipGeometry, &containsChk))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�s���E�Ƃ�Contains����Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (containsChk == VARIANT_TRUE) {
			// �R���e�i�ɓo�^
			intersectBldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
		} else {
			// �s���E��overlap���Ă��邩�`�F�b�N
			VARIANT_BOOL overlapChk = VARIANT_FALSE;
			if (FAILED(ipRel->Overlaps(ipGeometry, &overlapChk))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�s���E�Ƃ�Overlaps����Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
				return false;
			}
			if (overlapChk == VARIANT_TRUE) {
				// overlap���Ă����ꍇ�A���X�g�ɓo�^���ăR���e�i�ɓo�^
				overlapBldIDs->insert(oid);
				intersectBldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
			}
			// overlap�����Ă��Ȃ��ꍇ�͏����ΏۊO(�^�b�`���Ă���Ƃ������ƂɂȂ�)
		}
	}
	return true;
}

bool BldNameImporter::GetImportRecordInfos(long bldID, const IFeaturePtr& ipBldFeature, const std::set<long>& overlapBldIDs)
{
	// �������O�Ώی������`�F�b�N������
	CComVariant bldClass;
	if (! ::common_proc::GetValue((_IRowPtr)ipBldFeature, sindy::schema::building::kTableName, sindy::schema::building::kBldClass, m_bldIndeces, &bldClass))
		return false;
	if (m_bldExcList.find(bldClass.lVal) != m_bldExcList.end())
		return true;	// �����ΏۊO

	// �s���E��overlap�������������ׂ�
	if (overlapBldIDs.find(bldID) != overlapBldIDs.end()) {
		// �ϊ��ς����ׂ�
		if (m_cnvdOverlapBldIDs.find(bldID) != m_cnvdOverlapBldIDs.end())
			return true;	// �ϊ��ςȂ̂ŏ������Ȃ�
		// �ϊ���ID�Ƃ��ēo�^����B
		m_cnvdOverlapBldIDs.insert(bldID);
		// �o�͐�e�[�u��(�����r�����̃|�C���g)�Ɍ��ݏ������Ă��錚���ɕR�Â��|�C���g�����݂��邩���ׂ�
		// ���݂����ꍇ�A�폜����B
		bool existEditPoint = false;
		if (! ChkExistBldNamePt(bldID, &existEditPoint))
			return false;
		if (existEditPoint)
			return true;	// ���ƂŕҏW���������r�����̃|�C���g�����݂���ꍇ�́A�������̍s��Ȃ�
	}

	// �����t�B�[�`�����猊���ߍσ|���S�����擾
	IPolygonPtr ipFilledBldPolygon;
	if (! GetFilledPolygon(ipBldFeature, ipFilledBldPolygon))
		return false;

	// �����`����ɑ��݂���f�ރ|�C���g�擾
	IDFeatureMap tgtSrcPtFeatures;
	if (! GetTgtSrcPt(ipFilledBldPolygon, &tgtSrcPtFeatures))
		return false;

	// �����`����ɑ��݂��钍�LPOI�|�C���g�擾
	IDFeatureMap tgtPoiPtFeatures;
	if (! GetTgtPoiPt(ipFilledBldPolygon, &tgtPoiPtFeatures))
		return false;

	// �}�b�`���O����
	BldNamePtInfo bldNamePtInfo;
	std::set<long> relIDs;

	bool result = false;
	bool excepted = false;
	bool selected = false;
	if (! m_matchingProcessor->MatchingProc(tgtSrcPtFeatures,
											tgtPoiPtFeatures,
											bldID,
											ipFilledBldPolygon,
											&bldNamePtInfo,
											&relIDs,
											&result,
											&excepted,
											&selected))
		return false;
	if (result)
	{
		m_bldNamePtInfos[bldNamePtInfo.e_srcOID] = bldNamePtInfo;
		if (relIDs.size() > 0)
			m_relBldNameInfos[bldNamePtInfo.e_srcOID] = relIDs;
	}
	else
	{
		// �����Ώۃ|�C���g���X�g�o�̓��[�h�̏ꍇ�ATOWNPAGE�f�[�^�����݂��邩�`�F�b�N���A����Β����Ώۃ|�C���g�Ƃ��Ď擾����
		// �u���O�����v�Ƀ}�b�`�����ꍇ�͒����Ώۃ|�C���g�͐������Ȃ�
		// �f�ރ|�C���g�����ɒ����Ώۃ|�C���g�ݒ�ς݂̏ꍇ���������Ȃ�
		if (m_isSuvey && !excepted && !selected)
		{
			// �������townpage�I�u�W�F�N�g�擾
			IDFeatureMap tgtTWNPt;
			if (! GetTgtAtInfillGeo(bldID, ipFilledBldPolygon, m_ipTwnpage, sindy::schema::townpage::kTableName, &tgtTWNPt))
				return false;
			// ����擾�ł��Ȃ���ΏI��
			if (!tgtTWNPt.size())
				return true;
			// �擾�����I�u�W�F�N�g�̂�����\�|�C���g���擾����B
			long headTwnID = -1;
			IFeaturePtr ipHeadTwnFeature;
			if (! ::common_proc::SelectHeadNameFeature(tgtTWNPt, sindy::schema::townpage::kTableName, sindy::schema::townpage::kName, m_twnpageIndeces, &headTwnID, ipHeadTwnFeature))
				return false;
			// townpage�I�u�W�F�N�g���璲���|�C���g���擾
			if( !m_surveyPtMgr.addFromTownpage( headTwnID, bldID, ipHeadTwnFeature ) )
				return false;
		}
	}

	return true;
}

bool BldNameImporter::DBUpdate(bool flushAll)
{
	// DB�X�V���[�h�łȂ��ꍇ�̓R���e�i���N���A���Ă����߂�
	if (!m_isUpdate) {
		m_bldNamePtInfos.clear();
		m_relBldNameInfos.clear();
		m_deleteBldNamePtIDs.clear();
		return true;
	}

	// �S�����f���[�h�ł͂Ȃ��A������̃R���e�i�����̌����ɖ����Ȃ��ꍇ�͂܂����ߍ���
	if (!flushAll && ((m_bldNamePtInfos.size() < FETCHNUM) && (m_relBldNameInfos.size() < FETCHNUM) && (m_deleteBldNamePtIDs.size() < FETCHNUM))) {
		return true;
	}

	// �������̍폜
	if (m_deleteBldNamePtIDs.size() > 0) {
		if (! DeleteExistRecord())
			return false;
	}

	// �������́E�����r�����̕R�Â����R�[�h�o�^
	IFeatureCursorPtr ipBldNameCursor;
	if (FAILED(m_ipBldNamePt->Insert(VARIANT_TRUE, &ipBldNameCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�C���|�[�g�p�t�B�[�`���J�[�\���擾�Ɏ��s���܂���"), ::buildingname_point::kTableName);
		return false;
	}
	_ICursorPtr ipRelBldNameCursor;
	if (FAILED(m_ipRelBldName->Insert(VARIANT_TRUE, &ipRelBldNameCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�C���|�[�g�p�J�[�\���擾�Ɏ��s���܂��܂���"), ::rel_buildingname::kTableName);
		return false;
	}

	for (const auto& bldNamePtInfo : m_bldNamePtInfos) {
		// �t����񃌃R�[�h�̎擾
		BldNameAddInfo bldNameAddInfo;
		if(! GetAddInfo(bldNamePtInfo.second, bldNameAddInfo))
			return false;
		// �����r�����̂ւ̃I�u�W�F�N�g�}��
		long bldNamePtID = -1;
		if (! InsertBldNamePt(bldNamePtInfo.second, bldNameAddInfo, ipBldNameCursor, &bldNamePtID))
			return false;
		// �����r�����̕R�Â��e�[�u���ւ̃I�u�W�F�N�g�}��
		if (! InsertRelBldName(bldNamePtID, bldNamePtInfo.first, ipRelBldNameCursor))
			return false;
	}

	// �t���b�V��
	if (FAILED(ipBldNameCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t���b�V���Ɏ��s���܂���"), ::buildingname_point::kTableName);
		return false;
	}
	if (FAILED(ipRelBldNameCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t���b�V���Ɏ��s���܂���"), ::rel_buildingname::kTableName);
		return false;
	}

	m_bldNamePtInfos.clear();
	m_relBldNameInfos.clear();
	m_deleteBldNamePtIDs.clear();

	return true;
}

bool BldNameImporter::ChkExistBldNamePt(long bldID, bool* existEditPoint)
{
	CString errMsg;
	*existEditPoint = false;

	// Where��쐬
	CString whereClause;
	whereClause.Format(_T("%s = %d"), ::buildingname_point::kBuildingOID, bldID);
	// Cursor�擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByWhereClause(m_ipBldNamePt, ipFeatureCursor, whereClause))
		return false;

	// �t�B�[�`���擾
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// �M���R�[�h��1���`�F�b�N
		CComVariant priority;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_point::kTableName, ::buildingname_point::kPriority, m_bldNamePtIndeces, &priority))
			return false;
		if (priority.lVal == ::buildingname_point::priority_code::kEdit) {
			// �M���R�[�h��1�̏ꍇ�͍폜���Ȃ��B
			*existEditPoint = true;
			continue;
		}
		// objectid�擾
		long oid;
		ipFeature->get_OID(&oid);
		// �폜ID�Ƃ��ēo�^
		m_deleteBldNamePtIDs.insert(oid);
	}
	return true;
}

bool BldNameImporter::GetFilledPolygon(IFeaturePtr ipTgtFeature, IPolygonPtr& ipFilledPolygon)
{
	// OID�擾
	long tgtID;
	ipTgtFeature->get_OID(&tgtID);

	// �e�[�u�����擾
	ITablePtr ipTable;
	ipTgtFeature->get_Table(&ipTable);
	CComBSTR bstrName;
	((IDatasetPtr)ipTable)->get_Name(&bstrName);
	CString strTableName(bstrName);

	// �W�I���g���擾
	IGeometryPtr ipTgtGeometry;
	if (FAILED(ipTgtFeature->get_ShapeCopy(&ipTgtGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	// ExteriorRing���̎擾
	long exteriorCount = -1;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->get_ExteriorRingCount(&exteriorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing���̎擾�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	// ExteriorRing��1�łȂ�������G���[
	if (exteriorCount != 1) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing���������݂��܂�"), strTableName, tgtID);
		return false;
	}
	// ExteriorRing�̎擾
	IRingPtr ipRing;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->QueryExteriorRings(&ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing�̎擾�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	// InteriorRing���̎擾
	long interiorCount = -1;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->get_InteriorRingCount(ipRing, &interiorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("InteriorRing���̎擾�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	// InteriorRing��0�̏ꍇ�A�����߂̕K�v���Ȃ��̂Ō��̌`���Ԃ�
	if (0 == interiorCount) {
		ipFilledPolygon = ipTgtGeometry;
		return true;
	}
	// �����O�����Ă��邩�E�E��肩���`�F�b�N
	VARIANT_BOOL isClosed, isExterior;
	if (FAILED(ipRing->get_IsClosed(&isClosed)) || isClosed == VARIANT_FALSE ||
		FAILED(ipRing->get_IsExterior(&isExterior) || isExterior == VARIANT_FALSE)){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O�`�󂪕s���ł�"), strTableName, tgtID);
		return false;
	}
	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipTgtGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̎擾�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	IPolygonPtr ipNewPolygon(CLSID_Polygon);
	// �����ߍσ|���S���ɋ�ԎQ�Ƃ�t�^
	if (FAILED(ipNewPolygon->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O���|���S���̍ۂ̋�ԎQ�ƕt�^�Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	// �����ߍσ|���S���Ƀ����O��ǉ�
	if (FAILED(IGeometryCollectionPtr(ipNewPolygon)->AddGeometry(ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�|���S���ւ̃����O�̒ǉ��Ɏ��s���܂���"), strTableName, tgtID);
		return false;
	}
	ipFilledPolygon = ipNewPolygon;

	return true;
}

bool BldNameImporter::GetTgtSrcPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtSrcPtFeatures)
{
	CString errMsg;

	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipBldSrcPt, ipFeatureCursor, ipFilledBldPolygon, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid�擾
		long oid = -1;
		ipFeature->get_OID(&oid);
		// �o�T��ʃR�[�h�ƍ̗p��ʃR�[�h���擾
		CComVariant sourceType, adoption;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &sourceType))
			return false;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAdoption, m_bldSrcIndeces, &adoption))
			return false;
		// �o�T��ʃR�[�h�ɕs���̂��̂��������`�F�b�N
		// �i�R�[�h�l�h���C���Ɋ܂܂�Ȃ����A�u102: �⊮�p���LPOI�v�̏ꍇ�͕s���j
		if (m_srcTypeCodedValues.end() == m_srcTypeCodedValues.find(sourceType.lVal) || ::buildingname_src_point::source_type::kCompAnnoPOI == sourceType.lVal)
		{
			errMsg.Format(_T("%s���s���ł� : %ld"), ::buildingname_src_point::kSourceType, sourceType.lVal);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::buildingname_src_point::kTableName, oid);
			return false;
		}
		// �o�T��ʃR�[�h���u0: �Ǝ������v or �̗p��ʃR�[�h���u0: �̗p�v�̏ꍇ�ɁA�}�b�`���O�ΏۂƂ��đf�ރ|�C���g�Q�ɑ}��
		if( buildingname_src_point::source_type::kOwnSurvey == sourceType.lVal || buildingname_src_point::adoption_code::kAdopt == adoption.lVal )
		{
			tgtSrcPtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
		}
	}

	return true;
}

bool BldNameImporter::GetTgtPoiPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtPoiPtFeatures)
{
	CString errMsg;

	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipPoiPtOrg, ipFeatureCursor, ipFilledBldPolygon, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		using namespace sindy::schema::sindyk;

		// objectid�擾
		long oid = -1;
		ipFeature->get_OID(&oid);

		// ���̎擾
		CComVariant varName;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kName, m_poiPtOrgIndeces, &varName))
			return false;
		// �ǂݎ擾
		CComVariant varYomi;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kYomi, m_poiPtOrgIndeces, &varYomi))
			return false;
		// �����R�R�[�h�擾
		CComVariant varCloseC;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kCloseCode, m_poiPtOrgIndeces, &varCloseC))
			return false;

		// ���̂�NULL�A�������͖��̂��u�k���v���ǂ݂��u���~�k���v�̏ꍇ�͏��O����
		CString strName(varName.bstrVal), strYomi(varYomi.bstrVal);
		if (strName.IsEmpty() || (strName == _T("�k��") && strYomi == _T("���~�k��"))) continue;

		// �����R�R�[�h���uNULL�vor�ub�i�x�~���j�v�łȂ��ꍇ�͏��O����
		CString strCloseC(varCloseC.bstrVal);
		if (!strCloseC.IsEmpty() && CA2T(CLOSE_SUSPENDED) != strCloseC) continue;

		tgtPoiPtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
	}

	return true;
}

bool BldNameImporter::GetTgtAtInfillGeo(long bldID, const IGeometryPtr& ipInfillBldGeometry, const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, IDFeatureMap* tgtFeatures)
{
	CString errMsg;

	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(ipFeatureClass, ipFeatureCursor, ipInfillBldGeometry, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		long oid = -1;
		ipFeature->get_OID(&oid);
		// �R���e�i�Ɋi�[
		tgtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
	}
	return true;
}

bool BldNameImporter::DeleteExistRecord()
{
	CString errMsg;
	CString whereClause;

	// ���܂�ʂ������͂��Ȃ̂ŁA�������
	for (const auto& bldNamePtID : m_deleteBldNamePtIDs) {

		// �J�[�\������
		whereClause.Format(_T("%s=%d"), ::buildingname_point::kObjectID, bldNamePtID);
		IFeatureCursorPtr ipBldNamePtCursor;
		if (! ::common_proc::SearchByWhereClause(m_ipBldNamePt, ipBldNamePtCursor, whereClause))
			return false;

		// buildingname_point�̍폜
		IFeaturePtr ipBldNameFeature;
		while (SUCCEEDED(ipBldNamePtCursor->NextFeature(&ipBldNameFeature)) && ipBldNameFeature) {
			// ����ID�擾
			CComVariant bldID;
			if (! ::common_proc::GetValue(ipBldNameFeature, ::buildingname_point::kTableName, ::buildingname_point::kBuildingOID, m_bldNamePtIndeces, &bldID))
				return false;
			if (FAILED(ipBldNameFeature->Delete()) /*|| FAILED(ipBldNameFeature->Store())*/) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�I�u�W�F�N�g�폜�Ɏ��s���܂���"), ::buildingname_point::kTableName, bldNamePtID);
				return false;
			} else {
				// �폜���b�Z�[�W��O�̂��ߏo���Ă���
				errMsg.Format(_T("�����Ώی�����ɃI�u�W�F�N�g�����݂����ׁA�폜���܂����B:bldid = %d"), bldID.lVal);
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, errMsg, ::buildingname_point::kTableName, bldNamePtID);
			}
		}

		// �J�[�\������
		whereClause.Format(_T("%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
		_ICursorPtr ipRelBldNameCursor;
		if (! ::common_proc::SearchByWhereClause(m_ipRelBldName, ipRelBldNameCursor, whereClause))
			return false;

		_IRowPtr ipRelBldNameRow;
		while (SUCCEEDED(ipRelBldNameCursor->NextRow(&ipRelBldNameRow)) && ipRelBldNameRow) {
			if (FAILED(ipRelBldNameRow->Delete()) /*|| FAILED(ipRelBldNameRow->Store())*/) {
				errMsg.Format(_T("�I�u�W�F�N�g�폜�Ɏ��s���܂����F%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::rel_buildingname::kTableName);
				return false;
			} else {
				// �폜���b�Z�[�W��O�̂��ߏo���Ă���
				CString procMsg;
				procMsg.Format(_T("�����Ώی�����Ɍ������̃I�u�W�F�N�g�����݂����ׁA�֘A���R�[�h���폜���܂����F%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, procMsg, ::rel_buildingname::kTableName);
			}
		}
	}

	return true;
}

bool BldNameImporter::InsertBldNamePt(const BldNamePtInfo& bldNamePtInfo, const BldNameAddInfo& bldNameAddInfo, IFeatureCursorPtr& ipBldNamePtCursor,  long* bldNamePtID)
{
	IFeatureBufferPtr ipFeatureBuffer;
	if (FAILED(m_ipBldNamePt->CreateFeatureBuffer(&ipFeatureBuffer))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�C���|�[�g�p�t�B�[�`���o�b�t�@�擾�Ɏ��s���܂���"), ::buildingname_point::kTableName);
		return false;
	}
	// �e�푮���Z�b�g
	// OPERATOR
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kOperator,		m_bldNamePtIndeces, CComVariant(TOOLNAME)))
		return false;
	// PURPOSE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kPurpose,		m_bldNamePtIndeces, CComVariant(0)))
		return false;
	// MODIFYDATE��NULL
	// UPDATETYPE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kUpdateType,		m_bldNamePtIndeces, CComVariant(::buildingname_point::update_type::kImported)))
		return false;
	// PROGMODIFYDATE
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kProgModifyDate,	m_bldNamePtIndeces, CComVariant(GetTimeText(false).c_str())))
		return false;
	// MODIFYPROGNAME
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kModifyProgName,	m_bldNamePtIndeces, CComVariant(TOOLNAME)))
		return false;
	// USERCLAIM_F�͏����l, SOURCE��NULL
	// NAME
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kName,			m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_name)))
		return false;
	// BUILDING_OID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBuildingOID,	m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_bldOID)))
		return false;
	// SOUCETYPE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSourceType,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_srcType)))
		return false;
	// SRC_OID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSrcOID,			m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_srcOID)))
		return false;
	// SURVEYDATE��NULL
	// ANYPLACE_ID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kAnyPlaceID,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_anyPlaceID)))
		return false;
	// PRIORITY_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kPriority,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_priority)))
		return false;
	// MATCH_PER
	// �ő�}�b�`���O����10���̒P�ʂɂ���B(10,20,30,�E�E�E,90,100) -1�̏ꍇ�͂��̂܂�
	long matchPer = ( -1 == bldNamePtInfo.e_matchPer ) ? -1 : ( long( bldNamePtInfo.e_matchPer / 10 ) * 10 );
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kMatchPer,		m_bldNamePtIndeces, CComVariant(matchPer)))
		return false;
	// CONTENTS_CODE
	long contentsCode = bldNamePtInfo.GetContentsCode();
	if (contentsCode >= 0) {
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kContentsCode,	m_bldNamePtIndeces, CComVariant(contentsCode)))
			return false;
	}
	// CONTENTS_SEQ
	long contentsSeq = bldNamePtInfo.GetContentsSeq();
	if (contentsSeq >= 0) {
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kContentsSeq,	m_bldNamePtIndeces, CComVariant(contentsSeq)))
			return false;
	}

	// �t�����i���̏�񂪊i�[����Ă���ꍇ�͗L���Ɣ��f�j
	if (! bldNameAddInfo.e_name.IsEmpty())
	{
		// NAME_SEPARATED
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kNameSeparated,	m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_nameSep)))
			return false;
		// YOMI_SEPARATED
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kYomiSeparated,	m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_yomiSep)))
			return false;
		// BLDGCLASS1_C
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass1,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass1)))
			return false;
		// BLDGCLASS2_C
		if (0 < bldNameAddInfo.e_bldgClass2)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass2,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass2)))
				return false;
		}
		// BLDGCLASS3_C
		if (0 < bldNameAddInfo.e_bldgClass3)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass3,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass3)))
				return false;
		}
		// FLOORS
		if (0 < bldNameAddInfo.e_floors)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kFloors,			m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_floors)))
				return false;
		}
	}
	// �r�����̑Ó��t���O�A��ʊm��R�[�h��NULL�s���̂��ߖ����I�ɐݒ肷��i�ݒ�l���Ȃ���Ώ����l�i0�j���i�[�����j
	if (!::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSuitable, m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_suitable)))
		return false;

	if (!::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kFixClass, m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_fixclass)))
		return false;


	// �`��t�^
	if (FAILED(ipFeatureBuffer->putref_Shape(bldNamePtInfo.e_ipGeometry.m_T))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�^�Ɏ��s���܂���"), ::buildingname_point::kTableName);
		return false;
	}
	// �I�u�W�F�N�g�o�^
	CComVariant id;
	if (FAILED(ipBldNamePtCursor->InsertFeature(ipFeatureBuffer, &id))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�I�u�W�F�N�g�o�^�Ɏ��s���܂���"), ::buildingname_point::kTableName);
		return false;
	}
	*bldNamePtID = id.lVal;
	return true;
}

bool BldNameImporter::InsertRelBldName(long bldNamePtID, long bldSrcPtID, _ICursorPtr& ipRelBldNameCursor)
{
	std::map<long, std::set<long>>::const_iterator itr = m_relBldNameInfos.find(bldSrcPtID);
	if (itr == m_relBldNameInfos.end()) {
		// �֘A�f�ނ����݂��Ȃ��̂ŏI��
		return true;
	}

	for (const auto& srcOID : itr->second) {
		IRowBufferPtr ipRowBuffer;
		if (FAILED(m_ipRelBldName->CreateRowBuffer(&ipRowBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�C���|�[�g�prow�o�b�t�@�擾�Ɏ��s���܂���"), ::rel_buildingname::kTableName);
			return false;
		}
		// BUILDINGNAME_OID
		if (! ::common_proc::SetValue(ipRowBuffer, ::rel_buildingname::kTableName, ::rel_buildingname::kBuildingNameOID,	m_relBldNameIndeces, CComVariant(bldNamePtID)))
			return false;
		// SRC_OID
		if (! ::common_proc::SetValue(ipRowBuffer, ::rel_buildingname::kTableName, ::rel_buildingname::kSrcOID,				m_relBldNameIndeces, CComVariant(srcOID)))
			return false;
		// �I�u�W�F�N�g�o�^
		CComVariant id;
		if (FAILED(ipRelBldNameCursor->InsertRow(ipRowBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�I�u�W�F�N�g�o�^�Ɏ��s���܂���"), ::rel_buildingname::kTableName);
			return false;
		}
	}
	return true;
}

bool BldNameImporter::GetAddInfo(const BldNamePtInfo& bldNamePtInfo, BldNameAddInfo& bldNameAddInfo)
{
	// Where��̐���
	CString strTmp1, strTmp2, strTmp3;
	strTmp1.Format(_T("%s = %d"), ::buildingname_src_addinfo::kSourceType, bldNamePtInfo.e_srcType);
	// ���LPOI�i����J�e�S���E�⊮�J�e�S���j�ȊO�̏ꍇ�̂݁uSOURCENAME�v�������ɉ�����
	switch(bldNamePtInfo.e_srcType)
	{
	case ::buildingname_src_point::source_type::kAnnoPOI:		// ���LPOI�i����J�e�S���j
	case ::buildingname_src_point::source_type::kCompAnnoPOI:	// ���LPOI�i�⊮�J�e�S���j
		break;
	default:	// ���̑�
		strTmp2.Format(_T(" AND %s = '%s'"), ::buildingname_src_addinfo::kSourceName, bldNamePtInfo.e_srcName);
		break;
	}
	strTmp3.Format(_T(" AND %s = '%s' AND %s = '%s'"), ::buildingname_src_addinfo::kSourceID, bldNamePtInfo.e_srcID, ::buildingname_src_addinfo::kName, bldNamePtInfo.e_name);
	CString strWhere = strTmp1 + strTmp2 + strTmp3;

	// ����
	_ICursorPtr ipCurAddInfo;
	if (! ::common_proc::SearchByWhereClause(m_ipBldAddInfo, ipCurAddInfo, strWhere))
		return false;
	_IRowPtr ipAddInfoRow;
	if (SUCCEEDED(ipCurAddInfo->NextRow(&ipAddInfoRow)) && ipAddInfoRow)	// 1����������������Ȃ��͂�
	{
		// NAME�擾
		CComVariant varName;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kName, m_bldAddInfoIndeces, &varName))
			return false;
		bldNameAddInfo.e_name = varName.bstrVal;
		// NAME_SEPARATED�擾
		CComVariant varNameSep;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kNameSeparated, m_bldAddInfoIndeces, &varNameSep))
			return false;
		bldNameAddInfo.e_nameSep = varNameSep.bstrVal;
		// YOMI_SEPARATED�擾
		CComVariant varYomiSep;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kYomiSeparated, m_bldAddInfoIndeces, &varYomiSep))
			return false;
		bldNameAddInfo.e_yomiSep = varYomiSep.bstrVal;
		// BLDGCLASS1_C�擾
		CComVariant varBldgClass1;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass1, m_bldAddInfoIndeces, &varBldgClass1))
			return false;
		bldNameAddInfo.e_bldgClass1 = varBldgClass1.lVal;
		// BLDGCLASS2_C�擾
		CComVariant varBldgClass2;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass2, m_bldAddInfoIndeces, &varBldgClass2))
			return false;
		if (VT_I4 == varBldgClass2.vt)	bldNameAddInfo.e_bldgClass2 = varBldgClass2.lVal;
		// BLDGCLASS3_C�擾
		CComVariant varBldgClass3;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass3, m_bldAddInfoIndeces, &varBldgClass3))
			return false;
		if (VT_I4 == varBldgClass3.vt)	bldNameAddInfo.e_bldgClass3 = varBldgClass3.lVal;
		// FLOORS�擾
		CComVariant varFloors;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kFloors, m_bldAddInfoIndeces, &varFloors))
			return false;
		if (VT_I4 == varFloors.vt)		bldNameAddInfo.e_floors = varFloors.lVal;

		// SUITABLE_F�擾
		CComVariant varSuitable;
		if (!::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kSuitable, m_bldAddInfoIndeces, &varSuitable))
			return false;
		if (VT_I4 == varSuitable.vt)		bldNameAddInfo.e_suitable = varSuitable.lVal;

		// FIXCLASS_C�擾
		CComVariant varFixClass;
		if (!::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kFixClass, m_bldAddInfoIndeces, &varFixClass))
			return false;
		if (VT_I4 == varFixClass.vt)		bldNameAddInfo.e_fixclass = varFixClass.lVal;
	}

	return true;
}
