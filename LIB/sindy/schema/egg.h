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

#ifndef SINDY_SCHEMA_EGG_H_
#define SINDY_SCHEMA_EGG_H_

#include "replica_plus.h"

namespace egg {

namespace schema {

namespace objectid {
	extern const _TCHAR kObjectId[];		//!< �I�u�W�F�N�gID
}

namespace update_info {
	extern const _TCHAR kUpdateInfo[];
}

// ������p����
namespace common_mlang {
	extern const _TCHAR kId[];				//!< �Ή��I�u�W�F�N�gID
	extern const _TCHAR kLangCode[];		//!< ����R�[�h
	namespace lang_code {
		using namespace replica_plus::schema::mlang_annotation::lang_code;
	}
	extern const _TCHAR kName[];			//!< ����
} // namespace common_mlang

// �����꒍�L�p����
namespace common_mlang_anno {
	using namespace common_mlang;
	extern const _TCHAR kDefaultName[];		//!< �f�t�H���g����
} // common_mlang_anno

// ������g�b�v���L
namespace ml_top_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_top_annotation

// ������~�h�����L
namespace ml_middle_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_middle_annotation

// ������x�[�X���L
namespace ml_base_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_base_annotation

// ������s�s�n�}���L
namespace ml_city_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_city_annotation

// ����������_����
namespace ml_road_node {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang;
} // ml_road_node

// ��������ʈē�
namespace ml_inf_dirguide {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang;
	extern const _TCHAR kNameSeq[];
} // ml_inf_dirguide

// �����N�񋤒ʕ�
namespace common_linkqueue {
	extern const _TCHAR kInfId[];
	extern const _TCHAR kLinkId[];
	extern const _TCHAR kLinkDir[];
	extern const _TCHAR kSequence[];
} // common_linkqueue

// �C���X�g���
namespace inf_illust {
	extern const _TCHAR kTableName[];
	
	using namespace objectid;
	extern const _TCHAR kIllust[];
	extern const _TCHAR kFileName[];
} // inf_illust

// �C���X�g�����N��
namespace lq_illust {
	extern const _TCHAR kTableName[];

	using namespace common_linkqueue;
} // lq_illust

} // schema

} // egg

#endif // ifndef SINDY_SCHEMA_EGG_H_
