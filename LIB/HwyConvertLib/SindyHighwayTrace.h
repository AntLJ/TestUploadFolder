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

// SindyHighwayTrace.h: SindyHighwayTrace クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
/**
 * @file HwyConvertLib開発に伴い、SindyHighwayCheckのソースがSindyHighwayConvertでも使われていたのを解消するため、
 *       その共有部分をSindyHighwayCheckからHwyConvertLibに移管した。
 */

#if !defined(AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_)
#define AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayTools.h"
#include "SindyRoadLinkTraceCondition.h"

class SindyHighwayTrace : public SindyRoadLinkTraceCondition
{
public:
	enum TraceTarget
	{
		trace_all		= 0,	// 全て
		trace_ramp		= 1,	// 本線リンク以外のリンクのみ
		trace_mainline	= 2		// 本線リンクのみ
	};

	SindyHighwayTrace(TraceTarget cTraceTarget = trace_ramp);		// デフォルトでは本線以外のリンクのみを追跡対象とする
	virtual ~SindyHighwayTrace();

	/**
	 * @brief 追跡方向をsindy::rnw::relation::codeのfore,back,bothdirのいずれかで指定する
	 * @param[in]	cDirection	追跡方向
	 */
	virtual void set_direction(long cDirection) { mDirection = cDirection; }

	/**
	 * @brief 追跡対象リンクを「UpDownClass_CがHWY対象のものだけにする」か否かを設定する
	 * @note	[Bug8480]対応時に、trace()のこの部分を有効にするかを派生クラスが任意に選択できるようにするために作成。
	 *			（というか、そもそもtrace()にそれを追加したことが設計ミスなので、そのフォローである。）
	 *			これまでに作った派生クラスが「有効」になるように、mUpDownClassEnabledのデフォルトはコンストラクタで「有効」にしておく。
	 * @param[in]	cIsEnabled	設定値
	 */
	void set_up_down_class_option( bool cIsEnabled ) { mUpDownClassEnabled = cIsEnabled; }

	virtual HRESULT trace( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );

	/**
	 * @brief 対象ノードから対象リンクへ、追跡方向に応じて遷移可能であるか否かを判定する（[Bug7176]で追加）
	 * @note	[Bug7176]で、派生クラスにて上記だけを判定したい箇所があったので、trace()の一部を関数化
	 * @param[in]	cRoadInfo	道路情報
	 * @param[in]	ipLink		対象リンク
	 * @param[in]	ipNode		対象ノード
	 * @param[out]	cTrace		判定結果を返すバッファ
	 * @return	成否
	 */
	virtual HRESULT Passable( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );

protected:
	long		mDirection;				// 追跡方向
	TraceTarget	m_TraceTarget;			// 追跡対象
	bool		mUpDownClassEnabled;	// trace()でUpDownClass_Cによる判定を行うか否か（[Bug8480]で追加）
};

#endif // !defined(AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_)
