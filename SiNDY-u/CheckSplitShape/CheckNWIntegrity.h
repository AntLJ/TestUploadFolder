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

// NW�������`�F�b�N�N���X
class CCheckNWIntegrity {
public:
	CCheckNWIntegrity(check_mode::ECode CheckMode, bool bJGD2000, double dMinSegment)
	{
		m_CheckMode = CheckMode;
		m_bJGD2000 = bJGD2000;
		m_dMinSegment = dMinSegment;
	}
	~CCheckNWIntegrity(){}

public:
	/**
	 * @brief NW�����`�F�b�N
	 * @param cAfterRWLAll         [in] �ϊ���RWL
	 * @param cAfterRWNAll         [in] �ϊ���RWN
	 * @param cAfterRelHeightAll   [in] �ϊ���RelHeight
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool run(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll);

private:
	/**
	 * @brief NW�������`�F�b�N�iRWL�p�j
	 * @note CheckNWIntegrity()����Ă΂��
	 * @brief cAfterRWLAll     [in] RWL�f�[�^
	 * @brief cAfterRWNAll     [in] RWN�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckNWRWLIntegrity(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief NW�������`�F�b�N�iRWN�p�j
	 * @note CheckNWIntegrity()����Ă΂��
	 * @brief cAfterRWLAll     [in] RWL�f�[�^
	 * @brief cAfterRWNAll     [in] RWN�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckNWRWNIntegrity(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief NW�������`�F�b�N�irel_height�p�j
	 * @note CheckNWIntegrity()����Ă΂��
	 * @brief cAfterHNodeAll   [in] ���΍����m�[�h�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckNWRelHeightIntegrity(const CRWLAll& cAfterRWLAll, const CRelHeightAll& cAfterRelHeightAll);

	/**
	 * @brief RWN�`��d���m�F
	 * @note ��{�I�Ɍ`��d�����Ă�����G���[�����A�����N�������������͓��ꏈ��
	 * @brief cLonLat       [in] RWN�̈ܓx�o�x���
	 * @brief cRWNAll       [in] �SRWL�f�[�^
	 * @retval true  �d�����Ă�
	 * @retval false �d�����Ă��Ȃ�
	 */
	bool IsRWNDuplicateGeom(const LONLAT& cLonLat, const CRWNAll& cRWNAll);

private:
	check_mode::ECode m_CheckMode;           //!< ���������^�C�v
	bool m_bJGD2000;                         //!< JGD2000�����邩�ǂ���
	double m_dMinSegment;                    //!< �����Z�O�����g��
};
