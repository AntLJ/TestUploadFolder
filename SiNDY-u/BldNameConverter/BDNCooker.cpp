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
#include "BDNCooker.h"
#include "common.h"

BDNCooker::BDNCooker(const FieldIndexMap* bldNamePtIndeces)
	: m_bldNamePtIndeces(bldNamePtIndeces)
{
}

BDNCooker::~BDNCooker(void)
{
}

bool BDNCooker::cookBDN(
	const IGeometryPtr& ipMeshGeometry,
	const IDFeatureMap& bldFeatures,
	const IDFeatureMap& bldNamePtFeatures,
	IDFeatureMap* bldNamePtIDs,
	IDRecordMap* cnvInfo,
	bool* existWrongData
	)
{
	*existWrongData = true;
	bool retval = true;
	// ���b�V�����O����̂��߁A���b�V����`��min, max�擾
	LONLAT meshMin, meshMax;
	if (! common_proc::GetMinMax(ipMeshGeometry, &meshMin, &meshMax))
		return false;
	// ������ŏ���
	for (const auto& bldInfo :bldFeatures)
	{
		// �`��`�F�b�N�p�Ɍ����ߌ`��쐬
		IGeometryPtr ipInfillBldGeometry;
		if (! makeInfillBldGeometry(bldInfo, ipInfillBldGeometry))
			return false;
		// ����ID���猚�����̃|�C���g����
		bool relExist = false;
		bool posChk = true;
		if (! getRelatedBldNamePt(bldInfo.first, ipInfillBldGeometry, bldNamePtFeatures, meshMin, meshMax, bldNamePtIDs, cnvInfo, &relExist, &posChk))
			return false;
		// �������̃|�C���g��������ɑ��݂��Ȃ������ꍇ�Aretval=false�Ƃ���B(�����͑����������G���[�����݂������Ƃ�ێ�����������)
		if (! posChk)
			*existWrongData = false;
	}
	// �������̃|�C���g���Ŏg�p����Ă��Ȃ����̂�����悤�ł������烁�b�Z�[�W���o�͂���B
	for (const auto bldNamePtID : *bldNamePtIDs)
	{
		// �ϊ��ΏۊO�|�C���g�ł���Ώo�͂��Ȃ�(�ϊ��Ώۂ̃��b�V���ŏo�͂����̂�)
		IGeometryPtr ipGeometry;
		if (FAILED(bldNamePtID.second->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first);
			return false;
		}
		bool isCnv = false;
		if (! chkPointCnv(sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first, ipGeometry, meshMin, meshMax, &isCnv))
			return false;
		if (isCnv) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::notice, _T("�R�Â����������݂��܂���ł����B�{�I�u�W�F�N�g��ϊ��ΏۊO�Ƃ��܂��B"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first);
			*existWrongData = false;
		}
	}
	return retval;
}

bool BDNCooker::makeInfillBldGeometry(const IDFeatureMap::value_type& bldInfo, IGeometryPtr& ipInfillBldGeometry)
{
	// �����`��擾
	IGeometryPtr ipBldGeometry;
	if (FAILED(bldInfo.second->get_ShapeCopy(&ipBldGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// �������`����l�����AExteriorRing���擾����B
	long exteriorCount = -1;
	if (FAILED(((IPolygonPtr)ipBldGeometry)->get_ExteriorRingCount(&exteriorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing���̎擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// ExteriorRing��1�łȂ�������G���[
	if (exteriorCount != 1) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing���������݂��܂�"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	IRingPtr ipRing;
	if (FAILED(((IPolygonPtr)ipBldGeometry)->QueryExteriorRings(&ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing�̎擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// �����`��̋�ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipBldGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̎擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// Ring2Polygon
	IPolygonPtr ipPolygon;
	if (! ring2Polygon(bldInfo.first, ipRing, ipSpRef, ipPolygon))
		return false;
	ipInfillBldGeometry = (IGeometryPtr)ipPolygon;

	return true;

}

bool BDNCooker::getRelatedBldNamePt(
	long bldID,
	const IGeometryPtr& ipInfillBldGeometry,
	const IDFeatureMap& bldNamePtFeatures,
	const LONLAT& meshMin,
	const LONLAT& meshMax,
	IDFeatureMap* bldNamePtIDs, 
	IDRecordMap* cnvInfo,
	bool* relExist,
	bool* posChk
	)
{
	*relExist = false;
	*posChk = true;
	// ������OBJECTID�Ō����r�����̌���
	IDFeatureMap::const_iterator itr = bldNamePtFeatures.find(bldID);
	if (itr == bldNamePtFeatures.end()) 
		return true;			// �֘A�t�������r�����̂͑��݂��Ȃ��̂ŏI��
	// �R�Â��������r�����̃|�C���g��objectid�擾
	long bldNamePtID = -1;
	if (FAILED(itr->second->get_OID(&bldNamePtID))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�I�u�W�F�N�gID�擾�Ɏ��s���܂���"), sindy::schema::sj::buildingname_point::kTableName);
		return false;
	}
	// �R�Â��������r�����̃|�C���gID�������Ŏ擾���������Ώ�ID�Q����폜����B
	bldNamePtIDs->erase(bldNamePtID);

	// �r�����̃|�C���g�̃W�I���g���擾
	IGeometryPtr ipBldNamePtGeometry;
	if (FAILED(itr->second->get_ShapeCopy(&ipBldNamePtGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID);
		return false;
	}
	// �|�C���g����ƂȂ錚�����ɑ��݂��邩�`�F�b�N
	VARIANT_BOOL disjointChk = VARIANT_FALSE;
	IRelationalOperatorPtr ipRel(ipInfillBldGeometry);
	if (FAILED(ipRel->Disjoint(ipBldNamePtGeometry, &disjointChk))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����r�����Ƃ�DISJOINT�`�F�b�N�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	if (disjointChk == VARIANT_TRUE) {
		CString noticeMsg;
		noticeMsg.Format(_T("�R�Â��Ă���ƌ`��Ɍ`�󂪑��݂��܂���B�{�I�u�W�F�N�g��ϊ��ΏۊO�Ƃ��܂��B:����ID=%d"), itr->first); 
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::notice, noticeMsg, sindy::schema::sj::buildingname_point::kTableName, bldNamePtID);
		*posChk = false;
		return true;
	}
	// �����܂ŗ����猚����ID�E�ʒu���ɕR�Â��������̃|�C���g�����݂��邱�ƂƂȂ�̂�relExist��true�Ƃ���B
	*relExist = true;
	
	// �|�C���g���ϊ��Ώۂ��`�F�b�N(���b�V�����ɑ��݂��邩�`�F�b�N)
	bool isCnv = false;
	if (! chkPointCnv(sindy::schema::sj::buildingname_point::kTableName, bldNamePtID, ipBldNamePtGeometry, meshMin, meshMax, &isCnv))
		return false;

	// �ϊ��Ώۂ̏ꍇ�́A����ID�E�������̂�o�^
	if (isCnv) {
		// �������̎擾
		CComVariant name;
		if (! common_proc::GetValue(itr->second, sindy::schema::sj::buildingname_point::kTableName, sindy::schema::sj::buildingname_point::kName, *m_bldNamePtIndeces, &name))
			return false;
		// ���̂���̏ꍇ�������āA�R���e�i�}��
		CString bldName = name.bstrVal;
		if (! bldName.IsEmpty()) {
			RECORDINFO recordInfo(bldID, bldName, ipBldNamePtGeometry);
			cnvInfo->insert(IDRecordMap::value_type(bldID, recordInfo));
		}
	}
	return true;
}

bool BDNCooker::ring2Polygon(long bldID, const IRingPtr& ipRing, const ISpatialReferencePtr& ipSpRef, IPolygonPtr& ipPolygon)
{
	// �����O�����Ă��邩,�E��肩�`�F�b�N
	VARIANT_BOOL isClosed, isExterior;
	if (FAILED(ipRing->get_IsClosed(&isClosed)) || isClosed == VARIANT_FALSE ||
		FAILED(ipRing->get_IsExterior(&isExterior) || isExterior == VARIANT_FALSE)){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O�`�󂪕s���ł�"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	IPointCollectionPtr ipPointCol(ipRing);
	long pointCount;
	if (FAILED(ipPointCol->get_PointCount(&pointCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O�̃|�C���g���擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	WKSPoint* wksPoints = new WKSPoint[pointCount];
	if (FAILED(ipPointCol->QueryWKSPoints(0, pointCount, wksPoints))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O���|�C���g�Q���o�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	IPointCollectionPtr ipNewPointCol(CLSID_Polygon);
	if (FAILED(((IGeometryPtr)ipNewPointCol)->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O���|���S���̍ۂ̋�ԎQ�ƕt�^�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	if (FAILED(ipNewPointCol->AddWKSPoints(pointCount, wksPoints))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����O���|���S���ւ̕ϊ��Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	delete [] wksPoints;
	IPolygonPtr ipTemp(ipNewPointCol);
	ipPolygon = ipTemp;
	return true;
}

bool BDNCooker::chkPointCnv(LPCTSTR tableName, long id, const IGeometryPtr& ipPtGeometry, const LONLAT& meshMin, const LONLAT& meshMax, bool* isCnv)
{
	// �|�C���g�t�B�[�`���̍��W�擾
	IPointPtr ipPoint(ipPtGeometry);
	LONLAT ptLonLat;
	if (FAILED(ipPoint->get_X(&ptLonLat.e_lon)) || FAILED(ipPoint->get_Y(&ptLonLat.e_lat))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�|�C���g���W�̎擾�Ɏ��s���܂���"), tableName, id);
		return false;
	}
	// �|�C���g���W�����b�V����`�Ɋ܂܂�邩�`�F�b�N
	if ((ptLonLat.e_lon < meshMin.e_lon) || (ptLonLat.e_lat < meshMin.e_lat) ||
		(ptLonLat.e_lon > meshMax.e_lon) || (ptLonLat.e_lat > meshMax.e_lat)) {
		// ���b�V���̊O���ɂ���P�[�X�͕ϊ��ΏۊO
		*isCnv=false;
	} else if ((ptLonLat.e_lon > meshMin.e_lon) && (ptLonLat.e_lat > meshMin.e_lat) &&
		       (ptLonLat.e_lon < meshMax.e_lon) && (ptLonLat.e_lat < meshMax.e_lat)) {
		// ���b�V���̓����Ȃ̂ŕϊ��Ώ�
		*isCnv=true;
	} else if ((ptLonLat.e_lon == meshMin.e_lon) || (ptLonLat.e_lat == meshMin.e_lat)) {
		// ���b�V�����E��̏ꍇ�A�ŏ����b�V���ŕϊ�����B����āA���ӂƒ�ӂ̏ꍇ�͕ϊ����Ȃ�
		*isCnv=false;
	} else if ((ptLonLat.e_lon == meshMax.e_lon) || (ptLonLat.e_lat == meshMax.e_lat)) {
		// ���b�V�����E��̏ꍇ�A�ŏ����b�V���ŕϊ�����B����āA�E�ӂƏ�ӂ̏ꍇ�͕ϊ�����B(���ӂƒ�ӂƂ̐ڑ����͏���)
		*isCnv=true;
	} else {
		// �����܂ŗ���ꍇ�A�v���O��������������
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("�v���O�������s���ł��B�J���҂ɂ��₢���킹���������B"), tableName, id);
		return false;
	}
	return true;
}
