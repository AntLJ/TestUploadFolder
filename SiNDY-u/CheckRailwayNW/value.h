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

#include "stdafx.h"

namespace judge_value {

	using namespace sindy::schema;
	using namespace sindy::schema::sj;

	inline CString IsFindObj( long key, const std::map<long, CString>& objMap )
	{
		CString retStr;
		retStr.Format( _T("%ld"), key );
		auto itr = objMap.find( key );
		if( itr != objMap.end() )
			retStr.AppendFormat( _T("('%s')"), itr->second );
		
		else
			retStr.AppendFormat( _T("('<null>')"));

		return retStr;
	};

	inline bool IsDefineRailLinkClass( long railLinkClass )
	{
		using namespace railway_link::link_class;
		
		switch( railLinkClass ){

			case kDivideByPoi: //!< 鉄道POI種別で分類
			case kFreightLine: //!< 貨物線
			case kSideTrack:   //!< 鉄道側線部
				return true;

			default:
				return false;
		}	
	};

	inline bool IsDefineDispScale( long dispScale )
	{
		using namespace railway_link::disp_scale;

		switch( dispScale ){

			case kS1toS4: //!< S1～S4で表示
			case kS2toS4: //!< S2～S4で表示
			case kS3toS4: //!< S3～S4で表示
			case k4:      //!< S4で表示
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKFlag( const CString& flag )
	{
		if( 0 == flag.CompareNoCase( _T("0") ) || 0 == flag.CompareNoCase( _T("1") ) )
			return true;
		return false;
	};

	inline bool IsSameRailLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	{
		using namespace railway_link;

		try{
			if( 0 != geoClass1.attr.at( kRailwayClass_C ).CompareNoCase( geoClass2.attr.at( kRailwayClass_C ) ) ) return false;
			if( 0 != geoClass1.attr.at( kDispScale_C    ).CompareNoCase( geoClass2.attr.at( kDispScale_C    ) ) ) return false;
			if( 0 != geoClass1.attr.at( kUnderGround_F  ).CompareNoCase( geoClass2.attr.at( kUnderGround_F  ) ) ) return false;

			return true;
		}
		catch( ... ){
			return false;
		}
	};

	inline CString GetDiffRailLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	{
		using namespace railway_link;

		try{
			if( 0 != geoClass1.attr.at( kRailwayClass_C ).CompareNoCase( geoClass2.attr.at( kRailwayClass_C ) ) ) return kRailwayClass_C;
			if( 0 != geoClass1.attr.at( kDispScale_C    ).CompareNoCase( geoClass2.attr.at( kDispScale_C    ) ) ) return kDispScale_C;
			if( 0 != geoClass1.attr.at( kUnderGround_F  ).CompareNoCase( geoClass2.attr.at( kUnderGround_F  ) ) ) return kUnderGround_F;

			return _T("");
		}
		catch( ... ){
			return _T("");
		}
	};

	inline bool IsDefineRailNodeClass( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kNone:     //!< 機能なしノード
			case kTerminal: //!< 端点ノード
			case kStation:  //!< 駅POIノード
			case kAlter:    //!< 属性変化点ノード
			case kBranch:   //!< 分岐ノード
			case kOutline:  //!< 図郭上ノード
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKTerminalNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kTerminal: //!< 端点ノード
			case kStation:  //!< 駅POIノード
				return true;

			case kNone:     //!< 機能なしノード
			case kAlter:    //!< 属性変化点ノード
			case kBranch:   //!< 分岐ノード
			case kOutline:  //!< 図郭上ノード
			default:
				return false;
		}
	};

	inline bool IsOKBranchNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kBranch:   //!< 分岐ノード
			case kStation:  //!< 駅POIノード
				return true;

			case kNone:     //!< 機能なしノード
			case kAlter:    //!< 属性変化点ノード
			case kTerminal: //!< 端点ノード
			case kOutline:  //!< 図郭上ノード
			default:
				return false;
		}
	};

	inline bool IsOKAlterNode( long railNodeClass )
	{
		using namespace railway_node::node_class;

		switch( railNodeClass ){
			case kAlter:    //!< 属性変化点ノード
			case kBranch:   //!< 分岐ノード
			case kStation:  //!< 駅POIノード
				return true;

			case kNone:     //!< 機能なしノード
			case kOutline:  //!< 図郭上ノード
			case kTerminal: //!< 端点ノード
			default:
				return false;
		}
	}



	inline long GetLongValue( const GeoClass& geoClass, const CString& field )
	{
		try{
			auto& cStrValue = geoClass.attr.at( field );
			if( cStrValue.IsEmpty() )
				return -1;
			long retValue = _ttol( cStrValue );
			return retValue;
		}
		catch( ... ){
			return -1;
		}
	}


	//inline bool IsDefineWalkable( long walkable )
	//{
	//	using namespace road_link::walkable_class;

	//	switch( walkable ){
	//		case kNotWalk:   ///< 歩行禁止
	//		case kHumanCar:  ///< 人車共用
	//		case kWalkOnly:  ///< 歩行者専用
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsWalkOnly( long walkable )
	//{
	//	using namespace road_link::walkable_class;

	//	switch( walkable ){
	//		case kWalkOnly:  ///< 歩行者専用
	//			return true;
	//		case kNotWalk:   ///< 歩行禁止
	//		case kHumanCar:  ///< 人車共用
	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsSameRoadLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace road_link;

	//	try{
	//		if( 0 != geoClass1.attr.at( kRoadClass     ).CompareNoCase( geoClass2.attr.at( kRoadClass     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kNaviClass     ).CompareNoCase( geoClass2.attr.at( kNaviClass     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kMainLinkClass ).CompareNoCase( geoClass2.attr.at( kMainLinkClass ) ) ) return false;

	//		return true;
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};
	//
	//inline CString IsSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace walk_link;
	//	
	//	try{
	//		if( 0 != geoClass1.attr.at( kWalkClass      ).CompareNoCase( geoClass2.attr.at( kWalkClass      ) ) ) return kWalkClass;
	//		if( 0 != geoClass1.attr.at( kFloorMoveClass ).CompareNoCase( geoClass2.attr.at( kFloorMoveClass ) ) ) return kFloorMoveClass;
	//		if( 0 != geoClass1.attr.at( kAutoWalkFlag   ).CompareNoCase( geoClass2.attr.at( kAutoWalkFlag   ) ) ) return kAutoWalkFlag;
	//		if( 0 != geoClass1.attr.at( kStairFlag      ).CompareNoCase( geoClass2.attr.at( kStairFlag      ) ) ) return kStairFlag;
	//		if( 0 != geoClass1.attr.at( kSlopeFlag      ).CompareNoCase( geoClass2.attr.at( kSlopeFlag      ) ) ) return kSlopeFlag;
	//		if( 0 != geoClass1.attr.at( kEscalatorFlag  ).CompareNoCase( geoClass2.attr.at( kEscalatorFlag  ) ) ) return kEscalatorFlag;
	//		if( 0 != geoClass1.attr.at( kArcadeFlag     ).CompareNoCase( geoClass2.attr.at( kArcadeFlag     ) ) ) return kArcadeFlag;
	//	//	if( 0 != geoClass1.attr.at( kOneway         ).CompareNoCase( geoClass2.attr.at( kOneway         ) ) ) return kOneway;
	//		if( 0 != geoClass1.attr.at( kOnewayRF       ).CompareNoCase( geoClass2.attr.at( kOnewayRF       ) ) ) return kOnewayRF;
	//		if( 0 != geoClass1.attr.at( kNoPassage      ).CompareNoCase( geoClass2.attr.at( kNoPassage      ) ) ) return kNoPassage;
	//		if( 0 != geoClass1.attr.at( kNoPassageRF    ).CompareNoCase( geoClass2.attr.at( kNoPassageRF    ) ) ) return kNoPassageRF;
	//		if( 0 != geoClass1.attr.at( kSubWalkClass   ).CompareNoCase( geoClass2.attr.at( kSubWalkClass   ) ) ) return kSubWalkClass;
	//		if( 0 != geoClass1.attr.at( kRoadClass      ).CompareNoCase( geoClass2.attr.at( kRoadClass      ) ) ) return kRoadClass;
	//		if( 0 != geoClass1.attr.at( kRoadNo         ).CompareNoCase( geoClass2.attr.at( kRoadNo         ) ) ) return kRoadNo;
	//		if( 0 != geoClass1.attr.at( kRoadCode       ).CompareNoCase( geoClass2.attr.at( kRoadCode       ) ) ) return kRoadCode;

	//		return _T("");
	//	}
	//	catch( ... ){
	//		return _T("");
	//	}
	//};

	//inline bool IsSameOneway( const GeoClass& geoClass1, const GeoClass& geoClass2, long nodeID )
	//{
	//	using namespace walk_link;
	//	using namespace walk_link::oneway;

	//	// 条件付きはないため、考慮しない
	//	
	//	try{
	//		long onewayC1 = _ttol(geoClass1.attr.at( kOneway ));
	//		long onewayC2 = _ttol(geoClass2.attr.at( kOneway ));
	//		if( onewayC1 == kNoOneway && onewayC2 == kNoOneway )
	//			return true;

	//		bool bFrom1 = false, bFrom2 = false;
	//		CString strNodeID;
	//		strNodeID.Format( _T("%ld"), nodeID );
	//		if( 0 == geoClass1.attr.at( kFromNodeID ).CompareNoCase( strNodeID ) )
	//			bFrom1 = true;
	//		else if( 0 != geoClass1.attr.at( kToNodeID ).CompareNoCase( strNodeID ) )
	//			return false; // From/Toとも一致しなければOUT（ないはず）

	//		if( 0 == geoClass2.attr.at( kFromNodeID ).CompareNoCase( strNodeID ) )
	//			bFrom2 = true;
	//		else if( 0 != geoClass2.attr.at( kToNodeID ).CompareNoCase( strNodeID ) )
	//			return false; // From/Toとも一致しなければOUT（ないはず）
	//		
	//		if( ( bFrom1 && ! bFrom2 ) || ( ! bFrom1 && bFrom2 ) ){ // From・Toで正順の場合（同じ向きならOK）

	//			// 正・正 or 逆・逆 ならOK
	//			if( ( onewayC1 == kFore && onewayC2 == kFore ) || ( onewayC1 == kReverse && onewayC2 == kReverse ) )
	//				return true;

	//			return false;
	//		}
	//		else{ // From・Toで逆順の場合（逆向きならOK）

	//			// 正・逆 or 逆・正 ならOK
	//			if( ( onewayC1 == kFore && onewayC2 == kReverse ) || ( onewayC1 == kReverse && onewayC2 == kFore ) )
	//				return true;

	//			return false;
	//		}
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};

	//inline bool IsSimpleSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2 )
	//{
	//	using namespace walk_link;
	//	try{
	//		if( 0 != geoClass1.attr.at( kWalkClass      ).CompareNoCase( geoClass2.attr.at( kWalkClass      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kAutoWalkFlag   ).CompareNoCase( geoClass2.attr.at( kAutoWalkFlag   ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kStairFlag      ).CompareNoCase( geoClass2.attr.at( kStairFlag      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kSlopeFlag      ).CompareNoCase( geoClass2.attr.at( kSlopeFlag      ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kEscalatorFlag  ).CompareNoCase( geoClass2.attr.at( kEscalatorFlag  ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kArcadeFlag     ).CompareNoCase( geoClass2.attr.at( kArcadeFlag     ) ) ) return false;
	//		if( 0 != geoClass1.attr.at( kSubWalkClass   ).CompareNoCase( geoClass2.attr.at( kSubWalkClass   ) ) ) return false;

	//		return true;
	//	}
	//	catch( ... ){
	//		return false;
	//	}
	//};

	//inline bool IsDefineWalkLinkClass( long walkLinkClassCode )
	//{
	//	using namespace walk_link::walk_class;

	//	// スキーマ定義上のものは全てOK
	//	switch( walkLinkClassCode ){
	//		case kNone:          ///< 設定なし
	//		case kWalkRoad:      ///< 歩道
	//		case kOtherWalkRoad: ///< その他歩道	 
	//		case kCrossRoad:     ///< 横断歩道
	//		case kWalkBridge:    ///< 歩道橋
	//		case kPedestrian:    ///< ぺデストリアンデッキ
	//		case kConnect:       ///< 接続リンク
	//		case kHumanCar:      ///< 人車共用リンク
	//		case kStation:       ///< 駅構内
	//		case kIndoor:        ///< 施設内（屋内）
	//		case kUnderGround:   ///< 地下通路
	//		case kOutdoor:       ///< 施設敷地内（屋外）
	//		case kPark:          ///< 公園
	//		case kOpenspace:     ///< 広場
	//		case kRailCrossing:  ///< 踏切
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsOKWalkLinkClass( long walkLinkClassCode )
	//{
	//	using namespace walk_link::walk_class;

	//	// 使用するもののみOK
	//	switch( walkLinkClassCode ){
	//		case kWalkRoad:      ///< 歩道	 
	//		case kCrossRoad:     ///< 横断歩道
	//		case kWalkBridge:    ///< 歩道橋
	//		case kHumanCar:      ///< 人車共用リンク
	//		case kIndoor:        ///< 施設内（屋内）
	//		case kUnderGround:   ///< 地下通路
	//		case kOutdoor:       ///< 施設敷地内（屋外）
	//		case kRailCrossing:  ///< 踏切
	//			return true;

	//		case kNone:          ///< 設定なし
	//		case kOtherWalkRoad: ///< その他歩道	 
	//		case kPedestrian:    ///< ぺデストリアンデッキ
	//		case kConnect:       ///< 接続リンク
	//		case kStation:       ///< 駅構内
	//		case kPark:          ///< 公園
	//		case kOpenspace:     ///< 広場
	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineFloorMoveClass( long floorMoveClass )
	//{
	//	using namespace walk_link::floor_move_class;

	//	// スキーマ定義上のものは全てOK
	//	switch( floorMoveClass ){
	//		case kNone:      ///< 上下無し
	//		case kForeUp:    ///< 上り(正方向)
	//		case kReverseUp: ///< 上り(逆方向)
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineOnewayCode( long onewayCode )
	//{
	//	using namespace walk_link::oneway;

	//	// スキーマ定義上のものは全てOK
	//	switch( onewayCode ){
	//		case kNoOneway:           ///< 一方通行なし
	//		case kFore:               ///< 条件なし一方通行(正方向)
	//		case kReverse:            ///< 条件なし一方通行(逆方向)
	//		case kForeWithReg:        ///< 条件付き一方通行(正方向)
	//		case kReverseWithReg:     ///< 条件付き一方通行(逆方向)
	//		case kForeReverseWithReg: ///< 条件付き一方通行(正逆)
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineNoPassCode( long noPassCode )
	//{
	//	using namespace walk_link::nopassage;

	//	// スキーマ定義上のものは全てOK
	//	switch( noPassCode ){
	//		case kPassable:           ///< 通行禁止なし
	//		case kNoPassageWithReg:   ///< 条件付き通行禁止
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineSubWalkClass( long subWalkClass )
	//{
	//	using namespace walk_link::subwalk_class;

	//	// スキーマ定義上のものは全てOK
	//	switch( subWalkClass ){
	//		case kNone:   ///< なし
	//		case kWatari: ///< 渡りリンク
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineWalkRoadClass( long walkRoadClass )
	//{
	//	using namespace walk_link::walk_road_class;

	//	// スキーマ定義上のものは全てOK
	//	switch( walkRoadClass ){
	//		case kNone:                 ///< 表示種別なし
	//		case kCountry:              ///< 一般国道
	//		case kPrefectureMain:       ///< 主要地方道（都府県）
	//		case kCityMain:             ///< 主要地方道（指定市）
	//		case kPrefectureNormal:     ///< 一般都道府県道
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsDefineRoadNo( long roadNo )
	//{
	//	// 0～9999がOK
	//	if( 0 <= roadNo && roadNo <= 9999 )
	//		return true;
	//	
	//	return false;
	//};

	//inline bool IsDefineRoadCode( long roadCode )
	//{
	//	// 0(初期値) or 1000～999999がOK
	//	if( roadCode == 0 || ( 1000 <= roadCode && roadCode <= 999999 ) )
	//		return true;

	//	return false;
	//};

	//inline bool IsOKWalkRoadClassAndRoadNo( long walkRoadClass, long roadNo )
	//{
	//	using namespace walk_link::walk_road_class;
	//	
	//	switch( walkRoadClass ){
	//		case kCountry:              ///< 一般国道
	//		case kPrefectureMain:       ///< 主要地方道（都府県）
	//		case kCityMain:             ///< 主要地方道（指定市）
	//		case kPrefectureNormal:     ///< 一般都道府県道
	//			if( 0 < roadNo && roadNo <= 9999 )
	//				return true;
	//			
	//			return false;
	//			
	//		case kNone:                 ///< 表示種別なし
	//		default:
	//			if( 0 < roadNo && roadNo <= 9999 )
	//				return false;

	//			return true;
	//	}
	//};

	//inline bool IsOKFloorMoveClassAndFlag( long floorMoveClass, const CString& flag )
	//{
	//	using namespace walk_link::floor_move_class;

	//	if( 0 == flag.CompareNoCase( _T("1") ) ){
	//		switch( floorMoveClass ){
	//		case kForeUp:    ///< 上り(正方向)
	//		case kReverseUp: ///< 上り(逆方向)
	//			return true;

	//		case kNone:
	//		default:
	//			return false;
	//		}
	//	}
	//	return true;
	//};



	//inline bool IsDefineWalkNodeClass( long walkNodeClass )
	//{
	//	using namespace walk_node::node_class;

	//	// スキーマ定義上のものは全てOK
	//	switch( walkNodeClass ){
	//		case kPublic:    ///< 一般ノード
	//		case kTerminal:  ///< 端点ノード
	//		case kAlter:     ///< 属性遷移点ノード
	//		case kRoadNW:    ///< 道路ＮＷ接続ノード
	//		case kGate:      ///< 駅出入口接続ノード
	//		case kUnderGate: ///< 地下通路出入口ノード
	//		case kElevator:  ///< エレベータノード
	//		case kOutline:   ///< 図郭上ノード
	//			return true;

	//		default:
	//			return false;
	//	}
	//};

	//inline bool IsOKTerminalLink( long walkLinkClass )
	//{
	//	using namespace walk_link::walk_class;

	//	// 2：横断歩道、3：歩道橋、6：人車共用リンク、23：踏切 はNG
	//	switch( walkLinkClass ){
	//		case kWalkRoad:      ///< 歩道
	//		case kOtherWalkRoad: ///< その他歩道
	//		case kPedestrian:    ///< ぺデストリアンデッキ
	//		case kConnect:       ///< 接続リンク
	//		case kStation:       ///< 駅構内
	//		case kIndoor:        ///< 施設内（屋内）
	//		case kUnderGround:   ///< 地下通路
	//		case kOutdoor:       ///< 施設敷地内（屋外）
	//		case kPark:          ///< 公園
	//		case kOpenspace:     ///< 広場
	//			return true;

	//		case kCrossRoad:     ///< 横断歩道
	//		case kWalkBridge:    ///< 歩道橋
	//		case kHumanCar:      ///< 人車共用リンク
	//		case kRailCrossing:  ///< 踏切
	//		default:
	//			return false;
	//	}
	//};



	////inline bool IsZeroNull( const CString& value )
	////{
	////	if( value.IsEmpty() || 0 == value.CompareNoCase( _T("0") ) )
	////		return true;
	////	return false;
	////};




};
