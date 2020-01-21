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

#include <map>

// I/N��SiNDY�̃R�[�h�l�̑Ή���\���e�[�u��
typedef long SindyCode;
typedef long INCode;
typedef std::map<SindyCode, INCode> CodeTable;

INCode Sindy2INCode(const CodeTable& cCodeTable, SindyCode cSindyCode);


// I/N HWY�n�e�L�X�g�t�@�C���̃R�[�h�l�Ȃǂ̒�`

namespace in {

namespace schema {

namespace hwy_common_field
{
	namespace facil_class
	{
		enum EFacilClass	// �{�ݎ�ʃR�[�h�̃R�[�h�l
		{					// [Bug6309] �X�}�[�gIC�Ɨ������̃R�[�h�l������
			sa			= 1,	// SA
			pa			= 2,	// PA
			jct			= 3,	// JCT
			ramp		= 4,	// �����v
			ic			= 5,	// IC
			smartic		= 6,	// �X�}�[�gIC
			tollbooth	= 7,	// ������
		};
		extern CodeTable tFacilClass;
	}

	namespace inout
	{
		enum EInOut		// �o�����R�[�h�̃R�[�h�l
		{
			no_inout	= 0, ///< ��ʂȂ�
			in			= 1, ///< �����E����
			out			= 2, ///< �o���E����
		};
		extern CodeTable tInOut;
	}

	namespace facil_class_and_inout_code
	{
		enum EFacilClassAndInOutCode		// �{�݂̎�ނƏo�����̏������킹���u�{�ݎ�ʃR�[�h�v
		{								// fclpoint.txt�́u�{�݃|�C���g��ʃR�[�h�v������ƑS������
			sa			= 2,		// SA
			pa			= 3,		// PA
			jct			= 4,		// JCT
			ramp_ent	= 5,		// �����v����
			ramp_ext	= 6,		// �����v�o��
			ic_ent		= 7,		// IC����
			ic_ext		= 8,		// IC�o��
			tollgate	= 9,		// �{��������
			smartic_ent = 10,		// �X�}�[�gIC����
			smartic_ext = 11,		// �X�}�[�gIC�o��
									// roadpoint_normal.txt�p
			jct_ent		= 30,		// JCT���H
			jct_ext		= 31		// JCT�o�H
		};
	}

}	// namespace hwy_common_field


// hwynode.txt�Ƃ��̔h���t�@�C��(hwynode_inout.txt, tollnode.txt, tollguide.txt)
namespace hwynode
{
	using namespace hwy_common_field;

	namespace file_type
	{
		enum EFileType		// �o�̓t�@�C���̎��
		{
			hwynode			= 1,
			tollnode		= 2,
			tollguide		= 3,
			hwynode_inout	= 4
		};
	}

	namespace file_format
	{
		enum EFormat						// �t�@�C���t�H�[�}�b�g
		{
			hwynode_smartic_added,			/// �X�}�[�gIC�Ή���̃t�H�[�}�b�g(5.61�ȍ~)
			hwynode_etc_specific_toll,		/// ETC�����ʗ����Ή��t�H�[�}�b�g
		};
	}

	namespace toll_func_flags
	{
		enum ETollFuncFlags		// �������@�\�R�[�h�̃t���O��̍�����̌����ƈӖ�
		{
			ticket,					// ������
			adjust,					// ���Z��
			alone,					// �P�Ɨ�����
			check_ticket,			// ���D��
			alone_invalid_ticket,	// �P�Ɨ������������@�\
			num_of_tollfunc_value
		};
	}

	namespace facil_pos_code
	{
		enum EPosition					// �{�݈ʒu�R�[�h�̃R�[�h�l
		{
			ic_ent					= 1,	// IC���H
			ic_ext					= 2,	// IC�o�H
			sa_ent					= 3,	// SA���H
			sa_ext					= 4,	// SA�o�H
			pa_ent					= 5,	// PA���H
			pa_ext					= 6,	// PA�o�H
			jct_ent					= 7,	// JCT���H
			jct_ext					= 8,	// JCT�o�H
			line_tollgate			= 9,	// �{��������
			ramp_ent_tollgate		= 10,	// �����v����H������
			ramp_ext_tollgate		= 11,	// �����v��o�H������
			barrier					= 12,	// �o���A
			smartic_ent				= 13,	// �X�}�[�gIC����
			smartic_ext				= 14,	// �X�}�[�gIC�o��
											/// �ȉ��͏o�����m�[�h��p
			ent_ramp_origin			= 21,	// ���������v�n�_
			ext_ramp_end			= 22,	// �o�������v�I�_
			smartic_ent_ramp_origin = 23,	// �X�}�[�gIC���������v�n�_
			smartic_ext_ramp_end	= 24	// �X�}�[�gIC�o�������v�I�_

		};
	}

	namespace direction
	{
		enum EDirection	// �㉺���R�[�h�̃R�[�h�l
		{
			down	= 0,	// ������(����)
			up		= 1,	// �t����(���)
			nodir	= 2,	// �㉺����ʖ���
			common	= 3		// �㉺������
		};
		extern CodeTable tDirection;
	}

	namespace tollclass
	{
		enum ETollClass		// ��������ʃR�[�h�̃R�[�h�l
		{					// [Bug6309] �l�ƈӖ��̑Ή���ύX
							// SiNDY::TollClass_c�́u�o�����m�[�h�v�́AI/N�ł́u�������ȊO�v�Ɋ܂܂��
			no_tollbooth	= 0,		///< �������ȊO�iSiNDY�ł͕��򍇗��m�[�h�j
			main_tollbooth	= 1,		///< �{���㗿����
			ramp_tollbooth	= 2,		///< �����v�㗿����
			barrier			= 3,		///< �o���A
		};
	}

	namespace etc_facil
	{
		enum EEtcFacil	// ETC�{�݃R�[�h�̃R�[�h�l
		{
			no_etc_facil	= 0,	///< ETC��Ή�
			gate			= 1,	///< �Q�[�g
			antenna			= 2,	///< �A���e�i
		};
	}

}	// namespace hwynode


// roadpoint.txt�Ƃ��̔h���t�@�C��(roadpoint.bunnki.txt, roadpoint_normal.txt)
namespace roadpoint
{
	using namespace hwy_common_field;

/*	namespace file_type
	{
		enum EFileType		// �o�̓t�@�C���̎��
		{
			roadpoint_hwy,		/// HWY
			roadpoint_normal,	/// ��ʗL���i��HWY�j
		};
	}
*/
	namespace file_format
	{
		enum EFormat		// �t�@�C���t�H�[�}�b�g
		{
			roadpoint_smartic_added,		/// �X�}�[�gIC�Ή���̃t�H�[�}�b�g(5.61�ȍ~)
			roadpoint_etc_specific_toll,	/// ETC�����ʗ����Ή��t�H�[�}�b�g
			fclrep,							/// SiRiUS�p�i�{�ݑ�\�|�C���g�j
			fclpoint,						/// SiRiUS�p�i�{�݃|�C���g�j
		};
	}

	namespace jct_data_type
	{
		enum EJctDataType	// JCT�̍����_�̃��R�[�h�𕪊�_�̃��R�[�h�ɒu���������f�[�^�ł��邩�ۂ�
		{
			jct_interflow,		/// �����i�ʏ�̃f�[�^�j
			jct_branch,			/// ����i�����_�𕪊�_�ɕς����f�[�^�j
		};
	}

	namespace point_pos_code
	{
		enum EPointPos		// �|�C���g�ʒu�R�[�h�̃R�[�h�l
		{
			entrance	= 0,			// �����������͏o��
			up			= 1,			// ���
			down		= 2,			// ����
			in			= 3,			// �����
			out			= 4,			// �O���
			west		= 5,			// ���s��
			east		= 6,			// ���s��
			direction	= 7				// �~�~����
		};
	}

}	// namespace roadpoint

namespace fclrep
{
	namespace searchclass
	{
		enum EFacilClass	// fclrep.txt�p�̎{�ݎ�ʃR�[�h�i�����p�{�ݎ�ʃR�[�h�j�̃R�[�h�l
		{					// roadpoint.txt�̎{�ݎ�ʃR�[�h�Ƃ́A�X�}�[�gIC�Ɨ������̃R�[�h�l���t�Ȃ̂Œ���
			not_for_search	= 0,	// �����ΏۊO�{��
			sa				= 1,	// SA
			pa				= 2,	// PA
			jct				= 3,	// JCT
			ramp			= 4,	// �����v
			ic				= 5,	// IC
			tollbooth		= 6,	// ������
			smartic			= 7,	// �X�}�[�gIC
		};
		extern CodeTable tSearchClass;
	}

} // namespace fclrep


namespace fclpoint
{
	namespace direction
	{
		enum EDirection	// �㉺���R�[�h�̃R�[�h�l
		{				// hwynode.txt�̏㉺���R�[�h�Ƃ́A�R�[�h�l�̏������قȂ�̂Œ���
			nodir	= 0,	// �����Ȃ�
			up		= 1,	// ���
			down	= 2,	// ����
			common	= 3		// �㉺������
		};
		extern CodeTable tDirection;
	}

}	// namespace fclpoint


namespace highway
{
	using namespace hwy_common_field;

	namespace file_format
	{
		enum EFormat
		{
			highway_smartic_added,		/// �X�}�[�gIC�Ή���̃t�H�[�}�b�g(5.61�ȍ~)
			highway_etc_specific_toll,		/// ETC�����ʗ����Ή��t�H�[�}�b�g
		};
	}

	namespace direction
	{
		enum EDirection
		{
			fore		= 0,		// �������i�ʏ�͉���j
			back		= 1			// �t�����i�ʏ�͏��j
		};
	}

}	// namespace highway


namespace road_code_doc
{
	namespace file_type
	{
		enum EFileType
		{
			road_code			= 1,
			ferry_code			= 2,
			road_code_highway	= 3,
			road_code_display	= 4
		};
	}

	namespace file_format
	{
		enum EFormat	/// �t�@�C���ԂŃt�H�[�}�b�g�̈Ⴂ������̂�SiRiUS�p�̂�
		{
			roadtype			= 1,
			roadline			= 2
		};
	}

	namespace line_class
	{
		enum ELineClass
		{
			ferry = 1,			// �t�F���[�q�H
			street,				// ��ʊX������
			ordinary_tollroad,	// ��ʗL�����H
			highway,			// �������H
			tollroad,			// �L�����H
			metropolis,			// ��s�������H
			hanshin,			// ��_�������H
			nagoya,				// ���É��s�s�������H
			fukuoka,			// �����s�s�������H
			kitakyushu,			// �k��B�s�s�������H
			hiroshima,			// �L���������H
			kyouto,				// ���s�������H
		};
		extern CodeTable tLineClass;
		extern CodeTable tLineClassForSiRiUS;
	}

}	// namespace road_code_doc

}	// namespace schema

}	// namespace in

