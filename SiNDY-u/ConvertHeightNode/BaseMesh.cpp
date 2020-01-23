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
	// フィーチャ取得
	CString whereClause;
	whereClause.Format(_T("%s = %d"), ::kMeshCode, secondMesh);
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(m_ipFeatureClass, ::kTableName, NULL, esriSpatialRelUndefined, whereClause, ipFeatureCursor))
		return false;
	long count = 0;
	IFeaturePtr ipFeature;
	if ((ipFeatureCursor->NextFeature(&ipFeature)) != S_OK || !ipFeature) {
		errMsg.Format(_T("対象メッシュ形状が存在しません:%d"), secondMesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kTableName);
		return false;
	}
	// 形状取得
	if (FAILED(ipFeature->get_ShapeCopy(&m_ipGeometry))) {
		errMsg.Format(_T("メッシュ形状の取得に失敗しました：%d"), secondMesh);
		return false;
	}
	return true;
}