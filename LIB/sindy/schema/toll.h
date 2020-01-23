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

#ifndef SINDY_SCHEMA_TOLL_H_
#define SINDY_SCHEMA_TOLL_H_

#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// ��ʗL���̏㉺����ʂ�����e�[�u��
namespace normal_updown_table
{
	namespace updown_class
	{
		enum ECode
		{
			kUnsurveyed = -1,   ///< ������
			kDown       = 0,	///< ������(����)
			kUp         = 1,	///< �t����(���)
			kNonDivided = 2,	///< �㉺���񕪗�
			kCommon     = 3,    ///< �㉺������
		};
	}
}


/// �Б��̗����z�e�[�u��
namespace onesidetoll_table {
	extern const _TCHAR kKei_Toll[];		///< ��ցE�y����
	extern const _TCHAR kHutuu_Toll[];		///< ���ʎԗ���
	extern const _TCHAR kChuu_Toll[];		///< ���^�ԗ���
	extern const _TCHAR kDai_Toll[];		///< ��^�ԗ���
	extern const _TCHAR kTokudai_Toll[];	///< ����ԗ���

	/// �����N�����R�[�h
	namespace toll_category {
		enum ECode {
			kDummy		= -1, ///< ���z�����ݒ�(���̎Ԏ�͒ʂ�Ȃ��ꍇ�ɕt�^�����l)
			kDelete		= -2, ///< JH�p�b�`�̍폜�Ώۂ̒l
		};
	}
}

/// �㉺�ʂ̗����z�e�[�u��
namespace updowntoll_table {
	extern const _TCHAR kKei_Toll_Up[];			///< ��ցE�y����(���)
	extern const _TCHAR kHutuu_Toll_Up[];		///< ���ʎԗ���(���)
	extern const _TCHAR kChuu_Toll_Up[];		///< ���^�ԗ���(���)
	extern const _TCHAR kDai_Toll_Up[];			///< ��^�ԗ���(���)
	extern const _TCHAR kTokudai_Toll_Up[];		///< ����ԗ���(���)
	
	extern const _TCHAR kKei_Toll_Down[];		///< ��ցE�y����(����)
	extern const _TCHAR kHutuu_Toll_Down[];		///< ���ʎԗ���(����)
	extern const _TCHAR kChuu_Toll_Down[];		///< ���^�ԗ���(����)
	extern const _TCHAR kDai_Toll_Down[];		///< ��^�ԗ���(����)
	extern const _TCHAR kTokudai_Toll_Down[];	///< ����ԗ���(����)
}

/// FromTo�{�݃e�[�u��
namespace fromto_tolltable {
	extern const _TCHAR kRoadCode1[];		///< �H���R�[�h1
	extern const _TCHAR kRoadSeq1[];		///< �H�����V�[�P���X1
	extern const _TCHAR kRoadCode2[];		///< �H���R�[�h2
	extern const _TCHAR kRoadSeq2[];		///< �H�����V�[�P���X2
}

/// JH�����Ǝ������{�݃R�[�h�ϊ��e�[�u��
namespace facil_conv_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kJHRoadCode[];	///< JH�H���R�[�h
	extern const _TCHAR kJHRoadSeq[];	///< JH�H�����V�[�P���X
	extern const _TCHAR kMemo[];		///< �t���[�R�����g
}

/// JH�H���R�[�h�e�[�u��
namespace jh_road_code_list_table {
	extern const _TCHAR kJHDispCode[];		///< JH�\���ԍ�
	extern const _TCHAR kJHRoadCode[];		///< JH�H���R�[�h
	extern const _TCHAR kJHRoadName[];		///< JH�H������
}

/// JH�{�݃R�[�h�e�[�u��
namespace jh_facil_code_list_table {
	extern const _TCHAR kJHRoadCode[];		///< JH�H���R�[�h
	extern const _TCHAR kJHRoadSeq[];		///< JH�H�����V�[�P���X
	extern const _TCHAR kJHFacilName[];		///< JH�{�ݖ���
}
/// JH�����e�[�u��
namespace jh_toll_table {
	using namespace ::sindy::schema::onesidetoll_table;

	extern const _TCHAR kJHRoadCode1[];		///< JH�H���R�[�h1
	extern const _TCHAR kJHRoadSeq1[];		///< JH�H�����V�[�P���X1
	extern const _TCHAR kJHRoadCode2[];		///< JH�H���R�[�h2
	extern const _TCHAR kJHRoadSeq2[];		///< JH�H�����V�[�P���X2
	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h

	extern const _TCHAR kReserved1[];		///< ���U�[�u1
	extern const _TCHAR kDistance[];		///< �{�݊ԋ���
}

/// JH�p�b�`�e�[�u��
namespace jh_patch_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h

	extern const _TCHAR kTollMemo[];		///< ����
}

/// �Ǝ������̔����E���Z�����e�[�u��
namespace other_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;
	using namespace ::sindy::schema::normal_updown_table; // �㉺���R�[�h�̃R�[�h�l��`(bug 8304)

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h

	extern const _TCHAR kNormal_F[];		///< ��ʓ��t���O
	extern const _TCHAR kUpdown_C1[];		///< From�{�݂̏㉺���R�[�h
	extern const _TCHAR kUpdown_C2[];		///< To�{�݂̏㉺���R�[�h

	extern const _TCHAR kTollMemo[];		///< ����
}

/// �P�ƌ^�����e�[�u��
namespace alone_table {
	using namespace ::sindy::schema::updowntoll_table;
	using namespace ::sindy::schema::normal_updown_table; // �㉺���R�[�h�̃R�[�h�l��`(bug 8304)
	
	extern const _TCHAR kTableName[];

	extern const _TCHAR kLineClass_C[];		///< �H����ʃR�[�h
	extern const _TCHAR kRoadCode[];		///< �H���R�[�h
	extern const _TCHAR kRoadSeq[];			///< �H�����V�[�P���X
	
	extern const _TCHAR kUpdown_C[];		///< �㉺���R�[�h
	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h
	extern const _TCHAR kStdLinkClass_C[];	///< �W�������H����ʃR�[�h
	extern const _TCHAR kStdRoadCode[];		///< �W�������H���R�[�h

	extern const _TCHAR kNormal_F[];		///< ��ʓ��t���O
}

/// ����q�������e�[�u��
namespace nested_table {
	using namespace ::sindy::schema::onesidetoll_table;
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h

	extern const _TCHAR kTollMemo[];		///< ����
}

/// ���ݎ{�݃e�[�u��
namespace samefacil_table {
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kAlone_F[];			///< �P�Ǝ{�݃t���O

	extern const _TCHAR kTollMemo[];		///< ����
}

/// �P�Ɨ����������������{�݃e�[�u��
namespace free_table {
	using namespace ::sindy::schema::fromto_tolltable;

	extern const _TCHAR kTableName[];
}

/// �b�藿���{�݃e�[�u��
namespace temp_facil_table {
	extern const _TCHAR kTableName[];

	extern const _TCHAR kRoadCode[];		///< �H���R�[�h
	extern const _TCHAR kRoadSeq[];			///< �H�����V�[�P���X
	extern const _TCHAR kDiscount_C[];		///< ���������R�[�h

	extern const _TCHAR kTempTollClass[];	///< �b�藿�����

	extern const _TCHAR kTollMemo[];		///< ����
}

/// �H���R�[�h���X�g�e�[�u��(�ǉ���)
namespace road_code_list {
	extern const _TCHAR kLandClass[];		///< �n���ʃt���O

	/// �H����ʃR�[�h��`
	namespace land_class {
		enum ECode {
			kUnknown	 = 0, ///< ������
			kHokkaido	 = 1, ///< �k�C��
			kHonshu		 = 2, ///< �{�B�E�l���E��B
			kOkinawa	 = 3, ///< ����
			kIndependent = 4, ///< �Ɨ��H��
		};
	}
}

/// ���������E�������ԑя������ʕ���
namespace toll_discount_common {
	extern const _TCHAR kEtcDiscount[];		///< ETC���������R�[�h
	extern const _TCHAR kTollMemo[];		///< �t���[�R�����g	
}

/// ���������e�[�u��
namespace toll_discount_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::toll_discount_common;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kDiscountName[];	///< ��������
	extern const _TCHAR kManagerName[];		///< �Ǘ���Ж���
	extern const _TCHAR kEtc[];				///< ETC��p�t���O
	extern const _TCHAR kTypeKei[];			///< �Ԏ�t���O_�y
	extern const _TCHAR kTypeHutuu[];		///< �Ԏ�t���O_����
	extern const _TCHAR kTypeChuu[];		///< �Ԏ�t���O_���^
	extern const _TCHAR kTypeDai[];			///< �Ԏ�t���O_��^
	extern const _TCHAR kTypeTokuDai[];		///< �Ԏ�t���O_����	
	extern const _TCHAR kOverStart[];		///< �J�n�ׂ��L���t���O
	extern const _TCHAR kOverEnd[];			///< �I���ׂ��L���t���O
	extern const _TCHAR kOverAll[];			///< �S�ׂ��L���t���O
	extern const _TCHAR kRestriction[];		///< ������
	extern const _TCHAR kCloseDate[];		///< �I���\��N����		
}

/// �������ԑя����e�[�u��
namespace toll_discounttime_table {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::toll_discount_common;


	extern const _TCHAR kTableName[];

	extern const _TCHAR kWeekWeekday[];		///< �j���t���O_����
	extern const _TCHAR kWeekSaturday[];	///< �j���t���O_�y�j
	extern const _TCHAR kWeekSunday[];		///< �j���t���O_���j
	extern const _TCHAR kWeekHoliday[];		///< �j���t���O_�j��
	extern const _TCHAR kTermStartMonth[];	///< ���ԊJ�n��
	extern const _TCHAR kTermStartDay[];	///< ���ԊJ�n��
	extern const _TCHAR kTermEndMonth[];	///< ���ԏI����
	extern const _TCHAR kTermEndDay[];		///< ���ԏI����
	extern const _TCHAR kStartHour[];		///< ���ԑъJ�n��
	extern const _TCHAR kStartMin[];		///< ���ԑъJ�n��
	extern const _TCHAR kEndHour[];			///< ���ԑяI����
	extern const _TCHAR kEndMin[];			///< ���ԑяI����
}

} // namespace schema

} // namesaoce snindy

#endif // SINDY_SCHEMA_TOLL_H_
