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

// DRMACodeCommunity.h: DRMACodeCommunity �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMACodeCommunity.h
 * @brief DRMA�R�[�h��`�w�b�_�t�@�C��
 */
#if !defined(AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_)
#define AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// DRMA�R�[�h���O���
namespace drma {
	/// DRMA�R�[�h�ꗗ
	namespace codes {
		/// ���R�[�hID�R�[�h
		namespace a {
			/// ���R�[�hID�R�[�h�ꗗ
			enum ECode {
				kAdmin1Id              = 11, ///< �Ǘ��f�[�^�i����1�j
				kAdmin2Id              = 12, ///< �Ǘ��f�[�^�i����2�j
				kBaseRoadNodeId        = 21, ///< ��{���H�m�[�h�f�[�^
				kBaseRoadLinkId        = 22, ///< ��{���H�����N�f�[�^
				kBaseRoadLinkAttrId    = 23, ///< ��{���H�����N�������f�[�^
				kRoadLinkRelationId    = 24, ///< ��{���H�����N�E�S���H�����N�Ή��f�[�^
				kBeaconLocationId      = 29, ///< �r�[�R���ʒu�f�[�^
				kAllRoadNodeId         = 31, ///< �S���H�m�[�h�f�[�^
				kAllRoadLinkId         = 32, ///< �S���H�����N�f�[�^
				kSiteId                = 41, ///< ���n�f�[�^
				kAdminLocationId       = 42, ///< �s���E�ʒu�f�[�^
				kRailwayLocationId     = 43, ///< �S���ʒu�f�[�^
				kFacilLocationId       = 44, ///< �{�ݓ��ʒu�f�[�^
				kFacilShapeId          = 45, ///< �{�ݓ��`��f�[�^
				kPlaceNameLocationId   = 46, ///< �n�����\���ʒu�f�[�^
				kAllRoadLinkAttrId     = 93  ///< �S���H�����N�������f�[�^
			};
		}

		/// �m�[�h��ʃR�[�h
		namespace b {
			/// �m�[�h��ʃR�[�h�ꗗ
			enum ECode {
				kCross     = 1, ///< �����_�m�[�h
				kTerminal  = 2, ///< �s���~�܂�m�[�h
				kDummy     = 3, ///< �_�~�[�_�m�[�h
				kMeshJoint = 4, ///< ���ӌ�_�m�[�h
				kAlter     = 5, ///< �����ω��_�m�[�h
				kEvasion   = 6, ///< ��ʊǐ���K�v�ȃm�[�h
			};

			const char* kDomains[];
		}

		/// �����_�ʍs�R�[�h
		namespace c {
			/// �����_�ʍs�R�[�h�ꗗ
			enum ECode {
				kUnknown          = 0, ///< ������
				kPassable         = 1, ///< �ʍs��
				kNoPassage        = 2, ///< �ʍs�s��
				kTypePassable     = 3, ///< �����t�i�Ԏ�j�ʍs��
				kTimePassable     = 4, ///< �����t�i�����A�j���j�ʍs��
				kTypeTimePassable = 5, ///< �����t�i�����A�j���A�Ԏ�j�ʍs��
			};

			const char* kDomains[];
		}

		/// �Ǘ��҃R�[�h
		namespace d {
			/// �Ǘ��҃R�[�h�ꗗ
			enum ECode {
				kUnknown    = 0, ///< ������
				kJH         = 1, ///< ���{���H���c
				kPublic3    = 2, ///< ���H�R���c
				kPublic     = 3, ///< ���H����
				kCountry    = 4, ///< ��
				kPrefecture = 5, ///< �s���{��
				kCity       = 6, ///< �w��s
				kOtherTown  = 7, ///< ���̎s�������i�ܓ���23��j
				kOther      = 8, ///< ���̑��̊Ǘ���
			};

			const char* kDomains[];
		}

		/// ���H��ʃR�[�h
		namespace e {
			/// ���H��ʃR�[�h�ꗗ
			enum ECode {
				kUnknown          = 0, ///< ������
				kHighway          = 1, ///< ���������ԓ��H
				kCityHighway      = 2, ///< �s�s�������H�i�܎w��s�s�������H�j
				kCountry          = 3, ///< ��ʍ���
				kPrefectureMain   = 4, ///< ��v�n�����i�s���{�����j
				kCityMain         = 5, ///< ��v�n�����i�w��s���j
				kPrefectureNormal = 6, ///< ��ʓs���{����
				kCityNormal       = 7, ///< �w��s�̈�ʎs��
				kOther            = 9, ///< ���̑����H
			};

			const char* kDomains[];
		}

		/// ��E�]���H�敪�R�[�h
		namespace f {
			/// ��E�]���H�敪�R�[�h�ꗗ
			enum ECode {
				kUnknown = 0, ///< ������
				kMain    = 1, ///< �哹�H
				kSub     = 2, ///< �]���H
			};

			const char* kDomains[];
		}

		/// �����N��ʃR�[�h
		namespace h {
			/// �����N��ʃR�[�h�ꗗ
			enum ECode {
				kUnknown       = 0, ///< ������
				kMain          = 1, ///< �{���i�㉺���񕪗��j�����N
				kMainUpDown    = 2, ///< �{���i�㉺�����j�����N
				kConnect       = 3, ///< �A���H�i�{���Ԃ̓n����j�����N
				kCross         = 4, ///< �����_�������N
				kRamp          = 5, ///< �A���H�i�����v�j�����N
				kFontage       = 6, ///< �{���Ɠ���H���̑��������N
				kSA	           = 7, ///< �r�`�����������N
				kBicycle       = 8, ///< ���]�ԓ��������N
				kFontageToMain = 9, ///< �{�������ڑ������N
			};

			const char* kDomains[];
		}

		/// �����N�ʍs�E�����ԕs�R�[�h
		namespace i {
			/// �����N�ʍs�E�����ԕs�R�[�h�ꗗ
			enum ECode {
				kUnknown      = 0, ///< ������
				kCarPassable  = 1, ///< �����Ԓʍs��
				kCarNoPassage = 2, ///< �����Ԓʍs�s��
				kPreService   = 3, ///< �����p�i�܍H�����j
				kPlan         = 4, ///< �v�挈��
			};

			const char* kDomains[];
		}

		/// �ُ�C�ێ��ʍs�K����Ԏ�ʃR�[�h
		namespace j {
			/// �ُ�C�ێ��ʍs�K����Ԏ�ʃR�[�h�ꗗ
			enum ECode {
				kUnknown      = 0, ///< ������
				kNoRegulation = 1, ///< �K����
				kRain         = 2, ///< �J�ʋK���L
				kSnow         = 3, ///< ��K���L
				kOther        = 4, ///< ���̑��K���L
			};

			const char* kDomains[];
		}

		/// ���H�����敪�R�[�h
		namespace k {
			/// ���H�����敪�R�[�h�ꗗ
			enum ECode {
				kUnknown         = 0, ///< ������
				kOver130         = 1, ///< ����13.0m�ȏ�
				kBetween55And130 = 2, ///< ����5.5m�ȏ�`13.0m����
				kBetween30And55  = 3, ///< ����3.0m�ȏ�`5.5m����
				kUnder30         = 4, ///< ����3.0m����
			};

			const char* kDomains[];
		}

		/// �Ԑ����R�[�h
		namespace l {
			/// �Ԑ����R�[�h�ꗗ
			enum ECode {
				kUnknown = 0, ///< ������
				k1       = 1, ///< 1�Ԑ�
				k2       = 2, ///< 2�Ԑ�
				k3       = 3, ///< 3�Ԑ�
				k4       = 4, ///< 4�Ԑ�
				k5       = 5, ///< 5�Ԑ�
				kOver6   = 6, ///< 6�Ԑ��ُ�
			};

			const char* kDomains[];
		}

		/// ��ʋK����ʃR�[�h
		namespace m {
			/// ��ʋK����ʃR�[�h�ꗗ
			enum ECode {
				kUnknown             = 0, ///< ������
				kNoRegulation        = 1, ///< �K������
				kNoPassage           = 2, ///< �ʍs�֎~�i�������j
				kNoPassageWithCond   = 3, ///< �ʍs�֎~�i�����t�j
				kFore                = 4, ///< ����ʍs(�������A������)
				kReverse             = 5, ///< ����ʍs(�t�����A������)
				kForeWithCond        = 6, ///< ����ʍs(�������A�����t)
				kReverseWithCond     = 7, ///< ����ʍs(�t�����A�����t)
				kForeReverseWithCond = 8, ///< ����ʍs(���t�ؑւ��L��)
			};

			const char* kDomains[];
		}

		/// ��ʋK��������ʃR�[�h
		namespace n {
			/// ��ʋK��������ʃR�[�h�ꗗ
			enum ECode {
				kUnknown     = 0, ///< ������
				kType        = 1, ///< �Ԏ�̂�
				kTime        = 2, ///< �����̂�
				kWeekday     = 3, ///< �j���̂�
				kTypeTime    = 4, ///< �Ԏ�y�ю���
				kTypeWeekday = 5, ///< �Ԏ�y�їj��
				kWeekdayTime = 6, ///< �j���y�ю���
				kAll         = 7, ///< �Ԏ�A�����y�їj��
				kOther       = 8, ///< ���̑��̏���
			};

			const char* kDomains[];
		}

		/// �K�����x�R�[�h
		namespace o {
			/// �K�����x�R�[�h�ꗗ
			enum ECode {
				kUnknown = 0, ///< ������
				kUnder30 = 1, ///< 30km/h����
				k40      = 2, ///< 40km/h
				k50      = 3, ///< 50km/h
				k60      = 4, ///< 60km/h
				k70      = 5, ///< 70km/h
				k80      = 6, ///< 80km/h
				k100     = 7, ///< 100km/h
			};

			const char* kDomains[];
		}

		/// �`��f�[�^�擾�����R�[�h
		namespace o10 {
			/// �`��f�[�^�擾�����R�[�h�ꗗ
			enum ECode {
				kUnknown    = 0, ///< ������
				kAfter1991  = 1, ///< 91�N�x�ȍ~���H�Ǘ��ғ�����
				kTopography = 2, ///< �k�ڂP/2.5���n�`�}
				kBefore1990 = 3, ///< 90�N�x�ȑO���H�Ǘ��ғ�����
				kTopo10000  = 4, ///< �k��1/1���ȏ�̒n�`�}
				k5          = 5, ///< �y�n��搮�����Ǝғ�����
				k6          = 6, ///< �s�������������ƎҎ���
				k7          = 7, ///< �_���E�ѓ��E�`�p�������ƎҎ���
				k8          = 8, ///< ���n����
			};

			const char* kDomains[];
		}

		/// ������ʃR�[�h
		namespace p {
			/// ������ʃR�[�h�ꗗ
			enum ECode {
				kBridge       = 1, ///< ���E����
				kTunnel       = 2, ///< �g���l��
				kDoumon       = 3, ///< ���哙
				kRailCross    = 4, ///< ����
				kUnderPass    = 5, ///< ���̎{�݂��A���_�[�p�X
				kPedestrian   = 6, ///< ������
				kTollbooth    = 7, ///< �������iETC�����j
				kName         = 8, ///< ���H�ʏ̖�
				kEtcTollbooth = 9, ///< �������iETC�L��j
				kEtcTollOnly  = 10,///< �������iETC��p�j
				kBypass       = 11,///< �o�C�p�X���H��
			};

			const char* kDomains[];
		}

		/// ���n��ʃR�[�h
		namespace r {
			/// ���n��ʃR�[�h�ꗗ
			enum ECode {
				kPolygon =  1, ///< �ʕ\�����n
				kPolyline = 2, ///< ���\�����n
			};

			const char* kDomains[];
		}

		/// ����R�[�h
		namespace s {
			/// ����R�[�h�ꗗ
			enum ECode {
				kEmptiness = 0, ///< �Ӗ��Ȃ�
				kLine      = 1, ///< �����E��
				kBoundray  = 2, ///< ���Ӑ�
				kVirtual   = 3, ///< ���z��
			};

			const char* kDomains[];
		}

		/// �S����ʃR�[�h
		namespace u {
			/// �S����ʃR�[�h�ꗗ
			enum ECode {
				kUnknown      = 0, ///< ������
				kShinkansen   = 1, ///< �V����
				kJR           = 2, ///< JR��
				kNormal       = 3, ///< JR���ȊO�i���ʓS���j
				kSurfaceTrack = 4, ///< JR���ȊO�i�H�ʓS��
				kMonorail     = 5, ///< JR���ȊO�i���m���[���j
				kNewTraffic   = 6, ///< JR���ȊO�i�V��ʁj
				kOther        = 7, ///< ���̑�
			};

			const char* kDomains[];
		}

		/// �Y���L���R�[�h
		namespace y {
			/// �Y���L���R�[�h�ꗗ
			enum ECode {
				kUnknown = 0, ///< �������E�Ӗ��Ȃ�
				kTrue    = 1, ///< �Y������
				kFlase   = 2, ///< �Y������
			};

			const char* kDomains[];
		}
	}

	using namespace codes::a;
	typedef ECode ERecordId;

	/// �Ǘ��f�[�^�i���̂P�j�f�[�^�R�[�h���
	namespace admin1 {
		extern const int kRecordId;
	}

	/// �Ǘ��f�[�^�i���̂Q�j�f�[�^�R�[�h���
	namespace admin2 {
		extern const int kRecordId;
	}

	/// ��{���H�m�[�h�f�[�^�R�[�h���
	namespace base_road_node {
		extern const int kRecordId;

		namespace node_class {
			using namespace ::drma::codes::b;
		};

		namespace passage {
			using namespace ::drma::codes::c;
		};
	}

	/// ��{���H�����N�f�[�^�R�[�h���
	namespace base_road_link {
		extern const int kRecordId;

		namespace manager {
			using namespace ::drma::codes::d;
		};

		namespace road_class {
			using namespace ::drma::codes::e;
		}

		namespace kubun {
			using namespace ::drma::codes::f;
		}

		namespace link_class {
			using namespace ::drma::codes::h;
		}

		namespace beacon {
			using namespace ::drma::codes::y;
		}

		namespace car_only {
			using namespace ::drma::codes::y;
		}

		namespace toll {
			using namespace ::drma::codes::y;
		}

		namespace passage {
			using namespace ::drma::codes::i;
		}

		namespace weather_regulation {
			using namespace ::drma::codes::j;
		}

		namespace car_weight {
			using namespace ::drma::codes::y;
		}

		namespace car_height {
			using namespace ::drma::codes::y;
		}

		namespace car_width {
			using namespace ::drma::codes::y;
		}

		namespace width {
			using namespace ::drma::codes::k;
		}

		namespace lane {
			using namespace ::drma::codes::l;
		}

		namespace regulation {
			using namespace ::drma::codes::m;
		}

		namespace regulation_cond {
			using namespace ::drma::codes::n;
		}

		namespace no_passing {
			using namespace ::drma::codes::y;
		}

		namespace speed_regulation {
			using namespace ::drma::codes::o;
		}

		namespace spetial_vehicle {
			using namespace ::drma::codes::y;
		}

		namespace attr {
			using namespace ::drma::codes::y;
		}

		namespace shape_source {
			using namespace ::drma::codes::o10;
		}

		namespace designate {
			using namespace ::drma::codes::y;
		}
	}

	/// ��{���H�����N�������f�[�^�R�[�h���
	namespace base_road_link_attr {
		extern const int kRecordId;

		namespace attr_class {
			using namespace ::drma::codes::p;
		}

		namespace pre_begin {
			using namespace ::drma::codes::y;
		}

		namespace epi_begin {
			using namespace ::drma::codes::y;
		}

		namespace heavy_no_passage {
			using namespace ::drma::codes::y;
		}
	}

	/// ��{���H�����N�E�S���H�����N�Ή��f�[�^�R�[�h���
	namespace road_link_relation {
		extern const int kRecordId;
	}

	/// �r�[�R���ʒu�f�[�^�R�[�h���
	namespace beacon_location {
		extern const int kRecordId;
	}

	/// �S���H�m�[�h�f�[�^�R�[�h���
	namespace all_road_node {
		extern const int kRecordId;

		namespace node_class {
			using namespace ::drma::codes::b;
		};
	}

	/// �S���H�����N�f�[�^�R�[�h���
	namespace all_road_link {
		extern const int kRecordId;

		namespace manager {
			using namespace ::drma::codes::d;
		};

		namespace road_class {
			using namespace ::drma::codes::e;
		}

		namespace width {
			using namespace ::drma::codes::k;
		}

		namespace lane {
			using namespace ::drma::codes::l;
		}

		namespace regulation {
			using namespace ::drma::codes::m;
		}

		namespace regulation_cond {
			using namespace ::drma::codes::n;
		}

		namespace attr {
			using namespace ::drma::codes::y;
		}

	}

	/// ���n�f�[�^�R�[�h���
	namespace site {
		extern const int kRecordId;

		namespace site_class {
			using namespace ::drma::codes::r;
		}

		namespace line_class {
			using namespace ::drma::codes::s;
		}
	}

	/// �s���E�ʒu�f�[�^�R�[�h���
	namespace admin_location {
		extern const int kRecordId;
	}

	/// �S���ʒu�f�[�^�R�[�h���
	namespace railway_location {
		extern const int kRecordId;

		namespace railway_class {
			using namespace ::drma::codes::u;
		}

		namespace underground {
			using namespace ::drma::codes::y;
		}
	}

	/// �{�ݓ��ʒu�f�[�^�R�[�h���
	namespace facil_location {
		extern const int kRecordId;
	}

	/// �{�ݓ��`��f�[�^�R�[�h���
	namespace facil_shape {
		extern const int kRecordId;
	}

	/// �n�����\���ʒu�f�[�^�R�[�h���
	namespace place_name_location {
		extern const int kRecordId;
	}

	/// �S���H�����N�������f�[�^�R�[�h���
	namespace all_road_link_attr {
		extern const int kRecordId;
	}
}

#endif // !defined(AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_)
