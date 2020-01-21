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
#include "AddrSchema.h"

namespace address {

namespace schema {

// 住所マスタ
namespace areacode_master {

	extern const _TCHAR kTableName[]		= _T("AREACODE_MASTER");	//!< テーブル名

	extern const _TCHAR kKenCode[]			= _T("KEN_CODE");			//!< 都道府県コード
	extern const _TCHAR kShiCode[]			= _T("SHI_CODE");			//!< 市区町村コード
	extern const _TCHAR kOazaCode[]			= _T("OAZA_CODE");			//!< 大字・通称コード 
	extern const _TCHAR kAzaCode[]			= _T("AZA_CODE");			//!< 字・丁目コード
	extern const _TCHAR kNewKenCode[]		= _T("NEW_KEN_CODE");		//!< 新都道府県コード
	extern const _TCHAR kNewShiCode[]		= _T("NEW_SHI_CODE");		//!< 新市区郡町村コード
	extern const _TCHAR kNewOazaCode[]		= _T("NEW_OAZA_CODE");		//!< 新大字・通称コード
	extern const _TCHAR kNewAzaCode[]		= _T("NEW_AZA_CODE");		//!< 新字・丁目コード
	extern const _TCHAR kZipCode[]			= _T("ZIP_CODE");			//!< 郵便番号
	extern const _TCHAR kZipCount[]			= _T("ZIP_COUNT");			//!< 郵便番号個数
	extern const _TCHAR kSeireishiFlag[]	= _T("SEIREISHI_FLAG");		//!< 政令指定都市フラグ
	extern const _TCHAR kKenKana[]			= _T("KEN_KANA");			//!< 都道府県名カナ読み
	extern const _TCHAR kShiKana[]			= _T("SHI_KANA");			//!< 市区郡町村名カナ読み
	extern const _TCHAR kOazaKana[]			= _T("OAZA_KANA");			//!< 大字・通称名カナ読み
	extern const _TCHAR kAzaKana[]			= _T("AZA_KANA");			//!< 字名・丁目カナ読み
	extern const _TCHAR kKenKanji[]			= _T("KEN_KANJI");			//!< 都道府県名
	extern const _TCHAR kShiKanji[]			= _T("SHI_KANJI");			//!< 市区郡町村名
	extern const _TCHAR kOazaKanji[]		= _T("OAZA_KANJI");			//!< 大字・通称名
	extern const _TCHAR kAzaKanji[]			= _T("AZA_KANJI");			//!< 字名・丁目
	extern const _TCHAR kKenKanjiType[]		= _T("KEN_KANJI_TYPE");		//!< ［外字情報］都道府県名
	extern const _TCHAR kShiKanjiType[]		= _T("SHI_KANJI_TYPE");		//!< ［外字情報］市区郡町村名
	extern const _TCHAR kOazaKanjiType[]	= _T("OAZA_KANJI_TYPE");	//!< ［外字情報］大字・通称名
	extern const _TCHAR kAzaKanjiType[]		= _T("AZA_KANJI_TYPE");		//!< ［外字情報］字名・丁目
	extern const _TCHAR kShikoDate[]		= _T("SHIKO_DATE");			//!< 施行年月
	extern const _TCHAR kHaishiDate[]		= _T("HAISHI_DATE");		//!< 廃止年月
	extern const _TCHAR kTyoumeiDate[]		= _T("TYOUMEI_DATE");		//!< 町名変更年月
	extern const _TCHAR kZipCodeDate[]		= _T("ZIP_CODE_DATE");		//!< 郵便番号変更年月
	extern const _TCHAR kJukyoDate[]		= _T("JUKYO_DATE");			//!< 住居表示実施年月
	extern const _TCHAR kJukyoFlag[]		= _T("JUKYO_FLAG");			//!< 住居表示実施フラグ
	extern const _TCHAR kRecordFlag[]		= _T("RECORD_FLAG");		//!< レコード区分
	extern const _TCHAR kTsushoFlag[]		= _T("TSUSHO_FLAG");		//!< 通称フラグ
	extern const _TCHAR kTorinaFlag[]		= _T("TORINA_FLAG");		//!< 通り名識別
	extern const _TCHAR kOazaFlag[]			= _T("OAZA_FLAG");			//!< 大字・字フラグ
	extern const _TCHAR kAzaFlag[]			= _T("AZA_FLAG");			//!< 字・小字フラグ
	extern const _TCHAR kOldZipCode[]		= _T("OLD_ZIP_CODE");		//!< 旧5桁郵便番号
	extern const _TCHAR kShuseiCode[]		= _T("SHUSEI_CODE");		//!< 修正コード
} // areacode_master

// 全角読み仮名テーブル
namespace areacode_kana {

	extern const _TCHAR kTableName[]	= _T("AREACODE_KANA");		//!< テーブル名

	extern const _TCHAR kKenCode[]		= _T("KEN_CODE");			//!< 都道府県コード
	extern const _TCHAR kShiCode[]		= _T("SHI_CODE");			//!< 市区町村コード
	extern const _TCHAR kOazaCode[]		= _T("OAZA_CODE");			//!< 大字・通称コード 
	extern const _TCHAR kAzaCode[]		= _T("AZA_CODE");			//!< 字・丁目コード
	extern const _TCHAR kKenKanaD[]		= _T("KEN_KANA_D");			//!< 都道府県名全角カナ読み
	extern const _TCHAR kShiKanaD[]		= _T("SHI_KANA_D");			//!< 市区郡町村名全角カナ読み
	extern const _TCHAR kOazaKanaD[]	= _T("OAZA_KANA_D");		//!< 大字・通称名全角カナ読み
	extern const _TCHAR kAzaKanaD[]		= _T("AZA_KANA_D");			//!< 字名・丁目全角カナ読み
}// areacode_kana

}// schema

}// address