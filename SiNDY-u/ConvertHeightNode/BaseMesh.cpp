#include "stdafx.h"
#include "BaseMesh.h"
#include "LogSys.h"
#include "common.h"

using namespace sindy::schema::basemesh;


BaseMesh::BaseMesh(const IFeatureClassPtr& ipFeatureClass)
	:m_ipFeatureClass(ipFeatureClass)
{
}


BaseMesh::~BaseMesh(void)
{
}


bool BaseMesh::SetGeometry(const long secondMesh)
{
	CString errMsg;
	// �t�B�[�`���擾
	CString whereClause;
	whereClause.Format(_T("%s = %d"), ::kMeshCode, secondMesh);
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(m_ipFeatureClass, ::kTableName, NULL, esriSpatialRelUndefined, whereClause, ipFeatureCursor))
		return false;
	long count = 0;
	IFeaturePtr ipFeature;
	if ((ipFeatureCursor->NextFeature(&ipFeature)) != S_OK || !ipFeature) {
		errMsg.Format(_T("�Ώۃ��b�V���`�󂪑��݂��܂���:%d"), secondMesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kTableName);
		return false;
	}
	// �`��擾
	if (FAILED(ipFeature->get_ShapeCopy(&m_ipGeometry))) {
		errMsg.Format(_T("���b�V���`��̎擾�Ɏ��s���܂����F%d"), secondMesh);
		return false;
	}
	return true;
}