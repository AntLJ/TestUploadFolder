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
 * @file Progressor.h
 * @brief CProgressor定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _PROGRESSOR_H_
#define _PROGRESSOR_H_

#include "exportimport.h"

/**
 * @class CProgressor
 * @brief <b>ICancelTracker及びIProgressorをまとめて管理するクラス</b>\n
 * プログレスダイアログ、プログレスバー、キャンセルトラッカーをまとめて管理します。\n
 * プログレッサーの初期レンジは0-100、ステップは10です。
 */
class SINDYLIB_API CProgressor
{
public:
	CProgressor();
	virtual ~CProgressor();

	CProgressor( ITrackCancel* lp );

	/**
	 * @brief <b>プログレスダイアログを表示するための親ウィンドウハンドルをセットする</b>\n
	 * プログレスダイアログ使用時には必ずセットする必要があります。
	 * @param hWnd	[in]	親ウィンドウハンドル
	 */
	void SetHWND( HWND hWnd );

	/**
	 * @brief <b>キャンセルトラッカーを取得する</b>\n
	 * 無ければ自動的に作成されます。
	 */
	ITrackCancelPtr GetTrackCancel() const;
	IProgressorPtr GetProgressor() const;
	void ShowProgressBar( bool bShow = true ) const;
	void ShowProgressDialog( bool bShow = true );
	bool Continue() const;
private:
	HWND			m_hWnd;
	ITrackCancel*	m_ipTrackCancel;
};

#endif // ifndef _PROGRESSOR_H_
