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

#include "CurveInfoData.h"

namespace curve_info
{
	/**
	 * @brief �p�x�̗ݐςɊւ���T�O
	 * @note	�Z�O�����g������ɒH��ߒ��œ�����u�e�Z�O�����g�΂̂Ȃ��p�̊p�x�v�̏��Q�������B
	 */
	namespace angle_accumulating
	{
		/**
		 * @brief �J�[�u�̕���
		 */
		namespace  curve_direction
		{
			enum ECode
			{
				straight,	///< ���i
				left,		///< ��
				right,		///< �E
				both,		///< ���E����
			};
		}

		/**
		 * @brief �p�x�ݐϏ��
		 */
		struct Accumulating
		{
			const SingleRoad& m_rSingleRoad;	///< �Ώۈ�{��
			SegmentIt m_BeginSegment;			///< �ݐϊJ�n�Z�O�����g
			SegmentIt m_EndSegment;				///< �ݐϏI���Z�O�����g
			double m_Angle;						///< �ݐϊp�x

			const double m_CurveCertifyAngle;			///< �J�[�u�F��p�x�i�x�j

			/**
			 * @brief �R���X�g���N�^
			 * @param[in]	crSingleRoad	�Ώۈ�{��
			 */
			Accumulating(
				const SingleRoad& crSingleRoad,
				double cDirectionAngle
			) :
				m_rSingleRoad( crSingleRoad ),
				m_EndSegment( crSingleRoad.end() ),
				m_CurveCertifyAngle( cDirectionAngle )
			{
				reset();
			}

			/**
			 * @brief �p�x�̗ݐς��J�n�ς݂ł��邩�ۂ��𔻒肷��
			 * @return	���茋��
			 */
			bool hasBegun() const
			{
				return m_BeginSegment != m_rSingleRoad.end();
			}

			/**
			 * @brief �p�x�̗ݐς��J�n�����Z�O�����g���L�^����
			 * @param[in]	citSegment	�J�n�Z�O�����g
			 */
			void beginAt( const SegmentIt citSegment )
			{
				m_BeginSegment = citSegment;
			}

			/**
			 * @brief �J�n�Z�O�����g��������Ԃɖ߂�
			 */
			void resetBegin()
			{
				m_BeginSegment = m_rSingleRoad.end();
			}

			/**
			 * @brief �p�x�̗ݐς��I���ς݂ł��邩�ۂ��𔻒肷��
			 * @return	���茋��
			 */
			bool hasEnded() const
			{
				return m_EndSegment != m_rSingleRoad.end();
			}

			/**
			 * @brief �p�x�̗ݐς��I�������Z�O�����g���L�^����
			 * @param[in]	citSegment	�I���Z�O�����g
			 */
			void endAt( SegmentIt citSegment )
			{
				m_EndSegment = citSegment;
			}

			/**
			 * @brief �I���Z�O�����g��������Ԃɖ߂�
			 */
			void resetEnd()
			{
				m_EndSegment = m_rSingleRoad.end();
			}

			/**
			 * @brief �p�x�𑫂�
			 * @param[in]	cAngle	�p�x
			 */
			void addAngle( double cAngle )
			{
				m_Angle += fabs( cAngle );
			}

			/**
			 * @brief ������Ԃɖ߂�
			 */
			void reset()
			{
				m_BeginSegment = m_rSingleRoad.end();
				m_EndSegment = m_rSingleRoad.end();
				m_Angle = 0;
			}

			/**
			 * @brief ���ɃJ�[�u�ƔF�肳��Ă��邩�ۂ��𔻒肷��
			 * @return	���茋��
			 */
			bool isCurved()
			{
				return m_CurveCertifyAngle <= m_Angle;
			}
		};

		/**
		 * @brief ���E�̊p�x�ݐϏ��
		 */
		struct LRAccumulating
		{
			Accumulating m_LeftAccumulating;	///< �����p�x�ݐϏ��
			Accumulating m_RightAccumulating;	///< �E���p�x�ݐϏ��

			const SingleRoad& m_rSingleRoad;	///< �Ώۈ�{��

			double m_StraightAngle;				///< �����ƌ��Ȃ��p�x
			double m_CurveCertifyAngle;			///< �J�[�u�F��p�x�i�x�j

			ISpatialReferencePtr m_pSpRef;	///< �g�p�����ԎQ��

			// TODO: �\���̂ɂ���H "InflectionPointInfo"�Ƃ��B
			SegmentIt m_itInflectionSegment;	///< �ϋȓ_�Z�O�����g�i���̏�ɕϋȓ_�����݂���Z�O�����g�j
			IPointPtr m_pInflectionPoint;		///< �Ō�Ɍ��������ϋȓ_�i�������Ȃ�NULL�j

			curve_direction::ECode m_CurrentDirection;	///< ���݂ǂ���ɋȂ����Ă��邩�A�������͒�����

			/**
			 * @brief �R���X�g���N�^
			 * @param[in]	cSingleRoad		�Ώۈ�{��
			 * @param[in]	cStraightAngle	�����ƌ��Ȃ��p�x
			 * @param[in]	cDirectionAngle	���������肷��p�x�i�x�j
			 * @param[in]	cpSpRef			�g�p�����ԎQ��
			 */
			LRAccumulating(
				const SingleRoad& cSingleRoad,
				double cStraightAngle,
				double cDirectionAngle,
				ISpatialReferencePtr cpSpRef
			) : 
				m_LeftAccumulating( cSingleRoad, cDirectionAngle ),
				m_RightAccumulating( cSingleRoad, cDirectionAngle ),
				m_rSingleRoad( cSingleRoad ),
				m_StraightAngle( cStraightAngle ),
				m_CurveCertifyAngle( cDirectionAngle ),
				m_pSpRef( cpSpRef ),
				m_CurrentDirection( curve_direction::straight )
			{
				reset();
			}

			/**
			 * @brief �p�x�ݐϏ�Ԃ��X�V
			 * @param[in]	cAngle				�p�x
			 * @param[in]	citCurrentSegment	���ڒ��̃Z�O�����g
			 */
			void update( double cAngle, SegmentIt citCurrentSegment );

			/**
			 * @brief �p�x�ݐϏ�Ԃ��X�V�i���E�̋��ʕ����j
			 * @param[in]	cAngle				�p�x
			 * @param[in]	citCurrentSegment	���ڒ��̃Z�O�����g
			 * @param[in]	cDirection			����
			 */
			void update( const double cAngle, const SegmentIt citCurrentSegment, const curve_direction::ECode cDirection );

			/**
			 * @brief �p�x�ݐϏ�Ԃ�������Ԃɖ߂�
			 */
			void reset()
			{
				m_LeftAccumulating.reset();
				m_RightAccumulating.reset();
				m_itInflectionSegment = m_rSingleRoad.end();
				m_pInflectionPoint = NULL;
			}

			/**
			 * @brief �p�x���u�J�[�u�F��p�x�v�ȏ�ɗݐς��Ă��������Ԃ�
			 * @note	�ʏ�Aboth�ɂ͂Ȃ�Ȃ��z��B
			 * @return ����
			 */
			curve_direction::ECode direction()
			{
				bool left = m_CurveCertifyAngle < m_LeftAccumulating.m_Angle;
				bool right = m_CurveCertifyAngle < m_RightAccumulating.m_Angle;

				if      (left && right) { return curve_direction::both; }
				else if (left)          { return curve_direction::left; }
				else if (right)         { return curve_direction::right; }
				else                    { return curve_direction::straight; }
			}
		};
	} // namespace angle_accumulating
} // namespace curve_info
