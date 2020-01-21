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

#include <set>
#include "SindyHighwayTrace.h"

/**
 * @brief [Bug7307] 天保山JCTのケースでfclpoint.txtの上下線種別コードを決定するための追跡クラス
 * @note	・与えられた追跡対象路線コードを持つリンクまたは路線コード0のリンクを一方通行の逆方向に辿り、
 *			　始点アクセスポイントと同じ路線コードを持つ本線リンクを探す
 *			・目的のリンクに到達したか否かは、trace()のcTraceではなく、m_FoundLinksに１つ以上リンクが格納されたかどうかで判定する
 *			・複数のリンクを見つけられるようにするため、exit()では終了せず、trace()対象リンクが１つもなくなるまで追跡を続ける
 */
class SindyFindMainLineFromJCTInterflow : public SindyHighwayTrace
{
public:
	SindyFindMainLineFromJCTInterflow( long cOriginAccessPointRoadCode, long cTraceTargetRoadCode );
	~SindyFindMainLineFromJCTInterflow(void);

	virtual HRESULT trace( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );
	virtual HRESULT exit( SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit );
	
	/**
	 * @brief 見つかったリンク群の取得
	 * @return	追跡の結果、見つかったリンク群
	 */
	const GDBOBJLIST& getFoundLinks(void) { return m_FoundLinks; }

private:
	SindyFindMainLineFromJCTInterflow(void);	// デフォルトコンストラクタを禁止

	long m_OriginAccessPointRoadCode;	// 始点アクセスポイントの路線コード
	long m_TraceTargetRoadCode;			// 追跡対象とするリンクの路線コード

	GDBOBJLIST	m_FoundLinks;			// 見つかったリンク群

	typedef long OID;
	typedef std::set<OID> OIDs;
	OIDs m_TraceEndNodes;	// 追跡終端ノード群

};
