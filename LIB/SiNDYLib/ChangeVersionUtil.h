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
 * @file ChangeVersionUtil.h
 * @brief <b>バージョン切り替えユーティリティ関数定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _CHANGEVERSIONUTIL_H_
#define _CHANGEVERSIONUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {

/**
 * @brief <b>バージョンを変更する</b>\n
 * @param ipMxDoc				[in]	IMxDocument
 * @param ipOldWorkspace		[in]	バージョン変更前のワークスペース
 * @param lpcszNewVersionName	[in]	新しいバージョン名
 * @retval sindyErr_NoErr	バージョン変更成功
 * @retval 上記以外			バージョン変更失敗
 */
errorcode::sindyErrCode SINDYLIB_API AheChangeVersionByName( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, LPCTSTR lpcszNewVersionName );

/**
 * @brief <b>バージョンを変更する</b>\n
 * @param ipMxDoc				[in]	IMxDocument
 * @param ipOldWorkspace		[in]	バージョン変更前のワークスペース
 * @param ipNewWorkspace		[in]	バージョン変更後のワークスペース
 * @retval sindyErr_NoErr	バージョン変更成功
 * @retval 上記以外			バージョン変更失敗
 */
errorcode::sindyErrCode SINDYLIB_API AheChangeVersion( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, IFeatureWorkspace* ipNewWorkspace );

/**
 * @brief <b>渡されたバージョンの親バージョンを取得する</b>\n
 * @param ipVersion			[in]	バージョン
 * @retval 親バージョン
 */
IVersionPtr SINDYLIB_API AheGetParentVersion( IVersion* ipVersion );

} // sindy

#endif // ifndef _CHANGEVERSIONUTIL_H_
