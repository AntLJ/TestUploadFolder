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
* @file MenuHelper.h
*
* @brief CMenu クラスの定義ファイル
*
* @author 千葉 智博
*
* @date   2002/01/23
*/
#if	!defined( __MENUHELPER_H__)
#define	__MENUHELPER_H__	//!< 重複インクルード防止

/**
* @class CMenu
*
* @brief ICON描画機能つきのオーナー描画メニューヘルパクラス
*
* @note ツールバーや、メニューバーから利用される関数のため、メニュー表示時に親となるWindowの制御も行っている
*/
namespace AHE
{
class CMenu
{
public:
	CMenu( void) : m_hParent( NULL), m_hMenu( NULL), m_atmMenuParent( 0)
	{
		hFont = reinterpret_cast< HFONT>( ::GetStockObject( DEFAULT_GUI_FONT));
		CXFrame = ::GetSystemMetrics( SM_CXFRAME);
		CYBorder = ::GetSystemMetrics( SM_CYBORDER);
		CXBorder = ::GetSystemMetrics( SM_CXBORDER);
		dwColor = ::GetSysColor( COLOR_GRAYTEXT);
		clrBkColorHighlight = ::GetSysColor( COLOR_HIGHLIGHT);
		clrTextColorHighlight = ::GetSysColor( COLOR_HIGHLIGHTTEXT);
		clrBkColorMenu = ::GetSysColor( COLOR_MENU);
		clrTextColorGray = ::GetSysColor( COLOR_GRAYTEXT);
		clrTextColorText = ::GetSysColor( COLOR_MENUTEXT);
//		listData.clear();
	}
	~CMenu( void);
	/** 
	* @brief メニュー作成。
	*
	* @note 最上位階層を作成する場合、親となるWindowの作成を同時に行う。
	* @note 子メニューの場合、Windowの生成は行われない。
	*
	* @param blParent [in] 最上位メニューフラッグ
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL CreateMenu( BOOL blParent = FALSE);

	/** 
	* @brief メニュー項目の挿入
	*
	* @note ビットマップファイルの管理は記憶のみ行うため、アプリケーション側でメニュー表示終了後削除する必要がある。
	* @note 文字列を未指定は強制的にセパレータとする
	*
	* @param unPosition [in] 挿入位置(Win32SDK参照のこと
	* @param unFlag [in] 挿入操作フラグ(Win32SDK参照のこと
	* @param unIDNewItem [in] 挿入メニュー項目ID(Win32SDK参照のこと
	* @param lpcszNewItem [in] メニュー文字列
	* @param hBitmap [in] メニュー項目に設定されるビットマップハンドル
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL InsertMenu( UINT unPosition, UINT unFlag, UINT unIDNewItem = -1, LPCTSTR lpcszNewItem = NULL, HBITMAP hBitmap = NULL);

	/** 
	* @brief ポップアップメニューを表示
	*
	* @param unFlag [in] メニュー表示操作フラグ(Win32SDK参照のこと
	* @param x [in] メニュー表示位置
	* @param y [in] メニュー表示位置
	* @param lpstRest [in] 
	*
	* @return 選択されたメニューID。未選択は０(Zero)が返される
	*/
	int TrackPopupMenu( UINT unFlag, int x, int y, LPCRECT lpstRect = NULL);

	/** 
	* @brief メニューのチェック
	*
	* @note 指定のメニューにチェックマーク付加の設定を行う
	*
	* @param nIDCheckItem [in] メニューチェック対象ID(Win32SDK参照のこと
	* @param nCheck [in] チェック状態(Win32SDK参照のこと
	*
	* @return 直前のチェック状態。IDが存在しない場合は、-1
	*/
	UINT CheckMenuItem( UINT nIDCheckItem, UINT nCheck);

	/** 
	* @brief メニューの使用許可
	*
	* @note 指定のメニューに使用許可の設定を行う
	*
	* @param nIDEnableItem [in] メニュー使用許可対象ID(Win32SDK参照のこと
	* @param nEnable [in] 使用許可状態(Win32SDK参照のこと
	*
	* @return 直前の使用許可状態。IDが存在しない場合は、-1
	*/
	UINT EnableMenuItem( UINT nIDEnableItem, UINT nEnable);

	/** 
	* @brief メニューの状態取得
	*
	* @param nID [in] メニューチェック対象ID(Win32SDK参照のこと
	* @param nFlags [in] チェック状態(Win32SDK参照のこと
	*
	* @return 現在のチェック状態。IDが存在しない場合は、-1
	*/
	UINT GetMenuState( UINT nID, UINT nFlags ) const;

	/** 
	* @brief メニューに含まれれるアイテムの個数を返す
	*
	* @return メニュー項目数
	*/
	UINT GetMenuItemCount( ) const;

	/** 
	* @brief 親付きのメニューの親を取得
	*
	* @note WM_MENUSELECT&SubclassWindow対策
	*
	* @return Window handle
	*/
	HWND GetParentWnd() const;


	/** 
	* @brief HMENUをクラスに関連付ける
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Attach( HMENU hMenu);

	/** 
	* @brief HMENUをこのクラスから切り離す
	*
	* @note 切り離したHMENUは、元のクラスを削除してもデストラクタで削除されない。
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	HMENU Detach( void);

private:
	/**
	* @brief メニュー削除関数。
	*
	* @note 階層にしたがって再起的に削除を実行する。
	*
	* @param hMenu [in] 削除対象となるメニューのハンドル
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL DestroyMenu( HMENU hMenu);

	/**
	* @brief 親となるWindowの削除
	*
	* @note 親となっているWindowを破棄する
	*
	* @param hWnd [in] 親Windowのハンドル
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL DestroyWindow( HWND hWnd);

	/**
	* @brief メニューおよび、親となるWindowの削除
	*/
	void Delete( void);

	/**
	* @brief WindowsMessageのオーナー描画対応関数
	*
	* @param nIDCtl [in] 描画アイテムID
	* @param lpDrawItemStruct [in] 描画情報
	*/
	void OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	/**
	* @brief WindowsMessageのメニューサイズ応答関数
	*
	* @note InsertMenu時に計算しておいた値を返し、適当なサイズのメニューを作成する。
	*
	* @param nIDCtl [in] 描画アイテムID
	* @param lpDrawItemStruct [in/out] 描画情報
	*/
	void OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	/**
	* @brief 親となるWindowのクラス登録関数
	*
	* @note オーナ描画メニューに応答するためのWindowクラス登録処理
	*/
	ATOM RegisterClass( void);

	HWND m_hParent;			//!< 最上位メニューの場合の親Windowのハンドル
	HMENU m_hMenu;			//!< 管理対象のメニューハンドル
	ATOM m_atmMenuParent;	//!< 親Windowクラス登録情報

	static LPCTSTR m_lpcszMenuParent;	//!< 親Windowのクラス名称

	/**
	* @brief Windowコールバック関数
	*
	* @note 最上メニューの親Windowのコールバック関数
	*
	* @param hWnd [in]
	* @param unMessage [in]
	* @param wParam [in]
	* @param lParam [in]
	*
	* @return HRESULT
	*/
	static HRESULT CALLBACK WindowProc( HWND hWnd, UINT unMessage, WPARAM wParam, LPARAM lParam);

	/**
	* @brief 描画に利用するアイコンサイズ
	*
	* @note 16x16のサイズに限定しているが、サイズを取得して表示させることも可能なので、状況に応じてしゅうせいのこと。
	*/
	enum
	{
		_ICON_WIDTH = 16,	//!< アイコンサイズ（高さ
		_ICON_HEIGHT = 16	//!< アイコンサイズ（幅
	};

	/**
	* @brief メニュー項目に付随するデータ管理のための構造体
	*
	* @note このデータをメニューアイテムの固有データとして保存させる
	*/
	struct ITEMDATA
	{
		int			cbSize;				//!< 構造体サイズ
		HBITMAP		hBitmap;			//!< ビットマップ
		UINT		unIDItem;			//!< メニューID
		SIZE		stSize;				//!< メニューとして表示する場合のサイズ
		TCHAR		lpItemString[1];	//!< メニュー文字列
//		void		*mallocAddr;
	};

	HFONT		hFont;
	INT			CXFrame;
	INT			CYBorder;
	INT			CXBorder;
	DWORD		dwColor;
	COLORREF	clrBkColorHighlight;
	COLORREF	clrTextColorHighlight;
	COLORREF	clrBkColorMenu;
	COLORREF	clrTextColorGray;
	COLORREF	clrTextColorText;

//	list<LPVOID>		listData;

};
}
#endif	// __MENUHELPER_H__
