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
* @brief ���H���̂ƕ������̂������ł���ϊ��x�[�X�N���X
*/
class FullNameEqSpltNameCnv :
	public BaseCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipSubNameTable [in] ���H���̈ȊO�ɕK�v�Ȗ��̃e�[�u��
	 */
	FullNameEqSpltNameCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipSubNameTable)
		:BaseCnv(ipRoadLink, ipRoadNameList, ipSubNameTable) {}
	~FullNameEqSpltNameCnv() {}
protected:
	bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) override;

};

