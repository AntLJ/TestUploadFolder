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

/**
 * @file charutil.h
 * @brief <b>文字列操作ユーティリティー</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _CHARUTIL_H_
#define _CHARUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// ASCIIコードの判別
#define isasciikatakana(c)				( 0xa6 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0xdf )
//{ ADD 02.11.21 by hkakeno
#define isasciinumber(c)				( 0x30 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x39 )
#define isasciialphabet_large(c)		( 0x41 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x5a )
#define isasciialphabet_small(c)		( 0x61 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x7a )
#define isasciiminus(c)					( 0x2d == (unsigned char)((c)&0xff) )
#define isasciiperiod(c)				( 0x2e == (unsigned char)((c)&0xff) )
#define isasciislash(c)                 ( 0x2f == (unsigned char)((c)&0xff) )
#define isasciizero(c)					( 0x30 == (unsigned char)((c)&0xff) )
#define isasciichouon(c)				( 0xB0 == (unsigned char)((c)&0xff) )
//} ADD 02.11.21 by hkakeno
/// SJISの漢字かなコードの判別
#define issjiswide1(c)					((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))
#define issjiswide2(c)					((0x40 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x7e) || (0x80 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))	//!< 2バイト目の判定
#define issjiswide(c1,c2)				( issjiswide1(c1) && issjiswide2(c2) )
/// SJIS全角文字判定
/// issjiswideは第一バイトの判定が 0xE0～0xFC （CP932）に対し、
/// isjis0208kanji1は 0xE0～0xEF（MS_Kanji）で行う
/// 参考：http://www5d.biglobe.ne.jp/~noocyte/Programming/CharCode.html
#define isjis0208kanji1(c)					((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xef))
#define isjis0208kanji2(c)					((0x40 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x7e) || (0x80 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))	//!< 2バイト目の判定
#define isjis0208kanji(c1,c2)				( isjis0208kanji1(c1) && isjis0208kanji2(c2) )

#define issjiswidehiragana(c1,c2)		( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x9f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xf1 ) )
#define issjiswidekatakana(c1,c2)		( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x96 ) )
/// SJIS漢字（第一、第二水準）判定
/// 機種依存文字は考慮されない
#define issjiskanji1(c)					((0x88 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))
#define issjiskanji(c1,c2)				( issjiskanji1(c1) && issjiswide2(c2) && !( ( 0xee == (unsigned char)((c1)&0xff) ) && ( 0xef <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xfc ) ) )
#define issjishalfkana(c)				((0xa1 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xdf))
#define issjiswidenumber(c1,c2)			( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x4f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x58 ) )
#define issjiswidealphabet(c1,c2)		( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x60 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x9a ) )
#define issjiswidegreek(c1,c2)			( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( ( ( 0x9f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xb6 ) ) || ( ( 0xbf <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xd6 ) ) ) )					// ギリシャ文字 2002/11/14 Hide.Yamamoto(CCS)
#define issjis3kanji(c1,c2)				( ( ( 0x87 == (unsigned char)((c1)&0xff) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x9f ) ) ) || ( ( ( 0xed <= (unsigned char)((c1)&0xff) ) && ( (unsigned char)((c1)&0xff) <= 0xee ) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xff ) ) ) || ( ( ( 0xfa <= (unsigned char)((c1)&0xff) ) && ( (unsigned char)((c1)&0xff) <= 0xfc ) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xff ) ) ) )	// 第三水準漢字
// 記号
#define issjiswidenakaguro(c1,c2)		( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x45 == (unsigned char)((c2)&0xff) ) )	// 「・」
#define issjiswideperiod(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x44 == (unsigned char)((c2)&0xff) ) )	// 「．」
#define issjiswidesmallkakko(c1,c2)		( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x69 == (unsigned char)((c2)&0xff) ) )	// 「（」
#define issjiswidesmalltojikakko(c1,c2)	( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x6a == (unsigned char)((c2)&0xff) ) )	// 「）」
#define issjiswidenyoro(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x60 == (unsigned char)((c2)&0xff) ) )	// 「～」
#define issjiswidechouon(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x5b == (unsigned char)((c2)&0xff) ) )	// 「ー」
#define issjiswidenun(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x58 == (unsigned char)((c2)&0xff) ) )	// 「々」
#define issjiswidekanma(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x41 == (unsigned char)((c2)&0xff) ) )	// 「、」
#define issjiswideshime(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x59 == (unsigned char)((c2)&0xff) ) )	// 「〆」
#define issjiswideminus(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x7c == (unsigned char)((c2)&0xff) ) )	// 「－」
#define issjiswidekigou(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x41 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xfc ) )						// 記号(注記文字列用) 2002/11/14 Hide.Yamamoto(CCS)
#define issjiswidedash(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x5c == (unsigned char)((c2)&0xff) ) )	// 「―」全角ダッシュ
// カナ
#define issjiswidesmallke(c1,c2)		( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( 0x96 == (unsigned char)((c2)&0xff) ) )	// 「ヶ」

#endif // _CHARUTIL_H_
