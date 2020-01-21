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

#ifndef __ADDR_LIB_H__
#define __ADDR_LIB_H__

#define _USE_SINDYLIB_STATIC
#include <SiNDYLib/SiNDYLib.h>
#include <sindy/libschema.h>

#import <tlb/msado15.dll> no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")

#ifdef _UNICODE
#if _MSC_VER == 1700
#ifdef _DEBUG
#pragma comment( lib, "AddrLib_vc11-ugd.lib")
#else
#pragma comment( lib, "AddrLib_vc11-u.lib")
#endif // _DEBUG
#elif _MSC_VER == 1500
#ifdef _DEBUG
#pragma comment( lib, "addrlib-vc90-ugd.lib")
#else
#pragma comment( lib, "addrlib-vc90-u.lib")
#endif // _DEBUG
#else
#ifdef _DEBUG
#pragma comment( lib, "addrlib-vc90-gd.lib")
#else
#pragma comment( lib, "addrlib-vc90.lib")
#endif // _NDEBUG
#endif
#endif	// _UNICODE

#endif __ADDR_LIB_H__
