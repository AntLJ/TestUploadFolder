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

// SiNDYLib
#include <SiNDYLib/FieldMap.h>

#include "Arguments.h"
#include "ChildVerEditWorkspace.h"

class LinkagedLaneRoad;

/**
 * @brief   ���C���N���X
 */
class CAdamMaxSpeedUpdater
{
public:
	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	explicit CAdamMaxSpeedUpdater( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CAdamMaxSpeedUpdater(){};

public:
	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:
	/**
	* @brief   ���O������
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool initLog() const;

	/**
	* @brief   �e�[�u���̃I�[�v��
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool openTable();

	/**
	* @brief   ������
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool run();

	/**
	* @brief   �Ԑ������N�̐������x�R�[�h�i�ő�j���X�V����
	* @param[in]  linkagedLinks  �Ԑ������N�Ɠ��H�����N�̊֘A���Q
	* @param[in]  basemesh_no    �x�[�X���b�V���ԍ�(���O�o�͗p)
	* @retval true  �X�V����
	* @retval false �X�V���s
	*/
	bool updateMaxSpeed(LinkagedLaneRoad& linkagedLinks, long basemesh_no);

	/**
	* @brief   �Ԑ������N�Ɠ��H�����N�̍ō����x���r�����ʂ��o�͂���
	* @param[in]  linkagedLinks  �Ԑ������N�Ɠ��H�����N�̊֘A���Q
	* @param[in]  basemesh_no    �x�[�X���b�V���ԍ�(���O�o�͗p)
	*/
	void compareMaxSpeed(LinkagedLaneRoad& linkagedLinks, long basemesh_no);

	/**
	* @brief   �Ԑ������N�Ɠ��H�����N�̊֘A����ǂݍ���
	* @param[in]  basemesh_no          �x�[�X���b�V���ԍ�(���O�o�͗p)
	* @param[in]  where_clause         �Ԑ������N��������
	* @param[in]  exclude_group_code   �Ԑ������N���O�Ώۂ̃O���[�v���
	* @param[in, out] linkaged_links   �Ԑ������N�Ɠ��H�����N�̊֘A���
	* @retval true  �ǂݍ��ݐ���
	* @retval false �ǂݍ��ݎ��s
	*/
	bool readLinkagedLaneRoad(
		long basemesh_no, const CString& lane_where_clause, const std::list<long>& exclude_group_code, 
		LinkagedLaneRoad& linkaged_links
		);

	/**
	* @brief   ���H�����N�t�B�[�`�����w�肵�A�X�V����Ԑ������N�̐������x�R�[�h(�ő�)�̒l���擾����
	* @param[in]  road_link  ���H�����N�t�B�[�`��
	* @return  �������x�R�[�h(�ő�)�̒l
	*/
	long getUpdateMaxSpeedCode(const IFeaturePtr road_link) const;

	/**
	* @brief   �Ԑ������N�R���e�i����w��O���[�v��ʂ̃����N�����O����
	* @param[in]       exclude_codes  ���O�Ώۂ̃O���[�v���
	* @param[in, out]  lane_links     �Ԑ������N�Q
	* @retval true  �ǂݍ��ݐ���
	* @retval false �ǂݍ��ݎ��s
	*/
	bool excludeByGroupCode(const std::list<long>& exclude_codes, std::vector<IFeaturePtr>& lane_links) const;

	/**
	* @brief   �Ԑ������N::�������x�R�[�h(�ő�) �Ɠ��H�����N::�ō����x�R�[�h ���r����
	* @param[in]  lane_speed_c  �Ԑ������N::�������x�R�[�h(�ő�) 
	* @param[in]  road_speed_c  ���H�����N::�ō����x�R�[�h 
	* @retval  true  �������x�ł���
	* @retval  false �قȂ鑬�x�ł���
	*/
	bool isEquallMaxSpeed(long lane_speed_c, long road_speed_c) const;

	/**
	* @brief   �ҏW�J�n
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool startEdit();

	/**
	* @brief   �ҏW�I��
	*/
	void stopEdit();

	/**
	* @brief   �ҏW���f
	*/
	void abortEdit();

	/**
	* @brief   �㏈��
	* @param [in] isSucceeded ����I���������ۂ�
	*/
	void finish(bool isSucceeded) const;

	/**
	* @brief �q�o�[�W��������Ԃ�
	* @return �q�o�[�W������
	*/
	CString getChildVersionName() const;

private:
	Arguments m_args;      //!< ����

	ChildVerEditWorkspace m_workspace;		//!< �ҏW�Z�b�V��������

	ITablePtr m_laneLinkFc;					//!< �Ԑ������N�e�[�u��
	ITablePtr m_roadLinkFc;					//!< ���H�����N�e�[�u��
	ITablePtr m_basemeshTable;				//!< �x�[�X���b�V���e�[�u��
	ITablePtr m_relRoadLinkLaneLinkT;		//!< ���H�����N�|�Ԑ������N�֘A�e�[�u��
	ITablePtr m_laneLinkGroupT;				//!< �Ԑ������N�O���[�v�e�[�u��

	sindy::CFieldMap m_laneLinkFM;		//!< �Ԑ������N�̃t�B�[���h�}�b�v
	sindy::CFieldMap m_roadLinkFM;		//!< ���H�����N�̃t�B�[���h�}�b�v
	sindy::CFieldMap m_laneLinkGroupFM;	//!< �Ԑ������N�O���[�v�̃t�B�[���h�}�b�v
};

