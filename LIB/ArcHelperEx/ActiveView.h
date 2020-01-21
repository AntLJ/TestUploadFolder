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
 * @file ActiveView.h
 * @brief CActiveView クラス定義ファイル
 * @author 地図DB制作部システム開発グループ 古川 貴宏
 * @version $Id$
 */
#ifndef _ACTIVEVIEW_H_
#define _ACTIVEVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atl2/seal.h"

/**
 * @class CActiveView
 * @brief <b>IActiveView ラップクラス</b>\n
 */
class CActiveView : public atl2::seal<CComPtr<IActiveView> >
{
public:
	CActiveView(){}
	virtual ~CActiveView(){}

	CActiveView( IActiveView* lp )
	{
		*this = lp;
	}

	CActiveView& operator=( IActiveView* lp )
	{
        if(*this!=lp)
    		AtlComPtrAssign((IUnknown**)&p, lp);

        return *this;
	}

	IActiveView** Release()
	{
		CComPtr<IActiveView>::Release();
		return &p;
	}

	//@{ @name IActiveViewインターフェース
	IEnvelopePtr GetExtent() const;
	IMapPtr GetFocusMap() const;
	IScreenDisplayPtr GetScreenDisplay() const;
	//@}

	/**
	 * @brief <b>センタリングと拡大率変更</b>\n
	 * @param ipPoint	[in]			移動先中心ポイント
	 * @param dScale	[in,optional]	拡大率（負の場合は拡大率無視。デフォルトでは無視）
	 * @param async		[in,optional]	描画が終了するまで待たないかどうか（デフォルト：false）
	 */
	HRESULT MoveCenterTo( IPoint* ipPoint, double dScale = -1.0, bool async = false ) const;

	/**
	 * @brief <b>マウスポインタの位置が中心にくるようにスクロールする</b>\n
	 */
	bool MoveCenterToMousePointer(bool bSuppressEvents = true) const;

	/**
	 * @brief <b>キー操作での移動拡大縮小処理</b>\n
	 * @param lKeyCode	[in]	キーコード
	 * @param lShift	[in]	シフトキー
	 * @retval true 移動処理を行った
	 * @retval false 移動処理を行わなかった
	 */
	bool ZooMoveByKey( long lKeyCode, long lShift ) const;

	/**
	 * @brief <b>中心にカーソルを移動させる</b>\n
	 */
	bool MoveCursorToCenter() const;

	/**
	 * @brief <b>画面の移動を管理</b>\n
	 * @param nXStep	[in]	
	 * @param nYStep	[in]
	 */
	bool Shift(int nXStep, int nYStep) const;

	/**
	 * @brief <b>MapFANのZM値に則した拡大率での拡大処理</b>\n
	 * @param bUpScale	[in,optional]	ZM値
	 */
	bool Zoom( bool bUpScale/* = true*/ ) const;
};

#endif // _ACTIVEVIEW_H_
