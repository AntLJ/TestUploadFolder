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
#include "CurveInfoData.h"
#include "CurveInfoUtility.h"
#include <boost/utility.hpp>

namespace curve_info
{
	using namespace utility::spatial_reference;

	double Segment::length() const
	{
		// 始点と終点が同じなら長さは0
		if (m_pFromPoint->_Compare( m_pToPoint ) == 0)
			{ return 0; }

		return utility::distance( 
			m_pFromPoint->GetX(),	m_pFromPoint->GetY(),
			m_pToPoint->GetX(),		m_pToPoint->GetY()
		);
	}

	bool Segment::valid() const
	{
		return length() != 0;
	}

	void SingleRoad::addLink( IFeaturePtr cpLink, bool cStartIsFrom, bool cIsBack )
	{
		Segments aTempSegments;
		ISegmentCollectionPtr ipSegmentCol( cpLink->GetShapeCopy() );
		for (long i = 0; i < ipSegmentCol->GetSegmentCount(); ++i)
		{
			// SingleRoad内での向きが全てBack側に向かうようにSegmentの始終点を決定
			SegmentPtr pSegment;
			_ISegmentPtr ipSegment = ipSegmentCol->GetSegment( i );
			if (cIsBack == cStartIsFrom)
			{
				pSegment = SegmentPtr( new Segment( cpLink->OID, i, ipSegment->GetFromPoint(), ipSegment->GetToPoint(), utility::isHighSpeedArea( cpLink ) ) );
			}
			else
			{
				pSegment = SegmentPtr( new Segment( cpLink->OID, i, ipSegment->GetToPoint(), ipSegment->GetFromPoint(), utility::isHighSpeedArea( cpLink ) ) );
			}

			if (cStartIsFrom)
			{
				aTempSegments.push_back( pSegment );
			}
			else
			{
				aTempSegments.push_front( pSegment );
			}
		}

		if (cIsBack)
		{
			std::copy( aTempSegments.begin(), aTempSegments.end(), std::back_inserter( *this ) );
		}
		else
		{
			std::copy( aTempSegments.begin(), aTempSegments.end(), std::front_inserter( *this ) );
		}
	}

	LinkIDArray SingleRoad::linkIDArray() const
	{
		LinkIDArray aLinkIDArray;
		long aLastLinkID = 0;
		const SingleRoad& rThis = *this;					// BOOST_FOREACHにはthisを直接渡せないので
		BOOST_FOREACH (const SegmentPtr& pSegment, rThis)
		{
			if (! pSegment)
			{
				ATLASSERT(0);
				continue;
			}

			// 既出リンクならスキップ
			long aLinkID = pSegment->m_LinkID;
			if (aLinkID == aLastLinkID)
				{ continue; }
			aLastLinkID = aLinkID;

			aLinkIDArray.push_back( aLinkID );
		}
		return aLinkIDArray;
	}

	double SingleRoad::length() const
	{
		// 未算出なら算出する
		if (m_Length == 0)
		{
			const SingleRoad& rThis = *this;
			BOOST_FOREACH (const SegmentPtr pSegment, rThis)
			{
				m_Length += pSegment->length();
			}
		}
		return m_Length;
	}

	const SingleRoad::RatioMap& SingleRoad::ratioMap() const
	{
		// 未作成なら作成する
		if (m_RatioMap.empty())
		{
			double aFromDistance = 0;	///< 一本道の始点からセグメントの始点までの道のり
			for (SegmentIt itSegment = begin(); itSegment != end(); ++itSegment)
			{
				m_RatioMap.insert( std::make_pair( 
					aFromDistance / length(),
					itSegment
				) );
				aFromDistance += (*itSegment)->length();
			}
		}

		return m_RatioMap;
	}

	SegmentIt SingleRoad::segment( Ratio cRatio ) const
	{
		ATLASSERT( 0 <= cRatio && cRatio <= 1 );
		RatioMap::const_iterator itRatioMap = ratioMap().upper_bound( cRatio );

		// cRatioが「最後のセグメントの開始位置」以降を指している場合
		if( itRatioMap == ratioMap().end() )
		{
			// 一本道の最後のセグメントを返す
			return boost::prior( end() );
		}
		else
		{
			return boost::prior( itRatioMap->second );
		}
	}

	Segments SingleRoad::segments( Range cRange ) const
	{
		Segments aSegments;

		SegmentIt itBeginSegment = segment( cRange.first );
		SegmentIt itEndSegment = segment( cRange.second );

		for (SegmentIt itSegment = itBeginSegment; itSegment != boost::next( itEndSegment ); ++itSegment)
		{
			aSegments.push_back( *itSegment );
		}

		return aSegments;
	}

	IGeometryPtr SingleRoad::geometry() const
	{
		IPointCollection4Ptr ipPointCol = newGeometry< IPointCollection4Ptr >( CLSID_Polyline, m_pSpRef );

		// TODO: 高速化
		// セグメント単位の処理はコストが大きそうなので、分割されていない一本道はリンク単位で追加したい。
		// 仮想セグメントを１つも含まないSRならその処理でいいはず。
		// できればリンクごとに、仮想セグメントの有無を見て決められれば最適。

		// 一本道を構成する各セグメントについて
		const SingleRoad& rThis = *this;
		BOOST_FOREACH (const SegmentPtr& pSegment, rThis)
		{
			ATLASSERT( pSegment );
			long aLinkID = pSegment->m_LinkID;

			// セグメントの始終点を目的形状に追加
			if (ipPointCol->GetPointCount() == 0)
			{
				ipPointCol->AddPoint( pSegment->m_pFromPoint );
			}
			ipPointCol->AddPoint( pSegment->m_pToPoint );
		}

		return ipPointCol;
	}

	void SingleRoad::add( SegmentPtr cpSegment )
	{
		if (cpSegment->valid())
		{
			ATLASSERT( hasSpatialReference( cpSegment->m_pFromPoint ) );
			ATLASSERT( hasSpatialReference( cpSegment->m_pToPoint ) );

			push_back( cpSegment );
		}
	}

} // namespace curve_info

