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
	namespace circle_fit_evaluator
	{
		typedef long DistanceOf3Points;		///< �R�_�̊Ԋu�im�j
		typedef long FitCount;				///< �t�B�b�g�����_�̐�

		/**
		 * @brief �]������
		 * @note	����u�R�_�̊Ԋu�v��I�сA�~��`���ăt�B�b�e�B���O���Ă݂����ʁA���_�t�B�b�g�������Ƃ������B
		 *			CircleFitEvaluator�S�̂̌��ʂ͂���̏W���ɂȂ�B
		 */
		struct Result
		{
			IPointPtr		m_pTargetPoint;			///< �]���Ώۓ_
			long			m_DistanceOf3Points;	///< �R�_�̊Ԋu�im�j
			long			m_FitCount;				///< �~�`�󂪑Ώی`��ƃt�B�b�g�����_��
			double			m_ROC;					///< �ȗ����a�im�j
			mutable double	m_Score;				///< �X�R�A

			/// �R���X�g���N�^
			Result(
				IPointPtr cpTargetPoint,
				long cDistanceOf3Points,
				double cROC,
				long cFitCount
			) :
				m_pTargetPoint( cpTargetPoint ),
				m_DistanceOf3Points( cDistanceOf3Points ),
				m_ROC( cROC ),
				m_FitCount( cFitCount ),
				m_Score( -1 )
			{}

			/**
			 * @brief �X�R�A�i���̌��ʂ̕]���̍����j��Ԃ�
			 * @note	�u�t�B�b�g�����ȗ����a�Ŋ������l�v���X�R�A�Ƃ���B
			 *			�X�R�A���m���r���鎖�́A�u�t�B�b�g�����̈�̒������A�`�����~�̉~���̂����ǂꂾ���̊������߂邩�v���r���鎖�ɑ�������B
			 */
			double score() const
			{
				// ���Z�o�Ȃ�Z�o����
				if (m_Score == -1)
				{
					m_Score = m_FitCount / m_ROC;
				}
				return m_Score;
			}

		};
		typedef std::tr1::shared_ptr< Result > ResultPtr;
		typedef std::vector< ResultPtr > Results;
		typedef std::tr1::shared_ptr< Results > ResultsPtr;

		/**
		 * @brief �]���Ώۓ_��ʂ�~���A����̌`��Ƃǂꂾ���t�B�b�g���邩��]������N���X
		 * @note	�]���Ώۓ_�͑Ώی`��̏�ɂ���_�B
		 * @note	�]���Ώۓ_�͉~����̓_�ł��鎖��z��B�~����̓_�łȂ��ꍇ�́A���̓_�ɍł��߂��~����̓_���g�p�����B
		 * @note	DAC = Distance Along Curve�B�Ȑ��̎n�_����̓��̂�Bhttp://edndoc.esri.com/arcobjects/9.1/componenthelp/esrigeometry/ICurve_QueryPointAndDistance.htm
		 * @note	DACT = DAC on Target geometry�B�Ώی`���̓_�́A�Ώی`��̎n�_����̓��̂�B
		 * @note	DACC = DAC on Circle�B�~����̓_�́A�~�̎n�_����̓��̂�B
		 */
		class CircleFitEvaluator
		{
		public:
			typedef std::set< DistanceOf3Points > DistanceOf3PointsArray;	///< �u�R�_�̊Ԋu�v�̏W��

		private:
		// �O������^����ꂽ���
			const IPolycurve3Ptr m_pPolycurve;							///< �Ώی`��i�~�Ɣ�r����`��j
			IPointPtr m_pTargetPoint;									///< �]���Ώۓ_
			const double m_DACTofTargetPoint;							///< �Ώی`��̎n�_����]���Ώۓ_�܂ł̓��̂�
			const DistanceOf3PointsArray& m_rDistanceOf3PointsArray;	///< �]������u�R�_�̊Ԋu�v�̏W��
			const double m_FittingPointInterval;						///< �~�ʂ��Ώی`��Ƀt�B�b�g���邩��]������_�̊Ԋu�i�x�j
			const double m_FittingThreshold;							///< �~�ʂ��Ώی`��Ƀt�B�b�g����ƌ��Ȃ�臒l�i�x�j
			double m_DegreeToMeterCoef;									///< �����̒P�ʂ�x����m�ɕϊ����邽�߂Ɋ|����ׂ��W��
			std::ostream& m_rLogStream;									///< ���O�o�̓X�g���[��
			bool m_LogFitEvaluatingProcess;								///< �����ߒ������O�t�@�C���ɏo�͂��邩�ۂ�
			ISpatialReferencePtr m_pSpRef;								///< �g�p�����ԎQ�Ɓi�]���Ώۓ_����擾�j

		// ����
			typedef double ROC;										///< �ȗ����a�im�j
			double m_ROC;											///< �]���Ώۓ_�ɂ�����ȗ����a�im�j
			long m_DistanceOf3P;									///< �]���Ώۓ_�ɂ�����ȗ����a�����肵���ۂ̂R�_�̊Ԋu�im�j
			ResultsPtr m_pResults;									///< �]�����ʌQ

		public:
			/// �R���X�g���N�^
			CircleFitEvaluator(
				IPolycurve3Ptr cpPolycurve,
				IPointPtr cpTargetPoint,
				double cDACTofTargetPoint,
				const DistanceOf3PointsArray& cDistanceOf3PointsArray,
				double cFittingPointInterval,
				double cFittingThreshold,
				double cDegreeToMeterCoef,
				std::ostream& crStream,
				bool cLogFitEvaluatingProcess
			) :
				m_pPolycurve( cpPolycurve ),
				m_pTargetPoint( cpTargetPoint ),
				m_DACTofTargetPoint( cDACTofTargetPoint ),
				m_rDistanceOf3PointsArray( cDistanceOf3PointsArray ),
				m_FittingPointInterval( cFittingPointInterval ),
				m_FittingThreshold( cFittingThreshold ),
				m_DegreeToMeterCoef( cDegreeToMeterCoef ),
				m_rLogStream( crStream ),
				m_LogFitEvaluatingProcess( cLogFitEvaluatingProcess ),
				m_ROC( 0 ),
				m_DistanceOf3P( 0 )
			{
				m_pSpRef = m_pTargetPoint->GetSpatialReference();
			}

			/**
			 * @brief �]�����Č��ʂ�Ԃ�
			 * @return	�]�����ʌQ�i�]�����ׂ��_��1���Ȃ����NULL�j
			 */
			ResultsPtr evaluate();

			/// �]���Ώۓ_�ɂ�����ȗ����a��Ԃ�
			double roc() const { return m_ROC; }

			/// �ȗ����a�����肷��ۂɍ̗p���ꂽ�u�R�_�̊Ԋu�v��Ԃ�
			long distanceOf3P() const { return m_DistanceOf3P; }

		private:
			/**
			 * @brief �~��������Ԋu�Ői�݁A�Ώی`��Ƃ̊Ԃ̋������`�F�b�N���āA���ꂪ臒l�����ł���񐔁i�t�B�b�g�񐔁j��Ԃ�
			 * @note	IProximityOperator.QueryNearestPoint��esriExtendEmbedded���w�肷��Ɛ���ɓ��삵�Ȃ��̂�esriNoExtension���g�p���Ă���B
			 *			���̂��߁A�Ăяo��������͉~�ʂł͂Ȃ��~���^�����邱�Ƃ��O��B
			 * @note	cIsForward�̎w��ɉ����ĕБ������s����̂ŁA������t�B�b�g�񐔂͂��̂Q�񕪂����킹�ď��߂Ċ��S�ƂȂ�B
			 * @param[in]	cpCA				�~
			 * @param[in]	cDACCofTargetPoint	�~�̎n�_����]���Ώۓ_�܂ł̓��̂�i�x�j
			 * @param[in]	cDistanceOf3P		�R�_�̊Ԋu�im�j
			 * @param[in]	cIsForward			�i�ޕ����͉~�̏I�_�����ہi�n�_���j��
			 * @return	�t�B�b�g��
			 */
			long fitCount(
				const ICircularArcPtr cpCA,
				const double cDACCofTargetPoint,
				const long cDistanceOf3P,
				const bool cIsForward
			);

			/**
			 * @brief �]�����ʂ��R���e�i�ɒǉ�����
			 * @note	�]�����ʌQ�R���e�i���܂���������Ă��Ȃ���ΐ�������B
			 * @param[in]	cResult	�]������
			 */
			void addResult( const ResultPtr cpResult );
		};

		/**
		 * @brief �ړI�̕]�����ʂ�I�Ԃ��߂́A�]�����ʂ̔�r�p�v���f�B�P�[�g
		 * @note	�X�R�A�̍������̂�I�ԁB
		 *			�X�R�A�������Ȃ�A�ȗ����a�����������̂�I�ԁB
		 *			�ȗ����a�������Ȃ�A��ɒ��ڂ�������I�ԁB
		 */
		struct lessResult : public std::binary_function< const ResultPtr, const ResultPtr, bool >
		{
			bool operator() ( const ResultPtr r1, const ResultPtr r2 ) const
			{
				return
					r1->score()    != r2->score()    ? r1->score()    > r2->score()    :	// �X�R�A�̍~��
					r1->m_ROC      != r2->m_ROC      ? r1->m_ROC      < r2->m_ROC      :	// �ȗ����a�̏���
					                                 true;
			}
		};
		typedef std::set< const ResultPtr, lessResult > ResultsRanking;		// �]�����ʌQ�����]���ȏ��Ƀ\�[�g���邽�߂̃R���e�i

		/**
		 * @brief �w�肳�ꂽ�]�����ʌQ�̂����A�ł��]�����������̂�Ԃ�
		 * @note	�]���w�W�ɂ��Ă�lessResult�̃R�����g���Q�Ƃ̂��ƁB
		 * @param[in]	crResults		�]�����ʌQ
		 * @param[out]	cpLogStream		�]���ߒ����o�͂���X�g���[���iNULL�Ȃ�o�͂��Ȃ��j
		 * @return	�]������
		 */
		const ResultPtr bestFitResult( const Results& crResults, std::ostream* cpLogStream = NULL );

		/**
		 * @brief �]�����ʌQ���w�肳�ꂽ�X�g���[���Ƀ_���v����i�f�o�b�O�p�j
		 * @param[in]	crResults	�Ώە]�����ʌQ
		 * @param[in]	co			�o�̓X�g���[��
		 */
		void outputResults( const ResultsRanking& crResults, std::ostream& co );

	} // namespace circle_fit_evaluator
} // namespace curve_info
