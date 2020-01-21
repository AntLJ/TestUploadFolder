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
 * @file EditDef.h
 * @brief <b>編集系用定数定義ファイル</b>\n
 * ODLファイルと、C/C++のヘッダに入れられます。
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _INC_SINDYEDIT_DEF_H_
#define _INC_SINDYEDIT_DEF_H_

#if defined(__MKTYPLIB__) || defined(__midl)
// MIDLの場合
#define ODLCOMMENT(p) [helpstring(#p)]
#else
// C/C++の場合
#define ODLCOMMENT(p)
#endif

/**
 * @enum sindyeMoveMode
 * @brief <b>フィーチャ移動モード定義</b>\n
 * このモードは主にCFeatures::Moveのパラメータとして
 * 使用されます。
 */
typedef enum sindyeMoveMode
{
	ODLCOMMENT(構成点追加無しで移動)					sindyeMove_SegmentIntersect					= 0,
	ODLCOMMENT(構成点追加無しで移動)					sindyeMove_SegmentWithin					= 1,
	ODLCOMMENT(最近傍構成点のみ移動)					sindyeMove_Point							= 2,
	ODLCOMMENT(境界に構成点を一つ追加してIntersect移動)	sindyeMove_SegmentAddPointBorderIntersect	= 3,
	ODLCOMMENT(境界に構成点を一つ追加してWithin移動)	sindyeMove_SegmentAddPointBorderWithin		= 4,
	ODLCOMMENT(境界に構成点を二つ追加してWithin移動)	sindyeMove_SegmentAddPointsBorder			= 5,
	ODLCOMMENT(Intersectしたものを完全平行移動)         sindyeMove_WholeIntersect                   = 6,
	ODLCOMMENT(Withinしたものを完全平行移動)            sindyeMove_WholeWithin                      = 7,
} sindyeMoveMode;

#endif // _INC_SINDYEDIT_DEF_H_
