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

#ifndef SINDY_SCHEMA_REPPLUS_H_
#define SINDY_SCHEMA_REPPLUS_H_

#include "multi_language.h"

namespace replica_plus {

namespace schema {

	// 多言語注記
	namespace mlang_annotation {
		using namespace ::sindy::schema::multi_language::annotation;
	} // namespace mlang_annotation

	// デフォルト多言語注記
	namespace mlang_default_anno {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMulti_F[];			//!< 複数名称保持フラグ
		extern const _TCHAR kAnnoClass_C[];		//!< 注記種別
		namespace display_layer {
			using namespace sindy::schema::multi_language::annotation::display_layer;
		}
		extern const _TCHAR kAnnoClassName[];	//!< 注記名称
		extern const _TCHAR kMatchingStr[];		//!< マッチング文字列
		extern const _TCHAR kLangCode[];		//!< 言語コード
		namespace lang_code {
			using namespace sindy::schema::multi_language::ipc_mlang_table::lang_code;
		}
		extern const _TCHAR kName[];			//!< 名称
	} // namespace mlang_default_anno

	namespace mlang_common {
		extern const _TCHAR kNameKanji[];		//!< 日本語名称
		extern const _TCHAR kNameYomi[];		//!< 日本語読み
		extern const _TCHAR kLangCode[];		//!< 言語コード
		namespace lang_code {
			using namespace sindy::schema::multi_language::ipc_mlang_table::lang_code;
		}
		extern const _TCHAR kName[];			//!< 名称
	}

	// 多言語交差点名称
	namespace mlang_road_node {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNodeId[];			//!< 道路ノードID
		using namespace replica_plus::schema::mlang_common;
	} // namespace mlang_road_node

	// 多言語方面案内
	namespace mlang_inf_dirguide {
		extern const _TCHAR kTableName[];

		using namespace replica_plus::schema::mlang_common;
	} // namespace mlang_inf_dirguide

} // schema

} // replica_plus

#endif // ifndef SINDY_SCHEMA_REPPLUS_H_
