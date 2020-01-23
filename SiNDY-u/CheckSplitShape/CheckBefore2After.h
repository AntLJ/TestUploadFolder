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

// �ϊ��O�˕ϊ���`�F�b�N�p�N���X
class CCheckBefore2After {
public:
	CCheckBefore2After(check_mode::ECode CheckMode)
	{
		m_CheckMode = CheckMode;
	}
	~CCheckBefore2After(){}

public:
	/**
	 * @brief �ϊ��O�˕ϊ���`�F�b�N
	 * @param cBeforeRWLAll         [in] �ϊ��ORWL
	 * @param cBeforeRWNAll         [in] �ϊ��ORWN
	 * @param cBeforeHNodeAll       [in] �ϊ��OHeightNode
	 * @param cAfterRWLAll          [in] �ϊ���RWL
	 * @param cAfterRWNAll          [in] �ϊ���RWN
	 * @param cAfterRelHeightAll    [in] �ϊ���RelHeight
	 * @param mapRebuildHN          [in] �č\�zHeightNode
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CHeightNodeAll& cBeforeHNodeAll,
			const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>> mapRebuildHN);

private:
	/**
	 * @brief RWL�����O�˕�����`�F�b�N
	 * @brief cBeforeRWLAll    [in] �ϊ��ORWL�f�[�^
	 * @brief cAfterRWLAll     [in] �ϊ���RWL�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckRWLBefore2After(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll);

	/**
	 * @brief RWN�����O�˕�����`�F�b�N
	 * @brief cBeforeRWNAll    [in] �ϊ��ORWN�f�[�^
	 * @brief cAfterRWNAll     [in] �ϊ���RWN�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckRWNBefore2After(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief HN�����O��HN������`�F�b�N
	 * @brief cBeforeHNodeAll     [in] �ϊ��OHNode�f�[�^
	 * @brief cAfterRelHeightAll  [in] �ϊ���RelHeight�f�[�^
	 * @brief mapRebuildHN        [in] �ϊ���RelHeight����HNode���č\�z��������
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckHNBefore2After(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN);

	/**
	 * @brief �}�[�W�Ώ�RWL�擾
	 * @brief cBeforeRWLDiv        [in] �ϊ��ORWL�f�[�^
	 * @brief cAfterRWLAll         [in] �ϊ���SRWL�f�[�^
	 * @brief mapAfterMergeRWLDiv  [out] �}�[�W�Ώ�RWL�f�[�^
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool GetAfterMergeRWL(const CRWLDiv& cBeforeRWLDiv, const CRWLAll& cAfterRWLAll, std::map<UNIQID,CRWLDiv>& mapAfterMergeRWLDiv);

private:
	check_mode::ECode m_CheckMode;           //!< ���������^�C�v
};
