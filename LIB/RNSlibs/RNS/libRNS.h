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

#if _MSC_VER <= 1200
	#ifdef _DEBUG
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rnsmd.lib")
	#else
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rnsm.lib")
	#endif // _DEBUG //
#elif _MSC_VER <= 1400
	#ifdef _DEBUG
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc80-gd.lib")
	#else
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc80.lib")
	#endif // _DEBUG //
#elif _MSC_VER <= 1600
	#ifdef _DEBUG
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc90-gd.lib")
	#else
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc90.lib")
	#endif // _DEBUG //
	#else
	#ifdef _DEBUG
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc11-gd.lib")
	#else
		#pragma comment(lib, "../../../LIB/RNSlibs/lib/rns-vc11.lib")
	#endif // _DEBUG //
#endif
