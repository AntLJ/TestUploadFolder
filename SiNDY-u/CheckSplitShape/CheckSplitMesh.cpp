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
#include "CheckSplitMesh.h"



bool CCheckSplitMesh::Init(const CString& strMeshCode,
							const CString& strBeforeShpPath,
							const CString& strAfterShpPath,
							check_mode::ECode CheckMode,
							IFeatureClassPtr ipBeforeHNodeClass,
							ITablePtr ipAfterRelHeightTable,
							IFeatureClassPtr ipMeshClass,
							bool bJGD2000,
							double dMinSegment)
{
	m_strMeshCode = strMeshCode;
	m_CheckMode = CheckMode;

	m_ipBeforeHNodeClass = ipBeforeHNodeClass;
	m_ipAfterRelHeightTable = ipAfterRelHeightTable;
	m_ipMeshClass = ipMeshClass;

	m_bJGD2000 = bJGD2000;
	m_dMinSegment = dMinSegment;

	// �ϊ��O�V�F�[�v�t�@�C���̃��[�N�X�y�[�X�擾
	if(!GetWorkspace(strBeforeShpPath, m_ipBeforeShpWork))
		return false;

	// �ϊ���V�F�[�v�t�@�C���̃��[�N�X�y�[�X�擾
	if(!GetWorkspace(strAfterShpPath, m_ipAfterShpWork))
		return false;

	return true;
}

bool CCheckSplitMesh::LoadData()
{
	bool bReturn = true;

	// �V�F�[�v�t�@�C���̃t�B�[�`���N���X���Z�b�g
	CString strRWLFileName, strRWNFileName, strRSAFileName;
	strRWLFileName.Format(_T("%s_%s"), m_strMeshCode, shp05::schema::road::road_walk_link::kSuffix);
	strRWNFileName.Format(_T("%s_%s"), m_strMeshCode, shp05::schema::road::road_walk_node::kSuffix);
	strRSAFileName.Format(_T("%s_%s"), m_strMeshCode, shp05::schema::road::segment_attr::kSuffix);

	// �����ORWL�擾
	if(check_mode::kIntegrity != m_CheckMode){
		// ������
		if(m_cBeforeRWLAll.Init(true, m_ipBeforeShpWork, strRWLFileName)){
			// �f�[�^�ǂݍ���
			if(m_cBeforeRWLAll.LoadData()){
				// �O���[�v��
				if(!m_cBeforeRWLAll.ConstractGroupBefore())
					bReturn = false;
			} else
				bReturn = false;
		} else
			bReturn = false;
	}

	// ������RWL�擾
	{
		// ������
		if(m_cAfterRWLAll.Init(false, m_ipAfterShpWork, strRWLFileName)){
			// �f�[�^�ǂݍ���
			if(m_cAfterRWLAll.LoadData()){
				// �O���[�v��
				if(!m_cAfterRWLAll.ConstractGroupAfter(m_cBeforeRWLAll.m_mapDivGroup))
					bReturn = false;
			} else
				bReturn = false;
		} else
			bReturn = false;
	}

	// �����ORWN�擾
	if(check_mode::kIntegrity != m_CheckMode){
		// ������
		if(m_cBeforeRWNAll.Init(true, m_ipBeforeShpWork, strRWNFileName)){
			// �f�[�^�ǂݍ���
			if(!m_cBeforeRWNAll.LoadData())
				bReturn = false;
		} else
			bReturn = false;
	}

	// ������RWN�擾
	{
		// ������
		if(m_cAfterRWNAll.Init(false, m_ipAfterShpWork, strRWNFileName)){
			// �f�[�^�ǂݍ���
			if(!m_cAfterRWNAll.LoadData())
				bReturn = false;
		} else
			bReturn = false;
	}

	// �ϊ��ORSA�擾�i�����N�����������������̂ݎ擾�j
	if(check_mode::kIntegrity != m_CheckMode){
		if(check_mode::kSegmentAttr == m_CheckMode){
			// ������
			if(m_cBeforeRSAAll.Init(true, m_ipBeforeShpWork, strRSAFileName)){
				// �f�[�^�ǂݍ���
				if(!m_cBeforeRSAAll.LoadData())
					bReturn = false;
			} else
				bReturn = false;
		}
	}

	// �����OHeightNode�擾�i�����m�[�h�����������̂ݎ擾�j
	if(check_mode::kIntegrity != m_CheckMode){
		if(check_mode::kHeightNode == m_CheckMode){
			// ������
			if(m_cBeforeHNodeAll.Init(m_ipBeforeHNodeClass, m_ipMeshClass, m_strMeshCode, m_bJGD2000)){
				// �f�[�^�ǂݍ���
				if(!m_cBeforeHNodeAll.LoadData())
					bReturn = false;
			} else
				bReturn = false;
		}
	}

	// ������HeightNode�擾�i�����m�[�h�����������̂ݎ擾�j
	if(check_mode::kIntegrity != m_CheckMode){
		if(check_mode::kHeightNode == m_CheckMode){
			// ������
			if(m_cAfterRelHeightAll.Init(m_ipAfterRelHeightTable, m_cBeforeHNodeAll.m_setLinkID)){
				// �f�[�^�ǂݍ���
				if(!m_cAfterRelHeightAll.LoadData())
					bReturn = false;
			} else
				bReturn = false;
		}
	}

	return bReturn;
}

bool CCheckSplitMesh::CheckData()
{
	bool bReturn = true;

	std::map<LONLAT,std::vector<UNIQID>> mapRebuildHN;
	std::set<UNIQID> setRebuildLinkId;
	if(check_mode::kHeightNode == m_CheckMode){
		// �ϊ���̍�����񂩂獂���m�[�h�����\�z
		if(!RebuldHeightNodeFromRelReight(m_cBeforeHNodeAll, m_cAfterRWLAll, m_cAfterRWNAll, mapRebuildHN, setRebuildLinkId))
			bReturn = false;
	}

	// NW�������`�F�b�N
	CCheckNWIntegrity cCheckNWIntegrity(m_CheckMode, m_bJGD2000, m_dMinSegment);
	if(!cCheckNWIntegrity.run(m_cAfterRWLAll, m_cAfterRWNAll, m_cAfterRelHeightAll))
		bReturn = false;

	if(check_mode::kIntegrity != m_CheckMode){
		// ��ˑO�`�F�b�N
		CCheckAfter2Before cCheckAfter2Before(m_CheckMode);
		if(!cCheckAfter2Before.run(m_cBeforeRWLAll, m_cBeforeRWNAll, m_cBeforeRSAAll, m_cBeforeHNodeAll, m_cAfterRWLAll, m_cAfterRWNAll, m_cAfterRelHeightAll, mapRebuildHN, setRebuildLinkId))
			bReturn = false;

		// �O�ˌ�`�F�b�N
		CCheckBefore2After cCheckBefore2After(m_CheckMode);
		if(!cCheckBefore2After.run(m_cBeforeRWLAll, m_cBeforeRWNAll, m_cBeforeHNodeAll, m_cAfterRWLAll, m_cAfterRWNAll, m_cAfterRelHeightAll, mapRebuildHN))
			bReturn = false;
	}

	return bReturn;
}

bool CCheckSplitMesh::RebuldHeightNodeFromRelReight(const CHeightNodeAll& cBeforeHNodeAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, std::map<LONLAT,std::vector<UNIQID>>& mapReAfterHN, std::set<UNIQID>& setReAfterRH)
{
	bool bReturn = true;

	for(const auto beforeHNLonLat : cBeforeHNodeAll.m_mapLonLat2OID){
		// �ϊ��O�����m�[�h�Ɠ����`��̕ϊ���m�[�h�����݂��邩
		const auto afterRWNLonLat = cAfterRWNAll.m_mapLonLat2OID.find(beforeHNLonLat.first);
		if(cAfterRWNAll.m_mapLonLat2OID.cend() == afterRWNLonLat){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNExistInAfterRWN, cBeforeHNodeAll.m_strTableName, beforeHNLonLat.second.eID, beforeHNLonLat.second.eDivID);
			bReturn = false;
			continue;
		}
		// �ϊ��O�����m�[�h�̍������LINK1,LINK2�Ƃ��ɓ����l�̏ꍇ�͏����X���[
		const auto beforeHN = cBeforeHNodeAll.m_mapData.at(beforeHNLonLat.second);
		if(beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1Height)).lVal ==
			beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2Height)).lVal)
			continue;

		// ���̃m�[�h�ɕR�t�������N���擾
		const auto afterNode2Link = cAfterRWLAll.m_mapNode2Link.find(afterRWNLonLat->second);
		if(cAfterRWLAll.m_mapNode2Link.cend() == afterNode2Link){
			// ���̕�����NW�������`�F�b�N�Ō��o�ς�
			bReturn = false;
			continue;
		}
		// �ڑ������N��4�{�łȂ��ꍇ�̓G���[
		if(4 != afterNode2Link->second.size()){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNReconstractHNData, cBeforeHNodeAll.m_strTableName, beforeHNLonLat.second.eID, beforeHNLonLat.second.eDivID);
			bReturn = false;
			continue;
		}
		// 
		std::vector<UNIQID> vecID;
		for(const auto id : afterNode2Link->second)
			vecID.push_back(id);

		// �����N���������Ă���Ƃ���ŕ��f����Ă���̂�
		// 1,2�Ԗڂ̃����N��OBJECTID�͈�v�ADIVID�͘A��
		// 3,4�Ԗڂ̃����N��OBJECTID�͈�v�ADIVID�͘A��
		// ���Ă���͂�
		if(vecID[0].eID != vecID[1].eID || vecID[0].eDivID != vecID[1].eDivID - 1 ||
			vecID[2].eID != vecID[3].eID || vecID[2].eDivID != vecID[3].eDivID - 1){
				// �G���[
				bReturn = false;
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNReconstractNGHNData, cBeforeHNodeAll.m_strTableName, beforeHNLonLat.second.eID, beforeHNLonLat.second.eDivID);
				continue;
		}
		mapReAfterHN.insert(std::make_pair(beforeHNLonLat.first, vecID));
		for(const auto link : vecID)
			setReAfterRH.insert(link);
	}

	return bReturn;
}
