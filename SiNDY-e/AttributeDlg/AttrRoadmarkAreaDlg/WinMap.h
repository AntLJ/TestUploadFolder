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
#include "resource.h"
#include <SizingFramework/include/WinMgr.h>

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(ROADMARK_AREA_MAP)
	BEGINROWS(WRCT_REST, 0, RCMARGINS(5, 5))
			BEGINROWS(WRCT_REST,0,0)
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_ROADMARK_C)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_ROADMARK_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_DESCRIPTION)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_DESCRIPTION)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_REL_LANE_LINK)
					RCTOFIT(-1)
					RCREST(IDC_VSFLEXGRID_LANE_LINK)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_SPECULATION_C)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_SPECULATION_C)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_SOURCE_ID)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_SOURCE_ID)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4, 4))
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0, 0))
					RCREST(IDC_STATIC_BAR)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
					RCTOFIT(IDC_STATIC_SOURCE)
					RCTOFIT(-1)
					RCREST (IDC_EDIT_SOURCE)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
