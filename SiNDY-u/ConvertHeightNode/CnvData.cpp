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
	// エラー発生時もエラーを全てチェックするため、全レコードループを回す。
	bool ret = true;
	// 相対高さノード基準で処理を実施
	for (const auto hnInfo :heightNodeInfo) {
		// 相対高さノード形状へのバッファ適用
		IGeometryPtr ipBaseGeo;
		if (m_buffer > 0.0) {
			if (FAILED(((ITopologicalOperatorPtr)hnInfo.second.e_ipGeometry)->Buffer(m_buffer, &ipBaseGeo))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("相対高さノードへのバッファ適用に失敗しました"), sindy::schema::height_node::kTableName, hnInfo.first);
				ret = false;
				continue;
			}
		} else {
			ipBaseGeo = hnInfo.second.e_ipGeometry;
		}
		// 交差リンクの特定
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
		// 変換後のShape情報設定
		OutSHPRec outSHPRec(hnInfo.second.e_ipGeometry, roadWalkID1, roadWalkLinkRec1.e_divid, hnInfo.second.e_height1,
							roadWalkID2, roadWalkLinkRec2.e_divid, hnInfo.second.e_height2);
		if (! m_recordInfo.insert(std::pair<long, OutSHPRec>(hnInfo.first, outSHPRec)).second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("変換後レコード情報挿入に失敗しました"), sindy::schema::height_node::kTableName, hnInfo.first);
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
	
	// 相対高さノードに関連する道路リンク群取得
	// tableCodeが歩行者の場合は、1億プラスして検索
	switch (tableCode) {
		case sindy::schema::height_node::link_table::kWalkLink:
			roadWalkID = linkID + HUNDRED_MILLION;
			break;
		case sindy::schema::height_node::link_table::kRoadLink:
			roadWalkID = linkID;
			break;
		default:
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リンクテーブルの値が不正です"), sindy::schema::height_node::kTableName, heightNodeID);
			return false;
	}
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator candLinkItr = roadWalkLinkInfo.find(roadWalkID);
	if (roadWalkLinkInfo.end() == candLinkItr) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("関連付く道路・歩行者リンクが存在しません"), sindy::schema::height_node::kTableName, heightNodeID);
		return false;
	}
	// 取得したリンク群の中から、相対高さノードと交わっている歩行者・道路リンクを取得する。
	bool find = false;
	std::set<RoadWalkLinkRec>::const_iterator linkItr = candLinkItr->second.begin();
	for (; linkItr != candLinkItr->second.end(); ++linkItr) {
		// Shape形状に空間参照付与
		IGeometryPtr ipLinkGeo = linkItr->e_ipGeoemtry;
		if (FAILED(ipLinkGeo->putref_SpatialReference(ipSpRef))) {
			// エラー出力用にファイル名作成
			CString shpName;
			shpName.Format(_T("%d_%s"), secondMesh, shp05::schema::road::road_walk_link_a::kSuffix);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("道路・歩行者リンクへの空間参照付与に失敗しました"), shpName, roadWalkID);
			return false;
		}
		// リンク1とHeightNodeに関係があるかチェック(disjointでもtouchでもない)
		// →touchの場合は、リンクの始終点で接しているためダメ
		if (AheIsDisjoint(ipHeightNodeGeo, ipLinkGeo) || AheIsTouch(ipHeightNodeGeo, ipLinkGeo))
			continue;
		roadWalkLinkRec = *linkItr;
		if (find) {
			// 複数回、関連するリンクがくることは無いので、ここに来たらエラー
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("複数リンクの交点に相対高さノードが存在します"), sindy::schema::height_node::kTableName, heightNodeID);
			return false;
		}
		// 上記チェックのため、関連リンクが見つかっても処理は回す。
		find = true;
	}
	if (! find) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err,  _T("相対高さノード形状と一致する交点が見つかりません"), sindy::schema::height_node::kTableName, heightNodeID);
		return false;
	}
	return true;
}

