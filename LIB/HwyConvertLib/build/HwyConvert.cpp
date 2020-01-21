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

#include "stdafx.h"
#include <SindyDBConnection.h>
#include "SINDY2GS.h"
#include "SINDY2HIGHWAY.h"
#include "SINDY2ROADPOINT.h"
#include "SINDY2HWYNODE.h"
#include "SINDY2HWYSAME.h"
#include "SINDY2ROADCODE.h"
#include "SINDY2TOLLGATELANE.h"
#include "FacilNameChangeTable.h"
#include "INHwySchema.h"
#include "HwyConvert.h"

namespace hwy_convert_lib
{
	HwyConvert::HwyConvert(
		const std::string& roadDBStr,
		const std::string& addrDBStr
	)
	{
		// SindyDBConnectionを作成してデータの読み込みなどを行い、
		// SindyHighwayConvert内で作られるものと同じ状態にする
		m_pRoadDBConnection = SPSindyDBConnection( new SindyDBConnection );
		m_pAddrDBConnection = SPSindyDBConnection( new SindyDBConnection );
		m_pRoadDBConnection->Connect( roadDBStr );
		m_pAddrDBConnection->Connect( addrDBStr );
		m_pHwyInfo = SPSINDY2INHWYINFO( new SINDY2INHWYINFO( m_pRoadDBConnection.get() ) );
		m_pHwyInfo->Load( true );
		m_pHwyInfo->SetBaseAdmin( m_pAddrDBConnection.get() );
	}

	SPSINDY2HIGHWAY HwyConvert::highway()
	{
		// 作成済みならそれを返す
		if ( m_pHighway )
			{ return m_pHighway; }

		// 変換
		m_pHighway = SPSINDY2HIGHWAY( new SINDY2HIGHWAY( m_pHwyInfo.get() ) );
		if ( FAILED( m_pHighway->Convert() ) )
			{ return SPSINDY2HIGHWAY(); }	// 失敗したらNULLを返す

		return m_pHighway;
	}

	SPSINDY2GS HwyConvert::gs()
	{
		// 作成済みならそれを返す
		if ( m_pGS )
			{ return m_pGS; }

		// 変換
		m_pGS = SPSINDY2GS( new SINDY2GS( m_pHwyInfo.get() ) );
		if ( FAILED( m_pGS->Convert() ) )
			{ return SPSINDY2GS(); }	// 失敗したらNULLを返す

		return m_pGS;
	}

	SPSINDY2ROADCODE HwyConvert::roadCode()
	{
		// 作成済みならそれを返す
		if ( m_pRoadCode )
			{ return m_pRoadCode; }

		// 変換
		m_pRoadCode = SPSINDY2ROADCODE( new SINDY2ROADCODE( m_pHwyInfo.get() ) );
		if ( FAILED( m_pRoadCode->Convert() ) )
			{ return SPSINDY2ROADCODE(); }	// 失敗したらNULLを返す

		return m_pRoadCode;
	}

	SPSINDY2ROADPOINT HwyConvert::roadPoint(
		const std::wstring* pFNCTPath/*= NULL*/
	)
	{
		// 施設名称変換の有無に応じたコンテナに着目
		SPSINDY2ROADPOINT& pRoadPoint = pFNCTPath ? m_pRoadPointFNC : m_pRoadPoint;

		// 作成済みならそれを返す
		if ( pRoadPoint )
			{ return pRoadPoint; }

		// 変換
		pRoadPoint = SPSINDY2ROADPOINT( new SINDY2ROADPOINT( m_pHwyInfo.get() ) );
		if ( FAILED( pRoadPoint->Convert() ) )
			{ return SPSINDY2ROADPOINT(); }	// 失敗したらNULLを返す
		// 分岐版データ作成
		if ( FAILED( pRoadPoint->makeJctBranchData() ) )
			{ return SPSINDY2ROADPOINT(); }
		// 施設名称変換
		if ( pFNCTPath )
		{
			try
			{
				pRoadPoint->changeFacilName(
					&FacilNameChangeTable( uh::toStr( *pFNCTPath ), true )
				);
			}
			catch (...)
				{ return SPSINDY2ROADPOINT(); }
		}

		return pRoadPoint;
	}

	SPSINDY2HWYNODE HwyConvert::hwyNode(
		 hwynode::file_format::EFormat format/* = hwynode_smartic_added */
	)
	{
		// 作成済みならそれを返す
		if ( m_pHwyNode )
			{ return m_pHwyNode; }

		// 変換
		m_pHwyNode = SPSINDY2HWYNODE( new SINDY2HWYNODE( m_pHwyInfo.get() ) );
		if ( FAILED( m_pHwyNode->Convert( format ) ) )
			{ return SPSINDY2HWYNODE(); }	// 失敗したらNULLを返す

		return m_pHwyNode;
	}

	SPSINDY2HWYSAME HwyConvert::hwySame(
		const std::wstring* pFNCTPath/*= NULL*/
	)
	{
		// 施設名称変換の有無に応じたコンテナに着目
		SPSINDY2HWYSAME& pHwySame = pFNCTPath ? m_pHwySameFNC : m_pHwySame;

		// 作成済みならそれを返す
		if ( pHwySame )
			{ return pHwySame; }

		// 変換
		pHwySame = SPSINDY2HWYSAME( new SINDY2HWYSAME( m_pHwyInfo.get() ) );
		if ( FAILED( pHwySame->Convert() ) )
			{ return SPSINDY2HWYSAME(); }	// 失敗したらNULLを返す
		// 施設名称変換
		if ( pFNCTPath )
		{
			try
			{
				pHwySame->changeFacilName(
					&FacilNameChangeTable( uh::toStr( *pFNCTPath ), true )
				);
			}
			catch (...)
				{ return SPSINDY2HWYSAME(); }
		}
		// スマートIC除外版データ作成
		pHwySame->createHwySameListWithoutSmartIC();

		return pHwySame;
	}

	SPSINDY2TOLLGATELANE HwyConvert::tollGateLane()
	{
		// 作成済みならそれを返す
		if ( m_pTollGateLane )
			{ return m_pTollGateLane; }

		// 変換
		m_pTollGateLane = SPSINDY2TOLLGATELANE( new SINDY2TOLLGATELANE( m_pHwyInfo.get() ) );
		if ( FAILED( m_pTollGateLane->Convert() ) )
			{ return SPSINDY2TOLLGATELANE(); }	// 失敗したらNULLを返す

		return m_pTollGateLane;
	}

} // namespace hwy_convert_lib

