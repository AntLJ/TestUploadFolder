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
#include "AngleAccumulating.h"
#include "CurveInfoUtility.h"
#include <boost/utility.hpp>	// for prior/next

namespace curve_info
{
	/// �p�x�̗ݐςɊւ���T�O
	namespace angle_accumulating
	{
		using namespace utility;
		using namespace spatial_reference;

		void LRAccumulating::update( double cAngle, SegmentIt citCurrentSegment )
		{
			// �󋵊m�F
			bool aIsStraight = (fabs( cAngle ) < m_StraightAngle);
			curve_direction::ECode aDirection =
				aIsStraight ? curve_direction::straight :
				(0 < cAngle) ? curve_direction::left : curve_direction::right;

			// ���E�ɋȂ�������Ԃ���A����܂łƂ͈Ⴄ�����i�������܂ށj�ɂȂ�����I���Z�O�����g���L�^
			if (m_LeftAccumulating.isCurved() && aDirection != curve_direction::left)
			{
				if (! m_LeftAccumulating.hasEnded())
				{
					m_LeftAccumulating.endAt( boost::prior( citCurrentSegment ) );
				}
			}
			if (m_RightAccumulating.isCurved() && aDirection != curve_direction::right)
			{
				if (! m_RightAccumulating.hasEnded())
				{
					m_RightAccumulating.endAt( boost::prior( citCurrentSegment ) );
				}
			}

			if (! aIsStraight)
			{
				// ���ɋȂ����Ă���ꍇ
				if	(aDirection == curve_direction::left)
				{
					update( cAngle, citCurrentSegment, curve_direction::left );
				}
				// �E�ɋȂ����Ă���ꍇ
				else
				{
					update( cAngle, citCurrentSegment, curve_direction::right );
				}
			}
		}

		void LRAccumulating::update( const double cAngle, const SegmentIt citCurrentSegment, const curve_direction::ECode cDirection )
		{
			HRESULT hr;

			// ���E�ǂ��炩�Ȃ̂�O��Ƃ���
			if (cDirection != curve_direction::right && cDirection != curve_direction::left)
			{
				ATLASSERT(0);
				return;
			}

			Accumulating& rCurrentSideAcc = (cDirection == curve_direction::left) ? m_LeftAccumulating : m_RightAccumulating;
			Accumulating& rAnotherSideAcc = (cDirection == curve_direction::left) ? m_RightAccumulating : m_LeftAccumulating;

			// �ݐς��J�n���Ă��Ȃ���ΊJ�n����
			if (! rCurrentSideAcc.hasBegun())
			{
				rCurrentSideAcc.beginAt( boost::prior( citCurrentSegment ) );
			}

			// �p�x��ݐ�
			rCurrentSideAcc.addAngle( cAngle );
			m_CurrentDirection = cDirection;

			// �ݐϊp�x���u���������肷��p�x�v�𒴂�����J�[�u�̕��������肷��i�����Ă��Ȃ��Ȃ�I���j
			if (rCurrentSideAcc.m_Angle <= m_CurveCertifyAngle)
				{ return; }

			// �Ȃ����Ă�������̏I���Z�O�����g���������i�L�^����Ă��Ă��A�Ȃ��������ɂ���j
			rCurrentSideAcc.resetEnd();
			// �t�����̊J�n�Z�O�����g���������i�L�^����Ă��Ă��A�Ȃ��������ɂ���j
			rAnotherSideAcc.resetBegin();

			// ���ɋt�����̃J�[�u���ƔF�肳��Ă���Ȃ�A�ϋȓ_��ʂ�߂����ƌ��Ȃ��i�F�肳��Ă��Ȃ���ΏC���j
			if (rAnotherSideAcc.m_Angle <= m_CurveCertifyAngle)
				{ return; }

			// �ϋȓ_�̈ʒu������
			// ����̏I���Z�O�����g����A�����̊J�n�Z�O�����g�܂ł̊Ԃɂ���Z�O�����g�Q�̒��_��ϋȓ_�ƌ��Ȃ�
			SegmentIt itEndSegment = rAnotherSideAcc.m_EndSegment;
			SegmentIt itBeginSegment = rCurrentSideAcc.m_BeginSegment;
			IPointCollectionPtr ipPointCol = newGeometry< IPointCollectionPtr >( CLSID_Polyline, m_pSpRef );
			double aSegmentsLength = 0;		///< �Z�O�����g�Q�̒����i�x�j
#ifdef _DEBUG
			long aPointIndex = 0;
#endif
			for (SegmentIt itSegment = itEndSegment; itSegment != boost::next( itBeginSegment ); ++itSegment)
			{
#ifdef _DEBUG
				if (aPointIndex == 0)
				{
//								std::cout << std::setprecision( 20 ) << (*itSegment)->m_pFromPoint->GetX() << "\t" << (*itSegment)->m_pFromPoint->GetY() << "\t" << aPointIndex << std::endl;
					++aPointIndex;
				}
//							std::cout << std::setprecision( 20 ) << (*itSegment)->m_pToPoint->GetX() << "\t" << (*itSegment)->m_pToPoint->GetY() << "\t" << aPointIndex << std::endl;
				++aPointIndex;
#endif
				if (itSegment == itEndSegment)
				{
					hr = ipPointCol->AddPoint( (*itSegment)->m_pFromPoint );
					ATLASSERT( hr == S_OK );
				}
				hr = ipPointCol->AddPoint( (*itSegment)->m_pToPoint );
				ATLASSERT( hr == S_OK );

				aSegmentsLength += (*itSegment)->length();

				ATLASSERT( itSegment != m_rSingleRoad.end() );	// End��Begin�ŋ��܂ꂽ�̈悪����͂�
			}
			ICurvePtr ipCurve( ipPointCol );
			ATLASSERT( ipCurve );
			m_pInflectionPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
			hr = ipCurve->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, m_pInflectionPoint );
			ATLASSERT( hr == S_OK );
			ATLASSERT( m_pInflectionPoint );
			setSpatialReference( m_pInflectionPoint, m_pSpRef );

			// �ϋȓ_�Z�O�����g�����߂�
			double aSegmentsLengthHalf = aSegmentsLength / 2;	///< �Z�O�����g�Q�̒����̔����i�x�j
			double aPosOnSegments = 0;							///< �Z�O�����g�Q�̎n�_����̓��̂�i�x�j
			for (SegmentIt itSegment = itEndSegment; itSegment != boost::next( itBeginSegment ); ++itSegment)
			{
				aPosOnSegments += (*itSegment)->length();
				if (aSegmentsLengthHalf <= aPosOnSegments)
				{
					m_itInflectionSegment = itSegment;
					break;
				}
			}

			// �ϋȓ_�Ɋւ��鏈�����I�������A������O�̃J�[�u�̏��͖Y���
			rAnotherSideAcc.reset();
		}

	} // namespace angle_accumulating
} // namespace curve_info
