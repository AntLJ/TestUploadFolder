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

#ifndef SINDY_SCHEMA_TIMESPEED_H_
#define SINDY_SCHEMA_TIMESPEED_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// ���ԑѕʑ��x�e�[�u��
namespace timespeed_table {
	extern const _TCHAR kLinkID[]; ///< ���H�����NID
	extern const _TCHAR kLinkDir[]; ///< ���H�����N����
	extern const _TCHAR kHoliday[]; ///< �x���t���O
	extern const _TCHAR kTimeGroup[]; ///< ���ԑуR�[�h

	/// ���H�����N�����R�[�h(sindy::schema::category::directional_link_relational_table�Ƃ͈قȂ邽�ߒ���)
	namespace linkdir {
		enum ECode {
			kReverse           =   0, ///<  �t����
			kForward           =   1, ///<  ������
		};
	}

	/// ���ԑуR�[�h
	namespace timezone_class {
		enum ECode {
			kFrom01To03           =   0, ///<  1���` 3��
			kFrom04To06           =   1, ///<  4���` 6��
			kFrom07To09           =   2, ///<  7���` 9��
			kFrom10To12           =   3, ///< 10���`12��
			kFrom13To15           =   4, ///< 13���`15��
			kFrom16To18           =   5, ///< 16���`18��
			kFrom19To21           =   6, ///< 19���`21��
			kFrom22To24           =   7, ///< 22���`24��
		};
	}
}

/// ���ԑѕʃ����N���s���x�e�[�u��
namespace timezone_speed {
	using namespace ::sindy::schema::timespeed_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTripSpeed_M[]; ///< ���ϗ��s���x�i�␳�O�j
	extern const _TCHAR kTripSpeed_R[]; ///< ���ϗ��s���x�i�␳��j
	extern const _TCHAR kSampleCount[]; ///< �ݐσT���v����
	extern const _TCHAR kNewestDate[]; ///< �ŐV���ђl�̗p����
	extern const _TCHAR kOldestDate[]; ///< �ŌÎ��ђl�̗p����
	extern const _TCHAR kRenewCount[]; ///< �X�V��
	extern const _TCHAR kAdopt[]; ///< �f�[�^���ʃR�[�h

	/// �f�[�^���ʃR�[�h
	namespace adopt_class {
		enum ECode {
			kMasured          =   10, ///< ���ђl
			kCalculated       =   20, ///< �\���l
		};
	}
}

/// �v���[�u�W�v���x�e�[�u��
namespace timezone_speed_probe {
	using namespace ::sindy::schema::timespeed_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTripSpeed[]; ///< ���ϗ��s���x
	extern const _TCHAR kSampleCount[]; ///< �ݐσT���v����
}

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_TIMESPEED_H_
