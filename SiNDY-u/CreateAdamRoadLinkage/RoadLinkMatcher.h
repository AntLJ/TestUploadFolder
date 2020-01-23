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

#include <SiNDYLib/FieldMap.h>

/**
 * @brief WKSPoint��<���Z�q���I�[�o�[���[�h
 * @param lhs [in] ��r�Ώۂ̃|�C���g(����)
 * @param rhs [in] ��r�Ώۂ̃|�C���g(�E��)
 * @retval true ���ӂ̕���������
 * @retval false ���ӂ̕����������Ȃ�
 */
inline bool operator < (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X != rhs.X ? lhs.X < rhs.X : lhs.Y < rhs.Y;
}

/**
 * @brief WKSPoint��!=���Z�q���I�[�o�[���[�h
 * @param lhs [in] ��r�Ώۂ̃|�C���g(����)
 * @param rhs [in] ��r�Ώۂ̃|�C���g(�E��)
 * @retval true ���ӂ͈قȂ�
 * @retval false ���ӂ͓�����
 */
inline bool operator != (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X != rhs.X || lhs.Y != rhs.Y;
}

/**
 * @brief WKSPoint��!=���Z�q���I�[�o�[���[�h
 * @param lhs [in] ��r�Ώۂ̃|�C���g(����)
 * @param rhs [in] ��r�Ώۂ̃|�C���g(�E��)
 * @retval true ���ӂ͓�����
 * @retval false ���ӂ͈قȂ�
 */
inline bool operator == (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

typedef std::vector<WKSPoint> WKSPoints; //!< �_��̒�`

class RoadLinkMatcher
{
public:

	/**
	 * @brief ����
	 */
	enum Direction{
		kForword,	//!< ������
		kReverse,	//!< �t����
		kBoth		//!< ������
	};

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	RoadLinkMatcher(const ITablePtr& roadlink_t, double matching_range);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~RoadLinkMatcher(){}

public:
	/**
	 * @brief �w��t�B�[�`��(src_feature)�ƈʒu�I�Ɉ�ԃ}�b�`���̂��w��t�B�[�`���Q(dest_features)����擾����
	 * @note  �}�b�`����t�B�[�`����������Ȃ��ꍇ�� nullptr ��Ԃ�
	 * @param[in]  src_feature    �}�b�`���O���̃t�B�[�`��
	 * @param[in]  dest_features  �}�b�`���O���̃t�B�[�`���Q
	 * @param[out] is_reverse     �}�b�`���O���ꂽ���H�����N���t�����K���������Ă��邩�ǂ���
	 * @param[in]  as_src_is_reverse    �}�b�`���O���̃t�B�[�`�����t�����Ƃ��ă}�b�`���O���������{����
	 */
	IFeaturePtr getMatchLink(const IFeaturePtr& src_feature, const std::vector<IFeaturePtr>& dest_features, bool& is_reverse, bool as_src_is_reverse = false);

private:
	/**
	 * @brief �t�B�[�`���̍\���_��WKSPoint�̃x�N�^�ɂ��ĕԂ�
	 * @note  �t�B�[�`���̓|�����C���ł��邱�Ƃ�O��Ƃ���
	 * @param[in] feature  ���t�B�[�`��
	 * @param[in] reverse  �\���_�̕��я����t�ɂ��邩
	 */
	WKSPoints featureToWKSPoints(const IFeaturePtr& feature, bool reverse = false);

	/**
	 * @brief �_��̃}�b�`���O�����Z�o����
	 * @param[in] src_points ���H�����N�̓_��
	 * @param[in] dest_points  SIP�����N�Q�̓_��
	 * @retval DBL_MAX     �}�b�`���O���s
	 * @retval DBL_MAX�ȊO �}�b�`���O��
	 */
	double getMatchingRate(const WKSPoints& src_points, const WKSPoints& dest_points);

	/**
	 * @brief �_��̃}�b�`���O���x�����Z�o����
	 * @note �}�b�`���O���x���͗��_�񂪍ł��������鋗���B0��SIP�����N�Ɠ��H�����N�̏d�Ȃ�̈悪���S��v
	 * @param[in] linkPoints ���H�����N�̓_��
	 * @param[in] shpPoints  SIP�����N�Q�̓_��
	 * @param[in] coverRatio SIP�����N�񂪓��H�����N�𕢂��Ă���J�o�[��
	 */
	//double getMatchLevel( const WKSPoints& linkPoints, const WKSPoints& shpPoints, double coverRatio);

	/**
	 * @brief ���H�����N�t�B�[�`�����ǂ�����������K�����画�f����
	 * @note  ������feature �͓��H�����N�ł��邱�Ƃ�O��Ƃ���
	 * @param[in] feature  ���H�����N�t�B�[�`��
	 * @retval true  �t�����K���������Ă���
	 * @retval false �t�����K���������Ă��Ȃ�
	 */
	Direction getDirection( const IFeaturePtr& feature );

private:
	double m_MatchingRange;			//!< �}�b�`���O�͈�

	sindy::CFieldMap m_RoadLinkFM;	//!< ���H�����N�̃t�B�[���h�}�b�v
};

