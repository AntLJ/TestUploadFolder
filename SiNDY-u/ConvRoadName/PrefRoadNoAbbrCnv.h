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
#include "BaseCnv.h"
/**
 * @brief �����ԍ����̕ϊ��c�[�� 
 */
class PrefRoadNoAbbrCnv :
	public BaseCnv
{
public:
	/**
	 * @brief�R���X�g���N�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 */
	PrefRoadNoAbbrCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList)
		:BaseCnv(ipRoadLink, ipRoadNameList, nullptr) 
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kPrefRoadNoAbbr;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("%s IN (%ld,%ld,%ld,%ld) AND %s <> 0 AND %s <> 0"), sindy::schema::road_link::kRoadClass,
			sindy::schema::road_link::road_class::kPrefectureMain, sindy::schema::road_link::road_class::kPrefectureNormal,
			sindy::schema::road_link::road_class::kTollPrefectureMain, sindy::schema::road_link::road_class::kTollPrefectureNormal,
			sindy::schema::road_link::kPrefCode, sindy::schema::road_link::kRoadNo);;
	}
	~PrefRoadNoAbbrCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;
	bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) override;

private:
	long m_roadLinkPrefCodeIndex;		//!< ���H�����NPREF_CODE�t�B�[���h�C���f�b�N�X
	long m_roadLinkRoadNoIndex;			//!< ���H�����NROAD_NO�t�B�[���h�C���f�b�N�X

};

