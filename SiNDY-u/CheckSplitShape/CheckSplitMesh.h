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

#include "RWL.h"
#include "RWN.h"
#include "RSA.h"
#include "HeightNode.h"
#include "RelHeight.h"
#include "CheckNWIntegrity.h"
#include "CheckAfter2Before.h"
#include "CheckBefore2After.h"

class CCheckSplitMesh {
public:
	CCheckSplitMesh(){}
	~CCheckSplitMesh(){}

public:
	/**
	 * @brief �������p�֐�
	 * @param strMeshCode              [in] �����Ώۃ��b�V���R�[�h
	 * @param CheckMode                [in] ���������^�C�v
	 * @param ipBeforeHNodeClass       [in] �����O�����m�[�h�N���X
	 * @param m_ipAfterRelHeightTable  [in] �����㑊�΍����e�[�u��
	 * @param ipMeshClass              [in] ���b�V���N���X
	 * @param bJGD2000                 [in] JGD2000�����邩�ǂ����i�����m�[�h�p�j
	 * @param dMinSegment              [in] �����Z�O�����g��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool Init(const CString& strMeshCode,
				const CString& strBeforeShpPath,
				const CString& strAfterShpPath,
				check_mode::ECode CheckMode,
				IFeatureClassPtr ipBeforeHNodeClass,
				ITablePtr m_ipAfterRelHeightTable,
				IFeatureClassPtr ipMeshClass,
				bool bJGD2000,
				double dMinSegment);

	/**
	 * @brief �e�e�[�u���f�[�^�ǂݍ��ݗp�֐�
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool LoadData();

	/**
	 * @brief �e�e�[�u���f�[�^�̃`�F�b�N�����p�֐�
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool CheckData();

private:
	/**
	 * @brief H�����O��N������`�F�b�N
	 * @brief cBeforeHNodeAll     [in] �ϊ��OHNode�f�[�^
	 * @brief cAfterRWLAll        [in] �ϊ���RWL�f�[�^
	 * @brief cAfterRWNAll        [in] �ϊ���RWN�f�[�^
	 * @brief mapRebuildHN        [out] �ϊ���RelHeight����HNode���č\�z��������
	 * @brief setRebuildLinkId    [out] �ϊ���RelHeight����HNode���č\�z�������Ɏg�p���Ă���LINKID
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool RebuldHeightNodeFromRelReight(const CHeightNodeAll& cBeforeHNodeAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, std::set<UNIQID>& setRebuildLinkId);

private:
	bool m_bJGD2000;                         //!< JGD2000�����邩�ǂ���
	CString m_strMeshCode;                   //!< �����Ώۃ��b�V���R�[�h
	check_mode::ECode m_CheckMode;           //!< ���������^�C�v
	double m_dMinSegment;                    //!< �����Z�O�����g��

	IWorkspacePtr m_ipBeforeShpWork;         //!< �����O�V�F�[�v�t�@�C���̃��[�N�X�y�[�X
	IWorkspacePtr m_ipAfterShpWork;          //!< ������V�F�[�v�t�@�C���̃��[�N�X�y�[�X

	IFeatureClassPtr m_ipBeforeHNodeClass;   //!< �����O�����m�[�h�N���X
	ITablePtr m_ipAfterRelHeightTable;       //!< �����㑊�΍����e�[�u��
	IFeatureClassPtr m_ipMeshClass;          //!< ���b�V���N���X

	CRWLAll m_cBeforeRWLAll;                 //!< �ϊ��ORWL�S�f�[�^
	CRWLAll m_cAfterRWLAll;                  //!< �ϊ���RWL�S�f�[�^
	CRWNAll m_cBeforeRWNAll;                 //!< �ϊ��ORWN�S�f�[�^
	CRWNAll m_cAfterRWNAll;                  //!< �ϊ���RWN�S�f�[�^
	CRSAAll m_cBeforeRSAAll;                 //!< �ϊ��ORSA�S�f�[�^
	CHeightNodeAll m_cBeforeHNodeAll;        //!< �ϊ��O���΍����S�f�[�^
	CRelHeightAll m_cAfterRelHeightAll;      //!< �ϊ��O���΍����S�f�[�^
};
