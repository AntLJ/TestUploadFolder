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
#include <sindy/schema/glshp.h>

namespace glshp
{
namespace schema
{
namespace objectid
{
	const _TCHAR kObjectId[] = _T("OBJECTID");			//!< OBJECTID
}
namespace base_table
{
	const _TCHAR kCountry_C[] = _T("COUNTRY_C");		//!< ���R�[�h
}
namespace base_feature
{
	const _TCHAR kShape[]     = _T("SHAPE");			//!< �`��t�B�[���h��
}
namespace scale_class
{
	const _TCHAR kSc1Class_C[] = _T("SC1CLASS_C");		//!< �X�P�[��1�w�i���
	const _TCHAR kSc2Class_C[] = _T("SC2CLASS_C");		//!< �X�P�[��2�w�i���
	const _TCHAR kSc3Class_C[] = _T("SC3CLASS_C");		//!< �X�P�[��3�w�i���
	const _TCHAR kSc4Class_C[] = _T("SC4CLASS_C");		//!< �X�P�[��4�w�i���
}
namespace disp_scale
{
	const _TCHAR kDispScale1_F[] = _T("SC1DISP_F");		//!< �X�P�[��1�\���t���O
	const _TCHAR kDispScale2_F[] = _T("SC2DISP_F");		//!< �X�P�[��2�\���t���O
	const _TCHAR kDispScale3_F[] = _T("SC3DISP_F");		//!< �X�P�[��3�\���t���O
	const _TCHAR kDispScale4_F[] = _T("SC4DISP_F");		//!< �X�P�[��4�\���t���O
}
namespace lang_type
{
	const _TCHAR kOnJpn[] = _T("ON_JPN");				//!< ���C������(���{��)
	const _TCHAR kAnJpn[] = _T("AN_JPN");				//!< �T�u����(���{��)
	const _TCHAR kOnEng[] = _T("ON_ENG");				//!< ���C������(�p��)
	const _TCHAR kAnEng[] = _T("AN_ENG");				//!< �T�u����(�p��)
	const _TCHAR kOnTha[] = _T("ON_THA");				//!< ���C������(�^�C��)
	const _TCHAR kAnTha[] = _T("AN_THA");				//!< �T�u����(�^�C��)
	const _TCHAR kOnInd[] = _T("ON_IND");				//!< ���C������(�C���h�l�V�A��)
	const _TCHAR kAnInd[] = _T("AN_IND");				//!< �T�u����(�C���h�l�V�A��)
	const _TCHAR kOnMay[] = _T("ON_MAY");				//!< ���C������(�}���[��)
	const _TCHAR kAnMay[] = _T("AN_MAY");				//!< �T�u����(�}���[��)
	const _TCHAR kOnVie[] = _T("ON_VIE");				//!< ���C������(�x�g�i����)
	const _TCHAR kAnVie[] = _T("AN_VIE");				//!< �T�u����(�x�g�i����)
}
namespace under_f
{
	const _TCHAR kUnder_F[] = _T("UNDER_F");			//!< �n���t���O
}
namespace infid
{
	const _TCHAR kInfId[] = _T("INFID");				//!< InfID
}
///// �n�}�n /////
	// LAND_USE
	namespace land_use
	{
		const _TCHAR kSuffix[] = _T("BLU");				//!< ���ʎq(�����h���[�X)
	}
	// LAND_COVER
	namespace land_cover
	{
		const _TCHAR kSuffix[] = _T("BLC");				//!< ���ʎq(�����h�J�o�[)
	}
	// WATERWAY
	namespace waterway
	{
		const _TCHAR kSuffix[] = _T("BWW");				//!< ���ʎq(���n)
	}
	// ISLAND
	namespace island
	{
		const _TCHAR kSuffix[] = _T("BIL");				//!< ���ʎq(��)
	}
	// �w�i���C��
	namespace base_line
	{
		const _TCHAR kSuffix[] = _T("BGL");				//!< ���ʎq(�w�i���C��)

		const _TCHAR kBgClass_C[] = _T("BGCLS_C");		//!< �w�i���C�����
	}
	// PROVINCE
	namespace province
	{
		const _TCHAR kSuffix[] = _T("APR");				//!< ���ʎq(�s���E(�B�E�n��))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< �s���E�R�[�h
	}
	// DISTRICT
	namespace district
	{
		const _TCHAR kSuffix[] = _T("ADR");				//!< ���ʎq(�s���E(�n��))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< �s���E�R�[�h
	}
	// SUB DISTRICT
	namespace sub_district
	{
		const _TCHAR kSuffix[] = _T("ASD");				//!< ���ʎq(�s���E(����))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< �s���E�R�[�h
	}
	// ZIP_CODE
	namespace postal_district
	{
		const _TCHAR kSuffix[] = _T("PSD");				//!< ���ʎq(�X�֔ԍ�)

		const _TCHAR kPostCode[] = _T("POST_CODE");		//!< �X�֔ԍ��R�[�h
	}
	// BASE_RAILWAY
	namespace base_railway
	{
		const _TCHAR kSuffix[] = _T("RAL");				//!< ���ʎq(�S���H��)

		const _TCHAR kRalCls_C[] = _T("RALCLS_C");		//!< �S�����C�����
	}
	// BASE_STATION
	namespace base_station
	{
		const _TCHAR kSuffix[] = _T("STA");				//!< ���ʎq(�w��)

		const _TCHAR kStaCls_C[] = _T("STACLS_C");		//!< �w�Ɏ��
	}
	// BUILDING
	namespace building
	{
		const _TCHAR kSuffix[] = _T("BLD");				//!< ���ʎq(����)

		const _TCHAR kBldCls_C[] = _T("BLDCLS_C");		//!< �������
		const _TCHAR kFloors[] = _T("FLOORS");			//!< �K�����
		const _TCHAR kCorridor_F[] = _T("CORRIDOR_F");	//!< �n��L���t���O
	}
	// CITY_STATION
	namespace city_station
	{
		const _TCHAR kSuffix[] = _T("USP");				//!< ���ʎq(�n���w��)

		const _TCHAR kStaCls_C[] = _T("STACLS_C");		//!< �n���w�Ɏ��
	}
///// ���H�n /////
namespace linkid
{
	const _TCHAR kLinkId[] = _T("LINKID");				//!< ���H�����NID
}
namespace link_queue
{
	const _TCHAR kSequence[] = _T("SEQUENCE");			//!< �V�[�P���X�ԍ�
}
namespace reg_condition
{
	const _TCHAR kFromMonth[] = _T("FROM_MONTH");		//!< �J�n��
	const _TCHAR kFromDay[] = _T("FROM_DAY");			//!< �J�n��
	const _TCHAR kToMonth[] = _T("TO_MONTH");			//!< �I����
	const _TCHAR kToDay[] = _T("TO_DAY");				//!< �I����
	const _TCHAR kFromHour[] = _T("FROM_HOUR");			//!< �J�n��
	const _TCHAR kFromMin[] = _T("FROM_MIN");			//!< �J�n��
	const _TCHAR kToHour[] = _T("TO_HOUR");				//!< �I����
	const _TCHAR kToMin[] = _T("TO_MIN");				//!< �I����
	const _TCHAR kDayOfWeek[] = _T("DAYOFWEEK");		//!< �j���w��
}

	// ���H�����N
	namespace road_link
	{
		const _TCHAR kSuffix[] = _T("RLK");				//!< ���ʎq(���H�����N)

		const _TCHAR kFromNodeId[] = _T("FROMNODEID");	//!< �n�_�m�[�hID
		const _TCHAR kToNodeId[] = _T("TONODEID");		//!< �I�_�m�[�hID
		const _TCHAR kRoadCls_C[] = _T("ROADCLS_C");	//!< ���H���
		const _TCHAR kNaviCls_C[] = _T("NAVICLS_C");	//!< �o�H���
		const _TCHAR kLinkCls_C[] = _T("LINKCLS_C");	//!< �����N���
		const _TCHAR kWidth_C[] = _T("WIDTH_C");		//!< �����敪
		const _TCHAR kIsland_F[] = _T("ISLAND_F");		//!< �����t���O
		const _TCHAR kRoadNo[] = _T("ROADNO");			//!< ���H�ԍ�
		const _TCHAR kLaneCount[] = _T("LANECOUNT");	//!< ���[����
		const _TCHAR kNoPassage_C[] = _T("NOPASS_C");	//!< �ʍs�֎~�R�[�h
		const _TCHAR kOneway_C[] = _T("ONEWAY_C");		//!< ����ʍs�R�[�h
		const _TCHAR kVclReg_RF[] = _T("VCLREG_RF");	//!< �Ԏ�ʋK���֘A�t���O
		const _TCHAR kLegalSpd_C[] = _T("LEGALSPD_C");	//!< �@�葬�x�R�[�h
	}
	// ���H�m�[�h
	namespace road_node
	{
		const _TCHAR kSuffix[] = _T("RND");				//!< ���ʎq(���H�m�[�h)

		const _TCHAR kObjectId2[]  = _T("OBJECTID_2");	//!< OBJECTID2
		const _TCHAR kCountry_2C[] = _T("COUNTRY_2C");	//!< ���R�[�h2
		const _TCHAR kNodeCls_C[]  = _T("NODECLS_C");	//!< �m�[�h���
	}
	// �����N������
	namespace segment_attr
	{
		const _TCHAR kSuffix[] = _T("RSA");				//!< ���ʎq(�����N������)

		const _TCHAR kLinkAttr_C[] = _T("LINKATTR_C");	//!< �����N���������
	}
	// �ʍs�֎~
	namespace nopassage_condition
	{
		const _TCHAR kSuffix[] = _T("NPC");				//!< ���ʎq(�ʍs�֎~)
	}
	// ����ʍs
	namespace oneway_condition
	{
		const _TCHAR kSuffix[] = _T("OWC");				//!< ���ʎq(����ʍs)

		const _TCHAR kLinkDir[] = _T("LINKDIR");		//!< �����N����
	}
	// �Ԏ�ʋK��
	namespace vehicle_reg
	{
		const _TCHAR kSuffix[] = _T("VRI");				//!< ���ʎq(�Ԏ�ʋK��)

		const _TCHAR kRegType_C[] = _T("REGTYPE_C");	//!< �Ԏ�ʋK�����
		const _TCHAR kHeight[] = _T("HEIGHT");			//!< ��������
		const _TCHAR kWidth[] = _T("WIDTH");			//!< �ő啝����
		const _TCHAR kWeight[] = _T("WEIGHT");			//!< �d�ʐ���
		const _TCHAR kLength[] = _T("LENGTH");			//!< ��������
		const _TCHAR kLoadage[] = _T("LOADAGE");		//!< �ύڏd�ʐ���
		const _TCHAR kDanger_F[] = _T("DANGER_F");		//!< �댯�ݕ��K��
	}
	// �Ԏ�ʋK���^�C�v
	namespace vehicle_type
	{
		const _TCHAR kSuffix[] = _T("VRT");				//!< ���ʎq(�Ԏ�ʋK���^�C�v)

		const _TCHAR kRegId[] = _T("REGID");			//!< �Ԏ�ʋK������ID
		const _TCHAR kExclude_F[] = _T("EXCLUDE_F");	//!< ���O�����t���O
		const _TCHAR kRegName[] = _T("REG_NAME");		//!< �K����������
		const _TCHAR kPsgr_F[] = _T("PSGR_F");			//!< ��p�ԃt���O
		const _TCHAR kTaxi_F[] = _T("TAXI_F");			//!< �^�N�V�[�t���O
		const _TCHAR kPbBus_F[] = _T("PBBUS_F");		//!< �����o�X�t���O
		const _TCHAR kPrBus_F[] = _T("PRBUS_F");		//!< �v���C�x�[�g�o�X�t���O
		const _TCHAR kTruck_F[] = _T("TRUCK_F");		//!< �A���g���b�N�t���O
	}
	// �����N��K��
	namespace turnreg_inf
	{
		const _TCHAR kSuffix[] = _T("TRI");				//!< ���ʎq(�����N��K�����)
	}
	namespace turnreg_lq
	{
		const _TCHAR kSuffix[] = _T("TRL");				//!< ���ʎq(�����N��)
	}
	// ���ʈē�
	namespace dirguide_inf
	{
		const _TCHAR kSuffix[] = _T("DRI");				//!< ���ʎq(���ʈē����)

		const _TCHAR kGuideCls_C[] = _T("GUIDECLASS");	//!< �ē����
		const _TCHAR kName1[] = _T("NAME_1");			//!< ���ʈē�����(��ꌾ��)
		const _TCHAR kName2[] = _T("NAME_2");			//!< ���ʈē�����(��񌾌�)
		const _TCHAR kNameSeq[] = _T("NAME_SEQ");		//!< ���̃V�[�P���X
		const _TCHAR kBoardPtn_C[] = _T("BOARDPTN_C");	//!< �ŔF
	}
	namespace dirguide_lq
	{
		const _TCHAR kSuffix[] = _T("DRL");				//!< ���ʎq(�����N��)
	}
	// ���[��
	namespace lane_inf
	{
		const _TCHAR kSuffix[] = _T("LNI");				//!< ���ʎq(���[�����)

		const _TCHAR kLaneDir[] = _T("LANEDIR");		//!< ���[������
		const _TCHAR kAppLane[] = _T("APPLANE");		//!< ���ߎԐ���
		const _TCHAR kRightChg[] = _T("RIGHTCHG");		//!< �E�Ԑ��ω���
		const _TCHAR kLeftChg[] = _T("LEFTCHG");		//!< ���Ԑ��ω���
		const _TCHAR kPassable[] = _T("PASSABLE");		//!< �ʍs�E�s���
	}
	namespace lane_lq
	{
		const _TCHAR kSuffix[] = _T("LNL");				//!< ���ʎq(�����N��)
	}
	// �����U��
	namespace guide_inf
	{
		const _TCHAR kSuffix[] = _T("GDI");				//!< ���ʎq(�����U�����)

		const _TCHAR kGuideType_C[] = _T("GUIDETYPE");	//!< �ē��R�[�h
	}
	namespace guide_lq
	{
		const _TCHAR kSuffix[] = _T("GDL");				//!< ���ʎq(�����N��)
	}
///// POI /////
	// POI
	namespace poi
	{
		const _TCHAR kSuffix[] = _T("POI");				//!< ���ʎq(POI)

		const _TCHAR kCat_C[]      = _T("CAT_C");		//!< �J�e�S���R�[�h
		const _TCHAR kSubCat_C[]   = _T("SUBCAT_C");	//!< �T�u�J�e�S���R�[�h
		const _TCHAR kChainMst_C[] = _T("CHAINMST_C");	//!< �`�F�[���}�X�^�[�R�[�h
		const _TCHAR kAdmin_C[]    = _T("ADMIN_C");		//!< �s���E�R�[�h
		const _TCHAR kHouseNum[]   = _T("HOUSENUM");	//!< �n�E�X�i���o�[
		const _TCHAR kActAddr[]    = _T("ACTADDR");		//!< Actual Address
		const _TCHAR kTelNum[]     = _T("TELNUM");		//!< ��\�d�b�ԍ�

		const _TCHAR kRName_Jpn[] = _T("RNAME_JPN");	//!< ���H����(���{��)
		const _TCHAR kRName_Eng[] = _T("RNAME_ENG");	//!< ���H����(�p��)
		const _TCHAR kRName_Tha[] = _T("RNAME_THA");	//!< ���H����(�^�C��)
		const _TCHAR kRName_Ind[] = _T("RNAME_IND");	//!< ���H����(�C���h�l�V�A��)
		const _TCHAR kRName_May[] = _T("RNAME_MAY");	//!< ���H����(�}���[��)
		const _TCHAR kRName_Vie[] = _T("RNAME_VIE");	//!< ���H����(�x�g�i����)
	}
	// HNP
	namespace hnp
	{
		const _TCHAR kSuffix[] = _T("HNP");				//!< ���ʎq(�Z��)

		const _TCHAR kType_C[] = _T("TYPE_C");			//!< �n�E�X�i���o�[��ʃR�[�h
		const _TCHAR kNumber[] = _T("NUMBER");			//!< �n�E�X�i���o�[
		const _TCHAR kAdmin_C[] = _T("ADMIN_C");		//!< �s���E�R�[�h
		const _TCHAR kLinkID[] = _T("LINKID");			//!< ���H�����NID
	}
}	// schema
}	// glshp
