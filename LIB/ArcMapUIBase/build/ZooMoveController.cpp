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

#include "stdafx.h"
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
//#include "resource.h"
#include "ZooMoveController.h"
#include <math.h>
#include "../WinLib/WaitCursor.h"
#include "../LIB/ArcHelperEx/AheZooMoveFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {

/**
 * @brief カーソルを共用状態で読み込む。
 *
 * @param wID [in] リソースID。
 * @return カーソルハンドル。
 */
inline HCURSOR share_cursor(WORD wID)
{
	return
		static_cast<HCURSOR>(
			::LoadImage(
				_AtlBaseModule.GetResourceInstance(),
				MAKEINTRESOURCE(wID),
				IMAGE_CURSOR,
				0, 0,
				LR_DEFAULTSIZE | LR_SHARED
			)
		);
}

/**
 * @brief ウィンドウのデバイスコンテキストハンドルを取得・保持・解放するクラス。
 */
class WindowDC //: boost::noncopyable
{
public:
// コンストラクタとデストラクタ
	/**
	 * @brief デバイスコンテキストハンドルを GetDC() する。
	 *
	 * @param hWnd [in] ウィンドウハンドル。
	 */
	WindowDC(){}
	WindowDC(HWND hWnd)
	{
		init( hWnd );
	}

	/**
	 * @brief デバイスコンテキストハンドルを ReleaseDC() する。
	 */
	~WindowDC()
	{
		::ReleaseDC(m_hwnd, m_hdc);
	}

// 演算
	operator HDC() const
	{
		return m_hdc;
	}

	void init( HWND hWnd )
	{
		m_hwnd = hWnd;
		m_hdc = GetDC(hWnd);
	}
private:
// 変数
	HWND m_hwnd; ///< デバイスコンテキストハンドル取得元ウィンドウハンドル。 ReleaseDC() で必要になる。
	HDC m_hdc; ///< ウィンドウのデバイスコンテキストハンドル。
};

/**
 * @brief 互換性のあるメモリデバイスコンテキストを作成・保持・削除するクラス。
 */
class OffscreenDC : boost::noncopyable
{
public:
// コンストラクタとデストラクタ
	OffscreenDC(){}
	OffscreenDC(HDC hdcScreen, const RECT& rect)
	{
		init(hdcScreen, rect);
	}

	/**
	 * @brief メモリデバイスコンテキストを DeleteDC() する。
	 *
	 * DIBも DeleteObject() する。
	 */
	~OffscreenDC()
	{
		::DeleteObject(m_hbmp);
		::DeleteDC(m_hdc);
	}

// 演算
	operator HDC() const
	{
		return m_hdc;
	}

	/**
	 * @brief 互換性のあるメモリデバイスコンテキストを CreateCompatibleDC() する。
	 *
	 * メモリデバイスコンテキストにはDIBを割り当てる。
	 *
	 * @param hdcScreen [in] 元となるデバイスコンテキスト。
	 * @param rect [in] デバイスコンテキストの大きさ。
	 */
	void init(HDC hdcScreen, const RECT& rect)
	{
		m_hdc = ::CreateCompatibleDC(hdcScreen);

		BITMAPINFO bmpi;
		bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);
		bmpi.bmiHeader.biWidth = rect.right;
		bmpi.bmiHeader.biHeight = -rect.bottom;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;
		bmpi.bmiHeader.biClrUsed = 0;
		bmpi.bmiHeader.biClrImportant = 0;

		m_hbmp = ::CreateDIBSection(m_hdc, &bmpi, DIB_RGB_COLORS, 0, NULL, 0);

		::SelectObject(m_hdc, m_hbmp);
		::SetStretchBltMode( m_hdc, HALFTONE );
		::SetBrushOrgEx(m_hdc, 0,0, NULL);
	}
private:
// 変数
	HDC m_hdc;
	HBITMAP m_hbmp;
};

/**
 * @brief 拡大縮小スクロール処理に使用する情報をひとまとめにして管理するクラス。
 */
class Data
{
public:
// コンストラクタ
	/**
	 * @brief 環境を整える。
	 *
	 * -# アクティブビューウィンドウのデバイスコンテキストを取得する。
	 * -# 一時作業用のメモリデバイスコンテキストを用意する。
	 * -# 開始時のマウス座標を記録する。
	 * -# 移動量情報を初期化する。
	 */
	Data(IActiveView* ipActiveView, const POINT& rScreenPoint) :
	m_ipActiveView(ipActiveView),
	m_ptOrigin(rScreenPoint)
	{
		m_ipActiveView->get_ScreenDisplay(&m_ipScreenDisplay);
		IDisplayTransformationPtr ipDispTrans;
		m_ipScreenDisplay->get_DisplayTransformation(&ipDispTrans);
		ipDispTrans->get_DeviceFrame(&m_rectDeviceFrame);
		HWND hWnd = 0;
		m_ipScreenDisplay->get_hWnd( (OLE_HANDLE*)&hWnd );
		m_hdcScreen.init( hWnd );
		m_hdcOffscreen.init( m_hdcScreen, m_rectDeviceFrame );
		resetAmount();
	}

// 設定
	/**
	 * @brief スクロール量を設定する
	 *
	 * @param nX [in] 東方移動量。
	 * @param nY [in] 南方移動量。
	 */
	void setScrollAmount(int nX, int nY)
	{
		m_ptScrollAmount.x = nX;
		m_ptScrollAmount.y = nY;
	}

	/**
	 * @brief 拡大縮小量を設定する。
	 *
	 * @param nAmount [in] 南方移動量。
	 */
	void addZoomAmount(int nAmount)
	{
		m_dZoomAmount += nAmount * 0.009;
	}

	/**
	 * @brief 移動量を初期値に戻す。
	 */
	void resetAmount()
	{
		setScrollAmount(0, 0);
		m_dZoomAmount = 0.0;
	}

// 取得
	IActiveViewPtr getActiveView() const { return m_ipActiveView; }
	IScreenDisplayPtr getScreenDisplay() const { return m_ipScreenDisplay; }
	const RECT& getDeviceFrame() const { return m_rectDeviceFrame; }
	HDC getScreenDC() const { return m_hdcScreen; }
	HDC getOffscreenDC() const { return m_hdcOffscreen; }
	const POINT& getOrigin() const { return m_ptOrigin; }
	
	const POINT& getScrollAmount() const { return m_ptScrollAmount; }
	double getZoomScale() const { return ::exp(m_dZoomAmount); }
	double getInvertScale() const { return ::exp(-m_dZoomAmount); }

private:
// 変数
	IActiveViewPtr m_ipActiveView; ///< 操作対象アクティブビュー。
	IScreenDisplayPtr m_ipScreenDisplay; ///< 操作対象スクリーンディスプレイ。
	RECT m_rectDeviceFrame; ///< スクリーンディスプレイのデバイス枠。
	WindowDC m_hdcScreen; ///< スクリーンディスプレイのデバイスコンテキストハンドル。
	OffscreenDC m_hdcOffscreen; ///< @p m_hdcScreen 互換のメモリデバイスコンテキストハンドル。
	POINT m_ptOrigin; ///< 操作開始点。

	POINT m_ptScrollAmount; ///< スクロール量。
	double m_dZoomAmount; ///< 拡大縮小量。
};

/**
 * @brief 拡大縮小スクロール操作を行うクラスの基底クラス。
 */
class Operator
{
public:
// コンストラクタとデストラクタ
	/**
	 * @brief カーソルを読み込む。
	 */
	Operator(WORD wID = 0) :
	m_hCursor(share_cursor(wID))
	{
	}

	/**
	 * @brief 特に何もしない。
	 */
	virtual ~Operator()
	{
	}

// 取得
	HCURSOR getCursor() const { return m_hCursor; }

// 純粋仮想関数
	/**
	 * @brief 拡大縮小スクロール操作を行う。
	 *
	 * @param rPoint [in] 現在のマウス座標。
	 */
	virtual void operator()(const POINT& rPoint) = 0;

private:
// 変数
	HCURSOR m_hCursor; ///< カーソルハンドル。
};

/**
 * @brief スクロール処理を行うクラス。
 */
class Mover : public Operator
{
public:
// コンストラクタ
	Mover(Data& rData, const POINT& rScreenPoint) :
	Operator(/*IDC_PAN*/),
	m_rData(rData),
	m_ptBaseAmount(rData.getScrollAmount()),
	m_ptFulcrum(rScreenPoint)
	{
	}

// 純粋仮想関数の解決
	virtual void operator()(const POINT& rPoint)
	{
		const double dInvertScale = m_rData.getInvertScale();

		m_rData.setScrollAmount(
			static_cast<int>((rPoint.x - m_ptFulcrum.x) * dInvertScale) + m_ptBaseAmount.x,
			static_cast<int>((rPoint.y - m_ptFulcrum.y) * dInvertScale) + m_ptBaseAmount.y
		);
	}

private:
// 変数
	Data& m_rData;
	POINT m_ptBaseAmount;
	POINT m_ptFulcrum;
};

/**
 * @brief 拡大縮小処理を行うクラス。
 */
class Zoomer : public Operator
{
public:
// コンストラクタ
	Zoomer(Data& rData, const POINT& rScreenPoint) :
	Operator(/*IDC_CONTINUOUSZOOM*/),
	m_rData(rData),
	m_nPrevY(rScreenPoint.y)
	{
	}

// 純粋仮想関数の解決
	virtual void operator()(const POINT& rPoint)
	{
		m_rData.addZoomAmount(rPoint.y - m_nPrevY);
		m_nPrevY = rPoint.y;
	}

private:
// 変数
	Data& m_rData;
	int m_nPrevY;
};

} // anonymous namespace

/////////////////////////////////////////////////////////////////////////////
// ZooMoveController::Impl
/////////////////////////////////////////////////////////////////////////////

class ZooMoveController::Impl
{
public:
	Impl(IActiveView* ipActiveView, const POINT& rScreenPoint, bool bPage=false) :
	m_bPage(bPage),
	m_Data(ipActiveView, rScreenPoint),
	m_hcurCursor(::GetCursor())
	//m_hwndCaptured(::SetCapture(reinterpret_cast<HWND>(m_Data.getActiveView()->ScreenDisplay->GethWnd())))
	{
/*		HWND hWnd = 0;
		IScreenDisplayPtr ipDisp;
		m_Data.getActiveView()->get_ScreenDisplay( &ipDisp );
		ipDisp->get_hWnd( (OLE_HANDLE*)&hWnd );
		m_hwndCaptured = ::SetCapture( hWnd );*/
	}

	~Impl()
	{
/*		// マウスのキャプチャを元に戻す。
		if(::ReleaseCapture() && m_hwndCaptured) {
			::SetCapture(m_hwndCaptured);
		}*/

		// カーソルを戻す。
		::SetCursor(m_hcurCursor);
	}

	void changeMode(EMode eMode, const POINT& rScreenPoint)
	{
		OperatorRef pOp;

		switch(eMode) {
		case kMoveMode:
			pOp.reset(new Mover(m_Data, rScreenPoint));
			break;
		case kZoomMode:
			pOp.reset(new Zoomer(m_Data, rScreenPoint));
			break;
		}

		if(pOp) {
			::SetCursor(pOp->getCursor());
		}

		m_pOp = pOp;
	}

	void onMouseMove(const POINT& pt)
	{
		if(! m_pOp)
			return;

		// カーソルが動いたら随時操作。
		(*m_pOp)(pt);

		// そしてそれを画面に反映。
		bitblt();
	}

	void apply()
	{
		CWaitCursor cursor;
		// 前準備。
		IActiveViewPtr ipActiveView(m_Data.getActiveView());
		const double dInvertScale = m_Data.getInvertScale();
		const POINT& rScrollAmount = m_Data.getScrollAmount();

		// マウスクリックで再描画がかかりうるさいため、動いていないかどうか調べる
		if( rScrollAmount.x == 0 && rScrollAmount.y == 0 && dInvertScale == 1.0 )
			return;

		IDisplayTransformationPtr ipDispTrans;
		m_Data.getScreenDisplay()->get_DisplayTransformation(&ipDispTrans);
		double dResolutionTmp = 0.0;
		ipDispTrans->get_Resolution( &dResolutionTmp );
		const double dResolution = 72.0 / dResolutionTmp;

		// 描画範囲外接矩形を取得する。
		IEnvelopePtr ipEnv;
		ipActiveView->get_Extent(&ipEnv);

		// スクロールを適用する。
		double dX = 0, dY = 0;
		ipDispTrans->FromPoints(- rScrollAmount.x * dResolution, &dX);
		ipDispTrans->FromPoints(rScrollAmount.y * dResolution, &dY);
		if ( rScrollAmount.x != 0 || rScrollAmount.y != 0 )
			ipEnv->Offset( dX, dY );

		// 拡大縮小を適用する。
		if ( dInvertScale != 1.0 )
			ipEnv->Expand(dInvertScale, dInvertScale, VARIANT_TRUE);

		// 処理を確定し、再描画を要請する。
		if ( AheIsRefreshMode() && ! m_bPage )
		{
			HDC hdcCacheMemDC = 0;
			m_Data.getScreenDisplay()->get_CacheMemDC(0, (OLE_HANDLE*)&hdcCacheMemDC);
			ITrackCancelPtr ipCancel;
			m_Data.getScreenDisplay()->get_CancelTracker( &ipCancel );

			const HDC hdcScreen = m_Data.getScreenDC();
			RECT rRect = m_Data.getDeviceFrame();
			const int nWidth = rRect.right - rRect.left;
			const int nHeight = rRect.bottom - rRect.top;

			::BitBlt( hdcCacheMemDC, rRect.left, rRect.top, nWidth, nHeight, NULL, 0, 0, WHITENESS );
			ipActiveView->Output( (OLE_HANDLE)hdcCacheMemDC, 0, &rRect, ipEnv, ipCancel ); 
			::BitBlt( hdcScreen, rRect.left, rRect.top, nWidth, nHeight, hdcCacheMemDC, rRect.left, rRect.top, SRCCOPY );
			ipActiveView->put_Extent(ipEnv);

			m_Data.getScreenDisplay()->Invalidate( ipEnv, VARIANT_TRUE, (esriViewDrawPhase)(esriViewBackground) );
			ipActiveView->PartialRefresh((esriViewDrawPhase)(esriViewForeground), NULL, ipEnv);
		}
		else
		{
			ipActiveView->put_Extent(ipEnv);
			ipActiveView->PartialRefresh(static_cast<esriViewDrawPhase>(127), NULL, ipEnv);
		}
	}

	void cancel()
	{
		// 拡大縮小スクロールをなかったことにする。
		m_Data.resetAmount();

		// そしてそれを画面に反映。
		bitblt();
	}

private:
// 定義
	typedef boost::shared_ptr<Operator> OperatorRef;

// 操作
	/**
	 * @brief 操作状況を簡易的に表示する。
	 */
	void bitblt()
	{
		// 前準備。
		HDC hdcCacheMemDC = 0;
		m_Data.getScreenDisplay()->get_CacheMemDC(0, (OLE_HANDLE*)&hdcCacheMemDC);
		const HDC hdcScreen = m_Data.getScreenDC();
		const HDC hdcOffcscreen = m_Data.getOffscreenDC();

		const RECT& rRect = m_Data.getDeviceFrame();
		const int nWidth = rRect.right - rRect.left;
		const int nHeight = rRect.bottom - rRect.top;

		const POINT& rScrollAmount = m_Data.getScrollAmount();
		const double dZoomScale = m_Data.getZoomScale();
		const double dExpandPow = (dZoomScale - 1.0) / 2.0;

		// オフスクリーンを真っ白に。
		::BitBlt(
			hdcOffcscreen, rRect.left, rRect.top,
			nWidth, nHeight,
			NULL, 0, 0,
			WHITENESS
		);

		// 拡大縮小スクロールさせながらオフスクリーンへ複写。
		::StretchBlt(
			hdcOffcscreen,
			static_cast<int>(rRect.left + rScrollAmount.x * dZoomScale - nWidth * dExpandPow),
			static_cast<int>(rRect.top + rScrollAmount.y * dZoomScale - nHeight * dExpandPow),
			static_cast<int>(nWidth * dZoomScale),
			static_cast<int>(nHeight * dZoomScale),
			hdcCacheMemDC,
			0,
			0,
			nWidth,
			nHeight,
			SRCCOPY
		);

		// オフスクリーンから画面へ複写。
		::BitBlt(
			hdcScreen, rRect.left, rRect.top,
			nWidth, nHeight,
			hdcOffcscreen, rRect.left, rRect.top,
			SRCCOPY
		);
	}

// 変数
	Data m_Data; ///< データ。
	HCURSOR m_hcurCursor; ///< 元のカーソル。
//	HWND m_hwndCaptured; ///< 元のキャプチャウィンドウ。
	bool m_bPage;

	OperatorRef m_pOp; ///< 操作モード。
};

/////////////////////////////////////////////////////////////////////////////
// ZooMoveController
/////////////////////////////////////////////////////////////////////////////

ZooMoveController::ZooMoveController(IActiveView* ipActiveView, bool bPage)
{
	POINT pt;
	::GetCursorPos(&pt);
	pimpl.reset(new Impl(ipActiveView, pt, bPage));
}

ZooMoveController::ZooMoveController(IActiveView* ipActiveView, const POINT& rScreenPoint, bool bPage) :
pimpl(new Impl(ipActiveView, rScreenPoint, bPage))
{
}


ZooMoveController::~ZooMoveController()
{
}

/**
 * @brief 操作モードを切り替える。
 *
 * @param eMode [in] 操作モード。
 */
void ZooMoveController::changeMode(EMode eMode)
{
	POINT pt;
	::GetCursorPos(&pt);
	pimpl->changeMode(eMode, pt);
}
void ZooMoveController::changeMode(EMode eMode, const POINT& rScreenPoint)
{
	pimpl->changeMode(eMode, rScreenPoint);
}


/**
 * @brief マウスカーソルの現在位置を読み取り、拡大縮小スクロールする。
 */
void ZooMoveController::onMouseMove()
{
	POINT pt;
	if(! ::GetCursorPos(&pt))
		return;

	pimpl->onMouseMove(pt);
}

void ZooMoveController::onMouseMove(const POINT& rScreenPoint)
{
	pimpl->onMouseMove(rScreenPoint);
}

/**
 * @brief 操作を完了する。
 */
void ZooMoveController::apply()
{
	pimpl->apply();
}

/**
 * @brief 操作をキャンセルする。
 */
void ZooMoveController::cancel()
{
	pimpl->cancel();
}
