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
	// Shape格納ディレクトリパス作成
	long firstMesh = secondMesh/100;
	CString tgtDir;
	tgtDir.Format(_T("%s\\%d\\%d"), m_shpRootPath, firstMesh, secondMesh);
	// レイヤ名作成
	CString shpName;
	shpName.Format(_T("%d_%s"), secondMesh, ::kSuffix);
	
	// ワークスペース取得
	IWorkspacePtr ipWorkspace;
	if (! common_proc::GetWorkspace(tgtDir, ipWorkspace))
		return false;
	
	// フィーチャクラス取得
	IFeatureClassPtr ipFeatureClass;
	if (! common_proc::GetFeatureClass(ipWorkspace, shpName, _T(""), ipFeatureClass))
		return false;
	
	// index取得
	// →各Shapeのフィールド構成が一致している前提。一度取得したものを使いまわす
	if (-1 == m_objectidIndex || -1 == m_dividIndex) {
		// インデックス取得
		if (! SetIndex(ipFeatureClass, shpName))
			return false;
	}
	
	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(ipFeatureClass, shpName, NULL, esriSpatialRelUndefined, _T(""), ipFeatureCursor))
		return false;
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature){
		long fid = -1;
		if (FAILED(ipFeature->get_OID(&fid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("FIDの取得に失敗しました"), shpName);
			return false;
		}
		// 道路・歩行者リンク取得
		long objectid = -1;
		RoadWalkLinkRec roadWalkLinkRec;
		if (! GetRecInfo(ipFeature, shpName, objectid, roadWalkLinkRec))
			return false;
		// 情報登録
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
		// 処理が成功していればここにはこない
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("道路・歩行者リンク情報のコンテナ挿入に失敗しました"), shpName, fid);
		return false;
	}

	return true;
}

bool RoadWalkLinkInfo::SetIndex(const IFeatureClassPtr& ipFeatureClass, LPCTSTR shpName)
{
	IFieldsPtr ipFields;
	if (FAILED(ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド取得に失敗しました"), shpName);
		return false;
	}
	if (FAILED(ipFields->FindField(CComBSTR(shp05::schema::kObjectId), &m_objectidIndex)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId), &m_dividIndex)))
		return false;
	return true;
}

bool RoadWalkLinkInfo::GetRecInfo(const IFeaturePtr& ipFeature, LPCTSTR shpName, long& objectid, RoadWalkLinkRec& roadWalkLinkRec)
{
	// 各種属性取得
	CComVariant tempObjectid;
	if (! common_proc::GetValue(ipFeature, shpName, shp05::schema::kObjectId, m_objectidIndex, tempObjectid))
		return false;
	objectid = tempObjectid.lVal;
	CComVariant divid;
	if (! common_proc::GetValue(ipFeature, shpName, ::kDivId, m_dividIndex, divid))
		return false;
	roadWalkLinkRec.e_divid = divid.lVal;
	// 形状取得
	if (FAILED(ipFeature->get_ShapeCopy(&roadWalkLinkRec.e_ipGeoemtry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), shpName);
		return false;
	}
	return true;
}