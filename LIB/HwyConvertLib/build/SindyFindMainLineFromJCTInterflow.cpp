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

	// �ǐՂ̊�{�����ɍ��v���Ȃ���ΑΏۊO�Ƃ���
	if (FAILED(hr = SindyHighwayTrace::trace( cRoadInfo, ipLink, ipNode, cTrace ))) { return hr; }
	if (! *cTrace)
	{
		return S_OK;
	}
	*cTrace = false;

	// �ǐՏI�[�m�[�h�iexit()�ŖړI�̃����N�����������m�[�h�j�Ȃ�X�L�b�v
	if (m_TraceEndNodes.find( aNodeID ) != m_TraceEndNodes.end())
	{
		return  S_OK;
	}

	// �H���R�[�h��m_TraceTargetRoadCode�Ɠ�����0�łȂ���ΑΏۊO�Ƃ���
	static long aRoadCodeFieldIndex = ipLink->Fields->_FindField( sindy::road_link::ROAD_CODE.c_str() );
	long aRoadCode = ipLink->GetValue( aRoadCodeFieldIndex );
	if (aRoadCode != m_TraceTargetRoadCode && aRoadCode != 0 )
	{
		return S_OK;
	}

	// �����N��ʂ��{�����A���H�łȂ���ΑΏۊO�Ƃ���
	if ( ! isMainLineLink( ipLink ) && ! isJctLink( ipLink ) )
	{
		return S_OK;
	}

	// UpDownClass_C�������łȂ���ΑΏۊO�Ƃ���
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
		case updownclass_etcramp:		// ETC��p�����v�����N���ǐՂ��Ă��Ӗ����Ȃ��̂ŁA�ΏۊO�Ƃ���
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

	// �ڑ������N�̎擾
	GDBOBJLIST aLinkList;
	if (FAILED(hr = cRoadInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
		{ throw std::runtime_error("�ڑ������N�̎擾�Ɏ��s"); }

	// �ڑ������N�̒��ɖړI�̃����N������΁Am_FoundLinks�Ɋi�[
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

