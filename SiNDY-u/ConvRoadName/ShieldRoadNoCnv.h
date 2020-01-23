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
#include "FullNameEqSpltNameCnv.h"
/**
 * @brief �W�������ԍ��ϊ��N���X
 */
class ShieldRoadNoCnv : public FullNameEqSpltNameCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 */
	ShieldRoadNoCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList)
		: FullNameEqSpltNameCnv(ipRoadLink, ipRoadNameList, nullptr), m_roadLinkRoadNoIndex(-1)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kShieldRoadNo;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("%s<>0"), sindy::schema::road_link::kRoadNo);
	}
	~ShieldRoadNoCnv() {};

protected:
	bool setRoadLinkFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadLinkRoadNoIndex;			//!< ���H�����NRoadNo�t�B�[���h�C���f�b�N�X
};

