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

#ifndef ADDRESS_SCHEMA_H_
#define ADDRESS_SCHEMA_H_

#include "stdafx.h"

namespace address {

namespace schema {

// 住所マスタ
namespace areacode_master {

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kKenCode[];			//!< 都道府県コード
	extern const _TCHAR kShiCode[];			//!< 市区町村コード
	extern const _TCHAR kOazaCode[];		//!< 大字・通称コード
	extern const _TCHAR kAzaCode[];			//!< 字・丁目コード
	extern const _TCHAR kNewKenCode[];		//!< 新都道府県コード
	extern const _TCHAR kNewShiCode[];		//!< 新市区郡町村コード
	extern const _TCHAR kNewOazaCode[];		//!< 新大字・通称コード
	extern const _TCHAR kNewAzaCode[];		//!< 新字・丁目コード
	extern const _TCHAR kZipCode[];			//!< 郵便番号
	extern const _TCHAR kZipCount[];		//!< 郵便番号個数
	extern const _TCHAR kSeireishiFlag[];	//!< 政令指定都市フラグ
	extern const _TCHAR kKenKana[];			//!< 都道府県名カナ読み
	extern const _TCHAR kShiKana[];			//!< 市区郡町村名カナ読み
	extern const _TCHAR kOazaKana[];		//!< 大字・通称名カナ読み
	extern const _TCHAR kAzaKana[];			//!< 字名・丁目カナ読み
	extern const _TCHAR kKenKanji[];		//!< 都道府県名
	extern const _TCHAR kShiKanji[];		//!< 市区郡町村名
	extern const _TCHAR kOazaKanji[];		//!< 大字・通称名
	extern const _TCHAR kAzaKanji[];		//!< 字名・丁目
	extern const _TCHAR kKenKanjiType[];	//!< ［外字情報］都道府県名
	extern const _TCHAR kShiKanjiType[];	//!< ［外字情報］市区郡町村名
	extern const _TCHAR kOazaKanjiType[];	//!< ［外字情報］大字・通称名
	extern const _TCHAR kAzaKanjiType[];	//!< ［外字情報］字名・丁目
	extern const _TCHAR kShikoDate[];		//!< 施行年月
	extern const _TCHAR kHaishiDate[];		//!< 廃止年月
	extern const _TCHAR kTyoumeiDate[];		//!< 町名変更年月
	extern const _TCHAR kZipCodeDate[];		//!< 郵便番号変更年月
	extern const _TCHAR kJukyoDate[];		//!< 住居表示実施年月
	extern const _TCHAR kJukyoFlag[];		//!< 住居表示実施フラグ
	extern const _TCHAR kRecordFlag[];		//!< レコード区分
	extern const _TCHAR kTsushoFlag[];		//!< 通称フラグ
	extern const _TCHAR kTorinaFlag[];		//!< 通り名識別
	extern const _TCHAR kOazaFlag[];		//!< 大字・字フラグ
	extern const _TCHAR kAzaFlag[];			//!< 字・小字フラグ
	extern const _TCHAR kOldZipCode[];		//!< 旧5桁郵便番号
	extern const _TCHAR kShuseiCode[];		//!< 修正コード


	// 外字情報
	namespace kanji_type {
		enum ECode {
			kNone	= 0,	//!< 文字が入っていない
			kJis	= 1,	//!< JISの第１・２水準のみで構成
			kOther	= 2,	//!< JIS以外の文字が含まれる
		};
	}

	// 大字・字・小字フラグ
	namespace oaza_aza_flag {
		enum ECode {
			kNone	= 0,		//!< 何もつかない
			kOaza	= 1,		//!< 漢字名称/カナ名称の先頭が「大字」/「ｵｵｱｻﾞ」
			kAza	= 2,		//!< 漢字名称/カナ名称の先頭が「字」/「ｱｻﾞ」
			kKoaza	= 3,		//!< 漢字名称/カナ名称の先頭が「小字」/「ｺｱｻﾞ」
		};
	}

} // areacode_master

// 全角読み仮名テーブル
namespace areacode_kana {

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kKenCode[];			//!< 都道府県コード
	extern const _TCHAR kShiCode[];			//!< 市区町村コード
	extern const _TCHAR kOazaCode[];		//!< 大字・通称コード 
	extern const _TCHAR kAzaCode[];			//!< 字・丁目コード
	extern const _TCHAR kKenKanaD[];		//!< 都道府県名全角カナ読み
	extern const _TCHAR kShiKanaD[];		//!< 市区郡町村名全角カナ読み
	extern const _TCHAR kOazaKanaD[];		//!< 大字・通称名全角カナ読み
	extern const _TCHAR kAzaKanaD[];		//!< 字名・丁目全角カナ読み
} // areacode_kana

} // schema

} // address

#endif // ADDRESS_SCHEMA_H_
