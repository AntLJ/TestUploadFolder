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
 * @file ErrCode.h
 * @brief エラーコード表定義ファイル
 * エラーコード・エラーレベル・エラーメッセ―ジを定義する
*/

ERROR_CODE(
101,
kWarn,
道なり道路が編集前後で変化している
)
ERROR_CODE(
102,
kWarn,
接続している道路リンクが編集前後で変化している
)
ERROR_CODE(
103,
kWarn,
道路リンクが編集前後で増加している
)
ERROR_CODE(
104,
kWarn,
道路リンクが編集前後で減少している
)
ERROR_CODE(
105,
kWarn,
比較元バージョンで道なり道路を特定できません
)
ERROR_CODE(
106,
kWarn,
比較先バージョンで道なり道路を特定できません
)
#ifdef _DEBUG
ERROR_CODE(
199,
kWarn,
[Debug]道なり道路の角度が編集前後で20°以上変化しています
)
#endif //DEBUG
