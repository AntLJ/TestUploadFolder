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

#ifndef SINDY_SCHEMA_TOLL_H_
#define SINDY_SCHEMA_TOLL_H_

#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// 一般有料の上下線区別があるテーブル
namespace normal_updown_table
{
	namespace updown_class
	{
		enum ECode
		{
			kUnsurveyed = -1,   ///< 未調査
			kDown       = 0,	///< 順方向(下り)
			kUp         = 1,	///< 逆方向(上り)
			kNonDivided = 2,	///< 上下線非分離
			kCommon     = 3,    ///< 上下線共通
		};
	}
}


/// 片側の料金額テーブル
namespace onesidetoll_table {
	extern const _TCHAR kKei_Toll[];		///< 二輪・軽料金
	extern const _TCHAR kHutuu_Toll[];		///< 普通車料金
	extern const _TCHAR kChuu_Toll[];		///< 中型車料金
	extern const _TCHAR kDai_Toll[];		///< 大型車料金
	extern const _TCHAR kTokudai_Toll[];	///< 特大車料金

	/// リンク方向コード
	namespace toll_category {
		enum ECode {
			kDummy		= -1, ///< 金額が未設定(その車種は通れない場合に付与される値)
			kDelete		= -2, ///< JHパッチの削除対象の値
		};
	}
}

/// 上下別の料金額テーブル
namespace updowntoll_table {
	extern const _TCHAR kKei_Toll_Up[];			///< 二輪・軽料金(上り)
	extern const _TCHAR kHutuu_Toll_Up[];		///< 普通車料金(上り)
	extern const _TCHAR kChuu_Toll_Up[];		///< 中型車料金(上り)
	extern const _TCHAR kDai_Toll_Up[];			///< 大型車料金(上り)
	extern const _TCHAR kTokudai_Toll_Up[];		///< 特大車料金(上り)
	
	extern const _TCHAR kKei_Toll_Down[];		///< 二輪・軽料金(下り)
	extern const _TCHAR kHutuu_Toll_Down[];		///< 普通車料金(下り)
	extern const _TCHAR kChuu_Toll_Down[];		///< 中型車料金(下り)
	extern const _TCHAR kDai_Toll_Down[];		///< 大型車料金(下り)
	extern const _TCHAR kTokudai_Toll_Down[];	///< 特大車料金(下り)
}

/// FromTo施設テーブル
namespace fromto_tolltable {
	extern const _TCHAR kRoadCode1[];		///< 路線コード1
	extern const _TCHAR kRoadSeq1[];		///< 路線内シーケンス1
	extern const _TCHAR kRoadCode2[];		///< 路線コード2
	extern const _TCHAR kRoadSeq2[];		///< 路線内シーケンス2
}

/// JH←→独自整備施設コード変換テーブル
namespace facil_conv_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kJHRoadCode[];	///< JH路線コード
	extern const _TCHAR kJHRoadSeq[];	///< JH路線内シーケンス
	extern const _TCHAR kMemo[];		///< フリーコメント
}

/// JH路線コードテーブル
namespace jh_road_code_list_table {
	extern const _TCHAR kJHDispCode[];		///< JH表示番号
	extern const _TCHAR kJHRoadCode[];		///< JH路線コード
	extern const _TCHAR kJHRoadName[];		///< JH路線名称
}

/// JH施設コードテーブル
namespace jh_facil_code_list_table {
	extern const _TCHAR kJHRoadCode[];		///< JH路線コード
	extern const _TCHAR kJHRoadSeq[];		///< JH路線内シーケンス
	extern const _TCHAR kJHFacilName[];		///< JH施設名称
}
/// JH料金テーブル
namespace jh_toll_table {
	using namespace ::sindy::schema::onesidetoll_table;

	extern const _TCHAR kJHRoadCode1[];		///< JH路線コード1
	extern const _TCHAR kJHRoadSeq1[];		///< JH路線内シーケンス1
	extern const _TCHAR kJHRoadCode2[];		///< JH路線コード2
	extern const _TCHAR kJHRoadSeq2[];		///< JH路線内シーケンス2
	extern const _TCHAR kDiscount_C[];		///< 割引条件コード

	extern const _TCHAR kReserved1[];		///< リザーブ1
	extern const _TCHAR kDistance[];		///< 施設間距離
}

/// JHパッチテーブル
namespace jh_patch_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< 割引条件コード

	extern const _TCHAR kTollMemo[];		///< メモ
}

/// 独自整備の発券・精算料金テーブル
namespace other_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;
	using namespace ::sindy::schema::normal_updown_table; // 上下線コードのコード値定義(bug 8304)

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< 割引条件コード

	extern const _TCHAR kNormal_F[];		///< 一般道フラグ
	extern const _TCHAR kUpdown_C1[];		///< From施設の上下線コード
	extern const _TCHAR kUpdown_C2[];		///< To施設の上下線コード

	extern const _TCHAR kTollMemo[];		///< メモ
}

/// 単独型料金テーブル
namespace alone_table {
	using namespace ::sindy::schema::updowntoll_table;
	using namespace ::sindy::schema::normal_updown_table; // 上下線コードのコード値定義(bug 8304)
	
	extern const _TCHAR kTableName[];

	extern const _TCHAR kLineClass_C[];		///< 路線種別コード
	extern const _TCHAR kRoadCode[];		///< 路線コード
	extern const _TCHAR kRoadSeq[];			///< 路線内シーケンス
	
	extern const _TCHAR kUpdown_C[];		///< 上下線コード
	extern const _TCHAR kDiscount_C[];		///< 割引条件コード
	extern const _TCHAR kStdLinkClass_C[];	///< 標準料金路線種別コード
	extern const _TCHAR kStdRoadCode[];		///< 標準料金路線コード

	extern const _TCHAR kNormal_F[];		///< 一般道フラグ
}

/// 入れ子料金情報テーブル
namespace nested_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< 割引条件コード

	extern const _TCHAR kTollMemo[];		///< メモ
}

/// 併設施設テーブル
namespace samefacil_table {
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kAlone_F[];			///< 単独施設フラグ

	extern const _TCHAR kTollMemo[];		///< メモ
}

/// 単独料金所無効券発券施設テーブル
namespace free_table {
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];
}

/// 暫定料金施設テーブル
namespace temp_facil_table {
	extern const _TCHAR kTableName[];

	extern const _TCHAR kRoadCode[];		///< 路線コード
	extern const _TCHAR kRoadSeq[];			///< 路線内シーケンス
	extern const _TCHAR kDiscount_C[];		///< 割引条件コード

	extern const _TCHAR kTempTollClass[];	///< 暫定料金種別

	extern const _TCHAR kTollMemo[];		///< メモ
}

/// 路線コードリストテーブル(追加分)
namespace road_code_list {
	extern const _TCHAR kLandClass[];		///< 地域種別フラグ

	/// 路線種別コード定義
	namespace land_class {
		enum ECode {
			kUnknown	 = 0, ///< 未調査
			kHokkaido	 = 1, ///< 北海道
			kHonshu		 = 2, ///< 本州・四国・九州
			kOkinawa	 = 3, ///< 沖縄
			kIndependent = 4, ///< 独立路線
		};
	}
}

/// 割引条件・割引時間帯条件共通部分
namespace toll_discount_common {
	extern const _TCHAR kEtcDiscount[];		///< ETC割引条件コード
	extern const _TCHAR kTollMemo[];		///< フリーコメント	
}

/// 割引条件テーブル
namespace toll_discount_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::toll_discount_common;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscountName[];	///< 割引名称
	extern const _TCHAR kManagerName[];		///< 管理会社名称
	extern const _TCHAR kEtc[];				///< ETC専用フラグ
	extern const _TCHAR kTypeKei[];			///< 車種フラグ_軽
	extern const _TCHAR kTypeHutuu[];		///< 車種フラグ_普通
	extern const _TCHAR kTypeChuu[];		///< 車種フラグ_中型
	extern const _TCHAR kTypeDai[];			///< 車種フラグ_大型
	extern const _TCHAR kTypeTokuDai[];		///< 車種フラグ_特大	
	extern const _TCHAR kOverStart[];		///< 開始跨ぎ有効フラグ
	extern const _TCHAR kOverEnd[];			///< 終了跨ぎ有効フラグ
	extern const _TCHAR kOverAll[];			///< 全跨ぎ有効フラグ
	extern const _TCHAR kRestriction[];		///< 制限回数
	extern const _TCHAR kCloseDate[];		///< 終了予定年月日		
}

/// 割引時間帯条件テーブル
namespace toll_discounttime_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::toll_discount_common;


	extern const _TCHAR kTableName[];

	extern const _TCHAR kWeekWeekday[];		///< 曜日フラグ_平日
	extern const _TCHAR kWeekSaturday[];	///< 曜日フラグ_土曜
	extern const _TCHAR kWeekSunday[];		///< 曜日フラグ_日曜
	extern const _TCHAR kWeekHoliday[];		///< 曜日フラグ_祝日
	extern const _TCHAR kTermStartMonth[];	///< 期間開始月
	extern const _TCHAR kTermStartDay[];	///< 期間開始日
	extern const _TCHAR kTermEndMonth[];	///< 期間終了月
	extern const _TCHAR kTermEndDay[];		///< 期間終了日
	extern const _TCHAR kStartHour[];		///< 時間帯開始時
	extern const _TCHAR kStartMin[];		///< 時間帯開始分
	extern const _TCHAR kEndHour[];			///< 時間帯終了時
	extern const _TCHAR kEndMin[];			///< 時間帯終了分
}

} // namespace schema

} // namesaoce snindy

#endif // SINDY_SCHEMA_TOLL_H_
