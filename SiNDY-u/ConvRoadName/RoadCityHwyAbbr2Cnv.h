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

// �폜�Ώە����z��
const CString delNameWord2[] = {_T("�����������H"), _T("����"), _T("")};
const CString delYomiWord2[] = {_T("�g�E�L���E�R�E�\�N�h�E��"), _T("�R�E�\�N"), _T("")};

/**
 * @brief �s�s��������2�ϊ��N���X
 */
class RoadCityHwyAbbr2Cnv :
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
	RoadCityHwyAbbr2Cnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, nullptr)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kRoadCityHwyAbbr2;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("(300000 <= %s AND %s < 400000) OR (500000 <= %s AND %s < 600000) OR (700000 <= %s AND %s < 800000)"),
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode);
	}
	~RoadCityHwyAbbr2Cnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;


private:
	CString MkAbbrStr2(const CString& str, const CString delWords[]);

private:
	long m_roadCodeListDisplayKanjiIndex;		//!< �H���R�[�h���X�gDISPLAY_KANJI�t�B�[���h�C���f�b�N�X
	long m_roadCodeListDisplayYomiIndex;		//!< �H���R�[�h���X�gDISPLAY_YOMI�t�B�[���h�C���f�b�N�X
};

