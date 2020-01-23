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
 * @file ErrorCode.h
 * @brief <b>エラーコード表定義ファイル</b>\n
 * エラーコード・エラーレベル・エラーメッセ―ジを定義する
*/
ERROR_CODE(
1001,
kError,
複数のポイントが同一地点に存在
)
ERROR_CODE(
1002,
kError,
複数のポイントで流入リンク・流出ノードが同一
)
ERROR_CODE(
1003,
kWarn,		// エラーレベルをERROR⇒WARNINGに変更(bug 11277)
流入リンクが無条件の一通の逆走・無条件通禁・通行不可道路のいずれか
)
ERROR_CODE(
1004,
kWarn,
流出ノードが信号機あり交差点ノード
)
ERROR_CODE(
1005,
kError,
ポイントの位置が不正
)
ERROR_CODE(
1006,
kError,
流出ノードが2次メッシュ境界ノード
)
ERROR_CODE(
1007,
kError,
流入リンクが環状交差点リンク
)
ERROR_CODE(
1008,
kWarn,
流入リンクが本線( 上下分離 )リンク
)
ERROR_CODE(
1009,
kWarn,
流入リンクがHWY本線
)
ERROR_CODE(
1010,
kWarn,
流出ノードが交差点ノード以外
)
ERROR_CODE(
2001,
kError,
流入リンクが存在しないまたは整備対象外リンク
)
ERROR_CODE(
2002,
kError,
流出ノードが存在しない
)
ERROR_CODE(
2003,
kError,
流入リンクと流出ノードが存在しない
)
ERROR_CODE(
2004,
kError,
流出ノードが流入リンクの始点ノード・終点ノードのいずれとも一致しない
)
