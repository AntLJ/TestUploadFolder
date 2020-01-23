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

#ifndef WINLIB_MACRO_UTIL_H_
#define WINLIB_MACRO_UTIL_H_

// xをワイド文字列に変換するマクロ
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
// 定義済みマクロのワイド版
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCSIG__ WIDEN(__FUNCSIG__)

/// __FILE__LINE__マクロ
#define 	_WQUOTE(x)   L#x
#define 	_QUOTE(x)   # x
#define 	WQUOTE(x)   _WQUOTE(x)
#define 	QUOTE(x)   _QUOTE(x)
#define 	__WFILE__LINE__   __WFILE__ L"(" WQUOTE(__LINE__) L") : "
#define 	__AFILE__LINE__   __FILE__ "(" QUOTE(__LINE__) ") : "
#ifdef _UNICODE
#define __FILE__LINE__ __WFILE__LINE__
#else
#define __FILE__LINE__ __AFILE__LINE__
#endif // ifdef _UNICODE

/// __func__マクロ
#ifdef _UNICODE
#define __func__ __WFUNCSIG__
#else
#define __func__ __FUNCSIG__
#endif // ifdef _UNICODE

/// 参照カウンタトレースマクロ
#ifndef UNICODE
#define TRACEREFCOUNT(x) { \
	if(x) { \
		x->AddRef(); \
		int n = x->Release(); \
		ATLTRACE( _T("DEBUG:REFCOUNT %s %d\n"), #x, n ); } \
	}
#else
#define TRACEREFCOUNT(x) { \
	if(x) { \
		x->AddRef(); \
		int n = x->Release(); \
		ATLTRACE( _T("%s : DEBUG:REFCOUNT %s %d\n"), __FILE__LINE__, L#x, n ); } \
	}
#endif // UNICODE

/// プロパティ定義用マクロ
#define PROPERTY( _t, _m )  \
private: _t m_##_m ;        \
public:                     \
	__declspec(property(get=get##_m, put=set##_m)) _t _m ;  \
	_t get##_m() { return m_##_m; }                     \
	void set##_m( _t value ) { m_##_m = value; };

#endif // ifndef WINLIB_MACRO_UTIL_H_
