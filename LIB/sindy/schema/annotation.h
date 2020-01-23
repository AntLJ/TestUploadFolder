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

#ifndef SINDY_SCHEMA_ANNOTATION_H_
#define SINDY_SCHEMA_ANNOTATION_H_

#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

/// ���L�^�ʒu�e�[�u��
namespace annotation {
	using namespace ::sindy::schema::ipc_table;

	/// ���L��ʃR�[�h
	namespace annotation_code {
		enum ECode {
#define ANNOTATION_CODE(name,id,size,label) name = id,
#include "annotationcode.h"
#undef ANNOTATION_CODE
		};
	} // annotation_code

	namespace disp_type {
		enum ECode {
			kNone	=        0, //!< �\���Ȃ�
			kStr	=        1, //!< ������̂�
			kSymStr	=        2, //!< �L���{������
			kPoiStr	=        3, //!< �w���_�{������
			kSym	=        4, //!< �L���̂�
		};
	} // disp_type

	// TODO:���L�ɋ��ʂ��Ȃ�����������܂��̂ŁA�؂蕪����K�v������܂��B
	// �����_�ł͉e�����킩��Ȃ����߂��̂܂܂ɂ��Ă����܂��B(2013/05/30 ta_suga)
	// ���L������R�A���L�R�������͓s�s�n�}�ɂ͂Ȃ��A�\���^�C�v���s�s�n�}�p�Ƃ���ȊO�̂��͈̂قȂ�܂��B

	extern const _TCHAR  kNameString1[];			//!< ���L������P
	extern const _TCHAR  kNameString2[];			//!< ���L������Q
	extern const _TCHAR  kNameString3[];			//!< ���L������R
	extern const _TCHAR  kStringNum1[];				//!< ���L�P������
	extern const _TCHAR  kStringNum2[];				//!< ���L�Q������
	extern const _TCHAR  kStringNum3[];				//!< ���L�R������
	extern const _TCHAR  kAnnoCode[];				//!< ���L���
	extern const _TCHAR	 kNameYomi[];				//!< ���L�ǂ�
	extern const _TCHAR  kDispType[];				//!< �\���^�C�v�i�s�s�n�}�p�j
	extern const _TCHAR  kSC1DispType[];			//!< �\���^�C�v�i�X�P�[��1�j
	extern const _TCHAR  kSC2DispType[];			//!< �\���^�C�v�i�X�P�[��2�j
	extern const _TCHAR  kSC3DispType[];			//!< �\���^�C�v�i�X�P�[��3�j
	extern const _TCHAR  kSC4DispType[];			//!< �\���^�C�v�i�X�P�[��4�j
	extern const _TCHAR  kContentsCode[];			//!< �R���e���c�R�[�h
	extern const _TCHAR  kContentsSeq[];			//!< �R���e���c���V�[�P���X
	extern const _TCHAR  kAnnoDisp[];				//!< ���L�\���R�[�h
} // annotation

/// POI�֘A���L�^�ʒu�e�[�u��
namespace annotation_rel_poi {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kContentsSeq[];  //!< �R���e���c���V�[�P���X
	extern const _TCHAR kAnnoDispCode[]; //!< ���L�\���R�[�h
}

/// ���L�\���ʒu�e�[�u��
namespace disp_line {
	using namespace ::sindy::schema::ipc_table;

	namespace font_size {
		enum ECode {
			k10 =		10,	//!< �P�O�|�C���g
			k12	=       12, //!< �P�Q�|�C���g
			k14	=       14, //!< �P�S�|�C���g
			k16	=       16, //!< �P�U�|�C���g
			k20	=       20, //!< �Q�O�|�C���g
			k24	=       24, //!< �Q�S�|�C���g
		};
	} // font_size

	namespace target_str {
		enum ECode {
			kStr1	=        1, //!< ������1���Q��
			kStr2	=        2, //!< ������2���Q��
			kStr3	=        3, //!< ������3���Q��
		};
	} // target_str

	extern const _TCHAR  kFontSize[];			//!< �t�H���g�T�C�Y
	extern const _TCHAR  kAnnoID[];				//!< �Ή����L����ID
	extern const _TCHAR  kTargetStr[];			//!< �Ώە�����R�[�h
} // disp_line

/// �s�s�n�}���L�^�ʒu�e�[�u��
namespace city_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // city_annotation

/// 1/6250���L�^�ʒu�e�[�u��
namespace d1_annotation {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR  kTableName[];
} // d1_annotation

/// ���k���L�^�ʒu�e�[�u��
namespace base_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // base_annotation

/// �~�h�����L�^�ʒu�e�[�u��
namespace middle_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // middle_annotation

/// ��k���L�^�ʒu�e�[�u��
namespace top_annotation {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR  kTableName[];
} // top_annotation

/// ��k���L�^�ʒu�e�[�u�� JPN
namespace top_annotation_jpn {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR kTableName[];
} // top_annotation_jpn

/// �s�s�n�}���L�\���ʒu�e�[�u��
namespace city_disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // city_disp_line

/// 1/6250���L�\���ʒu�e�[�u��
namespace c_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // c_sc1disp_line

/// ���kS4�\���ʒu�e�[�u��
namespace b_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc4disp_line

/// ���kS3�\���ʒu�e�[�u��
namespace b_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc3disp_line

/// ���kS2�\���ʒu�e�[�u��
namespace b_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc2disp_line

/// ���kS1�\���ʒu�e�[�u��
namespace b_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc1disp_line

/// �~�h��S4�\���ʒu�e�[�u��
namespace m_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc4disp_line

/// �~�h��S3�\���ʒu�e�[�u��
namespace m_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc3disp_line

/// �~�h��S2�\���ʒu�e�[�u��
namespace m_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc2disp_line

/// �~�h��S1�\���ʒu�e�[�u��
namespace m_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc1disp_line

/// ��kS4�\���ʒu�e�[�u��
namespace t_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc4disp_line

/// ��kS3�\���ʒu�e�[�u��
namespace t_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc3disp_line

/// ��kS2�\���ʒu�e�[�u��
namespace t_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc2disp_line

/// ��kS1�\���ʒu�e�[�u��
namespace t_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc1disp_line

/// ��kS4�\���ʒu�e�[�u�� JPN
namespace t_sc4disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc4disp_line_jpn

/// ��kS3�\���ʒu�e�[�u�� JPN
namespace t_sc3disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc3disp_line_jpn

/// ��kS2�\���ʒu�e�[�u�� JPN
namespace t_sc2disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc2disp_line_jpn

/// ��kS1�\���ʒu�e�[�u�� JPN
namespace t_sc1disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc1disp_line_jpn


} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ANNOTATION_H_
