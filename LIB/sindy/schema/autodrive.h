#ifndef SINDY_SCHEMA_AUTODRIVE_H_
#define SINDY_SCHEMA_AUTODRIVE_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>
#include <sindy/libschema.h>

namespace sindy {

namespace schema {

/**
 * �����^�]�x���}�b�v(ADAM v2)
 * @version	2.1.6
 */ 
namespace adam_v2 {
	
	extern const _TCHAR kObjectID[];		///< �I�u�W�F�N�gID�J������

// �Ԑ������N
namespace lane_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;
	using namespace ::sindy::schema::category::adam_v2_link_code;

	extern const _TCHAR kTableName[];			///< �e�[�u����
	
	extern const _TCHAR kLaneGroupID[];			///< �Ԑ��O���[�vOID
	extern const _TCHAR kLaneNumber[];			///< �Ԑ��ԍ�
	extern const _TCHAR kLaneCount[];			///< �Ԑ���
	extern const _TCHAR kMaxLegalSpeedC[];		///< �������x�R�[�h�i�ő�j
	extern const _TCHAR kMinLegalSpeedC[];		///< �������x�R�[�h�i�ŏ��j
	extern const _TCHAR kTravelDirectionC[];	///< �i�s������ʃR�[�h
	extern const _TCHAR kRoadStructureC[];		///< ���H�\����ʃR�[�h
	extern const _TCHAR kOneWayC[];				///< ����ʍs��ʃR�[�h
	extern const _TCHAR kLeftChangeF[];			///< ���Ԑ��ύX�\�t���O
	extern const _TCHAR kRightChangeF[];		///< �E�Ԑ��ύX�\�t���O
	extern const _TCHAR kLeftProtrusionF[];		///< ���͂ݏo���\�t���O
	extern const _TCHAR kRightProtrusionF[];	///< �E�͂ݏo���\�t���O
	extern const _TCHAR kRubbingF[];			///< �C��t���t���O
	extern const _TCHAR kTunnelF[];				///< �g���l���t���O
}

/// �Ԑ��m�[�h
namespace lane_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kPositionC[];			///< �ʒu��ʃR�[�h
	extern const _TCHAR kStopPointF[];			///< ��~�_�t���O
	extern const _TCHAR kCoveringF[];			///< �������̃t���O
}

// �㉺�������N
namespace updown_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;
	using namespace ::sindy::schema::category::adam_v2_link_code;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kUpdoenGroupID[];		///< �㉺���O���[�vOID
	extern const _TCHAR kLaneCount[];			///< �Ԑ���
	extern const _TCHAR kRightChange[];			///< �E�������Ԑ���
	extern const _TCHAR kLeftChange[];			///< ���������Ԑ���
	extern const _TCHAR kMaxLegalSpeedC[];		///< �������x�R�[�h�i�ő�j
	extern const _TCHAR kMinLegalSpeedC[];		///< �������x�R�[�h�i�ŏ��j
	extern const _TCHAR kTravelDirectionC[];	///< �i�s������ʃR�[�h
	extern const _TCHAR kRoadStructureC[];		///< ���H�\����ʃR�[�h
	extern const _TCHAR kOneWayC[];				///< ����ʍs��ʃR�[�h
	extern const _TCHAR kRubbingF[];			///< �C��t���t���O
	extern const _TCHAR kTunnelF[];				///< �g���l���t���O
}

/// �㉺���m�[�h
namespace updown_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kPositionC[];			///< �ʒu��ʃR�[�h
	extern const _TCHAR kStopPointF[];			///< ��~�_�t���O
	extern const _TCHAR kCoveringF[];			///< �������̃t���O
}

// ���������N
namespace compart_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kCompartLineC[];		///< ����ʃR�[�h
	extern const _TCHAR kCompartStyleC[];		///< �����ݒu�l���R�[�h
	extern const _TCHAR kSlowdownLabelC[];		///< �����W���R�[�h
	extern const _TCHAR kInstallationF[];		///< �ݒu���t���O

	/// ADAM������ʃR�[�h
	namespace compart_line_code {
		enum ECode {
			kUnset			= -1,											///< ���ݒ�
			kVirtual		= 0,											///< ���z��
			kWhiteDashed	= 1,											///< ���j��
			kWhiteSolid		= 2,											///< ������
			kYellowSolid	= 3,											///< ������
			kWhiteDashedAndYellowSolid = 13,								///< ���j��(��) + ������(�E)�@ (2�d��)
			kYellowSolidAndWhiteDashed = 31,								///< ������(��) + ���j��(�E)�@ (2�d��)
			kWhiteDashedAndWhiteSolid = 12,									///< ���j��(��) + ������(�E)�@ (2�d��)
			kWhiteSolidAndWhiteDashed = 21,									///< ������(��) + ���j��(�E)�@ (2�d��)
			kWhiteSolidAndWhiteSolid = 22,									///< ������(��) + ������(�E)�@ (2�d��)
			kWhiteSolidAndYellowSolid = 23,									///< ������(��) + ������(�E)�@ (2�d��)
			kYellowSolidAndWhiteSolid = 32,									///< ������(��) + ������(�E)�@ (2�d��)
			kYellowSolidAndYellowSolid = 33,								///< ������(��) + ������(�E)�@ (2�d��)
			kWhiteSolidAndYellowSolidAndWhiteSolid = 232,					///< ������(��) + ������(��) + ������(�E)�@ (3�d��)
			kYellowSolidAndWhiteDashedAndYellowSolid = 313,					///< ������(��) + ���j��(��) + ������(�E)�@ (3�d��)
			kYellowSolidAndWhiteSolidAndYellowSolid = 323,					///< ������(��) + ������(��) + ������(�E)�@ (3�d��)
			kYellowSolidAndWhiteSolidAndWhiteSolidAndYellowSolid = 3223,	///< ������(��) + ������(����) + ������(���E) + ������(�E)�@ (4�d��)
		};
	}

	/// ADAM�����ݒu�l���R�[�h
	namespace compart_style_code {
		enum ECode {
			kUnset			= -1,	///< ���ݒ�
			kLeftOutside	= 1,	///< ���O����
			kRightOutside	= 2,	///< �E�O����
			kLaneMarking	= 3,	///< �Ԑ��敪��
			kCenterLane 	= 4,	///< ������
		};
	}

	namespace slow_down_code {
		enum ECode {
			kUnset		= -1,	///< ���ݒ�
			kNone		= 0,	///< �����W���Ȃ�
			kLeft		= 1,	///< ���Ɍ����W������
			kRight		= 2,	///< �E�Ɍ����W������
			kBoth		= 3,	///< ���E�Ɍ����W������
		};
	}
}

/// �����m�[�h
namespace compart_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

// ���H���E�������N
namespace border_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kImaginaryBorderF[];	///< ���z���t���O
}

/// ���H���E���m�[�h
namespace border_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// �M���@�|�C���g
namespace signal_point{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kHorizontalAngle[];		///< �����p
	extern const _TCHAR kGroundHeight[];		///< �n�㍂
	extern const _TCHAR kHeight[];				///< �c��
	extern const _TCHAR kWidth[];				///< ����
	extern const _TCHAR kSignalMoldC[];			///< �M���@�`��R�[�h
	extern const _TCHAR kLampCount[];			///< �M������
	extern const _TCHAR kArrowF[];				///< ���M���t���O
	extern const _TCHAR kWarningF[];			///< �\���M���t���O
	extern const _TCHAR kIndependentF[];		///< �Ɨ��M���t���O

	/// ADAM�M���@�`��R�[�h
	namespace signal_mold_code {
		enum ECode {
			kUnset			= -1,		///< ���ݒ�
			kVertical		= 1,		///< �c�^
			kHorizontal		= 2,		///< ���^
		};
	}
}

/// ���f�������C��
namespace crosswalk_line{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kWidth[];				///< ��
}

/// ���H�W���|�C���g
namespace roadsign_point{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kHorizontalAngle[];		///< �����p
	extern const _TCHAR kGroundHeight[];		///< �n�㍂
	extern const _TCHAR kHeight[];				///< �c��
	extern const _TCHAR kWidth[];				///< ����
	extern const _TCHAR kRoadSignC[];			///< ���H�W���R�[�h
	extern const _TCHAR kVariableF[];			///< �σt���O

	/// ADAM���H�W���R�[�h
	namespace road_sign_code {
		extern const _TCHAR kUnset[];			///< ���ݒ�
		extern const _TCHAR k116_4[];			///< (116��4)���d�b
		extern const _TCHAR k116_6[];			///< (116��6)��풓�ԑ�
		extern const _TCHAR k119_D[];			///< (119-D)���H�̒ʏ̖�
		extern const _TCHAR k201_A[];			///< (201-A)�\�`���H�����_����
		extern const _TCHAR k201_B[];			///< (201-B)�g�`���H�����_����
		extern const _TCHAR k201_C[];			///< (201-C)�s�`���H�����_����
		extern const _TCHAR k201_D[];			///< (201-D)�x�`���H�����_����
		extern const _TCHAR k201_2[];			///< (201��2)���[�^���[����
		extern const _TCHAR k202[];				///< (202)�E�i���͍��j�����Ȃ���
		extern const _TCHAR k203[];				///< (203)�E�i���͍��j�����܂���
		extern const _TCHAR k204[];				///< (204)�E�i���͍��j���w�����Ȃ���
		extern const _TCHAR k205[];				///< (205)�E�i���͍��j���w�����܂���
		extern const _TCHAR k206[];				///< (206)�E�i���͍��j�Â�܂肠��
		extern const _TCHAR k207_A[];			///< (207-A)���؂���
		extern const _TCHAR k207_B[];			///< (207-B)���؂���
		extern const _TCHAR k208[];				///< (208)�w�Z�A�c�t���A�ۈ珊������
		extern const _TCHAR k208_2[];			///< (208��2)�M���@����
		extern const _TCHAR k209[];				///< (209)���ׂ�₷��
		extern const _TCHAR k209_2[];			///< (209��2)���΂̂����ꂠ��
		extern const _TCHAR k209_3[];			///< (209��3)�H�ʉ��ʂ���
		extern const _TCHAR k210[];				///< (210)������ʂ���
		extern const _TCHAR k211[];				///< (211)�Ԑ�������
		extern const _TCHAR k212[];				///< (212)��������
		extern const _TCHAR k212_2[];			///< (212��2)��������
		extern const _TCHAR k212_3[];			///< (212��3)���}���z����
		extern const _TCHAR k212_4[];			///< (212��4)����}���z����
		extern const _TCHAR k213[];				///< (213)���H�H����
		extern const _TCHAR k214[];				///< (214)��������
		extern const _TCHAR k214_2[];			///< (214��2)��������яo�������ꂠ��
		extern const _TCHAR k215[];				///< (215)���̑��̊댯
		extern const _TCHAR k301[];				///< (301)�ʍs�~��
		extern const _TCHAR k302[];				///< (302)�ԗ��ʍs�~��
		extern const _TCHAR k303[];				///< (303)�ԗ��i���֎~
		extern const _TCHAR k304[];				///< (304)��ւ̎����ԈȊO�̎����Ԓʍs�~��
		extern const _TCHAR k305[];				///< (305)��^�ݕ������ԓ��ʍs�~��
		extern const _TCHAR k305_2[];			///< (305��2)����̍ő�ύڗʈȏ�̉ݕ������ԓ��ʍs�~��
		extern const _TCHAR k306[];				///< (306)��^��p�����ԓ��ʍs�~��
		extern const _TCHAR k307[];				///< (307)��ւ̎����ԁE�����@�t���]�Ԓʍs�~��
		extern const _TCHAR k308[];				///< (308)���]�ԈȊO�̌y�ԗ��ʍs�~��
		extern const _TCHAR k309[];				///< (309)���]�Ԓʍs�~��
		extern const _TCHAR k310[];				///< (310)�ԗ��i�g�����j�ʍs�~��
		extern const _TCHAR k310_2[];			///< (310��2)��^������֎ԋy�ѕ��ʎ�����֎ԓ�l���ʍs�֎~
		extern const _TCHAR k311_A[];			///< (311-A)�w������O�i�s�֎~
		extern const _TCHAR k311_B[];			///< (311-B)�w������O�i�s�֎~
		extern const _TCHAR k311_C[];			///< (311-C)�w������O�i�s�֎~
		extern const _TCHAR k311_D[];			///< (311-D)�w������O�i�s�֎~
		extern const _TCHAR k311_E[];			///< (311-E)�w������O�i�s�֎~
		extern const _TCHAR k311_F[];			///< (311-F)�w������O�i�s�֎~
		extern const _TCHAR k312[];				///< (312)�ԗ����f�֎~
		extern const _TCHAR k313[];				///< (313)�]��֎~
		extern const _TCHAR k314[];				///< (314)�ǉz���̂��߂̉E�������͂ݏo���ʍs�֎~
		extern const _TCHAR k314_2[];			///< (314��2)�ǉz���֎~
		extern const _TCHAR k315[];				///< (315)����ԋ֎~
		extern const _TCHAR k316[];				///< (316)���ԋ֎~
		extern const _TCHAR k317[];				///< (317)���ԗ]�n
		extern const _TCHAR k318[];				///< (318)���Ԑ������ԋ��
		extern const _TCHAR k319[];				///< (319)�댯���ύڎԗ��ʍs�~��
		extern const _TCHAR k320[];				///< (320)�d�ʐ���
		extern const _TCHAR k321[];				///< (321)��������
		extern const _TCHAR k322[];				///< (322)�ő啝
		extern const _TCHAR k323[];				///< (323)�ō����x
		extern const _TCHAR k323_2[];			///< (323��2)����̎�ނ̎ԗ��̍ō����x
		extern const _TCHAR k324[];				///< (324)�Œᑬ�x
		extern const _TCHAR k325[];				///< (325)�����Ԑ�p
		extern const _TCHAR k327_8[];			///< (327��8)�����@�t���]�Ԃ̉E�ܕ��@�i��i�K�j
		extern const _TCHAR k327_9[];			///< (327��9)�����@�t���]�Ԃ̉E�ܕ��@�i�����j
		extern const _TCHAR k327_10[];			///< (327��10)��̌����_�ɂ�����E���ʍs
		extern const _TCHAR k327_11[];			///< (327��11)���s����
		extern const _TCHAR k327_12[];			///< (327��12)���p����
		extern const _TCHAR k327_13[];			///< (327��13)�΂ߒ���
		extern const _TCHAR k328[];				///< (328)�x�J�点
		extern const _TCHAR k328_2[];			///< (328��2)�x�J���
		extern const _TCHAR k329[];				///< (329)���s
		extern const _TCHAR k329_2[];			///< (329��2)�O���D�擹�H
		extern const _TCHAR k330[];				///< (330)�ꎞ��~
		extern const _TCHAR k331[];				///< (331)���s�Ғʍs�~��
		extern const _TCHAR k332[];				///< (332)���s�҉��f�֎~
	}
}

/// �����ς݃G���A
namespace maintenance_area{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// ���H�W���G���A
namespace roadmark_area{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kRoadMarkC[];			///< ���H�W���R�[�h
	extern const _TCHAR kDescription[];			///< �W�����e�i�����j

	/// ADAM���H�W���R�[�h
	namespace roadmark_code {
		extern const _TCHAR kUnset[];			///< ���ݒ�
		extern const _TCHAR k210[];				///< (210)���f�������͎��_�҉��f�т���
		extern const _TCHAR k105_30[];			///< (105)�ō����x30km/h
		extern const _TCHAR k105_40[];			///< (105)�ō����x40km/h
		extern const _TCHAR k105_50[];			///< (105)�ō����x50km/h
	}
}

/// ���z�����C��
namespace virtual_compart_line{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// �������G���A
namespace covering_area{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// ���H�����N�|�㉺�������N�֘A
namespace rel_road_link_updown_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kRoadLinkID[];			///< ���H�����NOID
	extern const _TCHAR kUpdownLinkGID[];		///< �㉺�������NGID
	extern const _TCHAR kRoadLinkDirectionC[];	///< ���H�����N�����R�[�h
}

/// �㉺���y�A�����N
namespace pair_updown_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kUpdownLinkGID1[];		///< �㉺�������NGID1
	extern const _TCHAR kUpdownLinkGID2[];		///< �㉺�������NGID2
}

/// �㉺�������N�O���[�v
namespace updown_link_group{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_group;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// �㉺�������N�|�Ԑ������N�֘A
namespace rel_updown_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kUpdownLinkGID[];		///< �㉺�������NGID
	extern const _TCHAR kLaneLinkGID[];			///< �Ԑ������NGID
}

/// ���H�����N�|�Ԑ������N�֘A
namespace rel_road_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kRoadLinkID[];			///< ���H�����NOID
	extern const _TCHAR kLaneLinkGID[];			///< �Ԑ������NGID
	extern const _TCHAR kRoadLinkDirectionC[];	///< ���H�����N�����R�[�h
}

/// �Ԑ������N�O���[�v
namespace lane_link_group{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_group;

	extern const _TCHAR kTableName[];			///< �e�[�u����
	extern const _TCHAR kUpDownLinkGroupID[];	///< �㉺�������N�O���[�vOID
}

/// ���������N�|�Ԑ������N�֘A
namespace rel_compart_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;
	using namespace ::sindy::schema::category::adam_v2_lr_code;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kCompartLinkGID[];			///< ���������NGID
	extern const _TCHAR kLaneLinkGID[];				///< �Ԑ������NGID
	extern const _TCHAR kCompartLinkDirectionC[];	///< ���������N�����R�[�h
	extern const _TCHAR kLRC[];						///< ���E����R�[�h
	extern const _TCHAR kSequence[];				///< ����
}

/// ���H���E�������N�|�Ԑ������N�֘A
namespace rel_border_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;
	using namespace ::sindy::schema::category::adam_v2_lr_code;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kBorderLinkGID[];			///< ���H���E�������NGID
	extern const _TCHAR kLaneLinkGID[];				///< �Ԑ������NGID
	extern const _TCHAR kBorderLinkDirectionC[];	///< ���H���E�������N�����R�[�h
	extern const _TCHAR kLRC[];						///< ���E����R�[�h
	extern const _TCHAR kSequence[];				///< ����
}

/// �㉺���m�[�h�|�M���@�|�C���g�֘A
namespace rel_updown_node_signal_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kUpdownNodeGID[];			///< �㉺���m�[�hGID
	extern const _TCHAR kSignalPointGID[];			///< �M���@�|�C���gGID
}

/// �Ԑ��m�[�h�|�M���@�|�C���g�֘A
namespace rel_lane_node_signal_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kLaneNodeGID[];				///< �Ԑ��m�[�hGID
	extern const _TCHAR kSignalPointGID[];			///< �M���@�|�C���gGID
}

/// �Ԑ��m�[�h�|���f�������C���֘A
namespace rel_lane_node_crosswalk_line{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kLaneNodeGID[];				///< �Ԑ��m�[�hGID
	extern const _TCHAR kCrosswalkLineGID[];		///< ���f�������C��GID
}

/// �Ԑ��m�[�h�|���H�W���|�C���g�֘A
namespace rel_lane_node_roadsign_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kLaneNodeGID[];				///< �Ԑ��m�[�hGID
	extern const _TCHAR kRoadsignPointGID[];		///< ���H�W���|�C���gGID
}

/// ���H�W���G���A�|�Ԑ������N�֘A
namespace rel_roadmark_area_lane_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kRoadMarkAreaGID[];			///< ���H�W���G���AGID
	extern const _TCHAR kLaneLinkGID[];				///< �Ԑ������NGID
}

/// �㉺���m�[�h��
namespace nq_updown_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kSourceID[];				///< �񋟌�ID
	extern const _TCHAR kUpdownLinkGID[];			///< �㉺�������NGID
	extern const _TCHAR kUpdownNodeGID[];			///< �㉺���m�[�hGID
	extern const _TCHAR kWidth[];					///< ����
}

/// �Ԑ��m�[�h��
namespace nq_lane_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kSourceID[];				///< �񋟌�ID
	extern const _TCHAR kLaneLinkGID[];				///< �Ԑ������NGID
	extern const _TCHAR kLaneNodeGID[];				///< �Ԑ��m�[�hGID
	extern const _TCHAR kLinearElement[];			///< ���`�v�f�����R�[�h
	extern const _TCHAR kWidth[];					///< ����
	extern const _TCHAR kCurvature[];				///< �ȗ�
	extern const _TCHAR kCurvatureRadius[];			///< �ȗ����a
	extern const _TCHAR kCurvatureChangeRate[];		///< �ȗ��ω���
	extern const _TCHAR kClothoidParameter[];		///< �N���\�C�h�p�����[�^
	extern const _TCHAR kLongitudinalSlope[];		///< �c�f���z
	extern const _TCHAR kCrossSlope[];				///< ���f���z

	/// ADAM���`�v�f�����R�[�h
	namespace linear_element_code {
		enum ECode {
			kUnset = -1,		///< ���ݒ�
			kStraight = 1,		///< ����
			kCurve = 2,			///< �P�J�[�u
			kClothoid = 3,		///< �N���\�C�h
		};
	}
}

/// �����m�[�h��
namespace nq_compart_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kSourceID[];				///< �񋟌�ID
	extern const _TCHAR kCompartLinkGID[];			///< ���������NGID
	extern const _TCHAR kCompartNodeGID[];			///< �����m�[�hGID
	extern const _TCHAR kWidth[];					///< ����
}

/// ���H���E���m�[�h��
namespace nq_border_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< �e�[�u����

	extern const _TCHAR kSourceID[];				///< �񋟌�ID
	extern const _TCHAR kBorderLinkGID[];			///< ���H���E�������NGID
	extern const _TCHAR kBorderNodeGID[];			///< ���H���E���m�[�hGID
}

/// ADAM�o�[�W����
/// @NOTE	������ɂ́A�폜�����̂ŁA�e��c�[�����Ŗ{�e�[�u�������݂���O��ł̋@�\�����͂��Ȃ�����
namespace adam_version{
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];				///< �e�[�u����
}

} // namespace adam_v2

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_AUTODRIVE_H_