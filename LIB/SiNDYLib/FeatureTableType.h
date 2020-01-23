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

/**
 * @file FeatureTableType.h
 * @brief <b>フィーチャのクラスとネームスペースの対応管理定義ファイル</b>\n
 * このファイルでは、フィーチャのクラス名とネームスペースの対応を管理します。
 * テーブル名は全て大文字に統一するように注意して下さい。
 * 様々な場面で使用できるように、マクロ形式で定義しています。
 * FEATURETABLE_TYPEマクロを定義し、
 * その後にこのファイルをインクルードしてください。
 * インクルード後はFEATURETABLE_TYPEマクロはundefして下さい。
 * また、最後は必ず0になるようにして下さい。
 * <H3>例：enumとして使用する場合</H3>
 * @code
 * @endcode
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
//FEATURETABLE_TYPE( namespace, tablename, classname )
FEATURETABLE_TYPE( access_point        , ACCESS_POINT        , CAccessPoint )
FEATURETABLE_TYPE( adminarea           , ADMINAREA           , CAdminArea )
FEATURETABLE_TYPE( aerial_photo        , AERIAL_PHOTO        , CAerialPhoto )
FEATURETABLE_TYPE( alley_mainte_list   , ALLEY_MAINTE_LIST   , CAlleyMainteList )
FEATURETABLE_TYPE( annotation_class    , ANNOTATION_CLASS    , CAnnotationClass )
FEATURETABLE_TYPE( b_sc1disp_line      , B_SC1DISP_LINE      , CBSC1DispLine )
FEATURETABLE_TYPE( b_sc2disp_line      , B_SC2DISP_LINE      , CBSC2DispLine )
FEATURETABLE_TYPE( b_sc3disp_line      , B_SC3DISP_LINE      , CBSC3DispLine )
FEATURETABLE_TYPE( b_sc4disp_line      , B_SC4DISP_LINE      , CBSC4DispLine )
FEATURETABLE_TYPE( background_class    , BACKGROUND_CLASS    , CBackGroundClass )
FEATURETABLE_TYPE( base_admin          , BASE_ADMIN          , CBaseAdmin )
FEATURETABLE_TYPE( base_annotation     , BASE_ANNOTATION     , CBaseAnnotation )
FEATURETABLE_TYPE( base_contour        , BASE_CONTOUR        , CBaseContour )
FEATURETABLE_TYPE( base_land           , BASE_LAND           , CBaseLand )
FEATURETABLE_TYPE( base_line           , BASE_LINE           , CBaseLine )
FEATURETABLE_TYPE( base_railway        , BASE_RAILWAY        , CBaseRailway )
FEATURETABLE_TYPE( base_rep_point      , BASE_REP_POINT      , CBaseRepPoint )
FEATURETABLE_TYPE( base_site           , BASE_SITE           , CBaseSite )
FEATURETABLE_TYPE( base_station        , BASE_STATION        , CBaseStation )
FEATURETABLE_TYPE( basemesh            , BASEMESH            , CBaseMesh )
FEATURETABLE_TYPE( building            , BUILDING            , CBuilding )
FEATURETABLE_TYPE( building_line       , BUILDING_LINE       , CBuildingLine )
FEATURETABLE_TYPE( building_step       , BUILDING_STEP       , CBuildingStep )
FEATURETABLE_TYPE( city_admin          , CITY_ADMIN          , CCityAdmin )
FEATURETABLE_TYPE( city_admin_c        , CITY_ADMIN_C        , CCityAdminC )
FEATURETABLE_TYPE( city_admin_pref     , CITY_ADMIN_PREF     , CCityAdminPref )
FEATURETABLE_TYPE( city_annotation     , CITY_ANNOTATION     , CCityAnnotation )
FEATURETABLE_TYPE( city_disp_line      , CITY_DISP_LINE      , CCityDispLine )
FEATURETABLE_TYPE( city_line           , CITY_LINE           , CCityLine )
FEATURETABLE_TYPE( city_railway        , CITY_RAILWAY        , CCityRailway )
FEATURETABLE_TYPE( city_rep_point      , CITY_REP_POINT      , CCityRepPoint )
FEATURETABLE_TYPE( city_site           , CITY_SITE           , CCitySite )
FEATURETABLE_TYPE( city_station        , CITY_STATION        , CCityStation )
FEATURETABLE_TYPE( citymesh            , CITYMESH            , CCityMesh )
FEATURETABLE_TYPE( cityplanmap         , CITYPLANMAP         , CCityPlanMap )
FEATURETABLE_TYPE( claimpoly_addr      , CLAIMPOLY_ADDR      , CClaimPolyAddr )
FEATURETABLE_TYPE( claimpoly_map       , CLAIMPOLY_MAP       , CClaimPolyMap )
FEATURETABLE_TYPE( connectuser         , CONNECTUSER         , CConnectUser )
FEATURETABLE_TYPE( creatablelayers     , CREATABLELAYERS     , CCreatableLayers )
FEATURETABLE_TYPE( cs_addr_point       , CS_ADDR_POINT       , CCSAddrPoint )
FEATURETABLE_TYPE( daikei_point        , DAIKEI_POINT        , CDaikeiPoint )
FEATURETABLE_TYPE( deletablelayers     , DELETABLELAYERS     , CDeletableLayers )
FEATURETABLE_TYPE( dispnavirelation    , DISPNAVIRELATION    , CDispNaviRelation )
FEATURETABLE_TYPE( dispotherrelation   , DISPOTHERRELATION   , CDispOtherRelation )
FEATURETABLE_TYPE( editattrlayers      , EDITATTRLAYERS      , CEditAttrLayers )
FEATURETABLE_TYPE( edithistory         , EDITHISTORY         , CEditHistory )
FEATURETABLE_TYPE( editmesh            , EDITMESH            , CEditMesh )
FEATURETABLE_TYPE( facil_info_point    , FACIL_INFO_POINT    , CFacilInfoPoint )
FEATURETABLE_TYPE( gou_point           , GOU_POINT           , CGouPoint )
FEATURETABLE_TYPE( grad                , GRAD                , CGrad )
FEATURETABLE_TYPE( highway_node        , HIGHWAY_NODE        , CHighwayNode )
FEATURETABLE_TYPE( highway_text        , HIGHWAY_TEXT        , CHighwayText )
FEATURETABLE_TYPE( inf_byway           , INF_BYWAY           , CInfoByway )
FEATURETABLE_TYPE( inf_dirguide        , INF_DIRGUIDE        , CInfoDirguide )
FEATURETABLE_TYPE( inf_guide           , INF_GUIDE           , CInfoGuide )
FEATURETABLE_TYPE( inf_internavi       , INF_INTERNAVI       , CInfoInternavi )
FEATURETABLE_TYPE( inf_lane            , INF_LANE            , CInfoLane )
FEATURETABLE_TYPE( inf_route           , INF_ROUTE           , CInfoRoute )
FEATURETABLE_TYPE( inf_turnreg         , INF_TURNREG         , CInfoTurnReg )
FEATURETABLE_TYPE( inf_uturn           , INF_UTURN           , CInfoUTurn )
FEATURETABLE_TYPE( inf_vics            , INF_VICS            , CInfoVics )
FEATURETABLE_TYPE( inf_ipcvics         , INF_IPCVICS         , CInfoiPCVics )
FEATURETABLE_TYPE( inf_intersection    , INF_INTERSECTION    , CInfoIntersection )
FEATURETABLE_TYPE( ipc_toll_table      , TOLL_OTHER_TABLE    , CTollOtherTable )
FEATURETABLE_TYPE( klink_point         , KLINK_POINT         , CKlinkPoint )
FEATURETABLE_TYPE( layernames          , LAYERNAMES          , CLayerNames )
FEATURETABLE_TYPE( lockarea            , LOCKAREA            , CLockArea )
FEATURETABLE_TYPE( lockmesh            , LOCKMESH            , CLockMesh )
FEATURETABLE_TYPE( lq_byway            , LQ_BYWAY            , CLQByway )
FEATURETABLE_TYPE( lq_dirguide         , LQ_DIRGUIDE         , CLQDirGuide )
FEATURETABLE_TYPE( lq_guide            , LQ_GUIDE            , CLQGuide )
FEATURETABLE_TYPE( lq_internavi        , LQ_INTERNAVI        , CLQInternavi )
FEATURETABLE_TYPE( lq_lane             , LQ_LANE             , CLQLane )
FEATURETABLE_TYPE( lq_route            , LQ_ROUTE            , CLQRoute )
FEATURETABLE_TYPE( lq_turnreg          , LQ_TURNREG          , CLQTurnReg )
FEATURETABLE_TYPE( lq_uturn            , LQ_UTURN            , CLQUTurn )
FEATURETABLE_TYPE( lq_vics             , LQ_VICS             , CLQVics )
FEATURETABLE_TYPE( lq_ipcvics          , LQ_IPCVICS          , CLQiPCVics )
FEATURETABLE_TYPE( lq_intersection     , LQ_INTERSECTION     , CLQIntersection )
FEATURETABLE_TYPE( m_sc1disp_line      , M_SC1DISP_LINE      , CMSC1DispLine )
FEATURETABLE_TYPE( m_sc2disp_line      , M_SC2DISP_LINE      , CMSC2DispLine )
FEATURETABLE_TYPE( m_sc3disp_line      , M_SC3DISP_LINE      , CMSC3DispLine )
FEATURETABLE_TYPE( m_sc4disp_line      , M_SC4DISP_LINE      , CMSC4DispLine )
FEATURETABLE_TYPE( middle_annotation   , MIDDLE_ANNOTATION   , CMiddleAnnotation )
FEATURETABLE_TYPE( middle_contour      , MIDDLE_CONTOUR      , CMiddleContour )
FEATURETABLE_TYPE( middle_line         , MIDDLE_LINE         , CMiddleLine )
FEATURETABLE_TYPE( middle_site         , MIDDLE_SITE         , CMiddleSite )
FEATURETABLE_TYPE( middlemesh          , MIDDLEMESH          , CMiddleMesh )
FEATURETABLE_TYPE( movablelayers       , MOVABLELAYERS       , CMovableLayers )
FEATURETABLE_TYPE( org_internavi       , IVICS               , CIVics )
FEATURETABLE_TYPE( org_vics            , ORG_VICS            , COrgVics )
FEATURETABLE_TYPE( ortho_area          , ORTHO_AREA          , COrthoArea )
FEATURETABLE_TYPE( parea               , PAREA               , CParea )
FEATURETABLE_TYPE( parking_shape       , PARKING_SHAPE       , CParkingShape )
FEATURETABLE_TYPE( road_code_list      , ROAD_CODE_LIST      , CRoadCodeList )
FEATURETABLE_TYPE( road_link           , ROAD_LINK           , CRoadLink )
FEATURETABLE_TYPE( road_node           , ROAD_NODE           , CNode )
FEATURETABLE_TYPE( roadattribute       , ROADATTRIBUTE       , CRoadAttribute )
FEATURETABLE_TYPE( roaddispclass       , ROADDISPCLASS       , CRoadDispClass )
FEATURETABLE_TYPE( roadnaviclass       , ROADNAVICLASS       , CRoadNaviClass )
FEATURETABLE_TYPE( segment_attr        , SEGMENT_ATTR        , CSegmentAttr )
FEATURETABLE_TYPE( shareddeletelayers  , SHAREDDELETELAYERS  , CSharedDeleteLayers )
FEATURETABLE_TYPE( sharedmovelayers    , SHAREDMOVELAYERS    , CSharedMoveLayers )
FEATURETABLE_TYPE( soperator           , OPERATOR            , COperator )
FEATURETABLE_TYPE( t_sc1disp_line      , T_SC1DISP_LINE      , CTSC1DispLine )
FEATURETABLE_TYPE( t_sc2disp_line      , T_SC2DISP_LINE      , CTSC2DispLine )
FEATURETABLE_TYPE( t_sc3disp_line      , T_SC3DISP_LINE      , CTSC3DispLine )
FEATURETABLE_TYPE( t_sc4disp_line      , T_SC4DISP_LINE      , CTSC4DispLine )
FEATURETABLE_TYPE( time_nopassage	   , TIME_NOPASSAGE		 , CTimeNoPassage )
FEATURETABLE_TYPE( time_oneway		   , TIME_ONEWAY		 , CTimeOneway )
FEATURETABLE_TYPE( toll_alone_table    , TOLL_ALONE_TABLE    , CTollAloneTable )
FEATURETABLE_TYPE( top_annotation      , TOP_ANNOTATION      , CTopAnnotation )
FEATURETABLE_TYPE( top_contour         , TOP_CONTOUR         , CTopContour )
FEATURETABLE_TYPE( top_line            , TOP_LINE            , CTopLine )
FEATURETABLE_TYPE( top_site            , TOP_SITE            , CTopSite )
FEATURETABLE_TYPE( workpurpose         , WORKPURPOSE         , CWorkPurpose )
FEATURETABLE_TYPE( mpq_build_point     , MPQ_BUILD_POINT     , CMPQBuildPoint )
FEATURETABLE_TYPE( other               , DUMMY               , CRow )
// 自動注記用
FEATURETABLE_TYPE( city_disp_point     , CITY_DISP_POINT     , CCityDispPoint )
FEATURETABLE_TYPE( base_disp_point     , BASE_DISP_POINT     , CBaseDispPoint )
FEATURETABLE_TYPE( init_disp_point     , INIT_DISP_POINT     , CInitDispPoint )
FEATURETABLE_TYPE( init_disp_line      , INIT_DISP_LINE		 , CInitDispLine )

// D1注記用
// 2006.08.10 F.Abe
FEATURETABLE_TYPE( c_sc1disp_line      , C_SC1DISP_LINE      , CCSC1DispLine )
FEATURETABLE_TYPE( d1_annotation       , D1_ANNOTATION       , CD1Annotation )

// 歩行者用
FEATURETABLE_TYPE( walk_link           , WALK_LINK           , CWalkLink )
FEATURETABLE_TYPE( walk_node           , WALK_NODE           , CWalkNode )

FEATURETABLE_TYPE( guide_link          , GUIDE_LINK          , CGuideLink )
FEATURETABLE_TYPE( edit_area           , EDIT_AREA           , CEditArea )

FEATURETABLE_TYPE( walk_nopassage	   , WALK_NOPASSAGE		 , CWalkNoPassage )
FEATURETABLE_TYPE( walk_oneway		   , WALK_ONEWAY		 , CWalkOneway )
FEATURETABLE_TYPE( link_relation	   , LINK_RELATION		 , CLinkRelation )

FEATURETABLE_TYPE( underground_site    , UNDERGROUND_SITE    , CUndergroundSite )
FEATURETABLE_TYPE( underground_line    , UNDERGROUND_LINE    , CUndergroundLine )

// MPQ
FEATURETABLE_TYPE( mpq_point		   , MPQ_POINT			 , CMPQPoint )
FEATURETABLE_TYPE( inf_measure		   , INF_MEASURE		 , CInfMeasure )
FEATURETABLE_TYPE( inf_drawing		   , INF_DRAWING		 , CInfDrawing )
FEATURETABLE_TYPE( inf_segment		   , INF_SEGMENT		 , CInfSegment )
FEATURETABLE_TYPE( inf_pattern		   , INF_PATTERN		 , CInfPattern )
FEATURETABLE_TYPE( inf_excluobj		   , INF_EXCLUOBJ		 , CInfExcluObj )

FEATURETABLE_TYPE( pq_measure		   , PQ_MEASURE		     , CPqMeasure )
FEATURETABLE_TYPE( pq_drawing		   , PQ_DRAWING		 	 , CPqDrawing )
FEATURETABLE_TYPE( pq_segment		   , PQ_SEGMENT			 , CPqSegment )
FEATURETABLE_TYPE( pq_pattern		   , PQ_PATTERN			 , CPqPattern )
FEATURETABLE_TYPE( pq_excluobj		   , PQ_EXCLUOBJ		 , CPqExcluObj )

FEATURETABLE_TYPE( sector_info_master  , SECTOR_INFO_MASTER  , CSectorInfoMaster )
FEATURETABLE_TYPE( column_registry     , COLUMN_REGISTRY     , CColumnRegistry )
FEATURETABLE_TYPE( manageid_master     , MANAGEID_MASTER     , CManageIDMaster )

FEATURETABLE_TYPE( exclusive_obj	   , EXCLUSIVE_OBJ		 , CExclusiveObj )
FEATURETABLE_TYPE( ground		       , GROUND				 , CGround )
FEATURETABLE_TYPE( height_log		   , HEIGHT_LOG			 , CHeightLog )
FEATURETABLE_TYPE( pithead			   , PITHEAD			 , CPitHead )

// 国際都市地図
FEATURETABLE_TYPE( frm_a			   , FRM_A				 , CFrm_a )
FEATURETABLE_TYPE( frm_p			   , FRM_P				 , CFrm_p )
FEATURETABLE_TYPE( hig_a			   , HIG_A				 , CHig_a )
FEATURETABLE_TYPE( hig_p			   , HIG_P				 , CHig_p )
FEATURETABLE_TYPE( dec_p			   , DEC_P				 , CDec_p )
FEATURETABLE_TYPE( zlv_p			   , ZLV_P				 , CZlv_p )
FEATURETABLE_TYPE( tertiarymesh		   , TERTIARYMESH		 , CTertiarymesh )

// SJ国際都市地図2ndフェーズ
FEATURETABLE_TYPE( rooftop			   , ROOFTOP			 , CRooftop )
FEATURETABLE_TYPE( walkway			   , WALKWAY			 , CWalkway )
FEATURETABLE_TYPE( other_lc			   , OTHER_LC			 , COtherLC )
FEATURETABLE_TYPE( railway_ground	   , RAILWAY_GROUND		 , CRailwayGround )
FEATURETABLE_TYPE( road_polygon		   , ROAD_POLYGON		 , CRoadPolygon )
FEATURETABLE_TYPE( predominant_lc	   , PREDOMINANT_LC		 , CPredominantLC )

// SJ国内
FEATURETABLE_TYPE( subway_link         , SUBWAY_LINK         , CSubwayLink )
FEATURETABLE_TYPE( subway_node         , SUBWAY_NODE         , CSubwayNode )
FEATURETABLE_TYPE( railway_link        , RAILWAY_LINK        , CRailwayLink )
FEATURETABLE_TYPE( railway_node        , RAILWAY_NODE        , CRailwayNode )
FEATURETABLE_TYPE( rel_railway_link    , REL_RAILWAY_LINK    , CRelRailwayLink )
FEATURETABLE_TYPE( rel_railway_node    , REL_RAILWAY_NODE    , CRelRailwayNode )

FEATURETABLE_TYPE( zipcode_area        , ZIPCODE_AREA        , CZipCodeArea )

// GS
FEATURETABLE_TYPE( city_area           , CITY_AREA           , CCityArea )
FEATURETABLE_TYPE( city_bridge_line    , CITY_BRIDGE_LINE    , CCityBridgeLine )
FEATURETABLE_TYPE( city_road           , CITY_ROAD           , CCityRoad )
FEATURETABLE_TYPE( city_split_line     , CITY_SPLIT_LINE     , CCitySpritLine )
FEATURETABLE_TYPE( city_tunnel         , CITY_TUNNEL         , CCityTunnel )
FEATURETABLE_TYPE( building_roof       , BUILDING_ROOF       , CBuildingRoof )

// 車種別規制
FEATURETABLE_TYPE( vehicle_reg         , VEHICLE_REG         , CVehicleReg )
FEATURETABLE_TYPE( vehicle_type        , VEHICLE_TYPE        , CVehicleType )

// Global(Thai)
FEATURETABLE_TYPE( land_use            , LAND_USE            , CLandUse )
FEATURETABLE_TYPE( waterway            , WATERWAY            , CWaterWay )
FEATURETABLE_TYPE( waterway_line       , WATERWAY_LINE       , CWaterWayLine )
FEATURETABLE_TYPE( land_cover          , LAND_COVER          , CLandCover )
FEATURETABLE_TYPE( island              , ISLAND              , CIsland )
FEATURETABLE_TYPE( country             , COUNTRY             , CCountry )
FEATURETABLE_TYPE( district            , DISTRICT            , CDistrict )
FEATURETABLE_TYPE( subdistrict         , SUBDISTRICT         , CSubDistrict )
FEATURETABLE_TYPE( province            , PROVINCE            , CProvince )
FEATURETABLE_TYPE( village             , VILLAGE             , CVillage )
FEATURETABLE_TYPE( namedarea           , NAMEDAREA           , CNamedArea )
FEATURETABLE_TYPE( postaldistrict      , POSTALDISTRICT      , CPostalDistrict )
FEATURETABLE_TYPE( poi_entrypoint      , POI_ENTRYPOINT      , CPOIEntryPoint )

// 自動運転支援マップ
FEATURETABLE_TYPE( lane_link           , LANE_LINK           , CLaneLink )
FEATURETABLE_TYPE( lane_node           , LANE_NODE           , CLaneNode )
FEATURETABLE_TYPE( updown_link         , UPDOWN_LINK         , CUpDownLink )
FEATURETABLE_TYPE( updown_node         , UPDOWN_NODE         , CUpDownNode )
FEATURETABLE_TYPE( compart_link        , COMPART_LINK        , CCompartLink )
FEATURETABLE_TYPE( compart_node        , COMPART_NODE        , CCompartNode )
FEATURETABLE_TYPE( border_link         , BORDER_LINK         , CBorderLink )
FEATURETABLE_TYPE( border_node         , BORDER_NODE         , CBorderNode )
FEATURETABLE_TYPE( signal_point        , SIGNAL_POINT        , CSignalPoint )
FEATURETABLE_TYPE( roadsign_point      , ROADSIGN_POINT      , CRoadsignPoint )
FEATURETABLE_TYPE( crosswalk_line      , CROSSWALK_LINE      , CCrosswalkLine )
FEATURETABLE_TYPE( roadmark_area       , ROADMARK_AREA       , CRoadmarkArea )

FEATURETABLE_TYPE( unknown             , DUMMY               , CRow )

