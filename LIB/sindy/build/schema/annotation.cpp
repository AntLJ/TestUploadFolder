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
#include <sindy/schema/annotation.h>

namespace sindy {

namespace schema {

namespace annotation {
	const _TCHAR kNameString1[]		= _T("NAME_STRING1");		//!< 注記文字列２
	const _TCHAR kNameString2[]		= _T("NAME_STRING2");		//!< 注記文字列２
	const _TCHAR kNameString3[]		= _T("NAME_STRING3");		//!< 注記文字列３
	const _TCHAR kStringNum1[]		= _T("STRING_NUM1");		//!< 注記１文字数
	const _TCHAR kStringNum2[]		= _T("STRING_NUM2");		//!< 注記２文字数
	const _TCHAR kStringNum3[]		= _T("STRING_NUM3");		//!< 注記３文字数
	const _TCHAR kAnnoCode[]		= _T("ANNOCLASS_C");		//!< 注記種別
	const _TCHAR kNameYomi[]		= _T("NAME_YOMI");			//!< 注記読み
	const _TCHAR kDispType[]		= _T("DISPTYPE_C");			//!< 表示タイプ（都市地図用）
	const _TCHAR kSC1DispType[]		= _T("SC1DISPTYPE_C");		//!< 表示タイプ（スケール1）
	const _TCHAR kSC2DispType[]		= _T("SC2DISPTYPE_C");		//!< 表示タイプ（スケール2）
	const _TCHAR kSC3DispType[]		= _T("SC3DISPTYPE_C");		//!< 表示タイプ（スケール3）
	const _TCHAR kSC4DispType[]		= _T("SC4DISPTYPE_C");		//!< 表示タイプ（スケール4）
	const _TCHAR kContentsCode[]	= _T("CONTENTS_CODE");		//!< コンテンツコード
	const _TCHAR kContentsSeq[]		= _T("CONTENTS_SEQ");		//!< コンテンツ内シーケンス
	const _TCHAR kAnnoDisp[]		= _T("ANNO_DISP_C");		//!< 注記表示コード
} // annotation

namespace annotation_rel_poi {
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kContentsSeq[]  = _T("CONTENTS_SEQ"); //!< コンテンツ内シーケンス
	const _TCHAR kAnnoDispCode[] = _T("ANNO_DISP_C"); //!< 注記表示コード
} // annotation_rel_poi

namespace disp_line {
	const _TCHAR kFontSize[]		= _T("FONTSIZE");			//!< フォントサイズ
	const _TCHAR kAnnoID[]			= _T("ANNO_ID");			//!< 対応注記文字ID
	const _TCHAR kTargetStr[]		= _T("TARGET_STR_C");		//!< 対象文字列コード
} // disp_line

const _TCHAR city_annotation::kTableName[]		= _T("CITY_ANNOTATION");	//!< テーブル名
const _TCHAR d1_annotation::kTableName[]		= _T("D1_ANNOTATION");		//!< テーブル名
const _TCHAR base_annotation::kTableName[]		= _T("BASE_ANNOTATION");	//!< テーブル名
const _TCHAR middle_annotation::kTableName[]	= _T("MIDDLE_ANNOTATION");	//!< テーブル名
const _TCHAR top_annotation::kTableName[]		= _T("TOP_ANNOTATION");		//!< テーブル名
const _TCHAR top_annotation_jpn::kTableName[]		= _T("TOP_ANNOTATION_JPN");	//!< テーブル名
const _TCHAR city_disp_line::kTableName[]		= _T("CITY_DISP_LINE");
const _TCHAR c_sc1disp_line::kTableName[]		= _T("C_SC1DISP_LINE");
const _TCHAR b_sc1disp_line::kTableName[]		= _T("B_SC1DISP_LINE");
const _TCHAR b_sc2disp_line::kTableName[]		= _T("B_SC2DISP_LINE");
const _TCHAR b_sc3disp_line::kTableName[]		= _T("B_SC3DISP_LINE");
const _TCHAR b_sc4disp_line::kTableName[]		= _T("B_SC4DISP_LINE");
const _TCHAR m_sc1disp_line::kTableName[]		= _T("M_SC1DISP_LINE");
const _TCHAR m_sc2disp_line::kTableName[]		= _T("M_SC2DISP_LINE");
const _TCHAR m_sc3disp_line::kTableName[]		= _T("M_SC3DISP_LINE");
const _TCHAR m_sc4disp_line::kTableName[]		= _T("M_SC4DISP_LINE");
const _TCHAR t_sc1disp_line::kTableName[]		= _T("T_SC1DISP_LINE");
const _TCHAR t_sc2disp_line::kTableName[]		= _T("T_SC2DISP_LINE");
const _TCHAR t_sc3disp_line::kTableName[]		= _T("T_SC3DISP_LINE");
const _TCHAR t_sc4disp_line::kTableName[]		= _T("T_SC4DISP_LINE");
const _TCHAR t_sc1disp_line_jpn::kTableName[]		= _T("T_SC1DISP_LINE_JPN");
const _TCHAR t_sc2disp_line_jpn::kTableName[]		= _T("T_SC2DISP_LINE_JPN");
const _TCHAR t_sc3disp_line_jpn::kTableName[]		= _T("T_SC3DISP_LINE_JPN");
const _TCHAR t_sc4disp_line_jpn::kTableName[]		= _T("T_SC4DISP_LINE_JPN");

} // schema

} // sindy
