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
class ShieldHwyNoCnv : public FullNameEqSpltNameCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipRoadCodeList [in] ���H�H���e�[�u��
	 */
	ShieldHwyNoCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		: FullNameEqSpltNameCnv(ipRoadLink, ipRoadNameList, ipRoadCodeList),
		 m_roadLinkRoadCodeIndex(-1), m_roadCodeListRoadCodeIndex(-1), m_roadCodeListShieldNoStrIndex(-1)
	{
		// �o�͖��̎�ʐݒ�
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kShieldHwyNo;
		// ���H�����N���������ݒ�
		m_roadLinkWhere.Format(_T("%s<>0"), sindy::schema::road_link::kRoadCode);
	}
	~ShieldHwyNoCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;


private:
	long m_roadLinkRoadCodeIndex;		//!< ���H�����NRoadCode�t�B�[���h�C���f�b�N�X
	long m_roadCodeListRoadCodeIndex;	//!< �H���R�[�h���X�gRoadCode�t�B�[���h�C���f�b�N�X
	long m_roadCodeListShieldNoStrIndex;	//!< �H���R�[�h���X�gSHIELD_NO_STRING�t�B�[���h�C���f�b�N�X

	std::map<long, _IRowPtr> m_tgtRdCodeListRecs;	//!< �ϊ��Ώۓ��H�H�����X�g���R�[�h �L�[�F�H���R�[�h
	
};

