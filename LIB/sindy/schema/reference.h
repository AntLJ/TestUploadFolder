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

#ifndef SINDY_SCHEMA_REFERENCE_H_
#define SINDY_SCHEMA_REFERENCE_H_

#include <sindy/schema/category.h>
#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

/// �s�s�n�}���b�V���e�[�u��
namespace citymesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityMeshCode[];		//!< ���b�V���R�[�h(���C�u�����g�p���Ă��鑼�c�[���Ő�����邽�߁A�c���Ă��� 2007/10/16 s_suzuki)
	extern const _TCHAR kCityName[];			//!< �s�撬����
	extern const _TCHAR kCreateYear[];			//!< �쐬�N�x
	extern const _TCHAR kUpdateYear[];			//!< ��ƔN�x
	extern const _TCHAR kComplete[];			//!< ���S���t���O
	extern const _TCHAR kSea[];					//!< �C�t���O
	extern const _TCHAR kZuka[];				//!< �}���t���O
	extern const _TCHAR kRaster[];				//!< ���X�^�[
	extern const _TCHAR kCreateRasterYear[];	//!< ���X�^�[�쐬�N�x
	extern const _TCHAR kCurrentAdjYear[];		//!< �����C���N�x
	extern const _TCHAR kPhotoShotDate[];		//!< �g�p�ʐ^�B�e�N����
	extern const _TCHAR kFieldResearchYear[];	//!< ���n�������{�N�x�i�n�}�j
	extern const _TCHAR kTrSurveyYear[];		//!< ���n�������{�N�x�i�K���j
	extern const _TCHAR kShaBgworkDate[];		//!< ��C��ƔN���i���ʁj
	extern const _TCHAR kShaAnnoworkDate[];		//!< ��C��ƔN���i���L�j
	extern const _TCHAR kCreateAnnoMsDate[];	//!< ���L���e�쐬�N��
	extern const _TCHAR kAddrMainteYear[];		//!< �Z�������������N�x
	extern const _TCHAR kJmapPublicationYear[];	//!< �Q��j�n�}���s�N�x
	extern const _TCHAR kMapcubeWorkYear[];		//!< mapcube��ƔN�x
	extern const _TCHAR kPdmUseYear[];			//!< pdm�g�p�N�x
} // citymesh

/// �x�[�X���b�V���t�B�[�`���N���X
namespace basemesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �~�h�����b�V���e�[�u��
namespace middlemesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kMiddleMeshCode[];		//!< ���b�V���R�[�h(���C�u�����g�p���Ă��鑼�c�[���Ő�����邽�߁A�c���Ă��� 2007/10/16 s_suzuki)
} // middlemesh

/// �g�b�v���b�V���t�B�[�`���N���X
namespace topmesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			///< �e�[�u����
}

/// �Q�ƗpVICS�t�B�[�`���N���X
namespace org_vics {
	using namespace ::sindy::schema::category::original_vics_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �Q�Ɨp�C���^�[�i�rVICS�t�B�[�`���N���X
namespace ivics {
	using namespace ::sindy::schema::category::original_vics_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �Q�Ɨp����₷���}�b�v�t�B�[�`���N���X
namespace drive_map {
	using namespace ::sindy::schema::category::drive_map_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// TOWNPAGE�t�B�[�`���N���X
namespace townpage {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kIpCode[];		//!< IP�R�[�h
	extern const _TCHAR kDataKubun[];		//!< �f�[�^�擾�敪
	extern const _TCHAR kDataSeq[];		//!< �f�[�^�擾�V�[�P���X
	extern const _TCHAR kPstIpCode[];		//!< ���W�񋟉�ЃR�[�h
	extern const _TCHAR kAccCode[];		//!< ���x�R�[�h
	extern const _TCHAR kPinPointFlag[];	//!< �s���|�C���g�t���O
	extern const _TCHAR kPrefCode[];		//!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];		//!< �s�撬���R�[�h
	extern const _TCHAR kAdrCode1[];		//!< �厚�E�ʏ̃R�[�h
	extern const _TCHAR kAdrCode2[];		//!< ���E���ڃR�[�h
	extern const _TCHAR kChibanCode[];	//!< �n�ԃR�[�h
	extern const _TCHAR kJukyoCode[];		//!< �Z���ԍ��R�[�h
	extern const _TCHAR kNttGrpCode[];	//!< NTT���ރR�[�h
	extern const _TCHAR kNttCode[];		//!< NTT�Ǝ�R�[�h
	extern const _TCHAR kChainCode[];		//!< �X�܃R�[�h
	extern const _TCHAR kName[];			//!< �\���p����
	extern const _TCHAR kYomi[];			//!< �\���p���̓ǂ�
	extern const _TCHAR kTelC[];			//!< �s�X�ǔ�
	extern const _TCHAR kTel[];			//!< �d�b�ԍ�
	extern const _TCHAR kNaCode[];		//!< �g�p�֎~���R�R�[�h
}

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_REFERENCE_H_
