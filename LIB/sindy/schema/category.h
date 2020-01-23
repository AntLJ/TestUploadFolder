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

#ifndef SINDY_SCHEMA_CATEGORY_H_
#define SINDY_SCHEMA_CATEGORY_H_

namespace sindy {

namespace schema {

namespace category {

/// ���b�V���ɂЂ��Â��e�[�u��
namespace mesh_code_table {
	extern const _TCHAR kMeshCode[]; ///< ���b�V���R�[�h
}

/// �����N�ɂЂ��Â��e�[�u��
namespace link_relational_table {
	extern const _TCHAR kLinkID[]; ///< �����NID
}

/// �����N�Ƃ��̌����ɂЂ��Â��e�[�u��
namespace directional_link_relational_table {
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kLinkDir[]; ///< �����N�����R�[�h

	/// �����N�����R�[�h
	namespace link_dir {
		enum ECode {
			kFore    = 1, ///< ������
			kReverse = 2, ///< �t����
		};
	}
}

/// �n�I�_���m�[�h�ƂЂ��Â��e�[�u��
namespace from_to_node_relational_table {
	extern const _TCHAR kFromNodeID[]; ///< �n�_���m�[�h��OBJECTID
	extern const _TCHAR kToNodeID[]; ///< �I�_���m�[�h��OBJECTID
}

/// ���ԋK���ɂЂ��Â��e�[�u��
namespace time_regulation_relational_table {
	extern const _TCHAR kOneway[];      ///< ����ʍs��ʃR�[�h
	extern const _TCHAR kOnewayRF[];    ///< �����t����ʍs�֌W�t���O
	extern const _TCHAR kNoPassage[];   ///< �ʍs�֎~��ʃR�[�h
	extern const _TCHAR kNoPassageRF[]; ///< �����t�ʍs�֎~�֌W�t���O
} // time_regulation_relational_table
/// ADAM v2�p
/// �n�I�_�m�[�hGID
namespace adam_v2_from_to_node_relational_table_gid {
	extern const _TCHAR kFromNodeGID[]; ///< �n�_���m�[�h��GID
	extern const _TCHAR kToNodeGID[]; ///< �I�_���m�[�h��GID
}
/// �m�[�h
namespace adam_v2_node {
	extern const _TCHAR kNodeClassC[];			///< �m�[�h��ʃR�[�h
	/// ADAM�m�[�h��ʃR�[�h
	namespace node_class_code {
		enum ECode {
			kUnset					= -1,		///< ���ݒ�
			kNode					= 1,	///< �m�[�h
			kInterpolationPoint		= 2,	///< ��ԓ_
		};
	}
}
/// �����N�̃R�[�h�l�h���C��
namespace adam_v2_link_code {
	/// ADAM�������x�R�[�h
	namespace speed_code {
		enum ECode {
			kUnset				= -1,	///< ���ݒ�
			k20km				= 20,	///< 20km/h
			k30km				= 30,	///< 30km/h
			k40km				= 40,	///< 40km/h
			k50km				= 50,	///< 50km/h
			k60km				= 60,	///< 60km/h
			k70km				= 70,	///< 70km/h
			k80km				= 80,	///< 80km/h
			k90km				= 90,	///< 90km/h
			k100km				= 100,	///< 100km/h
			k110km				= 110,	///< 110km/h
			k120km				= 120,	///< 120km/h
			kNoLabel			= 1001,	///< �W��/�\���Ȃ�
			kVariable			= 1002,	///< �ώ�
			kVariable20km		= 2020,	///< �ώ��A20km/h
			kVariable30km		= 2030,	///< �ώ��A30km/h
			kVariable40km		= 2040,	///< �ώ��A40km/h
			kVariable50km		= 2050,	///< �ώ��A50km/h
			kVariable60km		= 2060,	///< �ώ��A60km/h
			kVariable70km		= 2070,	///< �ώ��A70km/h
			kVariable80km		= 2080,	///< �ώ��A80km/h
			kVariable90km		= 2090,	///< �ώ��A90km/h
			kVariable100km		= 2100,	///< �ώ��A100km/h
			kVariable110km		= 2110,	///< �ώ��A110km/h
			kVariable120km		= 2120,	///< �ώ��A120km/h
		};
	}

	/// ADAM�i�s������ʃR�[�h
	namespace travel_dir {
		enum ECode {
			kUnset				= -1,	///< ���ݒ�
			kStraight			= 1,	///< ���i
			kRight				= 2,	///< �E��
			kLeft				= 3,	///< ����
			kRightBranch		= 4,	///< �E����
			kLeftBranch			= 5,	///< ������
			kRightConfluence	= 6,	///< �E����
			kLeftConfluence		= 7,	///< ������
			kRightLaneChange	= 8,	///< �E���[���`�F���W
			kLeftLaneChange		= 9,	///< �����[���`�F���W
		};
	}

	/// ADAM���H�\����ʃR�[�h
	namespace road_structure_code {
		enum ECode {
			kUnset				= -1,	///< ���ݒ�
			kRoad				= 1,	///< �P�H
			kBranchConfluence	= 2,	///< ���򍇗��Ԑ�
			kClimbing			= 3,	///< �o��Ԑ�
			kSapa				= 4,	///< SAPA
			kBusOnly			= 5,	///< �o�X��p
			kNoLaneSection		= 6,	///< ���Ԑ����
			kTollGeneral		= 7,	///< �������i��ʁj
			kTollETC			= 8,	///< �������iETC�j
			kTollETCGeneral		= 9,	///< �������iETC�^��ʁj
			kTollUnknown		= 10,	///< �������i�s���j
			kTollDisabled		= 11,	///< �������i�g�p�s�j
			kTollSwitch			= 12,	///< �������i�؂�ւ��j
		};
	}

	/// ADAM����ʍs��ʃR�[�h
	namespace oneway_code {
		enum ECode {
			kUnset					= -1,	///< ���ݒ�
			kNoOneway				= 0,	///< ����ʍs�Ȃ�
			kFore					= 1,	///< �����Ȃ�����ʍs�i�������j
			kReverse				= 2,	///< �����Ȃ�����ʍs�i�t�����j
			kForeWithReg			= 3,	///< �����t������ʍs�i�������j
			kReverseWithReg			= 4,	///< �����t������ʍs�i�t�����j
			kForeReverseWithReg		= 5,	///< �����t������ʍs�i���t����j
		};
	}
}

/// �m�[�h�̃R�[�h�l�h���C��
namespace adam_v2_node_code {

	/// ADAM�ʒu��ʃR�[�h
	namespace position_code {
		enum ECode {
			kUnset = -1,	///< ���ݒ�
			kNone = 0,		///< �ʒu���Ȃ�
			kToll = 1,		///< �������i�u�[�X�J�n�ʒu�j
		};
	}
}

namespace adam_v2_group{

	extern const _TCHAR kGroupC[];				///< �O���[�v��ʃR�[�h

	/// ADAM�O���[�v��ʃR�[�h
	namespace group_code {
		enum ECode {
			kUnset				= -1,		///< ���ݒ�
			kRoad				= 1,		///< �Ԑ�
			kCross				= 2,		///< �����_
			kBranchConfluence	= 3,		///< ������
			kToll				= 4,		///< ������
			kSAPA				= 5,		///< SAPA
			kSmartIC			= 6,		///< �X�}�[�gIC
		};
	}
}

namespace adam_v2_link_dir_code{
	/// ADAM�����N�����R�[�h
	namespace roadlink_dir_code {
		enum ECode {
			kUnset				= -1,		///< ���ݒ�
			kForward			= 1,		///< ������
			kReverse			= 2,		///< �t����
		};
	}
}

namespace adam_v2_lr_code{
	/// ADAM���E����R�[�h
	namespace lr_code {
		enum ECode {
			kUnset				= -1,		///< ���ݒ�
			kLeft				= 0,		///< ��
			kRight				= 1,		///< �E
		};
	}
}
/* ============================================================================== */

/// ���ԋK�������e�[�u��
namespace time_condition_table {
	extern const _TCHAR kStartMonth[]; ///< �K������:�J�n��
	extern const _TCHAR kStartDay[]; ///< �K������:�J�n��
	extern const _TCHAR kStartHour[]; ///< �K������:�J�n����
	extern const _TCHAR kStartMin[]; ///< �K������:�J�n��
	extern const _TCHAR kEndMonth[]; ///< �K������;�I����
	extern const _TCHAR kEndDay[]; ///< �K������:�I����
	extern const _TCHAR kEndHour[]; ///< �K������:�I������
	extern const _TCHAR kEndMin[]; ///< �K������:�I����
	extern const _TCHAR kDayOfWeek[]; ///< �K������:�j���w��

	namespace dayofweek {
		enum ECode {
			kSunHoliday          = 1,  ///< ���E�x��
			kExceptSunHoliday    = 2,  ///< ���E�x�����̂���
			kHoliday             = 3,  ///< �x��(���j�͂̂���)
			kExceptHoliday       = 4,  ///< �x�����̂���)
			kWeekday             = 5,  ///< ����
			kSunday              = 6,  ///< ���j
			kMonday              = 7,  ///< ��
			kTuesday             = 8,  ///< ��
			kWednesday           = 9,  ///< ��
			kThursday            = 10, ///< ��
			kFriday              = 11, ///< ��
			kSaturday            = 12, ///< �y
			kSatSunHoliday       = 13, ///< �y���E�x��
			kExceptSatSunHoliday = 14, ///< �y���E�x�����̂���
			kAllDay              = 15, ///< �S�j��
			kExceptSunday        = 16, ///< ���j�����̂���
			kBeforeSunHoliday    = 17, ///< ���E�x���̑O��
		};
	}
}

/// VICS�e�[�u��
namespace vics_table {
	extern const _TCHAR kVICSMeshCode[]; ///< VICS���b�V���R�[�h
	extern const _TCHAR kVICSClass[]; ///< VICS�����N�敪
	extern const _TCHAR kVICSLinkID[]; ///< VICS�����N�ԍ�

	/// VICS�����N�敪
	namespace vics_class {
		enum ECode {
			kHighway     = 1, ///< �������H
			kCityHighway = 2, ///< �s�s�������H
			kNormal      = 3, ///< ��ʓ�
			kOther       = 4, ///< ���̑��̓��H
		};
	}
	/// IPC-VICS�����N�敪
	namespace ipcvics_class {
		enum ECode {
			kCode1		= 1, ///< �敪1
			kCode2		= 2, ///< �敪2
			kCode3		= 3, ///< �敪3
			kCode4		= 4, ///< �敪4
		};
	}
}

/// ��VICS�e�[�u��
namespace original_vics_table {
	extern const _TCHAR kMeshCode[]; ///< VICS���b�V���R�[�h
	extern const _TCHAR kVICSClass[]; ///< VICS�����N�敪
	extern const _TCHAR kVICSLinkID[]; ///< VICS�����N�ԍ�
}

namespace drive_map_table {
	extern const _TCHAR kMeshCode[]; ///< ����₷���}�b�v���b�V���R�[�h
	extern const _TCHAR kDriveMapRank[]; ///< ����₷���}�b�v�]��
	extern const _TCHAR kRankLinkID[]; ///< ����₷���}�b�v�����N�ԍ�
}

/// ���ԏ�֌W�e�[�u��
namespace parking_table {
	extern const _TCHAR kParkingClass[]; ///< ���ԏ��ʃR�[�h

	/// ���ԏ��ʃR�[�h
	namespace parking_class {
		enum ECode {
			kUnsurveyed           = 0, ///< ������
			kStructure            = 1, ///< ���̒��ԏ�
			kUnderGround          = 2, ///< �n�����ԏ�
			kUnderGroundStructure = 3, ///< ���̂��n�����ԏ�
			kTower                = 4, ///< �^���[�p�[�L���O
			kRoll                 = 5, ///< ������]�^���[�p�[�L���O
			kFlat                 = 6, ///< ���ʒ��ԏ�
		};
	}
}

/// �n�C�E�F�C�̘H���Ɋ֘A�t���e�[�u��
namespace highway_road_relational_table {
	extern const _TCHAR kRoadCode[]; ///< �H���R�[�h
}

/// �n�C�E�F�C�̎{�݂Ɋ֘A�t���e�[�u��
namespace highway_facil_relational_table {
	using namespace ::sindy::schema::category::highway_road_relational_table;

	extern const _TCHAR kRoadSeq[]; ///< �H�����V�[�P���X
}

/// �n�C�E�F�C�����e�[�u��
namespace highway_direction_table {
	extern const _TCHAR kDirection[]; ///< �n�C�E�F�C�����R�[�h

	/// �����R�[�h��`
	namespace direction {
		enum ECode {
			kNoDir  = 0, ///< �����Ȃ�
			kUp     = 1, ///< �����P�i���E�����E���s���j
			kDown   = 2, ///< �����Q�i����E�O���E���s���j
			kUpDown = 3, ///< �o��������
		};
	}
}

/// �����E�o�������R�[�h���܂ރe�[�u��
namespace highway_in_out_table {
	extern const _TCHAR kInOut[]; ///< �����E�o������

	/// �����i�����j�E�o���i����j�R�[�h��`
	namespace in_out {
		enum ECode {
			kInOut = 0, ///< ��ʂȂ�
			kIn    = 1, ///< �����E����
			kOut   = 2, ///< �o���E����
		};
	}
}

/// SiNDY-c�Ɋւ���e�[�u��
namespace sindyc_table {
	extern const _TCHAR kOrgOperator[];    ///< ORG_OPERATOR
	extern const _TCHAR kOrgModifyDate[];  ///< ORG_MODIFY_DATE
	extern const _TCHAR kOrgOBJID[];       ///< ORG_OBJID
	extern const _TCHAR kUpdateC[];        ///< UPDATE_C
	extern const _TCHAR kCheckC[];         ///< CHECK_C

	/// �X�V�R�[�h�i�d�˂��킹�j
	namespace update_code {
		enum ECode {
			kDefault = 0,      ///< �X�V�Ȃ�
			kShapeUpdated = 1, ///< �`��ύX
			kAttrUpdated = 2,  ///< �����ύX
			kDeleted = 4,      ///< �폜
			kCreated = 8,      ///< �V�K�쐬
		};
	}
}

/// �V���A��ID���܂ރe�[�u��
namespace serial_id_table
{
	extern const _TCHAR kSerialID[];	//!< �V���A��ID
}

/// �R���e���c���V�[�P���X���܂ރe�[�u��
namespace contents_seq_table
{
	extern const _TCHAR kContentsSeq[];	//!< �R���e���c���V�[�P���X
}

/// �f�[�^���ރR�[�h���܂ރe�[�u��
namespace group_code_table
{
	extern const _TCHAR kGroupCode[];	//!< �f�[�^���ރR�[�h
}

/// �X�܃R�[�h���܂ރe�[�u��
namespace chain_code_table
{
	extern const _TCHAR kChainCode[];	//!< �X�܃R�[�h
}

/// �R���e���c�R�[�h���܂ރe�[�u��
namespace contents_code_table
{
	extern const _TCHAR kContentsCode[];	//!< �R���e���c�R�[�h
}

/// ��ԕʏ̂Ɋ֘A�t���e�[�u�� [bug 12036]
namespace section_code_relational_table {
	extern const _TCHAR kSectionCode[];	//!< ��ԕʏ̃R�[�h
}

/// �s���{�������̂Ɋ֘A�t���e�[�u�� [bug 12036]
namespace pref_road_relational_table {
	extern const _TCHAR kRoadClass[];	//!< ���H�\�����
	extern const _TCHAR kPrefCode[];	//!< �s���{���R�[�h
	extern const _TCHAR kRoadNo[];		//!< �����ԍ�

		/// ���H�\���p��ʃR�[�h
	namespace road_class {
		enum ECode {
			kNone	              =   0, ///< �\����ʂȂ��i[Bug8941]�Œǉ��j
			kCountry              =   1, ///< ��ʍ���
			kPrefectureMain       =   2, ///< ��v�n�����i�s�{���j
			kCityMain             =   3, ///< ��v�n�����i�w��s�j
			kPrefectureNormal     =   4, ///< ��ʓs���{����
			kCityNormal           =   5, ///< �w��s�̈�ʎs��
			kOther                =   6, ///< ���̑����H
			kOtherNoDisp          =   7, ///< ���̑����H�i��\���j
			kNarrowL1             =   8, ///< �׊X�HL1
			kNarrowL2             =   9, ///< �׊X�HL2
			kNarrowL3             =  10, ///< �׊X�HL3
			kFreeHighway		  =	 11, ///< ���������ԓ��H
			kHighway              = 101, ///< ���������ԓ��H�i�L���j
			kCityHighway          = 102, ///< �s�s�������H�i�L���j
			kTollCountry          = 103, ///< ��ʍ����i�L���j
			kTollPrefectureMain   = 104, ///< ��v�n�����i�s�{���j�i�L���j
			kTollCityMain         = 105, ///< ��v�n�����i�w��s�j�i�L���j
			kTollPrefectureNormal = 106, ///< ��ʓs���{�����i�L���j
			kTollCityNormal       = 107, ///< �w��s�̈�ʎs���i�L���j
			kTollOther            = 108, ///< ���̑����H�i�L���j
			kFerryNoDisp          = 201, ///< �t�F���[�q�H�i��\�����j
			kFerryS2              = 202, ///< �t�F���[�q�H�iS2�j���\�����j
			kFerryS3              = 203, ///< �t�F���[�q�H�iS3�j���\�����j
			kFerryS4              = 204, ///< �t�F���[�q�H�iS4�j���\�����j
			kVirtual              = 301, ///< ���z�����N
			kGarden               = 302, ///< �뉀�H
			kBridge               = 303, ///< �u���b�W�����N
			kFacilityEntrance     = 304, ///< �{�ݏo���������N
			kParkingEntrance      = 305, ///< �{�ݓ������N
			kParking              = 401, ///< ���ԏ�\�����H�����N(PEC)
			kMatching             = 402, ///< �}�b�`���O�p�����N(PEC)
			kMatchingBT			  = 501, ///< �}�b�`���O�p�����N(BT)
		};
	}
}

} // namespace category

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_CATEGORY_H_
