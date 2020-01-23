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
#include "Arguments.h"

class ConvRoadName
{
public:
	ConvRoadName(){}
	~ConvRoadName(){}

public:
	/*
	 * @brief �������s
	 * @param args [in] �R�}���h���C������
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool run(const Arguments& args);

private:
	/*
	 * @brief �ϊ����e�[�u���擾
	 * @param args [in] �R�}���h���C������
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool getTgtTbl(const Arguments& args);

	/*
	 * @brief �����擾
	 * @param dicFile [in] �����t�@�C���p�X
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool getDictionary(const CString& dicFile);

	/*
	 * @breif �e�햼�̕ϊ�
	 * @param rdNameRecs	[out] ���H���̃f�[�^���R�[�h�ϊ�����
	 * @param rlNameLnkRecs [out] ���H�����N�ɕR�Â����H���̃��R�[�h�ϊ�����
	 * @param spltRdNameRecs [out] ���H���̕����f�[�^���R�[�h�ϊ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool cnvEachName(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs);

private:
	IFeatureWorkspacePtr m_ipRoadWork;	//!< ���HDB�p���[�N�X�y�[�X
	IFeatureWorkspacePtr m_ipNameWork;	//!< ���H���̗p���[�N�X�y�[�X
	IFeatureClassPtr m_ipRoadLink;		//!< ���H�����N�t�B�[�`���N���X
	ITablePtr m_ipRoadCodeList;			//!< �H�����̃e�[�u��
	ITablePtr m_ipPrefRoadList;			//!< �������̃e�[�u��
	ITablePtr m_ipSectionCodeList;		//!< ��Ԗ��̃e�[�u��
	ITablePtr m_ipRoadNameList;			//!< ���H���̃��X�g�e�[�u��

	std::map<CString, std::vector<CString>> m_dictionary;	//!< ����
};

