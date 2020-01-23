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

#ifndef _LIB
#if _MSC_VER == 1700
#ifdef _AFXDLL
	#ifdef _DEBUG
		#pragma comment(lib, "winmgr-vc11-ugd-afxdll.lib")
	#else
		#pragma comment(lib, "winmgr-vc11-u-afxdll.lib")
	#endif // ifdef _DEBUG
#else
	#ifdef _DEBUG
		#pragma comment(lib, "winmgr-vc11-ugd.lib")
#else
		#pragma comment(lib, "winmgr-vc11-u.lib")
	#endif // ifdef _DEBUG
#endif // ifdef _AFXDLL
#elif _MSC_VER == 1500
#ifdef _AFXDLL
#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "winmgr-vc90-ugd-afxdll.lib")
#else
#pragma comment(lib, "winmgr-vc90-gd-afxdll.lib")
#endif // ifdef _UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "winmgr-vc90-u-afxdll.lib")
#else
#pragma comment(lib, "winmgr-vc90-afxdll.lib")
#endif // ifdef _UNICODE
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "winmgr-vc90-ugd.lib")
#else
#pragma comment(lib, "winmgr-vc90-gd.lib")
#endif // ifdef _UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "winmgr-vc90-u.lib")
#else
#pragma comment(lib, "winmgr-vc90.lib")
#endif // ifdef _UNICODE
#endif // ifdef _DEBUG
#endif // ifdef _AFXDLL
#else
#ifdef _AFXDLL
#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "winmgrud_afxdll.lib")
#else
#pragma comment(lib, "winmgrd_afxdll.lib")
#endif // ifdef _UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "winmgru_afxdll.lib")
#else
#pragma comment(lib, "winmgr_afxdll.lib")
#endif // ifdef _UNICODE
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment(lib, "winmgrud.lib")
#else
#pragma comment(lib, "winmgrd.lib")
#endif // ifdef _UNICODE
#else
#ifdef _UNICODE
#pragma comment(lib, "winmgru.lib")
#else
#pragma comment(lib, "winmgr.lib")
#endif // ifdef _UNICODE
#endif // ifdef _DEBUG
#endif // ifdef _AFXDLL
#endif // if _MSC_VER == 1500
#endif // ifndef _LIB
////////////////////////////////////////////////////////////////
// WinMgr ライブラリ
// 
// このライブラリは、MSDNマガジン（2001/7月号）に掲載された
// ウインドウ配置管理クラスライブラリを元に、MFC、ATL（+WTL）
// 両方で使用できるように手直しをしてあります。
// また、動作中に配置マップを切り替えることができるように
// ChangeMgrMap()という関数を追加してあります。
// そのほか、グループ定義時にパーセント指定できるようにも
// 拡張してあります。
// LIB/WinLib、SiNDY-e/COMMON以下に同じファイルがありますが、
// 今後のメンテナンスはLIB/SizingFramework以下のみ行います。
// 
// 使い方は、testフォルダ以下を参考にしてください。
//                                         written by furukawa.
// 参考URL:
//
// July 2001 issue of MSDN Magazine.
// http://msdn.microsoft.com/msdnmag/issues/01/07/default.asp
// 
// ATL porting.
// http://www.codeguru.com/cpp/com-tech/atl/misc/article.php/c3563/
// 
// 参考書籍：
// MSDN マガジン 2001/7月号
////////////////////////////////////////////////////////////////
// MSDN Magazine -- July 2001
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0. Runs on Win 98 and probably Win 2000 too.
// Set tabsize = 3 in your editor.
//
// WinMgr.h -- Main header file for WinMgr library.
//
#pragma once

#ifndef ASSERT
#define ASSERT ATLASSERT
#endif // ifndef ASSERT

// when using screen dimensions, this is infinite
const LONG INFINITY=0x7fff; // max short

// useful size constants
#define SIZEZERO		CSize(0,0)
#define SIZEMAX		CSize(INFINITY,INFINITY)

// handy functions to take the min or max of a CSize
inline CSize minsize(CSize a, CSize b) {
	return CSize(min((UINT)a.cx,(UINT)b.cx),min((UINT)a.cy,(UINT)b.cy));
}

inline CSize maxsize(CSize a, CSize b) {
	return CSize(max((UINT)a.cx,(UINT)b.cx),max((UINT)a.cy,(UINT)b.cy));
}

//////////////////
// Size info about a rectangle/row/column
//
struct SIZEINFO {
	SIZE szAvail;		// total size avail (passed)
	SIZE szDesired;	// desired size: default=current
	SIZE szMin;			// minimum size: default=SIZEZERO
	SIZE szMax;			// maximum size: default=MAXSIZE
};

// types of rectangles:
#define	WRCT_END			0				// end of table
#define	WRCT_FIXED		0x0001		// height/width is fixed
#define	WRCT_PCT			0x0002		// height/width is percent of total
#define	WRCT_REST		0x0003		// height/width is whatever remains
#define	WRCT_TOFIT		0x0004		// height/width to fit contents
#define	WRCF_TYPEMASK	0x000F

// group flags
#define	WRCF_ROWGROUP	0x0010		// beginning of row group
#define	WRCF_COLGROUP	0x0020		// beginning of column group
#define	WRCF_ENDGROUP	0x00F0		// end of group
#define	WRCF_GROUPMASK	0x00F0

//////////////////
// This structure is used to hold a rectangle and describe its layout. Each
// WINRECT corresponds to a child rectangle/window. Each window that uses
// WinMgr provides a table (C array) of these to describe its layout.
//
class WINRECT {
protected:
	// pointers initialized by the window manager for easy traversing:
	WINRECT* next;			// next at this level
	WINRECT* prev;			// prev at this level

	// data
	CRect rc;				// current rectangle position/size
	WORD  flags;			// flags (see above)
	UINT	nID;				// window ID if this WINRECT represents a window
	LONG	param;			// arg depends on type

public:
	WINRECT(WORD f, UINT id, LONG p);

	static WINRECT* InitMap(WINRECT* map, WINRECT* parent=NULL);

	WINRECT* Prev()			{ return prev; }
	WINRECT* Next()			{ return next; }
	WINRECT* Children()		{ return IsGroup() ? this+1 : NULL; }
	WINRECT* Parent();
	WORD GetFlags()			{ return flags; }
	WORD SetFlags(WORD f)	{ return flags=f; }
	LONG GetParam()			{ return param; }
	LONG SetParam(LONG p)	{ return param=p; }
	UINT GetID()				{ return nID; }
	UINT SetID(UINT id)		{ return nID=id; }
	CRect& GetRect()					{ return rc; }
	void SetRect(const CRect& r)	{ rc = r; }
	WORD Type() const			{ return flags & WRCF_TYPEMASK; }
	WORD GroupType() const	{ return flags & WRCF_GROUPMASK; }
	BOOL IsGroup() const		{ return GroupType() && GroupType()!=WRCF_ENDGROUP; }
	BOOL IsEndGroup() const { return flags==0 || flags==WRCF_ENDGROUP; }
	BOOL IsEnd() const		{ return flags==0; }
	BOOL IsWindow() const	{ return nID>0; }
	BOOL IsRowGroup()	const { return (flags & WRCF_GROUPMASK)==WRCF_ROWGROUP; }
	void SetHeight(LONG h)	{ rc.bottom = rc.top + h; }
	void SetWidth(LONG w)	{ rc.right = rc.left + w; }
	LONG GetHeightOrWidth(BOOL bHeight) const {
		return bHeight ? rc.Height() : rc.Width();
	}
	void SetHeightOrWidth(LONG horw, BOOL bHeight) {
		bHeight ? SetHeight(horw) : SetWidth(horw);
	}
	BOOL GetMargins(int& w, int& h);

	// For TOFIT types, param is the TOFIT size, if nonzero. Used in dialogs,
	// with CWinMgr::InitToFitSizeFromCurrent.
	BOOL HasToFitSize()			{ return param != 0; }
	SIZE GetToFitSize()			{ return CSize(LOWORD(param),HIWORD(param)); }
	void SetToFitSize(SIZE sz)	{ param = MAKELONG(sz.cx,sz.cy); }
};

//////////////////
// Below are all the macros to build your window map. 
//

// Begin/end window map. 'name' can be anything you want
#define BEGIN_WINDOW_MAP(name)	WINRECT name[] = {
#define END_WINDOW_MAP()			WINRECT(WRCT_END,-1,0) }; 

// Begin/end a group.
// The first entry in your map must be BEGINROWS or BEGINCOLS.
#define BEGINROWS(type,id,m)	WINRECT(WRCF_ROWGROUP|type,id,m),
#define BEGINCOLS(type,id,m)  WINRECT(WRCF_COLGROUP|type,id,m),
#define ENDGROUP()				WINRECT(WRCF_ENDGROUP,-1,0),

// This macros is used only with BEGINGROWS or BEGINCOLS to specify margins
#define RCMARGINS(w,h)			MAKELONG(w,h)

// Macros for primitive (non-group) entries.
// val applies to height for a row entry; width for a column entry.
#define RCFIXED(id,val)		WINRECT(WRCT_FIXED,id,val),
#define RCPERCENT(id,val)	WINRECT(WRCT_PCT,id,val),
#define RCREST(id)			WINRECT(WRCT_REST,id,0),
#define RCTOFIT(id)			WINRECT(WRCT_TOFIT,id,0),
#define RCSPACE(val)			RCFIXED(-1,val)

//////////////////
// Use this to iterate the entries in a group.
//
//	CWinGroupIterator it;
//	for (it=pGroup; it; it.Next()) {
//   WINRECT* wrc = it;
//   ..
// }
//
class CWinGroupIterator {
protected:
	WINRECT* pCur;	  // current entry
public:
	CWinGroupIterator() { pCur = NULL; }
	CWinGroupIterator& operator=(WINRECT* pg) {
		ASSERT(pg->IsGroup()); // can only iterate a group!
		pCur = pg->Children();
		return *this;
	}
	operator WINRECT*()	{ return pCur; }
	WINRECT* pWINRECT()	{ return pCur; }
	WINRECT* Next()		{ return pCur = pCur ? pCur->Next() : NULL;}
};

// Registered WinMgr message
extern const UINT WM_WINMGR;

// Notification struct, passed as LPARAM
struct NMWINMGR : public NMHDR {
	enum {								// notification codes:
		GET_SIZEINFO = 1,				// WinMgr is requesting size info
		SIZEBAR_MOVED,					// user moved sizer bar
	};

	// each notification code has its own part of union
	union {
		SIZEINFO sizeinfo;	// used for GET_SIZEINFO
		struct {					// used for SIZEBAR_MOVED
			POINT ptMoved;		//  distance moved (x or y = zero)
		} sizebar;
	};

	// ctor: initialize to zeroes
	NMWINMGR() { memset(this,0,sizeof(NMWINMGR)); }
};

///////////////////
// Window manager. This class calculates all the sizes and positions of the
// rectangles in the window map.
//
#ifdef _AFXDLL
class CWinMgr : public CObject {
#else
class CWinMgr {
#endif // ifdef _AFXDLL
public:
	CWinMgr(WINRECT* map);
	virtual ~CWinMgr();

#ifdef _AFXDLL
	virtual void GetWindowPositions(CWnd* pWnd); // load map from window posns
	virtual void SetWindowPositions(CWnd* pWnd); // set window posns from map
#else
	virtual void GetWindowPositions(CWindow* pWnd); // load map from window posns
	virtual void SetWindowPositions(CWindow* pWnd); // set window posns from map
#endif // ifdef _AFXDLL

	// get min/max/desired size of a rectangle
#ifdef _AFXDLL
	virtual void OnGetSizeInfo(SIZEINFO& szi, WINRECT* pwrc, CWnd* pWnd=NULL);
#else
	virtual void OnGetSizeInfo(SIZEINFO& szi, WINRECT* pwrc, CWindow* pWnd=NULL);
#endif // ifdef _AFXDLL

	/**
	 * @brief Window定義マップの変更を行う
	 * @note この関数を呼んだ後は再配置を行う必要があります
	 * <H3>例：ChangeMgrMapを呼んだ後に再配置する</H3>
	 * @code 
	 * m_winMgr.ChangeMgrMap( DEFAULT );
	 * 
	 * // 再配置
	 * m_winMgr.InitToFitSizeFromCurrent(this);
	 * m_winMgr.CalcLayout(this);
	 * m_winMgr.SetWindowPositions(this);	
	 * @endcode
	 **/
	bool ChangeMgrMap( WINRECT* map);

	// calc layout using client area as total area
#ifdef _AFXDLL
	void CalcLayout(CWnd* pWnd) {
#else
	void CalcLayout(CWindow* pWnd) {
#endif // ifdef _AFXDLL
		ASSERT(pWnd);
		CRect rcClient;
		pWnd->GetClientRect(&rcClient);
		CalcLayout(rcClient, pWnd);
	}

	// calc layout using cx, cy (for OnSize)
#ifdef _AFXDLL
	void CalcLayout(int cx, int cy, CWnd* pWnd=NULL) {
#else
	void CalcLayout(int cx, int cy, CWindow* pWnd=NULL) {
#endif // ifdef _AFXDLL
		CalcLayout(CRect(0,0,cx,cy), pWnd);
	}

	// calc layout using given rect as total area
#ifdef _AFXDLL
	void CalcLayout(CRect rcTotal, CWnd* pWnd=NULL) {
#else
	void CalcLayout(CRect rcTotal, CWindow* pWnd=NULL) {
#endif // ifdef _AFXDLL
		ASSERT(m_map);
		m_map->SetRect(rcTotal);
		CalcGroup(m_map, pWnd);
	}

	// Move rectangle vertically or horizontally. Used with sizer bars.
#ifdef _AFXDLL
	void MoveRect(int nID, CPoint ptMove, CWnd* pParentWnd) {
#else
	void MoveRect(int nID, CPoint ptMove, CWindow* pParentWnd) {
#endif // ifdef _AFXDLL
		MoveRect(FindRect(nID), ptMove, pParentWnd);
	}
#ifdef _AFXDLL
	void MoveRect(WINRECT* pwrcMove, CPoint ptMove, CWnd* pParentWnd);
#else
	void MoveRect(WINRECT* pwrcMove, CPoint ptMove, CWindow* pParentWnd);
#endif // ifdef _AFXDLL

	CRect GetRect(UINT nID)						 { return FindRect(nID)->GetRect(); }
	void SetRect(UINT nID, const CRect& rc) { FindRect(nID)->SetRect(rc); }

	// get WINRECT corresponding to ID
	WINRECT* FindRect(UINT nID);

	// Calculate MINMAXINFO
#ifdef _AFXDLL
	void GetMinMaxInfo(CWnd* pWnd, MINMAXINFO* lpMMI);
	void GetMinMaxInfo(CWnd* pWnd, SIZEINFO& szi);
#else
	void GetMinMaxInfo(CWindow* pWnd, MINMAXINFO* lpMMI);
	void GetMinMaxInfo(CWindow* pWnd, SIZEINFO& szi);
#endif // ifdef _AFXDLL

	// set TOFIT size for all windows from current window sizes
#ifdef _AFXDLL
	void InitToFitSizeFromCurrent(CWnd* pWnd);
#else
	void InitToFitSizeFromCurrent(CWindow* pWnd);
#endif // ifdef _AFXDLL

	void TRACEDump() const { TRACEDump(m_map); }
	void TRACEDump(WINRECT* map) const;

protected:
	WINRECT*	m_map;			// THE window map

	int  CountWindows();
#ifdef _AFXDLL
	BOOL SendGetSizeInfo(SIZEINFO& szi, CWnd* pWnd, UINT nID);
#else
	BOOL SendGetSizeInfo(SIZEINFO& szi, CWindow* pWnd, UINT nID);
#endif // ifdef _AFXDLL

	// you can override to do wierd stuff or fix bugs
#ifdef _AFXDLL
	virtual void CalcGroup(WINRECT* group, CWnd* pWnd);
#else
	virtual void CalcGroup(WINRECT* group, CWindow* pWnd);
#endif // ifdef _AFXDLL
	virtual void AdjustSize(WINRECT* pEntry, BOOL bRow,
#ifdef _AFXDLL
		int& hwRemaining, CWnd* pWnd);
#else
		int& hwRemaining, CWindow* pWnd);
#endif // ifdef _AFXDLL
	virtual void PositionRects(WINRECT* pGroup,
		const CRect& rcTotal,BOOL bRow);

private:
	CWinMgr() { ASSERT(FALSE); } // no default constructor
};

//////////////////
// Sizer bar (splitter) lets users adjust the size of two siblings.
// See DopeyEdit for how to use.
//
#ifdef _AFXDLL
class CSizerBar : public CStatic {
#else
class CSizerBar : public CWindowImpl<CSizerBar> {
	typedef CWindowImpl<CSizerBar> baseClass;
#endif // ifdef _AFXDLL
public:
#ifndef _AFXDLL
	DECLARE_WND_SUPERCLASS( _T("SIZERBAR"), _T("STATIC") )
#endif // ifndef _AFXDLL

	CSizerBar();
	virtual ~CSizerBar();
	BOOL Create(DWORD dwStyle,				// window styles
		HWND hParentWnd,					// parent window
		CWinMgr& wmgr,						// window manger
		UINT nID,							// ID of sizer bar
		const RECT& rc = CRect(0,0,0,0));	// initial rectangle

protected:
	CWinMgr* m_pWinMgr;			// window manager
	BOOL		m_bHorz;				// horizontal bar; else vertical
	BOOL		m_bDragging;		// in drag mode?
	CRect		m_rcBar;				// bar rect in screen coords
	CPoint	m_ptOriginal;		// original mouse pos at start of drag
	CPoint	m_ptPrevious;		// previous mouse pos while dragging
	CRect		m_rcConstrain;		// constrain mouse to this rect
	HWND		m_hwndPrevFocus;	// to restore after dragging

	// override this if you insist on painting differently
	virtual void OnDrawBar(CDC& dc, CRect& rc);

	// helpers
	void CancelDrag();					 // cancel drag mode
	void DrawBar();						 // draw sizer bar
	BOOL IsHorizontal();					 // is bar horizontal?
	void NotifyMoved(CPoint ptDelta); // send move notification to parent
	CPoint Constrain(CPoint pt);		 // constrain pt to m_rcConstrain
	CPoint Rectify(CPoint pt) {		 // make point vertical/horizontal
		return IsHorizontal() ? CPoint(0, pt.y) : CPoint(pt.x, 0);
	}

#ifdef _AFXDLL
	// MFC overrides & message handlers
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCancelMode(); // may not be necessary

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CSizerBar)
#else
BEGIN_MSG_MAP_EX(CSizerBar)
	MSG_WM_SETCURSOR(OnSetCursor)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
	MSG_WM_LBUTTONUP(OnLButtonUp)
	MSG_WM_MOUSEMOVE(OnMouseMove)
	MSG_WM_CHAR(OnChar)
END_MSG_MAP()

	BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT msg);
	void OnLButtonDown(UINT nFlags, CPoint pt);
	void OnLButtonUp(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint pt);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnCancelMode(); // may not be necessary
#endif // ifdef _AFXDLL
};

//////////////////
// Generic sizeable dialog. To create a sizeable dialog, derive from this and
// give your window map to constructor. Otherwise (if you don't derive from
// CSizeableDlg), mimic OnInitDialog, OnSize, OnGetMinMaxInfo.
//
#ifdef _AFXDLL
class CSizeableDlg : public CDialog {
#else
template <class T>
class CSizeableDlg : public T {
//DECLARE_WND_SUPERCLASS( _T("SIZEABLEDLG"), _T("
#endif // ifdef _AFXDLL
public:
#ifdef _AFXDLL
	CSizeableDlg(UINT nID, CWnd* pParent, WINRECT* pWinMap);
	virtual ~CSizeableDlg();
#else
	CSizeableDlg(WINRECT* pWinMap) : m_winMgr(pWinMap)
	{
	}
	virtual ~CSizeableDlg()
	{
	}
#endif // ifdef _AFXDLL

protected:
	CWinMgr m_winMgr;	  // window manager

#ifdef _AFXDLL
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnWinMgr(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSizeableDlg)
#else
BEGIN_MSG_MAP_EX(CSizeableDialog)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
END_MSG_MAP()

	BOOL OnInitDialog(HWND hwnd, LPARAM lParam )
	{
		SetMsgHandled(FALSE);

		m_winMgr.InitToFitSizeFromCurrent(this);	// make tofit = current size
		m_winMgr.CalcLayout(this);				// recalc
		m_winMgr.SetWindowPositions(this);		// set positions

		return TRUE;
	}
	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);

		if( IsWindow() )
		{
			m_winMgr.CalcLayout(size.cx,size.cy,this);
			m_winMgr.SetWindowPositions(this);
		}
	}
	void OnGetMinMaxInfo(MINMAXINFO* lpMMI)
	{
		SetMsgHandled(FALSE);

		m_winMgr.GetMinMaxInfo(this, lpMMI);
	}
	LRESULT OnWinMgr(UINT uMsg, WPARAM wp, LPARAM lp, BOOL& bHandled)
	{
		SetMsgHandled(FALSE);

		ATLASSERT(lp);
		NMWINMGR& nmw = *(NMWINMGR*)lp;
		if (nmw.code==NMWINMGR::SIZEBAR_MOVED) {
			// User moved a sizer bar: call WinMgr to do it!
			m_winMgr.MoveRect(static_cast<int>(wp), nmw.sizebar.ptMoved, this);
			m_winMgr.SetWindowPositions(this);
			bHandled = TRUE;
		}
		bHandled = FALSE; // not handled

		return 0;
	}
#endif // ifdef _AFXDLL
};
