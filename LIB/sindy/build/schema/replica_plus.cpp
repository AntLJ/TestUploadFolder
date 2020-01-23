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

#include "stdafx.h"
#include <sindy/schema/replica_plus.h>

namespace replica_plus {

namespace schema
{
	// デフォルト多言語注記
	namespace mlang_default_anno {
		const _TCHAR kTableName[] = _T("mlang_default_anno");

		const _TCHAR kMulti_F[] = _T("multi_f");				//!< 複数名称保持フラグ
		const _TCHAR kAnnoClass_C[] = _T("annoclass_c");		//!< 注記種別
		const _TCHAR kAnnoClassName[] = _T("annoclass_name");	//!< 注記名称
		const _TCHAR kMatchingStr[] = _T("matching_str");		//!< マッチング文字列
		const _TCHAR kLangCode[] = _T("langcode");				//!< 言語コード
		const _TCHAR kName[] = _T("name");						//!< 名称
	} // namespace mlang_default_anno

	// 多言語共通
	namespace mlang_common {
		const _TCHAR kNameKanji[] = _T("name_kanji");		//!< 日本語名称
		const _TCHAR kNameYomi[] = _T("name_yomi");			//!< 日本語読み
		const _TCHAR kLangCode[] = _T("langcode");			//!< 言語コード
		const _TCHAR kName[] = _T("name");					//!< 名称
	}

	// 多言語交差点名称
	namespace mlang_road_node {
		const _TCHAR kTableName[] = _T("mlang_road_node");

		const _TCHAR kNodeId[] = _T("node_id");		//!< 道路ノードID
	} // namespace mlang_road_node

	// 多言語方面案内
	namespace mlang_inf_dirguide {
		const _TCHAR kTableName[] = _T("mlang_inf_dirguide");

	} // namespace mlang_inf_dirguide

}

}
