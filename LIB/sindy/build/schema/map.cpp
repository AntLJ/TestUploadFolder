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
#include <sindy/schema/map.h>

namespace sindy {

namespace schema {

namespace scaledisp_flag {
	const TCHAR kSC1Disp[]				= _T("DISPSCALE1_F");			//!< �X�P�[��1�\���t���O
	const TCHAR kSC2Disp[]				= _T("DISPSCALE2_F");			//!< �X�P�[��2�\���t���O
	const TCHAR kSC3Disp[]				= _T("DISPSCALE3_F");			//!< �X�P�[��3�\���t���O
	const TCHAR kSC4Disp[]				= _T("DISPSCALE4_F");			//!< �X�P�[��4�\���t���O
} // scaledisp_flag

namespace scale_class {
	const TCHAR kSC1BgClass[]			= _T("SC1BGCLASS_C");			//!< �X�P�[��1�w�i��ʃR�[�h
	const TCHAR kSC2BgClass[]			= _T("SC2BGCLASS_C");			//!< �X�P�[��2�w�i��ʃR�[�h
	const TCHAR kSC3BgClass[]			= _T("SC3BGCLASS_C");			//!< �X�P�[��3�w�i��ʃR�[�h
	const TCHAR kSC4BgClass[]			= _T("SC4BGCLASS_C");			//!< �X�P�[��4�w�i��ʃR�[�h
} // scale_class

namespace claimpoly {
	const TCHAR kClaimType[]			= _T("CLAIMTYPE_C");			//!< �N���[���^�C�v�R�[�h
	const TCHAR kClaimNo[]				= _T("CLAIM_NO");				//!< �N���[���ԍ�
	const TCHAR kComFirm[]				= _T("COMFIRM_F");				//!< �펞�m�F�t���O
	const TCHAR kRemarks[]				= _T("USELESS_F");				//!< �R�����g�A���l
	const TCHAR kUseLess[]				= _T("REMARKS");				//!< �s�v�t���O
} // claimpoly

namespace building {
	const TCHAR kTableName[]			= _T("BUILDING");				//!< �e�[�u����
	const TCHAR kBldClass[]				= _T("BLDCLASS_C");				//!< ������ʃR�[�h
	const TCHAR kCorridor[]				= _T("CORRIDOR_F");				//!< �n��L���t���O
	const TCHAR kNoWall[]				= _T("NOWALL_F");				//!< ���ǎɃt���O
	const TCHAR kGeospaceID[]			= _T("GEOSPACE_ID");			//!< GEOSPACE_ID
}

namespace building_line {
	const TCHAR kTableName[]			= _T("BUILDING_LINE");			//!< �e�[�u����
	const TCHAR kBldClass[]				= _T("BLDCLASS_C");				//!< �������C����ʃR�[�h
}

namespace building_step {
	const TCHAR kTableName[]			= _T("BUILDING_STEP");			//!< �e�[�u����
	const TCHAR kFloors[]				= _T("FLOORS");					//!< �K��
	const TCHAR kCheck[]				= _T("CHECK_F");				//!< �K�����m�F�t���O
	const TCHAR kBuildingID[]           = _T("BUILDING_ID");            //!< ����ID
}

namespace city_site {
	const TCHAR kTableName[]			= _T("CITY_SITE");				//!< �e�[�u����
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i��ʃR�[�h
}

namespace city_line {
	const TCHAR kTableName[]			= _T("CITY_LINE");				//!< �e�[�u����
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i���C����ʃR�[�h
}

namespace city_station {
	const TCHAR kTableName[]			= _T("CITY_STATION");			//!< �e�[�u����
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< �S���w��ʃR�[�h
}

namespace city_railway {
	const TCHAR kTableName[]			= _T("CITY_RAILWAY");			//!< �e�[�u����
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kRailwayClass[]			= _T("RAILWAYCLASS_C");			//!< �S����ʃR�[�h
}

namespace underground_site {
	const TCHAR kTableName[]			= _T("UNDERGROUND_SITE");		//!< �e�[�u����
	const TCHAR kUgClass[]				= _T("UGCLASS_C");				//!< �n���w�i���
}

namespace underground_line {
	const TCHAR kTableName[]			= _T("UNDERGROUND_LINE");		//!< �e�[�u����
	const TCHAR kUgClass[]				= _T("UGCLASS_C");				//!< �n���w�i���C�����
}

namespace city_admin {
	const TCHAR kTableName[]			= _T("CITY_ADMIN");				//!< �e�[�u����
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< �s�撬���R�[�h
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �Z���R�[�h
	const TCHAR kAddrCode2[]			= _T("ADDRCODE2");				//!< �Z���R�[�h2
	const TCHAR kTmpCode[]				= _T("TMPCODE_F");				//!< ���Z���R�[�h�t���O
	const TCHAR kGaikuFugo[]			= _T("GAIKUFUGO");				//!< �X�敄��
	const TCHAR kExtGaikuFugo[]			= _T("EXTGAIKUFUGO");			//!< �g���X�敄��
	const TCHAR kAddrClass[]			= _T("ADDRCLASS_C");			//!< �Z���������
	const TCHAR kAreaClass[]			= _T("AREACLASS_C");			//!< �G���A���ʎ��
	const TCHAR kManage[]				= _T("MANAGE_F");				//!< �Ǘ��t���O
	const TCHAR kDetach[]				= _T("DETACH_F");				//!< ��n�t���O
	const TCHAR kColorCode[]			= _T("COLORCODE");				//!< �X��F�t�^�p�F����
	const TCHAR kWorkColor[]			= _T("WORKCOLOR");				//!< �X��F�t�^��Ɨp
	const TCHAR kConvType[]				= _T("CONVTYPE_C");				//!< �Z���ϊ����[�h
}

namespace city_rep_point {
	const TCHAR kTableName[]			= _T("CITY_REP_POINT");			//!< �e�[�u����
	const TCHAR kChibanNo[]				= _T("CHIBAN_NO");				//!< �n��
	const TCHAR kAnno[]					= _T("ANNO_F");					//!< �n�Ԓ��L�t���O
	const TCHAR kPointClass[]			= _T("POINTCLASS_C");			//!< �|�C���g���
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �Z���P�P���R�[�h
	const TCHAR kRepType[]				= _T("REPTYPE_C");				//!< 
}

namespace cityplanmap {
	const TCHAR kTableName[]			= _T("CITYPLANMAP");			//!< �e�[�u����
	const TCHAR kMapID[]				= _T("MAPID");					//!< �}�tID
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< �s�����R�[�h
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< �s����
	const TCHAR kMapName[]				= _T("MAPNAME");				//!< �}�t��
	const TCHAR kMapNo[]				= _T("MAPNO");					//!< �}�tNO
	const TCHAR kPubDate[]				= _T("PUB_DATE");				//!< �쐬�N�x
}

namespace base_site {
	const TCHAR kTableName[]			= _T("BASE_SITE");				//!< �e�[�u����
}

namespace base_line {
	const TCHAR kTableName[]			= _T("BASE_LINE");				//!< �e�[�u����

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i���C����ʃR�[�h
}

namespace base_station {
	const TCHAR kTableName[]			= _T("BASE_STATION");			//!< �e�[�u����

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< �S���w��ʃR�[�h
}

namespace base_railway {
	const TCHAR kTableName[]			= _T("BASE_RAILWAY");			//!< �e�[�u����
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kRailwayClass[]			= _T("RAILWAYCLASS_C");			//!< �S����ʃR�[�h
}

namespace base_land {
	const TCHAR kTableName[]			= _T("BASE_LAND");				//!< �e�[�u����
}

namespace base_admin {
	const TCHAR kTableName[]			= _T("BASE_ADMIN");				//!< �e�[�u����
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< �s�撬���R�[�h
}

namespace base_rep_point {
	const TCHAR kTableName[]			= _T("BASE_REP_POINT");			//!< �e�[�u����
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �Z���P�P���R�[�h
	const TCHAR kAdminName[]			= _T("ADMINNAME");				//!< ��\�_����
	const TCHAR kZipCode[]				= _T("ZIPCODE");				//!< �X�֔ԍ�
}

namespace base_contour {
	const TCHAR kTableName[]			= _T("BASE_CONTOUR");			//!< �e�[�u����
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< �i�ʎ�ʃR�[�h
}

namespace middle_site {
	const TCHAR kTableName[]			= _T("MIDDLE_SITE");			//!< �e�[�u����
}

namespace middle_line {
	const TCHAR kTableName[]			= _T("MIDDLE_LINE");			//!< �e�[�u����
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< �n���t���O
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i���C����ʃR�[�h
}

namespace middle_contour {
	const TCHAR kTableName[]			= _T("MIDDLE_CONTOUR");			//!< �e�[�u����
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< �i�ʎ�ʃR�[�h
}

namespace top_site {
	const TCHAR kTableName[]			= _T("TOP_SITE");				//!< �e�[�u����
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �g�b�v�w�i��ʃR�[�h
}

namespace top_line {
	const TCHAR kTableName[]			= _T("TOP_LINE");				//!< �e�[�u����
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i���C����ʃR�[�h
}

namespace top_contour {
	const TCHAR kTableName[]			= _T("TOP_CONTOUR");			//!< �e�[�u����
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< �i�ʎ�ʃR�[�h
}

namespace ortho_area {
	const TCHAR kTableName[]			= _T("ORTHO_AREA");				//!< �e�[�u����
	const TCHAR kTileName[]				= _T("TILE_NAME");				//!< 
}

namespace adminarea {
	const TCHAR kTableName[]			= _T("ADMINAREA");				//!< �e�[�u����
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< �s�撬���R�[�h
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< �s�撬����
}

namespace aerial_photo {
	const TCHAR kTableName[]			= _T("AERIAL_PHOTO");			//!< �e�[�u����
	const TCHAR kMeshCode[]				= _T("MESHCODE");				//!< �s�s�n�}���b�V���R�[�h
	const TCHAR kBinderName[]			= _T("BINDERNAME");				//!< �o�C���_�[��
	const TCHAR kCourse[]				= _T("COURSE");					//!< �B�e�R�[�X
	const TCHAR kPhotoNo[]				= _T("PHOTONO");				//!< �ʐ^�ԍ�
	const TCHAR kShootYear[]			= _T("SHOOTYEAR");				//!< �ʐ^�B�e�N
	const TCHAR kShootDate[]			= _T("SHOOTDATE");				//!< �ʐ^�B�e�N����
	const TCHAR kOwnerCode[]			= _T("OWNERCODE");				//!< ���ݒn
}

namespace daikei_point {
	const TCHAR kTableName[]			= _T("DAIKEI_POINT");			//!< �e�[�u����
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �Z���R�[�h
	const TCHAR kAddrStr[]				= _T("ADDRSTR");				//!< �Z��������
	const TCHAR kSrcType[]				= _T("SRCTYPE");				//!< �f�[�^�\�[�X�^�C�v
	const TCHAR kDupType[]				= _T("DUPTYPE");				//!< �d���^�C�v
}

namespace parea {
	const TCHAR kTableName[]			= _T("PAREA");					//!< �e�[�u����
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< PAREA�Z���R�[�h
	const TCHAR kRepPointCode[]			= _T("REPPOINTCODE");			//!< ��\�_�R�[�h
	const TCHAR kUpdateCode[]			= _T("UPDATECODE");				//!< �X�V�R�[�h
	const TCHAR kTmpCode[]				= _T("TMPCODE_F");				//!< ���R�[�h�t���O
}

namespace gou_point {
	const TCHAR kTableName[]			= _T("GOU_POINT");				//!< �e�[�u����
	const TCHAR kGouNo[]				= _T("GOU_NO");					//!< ���ԍ�
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 20���Z���R�[�h
	const TCHAR kBuildRF[]				= _T("BUILD_RF");				//!< �����֘A�t���O
	const TCHAR kBuildID[]				= _T("BUILD_ID");				//!< �Ή�����ID
	const TCHAR kExpGouNo[]				= _T("EXPGOUNO_F");				//!< �g���ԍ��t���O
	const TCHAR kTmpCheck[]				= _T("TMP_CHECK_F");			//!< �m�F�t���O
	const TCHAR kTmpFloors[]			= _T("TMP_FLOORS");				//!< �K�����
	const TCHAR kGouType[]				= _T("GOUTYPE");				//!< ���^�C�v
	const TCHAR kPriority[]				= _T("PRIORITY_F");				//!< �D��t���O
	const TCHAR kPrecisionCode[]		= _T("PRECISION_CODE");			//!< �Z����v����
	const TCHAR kConfirm[]				= _T("CONFIRM_C");				//!< �Z���m�F�X�e�[�^�X�R�[�h
	const TCHAR kHimawari[]				= _T("HIMAWARI_F");				//!< �Ђ܂��t���O
}

namespace klink_point {
	const TCHAR kTableName[]			= _T("KLINK_POINT");			//!< �e�[�u����
	const TCHAR kAddrCodeOrg[]			= _T("ADDRCODE_ORG");			//!< ����22���R�[�h
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �ŐV22���R�[�h
	const TCHAR kKatagakiName[]			= _T("KATAGAKINAME");			//!< �^�E���y�[�W��������
	const TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< �^�E���y�[�W�f�ږ���
	const TCHAR kInfoSrc[]				= _T("INFOSRC_C");				//!< ���񋟌��R�[�h
	const TCHAR kInputSeido[]			= _T("INPUTSEIDO_C");			//!< ���W���x��ʃR�[�h
	const TCHAR kReleaseSeido[]			= _T("RELEASESEIDO_C");			//!< ���W���x��ʃR�[�h
	const TCHAR kBuildName[]			= _T("BUILDNAME");				//!< �r������
	const TCHAR kBuildInfoSrc[]			= _T("BUILDINFOSRC_C");			//!< �r�����񋟌��R�[�h
	const TCHAR kMaxFloors[]			= _T("MAX_FLOORS");				//!< �ŏ�K��
	const TCHAR kFloor[]				= _T("FLOOR");					//!< �K��
	const TCHAR kNequal[]				= _T("NEQUAL_F");				//!< 11���s��v�t���O
}

namespace claimpoly_addr {
	const TCHAR kTableName[]			= _T("CLAIMPOLY_ADDR");			//!< �e�[�u����
}

namespace claimpoly_map {
	const TCHAR kTableName[]			= _T("CLAIMPOLY_MAP");			//!< �e�[�u����
}

namespace cs_addr_point {
	const TCHAR kTableName[]			= _T("CS_ADDR_POINT");			//!< �e�[�u����
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< �Q�O���Z���R�[�h
}

namespace city_admin_c {
	const TCHAR kTableName[]			= _T("CITY_ADMIN_C");			//!< �e�[�u����
	const TCHAR kCityCode[]				= _T("ADDRCODE");				//!< �s�撬���R�[�h
}

namespace city_admin_pref {
	const TCHAR kTableName[]			= _T("CITY_ADMIN_PREF");		//!< �e�[�u����
	const TCHAR kCityCode[]				= _T("ADDRCODE");				//!< �s�撬���R�[�h
}

namespace extp_point {
	const _TCHAR kTableName[]			= _T("EXTP_POINT");				//!< �e�[�u����

	const _TCHAR kAddrCode[]			= _T("ADDRCODE");				//!< 20���Z���R�[�h
	const _TCHAR kAddrName[]			= _T("ADDRNAME");				//!< �Z����
	const _TCHAR kKatagakiName[]		= _T("KATAGAKINAME");			//!< �^�E���y�[�W��������
	const _TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< �^�E���y�[�W�f�ږ���
	const _TCHAR kAddPoint[]			= _T("ADDPOINT_F");				//!< �Z�������|�C���g���t���O
	const _TCHAR kInfoSrc[]				= _T("INFOSRC_C");				//!< �������W�񋟌��R�[�h
	const _TCHAR kPointStat[]			= _T("POINTSTAT_C");			//!< �������ʃR�[�h
}

namespace teltp_point {
	const _TCHAR kTableName[]			= _T("TELTP_POINT");			//!< �e�[�u����

	const _TCHAR kTelNum[]				= _T("TEL_NUM");				//!< �d�b�ԍ�
	const _TCHAR kKatagakiName[]		= _T("KATAGAKINAME");			//!< �^�E���y�[�W��������
	const _TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< �^�E���y�[�W�f�ږ���
	const _TCHAR kAddrCode[]			= _T("ADDRCODE");				//!< 11���Z���R�[�h
	const _TCHAR kAddrName[]			= _T("ADDRNAME");				//!< �Z����
	const _TCHAR kPointStat[]			= _T("POINTSTAT_C");			//!< �������ʃR�[�h
	const _TCHAR kSequence[]			= _T("SEQUENCE");				//!< �V�[�P���X�ԍ�
	const _TCHAR kAddXY[]				= _T("ADDXY_C");				//!< ���W�t�^���
}

// �s�s��������{��
namespace building_roof {
	const _TCHAR kTableName[]			= _T("BUILDING_ROOF");            //!< �e�[�u����

	const _TCHAR kRfClass[]				= _T("RFCLASS_C");				 //!< ����{�ݎ��
}
// �s�s�G���A
namespace city_area {
	const _TCHAR kTableName[]			= _T("CITY_AREA");				//!< �e�[�u����

	const _TCHAR kAreaClass[]			= _T("AREACLASS_C");			//!< �G���A���
}

// �s�s�|���S���������C��
namespace city_sprit_line {
	const _TCHAR kTableName[]			= _T("CITY_SPLIT_LINE");		//!< �e�[�u����

	const _TCHAR kSpritClass[]			= _T("SPLITCLASS_C");			//!< �������C�����
}

// �s�s�g���l���B��
namespace city_tunnel {
	const _TCHAR kTableName[]			= _T("CITY_TUNNEL");			//!< �e�[�u����

	const _TCHAR kTunnelClass[]			= _T("TUNNELCLASS_C");			//!< �g���l�����
}

// �s�s���H�|���S��
namespace city_road {
	const _TCHAR kTableName[]			= _T("CITY_ROAD");				//!< �e�[�u����

	const _TCHAR kRoadClass[]			= _T("ROADCLASS_C");			//!< ���H�|���S�����
}

// �s�s�����������у|���S��
namespace city_divider {
	const _TCHAR kTableName[]           = _T("CITY_DIVIDER");           //!< �e�[�u����

	const _TCHAR kDividerClass[]        = _T("DIVIDERCLASS_C");         //!< �����������ю��
}

// ���ˉ��w�i���C��
namespace city_line_under {
	const _TCHAR kTableName[]			= _T("CITY_LINE_UNDER");		//!< �e�[�u����

	const _TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i���C�����
}

// ���ˉ��w�i�|���S��
namespace city_site_under {
	const _TCHAR kTableName[]			= _T("CITY_SITE_UNDER");		//!< �e�[�u����

	const _TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< �w�i�|���S�����
}

// �n���ʘH�E�n���X�|���S��
namespace underground_area
{
	const _TCHAR kTableName[] = _T("UNDERGROUND_AREA"); //!< �e�[�u����

} // underground_area

// �X�֔ԍ��|���S��
namespace zipcode_area
{
	const _TCHAR kTableName[] = _T("ZIPCODE_AREA"); //!< �e�[�u����
	const _TCHAR kZipCode1[] = _T("ZIPCODE1"); //!< �e�[�u����
	const _TCHAR kZipCode2[] = _T("ZIPCODE2"); //!< �e�[�u����
}

// �X�֔ԍ��|���S���Z���R�t���e�[�u��
namespace rel_addr
{
	const _TCHAR kTableName20[] = _T("REL_ADDR20");
	const _TCHAR kTableName23[] = _T("REL_ADDR23");
	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kModifyStatus[] = _T("MODIFYSTATUS_C");
}


} // schema

} // sindy
