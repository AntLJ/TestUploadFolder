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

// KKDCodeCommunity.h: KKDCodeCommunity �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_)
#define AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DRMACodeCommunity.h>
#include "KKDVersion.h"

/// KKD�R�[�h���O���
namespace kkd {
	/// KKD�R�[�h�ꗗ
	namespace codes {
		/// ���R�[�hID�R�[�h
		namespace a {
			enum ECode {
				kAdminId                          = 16, ///< �Ǘ��f�[�^
				kRegulationRoadNodeId             = 71, ///< ��ʋK�����H�m�[�h�f�[�^
				kRegulationRoadLinkId             = 72, ///< ��ʋK�����H�����N�f�[�^
				kLaneRegulationRoadNodeId         = 73, ///< ���[���K�����H�m�[�h�f�[�^
				kLaneRegulationRoadLinkId         = 74, ///< ���[���K�����H�����N�f�[�^
				kLaneRegulationIntersectionNodeId = 75, ///< ���[���K���W�������_�m�[�h�f�[�^
			};
		}

		/// �����_�m�[�h��ʃR�[�h
		namespace b {
			enum ECode {
				kRealCross  = 1, ///< �������_
				kDummyCross = 2, ///< �_�~�[�����_
				kDummyNode  = 3, ///< �_�~�[�m�[�h
			};

			const char* kDomains[];
		}

		/// �����_�ʍs�R�[�h
		namespace c {
			enum ECode {
				kUnknown               = 0, ///< ������
				kPassable              = 1, ///< �ʍs��
				kNoPassage             = 2, ///< �ʍs�s��
				kNoPassageFromRoadCond = 3, ///< ���H�����ɂ��ʍs�s��
				kNoPassageWithCond     = 7, ///< �����t���ʍs�s��
			};

			const char* kDomains[];
		}

		/// ��ʋK����ʃR�[�h
		namespace m {
			using namespace ::drma::codes::m;
		}

		/// �Y���L���R�[�h
		namespace y {
			using namespace ::drma::codes::y;
		}

		/// �Ԏ�R�[�h
		namespace car_type {
			const char* kDomains[];
		}

		/// �j���R�[�h
		namespace weekday {
			const char* kDomains[];
		}

		/// ��ʋK�����[���R�[�h
		namespace regulation_lane {
			const char* kDomains[];
		}

#if KKD_VERSION >= 101
/**
 * @ingroup kkd_std_1_1
 */
//@{
		/// �����_���̊����������x���R�[�h
		namespace intersection_name_kanji_research_level {
			enum ECode {
				kNoName   = 0, ///< �����_���̂Ȃ�
				kOnSignal = 1, ///< �M������̏㉺���E�A�܂��́A�M���x���ɐݒu�̊ŔɋL�ڂ���Ă���
				kAlone    = 2, ///< �����_�t�߂̕������ɋL�ڂ̂��̂�����_�����ɓƗ����ė��ĂĂ���
			};
		}

		/// �����_���̃J�i�������x���R�[�h
		namespace intersection_name_kana_research_level {
			enum ECode {
				kNoKana       = 0, ///< �J�i�Ȃ�
				kFromKKD      = 1, ///< KKD���Q�l�ɃJ�i��ݒ肵������
				kFromAlphabet = 2, ///< �����_���̔ɋL�ڂ̃A���t�@�x�b�g���Q�l�ɃJ�i��ݒ肵������
			};
		}
//@}
	}
#endif // KKD_VERSION >= 101

	namespace regulation_road_node {
		extern const int kRecordId;
	
		namespace intersection_class {
			using namespace ::kkd::codes::b;
		}

		namespace regulation_lane {
			using namespace ::kkd::codes::regulation_lane;
		}

		namespace passage {
			using namespace ::kkd::codes::c;
		}

		namespace car_type {
			using namespace ::kkd::codes::car_type;
		}

		namespace weekday {
			using namespace ::kkd::codes::weekday;
		}

#if KKD_VERSION >= 101
/**
 * @ingroup kkd_std_1_1
 */
//@{
		namespace intersection_name_kanji_research_level {
			using namespace ::kkd::codes::intersection_name_kanji_research_level;
		}

		namespace intersection_name_kana_research_level {
			using namespace ::kkd::codes::intersection_name_kana_research_level;
		}
//@}
	}
#endif // KKD_VERSION >= 101

	namespace lane_regulation_road_node {
		extern const int kRecordId;
	
		namespace intersection_class {
			using namespace ::kkd::codes::b;
		}

		namespace regulation_lane {
			using namespace ::kkd::codes::regulation_lane;
		}

		namespace passage {
			using namespace ::kkd::codes::c;
		}

		namespace car_type {
			using namespace ::kkd::codes::car_type;
		}

		namespace weekday {
			using namespace ::kkd::codes::weekday;
		}
	}

	using namespace codes::a;
	typedef ECode ERecordId;
}

#endif // !defined(AFX_KKDCODECOMMUNITY_H__EC3DED0D_454D_47AA_AD99_7F0EC951D9D1__INCLUDED_)
