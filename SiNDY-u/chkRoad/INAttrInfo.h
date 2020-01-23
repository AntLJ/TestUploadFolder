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

namespace in
{
	namespace road
	{
		namespace authenticity
		{
			enum code
			{
				valid			= 1,
				invalid			= 0
			};
		};

		// ���H�Ǘ���
		namespace manager
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				jh				= 1,					///< �������H���
		//		public3			= 2,					///< ���H���c3��(�p�~)
				other_public	= 3,					///< ���H����
				country			= 4,					///< ��
				pref			= 5,					///< �s���{��
				city			= 6,					///< �w��s
				other_city		= 7,					///< ���̎s����(�ܓ���23��)
				others			= 8						///< ���̑��̊Ǘ���
			};
		};

		// �����N���
		namespace link_class
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				hibunri_honsen	= 1,					///< �{��(�㉺�񕪗�)
				bunri_honsen	= 2,					///< �{��(�㉺����)
				junction		= 3,					///< �A���H(�{���Ԃ̓n���)
				crossing		= 4,					///< �����_�������N
				ramp			= 5,					///< �A���H(�����v)�����N
				side			= 6,					///< �{���Ɠ���H���̑��������N
				sa				= 7,					///< SA�����������N
				bicycle			= 8,					///< ���]�ԓ���
				connection		= 9,					///< �{�������ڑ������N
				roundabout		= 10					///< ������_�����N
			};
		};

		// �o�C�p�X�t���O
		namespace bypass
		{
			using namespace authenticity;
		};

		// �����Ԑ�p���H�t���O
		namespace caronly
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				caronly			= 1,					///< �����Ԑ�p���H�ɊY��
				not_caronly		= 2						///< �����Ԑ�p���H�ɔ�Y��
			};
		};

		// ���H����
		namespace width
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				over_13m		= 1,					///< ����13.0m�ȏ�
				under_13m		= 2,					///< ����5.5m�`13.0m����
				over_3m			= 3,					///< ����3.0m�`5.5m����
				under_3m		= 4						///< ����3.0m����
			};
		};

		// �Ԑ���
		namespace lane
		{
			enum code 
			{
				unknown			= 0,					///< �������E��ƒ�
				one				= 1,					///< 1�Ԑ�
				two				= 2,					///< 2�Ԑ�
				three			= 3,					///< 3�Ԑ�
				four			= 4,					///< 4�Ԑ�
				five			= 5,					///< 5�Ԑ�
				six				= 6,					///< 6�Ԑ�
				seven			= 7,					///< 7�Ԑ�
				eight			= 8,					///< 8�Ԑ�
				nine			= 9,					///< 9�Ԑ�
				ten				= 10,					///< 10�Ԑ�
				eleven			= 11,					///< 11�Ԑ�
				twelve			= 12,					///< 12�Ԑ�
				thirteen		= 13,					///< 13�Ԑ�
				forteen			= 14,					///< 14�Ԑ�
				fifteen			= 15					///< 15�Ԑ�
			};
		};

		// �㉺�֌W
		namespace updown
		{
			enum code
			{
				up1				= 1,					///< ��E���s
				up2				= 2,					///< ��E�c���ł���
				down1			= 3,					///< ���E���s
				down2			= 4,					///< ���E�ւ���ł���
				unknown			= 0						///< ������
			};
		};

		// ��ʋK����ʃR�[�h
		namespace rest_class
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				no_rest			= 1,					///< �K���Ȃ�
				no_passage		= 2,					///< �ʍs�֎~(�����Ȃ�)
				no_passage2		= 3,					///< �ʍs�֎~(��������)
				oneway_order	= 4,					///< ����ʍs(�������E�����Ȃ�)
				oneway_reverse	= 5,					///< ����ʍs(�t�����E�����Ȃ�)
				oneway_order2	= 6,					///< ����ʍs(�������E��������)
				oneway_reverse2	= 7,					///< ����ʍs(�t�����E��������)
				oneway_both		= 8						///< ����ʍs(���t�؂�ւ�����)
			};
		};

		// �\����ʃR�[�h
		namespace road_class
		{
			enum code
			{
				highway2		= 0x05001000,			///< �s�s�ԍ������H
				highway			= 0x05001100,			///< �s�s�ԍ������H(�L��)
				city_highway	= 0x05002100,			///< �s�s�������H(�L��)
				kokudo			= 0x05003000,			///< ��ʍ���
				kokudo2			= 0x05003100,			///< ��ʍ���(�L��)
				pref_mlocal		= 0x05004000,			///< ��v�n����(�s�{��)
				pref_mlocal2	= 0x05004100,			///< ��v�n����(�s�{��)(�L��)
				city_mlocal		= 0x05005000,			///< ��v�n����(�w��s)
				city_mlocal2	= 0x05005100,			///< ��v�n����(�w��s)(�L��)
				pref_road		= 0x05006000,			///< ��ʓs���{����
				pref_road2		= 0x05006100,			///< ��ʓs���{����(�L��)
				city_road		= 0x05007000,			///< �w��s�̈�ʎs��
				city_road2		= 0x05007100,			///< �w��s�̈�ʎs��(�L��)
				other_road		= 0x05009000,			///< ���̑��̓��H
				other_road2		= 0x05009100,			///< ���̑��̓��H(�L��)
				other_road3		= 0x05008000,			///< �i�������̑���
				detail_road		= 0x05009010,			///< �׊X�H1
				detail_road2	= 0x05009020,			///< �׊X�H2
				detail_road3	= 0x05009030,			///< �׊X�H3
				dummy_link		= 0x0500d000,			///< �_�~�[�����N
				dummy_link2		= 0x0500d010,			///< �뉀�H
				dummy_link3		= 0x0500d020,			///< �}�b�`���O�p�����N
				dummy_link4		= 0x0500d030,			///< PEC�p�}�b�`���O�����N
				dummy_link5		= 0x0500d100,			///< BT�p�}�b�`���O�����N
				ferry			= 0x0500f100,			///< �t�F���[�q�H
				parking			= 0x0500e100,			///< ���ԏ�����H
				parking_shape	= 0x0500e000			///< ���ԏ�O�`
			};
		};

		// �o�H��ʃR�[�h
		namespace navi_class
		{
			enum code
			{
				highway			= 0x05001100,			///< �s�s�ԍ������H
				city_highway	= 0x05002100,			///< �s�s�������H
				kokudo			= 0x05003000,			///< ��ʍ���
				kokudo2			= 0x05003100,			///< ��ʍ���(�L��)
				pref_mlocal		= 0x05004000,			///< ��v�n����(�s�{��)
				pref_mlocal2	= 0x05004100,			///< ��v�n����(�s�{��)(�L��)
				city_mlocal		= 0x05005000,			///< ��v�n����(�w��s)
				city_mlocal2	= 0x05005100,			///< ��v�n����(�w��s)(�L��)
				pref_road		= 0x05006000,			///< ��ʓs���{����
				pref_road2		= 0x05006100,			///< ��ʓs���{����(�L��)
				city_road		= 0x05007000,			///< �w��s�̈�ʎs��
				city_road2		= 0x05007100,			///< �w��s�̈�ʎs��(�L��)
				other_road		= 0x05009000,			///< ���̑��̓��H
				other_road2		= 0x05009100,			///< ���̑��̓��H(�L��)
				other_road3		= 0x05008000,			///< �i�������̑���
				detail_road		= 0x05009010,			///< �׊X�H1
				detail_road2	= 0x05009020,			///< �׊X�H2
				detail_road3	= 0x05009030,			///< �׊X�H3
				dummy_link3		= 0x0500d020,			///< �}�b�`���O�p�����N
				dummy_link4		= 0x0500d030,			///< PEC�p�}�b�`���O�����N
				dummy_link5		= 0x0500d100,			///< BT�p�}�b�`���O�����N
				ferry			= 0x0500f100,			///< �t�F���[�q�H
				parking			= 0x0500e100,			///< ���ԏ�����H
				connectway		= 0x0500c000,			///< �ڑ����H
				kkd_route		= 0x0500b000			///< KKD�o�H�U���s�K���
			};
		};

		// �L��t���O
		namespace wide_area
		{
			enum code
			{
#if IN_VERSION >= 590
				not_wide_area	= 0x00000000,			///< �L��ɑ��݂��Ȃ�
				wide_area		= 0x00000001,			///< �L��ɑ��݂���
#else
				not_wide_area	= 0x00000000,			///< �L��ɑ��݂��Ȃ�
				wide_area1		= 0x00000001,			///< �L��1
				wide_area2		= 0x00000002			///< �L��2
#endif
			};
		};

		// ���ԏ���
		namespace parking_class
		{
			enum code
			{
				unknown			= 0,					///< �s��
				solid			= 1,					///< ���̒��ԏ�
				under			= 2,					///< �n�����ԏ�
				both			= 3,					///< ���̂��n�����ԏ�
				tower			= 5,					///< �^���[�p�[�L���O
				auto_tower		= 11,					///< ������]�^���[�p�[�L���O
				flat			= 15					///< ���ʒ��ԏ�
			};
		};

		// ���ԏꃊ���N���
		namespace parkinglink_class
		{
			enum code
			{
				unknown			= 0,					///< �������E��ƒ�
				enter			= 1,					///< ���������N
				exit			= 2,					///< �o�������N
				entrance		= 3,					///< �o���������N
				premises		= 4,					///< �\�����H�����N
				junction		= 5						///< ���x���Ԃ̓n�胊���N
			};
		};

		// VICS�敪
		namespace vics_class
		{
			enum code
			{
				highway			= 1,					///< �������H
				city_highway	= 2,					///< �s�s�������H
				street			= 3,					///< ��ʓ��H
				others			= 4						///< ���̑�
			};
		};

		// �����N�`��_�����i�X�΃t���O�j
		namespace grad
		{
			enum code
			{
				unknown			= 0,					///< ������
				up				= 1,					///< ���
				down			= 2,					///< ����
				flat			= 3						///< ���R
			};
		};

		// �����N�`��_�����i�����N�������j
		namespace linkpoint_attr
		{
			enum code
			{
				deformseg			= 0x00000080,		///< �f�t�H�����Z�O�����g
#if IN_VERSION >= 550
				dirtroad			= 0x00000020,		///< ���ܑ�
#else
				tollgate			= 0x00000040,		///< ������
				walkbridge			= 0x00000020,		///< ������
#endif
				underpass			= 0x00000010,		///< �A���_�[�p�X
				crossing			= 0x00000008,		///< ����
				doumon				= 0x00000004,		///< ���哙
				tunnel				= 0x00000002,		///< �g���l��
				bridge				= 0x00000001		///< ���E����
			};

		};

		// �m�[�h���
		namespace node_class
		{
			enum code
			{
				unknown				= 0x01000000,		///< �������E��ƒ�
				crossing			= 0x01000001,		///< �����_�m�[�h
				terminate			= 0x01000002,		///< �s���~�܂�_�m�[�h
				dummy				= 0x01000003,		///< �_�~�[�m�[�h
				edge				= 0x01000004,		///< ���ύX�_�m�[�h
				attr_change			= 0x01000005		///< �����ω��_�m�[�h
			};
		};

		// �M���@�t���O
		namespace signal
		{
			using namespace authenticity;
		};

		// ���H�����L���E�����t���O
		namespace height
		{
			using namespace authenticity;
		};

		// ��ʋK�������R�[�h
		namespace rest_cond
		{
			enum code
			{
				unknown					= 0,			///< �������E��ƒ�
				car						= 1,			///< �Ԏ�̂�
				time					= 2,			///< �����̂�
				day						= 3,			///< �j���̂�
				car_and_time			= 4,			///< �Ԏ�y�ю���
				car_and_day				= 5,			///< �Ԏ�y�їj��
				time_and_day			= 6,			///< �����y�їj��
				all						= 7,			///< �Ԏ�A�����y�їj��
				others					= 8				///< ���̑��̏���
			};
		};

		// �m�[�h�K�����
		namespace turn_class
		{
			enum code
			{
				turn_class				= 0x01000000	///< �m�[�h�K�����
			};
		}

		// �j���w��R�[�h
		namespace dayofweek
		{
			enum code
			{
				unknown					= 0,			///< �������E��ƒ�
				holiday					= 1,			///< ���E�x��
				except_holiday			= 2,			///< ���E�x��������
				public_holiday			= 3,			///< �x��(���j�͏���)
				except_public_holiday	= 4,			///< �x��������
				weekday					= 5,			///< ����
				sunday					= 6,			///< ���j
				monday					= 7,			///< ���j
				tuesday					= 8,			///< �Ηj
				wednesday				= 9,			///< ���j
				thursday				= 10,			///< �ؗj
				friday					= 11,			///< ���j
				saturday				= 12,			///< �y�j
				weekend					= 13,			///< �y���E�x��
				except_weekend			= 14,			///< �y���E�x��������
				allday					= 15,			///< �S�j��
				except_sunday			= 16,			///< ���j��������
				pre_holiday				= 17			///< ���E�x���̑O��
			};
		};

		// �����_���̎�ʃR�[�h
		namespace crossname_class
		{
			enum code
			{
				crossname				= 0x20015000
			};
		};

		// �K�����
		namespace regulation_class
		{
			enum code
			{
				unknown					= 0,			///< �������E��ƒ�
				link					= 1,			///< �����N�K��
				node					= 2,			///< �m�[�h�K��
				node_array				= 3				///< �m�[�h��K��
			};
		};

		// �K������
		namespace regulation_cond
		{
			enum code
			{
				unknown					= 0,			///< ������
				no_condition			= 1,			///< �ʏ�K��(�����Ȃ�)
				with_condition			= 2				///< �����t���K��
			};
		};

		// �ʍs�K��
		namespace passage_regulation
		{
			enum code
			{
				no_regulation			= 1,			///< �K���Ȃ�
				no_passage				= 2,			///< �ʍs�֎~
				oneway_order			= 4,			///< ����ʍs(������)
				oneway_reverse			= 5,			///< ����ʍs(�t����)
				bad_passage				= 6,			///< �ʍs�s��
				relation_passage		= 7,			///< �ʍs�֎~(�֌W�҈ȊO)
				etc_only				= 8				///< ETC��p
			};
		};

		// �m�[�h����ʃR�[�h
		namespace node_info_class
		{
			enum code
			{
				crossname_nodir			= 0x20015001,	///< �����_����(�������Ȃ�)
				crossname_dir			= 0x20015002,	///< �����_����(����������)
				highway_ic_entrance		= 0x01000010,	///< �������HIC�����m�[�h(��ʓ�����̓��H�m�[�h)
				highway_ic_enter		= 0x01000011,	///< �������HIC���H
				highway_ic_exit			= 0x01000012,	///< �������HIC�o�H
				jct_enter				= 0x01000041,	///< ���HJCT�m�[�h
				jct_exit				= 0x01000042,	///< �o�HJCT�m�[�h
				sapa_enter				= 0x01000051,	///< SAPA���H
				sapa_exit				= 0x01000052,	///< SAPA�o�H
				highway_dirguide		= 0x01000060,	///< ���ʈē�(�������H��)
				general_dirguide		= 0x01000070,	///< �ē��Ŕ�(��ʓ����ʈē�)
				noguide					= 0x01000080,	///< �m�[�K�C�h
				guide_code				= 0x01000090,	///< �����U��(�R�[�h)
				guide_string			= 0x01000091,	///< �����U��(������)
				illust_file				= 0x01000101,	///< �C���X�g�t�@�C����(�U����p)
				illust_file2			= 0x01000102,	///< �C���X�g�t�@�C����(�n�C�E�F�C���[�h��p)
				illust_file3			= 0x01000103	///< �C���X�g�t�@�C����(�U���A�n�C�E�F�C���[�h�p)
			};
		};

		// �n����
		namespace dirguide_class
		{
			enum code
			{
				general					= 1,			///< �n��(���)
				highway					= 2,			///< �n��(�L��)
				kokudo_no				= 3,			///< �����ԍ�
				kendo_no				= 4,			///< �����ԍ�
				others					= 5				///< ���̑�
			};
		};

		// �����U��
		namespace guide_class
		{
			enum code
			{
				noguide					= 0x00,			///< ���Ȃ�(���ʈē��Ȃ�)
				straight				= 0x01,			///< ���i(12������)
				right_front				= 0x02,			///< �E�O����(1������)
				right_ahead				= 0x03,			///< �E��O����(2������)
				right					= 0x04,			///< �E��(3������)
				right_behind			= 0x05,			///< �E�΂ߌ�����(4������)
				right_back				= 0x06,			///< �E�߂����(5������)
				back					= 0x07,			///< �߂����(6������)
				left_back				= 0x08,			///< ���߂����(7������)
				left_behind				= 0x09,			///< ���΂ߌ�����(8������)
				left					= 0x0a,			///< ����(9������)
				left_ahead				= 0x0b,			///< ����O����(10������)
				left_front				= 0x0c			///< ���O����(11������)
			};
		};

		// �����N�g�����C���L�[�R�[�h
		namespace link_info_class
		{
			enum code
			{
				kubun					= 1,			///< ��]�敪�R�[�h
				speed					= 2,			///< �K�����x�R�[�h
				trip					= 3,			///< ���s���x
				island					= 4,			///< �������e�t���O
				area					= 6,			///< �׊X�H�G���A���ʃt���O
				lane					= 7,			///< �Б��Ԑ���
			};
		};

		// �L�[ID
		namespace lext_key
		{
			enum code
			{
				kubun_code				= 1,			///< ��]�敪�R�[�h
				limit_speed				= 2,			///< �K�����x
				trip_speed				= 3,			///< ���s���x
				island_flag				= 4,			///< �������e�t���O
				area_flag				= 6,			///< �׊X�H�G���A�t���O
				oneside_lane			= 7,			///< �Б��Ԑ���
			};
		};

		// ��]�敪�R�[�h
		namespace kubun_code
		{
			enum code
			{
				unknown					= 0,			///< �������E��ƒ�
				main_line				= 1,			///< ��H��
				sub_line				= 2				///< �]�H��
			};
		};

		// �K�����x
		namespace limit_speed
		{
			enum code
			{
				unknown					= 0,			///< ������
				limit_under30km			= 1,			///< 30km/h����
				limit_40km				= 2,			///< 40km/h
				limit_50km				= 3,			///< 50km/h
				limit_60km				= 4,			///< 60km/h
				limit_70km				= 5,			///< 70km/h
				limit_80km				= 6,			///< 80km/h
				limit_100km				= 7				///< 100km/h
			};
		};

		// �����t���O
		namespace island_flag
		{
			using namespace authenticity;
		};

		// �׊X�H�G���A�t���O
		namespace area_flag
		{
			enum code
			{
				unknown					= 0x00000000,	///< ������
				year_1998				= 0x00000001,	///< 1998�N�x
				year_1999				= 0x00000002,	///< 1999�N�x
				year_2000_spr			= 0x00000004,	///< 2000�N�x�t
				year_2000_aut			= 0x00000008,	///< 2000�N�x�H
				year_2001				= 0x00000010,	///< 2001�N�x
				year_2002				= 0x00000020,	///< 2002�N�x
				year_2003				= 0x00000040,	///< 2003�N�x
				year_2004				= 0x00000080,	///< 2004�N�x
				year_2005				= 0x00000100,	///< 2005�N�x
				year_2006				= 0x00000200,	///< 2006�N�x
				year_2007				= 0x00000400	///< 2007�N�x
			};

			enum kisei_chousa
			{
				no_rest					= 0x00000000,	///< �K�������Ȃ�
				rest					= 0x80000000	///< �K����������
			};
		};
	};
};
