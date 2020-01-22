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


typedef CAttrBaseDlg ATTR_BASE_CLASS;

#define XMD_H
#include "util/Jpeg.h"

const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< 再描画の依頼を受けるためのメッセージ

// 画像扱うクラス(CitySurveyPointDlgからパクリ)
// CJpegはTrackLogManagerの画像描画用クラス。
// 画像読み込み等の基本機能を継承しつつ、属性ダイアログ用の描画機能を持たせたのがCJpeg2クラス。
class CJpeg2 : public CJpeg
{
public:
	CJpeg2()
	{
	}
	virtual ~CJpeg2()
	{
	}

	/*
	 * @brief 保持している画像を描画する
	 * @note 画像のあるなしを判定して、それぞれの場合に適した描画用関数を呼びます
	 * @param hWnd [in] ウィンドウのハンドル
	 * @param rc   [in] 描画範囲
	 */
	void draw(HWND hWnd, RECT& rc) const
	{
		m_cDib.m_pImage ? draw_Picture(hWnd, rc) : draw_NoPicture(hWnd, rc);
	}
	
	/*
	 * @brief 保持している画像を描画する
	 * @param hWnd [in] ウィンドウのハンドル
	 * @param rc   [in] 描画範囲
	 */
	void draw_Picture(HWND hWnd, RECT& rc) const
	{
		HDC dc = ::GetDC(hWnd);
		::SetStretchBltMode(dc, HALFTONE);
		::StretchDIBits(dc, rc.left, rc.top, abs(rc.left-rc.right), abs(rc.bottom-rc.top),
			0, 0, m_cDib.m_biHeader.biWidth, m_cDib.m_biHeader.biHeight, m_cDib.m_pImage, (BITMAPINFO *)&m_cDib.m_biHeader, DIB_RGB_COLORS, SRCCOPY);
		::ReleaseDC(hWnd, dc);
	}

	/*
	 * @brief 画像がない場合・読み込みに失敗している場合に黒く塗りつぶす
	 * @param hWnd [in] ウィンドウのハンドル
	 * @param rc   [in] 描画範囲
	 */
	void draw_NoPicture(HWND hWnd, RECT& rc) const
	{
		HRGN rgn = ::CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
		HDC dc = ::GetDC(hWnd);
		::GetClipRgn(dc, rgn);
		::FillRgn(dc, rgn, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
		PAINTSTRUCT paint;
		::BeginPaint(hWnd, &paint);
		::SetBkColor(dc, RGB(0, 0, 0));
		::SetTextColor(dc, RGB(255, 255, 255));
		::EndPaint(hWnd, &paint);
		::DeleteObject(rgn); // GDIリークする（bug 9786）
		::ReleaseDC(hWnd, dc);
	}
};

// 撮影ポイント保持クラス
struct PicPoint
{
	PicPoint()
	{
		m_x = 0;
		m_y = 0;
		m_pictureSeq = 0;
		m_angle = 0;
	}

	/**
	 * @brief コンストラクタ
	 * @note フィーチャからxy座標、画像格納順、撮影向きを取得・保持します
	 * @param ipFeature    撮影ポイントフィーチャ
	 * @param sequenceIdx  「画像格納順」フィールドインデックス
	 * @param directionIdx 「撮影向き」フィールドインデックス
	 */
	PicPoint( const IFeaturePtr& ipFeature, long sequenceIdx, long directionIdx )
	{
		// Point以外来ない前提
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );

		if( !ipGeom )
		{
			// 撮影ポイントはNULLもあり得るので0を入れておく。
			m_x = 0;
			m_y = 0;
		}
		else
		{
			((IPointPtr)ipGeom)->QueryCoords( &m_x, &m_y );
		}

		CComVariant vaSequence;
		ipFeature->get_Value( sequenceIdx, &vaSequence );
		m_pictureSeq = vaSequence.lVal;

		CComVariant vaAngle;
		ipFeature->get_Value( directionIdx, &vaAngle );
		m_angle = vaAngle.lVal;
	}

public:
	double m_x; //!< 経度
	double m_y; //!< 緯度
	long m_pictureSeq; //!< 格納順
	long m_angle; //!< 撮影向き(度) 真東を0度として左回りに増えていく
};

/////////////////////////////////////////////////////////////////////////////
// CAttrSpotSurveyPointDlg
class CAttrSpotSurveyPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSpotSurveyPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_BUTTON_ILLUST, BN_CLICKED, OnClickedIllustButton)
	COMMAND_HANDLER(IDC_BUTTON_POINT, BN_CLICKED, OnClickedPointButton)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
	MSG_WM_HSCROLL(OnHScroll)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS);

ALT_MSG_MAP(IDC_EDIT_NG_ATTR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NG_COMMENT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_ILLUST)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_POINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// 現調画像サムネイル描画
	LRESULT OnPaint( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// データビュー描画
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// 左クリックイベント
	LRESULT OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// 左ダブルクリックイベント
	LRESULT OnLButtonDblClk( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	/// スライダー動かした時のイベント
	void OnHScroll( int nCode, short nPos, HWND hWnd );

	/// イラスト表示ボタン
	LRESULT OnClickedIllustButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled );

	/// 移動前後確認ボタン
	LRESULT OnClickedPointButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled );


private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg メンバ関数定義
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
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrSpotSurveyPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

	/**
	 * @brief  移動前ポイント取得
	 * @return ポイント取得成否
	 */
	bool LoadSurveyPointOrg();

	/**
	 * @brief イラスト形状取得
	 * @param [in] ipWorkspace ワークスペース
	 * @param [in] owner       オーナ名
	 * @param [in] table       テーブル名
	 * @param [in] id          ObjectID
	 * @return イラスト形状
	 */
	IGeometryPtr GetIllustGeometry( const IWorkspacePtr& ipWorkspace, const CString& owner, const CString& table, const long id );

	/**
	 * @brief イラスト形状描画
	 */
	void DrawIllustGeometry();

	/**
	 * @brief 移動前ポイント描画
	 */
	void DrawOrgPointGeometry();

	/**
	 * @brief 撮影ポイント描画
	 */
	void DrawPicturePoint();

	/**
	 * @brief ワークスペース取得
	 * @return 選択中のSPOT_SURVEY_POINTが存在するワークスペース
	 */
	IWorkspacePtr GetWorkspace();

	/**
	 * @brief クリックした画像の画像格納順を取得
	 * @note XY座標はこのダイアログを基点にしたもの。スクリーンの絶対座標ではない。
	 * @param [in] clickedX       クリック地点のX座標
	 * @param [in] clickedY       クリック地点のY座標
	 * @return クリックした画像の画像格納順
	 */
	int getClickedPictureSeq( long clickedX, long clickedY );

	/**
	 * @brief サムネイルそれぞれの描画位置・幅・高さを取得
	 * @return <画像格納順,サムネイルの描画位置・幅・高さ>
	 */
	std::map<long,CRect> getThumbSize();

	/**
	 * @brief 撮影ポイントに紐づく画像取得
	 */
	void FindPicture();

	/**
	 * @brief レジストリから画像パスやら取得
	 */
	void GetRegCode();

	/**
	 * @brief 撮影ポイントハイライト
	 * @note  三角矢印シンボルを作成して描画します。
	 * @param [in] ipScreenDisplay IDisplayのインスタンス
	 * @param [in] ipPoint         対象ポイント
	 * @param [in] bXOR            XOR描画するかどうか
	 * @param [in] size            シンボルの大きさ
	 * @param [in] red             シンボルの色要素(赤)
	 * @param [in] green           シンボルの色要素(緑)
	 * @param [in] blue            シンボルの色要素(青)
	 * @param [in] angle           シンボルの回転角
	 * @param [in] bDoStartStop    StartDrawing, StopDrawingするかどうか（デフォルト true）
	 */
	void DrawPicturePointAllow( IDisplayPtr ipScreenDisplay, IPointPtr ipPoint, BOOL bXOR = FALSE, long size = 10, long red = 200, long green = 200, long blue = 200, long angle = 0, bool bDoStartStop = true );

	/**
	 * @brief ハイライト描画対象のジオメトリ変数にnullptrをセットします
	 */
	void SetNull2DrawGeometry();

	/**
	 * @brief 撮影画像ポイント矢印の再描画
	 */
	void RefreshPicturePointAllow();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////

private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ
	
	HWND                  m_hChildWnd;          //!< 子ダイアログハンドル
	long                  m_objectID;           //!< オブジェクトID
	CString               m_baseDir;            //!< 画像フォルダ
	CString               m_toolPath;           //!< 画像表示ツールのパス
	long                  m_slider;             //!< スライダー制御用
	std::vector<CString>  m_picPath;            //!< 画像パス格納用

	bool                  m_illustIsDrawed;     //!< イラスト描画してるかどうか
	bool                  m_orgPointIsDrawed;   //!< 調査前ポイントを描画しているかどうか
	IGeometryPtr          m_surveyPointGeom;    //!< 調査ポイントジオメトリ
	IGeometryPtr          m_orgSurveyPointGeom; //!< 調査前ポイントジオメトリ
	IGeometryPtr          m_illustPointGeom;    //!< イラストポイントジオメトリ
	IGeometryPtr          m_illustLineGeom;     //!< イラストラインジオメトリ
	IGeometryPtr          m_illustPolygonGeom;  //!< イラストポリゴンジオメトリ
	long                  m_pictureSeqIdx;      //!< 画像格納順フィールドのインデックス
	long                  m_directionIdx;       //!< 撮影向きフィールドのインデックス
	int                   m_selectedPicSeq;     //!< 選択中の撮影ポイント画像シーケンス
	std::vector<PicPoint> m_picPointVec;        //!< 撮影ポイント群
	std::vector<CJpeg2>   m_pictureVec;         //!< 撮影画像群
};
