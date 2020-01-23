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

#ifndef SINDY_SCHEMA_COMMON_H_
#define SINDY_SCHEMA_COMMON_H_

#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace object {
	extern const _TCHAR kObjectID[]; ///< �I�u�W�F�N�gID
}

namespace feature {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kShape[];
}

namespace ipc_table {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kOperator[]; ///< �ŏI�X�V��
	extern const _TCHAR kPurpose[]; ///< ��ƖړI
	extern const _TCHAR kModifyDate[]; ///< �ŏI�X�V����
	extern const _TCHAR kUpdateType[]; ///< �ŏI�X�V���e
	extern const _TCHAR kProgModifyDate[]; ///< �ŏI�v���O�����X�V����
	extern const _TCHAR kModifyProgName[]; ///< �ŏI�X�V�v���O������
	extern const _TCHAR kUserClaim[]; ///< ���[�U�N���[���t���O
	extern const _TCHAR kSource[]; ///< ���\�[�X

	namespace update_type {
		enum ECode {
			kDefault,       ///< �ڍs������
			kCreated,       ///< �V�K�쐬
			kDeleted,       ///< �폜
			kShapeUpdated,  ///< �`��ύX
			kAttrUpdated,   ///< �����ύX
			kiPSImported,   ///< ��C�C���|�[�g
			kImported,      ///< �C���|�[�g
		};
	}
}

namespace ipc_feature {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::feature;
}

namespace tmp_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTmpMeshCode[]; ///< vdb���b�V���R�[�h
	extern const _TCHAR kTmpPrimID[]; ///< vdb�v���~�e�B�uID
}

namespace fieldsurvey_table {
	using namespace ::sindy::schema::tmp_info;

	extern const _TCHAR kFieldSurvey[]; ///< ���n�����t���O
	extern const _TCHAR kWorkYear[]; ///< ���n�����N�x
}

namespace meshcode_class {
	using namespace ::sindy::schema::tmp_info;
	using namespace ::sindy::schema::category::mesh_code_table;
}

namespace ipcfeature {
	using namespace ::sindy::schema::tmp_info;
}

namespace link_queue {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;

	extern const _TCHAR kInfID[]; ///< �����N��ID
	extern const _TCHAR kSequence[]; ///< �����N�񏇔�
	extern const _TCHAR kLastLink[]; ///< �I�[�����N�t���O
}

namespace changetypecode {

	enum Code {
		kInsert       = 1,  //!< INSERT
		kDelete       = 2,  //!< DELETE
		kAfterUpdate  = 3,  //!< UPDATE(AFTER)
		kBeforeUpdate = 4,  //!< UPDATE(BEFORE)
	};
}

namespace countrycode {

	enum Code {
		kUndefined = 0,
		kAustralia = 36,
		kBangladesh = 50,
		kBhutan = 64,
		kBrunei_Darussalam = 96,
		kMyanmar = 104,
		kCambodia = 116,
		kChina = 156,
		kTaiwan = 158,
		kHong_Kong = 344,
		kIndia = 356,
		kIndonesia = 360,
		kJapan = 392,
		kLaos = 418,
		kMacao = 446,
		kMalaysia = 458,
		kFederated_States_of_Micronesia = 583,
		kPalau = 585,
		kPapua_New_Guinea = 598,
		kPhilippines = 608,
		kTimor_Leste = 626,
		kSingapore = 702,
		kVietnam = 704,
		kThailand = 764,
	};
}

/* ============================================================================== */
/// ADAM v2�p
namespace adam_v2_global_ipc_feature {
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kGlobalID[];		///< GLOBALID
}

namespace adam_v2_adam_ipc_feature {
	using namespace ::sindy::schema::adam_v2_global_ipc_feature;

	extern const _TCHAR kSourceID[];			///< �񋟌�ID
	extern const _TCHAR kSperculationC[];		///< �����R�[�h
	extern const _TCHAR kSourceLot[];	    	///< �񋟌����b�g
	extern const _TCHAR kSourceUpdateC[];		///< �񋟌��X�V�R�[�h


	namespace speculation_cpde {
		enum ECode {
			kNotSperculation	= 1,			///< �����l�łȂ�
			kSperculation		= 2,			///< �����l�ł���
		};
	}

	namespace source_update_code{
		enum ECode
		{
			kNotSet   = -1,			///< ���ݒ�
			kNoModify = 0,			///< �ύX�Ȃ�
			kAdd      = 1,          ///< �ǉ�
			kDelete   = 2,          ///< �폜
			kUpdate   = 3,          ///< �����X�V
		};

	}
}

namespace adam_v2_table {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kSourceID[];			///< �񋟌�ID
	extern const _TCHAR kSourceLot[];	    	///< �񋟌����b�g
	extern const _TCHAR kSourceUpdateC[];		///< �񋟌��X�V�R�[�h

	namespace source_update_code{
		using namespace ::sindy::schema::adam_v2_adam_ipc_feature::source_update_code;
	}
}

namespace adam_v2_global_table {
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kGlobalID[];		///< GLOBALID
}

namespace adam_v2_node_queue {
	using namespace ::sindy::schema::adam_v2_global_table;

	extern const _TCHAR kSequence[];		///< ����
	extern const _TCHAR kLastNodeF[];		///< �I�[�m�[�h�t���O
}
/* ============================================================================== */

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_COMMON_H_
