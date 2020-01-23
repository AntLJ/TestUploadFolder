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
 * @brief �s�s�������̕ϊ��N���X
 */
class RoadHwyAbbrCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param �����f�[�^
 	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
 	 * @param ipRoadCodeList [in] ���H�H���e�[�u��
	 */
	RoadHwyAbbrCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, nullptr)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kRoadHwyAbbr;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("(200000 <= %s AND %s < 300000) OR (400000 <= %s AND %s < 500000) OR (600000 <= %s AND %s < 700000)"),
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode, 
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode);
	}
	~RoadHwyAbbrCnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadCodeListDisplayKanjiIndex;		//!< �H���R�[�h���X�gDISPLAY_KANJI�t�B�[���h�C���f�b�N�X
	long m_roadCodeListDisplayYomiIndex;		//!< �H���R�[�h���X�gDISPLAY_YOMI�t�B�[���h�C���f�b�N�X
};

