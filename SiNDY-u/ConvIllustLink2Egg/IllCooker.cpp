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
	// RowPtr����K�v�ȑ������擾���A�R���e�i�Ɋi�[
	std::map<OID, IllLinkInfo> tgtIllLinkInfo; // �L�[:�C���X�g�����NID �v�f�F�C���X�g�����N���
	if (! GetIllLinkInfo(tgtRows, tgtIllLinkInfo))
		return false;
	// �e���R�[�h�̃m�[�hID���̍��v��1000���z���Ȃ��P�ʂŃm�[�h�����������������{
	std::vector<OID> procNodes;  // �����m�[�hID�i�[�R���e�i
	std::vector<IllLinkInfo> procIllLinkInfo; // �����ΏۃC���X�g�����N���R���e�i
	for (const auto& illLinkInfo: tgtIllLinkInfo) {
		// �����m�[�hID�i�[�R���e�i�ɃC���X�g�����N�̃m�[�hID���}��
		for (const auto& tgtNodeId : illLinkInfo.second.e_tgtNodeIds)
			procNodes.push_back(tgtNodeId);
		// �C���X�g�����N���������ΏۃC���X�g�����N���R���e�i�ɑ}��
		procIllLinkInfo.push_back(illLinkInfo.second);
		// tgtNodes�̐���UnderFetchNum���z������A�ϊ��������{(1���R�[�h�̃m�[�h��̍ő�l��20�Ȃ̂ŁA980���z�����珈�������邱�Ƃ�1000�͒����Ȃ�)
		if (UnderFetchNum < procNodes.size()) {
			// �ϊ��������{
			if (! ConvertIllLink(orgRlDataBase,
								 roadLinkTable,
								 procIllLinkInfo,
								 procNodes,
								 id2names, 
								 infIllInfos, 
								 lqIllInfos))
				retVal = false;
			// �����p�R���e�i������
			procNodes.clear();
			procIllLinkInfo.clear();
		}
	}
	// for�����ŕϊ�����Ȃ��������R�[�h�̕ϊ�
	if (0 < procNodes.size() || 0 < procIllLinkInfo.size()) {
		// �ϊ��������{
		if (! ConvertIllLink(orgRlDataBase,
								roadLinkTable,
								procIllLinkInfo,
								procNodes,
								id2names, 
								infIllInfos, 
								lqIllInfos))
			retVal = false;
		// �����p�R���e�i�������i����܂�Ӗ��͂Ȃ�������A���̏����̂��ƂɃR�[�h�������ꂽ���̂��Ƃ��l���āE�E�E�j 
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
		// objectid�擾
		long oid = row->getInt(CT2CA(::illust_link::kObjectID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%s��NULL�ł�"), ::illust_link::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName);
			return false;
		}
		// �C���X�g�����N�N���X�擾
		long illustClass  = row->getInt(CT2CA(::illust_link::kIllustClass), &isNull);
		if (isNull) {
			errMsg.Format(_T("%s��NULL�ł�"), ::illust_link::kIllustClass);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, oid);
			return false;
		} 
		// �m�[�hID��擾
		CString nodeFName;
		std::vector<OID> nodeIds;
		for (int i=1; i <= MaxNodeNum; ++i) {
			nodeFName.Format(_T("%s_%.2d"), CommonFName, i);
			long nodeid = row->getInt(CT2CA(nodeFName), &isNull);
			if (isNull) {
				if (1 == i || 2 == i) {
					// nodeid_01, nodeid02�͕K���m�[�h�����݂���͂�
					errMsg.Format(_T("%s��NULL�ł�"), nodeFName);
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
			errMsg.Format(_T("%s������ȃ��R�[�h���������݂��܂��B"), ::illust_link::kObjectID);
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
	// procTgtNodes��FromNode or ToNode�Ɏ����H�����N�擾
	std::map<RoadLinkKey, RoadLinkInfo> relRoadLinkInfos;
	if (! GetRelRoadLink(orgRlDataBase, roadLinkTable, procTgtNodes, relRoadLinkInfos))
		return false;
	// �C���X�g�����N�̕ϊ�
	for (const auto& illLinkInfo : procIllLinkInfo) {
		// �֘A���郊���N��̎擾
		std::vector<LinkQueueInfo> lqInfos;
		if (! nq2lq(illLinkInfo.e_objectId, illLinkInfo.e_tgtNodeIds, relRoadLinkInfos, lqInfos)) {
			retVal = false;
			continue; 
		}
		// �C���X�g���擾
		std::map<OID, IllFileNames>::const_iterator itr = id2names.find(illLinkInfo.e_objectId);
		if (id2names.end() == itr) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�Ή�����C���X�g�t�@�C���������݂��܂���"), ::illust_link::kTableName, illLinkInfo.e_objectId);
			retVal = false;
			continue;
		}
		// inf_illust���R�[�h�쐬
		InfIllInfo infIllInfo(illLinkInfo.e_objectId, illLinkInfo.e_illustClass, itr->second.e_tgtIllName);
		std::map<OID, InfIllInfo>::_Pairib pib;
		pib = infIllInfos.insert(std::pair<OID, InfIllInfo>(infIllInfo.e_objectID, infIllInfo));
		if (! pib.second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("����id�̃��R�[�h�����݂��܂�"), ::illust_link::kTableName, infIllInfo.e_objectID);
			retVal = false;
			continue;
		}
		// lq_illust���R�[�h�쐬
		for (long i = 0; i < lqInfos.size(); ++i) {
			LqIllKey lqIllKey(illLinkInfo.e_objectId, i+1);
			LqIllInfo lqIllInfo(illLinkInfo.e_objectId, lqInfos[i].e_linkid, lqInfos[i].e_linkDir, i+1);
			std::map<LqIllKey, LqIllInfo>::_Pairib pib;
			pib = lqIllInfos.insert(std::pair<LqIllKey, LqIllInfo>(lqIllKey, lqIllInfo));
			if (! pib.second) {
				CString errMsg;
				errMsg.Format(_T("����L�[�̃��R�[�h�����݂��܂�:%s = %d, %s = %d"), egg::schema::lq_illust::kInfId, lqIllKey.e_infId, egg::schema::lq_illust::kSequence, lqIllKey.e_Seq);
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
	// IN���ߗpNodeID��쐬
	// procTgtNodes��1000����
	uh::ElementsStrings nodeIdQueue = uh::enumerate_elements(procTgtNodes);
	// procTgtNodes��1000�ȉ��Ȃ̂ŁA������쐬����Ȃ��͂�
	if (1 != nodeIdQueue.size()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����N�擾���̏����ƂȂ�m�[�h����1000���z���Ă��܂��B�J���҂ɂ��₢���킹���������B"), sindy::schema::road_link::kTableName);
		return false;
	}
	// FromNode��Ń����N���擾
	CString getLinkSqlByFNode;
	getLinkSqlByFNode.Format(_T("SELECT * FROM %s WHERE %s IN (%s)"), roadLinkTable, ::road_link::kFromNodeID, CA2CT(nodeIdQueue[0].c_str()));
	ns::RowsPtr rowsByFrom = orgRlDataBase->execute(CT2CA(getLinkSqlByFNode));
	if (! rowsByFrom) {
		errMsg.Format(_T("�f�[�^���o���s:sql=%s"), getLinkSqlByFNode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, ::road_link::kTableName);
		return false;
	}
	if (! GetLinkInfo(rowsByFrom, relRoadLinkInfos))
		return false;
	// ToNode��Ń����N���擾
	CString getLinkSqlByTNode;
	getLinkSqlByTNode.Format(_T("SELECT * FROM %s WHERE %s IN (%s)"), roadLinkTable, ::road_link::kToNodeID, CA2CT(nodeIdQueue[0].c_str()));
	ns::RowsPtr rowsByTo = orgRlDataBase->execute(CT2CA(getLinkSqlByTNode));
	if (! rowsByTo) {
		errMsg.Format(_T("�f�[�^���o���s:sql=%s"), getLinkSqlByTNode);
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
			errMsg.Format(_T("%s��NULL�ł�"), ::road_link::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName);
			return false;
		}
		roadLinkInfo.e_fromNodeId = row->getInt(CT2CA(::road_link::kFromNodeID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%s��NULL�ł�"), ::road_link::kFromNodeID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName, roadLinkInfo.e_objectId);
			return false;
		}
		roadLinkInfo.e_toNodeId = row->getInt(CT2CA(::road_link::kToNodeID), &isNull);
		if (isNull) {
			errMsg.Format(_T("%s��NULL�ł�"), ::road_link::kToNodeID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::road_link::kTableName, roadLinkInfo.e_objectId);
			return false;
		}
		RoadLinkKey roadLinkKey(roadLinkInfo.e_fromNodeId, roadLinkInfo.e_toNodeId);
		std::map<RoadLinkKey, RoadLinkInfo>::iterator itr = relRoadLinkInfos.find(roadLinkKey);
		if (relRoadLinkInfos.end() == itr) {
			std::map<RoadLinkKey, RoadLinkInfo>::_Pairib pib;
			pib = relRoadLinkInfos.insert(std::pair<RoadLinkKey, RoadLinkInfo>(roadLinkKey, roadLinkInfo));
			if (! pib.second) {
				errMsg.Format(_T("�n�I�_�m�[�hID������̓��H�����N��������擾����܂���:%s=%d, %s=%d"), ::road_link::kFromNodeID, roadLinkKey.e_fromNodeId, ::road_link::kToNodeID, roadLinkKey.e_toNodeId);
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
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�֘A�m�[�h��2�����ł�"), ::illust_link::kTableName, illLinkId);
		return false;
	}
	for (long i = 0; i < nodeIdNum - 1; ++i) {
		LinkQueueInfo lqInfo = {-1, -1};
		// �������Ō���
		RoadLinkKey forwardKey(tgtNodeIds[i], tgtNodeIds[i+1]);
		std::map<RoadLinkKey, RoadLinkInfo>::const_iterator fItr = relRoadLinkInfos.find(forwardKey);
		if (relRoadLinkInfos.end() != fItr) {
			// �������Ń����N���o�^
			lqInfo.e_linkid = fItr->second.e_objectId;
			lqInfo.e_linkDir = ForwardDir;
			lqInfos.push_back(lqInfo);
			continue;
		}
		// �t�����Ō���
		RoadLinkKey reverseKey(tgtNodeIds[i+1], tgtNodeIds[i]);
		std::map<RoadLinkKey, RoadLinkInfo>::const_iterator rItr = relRoadLinkInfos.find(reverseKey);
		if (relRoadLinkInfos.end() != rItr) {
			// �t�����Ń����N���o�^
			lqInfo.e_linkid = rItr->second.e_objectId;
			lqInfo.e_linkDir = ReverseDir;
			lqInfos.push_back(lqInfo);
			continue;
		}
		// �����܂ŗ�����֘A�t�������N�����݂��Ȃ����ƂɂȂ�̂ŃG���[
		errMsg.Format(_T("�֘A�t�������N�����݂��܂���B�m�[�h1:%d, �m�[�h2:%d"), tgtNodeIds[i], tgtNodeIds[i+1]);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::illust_link::kTableName, illLinkId);
		return false;
	}
	return true;
}
