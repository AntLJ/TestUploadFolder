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
 * @file util.h
 * @brief ユーティリティー関数の定義
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

extern bool g_bTRACEMESSAGE_STDOUT; //!< TRACEMESSAGEを標準出力に出すかどうか（デフォルト：true）

/**
 * @brief <b>トレースを出す</b>\n
 * デバッグモードの時はデバッグウィンドウとストリームに、
 * リリースモードの時はストリームのみにトレースを出力します。
 * 出力ストリームにはsindy::g_Tracelogを使用するので、
 * アプリケーション起動時にセットしてください。
 * @param lpcszFormat	[in]	出力文字列フォーマット
 */
void SINDYLIB_API TRACEMESSAGE( LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>トレースを出す</b>\n
 * デバッグモードの時はデバッグウィンドウとストリームに、
 * リリースモードの時はストリームのみにトレースを出力します。
 * @param lpcszFormat	[in]	出力文字列フォーマット
 */
void SINDYLIB_API TRACEMESSAGE( FILE* out, LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>トレースを出す</b>\n
 * デバッグモードの時はデバッグウィンドウとストリームとプログレスに、
 * リリースモードの時はストリームとプログレスのみにトレースを出力します。
 * @param ipProgress	[in]	プログレスコンポーネント
 * @param lStep			[in]	ステップ数
 * @param bIsPercent	[in]	ステップ数がパーセンテージかどうか
 * @param bAddMsg		[in]	プログレスにメッセージを追追記する場合はtrue		
 * @param lpcszFormat	[in]	出力文字列フォーマット
 */
void SINDYLIB_API TRACEMESSAGE( IProgressor* ipProgress, long lStep, bool bIsPercent, bool bAddMsg, LPCTSTR lpcszFormat, ...);

/**
 * @brief <b>トレースを出す</b>\n
 * ファイルに出力する場合のみ、「# yyyy/mm/dd hh:mm:ss 」がヘッダとして行頭につきます
 * @param lpcszFormat	[in]			出力文字列フォーマット
 * @param args			[in,optional]	可変長引数リスト（デフォルト：NULL）
 * @param out			[in,optional]	ファイルハンドル（デフォルト：NULL）
 * @param bOutDebugger	[in,optional]	デバッグ版の場合にアウトプットウィンドウに出力する場合はtrue（デフォルト：false）
 * @param bOutStdOut	[in,optional]	標準出力に出力する場合はtrue（デフォルト：false）
 * @param ipProgress	[in,optional]	プログレスコンポーネント（デフォルト：NULL）
 * @param bStep			[in,optional]	同時にステップするならtrue（デフォルト：false）
 * @param bUseStepValue	[in,optinoal]	stepをステップ値として使用するならtrue（デフォルト：false）
 * @param bIsPercent	[in,optional]	stepがパーセンテージならtrue（デフォルト：false）
 * @param step			[in,optional]	ステップ値又はパーセンテージ（デフォルト：0）
 * @param bAddMsg		[in,optional]	プログレスにメッセージを追記する場合はtrue		
 */
void SINDYLIB_API TraceMessageV( 
				   LPCTSTR lpcszFormat, 
				   va_list args = NULL, 
				   FILE* out = NULL, 
				   bool bOutDebugger = false,
				   bool bOutStdOut = false,
				   IProgressor* ipProgress = NULL, 
				   bool bStep = false,
				   bool bUseStepValue = false,
				   bool bIsPercent = false,
				   long step = 0,
				   bool bAddMsg = false);

/**
 * @brief <b>エラーログに出力してからリターンする</b>\n
 * sindyErrCodeをリターンする場合は、このマクロを使用するとログに
 * 書いてくれるので便利です。
 * @param x	[in]	sindyErrCode
 */
#define ERR_RETURN(x) \
	{ \
		_ERRORLOG(x,__func__); \
		return x; \
	}

#ifdef _DEBUG
#define RETURN_IF(expr,err) \
	{ \
		if(expr) { \
			do { \
				if( 1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { \
					ERRORLOG(err); \
					_CrtDbgBreak(); \
				} \
			} while (0); \
			return (err); \
		} \
	}
#else
#define RETURN_IF(expr,err) \
	{ \
		if(expr) { \
			ERRORLOG(err); \
			return (err); \
		} \
	}
#endif

#define FUNCTRACE(isstart) TRACEMESSAGE( _T("%s : %s\n"), __func__, ( isstart ) ? _T("START") : _T("END") );
#define FUNCTRACE2(isstart,errcode) TRACEMESSAGE( _T("%s : %s %s\n"), __func__, ( isstart ) ? _T("START") : _T("END"), SINDYERRCODE2STRING(errcode) );

/**
 * @brief <b>アサート切り替え用マクロ</b>\n
 * メッセージボックス形式、ログ形式などの切り替えのための
 * マクロです。ライブラリ内では必ずSASSERTを使用してください。
 * メッセージボックス形式の時に判定式を表示したい場合は、
 * SASSERTEを使用してください。
 */
#ifdef _DEBUG
#define SASSERT(expr) \
	do { if (!(expr) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) _CrtDbgBreak(); } while (0)
#else
#define SASSERT(expr) ((void)0)
#endif // ifdef _DEBUG

/**
 * @brief <b>アサート切り替え用マクロ</b>\n
 * メッセージボックス形式、ログ形式などの切り替えのための
 * マクロです。ライブラリ内では必ずSASSERTを使用してください。\n
 */
#ifdef _DEBUG
#define SASSERTE(expr) \
	do { if (!(expr) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr))) _CrtDbgBreak(); } while (0)
#else
#define SASSERTE(expr) SASSERT(expr)
#endif // ifdef _DEBUG

/**
 * @brief <b>ログとASSERTを出すマクロ</b>\n
 * メッセージボックス形式の時に判定式を表示したい場合は、
 * LOGASSERTEを使用してください。
 */
#ifdef _DEBUG
#define LOGASSERT(expr,err) \
do { if (!(expr)) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { ERRORLOG(err); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERT(expr,err) do { if(!(expr)) ERRORLOG(err); } while (0)
#endif // ifdef _DEBUG

/**
 * @brief <b>ログとASSERTを出すマクロ</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTE(expr,err) \
do { if (!(expr)) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) { ERRORLOG(err); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERTE(expr,err) LOGASSERT(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>FAILED判定の場合にHRESULT込みのログとASSERTを出すマクロ</b>\n
 * if( SUCCEEDED() )の代わりに使用することができます。\n
 * 条件式がFAILEDの場合に行う処理がログ出力とASSERTEの場合のみに有効です
 * ので、else文を記述してエラー処理を行う必要がありません。
 * 
 * e.x.
 * SUCCEEDED_IF( ipCurve->get_Length( &dLen ), sindyErr_GeometryFunctionFailed, _T("%s"), _T("失敗したよ") )
 * {
 *   ...
 * }
 */
#ifdef _DEBUG
#define SUCCEEDED_IF(expr,err,format,...) \
	if(FAILED(expr)) \
	{ \
		do { \
			if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) \
			{ \
				_ERRORHRLOG((err),(expr),(format), __VA_ARGS__); \
				_CrtDbgBreak(); \
			} \
		} \
		while(0); \
	} else
#else
#define SUCCEEDED_IF(expr,err,format,...) \
	if(FAILED(expr)) \
	{ \
		_ERRORHRLOG((err),(expr),(format), __VA_ARGS__); \
	} else
#endif // ifdef _DEBUG

/**
 * @brief <b>ログとASSERTを出すマクロ</b>\n
 * このマクロはHRESULT専用です。exprにはHRESULTを指定してください。
 */
#ifdef _DEBUG
#define LOGASSERTHR(expr,err) \
do { if (FAILED((expr))) { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) { ERRORHRLOG((err),(expr)); _CrtDbgBreak(); } } } while (0)
#else
#define LOGASSERTHR(expr,err) do { if(FAILED((expr))) ERRORHRLOG((err),(expr)); } while (0)
#endif // ifdef _DEBUG
#ifdef _DEBUG
#define LOGASSERTEHR(expr,err) \
do { if (FAILED((expr))) { ERRORHRLOG((err),(expr)); if( 1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr) ) _CrtDbgBreak(); } } while (0)
#else
#define LOGASSERTEHR(expr,err) LOGASSERTHR(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTを出すマクロ</b>\n
 * アサートダイアログの中に条件式も出したい場合はLOGASSERTE_IFを
 * 使用してください。
 */
#ifdef _DEBUG
#define ASSERT_IF(expr) if( !(expr) ) { \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define ASSERT_IF(expr) if( ! (expr) ) {} else
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTを出すマクロ</b>\n
 */
#ifdef _DEBUG
#define ASSERTE_IF(expr) if( !(expr) ) { \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define ASSERTE_IF(expr) ASSERT_IF(expr)
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTEとログを出すマクロ</b>\n
 * アサートダイアログの中に条件式も出したい場合はLOGASSERTE_IFを
 * 使用してください。
 */
#ifdef _DEBUG
#define LOGASSERT_IF(expr,err) if( !(expr) ) { \
	ERRORLOG(err); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERT_IF(expr,err) if( !(expr) ) { ERRORLOG(err); } else
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTEとログを出すマクロ</b>\n
 * アサートダイアログの中に条件式も出したい場合はLOGASSERTE_IFを
 * 使用してください。
 */
#ifdef _DEBUG
#define LOGASSERTM_IF(expr,err,format,...) if( !(expr) ) { \
	_ERRORLOG((err),(format),__VA_ARGS__); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTM_IF(expr,err,format,...) if( !(expr) ) { _ERRORLOG((err),(format),__VA_ARGS__); } else
#endif // ifdef _DEBUG


/**
 * @brief <b>条件にそぐわなければASSERTとログを出すマクロ</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTE_IF(expr,err) if( !(expr) ) { \
	ERRORLOG(err); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTE_IF(expr,err) LOGASSERT_IF(expr,err)
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTとログを出すマクロ</b>\n
 */
#ifdef _DEBUG
#define LOGASSERTEM_IF(expr,err,format,...) if( !(expr) ) { \
	_ERRORLOG(err,(format),__VA_ARGS__); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTEM_IF(expr,err,format,...) LOGASSERTM_IF(expr,err,format,__VA_ARGS__)
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTEとログを出すマクロ</b>\n
 * emErrにエラーコードをセットします。emErrは呼び出し元で定義する
 * 必要があります。\n
 * アサートダイアログの中に条件式も出したい場合はLOGASSERTEERR_IFを
 * 使用してください。
 */
#ifdef _DEBUG
#define LOGASSERTERR_IF(expr,err) if( !(expr) ) { \
	emErr = err; \
	ERRORLOG(emErr); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTERR_IF(expr,err) if( !(expr) ) { emErr = err; ERRORLOG(err); } else
#endif // ifdef _DEBUG

/**
 * @brief <b>条件にそぐわなければASSERTとログを出すマクロ</b>\n
 * emErrにエラーコードをセットします。emErrは呼び出し元で定義する
 * 必要があります。
 */
#ifdef _DEBUG
#define LOGASSERTEERR_IF(expr,err) if( !(expr) ) { \
	emErr = err; \
	ERRORLOG(emErr); \
	do { if (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr)) _CrtDbgBreak(); } while (0); \
	} else
#else
#define LOGASSERTEERR_IF(expr,err) LOGASSERTERR_IF(expr,err)
#endif // ifdef _DEBUG

#if defined(_USE_SINDYLIB_STATIC) || defined(SINDYLIB_EXPORTS) || defined(_LIB)
/**
 * @brief <b>COMエラーを取得する関数</b>\n
 * この関数はCStringを返すため、DLL版では非公開メソッドになります
 */
CString GETCOMERRORSTRING();
#endif // if defined(_USE_SINDYLIB_STATIC) || defined(SINDYLIB_EXPORTS) || defined(_LIB)

#endif // _UTIL_H_
