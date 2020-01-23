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
#include <sindy/schema/sj.h>

namespace sindy {

namespace schema {

namespace sj{

const _TCHAR kRooftopTableName[]     = _T("ROOFTOP");

const _TCHAR kWalkwayTableName[]     = _T("WALKWAY");

const _TCHAR kOtherLCTableName[]     = _T("OTHER_LC");

const _TCHAR kRailwayTableName[]     = _T("RAILWAY_GROUND");

const _TCHAR kRoadTableName[]        = _T("ROAD_POLYGON");

const _TCHAR kPredominantTableName[] = _T("PREDOMINANT_LC");

const _TCHAR kLCCLS[] = _T("LCCLS");
const _TCHAR kLUCLS[] = _T("LUCLS");
const _TCHAR kRoadF[] = _T("ROAD_F");


/// �n���S�����N
namespace subway_link {
	const _TCHAR kTableName[]     = _T("SUBWAY_LINK");     //!< �e�[�u����

	const _TCHAR kFromNodeID[]    = _T("FROM_NODE_ID");    //!< �n�_�m�[�hID
	const _TCHAR kToNodeID[]      = _T("TO_NODE_ID");      //!< �I�_�m�[�hID
	const _TCHAR kRailwayLineID[] = _T("RAILWAY_LINE_ID"); //!< �S���H��ID
}

/// �n���S�m�[�h
namespace subway_node {
	const _TCHAR kTableName[]     = _T("SUBWAY_NODE");       //!< �e�[�u����

	const _TCHAR kNodeClassC[]    = _T("NODECLASS_C");       //!< �m�[�h���
	const _TCHAR kJoinLineInfo[]  = _T("JOIN_LINE_INFO_ID"); //!< ������H��ID
	const _TCHAR kStationNumber[] = _T("STATION_NUMBER");    //!< �w�ԍ�
}

/// �����E��������|���S��
namespace nationalpark_site {
	const _TCHAR kTableName[] = _T("NATIONALPARK_SITE"); //!< �e�[�u����

	const _TCHAR kNparkID[]   = _T("NPARKID");           //!< �����֘A�e�[�u��ID
}

///�����E��������֘A�e�[�u��
namespace nationalpark_table {
	const _TCHAR kTableName[]  = _T("NATIONALPARK_TABLE"); //!< �e�[�u����

	const _TCHAR kParkTypeC[]  = _T("PARKTYPE_C");         //!< ������ʃR�[�h
	const _TCHAR kParkNumber[] = _T("PARKNUMBER");         //!< �����ԍ�
	const _TCHAR kName[]       = _T("NAME");               //!< ����
}

// �s�s�n�}���L�w�i�|���S��
namespace city_anno_site {
	const _TCHAR kTableName[]  = _T("CITY_ANNO_SITE");  //!< �e�[�u����

	const _TCHAR kCityAnnoID[] = _T("CITYANNO_ID"); //!< �s�s���LObjectID
	const _TCHAR kAnnoClassC[] = _T("ANNOCLASS_C"); //!< �s�s���L��ʃR�[�h
	const _TCHAR kCitySiteID[] = _T("CITYSITE_ID"); //!< �s�s�w�iObjectID
	const _TCHAR kBGClassC[]   = _T("BGCLASS_C");   //!< �s�s�w�i��ʃR�[�h
}

// ���k���L�w�i�R�t���e�[�u��
namespace rel_base_anno_site {

	const _TCHAR kTableName[]  = _T("REL_BASE_ANNO_SITE"); //!< �e�[�u����

	const _TCHAR kBaseAnnoID[] = _T("BASEANNO_ID");        //!< ���k���LID
	const _TCHAR kAnnoClassC[] = _T("ANNOCLASS_C");        //!< ���k���L��ʃR�[�h
	const _TCHAR kBaseSiteID[] = _T("BASESITE_ID");        //!< ���k�w�iObjectID
	const _TCHAR kBGClassC[]   = _T("BGCLASS_C");          //!< ���k�w�i��ʃR�[�h
}

// �����r�����̋��ʕ���
namespace buildingname_common {
	const _TCHAR kName[] = _T("NAME");						//!< ����
	const _TCHAR kSourceType[] = _T("SOURCETYPE_C");		//!< �o�T�R�[�h
}

// �����r�����̋��ʕ����i�t�����j
namespace buildingname_common_addinfo {
	const _TCHAR kNameSeparated[] = _T("NAME_SEPARATED");	//!< ���́i�����j
	const _TCHAR kYomiSeparated[] = _T("YOMI_SEPARATED");	//!< ���~�i�����j
	const _TCHAR kBldgClass1[] = _T("BLDGCLASS1_C");		//!< ������ʃR�[�h1
	const _TCHAR kBldgClass2[] = _T("BLDGCLASS2_C");		//!< ������ʃR�[�h2
	const _TCHAR kBldgClass3[] = _T("BLDGCLASS3_C");		//!< ������ʃR�[�h3
	const _TCHAR kFloors[] = _T("FLOORS");					//!< �K��
	const _TCHAR kSuitable[] = _T("SUITABLE_F");			//!< �r�����̑Ó��t���O
	const _TCHAR kFixClass[] = _T("FIXCLASS_C");			//!< ��ʊm��R�[�h
}

// �����r�����̑f�ރ|�C���g
namespace buildingname_src_point {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_SRC_POINT"); //!< �e�[�u����

	const _TCHAR kOrgName1[] = _T("ORGNAME1");				//!< ���H�O����1
	const _TCHAR kOrgName2[] = _T("ORGNAME2");				//!< ���H�O����2
	const _TCHAR kKana[] = _T("KANA");						//!< �J�i
	const _TCHAR kAddr[] = _T("ADDR");						//!< �Z��
	const _TCHAR kOrgAddr[] = _T("ORGADDR");				//!< ���`�O�Z��
	const _TCHAR kAddrCode[] = _T("ADDRCODE");				//!< �Z���R�[�h
	const _TCHAR kAddrLon[] = _T("ADDR_LON");				//!< �Z���o�x
	const _TCHAR kAddrLat[] = _T("ADDR_LAT");				//!< �Z���ܓx
	const _TCHAR kRepType[] = _T("REPTYPE_C");				//!< �n�ԑ�\�_�����R�[�h
	const _TCHAR kAddrLevel[] = _T("ADDRLEVEL_C");			//!< �Z�����x���R�[�h
	const _TCHAR kPinPoint[] = _T("PINPOINT_F");			//!< �s���|�C���g�t���O
	const _TCHAR kMultiSameBldg[] = _T("MULTISAMEBLDG_F");	//!< ����ƌ`�������Z�����݃t���O
	const _TCHAR kSameAddr[] = _T("SAMEADDR_F");			//!< ����iPC�Z���������݃t���O
	const _TCHAR kSameAddrCount[] = _T("SAMEADDR_COUNT");	//!< ����iPC�Z����
	const _TCHAR kAddrVer[] = _T("ADDR_VER");				//!< �Z����͌�XYDB��
	const _TCHAR kSourceName[] = _T("SOURCENAME");			//!< �o�T��
	const _TCHAR kSourceDate[] = _T("SOURCEDATE");			//!< �o�T�������莞��
	const _TCHAR kSourceID[] = _T("SOURCE_ID");				//!< �o�T����ID
	const _TCHAR kBuilding1Div1[] = _T("BUILDING1_DIV1");	//!< �p�r�敪1
	const _TCHAR kBuilding1Div2[] = _T("BUILDING1_DIV2");	//!< �p�r�敪2
	const _TCHAR kBuilding2Div1[] = _T("BUILDING2_DIV1");	//!< �������p�r�敪1
	const _TCHAR kBuilding2Div2[] = _T("BUILDING2_DIV2");	//!< �������p�r�敪2
	const _TCHAR kHeight[] = _T("HEIGHT");					//!< ��������
	const _TCHAR kFloors[] = _T("FLOORS");					//!< �n��K��
	const _TCHAR kBasementFloors[] = _T("BASEMENTFLOORS");	//!< �n���K��
	const _TCHAR kBuildingArea[] = _T("BUILDINGAREA");		//!< �����ʐ�
	const _TCHAR kAllArea[] = _T("ALLAREA");				//!< �q�זʐ�
	const _TCHAR kSrcStartDate[] = _T("SRC_STARTDATE");		//!< �f�ގd�l�J�n�N����
	const _TCHAR kSrcUpdateDate[] = _T("SRC_UPDATEDATE");	//!< �f�ލX�V�N����
	const _TCHAR kAnyPlaceID[] = _T("ANYPLACE_ID");			//!< AnyPlaceID
	const _TCHAR kUpdateDate[] = _T("UPDATEDATE");			//!< �X�V�N����
	const _TCHAR kAdoption[] = _T("ADOPTION_C");			//!< �̗p�R�[�h
}

// �����r�����̑f�ޕt�����
namespace buildingname_src_addinfo {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_SRC_ADDINFO");		//!< �e�[�u����

	const _TCHAR kSourceName[] = _T("SOURCENAME");			//!< �o�T��
	const _TCHAR kSourceID[] = _T("SOURCE_ID");				//!< �o�T����ID
}

// �����r�����̃|�C���g
namespace buildingname_point {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_POINT");	//!< �e�[�u����

	const _TCHAR kBuildingOID[] = _T("BUILDING_OID");		//!< �����|���S��OID
	const _TCHAR kSrcOID[] = _T("SRC_OID");					//!< �Q�Ƒf��OID
	const _TCHAR kSurveyDate[] = _T("SURVEYDATE");			//!< ������
	const _TCHAR kAnyPlaceID[] = _T("ANYPLACE_ID");			//!< AnyPlaceID
	const _TCHAR kPriority[] = _T("PRIORITY_C");			//!< �M���R�[�h
	const _TCHAR kMatchPer[] = _T("MATCH_PER");				//!< �}�b�`���O��
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE");		//!< ���LPOI�R���e���c�R�[�h
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ");		//!< ���LPOI�R���e���c���V�[�P���X
}

// �����r�����̕R�t���e�[�u��
namespace rel_buildingname {
	const _TCHAR kTableName[] = _T("REL_BUILDINGNAME");			//!< �e�[�u����
	const _TCHAR kBuildingNameOID[] = _T("BUILDINGNAME_OID");	//!< �����r������OID
	const _TCHAR kSrcOID[] = _T("SRC_OID");						//!< �Q�Ƒf��OID
}

namespace railway_link {
	const _TCHAR kTableName[]     = _T("RAILWAY_LINK");			//!< �e�[�u����

	const _TCHAR kFromNodeOID[]   = _T("FROM_NODE_ID");			//!< �n�_�m�[�h��OID
	const _TCHAR kToNodeOID[]     = _T("TO_NODE_ID");			//!< �I�_�m�[�h��OID
	const _TCHAR kRailwayClass_C[] = _T("RAILWAYCLASS_C");		//!< �S�����
	const _TCHAR kDispScale_C[]    = _T("DISPSCALE_C");			//!< �X�P�[���\�����
	const _TCHAR kUnderGround_F[] = _T("UNDERGROUND_F");		//!< �n���t���O
}

namespace rel_railway_link {
	const _TCHAR kTableName[]      = _T("REL_RAILWAY_LINK");	//!< �e�[�u����

	const _TCHAR kRwLinkID[]       = _T("RW_LINK_ID");			//!< �S�������N��OID
	const _TCHAR kRailwayLineID[]  = _T("RAILWAY_LINE_ID");		//!< �S���H����OID
}

namespace railway_node {
	const _TCHAR kTableName[]      = _T("RAILWAY_NODE");		//!< �e�[�u����

	const _TCHAR kNodeClass_C[]	   = _T("NODECLASS_C");			//!< �m�[�h���
}

namespace rel_railway_node {
	const _TCHAR kTableName[]      = _T("REL_RAILWAY_NODE");	//!< �e�[�u����

	const _TCHAR kRwNodeID[]       = _T("RW_NODE_ID");			//!< �S���m�[�h��OID
	const _TCHAR kJoinLineInfoID[] = _T("JOIN_LINE_INFO_ID");	//!< ������H����OID
	const _TCHAR kStationNo[]      = _T("STATION_NUMBER");		//!< �w�ԍ�
}

namespace railway_color {
	const _TCHAR kTableName[]      = _T("RAILWAY_COLOR");		//!< �e�[�u����

	const _TCHAR kRailwayLineID[]  = _T("RAILWAY_LINE_ID");		//!< �S���H����OID
	const _TCHAR kColorName[]      = _T("COLOR_NAME");			//!< �F����
	const _TCHAR kColorNameEng[]   = _T("COLOR_NAME_ENG");		//!< �F����(�p��)
	const _TCHAR kColorCode[]      = _T("COLOR_CODE");			//!< �J���[�R�[�h
}

namespace station_detail {
	const _TCHAR kTableName[]      = _T("STATION_DETAIL");		//!< �e�[�u����

	const _TCHAR kPolygonType_C[]  = _T("POLYGONTYPE_C");		//!< �|���S�����
}

namespace midzoom_platform {
	const _TCHAR kTableName[]      = _T("MIDZOOM_PLATFORM");    //!< �e�[�u����
}

namespace highzoom_station {
	const _TCHAR kTableName[]      = _T("HIGHZOOM_STATION");    //!< �e�[�u����
}

namespace midzoom_station {
	const _TCHAR kTableName[]      = _T("MIDZOOM_STATION");     //!< �e�[�u����
}

namespace lowzoom_platform {
	const _TCHAR kTableName[]      = _T("LOWZOOM_PLATFORM");    //!< �e�[�u����
}

namespace rel_station_site {
	const _TCHAR kTableName[]      = _T("REL_STATION_SITE");    //!< �e�[�u����

	const _TCHAR kGroupID[]        = _T("GROUP_ID");		    //!< �wID
	const _TCHAR kLayerNo_C[]      = _T("LAYERNO_C");		    //!< ���C���[�ԍ�
	const _TCHAR kLayerOID[]       = _T("LAYER_OID");		    //!< ���C���[��OID
}

namespace groupid_master {
	const _TCHAR kTableName[]      = _T("GROUPID_MASTER");    //!< �e�[�u����

	const _TCHAR kStationName[]    = _T("STATION_NAME");       //!< �w��
}

/// ���H���̃e�[�u��
namespace road_name_list {
	const _TCHAR kTableName[] = _T("ROAD_NAME_LIST");

	const _TCHAR kNameClass[] = _T("NAMECLASS_C");		//!< ���H���̎��
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");		//!< ����
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");			//!< �ǂ�
}

} // sj

} // schema

} // sindy
