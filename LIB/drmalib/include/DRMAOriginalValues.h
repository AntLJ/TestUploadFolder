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

// 
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA__ORIGINAL_VALUES_H__
#define __DRMA__ORIGINAL_VALUES_H__

/**
 * @brief DRMA整数値1桁
 */
struct _DRMADec {
	unsigned char eCode;
	operator unsigned char () const { return eCode; }
};

/**
 * @brief DRMA16進整数値1桁
 */
struct _DRMAHex {
	unsigned char eCode;
	operator unsigned char () const { return eCode; }
};

/**
 * @brief DRMA半角カナ1文字
 */
struct _DRMAKana {
	unsigned char eCode;
	operator unsigned char () const { return eCode; }
};

/**
 * @brief 2バイト文字1文字
 */
struct WideChar {
	WideChar(char cFirst, char cSecond) :
	eFirst(cFirst),
	eSecond(cSecond)
	{
	}

	char eFirst;
	char eSecond;
};

/**
 * @brief DRMA2バイト文字1文字
 */
struct _DRMAKanji : public WideChar {
};

#endif
