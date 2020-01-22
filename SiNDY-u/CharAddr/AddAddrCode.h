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

#pragma once

#include <oracl.h>

#define CON_STR_SIZE		100		///< 接続文字列のサイズ

#define ALL_KANJI_MAX_LEN	93		///< 漢字名称（結合）の最大長
#define KEN_KANJI_MAX_LEN	9		///< 漢字名称（都道府県）の最大長
#define SHI_KANJI_MAX_LEN	25		///< 漢字名称（市区町村）の最大長
#define OAZA_KANJI_MAX_LEN	37		///< 漢字名称（大字）の最大長
#define AZA_KANJI_MAX_LEN	25		///< 漢字名称（字・丁目）の最大長

#define ALL_KANA_MAX_LEN	185		///< 全角カナ名称（結合）の最大長
#define KEN_KANA_MAX_LEN	17		///< 全角カナ名称（都道府県）の最大長
#define SHI_KANA_MAX_LEN	49		///< 全角カナ名称（市区町村）の最大長
#define OAZA_KANA_MAX_LEN	73		///< 全角カナ名称（大字）の最大長
#define AZA_KANA_MAX_LEN	49		///< 全角カナ名称（字・丁目）の最大長

void CheckLog( const CString& strLogPath );

bool Connect( const CString& strConnect );

/// 接続文字列をユーザ名／パスワード／サービス名に分解
bool fnAnalyzeConStr( const CString& strConnect, char *cUser, char *cPass, char *cService);

/// 住所コード（２・５・８・11桁）を漢字名称に変換
/**
 * @param cCode [in] 住所コード（2/5/8/11桁）
 * @param cKanji [out] 住所コードに対応する漢字名称（結合，全角スペース除去済）
 * @retval true 成功
 * @retval false 失敗
 */
bool CodeToKanji(const char *cCode, char *cKanji);

/// 住所コードを階層ごとに分解
/**
 * @param cCode [in] 住所コード（5/8/11/16/20/22桁）
 * @param cKen [out] 都道府県コード
 * @param cShi [out] 市区町村コード
 * @param cOaza [out] 大字コード
 * @param cAza [out] 字・丁目コード
 * @param cChiban [out] 地番コード
 * @param cBango [out] 番号コード
 * @param cKatagaki [out] 方書コード
 * @retval true 成功
 * @retval false 失敗
 */
bool PartitionCode( const char *cCode, char *cKen, char *cShi, char *cOaza = NULL, char *cAza = NULL, char *cChiban	= NULL, char *cBango = NULL, char *cKatagaki = NULL );

