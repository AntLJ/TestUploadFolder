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
 * @file SiNDYLogFunctions.h
 * @brief <b>SiNDYでハンドリング可能なログのユーティリティ関数群定義ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#ifndef _SINDYLOGFUNCTIONS_H_
#define _SINDYLOGFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {
enum sindyLogFormat {
	sindyLogFormat_Common,			//!< ログハンドル標準フォーマット（カンマ区切り）
	sindyLogFormat_Freestyle,		//!< 自由形式ログ（カンマ区切り）
	sindyLogFormat_CommonTab,		//!< ログハンドル標準フォーマット（タブ区切り）
	sindyLogFormat_FreestyleTab,	//!< 自由形式ログ（タブ区切り）
};

enum sindyLogLevel {
	sindyLogLevel_INFO,			//!< 情報
	sindyLogLevel_WARN,			//!< 警告
	sindyLogLevel_ERR,			//!< エラー
};

/**
 * @brief ログハンドルツール用のヘッダを出力する
 *
 * フリースタイルログの場合のフォーマットは以下のとおりです。<br>
 * @code
 *   FLAG,LAYER,OBJECTID,MESHCODE,X,Y,LATITUDE,LONGITUDE,SCALE,COMMENT{エラーレベル},COMMENT{エラーコード},COMMENT{エラーメッセージ},COMMENT{備考},COMMENT{フリーコメント}
 * @endcode
 * 出力文字コードはSJISです。
 *
 * @param out				[in]			ストリーム
 * @param emFormat			[in]			フォーマット形式
 */
void SINDYLIB_API SINDYLOGHEADER( FILE* out, sindyLogFormat emFormat );

/**
 * @brief ログハンドルツール対応のフォーマット形式でログを出力する
 *
 * エラーコード、エラー文字列はsindyErrCodeから取得します。<br>
 * 標準形式の場合、最後の文字列は情報１として出力されます。<br>
 * フリースタイルの場合のフォーマットはSINDYLOGHEADERを参照してください。<br>
 * 出力文字コードはSJISです。
 *
 * @param out				[in]			ストリーム
 * @param emFormat			[in]			フォーマット形式
 * @param lpcszTableName	[in]			テーブル名
 * @param lOID				[in]			オブジェクトID
 * @param scale				[in]			スケール
 * @param dX				[in]			代表X座標
 * @param dY				[in]			代表Y座標
 * @param emErrLevel		[in]			エラーレベル
 * @param emErr				[in]			エラーコード
 * @param console			[in]			標準出力に出すかどうか
 * @param dbg				[in]			デバッグウィンドウに出力するかどうか
 * @param lpcszFormat		[in,optional]	情報１
 */
void SINDYLIB_API SINDYCOMMONLOG2V( FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat, va_list args );
void SINDYLIB_API SINDYCOMMONLOG2(  FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat = NULL, ... );
void SINDYLIB_API SINDYCOMMONLOG(   FILE* out, sindyLogFormat emFormat, LPCTSTR lcpszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, errorcode::sindyErrCode emErr, LPCTSTR lpcszFormat = NULL, ... );

} // sindy

#endif // _SINDYLOGFUNCTIONS_H_
