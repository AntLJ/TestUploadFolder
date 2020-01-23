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

namespace curve_info
{
	/**
	 * @brief �o�͂Ɋւ���I�v�V����
	 */
	// TODO: �l��̃`�F�b�N
	//	�Em_StraightAngle��0�܂��͐��̎����łȂ���΂Ȃ�Ȃ�
	//�@�Em_ROCPointInterval�Am_FittingPointInterval�͐��̒l�łȂ���΂Ȃ�Ȃ�
	//	�Ȃ�
	struct OutputOptions
	{
		long m_MaxROCForRegionDeletion;				///< �̈�폜�̂��߂̍ő�ȗ����a�i�ᑬ��p�j(m)
		long m_MaxROCForRegionDeletionHigh;			///< �̈�폜�̂��߂̍ő�ȗ����a�i������p�j(m)
		long m_MaxROCOfSection;						///< ��Ԃ̍ő�ȗ����a�i�ᑬ��p�j(m)
		long m_MaxROCOfSectionHigh;					///< ��Ԃ̍ő�ȗ����a�i������p�j(m)
		double m_StraightAngle;						///< �����ƌ��Ȃ��p�x�i�x�j�@TODO: �s�v����
		double m_CurveCertifyAngle;					///< �J�[�u�F��p�x�i�x�j
		long m_ROCPointInterval;					///< �ȗ����a�����߂�_�̊Ԋu(m)
		long m_FittingPointInterval;				///< �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g���邩��]������_�̊Ԋu(m)
		long m_FittingThreshold;					///< �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g����ƌ��Ȃ�臒l(m)
		double m_SectionMinAngle;					///< ��ԂɑΉ�����~�ʂ̍ŏ��p�x�i�x�j
		bool m_CreateEachMesh;						///< ���b�V�����Ƃ�Shape���쐬���邩�ۂ�
		bool m_SHP05DirConfig;						///< SHP05�̃f�B���N�g���\���ŏo�͂��邩�ۂ�
		bool m_OutputVerificationLog;				///< ���ؗp���O���쐬���邩�ۂ�
		bool m_LogHandlingProcess;					///< �����ߒ������O�t�@�C���ɏo�͂��邩�ۂ�
		bool m_LogFitEvaluatingProcess;				///< �t�B�b�g�]���ߒ������O�t�@�C���ɏo�͂���
		CString m_QueryWhereClause;					///< �Ώۂ̓��H�����N�����肷�邽�߂̃N�G��Where��

		/// �R���X�g���N�^
		OutputOptions::OutputOptions(
			long cMaxROCForRegionDeletion,
			long cMaxROCForRegionDeletionHigh,
			long cMaxROCOfSection,
			long cMaxROCOfSectionHigh,
			double cCurveCertifyAngle,
			double cStraightAngle,
			long cROCPointInterval,
			long cFittingPointInterval,
			long cFittingThreshold,
			double cSectionMinAngle
		) :
			m_MaxROCForRegionDeletion( cMaxROCForRegionDeletion ),
			m_MaxROCForRegionDeletionHigh( cMaxROCForRegionDeletionHigh ),
			m_MaxROCOfSection( cMaxROCOfSection ),
			m_MaxROCOfSectionHigh( cMaxROCOfSectionHigh ),
			m_CurveCertifyAngle( cCurveCertifyAngle ),
			m_StraightAngle( cStraightAngle ),
			m_ROCPointInterval( cROCPointInterval ),
			m_FittingPointInterval( cFittingPointInterval ),
			m_FittingThreshold( cFittingThreshold ),
			m_SectionMinAngle( cSectionMinAngle ),
			m_QueryWhereClause( CString() ),
			m_CreateEachMesh( false ),					// �f�t�H���g�ł͎w�胁�b�V���S���܂Ƃ߂�Shape���쐬����
			m_SHP05DirConfig( false ),
			m_OutputVerificationLog( false ),			// �f�t�H���g�ł͍쐬���Ȃ�
			m_LogHandlingProcess( false ),
			m_LogFitEvaluatingProcess( false )
		{
		}

		/**
		 * @brief �����悩�ۂ��ɉ����ēK�؂ȁu�̈�폜�̂��߂̍ő�ȗ����a�v��Ԃ�
		 * @param[in]	cIsHighSpeedArea	�����悩�ۂ�
		 * @return	�ő�ȗ����a
		 */
		long maxROCForRegionDeletion( bool cIsHighSpeedArea ) const
		{
			return cIsHighSpeedArea ? m_MaxROCForRegionDeletionHigh : m_MaxROCForRegionDeletion;
		}

		/**
		 * @brief �����悩�ۂ��ɉ����ēK�؂ȁu��Ԃ̍ő�ȗ����a�v��Ԃ�
		 * @param[in]	cIsHighSpeedArea	�����悩�ۂ�
		 * @return	�ő�ȗ����a
		 */
		long maxROCOfSection( bool cIsHighSpeedArea ) const
		{
			return cIsHighSpeedArea ? m_MaxROCOfSectionHigh : m_MaxROCOfSection;
		}
	};
} // namespace curve_info
