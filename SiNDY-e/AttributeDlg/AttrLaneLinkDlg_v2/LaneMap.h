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

///  �E�B���h�E�}�l�[�W���z�u�}�b�v�i��{�����^�u�p�j
BEGIN_WINDOW_MAP(KIHONMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
      BEGINROWS(WRCT_PCT,0,90)
	    BEGINROWS(WRCT_REST,IDC_TAB_LANEATTR,0)
	        RCSPACE(22)	// �^�u�̂܂ݕ�			
			//���́E�ǂ�
			BEGINROWS(WRCT_TOFIT, IDC_STATIC_BASE_GROUP, RCMARGINS(10,10))
				RCSPACE(10)
				//��{���
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_FROM_NODE_ID)
					RCTOFIT(-1)
					RCSPACE(15)
					RCREST(IDC_EDIT_FROM_NODE_ID)	
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_TO_NODE_ID)
					RCTOFIT(-1)
					RCSPACE(15)
					RCREST(IDC_EDIT_TO_NODE_ID)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE_GROUP_ID)
					RCTOFIT(-1)
					RCSPACE(15)
					RCREST(IDC_EDIT_LANE_GROUP_ID)	
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE_GROUP_C)
					RCTOFIT(-1)
					RCSPACE(15)
					RCREST(IDC_EDIT_LANE_GROUP_C)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_ROAD_STRUCTURE_C)
					RCTOFIT(-1)
					RCSPACE(5)
					RCREST(IDC_COMBO_ROAD_STRUCTURE_C)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE_NUMBER)
					RCTOFIT(-1)
					RCSPACE(5)
					RCREST(IDC_EDIT_LANE_NUMBER)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_LANE_COUNT)
					RCTOFIT(-1)
					RCSPACE(5)
					RCREST(IDC_EDIT_LANE_COUNT)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_MAX_LEGAL_SPEED_C)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_COMBO_MAX_LEGAL_SPEED_C)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_MIN_LEGAL_SPEED_C)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_COMBO_MIN_LEGAL_SPEED_C)
				ENDGROUP()
			ENDGROUP()
			BEGINROWS(WRCT_REST, IDC_STATIC_EXTENSION_GROUP, RCMARGINS(10,10))
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_TRAVEL_DIRECTION_C)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_COMBO_TRAVEL_DIRECTION_C)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_CHANGE_F)
					RCTOFIT(-1)
					RCSPACE(0)
					RCTOFIT(IDC_CHECK_LEFT_CHANGE_F)
					RCSPACE(0)
					RCTOFIT(IDC_CHECK_RIGHT_CHANGE_F)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_PROTRUSION_F)
					RCTOFIT(-1)
					RCSPACE(0)
					RCTOFIT(IDC_CHECK_LEFT_PROTRUSION_F)
					RCSPACE(0)
					RCTOFIT(IDC_CHECK_RIGHT_PROTRUSION_F)
				ENDGROUP()
				RCSPACE(0)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_CHECK_RUBBING_F)
				ENDGROUP()
				RCSPACE(0)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_CHECK_TUNNEL_F)
				ENDGROUP()
				RCSPACE(10)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_ROAD_LINK_ID)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_EDIT_ROAD_LINK_ID)	
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_SPECULATION_C)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_COMBO_SPECULATION_C)
				ENDGROUP()
				RCSPACE(3)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
					RCTOFIT(IDC_STATIC_SOURCE_ID)
					RCTOFIT(-1)
					RCSPACE(0)
					RCREST(IDC_EDIT_SOURCE_ID)
				ENDGROUP()
			ENDGROUP()
		  ENDGROUP()
		ENDGROUP()			
		//���\�[�X
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

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�m�[�h�񑮐��^�u�p�j
BEGIN_WINDOW_MAP(NODEMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
	  BEGINROWS(WRCT_PCT,0,90)
		BEGINROWS(WRCT_REST,IDC_TAB_LANEATTR,0)
			RCSPACE(22)	// �^�u�̂܂ݕ�			
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