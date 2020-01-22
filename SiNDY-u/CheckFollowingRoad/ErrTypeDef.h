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

namespace err_type
{

/**
 * @brief	エラーの種類
 */
enum error_type
{
	kChangeFollow = 101, //!< 道なり道路が編集前後で変化
	kChangeLink = 102, //!< 接続している道路リンクが編集前後で変化
	kIncreaseLink = 103, //!< 道路リンクが編集前後で増加
	kDecreaseLink = 104, //!< 道路リンクが編集前後で減少
	kBaseUnspecified = 105, //!< 比較元バージョンで道なり道路を特定不可
	kCompUnspecified = 106, //!< 比較先バージョンで道なり道路を特定不可
	kDiffAngle = 199 //!< [Debug]道なり道路の角度が編集前後で20°以上変化
};

} // err_type
