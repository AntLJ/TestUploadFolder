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
#include <sindy/schema/cds.h>

namespace sindy {

namespace schema {

namespace city_annotation_gs {
	const _TCHAR kTableName[]		= _T("CITY_ANNOTATION_GS");		//!< �e�[�u����

	const _TCHAR kNameString1[]		= _T("NAME_STRING1");			//!< ���L������1
	const _TCHAR kNameString2[]		= _T("NAME_STRING2");			//!< ���L������2
	const _TCHAR kStringNum1[]		= _T("STRING_NUM1");			//!< ���L1������
	const _TCHAR kStringNum2[]		= _T("STRING_NUM2");			//!< ���L2������
	const _TCHAR kAnnoClass[]		= _T("ANNOCLASS_C");			//!< ���L���
	const _TCHAR kNameYomi[]		= _T("NAME_YOMI");				//!< ���L�ǂ�
	const _TCHAR kEngDefault[]		= _T("ENG_DEFAULT");			//!< �f�t�H���g�p�꒍�L
	const _TCHAR kEngName[]			= _T("ENG_NAME");				//!< �ʉp�꒍�L
	const _TCHAR kEngOut[]			= _T("ENGOUT_F");				//!< �p�꒍�L�o�̓t���O
	const _TCHAR kDispType[]		= _T("DISPTYPE_C");				//!< �\���^�C�v
	const _TCHAR kHimawari[]		= _T("HIMAWARI_F");				//!< �Ђ܂��t���O
	const _TCHAR kContentsCode[]	= _T("CONTENTS_CODE");			//!< �R���e���c�R�[�h
	const _TCHAR kContentsSeq[]		= _T("CONTENTS_SEQ");			//!< �R���e���c�V�[�P���X
	const _TCHAR kAnnoDisp[]		= _T("ANNO_DISP_C");			//!< ���L�\���R�[�h
} // city_annotation_gs

namespace building_gs {
	const _TCHAR kTableName[]		= _T("BUILDING_GS");		//!< �e�[�u����

	const _TCHAR kBldClass[]		= _T("BLDCLASS_C");			//!< �������
	const _TCHAR kCorridor[]		= _T("CORRIDOR_F");			//!< �n��L���t���O
	const _TCHAR kNoWall[]			= _T("NOWALL_F");			//!< ���ǃt���O
	const _TCHAR kBldName[]			= _T("BLDNAME");			//!< ��������
	const _TCHAR kBldAddr[]			= _T("BLDADDR");			//!< �����Z��
	const _TCHAR kGeospaceID[]		= _T("GEOSPACE_ID");		//!< GEOSPACE�ƌ`ID
} // building_gs

namespace city_line_gs {
	const _TCHAR kTableName[]		= _T("CITY_LINE_GS");		//!< �e�[�u����

	const _TCHAR kBgClass[]			= _T("BGCLASS_C");			//!< �w�i���C�����
} // city_line_gs

namespace city_railway_gs {
	const _TCHAR kTableName[]		= _T("CITY_RAILWAY_GS");	//!< �e�[�u����

	const _TCHAR kRailwayClass[]	= _T("RAILWAYCLASS_C");		//!< �S�����
	const _TCHAR kUnderground[]		= _T("UNDERGROUND_F");		//!< �n���t���O
} // city_railway_gs

namespace city_site_gs {
	const _TCHAR kTableName[]		= _T("CITY_SITE_GS");		//!< �e�[�u����

	const _TCHAR kBgClass[]			= _T("BGCLASS_C");			//!< �w�i���
} // city_site_gs

namespace land_cover_gs {
	const _TCHAR kTableName[]		= _T("LAND_COVER_GS");		//!< �e�[�u����
} // land_cover_gs

namespace underground_area_gs {
	const _TCHAR kTableName[]		= _T("UNDERGROUND_AREA_GS");	//!< �e�[�u����
} // underground_area_gs

namespace bld_tenant_gs {
	const _TCHAR kTableName[]		= _T("BLD_TENANT_GS");		//!< �e�[�u����

	const _TCHAR kGeospaceID[]		= _T("GEOSPACE_ID");		//!< �Ɖ��`��n��ID
	const _TCHAR kLClass[]			= _T("L_CLASS");			//!< �啪�ރR�[�h
	const _TCHAR kMClass[]			= _T("M_CLASS");			//!< �����ރR�[�h
	const _TCHAR kSClass[]			= _T("S_CLASS");			//!< �����ރR�[�h
	const _TCHAR kPopularName[]		= _T("POPULAR_NAME");		//!< �ʏ̖���
	const _TCHAR kSignageName[]		= _T("SIGNAGE_NAME");		//!< �\�L����
	const _TCHAR kNameKana[]		= _T("NAME_KANA");			//!< �J�i����
	const _TCHAR kIconClass[]		= _T("ICON_CLASS");			//!< �A�C�R�����
	const _TCHAR kReserved[]		= _T("RESERVED");			//!< reserved
	const _TCHAR kAddrCode11[]		= _T("ADDR_CODE11");		//!< �Z���R�[�h11��
	const _TCHAR kPrefKanji[]		= _T("PREF_KANJI");			//!< ��������
	const _TCHAR kCityKanji[]		= _T("CITY_KANJI");			//!< �����s������
	const _TCHAR kOazaKanji[]		= _T("OAZA_KANJI");			//!< �����厚��
	const _TCHAR kAzaKanji[]		= _T("AZA_KANJI");			//!< �������E����
	const _TCHAR kBlockKanji[]		= _T("BLOCK_KANJI");		//!< �Ԓn
	const _TCHAR kGoKanji[]			= _T("GO_KANJI");			//!< ��
	const _TCHAR kBranchNum[]		= _T("BRANCH_NUM");			//!< �}��
	const _TCHAR kPrefKana[]		= _T("PREF_KANA");			//!< ���p�J�i����
	const _TCHAR kCityKana[]		= _T("CITY_KANA");			//!< ���p�J�i�s������
	const _TCHAR kOazaKana[]		= _T("OAZA_KANA");			//!< ���p�J�i�厚��
	const _TCHAR kAzaKana[]			= _T("AZA_KANA");			//!< ���p�J�i���E����
	const _TCHAR kPostNum[]			= _T("POST_NUM");			//!< �X�֔ԍ�
} // bld_tenant_gs

namespace city_station_gs {
	const TCHAR kTableName[]			= _T("CITY_STATION_GS");			//!< �e�[�u����

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< �S���w��ʃR�[�h
} // city_station_gs

} // schema

} // sindy
