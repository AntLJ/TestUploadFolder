#include "stdafx.h"
#include "HeightNodeInfo.h"
#include "LogSys.h"
#include "common.h"

using namespace sindy::schema::height_node;

long					HeightNodeInfo::m_linkid1Index = 0;
long					HeightNodeInfo::m_height1Index = 0;
long					HeightNodeInfo::m_linkTable1Index = 0;
long					HeightNodeInfo::m_linkid2Index = 0;
long					HeightNodeInfo::m_height2Index = 0;
long					HeightNodeInfo::m_linkTable2Index = 0;

HeightNodeInfo::HeightNodeInfo(const IFeatureClassPtr& ipFeatureClass, const ISpatialReferencePtr& ipSpRef)
	:m_ipFeatureClass(ipFeatureClass), m_ipSpRef(ipSpRef)
{
}


HeightNodeInfo::~HeightNodeInfo(void)
{
}

bool HeightNodeInfo::Init()
{
	
	// インデックス取得
	IFieldsPtr ipFields;
	if (FAILED(m_ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド取得に失敗しました"), ::kTableName);
		return false;
	}
	if (FAILED(ipFields->FindField(CComBSTR(::kLinkID1), &m_linkid1Index)) || 
		FAILED(ipFields->FindField(CComBSTR(::kLinkID1Height), &m_height1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkID1Table), &m_linkTable1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkID2), &m_linkid2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkID2Height), &m_height2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkID2Table), &m_linkTable2Index)))
		return false;
	return true;
}

bool HeightNodeInfo::SetHeightNodeInfo(const IGeometryPtr ipMeshGeometry)
{
	// メッシュに含まれるHeightNode取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(m_ipFeatureClass, ::kTableName, ipMeshGeometry, esriSpatialRelContains, _T(""), ipFeatureCursor))
		return false;
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid取得
		long objectid = -1;
		if (FAILED(ipFeature->get_OID(&objectid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("OBEJCTIDの取得に失敗しました"), ::kTableName);
			return false;
		}
		// 各種情報取得
		HeightNodeRec heightNodeRec;
		if (! GetRecInfo(ipFeature, heightNodeRec))
			return false;
		// 情報登録
		if (! m_recordInfo.insert(std::pair<long, HeightNodeRec>(objectid, heightNodeRec)).second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("相対高さ情報のコンテナ挿入に失敗しました"), ::kTableName, objectid);
			return false;
		}
	}
		return true;
}

bool HeightNodeInfo::GetRecInfo(const IFeaturePtr& ipFeature, HeightNodeRec& heightNodeRec)
{
	// 各種属性取得
	CComVariant linkid1;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID1, m_linkid1Index, linkid1))
		return false;
	heightNodeRec.e_linkid1 = linkid1.lVal;
	
	CComVariant height1;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID1Height, m_height1Index, height1))
		return false;
	heightNodeRec.e_height1 = height1.lVal;
	
	CComVariant table1;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID1Table, m_linkTable1Index, table1))
		return false;
	heightNodeRec.e_table1 = table1.lVal;

	CComVariant linkid2;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID2, m_linkid2Index, linkid2))
		return false;
	heightNodeRec.e_linkid2 = linkid2.lVal;

	CComVariant height2;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID2Height, m_height2Index, height2))
		return false;
	heightNodeRec.e_height2 = height2.lVal;

	CComVariant table2;
	if (! common_proc::GetValue(ipFeature, ::kTableName, ::kLinkID2Table, m_linkTable2Index, table2))
		return false;
	heightNodeRec.e_table2 = table2.lVal;

	// 形状取得
	if (FAILED(ipFeature->get_ShapeCopy(&heightNodeRec.e_ipGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), ::kTableName);
		return false;
	}
	return true;
}
