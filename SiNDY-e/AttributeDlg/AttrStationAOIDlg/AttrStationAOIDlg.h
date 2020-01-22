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

#include "TableManagerBase.h"
#include "AOIListViewCtrl.h"
#include "RelationTableManager.h"

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< 再描画の依頼を受けるためのメッセージ

#include "AOISchema.h"
#include "sindy/schema.h"
#include "CommonData.h"

using namespace sindy::schema;

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrStationAOIDlg
class CAttrStationAOIDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	/**
	*  @brief コンストラクタ
	*/
	CAttrStationAOIDlg();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrStationAOIDlg)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH_POI, BN_CLICKED, OnClickedSearchButton)
	COMMAND_HANDLER(IDC_BUTTON_APPLY, BN_CLICKED, OnClickedApplyButton)
	NOTIFY_HANDLER_EX(IDC_SEARCHLIST,LVN_ITEMCHANGED,OnClickedCheckBox)
	NOTIFY_HANDLER_EX(IDC_SEARCHLIST,LVN_ITEMCHANGING,OnClickedList)
	NOTIFY_HANDLER_EX(IDC_RELATIONLIST,LVN_ITEMCHANGING,OnClickedList)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS);
// メッセージマップ
ALT_MSG_MAP(IDC_BUTTON_SEARCH_POI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()


/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg メッセージハンドラ
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
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		// FALSEにするとATTR_BASE_CLASS::OnInitDialog()が呼ばれる
		bHandled = FALSE;

		return TRUE;
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
			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * @brief 「検索」ボタンが押された時の処理
	 * @note このイベントに関してはCControlDef等でまかなえる処理ではないので、ここで処理
	 */
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief 「適用」ボタンが押された時の処理
	 * @note このイベントに関してはCControlDef等でまかなえる処理ではないので、ここで処理
	 */
	LRESULT OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief リストがクリックされた時の処理
	 */
	LRESULT OnClickedList(LPNMHDR pnmd);

	/**
	 * @brief チェックボックスがチェックされた時の処理
	 */
	LRESULT OnClickedCheckBox(LPNMHDR pnmd);

	/**
	 * @brief WM_REFRESHFEATURE 用イベントハンドラ
	 * @note  属性ダイアログから送られてくる再描画イベントを受けて、独自の再描画を行う
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL) override;
	virtual void Delete() override;
	virtual void SetArcHelper( IApplication* ipApp ) override { ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ) override { ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ) override {};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ) override { ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs() override { ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg() override { return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ) override { return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 

	/**
	* @brief OKが押された後、関連テーブルに編集内容を反映する
	* @note  新規フィーチャー作成時だけ呼ばれる
	* @retval TRUE 編集内容の反映に成功
	* @retval FALSE 編集内容の反映に失敗
	*/
	virtual BOOL ErrorCheck() override;
	virtual LONG GetTabNum() override { return -1; };
	virtual void SetTabNum(LONG lTabNum) override {};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName) override {};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) override ;
	BOOL CheckReturnKeyItem(INT nClassID) override { return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID) override { return TRUE; }
private:
	void CreateControlRelation();

	/**
	* @brief リストビューの初期化処理
	* @retval true  リストビューの初期化に成功
	* @retval false リストビューの初期化に失敗
	*/
	bool initializeListView();

	/**
	* @brief 指定されたOBJECTIDの関連情報を更新する
	* @param[in] lOID  関連情報を更新するOBJECTID
	* @param[in] lList 更新する情報群(元々はCAOIListViewCtrlで内部的に保持されているデータ)
	* @retval true  更新に成功
	* @retval false 更新に失敗
	*/
	bool update( long lOID, const AOI_MATRIX& lList );

	/**
	 * @brief 指定されたオブジェクトIDをもつSTATION_POINTをハイライト表示する
	 * @param[in] lOID ハイライト表示するSTATION_POINTのOBJECTID
	 * @retval true  成功
	 * @retval false DBからポイントを取得できなかった、といった要因のために失敗
	 */
	bool setHighLight( long lOID );

	/**
	* @brief 選択されたフィーチャーに関連付けられているPOIを取得し、リストビューに表示
	* @param[in] nClassID POIの情報を表示するリストビューのID
	*/
	void dispPOIInfo( UINT nClassID );

//以下、上記関数内で必要な処理を別関数として実装したもの

	// update
	/**
	* @brief リストからユニークなIDのカラムだけを抽出し、1つのリストとして取得
	* @param[in] カラムを抽出するリスト群(1要素=1行 1要素中の要素が各カラムの値になっている)
	* @return 取得したデータのリスト
	*/
	AOI_COLUMN getUniqueData( const AOI_MATRIX& lList ) const;

	/**
	* @brief std::list<long>同士の差を出す(引数1に存在し、引数2に存在しないものが返り値として得られる)
	* @param[in] argList1 差分を取るstd::list<long>その1
	* @param[in] argList2 差分を取るstd::list<long>その2
	* @return argList1に存在し、argList2に存在しない要素のリスト
	*/
	std::list<CString> getDiffList( const std::list<CString>& argList1, const std::list<CString>& argList2 ) const;

	/**
	* @brief 紐付けられているPOIのリストビューから、チェックを外されたPOIを消す 
	*/
	bool removePOIFromRelationLV();

	/**
	* @brief 紐付けられているPOIのリストビューに、チェックをつけられたPOIを足す 
	*/
	bool addPOIToRelationLV();

	/// リストビューに表示するデータを取得
	AOI_MATRIX getViewList( long lOID );

	/// リストビューに表示するデータを取得 二つのリストを抽出して並べ替えている
	AOI_MATRIX getViewList( AOI_MATRIX& railCorp, AOI_MATRIX& stationInfo ) const;

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	/*
	* @brief 各コントロールウィンドウ
	* @note  コントロールIDをキーとして、そのコントロール自身を取得できる
	*/
	std::map<int, CContainedWindow> m_mapSubClass;

	CRelationTableManager m_RelTableM;       //!< 関連テーブルに関わる処理を管理するインスタンス

	CTableManagerBase m_LineCorpManager;     //!< 参照テーブルの情報を読み出すためのクラス
	CTableManagerBase m_StationNameManager;  //!< 関連テーブルの情報を読み出すためのクラス

	CAOIListViewCtrl m_RelationView;         //!< 駅AOIと関連付けられているJOIN_LINE_INFOを表示するリストビュー
	CAOIListViewCtrl m_SearchView;           //!< 駅AOIの形状から周囲○m以内に存在するJOIN_LINE_INFOを表示するリストビュー

	IFeaturePtr      m_ipFeature;            //!< ハイライト表示するSTATION_POINTのフィーチャー
	IFeatureClassPtr m_ipStationClass;       //!< STATION_POINTのフィーチャークラス

	/**
	* @brief チェックボックスの状態の制御に使用
	* @warning プログラム上でチェックボックスのチェック状態を変更する場合には、変更する前にtrueにし、変更後、false戻す
	*/
	bool m_IsCheckedChangeFromProg;
	std::set<CString> m_CheckedRemove; //!< チェックを外されたもののリスト
};
