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
#include "IllCooker.h"
#include "LogSys.h"

#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <sindy/schema/egg.h>


using namespace sindy::schema;

const LPCTSTR CommonFName = _T("road_node");
const long MaxNodeNum = 20;

IllCooker::IllCooker(void)
{
}


IllCooker::~IllCooker(void)
{
}

const long UnderFetchNum = 980; 

bool IllCooker::Cook(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
					 const std::vector<ns::RowPtr>& tgtRows,
					 const std::map<OID, IllFileNames>& id2names,
					 LPCTSTR roadLinkTable,
					 std::map<OID,InfIllInfo>& infIllInfos,
					 std::map<LqIllKey, LqIllInfo>& lqIllInfos)
{
	bool retVal = true;
	// RowPtrから必要な属性を取得し、コンテナに格納
	std::map<OID, IllLinkInfo> tgtIllLinkInfo; // キー:イラストリンクID 要素：イラストリンク情報
	if (! GetIllLinkInfo(tgtRows, tgtIllLinkInfo))
		return false;
	// 各レコードのノードID数の合計が1000個を越えない単位でノードを検索し処理を実施
	std::vector<OID> procNodes;  // 検索ノードID格納コンテナ
	std::vector<IllLinkInfo> procIllLinkInfo; // 処理対象イラストリンク情報コンテナ
	for (const auto& illLinkInfo: tgtIllLinkInfo) {
		// 検索ノードID格納コンテナにイラストリンクのノードID列を挿入
		for (const auto& tgtNodeId : illLinkInfo.second.e_tgtNodeIds)
			procNodes.push_back(tgtNodeId);
		// イラストリンク情報を処理対象イラストリンク情報コンテナに挿入
		procIllLinkInfo.push_back(illLinkInfo.second);
		// tgtNodesの数がUnderFetchNumを越えたら、変換処理実施(1レコードのノード列の最大値が20なので、980を越えたら処理をすることで1000は超えない)
		if (UnderFetchNum < procNodes.size()) {
			// 変換処理実施
			if (! ConvertIllLink(orgRlDataBase,
								 roadLinkTable,
								 procIllLinkInfo,
								 procNodes,
								 id2names, 
								 infIllInfos, 
								 lqIllInfos))
				retVal = false;
			// 処理用コンテナ初期化
			procNodes.clear();
			procIllLinkInfo.clear();
		}
	}
	// for文内で変換されなかったレコードの変換
	if (0 < procNodes.size() || 0 < procIllLinkInfo.size()) {
		// 変換処理実施
		if (! ConvertIllLink(orgRlDataBase,
								roadLinkTable,
								procIllLinkInfo,
								procNodes,
								id2names, 
								infIllInfos, 
								lqIllInfos))
			retVal = false;
		// 処理用コンテナ初期化（あんまり意味はないが今後、この処理のあとにコードが足された時のことを考えて・・・） 
		procNodes.clear();
		procIllLinkInfo.clear();
	}
	return retVal;
}


bool IllCooker::GetIllLinkInfo(const std::vector<ns::RowPtr>& tgtRows,
							   std::map<OID, IllLinkInfo>& tgtIllLinkInfo)
{
	CString errMsg;

	bool isNull = false;
	for (const auto& row :tgtRows) {
		// objectid取得
		long oid = row->getInt(CT2CA(::illust_link::kObjectID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%sがNULLです"), ::illust_link::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName);
			return false;
		}
		// イラストリンククラス取得
		long illustClass  = row->getInt(CT2CA(::illust_link::kIllustClass), &isNull);
		if (isNull) {
			errMsg.Format(_T("%sがNULLです"), ::illust_link::kIllustClass);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, oid);
			return false;
		} 
		// ノードID列取得
		CString nodeFName;
		std::vector<OID> nodeIds;
		for (int i=1; i <= MaxNodeNum; ++i) {
			nodeFName.Format(_T("%s_%.2d"), CommonFName, i);
			long nodeid = row->getInt(CT2CA(nodeFName), &isNull);
			if (isNull) {
				if (1 == i || 2 == i) {
					// nodeid_01, nodeid02は必ずノードが存在するはず
					errMsg.Format(_T("%sがNULLです"), nodeFName);
					LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, oid);
					return false;
				}
				break;
			}
			nodeIds.push_back(nodeid);
		}
		IllLinkInfo illLinkInfo(oid, illustClass, nodeIds);
		std::map<OID, IllLinkInfo>::_Pairib pib;
		pib = tgtIllLinkInfo.insert(std::pair<OID, IllLinkInfo>(oid, illLinkInfo));
		if (! pib.second) {
			errMsg.Format(_T("%sが同一なレコードが複数存在します。"), ::illust_link::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, oid);
			return false;
		}
	}
	return true;
}

bool IllCooker::ConvertIllLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
							   LPCTSTR roadLinkTable,
							   const std::vector<IllLinkInfo>& procIllLinkInfo, 
							   const std::vector<OID>&  procTgtNodes,
							   const std::map<OID, IllFileNames>& id2names,
							   std::map<OID, InfIllInfo>& infIllInfos,
						 	   std::map<LqIllKey, LqIllInfo>& lqIllInfos)
{
	bool retVal = true;
	// procTgtNodesをFromNode or ToNodeに持つ道路リンク取得
	std::map<RoadLinkKey, RoadLinkInfo> relRoadLinkInfos;
	if (! GetRelRoadLink(orgRlDataBase, roadLinkTable, procTgtNodes, relRoadLinkInfos))
		return false;
	// イラストリンクの変換
	for (const auto& illLinkInfo : procIllLinkInfo) {
		// 関連するリンク列の取得
		std::vector<LinkQueueInfo> lqInfos;
		if (! nq2lq(illLinkInfo.e_objectId, illLinkInfo.e_tgtNodeIds, relRoadLinkInfos, lqInfos)) {
			retVal = false;
			continue; 
		}
		// イラスト名取得
		std::map<OID, IllFileNames>::const_iterator itr = id2names.find(illLinkInfo.e_objectId);
		if (id2names.end() == itr) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("対応するイラストファイル名が存在しません"), ::illust_link::kTableName, illLinkInfo.e_objectId);
			retVal = false;
			continue;
		}
		// inf_illustレコード作成
		InfIllInfo infIllInfo(illLinkInfo.e_objectId, illLinkInfo.e_illustClass, itr->second.e_tgtIllName);
		std::map<OID, InfIllInfo>::_Pairib pib;
		pib = infIllInfos.insert(std::pair<OID, InfIllInfo>(infIllInfo.e_objectID, infIllInfo));
		if (! pib.second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("同一idのレコードが存在します"), ::illust_link::kTableName, infIllInfo.e_objectID);
			retVal = false;
			continue;
		}
		// lq_illustレコード作成
		for (long i = 0; i < lqInfos.size(); ++i) {
			LqIllKey lqIllKey(illLinkInfo.e_objectId, i+1);
			LqIllInfo lqIllInfo(illLinkInfo.e_objectId, lqInfos[i].e_linkid, lqInfos[i].e_linkDir, i+1);
			std::map<LqIllKey, LqIllInfo>::_Pairib pib;
			pib = lqIllInfos.insert(std::pair<LqIllKey, LqIllInfo>(lqIllKey, lqIllInfo));
			if (! pib.second) {
				CString errMsg;
				errMsg.Format(_T("同一キーのレコードが存在します:%s = %d, %s = %d"), egg::schema::lq_illust::kInfId, lqIllKey.e_infId, egg::schema::lq_illust::kSequence, lqIllKey.e_Seq);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, egg::schema::lq_illust::kTableName);
				retVal = false;
				continue;
			}
		}
	}
	return retVal;
}

bool IllCooker::GetRelRoadLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
							   LPCTSTR roadLinkTable,
							   const std::vector<OID>& procTgtNodes,
							   std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos)
{
	CString errMsg;
	// IN命令用NodeID列作成
	// procTgtNodesを1000個ずつ列挙
	uh::ElementsStrings nodeIdQueue = uh::enumerate_elements(procTgtNodes);
	// procTgtNodesは1000個以下なので、一つしか作成されないはず
	if (1 != nodeIdQueue.size()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リンク取得時の条件となるノード数が1000を越えています。開発者にお問い合わせください。"), sindy::schema::road_link::kTableName);
		return false;
	}
	// FromNode基準でリンク情報取得
	CString getLinkSqlByFNode;
	getLinkSqlByFNode.Format(_T("SELECT * FROM %s WHERE %s IN (%s)"), roadLinkTable, ::road_link::kFromNodeID, CA2CT(nodeIdQueue[0].c_str()));
	ns::RowsPtr rowsByFrom = orgRlDataBase->execute(CT2CA(getLinkSqlByFNode));
	if (! rowsByFrom) {
		errMsg.Format(_T("データ抽出失敗:sql=%s"), getLinkSqlByFNode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, ::road_link::kTableName);
		return false;
	}
	if (! GetLinkInfo(rowsByFrom, relRoadLinkInfos))
		return false;
	// ToNode基準でリンク情報取得
	CString getLinkSqlByTNode;
	getLinkSqlByTNode.Format(_T("SELECT * FROM %s WHERE %s IN (%s)"), roadLinkTable, ::road_link::kToNodeID, CA2CT(nodeIdQueue[0].c_str()));
	ns::RowsPtr rowsByTo = orgRlDataBase->execute(CT2CA(getLinkSqlByTNode));
	if (! rowsByTo) {
		errMsg.Format(_T("データ抽出失敗:sql=%s"), getLinkSqlByTNode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, ::road_link::kTableName);
		return false;
	}
	if (! GetLinkInfo(rowsByTo, relRoadLinkInfos))
		return false;
	return true;
}

bool IllCooker::GetLinkInfo(const ns::RowsPtr& rows, std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos)
{
	CString errMsg;
	ns::RowPtr row;
	while (row = rows->next()) {
		RoadLinkInfo roadLinkInfo = {-1, -1, -1};
		bool isNull = false;
		roadLinkInfo.e_objectId = row->getInt(CT2CA(::road_link::kObjectID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%sがNULLです"), ::road_link::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName);
			return false;
		}
		roadLinkInfo.e_fromNodeId = row->getInt(CT2CA(::road_link::kFromNodeID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%sがNULLです"), ::road_link::kFromNodeID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName, roadLinkInfo.e_objectId);
			return false;
		}
		roadLinkInfo.e_toNodeId = row->getInt(CT2CA(::road_link::kToNodeID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%sがNULLです"), ::road_link::kToNodeID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName, roadLinkInfo.e_objectId);
			return false;
		}
		RoadLinkKey roadLinkKey(roadLinkInfo.e_fromNodeId, roadLinkInfo.e_toNodeId);
		std::map<RoadLinkKey, RoadLinkInfo>::iterator itr = relRoadLinkInfos.find(roadLinkKey);
		if (relRoadLinkInfos.end() == itr) {
			std::map<RoadLinkKey, RoadLinkInfo>::_Pairib pib;
			pib = relRoadLinkInfos.insert(std::pair<RoadLinkKey, RoadLinkInfo>(roadLinkKey, roadLinkInfo));
			if (! pib.second) {
				errMsg.Format(_T("始終点ノードIDが同一の道路リンクが複数回取得されました:%s=%d, %s=%d"), ::road_link::kFromNodeID, roadLinkKey.e_fromNodeId, ::road_link::kToNodeID, roadLinkKey.e_toNodeId);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName);
				return false;
			}
		}
	}
	return true;
}

bool IllCooker::nq2lq(OID illLinkId,
					  const std::vector<OID>& tgtNodeIds,
					  const std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos,
					  std::vector<LinkQueueInfo>& lqInfos)
{
	CString errMsg;
	long nodeIdNum = tgtNodeIds.size();
	if (2 > nodeIdNum) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("関連ノードが2個未満です"), ::illust_link::kTableName, illLinkId);
		return false;
	}
	for (long i = 0; i < nodeIdNum - 1; ++i) {
		LinkQueueInfo lqInfo = {-1, -1};
		// 正方向で検索
		RoadLinkKey forwardKey(tgtNodeIds[i], tgtNodeIds[i+1]);
		std::map<RoadLinkKey, RoadLinkInfo>::const_iterator fItr = relRoadLinkInfos.find(forwardKey);
		if (relRoadLinkInfos.end() != fItr) {
			// 正方向でリンク情報登録
			lqInfo.e_linkid = fItr->second.e_objectId;
			lqInfo.e_linkDir = ForwardDir;
			lqInfos.push_back(lqInfo);
			continue;
		}
		// 逆方向で検索
		RoadLinkKey reverseKey(tgtNodeIds[i+1], tgtNodeIds[i]);
		std::map<RoadLinkKey, RoadLinkInfo>::const_iterator rItr = relRoadLinkInfos.find(reverseKey);
		if (relRoadLinkInfos.end() != rItr) {
			// 逆方向でリンク情報登録
			lqInfo.e_linkid = rItr->second.e_objectId;
			lqInfo.e_linkDir = ReverseDir;
			lqInfos.push_back(lqInfo);
			continue;
		}
		// ここまで来たら関連付くリンクが存在しないことになるのでエラー
		errMsg.Format(_T("関連付くリンクが存在しません。ノード1:%d, ノード2:%d"), tgtNodeIds[i], tgtNodeIds[i+1]);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, illLinkId);
		return false;
	}
	return true;
}
