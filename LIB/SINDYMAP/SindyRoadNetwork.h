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

/**
 *	SindyRoadNetwork.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		新規作成
 */

#ifndef	___SINDY_ROAD_NETWORK_H___
#define	___SINDY_ROAD_NETWORK_H___

#include "SindyRoadLink.h"
#include "SindyRoadNode.h"
#include "SindySegmentAttr.h"
#include "SindyGrad.h"
#include "SindyTurnreg.h"
#include "SindyUturn.h"
#include "SindyVics.h"
#include "SindyInterNavi.h"
#include "SindyByway.h"
#include "SindyRoute.h"
#include "SindyDirguide.h"
#include "SindyGuide.h"
#include "SindyLane.h"
#include "SindyParkingShape.h"
#include "SindyRoadCodeList.h"
#include "SindyDupliLink.h"
#include "SindyTimeNoPassage.h"
#include "SindyTimeOneway.h"
#include "SindyHighwayText.h"
#include "SindyHighwayNode.h"
#include "SindyAccessPoint.h"
#include "SindyFacilInfoPoint.h"
#include "SindyAccessStore.h"
#include "SindyEtcTollgate.h"
#include "SindyIntersection.h"

namespace sindy
{
	namespace rnw
	{
		namespace data_type
		{
			enum road_type
			{
				road_link			= 0x00000001,		// 道路リンク
				road_node			= 0x00000002,		// 道路ノード
				segment_attr		= 0x00000004,		// リンク内属性
				grad				= 0x00000008,		// 傾斜
				turnreg				= 0x00000010,		// 右左折禁止・Uターン禁止
				uturn				= 0x00000020,		// Uターン可能箇所
				vics				= 0x00000040,		// VICS
				inter_navi			= 0x00000080,		// インターナビVICS
				byway				= 0x00000100,		// 抜け道・裏道
				route				= 0x00000200,		// 乗り継ぎ道・観光案内等
				lane				= 0x00004000,		// レーン情報
				guide				= 0x00008000,		// 強制誘導
				dirguide			= 0x00010000,		// 方面案内
				time_nopassage		= 0x00020000,		// 通行禁止条件
				time_oneway			= 0x00040000,		// 一方通行条件
				dupli_link			= 0x00080000,		// 重用路線
				intersection		= 0x00100000		//進入方向別交差点名
			};

			enum highway_type
			{
				facil_info_point	= 0x01000000,		// 施設情報ポイント
				access_point		= 0x02000000,		// アクセスポイント
				highway_node		= 0x04000000,		// ハイウェイノード
				highway_text		= 0x08000000		// ハイウェイテキスト
			};

			enum other_type
			{
				road_code_list		= 0x10000000,		// 路線コードリスト
				parking_shape		= 0x20000000		// 駐車場外形
			};
		};

		namespace relation
		{
			enum code
			{
				from				= 0x00000001,		// 始点
				to					= 0x00000002,		// 終点
				fore				= 0x00000010,		// 順方向
				back				= 0x00000020,		// 逆方向
				in					= 0x00000100,		// 内
				out					= 0x00000200,		// 外

				fromto				= (from|to),
				bothdir				= (fore|back),
				inout				= (in|out)
			};
		};

		extern long ROADDATA;
		extern long HWYDATA;
		extern long BASEROAD;
	};
};

#endif	//___SINDY_ROAD_NETWORK_H___
