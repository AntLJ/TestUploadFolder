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

#include "SindyFindMainLineFromJCTInterflow.h"
#include <boost/foreach.hpp>

SindyFindMainLineFromJCTInterflow::SindyFindMainLineFromJCTInterflow(
	long cOriginAccessPointRoadCode,
	long cTraceTargetRoadCode ) :
SindyHighwayTrace( trace_all ),
m_OriginAccessPointRoadCode( cOriginAccessPointRoadCode ),
m_TraceTargetRoadCode( cTraceTargetRoadCode )
{
}

SindyFindMainLineFromJCTInterflow::~SindyFindMainLineFromJCTInterflow(void)
{
}

HRESULT SindyFindMainLineFromJCTInterflow::trace(SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace)
{
	HRESULT hr;
	long aNodeID = ipNode->OID;
	long aLinkID = ipLink->OID;

	// 追跡の基本条件に合致しなければ対象外とする
	if (FAILED(hr = SindyHighwayTrace::trace( cRoadInfo, ipLink, ipNode, cTrace ))) { return hr; }
	if (! *cTrace)
	{
		return S_OK;
	}
	*cTrace = false;

	// 追跡終端ノード（exit()で目的のリンクが見つかったノード）ならスキップ
	if (m_TraceEndNodes.find( aNodeID ) != m_TraceEndNodes.end())
	{
		return  S_OK;
	}

	// 路線コードがm_TraceTargetRoadCodeと同じか0でなければ対象外とする
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	if (aRoadCode != m_TraceTargetRoadCode && aRoadCode != 0 )
	{
		return S_OK;
	}

	// リンク種別が本線か連結路でなければ対象外とする
	if ( ! isMainLineLink( ipLink ) && ! isJctLink( ipLink ) )
	{
		return S_OK;
	}

	// UpDownClass_Cが高速でなければ対象外とする
	static long aUpDownClassFiledIndex = ipLink->Fields->_FindField( sindy::road_link::UPDOWNCLASS_C.c_str() );
	long aUpDownClass = ipLink->GetValue( aUpDownClassFiledIndex );
	switch (aUpDownClass)
	{
		using namespace sindy::road_link;
		case updownclass_nodir:
		case updownclass_up:
		case updownclass_down:
		case updownclass_common:
			break;
		case updownclass_nothwy:
		case updownclass_etcramp:		// ETC専用ランプリンク上を追跡しても意味がないので、対象外とする
		case updownclass_etcaccess:
			return S_OK;
			break;
		default:
			ATLASSERT(0);
			return S_OK;
	}

#ifdef	_DEBUG
	std::cout << "\t" << "Road_Link" << "\t" << aLinkID << "\t" << "SindyFindMainLineFromJCTInterflow::trace" << std::endl;
	if (aLinkID == 3825878)
	{
		std::cout << "";
	}
#endif
//	m_TracedLinks.insert( aLinkID );
	*cTrace = true;
	return S_OK;
}

HRESULT SindyFindMainLineFromJCTInterflow::exit(SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit)
{
	HRESULT hr;
	long aNodeID = ipNode->OID;

	// 接続リンクの取得
	GDBOBJLIST aLinkList;
	if (FAILED(hr = cRoadInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
		{ throw std::runtime_error("接続リンクの取得に失敗"); }

	// 接続リンクの中に目的のリンクがあれば、m_FoundLinksに格納
	BOOST_FOREACH ( const GDBOBJLIST::value_type& aElement, aLinkList )
	{
		long aLinkID = aElement.first;
		const _IRowPtr& ipLink = aElement.second;

		static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
		long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );

		if (aRoadCode == m_OriginAccessPointRoadCode)
		{
			m_FoundLinks.insert( std::make_pair( aLinkID, ipLink ) );
			m_TraceEndNodes.insert( aNodeID );
		}
	}

	return S_OK;
}

