#pragma once

#include "stdafx.h"
#include "HeightInfoMgr.h"

#include <ArcHelperEx/AheBuddyGlobals.h>

namespace judge_value {

	/* [Bug12501] 1km→50m コの字型で遠い方がヒットしてしまうため修正。
		適切な値とは限らないが、誤ヒットは減る見込み
	*/
	const double LINK_RELATION_DISTANCE = 50.0;

	using namespace sindy;
	using namespace sindy::schema;
	/*
	 名前がおかしいけど、コード値（key）とドメインマップから
	 コード値('コード名')
	 又は
	 コード値('<null>')
	 の文字列を返す
	 例）
	 100('あああ')
	 101('<null>')
	*/
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


	inline bool IsDefineWalkable( long walkable )
	{
		using namespace road_link::walkable_class;

		switch( walkable ){
			case kNotWalk:   ///< 歩行禁止
			case kWalkable:  ///< 人車共用
			case kWalkOnly:  ///< 歩行者専用
			case kRightSideWalkWalkable:  ///< 右側歩道あり（人車共用）
			case kRightSideWalkNonWalkable: ///< 右側歩道あり（歩行禁止）
			case kLeftSideWalkWalkable: ///< 左側歩道あり（人車共用）
			case kLeftSideWalkNonWalkable: ///< 左側歩道あり（歩行禁止）
			case kBothSideWalk: ///< 両側歩道あり
				return true;

			default:
				return false;
		}
	};

	inline bool IsWalkOnly( long walkable )
	{
		using namespace road_link::walkable_class;

		switch( walkable ){
			case kWalkOnly:  ///< 歩行者専用
				return true;
			default:
				return false;
		}
	};

	inline CString IsSameWalkLinkAttr(const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap)
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt(fieldMap.FindField( kWalkClass ), geoClass2 ) ) return kWalkClass;
		if ( !geoClass1.CompareAt(fieldMap.FindField( kFloorMoveClass ), geoClass2) ) return kFloorMoveClass;
		if ( !geoClass1.CompareAt(fieldMap.FindField(kAutoWalkFlag), geoClass2) ) return kAutoWalkFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kStairFlag ), geoClass2 ) ) return kStairFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kSlopeFlag ), geoClass2 ) ) return kSlopeFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kEscalatorFlag ), geoClass2 ) ) return kEscalatorFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kArcadeFlag ), geoClass2 ) ) return kArcadeFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOpenAreaFlag ), geoClass2 ) ) return kOpenAreaFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kTunnelFlag ), geoClass2 ) ) return kTunnelFlag;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kRoadSurface ), geoClass2 ) ) return kRoadSurface;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOneway ), geoClass2 ) ) return kOneway;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOnewayRF ), geoClass2 ) ) return kOnewayRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kNoPassage ), geoClass2 ) ) return kNoPassage;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kNoPassageRF ), geoClass2 ) ) return kNoPassageRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kLinkRelationRF ), geoClass2 ) ) return kLinkRelationRF;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kWalkCode ), geoClass2 ) ) return kWalkCode;

		return _T("");
	};

	inline bool IsSameOneway( const GeoClass& geoClass1, const GeoClass& geoClass2, const long nodeID, const CFieldMap& fieldMap )
	{
		using namespace walk_link;
		using namespace walk_link::oneway;

		// 条件付きはないため、考慮しない
		
		long onewayC1 = geoClass1.GetLongValue( fieldMap.FindField(kOneway ) );
		long onewayC2 =  geoClass2.GetLongValue(fieldMap.FindField( kOneway ) );
		if( onewayC1 == kNoOneway && onewayC2 == kNoOneway )
			return true;

		bool bFrom1 = false, bFrom2 = false;
		if ( nodeID == geoClass1.GetLongValue( fieldMap.FindField( kFromNodeID ) ) )
			bFrom1 = true;
		else if ( nodeID != geoClass1.GetLongValue( fieldMap.FindField( kToNodeID ) ) )
			return false; // From/Toとも一致しなければOUT（ないはず）

		if ( nodeID == geoClass2.GetLongValue( fieldMap.FindField( kFromNodeID ) ) )
			bFrom2 = true;
		else if ( nodeID != geoClass2.GetLongValue( fieldMap.FindField( kToNodeID ) ) )
			return false; // From/Toとも一致しなければOUT（ないはず）
			
		if( ( bFrom1 && ! bFrom2 ) || ( ! bFrom1 && bFrom2 ) ){ // From・Toで正順の場合（同じ向きならOK）

			// 正・正 or 逆・逆 ならOK
			if( ( onewayC1 == kFore && onewayC2 == kFore ) || ( onewayC1 == kReverse && onewayC2 == kReverse ) )
				return true;

			return false;
		}
		
		// From・Toで逆順の場合（逆向きならOK）
		// 正・逆 or 逆・正 ならOK
		if( ( onewayC1 == kFore && onewayC2 == kReverse ) || ( onewayC1 == kReverse && onewayC2 == kFore ) )
			return true;

		return false;
	};

	inline bool IsSameWalkLinkFlagAttr( const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap )
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt( fieldMap.FindField( kAutoWalkFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kStairFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kSlopeFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kEscalatorFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kArcadeFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kOpenAreaFlag ), geoClass2 ) ) return false;
		if ( !geoClass1.CompareAt( fieldMap.FindField( kTunnelFlag ), geoClass2 ) ) return false;

		return true;
	}

	inline bool IsSimpleSameWalkLinkAttr( const GeoClass& geoClass1, const GeoClass& geoClass2, const CFieldMap& fieldMap )
	{
		using namespace walk_link;

		if ( !geoClass1.CompareAt( fieldMap.FindField( kWalkClass ), geoClass2 ) ) return false;
		if ( !IsSameWalkLinkFlagAttr( geoClass1, geoClass2, fieldMap ) ) return false;

		return true;
	};

	inline bool IsDefineWalkLinkClass( long walkLinkClassCode, sindy::schema::walk_type::type_class::ECode dataType = walk_type::type_class::kGeneral )
	{
		using namespace walk_link::walk_class;
		// 汎用向け場合
		if( dataType == walk_type::type_class::kGeneral )
		{
			// スキーマ定義上のものは全てOK
			switch( walkLinkClassCode ){
				case kNone:               ///< 設定なし
				case kWalkRoad:           ///< 歩道
				case kInferredCrossRoad:  ///< みなし横断歩道
				case kCrossRoad:          ///< 横断歩道
				case kOtherWalkRoad:      ///< その他歩道
				case kWalkableRoad:       ///< 人車共用リンク
				case kConnect:            ///< 接続リンク
				case kWalkBridge:         ///< 歩道橋
				case kBridge:             ///< 橋
				case kRailCrossing:       ///< 踏切
				case kOutdoor:            ///< 施設敷地内（屋外）
				case kIndoor:             ///< 施設敷地内（屋内）
				case kInternalTicketGate: ///< 地上改札内
				case kPlatForm:			  ///< 地上プラットフォーム
				case kUnsurveyInternalTicketGate: ///< 改札内（仮）
				case kUnderGround:        ///< 地下通路
				case kUnderGroundPath:    ///< 横断地下道
				case kInternalTicketGateUG:///< 地下改札内
				case kPlatFormUG:         ///< 地下プラットフォーム
				case kWalkFerry:          ///< 歩行者専用フェリー
					return true;

				default:
					return false;
			}
		}
		// ダウンコンバート向け場合 
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// スキーマ定義上のものは全てOK
			switch( walkLinkClassCode ){
				case kNone:               ///< 設定なし
				case kInferredCrossRoad:  ///< みなし横断歩道
				case kCrossRoad:          ///< 横断歩道
				case kOtherWalkRoad:      ///< その他歩道
				case kConnect:            ///< 接続リンク
				case kLogicalConnection:  ///< ロジカルコネクション
				case kWalkBridge:         ///< 歩道橋
				case kBridge:             ///< 橋
				case kRailCrossing:       ///< 踏切
				case kOutdoor:            ///< 施設敷地内（屋外）
				case kIndoor:             ///< 施設敷地内（屋内）
				case kUnderGround:        ///< 地下通路
				case kUnderGroundPath:    ///< 横断地下道
				case kWalkFerry:          ///< 歩行者専用フェリー
					return true;

				default:
					return false;
			}			
		}
		return true;
	};

	inline bool IsOKWalkLinkClass( long walkLinkClassCode )
	{
		using namespace walk_link::walk_class;

		// 使用するもののみOK
		if ( kNone != walkLinkClassCode && IsDefineWalkLinkClass( walkLinkClassCode ) ) {
			return true;
		}
		return false;
	};

	inline bool IsDefineFloorMoveClass( long floorMoveClass )
	{
		using namespace walk_link::floor_move_class;

		// スキーマ定義上のものは全てOK
		switch( floorMoveClass ){
			case kNone:      ///< 上下無し
			case kForeUp:    ///< 上り(正方向)
			case kReverseUp: ///< 上り(逆方向)
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineOnewayCode( long onewayCode )
	{
		using namespace walk_link::oneway;

		// スキーマ定義上のものは全てOK
		switch( onewayCode ){
			case kNoOneway:           ///< 一方通行なし
			case kFore:               ///< 条件なし一方通行(正方向)
			case kReverse:            ///< 条件なし一方通行(逆方向)
			case kForeWithReg:        ///< 条件付き一方通行(正方向)
			case kReverseWithReg:     ///< 条件付き一方通行(逆方向)
			case kForeReverseWithReg: ///< 条件付き一方通行(正逆)
				return true;

			default:
				return false;
		}
	};

	inline bool IsOKOnewayCode( long onewayCode )
	{
		using namespace walk_link::oneway;

		switch( onewayCode ){
			case kNoOneway:           ///< 一方通行なし
			case kFore:               ///< 条件なし一方通行(正方向)
			case kReverse:            ///< 条件なし一方通行(逆方向)
				return true;
			default:
				return false;
		}
	};



	inline bool IsDefineNoPassCode( long noPassCode )
	{
		using namespace walk_link::nopassage;

		// スキーマ定義上のものは全てOK
		switch( noPassCode ){
			case kPassable:           ///< 通行禁止なし
			case kNoPassageWithReg:   ///< 条件付き通行禁止
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineRoadSurfaceClass( long roadSurfaceClass )
	{
		using namespace walk_link::road_surface_class;

		// スキーマ定義上のものは全てOK
		switch( roadSurfaceClass ){
			case kPaved:   ///< 舗装
			case kUnpaved: ///< 未舗装
				return true;

			default:
				return false;
		}
	};

	inline bool IsDefineWalkCode( long walkCode )
	{
		// 0(初期値) or 1000～999999がOK
		return ( walkCode == 0 || ( 1000 <= walkCode && walkCode <= 999999 ) );
	};

	inline bool IsOKFloorMoveClassAndFlag(long floorMoveClass, const long flag)
	{
		using namespace walk_link::floor_move_class;

		if (FLAG_ON != flag) return true;
		
		switch( floorMoveClass ){
			case kForeUp:    ///< 上り(正方向)
			case kReverseUp: ///< 上り(逆方向)
				return true;

			case kNone:
			default:
				return false;
		}
	};

	inline bool IsOKMovingWalkwayAndArcade(long walkLinkClassCode, const long autowalkFlag, const long escalatorFlag, const long arcadeFlag)
	{
		using namespace walk_link::walk_class;

		// チェック対象外の歩行者種別コードの場合スキップ
		switch( walkLinkClassCode ){
			case kIndoor:             ///< 施設敷地内（屋内）
			case kUnderGround:        ///< 地下通路
			case kUnderGroundPath:    ///< 横断地下道
			case kInternalTicketGate: ///< 地上改札内
			case kPlatForm:			  ///< 地上プラットフォーム
			case kUnsurveyInternalTicketGate: ///< 改札内（仮）
			case kInternalTicketGateUG:///< 地下改札内
			case kPlatFormUG:         ///< 地下プラットフォーム
				return true;

			default:
				break;
		}

		// エスカレーターフラグ または オートウォークフラグがONで屋根付きフラグがOFFの場合
		if ((FLAG_ON == autowalkFlag || FLAG_ON == escalatorFlag) && FLAG_ON != arcadeFlag)
			return false;

		return true;
	}

	inline bool IsOKFloorMoveClassAndFlagOFF( long floorMoveClass, const long flag )
	{
		using namespace walk_link::floor_move_class;

		if( FLAG_ON == flag ) return true;

		switch( floorMoveClass ){
			case kForeUp:    ///< 上り(正方向)
			case kReverseUp: ///< 上り(逆方向)
				return false;
			default:
				return true;
		}
	};

	inline bool IsOKOnewayCodeAndFlag(long oneway, const long flag)
	{
		using namespace walk_link::oneway;

		if ( FLAG_ON != flag ) return true;

		switch( oneway ){
			case kFore:					///< 条件なし一方通行(正方向)
			case kReverse:				///< 条件なし一方通行(逆方向)
			case kForeWithReg:			///< 条件付き一方通行(正方向)
			case kReverseWithReg:		///< 条件付き一方通行(逆方向)
			case kForeReverseWithReg:	///< 条件付き一方通行(正逆)
				return true;
			default:
				return false;
		}
	};


	inline bool IsOKOnewayCodeAndRelFlag( long oneway, const long relFlag )
	{
		using namespace walk_link::oneway;

		switch( oneway ){
			case kNoOneway:     ///< 一方通行なし
			case kFore:        ///< 条件なし一方通行(正方向)
			case kReverse:      ///< 条件なし一方通行(逆方向)
				if ( FLAG_ON == relFlag ) return false;
				return true;
		
			default:
				if ( FLAG_ON != relFlag ) return false;
				return true;
		}
	};

	enum SimpleDirection
	{
		noDirect,
		force,
		reverce,
	};

	inline int GetOnwayDirection( long oneway )
	{
		using namespace walk_link::oneway;

		switch( oneway ){
		case kFore:					///< 条件なし一方通行(正方向)
		case kForeWithReg:			///< 条件付き一方通行(正方向)
			return SimpleDirection::force;

		case kReverse:				///< 条件なし一方通行(逆方向)
		case kReverseWithReg:		///< 条件付き一方通行(逆方向)
			return SimpleDirection::reverce;

		default:
			return SimpleDirection::noDirect;
		}
	}

	inline bool IsOKNoPassageCodeAndRelFlag( long noPass, const long relFlag )
	{
		using namespace walk_link::nopassage;

		switch( noPass ){
		case kPassable:     ///< 通行禁止なし
			if ( FLAG_ON == relFlag ) return false;
			return true;

		case kNoPassageWithReg:   ///< 条件付き通行禁止
		default:
			if ( FLAG_ON != relFlag ) return false;
			return true;
		}
	};

	inline bool IsOKWalkLinkClassAndFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		if(dataType == walk_type::type_class::kDownConvert)
		{
			switch( walkLinkClassCode ){
			case kOtherWalkRoad:      ///< その他歩道
			case kWalkBridge:         ///< 歩道橋
			case kOutdoor:            ///< 施設敷地内（屋外）
			case kIndoor:             ///< 施設敷地内（屋内）
			case kInternalTicketGate: ///< 地上改札内
			case kPlatForm:           ///< 地上プラットフォーム
			case kUnsurveyInternalTicketGate: ///< 改札内（仮）
			case kUnderGround:        ///< 地下通路
			case kUnderGroundPath:    ///< 横断地下道
			case kInternalTicketGateUG:///< 地下改札内
			case kPlatFormUG:          ///< 地下プラットフォーム
				return true;
			default:
				return false;
			}
		}
		else
		{
			switch( walkLinkClassCode ){
			case kWalkRoad:           ///< 歩道
			case kOtherWalkRoad:      ///< その他歩道
			case kWalkBridge:         ///< 歩道橋
			case kOutdoor:            ///< 施設敷地内（屋外）
			case kIndoor:             ///< 施設敷地内（屋内）
			case kInternalTicketGate: ///< 地上改札内
			case kUnsurveyInternalTicketGate: ///< 改札内（仮）
			case kUnderGround:        ///< 地下通路
			case kUnderGroundPath:    ///< 横断地下道
			case kInternalTicketGateUG:///< 地下改札内
				return true;
			default:
				return false;
			}
		}
	};

	inline bool IsOKWalkLinkClassAndArcadeFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		if(dataType == walk_type::type_class::kDownConvert)
		{
			switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< みなし横断歩道
			case kCrossRoad:          ///< 横断歩道
			case kOtherWalkRoad:      ///< その他歩道
			case kWalkBridge:         ///< 歩道橋
			case kOutdoor:            ///< 施設敷地内（屋外）
			case kInternalTicketGate: ///< 地上改札内
			case kPlatForm:			  ///< 地上プラットフォーム
			case kUnsurveyInternalTicketGate:         ///< 改札内（仮）
				return true;
			default:
				return false;
			}
		}
		else
		{
			switch( walkLinkClassCode ){
			case kWalkRoad:           ///< 歩道
			case kInferredCrossRoad:  ///< みなし横断歩道
			case kCrossRoad:          ///< 横断歩道
			case kOtherWalkRoad:      ///< その他歩道
			case kWalkBridge:         ///< 歩道橋
			case kOutdoor:            ///< 施設敷地内（屋外）
			case kPlatForm:			  ///< 地上プラットフォーム
			case kPlatFormUG:         ///< 地下プラットフォーム
				return true;
			default:
				return false;
			}
		}
	};

	inline bool IsOKWalkLinkClassAndOpenAreaFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		// 汎用版のチェックのみ場合に、歩行者表示種別コード「歩道」のチェックを行う。
		if(dataType == walk_type::type_class::kGeneral && walkLinkClassCode == kWalkRoad) return true;

		switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< みなし横断歩道
			case kCrossRoad:          ///< 横断歩道
			case kOtherWalkRoad:      ///< その他歩道
			case kConnect:            ///< 接続リンク
			case kWalkBridge:         ///< 歩道橋
			case kOutdoor:            ///< 施設敷地内（屋外）
			case kIndoor:             ///< 施設敷地内（屋内）
			case kUnderGround:        ///< 地下通路
				return true;
			default:
				return false;
		}
	};

	inline bool IsOKWalkLinkClassAndTunnelFlag( long walkLinkClassCode, const long flag, walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;
		// 汎用版のチェックのみ場合に、歩行者表示種別コード「歩道」のチェックを行う。
		if(dataType == walk_type::type_class::kGeneral && walkLinkClassCode == kWalkRoad) return true;

		switch( walkLinkClassCode ){
			case kInferredCrossRoad:  ///< みなし横断歩道
			case kCrossRoad:          ///< 横断歩道
			case kOtherWalkRoad:      ///< その他歩道
			case kConnect:            ///< 接続リンク
			case kWalkBridge:         ///< 歩道橋
			case kBridge:             ///< 橋
			case kOutdoor:            ///< 施設敷地内（屋外）
				return true;
			default:
				return false;
		}
	};

	inline bool IsOKWalkLinkClassAndScrambleFlag( long walkLinkClassCode, const long flag )
	{
		using namespace walk_link::walk_class;

		if ( FLAG_ON != flag ) return true;

		switch ( walkLinkClassCode ) {
			case kInferredCrossRoad:  ///< みなし横断歩道
			case kCrossRoad:          ///< 横断歩道
			case kOtherWalkRoad:      ///< その他歩道
				return true;
			default:
				return false;
		}
	};
	inline bool IsDefineWalkNodeClass( long walkNodeClass, walk_type::type_class::ECode dataType )
	{
		using namespace walk_node::node_class;

		// 汎用向け場合
		if( dataType ==  walk_type::type_class::kGeneral )
		{
			// スキーマ定義上のものは全てOK
			switch( walkNodeClass ){
				case kPublic:			///< 一般ノード
				case kTerminal:			///< 端点ノード
				case kAlter:			///< 属性遷移点ノード
				case kRoadNW:			///< 道路ＮＷ接続ノード
				case kGate:				///< 駅出入口ノード
				case kUndergroundGate:	///< 地下街出入口ノード
				case kElevator:			///< エレベータノード
				case kKaisatsu:         ///< 空港改札ノード
				case kCarPratform:      ///< 一般車乗降場ノード
				case kBusPratform:      ///< バス乗降場ノード
				case kTaxiPratform:     ///< タクシー乗降場ノード
				case kParking:          ///< 駐車場出入口ノード
				case kDepartureGate:    ///< 空港出発ゲートノード
				case kArrivalGate:      ///< 空港到着ゲートノード
				case kTicketGate:       ///< 駅改札ノード
				case kUnsurveyTicketGate:///< 改札ノード（仮）
				case kOutline:			///< 図郭上ノード
					return true;

				default:
					return false;
			}
		}
		// ダウンコンバート向け場合
		if( dataType ==  walk_type::type_class::kDownConvert )
		{
			// スキーマ定義上のものは全てOK
			switch( walkNodeClass ){
				case kPublic:			///< 一般ノード
				case kTerminal:			///< 端点ノード
				case kAlter:			///< 属性遷移点ノード
				case kRoadNW:			///< 道路ノード接続ノード
				case kElevator:			///< エレベータノード
				case kKaisatsu:         ///< 空港改札ノード
				case kCarPratform:      ///< 一般車乗降場ノード
				case kBusPratform:      ///< バス乗降場ノード
				case kTaxiPratform:     ///< タクシー乗降場ノード
				case kParking:          ///< 駐車場出入口ノード
				case kDepartureGate:    ///< 空港出発ゲートノード
				case kArrivalGate:      ///< 空港到着ゲートノード
				case kConnectedRoad:    ///< 道路リンク接続ノード
				case kOutline:			///< 図郭上ノード
					return true;

				default:
					return false;
			}			
		}
		return true;
	};
	
	inline bool IsOKTerminalLink( long walkLinkClass, sindy::schema::walk_type::type_class::ECode dataType )
	{
		using namespace walk_link::walk_class;
		// 汎用向け場合
		if( dataType == walk_type::type_class::kGeneral )
		{
			// 3：横断歩道、100：人車共用リンク、200：歩道橋、202：踏切、401:横断地下道 はNG
			// OKなものを記述→NGなものを記述とした。2:みなし横断歩道 500:歩行者専用フェリーもNGに
			switch( walkLinkClass ){
				case kInferredCrossRoad:  ///< みなし横断歩道
				case kCrossRoad:          ///< 横断歩道
				case kWalkableRoad:       ///< 人車共用リンク
				case kWalkBridge:         ///< 歩道橋
				case kRailCrossing:       ///< 踏切
				case kUnderGroundPath:    ///< 横断地下道
				case kWalkFerry:          ///< 歩行者専用フェリー
					return false;
				default:
					return IsOKWalkLinkClass( walkLinkClass );
			}
		}
		// ダウンコンバート向け場合
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// 101：接続リンク、199：ロジカルコネクション、 はNG
			switch( walkLinkClass ){
				case kConnect:                   ///< 接続リンク
				case kLogicalConnection:         ///< ロジカルコネクション
					return false;
				default:
					return true;
			}		
		}
		return true;
	};

	// WalkNodeCheckFunction::checkConenctLinkから定義を移して関数化
	inline bool IsOKTerminalNode( long walkNodeClass )
	{
		using namespace walk_node::node_class;

		switch ( walkNodeClass ) {
			case kTerminal:			///< 端点ノード
			case kRoadNW:			///< 道路ＮＷ接続ノード
			case kKaisatsu:         ///< 空港改札ノード
			case kCarPratform:      ///< 一般車乗降場ノード
			case kBusPratform:      ///< バス乗降場ノード
			case kTaxiPratform:     ///< タクシー乗降場ノード
			case kParking:          ///< 駐車場出入口ノード
			case kDepartureGate:    ///< 空港出発ゲートノード
			case kArrivalGate:      ///< 空港到着ゲートノード

			//以下、[bug 12687] により追加
			case kTicketGate:       ///< 駅改札ノード
			case kUnsurveyTicketGate:///< 改札ノード（仮）
				return true;

			default:
				return false;
		}
	}

	inline bool IsTicketGate( long walkNodeClass )
	{
		using namespace walk_node::node_class;
		//改札ノードとして扱うのは、14：駅改札ノード、98：改札ノード（仮）
		switch ( walkNodeClass )
		{
			case kTicketGate:			///< 駅改札ノード
			case kUnsurveyTicketGate:	///< 改札ノード（仮）
				return true;
			default:
				return false;
		}
	}

	inline bool IsInternalTicketGateLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		//改札内リンクとして扱うのは、
		//302：地上改札内、303：地上プラットホーム、399：改札内（仮）、402：地下改札内、403：地下プラットホーム
		switch ( walkLinkClass ) {
			case kInternalTicketGate: ///< 地上改札内
			case kPlatForm:			  ///< 地上プラットフォーム
			case kUnsurveyInternalTicketGate: ///< 改札内（仮）
			case kInternalTicketGateUG:///< 地下改札内
			case kPlatFormUG:         ///< 地下プラットフォーム
				return true;
			default:
				return false;
		}
	}

	inline bool IsParalleCheckTargetlLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		//エラーコード355,356:並行リンク関連チェック対象
		//『施設敷地内（屋内）』『地上改札内』『地上プラットホーム』『地下通路』『地下改札内』『地下プラットホーム』
		switch ( walkLinkClass ) {
			case kIndoor:             ///< 施設敷地内（屋内）
			case kInternalTicketGate: ///< 地上改札内
			case kPlatForm:			  ///< 地上プラットフォーム
			case kUnderGround:        ///< 地下通路
			case kInternalTicketGateUG:///< 地下改札内
			case kPlatFormUG:         ///< 地下プラットフォーム
				return true;
			default:
				return false;
		}

	}

	inline bool IsUnderGroundLink( long walkLinkClass )
	{
		using namespace walk_link::walk_class;
		// 地下リンク
		//「地下通路」「横断地下道」「地下改札内」「地下プラットホーム」
		switch ( walkLinkClass ) {
			case kUnderGround:        ///< 地下通路
			case kUnderGroundPath:    ///< 横断地下道
			case kInternalTicketGateUG:///< 地下改札内
			case kPlatFormUG:         ///< 地下プラットフォーム
				return true;
			default:
				return false;
		}

	}
	inline bool IsOKHumanCarAndRegFlag( long walkLinkClass, long onwayClass, long noPassageClass )
	{
		using namespace walk_link::walk_class;
		using namespace walk_link::oneway;
		using namespace walk_link::nopassage;

		if( walkLinkClass == kWalkableRoad )
			if( ! (onwayClass == kNoOneway && noPassageClass == kPassable ))
				return false;
		return true;

	};

	inline bool IsDefineHeight( long value )
	{
		return ( -2 <= value  && value <= 2 );
	};

	inline bool IsDefineLinkTable( long value )
	{
		using namespace height_node::link_table;
		switch (value)
		{
		case kNone: return true;
		case kRoadLink: return true;
		case kWalkLink: return true;
		default:
			break;
		}
		return false;
	};

	inline bool IsOKLinkTable( long value )
	{
		using namespace height_node::link_table;
		switch (value)
		{
		case kRoadLink: return true;
		case kWalkLink: return true;
		default:
			break;
		}
		return false;
	};

	inline bool IsOKBaseLinkWalkCode( long walkLinkClass)
	{
		using namespace walk_link::walk_class;

		// 歩道、橋、踏切 はOK
		switch( walkLinkClass ){
		case kWalkRoad:      ///< 歩道
		case kBridge:        ///< 橋
		case kRailCrossing:  ///< 踏切
			return true;

		default:
			return false;
		}
	}

	inline bool IsOKHeightNodeWalkCode( long walkLinkClass, sindy::schema::walk_type::type_class::ECode dataType = walk_type::type_class::kGeneral)
	{
		using namespace walk_link::walk_class;
		// 汎用向け場合
		if( dataType == walk_type::type_class::kGeneral )
		{
			// 対象外を列挙
			switch( walkLinkClass ){
				case kWalkableRoad:					///< 人車共用リンク
				case kUnderGround:					///< 地下通路
				case kConnect:						///< 接続リンク
				case kUnderGroundPath:			    ///< 横断地下道
				case kInternalTicketGateUG:			///< 地下改札内
				case kPlatFormUG:					///< 地下プラットフォーム
					return false;

				default:
					return true;
			}
		}
		// ダウンコンバート向け場合
		if( dataType == walk_type::type_class::kDownConvert )
		{
			// 対象外を列挙
			switch( walkLinkClass ){
				case kConnect:						///< 接続リンク
				case kLogicalConnection:            ///< ロジカルコネクション
				case kUnderGround:                  ///< 地下通路
				case kUnderGroundPath:			    ///< 横断地下道
					return false;

				default:
					return true;
			}			
		}
		return true;
	}


	inline bool IsOKCompareHeightRef(long height, int refHeight)
	{
		bool result = false;
		switch (refHeight)
		{
		case Height::high:
		case Height::higher:
			result = (refHeight <= height );
			break;
		case Height::low:
		case Height::loweer:
			result = (refHeight >= height );
			break;
		default:
			result = true;
		}
		return result;
	}

	inline bool IsOKBuddyRelation( const IGeometryPtr& baseLinkObj, const std::map<long, IGeometryPtr>& relLinks, std::list<long>& errorRelLinks, double& maxDistance )
	{
		bool existOKRelation = false;
		for( const auto& relLink : relLinks )
		{
			LinkRelationPosition position = LinkRelationPosition::kNotFound;
			double minMeter = 0.0;
			
			// 基準リンクと関連リンクが付与処理で正常に紐付られる
			if( AheJudgeLinkRelation( (IPolylinePtr)baseLinkObj, (IPolylinePtr)relLink.second, LINK_RELATION_DISTANCE, position, minMeter )
				&& position != LinkRelationPosition::kNotFound )
			{
				existOKRelation = true;
				if( maxDistance < minMeter)
					maxDistance = minMeter;
			}
			else
				errorRelLinks.push_back(relLink.first);
		}

		return existOKRelation;
	}

	inline bool IsOKFlag( const long flag )
	{
		if( 0 == flag || 1 == flag )
			return true;
		return false;
	};


	/**
	* @brief リリース対象の道路リンクか
	* @retval true リリース対象
	* @retval true リリース対象外
	*/
	inline bool IsOKReleaseRoadLink( const GeoClass& geoClass, const CFieldMap& fieldMap )
	{
		using namespace road_link;

		// 道路種別が400未満 かつ 細街路作業年度コードが9じゃない
		return geoClass.GetLongValue( fieldMap.FindField( kRoadClass ) ) < 400 &&
			 geoClass.GetLongValue( fieldMap.FindField( kAlleyMainte ) ) != alley_mainte::k2005;
	}

	/**
	* 「歩行者種別が前後のリンクと異なる」（エラーコード353）のチェック対象か
	* @param walkLinkClass    [in] 歩行者リンクの種別コード
	* @retval true  チェック対象
	* @retval false チェック対象外
	*/
	inline bool IsTargetOfCheck353(const long walkLinkClass)
	{
		using namespace walk_link::walk_class;

		switch( walkLinkClass ){
		case kWalkBridge:       ///< 歩道橋
		case kOutdoor:          ///< 施設敷地内（屋外）
		case kIndoor:           ///< 施設敷地内（屋内）
		case kUnderGround:      ///< 地下通路
		case kUnderGroundPath:  ///< 横断地下道
			return true;

		default:
			return false;
		}
	}
};