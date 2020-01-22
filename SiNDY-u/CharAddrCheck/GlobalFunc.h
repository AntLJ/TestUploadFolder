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

/**
* @brief	フィーチャクラスからWhere句を指定してOIDと指定フィールドの値を取得
* @param	strWhere[in]				Where句
* @param	strField[in]				指定フィールド
* @param	ipFeatureClass[in]			指定フィーチャクラス
* @param	lType[in]					処理タイプ（0:号ポイント, それ以外:行政界）
* @retval	OIDをキーにした指定フィールドの値のマップ
*/
std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, long lType );

/**
* @brief	読み変換後のチェックリストからWhere句を指定してOIDと指定フィールドの値を取得
* @param	strWhere[in]				Where句
* @param	strField[in]				指定フィールド
* @param	ipFeatureClass[in]			指定フィーチャクラス
* @param	lpcszPath[in]				読み変換後のチェックリスト
* @param	lType[in]					処理タイプ（0:号ポイント, それ以外:行政界）
* @retval	OIDをキーにした指定フィールドの値のマップ
*/
std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems2( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, LPCTSTR lpcszListPath, long lType );

bool CodeToKanji( LPCTSTR lpcszCode, char *lpszKanji, const ODatabase& eOraDB );
bool PartitionCode( LPCTSTR lpcszCode, LPTSTR lpszKen, LPTSTR lpszShi, LPTSTR lpszOaza, LPTSTR lpszAza );

/**
* @brief	ファイル名設定（同じファイル名が無いように　001～999まで）
* @param	lpcszOutPut[in]				出力ディレクトリ
* @param	lpcszFile[in]				ファイル名（拡張子はtxtで固定）
* @param	lCount[in]					ファイル番号
* @retval	出力ファイル名（フルパス）
*/
CString CreateLogFile( LPCTSTR lpcszOutput, LPCTSTR lpcszFile, long lCount );
