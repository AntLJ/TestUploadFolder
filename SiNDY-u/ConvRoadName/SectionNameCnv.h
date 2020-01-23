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

#pragma once
#include "SplitBasicTokenRuleCnv.h"
/**
 * @brief ��Ԗ��̕ϊ��N���X
 */
class SectionNameCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param �����f�[�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipRoadCodeList [in] ���H�H���e�[�u��
	 * @param ipPrefRoadList [in] ��Ԗ��̃e�[�u��
	 */
	SectionNameCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipSectionCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipSectionCodeList)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kSectionName;
		// ���H�����N������ݒ�
		m_roadLinkWhere.Format(_T("%s <> 0"), sindy::schema::road_link::kSectionCode);
	}
	~SectionNameCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadLinkSectionCodeIndex;			//!< ���H�����NSECTiON_CODE�t�B�[���h�C���f�b�N�X
	long m_sectionCodeListSectionCodeIndex;		//!< ��Ԗ���SECTION_CODE�t�B�[���h�C���f�b�N�X
	long m_sectionCodeListNameKanjiIndex;		//!< ��Ԗ���NAME_KANJI�t�B�[���h�C���f�b�N�X
	long m_sectionCodeListNameYomiIndex;		//!< ��Ԗ���NAME_YOMI�t�B�[���h�C���f�b�N�X

	std::map<long, _IRowPtr> m_tgtSecCodeListRecs;	//!< �ϊ��Ώۋ�Ԗ��̃��X�g�@�L�[�FSECTION_CODE
};

