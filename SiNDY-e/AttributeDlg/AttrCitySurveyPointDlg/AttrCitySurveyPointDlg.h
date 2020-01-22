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

// AttrCityLineDlg.h : CAttrCitySurveyPointDlg の宣言

#ifndef __ATTRCITYLINEDLG_H_
#define __ATTRCITYLINEDLG_H_

#include "resource.h"
#define XMD_H
#include "util/Jpeg.h"

#define	THUMBNAIL_WIDTH		200
#define	THUMBNAIL_HEIGHT	150

typedef CAttrBaseDlg ATTR_BASE_CLASS;

class CJpeg2 : public CJpeg  
{
public:
	CJpeg2()
	{
	}
	virtual ~CJpeg2()
	{
	}

	void draw(HWND hWnd, RECT rc) const
	{
		m_cDib.m_pImage ? draw_Picture2(hWnd, rc) : draw_NoPicture(hWnd);
	}
	void draw_Picture2(HWND hWnd, RECT rc) const
	{
		HDC dc = ::GetDC(hWnd);
		::SetStretchBltMode(dc, HALFTONE);
		::StretchDIBits(dc, rc.left, rc.top, abs(rc.left-rc.right), abs(rc.bottom-rc.top),
			0, 0, m_cDib.m_biHeader.biWidth, m_cDib.m_biHeader.biHeight, m_cDib.m_pImage, (BITMAPINFO *)&m_cDib.m_biHeader, DIB_RGB_COLORS, SRCCOPY);
		::ReleaseDC(hWnd, dc);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAttrCitySurveyPointDlg
class CAttrCitySurveyPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrCitySurveyPointDlg();
	~CAttrCitySurveyPointDlg()
	{
		::DeleteObject( m_hFont );
	}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCitySurveyPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_DISPLAYPICTURE, BN_CLICKED, OnClickedButton)
	COMMAND_HANDLER(IDC_BUTTON_CONFIRM, BN_CLICKED, OnClkConfirmBtn)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MSG_WM_HSCROLL(OnHScroll)
ALT_MSG_MAP(IDC_EDIT_MESH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RESEARCHDAY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_IDINMESH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TARGETDATA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ATTRNAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LAYER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_PRENAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_AFTNAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_POS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NAMEOK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CLOSE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CLEAR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CONSTRUCT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_UNCERTAIN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DISPLAYPICTURE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_CONFIRM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_CONFIRM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_SETFOCUS, OnComboFoucus) //[bug 6201]小細工用
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_MESH));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_RESEARCHDAY));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_IDINMESH));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_TARGETDATA));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_ATTRNAME));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_LAYER));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_PRENAME));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_AFTNAME));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_CHECK_POS));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_CHECK_NAMEOK));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_CHECK_CLOSE));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_CHECK_CLEAR));
		m_wnd13.SubclassWindow(GetDlgItem(IDC_CHECK_CONSTRUCT));
		m_wnd14.SubclassWindow(GetDlgItem(IDC_CHECK_UNCERTAIN));
		m_wnd15.SubclassWindow(GetDlgItem(IDC_BUTTON_DISPLAYPICTURE));
		m_wnd16.SubclassWindow(GetDlgItem(IDC_COMBO_CONFIRM));
		m_wnd17.SubclassWindow(GetDlgItem(IDC_BUTTON_CONFIRM));

		m_wnd0.SetFont( m_hFont );
		m_wnd1.SetFont( m_hFont );
		m_wnd2.SetFont( m_hFont );
		m_wnd3.SetFont( m_hFont );
		m_wnd6.SetFont( m_hFont );
		m_wnd7.SetFont( m_hFont );

		GetRegCode();
		m_lSlider = 0;
		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// 操作メッセージ
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

		// エディットボックスの時に全選択にする
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( m_strBaseDir.IsEmpty() || m_strCode2Path.IsEmpty() )
			return 0;

		CTrackBarCtrl cTrackBar( GetDlgItem( IDC_SLIDER_PIC ) );
		long lBarPos = cTrackBar.GetPos();

		HWND hPicWnd = GetDlgItem( IDC_STATIC_PIC );
		CWindow cWnd( hPicWnd );
		long lWidth, lHeight, lPicWidth, lPicHeight;
		RECT rect;
		cWnd.GetClientRect(&rect);
		lWidth = rect.right - rect.left;
		lHeight = rect.bottom - rect.top;

		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(&ps);
		CJpeg2 cJpeg;
		if( m_vecPath.size() == 1 )
		{
			RECT PicRect;
			if( lHeight - lWidth*3/4 < 0 )
			{
				lPicHeight = lHeight;
				lPicWidth = lHeight*4/3;
				PicRect.left = (lWidth - lPicWidth)/2;
				PicRect.right = lWidth - (lWidth - lPicWidth)/2;
				PicRect.top = 0;
				PicRect.bottom = lHeight;
			}
			else
			{
				lPicHeight = lWidth*3/4;
				lPicWidth = lWidth;
				PicRect.left = 0;
				PicRect.right = lWidth;
				PicRect.top = (lHeight - lPicHeight)/2;
				PicRect.bottom = lHeight - (lHeight - lPicHeight)/2;
			}

			cJpeg.load( m_vecPath[0] );
			cJpeg.draw( hPicWnd, PicRect );
		}
		else if( m_vecPath.size() >= 2 )
		{
			// 画面を半分にする
			lWidth = (lWidth/2)-2;
			RECT PicRect;
			long lPicNo = m_lSlider;
			if( m_vecPath.size() == m_lSlider + 1 )
				lPicNo = m_lSlider - 1;
			if( lHeight - lWidth*3/4 < 0 )
			{
				lPicHeight = lHeight;
				lPicWidth = lHeight*4/3;
				PicRect.left = ((lWidth - lPicWidth)/2);
				PicRect.right = (lWidth - (lWidth - lPicWidth)/2);
				PicRect.top = 0;
				PicRect.bottom = lHeight;

				cJpeg.load( m_vecPath[lPicNo] );
				cJpeg.draw( hPicWnd, PicRect );

				PicRect.left = (lWidth + (lWidth - lPicWidth)/2 )+2;
				PicRect.right = (2*lWidth - (lWidth - lPicWidth)/2)+2;
				PicRect.top = 0;
				PicRect.bottom = lHeight;
				cJpeg.load( m_vecPath[lPicNo+1] );
				cJpeg.draw( hPicWnd, PicRect );
			}
			else
			{
				lPicHeight = lWidth*3/4;
				lPicWidth = lWidth;
				PicRect.left = 0;
				PicRect.right = lWidth;
				PicRect.top = (lHeight - lPicHeight)/2;
				PicRect.bottom = lHeight - (lHeight - lPicHeight)/2;

				cJpeg.load( m_vecPath[lPicNo] );
				cJpeg.draw( hPicWnd, PicRect );

				PicRect.left = lWidth+2;
				PicRect.right = 2*lWidth+2;
				PicRect.top = (lHeight - lPicHeight)/2;
				PicRect.bottom = lHeight - (lHeight - lPicHeight)/2;
				cJpeg.load( m_vecPath[lPicNo+1] );
				cJpeg.draw( hPicWnd, PicRect );
			}
		}
		EndPaint(&ps);
		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( m_vecPath.size() == 0 )
			return 0;

		// 子ダイアログ内のクリック位置
		int nXPos = (int)LOWORD(lParam);
		int	nYPos = (int)HIWORD(lParam);

		CWindow cWnd( GetDlgItem( IDC_STATIC_PIC ) );
		CWindow cParentWnd( m_hChildWnd );

		RECT PicRect, PicRectParent;
		cWnd.GetWindowRect( &PicRect );
		cParentWnd.GetWindowRect( &PicRectParent );

		// 子ダイアログのウィンドウ位置からクリックしポイントを求める
		// つまりこのX、YがIDC_STATIC_PICのウィンドウ範囲に入っていればよい
		int nStaticPicX = PicRectParent.left + nXPos;
		int nStaticPixY = PicRectParent.top + nYPos;

		if( nStaticPicX > PicRect.left && nStaticPicX < PicRect.right && nStaticPixY > PicRect.top && nStaticPixY < PicRect.bottom )
		{
			// 領域の右と左どっち側をクリックしたか？
			CString strPath;
			if( m_vecPath.size() > 1 )
			{
				long lPicNo = m_lSlider;
				if( m_vecPath.size() == m_lSlider + 1 )
					lPicNo = m_lSlider - 1;

				// 左画像をクリック
				if( nStaticPicX < ( PicRect.left + PicRect.right )/2 )
					strPath = m_vecPath[lPicNo];
				// 右画像をクリック
				else
					strPath = m_vecPath[lPicNo+1];
			}
			else
				strPath = m_vecPath[0];

			//[Bug 5998]外部ツールで参照するときにウインドウが表示されない場合があるに対応
			// SW_SHOWをつけないと表示されないツールも多い
			ShellExecute( NULL, _T("open"), m_strToolPath, strPath, NULL, SW_SHOW );
		}
		return 0;
	}

	void OnHScroll(int nCode, short nPos, HWND hWnd)
	{
        CTrackBarCtrl trackbar = hWnd;
		m_lSlider = trackbar.GetPos();
		InvalidateRect( NULL, FALSE );
    }

	LRESULT OnClickedButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CString strParameters;
		if( m_vecPath.size() > 0 )
		{
			for( int i = 0; i < m_vecPath.size(); i++ )
				strParameters += m_vecPath[i] + _T(" ");
		}

		//[Bug 5998]外部ツールで参照するときにウインドウが表示されない場合があるに対応
		// SW_SHOWをつけないと表示されないツールも多い
		if( strParameters.IsEmpty() == false )
			ShellExecute( NULL, _T("open"), m_strToolPath, strParameters, NULL, SW_SHOW );

		return 0;
	}


	LRESULT OnClkConfirmBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// コンボボックスの表示をインクリメントする( bug 6201 6202 )
		// index[0] 1:（未確認）
		// index[1] 2:（1校確認）
		// index[2] 3:（2校確認）
		// のため、GetItemした値はインデックス的には既にインクリメントされている
		CComboBox combo = ( GetDlgItem(IDC_COMBO_CONFIRM) );
		combo.SetCurSel( combo.GetItemData( combo.GetCurSel() ) %3 );

		// メッセージをでっち上げて変更を通知
		OnCommand(0, MAKEWPARAM(IDC_COMBO_CONFIRM, CBN_SELCHANGE), 0, bHandled);
		return 0;
	}

	LRESULT OnComboFoucus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// [bug 6201]コンボボックスを操作させないため、フォーカスがあたったら強引にボタンに当て直す
		::SetFocus( GetDlgItem( IDC_BUTTON_CONFIRM ) );
		return 0;
	}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strFeatureClassName = lpcszFeatureClassName;};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void GetRegCode();
	void FindPicture();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6,m_wnd7,m_wnd8,
						m_wnd9,m_wnd10,m_wnd11,m_wnd12,m_wnd13,m_wnd14,m_wnd15,m_wnd16,m_wnd17;	//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	CString				m_strBaseDir, m_strCode2Path, m_strToolPath;
	HFONT				m_hFont;
	long				m_lSlider;
	std::vector<CString> m_vecPath;
	CString				m_strFeatureClassName;

};

#endif //__ATTRCITYLINEDLG_H_
