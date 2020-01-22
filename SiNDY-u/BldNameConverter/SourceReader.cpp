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
#include "SourceReader.h"

SourceReader::SourceReader(
	const IFeatureClassPtr& ipBldNamePt,
	const IFeatureClassPtr& ipBld,
	const FieldIndexMap* bldNamePtIndeces,
	const FieldIndexMap* bldIndeces,
	const std::set<long>* bldExcList
	)
	: m_ipBldNamePt(ipBldNamePt), m_ipBld(ipBld), m_bldNamePtIndeces(bldNamePtIndeces), m_bldIndeces(bldIndeces), m_bldExcList(bldExcList)
{
}

SourceReader::~SourceReader(void)
{
}

bool SourceReader::readSource(const IGeometryPtr& ipMeshGeometry, IDFeatureMap* bldFeatures, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData)
{
	// ���b�V����`�ɃC���^�[�Z�N�g���錚���ƃ��b�V������͂ݏo�������̃G�N�X�e���g�擾
	IGeometryPtr ipExtentGeometry;
	if (! getTgtBldAndExtentGeometry(ipMeshGeometry, bldFeatures, ipExtentGeometry))
		return false;
	// �G�N�X�e���g�ɃC���^�[�Z�N�g�������̎擾
	if (! getTgtBldNamePt(ipExtentGeometry, bldNamePtFeatures, bldNamePtOIDs, existWrongData))
		return false;
	return true;
}

bool SourceReader::getTgtBldAndExtentGeometry(const IGeometryPtr ipMeshGeometry, IDFeatureMap* bldFeatures, IGeometryPtr& ipExtentGemetry)
{
	CString errMsg;
	// �G�N�X�e���g��`�p�Ƀ��b�V����`��min, max�擾
	LONLAT minLonLat, maxLonLat;
	if (! common_proc::GetMinMax(ipMeshGeometry, &minLonLat, &maxLonLat))
		return false;
	// overlap�����p��IRelationalOperator������
	IRelationalOperatorPtr ipRel(ipMeshGeometry);
	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetRelFeatureCursor(sindy::schema::building::kTableName, m_ipBld, ipMeshGeometry, esriSpatialRelIntersects, ipFeatureCursor))
		return false;
	// �t�B�[�`���擾
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid�擾
		long oid = -1;
		if (FAILED(ipFeature->get_OID(&oid))) {
			errMsg.Format(_T("%s�̎擾�Ɏ��s���܂���"), sindy::schema::building::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::building::kTableName);
			return false;
		}
		// �����Ώۂ��`�F�b�N
		CComVariant bldClass;
		if (! common_proc::GetValue((_IRowPtr)ipFeature, sindy::schema::building::kTableName, sindy::schema::building::kBldClass, *m_bldIndeces, &bldClass))
			return false;
		if (m_bldExcList->find(bldClass.lVal) != m_bldExcList->end()) 
			continue;	// �ϊ��ΏۊO
		// �R���e�i�ɃC���T�[�g
		bldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));

		// ���b�V����`��contain���Ă���ꍇ�A�G�N�X�e���g�p�̍��W�擾
		VARIANT_BOOL containChk = VARIANT_FALSE;
		IGeometryPtr ipBldGeometry;
		if (FAILED(ipFeature->get_ShapeCopy(&ipBldGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (FAILED(ipRel->Contains(ipBldGeometry, &containChk))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���b�V����`�Ƃ�contain����Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (containChk == VARIANT_FALSE) {
			// �G�N�X�e���g���������̍ő�E�ŏ����W�擾
			IGeometryPtr ipBldGeometry;
			if (FAILED(ipFeature->get_ShapeCopy(&ipBldGeometry))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����`��̎擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, oid);
				return false;
			}
			LONLAT bldMinLonLat, bldMaxLonLat;
			if (! common_proc::GetMinMax(ipBldGeometry, &bldMinLonLat, &bldMaxLonLat))
				return false;
			// �ő�(�ŏ�)���W��x,y�������̂��̂��傫��(������)�ꍇ�͍X�V
			if (bldMinLonLat.e_lon < minLonLat.e_lon)
				minLonLat.e_lon = bldMinLonLat.e_lon;
			if (bldMinLonLat.e_lat < minLonLat.e_lat)
				minLonLat.e_lat = bldMinLonLat.e_lat;
			if (bldMaxLonLat.e_lon > maxLonLat.e_lon)
				maxLonLat.e_lon = bldMaxLonLat.e_lon;
			if (bldMaxLonLat.e_lat > maxLonLat.e_lat)
				maxLonLat.e_lat = bldMaxLonLat.e_lat;
		}
	}
	// �ŏI�I�ȍő�E�ŏ����W����G���x���[�vGeometry�쐬
	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipMeshGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̎擾�Ɏ��s���܂���"), sindy::schema::citymesh::kTableName);
		return false;
	}
	// �G���x���[�v�̍쐬���s��
	IEnvelopePtr ipEnvelope(CLSID_Envelope);
	if (FAILED(ipEnvelope->putref_SpatialReference(ipSpRef)) ||
		FAILED(ipEnvelope->PutCoords(minLonLat.e_lon, minLonLat.e_lat, maxLonLat.e_lon, maxLonLat.e_lat))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�G�N�X�e���g��`�쐬�Ɏ��s���܂���"), sindy::schema::citymesh::kTableName);
			return false;
	}
	ipExtentGemetry = (IGeometryPtr)ipEnvelope;
	return true;
}

bool SourceReader::getTgtBldNamePt(const IGeometryPtr ipExtentGeometry, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData)
{
	*existWrongData = true;
	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetRelFeatureCursor(sindy::schema::sj::buildingname_point::kTableName, m_ipBldNamePt, ipExtentGeometry, esriSpatialRelIntersects, ipFeatureCursor, _T("")))
		return false;
	// �t�B�[�`���擾
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid�擾
		long oid = -1;
		if (FAILED(ipFeature->get_OID(&oid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�I�u�W�F�N�gID�̎擾�Ɏ��s���܂���"), sindy::schema::sj::buildingname_point::kTableName);
			return false;
		}
		// ����ID�擾
		CComVariant bldID;
		if (! common_proc::GetValue((_IRowPtr)ipFeature, sindy::schema::sj::buildingname_point::kTableName, sindy::schema::sj::buildingname_point::kBuildingOID, *m_bldNamePtIndeces, &bldID))
			return false;
		// �R���e�i�ɃC���T�[�g
		std::pair<IDFeatureMap::iterator, bool> result;
		result = bldNamePtFeatures->insert(IDFeatureMap::value_type(bldID.lVal, ipFeature));
		if (! result.second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�������̂̃R���e�i�}���Ɏ��s���܂����B�{�I�u�W�F�N�g��ϊ��ΏۊO�Ƃ��܂��B"), sindy::schema::sj::buildingname_point::kTableName, oid);
			*existWrongData = false;
			continue;
		}
		bldNamePtOIDs->insert(IDFeatureMap::value_type(oid, ipFeature));
	}
	return true;
}
