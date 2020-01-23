#include "stdafx.h"
#include <boost/preprocessor.hpp>
#include <sindy/schema/autodrive.h>

namespace sindy {

namespace schema {

namespace adam_v2 {
	const _TCHAR kObjectID[] = _T("OBJECTID");	///< �I�u�W�F�N�gID�J������

	// �Ԑ������N
namespace lane_link {
	const _TCHAR kTableName[]			= _T("LANE_LINK");				///< �e�[�u����
	
	const _TCHAR kLaneGroupID[]			= _T("LANE_GROUP_ID");			///< �Ԑ��O���[�vOID
	const _TCHAR kLaneNumber[]			= _T("LANE_NUMBER");			///< �Ԑ��ԍ�
	const _TCHAR kLaneCount[]			= _T("LANE_COUNT");				///< �Ԑ���
	const _TCHAR kMaxLegalSpeedC[]		= _T("MAX_LEGAL_SPEED_C");		///< �������x�R�[�h�i�ő�j
	const _TCHAR kMinLegalSpeedC[]		= _T("MIN_LEGAL_SPEED_C");		///< �������x�R�[�h�i�ŏ��j
	const _TCHAR kTravelDirectionC[]	= _T("TRAVEL_DIRECTION_C");		///< �i�s������ʃR�[�h
	const _TCHAR kRoadStructureC[]		= _T("ROAD_STRUCTURE_C");		///< ���H�\����ʃR�[�h
	const _TCHAR kOneWayC[]				= _T("ONEWAY_C");				///< ����ʍs��ʃR�[�h
	const _TCHAR kLeftChangeF[]			= _T("LEFT_CHANGE_F");			///< ���Ԑ��ύX�\�t���O
	const _TCHAR kRightChangeF[]		= _T("RIGHT_CHANGE_F");			///< �E�Ԑ��ύX�\�t���O
	const _TCHAR kLeftProtrusionF[]		= _T("LEFT_PROTRUSION_F");		///< ���͂ݏo���\�t���O
	const _TCHAR kRightProtrusionF[]	= _T("RIGHT_PROTRUSION_F");		///< �E�͂ݏo���\�t���O
	const _TCHAR kRubbingF[]			= _T("RUBBING_F");				///< �C��t���t���O
	const _TCHAR kTunnelF[]				= _T("TUNNEL_F");				///< �g���l���t���O
}

/// �Ԑ��m�[�h
namespace lane_node{
	const _TCHAR kTableName[]			= _T("LANE_NODE");				///< �e�[�u����

	const _TCHAR kPositionC[]			= _T("POSITION_C");				///< �ʒu��ʃR�[�h
	const _TCHAR kStopPointF[]			= _T("STOP_POINT_F");			///< ��~�_�t���O
	const _TCHAR kCoveringF[]			= _T("COVERING_F");				///< �������̃t���O
}

// �㉺�������N
namespace updown_link {
	const _TCHAR kTableName[]			= _T("UPDOWN_LINK");			///< �e�[�u����

	const _TCHAR kUpdoenGroupID[]		= _T("UPDOWN_GROUP_ID");		///< �㉺���O���[�vOID
	const _TCHAR kLaneCount[]			= _T("LANE_COUNT");				///< �Ԑ���
	const _TCHAR kRightChange[]			= _T("RIGHT_CHANGE");			///< �E�������Ԑ���
	const _TCHAR kLeftChange[]			= _T("LEFT_CHANGE");			///< ���������Ԑ���
	const _TCHAR kMaxLegalSpeedC[]		= _T("MAX_LEGAL_SPEED_C");		///< �������x�R�[�h�i�ő�j
	const _TCHAR kMinLegalSpeedC[]		= _T("MIN_LEGAL_SPEED_C");		///< �������x�R�[�h�i�ŏ��j
	const _TCHAR kTravelDirectionC[]	= _T("TRAVEL_DIRECTION_C");		///< �i�s������ʃR�[�h
	const _TCHAR kRoadStructureC[]		= _T("ROAD_STRUCTURE_C");		///< ���H�\����ʃR�[�h
	const _TCHAR kOneWayC[]				= _T("ONEWAY_C");				///< ����ʍs��ʃR�[�h
	const _TCHAR kRubbingF[]			= _T("RUBBING_F");				///< �C��t���t���O
	const _TCHAR kTunnelF[]				= _T("TUNNEL_F");				///< �g���l���t���O
}

/// �㉺���m�[�h
namespace updown_node{
	const _TCHAR kTableName[]			= _T("UPDOWN_NODE");			///< �e�[�u����

	const _TCHAR kPositionC[]			= _T("POSITION_C");				///< �ʒu��ʃR�[�h
	const _TCHAR kStopPointF[]			= _T("STOP_POINT_F");			///< ��~�_�t���O
	const _TCHAR kCoveringF[]			= _T("COVERING_F");				///< �������̃t���O
}

// ���������N
namespace compart_link {
	const _TCHAR kTableName[]			= _T("COMPART_LINK");			///< �e�[�u����

	const _TCHAR kCompartLineC[]		= _T("COMPART_LINE_C");			///< ����ʃR�[�h
	const _TCHAR kCompartStyleC[]		= _T("COMPART_STYLE_C");		///< �����ݒu�l���R�[�h
	const _TCHAR kSlowdownLabelC[]		= _T("SLOWDOWN_LABEL_C");		///< �����W���R�[�h
	const _TCHAR kInstallationF[]		= _T("INSTALLATION_F");			///< �ݒu���t���O
}

/// �����m�[�h
namespace compart_node{
	const _TCHAR kTableName[]			= _T("COMPART_NODE");			///< �e�[�u����
}

// ���H���E�������N
namespace border_link {
	const _TCHAR kTableName[]			= _T("BORDER_LINK");			///< �e�[�u����

	const _TCHAR kImaginaryBorderF[]	= _T("IMAGINARY_BODER_F");		///< ���z���t���O
}

/// ���H���E���m�[�h
namespace border_node{
	const _TCHAR kTableName[]			= _T("BORDER_NODE");			///< �e�[�u����
}

/// �M���@�|�C���g
namespace signal_point{
	const _TCHAR kTableName[]			= _T("SIGNAL_POINT");			///< �e�[�u����

	const _TCHAR kHorizontalAngle[]		= _T("HORIZONTAL_ANGLE");		///< �����p
	const _TCHAR kGroundHeight[]		= _T("GROUND_HEIGHT");			///< �n�㍂
	const _TCHAR kHeight[]				= _T("HEIGHT");					///< �c��
	const _TCHAR kWidth[]				= _T("WIDTH");					///< ����
	const _TCHAR kSignalMoldC[]			= _T("SIGNAL_MOLD_C");			///< �M���@�`��R�[�h
	const _TCHAR kLampCount[]			= _T("LAMP_COUNT");				///< �M������
	const _TCHAR kArrowF[]				= _T("ARROW_F");				///< ���M���t���O
	const _TCHAR kWarningF[]			= _T("WARNING_F");				///< �\���M���t���O
	const _TCHAR kIndependentF[]		= _T("INDEPENDENT_F");			///< �Ɨ��M���t���O
}

/// ���f�������C��
namespace crosswalk_line{
	const _TCHAR kTableName[]			= _T("CROSSWALK_LINE");			///< �e�[�u����

	const _TCHAR kWidth[]				= _T("WIDTH");					///< ��
}

/// ���H�W���|�C���g
namespace roadsign_point{
	const _TCHAR kTableName[]			= _T("ROADSIGN_POINT");			///< �e�[�u����

	const _TCHAR kHorizontalAngle[]		= _T("HORIZONTAL_ANGLE");		///< �����p
	const _TCHAR kGroundHeight[]		= _T("GROUND_HEIGHT");			///< �n�㍂
	const _TCHAR kHeight[]				= _T("HEIGHT");					///< �c��
	const _TCHAR kWidth[]				= _T("WIDTH");					///< ����
	const _TCHAR kRoadSignC[]			= _T("ROADSIGN_C");				///< ���H�W���R�[�h
	const _TCHAR kVariableF[]			= _T("VARIABLE_F");				///< �σt���O

	/// ADAM���H�W���R�[�h
	namespace road_sign_code {
		const _TCHAR kUnset[]		= _T("-1");			///< ���ݒ�
		const _TCHAR k116_4[]		= _T("116��4");		///< (116��4)���d�b
		const _TCHAR k116_6[]		= _T("116��6");		///< (116��6)��풓�ԑ�
		const _TCHAR k119_D[]		= _T("119-D");		///< (119-D)���H�̒ʏ̖�
		const _TCHAR k201_A[]		= _T("201-A");		///< (201-A)�\�`���H�����_����
		const _TCHAR k201_B[]		= _T("201-B");		///< (201-B)�g�`���H�����_����
		const _TCHAR k201_C[]		= _T("201-C");		///< (201-C)�s�`���H�����_����
		const _TCHAR k201_D[]		= _T("201-D");		///< (201-D)�x�`���H�����_����
		const _TCHAR k201_2[]		= _T("201��2");		///< (201��2)���[�^���[����
		const _TCHAR k202[]			= _T("202");		///< (202)�E�i���͍��j�����Ȃ���
		const _TCHAR k203[]			= _T("203");		///< (203)�E�i���͍��j�����܂���
		const _TCHAR k204[]			= _T("204");		///< (204)�E�i���͍��j���w�����Ȃ���
		const _TCHAR k205[]			= _T("205");		///< (205)�E�i���͍��j���w�����܂���
		const _TCHAR k206[]			= _T("206");		///< (206)�E�i���͍��j�Â�܂肠��
		const _TCHAR k207_A[]		= _T("207-A");		///< (207-A)���؂���
		const _TCHAR k207_B[]		= _T("207-B");		///< (207-B)���؂���
		const _TCHAR k208[]			= _T("208");		///< (208)�w�Z�A�c�t���A�ۈ珊������
		const _TCHAR k208_2[]		= _T("208��2");		///< (208��2)�M���@����
		const _TCHAR k209[]			= _T("209");		///< (209)���ׂ�₷��
		const _TCHAR k209_2[]		= _T("209��2");		///< (209��2)���΂̂����ꂠ��
		const _TCHAR k209_3[]		= _T("209��3");		///< (209��3)�H�ʉ��ʂ���
		const _TCHAR k210[]			= _T("210");		///< (210)������ʂ���
		const _TCHAR k211[]			= _T("211");		///< (211)�Ԑ�������
		const _TCHAR k212[]			= _T("212");		///< (212)��������
		const _TCHAR k212_2[]		= _T("212��2");		///< (212��2)��������
		const _TCHAR k212_3[]		= _T("212��3");		///< (212��3)���}���z����
		const _TCHAR k212_4[]		= _T("212��4");		///< (212��4)����}���z����
		const _TCHAR k213[]			= _T("213");		///< (213)���H�H����
		const _TCHAR k214[]			= _T("214");		///< (214)��������
		const _TCHAR k214_2[]		= _T("214��2");		///< (214��2)��������яo�������ꂠ��
		const _TCHAR k215[]			= _T("215");		///< (215)���̑��̊댯
		const _TCHAR k301[]			= _T("301");		///< (301)�ʍs�~��
		const _TCHAR k302[]			= _T("302");		///< (302)�ԗ��ʍs�~��
		const _TCHAR k303[]			= _T("303");		///< (303)�ԗ��i���֎~
		const _TCHAR k304[]			= _T("304");		///< (304)��ւ̎����ԈȊO�̎����Ԓʍs�~��
		const _TCHAR k305[]			= _T("305");		///< (305)��^�ݕ������ԓ��ʍs�~��
		const _TCHAR k305_2[]		= _T("305��2");		///< (305��2)����̍ő�ύڗʈȏ�̉ݕ������ԓ��ʍs�~��
		const _TCHAR k306[]			= _T("306");		///< (306)��^��p�����ԓ��ʍs�~��
		const _TCHAR k307[]			= _T("307");		///< (307)��ւ̎����ԁE�����@�t���]�Ԓʍs�~��
		const _TCHAR k308[]			= _T("308");		///< (308)���]�ԈȊO�̌y�ԗ��ʍs�~��
		const _TCHAR k309[]			= _T("309");		///< (309)���]�Ԓʍs�~��
		const _TCHAR k310[]			= _T("310");		///< (310)�ԗ��i�g�����j�ʍs�~��
		const _TCHAR k310_2[]		= _T("310��2");		///< (310��2)��^������֎ԋy�ѕ��ʎ�����֎ԓ�l���ʍs�֎~
		const _TCHAR k311_A[]		= _T("311-A");		///< (311-A)�w������O�i�s�֎~
		const _TCHAR k311_B[]		= _T("311-B");		///< (311-B)�w������O�i�s�֎~
		const _TCHAR k311_C[]		= _T("311-C");		///< (311-C)�w������O�i�s�֎~
		const _TCHAR k311_D[]		= _T("311-D");		///< (311-D)�w������O�i�s�֎~
		const _TCHAR k311_E[]		= _T("311-E");		///< (311-E)�w������O�i�s�֎~
		const _TCHAR k311_F[]		= _T("311-F");		///< (311-F)�w������O�i�s�֎~
		const _TCHAR k312[]			= _T("312");		///< (312)�ԗ����f�֎~
		const _TCHAR k313[]			= _T("313");		///< (313)�]��֎~
		const _TCHAR k314[]			= _T("314");		///< (314)�ǉz���̂��߂̉E�������͂ݏo���ʍs�֎~
		const _TCHAR k314_2[]		= _T("314��2");		///< (314��2)�ǉz���֎~
		const _TCHAR k315[]			= _T("315");		///< (315)����ԋ֎~
		const _TCHAR k316[]			= _T("316");		///< (316)���ԋ֎~
		const _TCHAR k317[]			= _T("317");		///< (317)���ԗ]�n
		const _TCHAR k318[]			= _T("318");		///< (318)���Ԑ������ԋ��
		const _TCHAR k319[]			= _T("319");		///< (319)�댯���ύڎԗ��ʍs�~��
		const _TCHAR k320[]			= _T("320");		///< (320)�d�ʐ���
		const _TCHAR k321[]			= _T("321");		///< (321)��������
		const _TCHAR k322[]			= _T("322");		///< (322)�ő啝
		const _TCHAR k323[]			= _T("323");		///< (323)�ō����x
		const _TCHAR k323_2[]		= _T("323��2");		///< (323��2)����̎�ނ̎ԗ��̍ō����x
		const _TCHAR k324[]			= _T("324");		///< (324)�Œᑬ�x
		const _TCHAR k325[]			= _T("325");		///< (325)�����Ԑ�p
		const _TCHAR k327_8[]		= _T("327��8");		///< (327��8)�����@�t���]�Ԃ̉E�ܕ��@�i��i�K�j
		const _TCHAR k327_9[]		= _T("327��9");		///< (327��9)�����@�t���]�Ԃ̉E�ܕ��@�i�����j
		const _TCHAR k327_10[]		= _T("327��10");	///< (327��10)��̌����_�ɂ�����E���ʍs
		const _TCHAR k327_11[]		= _T("327��11");	///< (327��11)���s����
		const _TCHAR k327_12[]		= _T("327��12");	///< (327��12)���p����
		const _TCHAR k327_13[]		= _T("327��13");	///< (327��13)�΂ߒ���
		const _TCHAR k328[]			= _T("328");		///< (328)�x�J�点
		const _TCHAR k328_2[]		= _T("328��2");		///< (328��2)�x�J���
		const _TCHAR k329[]			= _T("329");		///< (329)���s
		const _TCHAR k329_2[]		= _T("329��2");		///< (329��2)�O���D�擹�H
		const _TCHAR k330[]			= _T("330");		///< (330)�ꎞ��~
		const _TCHAR k331[]			= _T("331");		///< (331)���s�Ғʍs�~��
		const _TCHAR k332[]			= _T("332");		///< (332)���s�҉��f�֎~
	}
}

/// �����ς݃G���A
namespace maintenance_area{
	const _TCHAR kTableName[]				= _T("MAINTENANCE_AREA");				///< �e�[�u����
}

/// ���H�W���G���A
namespace roadmark_area{
	const _TCHAR kTableName[]				= _T("ROADMARK_AREA");				///< �e�[�u����

	const _TCHAR kRoadMarkC[]				= _T("ROADMARK_C");					///< ���H�W���R�[�h
	const _TCHAR kDescription[]				= _T("DESCRIPTION");				///< �W�����e�i�����j

	/// ADAM���H�W���R�[�h
	namespace roadmark_code {
		const _TCHAR kUnset[]				= _T("-1");							///< ���ݒ�
		const _TCHAR k210[]					= _T("210");						///< (210)���f�������͎��_�҉��f�т���
		const _TCHAR k105_30[]				= _T("105(30)");					///< (105)�ō����x30km/h
		const _TCHAR k105_40[]				= _T("105(40)");					///< (105)�ō����x40km/h
		const _TCHAR k105_50[]				= _T("105(50)");					///< (105)�ō����x50km/h
	}
}

/// ���z�����C��
namespace virtual_compart_line{
	const _TCHAR kTableName[]				= _T("VIRTUAL_COMPART_LINE");				///< �e�[�u����
}

/// �������G���A
namespace covering_area{
	const _TCHAR kTableName[]				= _T("COVERING_AREA");				///< �e�[�u����
}

/// ���H�����N�|�㉺�������N�֘A
namespace rel_road_link_updown_link{
	const _TCHAR kTableName[]				= _T("REL_ROAD_LINK_UPDOWN_LINK");		///< �e�[�u����

	const _TCHAR kRoadLinkID[]				= _T("ROAD_LINK_ID");					///< ���H�����NOID
	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< �㉺�������NGID
	const _TCHAR kRoadLinkDirectionC[]		= _T("ROAD_LINK_DIRECTION_C");			///< ���H�����N�����R�[�h
}

/// �㉺���y�A�����N
namespace pair_updown_link{
	const _TCHAR kTableName[]				= _T("PAIR_UPDOWN_LINK");				///< �e�[�u����

	const _TCHAR kUpdownLinkGID1[]			= _T("UPDOWN_LINK_GID1");				///< �㉺�������NGID1
	const _TCHAR kUpdownLinkGID2[]			= _T("UPDOWN_LINK_GID2");				///< �㉺�������NGID2
}

/// �㉺�������N�O���[�v
namespace updown_link_group{
	const _TCHAR kTableName[]				= _T("UPDOWN_LINK_GROUP");				///< �e�[�u����
}

/// �㉺�������N�|�Ԑ������N�֘A
namespace rel_updown_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_UPDOWN_LINK_LANE_LINK");		///< �e�[�u����

	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< �㉺�������NGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
}

/// ���H�����N�|�Ԑ������N�֘A
namespace rel_road_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_ROAD_LINK_LANE_LINK");		///< �e�[�u����

	const _TCHAR kRoadLinkID[]				= _T("ROAD_LINK_ID");					///< ���H�����NOID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
	const _TCHAR kRoadLinkDirectionC[]		= _T("ROAD_LINK_DIRECTION_C");			///< ���H�����N�����R�[�h
}

/// �Ԑ������N�O���[�v
namespace lane_link_group{
	const _TCHAR kTableName[]				= _T("LANE_LINK_GROUP");				///< �e�[�u����
	const _TCHAR kUpDownLinkGroupID[]		= _T("UPDOWN_LINK_GROUP_ID");			///< �㉺�������N�O���[�vOID
}

/// ���������N�|�Ԑ������N�֘A
namespace rel_compart_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_COMPART_LINK_LANE_LINK");		///< �e�[�u����

	const _TCHAR kCompartLinkGID[]			= _T("COMPART_LINK_GID");				///< ���������NGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
	const _TCHAR kCompartLinkDirectionC[]	= _T("COMPART_LINK_DIRECTION_C");		///< ���������N�����R�[�h
	const _TCHAR kLRC[]						= _T("LR_C");							///< ���E����R�[�h
	const _TCHAR kSequence[]				= _T("SEQUENCE");						///< ����
}

/// ���H���E�������N�|�Ԑ������N�֘A
namespace rel_border_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_BORDER_LINK_LANE_LINK");		///< �e�[�u����

	const _TCHAR kBorderLinkGID[]			= _T("BORDER_LINK_GID");				///< ���H���E�������NGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
	const _TCHAR kBorderLinkDirectionC[]	= _T("BORDER_LINK_DIRECTION_C");		///< ���H���E�������N�����R�[�h
	const _TCHAR kLRC[]						= _T("LR_C");							///< ���E����R�[�h
	const _TCHAR kSequence[]				= _T("SEQUENCE");						///< ����
}

/// �㉺���m�[�h�|�M���@�|�C���g�֘A
namespace rel_updown_node_signal_point{
	const _TCHAR kTableName[]				= _T("REL_UPDOWN_NODE_SIGNAL_POINT");	///< �e�[�u����

	const _TCHAR kUpdownNodeGID[]			= _T("UPDOWN_NODE_GID");				///< �㉺���m�[�hGID
	const _TCHAR kSignalPointGID[]			= _T("SIGNAL_POINT_GID");				///< �M���@�|�C���gGID
}

/// �Ԑ��m�[�h�|�M���@�|�C���g�֘A
namespace rel_lane_node_signal_point{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_SIGNAL_POINT");		///< �e�[�u����

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< �Ԑ��m�[�hGID
	const _TCHAR kSignalPointGID[]			= _T("SIGNAL_POINT_GID");				///< �M���@�|�C���gGID
}

/// �Ԑ��m�[�h�|���f�������C���֘A
namespace rel_lane_node_crosswalk_line{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_CROSSWALK_LINE");	///< �e�[�u����

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< �Ԑ��m�[�hGID
	const _TCHAR kCrosswalkLineGID[]		= _T("CROSSWALK_LINE_GID");				///< ���f�������C��GID
}

/// �Ԑ��m�[�h�|���H�W���|�C���g�֘A
namespace rel_lane_node_roadsign_point{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_ROADSIGN_POINT");	///< �e�[�u����

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< �Ԑ��m�[�hGID
	const _TCHAR kRoadsignPointGID[]		= _T("ROADSIGN_POINT_GID");				///< ���H�W���|�C���gGID
}

/// ���H�W���G���A�|�Ԑ������N�֘A
namespace rel_roadmark_area_lane_link{
	const _TCHAR kTableName[]				= _T("REL_ROADMARK_AREA_LANE_LINK");	///< �e�[�u����

	const _TCHAR kRoadMarkAreaGID[]			= _T("ROADMARK_AREA_GID");				///< ���H�W���G���AGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
}

/// �㉺���m�[�h��
namespace nq_updown_node{
	const _TCHAR kTableName[]				= _T("NQ_UPDOWN_NODE");					///< �e�[�u����

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< �񋟌�ID
	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< �㉺�������NGID
	const _TCHAR kUpdownNodeGID[]			= _T("UPDOWN_NODE_GID");				///< �㉺���m�[�hGID
	const _TCHAR kWidth[]					= _T("WIDTH");							///< ����
}

/// �Ԑ��m�[�h��
namespace nq_lane_node{
	const _TCHAR kTableName[]				= _T("NQ_LANE_NODE");					///< �e�[�u����

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< �񋟌�ID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< �Ԑ������NGID
	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< �Ԑ��m�[�hGID
	const _TCHAR kLinearElement[]			= _T("LINEAR_ELEMENT_C");				///< ���`�v�f�����R�[�h
	const _TCHAR kWidth[]					= _T("WIDTH");							///< ����
	const _TCHAR kCurvature[]				= _T("CURVATURE");						///< �ȗ�
	const _TCHAR kCurvatureRadius[]			= _T("CURVATURE_RADIUS");				///< �ȗ����a
	const _TCHAR kCurvatureChangeRate[]		= _T("CURVATURE_CHANGE_RATE");			///< �ȗ��ω���
	const _TCHAR kClothoidParameter[]		= _T("CLOTHOID_PARAMETER");				///< �N���\�C�h�p�����[�^
	const _TCHAR kLongitudinalSlope[]		= _T("LONGITUDINAL_SLOPE");				///< �c�f���z
	const _TCHAR kCrossSlope[]				= _T("CROSS_SLOPE");					///< ���f���z
}

/// �����m�[�h��
namespace nq_compart_node{
	const _TCHAR kTableName[]				= _T("NQ_COMPART_NODE");				///< �e�[�u����

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< �񋟌�ID
	const _TCHAR kCompartLinkGID[]			= _T("COMPART_LINK_GID");				///< ���������NGID
	const _TCHAR kCompartNodeGID[]			= _T("COMPART_NODE_GID");				///< �����m�[�hGID
	const _TCHAR kWidth[]					= _T("WIDTH");							///< ����
}

/// ���H���E���m�[�h��
namespace nq_border_node{
	const _TCHAR kTableName[]				= _T("NQ_BORDER_NODE");					///< �e�[�u����

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< �񋟌�ID
	const _TCHAR kBorderLinkGID[]			= _T("BORDER_LINK_GID");				///< ���H���E�������NGID
	const _TCHAR kBorderNodeGID[]			= _T("BORDER_NODE_GID");				///< ���H���E���m�[�hGID
}

/// ADAM�o�[�W����
namespace adam_version{
	const _TCHAR kTableName[]				= _T("ADAM_VERSION_216");					///< �e�[�u����
}

} //namespace adam_v2

} // namespace schema

} // namespace sindy