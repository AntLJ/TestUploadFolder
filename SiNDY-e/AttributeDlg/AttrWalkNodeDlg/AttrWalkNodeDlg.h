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

/* MIDL: この毎回生成されるファイルにはインターフェイスの定義が含まれています */
// AttrWalkNodeDlgDlg.h : CAttrWalkNodeDlgDlg の宣言
#pragma once

#include "resource.h"
#include "globalfunc.h"
#include "ErrorCode.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;


/////////////////////////////////////////////////////////////////////////////
// CAttrWalkLinkDlg
class CAttrWalkNodeDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
	// 地下街出入口ポイントのコンテンツコード
	static const long UNDERGROUND_GATE_CONTENTS_CODE = 289000001L;

public:
	CAttrWalkNodeDlg();
	~CAttrWalkNodeDlg()
	{}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlgDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrWalkNodeDlg)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHSTATIONGATE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHUNDERGROUNDGATE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADNODE, BN_CLICKED, OnClkSearchBtn)
	COMMAND_HANDLER(IDC_BUTTON_INSERT, BN_CLICKED, OnClkInsertBtn)
	COMMAND_HANDLER(IDC_DELETE_ROADNODEID, BN_CLICKED, OnClickedDeleteButton)
	COMMAND_HANDLER(IDC_DELETE_STATIONGATEID, BN_CLICKED, OnClickedDeleteButton)
	COMMAND_HANDLER(IDC_DELETE_UNDERGROUNDGATEID, BN_CLICKED, OnClickedDeleteButton)
	NOTIFY_HANDLER(IDC_LIST_RESULT, NM_CLICK, OnClkList)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	
ALT_MSG_MAP(IDC_COMBO_WALKNODECLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROADNODEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID) // IDを手動入力した場合の処理
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // フォーカスが外れたときの処理
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADNODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_ROADNODEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_STATIONGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID)  // IDを手動入力した場合の処理
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // フォーカスが外れたときの処理
ALT_MSG_MAP(IDC_BUTTON_SEARCHSTATIONGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_STATIONGATEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_UNDERGROUNDGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDownEditID)  // IDを手動入力した場合の処理
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditFocusOut) // フォーカスが外れたときの処理
ALT_MSG_MAP(IDC_BUTTON_SEARCHUNDERGROUNDGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_DELETE_UNDERGROUNDGATEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEARCH)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_INSERT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_LIST_RESULT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg メッセージハンドラ
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
		for( auto& subclass : m_mapSubClass)
		{
			subclass.second.SubclassWindow( GetDlgItem( subclass.first ) );
		}

		// エディットボックスに初期値を設定
		// 変更されるとOnCommandが呼ばれて落ちるので、OnCommandの挙動を一旦止める
		m_bFinishInit = false;
		CString strMeter;
		strMeter.Format(_T("%ld"), 100);
		GetDlgItem( IDC_EDIT_SEARCH ).SetWindowText( strMeter );
		m_bFinishInit = true;

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );

			// ノード種別による編集可否をチェックするためコンボボックスを監視
			if( IDC_COMBO_WALKNODECLASS == nTargetControl )
				// ここまで処理がくるということは編集開始されている状態といえるので
				// 引数にはtrueを入れる
				SetControlState(true);
		}
		return 0;
	}

	/**
	 * @brief 関連IDエディットボックスでキーが押されたときの処理
	 */
	LRESULT OnKeyDownEditID( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
	{
		using namespace sindy::schema;
		// Enterキーが押されるか、親でTRUEが返された=フォーカス変更されたということで名称を検索
		if( wParam == VK_RETURN || OnKeyDown(uMsg, wParam, lParam, bHandled))
		{
			SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2);
			SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME);
			SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME);
			bHandled = TRUE;
			// 不正な入力データがあればエラーフラグを立てる
			CheckInvaliedValue();
			return TRUE;
		}
		return FALSE;
	}

	/**
	 * @brief 関連IDエディットボックスからフォーカスが外れたときの処理
	 */
	LRESULT OnEditFocusOut( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
	{
		using namespace sindy::schema;
		SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2);
		SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME);
		SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME);
		// 不正な入力データがあればエラーフラグを立てる
		CheckInvaliedValue();
		bHandled = FALSE;
		return FALSE;
	}

	/**
	 * @brief 検索ボタンが押されたときイベントハンドラ
	 * @note 押されたボタンに合わせて、指定された範囲で周辺検索する
	 */
	LRESULT OnClkSearchBtn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
	{
		using namespace  sindy::schema;

		// 名称カラム列の表示と周辺検索対象は押されたボタンによって切替える
		CString whereClause;
		switch(wID)
		{
		case IDC_BUTTON_SEARCHSTATIONGATE:
			// Msg 駅出入口名称, 駅出入口周辺検索結果
			ReplaceListViewNameColumn(IDS_STATIONGATECOLUMN, IDS_STATIONGATERESULT);
			SearchAroundPoint(station_gate_point::kTableName, station_gate_point::kName, station_gate_point::kYomi);
			break;
		case IDC_BUTTON_SEARCHUNDERGROUNDGATE:
			// Msg 地下街出入口名称, 地下街出入口周辺検索結果
			ReplaceListViewNameColumn(IDS_UNDERGROUNDGATECOLUMN, IDS_UNDERGROUNDGATERESULT);
			// [bug 11559]生存しているものだけを検索する
			whereClause.Format(_T("%s = %ld and %s = %d")
				, sindyk::poi_point_org::kContentsCode, UNDERGROUND_GATE_CONTENTS_CODE
				, sindyk::poi_point_org::kDelete, sindyk::poi_point_org::delete_code::kAlive );
			SearchAroundPoint(sindyk::poi_point_org::kTableName, sindyk::poi_point_org::kName, sindyk::poi_point_org::kYomi, whereClause );
			break;
		case IDC_BUTTON_SEARCHROADNODE:
			// Msg 交差点名称, 交差点周辺検索結果
			ReplaceListViewNameColumn(IDS_CROSSNODECOLUMN, IDS_CROSSRESULT);
			SearchAroundPoint(road_node::kTableName, road_node::kNameKanji, road_node::kNameYomi);
			break;
		default:
			break;
		}
		return 0;
	}

	/**
	 * @brief リストビュー上で左クリックした場合の処理
	 * @note 選択されたリストに対応するポイントフィーチャを点滅させる
	 */
	LRESULT OnClkList( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
	{
		// リストビューで選択されているフィーチャを光らせる
		BlinkPointFeature();
		return 0;
	}

	/**
	 * @brief [ << ]ボタン（適用）が押された場合の処理
	 */
	LRESULT OnClkInsertBtn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
	{
		using namespace  sindy::schema;

		CListViewCtrl lvResult( GetDlgItem(IDC_LIST_RESULT) );
		int nCurIdx = lvResult.GetSelectedIndex();
		if( -1 == nCurIdx )
		{
			// Msg 検索結果リストから該当レコードを選択してください。
			AttrMessageBox( AheLoadString( IDS_SELECTRECORD ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
			return 0;
		}
		InsertToEditBox(nCurIdx);
		return 0;
	}

	/**
	 * @brief 削除ボタンが押されたときの処理
	 */
	LRESULT OnClickedDeleteButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		switch(wID)
		{
		case IDC_DELETE_ROADNODEID: // 交差点ノード用の削除ボタンが押された
			SetDlgItemText( IDC_EDIT_ROADNODEID, _T("") );
			SetDlgItemText( IDC_STATIC_CROSSINGNAME2, _T("") );
			break;
		case IDC_DELETE_STATIONGATEID: // 駅出入口用の削除ボタンが押された
			SetDlgItemText( IDC_EDIT_STATIONGATE, _T("") );
			SetDlgItemText( IDC_STATIC_STATIONGATE_NAME, _T("") );
			break;
		case IDC_DELETE_UNDERGROUNDGATEID: // 地下街出入口用の削除ボタンが押された
			SetDlgItemText( IDC_EDIT_UNDERGROUNDGATE, _T("") );
			SetDlgItemText( IDC_STATIC_UNDERGROUNDGATE_NAME, _T("") );
			break;
		default:
			break;
		}
		// テキストボックスが変更された結果、不正な入力データになっているならエラーフラグを立てる
		CheckInvaliedValue();
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg メンバ関数定義
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
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	/**
	 * @brief コンボボックスの選択状態に合わせて、各コントロールの入力可否を変更
	 */
	void SetControlState(bool bEditable);

	/**
	 * @brief 指定フィーチャクラスを取得
	 * @note 取得失敗時にエラーメッセージをポップアップする
	 * 
	 * @param fcName	[in]	取得するフィーチャクラス名
	 * @param popupMsgTOCHide	[in]	TOC非表示時のエラーメッセージの実行有無(デフォルト：true)
	 * @retval nullptr以外	fcNameの取得に成功
	 * @retval nullptr	fcNameの取得に失敗
	 */
	IFeatureClassPtr GetFeatureClass( LPCTSTR fcName , bool popupMsgTOCHide=true );
		/**
		 * @breif 指定フィーチャと同じグループレイヤに所属する特定のフィーチャクラスを取得
		 * @note GetFeatureClass内でのみ使用
		 * @note ipSameGroupFeatureと同じグループレイヤ内にあるtargetNameを取得する
		 * @note ipSameGroupFeatureと同じグループレイヤ内にtargetNameが存在しない場合はTOC上からtargetNameを取得する
		 * @note 非表示レイヤの場合は基本的に無視するが、候補が全て非表示レイヤであるなら
		 * 関連付けて良いか、ポップアップでユーザに確認する
		 * @note 候補が複数ある場合は、最初に見つかったものを取得し、メッセージをポップアップする
		 *
		 * @param targetName         [in] 取得するフィーチャクラス名
		 * @param ipSameGroupFeature [in] 指定フィーチャ
		 * @param bMsgDisp           [in] メッセージを表示するか否か
		 * @param bCancel            [in] ポップアップでキャンセルが押されたか否か
		 * @param popupMsgTOCHide [in] TOC非表示時のエラーメッセージの実行有無(デフォルト：true)
		 * @retval nullptr以外 targetNameの取得に成功
		 * @retval nullpr targetNameの取得に失敗
		 */
		IFeatureClassPtr GetSameGroupFeatureClass( LPCTSTR targetName, const IFeaturePtr& ipSameGroupFeature, bool bMsgDisp, bool& bCancel, bool popupMsgTOCHide=true );

	/**
	 * @brief fcNameから対応IDをもつフィーチャを検索し、その名称を表示
	 * @note  交差点名称・駅出入り口ポイント・地下街出入口ポイントからnCodeIDに入力されているIDをもつフィーチャを検索して
	 * 第3引数で示されるコントロールに表示する
	 * @param fcName				[in] 検索対象のフィーチャクラス
	 * @param nCodeID				[in] 対応IDが入っているエディットコントロールのID
	 * @param nLabelID				[in] 名称を表示するスタティックコントロールのID
	*/
	void SetIDName(LPCTSTR fcName, INT nCodeID, INT nLabelID);

	/**
	 * @brief 検索リストの指定レコードの要素をエディットボックス・スタティックテキストに入力
	 * @warning m_searchedLayerで保持しているフィーチャクラスに合わせて入力先のエディットボックスを変更している
	 * そのため、m_searchedLayerを更新する前にInsertToEditBoxは呼んではならない
	 * @param nCurIdx [in] 挿入するリスト中のレコード番号
	 */
	void InsertToEditBox(int nCurIdx);

	/**
	 * @brief リストビューの名称カラム列を確認し、指定文字列が入っていない場合は指定文字列に置き換える
	 * @param colNameID [in] 名称を格納するカラム用のStringTableID
	 * @param resultID  [in] 検索結果ラべルに表示される文字列用のStringTableID
	 */
	void ReplaceListViewNameColumn(UINT colNameID, UINT resultID);

	/**
	 * @brief 歩道ノードの周辺に存在するfcNameフィーチャクラスのフィーチャを検索してリストビューに表示
	 * @note エディットボックスに入力されたメートル分、歩道ノードのバッファをとって
	 * それに含まれるフィーチャを取得する
	 * @param fcName        [in] 検索対象フィーチャ
	 * @param nameFieldName [in] 検索対象フィーチャにおける名称フィールドの名前
	 * @param yomiFieldName [in] 検索対象フィーチャにおける読みフィールドの名前
	 * @param whereClause   [in] 検索条件（デフォルト：NULL）
	 */
	void SearchAroundPoint(LPCTSTR fcName, LPCTSTR nameFieldName, LPCTSTR yomiFieldName, LPCTSTR whereClause=NULL );

	/**
	 * @brief リストビューで選択されているフィーチャを点滅させる
	 */
	void BlinkPointFeature();

	/**
	 * @brief 検索結果表示用リストビューの中身を作成
	 * @param nameColName [in] 名称を格納するカラムの名前
	 * @param resultStr   [in] 検索結果ラべルに表示される文字列
	 * 例えば、"駅出入り口の検索結果"、"交差点ノードの検索結果"といった文字列を渡す
	 */
	void CreateListView(LPCTSTR nameColName, LPCTSTR resultStr);

	/**
	 * @brief エディットボックスの入力に不正がないかチェック
	 */
	void CheckInvaliedValue();
		/**
		 * @brief POI対応IDの入力状態をみて、エラーフラグのONOFFを制御
		 * @note CheckInvaliedValue()内でのみ使用
		 * @note エラーがあった場合には、エラー内容をm_errorsにPushする
		 * @param nodeClass [in] コンボボックスで選択されているノード種別
		 * @param nID [in] 対応IDが入っているエディットコントロールのID
		 */
		void CheckCommonGateCtrl( DWORD_PTR nodeClass, INT nID);
		/**
		 * @brief 駅出入口対応IDの入力状態をみて、エラーフラグのONOFFを制御
		 * @note CheckInvaliedValue()内でのみ使用
		 * @note エラーがあった場合には、エラー内容をm_errorsにPushする
		 * @param nodeClass [in] コンボボックスで選択されているノード種別
		 */
		void CheckStationGateCtrl( DWORD_PTR nodeClass );
		/**
		 * @brief 地下街出入口対応IDの入力状態をみて、エラーフラグのONOFFを制御
		 * @note CheckInvaliedValue()内でのみ使用
		 * @note エラーがあった場合には、エラー内容をm_errorsにPushする
		 * @param nodeClass [in] コンボボックスで選択されているノード種別
		 */
		void CheckUndergroundGateCtrl( DWORD_PTR nodeClass);
		/**
		 * @brief 交差点ノード対応IDの入力状態をみて、エラーフラグのONOFFを制御
		 * @note CheckInvaliedValue()内でのみ使用
		 * @note エラーがあった場合には、エラー内容をm_errorsにPushする
		 * @param nodeClass [in] コンボボックスで選択されているノード種別
		 */
		void CheckCrossNodeCtrl(DWORD_PTR nodeClass);

	/**
	 * @brief フィーチャにフィールドが存在するかチェック
	 * @param	fcName		[in] 検索対象フィーチャ
	 * @param	fieldName	[in] 検索対象フィールド
	 * @return	true		存在する
	 * @return	false		存在しない
	 */
	bool CheckExistField( LPCTSTR fcName, LPCTSTR fieldName );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	HWND  m_hChildWnd;  //!< 子ダイアログハンドル
	HFONT m_hFont;
	static LVCOLUMN m_cResultColInfo[];		//!< 検索結果リストビューカラム情報
	static const int m_ListViewColumnSize = 2; //!< リストビューのカラム数

	/**
	 * @brief 歩道ノードフィーチャとそれに関連付けられるポイントフィーチャクラスの対応表
	 * @note 属性ダイアログを開いて周辺検索した状態でTOCから検索対象フィーチャを消されたときの判定に使用
	 * @note 現在、リストビューに表示されているフィーチャクラスを特定するために使用
	 */
	std::map<IFeaturePtr,IFeatureClassPtr> m_searchedLayer;

	/**
	 * @brief 歩道ノードフィーチャとエラー保持状態の対応表
	 * @note 複数のフィーチャを選択しているとき、あるフィーチャを不正な入力がある状態にし
	 * 別フィーチャに切り替えて、そのフィーチャの入力を正常なものにしてからOKを押すと
	 * 一つ目のフィーチャの属性として不正な入力が保存されてしまうので
	 * フィーチャごとのエラー情報を保持するマップを用意してErroCheckで使用している
	 * @note エラーがあるかどうかだけでは適切なエラー文を出力できないため、
	 * エラーコードを保持するErrorCodeKeeperを要素としてもつ
	 * TODO: GlobalのHNPを参照。
	 */
	std::map<IFeaturePtr,ErrorCodeKeeper> m_errors;
};
