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
#include <sindy/schema/poi.h>

namespace sindy {

namespace schema {

namespace railway_corp {
	const TCHAR kTableName[]		= _T("RAILWAY_CORP");		//!< テーブル名
	const TCHAR kCorpCode[]			= _T("CORPCODE");			//!< 会社コード
	const TCHAR kName[]				= _T("NAME");				//!< 会社名
	const TCHAR kYomi[]				= _T("YOMI");				//!< 会社名読み
	const TCHAR kExName[]			= _T("EXNAME");				//!< 会社略称
	const TCHAR kExYomi[]			= _T("EXYOMI");				//!< 会社略称読み
} // railway_corp

namespace railway_line {
	const TCHAR kTableName[]		= _T("RAILWAY_LINE");		//!< テーブル名
	const TCHAR kCorpID[]			= _T("CORPID");				//!< 鉄道会社ＩＤ
	const TCHAR kLineCode[]			= _T("LINECODE");			//!< 路線コード
	const TCHAR kName[]				= _T("NAME");				//!< 路線名
	const TCHAR kYomi[]				= _T("YOMI");				//!< 路線名読み
	const TCHAR kExName[]			= _T("EXNAME");				//!< 路線略称
	const TCHAR kExYomi[]			= _T("EXYOMI");				//!< 路線略称読み
	const TCHAR kTypeID[]			= _T("TYPEID");				//!< 種別ＩＤ
} // railway_line

namespace join_line_info {
	const TCHAR kTableName[]	= _T("JOIN_LINE_INFO");		//!< テーブル名
	const TCHAR kLineID[]		= _T("LINEID");				//!< 路線ID
	const TCHAR kLineSeq[]		= _T("LINESEQ");			//!< 路線内シーケンス
	const TCHAR kInfoID[]		= _T("INFOID");				//!< 駅情報ID
	const TCHAR kSubName[]		= _T("SUBNAME");			//!< 補足名称
	const TCHAR kSubYomi[]		= _T("SUBYOMI");			//!< 補足名称読み
	const TCHAR kStationID[]	= _T("STATIONID");			//!< 駅ポイントID
	const TCHAR kContentsSeq[]	= _T("CONTENTS_SEQ");		//!< コンテンツ内シーケンス
} // join_line_info

namespace station_info {
	const TCHAR kTableName[]	= _T("STATION_INFO");	//!< テーブル名
	const TCHAR kCorpID[]		= _T("CORPID");			//!< 鉄道会社ID
	const TCHAR kName[]			= _T("NAME");			//!< 駅名
	const TCHAR kYomi[]			= _T("YOMI");			//!< 駅名読み
	const TCHAR kAddrCode[]		= _T("ADDRCODE");		//!< 住所コード
	const TCHAR kTel[]			= _T("TEL");			//!< 電話番号
	const TCHAR kAnAreaCode[]	= _T("ANAREACODE");		//!< 市外局番
} // station_info

namespace join_station_gate {
	const TCHAR kTableName[]	= _T("JOIN_STATION_GATE");		//!< テーブル名
	const TCHAR kJLineID[]		= _T("JLINEID");				//!< 乗り入れ路線ID
	const TCHAR kGateID[]		= _T("GATEID");					//!< 駅出入口ID
	const TCHAR kContentsSeq[]	= _T("CONTENTS_SEQ");			//!< コンテンツ内シーケンス
} // join_station_gate

namespace station_point {
	const TCHAR kTableName[]		= _T("STATION_POINT");		//!< テーブル名	
	const TCHAR kGuideID[]			= _T("GUIDEID");			//!< 誘導ポイントID
} // station_point

namespace station_gate_point {
	const TCHAR kTableName[]		= _T("STATION_GATE_POINT");	//!< テーブル名

	const TCHAR kName[]				= _T("NAME");				//!< 出入口名称
	const TCHAR kYomi[]				= _T("YOMI");				//!< 出入口名称読み	
	const TCHAR kGuideID[]			= _T("GUIDEID");			//!< 誘導ポイントID
} // station_gate_point

namespace station_guide_point {
	const TCHAR kTableName[]		= _T("STATION_GUIDE_POINT");	//!< テーブル名
	const TCHAR kTollRoad[]			= _T("TOLLROAD_F");				//!< 有料道路フラグ
} // station_guide_point

namespace join_station_walk {
	const TCHAR kTableName[]	= _T("JOIN_STATION_WALK");		//!< テーブル名
	const TCHAR kJLineID[]		= _T("JLINEID");				//!< 乗り入れ路線ID
	const TCHAR kWalkPoiID[]	= _T("WALKPOIID");				//!< 駅出入口ID
} // join_station_gate

namespace walk_poi_point {
	const TCHAR kTableName[]		= _T("WALK_POI_POINT");	//!< テーブル名

	const TCHAR kPOIClass[]		= _T("POICLASS_C");		//!< 歩行者POI種別
	const TCHAR kName[]			= _T("NAME");			//!< 名称
	const TCHAR kYomi[]			= _T("YOMI");			//!< 名称読み
	const TCHAR kFloorLevel[]		= _T("FLOOR_LEVEL");	//!< 階層値
	const TCHAR kIndoor[]			= _T("INDOOR_F");		//!< 施設内フラグ
	const TCHAR kPicName[]			= _T("PICNAME");		//!< 画像ファイル名
} // walk_poi_point

namespace poi_mstchn {
	const _TCHAR kTableName[]		= _T("POI_MSTCHN");			//!< テーブル名

	const _TCHAR kChainCode[]		= _T("CHAINCODE");			//!< チェーン店マスターコード（8桁）
	const _TCHAR kChainCodeOld[]	= _T("CHAINCODE_OLD");		//!< チェーン店マスターコード（6桁）
	const _TCHAR kName[]			= _T("NAME");				//!< 店舗名称
	const _TCHAR kYomi[]			= _T("YOMI");				//!< 店舗名称読み
} // poi_mstchn

namespace traintype {
	const _TCHAR kTableName[] = _T("TRAINTYPE"); //!< テーブル名

	const _TCHAR kTypeCode[] = _T("TYPECODE"); //!<種別コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kYomi[] = _T("YOMI"); //!< 読み
} // traintype

} // schema

} // sindy
