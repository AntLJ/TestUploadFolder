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
	extern const _TCHAR kObjectId[];		//!< オブジェクトID
}

namespace update_info {
	extern const _TCHAR kUpdateInfo[];
}

// 多言語用共通
namespace common_mlang {
	extern const _TCHAR kId[];				//!< 対応オブジェクトID
	extern const _TCHAR kLangCode[];		//!< 言語コード
	namespace lang_code {
		using namespace replica_plus::schema::mlang_annotation::lang_code;
	}
	extern const _TCHAR kName[];			//!< 名称
} // namespace common_mlang

// 多言語注記用共通
namespace common_mlang_anno {
	using namespace common_mlang;
	extern const _TCHAR kDefaultName[];		//!< デフォルト名称
} // common_mlang_anno

// 多言語トップ注記
namespace ml_top_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_top_annotation

// 多言語ミドル注記
namespace ml_middle_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_middle_annotation

// 多言語ベース注記
namespace ml_base_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_base_annotation

// 多言語都市地図注記
namespace ml_city_annotation {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang_anno;
} // ml_city_annotation

// 多言語交差点名称
namespace ml_road_node {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang;
} // ml_road_node

// 多言語方面案内
namespace ml_inf_dirguide {
	extern const _TCHAR kTableName[];
	
	using namespace common_mlang;
	extern const _TCHAR kNameSeq[];
} // ml_inf_dirguide

// リンク列共通部
namespace common_linkqueue {
	extern const _TCHAR kInfId[];
	extern const _TCHAR kLinkId[];
	extern const _TCHAR kLinkDir[];
	extern const _TCHAR kSequence[];
} // common_linkqueue

// イラスト情報
namespace inf_illust {
	extern const _TCHAR kTableName[];
	
	using namespace objectid;
	extern const _TCHAR kIllust[];
	extern const _TCHAR kFileName[];
} // inf_illust

// イラストリンク列
namespace lq_illust {
	extern const _TCHAR kTableName[];

	using namespace common_linkqueue;
} // lq_illust

} // schema

} // egg

#endif // ifndef SINDY_SCHEMA_EGG_H_
