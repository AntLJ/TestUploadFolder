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
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			RCPERCENT(0, 5)
			BEGINROWS(WRCT_PCT, 0, 15)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_BATCHCHANGE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_BATCHCHANGE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 15)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_DISPCLASS)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_DISPCLASS)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 15)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_NAVICLASS)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_NAVICLASS)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 15)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_MANAGER)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_MANAGER)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 15)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_ROADWIDTH)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_ROADWIDTH)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 25)
				BEGINCOLS(WRCT_TOFIT, 0, 10)
					RCTOFIT(IDC_STATIC_LINKCLASS)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_COMBO_LINKCLASS1)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（追加属性タブ用）
BEGIN_WINDOW_MAP(TUIKAMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			RCPERCENT(0, 5)
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_CHECK_ISLANDF)
					RCTOFIT(IDC_CHECK_BYPASS)
					RCTOFIT(IDC_CHECK_CARONLYF)
					RCTOFIT(IDC_CHECK_WIDEAREAF)			// [bug 6712] 広域フラグ用
					RCTOFIT(-1)
					RCREST(IDC_COMBO_WIDEAREA)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_UPDOWNC)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_UPDOWNC)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_LANE)
					RCTOFIT(IDC_CHECK_LANE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_RIGHTCHANGE)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_RIGHTCHANGE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LEFTCHANGE)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_LEFTCHANGE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LEGALSPEED)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_LEGALSPEED)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_ALLEYCODE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_ALLEYMAINTE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_WORKYEAR)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_WORKYEAR)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_WALKABLE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_WALKABLE)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（P_LINK用）
BEGIN_WINDOW_MAP(KOTUKISEIMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			RCTOFIT(-1)
			BEGINROWS(WRCT_REST, 0, RCMARGINS(10,2))
				BEGINROWS(WRCT_PCT, IDC_STATIC_ONEWAYGROUP, 50)
					RCSPACE(15)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_REGULERONEWAY)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_ONEWAY)
					ENDGROUP()
					RCSPACE(5)
					RCTOFIT(-1)
					BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
						RCREST(IDC_GRID_ONEWAY)
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
				RCSPACE(10)
				BEGINROWS(WRCT_PCT, IDC_STATIC_NOPASSAGEGROUP, 50)
					RCSPACE(15)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))	// 通常通行禁止
						RCTOFIT(IDC_STATIC_REGULERNOPASSAGE)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_NOPASSAGE)
					ENDGROUP()
					RCSPACE(5)
					RCTOFIT(-1)
					BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
						RCREST(IDC_GRID_NOPASSAGE)
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(4)
		ENDGROUP()
		RCTOFIT(-1)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（路線番号タブ用）[bug 12036]
BEGIN_WINDOW_MAP(ROSENMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			RCPERCENT(0, 3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_ROADCODE)
				RCREST(IDC_EDIT_ROADCODE)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHROADCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_NAMEKANJI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAMEKANJI)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_DISPLAYKANJI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DISPLAYKANJI)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_SHIELDNUMBER)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SHIELDNUMBER)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_BAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_PREFCODE)
				RCREST(IDC_EDIT_PREFCODE)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHPREFCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_PREFNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PREFNAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_ROADNUMBER)
				RCREST(IDC_EDIT_ROADNUMBER)
				RCTOFIT(-1)
				RCTOFIT(IDC_STATIC_ROADNUMBER2)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHROADNUMBER)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_PREFROADNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PREFROADNAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_BAR2)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_HIGHWAYNUMBER)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_HIGHWAYNUMBER)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_BAR3)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_SECTIONCODE)
				RCREST(IDC_EDIT_SECTIONCODE)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHSECTIONCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_SECTIONNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SECTIONNAME)
			ENDGROUP()
			RCSPACE(5)
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（DRMAタブ用）
BEGIN_WINDOW_MAP(DRMAMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			BEGINROWS(WRCT_PCT, 0, 30)
				BEGINCOLS(WRCT_TOFIT, 10, 10)
					RCTOFIT(IDC_STATIC_KUBUNCODE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_KUBUNCODE)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 30)
				BEGINCOLS(WRCT_TOFIT, 10, 10)
					RCTOFIT(IDC_STATIC_LIMITSPEED)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_LIMITSPEED)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 30)
				BEGINCOLS(WRCT_TOFIT, 10, 10)
					RCTOFIT(IDC_STATIC_TRIPSPEED)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_TRIPSPEED)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// ウィンドウマネージャ配置マップ（駐車場タブ用）
BEGIN_WINDOW_MAP(PARKINGMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,IDC_TAB_ROADATTR,0)
			RCSPACE(22)	// タブのつまみ分
			RCSPACE(30)
			BEGINROWS(WRCT_PCT, 0, 20)
				BEGINCOLS(WRCT_TOFIT, 10, 10 )
					RCTOFIT(IDC_STATIC_PARKINGCLASS)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_PARKINGCLASS)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 20)
				BEGINCOLS(WRCT_TOFIT, 10, 10 )
					RCTOFIT(IDC_STATIC_PARKINGLINKCLASS)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_PARKINGLINKCLASS)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 20)
				BEGINCOLS(WRCT_TOFIT, 10, 10 )
					RCTOFIT(IDC_STATIC_FLOORLEVEL)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_FLOORLEVEL)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_PCT, 0, 20)
				BEGINCOLS(WRCT_TOFIT, 10, 10 )
					RCTOFIT(IDC_STATIC_MIDDLEFLOOR_LEVEL)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_MIDDLEFLOOR_LEVEL)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, 0)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

