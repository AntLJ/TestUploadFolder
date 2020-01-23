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
#include <sindy/schema/category.h>

namespace sindy
{
namespace schema
{
/// ������DB
namespace multi_language
{
	using namespace category;

	/// ������֘A�e�[�u���̋��ʊ��I�u�W�F�N�g�N���X
	namespace ipc_mlang_table
	{
		extern const _TCHAR kDeleteF[];    //!< �폜�t���O
		extern const _TCHAR kLangCode[];   //!< ����R�[�h

		/// SiNDY-k ����R�[�h
		namespace lang_code
		{
			enum ECode
			{
				romaji = 1,                 //!< ���[�}���\�L
				english = 2,                //!< �p��
				simplified_chinese = 3,     //!< ������ȑ̎�
				traditional_chinese = 4,    //!< ������ɑ̎�
				korean = 5,                 //!< �؍���
				thai = 6,                   //!< �^�C��
				indonesia = 7,              //!< �C���h�l�V�A��
			};
		}
	}

	/// ������v���̂��܂ރe�[�u��
	namespace partial_name_table
	{
		extern const _TCHAR kSortOrder[];   //!< ���я�
		extern const _TCHAR kPartialName[]; //!< ��������
	}

	/// �����꒍�L
	namespace annotation
	{
		using namespace ipc_mlang_table;
		using namespace contents_seq_table;
		using namespace contents_code_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����

		extern const _TCHAR kAnnotationScale[]; //!< ���L�\���X�P�[��
		extern const _TCHAR kAnnotationID[];    //!< ANNOTATIONID
		extern const _TCHAR kWorkName[];        //!< ��Ɩ�
		extern const _TCHAR kOrgName[];         //!< ������
		extern const _TCHAR kOrgYomi[];         //!< ���ǂ�
		extern const _TCHAR kName[];            //!< ����

		/// �\�����C���R�[�h
		namespace display_layer
		{
			enum ECode
			{
				city   = 0,      //!< CITY
				base   = 1,      //!< BASE
				middle = 2,      //!< MIDDLE
				top    = 3,      //!< TOP
			};
		}
	}

	/// ��������POI
	namespace poi
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace chain_code_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����

		extern const _TCHAR kName[]; //!< ����
	}

	/// ��������POI������v
	namespace poi_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace chain_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����

	}

	/// ������S��
	namespace railway
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����

		extern const _TCHAR kTrainTypeCode[]; //!< �S�����ރR�[�h
		extern const _TCHAR kTrainCorpCode[]; //!< �S����ЃR�[�h
		extern const _TCHAR kTrainLineCode[]; //!< �S���H���R�[�h
		extern const _TCHAR kLineSeq[];       //!< �H�����V�[�P���X
		extern const _TCHAR kStationSeq[];    //!< �S���w���V�[�P���X
		extern const _TCHAR kName[];          //!< ����
		extern const _TCHAR kSubName[];       //!< ���́E�⑫����
	}

	/// ������S��������v
	namespace railway_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����
	}

	/// ������Z��
	namespace addr
	{
		using namespace ipc_mlang_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[]; //!< �e�[�u����

		extern const _TCHAR kAddrCode[];      //!< 20���Z���R�[�h
		extern const _TCHAR kName[];          //!< ����
		extern const _TCHAR kSubName[];       //!< ���́E�⑫����
	}

	/// �����ꓹ�H�{��
	namespace road
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[];    //!< �e�[�u����

		extern const _TCHAR kRoadTypeCode[]; //!< ���H���ރR�[�h
		extern const _TCHAR kRoadLineCode[]; //!< ���H�H���R�[�h
		extern const _TCHAR kRoadSeq[];      //!< �H�����V�[�P���X
		extern const _TCHAR kFclAttCode[];   //!< ���H�{�ݎ�ʃR�[�h
		extern const _TCHAR kDirectionCode[];//!< ���ʃR�[�h
		extern const _TCHAR kName[];         //!< ����
		extern const _TCHAR kSubName[];      //!< �⑫����
		extern const _TCHAR kShortName[];    //!< ����
		extern const _TCHAR kDisplayName[];    //!< �\���p����
	}

	/// �����ꓹ�H�{�ݕ�����v
	namespace road_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����
	}

	/// ������}�X�^
	namespace master
	{
		using namespace ipc_mlang_table;

		namespace chain_master
		{
			extern const _TCHAR kTableName[]; //!< �e�[�u����

			extern const _TCHAR kChainCode[]; //!< �X�܃R�[�h
			extern const _TCHAR kName[]; //!< ����
		}

		namespace group_master
		{
			extern const _TCHAR kTableName[]; //!< �e�[�u����

			extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
			extern const _TCHAR kName[]; //!< ����
		}

	}

	// �����[�X�p ������s���{���|�C���g
	namespace pref_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// �����[�X�p ������s�撬���|�C���g
	namespace city_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kCityCode[];    //!< �s�撬���R�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// �����[�X�p �����꒬�E�厚�|�C���g
	namespace oaza_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kCityCode[];    //!< �s�撬���R�[�h
		extern const _TCHAR kAddrCode1[];   //!< ���E�厚�R�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// �����[�X�p �����꒚�ځE���|�C���g
	namespace aza_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kCityCode[];    //!< �s�撬���R�[�h
		extern const _TCHAR kAddrCode1[];   //!< ���E�厚�R�[�h
		extern const _TCHAR kAddrCode2[];   //!< ���ځE��R�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// �����[�X�p ������X��E�n�ԃ|�C���g
	namespace main_no_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kCityCode[];    //!< �s�撬���R�[�h
		extern const _TCHAR kAddrCode1[];   //!< ���E�厚�R�[�h
		extern const _TCHAR kAddrCode2[];   //!< ���ځE��R�[�h
		extern const _TCHAR kChibanCode[];  //!< �X�敄���E�n�ԃR�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// �����[�X�p �����ꍆ�E�}�ԃ|�C���g
	namespace sub_no_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< �e�[�u����

		extern const _TCHAR kPrefCode[];    //!< �s���{���R�[�h
		extern const _TCHAR kCityCode[];    //!< �s�撬���R�[�h
		extern const _TCHAR kAddrCode1[];   //!< ���E�厚�R�[�h
		extern const _TCHAR kAddrCode2[];   //!< ���ځE��R�[�h
		extern const _TCHAR kChibanCode[];  //!< �X�敄���E�n�ԃR�[�h
		extern const _TCHAR kJukyoCode[];   //!< �Z���ԍ��E�}�ԃR�[�h
		extern const _TCHAR kName[];        //!< ����
	}
} // namespace multi_language
} // namespace schema
} // namespace sindy
