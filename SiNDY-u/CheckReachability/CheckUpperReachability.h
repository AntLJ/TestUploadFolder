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

#include "RegionPyramid.h"

/**
 * @brief 再上層のリンクへの到達可能性を検査するクラス
 *
 * @note
 * 例外安全なクラス。<br>
 * 生成コスト、コピーコストともに低い。
 */
class CheckUpperReachability
{
public:
	CheckUpperReachability(RegionPyramid::const_iterator first, RegionPyramid::const_iterator last, bool bReverse);

	bool checkByNodeID(long nNodeID);

private:
	const RegionPyramid::const_iterator m_it, m_itEnd;
	const bool m_bReverse;
};
