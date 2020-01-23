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

// CheckWNRelPoiPoint.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <sindy/libschema.h>
#include <sindy/workspace.h>
#include <crd_cnv\coord_converter.h>
#include "CheckWNRelPoiPoint.h"
#ifdef _DEBUG_DISTANCE
#include <iomanip>
#endif _DEBUG_DISTANCE

CCheckWNRelPoiPoint::CCheckWNRelPoiPoint(CParam& cParam):m_isError(false) {
	// �p�����[�^���ێ�
	m_CParam = cParam;
}

CCheckWNRelPoiPoint::~CCheckWNRelPoiPoint() {
}

bool CCheckWNRelPoiPoint::execute() {
	// ��������(�e�f�[�^�Ǎ�)
	if (!init()) {
		// �ُ�I��
		return false;
	}

	// ���b�V�����Ƀ`�F�b�N����
	std::vector<std::string>::iterator itrMesh = m_vecMeshList.begin();
	for (; itrMesh != m_vecMeshList.end(); ++itrMesh) {
		// ���s�Ȃ��Ń��b�V���R�[�h�\��
		CLog::WriteRunLogNotNewLine((boost::format("%s\t") % itrMesh->c_str()).str());

		std::string shapeDir = makeShapeDirName(itrMesh->c_str());
		bool bContinue = false;
		// �`�F�b�N(�A,�B,�C)
		if (!checkRelation(shapeDir, itrMesh->c_str(), m_mapRelTbl, m_mapPoiData, bContinue)) {
			if (bContinue) {
				RUN_LOG(DSP_NG);
				m_isError = true;
				continue;
			}
			else {
				// �ُ�I��
				RUN_LOG(DSP_HYPHEN);
				return false;
			}
		}
			
		RUN_LOG(DSP_OK);
	}

	// �Ō�Ɋ֘A�e�[�u���ߑ��`�F�b�N(�D)
	if (!checkRelTbl()) {
		m_isError = true;
	}

	return true;
}

bool CCheckWNRelPoiPoint::init() {
	std::string sLayer, sNid, sPid, sMesh, sErrMsg;		// �G���[���O�\���p

	CRelPoiTbl cRelPOITbl;
	// �֘A�e�[�u���Ǎ����`�F�b�N(�@)
	if (!cRelPOITbl.readRelTbl(m_CParam.getRelTbl(), m_mapRelTbl)) {
		return false;
	}

	std::set<std::string> setCodeList;					// �R�[�h���X�g�R���e�i 
	// �R���e���c�R�[�h���X�g�Ǎ�
	if (!nsCommon::loadList2set(m_CParam.getCodeList().c_str(), setCodeList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}
	
	CPoiFile cPOIFile;
	// POI�t�@�C���Ǎ�
	if (!cPOIFile.readPoiList(m_CParam.getPoiList(), setCodeList, m_mapPoiData)) {
		return false;
	}

	// ���b�V�����X�g�Ǎ�
	if (!nsCommon::loadList2vec(m_CParam.getMeshList().c_str(), m_vecMeshList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}
	// ���b�V���R�[�h�`�F�b�N
	if (!checkMeshCode()) {
		return false;
	}

	// ���E���n�n�p�����[�^�擾
	if (!m_Ccrd_cnv.JGD2000_RP_Init((char*)m_CParam.getParam().c_str())) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("���E���n�n�ϊ��p�p�����[�^�擾���s, file=%s") % m_CParam.getParam().c_str()).str());
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::checkMeshCode() {
	std::string sLayer, sNid, sPid;		// �G���[���O�\���p
	bool bRet = true;

	std::vector<std::string>::iterator itrMesh = m_vecMeshList.begin();
	for (; itrMesh != m_vecMeshList.end(); ++itrMesh) {
		CString csMeshCode(itrMesh->c_str());
		// ���b�V���R�[�h���l�`�F�b�N
		if (csMeshCode != csMeshCode.SpanIncluding(CHECK_INTEGER_STR)) {
			ERR_LOG(sLayer, sNid, sPid, itrMesh->c_str(), "���b�V���R�[�h�s��");
			bRet = false;
			continue;
		}

		int iMeshCode = atoi(itrMesh->c_str());
		// ���b�V���R�[�h�͈̓`�F�b�N
		if (iMeshCode < 100000 || 1000000 <= iMeshCode) {
			ERR_LOG(sLayer, sNid, sPid, itrMesh->c_str(), "���b�V���R�[�h�s��");
			bRet = false;
			continue;
		}
	}

	return bRet;
}

std::string CCheckWNRelPoiPoint::makeShapeDirName(const std::string& strMesh) {
	int iMeshCode = atoi(strMesh.c_str());
	std::stringstream	ssDirPath;

	ssDirPath << m_CParam.getWnDir().c_str() << "\\" << iMeshCode/100 << "\\" << iMeshCode;

	return ssDirPath.str();
}

bool CCheckWNRelPoiPoint::checkRelation(const std::string& sShapeDir,
										const std::string& sMeshcode,
										const MAP_REL_TBL& mapRelTbl,
										const MAP_POI_DATA& mapPoiData,
										bool& bContinue) {
	const std::string sLayerDef, sNidDef, sPidDef;	// �G���[���O�o��(�w�薳��)�p

	CString csLayerName;
	CString csMeshCode(sMeshcode.c_str());
	csLayerName.Format(_T("%s_%s"), (LPCTSTR)csMeshCode, MWN_LAYER);
	std::string sLayer = (boost::format("%s") % CT2CA(csLayerName)).str();	// �G���[���O�o�͗p���C����

	CString csShapeFile;
	CString csShapeDir(sShapeDir.c_str());
	csShapeFile.Format(_T("%s\\%s.dbf"), (LPCTSTR)csShapeDir, (LPCTSTR)csLayerName);

	// ���H�E���s�҃m�[�h�f�[�^���݃`�F�b�N
	if (!PathFileExists(csShapeFile)) {
		ERR_LOG(sLayerDef, sNidDef, sPidDef, sMeshcode, "�Ώۃ��b�V���̓��H�E���s�҃m�[�h�f�[�^�Ȃ�");
		bContinue = true;
		return false;
	}

	// ���H�E���s�҃m�[�h�f�[�^���擾

	IWorkspacePtr ipWorkspace = sindy::create_workspace(csShapeDir);
	if (!ipWorkspace) {
		ERR_LOG(sLayerDef, sNidDef, sPidDef, sMeshcode, (boost::format("���[�N�X�y�[�X�ڑ����s, dir=%s") % CT2CA(csShapeDir) ).str());
		return false;
	}

	IFeatureClassPtr ipFetureclass;
	if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(csLayerName), &ipFetureclass))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, "�I�[�v�����s");
		return false;
	}

	long oidIndex = 0;
	std::string sErrMsg;	// �G���[���O�o�͗p�G���[���b�Z�[�W
	if (!nsCommon::getFieldIndex(ipFetureclass, shp05::schema::kObjectId, oidIndex, sErrMsg)) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, sErrMsg);
		return false;
	}

	CString csWhere;
	csWhere.Format(_T("%s%s"), SQL_NODECLS, CA2CT(m_CParam.getNodeClass().c_str()));
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(csWhere)))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, (boost::format("SQL�ݒ莸�s, where��[%s]") % CT2CA(csWhere)).str());
		return false;
	}

	IFeatureCursorPtr ipFeatureCursor;
	// NODECLASS_C ���ݒ�t�@�C���̒l�ƈ�v���郌�R�[�h������
	if (FAILED(ipFetureclass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, "�J�[�\���擾���s");
		return false;
	}

#ifdef _DEBUG_DISTANCE
	std::cout << "NODE\tPOI\tMESH\tDIST" << std::endl;
#endif _DEBUG_DISTANCE

	std::string sRelTblLayer = nsCommon::getLayerName(m_CParam.getRelTbl());	// �G���[���O�o�͗p�֘A�e�[�u�����C����
	IFeaturePtr ipFeature;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
		CComVariant objectId;
		// NODECLASS_C ���ݒ�t�@�C���̒l�ƈ�v���郌�R�[�h�̃I�u�W�F�N�gID���擾
		if (FAILED(ipFeature->get_Value(oidIndex, &objectId))) {
			ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, (boost::format("�l�擾���s, field=%s")
					% shp05::schema::kObjectId).str());
			return false;
		}
		objectId.ChangeType(VT_I4);
		std::string sNid = std::to_string(objectId.lVal);	// �G���[���O�o�͗pNODE ID

		auto iteRelTbl = m_mapRelTbl.find(objectId.lVal);
		// �擾����objectId���`�F�b�N�Ώۊ֘A�e�[�u���R���e�i�̃L�[�ɂ��邩�`�F�b�N
		if (iteRelTbl == m_mapRelTbl.end()) {
			// �y�G���[�A�z�Ώۃm�[�h���(WKNDCLS_C)��OBJCTID���֘A�e�[�u���ɑ��݂��Ȃ��B
			ERR_LOG(sRelTblLayer, sNid, sPidDef, sMeshcode, "�֘A�e�[�u���ɑΏۃm�[�hID�̑��ݖ���");
			bContinue = true;
			continue;
		}

		// �ΏۃL�[�̗v�f(�֘APOI)���擾
		long lPoiId = iteRelTbl->second;
		std::string sPid = std::to_string(lPoiId);			// �G���[���O�o�͗pPOI ID

		// ���H�E���s�҃m�[�h�R���e�i�ɋl����
		// ���v�f��POI_ID�͓��H�E���s�҃m�[�h�̃f�[�^�ł͂Ȃ����A�̂��̔�r��e�Ղɂ��邽�ߕ֋X������
		m_mapMwnChk.insert(MAP_REL_TBL::value_type(objectId.lVal, lPoiId));
		
		// �֘APOI��POI�f�[�^�R���e�i�ɂ��邩�`�F�b�N
		auto itePoi = m_mapPoiData.find(lPoiId);
		if (itePoi == m_mapPoiData.end()) {
			// �y�G���[�B�z�֘APOI��POI�t�@�C���ɑ��݂��Ȃ��B
			ERR_LOG(sRelTblLayer, sNid, sPid, sMeshcode, "�֘A�e�[�u���ɑ��݂���Ώ�POI ID��POI�t�@�C���ɑ��ݖ���");
			bContinue = true;
			continue;
		}
		// �ΏۃL�[�̗v�f(�o�x�E�ܓx)���擾
		WKSPoint poiPoint = itePoi->second;

		IGeometryPtr ipGeometry;
		// ���H�E���s�҃m�[�h�̌`��擾
		if (FAILED(ipFeature->get_ShapeCopy(&ipGeometry))) {
			ERR_LOG(sLayer, sNid, sPid, sMeshcode, "�`��擾���s");
			return false;
		}
		
		WKSPoint wnPoint;
		if (!getPoint(ipGeometry, wnPoint)) {
			ERR_LOG(sLayer, sNid, sPid, sMeshcode, "���W�擾���s");
			return false;
		}

#ifdef _DEBUG_DISTANCE
		std::cout << "\t" << sNid << "\t" << sPid << "\t" << sMeshcode << "\t" << std::flush;
#endif _DEBUG_DISTANCE
		double dDistance = 0;
		if (!checkDiscance(poiPoint, wnPoint, dDistance)) {
			// �y�G���[�C�z���H�E���s�҃m�[�h�Ɗ֘A�t��POI�̋�����臒l�ȉ����`�F�b�N
			ERR_LOG(sRelTblLayer, sNid, sPid, sMeshcode, (boost::format("�m�[�h��POI�ԋ���臒l����, distance=%.10f") % dDistance).str());
			bContinue = true;
			continue;
		}
	}

	if (bContinue) {
		return false;
	}
	return true;
}

bool CCheckWNRelPoiPoint::checkRelTbl() {
	if (m_mapRelTbl != m_mapMwnChk ) {
		auto iteRelTbl = m_mapRelTbl.begin();
		// �擾����objectId���`�F�b�N�Ώۊ֘A�e�[�u���R���e�i�̃L�[�ɂ��邩�`�F�b�N
		for (; iteRelTbl != m_mapRelTbl.end(); ++iteRelTbl) {
			auto iteMwnChk = m_mapMwnChk.find(iteRelTbl->first);
			if (iteMwnChk == m_mapMwnChk.end()) {
				std::string sLayer = nsCommon::getLayerName(m_CParam.getRelTbl());
				std::string sNid = std::to_string(iteRelTbl->first);
				std::string sPid = std::to_string(iteRelTbl->second);
				std::string sMesh;
				// �y�G���[�D�z�Ώۃ��b�V���̓��H�E���s�҃m�[�h�̑Ώ�OBJECTID�ȊO�̃m�[�hID���֘A�e�[�u���ɑ��݂���B
				ERR_LOG(sLayer, sNid, sPid, sMesh, "�֘A�e�[�u���ɑΏۊO�m�[�hID�̑��ݗL��");
			}
		}
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::getPoint(const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint) {
	esriGeometryType aGeomType;
	if (FAILED(ipGeometry->get_GeometryType(&aGeomType))) {
		return false;
	}

	if (esriGeometryPoint != aGeomType) {
		return false;
	}

	IPointPtr ipPoint(ipGeometry);
	if (FAILED(ipPoint->get_X(&(rWKSPoint.X))) || FAILED(ipPoint->get_Y(&(rWKSPoint.Y)))) {
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::checkDiscance(const WKSPoint& poiPoint, const WKSPoint& wnPoint, double& dDistance) {
	dDistance = m_Ccrd_cnv.GetDist_JGD2000(poiPoint.X, poiPoint.Y, wnPoint.X, wnPoint.Y);
#ifdef _DEBUG_DISTANCE
	std::cout << std::fixed << std::setprecision(10) << dDistance << std::endl;
#endif _DEBUG_DISTANCE
	if (dDistance > std::stod(m_CParam.getDistance().c_str())) {
		return false;
	}

	return true;
}
