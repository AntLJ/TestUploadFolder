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

///  ウィンドウマネージャ配置マップ（基本属性タブ用）
BEGIN_WINDOW_MAP(KIHONMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
        BEGINROWS(WRCT_PCT,0,90)
	      BEGINROWS(WRCT_REST,IDC_TAB_UPDOWNATTR,0)
	      RCSPACE(22)	// タブのつまみ分			
		  	
			 BEGINROWS(WRCT_TOFIT, IDC_STATIC_ATTR_GROUP, RCMARGINS(10,10))
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_UPDOWN_GROUP_ID)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_UPDOWN_GROUP_ID)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_GROUP_C)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_GROUP_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE_COUNT)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_LANE_COUNT)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_RIGHT_CHANGE)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_RIGHT_CHANGE)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LEFT_CHANGE)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_LEFT_CHANGE)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_MAX_LEGAL_SPEED_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_MAX_LEGAL_SPEED_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_MIN_LEGAL_SPEED_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_MIN_LEGAL_SPEED_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_TRAVEL_DIRECTION_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_TRAVEL_DIRECTION_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_ROAD_STRUCTURE_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_ROAD_STRUCTURE_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_ONEWAY_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_ONEWAY_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_CHECK_RUBBING_F)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_CHECK_TUNNEL_F)
				ENDGROUP()
			ENDGROUP()
			
			BEGINROWS(WRCT_REST, IDC_STATIC_PECULATION_C_GROUP, RCMARGINS(10,10))
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_SPECULATION_C)
					RCFIXED(0,10)
					RCREST(IDC_COMBO_SPECULATION_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_SOURCE_ID)
					RCFIXED(0,10)
					RCREST(IDC_EDIT_SOURCE_ID)
				ENDGROUP()
			ENDGROUP()
		  ENDGROUP()
		ENDGROUP()			
		//情報ソース
		BEGINROWS(WRCT_PCT, 0, 10)
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()	
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（ノード列属性タブ用）
BEGIN_WINDOW_MAP(NODEMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
	  BEGINROWS(WRCT_PCT,0,90)
		BEGINROWS(WRCT_REST,IDC_TAB_UPDOWNATTR,0)
			RCSPACE(22)	// タブのつまみ分			
			BEGINROWS(WRCT_REST, 0, RCMARGINS(10,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					    RCTOFIT(IDC_STATIC_NODELIST_INFO)
				    ENDGROUP()
				    RCSPACE(3)					
					RCTOFIT(-1)
					BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
						RCREST(IDC_GRID_NODELIST)
					ENDGROUP()							
			ENDGROUP()
			RCSPACE(4)
		ENDGROUP()
	ENDGROUP()
	BEGINROWS(WRCT_PCT,0,10)		
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
  ENDGROUP()
END_WINDOW_MAP()