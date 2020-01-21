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
* @file ArcHelperEx.h
*
* @brief 共通関数を利用する際に、総括してインクルードするための定義ファイル。
*
* @note ライブラリのリンク指定を同時に行っている。
*/
#if	!defined( __ARCHELPEREX_H__)
#define	__ARCHELPEREX_H__

#include "libarchelper.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "LoadQueueInfo.h"
#include "AheGlobals.h"
#include "Mesh.h"
#include "AheDraw.h"
#include "ArcHelper.h"
#include "MenuHelper.h"
#include "InifileHelper.h"
#include "StringConvert.h"
#include "AheGeometryOp.h"
#include "AheBugEvasion.h"

#ifndef __BUILD_SINDYRULE__
#include "MapHelper.h"
#include "LQHelper.h"
#include "TargetFeatures.h"
#include "AheGlobalsX.h"
#include "EditHelper.h"
#endif //__BUILD_SINDYRULE__


#endif	//__ARCHELPEREX_H__
