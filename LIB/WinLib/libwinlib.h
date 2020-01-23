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

#if _MSC_VER == 1900
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc14-ugd.lib")
		#endif // #ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc14-gd.lib")
		#endif // #ifdef _UNICODE
	#else // _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc14-u.lib")
		#endif // ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc14.lib")
		#endif // #ifdef _UNICODE
	#endif
#elif _MSC_VER == 1700
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc11-ugd.lib")
		#endif // #ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc11-gd.lib")
		#endif // #ifdef _UNICODE
	#else // _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc11-u.lib")
		#endif // ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc11.lib")
		#endif // #ifdef _UNICODE
	#endif
#elif _MSC_VER == 1500
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc90-ugd.lib")
		#endif // #ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc11-gd.lib")
		#endif // #ifdef _UNICODE
	#else // _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "winlib_vc90-u.lib")
		#endif // ifdef _UNICODE
		#ifdef _MBCS
			#pragma comment( lib, "winlib_vc11.lib")
		#endif // #ifdef _UNICODE
	#endif
#endif
