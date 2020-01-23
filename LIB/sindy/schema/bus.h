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

namespace sindy {

namespace schema {

namespace bus {
	
/// バス停ポイント
namespace bus_stop_point {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kStopID[];        //!< 停留所コード
	extern const _TCHAR kPointAttrCode[]; //!< バスポイント種別コード
	extern const _TCHAR kName[];          //!< 停留所名
	extern const _TCHAR kYomi[];          //!< 停留所名よみがな
	extern const _TCHAR kChangedName[];   //!< 変更後停留所名
	extern const _TCHAR kChangedYomi[];   //!< 変更後停留所名よみがな
	extern const _TCHAR kRemarks[];       //!< 名称変更備考
	extern const _TCHAR kPoleNo[];        //!< 標柱番号
	extern const _TCHAR kGettingOffF[];   //!< 降車専用フラグ

	/// バスポイント種別コード
	namespace point_attr_code {
		enum ECode {
			kPole         = 1, //!< 停留所標柱ポイント
			kRoute        = 5, //!< 経路中間ポイント
			kNationalLand = 9, //!< 国土数値情報ポイント
		};
	}
}
/// バス停ルート探索用ポイント
namespace bus_route_point {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kBusStopPointID[]; //!< バス停ポイントID
}
/// バス停車順管理
namespace bus_stop_seq {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kBusLineMasterID[]; //!< 路線情報ID
	extern const _TCHAR kStopSeq[];         //!< 順序番号
	extern const _TCHAR kBusStopPointID[];  //!< バス停ポイントID
	extern const _TCHAR kPassF[];           //!< 通貨停留所フラグ
	extern const _TCHAR kDayCodes[];        //!< 経路曜日
	extern const _TCHAR kTimeCodes[];       //!< 経路時間帯
	extern const _TCHAR kRemarks[];         //!< 曜日時間備考
}
/// バス路線マスタ
namespace bus_line_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kCompany[];      //!< 事業者名
	extern const _TCHAR kRouteNumber[];  //!< 系統番号
	extern const _TCHAR kRouteName[];    //!< 系統名
	extern const _TCHAR kRouteDetail[];  //!< 経由
	extern const _TCHAR kRouteRemarks[]; //!< 経由地等備考
}
}
}
}
