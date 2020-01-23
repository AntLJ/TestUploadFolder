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
		// �w�i�|���S��
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// �w�i���C��
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// �i��
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace middlemap
	{
		// �w�i�|���S��
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// �w�i���C��
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
		// �i��
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace basemap
	{
		// �w�i�|���S��
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// �w�i���C��
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
		// �i��
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// �S��
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
		// ���k�w��
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
			extern LPCTSTR kUnder_f;
		}
		// �s���E����
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
		// �w�i�|���S��
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kColorCode;
		}
		// �w�i���C��
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// �S��
		namespace railway
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRalClass_c;
			extern LPCTSTR kUnder_f;
		}
		// ����
		namespace building
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldClass_c;
			extern LPCTSTR kCorridor_f;
		}
		// �n���w��
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
		}
		// ����������
		namespace bld_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldId;
		}
		// �����r����	
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

		// ���H�����N
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
		// �����N�����������N���Ή��œ��H�����N
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
					kBridge			= 1, ///< ���E����
					kTunnel			= 2, ///< �g���l��
					kDoumon			= 4, ///< ����
					kRailCross		= 8, ///< ����
					kUnderPass		= 16, ///< �A���_�[�p�X
					kDirtRoad		= 32, ///< ���ܑ����H
				};
			}

		}

		// ���H�m�[�h
		namespace road_node
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSignal_f;
			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}

		// �����N�����������N���Ή��œ��H�m�[�h
		namespace road_node_a
		{
			using namespace shp05::schema::road::road_node;

			extern LPCTSTR kSuffix;

			extern LPCTSTR kDivId;
		}

		// �����N������
		namespace segment_attr
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			extern LPCTSTR kLinkAttr_c;
			
			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< ���E����
					kTunnel			= 2, ///< �g���l��
					kDoumon			= 3, ///< ����
					kRailCross		= 4, ///< ����
					kUnderPass		= 5, ///< �A���_�[�p�X
					kDirtRoad		= 7  ///< ���ܑ����H
				};
			}
		}
		namespace segment_attr_vehicle
		{
			using namespace shp05::schema::road::segment_attr;

			extern LPCTSTR kSuffix;

			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< ���E����
					kTunnel			= 2, ///< �g���l��
					kDoumon			= 3, ///< ����
					kRailCross		= 4, ///< ����
					kUnderPass		= 5, ///< �A���_�[�p�X
					kDirtRoad		= 7,  ///< ���ܑ����H
					kTunnelSquare      = 100, ///< �g���l���i�l�p�j
					kTunnelCircle      = 101, ///< �g���l���i�ہj
					kRailCrossKamaboko = 102 ///< ���؁i���܂ڂ��^�j
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
		// �ʍs�֎~
		namespace nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace reg_condition;
		}
		// ����ʍs
		namespace oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace reg_condition;
		}
		// �Ԏ�ʋK��
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
					kOneway    = 0,            //!< ����ʍs�i�������j
					kROneway   = 1,            //!< ����ʍs�i�t�����j
					kNoPassage = 2,            //!< �ʍs�֎~
				};

			}
		}
		// �Ԏ�^�C�v
		namespace vehicle_type
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRegId;
			extern LPCTSTR kVehicleRegCode;
			extern LPCTSTR kExclude_f;
		}
		// �Ԏ�ʋK���}�X�^
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
		// �����N��K��
		namespace turnreg
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			using namespace reg_condition;
		}
		// ���ʈē�
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
		// ���[�����
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
		// �����U��
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
		// �i�������ʌ����_
		namespace intersection
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}
		// ���H����
		namespace road_code
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kRoadType;
			extern LPCTSTR kRoadCode;
			extern LPCTSTR kRoadName;
			extern LPCTSTR kRoadYomi;

			/// �H����ʃR�[�h��`
			namespace road_type {
				enum ECode {
					kHighway     =   1, //!< �������H
					kToll        =   2, //!< �L�����H
					kCityHighway =   3, //!< ��s�������H
					kHanshin     =   4, //!< ��_�������H
					kNagoya      =   5, //!< ���É��s�s�������H
					kFukuoka     =   6, //!< �����s�s�������H
					kKitakyu     =   7, //!< �k��B�s�s�������H
					kHiroshima   =   8, //!< �L���������H
					kFerry       =   9, //!< �t�F���[�q�H
					kNormal      =  10, //!< ��ʊX������
					kNormalToll  =  11, //!< ��ʗL�����H
					kWalk		 = 101, //!< ���s�ҒʘH����
				};
			}
		}
		// ���H�E���s�҃����N
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
		// �����N�����������N���Ή��œ��H�E���s�҃����N
		namespace road_walk_link_a
		{
			using namespace shp05::schema::road::road_walk_link;
			
			extern LPCTSTR kDivId;
			extern LPCTSTR kFromDivId;
			extern LPCTSTR kToDivId;
			extern LPCTSTR kRoadAttr_c;
			
			namespace road_attr {
				enum ECode {
					kBridge			= 1, ///< ���E����
					kTunnel			= 2, ///< �g���l��
					kDoumon			= 4, ///< ����
					kRailCross		= 8, ///< ����
					kUnderPass		= 16, ///< �A���_�[�p�X
					kDirtRoad		= 32, ///< ���ܑ����H
				};
			}
		}
		// ���H�E���s�҃m�[�h
		namespace road_walk_node
		{
			extern LPCTSTR kSuffix;

			using namespace shp05::schema::road::road_node;
			extern LPCTSTR kWalkNodeClass_c;
		}
		// �����N�����������N���Ή��œ��H�E���s�҃m�[�h
		namespace road_walk_node_a
		{
			using namespace shp05::schema::road::road_walk_node;
			extern LPCTSTR kDivId;
		}
		
		// ���H�E���s�Ғʍs�֎~
		namespace road_walk_nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace kind;
			using namespace reg_condition;
		}
		// ���H���s�҈���ʍs
		namespace road_walk_oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace kind;
			using namespace reg_condition;
		}
		// ���H�E���s�҃m�[�h�|�w�o����POI�֘A�e�[�u��
		namespace rel_sgp
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// ���H�E���s�҃m�[�h�|���POI�֘A�e�[�u��
		namespace rel_walk_node_general_poi
		{
			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// ���H�����N���f�ۃe�[�u��
		namespace roadcrossing
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kCrs_f;
			extern LPCTSTR kPedeZone_f;
		}

		// ���H�E���s�҃����N�֘A�t���e�[�u��
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
					kRight		= 1, ///< �E
					kLeft		= 2, ///< ��
				};
			}
		}

		// ���H�E���s�҃����N���΍����e�[�u��
		namespace rel_height
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kLinkDivId;
			extern LPCTSTR kFromLevel;
			extern LPCTSTR kToLevel;
		}

		// �ėp�������H�E���s�҃����N���΍������C��
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
				kRoad			= 1, ///< ���H
				kWalk			= 2, ///< ���s��
			};
		}
		namespace walkable {
			enum ECode
			{
				kNotWalk  = 0,  ///< ���s�֎~
				kHumanCar = 1,  ///< �l�ԋ��p
				kWalkOnly = 2,  ///< ���s�Ґ�p
			};
		} // walkable_class
			
	}
	// ���H�W���f�[�^
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
