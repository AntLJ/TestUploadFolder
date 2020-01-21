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

#if _MSC_VER == 1700
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "arclib_vc11-ugd.lib")
		#else
			#pragma comment( lib, "arclib_vc11-gd.lib")
		#endif // ifdef _UNICODE
	#else // _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "arclib_vc11-u.lib")
		#else
			#pragma comment( lib, "arclib_vc11.lib")
		#endif // ifdef _UNICODE
	#endif
#elif _MSC_VER == 1500
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "arclib_vc90-ugd.lib")
		#else
			#pragma comment( lib, "arclib_vc90-gd.lib")
		#endif // ifdef _UNICODE
	#else // _DEBUG
		#ifdef _UNICODE
			#pragma comment( lib, "arclib_vc90-u.lib")
		#else
			#pragma comment( lib, "arclib_vc90.lib")
		#endif // ifdef _UNICODE
	#endif
#endif
