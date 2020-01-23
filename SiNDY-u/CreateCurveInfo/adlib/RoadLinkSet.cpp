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
#include "RoadLinkSet.h"
#include <boost/foreach.hpp>
#include <sindy/schema.h>

namespace adlib
{
	namespace road_link_set
	{
		RoadLinkSet::RoadLinkSet(void)
		{
		}

		RoadLinkSet::~RoadLinkSet(void)
		{
		}

		NodeIDTableRange RoadLinkSet::searchByFromNodeID(long cNodeID) const
		{
			if (m_FromNodeIDTable.size() == 0)
			{
				BOOST_FOREACH (OIDTable::value_type aPair, getOIDTable())
				{
					_IRowPtr& ipRoadLink = aPair.second;
					// FromNodeID‚ÌŽæ“¾
					static long aFromNodeIDFieldIndex = ipRoadLink->Fields->_FindField(sindy::schema::road_link::kFromNodeID);
					long aFromNodeID = ipRoadLink->GetValue(aFromNodeIDFieldIndex);
					// Ši”[
					m_FromNodeIDTable.insert(
						std::make_pair(aFromNodeID, ipRoadLink));
				}
			}

			return m_FromNodeIDTable.equal_range(cNodeID);
		}

		NodeIDTableRange RoadLinkSet::searchByToNodeID(long cNodeID) const
		{
			if (m_ToNodeIDTable.size() == 0)
			{
				BOOST_FOREACH (OIDTable::value_type aPair, getOIDTable())
				{
					_IRowPtr& ipRoadLink = aPair.second;
					// ToNodeID‚ÌŽæ“¾
					static long aToNodeIDFieldIndex = ipRoadLink->Fields->_FindField(sindy::schema::road_link::kToNodeID);
					long aToNodeID = ipRoadLink->GetValue(aToNodeIDFieldIndex);
					// Ši”[
					m_ToNodeIDTable.insert(
						std::make_pair(aToNodeID, ipRoadLink));
				}
			}

			return m_ToNodeIDTable.equal_range(cNodeID);
		}

		IFeaturePtr RoadLinkSet::searchByFromToNodeID(long cNodeID1, long cNodeID2) const
		{
			if (m_FromToNodeIDTable.size() == 0)
			{
				BOOST_FOREACH (OIDTable::value_type aPair, getOIDTable())
				{
					_IRowPtr& ipRoadLink = aPair.second;
					// FromNodeID‚ÌŽæ“¾
					static long aFromNodeIDFieldIndex = ipRoadLink->Fields->_FindField(sindy::schema::road_link::kFromNodeID);
					long aFromNodeID = ipRoadLink->GetValue(aFromNodeIDFieldIndex);
					// ToNodeID‚ÌŽæ“¾
					static long aToNodeIDFieldIndex = ipRoadLink->Fields->_FindField(sindy::schema::road_link::kToNodeID);
					long aToNodeID = ipRoadLink->GetValue(aToNodeIDFieldIndex);
					// Ši”[
					m_FromToNodeIDTable.insert(
						std::make_pair(
							std::make_pair( aFromNodeID, aToNodeID ),
							ipRoadLink
						)
					);
				}
			}

			IFeaturePtr ipRoadLink;

			FromToNodeIDTable::iterator it = m_FromToNodeIDTable.find( std::make_pair( cNodeID1, cNodeID2 ) );
			if (it != m_FromToNodeIDTable.end())
			{
				ipRoadLink = it->second;
			}
			else
			{
				it = m_FromToNodeIDTable.find( std::make_pair( cNodeID2, cNodeID1 ) );
				if (it != m_FromToNodeIDTable.end())
				{
					ipRoadLink = it->second;
				}
			}
			return ipRoadLink;
		}

		template <typename OutputIterator>
		void insertLinkIDs( const NodeIDTableRange& cRange, OutputIterator coIt )
		{
			for (NodeIDTable::const_iterator it = cRange.first; it != cRange.second; ++it)
			{
				const IFeaturePtr& ipLink = it->second;
				*coIt = ipLink->OID;
			}
		}


		LinkIDs RoadLinkSet::searchByNodeID( long cNodeID ) const
		{
			LinkIDs aLinkIDs;

			// cNodeID‚ðFromƒm[ƒhID‚Æ‚·‚éƒŠƒ“ƒN‚ÌƒŠƒ“ƒNID‚ðŠi”[
			NodeIDTableRange aFromRange = searchByFromNodeID( cNodeID );
			insertLinkIDs( aFromRange, std::inserter( aLinkIDs, aLinkIDs.begin() ) );

			// cNodeID‚ðToƒm[ƒhID‚Æ‚·‚éƒŠƒ“ƒN‚ÌƒŠƒ“ƒNID‚ðŠi”[
			NodeIDTableRange aToRange = searchByToNodeID( cNodeID );
			insertLinkIDs( aToRange, std::inserter( aLinkIDs, aLinkIDs.begin() ) );

			return aLinkIDs;
		}


		NodeIDPair RoadLinkSet::getFromToNodeID( long cOID ) const
		{
			IFeaturePtr ipRoadLink = searchByOID( cOID );
			NodeIDPair aNodeIDPair = std::make_pair(0, 0);

			if (cOID)
			{
				static long aFromNodeIDFieldIndex = ipRoadLink->Fields->_FindField( sindy::schema::road_link::kFromNodeID );
				aNodeIDPair.first = ipRoadLink->GetValue( aFromNodeIDFieldIndex );
				static long aToNodeIDFieldIndex = ipRoadLink->Fields->_FindField( sindy::schema::road_link::kToNodeID );
				aNodeIDPair.second = ipRoadLink->GetValue( aToNodeIDFieldIndex );
			}

			return aNodeIDPair;
		}


		void RoadLinkSet::clear(void)
		{
			RowSet::clear();
			m_FromNodeIDTable.clear();
			m_ToNodeIDTable.clear();
			m_FromToNodeIDTable.clear();
		}

		void RoadLinkSet::dump(std::ostream& co) const
		{
			co << "# OIDTable" << std::endl;
			BOOST_FOREACH (OIDTable::value_type aElement, getOIDTable())
			{
				co << aElement.first << std::endl;;
			}
			
			co << "# FromToNodeIDTable" << std::endl;
			BOOST_FOREACH (FromToNodeIDTable::value_type aElement, m_FromToNodeIDTable)
			{
				long aFromNodeID = aElement.first.first;
				long aToNodeID = aElement.first.second;
				long aLinkID = aElement.second->OID;
				co << aFromNodeID << "\t" << aToNodeID << "\t" << aLinkID << std::endl;
			}
		}
	} // namespace road_link_set
} // namespace adlib
