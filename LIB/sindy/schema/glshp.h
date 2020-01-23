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

#ifndef	_GLSHP_H_
#define	_GLSHP_H_

// �O���[�o��SHP�@�X�L�[�}��`

namespace glshp
{
namespace schema
{
namespace objectid
{
	extern const _TCHAR kObjectId[];			//!< OBJECTID
}
namespace base_table
{
	using namespace glshp::schema::objectid;
	extern const _TCHAR kCountry_C[];			//!< ���R�[�h
}
namespace base_feature
{
	using namespace glshp::schema::base_table;
	extern const _TCHAR kShape[];				//!< �`��t�B�[���h��
}
namespace scale_class
{
	extern const _TCHAR kSc1Class_C[];			//!< �X�P�[��1�w�i���
	extern const _TCHAR kSc2Class_C[];			//!< �X�P�[��2�w�i���
	extern const _TCHAR kSc3Class_C[];			//!< �X�P�[��3�w�i���
	extern const _TCHAR kSc4Class_C[];			//!< �X�P�[��4�w�i���
}
namespace disp_scale
{
	extern const _TCHAR kDispScale1_F[];		//!< �X�P�[��1�\���t���O
	extern const _TCHAR kDispScale2_F[];		//!< �X�P�[��2�\���t���O
	extern const _TCHAR kDispScale3_F[];		//!< �X�P�[��3�\���t���O
	extern const _TCHAR kDispScale4_F[];		//!< �X�P�[��4�\���t���O
}
namespace lang_type
{
	extern const _TCHAR kOnJpn[];				//!< ���C������(���{��)
	extern const _TCHAR kAnJpn[];				//!< �T�u����(���{��)
	extern const _TCHAR kOnEng[];				//!< ���C������(�p��)
	extern const _TCHAR kAnEng[];				//!< �T�u����(�p��)
	extern const _TCHAR kOnTha[];				//!< ���C������(�^�C��)
	extern const _TCHAR kAnTha[];				//!< �T�u����(�^�C��)
	extern const _TCHAR kOnInd[];				//!< ���C������(�C���h�l�V�A��)
	extern const _TCHAR kAnInd[];				//!< �T�u����(�C���h�l�V�A��)
	extern const _TCHAR kOnMay[];				//!< ���C������(�}���[��)
	extern const _TCHAR kAnMay[];				//!< �T�u����(�}���[��)
	extern const _TCHAR kOnVie[];				//!< ���C������(�x�g�i����)
	extern const _TCHAR kAnVie[];				//!< �T�u����(�x�g�i����)
}
namespace under_f
{
	extern const _TCHAR kUnder_F[];				//!< �n���t���O
}

///// �n�}�n /////
	// LAND_USE
	namespace land_use
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����h���[�X)
	}
	// LAND_COVER
	namespace land_cover
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����h�J�o�[)
	}
	// WATERWAY
	namespace waterway
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(���n)
	}
	// ISLAND
	namespace island
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(��)
	}
	// �w�i���C��
	namespace base_line
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�w�i���C��)

		extern const _TCHAR kBgClass_C[];		//!< �w�i���C�����
	}
	// PROVINCE
	namespace province
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�s���E(�B�E�n��))

		extern const _TCHAR kAdminCode[];		//!< �s���E�R�[�h
	}
	// DISTRICT
	namespace district
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�s���E(�n��))

		extern const _TCHAR kAdminCode[];		//!< �s���E�R�[�h
	}
	// SUB DISTRICT
	namespace sub_district
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�s���E(����))

		extern const _TCHAR kAdminCode[];		//!< �s���E�R�[�h
	}
	// POSTAL DISTRICT
	namespace postal_district
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�X�֔ԍ�)

		extern const _TCHAR kPostCode[];		//!< �X�֔ԍ��R�[�h
	}
	// BASE_RAILWAY
	namespace base_railway
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�S���H��)

		extern const _TCHAR kRalCls_C[];		//!< �S�����C�����
	}
	// BASE_STATION
	namespace base_station
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�w��)

		extern const _TCHAR kStaCls_C[];		//!< �w�Ɏ��
	}
	// BUILDING
	namespace building
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(����)

		extern const _TCHAR kBldCls_C[];		//!< �������
		extern const _TCHAR kFloors[];			//!< �K�����
		extern const _TCHAR kCorridor_F[];		//!< �n��L���t���O
	}
	// CITY_STATION
	namespace city_station
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�n���w��)

		extern const _TCHAR kStaCls_C[];		//!< �n���w�Ɏ��
	}
///// ���H�n /////
namespace linkid
{
	extern const _TCHAR kLinkId[];				//!< ���H�����NID
}
namespace infid
{
	extern const _TCHAR kInfId[];				//!< InfID
}
namespace link_queue
{
	using namespace glshp::schema::base_table;
	using namespace glshp::schema::linkid;
	using namespace glshp::schema::infid;

	extern const _TCHAR kSequence[];		//!< �V�[�P���X�ԍ�
}
namespace inf_table
{
	using namespace glshp::schema::base_table;
	using namespace glshp::schema::infid;
}
namespace reg_condition
{
	using namespace glshp::schema::base_table;

	extern const _TCHAR kFromMonth[];			//!< �J�n��
	extern const _TCHAR kFromDay[];				//!< �J�n��
	extern const _TCHAR kToMonth[];				//!< �I����
	extern const _TCHAR kToDay[];				//!< �I����
	extern const _TCHAR kFromHour[];			//!< �J�n��
	extern const _TCHAR kFromMin[];				//!< �J�n��
	extern const _TCHAR kToHour[];				//!< �I����
	extern const _TCHAR kToMin[];				//!< �I����
	extern const _TCHAR kDayOfWeek[];			//!< �j���w��
}

	// ���H�����N
	namespace road_link
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(���H�����N)

		extern const _TCHAR kFromNodeId[];		//!< �n�_�m�[�hID
		extern const _TCHAR kToNodeId[];		//!< �I�_�m�[�hID
		extern const _TCHAR kRoadCls_C[];		//!< ���H���
		extern const _TCHAR kNaviCls_C[];		//!< �o�H���
		extern const _TCHAR kLinkCls_C[];		//!< �����N���
		extern const _TCHAR kWidth_C[];			//!< �����敪
		extern const _TCHAR kIsland_F[];		//!< �����t���O
		extern const _TCHAR kRoadNo[];			//!< ���H�ԍ�
		extern const _TCHAR kLaneCount[];		//!< ���[����
		extern const _TCHAR kNoPassage_C[];		//!< �ʍs�֎~�R�[�h
		extern const _TCHAR kOneway_C[];		//!< ����ʍs�R�[�h
		extern const _TCHAR kVclReg_RF[];		//!< �Ԏ�ʋK���֘A�t���O
		extern const _TCHAR kLegalSpd_C[];		//!< �@�葬�x�R�[�h
	}
	// ���H�m�[�h
	namespace road_node
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(���H�m�[�h)

		extern const _TCHAR kObjectId2[];		//!< OBJECTID2
		extern const _TCHAR kCountry_2C[];		//!< ���R�[�h2
		extern const _TCHAR kNodeCls_C[];		//!< �m�[�h���
	}
	// �����N������
	namespace segment_attr
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::linkid;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N������)

		extern const _TCHAR kLinkAttr_C[];		//!< �����N���������
	}
	// �ʍs�֎~
	namespace nopassage_condition
	{
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�ʍs�֎~)
	}
	// ����ʍs
	namespace oneway_condition
	{
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(����ʍs)

		extern const _TCHAR kLinkDir[];			//!< �����N����
	}
	// �Ԏ�ʋK��
	namespace vehicle_reg
	{
		using namespace glshp::schema::base_table;
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�Ԏ�ʋK��)

		extern const _TCHAR kRegType_C[];		//!< �Ԏ�ʋK�����
		extern const _TCHAR kHeight[];			//!< ��������
		extern const _TCHAR kWidth[];			//!< �ő啝����
		extern const _TCHAR kWeight[];			//!< �d�ʐ���
		extern const _TCHAR kLength[];			//!< ��������
		extern const _TCHAR kLoadage[];			//!< �ύڏd�ʐ���
		extern const _TCHAR kDanger_F[];		//!< �댯�ݕ��K��
	}
	// �Ԏ�ʋK���^�C�v
	namespace vehicle_type
	{
		using namespace glshp::schema::base_table;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�Ԏ�ʋK���^�C�v)

		extern const _TCHAR kRegId[];			//!< �Ԏ�ʋK������ID
		extern const _TCHAR kExclude_F[];		//!< ���O�����t���O
		extern const _TCHAR kRegName[];			//!< �K����������
		extern const _TCHAR kPsgr_F[];			//!< ��p�ԃt���O
		extern const _TCHAR kTaxi_F[];			//!< �^�N�V�[�t���O
		extern const _TCHAR kPbBus_F[];			//!< �����o�X�t���O
		extern const _TCHAR kPrBus_F[];			//!< �v���C�x�[�g�o�X�t���O
		extern const _TCHAR kTruck_F[];			//!< �A���g���b�N�t���O
	}
	// �����N��K��
	namespace turnreg_inf
	{
		using namespace reg_condition;
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N��K�����)
	}
	namespace turnreg_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N��)
	}
	// ���ʈē�
	namespace dirguide_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(���ʈē����)

		extern const _TCHAR kGuideCls_C[];		//!< �ē����
		extern const _TCHAR kName1[];			//!< ���ʈē�����(��ꌾ��)
		extern const _TCHAR kName2[];			//!< ���ʈē�����(��񌾌�)
		extern const _TCHAR kNameSeq[];			//!< ���̃V�[�P���X
		extern const _TCHAR kBoardPtn_C[];		//!< �ŔF
	}
	namespace dirguide_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N��)
	}
	// ���[��
	namespace lane_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(���[�����)

		extern const _TCHAR kLaneDir[];			//!< ���[������
		extern const _TCHAR kAppLane[];			//!< ���ߎԐ���
		extern const _TCHAR kRightChg[];		//!< �E�Ԑ��ω���
		extern const _TCHAR kLeftChg[];			//!< ���Ԑ��ω���
		extern const _TCHAR kPassable[];		//!< �ʍs�E�s���
	}
	namespace lane_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N��)
	}
	// �����U��
	namespace guide_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����U�����)

		extern const _TCHAR kGuideType_C[];		//!< �ē��R�[�h
	}
	namespace guide_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(�����N��)
	}
///// POI /////
	// POI
	namespace poi
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(POI)

		extern const _TCHAR kCat_C[];			//!< �J�e�S���R�[�h
		extern const _TCHAR kSubCat_C[];		//!< �T�u�J�e�S���R�[�h
		extern const _TCHAR kChainMst_C[];		//!< �`�F�[���R�[�h
		extern const _TCHAR kAdmin_C[];			//!< �s���E�R�[�h
		extern const _TCHAR kHouseNum[];		//!< �n�E�X�i���o�[
		extern const _TCHAR kActAddr[];			//!< Actual Address
		extern const _TCHAR kTelNum[];			//!< ��\�d�b�ԍ�

		extern const _TCHAR kRName_Jpn[];		//!< ���H����(���{��)
		extern const _TCHAR kRName_Eng[];		//!< ���H����(�p��)
		extern const _TCHAR kRName_Tha[];		//!< ���H����(�^�C��)
		extern const _TCHAR kRName_Ind[];		//!< ���H����(�C���h�l�V�A��)
		extern const _TCHAR kRName_May[];		//!< ���H����(�}���[��)
		extern const _TCHAR kRName_Vie[];		//!< ���H����(�x�g�i����)
	}
	// HNP
	namespace hnp
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< ���ʎq(POI)

		extern const _TCHAR kType_C[];			//!< �n�E�X�i���o�[��ʃR�[�h
		extern const _TCHAR kNumber[];			//!< �n�E�X�i���o�[
		extern const _TCHAR kAdmin_C[];			//!< �s���E�R�[�h
		extern const _TCHAR kLinkID[];			//!< ���H�����NID
	}

}	// schema
}	// glshp
#endif	//_GLSHP_H_
