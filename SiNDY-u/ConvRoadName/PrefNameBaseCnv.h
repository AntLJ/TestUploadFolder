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
 * @brief PREF_CODE_LISTmap�i�[�p�L�[
 */
class PrefCodeKey {
public:
	PrefCodeKey(const long& roadNo, const long& roadCls, const long& prefCode)
		:m_roadNo(roadNo), m_roadCls(roadCls), m_prefCode(prefCode) {}
	~PrefCodeKey() {}
public:
	bool operator < (const PrefCodeKey& prefCodeKey) const
	{
		return this->m_prefCode != prefCodeKey.m_prefCode ? this->m_prefCode < prefCodeKey.m_prefCode :
			this->m_roadNo != prefCodeKey.m_roadNo ? this->m_roadNo < prefCodeKey.m_roadNo :
			this->m_roadCls < prefCodeKey.m_roadCls;
	}
	bool operator == (const PrefCodeKey& prefCodeKey) const
	{
		return this->m_prefCode == prefCodeKey.m_prefCode &&
			this->m_roadNo == prefCodeKey.m_roadNo &&
			this->m_roadCls == prefCodeKey.m_roadCls;
	}
public:
	long m_prefCode;		//!< ���R�[�h
	long m_roadNo;			//!< ���HNo
	long m_roadCls;			//!< ���H�\�����
};

/**
 * @brief �������̕ϊ��x�[�X�N���X
 */
class PrefNameBaseCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/*
	 * @brief �R���X�g���N�^
	 * @param �����f�[�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipRoadCodeList [in] ���H�H���e�[�u��
	 * @param ipPrefRoadList [in] �������̃e�[�u��
	 */
	PrefNameBaseCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipPrefRoadList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipPrefRoadList) {}
	~PrefNameBaseCnv() {}

protected:
	virtual bool setRoadLinkFieldIndex() override;
	virtual bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;

protected:
	long m_roadLinkPrefCodeIndex;		//!< ���H�����NPREF_CODE�t�B�[���h�C���f�b�N�X
	long m_roadLinkRoadNoIndex;			//!< ���H�����NROAD_NO�t�B�[���h�C���f�b�N�X
	long m_roadLinkRoadClsIndex;		//!< ���H�����NROAD_CLASS_C�t�B�[���h�C���f�b�N�X
	long m_prefRdListPrefCodeIndex;	//!< �s���{�����X�gPREF_CODE�t�B�[���h�C���f�b�N�X
	long m_prefRdListRoadNoIndex;		//!< �s���{�����X�gROAD_NO�t�B�[���h�C���f�b�N�X
	long m_prefRdListRoadClsIndex;	//!< �s���{�����X�gRoad_CLASS_C�t�B�[���h�C���f�b�N�X

	std::map<PrefCodeKey, _IRowPtr> m_tgtPrefRdListRecs;	//!< �ϊ��Ώۓs���{�����X�g

};

