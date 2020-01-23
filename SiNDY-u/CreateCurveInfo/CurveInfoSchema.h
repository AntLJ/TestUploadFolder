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

/**
 * @brief カーブ情報データのスキーマ定義
 */
namespace curve_info
{
	namespace schema
	{
		namespace curve_line
		{
			extern const _TCHAR kROC[];				///< 曲率半径
			extern const _TCHAR kRepLinkID[];		///< 代表点存在リンクID
			extern const _TCHAR kWayToRepPoint[];	///< 代表点位置
		}
	} // namespace schema
} // namespace curve_info
