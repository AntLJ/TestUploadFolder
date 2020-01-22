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

// AttrWarningSignDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrWarningSignDlg.h"
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <WinLib/winutil.h>
#include <ArcHelperEx/Map.h>
#include <WinLib/grid_util.h>

using namespace std;
using namespace sindy::schema;
using namespace uh::str_util;
using namespace search_road_link_grid;

namespace
{
	// グリッドのヘッダ
	static const _TCHAR* SEARCH_ROAD_LINK_GRID_HEADER = _T("ID|距離(m)");

	// 道路リンク検索の初期値(メートル)
	static const long INIT_SEARCH_METER = 10;

	// 道路リンク検索の最大距離(メートル)
	static const long MAX_SEARCH_METER = 50;
}

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(WARNING_SIGN_MAP)
	BEGINROWS(WRCT_REST, 0, RCMARGINS(2, 2))
		BEGINROWS(WRCT_REST, 0, 0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_WARNING_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WARNING_SIGN)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_LINK_ID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINK_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_PICTURE_PATH)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PICTURE_PATH)
				RCTOFIT(IDC_BUTTON_COPY_PICTURE_PATH)
				RCTOFIT(-1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10, 0))
				RCTOFIT(IDC_STATIC_DISTANCE)
				RCSPACE(6)
				RCTOFIT(IDC_EDIT_SEARCH_METER)
				RCSPACE(6)
				RCTOFIT(IDC_STATIC_METER)
				RCSPACE(10)
				RCTOFIT(IDC_BUTTON_SEARCH_ROAD_LINK)
				RCSPACE(10)
				RCTOFIT(IDC_BUTTON_UPDATE_LINK_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_PCT, 0, 27)
				RCREST(IDC_GRID_ROAD_LINK)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrWarningSignDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrWarningSignDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

/**
 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
 */
LRESULT CAttrWarningSignDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);

	// コントロール
	CreateControlRelation();
	// サブクラス化
	for(auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	// シンクイベントのアドバイズ
	AtlAdviseSinkMap(this, true);
	
	return 1;
}

/**
 * WM_COMMAND 用イベントハンドラ
 *
 * 各コモンコントロールで変更があった場合、ここで処理します
 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
 */
LRESULT CAttrWarningSignDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if(!m_bFinishInit) return 0;

	int msg = HIWORD(wParam);						// 操作メッセージ
	int	nTargetControl = (int)LOWORD(wParam);		// 操作対象コントロール

	// エディットボックスの時に全選択にする
	if(msg == EN_SETFOCUS)
		SelectDlgItem(nTargetControl);
	// コンボボックスのリストボックスの長さを調節
	if(msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged((int)LOWORD(wParam));
		// ダイアログを更新領域に追加します
		InvalidateRect(NULL, FALSE);
		// 変更された場合はボタンを Enable に
		SetButton(Changed());
	}

	return 0;
}

/**
 * 走行画像ファイルパスコピーボタンを押したときの処理
 */
LRESULT CAttrWarningSignDlg::OnClickedButtonCopyPicturePath(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// クリップボードに走行画像ファイルパスを渡す
	CFieldDef* pPicPathDef = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kPicturePath);
	CString strPicPath = (pPicPathDef->m_vaValue.vt == VT_NULL) ? CString() : pPicPathDef->m_vaValue;
	winutil::SetToClipboard(strPicPath);

	return 0;
}

/**
* 道路リンク検索ボタンを押したときの処理
* 選択中の警戒標識から指定した距離以内に存在する道路リンクを探す
*/
LRESULT CAttrWarningSignDlg::OnClickedSearchRoadLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 道路リンクの検索を実施
	ExecuteSearchRoadLink();
	return 0;
}

/**
* リンクID適用ボタンを押したときの処理
*/
LRESULT CAttrWarningSignDlg::OnClickedButtonUpdateLinkID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// グリッドで選択したフィーチャを取得
	// フィーチャが空、または同じフィーチャであった場合は、変更なし
	// 変更ありならLINK_IDに反映
	
	if(m_ipSelectedRoadLinkItem.GetFeature() && m_ipRoadLinkItem != m_ipSelectedRoadLinkItem)
	{
		m_ipRoadLinkItem = m_ipSelectedRoadLinkItem;
		SetDlgItemText(IDC_EDIT_LINK_ID, ToString(m_ipRoadLinkItem.GetOID()));
		CFieldDef* pLinkIdDef = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kLinkID);
		pLinkIdDef->SetNewValue(CComVariant(m_ipRoadLinkItem.GetOID()), m_hWnd);
		SetButton(Changed());
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
	}

	return 0;
}


/**
 * 描画処理
 */
LRESULT CAttrWarningSignDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 紐付く道路リンクをハイライト
	IGeometryPtr ipRoadLinkGeom = m_ipRoadLinkItem.GetShape();
	if(ipRoadLinkGeom)
	{
		AheDrawPolyline(m_cArcHelper.GetDisplay(), ipRoadLinkGeom, RGB(255, 0, 0), 5, esriSLSSolid, TRUE, TRUE);
	}
	// グリッドで選択中の道路リンクをハイライト
	IGeometryPtr ipSelectedRoadLinkGeom = m_ipSelectedRoadLinkItem.GetShape();
	if(ipSelectedRoadLinkGeom)
	{
		// 選択したフィーチャをハイライト
		AheDrawPolyline(m_cArcHelper.GetDisplay(), ipSelectedRoadLinkGeom, RGB(0, 0, 255), 5, esriSLSSolid, TRUE, TRUE);
	}

	return 0;
}

/**
 * ドロップダウンリストを開くときの処理
 */
LRESULT CAttrWarningSignDlg::OnOpeningDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ドロップダウンリスト内の"x : 未定義"を選択できないよう、開く直前にリストから削除する
	CComboBox cb(GetDlgItem(IDC_COMBO_WARNING_SIGN));

	int nIdx = cb.FindStringExact(0, AheLoadString(IDS_COMBO_ITEM_FOR_UNDEFINED_VALUE).GetString());
	if(nIdx != CB_ERR)
		cb.DeleteString(nIdx);

	return 0;
}

/**
 * ドロップダウンリストを閉じたときの処理
 */
LRESULT CAttrWarningSignDlg::OnClosedDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// リストから選択した項目のコード値を一旦取得
	CComboBox cb(GetDlgItem(IDC_COMBO_WARNING_SIGN));
	int nCurSel = cb.GetCurSel();
	long lSelVal = cb.GetItemData(nCurSel);
	
	// ドロップダウンリストを作り直す
	SettingWarningSignDropdownList();
	
	// リストから選択したコード値の項目を改めてセットし直す
	int nCount = cb.GetCount();
	int nIdxUndefinedItem = -1;
	for(int i = 0; i < nCount; ++i)
	{
		long lItemVal = cb.GetItemData(i);
		if(lItemVal == warning_sign::warning_sign_class::kNone)
		{
			nIdxUndefinedItem = i;
		}
		if(lItemVal == lSelVal)
		{
			cb.SetCurSel(i);
			return 0;
		}
	}

	// ドメインで定義されていないコード値の場合は、
	// コード値0の項目をセットする
	cb.SetCurSel(nIdxUndefinedItem);

	return 0;
}

/**
* TABキーでのコントロール移動、及び矢印キーの上下でコンボボックスのアイテムの変更のエミュレート
*/
LRESULT CAttrWarningSignDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// エンターキー以外の場合はATTR_BASE_CLASSの方の処理を呼び出し
	if(wParam != VK_RETURN)
		return ATTR_BASE_CLASS::OnKeyDown(uMsg, wParam, lParam, bHandled);

	// 検索距離のテキストボックスでエンターキーが押された場合は道路リンクを検索
	// それ以外では何もしない
	CWindow cWnd = GetFocus();
	if(cWnd.GetDlgCtrlID() == IDC_EDIT_SEARCH_METER)
		ExecuteSearchRoadLink();

	return 1;
}

/**
* 道路リンク検索結果グリッドでマウスを離したときの処理
*/
void __stdcall CAttrWarningSignDlg::OnRoadLinkMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	long lRow = -1;
	m_spSearchRoadLinkGrid->get_MouseRow(&lRow);

	// グリッド外でマウスを離した場合、カレントセルを選択状態にする
	if(lRow <= 0)
		m_spSearchRoadLinkGrid->get_Row(&lRow);

	// 行の選択状態を更新
	for(int i = m_spSearchRoadLinkGrid->FixedRows; i <= m_spSearchRoadLinkGrid->Rows - m_spSearchRoadLinkGrid->FixedRows; ++i)
	{
		m_spSearchRoadLinkGrid->put_IsSelected(i, (i == lRow) ? VARIANT_TRUE : VARIANT_FALSE);
	}

	// 複数行の選択はできないようにする
	m_spSearchRoadLinkGrid->put_Row(lRow);

	// 選択した行に対応する道路リンクのフィーチャをメンバ変数に保持
	_variant_t va;
	m_spSearchRoadLinkGrid->get_RowData(lRow, &va);
	IFeaturePtr ipFeature = (IFeature*)va.punkVal;
	m_ipSelectedRoadLinkItem = ipFeature;
	
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/**
* @fn CAttrWarningSignDlg::CAttrWarningSignDlg()
* @brief コンストラクタ
*
* ブラシの初期化をしておきます。
*/
CAttrWarningSignDlg::CAttrWarningSignDlg()
	: ATTR_BASE_CLASS(WARNING_SIGN_MAP, IDD_WARNING_SIGN_DLG), m_lSearchDist(INIT_SEARCH_METER)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_WARNING_SIGN, CContainedWindow(_T("COMBO"), this, IDC_COMBO_WARNING_SIGN))
		(IDC_EDIT_LINK_ID, CContainedWindow(_T("EDIT"), this, IDC_EDIT_LINK_ID))
		(IDC_EDIT_PICTURE_PATH, CContainedWindow(_T("EDIT"), this, IDC_EDIT_PICTURE_PATH))
		(IDC_EDIT_SEARCH_METER, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SEARCH_METER))
		(IDC_BUTTON_UPDATE_LINK_ID, CContainedWindow(_T("BUTTON"), this, IDC_BUTTON_UPDATE_LINK_ID))
		(IDC_EDIT_SOURCE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE));
}

/** 
 * @fn HWND CAttrWarningSignDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrWarningSignDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrWarningSignDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrWarningSignDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @brief エラーチェックを行う
 *
 * @retval  TRUE  エラーなし
 * @retval  FALSE エラーあり
 */
BOOL CAttrWarningSignDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strMsg;

	for(auto& featureDef : *m_pFeatureDefList)
	{
		// 警戒標識レイヤでなければスキップ
		CString strTableName(featureDef.GetTableName());
		if(strTableName.CompareNoCase(warning_sign::kTableName) != 0)
			continue;

		CString strErr; // エラーメッセージ

		// 警戒標識種別のチェック
		CheckWarningSign(featureDef, strErr);

		// エラーメッセージがなければ次のフィーチャへ
		if(strErr.IsEmpty())
			continue;

		// エラーメッセージがあれば、表示用のメッセージに成形
		long lOID = -1;
		featureDef.GetFeature()->get_OID(&lOID);
		CString strTmpMsg;
		strTmpMsg.Format(AheLoadString(IDS_ERR_INVALID_ATTR), featureDef.GetTableName(), lOID);
		int nCurPos = 0;
		CString strToken(strErr.Tokenize(_T(","), nCurPos));
		while(-1 != nCurPos)
		{
			strTmpMsg += _T("\n") + strToken;
			strToken = strErr.Tokenize(_T(","), nCurPos);
		}
		strMsg += strTmpMsg + _T("\n");

		bRet = FALSE;
	}
	
	// エラーがあればメッセージを出力
	if(!bRet)
		AttrMessageBox(strMsg, AheLoadString(IDS_ERRCHK), MB_OK | MB_ICONWARNING, WARNLEVEL_HIGH);

	return bRet;
}

/**
 * @fn BOOL CAttrWarningSignDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
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
BOOL CAttrWarningSignDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// TOCから表示されている道路リンクレイヤを取得
	// 複数ある場合はTOCの中で一番上にあるものが対象
	m_ipRoadLinkFC = GetFeatureClass(road_link::kTableName);

	// レイヤを取得できたら対応する道路リンクを取得
	// できなかったらエラーメッセージを出す
	if(m_ipRoadLinkFC)
	{
		m_ipRoadLinkItem = GetRelatedLink(m_ipRoadLinkFC);
	}
	else
	{
		// フィーチャクラスは空にしておく
		m_ipRoadLinkItem = CTargetItem();
		AttrMessageBox(AheLoadString(IDS_ERR_NO_EXIST_ROAD_LINK), AheLoadString(IDS_ERR_FAILED_READ_LAYER), MB_OK | MB_TASKMODAL, WARNLEVEL_NONE);
	}
	
	// 警戒標識種別コンボボックスのリストを設定
	SettingWarningSignDropdownList();
	
	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_COMBO_WARNING_SIGN) );
	}
	
	// 対応リンクIDは手動で編集できないようにするため、DISABLEに
	SetDlgStatus( IDC_EDIT_LINK_ID, FALSE, TRUE );

	// 検索距離は常に編集可能とするため、ENABLEに
	SetDlgStatus(IDC_EDIT_SEARCH_METER, TRUE, TRUE);

	// 検索結果のグリッドの初期設定
	InitSearchRoadLinkGrid();

	// 検索距離のエディットボックスに初期値をセット
	m_lSearchDist = INIT_SEARCH_METER;
	SetDlgItemInt(IDC_EDIT_SEARCH_METER, m_lSearchDist);

	// 初期値(10m)で検索しておく
	SearchRoadLink();

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

/**
 * @fn void CAttrWarningSignDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrWarningSignDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ControlRelにフィールド名とテーブル名を登録

		// 警戒標識種別
		m_cControlRel.SetControl( IDC_COMBO_WARNING_SIGN, TYPE_COMBO, 0, 0, warning_sign::kWarningSign, 0, warning_sign::kTableName );

		// 対応リンクID
		m_cControlRel.SetControl( IDC_EDIT_LINK_ID, TYPE_EDITSTRING, 0, 0, warning_sign::kLinkID, 0, warning_sign::kTableName );

		// 走行画像ファイルパス
		m_cControlRel.SetControl( IDC_EDIT_PICTURE_PATH, TYPE_EDITSTRING, 0, 0, warning_sign::kPicturePath, 0, warning_sign::kTableName );

		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, warning_sign::kTableName );

		// 検索距離
		m_cControlRel.SetControl( IDC_EDIT_SEARCH_METER, TYPE_EDITSTRING, 0, 0 );

		// リンクID適用ボタン
		m_cControlRel.SetControl( IDC_BUTTON_UPDATE_LINK_ID, TYPE_BUTTON, 0, 0);
	}
}

/**
 * @brief 警戒標識種別コンボボックスのドロップダウンリストを作成する
 */
void CAttrWarningSignDlg::SettingWarningSignDropdownList()
{
	CComboBox cbWarningSign(GetDlgItem(IDC_COMBO_WARNING_SIGN));
	cbWarningSign.ResetContent();
	
	CFieldDef* pWarningSignDef = GetCurrentFeatureDef()->GetFieldDef( warning_sign::kTableName, warning_sign::kWarningSign );
	auto mapWarningSignDomain = pWarningSignDef->GetDomain();
	long lWarningSign = pWarningSignDef->m_vaValue.lVal;
	int nCounter = 0;
	int nUndefinedItemIdx = -1;
	bool bSetSel = false;
	for(const auto& domain : *mapWarningSignDomain)
	{
		// "コード値 : ドメイン"のフォーマットに沿うよう成形し、
		// ドロップダウンリストに追加する
		// なお、コード値が0の場合は"x ： 未定義"とする
		CString strItem;
		if(domain.code.lVal == warning_sign::warning_sign_class::kNone)
		{
			strItem = AheLoadString(IDS_COMBO_ITEM_FOR_UNDEFINED_VALUE);
			nUndefinedItemIdx = nCounter;
		}
		else
		{
			strItem = ToString(domain.code.lVal) + CString(_T(" : ")) + domain.code_discription;
		}

		cbWarningSign.InsertString( nCounter, strItem );
		cbWarningSign.SetItemData( nCounter, domain.code.lVal );
		if(lWarningSign == domain.code.lVal)
		{
			cbWarningSign.SetCurSel(nCounter);
			bSetSel = true;
		}
		++nCounter;
	}

	// 値が定義されていない場合、"x ： 未定義"をセット
	if(!bSetSel)
		cbWarningSign.SetCurSel(nUndefinedItemIdx);
}

/**
 * @brief 指定したレイヤのフィーチャクラスを取得する
 * @note  TOCの一番上にある表示状態のレイヤが対象
 * @note  グループレイヤの表示状態も確認
 * @param strLayerName	[in]	レイヤ名
 * @return TOCの一番上にある表示状態のフィーチャクラス
 */
IFeatureClassPtr CAttrWarningSignDlg::GetFeatureClass(const CString& strLayerName)
{
	CMap cMap(m_cArcHelper.GetFocusMap());

	const auto& listLayer = cMap.FindLayers(nullptr, nullptr, strLayerName);
	if(listLayer.empty())
		return nullptr;

	for(auto& it = listLayer.begin(); it != listLayer.end(); ++it)
	{
		// 属しているグループレイヤが非表示の場合は対象外
		ICompositeLayerPtr ipCompositeLayer = cMap.GetParentCompositeLayer(it->m_T);
		if(ipCompositeLayer)
		{
			VARIANT_BOOL bGroupVisible = VARIANT_FALSE;
			((ILayerPtr)ipCompositeLayer)->get_Visible(&bGroupVisible);
			if(!bGroupVisible)
				continue;
		}

		// 表示状態のものを返す
		VARIANT_BOOL bVisible = VARIANT_FALSE;
		it->m_T->get_Visible(&bVisible);
		if(bVisible)
		{
			return AheGetFeatureClass(it->m_T);
		}
	}

	return nullptr;
}

/**
 * @brief フィーチャクラスから指定したOIDのフィーチャを取得する
 * @param ipFeatureClass	[in]	フィーチャクラス
 * @param lOID	[in]	取得したいフィーチャのOID
 * @return 指定したOIDのフィーチャ
 */
CTargetItem CAttrWarningSignDlg::GetTargetItem(const IFeatureClassPtr& ipFeatureClass, long lOID)
{
	if(!ipFeatureClass)
		return CTargetItem();

	// 指定された値と同じOIDを持つレコードを取得
	IFeaturePtr ipFeature;
	return (ipFeatureClass->GetFeature(lOID, &ipFeature) == S_OK && ipFeature) ? CTargetItem(ipFeature) : CTargetItem();
}

/**
 * @brief 警戒標識に対応する道路リンクのフィーチャを取得する
 * @param ipRoadLinkFC	[in]	道路リンクのフィーチャクラス
 * @return 対応する道路リンクのフィーチャ
 */
CTargetItem CAttrWarningSignDlg::GetRelatedLink(const IFeatureClassPtr& ipRoadLinkFC)
{
	if(!ipRoadLinkFC)
		return CTargetItem();

	CComVariant vaLinkID = GetCurrentFeatureDef()->GetFieldDef(warning_sign::kTableName, warning_sign::kLinkID)->m_vaValue;

	// LINK_IDと同じOIDの道路リンクフィーチャを返す
	// LINK_IDがNULLなら空のフィーチャを返す
	return (vaLinkID.vt == VT_I4) ? GetTargetItem(ipRoadLinkFC, vaLinkID.lVal) : CTargetItem();
}

/**
* @brief 選択中の警戒標識から指定した距離以内に存在する道路リンクを探す
* @note  TOCの道路リンクレイヤの状態や、指定距離のチェックを行い、問題があれば検索はしない
*/
void CAttrWarningSignDlg::ExecuteSearchRoadLink()
{
	// TOCに道路リンクが存在しない、またはすべて非表示になっている場合、
	// エラーメッセージを出して検索させない
	if(!m_ipRoadLinkFC)
	{
		AttrMessageBox(AheLoadString(IDS_ERR_NO_EXIST_ROAD_LINK), AheLoadString(IDS_ERR_FAILED_READ_LAYER), MB_OK | MB_TASKMODAL, WARNLEVEL_NONE);
		return;
	}

	// 検索前にテキストボックスの距離に問題がないかチェック
	// 問題がある場合、エラーメッセージを出して検索させない
	CString strErrMsg = CheckSearchMeter();
	if(!strErrMsg.IsEmpty())
	{
		AheMessageBox(nullptr, strErrMsg, AheLoadString(IDS_ERR_SEARCH_METER_INVALID), MB_OK | MB_ICONWARNING | MB_TASKMODAL, WARNLEVEL_NONE);
		return;
	}

	// 道路リンクを検索し、グリッドを更新
	SearchRoadLink();
}

/**
* @brief 道路リンク検索結果グリッドの初期設定
*/
void CAttrWarningSignDlg::InitSearchRoadLinkGrid()
{
	// グリッドコントロール作成
	CAxWindow axWnd(GetDlgItem(IDC_GRID_ROAD_LINK));
	axWnd.QueryControl(&m_spSearchRoadLinkGrid);

	// ヘッダ作成
	grid_util::FormatString(m_spSearchRoadLinkGrid, SEARCH_ROAD_LINK_GRID_HEADER);
	// 列幅調整
	grid_util::AutoRowColSize(m_spSearchRoadLinkGrid);

	// 選択中のフィーチャはここで空にしておく
	m_ipSelectedRoadLinkItem = CTargetItem();
}

/**
* @brief 道路リンク検索結果グリッドの更新
* @param resultMap [in] 道路リンク検索結果のマップ
*/
void CAttrWarningSignDlg::UpdateSearchRoadLinkGrid(const ResultMap& resultMap)
{
	// グリッドのサイズ設定(+1はヘッダ分)
	m_spSearchRoadLinkGrid->PutRows(resultMap.size() + 1);

	long lRow = 1;
	for(const auto& record : resultMap)
	{
		// OID
		m_spSearchRoadLinkGrid->put_TextMatrix(lRow, grid_cols::kOID, CComBSTR(ToString(record.second.GetOID())));
		// 距離(m)
		m_spSearchRoadLinkGrid->put_TextMatrix(lRow, grid_cols::kDist, CComBSTR(ToString(record.first, _T("%.8lf"))));

		// グリッドに対応するフィーチャをセットしておく
		IFeaturePtr ipFeature = record.second.GetFeature();
		_variant_t va = (IUnknown*)ipFeature;
		m_spSearchRoadLinkGrid->put_RowData(lRow, va);

		++lRow;
	}

	// 列幅調整
	grid_util::AutoRowColSize(m_spSearchRoadLinkGrid);
}

/**
* @brief 警戒標識から指定した距離以内に存在する道路リンクを検索
* @note 距離の単位はメートル
*/
void CAttrWarningSignDlg::SearchRoadLink()
{
	IGeometryPtr ipWarningSignGeom = AheGetShape((IFeaturePtr)GetCurrentFeatureDef()->GetFeature());
	if(!ipWarningSignGeom)
		return;

	ITopologicalOperatorPtr ipTopo(ipWarningSignGeom);
	if(!ipTopo)
		return;

	// 検索範囲を度に変換
	IUnitConverterPtr ipUnitConv(CLSID_UnitConverter);
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits(m_lSearchDist, esriMeters, esriDecimalDegrees, &dConvRange);

	// バッファ作成
	IGeometryPtr ipBuff;
	if(FAILED(ipTopo->Buffer(dConvRange, &ipBuff)) || !ipBuff)
	{
		AttrMessageBox(AheLoadString(IDS_FAILED_TO_CREATE_BUFFER), AheLoadString(IDS_ERROR), MB_OK | MB_TASKMODAL, WARNLEVEL_MIDDLE);
		return;
	}

	ipBuff->putref_SpatialReference(AheGetSpatialReference(m_ipRoadLinkFC));

	// 検索
	CTargetItems cItems;
	AheSelectByShapeFromFeatureClass(ipBuff, cItems, m_ipRoadLinkFC);

	// 警戒標識から道路リンクまでの最短距離を求める
	ResultMap resultMap;
	for(const auto& link : cItems)
	{
		// 警戒標識に最も近い道路リンク上のポイントを取得
		IProximityOperatorPtr ipProp(link.GetShape());
		IPointPtr ipWarningSignPoint(ipWarningSignGeom);
		IPointPtr ipNearestPoint;
		ipProp->ReturnNearestPoint(ipWarningSignPoint, esriNoExtension, &ipNearestPoint);
		if(!ipNearestPoint)
			continue;

		ipNearestPoint->putref_SpatialReference(AheGetSpatialReference(ipWarningSignPoint));
		double dDist = AheGetMeterLength(ipWarningSignPoint, ipNearestPoint);
		resultMap.emplace(dDist, link);
	}

	// グリッドを更新
	UpdateSearchRoadLinkGrid(resultMap);
}

/**
* @brief 道路リンク検索時の指定距離をチェック
* @note 問題がなければそのまま検索距離として保持
*
* @return エラーメッセージ
*/
CString CAttrWarningSignDlg::CheckSearchMeter()
{
	CString strMeter;
	GetDlgItemText(IDC_EDIT_SEARCH_METER, strMeter);

	// 空欄でないか
	if(strMeter.IsEmpty())
		return AheLoadString(IDS_ERR_SEARCH_METER_EMPTY);

	// 文字列→数値変換
	long lSearchDist = _tstol(strMeter);

	// マイナス値でないか
	if(lSearchDist < 0)
		return AheLoadString(IDS_ERR_SEARCH_METER_MINUS);

	// 上限値を超えていないか
	if(lSearchDist > MAX_SEARCH_METER)
		return format(AheLoadString(IDS_ERR_SEARCH_METER_OVER_LIMIT), MAX_SEARCH_METER);

	// 問題なければ検索距離としてセットし、空のCStringを返す
	m_lSearchDist = lSearchDist;
	return CString();
}

/**
 * @brief 警戒標識種別のチェック
 * @param	featureDef	[in]		チェック対象のフィーチャ
 * @param	strErr		[out]		エラーメッセージ
 *
 * @retval  TRUE  フィールドサイズを超えていない
 * @retval  FALSE フィールドサイズを超えている
 */
BOOL CAttrWarningSignDlg::CheckWarningSign(CFeatureDef& cFeatureDef, CString& strErr)
{
	CFieldDef* pWarningSignDef = cFeatureDef.GetFieldDef(warning_sign::kTableName, warning_sign::kWarningSign);
	auto mapWarningSignDomain = pWarningSignDef->GetDomain();
	long lWarningSign = pWarningSignDef->m_vaValue.lVal;
	auto itr = find_if(mapWarningSignDomain->begin(), mapWarningSignDomain->end(),
		[&lWarningSign](const CODEVALUE_DOMAIN& domain){ return domain.code.lVal == lWarningSign; });
	if(lWarningSign == warning_sign::warning_sign_class::kNone || itr == mapWarningSignDomain->end())
	{
		strErr += format(AheLoadString(IDS_ERR_WARNING_SIGN_VALUE_NOT_EXIST), warning_sign::kWarningSign, lWarningSign);
		return FALSE;
	}
	
	return TRUE;
}