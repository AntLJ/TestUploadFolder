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

#ifndef _USE_SINDYLIB
#define _USE_SINDYLIB
#if _MSC_VER == 1700
	#ifdef _DEBUG
		#ifdef _USE_SINDYLIB_STATIC
			#pragma comment( lib, "sindylib_vc11-ugd.lib")
		#else
			#pragma comment( lib, "sindylib_vc11-ugd_dll.lib")
		#endif
	#else // _DEBUG
		#ifdef _USE_SINDYLIB_STATIC
			#pragma comment( lib, "sindylib_vc11-u.lib")
		#else
			#pragma comment( lib, "sindylib_vc11-u_dll.lib")
		#endif
	#endif
#elif _MSC_VER == 1500
#ifdef _DEBUG
#ifdef _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylib_vc90-ugd.lib")
#endif // _UNICODE
#else // _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylib_vc90-ugd_dll.lib")
#endif // _UNICODE
#endif // _USE_SINDYLIB_STATIC
#else // _DEBUG
#ifdef _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylib_vc90-u.lib")
#endif // _UNICODE
#else // _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylib_vc90-u_dll.lib")
#endif // _UNICODE
#endif // _USE_SINDYLIB_STATIC
#endif // _DEBUG
#else
#ifdef _DEBUG
#ifdef _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylibmud.lib")
#else // _UNICODE
#pragma comment( lib, "sindylibmd.lib")
#endif // _UNICODE
//#pragma comment( lib, "crd_cnvd.lib")
#else // _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylibmud_dll.lib")
#else // _UNICODE
#pragma comment( lib, "sindylibmd_dll.lib")
#endif // _UNICODE
#endif // _USE_SINDYLIB_STATIC
#else // _DEBUG
#ifdef _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylibmu.lib")
#else // _UNICODE
#pragma comment( lib, "sindylibm.lib")
#endif // _UNICODE
//#pragma comment( lib, "crd_cnv.lib")
#else // _USE_SINDYLIB_STATIC
#ifdef _UNICODE
#pragma comment( lib, "sindylibmu_dll.lib")
#else // _UNICODE
#pragma comment( lib, "sindylibm_dll.lib")
#endif // _UNICODE
#endif // _USE_SINDYLIB_STATIC
#endif // _DEBUG
#endif // if _MSC_VER == 1500
#endif // ifndef _USE_SINDYLIB
