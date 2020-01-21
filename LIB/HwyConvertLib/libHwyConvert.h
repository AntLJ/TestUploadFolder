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

#include <memory>

#include "SINDY2GS.h"
#include "SINDY2HIGHWAY.h"
#include "SINDY2ROADPOINT.h"
#include "SINDY2HWYNODE.h"
#include "SINDY2HWYSAME.h"
#include "SINDY2ROADCODE.h"
#include "SINDY2TOLLGATELANE.h"
#include "Customer.h"
#include "INHwySchema.h"
#include "HwyConvert.h"

#include <libgdb.h>
#include <libsindybase.h>
#include <libsindydbinfo.h>
#include <libsindymap.h>
#include <libzcl.h>
#include <libwkh.h>
#include <STRCODE.h>

#if _MSC_VER == 1700
	#ifdef _DEBUG
//		#ifdef _UNICODE // TODO: Unicode対応したらコメントアウト解除
//			#pragma comment( lib, "HwyConvertLib_vc11-ugd.lib")
//		#else
			#pragma comment( lib, "HwyConvertLib_vc11-gd.lib")
//		#endif // #ifdef _UNICODE
	#else // _DEBUG
//		#ifdef _UNICODE
//			#pragma comment( lib, "HwyConvertLib_vc11-u.lib")
//		#else
			#pragma comment( lib, "HwyConvertLib_vc11.lib")
//		#endif // ifdef _UNICODE
	#endif
#endif
