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

#if !defined(CHKMEMORYLEAK_HPP__INCLUDED_)
#define CHKMEMORYLEAK_HPP__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef	__AFXWIN_H__            // MFCのウィンドウを使う場合に限定しています
 #ifdef _DEBUG
  #pragma message("以下の五行をソースコードの最初に書いてください")
  #pragma message(" #ifdef _DEBUG")
  #pragma message("  #define new DEBUG_NEW")
  #pragma message (" #undef THIS_FILE")
  #pragma message (" static char THIS_FILE[] = __FILE__;")
  #pragma message(" #endif")
 #endif
#else
 #if defined(_DEBUG)
  #define _CRTDBG_MAP_ALLOC
// Macros for setting or clearing bits in the CRT debug flag 
#ifdef _DEBUG
#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif
  void*  operator new(size_t size, const char *filename, int linenumber);
  void   operator delete(void * _P, const char *filename, int linenumber);
  #pragma message("以下の４行を各ソースコードの最初に書いてください")
  #pragma message(" #ifdef _DEBUG")
  #pragma message(" #define DEBUG_NEW new(__FILE__, __LINE__)")
  #pragma message(" #define new DEBUG_NEW")
  #pragma message(" #endif")
  #pragma message("以下の三行はプログラムの最初に実行してください")
  #pragma message(" #ifdef _DEBUG")
  #pragma message(" ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);")
  #pragma message(" #endif")
 #endif
 #include <malloc.h>
 #include <crtdbg.h>
#endif

#endif //CHKMEMORYLEAK_HPP__INCLUDED_
