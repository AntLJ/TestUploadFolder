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
#include "PrefNameBaseCnv.h"

/**
 * @brief �������̕ϊ��N���X
 */
class PrefRoadNameCnv :
	public PrefNameBaseCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param �����f�[�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipRoadCodeList [in] ���H�H���e�[�u��
	 * @param ipPrefRoadList [in] �������̃e�[�u��
	 */
	PrefRoadNameCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipPrefRoadList)
		:PrefNameBaseCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipPrefRoadList)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kPrefRoadName;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("%s IN (%ld,%ld,%ld,%ld) AND %s <> 0 AND %s <> 0"), sindy::schema::road_link::kRoadClass,
			sindy::schema::road_link::road_class::kPrefectureMain, sindy::schema::road_link::road_class::kPrefectureNormal,
			sindy::schema::road_link::road_class::kTollPrefectureMain, sindy::schema::road_link::road_class::kTollPrefectureNormal,
			sindy::schema::road_link::kPrefCode, sindy::schema::road_link::kRoadNo);;
	}
	~PrefRoadNameCnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_prefRdListNameKanjiIndex;		//!< �s���{�����X�gNAME_KANJI�t�B�[���h�C���f�b�N�X
	long m_prefRdListNameYomiIndex;			//!< �s���{�����X�gNAME_YOMI�t�B�[���h�C���f�b�N�X
};

