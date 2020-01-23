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

#include <stdafx.h>
#include <sindy/schema/bus.h>

namespace sindy {

namespace schema {

namespace bus {
	
/// バス停ポイント
namespace bus_stop_point {
	const _TCHAR kTableName[] = _T("BUS_STOP_POINT"); //!< テーブル名

	const _TCHAR kStopID[] = _T("STOP_ID");        //!< 停留所コード
	const _TCHAR kPointAttrCode[] = _T("POINT_ATTR_CODE"); //!< バスポイント種別コード
	const _TCHAR kName[] = _T("STOP_NAME_KANJI");          //!< 停留所名
	const _TCHAR kYomi[] = _T("STOP_NAME_YOMI");          //!< 停留所名よみがな
	const _TCHAR kChangedName[] = _T("CHANGED_STOP_NAME_KANJI");   //!< 変更後停留所名
	const _TCHAR kChangedYomi[] = _T("CHANGED_STOP_NAME_YOMI");   //!< 変更後停留所名よみがな
	const _TCHAR kRemarks[] = _T("REMARKS");       //!< 名称変更備考
	const _TCHAR kPoleNo[] = _T("POLE_NO");        //!< 標柱番号
	const _TCHAR kGettingOffF[] = _T("GETTING_OFF_ONLY_F");   //!< 降車専用フラグ
}
/// バス停ルート探索用ポイント
namespace bus_route_point {
	const _TCHAR kTableName[] = _T("BUS_ROUTE_POINT"); //!< テーブル名

	const _TCHAR kBusStopPointID[] = _T("BUS_STOP_POINT_ID"); //!< バス停ポイントID
}
/// バス停車順管理
namespace bus_stop_seq {
	const _TCHAR kTableName[] = _T("BUS_STOP_SEQ"); //!< テーブル名

	const _TCHAR kBusLineMasterID[] = _T("BUS_LINE_MASTER_ID"); //!< 路線情報ID
	const _TCHAR kStopSeq[] = _T("STOP_SEQ");         //!< 順序番号
	const _TCHAR kBusStopPointID[] = _T("BUS_STOP_POINT_ID");  //!< バス停ポイントID
	const _TCHAR kPassF[] = _T("PASS_F");           //!< 通貨停留所フラグ
	const _TCHAR kDayCodes[] = _T("DAY_CODES");        //!< 経路曜日
	const _TCHAR kTimeCodes[] = _T("TIME_CODES");       //!< 経路時間帯
	const _TCHAR kRemarks[] = _T("REMARKS");         //!< 曜日時間備考
}
/// バス路線マスタ
namespace bus_line_master {
	const _TCHAR kTableName[] = _T("BUS_LINE_MASTER"); //!< テーブル名

	const _TCHAR kCompany[] = _T("COMPANY");            //!< 事業者名
	const _TCHAR kRouteNumber[] = _T("ROUTE_NUMBER");   //!< 系統番号
	const _TCHAR kRouteName[] = _T("ROUTE_NAME");       //!< 系統名
	const _TCHAR kRouteDetail[] = _T("ROUTE_DETAIL");   //!< 経由
	const _TCHAR kRouteRemarks[] = _T("ROUTE_REMARKS"); //!< 経由地等備考
}
}
}
}
