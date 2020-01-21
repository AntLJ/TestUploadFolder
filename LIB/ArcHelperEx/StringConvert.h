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

#ifndef ARCHELPEREX_STRINGCONVERT_H__
#define ARCHELPEREX_STRINGCONVERT_H__
/**
 * @file StringConvert.h
 * @brief 文字列正規化？関数群定義ファイル
 */

/**
* @brief 正しい交差点名称に修正する
*
* @note [ルール]
* @note 1. 漢字名称に全角文字で、漢字、平仮名、片仮名、数字、英字、「、」「〆」「ー」「々」「－」「（」「）」以外の文字が含まれてはいけない
* @note 2. 漢字名称で先頭文字が記号ではいけない
* @note 3. 漢字名称で'ー'が片仮名とひらがな以外の後にあってはいけない
* @note 4. 漢字名称で'－'が数字以外の後にあってはいけない
* @note 5. カナ名称に半角カタカナ以外の文字が含まれていてはいけない		←「全角カタカナ以外」では？
*
* @param lpcszText			[in]	漢字（カナ）文字列
* @param bIsKanji			[in]	漢字の場合は TRUE、カナの場合は FALSE
*
* @retval CString 変換後の文字列
*/
CString AheConvertToIntersectionName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief 正しい方面案内名称に修正する
*
* @note [ルール]
* @note 1. 漢字名称に全角文字で、漢字、平仮名、片仮名、数字、英字、「、」「〆」「ー」「々」「（」「）」「．」「～」「・」以外の文字が含まれてはいけない
* @note 4. カナ名称に全角文字で、片仮名、数字、英字、「・」「、」「ー」「（」「）」以外の文字が含まれてはいけない
* @note 2. 漢字名称、カナ名称で先頭文字が記号ではいけない
* @note 3. 漢字名称、カナ名称で'ー'が片仮名以外の後にあってはいけない
*
* @param lpcszText			[in]	漢字（カナ）文字列
* @param bIsKanji			[in]	漢字の場合は TRUE、カナの場合は FALSE
*
* @retval CString 変換後の文字列
*/
CString AheConvertToDirguideName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief 正しいアクセスポイントの方面案内に修正する
*
* @note [ルール]
* @note 1. 漢字名称に全角文字で、漢字、平仮名、片仮名、数字、英字、「、」「〆」「ー」「々」「（」「）」「．」「～」「・」以外の文字が含まれてはいけない
* @note 2. 漢字名称で先頭文字が記号ではいけない
* @note 3. 漢字名称で'ー'が片仮名以外の後にあってはいけない
* @note 4. 漢字名称で'－'が数字以外の後にあってはいけない
* @note 5. カナ名称に半角カタカナとナカグロ「・」の以外の文字が含まれていてはいけない
*
* @param lpcszText			[in]	漢字（カナ）文字列
* @param bIsKanji			[in]	漢字の場合は TRUE、カナの場合は FALSE
*
* @retval CString 変換後の文字列
*/
CString AheConvertToAccessPointName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief 正しい施設情報名称に修正する
*
* @note [ルール]
* @note 1. 漢字名称に全角文字で、漢字、平仮名、片仮名、数字、英字、「、」「〆」「ー」「々」「－」「（」「）」以外の文字が含まれてはいけない
* @note 2. 漢字名称で先頭文字が記号ではいけない
* @note 3. 漢字名称で'ー'が片仮名以外の後にあってはいけない
* @note 4. 漢字名称で'－'が数字以外の後にあってはいけない
* @note 5. カナ名称に半角カタカナ以外の文字が含まれていてはいけない
*
* @param lpcszText			[in]	漢字（カナ）文字列
* @param bIsKanji			[in]	漢字の場合は TRUE、カナの場合は FALSE
*
* @retval CString 変換後の文字列
*/
CString AheConvertToFacilInfoName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
 * @brief 注記に使われる文字列を適正な文字列に修正
 *
 * @param lpcszText         [in]			元の文字列
 * @param bIsFull           [in]			文字列の形式（TRUE:全角　FALSE:半角）
 * @param bEnabelAlphabet	[in]			英字を使用するかどうか (TRUE:使用する, FALSE:使用しない)
 * @param bUseMinusPeriod   [in,optional]	マイナス記号、小数点を使用するかどうか
 * @param pstrErr			[out,optional]	エラーとなった文字列が入る（先頭の場合に弾かれるものは除く）
 *
 * @retval 修正された文字列
 */
CString AheConvertToAnnoJustStr( LPCTSTR lpcszText, BOOL bIsFull, BOOL bUseAlphabet, BOOL bUseMinusPeriod = FALSE, CString* pstrErr = NULL );

/**
 * @brief 原稿番号に使われる文字列を適正な文字列に修正
 *
 * @param lpcszText         [in]			元の文字列
 *
 * @retval 修正された文字列
 */
CString AheConvertToGenkouNoStr( LPCTSTR lpcszText );

/**
 * @brief 日付に使われる文字列を適正な文字列に修正
 *
 * @param lpcszText         [in]			元の文字列
 *
 * @retval 修正された文字列
 */
CString AheConvertToDateStr( LPCTSTR lpcszText );

#endif // ifndef ARCHELPEREX_STRINGCONVERT_H__
