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

// DRMARecord.cpp: DRMARecord クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMARecord.h"

/// EBCDICカナ→ASCIIカナ変換テーブル
static unsigned char kana_ebcdic_to_ascii[] = {
	0x00,0x01,0x02,0x03,0x5c,0x09,0x5c,0x7f, 0x5c,0x5c,0x5c,0x0b,0x0c,0x0d,0x0e,0x0f,
	0x10,0x11,0x12,0x13,0x5c,0x5c,0x08,0x5c, 0x18,0x19,0x5c,0x5c,0x1c,0x1d,0x1e,0x1f,
	0x5c,0x5c,0x5c,0x5c,0x5c,0x0a,0x17,0x1b, 0x5c,0x5c,0x5c,0x5c,0x5c,0x05,0x06,0x07,
	0x5c,0x5c,0x16,0x5c,0x5c,0x5c,0x5c,0x04, 0x5c,0x5c,0x5c,0x5c,0x14,0x15,0x5c,0x1a,
	0x20,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7, 0xa8,0xa9,0x5b,0x2e,0x3c,0x28,0x2b,0x5d,
	0x26,0xaa,0xab,0xac,0xad,0xae,0xaf,0x5c, 0xb0,0x5c,0x21,0x24,0x2a,0x29,0x3b,0x5e,
	0x2d,0x2f,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c, 0x5c,0x5c,0x7c,0x2c,0x25,0x5f,0x3e,0x3f,
	0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c, 0x5c,0x60,0x3a,0x23,0x40,0x27,0x3d,0x22,
	0x5c,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7, 0xb8,0xb9,0xba,0x5c,0xbb,0xbc,0xbd,0xbe,
	0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6, 0xc7,0xc8,0xc9,0x5c,0x5c,0xca,0xcb,0xcc,
	0x5c,0x7e,0xcd,0xce,0xcf,0xd0,0xd1,0xd2, 0xd3,0xd4,0xd5,0x5c,0xd6,0xd7,0xd8,0xd9,
	0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c, 0x5c,0x5c,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
	0x7b,0x41,0x42,0x43,0x44,0x45,0x46,0x47, 0x48,0x49,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,
	0x7d,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50, 0x51,0x52,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,
	0x5c,0x5c,0x53,0x54,0x55,0x56,0x57,0x58, 0x59,0x5a,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37, 0x38,0x39,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c
};

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

/**
 * @brief EBCDICコードを10進数として解釈し、整数型に変換する
 *
 * @note 変換に失敗した場合、cItemの値は不定である
 *
 * @param cOriginalItem [in] EBCDICコード開始アドレス
 * @param cSize [in] EBCDICコードバイト数
 * @param cItem [out] 変換先
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMAItemConverter::setValueTo(const _DRMADec* cOriginalItem, int cSize, int& cItem)
{
	assert(cOriginalItem != 0);

	bool aResult = true;
	cItem = 0;
	for(int i = 0; i < cSize; i++) {
		cItem *= 10;
		switch(cOriginalItem[i] & 0xf0) {
		case 0xf0: // 0～9を変換
			cItem += cOriginalItem[i] & 0xf;
			break;
		case 0x40: // 空白は0
			break;
		default: // 0～9以外はエラー
			aResult = false;
			break;
		}
	}

	return aResult;
}

/**
 * @brief EBCDICコードを16進数として解釈し、整数型に変換する
 *
 * @note 変換に失敗した場合、cItemの値は不定である
 *
 * @param cOriginalItem [in] EBCDICコード開始アドレス
 * @param cSize [in] EBCDICコードバイト数
 * @param cItem [out] 変換先
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMAItemConverter::setValueTo(const _DRMAHex* cOriginalItem, int cSize, int& cItem)
{
	assert(cOriginalItem != 0);

	bool aResult = true;
	cItem = 0;
	for(int i = 0; i < cSize; i++) {
		cItem *= 0x10;
		switch(cOriginalItem[i] & 0xf0) {
		case 0xf0: // 0～9を変換
			cItem += cOriginalItem[i] & 0xf;
			break;
		case 0xc0: // A～Fを変換
			cItem += (cOriginalItem[i] & 0xf) + 0x9;
			break;
		case 0x40: // 空白は0
			break;
		default: // 0～F以外はエラー
			aResult = false;
			break;
		}
	}

	return aResult;
}

/**
 * @brief JIS漢字列をプラットフォームに合わせた漢字コードに変換する
 *
 * @note Win32ではSJISに変換（とりあえず今はこれだけ）
 *
 * @param cOriginalItem [in] 漢字列開始アドレス
 * @param cSize [in] 漢字列バイト数
 * @param cItem [out] 変換先
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMAItemConverter::setValueTo(const _DRMAKanji* cOriginalItem, int cSize, char* cItem)
{
	assert(cOriginalItem != 0 && cItem != 0);

	int aKanjiSize = cSize/2;

	// 末尾の空白を削除する
	for(int i = aKanjiSize-1; i >= 0; i--) {
		if(cOriginalItem[i].eFirst == 0x21 && cOriginalItem[i].eSecond == 0x21) {
			aKanjiSize--;
		}
		else
			break;
	};

	bool aResult = true;
	for(int j = 0; j < aKanjiSize; j++) {
		WideChar aKanji = jisToSjis(cOriginalItem[j]);
		cItem[j*2] = aKanji.eFirst;
		cItem[j*2+1] = aKanji.eSecond;
	}
	cItem[aKanjiSize*2] = '\0';

	return aResult;
}


/**
 * @brief EBCDICカナ文字列をプラットフォームに合わせたカナ文字列に変換する
 *
 * @note Win32ではASCIIに変換（とりあえず今はこれだけ）
 *
 * @param cOriginalItem [in] EBCDICカナ文字列開始アドレス
 * @param cSize [in] カナ文字列バイト数
 * @param cItem [out] 変換先
 * @retval true 成功
 * @retval false 失敗
 */
bool DRMAItemConverter::setValueTo(const _DRMAKana* cOriginalItem, int cSize, char* cItem)
{
	assert(cOriginalItem != 0 && cItem != 0);

	// 末尾の空白を削除する
	for(int i = cSize-1; i >= 0; i--) {
		if(cOriginalItem[i] == 0x40) {
			cSize--;
		}
		else
			break;
	};

	bool aResult = true;
	for(int j = 0; j < cSize; j++) {
		cItem[j] = kana_ebcdic_to_ascii[cOriginalItem[j]];
	}
	cItem[cSize] = '\0';

	return aResult;
}

/**
 * @brief JIS全角文字をSJIS全角文字に変換する
 *
 * @param cJis [in] JIS全角文字
 * @return cJisに対応するSJIS全角文字
 */
WideChar DRMAItemConverter::jisToSjis(const WideChar& cJis)
{
	int aC1 = cJis.eFirst;
	int aC2 = cJis.eSecond;

	// 第二バイトの変換
	if((aC1 & 0x01) == 0) aC2 = (aC2 + 0x7d) & 0x00ff;
	else                  aC2 = (aC2 + 0x1f) & 0x00ff;
	if(aC2 > 0x7e) ++aC2;

	// 第一バイトの変換
	if(aC1 < 0x5f)
	{
		++aC1;
		aC1 = aC1 >> 1;
		aC1 = (aC1 + 0x70) & 0x00ff;
	}
	else
	{
		++aC1;
		aC1 = aC1 >> 1;
		aC1 = (aC1 + 0xb0) & 0x00ff;
	}


	return WideChar(aC1, aC2);
}
