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
#include "CurveInfoUtility.h"
#include <Type.h>
#include <RNSlibs/RNSShape/RNSLine.h>	// for 角度計算
#include <RNSlibs/RNSShape/RNSAngle.h>	// for 角度計算
#include <iomanip>
#include <crd_cnv/crd_cnv.h>
#include "RoadLinkTable.h"
#include "RowContainer.h"
#include "notifier.h"

namespace curve_info
{
	namespace utility
	{
		double angle( const Segment& crSegment1, const Segment& crSegment2 )
		{
			const static CInfo coordInfo(CCoordinate::kLonLat, country::country_type::japan);
			RNSPoint aFromPoint1( crSegment1.m_pFromPoint->GetX(), crSegment1.m_pFromPoint->GetY(), coordInfo );
			RNSPoint aToPoint1  ( crSegment1.m_pToPoint->GetX(),   crSegment1.m_pToPoint->GetY(), coordInfo );
			RNSPoint aFromPoint2( crSegment2.m_pFromPoint->GetX(), crSegment2.m_pFromPoint->GetY(), coordInfo );
			RNSPoint aToPoint2  ( crSegment2.m_pToPoint->GetX(),   crSegment2.m_pToPoint->GetY(), coordInfo );

			return RNSAngle( RNSLine( aFromPoint2, aToPoint2 ).angle() - RNSLine( aFromPoint1, aToPoint1 ).angle() ).angle();
		}

#if 0	// 未使用だが、使えそうなので残しておく
	double meterLength( const SegmentPtr cpSegment )
	{
		crd_cnv aCrdCnv;
		return aCrdCnv.GetDist(
			cpSegment->m_pFromPoint->GetX(),
			cpSegment->m_pFromPoint->GetY(),
			cpSegment->m_pToPoint->GetX(),
			cpSegment->m_pToPoint->GetY()
		);
	}
#endif
		namespace log_format
		{
			std::vector< std::string > fieldNames()
			{
				return boost::assign::list_of("LAYER")("OBJECTID")("LON")("LAT");
			}

			std::string freeComment( std::string& crMessage )
			{
				std::stringstream ss;
				ss << "# " << crMessage << std::endl;
				return ss.str();
			}

			std::string pointRecord( double cLon, double cLat, const std::string& crMessage )
			{
				std::stringstream ss;
				ss << std::setprecision( 20 ) << fd << fd << cLon << fd << cLat << fd << crMessage << std::endl;
				return ss.str();
			}

			std::string linkRecord( const std::string& cTable, long cOID, const std::string& crMessage )
			{
				std::stringstream ss;
				ss << cTable << fd << cOID << fd << fd << fd << crMessage << std::endl;
				return ss.str();
			}

		} // namespace log_record

		double distance( double x1, double y1, double x2, double y2 )
		{
			return sqrt(pow( x1 - x2, 2 ) + pow( y1 - y2, 2 ));
		}

		double degreeToMeterCoef( double cLon1, double cLat1, double cLon2, double cLat2 )
		{
			ATLASSERT( ! (cLon1 == cLon2 && cLat1 == cLat2 ) );		// ２点は同一ではない事が前提

			double aDegreeDistance = distance( cLon1, cLat1, cLon2, cLat2 );

			double aMeterDistance = 0;
			double tempX, tempY;		// 使用しない
			crd_cnv cc;
			aMeterDistance = cc.GetDistXY( cLon1, cLat1, cLon2, cLat2, &tempX, &tempY );

			return aMeterDistance / aDegreeDistance;
		}

		double circularArcAngle( double cCircularArcLength, double cRadius )
		{
			return 180 * cCircularArcLength / ( M_PI * cRadius );
		}

		bool isPassable( IFeaturePtr cpLink )
		{
			using namespace sindy::schema;
			long aNoPassage = cpLink->GetValue( fieldIndex( cpLink, road_link::kTableName, road_link::kNoPassage ) );
			switch (aNoPassage)
			{
				using namespace road_link::no_passage;
				case kNoPassage:
				case kInpassable:
					return false;
				default:
					return true;
			}
		}

		bool isPassableByTurnreg( IFeaturePtr cpLink1, IFeaturePtr cpLink2, sindy::CRoadLinkTable& crLinkTable )
		{
			using namespace sindy;
			using namespace schema;
			using namespace utility::sindylib_base;

			long aLinkID1 = cpLink1->OID;
			long aLinkID2 = cpLink2->OID;

			CTableContainer::iterator itLink1 = crLinkTable.find_by_key( kObjectID, aLinkID1 );	///< リンクテーブル内の対象リンク１
			// リンクテーブル内に対象リンク１が含まれていなければ終了
			if (itLink1 == crLinkTable.end())
			{
				ATLASSERT(0);	// そういう使い方はされない想定。
				return true;
			}

			// 「対象リンク１→対象リンク２」を含む右左折禁止規制がないかチェックする
			CSPRowContainer pRCLink1 = row( *itLink1 );
			CSPTableContainer pLQTable = pRCLink1->GetItemTable( lq_turnreg::kTableName );		///< 対象リンク１に対応するLQ群

			// 各LQについて
			BOOST_FOREACH (CIndexedContainer::iterator itLQ, pLQTable->finds_by_key( lq_turnreg::kLinkID, aLinkID1 ))
			{
				// 着目LQが属するInfを取得
				CSPRowContainer pRCLQ = row( *itLQ );
				_IRowPtr ipLQ = (_IRow*)*pRCLQ;
				long aInfID = pRCLQ->GetValue( lq_turnreg::kInfID, -1L );
				CSPTableContainer pInfTable = pRCLQ->GetItemTable( inf_turnreg::kTableName );
				CIndexedContainer::iterator itInf = pInfTable->find_by_key( kObjectID, aInfID );	///< 着目LQが属するInf
				ATLASSERT( itInf != pInfTable->end());	// 失敗するなら、LQの子としてのInfの読み込みがうまくいっていない。
				CSPRowContainer pRCInf = row( *itInf );

				CSPTableContainer pLQTable2 = pRCInf->GetItemTable( lq_turnreg::kTableName );	///< Infに属するLQ群
				ATLASSERT( pLQTable2 );	// 失敗するなら、Infの子としてのLQの読み込み（CRoadLinkTable::SelectLQs()）がうまくいっていない。
				long aPrevLQLinkID = 0;		///< １つ前のLQに対応するリンクID
				// Infに属する各LQについて
				BOOST_FOREACH (CIndexedContainer::iterator itLQ2, pLQTable2->finds_by_key( lq_turnreg::kInfID, aInfID ))
				{
					CSPRowContainer pRCLQ2 = row( *itLQ2 );
					_IRowPtr ipLQ2 = (_IRow*)*pRCLQ2;
					long aLQLinkID = ipLQ2->GetValue( fieldIndex( ipLQ2, lq_turnreg::kTableName, lq_turnreg::kLinkID ) );	///< LQに対応するリンクID
					// LQ列中に「対象リンク１→対象リンク２」という並びが１つでもあれば通行不可
					if (aPrevLQLinkID == aLinkID1 && aLQLinkID == aLinkID2)
					{
						ATLASSERT( aPrevLQLinkID != 0 );
						return false;
					}
					aPrevLQLinkID = aLQLinkID;
				}
			}

			return true;
		}

		bool isHighSpeedArea( IFeaturePtr cpLink )
		{
			using namespace sindy::schema::road_link;

			// UpDownClass_Cによる判定
			long aUpDownClass = cpLink->GetValue( fieldIndex( cpLink, kTableName, kUpDownClass ) );
			switch (aUpDownClass)
			{
				using namespace up_down_class;
				case kUndirected:
				case kUpInWest:
				case kDownOutEast:
				case kCommon:
					break;
				default:
					return false;
			}

			// 道路表示種別による判定
			long aRoadClass = cpLink->GetValue( fieldIndex( cpLink, kTableName, kRoadClass ) );
			switch (aRoadClass)
			{
				using namespace road_class;
				case kFreeHighway:
				case kHighway:
					break;
				default:
					return false;
			}

			// リンク種別による判定
			long aLinkClass = cpLink->GetValue( fieldIndex( cpLink, kTableName, kMainLinkClass ) );
			switch (aLinkClass)
			{
				using namespace link_class;
				case kMain:
				case kMainUpDown:
					break;
				default:
					return false;
			}

			return true;
		}

		namespace id_lists
		{
			IDLists::IDLists( const IDs& crIDs, const std::string& cDelimiter, unsigned cMax )
			{
				std::stringstream ss;
				long aCount = 1;			///< 列挙数
				for (IDs::const_iterator it = crIDs.begin(); it != crIDs.end(); ++it)
				{
					long aID = *it;

					ss << aID;

					// 列挙数が最大値に達したら、そこまでの文字列を自身のコンテナに格納して仕切り直し
					if (aCount == cMax)
					{
						push_back( ss.str() );
						ss.clear();
						aCount = 1;
					}
					// 最後のIDでなければデリミタを書き込んで次へ
					else if (it != boost::prior( crIDs.end() ))
					{
						ss << cDelimiter;
						++aCount;
					}
				}
			}

		} // namespace id_lists

		void createDirectory( const boost::filesystem::path& crDirPath )
		{
			using namespace boost::filesystem;
			try
			{
				create_directories( crDirPath );
			}
			catch (...)
			{
				notifier::critical::asCannotCreateDirectory( crDirPath.string() );
			}
		}

		namespace spatial_reference
		{
			bool hasSpatialReference( const IGeometryPtr cpGeom )
			{
				return cpGeom->GetSpatialReference();
			}

			void setSpatialReference( IGeometryPtr cpGeom, const ISpatialReferencePtr cpSpatialRef )
			{
				// 同じ空間参照が既にセットされている場合は何もしない
				VARIANT_BOOL aIsEqual = VARIANT_FALSE;
				if (FAILED( cpSpatialRef->IsPrecisionEqual( cpGeom->GetSpatialReference(), &aIsEqual ) ))
				{
					notifier::critical::asCannotSetSpatialReference();
				}
				if ( aIsEqual )
					{ return; }

				HRESULT hr = cpGeom->putref_SpatialReference( cpSpatialRef );
				if (FAILED( hr ) || ! cpGeom->GetSpatialReference() )
				{
					notifier::critical::asCannotSetSpatialReference();
				}
			}

		} // namespace spatial_reference

		std::string toStr( double cNum, std::streamsize cPrecision )
		{
			std::stringstream ss;
			ss << std::setprecision( cPrecision ) << cNum;
			return ss.str();
		}

	} // namespace utility
} // namespace curve_info


