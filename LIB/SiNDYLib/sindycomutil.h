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

/**
 * @file sindycomutil.h
 * @brief COMユーティリティー定義
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _SINDYCOMUTIL_H_
#define _SINDYCOMUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "util.h"

#define QUERYINTERFACE(x,i,p) \
	{ \
		if( x == NULL ) \
			ERRORLOG(sindyErr_COMInterfaceIsNull); \
		else { \
			if( FAILED( x->QueryInterface( i, p ) ) ) \
				ERRORLOG(sindyErr_COMQueryInterfaceFailed); \
		} \
	}

/**
 * @def INVOKEMETHOD(x)
 * インターフェースメソッド実行を安全に行うマクロです。
 * このマクロは、CComWrapper,CComPtr,CComQIPtrを継承している場合にのみ有効なので、スマートポインタ
 * のメソッドのラッパーを実装するときに使用してください。
 * また、HRESULTは定義済みですので、HRESULTが必要な場合はhrを参照してください。
 *
 * <h3>例：</h3>
 *  IHoge::get_Name()を呼び出したい場合：
 *  @code INVOKEMETHOD( get_Name( &bstrName ) ); @endcode
 *
 * @param x		[in]	実行メソッド
 */
#define INVOKEMETHOD(x) \
	HRESULT hr = -1; \
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing ) \
	{ \
		hr = p->x; \
		LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_COMFunctionFailed ); \
	} \

#define QUERYINVOKEMETHOD(t,i,x) \
	HRESULT hr = -1; \
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing ) \
	{ \
		t * p2 = NULL; \
		LOGASSERTE_IF( SUCCEEDED( hr = p->QueryInterface( i, (void**)&p2 ) ), sindyErr_COMQueryInterfaceFailed ) \
		{ \
			LOGASSERTE_IF( SUCCEEDED( hr = p2->x ), sindyErr_COMFunctionFailed ); \
			p2->Release(); \
		} \
	}

#define _INVOKEMETHOD2 INVOKEMETHOD
/**
 * @brief インターフェースメソッド実行を安全に行うマクロです。
 * このマクロは、呼び出すメソッドの引
 * 数が一つで、それはCOMインターフェースポインタである必要があります。
 * 内部でエラーコードをリターンしますが、その場合は常にsindyErr_NoErr
 * 又はsindyErr_COMFunctionFailedですので、後者を受け取った場合は
 * 必ずIErrorInfoでエラー内容を確認してください。
 *
 * @see INVOKEMETHOD
 *
 * <h3>例：</h3>
 * IHoge::Exec( IFuga* pFuga )を呼び出したい場合：
 * @code INVOKEMETHOD3( pFuga, Exec ); @endcode
 *
 * @param arg	[in]	引数
 * @param func	[in]	実行メソッド
 */
#define INVOKEMETHOD3(arg,func) \
	if( (arg) == NULL ) return sindyErr_COMInterfaceIsNull; \
	_INVOKEMETHOD2( func( arg ) ); \
	if( SUCCEEDED( hr ) ) \
		return sindyErr_NoErr; \
	else { \
		SASSERT( hr ); \
		return sindyErr_COMFunctionFailed; \
	}

/**
 * @brief インターフェースメソッド実行を安全に行うマクロです。
 * このマクロは、::INVOKEMETHOD3と殆ど一緒ですが、argがNULLでも
 * 警告を出しません。NULL OKな引数のときのみ使用してください。
 *
 * @see INVOKEMETHOD3
 *
 * <h3>例：</h3>
 * IHoge::Exec( IFuga* pFuga )を呼び出したい場合：
 * @code INVOKEMETHOD3NULLOK( pFuga, Exec ); @endcode
 *
 * @param arg	[in]	引数
 * @param func	[in]	実行メソッド
 */
#define INVOKEMETHOD3NULLOK(arg,func) \
	_INVOKEMETHOD2( func( arg ) ); \
	if( SUCCEEDED( hr ) ) \
		return sindyErr_NoErr; \
	else { \
		SASSERT( hr ); \
		return sindyErr_COMFunctionFailed; \
	} 

#endif // _SINDYCOMUTIL_H_
