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

// �ϊ���˕ϊ��O�`�F�b�N�p�N���X
class CCheckAfter2Before {
public:
	CCheckAfter2Before(check_mode::ECode CheckMode)
	{
		m_CheckMode = CheckMode;
	}
	~CCheckAfter2Before(){}

public:
	/**
	 * @brief �ϊ���˕ϊ��O�`�F�b�N
	 * @param cBeforeRWLAll         [in] �ϊ��ORWL
	 * @param cBeforeRWNAll         [in] �ϊ��ORWN
	 * @param cBeforeRSAAll         [in] �ϊ��ORSA
	 * @param cBeforeHNodeAll       [in] �ϊ��OHeightNode
	 * @param cAfterRWLAll          [in] �ϊ���RWL
	 * @param cAfterRWNAll          [in] �ϊ���RWN
	 * @param cAfterRelHeightAll    [in] �ϊ���RelHeight
	 * @param mapRebuildHN          [in] �č\�zHeightNode
	 * @param setReAfterRH          [in] �č\�zHeightNode�Ɏg�p�����LINKID���X�g
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll,
			const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setReAfterRH);

private:
	/**
	 * @brief RWL������˕����O�`�F�b�N
	 * @brief cBeforeRWLAll    [in] �ϊ��ORWL�f�[�^
	 * @brief cAfterRWLAll     [in] �ϊ���RWL�f�[�^
	 * @brief cBeforeRWNAll    [in] �ϊ��ORWN�f�[�^
	 * @brief cBeforeRSAAll    [in] �ϊ��ORSA�f�[�^
	 * @brief cBeforeHNodeAll  [in] �ϊ��OHeightNode�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckRWLAfter2Before(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief RWN������˕����O�`�F�b�N
	 * @brief cBeforeRWNAll    [in] �ϊ��ORWN�f�[�^
	 * @brief cAfterRWNAll     [in] �ϊ���RWN�f�[�^
	 * @brief cBeforeRSAAll    [in] �ϊ��ORSA�f�[�^
	 * @brief cBeforeHNodeAll  [in] �ϊ��OHeightNode�f�[�^
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckRWNAfter2Before(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief HN������˕����O�`�F�b�N
	 * @brief cBeforeHNodeAll     [in] �ϊ��OHNode�f�[�^
	 * @brief cAfterRelHeightAll  [in] �ϊ���RelHeight�f�[�^
	 * @brief mapRebuildHN        [in] �ϊ���RelHeight����HNode���č\�z��������
	 * @brief setReAfterRH        [in] �ϊ���RelHeight����HNode���č\�z�������Ɏg�p���Ă���LINKID
	 * @retval true  �������Ƀ`�F�b�N�G���[�Ȃ�
	 * @retval false �������Ƀ`�F�b�N�G���[����
	 */
	bool CheckHNAfter2Before(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setReAfterRH);

	/**
	 * @brief �`���v�m�F
	 * @note �`�F�b�N�Ώۂ̓O���[�v�P��
	 * @brief strTableName     [in] �ϊ���f�[�^�̃e�[�u�����i�G���[���b�Z�[�W�p�j
	 * @brief lOID             [in] �ϊ���f�[�^��OID�i�G���[���b�Z�[�W�p�j
	 * @brief lDivID           [in] �ϊ���f�[�^��DivID�i�G���[���b�Z�[�W�p�j
	 * @brief vecBeforeLonLat  [in] �ϊ��O�f�[�^�̃O���[�v�`��
	 * @brief vecAfterLonLat   [in] �ϊ���f�[�^�̃O���[�v�`��
	 * @brief cBeforeRSAAll    [in] �ϊ��ORSA�f�[�^
	 * @brief cBeforeHNodeAll  [in] �ϊ��O�����m�[�h�f�[�^
	 * @retval true  �}�b�`���Ă���
	 * @retval false �}�b�`���Ă��Ȃ�
	 */
	bool IsMatchGroupGeom(const CString& strTableName, long lOID, long lDivID, const std::vector<LONLAT>& vecBeforeLonLat, const std::vector<LONLAT>& vecAfterLonLat, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief �����N�������Ƒ����l���}�b�`���Ă��邩�`�F�b�N
	 * @brief cAfterRWLDiv       [in] �ϊ���RWL�f�[�^
	 * @brief lAfterRWLRoadAttr  [in] �ϊ���RWL��RoadAttr�����l
	 * @brief cBeforeRSAAll      [in] �ϊ��O�SRSA�f�[�^
	 * @brief lBeforeRWLRoadAttr [in] �ϊ��ORWL��RoadAttr�����l
	 * @retval true  �}�b�`���Ă���
	 * @retval false �}�b�`���Ă��Ȃ�
	 */
	bool IsMatchSegmentAttr(const CRWLDiv& cAfterRWLDiv, long lAfterRWLRoadAttr, const CRSAAll& cBeforeRSAAll, long& lBeforeRWLRoadAttr);

	/**
	 * @brief �}�[�W�Ώۃ����N�Ƒ����l���}�b�`���Ă��邩�`�F�b�N
	 * @brief cAfterRWLDiv       [in] �ϊ���RWL�f�[�^
	 * @brief cBeforeRWNAll      [in] �ϊ��O�SRWN�f�[�^
	 * @retval true  �}�b�`���Ă���
	 * @retval false �}�b�`���Ă��Ȃ�
	 */
	bool IsMatchMergeLink(const CRWLDiv& cAfterRWLDiv, const CRWLAll& cBeforeRWLAll);

	/**
	 * @brief �ϊ���˕ϊ��O�`�F�b�N�ł̃f�[�^���݃`�F�b�N
	 * @brief cAfterRWNAll       [in] �ϊ���SRWN�f�[�^
	 * @brief cBeforeRSAAll      [in] �ϊ��O�SRSA�f�[�^
	 * @brief cBeforeHNodeAll    [in] �ϊ��O�S�����m�[�h�f�[�^
	 * @brief cAfterRWLDiv       [in] �ϊ���RWL�f�[�^
	 * @retval true  �`�F�b�N�G���[����
	 * @retval false �`�F�b�N�G���[�Ȃ�
	 */
	bool CheckRWNExist(const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll, const CRWNDiv& cAfterRWNDiv);

	/**
	 * @brief �ϊ���˕ϊ��O�`�F�b�N�ł̃O���[�v�������`�F�b�N�p�֐�
	 * @brief cAfterRWLDivBase      [in] ��ƂȂ�ϊ���RWL�̃O���[�v���f�[�^
	 * @brief cAfterRWLDiv          [in] �ϊ���RWL�f�[�^
	 * @brief strAfterRWLTableName  [in] �ϊ���RWL�e�[�u����
	 * @brief strFieldName          [in] �`�F�b�N�Ώۂ̃t�B�[���h��
	 * @brief lIndex                [in] �`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @retval true  �`�F�b�N�G���[����
	 * @retval false �`�F�b�N�G���[�Ȃ�
	 */
	bool CheckRWLGroupAttr(const CRWLDiv& cAfterRWLDivBase, const CRWLDiv& cAfterRWLDiv, const CString& strAfterRWLTableName, const CString& strFieldName, const long lIndex);

private:
	check_mode::ECode m_CheckMode;           //!< ���������^�C�v
};
