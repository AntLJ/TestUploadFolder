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

// AttributeToolObj.h : CAttributeToolObj の宣言

#ifndef __ATTRIBUTETOOLOBJ_H_
#define __ATTRIBUTETOOLOBJ_H_

#include "resource.h"       // メイン シンボル
#include "stdafx.h"
#include "ArcHelperEx/Feedback.h"
#include "ArcHelperEx/SketchFeedback.h"
#include <WinLib/AdviseAdapter.h>
#include "ArcMapUIBase/ToolKeys.h"
#include "../../LIB/ArcMapUIBase/ToolKeys.h"

/////////////////////////////////////////////////////////////////////////////
// CAttributeToolObj
class ATL_NO_VTABLE CAttributeToolObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAttributeToolObj, &CLSID_AttributeToolObj>,
	public IAttributeToolObj,
	public _ICommand,
	public ITool,
	public IActiveViewEvents,
	public IDocumentEvents,
	public ICommandSubType,
	public CToolKeys
{
public:
	CAttributeToolObj()
	{
		// マウスポインタ読み込み
		m_cursor		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_IDENTIFY), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_curMove		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_IDENTIFYMOVE), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_curFreeSel	= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_FREESELECT), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcursor		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQ), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcurMove		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQMOVE), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcurFreeSel	= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQFREESELECT), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_pCurPointer = &m_cursor;

		// オブジェクトビットマップ読み込み
		m_bitmap	= static_cast<HBITMAP>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_IDENTIFY), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
		m_LQbitmap	= static_cast<HBITMAP>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LQ), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));

		// 初期化
		m_bIsMouseDown = false;
		m_bIsRightDown = false;
		m_ipMouseDownPoint = NULL;
		m_ipFeedbackEnv = NULL;
		m_bDrawingSelectArea = false;
		m_lMouseDownX = 0;
		m_lMouseDownY = 0;
		m_bFreeSelectMode = false;
		m_bShowContextMenu = false;
		m_lShift = 0;
		m_lSearchItemNum = 0;

		// プロパティ初期化
		m_bPerfectSelect = TRUE;
		m_bIsSelectedLastSequence = FALSE;
	}
	// デストラクタ
	~CAttributeToolObj()
	{
		// マウスポインタ及びビットマップ開放
		// カーソルは開放してはいけない。システム側でいらなくなったら勝手に開放してくれるので
		DeleteObject(m_bitmap);
		DeleteObject(m_LQbitmap);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATTRIBUTETOOLOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj COMマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_COM_MAP(CAttributeToolObj)
	COM_INTERFACE_ENTRY(IAttributeToolObj)
	COM_INTERFACE_ENTRY(_ICommand)
	COM_INTERFACE_ENTRY(ITool)
	COM_INTERFACE_ENTRY(IToolKeys)
	COM_INTERFACE_ENTRY(IActiveViewEvents)
	COM_INTERFACE_ENTRY(IDocumentEvents)
	COM_INTERFACE_ENTRY(ICommandSubType)
	COM_INTERFACE_ENTRY(IToolKeys)
END_COM_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj プロパティマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_PROP_MAP(CAttributeToolObj)
	PROP_ENTRY("SelectLastSequence", 1, CLSID_AttributeToolPropLQ)
	PROP_ENTRY("PerfectSelect", 2, CLSID_AttributeToolPropLQ)
END_PROP_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj カテゴリマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_CATEGORY_MAP(CAttributeToolObj)
	IMPLEMENTED_CATEGORY(__uuidof(CATID_MxCommands))
END_CATEGORY_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents インターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(ContentsChanged)();
	STDMETHOD(ContentsCleared)();
	STDMETHOD(ItemAdded)(VARIANT Item);
	STDMETHOD(ItemDeleted)(VARIANT Item);
	STDMETHOD(ItemReordered)(VARIANT Item, LONG toIndex);
	STDMETHOD(SelectionChanged)();
	STDMETHOD(ViewRefreshed)(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope);
	STDMETHOD(AfterDraw)(IDisplay * Display, esriViewDrawPhase phase);
	STDMETHOD(AfterItemDraw)(SHORT Index, IDisplay * Display, esriDrawPhase phase);
	STDMETHOD(FocusMapChanged)();
	STDMETHOD(SpatialReferenceChanged)();
	
/////////////////////////////////////////////////////////////////////////////
//
// ICommandインターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled);
	STDMETHOD(get_Checked)(VARIANT_BOOL * Checked);
	STDMETHOD(get_Name)(BSTR * Name);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(get_Tooltip)(BSTR * Tooltip);
	STDMETHOD(get_Message)(BSTR * Message);
	STDMETHOD(get_HelpFile)(BSTR * HelpFile);
	STDMETHOD(get_HelpContextID)(LONG * helpID);
	STDMETHOD(get_Bitmap)(OLE_HANDLE * Bitmap);
	STDMETHOD(get_Category)(BSTR * categoryName);
	STDMETHOD(OnCreate)(IDispatch * hook);
	STDMETHOD(OnClick)();
/////////////////////////////////////////////////////////////////////////////
//
// ICommandSubTypeインターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(SetSubType)(LONG SubType);
	STDMETHOD(GetCount)(LONG * Count);
/////////////////////////////////////////////////////////////////////////////
//
// IToolインターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_Cursor)(OLE_HANDLE * Cursor);
	STDMETHOD(OnMouseDown)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnMouseMove)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnMouseUp)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnDblClick)();
	STDMETHOD(OnKeyDown)(LONG keyCode, LONG Shift);
	STDMETHOD(OnKeyUp)(LONG keyCode, LONG Shift);
	STDMETHOD(OnContextMenu)(LONG X, LONG Y, VARIANT_BOOL * handled);
	STDMETHOD(Refresh)(OLE_HANDLE hDC);
	STDMETHOD(Deactivate)(VARIANT_BOOL * complete);
/////////////////////////////////////////////////////////////////////////////
//
// IDocumentEventsインターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(ActiveViewChanged)();
	STDMETHOD(MapsChanged)();
	STDMETHOD(NewDocument)();
	STDMETHOD(OpenDocument)();
	STDMETHOD(BeforeCloseDocument)(VARIANT_BOOL * abortClose);
	STDMETHOD(CloseDocument)();
/////////////////////////////////////////////////////////////////////////////
//
// IAttributeToolObj インターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_SelectLastSequence)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_SelectLastSequence)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_PerfectSelect)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_PerfectSelect)(/*[in]*/ VARIANT_BOOL newVal);

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	/**
	 * @brief フィーチャ選択専用のOnMouseUp関数\n
	 * この中で範囲選択した中からフィーチャを見つけ出し、AttributeDlgに渡して、ダイアログを出します
	 *
	 * @param Button	[in]	LONG 右クリック(1)or左クリック(2)
	 * @param Shift		[in]	LONG Shiftを押したかどうか
	 * @param X			[in]	LONG クリックしたX座標
	 * @param Y			[in]	LONG クリックしたY座標
	 */
	void OnAttrMouseUp(LONG Button, LONG Shift, LONG X, LONG Y);

	/**
	 * @brief リンク列選択専用のOnMouseUp関数\n
	 * この中で範囲選択した中からリンク列を見つけ出し、AttributeDlgに渡して、ダイアログを出します
	 * リンク列は基本的に、短形選択し、その範囲に入っている道路リンクを探して、その道路リンクについている
	 * リンク列が選択されます。
	 *
	 * @param Button	[in]	LONG 右クリック(1)or左クリック(2)
	 * @param Shift		[in]	LONG Shiftを押したかどうか
	 * @param X			[in]	LONG クリックしたX座標
	 * @param Y			[in]	LONG クリックしたY座標
	 */
	void OnLQMouseUp(LONG Button, LONG Shift, LONG X, LONG Y);

	/** 
	 * @brief 検索されるべきレイヤの一覧を取得する\n
	 * リンク列属性編集ツールと共通ゆえに道路リンクレイヤのみを持ってくる場合とで場合わけがされています
	 *
	 * @note 検索されるべきレイヤとは、フィーチャレイヤであり、TOCでレイヤが選択されていなければ、
	 * 表示されているすべてのレイヤとなり、レイヤが選択されていれば、そのレイヤ、またグループレイヤが選択されて
	 * いれば、そのグループレイヤ以下の表示されているレイヤすべてとなります
	 *
	 * @param bIsRoadLinkOnly	[in]	リンク列選択かフィーチャ選択か
	 *
	 * @return 検索されるべきレイヤのリスト
	 */
	std::list< CAdapt<ILayerPtr> > GetSearchLayer(bool bIsRoadLinkOnly);

	/** 
	 * @brief 選択レイヤの一覧を取得する\n
	 * リンク列属性編集ツールと共通ゆえに道路リンクレイヤのみを持ってくる場合とで場合わけがされています
	 *
	 * @note GetSearchLayerからのみ呼ばれる関数であり、選択しているレイヤ、またはグループレイヤからレイヤ
	 * のリストを返す関数です。
	 *
	 * @param ipSelectedLayer	[in]	選択しているレイヤ
	 * @param bIsRoadLinkOnly	[in]	リンク列選択かフィーチャ選択か
	 *
	 * @return 選択しているレイヤのリスト
	 */
	std::list< CAdapt<ILayerPtr> > _GetSearchLayer(ILayerPtr ipSelectedLayer, bool bIsRoadLinkOnly);

	/** 
	 * @brief レイヤが表示されているかどうか
	 *  
	 * @param ipLayer	[in]	レイヤ
	 *
	 * @return 表示されているなら true
	 */
	bool IsVisibleLayer( ILayerPtr ipLayer );

	/** 
	 * @brief グループレイヤ以下にある表示されているレイヤを返す
	 *  
	 * @param ipComositeLayer	[in]	グループレイヤ
	 *
	 * @return レイヤのリスト
	 */
	std::list< CAdapt< ILayerPtr > > GetAllLayerUnderGroup(ICompositeLayerPtr ipCompositeLayer);

	/** 
	 * @brief レイヤを検索して属性ダイアログにセットする
	 * 
	 * @param ipGeom	[in]	検索形状
	 * @param ipLayer	[in]	レイヤ
	 *
	 * @retval true 属性ダイアログにフィーチャセットした
	 * @retval false 属性ダイアログにフィーチャセットしなかった
	 */
	bool SelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer );

	/** 
	 * @brief 道路リンクについているリンク列を検索する\n
	 *  
	 * @param ipRoadClass	[in]	検索する道路リンククラス
	 * @param ipEnv			[in]	検索範囲形状
	 * @param type			[in]	リンク列のタイプ
	 * @param bLastLink		[in]	終端のリンクのみをもってくるかどうか
	 *
	 * @return 選択できたリンク列があるなら true
	 */
	bool SelectTargetLQs( IFeatureClassPtr ipRoadClass, IGeometryPtr ipGeom, sindyeRoadQueueType type, bool bLastLink  );

	/** 
	 * @brief AttributeDlgを作成・返す関数\n
	 * 属性ダイアログが作成されていなかったら作成する
	 *  
	 * @return 属性ダイアログ
	 */
	IAttributeDialogPtr GetAttributeDlg();

	/** 
	 * @brief 属性ダイアログに選択したフィーチャを渡す\n
	 *
	 * @param ipFeatureCursor 検索したフィーチャカーソル 
	 *
	 * @return 検索したフィーチャになにかフィーチャが入っていたらtrue
	 */
	bool AddFeatureToDialog( IFeatureCursorPtr ipFeatureCursor );

	/** 
	 * @brief 属性編集のプロパティの値が変更されたら、レジストリに登録する\n
	 *
	 * @param bIsPerfectSelect			方面、レーンの場合に取得リンクに関するすべてのリンク列取得するかどうか 
	 * @param bIsSelectedLastSequence	方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
	 */
	void WriteProperty( BOOL bIsPerfectSelect, BOOL bIsSelectedLastSequence );

	/** 
	 * @brief 起動時にレジストリに登録されているプロパティの内容を読み取る
	 */
	void LoadProperty();

	/**
	 * @brief <b>リンク列が表示されていて編集できるモード取得</b>\n
	 * 編集OKのモードは表示されている事が前提になっている。
	 * [Bug 6042] シンボル変更ビューツールのリンク列編集ボタンに対応して欲しい
	 * @return 編集OKなモード
	 */
	long GetEditLinkQueueMode(IFeatureRenderer* ipRednerer);

	/**
	 * @brief リンク列テーブル取得
	 * @param ipTable [in] ITable(道路)
	 * @param lpcszName [in] テーブル名
	 * @param lpcszExName [in] 例外処理を行うテーブル名(INF_IPCVICS/LQ_IPCVICS)
	 * @return ITable リンク列テーブル
	*/
	ITablePtr GetTable(const ITablePtr& ipTable, LPCTSTR lpcszName, LPCTSTR lpcszExName);

	/**
	 * @brief TOCにある指定したテーブル名をのITableを取得
	 * @param lpcszName [in] テーブル名
	 * @param lpcszOwner [in/optional] オーナ名
	 * @return 存在する時はITableが返る。存在しない時はNULL
	*/
	ITablePtr GetSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner=nullptr);

	/**
	 * @brief オーナ名を取得
	 * @param ipTable [in] テーブル
	 * @return オーナ名（空の時はパス名を返す）
	*/
	CString GetOwnerName(const ITablePtr& ipTable);

	/**
	 * 指定のエリア内に存在するロードキューのINFOレコードを取得
	 * ArcHelperEx::AheSelectInfoRowByShapeとほぼ同じ（LQRFを見ない版）
	 * 
	 * @param ipRoadClass			[in]			道路リンククラス
	 * @param ipInfoTable			[in]			検索するロードキューの INFO テーブル
	 * @param ipLQTable				[in]			検索するロードキューの LQ テーブル
	 * @param ipGeom				[in]			選択範囲（道路リンクを選択）
	 * @param lDrawMode				[in]			リンク列の種類（IRoadRenderer で使用する描画モードを使用すること）
	 * @param listInfoRows			[out]			取得できた INFO のレコード
	 * @param listRoads				[out]			取得できた道路リンク
	 * @param lSequence				[in,optional]	LQ_* を検索する際に指定するシーケンス番号（デフォルト：-1）
	 * @param bLastLink				[in,optional]	流出リンクを含めるかどうか
	 *
	 * @retval 取得できたINFOのレコード数
	 */
	long SelectInfoRowByShape( const IFeatureClassPtr& ipRoadClass, const ITablePtr& ipInfoTable, const ITablePtr& ipLQTable, const IGeometryPtr& ipGeom, long lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, long lSequence = -1, bool bLastLink = false );


/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CArcHelper				m_cArcHelper;		//!< ArcObjects ヘルパークラス
	IAttributeDialogPtr		m_ipAttributeDlg;	//!< 属性表示・編集ダイアログ
	CLoadQueueInfo			m_cLQInfo;			//!< リンク列の情報

	bool					m_bIsMouseDown;		//!< 左クリック判定用
	bool					m_bIsRightDown;		//!< 右クリック判定用
	long					m_lMouseDownX, m_lMouseDownY; //!< MouseDownした座標
	bool					m_bFreeSelectMode;	//!< 自由選択モードフラグ
	bool					m_bShowContextMenu;	//!< 右クリックメニューを出すべきかどうか
	long					m_lShift;			//!< シフトを押しているかどうか

	IPointPtr				m_ipMouseDownPoint;	//!< マウスをクリックした地点の座標
	INewEnvelopeFeedbackPtr	m_ipFeedbackEnv;	//!< 選択矩形描画用
	bool					m_bDrawingSelectArea;	//!< 短形選択中かどうか
	CFeedback				m_cFeedback;		//!< 短形選択用のフィードバッククラス
	CSketchFeedback	m_cSketchFeedback;			//!< 領域選択用フィードバック
	IGeometryPtr			m_ipSelectGeom;		//!< 自由選択モードの形状
	long					m_lSearchItemNum;	//!< 選択したアイテムの数

	long					m_lSubType;			//!< サブタイプ

	// カーソル関係
	HCURSOR					m_cursor;			//!< 属性表示編集ツールのデフォルトカーソル
	HCURSOR					m_LQcursor;			//!< リンク列属性表示編集ツールのデフォルトカーソル
	HCURSOR					m_curMove;			//!< 動いている途中のカーソル
	HCURSOR					m_LQcurMove;		//!< リンク列属性表示編集ツールの動いている途中のカーソル
	HCURSOR					m_curFreeSel;		//!< 自由選択用途中のカーソル
	HCURSOR					m_LQcurFreeSel;		//!< リンク列属性表示編集ツールの自由選択途中のカーソル
	HCURSOR*                m_pCurPointer;		//!< 現在表示中のカーソルポインタ
	HBITMAP					m_bitmap;			//!< 属性表示編集ツールボタンのビットマップ
	HBITMAP					m_LQbitmap;			//!< リンク列属性表示編集ツールボタンのビットマップ

	// イベント接続
	CAdviseAdapter<IDocument, &IID_IDocumentEvents>		m_cDocumentEvents;	//!< IDocumentEventsへの接続クラス
	CAdviseAdapter<IActiveView, &IID_IActiveViewEvents>	m_cActiveViewEvents;	//!< IActiveViewEventsへの接続クラス

	BOOL					m_bPerfectSelect;	//!< 方面、レーンの場合に完璧に取得するかどうか
	BOOL					m_bIsSelectedLastSequence;	//!< 方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
};

#endif //__ATTRIBUTETOOLOBJ_H_
