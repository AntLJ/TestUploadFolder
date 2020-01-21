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

// SindyHighwayTrace.cpp: SindyHighwayTrace クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyHighwayTrace.h"


SindyHighwayTrace::SindyHighwayTrace(TraceTarget cTraceTarget) :
mDirection(sindy::rnw::relation::fore),
m_TraceTarget(cTraceTarget),
mUpDownClassEnabled( true )		// [Bug8480]で追加
{
}

SindyHighwayTrace::~SindyHighwayTrace()
{
}

/**
 * @note 与えられたリンクが追跡対象であるか否かを判定する
 * @note デフォルトでは、本線は追跡しない。
 */
HRESULT SindyHighwayTrace::trace(SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace)
{
	HRESULT hr;
	const char *func = "SindyHighwayTrace::trace";

	*cTrace = true;

#ifdef	_DEBUG
	long aLinkID = ipLink->GetOID();
#endif

	if (mUpDownClassEnabled)	// [Bug8480]で追加
	{
		// UpDownClass_CがHWY対象でなければ追跡対象外
		static long aUpDownClassFieldIndex = ipLink->Fields->_FindField( sindy::road_link::UPDOWNCLASS_C.c_str() );
		long aUpDownClass = ipLink->GetValue( aUpDownClassFieldIndex );
		switch (aUpDownClass)
		{
			case sindy::road_link::updownclass_nodir:
			case sindy::road_link::updownclass_up:
			case sindy::road_link::updownclass_down:
			case sindy::road_link::updownclass_common:
			case sindy::road_link::updownclass_etcramp:
				break;
			case sindy::road_link::updownclass_nothwy:
			case sindy::road_link::updownclass_etcaccess:
				*cTrace = false;
				return S_OK;
			default:
				ATLASSERT(0);
		}
	}

	// 既に通過済みのリンクなら追跡対象外
	bool already = false;
	if (FAILED(hr = already_traced(ipLink, &already))) return hr;
	if (already)
	{
		*cTrace = false;
		return S_OK;
	}

	// 追跡対象リンクであるか？
	switch (m_TraceTarget)
	{
		case trace_ramp:
			{
				if (isMainLineLink(ipLink))
				{
					*cTrace = false;
					return S_OK;
				}
			}
			break;
		case trace_mainline:
			{
				if (! isMainLineTraceTarget(ipLink))
				{
					*cTrace = false;
					return S_OK;
				}
			}
			break;
		case trace_all:
			break;
		default:
			ATLASSERT(0);
	}

	if (FAILED(hr = Passable( cRoadInfo, ipLink, ipNode, cTrace ))) return hr;

#ifdef	_DEBUG
	if (*cTrace)
	{
		std::cout << "\t" << "Road_Link" << "\t" << aLinkID << "\t" << func << std::endl;
	}
#endif

	return S_OK;
}

HRESULT SindyHighwayTrace::Passable( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace )
{
	HRESULT hr;
	const char *func = "SindyHighwayTrace::Passable";

	*cTrace = true;

	// 前のリンクとの関係のチェック
	unsigned long size = mRoute.size();
	if (size != 0)
	{
		// １つ前のリンクとの間に右左折禁止がないか確認
		IFeaturePtr ipPreLink = NULL;
		if (FAILED(hr = cRoadInfo->eRoadLink.SearchByObjectID(mRoute[mRoute.size()-1], &ipPreLink))) return hr;
		bool aTurnCorner = true;
		if (FAILED(hr = cRoadInfo->TurnCorner((mDirection==sindy::rnw::relation::fore)?ipPreLink:ipLink, (mDirection==sindy::rnw::relation::fore)?ipLink:ipPreLink, &aTurnCorner))) return hr;
		if (! aTurnCorner)
		{
			*cTrace = false;
			return S_OK;
		}
	}

	// リンクの反対側のノード取得
	long aRelation = 0;
	if (FAILED(hr = cRoadInfo->GetRelation(ipLink, ipNode, &aRelation))) return hr;
	aRelation = (aRelation==sindy::rnw::relation::from) ? sindy::rnw::relation::to : sindy::rnw::relation::from;

	IFeaturePtr ipNode2 = NULL;
	if (FAILED(hr = cRoadInfo->GetNode(ipLink, (sindy::rnw::relation::code)aRelation, &ipNode2))) return hr;
	if (ipNode2 == NULL) return GDBERROR2(E_UNEXPECTED, func);

	// 通行禁止がないか確認
	long aNoPassage = ipLink->GetValue( ipLink->Fields->_FindField("NOPASSAGE_C") );

	switch (aNoPassage)
	{
	case sindy::road_link::nopassage_nocondition:
		*cTrace = false;
		return S_OK;	// 通行禁止の場合は無条件で通れないものとする
	default:
		break;
	}

	// 一方通行があるか、あれば追跡方向と一致するか確認
	long aOneway = ipLink->GetValue( ipLink->Fields->_FindField("ONEWAY_C") );
	if (aOneway == sindy::road_link::oneway_order || aOneway == sindy::road_link::oneway_reverse)
	{
		long aRel1, aRel2;
		if (FAILED(hr = cRoadInfo->GetRelation(ipLink, ipNode,  &aRel1))) return hr;
		if (FAILED(hr = cRoadInfo->GetRelation(ipLink, ipNode2, &aRel2))) return hr;

		switch (aOneway)
		{
			case sindy::road_link::oneway_order:
				if (mDirection & sindy::rnw::relation::fore && (aRel1 == sindy::rnw::relation::from && aRel2 == sindy::rnw::relation::to))
					*cTrace = true;
				else if (mDirection & sindy::rnw::relation::back && (aRel1 == sindy::rnw::relation::to && aRel2 == sindy::rnw::relation::from))
					*cTrace = true;
				else
					*cTrace = false;
				break;
			case sindy::road_link::oneway_reverse:
				if (mDirection & sindy::rnw::relation::fore && (aRel1 == sindy::rnw::relation::to && aRel2 == sindy::rnw::relation::from))
					*cTrace = true;
				else if (mDirection & sindy::rnw::relation::back && (aRel1 == sindy::rnw::relation::from && aRel2 == sindy::rnw::relation::to))
					*cTrace = true;
				else
					*cTrace = false;
				break;
		}
	}

	return S_OK;
}
