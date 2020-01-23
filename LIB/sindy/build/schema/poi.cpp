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
	const TCHAR kTableName[]		= _T("RAILWAY_CORP");		//!< �e�[�u����
	const TCHAR kCorpCode[]			= _T("CORPCODE");			//!< ��ЃR�[�h
	const TCHAR kName[]				= _T("NAME");				//!< ��Ж�
	const TCHAR kYomi[]				= _T("YOMI");				//!< ��Ж��ǂ�
	const TCHAR kExName[]			= _T("EXNAME");				//!< ��З���
	const TCHAR kExYomi[]			= _T("EXYOMI");				//!< ��З��̓ǂ�
} // railway_corp

namespace railway_line {
	const TCHAR kTableName[]		= _T("RAILWAY_LINE");		//!< �e�[�u����
	const TCHAR kCorpID[]			= _T("CORPID");				//!< �S����Ђh�c
	const TCHAR kLineCode[]			= _T("LINECODE");			//!< �H���R�[�h
	const TCHAR kName[]				= _T("NAME");				//!< �H����
	const TCHAR kYomi[]				= _T("YOMI");				//!< �H�����ǂ�
	const TCHAR kExName[]			= _T("EXNAME");				//!< �H������
	const TCHAR kExYomi[]			= _T("EXYOMI");				//!< �H�����̓ǂ�
	const TCHAR kTypeID[]			= _T("TYPEID");				//!< ��ʂh�c
} // railway_line

namespace join_line_info {
	const TCHAR kTableName[]	= _T("JOIN_LINE_INFO");		//!< �e�[�u����
	const TCHAR kLineID[]		= _T("LINEID");				//!< �H��ID
	const TCHAR kLineSeq[]		= _T("LINESEQ");			//!< �H�����V�[�P���X
	const TCHAR kInfoID[]		= _T("INFOID");				//!< �w���ID
	const TCHAR kSubName[]		= _T("SUBNAME");			//!< �⑫����
	const TCHAR kSubYomi[]		= _T("SUBYOMI");			//!< �⑫���̓ǂ�
	const TCHAR kStationID[]	= _T("STATIONID");			//!< �w�|�C���gID
	const TCHAR kContentsSeq[]	= _T("CONTENTS_SEQ");		//!< �R���e���c���V�[�P���X
} // join_line_info

namespace station_info {
	const TCHAR kTableName[]	= _T("STATION_INFO");	//!< �e�[�u����
	const TCHAR kCorpID[]		= _T("CORPID");			//!< �S�����ID
	const TCHAR kName[]			= _T("NAME");			//!< �w��
	const TCHAR kYomi[]			= _T("YOMI");			//!< �w���ǂ�
	const TCHAR kAddrCode[]		= _T("ADDRCODE");		//!< �Z���R�[�h
	const TCHAR kTel[]			= _T("TEL");			//!< �d�b�ԍ�
	const TCHAR kAnAreaCode[]	= _T("ANAREACODE");		//!< �s�O�ǔ�
} // station_info

namespace join_station_gate {
	const TCHAR kTableName[]	= _T("JOIN_STATION_GATE");		//!< �e�[�u����
	const TCHAR kJLineID[]		= _T("JLINEID");				//!< ������H��ID
	const TCHAR kGateID[]		= _T("GATEID");					//!< �w�o����ID
	const TCHAR kContentsSeq[]	= _T("CONTENTS_SEQ");			//!< �R���e���c���V�[�P���X
} // join_station_gate

namespace station_point {
	const TCHAR kTableName[]		= _T("STATION_POINT");		//!< �e�[�u����	
	const TCHAR kGuideID[]			= _T("GUIDEID");			//!< �U���|�C���gID
} // station_point

namespace station_gate_point {
	const TCHAR kTableName[]		= _T("STATION_GATE_POINT");	//!< �e�[�u����

	const TCHAR kName[]				= _T("NAME");				//!< �o��������
	const TCHAR kYomi[]				= _T("YOMI");				//!< �o�������̓ǂ�	
	const TCHAR kGuideID[]			= _T("GUIDEID");			//!< �U���|�C���gID
} // station_gate_point

namespace station_guide_point {
	const TCHAR kTableName[]		= _T("STATION_GUIDE_POINT");	//!< �e�[�u����
	const TCHAR kTollRoad[]			= _T("TOLLROAD_F");				//!< �L�����H�t���O
} // station_guide_point

namespace join_station_walk {
	const TCHAR kTableName[]	= _T("JOIN_STATION_WALK");		//!< �e�[�u����
	const TCHAR kJLineID[]		= _T("JLINEID");				//!< ������H��ID
	const TCHAR kWalkPoiID[]	= _T("WALKPOIID");				//!< �w�o����ID
} // join_station_gate

namespace walk_poi_point {
	const TCHAR kTableName[]		= _T("WALK_POI_POINT");	//!< �e�[�u����

	const TCHAR kPOIClass[]		= _T("POICLASS_C");		//!< ���s��POI���
	const TCHAR kName[]			= _T("NAME");			//!< ����
	const TCHAR kYomi[]			= _T("YOMI");			//!< ���̓ǂ�
	const TCHAR kFloorLevel[]		= _T("FLOOR_LEVEL");	//!< �K�w�l
	const TCHAR kIndoor[]			= _T("INDOOR_F");		//!< �{�ݓ��t���O
	const TCHAR kPicName[]			= _T("PICNAME");		//!< �摜�t�@�C����
} // walk_poi_point

namespace poi_mstchn {
	const _TCHAR kTableName[]		= _T("POI_MSTCHN");			//!< �e�[�u����

	const _TCHAR kChainCode[]		= _T("CHAINCODE");			//!< �`�F�[���X�}�X�^�[�R�[�h�i8���j
	const _TCHAR kChainCodeOld[]	= _T("CHAINCODE_OLD");		//!< �`�F�[���X�}�X�^�[�R�[�h�i6���j
	const _TCHAR kName[]			= _T("NAME");				//!< �X�ܖ���
	const _TCHAR kYomi[]			= _T("YOMI");				//!< �X�ܖ��̓ǂ�
} // poi_mstchn

namespace traintype {
	const _TCHAR kTableName[] = _T("TRAINTYPE"); //!< �e�[�u����

	const _TCHAR kTypeCode[] = _T("TYPECODE"); //!<��ʃR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kYomi[] = _T("YOMI"); //!< �ǂ�
} // traintype

} // schema

} // sindy
