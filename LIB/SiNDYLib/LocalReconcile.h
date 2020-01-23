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
 * @file LocalReconcile.h
 * @brief <b>ローカルリコンサイル定義ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _LOCALRECONCILE_H_
#define _LOCALRECONCILE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {

/**
 * @brief <b>ローカルリコンサイルを実行する</b>\n
 * 渡されるワークスペースは編集バージョン（子バージョン）であり、
 * 且つ編集が終了（StopEditing）している必要があります。
 * また、ローカルリコンサイル実行後編集バージョンは削除しませんので、
 * 削除する必要があります。
 * @param ipWorkspace	[in]			編集バージョン
 * @param ipTrackCancel	[in]	キャンセルトラッカー（NULL OK）
 * @param hr			[out]			HRESULT
 * @param nRetryMax		[in,optional]	リトライ最大数（デフォルト：-1 … 無制限）
 * @param nRetryWait	[in,optional]	リトライ時のウェイト（デフォルト：0ms）
 * @retval sindyErr_EditConflictDetected コンフリクトがおきた
 * @retval sindyErr_NoErr ローカルリコンサイル成功
 * @retval 上記以外 エラー
 */
errorcode::sindyErrCode SINDYLIB_API LocalReconcile( IWorkspace* ipWorkspace, ITrackCancel* ipTrackCancel, HRESULT& hr, int bRetryMax = -1, int nRetryWait = 0 );

} // sindy

#endif // ifndef _LOCALRECONCILE_H_
