#include "stdafx.h"
#include "RoadWalkLinkInfo.h"
#include "LogSys.h"
#include "common.h"

using namespace shp05::schema::road::road_walk_link_a;

CString	RoadWalkLinkInfo::m_shpRootPath;
long RoadWalkLinkInfo::m_objectidIndex = -1;
long RoadWalkLinkInfo::m_dividIndex = -1;

RoadWalkLinkInfo::RoadWalkLinkInfo(void)
{
}


RoadWalkLinkInfo::~RoadWalkLinkInfo(void)
{
}

bool RoadWalkLinkInfo::SetRoadWalkLinkInfo(const long secondMesh)
{
	// Shape�i�[�f�B���N�g���p�X�쐬
	long firstMesh = secondMesh/100;
	CString tgtDir;
	tgtDir.Format(_T("%s\\%d\\%d"), m_shpRootPath, firstMesh, secondMesh);
	// ���C�����쐬
	CString shpName;
	shpName.Format(_T("%d_%s"), secondMesh, ::kSuffix);
	
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr ipWorkspace;
	if (! common_proc::GetWorkspace(tgtDir, ipWorkspace))
		return false;
	
	// �t�B�[�`���N���X�擾
	IFeatureClassPtr ipFeatureClass;
	if (! common_proc::GetFeatureClass(ipWorkspace, shpName, _T(""), ipFeatureClass))
		return false;
	
	// index�擾
	// ���eShape�̃t�B�[���h�\������v���Ă���O��B��x�擾�������̂��g���܂킷
	if (-1 == m_objectidIndex || -1 == m_dividIndex) {
		// �C���f�b�N�X�擾
		if (! SetIndex(ipFeatureClass, shpName))
			return false;
	}
	
	// �J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(ipFeatureClass, shpName, NULL, esriSpatialRelUndefined, _T(""), ipFeatureCursor))
		return false;
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature){
		long fid = -1;
		if (FAILED(ipFeature->get_OID(&fid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("FID�̎擾�Ɏ��s���܂���"), shpName);
			return false;
		}
		// ���H�E���s�҃����N�擾
		long objectid = -1;
		RoadWalkLinkRec roadWalkLinkRec;
		if (! GetRecInfo(ipFeature, shpName, objectid, roadWalkLinkRec))
			return false;
		// ���o�^
		std::map<long, std::set<RoadWalkLinkRec>>::iterator itr = m_recordInfo.find(objectid);
		if (m_recordInfo.end() != itr) {
			if (itr->second.insert(roadWalkLinkRec).second)
				continue;
		} else {
			std::set<RoadWalkLinkRec> temp;
			if (temp.insert(roadWalkLinkRec).second) {
				if (m_recordInfo.insert(std::pair<long, std::set<RoadWalkLinkRec>>(objectid, temp)).second)
					continue;
			}
		}
		// �������������Ă���΂����ɂ͂��Ȃ�
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���H�E���s�҃����N���̃R���e�i�}���Ɏ��s���܂���"), shpName, fid);
		return false;
	}

	return true;
}

bool RoadWalkLinkInfo::SetIndex(const IFeatureClassPtr& ipFeatureClass, LPCTSTR shpName)
{
	IFieldsPtr ipFields;
	if (FAILED(ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�擾�Ɏ��s���܂���"), shpName);
		return false;
	}
	if (FAILED(ipFields->FindField(CComBSTR(shp05::schema::kObjectId), &m_objectidIndex)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId), &m_dividIndex)))
		return false;
	return true;
}

bool RoadWalkLinkInfo::GetRecInfo(const IFeaturePtr& ipFeature, LPCTSTR shpName, long& objectid, RoadWalkLinkRec& roadWalkLinkRec)
{
	// �e�푮���擾
	CComVariant tempObjectid;
	if (! common_proc::GetValue(ipFeature, shpName, shp05::schema::kObjectId, m_objectidIndex, tempObjectid))
		return false;
	objectid = tempObjectid.lVal;
	CComVariant divid;
	if (! common_proc::GetValue(ipFeature, shpName, ::kDivId, m_dividIndex, divid))
		return false;
	roadWalkLinkRec.e_divid = divid.lVal;
	// �`��擾
	if (FAILED(ipFeature->get_ShapeCopy(&roadWalkLinkRec.e_ipGeoemtry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), shpName);
		return false;
	}
	return true;
}