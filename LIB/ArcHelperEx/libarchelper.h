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

#ifndef _ARCHELPEREX_LIBARCHELPER_H_
#define _ARCHELPEREX_LIBARCHELPER_H_

#pragma once 

#define _RELATIONALOPERATOR_EQUALS_BUG_FIXED
#define _NOT_USE_SDE_RELATIONSHIP


#if _MSC_VER == 1900
#ifdef	_DEBUG
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex_vc14-ugd.lib")		// UNICODE DEBUG
	#else
		#pragma comment( lib, "archelperex_vc14-gd.lib")		// ASCII DEBUG
	#endif	// _UNICODE
#else
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex_vc14-u.lib")		// UNICODE RELEASE
	#else
		#pragma comment( lib, "archelperex_vc14.lib")		// ASCII RELEASE
	#endif	// _UNICODE
#endif	//_DEBUG

#elif _MSC_VER == 1700
#ifdef	_DEBUG
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-vc11-ugd.lib")		// UNICODE DEBUG
	#else
		#pragma comment( lib, "archelperex-vc11-gd.lib")		// ASCII DEBUG
	#endif	// _UNICODE
#else
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-vc11-u.lib")		// UNICODE RELEASE
	#else
		#pragma comment( lib, "archelperex-vc11.lib")		// ASCII RELEASE
	#endif	// _UNICODE
#endif	//_DEBUG

#elif _MSC_VER == 1500
#ifdef	_DEBUG
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-vc90-ugd.lib")		// UNICODE DEBUG
	#else
		#pragma comment( lib, "archleperex-vc90-gd.lib")		// ASCII DEBUG
	#endif	// _UNICODE
#else
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-vc90-u.lib")		// UNICODE RELEASE
	#else
		#pragma comment( lib, "archelperex-vc90.lib")		// ASCII RELEASE
	#endif	// _UNICODE
#endif	//_DEBUG

#else
#ifdef	_DEBUG
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-ugd.lib")		// UNICODE DEBUG
	#else
		#pragma comment( lib, "archleperex-gd.lib")		// ASCII DEBUG
	#endif	// _UNICODE
#else
	#ifdef	_UNICODE
		#pragma comment( lib, "archelperex-u.lib")		// UNICODE RELEASE
	#else
		#pragma comment( lib, "archelperex.lib")		// ASCII RELEASE
	#endif	// _UNICODE
#endif	//_DEBUG
#endif // if _MSC_VER == 1700

#endif // ifndef _ARCHELPEREX_LIBARCHELPER_H_
