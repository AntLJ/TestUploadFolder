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

// AttrHwyNodeDlg.h : CAttrHwyNodeDlg の宣言

#ifndef __ATTRHWYNODEDLG_H_
#define __ATTRHWYNODEDLG_H_

#include <atlframe.h>			// for COwnerDraw
#include "resource.h"
#include "RoadCodeListInfo.h"

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< 再描画の依頼を受けるためのメッセージ
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrHwyNodeDlg
class CAttrHwyNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public COwnerDraw<CAttrHwyNodeDlg>	// 料金所レーン情報イメージの描画のため、ダイアログをオーナードローに対応させる
{
public:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrHwyNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	COMMAND_HANDLER(IDC_COMBO_HWYNODE_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_HWYNODE_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_SEQUENCE, CBN_SELCHANGE, OnSelchangeRoadSequence)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	CHAIN_MSG_MAP(COwnerDraw<CAttrHwyNodeDlg>)

ALT_MSG_MAP(IDC_CHECK_ISHWMODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HWYNODE_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HWYNODE_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_JUNCTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GUIDE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLFUNC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_FROMNODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TONODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NOETC_CAR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ETC_FACIL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLFUNC_ETC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NOGUIDETOLL_ETC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_TAB_HWYNODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TOLLGATELANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HWYGUIDE_NUM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ETC_TOLLGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_ipFacilInfoTable = NULL;
		m_ipRoadCodeListTable = NULL;
		m_ipRoadNodeTable = NULL;

		// ビットマップの読み込み
		m_ImpassableImg.LoadBitmap(IDB_BITMAP_IMPASSABLE);
		m_PublicImg.LoadBitmap(IDB_BITMAP_PUBLIC);
		m_EtcImg.LoadBitmap(IDB_BITMAP_ETC);
		m_MixedImg.LoadBitmap(IDB_BITMAP_MIXED);
		m_SwitchImg.LoadBitmap(IDB_BITMAP_SWITCH);
		m_UnknownImg.LoadBitmap(IDB_BITMAP_UNKNOWN);
		m_InvalidImg.LoadBitmap(IDB_BITMAP_INVALID);

		// フォントの作成
		m_TollGateLaneImgFont.CreatePointFont(75, _T("Arial"));

		// コントロールとフィールドの関連付け
		CreateControlRelation();

		// 料金所レーン情報エディットコントロールの桁数制限
		CEdit aTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		aTollGateLaneEdit.SetLimitText(m_TollGateLaneStrMaxLength);

		// HWYNODE 用タブコントロール
		CTabCtrl cTab( GetDlgItem( IDC_TAB_HWYNODE ) );
		cTab.AddItem( AheLoadString(IDS_BASEINFO) );
		cTab.AddItem( AheLoadString(IDS_TOLLGATELANEINFO) );
		cTab.AddItem( AheLoadString(IDS_ETC_TOLLGATEINFO) );

		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_CHECK_ISHWMODE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_COMBO_SEQUENCE));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTION));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_COMBO_JUNCTION));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMY));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_CHECK_GUIDE));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLCLASS));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLFUNC));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_COMBO_FROMNODE_ID));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_COMBO_TONODE_ID));	
		m_wnd13.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd16.SubclassWindow(GetDlgItem(IDC_CHECK_NOETC_CAR));
		m_wnd17.SubclassWindow(GetDlgItem(IDC_COMBO_ETC_FACIL));
		m_wnd18.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLFUNC_ETC));
		m_wnd19.SubclassWindow(GetDlgItem(IDC_CHECK_NOGUIDETOLL_ETC));
		m_wnd20.SubclassWindow(GetDlgItem(IDC_EDIT_TOLLGATELANE));
		m_wnd21.SubclassWindow(GetDlgItem(IDC_EDIT_HWYGUIDE_NUM));
		m_wnd22.SubclassWindow(GetDlgItem(IDC_EDIT_ETC_TOLLGATE));

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_HWYNODE) );

		cTab.SetCurSel( m_ChangeType );
		ChangeWindowMgrMap( m_ChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_REFRESHFEATURE 用イベントハンドラ
	 *
	 * 再描画のイベントが属性ダイアログから送られてきます
	 * Refresh()で再描画します
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		if( wParam == 1 )
		{
			if( m_ipFromRoadGeom )
			{
				IEnvelopePtr ipFeatEnv;
				m_ipFromRoadGeom->get_Envelope( &ipFeatEnv );
				m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
			}
			m_ipFromRoadGeom = NULL;
			if( m_ipToRoadGeom )
			{
				IEnvelopePtr ipFeatEnv;
				m_ipToRoadGeom->get_Envelope( &ipFeatEnv );
				m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
			}
			m_ipToRoadGeom = NULL;
		}
		else
			Refresh();

		return 0;
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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );

			switch (nTargetControl)
			{
				// ハイウェイモードフラグが変更されたら流入・流出ノードのコンボボックスのオンオフ切り替え
				case IDC_CHECK_ISHWMODE:
					SetInOutNodeComboBox();
					break;
				// 流入・流出ノードのコンボボックスの値が変更されたら対応する矢印を再描画する
				case IDC_COMBO_FROMNODE_ID:
				case IDC_COMBO_TONODE_ID:
					DrawInOutNode();
					break;
				// [Bug6217] 一般車両対応フラグとETC施設コードの状態によって、それぞれに依存するコントロールのEnable/Disableを切り替える
				case IDC_CHECK_NOETC_CAR:
					ChangeDependentControlsOnNoEtcCarFlag();
					break;
				case IDC_COMBO_ETC_FACIL:
					ChangeDependentControlsOnEtcFacilCode();
					break;
				// 料金所レーン情報エディットコントロールが変更されたら対応するイメージを再描画
				case IDC_EDIT_TOLLGATELANE:
					DrawLaneImage();
					break;
			}

			// 料金所種別、料金所機能にエラーがないか調べる
			CheckItems();
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * 路線種別参照用が変更されたときのイベントハンドラ
	 *
	 * 路線名称参照用と、路線コードの値を変更します
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// 選択された路線種別の取得
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// 路線コードの再描画
		if(! RedrawRoadNameList(0))
			return 0;
		// 路線コードの設定
		SetRoadCode(m_nRoadCode);

		// 路線内シーケンスの値を0に設定する
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		// 路線コードの値に従ってハイウェイモードフラグのチェックボックスを更新（[Bug 6217]対応時に追加）
		ChangeHwyModeCheckBox(m_nRoadCode);

		return 0;
	}

	/**
	 * 路線名称が変更されたときのイベントハンドラ
	 *
	 * 路線コードの値を変更します
	 */
	LRESULT OnSelchangeRsel_Name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された路線コードの取得
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// 路線コードの設定
		SetRoadCode(m_nRoadCode);

		// 路線内シーケンスの値を0に設定する
		CFieldDef*		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		return 0;
	}

	/**
	 * 路線内シーケンス参照用が変更されたときのイベントハンドラ
	 *
	 * 路線内シーケンスの値を変更します。
	 */
	LRESULT OnSelchangeRoadSequence(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された路線内シーケンスの取得
		CComboBox cbRoadSequence = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_SEQUENCE));
		int nIdx = cbRoadSequence.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadSequence = cbRoadSequence.GetItemData(nIdx);
		m_cRoadSeq.Format(_T("%d"), m_nRoadSequence );	

		SetRoadSequence();

		return 0;
	}

	void DrawItem(LPDRAWITEMSTRUCT lpdis);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp );
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrHwyNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };
	void Refresh();

private:
	void CreateControlRelation();
	void ChangeWindowMgrMap(int cWindowMapIndex);
	void SetRoadSequence();
	void ChangeRoadSeqCombobox();
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	void SetRoadCode(int nRoadCode);
	void CheckItems();
	void SetInOutNodeComboBox();
	void CAttrHwyNodeDlg::DisableAllControls();
	IGeometryPtr GetConnectedRoad( LONG lNodeID, BOOL bIsIN, CFeatureDef* ipFeatureDef );
	void DrawInOutNode();
	void ChangeDependentControlsOnNoEtcCarFlag();
	void ChangeDependentControlsOnEtcFacilCode();
	void ChangeHwyModeCheckBox(long cRoadCode);
	bool EtcSpecificTollFieldsExist();
	bool TollGateLaneFieldExist();
	void SetAvailabilityOfEtcSpecificTollControls(void);
	void DrawLaneImage(void);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12,m_wnd13,
						m_wnd16,m_wnd17,m_wnd18,m_wnd19,m_wnd20,
						m_wnd21,m_wnd22;	//!< 各コントロールウィンドウ
	CContainedWindow	m_wndTab;

	CString		m_cRoadSeq;					//!< 文字列での路線内シーケンス
	// ダイアログの値保存用データ
	LONG	m_nLineClass;					//!< 現在選択中の路線種別コード
	LONG	m_nRoadCode;					//!< 現在選択中の路線コード
	LONG	m_nRoadSequence;				//!< 現在選択中の路線内シーケンス
	// 元の値
	LONG m_lOrgRoadSequence;				//!< 最初の路線内シーケンス
	LONG m_lOrgRoadCode;					//!< 最初の路線コード

	LONG	m_lFeatureIndex;				//!< 子ダイアログ側でのフィーチャインデックス保持用
	LONG	m_lTableIndex;					//!< 子ダイアログ側でのテーブルインデックス保持用
	LONG	m_lRowIndex;					//!< 子ダイアログ側でのRowインデックス保持用
	BOOL	m_bForce;						//!< 子ダイアログ側での強制編集モード保持用
	BOOL	m_bEditable;					//!< 子ダイアログ側での編集できるかどうか保持用

	HWND	m_hChildWnd;					//!< 子ダイアログハンドル

	IFeaturePtr	m_ipNodeUnderHwyNode;		//!< ハイウェイノードの下のノード
	IGeometryPtr m_ipFromRoadGeom;			//!< 流入道路ジオメトリ
	IGeometryPtr m_ipToRoadGeom;			//!< 流出道路ジオメトリ
	IGeometryPtr m_ipDummyFrom;				//!< ダミーの流入道路ジオメトリ
	IGeometryPtr m_ipDummyTo;				//!< ダミーの流出道路ジオメトリ

	ITablePtr	m_ipFacilInfoTable;			//!< 施設情報テーブル
	ITablePtr	m_ipRoadCodeListTable;		//!< 路線コードリストテーブル
	ITablePtr	m_ipRoadNodeTable;			//!< 道路ノードテーブル

	long	m_CurrentTabIndex;				//!< タブの表示位置保存用
	int		m_ChangeType;					//!< コントロール切り替え用

	// 料金所レーン情報のイメージ用ビットマップ
	CBitmap m_ImpassableImg;
	CBitmap m_PublicImg;
	CBitmap m_EtcImg;
	CBitmap m_MixedImg;
	CBitmap m_SwitchImg;
	CBitmap m_UnknownImg;
	CBitmap m_InvalidImg;

	CFont	m_TollGateLaneImgFont;						//!< 料金所レーン情報のイメージのレーン番号に使用するフォント
	static const long m_TollGateLaneStrMaxLength;		//!< 料金所レーン情報エディットコントロールの最大桁数
	static const long m_TollGateLaneImageColumnMaxNum;	//!< 料金所レーン情報イメージの１行に並べる画像の数

	// ハイウェイ編集用各種データのコンテナ
	RoadCodeListArray	m_RoadCodeListArray;	//!< Road_Code_Listから取得した情報のコンテナ

	std::auto_ptr<bool>	m_pEtcSpecificTollFieldsExist;	//!< ETC距離別料金対応のためのデータモデル変更[Bug6198]で追加されたフィールドが存在するか（NULLなら未チェック）
	std::auto_ptr<bool>	m_pTollGateLaneFieldExist;		//!< 料金所レーン対応のためのデータモデル変更[Bug6557]で追加された「料金所レーン情報」フィールドが存在するか（NULLなら未チェック）
};

namespace hwynode_dialog_tab	//!< タブの種類
{
	enum EHwyNodeDialogTab
	{
		base_attr,			//!< 「基本属性」タブ
		tollgate_lane,		//!< 「料金所レーン情報」タブ
		etc_tollgate,       //!< 「ETC料金所情報」タブ

		tab_end             //!<  タブ終端制御用
	};
}

namespace tollgate_lane_type	//!< 「料金所レーン情報」フィールドの数字と意味（料金所レーンの種類）との対応
{
	enum ETollGateLaneType
	{
		impassable		= 0,	//!< 使用不可
		public_lane		= 1,	//!< 一般レーン
		etc_lane		= 2,	//!< ETCレーン
		mixed_lane		= 3,	//!< 混在レーン
		switch_lane		= 4,	//!< 切り替えレーン
		unknown_lane	= 5,	//!< 不明レーン
		invalid,				//!< 上記以外の不正な値（コード値が増えた時は、invalidの前に追加すること）
		tollgate_lane_type_num
	};
}

#endif //__ATTRHWYNODEDLG_H_
