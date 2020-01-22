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

const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< 再描画の依頼を受けるためのメッセージ

class CAttrSignalPointDlg
	: public CAttrBaseDlg
	, public CExportDlg2
	, public IDispEventImpl<IDC_VSFLEXGRID_NODE, CAttrSignalPointDlg>
{
	static const LONG kDRAW_SIZE =   10;
	static const LONG kDRAW_RED  = ~0xE0;
	static const LONG kDRAW_GRN  = ~0x80;
	static const LONG kDRAW_BLU  = ~0x80;

	static const long LAYER_COL = 0;
	static const long OID_COL = 1;

public:
	CAttrSignalPointDlg(void);
	~CAttrSignalPointDlg(void);

BEGIN_MSG_MAP(CAttrSignalPointDlg)
	MESSAGE_HANDLER(WM_WINMGR,          OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG,      OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO,   OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT,    OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN,     OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC,  OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_TRANSFER_ALTITUDE, BN_CLICKED, OnClickedTransferAltitudeButton)
	COMMAND_HANDLER(IDC_BUTTON_TRANSFER_GR_HEIGHT, BN_CLICKED, OnClickedTransferGroundHeightButton)

	MESSAGE_HANDLER(WM_COMMAND,         OnCommand)
	MESSAGE_HANDLER(WM_SIZE,            OnSize)
	MESSAGE_HANDLER(WM_DESTROY,         OnDestroy)
	MESSAGE_HANDLER(WM_REFRESHFEATURE,  OnRefresh)

	ALT_MSG_MAP(IDC_EDIT_HORIZONTAL_ANGLE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_COMBO_SIGNAL_MOLD_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_LAMP_COUNT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_ARROW_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_WARNING_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_INDEPENDENT_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_ALTITUDE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_TRANSFER_ALTITUDE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_GROUND_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_TRANSFER_GR_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_HEIGHT)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_WIDTH)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_SOURCE_ID)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_COMBO_SPECULATION_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_EDIT_SOURCE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrSignalPointDlg)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_KEYDOWN, OnKeyDownGrid)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_MOUSEUP, OnMouseUpGrid)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, 0x01,           OnCellChangedGrid)          // セル移動
END_SINK_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		CreateControlRelation();

		// グリッドコントロールがある場合
		CAxWindow grid = GetDlgItem(IDC_VSFLEXGRID_NODE);
		grid.QueryControl(&m_ipFlexGrid);
		//grid_util::FormatString(m_ipFlexGrid, _T(""));
		if (m_ipFlexGrid)
		{
			long lWidth = 0;
			m_ipFlexGrid->get_ClientWidth(&lWidth);

			m_ipFlexGrid->put_ColWidth(LAYER_COL, 3500);
			m_ipFlexGrid->put_ColWidth(OID_COL, 1500);

			m_ipFlexGrid->put_ExplorerBar  (flexExSortShowAndMove);
			m_ipFlexGrid->put_ExtendLastCol(VARIANT_TRUE);
			m_ipFlexGrid->put_SelectionMode(flexSelectionByRow);

			CString combo;
			combo.Format(_T("%s|%s")
				, sindy::schema::adam_v2::lane_node::kTableName
				, sindy::schema::adam_v2::updown_node::kTableName
			);

			m_ipFlexGrid->PutColComboList(0, _bstr_t(combo.GetString()));
		}

		// GridCtrlからのイベントを取得
		AtlAdviseSinkMap(this, TRUE);

		// サブクラス化
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		return CAttrBaseDlg::OnInitDialog(uMsg, wParam, lParam, bHandled);
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		//if (m_ipPoint)
		//{
		//	AheDrawPoint(m_cArcHelper.GetDisplay(), m_ipPoint, TRUE, kDRAW_SIZE, kDRAW_RED, kDRAW_GRN, kDRAW_BLU);
		//	m_ipPoint = NULL;
		//}
		
		ClearNodePoint();
		
		ClearRelationLine();

		AtlAdviseSinkMap(this, FALSE);

		//地図再描画
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

		return 0;
	}

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		if (! m_bFinishInit) { return 0; }

		INT iMsg  = HIWORD(wParam);
		INT nCtrl = LOWORD(wParam);

		if      (iMsg == EN_SETFOCUS)  { SelectDlgItem  (nCtrl); }
		else if (iMsg == CBN_DROPDOWN) { SetComboboxList(nCtrl); }
		else if (iMsg == CBN_SELCHANGE || iMsg == BN_CLICKED || iMsg == EN_UPDATE)
		{
			m_cControlRel.ControlChanged(nCtrl);

			InvalidateRect(NULL, FALSE);
			SetButton(Changed());
		}

		// 編集可否の取得と関連処理
		return 0;
	}

	/**
	 * 再描画のイベントハンドラ
	 * 
	 * @see	SiNDY-e\AttributeDlg\AttrRailwayLinkDlg\AttrRailwayLinkDlg.h	OnRefresh
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		UpdateNodePoint();
		UpdateRelationLine();

		return 0;
	}

	/**
	 * @brief 「関連ノードのZ値を転写」ボタンを押したときの処理
	 * @note 関連ノードひとつと地上高の設定が取得できれば、ノードのZ値+地上高をZ値に設定する。ダメならメッセージボックス出して終わり
	 */
	LRESULT OnClickedTransferAltitudeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief 「Z値から地上高を転写」ボタンを押したときの処理
	 * @note 関連ノードひとつとフィーチャのZ値(0以外)が取得できれば、フィーチャのZ-ノードのZ値を地上高に設定する。ダメならメッセージボックス出して終わり
	 */
	LRESULT OnClickedTransferGroundHeightButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = 0L);
	virtual void Delete();
	virtual bool Update();
	virtual HWND GetDlg() { return m_hChildWnd; }
	virtual void SetArcHelper   (IApplication* ipApp) { CAttrBaseDlg::SetArcHelper(ipApp); }
	virtual void SetAliasOrField(BOOL bAliasOrField)  { m_cControlRel.SetAliasOrField(bAliasOrField); }
	virtual void SetFeatureDefList(std::list<CLQRowDef>*   pFeatureDefList) {}
	virtual void SetFeatureDefList(std::list<CFeatureDef>* pFeatureDefList) { CAttrBaseDlg::SetFeatureDefList(pFeatureDefList); }
	virtual void ClearFeatureDefs () { CAttrBaseDlg::ClearFeatureDefs(); }
	virtual BOOL SetCurrentFeatureDefIndex(LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable);
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum () { return -1; }
	virtual void SetTabNum (LONG lTabNum) {}
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) {}

	virtual LRESULT SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return ::SendMessage(m_hWnd, uMsg, wParam, lParam); }


	inline BOOL CheckReturnKeyItem(INT nClassID) { return CAttrBaseDlg::CheckReturnKeyItem(nClassID); }
	inline BOOL CheckEscKeyItem   (INT nClassID) { return CAttrBaseDlg::CheckEscKeyItem   (nClassID); }

private:
	void CreateControlRelation();
	BOOL SetRelationValue(CString gID = _T(""));
	/**
	 * ポイント表示/非表示/消去
	 */
	void DrawNodePoint(const IPointPtr& ipPoint);		// 描画(描いてなければ描く)
	void UpdateNodePoint();								// 再描画(描いてたのは描く)
	void EraseNodePoint(const IPointPtr& ipPoint);		// 削除(再描画あり)
	void ClearNodePoint();								// 削除(再描画なし)
	/**
	 * 再描画
	 *
	 * @param[in]	drawObj	true:ポイント、ポリライン再描画あり
	 */
	void RefreshScreen(bool drawObj = false);

	/**
	 * ポイント表示
	 *
	 * @parma[in]	ipPoint	ポイントジオメトリ
	 */
	void DrawPoint(const IPointPtr& ipPoint);

	// VS-FlexGridイベントハンドラ
	void __stdcall OnKeyDownGrid();
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/);

	/**
	 * ポリライン表示/非表示/消去
	 */
	void DrawRelationLine();	// 描画(描いてなければ描く)
	void UpdateRelationLine();	// 再描画(描いてたのは描く)
	void EraseRelationLine();	// 削除(再描画あり)
	void ClearRelationLine();	// 削除(再描画なし)

	/**
	 * ポリライン表示
	 *
	 * @param[in]	ipLine	ジオメトリ
	 * @param[in]	color	表示色
	 */
	void DrawPolyLine(const IPolylinePtr& ipLine, const DWORD& color);

	/**
	 * ポリラインジオメトリの作成
	 *
	 * @param[in]	point1	始点側ポイント
	 * @param[in]	point1	終点側ポイント
	 * @return		ポリラインジオメトリ
	 */
	IPolylinePtr makeLine(const IPointPtr& point1, const IPointPtr& point2);

	/**
	 * 表示用ポリラインの作成
	 *
	 * @param[in]	relTableName	関連テーブル名
	 * @param[in]	relOid			関連テーブルOID
	 * @param[in]	nodeTableName	ノードテーブル名
	 * @param[in]	nodeGid			ノードGID
	 * @param[in]	signalTableName	信号機テーブル名
	 * @param[in]	signalGid		信号機GID
	 */
	void setLine(
		const CString& relTableName , long relOid, 
		const CString& nodeTableName , CString nodeGid, 
		const CString& signalTableName , CString signaGid
		);

	/**
	 * GLOBALID を取得する
	 *
	 * @param[in]	table   取得元のテーブル
	 * @param[in]	oid     GLOBALID 取得対象の OBJECTID
	 * @return		GLOBALID
	 */
	CString getGID(const ITablePtr& table, long oid);

	/**
	 * OBJECTID を取得する
	 *
	 * @param[in]	table   取得元のテーブル
	 * @param[in]	gid     OBJECTID 取得対象の GLOBALID
	 * @return		OBJECTID
	 */
	long getOID(const ITablePtr& table, const CString& gid);

	/**
	 * @brief エディットコントロールの文字列からdouble値を得る
	 * @param [in] controlId コントロールのリソースID
	 * @param [out] value 得られたdouble値
	 * @retval true double値の取得に成功
	 * @retval false double値の取得に失敗(数値として読み取れない文字列。対数表示もダメ)
	 */
	bool GetDoubleFromEditControl(long controlId, double& value);

	/**
	 * @brief 道路標識が関連する車線ノードIDを得る
	 * @note 車線ノードが1つならば選択に関わらずそのIDを返す。複数なら選択されているIDを返す
	 * @return 車線ノードID
	 * @retval -1 車線ノードが得られなかった
	 */
	bool GetTargetNodeInfo(CString& tableName, long& nodeId);

	/**
	 * @brief 関連ノードのZ値を取得する
	 * @note 関連ノードが一意に特定できなかったりフィーチャが取得できなかったりすればエラー(警告も出す)
	 * @param nodeAlt [out] ノードのZ値
	 * @retval true Z値の取得成功
	 * @retval false Z値の取得失敗
	 */
	bool GetNodeAlt(double& nodeAlt);

private:
	IVSFlexGridPtr      m_ipFlexGrid;
	std::map<int, CContainedWindow> m_mapSubClass;	//!< 各コントロールウィンドウ

	HWND    m_hChildWnd;        // 子ダイアログハンドル


	/**
	 *	@brief  紐付けノード情報
	 */
	struct LinkageNode
	{
		CString layerName;
		long    oID;
		CString	gID;

		LinkageNode() : oID(0) {}
		LinkageNode(CString name, long oid, CString gid) 
			: layerName(name), oID(oid), gID(gid)  {}
		~LinkageNode() {}

		bool operator<(const LinkageNode& node) const {
			if( layerName.Compare(node.layerName) != 0 )
				return layerName < node.layerName;
			if( oID != node.oID )
				return oID < node.oID;
			return gID < node.gID;
		}

		bool operator=(const LinkageNode& node) const {
			return
				layerName.Compare(node.layerName) == 0 &&
				oID == node.oID &&
				gID.Compare(node.gID);
		}
	};

	std::set<LinkageNode> m_LinkagePoints;	//!<  紐付けノード情報
	IPointPtr       m_ipPoint;
	bool            m_isDraw;

	class NameID
	{
	public:
		long    oid;			// OID
		CString gid;			// GID
		CString className;		// テーブル名

		NameID() : oid(0) {}
		~NameID(){}
		void clear()
		{
			oid = 0;
			gid.Empty();
			className.Empty();
		}
	};

	class PolyLineInfo
	{
	public:
		class Key
		{
		public:
			NameID signalNameID;	// 信号機
			NameID nodeNameID;		// ノード
			NameID relNameID;		// 関連テーブル

			Key(){}
			~Key(){}
			void clear()
			{
				signalNameID.clear();
				nodeNameID.clear();
				relNameID.clear();
			}

			bool operator<(const Key &right) const
			{
				//relNameIDで比較
				return this->relNameID.oid < right.relNameID.oid;
			}
		};

		class Value
		{
		public:
			IPolylinePtr ipLine;	// ジオメトリ
			COLORREF color;			// 表示色
			bool isDraw;			// 表示状態(true:表示中)
			long row;				//行目
			
			Value() : 
				ipLine(nullptr),
				color(RGB(0xC0, 0x80, 0xC0)),
				isDraw(false),
				row(0)
			{}
			~Value(){}
			void clear()
			{
				ipLine = nullptr;
				color = RGB(0xC0, 0x80, 0xC0);
				isDraw = false;
				row = 0;
			}
		};
	public:
		Key key;
		Value value;

		PolyLineInfo(){}
		~PolyLineInfo(){}

		void clear()
		{
			key.clear();
			value.clear();
		}
	};

	std::map<PolyLineInfo::Key, PolyLineInfo::Value> m_mDrawLine;
	static const COLORREF LINE_COLOR;
};

