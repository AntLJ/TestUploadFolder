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

// KKDCodeCommunity.h: KKDCodeCommunity クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_)
#define AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DRMACodeCommunity.h>
#include "KKDVersion.h"

/// KKDコード名前空間
namespace kkd {
	/// KKDコード一覧
	namespace codes {
		/// レコードIDコード
		namespace a {
			enum ECode {
				kAdminId                          = 16, ///< 管理データ
				kRegulationRoadNodeId             = 71, ///< 交通規制道路ノードデータ
				kRegulationRoadLinkId             = 72, ///< 交通規制道路リンクデータ
				kLaneRegulationRoadNodeId         = 73, ///< レーン規制道路ノードデータ
				kLaneRegulationRoadLinkId         = 74, ///< レーン規制道路リンクデータ
				kLaneRegulationIntersectionNodeId = 75, ///< レーン規制集合交差点ノードデータ
			};
		}

		/// 交差点ノード種別コード
		namespace b {
			enum ECode {
				kRealCross  = 1, ///< 実交差点
				kDummyCross = 2, ///< ダミー交差点
				kDummyNode  = 3, ///< ダミーノード
			};

			const char* kDomains[];
		}

		/// 交差点通行コード
		namespace c {
			enum ECode {
				kUnknown               = 0, ///< 未調査
				kPassable              = 1, ///< 通行可
				kNoPassage             = 2, ///< 通行不可
				kNoPassageFromRoadCond = 3, ///< 道路条件により通行不可
				kNoPassageWithCond     = 7, ///< 条件付き通行不可
			};

			const char* kDomains[];
		}

		/// 交通規制種別コード
		namespace m {
			using namespace ::drma::codes::m;
		}

		/// 該当有無コード
		namespace y {
			using namespace ::drma::codes::y;
		}

		/// 車種コード
		namespace car_type {
			const char* kDomains[];
		}

		/// 曜日コード
		namespace weekday {
			const char* kDomains[];
		}

		/// 交通規制レーンコード
		namespace regulation_lane {
			const char* kDomains[];
		}

#if KKD_VERSION >= 101
/**
 * @ingroup kkd_std_1_1
 */
//@{
		/// 交差点名称漢字調査レベルコード
		namespace intersection_name_kanji_research_level {
			enum ECode {
				kNoName   = 0, ///< 交差点名称なし
				kOnSignal = 1, ///< 信号灯器の上下左右、または、信号支柱に設置の看板に記載されている
				kAlone    = 2, ///< 交差点付近の歩道橋に記載のものや交差点中央に独立して立てている
			};
		}

		/// 交差点名称カナ調査レベルコード
		namespace intersection_name_kana_research_level {
			enum ECode {
				kNoKana       = 0, ///< カナなし
				kFromKKD      = 1, ///< KKDを参考にカナを設定したもの
				kFromAlphabet = 2, ///< 交差点名称板に記載のアルファベットを参考にカナを設定したもの
			};
		}
//@}
	}
#endif // KKD_VERSION >= 101

	namespace regulation_road_node {
		extern const int kRecordId;
	
		namespace intersection_class {
			using namespace ::kkd::codes::b;
		}

		namespace regulation_lane {
			using namespace ::kkd::codes::regulation_lane;
		}

		namespace passage {
			using namespace ::kkd::codes::c;
		}

		namespace car_type {
			using namespace ::kkd::codes::car_type;
		}

		namespace weekday {
			using namespace ::kkd::codes::weekday;
		}

#if KKD_VERSION >= 101
/**
 * @ingroup kkd_std_1_1
 */
//@{
		namespace intersection_name_kanji_research_level {
			using namespace ::kkd::codes::intersection_name_kanji_research_level;
		}

		namespace intersection_name_kana_research_level {
			using namespace ::kkd::codes::intersection_name_kana_research_level;
		}
//@}
	}
#endif // KKD_VERSION >= 101

	namespace lane_regulation_road_node {
		extern const int kRecordId;
	
		namespace intersection_class {
			using namespace ::kkd::codes::b;
		}

		namespace regulation_lane {
			using namespace ::kkd::codes::regulation_lane;
		}

		namespace passage {
			using namespace ::kkd::codes::c;
		}

		namespace car_type {
			using namespace ::kkd::codes::car_type;
		}

		namespace weekday {
			using namespace ::kkd::codes::weekday;
		}
	}

	using namespace codes::a;
	typedef ECode ERecordId;
}

#endif // !defined(AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_)
