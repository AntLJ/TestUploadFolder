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
 * @brief �J�[�\�������p��Ԃœǂݍ��ށB
 *
 * @param wID [in] ���\�[�XID�B
 * @return �J�[�\���n���h���B
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
 * @brief �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����擾�E�ێ��E�������N���X�B
 */
class WindowDC //: boost::noncopyable
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief �f�o�C�X�R���e�L�X�g�n���h���� GetDC() ����B
	 *
	 * @param hWnd [in] �E�B���h�E�n���h���B
	 */
	WindowDC(){}
	WindowDC(HWND hWnd)
	{
		init( hWnd );
	}

	/**
	 * @brief �f�o�C�X�R���e�L�X�g�n���h���� ReleaseDC() ����B
	 */
	~WindowDC()
	{
		::ReleaseDC(m_hwnd, m_hdc);
	}

// ���Z
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
// �ϐ�
	HWND m_hwnd; ///< �f�o�C�X�R���e�L�X�g�n���h���擾���E�B���h�E�n���h���B ReleaseDC() �ŕK�v�ɂȂ�B
	HDC m_hdc; ///< �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h���B
};

/**
 * @brief �݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬�E�ێ��E�폜����N���X�B
 */
class OffscreenDC : boost::noncopyable
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	OffscreenDC(){}
	OffscreenDC(HDC hdcScreen, const RECT& rect)
	{
		init(hdcScreen, rect);
	}

	/**
	 * @brief �������f�o�C�X�R���e�L�X�g�� DeleteDC() ����B
	 *
	 * DIB�� DeleteObject() ����B
	 */
	~OffscreenDC()
	{
		::DeleteObject(m_hbmp);
		::DeleteDC(m_hdc);
	}

// ���Z
	operator HDC() const
	{
		return m_hdc;
	}

	/**
	 * @brief �݊����̂��郁�����f�o�C�X�R���e�L�X�g�� CreateCompatibleDC() ����B
	 *
	 * �������f�o�C�X�R���e�L�X�g�ɂ�DIB�����蓖�Ă�B
	 *
	 * @param hdcScreen [in] ���ƂȂ�f�o�C�X�R���e�L�X�g�B
	 * @param rect [in] �f�o�C�X�R���e�L�X�g�̑傫���B
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
// �ϐ�
	HDC m_hdc;
	HBITMAP m_hbmp;
};

/**
 * @brief �g��k���X�N���[�������Ɏg�p��������ЂƂ܂Ƃ߂ɂ��ĊǗ�����N���X�B
 */
class Data
{
public:
// �R���X�g���N�^
	/**
	 * @brief ���𐮂���B
	 *
	 * -# �A�N�e�B�u�r���[�E�B���h�E�̃f�o�C�X�R���e�L�X�g���擾����B
	 * -# �ꎞ��Ɨp�̃������f�o�C�X�R���e�L�X�g��p�ӂ���B
	 * -# �J�n���̃}�E�X���W���L�^����B
	 * -# �ړ��ʏ�������������B
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

// �ݒ�
	/**
	 * @brief �X�N���[���ʂ�ݒ肷��
	 *
	 * @param nX [in] �����ړ��ʁB
	 * @param nY [in] ����ړ��ʁB
	 */
	void setScrollAmount(int nX, int nY)
	{
		m_ptScrollAmount.x = nX;
		m_ptScrollAmount.y = nY;
	}

	/**
	 * @brief �g��k���ʂ�ݒ肷��B
	 *
	 * @param nAmount [in] ����ړ��ʁB
	 */
	void addZoomAmount(int nAmount)
	{
		m_dZoomAmount += nAmount * 0.009;
	}

	/**
	 * @brief �ړ��ʂ������l�ɖ߂��B
	 */
	void resetAmount()
	{
		setScrollAmount(0, 0);
		m_dZoomAmount = 0.0;
	}

// �擾
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
// �ϐ�
	IActiveViewPtr m_ipActiveView; ///< ����ΏۃA�N�e�B�u�r���[�B
	IScreenDisplayPtr m_ipScreenDisplay; ///< ����ΏۃX�N���[���f�B�X�v���C�B
	RECT m_rectDeviceFrame; ///< �X�N���[���f�B�X�v���C�̃f�o�C�X�g�B
	WindowDC m_hdcScreen; ///< �X�N���[���f�B�X�v���C�̃f�o�C�X�R���e�L�X�g�n���h���B
	OffscreenDC m_hdcOffscreen; ///< @p m_hdcScreen �݊��̃������f�o�C�X�R���e�L�X�g�n���h���B
	POINT m_ptOrigin; ///< ����J�n�_�B

	POINT m_ptScrollAmount; ///< �X�N���[���ʁB
	double m_dZoomAmount; ///< �g��k���ʁB
};

/**
 * @brief �g��k���X�N���[��������s���N���X�̊��N���X�B
 */
class Operator
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief �J�[�\����ǂݍ��ށB
	 */
	Operator(WORD wID = 0) :
	m_hCursor(share_cursor(wID))
	{
	}

	/**
	 * @brief ���ɉ������Ȃ��B
	 */
	virtual ~Operator()
	{
	}

// �擾
	HCURSOR getCursor() const { return m_hCursor; }

// �������z�֐�
	/**
	 * @brief �g��k���X�N���[��������s���B
	 *
	 * @param rPoint [in] ���݂̃}�E�X���W�B
	 */
	virtual void operator()(const POINT& rPoint) = 0;

private:
// �ϐ�
	HCURSOR m_hCursor; ///< �J�[�\���n���h���B
};

/**
 * @brief �X�N���[���������s���N���X�B
 */
class Mover : public Operator
{
public:
// �R���X�g���N�^
	Mover(Data& rData, const POINT& rScreenPoint) :
	Operator(/*IDC_PAN*/),
	m_rData(rData),
	m_ptBaseAmount(rData.getScrollAmount()),
	m_ptFulcrum(rScreenPoint)
	{
	}

// �������z�֐��̉���
	virtual void operator()(const POINT& rPoint)
	{
		const double dInvertScale = m_rData.getInvertScale();

		m_rData.setScrollAmount(
			static_cast<int>((rPoint.x - m_ptFulcrum.x) * dInvertScale) + m_ptBaseAmount.x,
			static_cast<int>((rPoint.y - m_ptFulcrum.y) * dInvertScale) + m_ptBaseAmount.y
		);
	}

private:
// �ϐ�
	Data& m_rData;
	POINT m_ptBaseAmount;
	POINT m_ptFulcrum;
};

/**
 * @brief �g��k���������s���N���X�B
 */
class Zoomer : public Operator
{
public:
// �R���X�g���N�^
	Zoomer(Data& rData, const POINT& rScreenPoint) :
	Operator(/*IDC_CONTINUOUSZOOM*/),
	m_rData(rData),
	m_nPrevY(rScreenPoint.y)
	{
	}

// �������z�֐��̉���
	virtual void operator()(const POINT& rPoint)
	{
		m_rData.addZoomAmount(rPoint.y - m_nPrevY);
		m_nPrevY = rPoint.y;
	}

private:
// �ϐ�
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
/*		// �}�E�X�̃L���v�`�������ɖ߂��B
		if(::ReleaseCapture() && m_hwndCaptured) {
			::SetCapture(m_hwndCaptured);
		}*/

		// �J�[�\����߂��B
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

		// �J�[�\�����������琏������B
		(*m_pOp)(pt);

		// �����Ă������ʂɔ��f�B
		bitblt();
	}

	void apply()
	{
		CWaitCursor cursor;
		// �O�����B
		IActiveViewPtr ipActiveView(m_Data.getActiveView());
		const double dInvertScale = m_Data.getInvertScale();
		const POINT& rScrollAmount = m_Data.getScrollAmount();

		// �}�E�X�N���b�N�ōĕ`�悪�����肤�邳�����߁A�����Ă��Ȃ����ǂ������ׂ�
		if( rScrollAmount.x == 0 && rScrollAmount.y == 0 && dInvertScale == 1.0 )
			return;

		IDisplayTransformationPtr ipDispTrans;
		m_Data.getScreenDisplay()->get_DisplayTransformation(&ipDispTrans);
		double dResolutionTmp = 0.0;
		ipDispTrans->get_Resolution( &dResolutionTmp );
		const double dResolution = 72.0 / dResolutionTmp;

		// �`��͈͊O�ڋ�`���擾����B
		IEnvelopePtr ipEnv;
		ipActiveView->get_Extent(&ipEnv);

		// �X�N���[����K�p����B
		double dX = 0, dY = 0;
		ipDispTrans->FromPoints(- rScrollAmount.x * dResolution, &dX);
		ipDispTrans->FromPoints(rScrollAmount.y * dResolution, &dY);
		if ( rScrollAmount.x != 0 || rScrollAmount.y != 0 )
			ipEnv->Offset( dX, dY );

		// �g��k����K�p����B
		if ( dInvertScale != 1.0 )
			ipEnv->Expand(dInvertScale, dInvertScale, VARIANT_TRUE);

		// �������m�肵�A�ĕ`���v������B
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
		// �g��k���X�N���[�����Ȃ��������Ƃɂ���B
		m_Data.resetAmount();

		// �����Ă������ʂɔ��f�B
		bitblt();
	}

private:
// ��`
	typedef boost::shared_ptr<Operator> OperatorRef;

// ����
	/**
	 * @brief ����󋵂��ȈՓI�ɕ\������B
	 */
	void bitblt()
	{
		// �O�����B
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

		// �I�t�X�N���[����^�����ɁB
		::BitBlt(
			hdcOffcscreen, rRect.left, rRect.top,
			nWidth, nHeight,
			NULL, 0, 0,
			WHITENESS
		);

		// �g��k���X�N���[�������Ȃ���I�t�X�N���[���֕��ʁB
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

		// �I�t�X�N���[�������ʂ֕��ʁB
		::BitBlt(
			hdcScreen, rRect.left, rRect.top,
			nWidth, nHeight,
			hdcOffcscreen, rRect.left, rRect.top,
			SRCCOPY
		);
	}

// �ϐ�
	Data m_Data; ///< �f�[�^�B
	HCURSOR m_hcurCursor; ///< ���̃J�[�\���B
//	HWND m_hwndCaptured; ///< ���̃L���v�`���E�B���h�E�B
	bool m_bPage;

	OperatorRef m_pOp; ///< ���샂�[�h�B
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
 * @brief ���샂�[�h��؂�ւ���B
 *
 * @param eMode [in] ���샂�[�h�B
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
 * @brief �}�E�X�J�[�\���̌��݈ʒu��ǂݎ��A�g��k���X�N���[������B
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
 * @brief �������������B
 */
void ZooMoveController::apply()
{
	pimpl->apply();
}

/**
 * @brief ������L�����Z������B
 */
void ZooMoveController::cancel()
{
	pimpl->cancel();
}
