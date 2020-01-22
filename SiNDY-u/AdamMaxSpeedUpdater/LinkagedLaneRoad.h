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

/**
 *  @brief  �Ԑ������N�Ɠ��H�����N�̃y�A�̑Ó��x
 *          �Ԑ������N�ɕR�Â����H�����N����������ꍇ������A
 *          ���̏ꍇ�͍ō����x�̎擾���Ƃ��đ��������y�A��1���߂�
 *          �����ł͌��߂�ꂽ�y�A�̑Ó������`����
 */
enum LinkageAccuracy
{
	kNoPair,			//!< �R�Â����H�����N���Ȃ�
	kAccurate,			//!< �y�A���ō����x�̎擾���Ƃ��Đ��m�ł���
	kMostlyAccurate,	//!< �y�A���ō����x�̎擾���Ƃ��Đ��m�ł���(�������擾���̌�₪2�ȏ゠����)
	kInaccurate,		//!< �y�A���ō����x�̎擾���Ƃ��ĕs���m�ȉ\��������
};

/**
*  @brief  �Ԑ������N�Ɠ��H�����N�̕R�t�����Ǘ��N���X
*  @detail �Ԑ������N�Ƃ���ɕR�Â����H�����N�Ƃ̑Ή��Ǘ����s��
*          �Ԑ������N�ɕ����̓��H�����N���R�Â��ꍇ�ɁA
*          �ō����x�̍X�V���Ƃ��Ăǂ̓��H�����N��I�����邩�̌�������̃N���X�ōs��
*/
class LinkagedLaneRoad
{
public:
	/**
	*  @brief   �Ԑ������N�Ɠ��H�����N�̕R�t�����
	*  @detail  �Ԑ������N �� ���H�����N = 1 �� ��
	*/
	struct Record
	{
		IFeaturePtr m_laneLink;		//!< �Ԑ������N
		std::vector<IFeaturePtr> m_roadLinks;	//!< �Ԑ������N�ɕR�Â����H�����N
	};
	typedef std::vector<Record>::iterator Iter;

public:
	/**
	*  @brief  �R���X�g���N�^
	*  @param[in]  lane_fc        �Ԑ������N�t�B�[�`���N���X
	*  @param[in]  road_fc        ���H�����N�t�B�[�`���N���X
	*  @param[in]  rel_road_lane  ���H�����N�|�Ԑ������N�֘A�e�[�u��
	*/
	LinkagedLaneRoad(IFeatureClassPtr lane_fc, IFeatureClassPtr road_fc, ITablePtr rel_road_lane);
	virtual ~LinkagedLaneRoad() {};

public:
	Iter begin() { return m_data.begin(); }
	Iter end() { return m_data.end(); }

	/**
	*  @brief      �f�[�^�ǂݍ���
	*  @detail     �����Ŏw�肵���Ԑ������N�t�B�[�`���ɕR�Â����H�����N��ǂݍ��݁A
	*              �����ŕێ�����R���e�i��"�Ԑ������N->���H�����N"�̑Ή�
	*  @param[in]  lane_links  �Ԑ������N�t�B�[�`���Q
	*  @retval true  �ǂݍ��ݐ���
	*  @retval false �ǂݍ��ݎ��s
	*/
	bool read(const std::vector<IFeaturePtr>& lane_links);

	/**
	*  @brief   �Ԑ������N�ɕR�Â����H�����N�̂����D��x�̍������H�����N��I��ŕԂ�
	*  @detail  �������N�̎�ʂ���ō����x�̍X�V���Ƃ��Ă̗D��x�̍������H�����N�����߂�
	*  @param[in]  record �Ԑ������N1���̓��H�����N�R�t�����
	*  @param[out] lane_link �Ԑ������N
	*  @param[out] road_link ���H�����N
	*  @return  �ō����x�X�V�ɂ�����R�t���̑Ó��x
	*/
	LinkageAccuracy getHigherPriorityPairLink(const Record& record, IFeaturePtr& lane_link, IFeaturePtr& road_link) const;

private:
	/**
	*  @brief   �Ԑ������N�Ɠ��H�����N�̑g�ݍ��킹�̗D��x��Ԃ�
	*  @detail  �������N�̎�ʂ���ō����x�̍X�V���Ƃ��Ă̗D��x�̍����g�ݍ��킹�ł��邩�𔻒f����
	*           �������������قǗD��x�͍���
	*  @param[in] lane_link  �Ԑ������N
	*  @param[in] road_link  ���H�����N
	*  @retval  �Ԑ������N�Ɠ��H�����N�̑g�ݍ��킹�̗D��x
	*/
	long getPriority(IFeaturePtr lane_link, IFeaturePtr road_link) const;

	/**
	*  @brief   �w�肵���Ԑ������N�̑������ō����x���X�V����ɂ������Đ��m���̒Ⴂ���̂ł��邩��Ԃ�
	*  @paran[in]  lane_link  �Ԑ������N
	*  @retval true   ���m�����Ⴂ
	*  @retval false  ���m�����Ⴍ�Ȃ�
	*/
	bool isInaccuracyPair(IFeaturePtr lane_link) const;

private:
	IFeatureClassPtr m_roadLinkFc;		//!< ���H�����N�t�B�[�`���N���X
	ITablePtr        m_relRoadLaneT;	//!< ���H�����N-�Ԑ������N�֘A�e�[�u��

	sindy::CFieldMap m_relRoadLaneFM;	//!< ���H�����N�|�Ԑ������N�֘A�e�[�u���̃t�B�[���h�}�b�v
	sindy::CFieldMap m_lanelinkFM;		//!< �Ԑ������N�t�B�[�`���N���X�̃t�B�[���h�}�b�v
	sindy::CFieldMap m_roadlinkFM;		//!< ���H�����N�t�B�[�`���N���X�̃t�B�[���h�}�b�v

	std::vector<Record> m_data;		//!< �Ԑ������N�Ɠ��H�����N�̕R�t�����
};

