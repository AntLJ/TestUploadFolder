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

#ifndef ZOOMOVE_CONTROLLER_H_
#define ZOOMOVE_CONTROLLER_H_

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

/**
 * @briev 拡大縮小スクロール操作を制御するクラス
 *
 * コンストラクタで
 * - マウスキャプチャの保持
 * - マウスカーソルの保持
 * を行い、デストラクタで元に戻す。
 */
class ZooMoveController : boost::noncopyable
{
	class Impl;
public:
	/// 操作モード。
	enum EMode {
		kMoveMode, ///< スクロール
		kZoomMode, ///< 拡大縮小
	};

// コンストラクタとデストラクタ
	ZooMoveController(IActiveView* ipActiveView, bool bPage=false);
	ZooMoveController(IActiveView* ipActiveView, const POINT& rScreenPoint, bool bPage=false);
	~ZooMoveController();

	void changeMode(EMode eMode);
	void changeMode(EMode eMode, const POINT& rScreenPoint);

	void onMouseMove();
	void onMouseMove(const POINT& rScreenPoint);

	void apply();
	void cancel();

private:
// 変数
	boost::scoped_ptr<Impl> pimpl;
};

#endif // ZOOMOVE_CONTROLLER_H_
