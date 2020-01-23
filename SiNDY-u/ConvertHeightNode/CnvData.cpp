#include "stdafx.h"
#include "CnvData.h"
#include "LogSys.h"
#include "common.h"
#include <ArcHelperEx/AheGeometryOp.h>

double CnvData::m_buffer = 0.0;

CnvData::CnvData(void)
{
}


CnvData::~CnvData(void)
{
}

bool CnvData::ConverteHeightNode(const long secondMesh, const HeightNodeInfo& heightNodeInfo, const RoadWalkLinkInfo& roadWalkLinkInfo)
{
	// �G���[���������G���[��S�ă`�F�b�N���邽�߁A�S���R�[�h���[�v���񂷁B
	bool ret = true;
	// ���΍����m�[�h��ŏ��������{
	for (const auto hnInfo :heightNodeInfo) {
		// ���΍����m�[�h�`��ւ̃o�b�t�@�K�p
		IGeometryPtr ipBaseGeo;
		if (m_buffer > 0.0) {
			if (FAILED(((ITopologicalOperatorPtr)hnInfo.second.e_ipGeometry)->Buffer(m_buffer, &ipBaseGeo))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���΍����m�[�h�ւ̃o�b�t�@�K�p�Ɏ��s���܂���"), sindy::schema::height_node::kTableName, hnInfo.first);
				ret = false;
				continue;
			}
		} else {
			ipBaseGeo = hnInfo.second.e_ipGeometry;
		}
		// ���������N�̓���
		long roadWalkID1 = -1, roadWalkID2 = -1;
		RoadWalkLinkRec roadWalkLinkRec1, roadWalkLinkRec2;
		if (! SelectTgtLink(secondMesh, hnInfo.first, hnInfo.second.e_linkid1, hnInfo.second.e_table1, ipBaseGeo,
							heightNodeInfo.GetSpRef(), roadWalkLinkInfo, roadWalkID1, roadWalkLinkRec1)) {
			ret = false;
			continue;
		}
		if (! SelectTgtLink(secondMesh, hnInfo.first, hnInfo.second.e_linkid2, hnInfo.second.e_table2, ipBaseGeo,
							heightNodeInfo.GetSpRef(), roadWalkLinkInfo, roadWalkID2, roadWalkLinkRec2)) {
			ret = false;
			continue;
		}
		// �ϊ����Shape���ݒ�
		OutSHPRec outSHPRec(hnInfo.second.e_ipGeometry, roadWalkID1, roadWalkLinkRec1.e_divid, hnInfo.second.e_height1,
							roadWalkID2, roadWalkLinkRec2.e_divid, hnInfo.second.e_height2);
		if (! m_recordInfo.insert(std::pair<long, OutSHPRec>(hnInfo.first, outSHPRec)).second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ϊ��ヌ�R�[�h���}���Ɏ��s���܂���"), sindy::schema::height_node::kTableName, hnInfo.first);
			ret = false;
			continue;
		}
	}
	return ret;
}

bool CnvData::SelectTgtLink(const long secondMesh,
							const long heightNodeID, 
							const long linkID,
							const long tableCode,
							const IGeometryPtr& ipHeightNodeGeo, 
							const ISpatialReferencePtr& ipSpRef,
							const RoadWalkLinkInfo& roadWalkLinkInfo,
							long& roadWalkID,
							RoadWalkLinkRec& roadWalkLinkRec)
{
	
	// ���΍����m�[�h�Ɋ֘A���铹�H�����N�Q�擾
	// tableCode�����s�҂̏ꍇ�́A1���v���X���Č���
	switch (tableCode) {
		case sindy::schema::height_node::link_table::kWalkLink:
			roadWalkID = linkID + HUNDRED_MILLION;
			break;
		case sindy::schema::height_node::link_table::kRoadLink:
			roadWalkID = linkID;
			break;
		default:
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����N�e�[�u���̒l���s���ł�"), sindy::schema::height_node::kTableName, heightNodeID);
			return false;
	}
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator candLinkItr = roadWalkLinkInfo.find(roadWalkID);
	if (roadWalkLinkInfo.end() == candLinkItr) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�֘A�t�����H�E���s�҃����N�����݂��܂���"), sindy::schema::height_node::kTableName, heightNodeID);
		return false;
	}
	// �擾���������N�Q�̒�����A���΍����m�[�h�ƌ�����Ă�����s�ҁE���H�����N���擾����B
	bool find = false;
	std::set<RoadWalkLinkRec>::const_iterator linkItr = candLinkItr->second.begin();
	for (; linkItr != candLinkItr->second.end(); ++linkItr) {
		// Shape�`��ɋ�ԎQ�ƕt�^
		IGeometryPtr ipLinkGeo = linkItr->e_ipGeoemtry;
		if (FAILED(ipLinkGeo->putref_SpatialReference(ipSpRef))) {
			// �G���[�o�͗p�Ƀt�@�C�����쐬
			CString shpName;
			shpName.Format(_T("%d_%s"), secondMesh, shp05::schema::road::road_walk_link_a::kSuffix);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���H�E���s�҃����N�ւ̋�ԎQ�ƕt�^�Ɏ��s���܂���"), shpName, roadWalkID);
			return false;
		}
		// �����N1��HeightNode�Ɋ֌W�����邩�`�F�b�N(disjoint�ł�touch�ł��Ȃ�)
		// ��touch�̏ꍇ�́A�����N�̎n�I�_�Őڂ��Ă��邽�߃_��
		if (AheIsDisjoint(ipHeightNodeGeo, ipLinkGeo) || AheIsTouch(ipHeightNodeGeo, ipLinkGeo))
			continue;
		roadWalkLinkRec = *linkItr;
		if (find) {
			// ������A�֘A���郊���N�����邱�Ƃ͖����̂ŁA�����ɗ�����G���[
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���������N�̌�_�ɑ��΍����m�[�h�����݂��܂�"), sindy::schema::height_node::kTableName, heightNodeID);
			return false;
		}
		// ��L�`�F�b�N�̂��߁A�֘A�����N���������Ă������͉񂷁B
		find = true;
	}
	if (! find) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err,  _T("���΍����m�[�h�`��ƈ�v�����_��������܂���"), sindy::schema::height_node::kTableName, heightNodeID);
		return false;
	}
	return true;
}

