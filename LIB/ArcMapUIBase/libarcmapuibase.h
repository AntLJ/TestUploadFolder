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

#ifndef LIBARCMAPUIBASE_H_
#define LIBARCMAPUIBASE_H_

#if _MSC_VER == 1700
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-vc11-ugd.lib")
#else
#pragma comment(lib,"arcmapuibase-vc11-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-vc11-gd.lib")
#else
#pragma comment(lib,"arcmapuibase-vc11.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#elif _MSC_VER == 1500
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-vc90-ugd.lib")
#else
#pragma comment(lib,"arcmapuibase-vc90-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-vc90-gd.lib")
#else
#pragma comment(lib,"arcmapuibase-vc90.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#else
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-ugd.lib")
#else
#pragma comment(lib,"arcmapuibase-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"arcmapuibase-gd.lib")
#else
#pragma comment(lib,"arcmapuibase.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#endif // if _MSC_VER == 1700

#endif // ifndef LIBARCMAPUIBASE_H_
