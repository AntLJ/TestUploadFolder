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

#pragma once
//#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace sindym {

/// �����|�C���g
namespace spot_survey_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kObjectidPostgres[];  //!< �|�X�O��OID
	extern const _TCHAR kCompanyName[];  //!< ������Ж�
	extern const _TCHAR kAccountName[];  //!< �A�J�E���g��
	extern const _TCHAR kPermissionLevel[];  //!< �������x��
	extern const _TCHAR kModifyDatetime[];  //!< �ŏI�X�V��
	extern const _TCHAR kCitymesh[];  //!< �s�s�n�}���b�V��
	extern const _TCHAR kCreatedBySurveyorF[];  //!< �������쐬�t���O
	extern const _TCHAR kManagementId[];  //!< �Ǘ�ID
	extern const _TCHAR kTargetType[];  //!< �Ώێ��
	extern const _TCHAR kSeason[];  //!< �����f�[�^
	extern const _TCHAR kSurveyType[];  //!< �������
	extern const _TCHAR kAllDisplayF[];  //!< �\�����������t���O
	extern const _TCHAR kSurveyRank[];  //!< ���������N
	extern const _TCHAR kName[];  //!< ����
	extern const _TCHAR kNameOrg[];  //!< �C���O����
	extern const _TCHAR kAddress11[];  //!< �Z��������11��
	extern const _TCHAR kSurveyAttribute[];  //!< �����Ώۑ���
	extern const _TCHAR kSurveyRequestType[];  //!< �˗����e
	extern const _TCHAR kCompanyComment[];  //!< ������ЃR�����g
	extern const _TCHAR kChangedCode[];  //!< �ύX���e�R�[�h
	extern const _TCHAR kSurveyComment[];  //!< �����R�����g
	extern const _TCHAR kPictureImpossibleF[];  //!< �B�e�s�t���O
	extern const _TCHAR kCreatedByIPCF[];  //!< iPC�쐬�t���O
	extern const _TCHAR kSurveyDate[];  //!< ������
	extern const _TCHAR kRemarks1[];  //!< ���l1
	extern const _TCHAR kRemarks2[];  //!< ���l2
	extern const _TCHAR kFirstCheckCompleteDate[];  //!< �ꎟ����������
	extern const _TCHAR kNgAttribute[];  //!< NG���
	extern const _TCHAR kNgComment[];  //!< NG�R�����g
}

/// survey_point��OID�����e�[�u��
namespace spot_survey_point_relational_table {
	extern const _TCHAR kSurveyPointId[];  //!< �����|�C���gID
}

/// �C���O�����|�C���g
namespace spot_survey_point_org {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	using namespace spot_survey_point_relational_table;
	extern const _TCHAR kCitymeshOrg[];  //!< �C���O�s�s�n�}���b�V��
	extern const _TCHAR kMeshOid[];  //!< ���b�V��OID
}

/// �摜�|�C���g
namespace picture_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	using namespace spot_survey_point_relational_table;
	extern const _TCHAR kPictureSeq[];  //!< �摜�i�[��
	extern const _TCHAR kDirection[];  //!< �B�e����
}

/// �C���X�g�|�C���g
namespace illustration_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	using namespace spot_survey_point_relational_table;
}

/// �C���X�g���C��
namespace illustration_line {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	using namespace spot_survey_point_relational_table;
}

/// �C���X�g�|���S��
namespace illustration_polygon {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	using namespace spot_survey_point_relational_table;
}

} // sindym

} // schema

} // sindy

