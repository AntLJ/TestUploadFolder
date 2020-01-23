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

#include "StdAfx.h"
#include "CircleFitEvaluator.h"
#include "CurveInfoUtility.h"
#include <iomanip>

namespace curve_info
{
	namespace circle_fit_evaluator
	{
		using namespace utility;
		using namespace spatial_reference;

		ResultsPtr CircleFitEvaluator::evaluate()
		{
			ATLASSERT( m_pTargetPoint );
			IPointPtr ipPoint2 = m_pTargetPoint;	///< �]���Ώۓ_���A�R�_�̂����̂Q�_��

			const double aDACT2 = m_DACTofTargetPoint;

			// �u�R�_�̊Ԋu�v�̂��ꂼ��ɂ���
			BOOST_FOREACH (const long aDistanceOf3P, m_rDistanceOf3PointsArray)
			{
				HRESULT hr;

				// ��P�_�Ƒ�R�_�̈ʒu������
				IPointPtr ipPoint1 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				IPointPtr ipPoint3 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				double aDACT1 = aDACT2 - aDistanceOf3P / m_DegreeToMeterCoef;		///< �R�_�̓��̑�P�_�́A�Ώی`�󂩂�̓��̂�
				double aDACT3 = aDACT2 + aDistanceOf3P / m_DegreeToMeterCoef;		///< �R�_�̓��̑�R�_�́A�Ώی`��̎n�_����̓��̂�
				hr = m_pPolycurve->QueryPoint( esriNoExtension, aDACT1, VARIANT_FALSE, ipPoint1 );
				ATLASSERT( hr == S_OK );
				hr = m_pPolycurve->QueryPoint( esriNoExtension, aDACT3, VARIANT_FALSE, ipPoint3 );
				ATLASSERT( hr == S_OK );
				ATLASSERT( ipPoint1 && ipPoint3 );
				setSpatialReference( ipPoint1, m_pSpRef );
				setSpatialReference( ipPoint3, m_pSpRef );

				// ��P�_�܂��͑�R�_���Ώی`��͈̔͊O�Ȃ�X�L�b�v
				// TODO: �����ŋ͂��ɔ͈͊O�ɏo�ĕ]������Ȃ��ꍇ�������B
				if (aDACT1 <= 0 || m_pPolycurve->Length <= aDACT3)
					{ continue; }

				// �R�_��ʂ�~�ʂ����߂�
				ICircularArcPtr ipCA = newGeometry< ICircularArcPtr >( CLSID_CircularArc, m_pSpRef );
				IConstructCircularArcPtr ipCCA( ipCA );
				hr = ipCCA->ConstructThreePoints( ipPoint1, ipPoint2, ipPoint3, VARIANT_FALSE );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCA, m_pSpRef );
				// �~�ʂ����S�ȉ~�ɂ���
				IPointPtr ipCenterPoint( ipCA->GetCenterPoint() );
				// ���S�_�������Ȃ���΃X�L�b�v
					// �R�_���꒼����ɂ��鎞�A�x�W�G�Ȑ��Q�͐����ƂȂ�AICircularArc.GetCenterPoint()�����s����B
					// �i��������IConstructCircularArcPtr.ConstructThreePoints()�Ŏ��s���ׂ��ȋC�����邪�c�B�j
					// TODO: �u�Z�O�����g�Q�������i����؋Ȃ����Ă��Ȃ��j���v�͂����Ƒ����i�K�Ń`�F�b�N���ׂ����H
				if (! ipCenterPoint)
					{ continue; }

				hr = ipCCA->ConstructCircle( ipCenterPoint, ipCA->GetRadius(), VARIANT_FALSE );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCA, m_pSpRef );
				ATLASSERT( ipCA->IsClosed == VARIANT_TRUE );	// �~�ɂȂ������̊m�F
				// ���F�~�̎n�_��0�������̓_�ɂȂ�B http://edndoc.esri.com/arcobjects/9.1/componenthelp/esrigeometry/IConstructCircularArc_ConstructCircle.htm

				// ������~������������Ȃ�]���ΏۊO�Ƃ���iBug9737�j
				// �u�~�ʂ��Ώی`��Ƀt�B�b�g���邩��]������_�̊Ԋu�v���~���̔����ȏ�Ȃ�A���̉~�͕]���ΏۊO
				if ( ipCA->GetLength() <= m_FittingPointInterval * 2 )
					{ continue; }

				// �~�̎n�_�����Q�_�܂ł̓��̂�����߂�
				double aDACCofP2 = 0;						///< �~�̎n�_�����Q�_�܂ł̓��̂�
				{
					double aDFCCofP2 = 0;						// �g�p���Ȃ�
					VARIANT_BOOL aRightSide = VARIANT_FALSE;	// �g�p���Ȃ�
					hr = ipCA->QueryPointAndDistance( esriNoExtension, ipPoint2, VARIANT_FALSE, IPointPtr( CLSID_Point ), &aDACCofP2, &aDFCCofP2, &aRightSide );
					ATLASSERT( hr == S_OK );
				}

				// �~��������Ԋu�Ői�݁A�x�W�G�Ȑ��Q�Ƃ̋������`�F�b�N���Ă���
				long aFitCount = 0;
				aFitCount += fitCount( ipCA, aDACCofP2, aDistanceOf3P, true );		// �������i�~�̏I�_���j
				aFitCount += fitCount( ipCA, aDACCofP2, aDistanceOf3P, false );		// �t�����i�~�̎n�_���j

				// ���ʂ��L�^
				addResult( ResultPtr( new Result(
					m_pTargetPoint,
					aDistanceOf3P,
					ipCA->GetRadius() * m_DegreeToMeterCoef,
					aFitCount
				) ) );
			}

			return m_pResults;
		}

		long CircleFitEvaluator::fitCount(
			const ICircularArcPtr cpCA,
			const double cDACCofTargetPoint,
			const long cDistanceOf3P,
			const bool cIsForward
		)
		{
			HRESULT hr;

			long aCircleIndex = 0;											///< �~����̓��Ԋu�ȓ_�Q�̃C���f�b�N�X
			double aDistanceFromP2 = 0;										///< ��Q�_����~����̓_�܂ł̓��̂�
			double aDistanceFromP2Max = M_PI * cpCA->GetRadius() / 2;		///< aDistanceFromP2�̍ő�l�i�~����1/4�j
			long aFitCount = 0;
			for (; aDistanceFromP2 < aDistanceFromP2Max; ++aCircleIndex)
			{
				aDistanceFromP2 = m_FittingPointInterval * aCircleIndex;
				
				// �ŏ��̓_�͏������̏ꍇ�̂ݕ]������
				if (aDistanceFromP2 == 0 && ! cIsForward)
					{ continue; }

				// �~����̓_������
				IPointPtr ipCirclePoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				double aDAC = cIsForward ?						///< �~�̎n�_����ipPoint�܂ł̓��̂�
					cDACCofTargetPoint + aDistanceFromP2 :
					cDACCofTargetPoint - aDistanceFromP2;
				hr = cpCA->QueryPoint( esriExtendEmbedded, aDAC, VARIANT_FALSE, ipCirclePoint );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCirclePoint, m_pSpRef );

				// �Ώی`��܂ł̋��������߂�
				IProximityOperatorPtr ipPOpe( m_pPolycurve );
				IPointPtr ipCurvePoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				hr = ipPOpe->QueryNearestPoint( ipCirclePoint, esriNoExtension, ipCurvePoint );
				ATLASSERT( hr == S_OK );
				setSpatialReference( m_pPolycurve, m_pSpRef );
				setSpatialReference( ipCurvePoint, m_pSpRef );
				setSpatialReference( ipCirclePoint, m_pSpRef );

				bool aFit = false;	///< �t�B�b�g�������ۂ�

				// �u�~����̓_�ɍł��߂��Ώی`���̓_�v���Ώی`��̒[�_�ł���ꍇ�̓t�B�b�g���Ȃ������ƌ��Ȃ�
				if (ipCurvePoint->_Compare( m_pPolycurve->GetFromPoint() ) != 0 && ipCurvePoint->_Compare( m_pPolycurve->GetToPoint() ) != 0)
				{
					// �[�_�łȂ���΁A�����Ŕ���
					// TODO: �����ɂ�crd_cnv�Ōv�Z���ׂ��H
					double aDistanceToTargetGeom = distance(
						ipCirclePoint->GetX(),	ipCirclePoint->GetY(),
						ipCurvePoint->GetX(),	ipCurvePoint->GetY()
					);

					aFit = ( aDistanceToTargetGeom < m_FittingThreshold );
				}

				// �K�v�Ȃ�t�B�b�g�]���_�̏������O�ɏo��
				if (m_LogFitEvaluatingProcess)
				{
					m_rLogStream << utility::log_format::pointRecord( ipCirclePoint->GetX(), ipCirclePoint->GetY(), aFit ? "�t�B�b�g�]���_�i�t�B�b�g�����j" : "�t�B�b�g�]���_�i�t�B�b�g���Ȃ������j" );
				}

				// ������臒l�����Ȃ�J�E���g���Z
				if (aFit)
				{
					++aFitCount;
				}
				// ������臒l�ȏ�Ȃ炻�̐�̓t�B�b�g���Ȃ��ƌ��Ȃ��i�������̂��߁j
				// TODO: ������܂ł͗l�q�����Ă����������B10�_���čăt�B�b�g���Ȃ���ΏI���Ƃ��B
				else
				{
					break;
				}
			}

			return aFitCount;
		}

		void CircleFitEvaluator::addResult( const ResultPtr cpResult )
		{
			// �]�����ʌQ�R���e�i���܂���������Ă��Ȃ���ΐ�������
			if (! m_pResults)
			{
				m_pResults = ResultsPtr( new Results() );
			}

			m_pResults->push_back( cpResult );
		}

		const ResultPtr bestFitResult( const Results& crResults, std::ostream* cpLogStream /*= NULL*/ )
		{
			ResultsRanking aResultsRanking;
			std::copy( crResults.begin(), crResults.end(), std::inserter( aResultsRanking, aResultsRanking.begin() ) );

			// �K�v�Ȃ�]���ߒ������O�ɏo��
			if (cpLogStream)
			{
				outputResults( aResultsRanking, *cpLogStream );
			}

			return *aResultsRanking.begin();
		}

		void outputResults( const ResultsRanking& crResults, std::ostream& co )
		{
			BOOST_FOREACH (const ResultPtr pResult, crResults)
			{
				ATLASSERT( pResult );
				const Result& rResult = *pResult;
				std::stringstream ss;
				ss  << std::setprecision( 20 )
					<< "�R�_�̊Ԋu=" << rResult.m_DistanceOf3Points << ", "
					<< "�ȗ����a=" << rResult.m_ROC << ", "
					<< "�t�B�b�g��=" << rResult.m_FitCount << ", "
					<< "�X�R�A=" << rResult.score();
				co << utility::log_format::pointRecord( rResult.m_pTargetPoint->GetX(), rResult.m_pTargetPoint->GetY(), ss.str() );
			}
		}

	} // namespace circle_fit_evaluator
} // namespace curve_info
