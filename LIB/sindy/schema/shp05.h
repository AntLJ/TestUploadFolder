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

#ifndef	_SHP05_H_
#define	_SHP05_H_

namespace shp05 {

namespace schema
{
	extern LPCTSTR kObjectId;
	extern LPCTSTR kShape;

	namespace annotation
	{
		extern LPCTSTR kAnnoClass_c;
		extern LPCTSTR kNameString;
		extern LPCTSTR kDispScale;
		extern LPCTSTR kFontSize;
		extern LPCTSTR kMark;
	}

	namespace admin
	{
		extern LPCTSTR kSuffix;

		extern LPCTSTR kCityCode;
	}

	namespace topmap
	{
		// wi|S
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// wiC
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// iÊ
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace middlemap
	{
		// wi|S
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// wiC
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// iÊ
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace basemap
	{
		// wi|S
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// wiC
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// iÊ
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// S¹
		namespace railway
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRalClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// kwÉ
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
			extern LPCTSTR kUnder_f;
		}
		// s­E¼Ì
		namespace admin_table
		{
			extern LPCTSTR kCityCode;
			extern LPCTSTR kPrefName;
			extern LPCTSTR kCityName;
			extern LPCTSTR kCityYomi;
		}
	}

	namespace citymap
	{
		// wi|S
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kColorCode;
		}
		// wiC
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// S¹
		namespace railway
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRalClass_c;
			extern LPCTSTR kUnder_f;
		}
		// ¨
		namespace building
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldClass_c;
			extern LPCTSTR kCorridor_f;
		}
		// nºwÉ
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
		}
		// ¨üü
		namespace bld_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldId;
		}
		// ¨r¼	
		namespace bld_name
		{
			extern LPCTSTR kSuffix;
			
			extern LPCTSTR kBldId;
			extern LPCTSTR kBldName;
		}
	}

	namespace road
	{
		namespace link_id
		{
			extern LPCTSTR kLinkId;
		}

		namespace link_dir
		{
			extern LPCTSTR kLinkDir;
		}

		namespace kind {
			extern LPCTSTR kKind_c;
		}

		// ¹HN
		namespace road_link
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kFromNodeId;
			extern LPCTSTR kToNodeId;
			extern LPCTSTR kRoadClass_c;
			extern LPCTSTR kNaviClass_c;
			extern LPCTSTR kLinkClass_c;
			extern LPCTSTR kManager_c;
			extern LPCTSTR kWidth_c;
			extern LPCTSTR kWideArea_f;
			extern LPCTSTR kIsland_f;
			extern LPCTSTR kBypass_f;
			extern LPCTSTR kCaronly_f;
			extern LPCTSTR kRoadNo;
			extern LPCTSTR kRoadCode;
			extern LPCTSTR kLaneCount;
			extern LPCTSTR kNoPassage_c;
			extern LPCTSTR kOneway_c;
		}
		// Nà®«N»ÎÅ¹HN
		namespace road_link_a
		{
			using namespace shp05::schema::road::road_link;

			extern LPCTSTR kSuffix;

			extern LPCTSTR kDivId;
			extern LPCTSTR kFromDivId;
			extern LPCTSTR kToDivId;
			extern LPCTSTR kRoadAttr_c;
			
			namespace road_attr {
				enum ECode {
					kBridge			= 1, ///< ´EË
					kTunnel			= 2, ///< gl
					kDoumon			= 4, ///< ´å
					kRailCross		= 8, ///< ¥Ø
					kUnderPass		= 16, ///< A_[pX
					kDirtRoad		= 32, ///< ¢Ü¹H
				};
			}

		}

		// ¹Hm[h
		namespace road_node
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSignal_f;
			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}

		// Nà®«N»ÎÅ¹Hm[h
		namespace road_node_a
		{
			using namespace shp05::schema::road::road_node;

			extern LPCTSTR kSuffix;

			extern LPCTSTR kDivId;
		}

		// Nà®«
		namespace segment_attr
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			extern LPCTSTR kLinkAttr_c;
			
			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< ´EË
					kTunnel			= 2, ///< gl
					kDoumon			= 3, ///< ´å
					kRailCross		= 4, ///< ¥Ø
					kUnderPass		= 5, ///< A_[pX
					kDirtRoad		= 7  ///< ¢Ü¹H
				};
			}
		}
		namespace segment_attr_vehicle
		{
			using namespace shp05::schema::road::segment_attr;

			extern LPCTSTR kSuffix;

			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< ´EË
					kTunnel			= 2, ///< gl
					kDoumon			= 3, ///< ´å
					kRailCross		= 4, ///< ¥Ø
					kUnderPass		= 5, ///< A_[pX
					kDirtRoad		= 7,  ///< ¢Ü¹H
					kTunnelSquare      = 100, ///< glilpj
					kTunnelCircle      = 101, ///< gliÛj
					kRailCrossKamaboko = 102 ///< ¥Øi©ÜÚ±^j
				};
			}
		}

		namespace reg_condition
		{
			extern LPCTSTR kFromMonth;
			extern LPCTSTR kFromDay;
			extern LPCTSTR kToMonth;
			extern LPCTSTR kToDay;
			extern LPCTSTR kFromHour;
			extern LPCTSTR kFromMin;
			extern LPCTSTR kToHour;
			extern LPCTSTR kToMin;
			extern LPCTSTR kDayOfWeek;
		}
		// ÊsÖ~
		namespace nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace reg_condition;
		}
		// êûÊs
		namespace oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace reg_condition;
		}
		// ÔíÊK§
		namespace vehicle_reg
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace reg_condition;

			extern LPCTSTR kRegType_c;
			extern LPCTSTR kHeight;
			extern LPCTSTR kWidth;
			extern LPCTSTR kWeight;
			extern LPCTSTR kLength;
			extern LPCTSTR kLoadage;
			extern LPCTSTR kDanger_f;

			namespace reg_type {
				enum ECode {
					kOneway    = 0,            //!< êûÊsi³ûüj
					kROneway   = 1,            //!< êûÊsitûüj
					kNoPassage = 2,            //!< ÊsÖ~
				};

			}
		}
		// Ôí^Cv
		namespace vehicle_type
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRegId;
			extern LPCTSTR kVehicleRegCode;
			extern LPCTSTR kExclude_f;
		}
		// ÔíÊK§}X^
		namespace vehicle_reg_master
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kVehicleRegCode;
			extern LPCTSTR kVehicleRegName;
			extern LPCTSTR kOgataJoyo_f;
			extern LPCTSTR kTokuteiChuJo_f;
			extern LPCTSTR kChugataJoyo_f;
			extern LPCTSTR kFutsuJoyo_f;
			extern LPCTSTR kFutsuJoyoKei_f;
			extern LPCTSTR kOgataKamotsu_f;
			extern LPCTSTR kTokuteiChuKa_f;
			extern LPCTSTR kChugataKamotsu_f;
			extern LPCTSTR kFutsuKamotsu_f;
			extern LPCTSTR kFutsuKamotsuKei_f;
			extern LPCTSTR kOgataTokushu_f;
			extern LPCTSTR kKogataTokushu_f;
			extern LPCTSTR kOgataBike_f;
			extern LPCTSTR kFutsuBike_f;
			extern LPCTSTR kKogataBike_f;
			extern LPCTSTR kTaxi_f;
			extern LPCTSTR kBus_f;
			extern LPCTSTR kMiniBike_f;
			extern LPCTSTR kTrolley_f;
			extern LPCTSTR kBicycle_f;
			extern LPCTSTR kExceptBicycle_f;
		}

		namespace link_queue
		{
			extern LPCTSTR kFromLinkId;
			extern LPCTSTR kToLinkId;
			extern LPCTSTR kMidCount;
			extern LPCTSTR kMidLinkId;
		}
		// NñK§
		namespace turnreg
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			using namespace reg_condition;
		}
		// ûÊÄà
		namespace dir_guide
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kGuideClass;
			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
			extern LPCTSTR kBoardPtn;
			extern LPCTSTR kBoardSeq;
		}
		// [îñ
		namespace lane
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kLaneDir;
			extern LPCTSTR kAppLane;
			extern LPCTSTR kRightChange;
			extern LPCTSTR kLeftChange;
			extern LPCTSTR kPassable;
		}
		// ­§U±
		namespace guide
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kGuideType;
		}
		// VICS
		namespace vics
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kVicsMesh;
			extern LPCTSTR kVicsId;
			extern LPCTSTR kVicsClass;
			using namespace link_dir;
			using namespace link_id;
		}
		// iüûüÊð·_
		namespace intersection
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}
		// ¹H¼Ì
		namespace road_code
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kRoadType;
			extern LPCTSTR kRoadCode;
			extern LPCTSTR kRoadName;
			extern LPCTSTR kRoadYomi;

			/// HüíÊR[hè`
			namespace road_type {
				enum ECode {
					kHighway     =   1, //!< ¬¹H
					kToll        =   2, //!< L¿¹H
					kCityHighway =   3, //!< ñs¬¹H
					kHanshin     =   4, //!< ã_¬¹H
					kNagoya      =   5, //!< ¼Ã®ss¬¹H
					kFukuoka     =   6, //!< ªss¬¹H
					kKitakyu     =   7, //!< kãBss¬¹H
					kHiroshima   =   8, //!< L¬¹H
					kFerry       =   9, //!< tF[qH
					kNormal      =  10, //!< êÊX¹¼Ì
					kNormalToll  =  11, //!< êÊL¿¹H
					kWalk		 = 101, //!< àsÒÊH¼Ì
				};
			}
		}
		// ¹HEàsÒN
		namespace road_walk_link
		{
			extern LPCTSTR kSuffix;

			using namespace shp05::schema::road::road_link;
			extern LPCTSTR kWalkClass_c;
			extern LPCTSTR kFloorMove_c;
			extern LPCTSTR kWalkAttr_c;
			extern LPCTSTR kWalkable_c;
			extern LPCTSTR kSurface_c;
			extern LPCTSTR kWheelChair_c;
		}
		// Nà®«N»ÎÅ¹HEàsÒN
		namespace road_walk_link_a
		{
			using namespace shp05::schema::road::road_walk_link;
			
			extern LPCTSTR kDivId;
			extern LPCTSTR kFromDivId;
			extern LPCTSTR kToDivId;
			extern LPCTSTR kRoadAttr_c;
			
			namespace road_attr {
				enum ECode {
					kBridge			= 1, ///< ´EË
					kTunnel			= 2, ///< gl
					kDoumon			= 4, ///< ´å
					kRailCross		= 8, ///< ¥Ø
					kUnderPass		= 16, ///< A_[pX
					kDirtRoad		= 32, ///< ¢Ü¹H
				};
			}
		}
		// ¹HEàsÒm[h
		namespace road_walk_node
		{
			extern LPCTSTR kSuffix;

			using namespace shp05::schema::road::road_node;
			extern LPCTSTR kWalkNodeClass_c;
		}
		// Nà®«N»ÎÅ¹HEàsÒm[h
		namespace road_walk_node_a
		{
			using namespace shp05::schema::road::road_walk_node;
			extern LPCTSTR kDivId;
		}
		
		// ¹HEàsÒÊsÖ~
		namespace road_walk_nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace kind;
			using namespace reg_condition;
		}
		// ¹HàsÒêûÊs
		namespace road_walk_oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace kind;
			using namespace reg_condition;
		}
		// ¹HEàsÒm[h|woüûPOIÖAe[u
		namespace rel_sgp
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// ¹HEàsÒm[h|êÊPOIÖAe[u
		namespace rel_walk_node_general_poi
		{
			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// ¹HN¡fÂÛe[u
		namespace roadcrossing
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kCrs_f;
			extern LPCTSTR kPedeZone_f;
		}

		// ¹HEàsÒNÖAt¯e[u
		namespace buddy
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kBaseLinkId;
			extern LPCTSTR kBaseDivId;
			extern LPCTSTR kRelLinkId;
			extern LPCTSTR kRelDivId;
			extern LPCTSTR kSide_c;

			namespace side {
				enum ECode {
					kRight		= 1, ///< E
					kLeft		= 2, ///< ¶
				};
			}
		}

		// ¹HEàsÒNÎ³e[u
		namespace rel_height
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kLinkDivId;
			extern LPCTSTR kFromLevel;
			extern LPCTSTR kToLevel;
		}

		// Äpü¯¹HEàsÒNÎ³C
		namespace height_node
		{
			extern LPCTSTR kLayerName;

			extern LPCTSTR kLinkId1;
			extern LPCTSTR kDivId1;
			extern LPCTSTR kRelHeight1;
			extern LPCTSTR kLinkId2;
			extern LPCTSTR kDivId2;
			extern LPCTSTR kRelHeight2;
		}
		

		namespace kind {
			enum ECode {
				kRoad			= 1, ///< ¹H
				kWalk			= 2, ///< àsÒ
			};
		}
		namespace walkable {
			enum ECode
			{
				kNotWalk  = 0,  ///< àsÖ~
				kHumanCar = 1,  ///< lÔ¤p
				kWalkOnly = 2,  ///< àsÒêp
			};
		} // walkable_class
			
	}
	// ¹HWf[^
	namespace road_alt
	{
		extern LPCTSTR kSuffix;

		extern LPCTSTR kLinkId;
		extern LPCTSTR kNodeId;
		extern LPCTSTR kSequence;
		extern LPCTSTR kHeightF;
	}

}

}
#endif	//_SHP05_H_
