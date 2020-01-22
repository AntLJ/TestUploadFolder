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

// AttrDistancePostPointDlg.h : CAttrDistancePostPointDlg の宣言

#pragma once

#include "resource.h"
#include "../HwyStartTool/HwyHelper.h"
#include "../../LIB/WinLib/str2.h"

typedef CHetCVDMap CCodeValueDomain;	//!< コードドメインを格納する
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrDistancePostPointDlg
class CAttrDistancePostPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrDistancePostPointDlg();
	~CAttrDistancePostPointDlg()
	{
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrDistancePostPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_DIRECTIONCODE, CBN_SELCHANGE, OnSelchangeDirection)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KILOPOST)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KILOPOST2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KILOPOST));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_KILOPOST2));

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * 路線種別参照用が変更されたときのイベントハンドラ
	 *
	 * 路線名称参照用と、路線コードの値を変更します
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// 選択された路線種別の取得
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// 路線コードの再描画
		if(! RedrawRoadNameList(0))
			return 0;
		// 路線コードの設定
		if(! SetRoadCode(m_nRoadCode))
			return 0;

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
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// 路線コードの設定
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

	/**
	 * 方向コードが変更されたときのイベントハンドラ
	 *
	 * 方向コードの値を変更します
	 */
	LRESULT OnSelchangeDirection(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された路線コードの取得
		CComboBox cbDirection = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
		int nIdx = cbDirection.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nDirCode = cbDirection.GetItemData(nIdx);

		CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_direction_table::kDirection );
		pFieldDef->m_vaValue = m_nDirCode;
		if(m_nDirCode != m_lOrgDirCode)
		{
			pFieldDef->m_bChanged = TRUE;	
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	/** 
	 * @fn HWND CAttrDistancePostPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
	 * @brief 子ダイアログを作成します。
	 *
	 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
	 *
	 * @param hWndParent [in] 親ウィンドウハンドル
	 * @param dwInitParam [in] ダイアログ初期化パラメータ（現在特に使用していません）
	 *
	 * @retval 作成された子ダイアログのハンドルを返します。
	 */
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);

	/**
	 * @fn void CAttrDistancePostPointDlg::Delete()
	 * @brief 作成した子ダイアログを削除します。
	 *
	 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
	 */
	virtual void Delete();

	/**
	 * @fn void CAttrDistancePostPointDlg::SetArcHelper( IApplication* ipApp )
	 * @brief ATTR_BASE_CLASSにArcHelperを渡します、また、HwyHelperにもArcHelperをセットします。
	 */
	virtual void SetArcHelper( IApplication* ipApp );
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 

	/**
	 * @fn BOOL CAttrDistancePostPointDlg::ErrorCheck()
	 * @brief エラーがないかどうか調べる
	 *
	 * ダイアログを閉じる際に、AttributeDlgObjから呼ばれる
	 * 施設情報名称と施設情報カナ名称がちゃんと対になって両方入っているか、または入っていないかたしかめる
	 *
	 * @return	エラーがなかったらTRUE、エラーがあればFALSE
	 */
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	/**
	 * @fn BOOL CAttrDistancePostPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
	 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
	 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
	 * 路線コード、路線内シーケンスの設定をしておきます。
	 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
	 *
	 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
	 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
	 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
	 * @param bForce		[in]	強制的に編集するかどうか
	 * @param bEditable		[in]	編集可能かどうか
	 *
	 * @retval 意味がない（TRUEしか返さない）
	 */
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	/**
	 * @fn void CAttrDistancePostPointDlg::CreateControlRelation()
	 * @brief 子ダイアログを実際に作成します。
	 *
	 * 実際の作成はControlRelに任せます。
	 */
	void CreateControlRelation();

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClassList(LONG nLineClass)
	 * @brief ダイアログが呼ばれたときに路線種別参照用のコンボボックスに値をセットする
	 *
	 * @param nLineClass [in] 路線コードの値
	 *
	 * @return 路線種別が取得できたらTRUE、取得できなかったらFALSE
	 */
	bool SetLineClassList(int nLineClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetLineClass(int* pClass)
	 * @brief 設定されている路線種別を取得する
	 *
	 * @param pClass [out] 路線コードの値
	 *
	 * @return 路線種別が取得できたらTRUE、取得できなかったらFALSE
	 */
	bool GetLineClass(int* pClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClass(int nClass)
	 * @brief 路線種別の値からコンボボックスを設定
	 *
	 * @param nClass [in] 路線コードの値
	 *
	 * @return 路線種別が取得できたらTRUE、取得できなかったらFALSE
	 */
	bool SetLineClass(int nClass);

	/**
	 * @fn bool CAttrDistancePostPointDlg::SetLineClassList(LONG nLineClass)
	 * @brief ダイアログが呼ばれたときに方向コード用のコンボボックスに値をセットする
	 *
	 * @param nDir [in] 方向コードの値
	 */
	void SetDirList(int nDir);

	/**
	 * @fn bool CAttrDistancePostPointDlg::RedrawRoadNameList(LONG nRoadCode)
	 * @brief 路線名称コンボボックスの更新
	 *
	 * @param nRoadCode [in] 路線コードの値
	 *
	 * @return 路線名称が取得できたらTRUE、取得できなかったらFALSE
	 */
	bool RedrawRoadNameList(int nRoadCode);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
	 * @brief 路線コードエディットボックスから路線コードの取得
	 *
	 * @param nRoadCode [out] 路線コードの値
	 *
	 * @return 路線コードが取得できたらTRUE、取得できなかったらFALSE
	 */
	bool GetRoadCode(int* pRoadCode);

	/**
	 * @fn bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
	 * @brief 路線コードエディットボックスから路線コードの取得
	 *
	 * @param nRoadCode [in] 路線コードの値
	 *
	 * @return 路線コードが取得できたらTRUE、取得できなかったらFALSE
	 */
	bool SetRoadCode(int nRoadCode);

	/**
	* @brief 路線コードテーブル取得
	* @return 路線コードテーブル
	*/
	ITablePtr GetRoadCodeTable();

	/**
	* @brief 路線コード取得
	* @return 路線コードが取得できたかどうか
	*/
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);

////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5;

	CString m_cRoadCode;                    //!< 文字列での路線コード

	// 元の値
	long m_lOrgRoadCode;                    //!< 最初の路線コード
	long m_lOrgDirCode;                     //!< 最初の方向コード

	// ダイアログの値保存用データ
	int m_nLineClass;                       //!< 現在選択中の路線種別コード
	int m_nRoadCode;                        //!< 現在選択中の路線コード
	int m_nDirCode;                         //!< 現在選択中の方向コード

	// ハイウェイ編集用各種データのコンテナ
	CHetRoadCodeSet m_cRoadCodeSet;         //!< 路線コード情報コンテナ
	HWND m_hChildWnd;                       //!< 子ダイアログハンドル
};
