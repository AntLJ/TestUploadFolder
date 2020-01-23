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

#ifndef SINDY_SCHEMA_ADDRESS_H_
#define SINDY_SCHEMA_ADDRESS_H_

#include <sindy/schema/common.h>
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

// �s��n�|���S��
namespace indef_polygon {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];			//!< �Z���R�[�h
} // indef_polygon

// �ҏW�p �s�撬���|���S��
namespace city_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
}

// �ҏW�p �Z���|���S��
namespace addr_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< �Z���R�[�h�i���́j
	extern const _TCHAR kExtName1[];      //!< �g�����́i���́j
	extern const _TCHAR kExtYomi1[];      //!< �g���ǂ݁i���́j
	extern const _TCHAR kAddrCode2[];     //!< �Z���R�[�h�i�ʏ́j
	extern const _TCHAR kExtName2[];      //!< �g�����́i�ʏ́j
	extern const _TCHAR kExtYomi2[];      //!< �g���ǂ݁i�ʏ́j
	extern const _TCHAR kGaikufugo[];     //!< �X�敄��
	extern const _TCHAR kAddrType[];      //!< �Z��������ʃR�[�h
	extern const _TCHAR kColorCode[];     //!< �X��F�t�^�p�F�����R�[�h

	/// �Z��������ʃR�[�h
	namespace addr_type {
		enum ECode {
			kAreaUndefined           = 0, //!< �����ڊE������
			kDefinedWithDispAddr     = 1, //!< �Z���\�������ς�
			kDefinedWithChiban       = 2, //!< �n�Ԑ����ς�
			kUndefinedWithChiban     = 3, //!< �Z���\���E�n�Ԗ�����
		};
	} // addr_type
}

// �ҏW�p �n����\�|�C���g
namespace placename_rep_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];      //!< �Z���R�[�h
	extern const _TCHAR kName[];          //!< ��\�_����
	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
}

// �����p �Z���|�C���g
namespace addr_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];      //!< �Z���R�[�h
	extern const _TCHAR kGouNo[];         //!< ���ԍ�
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kPriority[];      //!< �D��t���O
	extern const _TCHAR kIgnore[];        //!< �X�敄�������t���O
	extern const _TCHAR kPointType[];     //!< �|�C���g������ʃR�[�h
	extern const _TCHAR kConfirm[];       //!< �m�F�X�e�[�^�X�R�[�h
	extern const _TCHAR kGeoBldID[];      //!< Geospace����ID
	extern const _TCHAR kIPCBldID[];      //!< iPC����ID

	// �Z���|�C���g������ʃR�[�h
	namespace point_type {
		enum ECode {
			kAddrWithBldg         = 1, //!< �����ɕR�Â��Z��
			kMultiAddrWithBldg    = 2, //!< ���ꌚ�������ُZ��
			kOutsideArea          = 3, //!< �G���A�O�Z��
			kWithinArea           = 4, //!< �G���A���Z���i�����R�t���Ȃ��j
			kAddrWithSubBldg      = 5, //!< �t������
		};
	} // pointy_type

	// �Z���|�C���g�m�F�X�e�[�^�X�R�[�h
	namespace confirm {
		enum ECode {
			kNone                = 0, //!< ���m�F�i�G�r�f���X�Ȃ�
			kInitGS              = 1, //!< �����f�[�^�iGeospace�j
			kInitIPC             = 2, //!< �����f�[�^�iiPC�j
			kPublicData          = 3, //!< ���I����
			kResearched          = 4, //!< ��������
			kClaim               = 5, //!< �N���[��
		};
	} // confirm
}

// �ҏW�p �g���R�[�h���X�g
namespace extcode_list
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kExtCode[];      //!< �g���R�[�h
	extern const _TCHAR kName[];         //!< ����
	extern const _TCHAR kYomi[];         //!< �ǂ�
	extern const _TCHAR kCreateDate[];   //!< �쐬�N��
}

// �ҏW�p �����t�n�ԃ}�X�^
namespace char_main_no_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];      //!< �Z���R�[�h
	extern const _TCHAR kChibanCode[];    //!< �X�敄���E�n�ԃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
}

// �ҏW�p �����t���}�X�^
namespace char_sub_no_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];      //!< �Z���R�[�h
	extern const _TCHAR kChibanCode[];    //!< �X�敄���E�n�ԃR�[�h
	extern const _TCHAR kJukyoCode[];     //!< �Z���ԍ��E�}�ԃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
}

// �ړ����ڔ��������R�[�h
namespace condition_code_table
{
	extern const _TCHAR kConditionCode[];   //!< �����R�[�h

	namespace condition_code {
		enum ECode {
			kNone = 0,                      //!< �Y������
			kHead = 1,                      //!< �擪
			kTail = 2,                      //!< ����
			kTailAfterNum = 3,              //!< ����+�Z�p�����̒���
		};
	} // condition_code
}

// �ҏW�p �ړ�����ʃ}�X�^
namespace prefix_master
{
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::condition_code_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefix[];        //!< �ړ�����ʃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
}

// �ҏW�p �ڔ�����ʃ}�X�^
namespace suffix_master
{
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::condition_code_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSuffix[];          //!< �ڔ�����ʃR�[�h
	extern const _TCHAR kName[];            //!< ����
	extern const _TCHAR kYomi[];            //!< �ǂ�
}

// �ҏW�p �s������ʃ}�X�^
namespace admin_att_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kAdminAtt[];      //!< �ڔ�����ʃR�[�h
	extern const _TCHAR kName[];          //!< ����
}

// ���̒ʏ̎�ʃR�[�h
namespace name_code_table {
	extern const _TCHAR kNameCode[];      //!< ���̒ʏ̎�ʃR�[�h

	namespace name_code {
		enum ECode {
			kOfficial      = 0, //!< ����
			kLocal         = 1, //!< �ʏ�
			kStreet        = 2, //!< �ʂ薼
		};
	} // name_code
}

// �Z���\�����R�[�h
namespace addr_disp_table {
	extern const _TCHAR kAddrDisp[];      //!< �Z���\�����R�[�h

	namespace addr_disp {
		enum ECode {
			kOutsideArea   = 0, //!< �Z���\�����O
			kWithinArea    = 1, //!< �Z���\�����
			kMixed         = 2, //!< ����
		};
	} // addr_disp
}

// �����f�[�^��ʃR�[�h
namespace gnrt_type_table {
	extern const _TCHAR kGnrtType[];      //!< �����f�[�^��ʃR�[�h

	namespace gnrt_type {
		enum ECode {
			kNormal          = 0, //!< �ʏ퐮��
			kStreet          = 1, //!< �ʂ薼�i�@�B�����j
		};
	} // gnrt_type
}

// �����[�X�p ���k�s���E�i�s�撬���j�|���S��
namespace base_admin_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];     //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];     //!< �s�撬���R�[�h
}

// �����[�X�p �s�s�n�}�s���E�i�X��j�|���S��
namespace city_admin_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];       //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];       //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];      //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];      //!< ���ځE���R�[�h
	extern const _TCHAR kChibanCode[];     //!< �X�敄���E�n�ԃR�[�h
}

// �����[�X�p �s���{���|�C���g
namespace pref_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
}

// �����[�X�p �s�撬���|�C���g
namespace city_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kAdminAtt[];
}

// �����[�X�p ���E�厚�|�C���g
namespace oaza_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::name_code_table;
	using namespace ::sindy::schema::addr_disp_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kPrefix[];        //!< �ړ�����ʃR�[�h
	extern const _TCHAR kSuffix[];        //!< �ڔ�����ʃR�[�h
}

// �����[�X�p ���ځE���|�C���g
namespace aza_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::name_code_table;
	using namespace ::sindy::schema::addr_disp_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];     //!< ���ځE���R�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kPrefix[];        //!< �ړ�����ʃR�[�h
	extern const _TCHAR kSuffix[];        //!< �ڔ�����ʃR�[�h
}

// �����[�X�p �X��E�n�ԃ|�C���g
namespace main_no_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::addr_disp_table;
	using namespace ::sindy::schema::gnrt_type_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];     //!< ���ځE���R�[�h
	extern const _TCHAR kChibanCode[];    //!< �X�敄���E�n�ԃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kStatus[];        //!< �n�ԏ�ԃR�[�h
	extern const _TCHAR kPntType[];       //!< �X��E�n�Ԏ��ʃR�[�h
	extern const _TCHAR kRep[];           //!< ��\�_�t���O
	extern const _TCHAR kBranch[];        //!< �x���t���O
	extern const _TCHAR kMulti[];         //!< �����Z���t���O
	extern const _TCHAR kBldOID[];        //!< ����ID

	/// �n�ԏ�ԃR�[�h
	namespace status {
		enum ECode {
			kNone           = 0, //!< ���ݒ�
			kPolygon        = 1, //!< �|���S�����琶��
			kSingleChiban   = 2, //!< �n�Ԏ~�܂�̃|�C���g��1�_�A�n�Ԏ~�܂�ł͂Ȃ��|�C���g��0
			kSingleGou      = 3, //!< �n�Ԏ~�܂�̃|�C���g��0�A�n�Ԏ~�܂�ł͂Ȃ��|�C���g��1�_
			kMultiChiban    = 4, //!< �n�Ԏ~�܂�̃|�C���g�������A�n�Ԏ~�܂�ł͂Ȃ��|�C���g��0
			kMultiGou       = 5, //!< �n�Ԏ~�܂�̃|�C���g��0�A�n�Ԏ~�܂�ł͂Ȃ��|�C���g������
			kChibanGouMixed = 6, //!< �n�Ԏ~�܂�̃|�C���g��1�ȏ�A�n�Ԏ~�܂�ł͂Ȃ��|�C���g��1�ȏ�
		};
	} // status

	/// �X��E�n�Ԏ��ʃR�[�h
	namespace pnt_type {
		enum ECode {
			kBlock       = 0, //!< �X��
			kChiban      = 1, //!< �n��
		};
	} // pnt_type
}

// �����[�X�p ���E�}�ԃ|�C���g
namespace sub_no_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::gnrt_type_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];     //!< ���ځE���R�[�h
	extern const _TCHAR kChibanCode[];    //!< �X�敄���E�n�ԃR�[�h
	extern const _TCHAR kJukyoCode[];     //!< �Z���ԍ��E�}�ԃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kRep[];           //!< ��\�_�t���O
	extern const _TCHAR kBranch[];        //!< �}�ԃt���O
	extern const _TCHAR kMulti[];         //!< �����Z���t���O
	extern const _TCHAR kBldOID[];        //!< ����ID
}

// �����[�X�p �X�֔ԍ��|�C���g
namespace zip_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
	extern const _TCHAR kZipAtt[];        //!< �X�֔ԍ���ʃR�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kYomi[];          //!< �ǂ�
	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];     //!< ���ځE���R�[�h
	extern const _TCHAR kRep[];           //!< ��\�_�t���O

	/// �X�֔ԍ���ʃR�[�h
	namespace zip_att {
		enum ECode {
			kAddress           = 1, //!< �Z��
			kBuilding          = 2, //!< �r��
			kOffice            = 3, //!< ������Ə�
			kPostOfficeBox     = 4, //!< ������
		};
	}
}

// �����[�X�p �X�֔ԍ��|���S��
namespace zip_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
	extern const _TCHAR kPriority[];      //!< �D��x
}

// �����[�X�p �X�֔ԍ��|���S���d�S�|�C���g
namespace zip_centroid
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
}

// �����[�X�p �X�֔ԍ��ɕR�t���s�撬����\�|�C���g
namespace zip_rep
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
}

// �����[�X�p �X�֔ԍ��ɕR�t���Z���R�[�h 
namespace rel_zip_addr
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< �X�֔ԍ�
	extern const _TCHAR kPrefCode[];      //!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];      //!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode1[];     //!< ���E�厚�R�[�h
	extern const _TCHAR kAddrCode2[];     //!< ���ځE���R�[�h
	extern const _TCHAR kChibanCode[];    //!< �X�敄���E�n�ԃR�[�h
	extern const _TCHAR kJukyoCode[];     //!< �Z���ԍ��E�}�ԃR�[�h
}

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ADDRESS_H_
