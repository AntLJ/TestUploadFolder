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

#ifndef SINDY_SCHEMA_TIMESPEED_H_
#define SINDY_SCHEMA_TIMESPEED_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// 時間帯別速度テーブル
namespace timespeed_table {
	extern const _TCHAR kLinkID[]; ///< 道路リンクID
	extern const _TCHAR kLinkDir[]; ///< 道路リンク方向
	extern const _TCHAR kHoliday[]; ///< 休日フラグ
	extern const _TCHAR kTimeGroup[]; ///< 時間帯コード

	/// 道路リンク方向コード(sindy::schema::category::directional_link_relational_tableとは異なるため注意)
	namespace linkdir {
		enum ECode {
			kReverse           =   0, ///<  逆方向
			kForward           =   1, ///<  正方向
		};
	}

	/// 時間帯コード
	namespace timezone_class {
		enum ECode {
			kFrom01To03           =   0, ///<  1時～ 3時
			kFrom04To06           =   1, ///<  4時～ 6時
			kFrom07To09           =   2, ///<  7時～ 9時
			kFrom10To12           =   3, ///< 10時～12時
			kFrom13To15           =   4, ///< 13時～15時
			kFrom16To18           =   5, ///< 16時～18時
			kFrom19To21           =   6, ///< 19時～21時
			kFrom22To24           =   7, ///< 22時～24時
		};
	}
}

/// 時間帯別リンク旅行速度テーブル
namespace timezone_speed {
	using namespace ::sindy::schema::timespeed_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTripSpeed_M[]; ///< 平均旅行速度（補正前）
	extern const _TCHAR kTripSpeed_R[]; ///< 平均旅行速度（補正後）
	extern const _TCHAR kSampleCount[]; ///< 累積サンプル数
	extern const _TCHAR kNewestDate[]; ///< 最新実績値採用日時
	extern const _TCHAR kOldestDate[]; ///< 最古実績値採用日時
	extern const _TCHAR kRenewCount[]; ///< 更新回数
	extern const _TCHAR kAdopt[]; ///< データ識別コード

	/// データ識別コード
	namespace adopt_class {
		enum ECode {
			kMasured          =   10, ///< 実績値
			kCalculated       =   20, ///< 予測値
		};
	}
}

/// プローブ集計速度テーブル
namespace timezone_speed_probe {
	using namespace ::sindy::schema::timespeed_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTripSpeed[]; ///< 平均旅行速度
	extern const _TCHAR kSampleCount[]; ///< 累積サンプル数
}

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_TIMESPEED_H_
