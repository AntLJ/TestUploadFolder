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
	
	// �C���f�b�N�X�擾
	IFieldsPtr ipFields;
	if (FAILED(m_ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�擾�Ɏ��s���܂���"), ::kTableName);
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
	// ���b�V���Ɋ܂܂��HeightNode�擾
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetFeatureCursor(m_ipFeatureClass, ::kTableName, ipMeshGeometry, esriSpatialRelContains, _T(""), ipFeatureCursor))
		return false;
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid�擾
		long objectid = -1;
		if (FAILED(ipFeature->get_OID(&objectid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("OBEJCTID�̎擾�Ɏ��s���܂���"), ::kTableName);
			return false;
		}
		// �e����擾
		HeightNodeRec heightNodeRec;
		if (! GetRecInfo(ipFeature, heightNodeRec))
			return false;
		// ���o�^
		if (! m_recordInfo.insert(std::pair<long, HeightNodeRec>(objectid, heightNodeRec)).second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���΍������̃R���e�i�}���Ɏ��s���܂���"), ::kTableName, objectid);
			return false;
		}
	}
		return true;
}

bool HeightNodeInfo::GetRecInfo(const IFeaturePtr& ipFeature, HeightNodeRec& heightNodeRec)
{
	// �e�푮���擾
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

	// �`��擾
	if (FAILED(ipFeature->get_ShapeCopy(&heightNodeRec.e_ipGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��̎擾�Ɏ��s���܂���"), ::kTableName);
		return false;
	}
	return true;
}
