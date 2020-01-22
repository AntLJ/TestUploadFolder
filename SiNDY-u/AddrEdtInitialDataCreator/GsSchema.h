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

#include "stdafx.h"

namespace gs {

namespace schema {

// 整備用住所ポイント
namespace gs_addr_point
{
	extern const _TCHAR kTableName[];

	extern const _TCHAR kGeoBFid[];
	extern const _TCHAR kIpcObjId[];
	extern const _TCHAR kPlaceCd[];
	extern const _TCHAR kStreetCd[];
	extern const _TCHAR kAddress1[];
	extern const _TCHAR kAddress2[];
	extern const _TCHAR kAddress3[];
	extern const _TCHAR kSourceCd[];
	extern const _TCHAR kPtn[];
	extern const _TCHAR kOrderCd[];
	extern const _TCHAR kJudgCd[];
	extern const _TCHAR kGeoSf[];
	extern const _TCHAR kIpcSf[];
	extern const _TCHAR kShinrai[];
	extern const _TCHAR kHyoki[];
	extern const _TCHAR kAdopt[];
	extern const _TCHAR kPriority[];
}

// 整備用字ポリゴン
namespace gs_addr_polygon
{
	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];
	extern const _TCHAR kAddrCode1[];
	extern const _TCHAR kKCode1[];
	extern const _TCHAR kAddrCode2[];
	extern const _TCHAR kKCode2[];
	extern const _TCHAR kGaikuFugo[];
	extern const _TCHAR kAddrClass[];
}

} // schema

} // gs
