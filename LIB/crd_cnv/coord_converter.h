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

#ifndef COORD_CONVERTER_H__
#define COORD_CONVERTER_H__

#pragma once

#include "crd_cnv.h"

#ifdef _DEBUG
#ifdef _UNICODE
#if _MSC_VER == 1900
#pragma comment(lib,"crd_cnv_vc14-mud.lib")
#elif _MSC_VER == 1700
#pragma comment(lib,"crd_cnv_vc11-mud.lib")
#elif _MSC_VER == 1500
#pragma comment(lib,"crd_cnv_vc90-mud.lib")
#elif _MSC_VER == 1400
#pragma comment(lib,"crd_cnvmud.lib")
#else
#pragma comment(lib,"crd_cnv_vc60-mud.lib")
#endif
#else
#if _MSC_VER == 1900
#pragma comment(lib,"crd_cnv_vc14-d.lib")
#elif _MSC_VER == 1700
#pragma comment(lib,"crd_cnv_vc11-d.lib")
#elif _MSC_VER == 1500
#pragma comment(lib,"crd_cnv_vc90-d.lib")
#elif _MSC_VER == 1400
#pragma comment(lib,"crd_cnvd.lib")
#else
#pragma comment(lib,"crd_cnv_vc60-d.lib")
#endif
#endif // ifdef _UNICODE
#else
#ifdef _UNICODE
#if _MSC_VER == 1900
#pragma comment(lib,"crd_cnv_vc14-mu.lib")
#elif _MSC_VER == 1700
#pragma comment(lib,"crd_cnv_vc11-mu.lib")
#elif _MSC_VER == 1500
#pragma comment(lib,"crd_cnv_vc90-mu.lib")
#elif _MSC_VER == 1400
#pragma comment(lib,"crd_cnvmu.lib")
#else
#pragma comment(lib,"crd_cnv_vc60-mu.lib")
#endif
#else
#if _MSC_VER == 1900
#pragma comment(lib,"crd_cnv_vc14.lib")
#elif _MSC_VER == 1700
#pragma comment(lib,"crd_cnv_vc11.lib")
#elif _MSC_VER == 1500
#pragma comment(lib,"crd_cnv_vc90.lib")
#elif _MSC_VER == 1400
#pragma comment(lib,"crd_cnv.lib")
#else
#pragma comment(lib,"crd_cnv_vc60.lib")
#endif
#endif // ifdef _UNICODE
#endif // ifdef _DEBUG

#endif // ifndef COORD_CONVERTER_H__
