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

/**
 *	SindyAttrdef.h : ������`
 */

#ifndef	___SINDY_ATTR_DEF_H___
#define	___SINDY_ATTR_DEF_H___

#include <string>
#include <atlbase.h>

namespace sindy
{
	namespace layer
	{
		extern std::string ROAD_LINK;
		extern std::string ROAD_NODE;
		extern std::string SEGMENT_ATTR;
		extern std::string GRAD;
		extern std::string PARKING_SHAPE;
		extern std::string ACCESS_POINT;
		extern std::string FACIL_INFO_POINT;
		extern std::string HIGHWAY_NODE;
		extern std::string HIGHWAY_TEXT;
		extern std::string TIME_NOPASSAGE;
		extern std::string TIME_ONEWAY;
		extern std::string INF_BYWAY;
		extern std::string INF_DIRGUIDE;
		extern std::string INF_GUIDE;
		extern std::string INF_LANE;
		extern std::string INF_INTERNAVI;
		extern std::string INF_ROUTE;
		extern std::string INF_VICS;
		extern std::string INF_TURNREG;
		extern std::string INF_UTURN;
		extern std::string LQ_BYWAY;
		extern std::string LQ_DIRGUIDE;
		extern std::string LQ_GUIDE;
		extern std::string LQ_LANE;
		extern std::string LQ_INTERNAVI;
		extern std::string LQ_ROUTE;
		extern std::string LQ_VICS;
		extern std::string LQ_TURNREG;
		extern std::string LQ_UTURN;
		extern std::string BASE_ADMIN;
		extern std::string BASE_MESH;
		extern std::string MIDDLE_MESH;
		extern std::string CITY_MESH;
		extern std::string ROAD_CODE_LIST;
		extern std::string DUPLI_LINK;
		extern std::string MIDDLE_ANNOTATION;
		extern std::string BASE_ANNOTATION;
		extern std::string CITY_ANNOTATION;
	};
	namespace object
	{
		extern std::string OBJECTID;
	};
	namespace feature
	{
		extern std::string SHAPE;
	};
	namespace ipc_table
	{
		extern std::string OPERATOR;
		extern std::string PURPOSE_C;
		extern std::string MODIFYDATE;
		extern std::string UPDATE_TYPE;
		extern std::string PROGMODIFYDATE;
		extern std::string MODIFYPROGNAME;
		extern std::string USERCLAIM_F;
		extern std::string SOURCE;
		extern std::string TMP_MESHCODE;
		extern std::string TMP_PRIMID;
	};
	namespace ipc_feature
	{
		using namespace ipc_table;
		using namespace feature;
	};
	namespace meshcode_cls
	{
		extern std::string MESHCODE;
	};
	namespace field_survey
	{
		extern std::string FIELDSURVEY_F;
		extern std::string WORK_YEAR;
	};
	namespace road_link
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		extern std::string TMPFROM_NODE;
		extern std::string TMPTO_NODE;
		extern std::string FROM_NODE_ID;
		extern std::string TO_NODE_ID;
		extern std::string ROADCLASS_C;
		extern std::string NAVICLASS_C;
		extern std::string MAIN_LINKCLASS_C;
		extern std::string SUB_LINKCLASS_C;
		extern std::string ADMIN_CODE;
		extern std::string ROADMANAGER_C;
		extern std::string ROADWIDTH_C;
		extern std::string WIDEAREA_C;
		extern std::string UPDOWNCLASS_C;
		extern std::string BYPASS_F;
		extern std::string CARONLY_F;
		extern std::string ISLAND_F;
		extern std::string ROAD_NO;
		extern std::string ROAD_CODE;
		extern std::string NOPASSAGE_C;
		extern std::string NOPASSAGE_RF;
		extern std::string ONEWAY_C;
		extern std::string ONEWAY_RF;
		extern std::string TRIP_SPEED;
		extern std::string LIMITSPEED_C;
		extern std::string KUBUN_C;
		extern std::string LANE_COUNT;
		extern std::string RIGHT_CHANGE;
		extern std::string LEFT_CHANGE;
		extern std::string LANEVALID_F;
		extern std::string DUPLINK_RF;
		extern std::string TURNREG_LQRF;
		extern std::string DIRGUIDE_LQRF;
		extern std::string GUIDE_LQRF;
		extern std::string LANE_LQRF;
		extern std::string VICS_LQRF;
		extern std::string BYWAY_LQRF;
		extern std::string ROUTE_LQRF;
		extern std::string UTURN_LQRF;
		extern std::string INTERNAVI_LQRF;
		extern std::string PARKINGCLASS_C;
		extern std::string PARKINGLINKCLASS_C;
		extern std::string FLOOR_LEVEL;
		extern std::string MIDDLEFLOOR_LEVEL;
		extern std::string ALLEYMAINTE_C;

		extern std::string ATTR[];

		/// ���H��ʃR�[�h
		enum roadclass_c
		{
			roadclass_kokudo		= 1,		// ��ʍ���
			roadclass_prefmlocal	= 2,		// ��v�n����(�s�{��)
			roadclass_citymlocal	= 3,		// ��v�n����(�w��s)
			roadclass_prefroad		= 4,		// ��ʓs���{����
			roadclass_cityroad		= 5,		// �w��s�̈�ʎs��
			roadclass_otherroad		= 6,		// ���̑��̓��H
			roadclass_nodisproad	= 7,		// ���̑��̓��H(�i�������̑���) --- �\���p
			roadclass_detail1		= 8,		// �׊X�HL1
			roadclass_detail2		= 9,		// �׊X�HL2
			roadclass_detail3		= 10,		// �׊X�HL3
			roadclass_highway		= 101,		// ���������ԓ��H
			roadclass_cityhighway	= 102,		// �s�s�������H
			roadclass_kokudo2		= 103,		// ��ʍ���(�L��)
			roadclass_prefmlocal2	= 104,		// ��v�n����(�s�{��)(�L��)
			roadclass_citymlocal2	= 105,		// ��v�n����(�w��s)(�L��)
			roadclass_prefroad2		= 106,		// ��ʓs���{����(�L��)
			roadclass_cityroad2		= 107,		// �w��s�̈�ʎs��(�L��)
			roadclass_otherroad2	= 108,		// ���̑��̓��H(�L��)
			roadclass_ferry			= 201,		// �t�F���[�q�H(��\����)
			roadclass_ferry_s2		= 202,		// �t�F���[�q�H(S2�j���\����)
			roadclass_ferry_s3		= 203,		// �t�F���[�q�H(S3�j���\����)
			roadclass_ferry_s4		= 204,		// �t�F���[�q�H(S4�j���\����)
			roadclass_virtual		= 301,		// ���z�����N
			roadclass_garden		= 302,		// �뉀�H
			roadclass_bridge		= 303,		// �u���b�W�����N
			roadclass_entrance		= 304,		// �{�ݏo���������N
			roadclass_facil			= 305,		// �{�ݓ������N
			roadclass_parking		= 401,		// ���ԏ�\�����H
			roadclass_match			= 402		// �}�b�`���O�p�����N(PEC�p)
		};

		/// �o�H��ʃR�[�h
		enum naviclass_c
		{
			naviclass_kokudo		= 1,		// ��ʍ���
			naviclass_prefmlocal	= 2,		// ��v�n����(�s�{��)
			naviclass_citymlocal	= 3,		// ��v�n����(�w��s)
			naviclass_prefroad		= 4,		// ��ʓs���{����
			naviclass_cityroad		= 5,		// �w��s�̈�ʎs��
			naviclass_otherroad		= 6,		// ���̑��̓��H
			naviclass_highway		= 101,		// ���������ԓ��H
			naviclass_cityhighway	= 102,		// �s�s�������H
			naviclass_kokudo2		= 103,		// ��ʍ���(�L��)
			naviclass_prefmlocal2	= 104,		// ��v�n����(�s�{��)(�L��)
			naviclass_citymlocal2	= 105,		// ��v�n����(�w��s)(�L��)
			naviclass_prefroad2		= 106,		// ��ʓs���{����(�L��)
			naviclass_cityroad2		= 107,		// �w��s�̈�ʎs��(�L��)
			naviclass_otherroad2	= 108,		// ���̑��̓��H(�L��)
			naviclass_ferry			= 201,		// �t�F���[�q�H
			naviclass_rampway		= 403,		// �ڑ�����
			naviclass_notnavi		= 0			// �o�H�ΏۊO
		};

		/// �����N���
		enum linkclass_c
		{
			linkclass_unknown		= 0,		// �������E��ƒ�
			linkclass_hibunri		= 1,		// �{��(�㉺�񕪗�)�����N
			linkclass_bunri			= 2,		// �{��(�㉺����)�����N
			linkclass_junction		= 3,		// �A���H(�{���Ԃ̓n���)
			linkclass_crossing		= 4,		// �����_�������N
			linkclass_ramp			= 5,		// �A���H(�����v)�����N
			linkclass_sidelink		= 6,		// �{���Ɠ���H���̑��������N
			linkclass_salink		= 7,		// SA�����������N
			linkclass_bicycle		= 8,		// ���]�ԓ�
			linkclass_connect		= 9			// �{�������ڑ������N
		};

		/// ���H�Ǘ���
		enum roadmanager_c
		{
			roadmanager_unknown		= 0,		// �������E��ƒ�
			roadmanager_jh			= 1,		// ���{���H���c
			roadmanager_public3		= 2,		// ���H3����
			roadmanager_public		= 3,		// ���H����
			roadmanager_national	= 4,		// ��
			roadmanager_pref		= 5,		// �s���{��
			roadmanager_city		= 6,		// �w��s
			roadmanager_town		= 7,		// ���̑��̎s����
			roadmanager_other		= 8			// ���̑��̊Ǘ���
		};

		/// ���H�����敪
		enum roadwidth_c
		{
			roadwidth_unknown		= 0,		// �������E��ƒ�
			roadwidth_over13m		= 1,		// ����13.0m�ȏ�
			roadwidth_under13m		= 2,		// ����5.5m�`13.0m����
			roadwidth_over3m		= 3,		// ����3.0m�`5.5m����
			roadwidth_under3m		= 4			// ����3.0m����
		};

		/// �L��
		enum widearea_c
		{
			widearea_kouiki0		= 0,		// �L��ł͂Ȃ�
			widearea_kouiki1		= 1,		// �L��1(��ʓ��D��)
			widearea_kouiki2		= 2			// �L��2(�L�����D��)
		};

		/// �n�C�E�F�C�H���R�[�h�i�� �㉺�������j
		enum updown_c
		{
			updownclass_nothwy		= 0,		// �ΏۊO
			updownclass_nodir		= 1,		// HWY(�㉺�Ȃ�)
			updownclass_up			= 2,		// HWY���(���E��)
			updownclass_down		= 3,		// HWY����(�O�E��)
			updownclass_common		= 4			// HWY�㉺������
		};

		/// �ʍs�֎~
		enum nopassage_c
		{
			nopassage_passable		= 0,		// �ʍs�֎~�Ȃ�
			nopassage_nocondition	= 1,		// �ʍs�֎~(�����Ȃ�)
			nopassage_withcondition	= 2,		// �ʍs�֎~(��������)
			badpassage				= 3,		// �ʍs�s��
			relation_nocondition	= 4,		// �ʍs�֎~(�֌W�҈ȊO)(�����Ȃ�)
			relation_withcondition	= 5			// �ʍs�֎~(�֌W�҈ȊO)(��������)
		};

		/// ����ʍs
		enum oneway_c
		{
			oneway_no				= 0,		// ����ʍs�Ȃ�
			oneway_order			= 1,		// ����ʍs(�������E�����Ȃ�)
			oneway_reverse			= 2,		// ����ʍs(�t�����E�����Ȃ�)
			oneway_ordwithcond		= 3,		// ����ʍs(�������E��������)
			oneway_revwithcond		= 4,		// ����ʍs(�t�����E�����Ȃ�)
			oneway_both				= 5			// ����ʍs(���t�؂�ւ�����)
		};

		/// �K�����x
		enum limitspeed_c
		{
			limitspeed_unknown		= 0,		// ������
			limitspeed_under30km	= 1,		// 30km/h����
			limitspeed_40km			= 2,		// 40km/h
			limitspeed_50km			= 3,		// 50km/h
			limitspeed_60km			= 4,		// 60km/h
			limitspeed_70km			= 5,		// 70km/h
			limitspeed_80km			= 6,		// 80km/h
			limitspeed_100km		= 7			// 100km/h
		};

		/// ��]�敪
		enum kubun_c
		{
			kubun_unknown			= 0,		// ������
			kubun_main				= 1,		// �哹�H
			kubun_sub				= 2			// �]���H
		};

		/// ���ԏ���
		enum parkingclass_c
		{
			parkingclass_unknown	= 0,		// ������
			parkingclass_solid		= 1,		// ���̒��ԏ�
			parkingclass_under		= 2,		// �n�����ԏ�
			parkingclass_both		= 3,		// ���̂��n�����ԏ�
			parkingclass_tower		= 4,		// �^���[�p�[�L���O
			parkingclass_auto		= 5,		// ������]�^���[�p�[�L���O
			parkingclass_flat		= 6			// ���ʒ��ԏ�
		};

		/// ���ԏꃊ���N���
		enum parkinglinkclass_c
		{
			parkinglinkclass_unknown	= 0,	// ������
			parkinglinkclass_enter		= 1,	// ���������N
			parkinglinkclass_exit		= 2,	// �o�������N
			parkinglinkclass_entrance	= 3,	// �o���������N
			parkinglinkclass_premises	= 4,	// �\�����H�����N
			parkinglinkclass_junction	= 5,	// ���x���Ԃ̓n�胊���N
		};

		/// �׊X�H��ƔN�x�t���O
		enum alley_mainte
		{
			alleymainte_notavailable	= 0,		// �Y���Ȃ�
			alleymainte_1998			= 1,		// 1998�N�x
			alleymainte_1999			= 2,		// 1999�N�x
			alleymainte_2000spring		= 3,		// 2000�N�x(�t)
			alleymainte_2000autumn		= 4,		// 2000�N�x(�H)
			alleymainte_2001			= 5,		// 2001�N�x
			alleymainte_2002			= 6,		// 2002�N�x
			alleymainte_2003			= 7,		// 2003�N�x
			alleymainte_2004			= 8,		// 2004�N�x
			alleymainte_2005			= 9			// 2005�N�x
		};
	};
	namespace road_node
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string NODECLASS_C;
		extern std::string SIGNAL_C;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string HEIGHT_C;
		extern std::string HEIGHT;
		extern std::string PARKING_ID;

		extern std::string ATTR[];

		/// �m�[�h���
		enum nodeclass_c
		{
			nodeclass_crossing		= 1,		// �����_�m�[�h
			nodeclass_term			= 2,		// �[�_�m�[�h
			nodeclass_attrchange	= 3,		// �����ω��_�m�[�h
			nodeclass_attrnode		= 4,		// �����t���m�[�h
			nodeclass_peclink		= 5,		// PEC�����N�ڑ��m�[�h
			nodeclass_nodearray		= 6,		// �m�[�h���_�m�[�h
			nodeclass_tollgate		= 7,		// �������m�[�h
			nodeclass_highway		= 8,		// �n�C�E�F�C�֌W�m�[�h
			nodeclass_meshedge		= 9,		// 2�����b�V�����ӏ�m�[�h
			nodeclass_notsamelink	= 10,		// ���ꃊ���NID�h�~�m�[�h
			nodeclass_pecnode		= 11,		// PEC�m�[�h
			nodeclass_pecnode2		= 12,		// 2�����b�V�����ӏ�PEC�m�[�h
			nodeclass_dummyfacil	= 13		// �_�~�[�{�݃m�[�h
		};

		/// �M���@�L��
		enum signal_c
		{
			signal_no				= 0,		// �M���@�Ȃ�
			signal_exist			= 1,		// �M���@����
			signal_unknown			= 2			// ������
		};

		/// �����t���O
		enum height_c
		{
			height_no				= 0,		// ���΍����Ȃ�
			height_valid			= 1,		// ���΍����L��
			height_auto				= 2			// ���΍��������t�^
		};
	};
	namespace link_info
	{
		extern std::string LINK_ID;
	};
	namespace segment_attr
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		using namespace link_info;
		extern std::string LINKATTR_C;
		extern std::string UNDER_OVER;

		extern std::string ATTR[];

		/// �����N�������R�[�h
		enum linkattr_c
		{
			linkattr_tunnel		= 2,		// �g���l��
			linkattr_underpass	= 5,		// �A���_�[�p�X
			linkattr_bridge		= 1,		// ���E����
			linkattr_crossing	= 4,		// ����
			linkattr_doumon		= 3,		// ���哙
			linkattr_underover	= 6,		// �㉺�֌W
			linkattr_dirtroad	= 7			// ���ܑ�
		};
	};
	namespace grad
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		using namespace link_info;
		extern std::string GRAD_VALUE;

		extern std::string ATTR[];
	};
	namespace parking_shape
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		extern std::string PARKING_ORGID;
		extern std::string PARKINGCLASS_C;
		extern std::string NAME_KANJI;

		extern std::string ATTR[];

		/// ���ԏ���
		enum parkingclass_c
		{
			parkingclass_unknown	= 0,		// ������
			parkingclass_solid		= 1,		// ���̒��ԏ�
			parkingclass_under		= 2,		// �n�����ԏ�
			parkingclass_both		= 3,		// ���̂��n�����ԏ�
			parkingclass_tower		= 4,		// �^���[�p�[�L���O
			parkingclass_auto		= 5,		// ������]�^���[�p�[�L���O
			parkingclass_flat		= 6			// ���ʒ��ԏ�
		};
	};
	namespace hwy_facil_code
	{
		extern std::string ROAD_CODE;
		extern std::string ROAD_SEQ;
	};
	namespace hwy_mode
	{
		extern std::string HWYMODE_F;
	};
	namespace inout
	{
		extern std::string INOUT_C;
		/// �o�����R�[�h
		enum inout_c
		{
			inout_no			= 0,		// ��ʂȂ�
			inout_enter			= 1,		// �����E����
			inout_exit			= 2			// �o���E����
		};
	};
	namespace direction
	{
		extern std::string DIRECTION_C;

		/// �����R�[�h
		enum direction_c
		{
			direction_no		= 0,		// �����Ȃ�
			direction_up		= 1,		// ���E�����E���s��
			direction_down		= 2,		// ����E�O���E���s��
			direction_common	= 3			// �o��������
		};
	};
	namespace access_point
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		using namespace hwy_mode;
		using namespace direction;
		using namespace inout;
		extern std::string DIRECTION_KANJI;
		extern std::string DIRECTION_YOMI;
		extern std::string GS_C;
		extern std::string TOILET_F;
		extern std::string HANDICAP_F;
		extern std::string RESTAURANT_F;
		extern std::string CAFETERIA_F;
		extern std::string SOUVENIR_F;
		extern std::string RESTING_F;
		extern std::string NAP_F;
		extern std::string INFODESK_F;
		extern std::string INFORMATION_F;
		extern std::string SHOWER_F;
		extern std::string LAUNDRY_F;
		extern std::string SPA_F;
		extern std::string FAX_F;
		extern std::string MAILBOX_F;
		extern std::string CASHSERVICE_F;
		extern std::string HWYOASIS_F;
		extern std::string CARWASH_F;

		extern std::string ATTR[];
		extern std::string SAPAINFO[];
		extern std::string NEWSAPAINFO[];

		/// �K�\�����X�^���h
		enum gs_c
		{
			gs_no				= 0,		// �K�\�����X�^���h�Ȃ�
			gs_other			= 1,		// ���̑��E����
			gs_esso				= 2,		// �G�b�\
			gs_mobil			= 3,		// ���[�r��
			gs_kygnus			= 4,		// �L�O�i�X
			gs_cosmo			= 5,		// �R�X��
			gs_shell			= 6,		// �V�F��
			gs_jomo				= 7,		// �W����
			gs_general			= 8,		// �[�l����
			gs_kyushu			= 9,		// ��B
			gs_idemitsu			= 10,		// �o��
			gs_taiyo			= 11,		// ���z
			gs_eneos			= 12		// ENEOS
		};
	};
	namespace facil_info_point
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string DUMMYNAME_F;
		extern std::string FACILCLASS_C;
		extern std::string AUTOPOSITION_F;

		extern std::string ATTR[];

		/// �{�ݎ��
		enum facilclass_c
		{
			facilclass_sa		= 1,		// SA
			facilclass_pa		= 2,		// PA
			facilclass_jct		= 3,		// JCT
			facilclass_ramp		= 4,		// �����v
			facilclass_ic		= 5,		// IC
			facilclass_tollgate	= 6			// ������
		};
	};
	namespace highway_node
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		using namespace hwy_mode;
		using namespace inout;
		using namespace direction;
		extern std::string TOLLCLASS_C;
		extern std::string TOLLFUNC_C;
		extern std::string DUMMYTOLL_F;
		extern std::string GUIDE_F;
		extern std::string ROAD_SEQ2;
		extern std::string APPROACHNODE_ID;
		extern std::string FLOWNODE_ID;

		extern std::string ATTR[];

		/// ���������
		enum tollclass_c
		{
			tollclass_other		= 0,		// �������ȊO�i���򍇗��m�[�h�j
			tollclass_honsen	= 1,		// �{���㗿����
			tollclass_ramp		= 2,		// �����v�㗿����
			tollclass_barrier	= 3,		// �o���A
			tollclass_inout		= 4			// �o�����m�[�h		/////// �o�����m�[�h�Ή��ǉ�
		};

		/// �������@�\
		enum tollfunc_c
		{
			tollfunc_no			= 0,		// �@�\�Ȃ�
			tollfunc_issue		= 1,		// �����@�\
			tollfunc_pay		= 2,		// ���Z�@�\
			tollfunc_single		= 3,		// �P�Ɨ�����
			tollfunc_singleissue	= 4,	// �P�Ɨ������{�����@�\
			tollfunc_singlepay	= 5,		// �P�Ɨ������{���Z�@�\
			tollfunc_check		= 6,		// ���D�@�\
			tollfunc_issue2		= 7			// �P�Ɨ������������@�\
		};
	};
	namespace highway_text
	{
		using namespace object;
		using namespace ipc_table;
		using namespace hwy_facil_code;
		using namespace inout;
		extern std::string POINT_ID;
		extern std::string ORDER_SEQ;
		extern std::string UPDOWN_C;
		extern std::string NAME_KANJI;
		extern std::string PREV_ID;
		extern std::string NEXT_ID;
		extern std::string PARENT_ID;
		extern std::string FORE_COUNT;
		extern std::string FORE1_ID;
		extern std::string FORE2_ID;
		extern std::string FORE3_ID;
		extern std::string FORE4_ID;
		extern std::string BACK_COUNT;
		extern std::string BACK1_ID;
		extern std::string BACK2_ID;
		extern std::string BACK3_ID;
		extern std::string BACK4_ID;

		extern std::string ATTR[];
		extern std::string FOREID[];
		extern std::string BACKID[];

		/// �㉺������
		enum updown_c
		{
			updown_up		= 1,		// ���E�����E���s��
			updown_down		= 2			// ����E�O���E���s��
		};
	};
	namespace base_admin
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string CITYCODE;

		extern std::string ATTR[];
	};
	namespace base_mesh
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace meshcode_cls;

		extern std::string ATTR[];
	};
	namespace middle_mesh
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string MIDDLEMESHCODE;
	};
	namespace city_mesh
	{
		using namespace object;
		extern std::string CITYMESHCODE;
		extern std::string CITYNAME;
		extern std::string CREATE_YEAR;
		extern std::string UPDATE_YEAR;
		extern std::string COMPLETE_F;
		extern std::string SEA_F;
		extern std::string ZUKA_F;
		extern std::string RASTER;
		extern std::string CREATE_RASTER_YEAR;
		extern std::string CURRENT_ADJ_YEAR;
		extern std::string PHOTO_SHOT_DATE;
		extern std::string FIELD_RESEARCH_YEAR;
		extern std::string SHA_BGWORK_YEAR;
		extern std::string SHA_ANNOWORK_YEAR;
		extern std::string CREATE_ANNO_MS_DATE;
		extern std::string ADDR_MAINTE_YEAR;
		extern std::string JMAP_PUBLICATION_YEAR;
		extern std::string MAPCUBE_WORK_YEAR;
		extern std::string PDM_USE_YEAR;
	};
	namespace dupli_link
	{
		using namespace object;
		using namespace ipc_table;
		using namespace meshcode_cls;
		using namespace link_info;
		extern std::string KUBUN_C;
		extern std::string DUPCLASS_C;
		extern std::string ROAD_NO;
		extern std::string ROAD_CODE;

		extern std::string ATTR[];

		/// ��]�敪
		enum kubun_c
		{
			kubun_unknown			= 0,		// ������
			kubun_main				= 1,		// �哹�H
			kubun_sub				= 2			// �]���H
		};

		/// �H��ID�����H�ԍ�
		enum dupclass_c
		{
			dupclass_kokudo			= 1,		// �����ԍ�
			dupclass_pref			= 2,		// �s���{�����ԍ�
			dupclass_nameid			= 3			// �H������ID
		};
	};

	namespace linkdir
	{
		extern std::string LINKDIR_C;
		enum linkdir_c
		{
			linkdir_fore		= 1,		// ������
			linkdir_back		= 2			// �t����
		};
	};

	namespace link_queue
	{
		using namespace object;
		using namespace ipc_table;
		using namespace meshcode_cls;
		extern std::string TMP_LINK_ID;
		using namespace link_info;
		using namespace linkdir;
		extern std::string SEQUENCE;
		extern std::string LASTLINK_F;
		extern std::string INF_ID;
	};
	namespace lq_byway
	{
		using namespace link_queue;
	};
	namespace lq_route
	{
		using namespace link_queue;
	};
	namespace lq_guide
	{
		using namespace link_queue;
	};
	namespace lq_dirguide
	{
		using namespace link_queue;
	};
	namespace lq_lane
	{
		using namespace link_queue;
	};
	namespace lq_turnreg
	{
		using namespace link_queue;
	};
	namespace lq_uturn
	{
		using namespace link_queue;
	};
	namespace lq_vics
	{
		using namespace link_queue;
	};
	namespace lq_internavi
	{
		using namespace link_queue;
	};
	namespace inf_byway
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string BYWAY_F;
		extern std::string BACKROAD_F;
		extern std::string SOURCE_NAME;

		extern std::string ATTR[];
	};
	namespace inf_route
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string ROUTECLASS_C;
		extern std::string SOURCE_NAME;

		extern std::string ATTR[];

		/// ���[�g�N���X
		enum routeclass_c
		{
			routeclass_access		= 1,		// �A�N�Z�X��
			routeclass_connectway	= 2,		// ���p����
			routeclass_tourguide	= 3			// �ό��ē�
		};
	};
	namespace inf_dirguide
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string GUIDECLASS1_C;
		extern std::string NAME_KANJI1;
		extern std::string NAME_YOMI1;
		extern std::string GUIDECLASS2_C;
		extern std::string NAME_KANJI2;
		extern std::string NAME_YOMI2;
		extern std::string GUIDECLASS3_C;
		extern std::string NAME_KANJI3;
		extern std::string NAME_YOMI3;
		extern std::string GUIDECLASS4_C;
		extern std::string NAME_KANJI4;
		extern std::string NAME_YOMI4;
		extern std::string GUIDECLASS5_C;
		extern std::string NAME_KANJI5;
		extern std::string NAME_YOMI5;
		extern std::string GUIDECLASS6_C;
		extern std::string NAME_KANJI6;
		extern std::string NAME_YOMI6;
		extern std::string GUIDECLASS7_C;
		extern std::string NAME_KANJI7;
		extern std::string NAME_YOMI7;
		extern std::string GUIDECLASS8_C;
		extern std::string NAME_KANJI8;
		extern std::string NAME_YOMI8;
		extern std::string GUIDECLASS9_C;
		extern std::string NAME_KANJI9;
		extern std::string NAME_YOMI9;
		extern std::string GUIDECLASS10_C;
		extern std::string NAME_KANJI10;
		extern std::string NAME_YOMI10;

		extern std::string ATTR[];

		/// �n����
		enum guideclass_c
		{
			guideclass_normal		= 1,		// �n��(���)
			guideclass_toll			= 2,		// �n��(�L��)
			guideclass_kokudo		= 3,		// �����ԍ�
			guideclass_pref			= 4,		// �s���{�����ԍ�
			guideclass_other		= 5,		// ���̑�
			guideclass_tstring		= 12,		// �L��(������)
			guideclass_tkokudo		= 13,		// �L��(�����ԍ�)
			guideclass_tpref		= 14		// �L��(�����ԍ�)
		};
	};
	namespace inf_guide
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string GUIDETYPE_C;

		extern std::string ATTR[];

		/// �����U���R�[�h
		enum guidetype_c
		{
			guidetype_unknown		= 0,		// ���ݒ�
			guidetype_noguide		= 1,		// ���Ȃ�(�ē��Ȃ�)
			guidetype_straight		= 2,		// ���i(12������)
			guidetype_rightfront	= 3,		// �E�O����(1������)
			guidetype_rightahead	= 4,		// �E��O����(2������)
			guidetype_right			= 5,		// �E��(3������)
			guidetype_rightbehind	= 6,		// �E�΂ߌ�����(4������)
			guidetype_rightback		= 7,		// �E�߂����(5������)
			guidetype_back			= 8,		// �߂����(6������)
			guidetype_leftback		= 9,		// ���߂����(7������)
			guidetype_leftbehind	= 10,		// ���΂ߌ�����(8������)
			guidetype_left			= 11,		// ����(9������)
			guidetype_leftahead		= 12,		// ����O����(10������)
			guidetype_leftfront		= 13		// ���O����(11������)
		};
	};
	namespace inf_lane
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string LANEDIR_C;
		extern std::string APPROACH_LANE;
		extern std::string RIGHT_CHANGE;
		extern std::string LEFT_CHANGE;
		extern std::string FLOW_LANE;
		extern std::string LANE_PASSABLE;

		extern std::string ATTR[];

		/// ���[�������R�[�h
		enum lanedir_c
		{
			lanedir_front		= 1,		// �O����(0)
			lanedir_rightfront	= 2,		// �E�΂ߑO����(45)
			lanedir_right		= 3,		// �E����(90)
			lanedir_rightback	= 4,		// �E�΂ߌ�����(135)
			lanedir_back		= 5,		// ������(180)
			lanedir_leftback	= 6,		// ���΂ߌ�����(225)
			lanedir_left		= 7,		// ������(270)
			lanedir_leftfront	= 8			// ���΂ߑO����(315)
		};

		/// ���[���ʍs�R�[�h
		enum lane_passable
		{
			lanepass_nopassage	= 0,		// �ʍs�s��
			lanepass_passable	= 1,		// �ʍs��
			lanepass_passage2	= 2			// ��ʎԗ��ʍs�s��
		};
	};
	namespace inf_vics
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string VICS_MESHCODE;
		extern std::string VICSCLASS_C;
		extern std::string VICS_LINK_ID;

		extern std::string ATTR[];

		/// VICS�敪
		enum vicsclass_c
		{
			vicsclass_highway		= 1,		// �������H
			vicsclass_cityhighway	= 2,		// �s�s�������H(�܎w��s�s�������H)
			vicsclass_street		= 3,		// ��ʓ��H
			vicsclass_other			= 4			// ���̑�
		};
	};
	namespace inf_internavi
	{
		using namespace inf_vics;
	};
	namespace dayofweek
	{
		/// �j���w��
		enum dayofweek_c
		{
			dayofweek_holiday			= 1,		// ���E�x��
			dayofweek_except_holiday	= 2,		// ���E�x��������
			dayofweek_pubholiday		= 3,		// �x��(���j�͏���)
			dayofweek_except_pubholiday	= 4,		// �x��������
			dayofweek_weekday			= 5,		// ����
			dayofweek_sunday			= 6,		// ���j��
			dayofweek_monday			= 7,		// ���j��
			dayofweek_tuesday			= 8,		// �Ηj��
			dayofweek_wednesday			= 9,		// ���j��
			dayofweek_thursday			= 10,		// �ؗj��
			dayofweek_friday			= 11,		// ���j��
			dayofweek_saturday			= 12,		// �y�j��
			dayofweek_weekend			= 13,		// �y���E�x��
			dayofweek_except_weekend	= 14,		// �y���E�x��������
			dayofweek_allday			= 15,		// �S�j��
			dayofweek_except_sunday		= 16,		// ���j��������
			dayofweek_pre_holiday		= 17		// ���E�x���̑O��
		};
	};
	namespace inf_turnreg
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		using namespace dayofweek;
		extern std::string START_MONTH1;
		extern std::string START_DAY1;
		extern std::string START_HOUR1;
		extern std::string START_MIN1;
		extern std::string END_MONTH1;
		extern std::string END_DAY1;
		extern std::string END_HOUR1;
		extern std::string END_MIN1;
		extern std::string DAYOFWEEK1_C;
		extern std::string START_MONTH2;
		extern std::string START_DAY2;
		extern std::string START_HOUR2;
		extern std::string START_MIN2;
		extern std::string END_MONTH2;
		extern std::string END_DAY2;
		extern std::string END_HOUR2;
		extern std::string END_MIN2;
		extern std::string DAYOFWEEK2_C;
		extern std::string START_MONTH3;
		extern std::string START_DAY3;
		extern std::string START_HOUR3;
		extern std::string START_MIN3;
		extern std::string END_MONTH3;
		extern std::string END_DAY3;
		extern std::string END_HOUR3;
		extern std::string END_MIN3;
		extern std::string DAYOFWEEK3_C;
		extern std::string START_MONTH4;
		extern std::string START_DAY4;
		extern std::string START_HOUR4;
		extern std::string START_MIN4;
		extern std::string END_MONTH4;
		extern std::string END_DAY4;
		extern std::string END_HOUR4;
		extern std::string END_MIN4;
		extern std::string DAYOFWEEK4_C;

		extern std::string ATTR[];
	};
	namespace inf_uturn
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string SOURCE_NAME;

//		extern std::string ATTR[];
	};
	namespace road_code_list
	{
		using namespace object;
		using namespace ipc_table;
		extern std::string ROAD_CODE;
		extern std::string LINECLASS_C;
		extern std::string LINEDIRTYPE_C;
		extern std::string LANDCLASS_C;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string WORK_KANJI;
		extern std::string WORK_YOMI;

		extern std::string ATTR[];

		/// �H����ʃR�[�h
		enum lineclass_c
		{
			lineclass_highway		= 1,		// �������H
			lineclass_tollroad		= 2,		// �L�����H
			lineclass_metropolis	= 3,		// ��s�������H
			lineclass_hanshin		= 4,		// ��_�������H
			lineclass_nagoya		= 5,		// ���É��s�s�������H
			lineclass_fukuoka		= 6,		// �����s�s�������H
			lineclass_kitakyushu	= 7,		// �k��B�s�s�������H
			lineclass_hiroshima		= 8,		// �L���������H
			lineclass_ferry			= 9,		// �t�F���[�q�H
			lineclass_street		= 10,		// ��ʊX������
			lineclass_tfspecial		= 11		// TF�����Ή��H��
		};

		/// �H�������^�C�v
		enum linedirtype_c
		{
			linedirtype_none		= 0,		// �^�C�v�Ȃ�
			linedirtype_updown		= 1,		// ���E����
			linedirtype_inout		= 2,		// �����E�O���
			linedirtype_eastwest	= 3,		// ���s���E���s��
			linedirtype_other		= 4			// ���̑�
		};

		/// �n����
		enum landclass_c
		{
			landclass_unknown		= 0,		// ������
			landclass_hokkaido		= 1,		// �k�C��
			landclass_honshu		= 2,		// �{�B
			landclass_shikoku		= 3,		// �l��
			landclass_kyushu		= 4,		// ��B
			landclass_okinawa		= 5,		// ����
			landclass_detached		= 6			// �Ɨ��H��
		};
	};
	namespace link_rest
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		using namespace link_info;
		extern std::string MESHCODE;
		extern std::string START_MONTH;
		extern std::string START_DAY;
		extern std::string START_HOUR;
		extern std::string START_MIN;
		extern std::string END_MONTH;
		extern std::string END_DAY;
		extern std::string END_HOUR;
		extern std::string END_MIN;
		extern std::string DAYOFWEEK_C;
		using namespace dayofweek;

		extern std::string ATTR[];
	};
	namespace time_nopassage
	{
		using namespace link_rest;
	};
	namespace time_oneway
	{
		using namespace link_rest;
		using namespace linkdir;
	};

	namespace scale_disptype
	{
		enum scdisptype_c
		{
			scdisptype_nodisp	= 0,	// �\���Ȃ�
			scdisptype_stronly	= 1,	// ������̂�
			scdisptype_markstr	= 2,	// �L���{���L
			scdisptype_pntstr	= 3,	// �w���_�{���L
			scdisptype_markonly	= 4		// �L���̂�
		};
	};

	namespace annotation
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace scale_disptype;
		extern std::string NAME_STRING1;
		extern std::string NAME_STRING2;
		extern std::string NAME_STRING3;
		extern std::string STRING_NUM1;
		extern std::string STRING_NUM2;
		extern std::string STRING_NUM3;
		extern std::string ANNOCLASS_C;
		extern std::string TMP_ANNO_CLASS;
	};

	namespace middle_annotation
	{
		using namespace annotation;
		extern std::string SC1DISPTYPE_C;
		extern std::string SC2DISPTYPE_C;
		extern std::string SC3DISPTYPE_C;
		extern std::string SC4DISPTYPE_C;
	};

	namespace base_annotation
	{
		using namespace annotation;
		extern std::string SC1DISPTYPE_C;
		extern std::string SC2DISPTYPE_C;
		extern std::string SC3DISPTYPE_C;
		extern std::string SC4DISPTYPE_C;
	};

	namespace city_annotation
	{
		using namespace annotation;
		extern std::string SCDISPTYPE_C;
	};
};

#endif	//___SINDY_ATTR_DEF_H___
