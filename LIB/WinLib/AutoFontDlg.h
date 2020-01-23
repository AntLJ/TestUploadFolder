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

#pragma once

#include <boost/shared_ptr.hpp>

namespace autodlg
{
	struct Observer
	{
		// 純粋仮想関数
		virtual void LoadColSort(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveColSort(IVSFlexGridPtr ipFlexGrid, long lCol, SortSettings sortype = flexSortNone, long mode=0) = 0;
		virtual void LoadGridColWidth(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long width, long mode=0) = 0;
		virtual bool HasGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0) = 0;
		virtual void ResetGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0) = 0;
		virtual void LoadGridRowHeight(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long height, long mode=0) = 0;
		virtual bool HasGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0) = 0;
		virtual void ResetGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0) = 0;
		virtual bool IsSaveGridSize(IVSFlexGridPtr ipFlexGrid, long nColRowMode, long id, long mode=0) = 0;
		virtual void LoadGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void ClearGridSelect(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0) = 0;
		virtual void LoadGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void ClearGridPos(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0) = 0;
	};

	enum FontSetMode
	{
		font_small = 0, //!< フォントサイズ縮小
		font_big   = 1, //!< フォントサイズ拡大
		font_init  = 2  //!< フォントサイズ初回設定
	};
};

typedef std::list< HFONT > HFONTS; //!< フォントのリスト
#define AUTOFONTDLG_INIT_FONTSIZE 9 //!< コントロールのデフォルトのフォントサイズ
#define AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE 8.25 //!< FlexGrid用のデフォルトのフォントサイズ

class CControlsImpl : public std::list<HWND>
{
public:
	CControlsImpl() : m_idd(0), m_size(0.0f), m_size_min(8.0f), m_size_max(72.0f){};
	CControlsImpl(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, double size) : m_strKey(lpcszKey), m_idd(idd), m_strCtlName(lpcszName), m_size(size), m_size_min(8.0f), m_size_max(72.0f) {};
	virtual ~CControlsImpl(){};

	LPCTSTR GetControlName() const { return m_strCtlName; };
	double GetSize() const { return m_size; };

	virtual void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS >& mapExFonts) = 0;

	void GetRegData()
	{
		// レジストリ読み込み
		CString strReg;

		strReg.Format( _T("%s\\AutoFont\\%d\\%s"), m_strKey, m_idd, m_strCtlName );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) ) {
			m_size = GetRegSize( reg, _T("FontSize") );
			m_size_min = GetRegSize( reg, _T("FontMinSize") );
			m_size_max = GetRegSize( reg, _T("FontMaxSize"), 72.0f );
		}
	}

	void SetRegData()
	{
		if ( ! m_strKey.IsEmpty() ) {
			// レジストリに保存
			CString strReg;

			strReg.Format( _T("%s\\AutoFont\\%d\\%s"), m_strKey, m_idd, m_strCtlName );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
				SetRegSize( reg, _T("FontSize"), m_size );
		}
	}

	bool ChgSize(autodlg::FontSetMode nMode)
	{
		double size = m_size;

		switch( nMode ) 
		{
		case autodlg::FontSetMode::font_small: size -= 1.0f; break;
		case autodlg::FontSetMode::font_big  : size += 1.0f; break;
		default:break;
		}

		if ( size >= m_size_min && size <= m_size_max ) {
			m_size = size;
			return true;
		}

		return false;
	}

protected:
	double GetRegSize(CRegKey& reg, LPCTSTR lpcszName, double defsize=8.0f)
	{
		CString strSize;
		ULONG BufSize = 0;
		double size = defsize;

		reg.QueryStringValue( lpcszName, NULL, &BufSize );
		LPTSTR lpszSize = strSize.GetBuffer( BufSize + 1 );
		if( ERROR_SUCCESS == reg.QueryStringValue( lpcszName, lpszSize, &BufSize ) )
			size = _tcstod( lpszSize, NULL );
		strSize.ReleaseBuffer();

		return size;
	}

	void SetRegSize(CRegKey& reg, LPCTSTR lpcszName, double size)
	{
		CString strSize;

		strSize.Format( _T("%.2lf"), size );
		reg.SetStringValue( lpcszName, strSize );
	}

protected:
	CString m_strKey;
	long m_idd;	// ダイアログID
	double m_size;
	double m_size_min;
	double m_size_max;
	CString m_strCtlName;
};

class CNormalCtls : public CControlsImpl
{
public:
	CNormalCtls() {};
	CNormalCtls(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, long size) : CControlsImpl(lpcszKey, idd, lpcszName, size){};
	~CNormalCtls()
	{
		for ( std::map<HWND, HFONTS>::iterator it = m_mapDefaultFonts.begin(); it != m_mapDefaultFonts.end(); ++it )
		{
			for( HFONTS::iterator font = it->second.begin(); font != it->second.end(); ++font )
				::DeleteObject( *font );
		}

		m_mapDefaultFonts.clear();
	};

	/**
	 * @brief  コントロールに変更したフォントを設定する関数
	 * @note   この関数内では、フォントの削除は行いません
	 *         メンバで作成されたフォントを確保しておき、ダイアログが閉じられるときに
	 *         デストラクタで一括削除されます。
	 *         いらなくなったフォントを削除すると、いろいろとおかしくなる
	 *         設定後にもう一度フォント取得するとオブジェクトタイプがフォントじゃないといわれる。
	 *         フォントの削除はそのコントロール内のフォントすべて削除されるようなイメージ
	 *         フォント作成したら、最後に削除するのがお作法のようだ。
	 *         http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+200701/07010038.txt
	 *         TODO:この方法だと、結構な量のフォントオブジェクトを抱え込むことになるので、
	 *              フォントを作成したら削除するという方法で対応したほうがよい
	 *
	 * @param  nMode	  [in]  フォントサイズの設定方法
	 * @param  mapExFonts [in]  コントロールをキーとしたフォントのリスト
	 *
	 */
	void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS>& mapExFonts)
	{
		if ( ! empty() ) {
			GetRegData();

			if ( ChgSize(nMode) ) 
			{
				// コントロールのフォント変更
				for ( std::list<HWND>::const_iterator it = begin(); it != end(); ++it )
				{
					// 現在のフォント取得（絶対にある）
					HFONT hCurrentFont = (HFONT)::SendMessage( (*it), WM_GETFONT, 0, 0 );
					// ないなら、どこかで削除されてしまっているのでおかしい
					ATLASSERT( ::GetObjectType( hCurrentFont ) == OBJ_FONT);

					// CLogFontを作成
					HDC hDC = ::GetDC((*it));
					CLogFont cChangedLogFont( hCurrentFont );

					HFONT hFont;
					// 特殊設定フォントを持っているコントロールなら、そちらの設定のフォントを変更する
					std::map<HWND, HFONTS>::iterator itExFonts = mapExFonts.find( (*it) );
					if ( itExFonts != mapExFonts.end() ) 
					{
						// 特殊設定フォントの場合にはm_sizeを使えないので、今設定されているフォント
						// からの拡大・縮小を行う
						long size = cChangedLogFont.GetHeight( hDC );
						// モードによりフォントサイズ変更
						switch( nMode ) 
						{
						case autodlg::FontSetMode::font_small: size -= 1; break;
						case autodlg::FontSetMode::font_big  : size += 1; break;
						default: size = (long)(m_size + ((double)size - m_size));break;
						}
						// サイズ再設定
						cChangedLogFont.SetHeight( size, hDC );

						// すでにもっているフォントかどうか確認
						if( !HasFont( itExFonts->second, cChangedLogFont, hFont) )
						{
							// ないならフォント作成して、確保しておく
							HFONT hNewFont = cChangedLogFont.CreateFontIndirect();
							itExFonts->second.push_back( hNewFont );
							hFont = hNewFont;
						}
					} 
					else
					{
						cChangedLogFont.SetHeight( (long)m_size, hDC );

						HFONTS fonts = m_mapDefaultFonts[ *it ];
						// すでにもっているかどうか確認
						if( !HasFont( fonts, cChangedLogFont, hFont ) )
						{
							// ないならフォント作成して、確保しておく
							HFONT hNewFont = cChangedLogFont.CreateFontIndirect();
							m_mapDefaultFonts[(*it)].push_back( hNewFont );
							hFont = hNewFont;
						}
					}
					// 用済みデバイスコンテキスト削除
					::ReleaseDC( (*it), hDC );

					// コントロールへフォント再設定
					if ( hFont )
						::SendMessage( (*it), WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0) );
				}

				SetRegData();
			}
		}
	}

	/**
	 * @brief  すでに変更しようとしているフォントを持っているかどうかを調べる関数
	 *
	 * @param  hFonts	     [in]   チェックしたいフォントのリスト
	 * @param  cCheckLogFont [in]   変更しようとしているフォント
	 * @param  hRetFont	     [out]  メンバで抱えていたフォント
	 * @return bool    true  第1引数のフォントのリスト内に第2引数のフォントがあった
	 * @return bool    false 第1引数のフォントのリスト内に第2引数のフォントがなかった
	 *
	 */
	bool HasFont( const HFONTS& hFonts, const CLogFont& cCheckLogFont, HFONT& hRetFont )
	{
		bool bHasFont = false;
		for( HFONTS::const_iterator font = hFonts.begin(); font != hFonts.end(); ++font )
		{
			CLogFont cCheckFont( *font );
			if( cCheckFont == cCheckLogFont )
			{
				hRetFont = *font;
				bHasFont = true;
			}
		}
		return bHasFont;
	}


protected:
	std::map<HWND, HFONTS> m_mapDefaultFonts;	//!< 標準フォント map<コントロール名,フォント>
};

class CGridCtls : public CControlsImpl
{
public:
	CGridCtls() : m_pObserver(NULL) {};
	CGridCtls(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, double size, autodlg::Observer* pObserver=NULL) : CControlsImpl(lpcszKey, idd, lpcszName, size), m_pObserver(pObserver) {};
	~CGridCtls(){};

	void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS >& mapExFonts)
	{
		if ( ! empty() ) {
			GetRegData();

			if ( ChgSize(nMode) ) {
				for ( std::list<HWND>::const_iterator it = begin(); it != end(); ++it )
				{
					IVSFlexGridPtr ipGrid = GetFlexGrid( (*it) );
					if ( ipGrid ) {
						// フォントサイズ
						ipGrid->put_FontSize( (float)m_size );

						ipGrid->put_Redraw( flexRDNone );

						long lCols = -1;

						ipGrid->get_Cols( &lCols );
						if ( lCols > 0 )
						{
							ipGrid->put_AutoSizeMode( flexAutoSizeColWidth );
							ipGrid->AutoSize( 0, lCols -1 );
							ipGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
							ipGrid->AutoSize( 0, lCols -1 );
						}

						if ( m_pObserver )
						{
							m_pObserver->LoadGridColWidth( ipGrid );	// [Bug 7768][i][要望]グリッドの列幅保持機能
							m_pObserver->LoadGridRowHeight( ipGrid );	// [Bug 7768][i][要望]グリッドの列幅保持機能
						}

						ipGrid->put_Redraw( flexRDDirect );
					}
				}

				SetRegData();
			}
		}
	}

public:
	IVSFlexGridPtr GetFlexGrid(HWND hWnd)
	{
		IVSFlexGridPtr ipGrid;
		CComPtr<IUnknown> spUnk;

		HRESULT hr = AtlAxGetControl( hWnd, &spUnk );
		if ( SUCCEEDED(hr) && spUnk )
			hr = spUnk->QueryInterface( __uuidof(IVSFlexGrid), (void**)&ipGrid );

		return ipGrid;
	}

private:
	autodlg::Observer* m_pObserver;
};

template<class TDlg>
class CAutoFontDlgImpl : public CAxDialogImpl<TDlg>, public autodlg::Observer
{
public:
	CAutoFontDlgImpl() {};
	virtual ~CAutoFontDlgImpl(){ Clear(); };

BEGIN_MSG_MAP(CAutoFontDlgImpl)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnWheel)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	CHAIN_MSG_MAP(CAxDialogImpl<TDlg>)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Init();

		bHandled = FALSE;
		return 0;  // システムでフォーカスを設定します。
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 位置・サイズをレジストリに保持（bug 6567）
		OnPosition( uMsg, wParam, lParam, bHandled );
		Clear();

		bHandled = FALSE;
		return 0;
	}
	LRESULT OnWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Auto(wParam);

		bHandled = FALSE;
		return 0;
	}

	// [Bug 6567][i][要望]登録ダイアログの前回表示位置と大きさを保持できるようにしてほしい。
	LRESULT OnPosition(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			long width = LOWORD(lParam);
			long height = HIWORD(lParam);

			CRect rc;
			::GetWindowRect( m_hWnd, &rc );

			SavePosition( rc.left, rc.top, rc.Width(), rc.Height() );
		}

		bHandled = FALSE;
		return 0;
	}

	/**
	 * @brief 他のコントロールとは異なるフォントサイズを設定する関数
	 *
	 * @note 継承先のダイアログのOnInitDialogで特別にフォントサイズを変えたいコントロールを指定します。
	 *       紛らわしいので、CreateExFontとかに名前が違うほうがいいかも
	 *       さらに、ここで設定するフォントサイズはデフォルトフォントサイズからの差であることに注意!
	 *       例えば、デフォルトでフォントサイズを8と設定しているが、特別に12にしたい場合に
	 *       継承先のダイアログでは、CreateFont( hWnd, 12, ...)とフォントサイズを設定する
	 *       だが、このクラスによってダイアログ全体のフォントサイズが15と設定された場合に
	 *       次回ダイアログ起動時（レジストリに登録されて、それを読み取ることになっている）
	 *       他よりも小さいフォントサイズになってしまう（特別設定したコントロールは12で、
	 *       ほかのコントロールは15となる）。そのため、この関数で与えるフォントサイズは
	 *       デフォルトからいくつ大きく、または小さくしたいかを与えるべきだが、他のツール
	 *       を修正するのが面倒なので、この関数では、設定されたフォントサイズがデフォルト
	 *       サイズといくつ違うかを確認して行う
	 *
	 * @param	hWnd   [in] フォントサイズを変えたいウィンドウハンドル
	 * @param	size   [in] フォントサイズ
	 * @param	Weight [in] フォントの太さ
	 */
	void CreateFont(HWND hWnd, long size, long Weight)
	{
		// レジストリに登録されているコントロールの情報取得
		CNormalCtls cNormal( static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), GetClassName( hWnd ), 8 );
		cNormal.GetRegData();

		// 現在のコントロールのフォント取得
		CLogFont cLogFont( (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );

		// デフォルトのコントロールサイズを取得する
		CString strControlName = cNormal.GetControlName();
		double dInitFontSize = 0.0;
		if( strControlName.CompareNoCase( _T(ATLAXWINLIC_CLASS) ) == 0 )
			dInitFontSize = AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE;
		else
			dInitFontSize = AUTOFONTDLG_INIT_FONTSIZE;
		// この値がレジストリの値からいくつ大きく、または小さくしたいかの値となる
		double dDevFontSize = (double)size - dInitFontSize;

		HDC hDC = ::GetDC(hWnd);
		double dSetFontSize = 0.0;
		//レジストリに設定されている値取得して差を足す
		if( cNormal.GetSize() > 0 )
			dSetFontSize = cNormal.GetSize() + dDevFontSize;
		else
			dSetFontSize = dInitFontSize;
		// フォントサイズ設定
		cLogFont.SetHeight( dSetFontSize, hDC );
		if ( Weight >= FW_BOLD )
			cLogFont.SetBold();
		// 用済みデバイスコンテキスト削除
		::ReleaseDC( hWnd, hDC );

		// フォント作成しメンバで確保しておく
		HFONT hExFont = cLogFont.CreateFontIndirect();
		HFONTS fonts;
		fonts.push_back( hExFont );
		m_mapExFonts[hWnd] = fonts;
		// コントロールへフォントサイズ変更
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)hExFont, MAKELPARAM(TRUE, 0));
	}

	/**
	 * @brief 作成したフォントを削除する関数
	 *
	 * @note 途中でフォント削除したりするとおかしくなるのでフォントはダイアログ終了時に削除する
	 *       http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+200701/07010038.txt
	 */
	void Clear()
	{
		for ( std::map<HWND, HFONTS>::iterator it = m_mapExFonts.begin(); it != m_mapExFonts.end(); ++it )
		{
			for( HFONTS::iterator font = it->second.begin(); font != it->second.end(); ++font )
				::DeleteObject( *font );
		}
		m_mapExFonts.clear();

		m_mapCtrls.clear();
	}

	void Init()
	{
		// 除外リストレジストリ読み込み
		CString strReg;

		strReg.Format( _T("%s\\AutoFont\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) ) {
			CString strCtrlIDs;
			ULONG BufSize = 0;

			reg.QueryStringValue( _T("ExclutionCtrls"), NULL, &BufSize );
			if( ERROR_SUCCESS != reg.QueryStringValue( _T("ExclutionCtrls"), strCtrlIDs.GetBuffer( BufSize + 1 ), &BufSize ) )
			{
				strCtrlIDs.ReleaseBuffer();
				strCtrlIDs.Empty();
			} else
				strCtrlIDs.ReleaseBuffer();

			CString strToken( strCtrlIDs ), strID;
			int curPos=0;

			strID = strToken.Tokenize(_T(","), curPos);
			while ( strID != _T("") ) {
				m_listExclutions.push_back( _tcstol( strID, NULL, 10 ) );

				if ( curPos >= 0 )
					strID = strToken.Tokenize(_T(","), curPos);
				else
					break;
			}
		}

		m_listExclutions.sort();
		m_listExclutions.unique();

		InitControls();

		ChgFontSize( autodlg::FontSetMode::font_init );	// レジストリ

		// [Bug 6567][i][要望]登録ダイアログの前回表示位置と大きさを保持できるようにしてほしい。
		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			SetPosition(false);
		}
		else
		{
			// デフォルトサイズを記憶
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CRect rc;

				::GetWindowRect( m_hWnd, &rc );

				CString strRect;
				strRect.Format( _T("%d,%d,%d,%d"), rc.left, rc.top, rc.Width(), rc.Height() );

				reg.SetStringValue( _T("Default_Position"), strRect );
			}
		}
	}

	/**
	 * @brief  ウィンドウサイズとウィンドウ位置保存
	 *		   [Bug 6567][i][要望]登録ダイアログの前回表示位置と大きさを保持できるようにしてほしい。
	 * @param	x	[in]	X位置
	 * @param	y	[in]	Y位置
	 * @param	cx	[in]	幅
	 * @param	cy	[in]	高さ
	 *
	 */
	void SavePosition(long x, long y, long cx, long cy)
	{
		CString strReg;

		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
		{
			CString strRect;
			strRect.Format( _T("%d,%d,%d,%d"), x, y, cx, cy );

			reg.SetStringValue( _T("Position"), strRect );
		}
	}

	/**
	 * @brief  ウィンドウサイズとウィンドウ位置設定
	 *		   [Bug 6567][i][要望]登録ダイアログの前回表示位置と大きさを保持できるようにしてほしい。
	 * @param	bDefSize	[in]	デフォルトを適用するかどうか
	 *
	 */
	void SetPosition(bool bDefSize)
	{
		// [Bug 6567][i][要望]登録ダイアログの前回表示位置と大きさを保持できるようにしてほしい。
		CString strReg;

		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			CString strRect;
			ULONG BufSize = 0;

			CString strKey;

			if ( bDefSize )
				strKey += _T("Default_");

			strKey += _T("Position");

			reg.QueryStringValue( strKey, NULL, &BufSize );
			if( ERROR_SUCCESS != reg.QueryStringValue( strKey, strRect.GetBuffer( BufSize + 1 ), &BufSize ) )
			{
				strRect.ReleaseBuffer();
				strRect.Empty();
			}
			else
				strRect.ReleaseBuffer();

			if ( ! strRect.IsEmpty() )
			{
				CString strToken( strRect ), strSize;
				int curPos=0;
				long x=0,y=0,cx=0,cy=0;

				strSize = strToken.Tokenize(_T(","), curPos);
				x = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				y = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				cx = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				cy = _tcstol( strSize, NULL, 10 );

				::SetWindowPos( m_hWnd, HWND_TOP, x, y, cx, cy, SWP_NOACTIVATE|SWP_NOZORDER );
			}
		}
	}

	void Auto(WPARAM wParam)
	{
		if ( GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL ) {
			autodlg::FontSetMode nMode = autodlg::FontSetMode::font_big;	// フォントサイズ拡大

			if ( 0 < (short)GET_WHEEL_DELTA_WPARAM(wParam) )
				nMode =  autodlg::FontSetMode::font_small;	// フォントサイズ縮小

			ChgFontSize( nMode );
		}
	}

	void ChgFontSize(autodlg::FontSetMode nMode)
	{
		for ( std::map<long, boost::shared_ptr<CControlsImpl>>::iterator it = m_mapCtrls.begin(); it != m_mapCtrls.end(); ++it )
			it->second->ChgFontSize( nMode, m_mapExFonts );

		static_cast<TDlg*>(this)->FontResized();
	}

	void InitControls()
	{
		long idd = GetIDD();

		_InitControls( idd, WC_EDIT, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_STATIC, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_BUTTON, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_COMBOBOX, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_LISTBOX, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_LISTVIEW, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_TREEVIEW, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_TABCONTROL, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, MONTHCAL_CLASS, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, _T(ATLAXWINLIC_CLASS), AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE );

		GetControls( m_hWnd );	// 全コントロール取得
	}

	void FontResized(){};
	CString GetRegKey(){ return _T(""); };

	static CString GetClassName(HWND hWnd)
	{
		// クラス名取得
		CString strClassName;

		::GetClassName( hWnd, strClassName.GetBuffer(513), 512 );
		strClassName.ReleaseBuffer();

		return strClassName;
	}

	// ソート順読み込み
	// [Bug 7767][i][要望]問い合わせダイアログ上のソート順保持
	void LoadColSort(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, r2=0, c1=0, c2=0;
			ipFlexGrid->GetSelection(&r1, &c1, &r2, &c2);

			CString strReg;
			strReg.Format( _T("%s\\Sort\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lCols = -1;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					DWORD dwValue = 0;
					CString strCol;
					strCol.Format( _T("%d"), i );
					if ( ERROR_SUCCESS != reg.QueryDWORDValue( strCol, dwValue ) )
						dwValue = flexSortNone;
					else
						ipFlexGrid->Select( 0, i, 0, i );	// これが重要みたい

					ipFlexGrid->put_ColSort( i, (SortSettings)dwValue );
				}
			}

			ipFlexGrid->put_Sort( flexSortUseColSort );
			if( r1 >= 0 && c1 >= 0 )
				ipFlexGrid->Select( r1, c1 );
		}
	}

	// ソート順保存
	// [Bug 7767][i][要望]問い合わせダイアログ上のソート順保持
	void SaveColSort(IVSFlexGridPtr ipFlexGrid, long lCol, SortSettings sortype = flexSortNone, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 )
		{
			CString strReg;
			strReg.Format( _T("%s\\Sort\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				// 指定列以外は削除
				long lCols = -1;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					if ( lCol != i )
					{
						CString strCol;
						strCol.Format( _T("%d"), i );
						DWORD dwValue=0;
						if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
							reg.DeleteValue( strCol );
					}
				}

				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.SetDWORDValue( strCol, sortype );
			}
		}
	}

	// 列幅読み込み
	// [Bug 7768][i][要望]グリッドの列幅保持機能
	void LoadGridColWidth(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lCols = -1;
				bool bChg = false;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					DWORD dwValue = 0;
					CString strCol;
					strCol.Format( _T("%d"), i );
					if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
					{
						ipFlexGrid->Select( 0, i );
						ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 0, 100, 0, 0, 0, 0 );
						ipFlexGrid->put_ColWidth( i, dwValue );
					}
				}
			}
		}
	}

	// 列幅保存
	// [Bug 7768][i][要望]グリッドの列幅保持機能
	void SaveGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long width, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.SetDWORDValue( strCol, width );

				ipFlexGrid->Select( 0, lCol );
				ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 0, 100, 0, 0, 0, 0 );
			}
		}
	}

	// 列幅存在？
	bool HasGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0)
	{
		bool bRet = false;
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );

				DWORD dwValue = 0;
				if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
					bRet = true;
			}
		}

		return bRet;
	}

	// 列幅リセット
	// [Bug 7768][i][要望]グリッドの列幅保持機能
	void ResetGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.DeleteValue( strCol );

				ipFlexGrid->Select( 0, lCol );
				ipFlexGrid->CellBorder( RGB(172, 168, 153 ), 0, 0, 0, 0, 0, 0 );

				long lCols = -1;
				ipFlexGrid->get_Cols( &lCols );
				if ( lCols >= 1 )
				{
					ipFlexGrid->put_Redraw( flexRDNone );
					ipFlexGrid->put_AutoSizeMode( flexAutoSizeColWidth );
					ipFlexGrid->AutoSize( lCol );
					ipFlexGrid->put_Redraw( flexRDDirect );

					LoadGridColWidth( ipFlexGrid, mode );
				}
			}
		}
	}

	// 行高読み込み
	void LoadGridRowHeight(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lRows = -1;

				ipFlexGrid->get_Rows( &lRows );
				for( int i = 0; i < lRows; ++i )
				{
					DWORD dwValue = 0;
					CString strRow;
					strRow.Format( _T("%d"), i );
					if ( ERROR_SUCCESS == reg.QueryDWORDValue( strRow, dwValue ) )
					{
						ipFlexGrid->Select( i, 0 );
						ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 100, 0, 0, 0, 0, 0 );
						ipFlexGrid->put_RowHeight( i, dwValue );
					}
				}
			}
		}
	}

	// 行高保存
	void SaveGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long height, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );
				reg.SetDWORDValue( strRow, height );

				ipFlexGrid->Select( lRow, 0 );
				ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 100, 0, 0, 0, 0, 0 );
			}
		}
	}

	// 行高存在？
	bool HasGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0)
	{
		bool bRet = false;
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );

				DWORD dwValue = 0;
				if ( ERROR_SUCCESS == reg.QueryDWORDValue( strRow, dwValue ) )
					bRet = true;
			}
		}

		return bRet;
	}

	// 行高リセット
	void ResetGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );
				reg.DeleteValue( strRow );

				ipFlexGrid->Select( lRow, 0 );
				ipFlexGrid->CellBorder( RGB(172, 168, 153 ), 0, 0, 0, 0, 0, 0 );

				long lRows = -1;
				ipFlexGrid->get_Rows( &lRows );
				if ( lRows >= 1 )
				{
					long lCols = -1;
					ipFlexGrid->get_Cols( &lCols );
					ipFlexGrid->put_Redraw( flexRDNone );
					ipFlexGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
					ipFlexGrid->AutoSize( 0 );
					ipFlexGrid->put_Redraw( flexRDDirect );

					LoadGridRowHeight( ipFlexGrid, mode );
				}
			}
		}
	}

	// 列幅行高保存モード読み込み
	bool IsSaveGridSize(IVSFlexGridPtr ipFlexGrid, long nColRowMode, long id, long mode=0)
	{
		bool bRet = false;
		CString strReg;
		//strReg.Format( _T("%s\\%s\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), bRow ? _T("RowHeight") : _T("ColWidth"), GetIDD(), id, mode );
		strReg.Format( _T("%s"), static_cast<TDlg*>(this)->GetRegKey() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			DWORD dwValue = 0;
			if ( ERROR_SUCCESS == reg.QueryDWORDValue( _T("SaveGridHeightWidth"), dwValue ) )
				bRet = dwValue == 1 ? true : false;
		}

		return bRet;
	}

	// セル選択状態読み込み
	void LoadGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && m_mapGridSelects[mode].find( id ) != m_mapGridSelects[mode].end() )
		{
			long lRows = 0, lCols = 0;
			ipFlexGrid->get_Rows( &lRows );
			ipFlexGrid->get_Cols( &lCols );

			long lRow = m_mapGridSelects[mode][id].first;
			long lCol = m_mapGridSelects[mode][id].second;

			if ( lRow >= 0 && lRow <= lRows - 1 )
			{
				if ( lCol >= 0 )
					lCol = 0;
				if( lRow < 0 )
					lRow = 0;

				ipFlexGrid->Select( lRow, lCol <= lCols - 1 ? lCol : 0 );
				if ( ! ipFlexGrid->GetRowIsVisible( lRow ) || ipFlexGrid->GetRowPos( lRow ) == ipFlexGrid->GetClientHeight() )
					ipFlexGrid->put_TopRow( lRow );	// 選択セルを見える位置にスクロール
			}
			else
			{
				// 一番下or一番上かな
				;
			}
		}
	}

	// セル選択状態保持
	void SaveGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, r2=0, c1=0, c2=0;
			ipFlexGrid->GetSelection(&r1, &c1, &r2, &c2);

			m_mapGridSelects[mode][id] = std::pair<long,long>(r1, c1);
		}
	}

	// セル選択状態クリア
	void ClearGridSelect(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0)
	{
		if ( ! ipFlexGrid )
			m_mapGridSelects.clear();
		else
		{
			long id = GetGridID( ipFlexGrid );
			if ( id != -1 && m_mapGridSelects[mode].find( id ) != m_mapGridSelects[mode].end() )
				m_mapGridSelects[mode].erase( m_mapGridSelects[mode].find( id ) );
		}
	}

	// グリッド位置読み込み
	void LoadGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && m_mapGridPos[mode].find( id ) != m_mapGridPos[mode].end() )
		{
			long lRows = 0, lCols = 0;
			ipFlexGrid->get_Rows( &lRows );
			ipFlexGrid->get_Cols( &lCols );

			long lRow = m_mapGridPos[mode][id].first;
			long lCol = m_mapGridPos[mode][id].second;

			if ( lRow >= 0 && lRow <= lRows - 1 )
			{
				if ( lCol >= 0 )
					lCol = 0;
				if( lRow < 0 )
					lRow = 0;

				ipFlexGrid->Select( lRow, lCol <= lCols - 1 ? lCol : 0 );
				ipFlexGrid->put_TopRow( lRow );	// 選択セルを見える位置にスクロール
			}
		}
	}

	// グリッド位置保持
	void SaveGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, c1=0;
			ipFlexGrid->get_TopRow( &r1 );
			// とりあえず、トップ位置だけでいいんじゃね？
			// 列位置もってことになったら、get_LeftCol使ってね
			m_mapGridPos[mode][id] = std::pair<long,long>(r1, c1);
		}
	}

	// グリッド位置クリア
	void ClearGridPos(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0)
	{
		if ( ! ipFlexGrid )
			m_mapGridPos.clear();
		else
		{
			long id = GetGridID( ipFlexGrid );
			if ( id != -1 && m_mapGridPos[mode].find( id ) != m_mapGridPos[mode].end() )
				m_mapGridPos[mode].erase( m_mapGridPos[mode].find( id ) );
			// ちょっと無理やりかもしれないが、先頭にポジション移しておく
			ipFlexGrid->put_TopRow(0);
		}
	}

private:
	/**
	 * @brief  全コントロールのウィンドウハンドル取得
	 */
	void GetControls(HWND hWnd)
	{
		// 全コントロール取得
		for ( HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT) )
		{
			long nCtrlID = ::GetDlgCtrlID(hWndChild);

			// 除外コントロールかチェック
			std::list<long>::const_iterator itCtrl = std::find( m_listExclutions.begin(), m_listExclutions.end(), nCtrlID );
			if ( itCtrl == m_listExclutions.end() ) {
				// クラス名取得
				CString strClassName = GetClassName( hWndChild );

				// 同クラス名のクラスに格納
				std::map<long, boost::shared_ptr<CControlsImpl>>::iterator it = m_mapCtrls.find( GetClassType(strClassName) );
				if ( it != m_mapCtrls.end() )
				{
					long nStyle = ::GetWindowLong( hWndChild, GWL_STYLE );

					bool bAdd = false;
					switch ( it->first )
					{
						case 2: // Static
							if ( nStyle & SS_ICON || nStyle & SS_BITMAP )
								bAdd = false;
							else
								bAdd = true;
							break;
						case 4: // Button
							if ( ! (nStyle & BS_ICON) && ! (nStyle & BS_BITMAP) )
								bAdd = true;
							break;
						default: bAdd = true; break;
					}

					if ( bAdd )
						it->second->push_back( hWndChild );
				}

				if ( ::GetTopWindow(hWndChild) )
					GetControls( hWndChild );
			}
		}
	}

	void _InitControls(long idd, LPCTSTR lpcszName, double dbInitsize=8.0)
	{
		long nCtrID = GetClassType( lpcszName );

		if ( m_mapCtrls.find( nCtrID ) == m_mapCtrls.end() ) {
			switch ( nCtrID ) {
				case 1024:	// Grid
					m_mapCtrls[nCtrID] = (boost::shared_ptr<CControlsImpl>)new CGridCtls(static_cast<TDlg*>(this)->GetRegKey(), idd, lpcszName, dbInitsize, this);	// ActiveXコントロール(ライセンス付き？)
					break;
				default:
					m_mapCtrls[nCtrID] = (boost::shared_ptr<CControlsImpl>)new CNormalCtls(static_cast<TDlg*>(this)->GetRegKey(), idd, lpcszName, (long)dbInitsize);
					break;
			}
		}
	}

	static long GetClassType(LPCTSTR lpcszName)
	{
		if ( _tcsicmp( WC_EDIT, lpcszName ) == 0 )
			return 1;
		else if ( _tcsicmp( WC_STATIC, lpcszName ) == 0 )
			return 2;
		else if ( _tcsicmp( WC_BUTTON, lpcszName ) == 0 )
			return 4;
		else if ( _tcsicmp( WC_COMBOBOX, lpcszName ) == 0 )
			return 8;
		else if ( _tcsicmp( WC_LISTBOX, lpcszName ) == 0 )
			return 16;
		else if ( _tcsicmp( WC_LISTVIEW, lpcszName ) == 0 )
			return 32;
		else if ( _tcsicmp( WC_TREEVIEW, lpcszName ) == 0 )
			return 64;
		else if ( _tcsicmp( WC_TABCONTROL, lpcszName ) == 0 )
			return 128;
		else if ( _tcsicmp( MONTHCAL_CLASS, lpcszName ) == 0 )
			return 256;
		else if ( _tcsicmp( _T(ATLAXWINLIC_CLASS), lpcszName ) == 0 )
			return 1024;

		return -1;
	}

	// グリッドのID取得
	// [Bug 7768][i][要望]グリッドの列幅保持機能
	// [Bug 7767][i][要望]問い合わせダイアログ上のソート順保持
	long GetGridID(IVSFlexGridPtr ipFlexGrid)
	{
		long id = -1;

		if ( m_mapCtrls.find( 1024 ) != m_mapCtrls.end() )
		{
			CGridCtls* pGrid = (CGridCtls*)m_mapCtrls[1024].get();

			for ( std::list<HWND>::const_iterator it = pGrid->begin(); it != pGrid->end(); ++it )
			{
				IVSFlexGridPtr ipGrid = pGrid->GetFlexGrid( (*it) );

				if ( ipGrid == ipFlexGrid )
				{
					id = ::GetDlgCtrlID((*it));
					break;
				}
			}
		}

		return id;
	}

private:
	std::map<long, boost::shared_ptr<CControlsImpl>> m_mapCtrls;	//!< 取得クラス名群 map<コントロールID,フォント変更クラス>
	std::list<long> m_listExclutions;	//!< 除外コントロール群 list<コントロールID>
	std::map<HWND, HFONTS> m_mapExFonts;	//!< 拡張フォント map<ウィンドウハンドル,フォント>
	std::map<long,std::map<long, std::pair<long,long>>> m_mapGridSelects;
	std::map<long,std::map<long, std::pair<long,long>>> m_mapGridPos;
};
