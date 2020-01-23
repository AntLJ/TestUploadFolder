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

/// 注記真位置テーブル
namespace annotation {
	using namespace ::sindy::schema::ipc_table;

	/// 注記種別コード
	namespace annotation_code {
		enum ECode {
#define ANNOTATION_CODE(name,id,size,label) name = id,
#include "annotationcode.h"
#undef ANNOTATION_CODE
		};
	} // annotation_code

	namespace disp_type {
		enum ECode {
			kNone	=        0, //!< 表示なし
			kStr	=        1, //!< 文字列のみ
			kSymStr	=        2, //!< 記号＋文字列
			kPoiStr	=        3, //!< 指示点＋文字列
			kSym	=        4, //!< 記号のみ
		};
	} // disp_type

	// TODO:注記に共通しない属性もありますので、切り分ける必要があります。
	// 現時点では影響がわからないためそのままにしておきます。(2013/05/30 ta_suga)
	// 注記文字列３、注記３文字数は都市地図にはなく、表示タイプも都市地図用とそれ以外のものは異なります。

	extern const _TCHAR  kNameString1[];			//!< 注記文字列１
	extern const _TCHAR  kNameString2[];			//!< 注記文字列２
	extern const _TCHAR  kNameString3[];			//!< 注記文字列３
	extern const _TCHAR  kStringNum1[];				//!< 注記１文字数
	extern const _TCHAR  kStringNum2[];				//!< 注記２文字数
	extern const _TCHAR  kStringNum3[];				//!< 注記３文字数
	extern const _TCHAR  kAnnoCode[];				//!< 注記種別
	extern const _TCHAR	 kNameYomi[];				//!< 注記読み
	extern const _TCHAR  kDispType[];				//!< 表示タイプ（都市地図用）
	extern const _TCHAR  kSC1DispType[];			//!< 表示タイプ（スケール1）
	extern const _TCHAR  kSC2DispType[];			//!< 表示タイプ（スケール2）
	extern const _TCHAR  kSC3DispType[];			//!< 表示タイプ（スケール3）
	extern const _TCHAR  kSC4DispType[];			//!< 表示タイプ（スケール4）
	extern const _TCHAR  kContentsCode[];			//!< コンテンツコード
	extern const _TCHAR  kContentsSeq[];			//!< コンテンツ内シーケンス
	extern const _TCHAR  kAnnoDisp[];				//!< 注記表示コード
} // annotation

/// POI関連注記真位置テーブル
namespace annotation_rel_poi {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kContentsSeq[];  //!< コンテンツ内シーケンス
	extern const _TCHAR kAnnoDispCode[]; //!< 注記表示コード
}

/// 注記表示位置テーブル
namespace disp_line {
	using namespace ::sindy::schema::ipc_table;

	namespace font_size {
		enum ECode {
			k10 =		10,	//!< １０ポイント
			k12	=       12, //!< １２ポイント
			k14	=       14, //!< １４ポイント
			k16	=       16, //!< １６ポイント
			k20	=       20, //!< ２０ポイント
			k24	=       24, //!< ２４ポイント
		};
	} // font_size

	namespace target_str {
		enum ECode {
			kStr1	=        1, //!< 文字列1を参照
			kStr2	=        2, //!< 文字列2を参照
			kStr3	=        3, //!< 文字列3を参照
		};
	} // target_str

	extern const _TCHAR  kFontSize[];			//!< フォントサイズ
	extern const _TCHAR  kAnnoID[];				//!< 対応注記文字ID
	extern const _TCHAR  kTargetStr[];			//!< 対象文字列コード
} // disp_line

/// 都市地図注記真位置テーブル
namespace city_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // city_annotation

/// 1/6250注記真位置テーブル
namespace d1_annotation {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR  kTableName[];
} // d1_annotation

/// 中縮注記真位置テーブル
namespace base_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // base_annotation

/// ミドル注記真位置テーブル
namespace middle_annotation {
	using namespace ::sindy::schema::annotation_rel_poi;

	extern const _TCHAR  kTableName[];
} // middle_annotation

/// 大縮注記真位置テーブル
namespace top_annotation {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR  kTableName[];
} // top_annotation

/// 大縮注記真位置テーブル JPN
namespace top_annotation_jpn {
	using namespace ::sindy::schema::annotation;

	extern const _TCHAR kTableName[];
} // top_annotation_jpn

/// 都市地図注記表示位置テーブル
namespace city_disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // city_disp_line

/// 1/6250注記表示位置テーブル
namespace c_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // c_sc1disp_line

/// 中縮S4表示位置テーブル
namespace b_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc4disp_line

/// 中縮S3表示位置テーブル
namespace b_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc3disp_line

/// 中縮S2表示位置テーブル
namespace b_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc2disp_line

/// 中縮S1表示位置テーブル
namespace b_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // b_sc1disp_line

/// ミドルS4表示位置テーブル
namespace m_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc4disp_line

/// ミドルS3表示位置テーブル
namespace m_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc3disp_line

/// ミドルS2表示位置テーブル
namespace m_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc2disp_line

/// ミドルS1表示位置テーブル
namespace m_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // m_sc1disp_line

/// 大縮S4表示位置テーブル
namespace t_sc4disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc4disp_line

/// 大縮S3表示位置テーブル
namespace t_sc3disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc3disp_line

/// 大縮S2表示位置テーブル
namespace t_sc2disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc2disp_line

/// 大縮S1表示位置テーブル
namespace t_sc1disp_line {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc1disp_line

/// 大縮S4表示位置テーブル JPN
namespace t_sc4disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc4disp_line_jpn

/// 大縮S3表示位置テーブル JPN
namespace t_sc3disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc3disp_line_jpn

/// 大縮S2表示位置テーブル JPN
namespace t_sc2disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc2disp_line_jpn

/// 大縮S1表示位置テーブル JPN
namespace t_sc1disp_line_jpn {
	using namespace ::sindy::schema::disp_line;

	extern const _TCHAR  kTableName[];
} // t_sc1disp_line_jpn


} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ANNOTATION_H_
