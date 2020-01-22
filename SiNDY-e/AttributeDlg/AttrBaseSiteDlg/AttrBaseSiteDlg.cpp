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

// AttrBaseSiteDlg.cpp: CAttrBaseSiteDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseSiteDlg.h"

/// ウィンドウマネージャ配置マップ（中縮背景ポリゴン用）
BEGIN_WINDOW_MAP(BASE_SITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC4BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC4BGCLASS_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU4)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU4)
				RCTOFIT(IDC_BUTTON_SEARCH4)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC3BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC3BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME3)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU3)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU3)
				RCTOFIT(IDC_BUTTON_SEARCH3)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC2BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC2BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU2)
				RCTOFIT(IDC_BUTTON_SEARCH2)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC1BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC1BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU1)
				RCTOFIT(IDC_BUTTON_SEARCH1)
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
 * @retval CAttrBaseSiteDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseSiteDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseSiteDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseSiteDlg::CAttrBaseSiteDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrBaseSiteDlg::CAttrBaseSiteDlg()
	: ATTR_BASE_CLASS(BASE_SITE_MAP, IDD_ATTRBASESITEDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_SC1BGCLASS_C ), 
		m_wnd1( _T("BUTTON"), this, IDC_BUTTON_SAME1 ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_SC2BGCLASS_C ), 
		m_wnd3( _T("BUTTON"), this, IDC_BUTTON_SAME2 ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_SC3BGCLASS_C ), 
		m_wnd5( _T("BUTTON"), this, IDC_BUTTON_SAME3 ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_SC4BGCLASS_C ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_GENKOU1 ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_GENKOU2 ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_GENKOU3 ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_GENKOU4 ),
		m_wnd11( _T("BUTTON"), this, IDC_BUTTON_SEARCH1 ), 
		m_wnd12( _T("BUTTON"), this, IDC_BUTTON_SEARCH2 ), 
		m_wnd13( _T("BUTTON"), this, IDC_BUTTON_SEARCH3 ), 
		m_wnd14( _T("BUTTON"), this, IDC_BUTTON_SEARCH4 ),
		m_wnd15( _T("EDIT"), this, IDC_EDIT_SOURCE )	
{
}

/** 
 * @fn HWND CAttrBaseSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBaseSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseSiteDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBaseSiteDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBaseSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBaseSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU1);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU2);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU3);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU4);

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_EDIT_GENKOU4) );
	}

	return bRet;
}

/**
 * @fn void CAttrBaseSiteDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBaseSiteDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 中縮背景ポリゴン用

		// S4の背景種別
		m_cControlRel.SetControl( IDC_COMBO_SC4BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC4BGCLASS_C"),	NULL, m_strClassName );
		// S4の原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU4,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH4,		TYPE_BUTTON,	0,	0 );
		// S3の背景種別
		m_cControlRel.SetControl( IDC_COMBO_SC3BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC3BGCLASS_C"),	NULL, m_strClassName );
		// 以上同じにボタン
		m_cControlRel.SetControl( IDC_BUTTON_SAME3,			TYPE_BUTTON,	0,	0 );
		// S3の原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU3,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH3,		TYPE_BUTTON,	0,	0 );
		// S2の背景種別
		m_cControlRel.SetControl( IDC_COMBO_SC2BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC2BGCLASS_C"),	NULL, m_strClassName );
		// 以上同じにボタン
		m_cControlRel.SetControl( IDC_BUTTON_SAME2,			TYPE_BUTTON,	0,	0 );
		// S2の原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU2,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH2,		TYPE_BUTTON,	0,	0 );
		// S1の背景種別
		m_cControlRel.SetControl( IDC_COMBO_SC1BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC1BGCLASS_C"),	NULL, m_strClassName );
		// 以上同じにボタン
		m_cControlRel.SetControl( IDC_BUTTON_SAME1,			TYPE_BUTTON,	0,	0 );
		// S1の原稿番号入力
		m_cControlRel.SetControl( IDC_EDIT_GENKOU1,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH1,		TYPE_BUTTON,	0,	0 );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, m_strClassName );
	}
}


/**
 * @fn BOOL CAttrBaseSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief 背景種別設定
 *
 * 背景種別EditBoxに入力された背景種別原稿番号から背景種別コードを取得し、背景種別ComboBoxに設定する
 *
 * @param nCodeID			[in]	背景種別原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	背景種別ComboBoxのコードID
 *
 * @return 背景種別をセットすることができたらTRUE,できなかったらFALSE
 */
BOOL CAttrBaseSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
{
	// 原稿番号コードをエディットボックスから取得
	CEdit edit( GetDlgItem( nCodeID ) );
	CString strGenkouCode;
	edit.GetWindowText( strGenkouCode );

	// 原稿番号を半角に変換
	CString	strGenkou = AheConvertToGenkouNoStr(strGenkouCode);	// 半角,英字OK,マイナスNO

	// BACKGROUND_CLASSからコード取得
	long lSearchClassID = -1;
	if (!strGenkou.IsEmpty())
		lSearchClassID = GetClassByMSNO( strGenkou );

	if (lSearchClassID >= 0)
	{
		// 背景種別コンボボックス取得
		CComboBox	cWAnnoClass( GetDlgItem(nClassID) );
		LONG		lClassNum = cWAnnoClass.GetCount();
		LONG		lComboBoxItemNum = 0;
		DWORD		dwComboBoxValue = 0;
		LONG		lComboBoxValue = 0;
		for (lComboBoxItemNum ; lComboBoxItemNum < lClassNum ; lComboBoxItemNum++)
		{
			dwComboBoxValue = cWAnnoClass.GetItemData(lComboBoxItemNum);
			lComboBoxValue = (INT)dwComboBoxValue;
			if (lComboBoxValue == lSearchClassID)
				break;
		}
		cWAnnoClass.SetCurSel(lComboBoxItemNum);

		SetDlgItemText(nCodeID, strGenkou);			// 原稿番号を設定

		m_cControlRel.ControlChanged( nClassID );
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}
	else
	{
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		ShowGenkouByBackgroundClass(nCodeID);			// 原稿番号を設定
		return FALSE;
	}
	return TRUE;
}

/**
 * @fn void CAttrBaseSiteDlg::ShowGenkouByBackgroundClass()
 * @brief 現在選択中の背景種別の原稿番号を、原稿番号フィールドに表示
 */
void CAttrBaseSiteDlg::ShowGenkouByBackgroundClass(INT nCodeID)
{
	if( nCodeID == IDC_COMBO_SC1BGCLASS_C)
		nCodeID = IDC_EDIT_GENKOU1;
	else if( nCodeID == IDC_COMBO_SC2BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU2;
	else if( nCodeID == IDC_COMBO_SC3BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU3;
	else if( nCodeID == IDC_COMBO_SC4BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU4;

	CWindow	cWGenkou(GetDlgItem(nCodeID));			// 原稿番号テキストボックス
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass,nCodeID);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrBaseSiteDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief 現在選択中の背景種別IDを取得
 *
 * @param	lBackgroundClass		[o]		選択中の背景種別ID		
 */
void CAttrBaseSiteDlg::GetBackgroundClass(LONG *lBackgroundClass, INT nCodeID)
{
	INT cClassID = 0;

	if( nCodeID == IDC_EDIT_GENKOU1 )
		cClassID = IDC_COMBO_SC1BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU2 )
		cClassID = IDC_COMBO_SC2BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU3 )
		cClassID = IDC_COMBO_SC3BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU4 )
		cClassID = IDC_COMBO_SC4BGCLASS_C;

	CComboBox	cWAnnoClass(GetDlgItem(cClassID));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn void CAttrBaseSiteDlg::CopyValue(INT nOrgID, INT nCpyID)
 * @brief 下のスケールに原稿番号をコピーする
 *
 * @param	nOrgID		[in]		コピー元の背景種別コントロールID		
 * @param	nCpyID		[in]		コピー先の背景種別コントロールID		
 */
void CAttrBaseSiteDlg::CopyValue(INT nOrgID, INT nCpyID)
{
	CControlDef* pControlDef1 = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nOrgID ) );
	CComVariant vaValue;
	pControlDef1->GetControlValue(vaValue);

	CControlDef* pControlDef2 = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nCpyID ) );
	pControlDef2->SetValue(vaValue);
	pControlDef2->SetComboBox(vaValue.intVal);
	m_cControlRel.ControlChanged( nCpyID );	

	// コピーし終えたら原稿番号も変更する
	if( nCpyID == IDC_COMBO_SC1BGCLASS_C)
		nCpyID = IDC_EDIT_GENKOU1;
	else if( nCpyID == IDC_COMBO_SC2BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU2;
	else if( nCpyID == IDC_COMBO_SC3BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU3;
	else if( nCpyID == IDC_COMBO_SC4BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU4;

	ShowGenkouByBackgroundClass(nCpyID);			// 原稿番号を設定
}


/**
 * @fn BOOL CAttrBaseSiteDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrBaseSiteDlg::CheckReturnKeyItem(INT nClassID)
{
	// 原稿番号エディットボックス or 検索ボタン
	if ((nClassID == IDC_EDIT_GENKOU1) || (nClassID == IDC_BUTTON_SEARCH1) ||
		(nClassID == IDC_EDIT_GENKOU2) || (nClassID == IDC_BUTTON_SEARCH2) ||
		(nClassID == IDC_EDIT_GENKOU3) || (nClassID == IDC_BUTTON_SEARCH3) ||
		(nClassID == IDC_EDIT_GENKOU4) || (nClassID == IDC_BUTTON_SEARCH4))
	{
		// どのスケールでエンターを押したかを再設定しなおす
		INT cCodeID = 0;

		if( nClassID == IDC_EDIT_GENKOU1 || nClassID == IDC_BUTTON_SEARCH1)
		{
			cCodeID = IDC_EDIT_GENKOU1;
			nClassID = IDC_COMBO_SC1BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU2 || nClassID == IDC_BUTTON_SEARCH2)
		{
			cCodeID = IDC_EDIT_GENKOU2;
			nClassID = IDC_COMBO_SC2BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU3 || nClassID == IDC_BUTTON_SEARCH3)
		{
			cCodeID = IDC_EDIT_GENKOU3;
			nClassID = IDC_COMBO_SC3BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU4 || nClassID == IDC_BUTTON_SEARCH4)
		{
			cCodeID = IDC_EDIT_GENKOU4;
			nClassID = IDC_COMBO_SC4BGCLASS_C;
		}

		if( !SetBackgroundClassCode(cCodeID, nClassID) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrBaseRailwayDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESCキーが押された時の動作
 *
 * @param	ClassID	[i]		ESCキーが押された時のItemID
 *
 * @return	TRUE : ESCキーとしての動作(適用処理)可 , FALSE : ESCキーとしての動作不可 
 */
BOOL CAttrBaseSiteDlg::CheckEscKeyItem(INT nClassID)
{
	// 原稿番号エディットボックス or 検索ボタン
	if ((nClassID == IDC_EDIT_GENKOU1) || (nClassID == IDC_BUTTON_SEARCH1) ||
		(nClassID == IDC_EDIT_GENKOU2) || (nClassID == IDC_BUTTON_SEARCH2) ||
		(nClassID == IDC_EDIT_GENKOU3) || (nClassID == IDC_BUTTON_SEARCH3) ||
		(nClassID == IDC_EDIT_GENKOU4) || (nClassID == IDC_BUTTON_SEARCH4))
	{
		// どのスケールでエンターを押したかを再設定しなおす
		INT cCodeID = 0;

		if( nClassID == IDC_EDIT_GENKOU1 || nClassID == IDC_BUTTON_SEARCH1)
		{
			cCodeID = IDC_EDIT_GENKOU1;
			nClassID = IDC_COMBO_SC1BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU2 || nClassID == IDC_BUTTON_SEARCH2)
		{
			cCodeID = IDC_EDIT_GENKOU2;
			nClassID = IDC_COMBO_SC2BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU3 || nClassID == IDC_BUTTON_SEARCH3)
		{
			cCodeID = IDC_EDIT_GENKOU3;
			nClassID = IDC_COMBO_SC3BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU4 || nClassID == IDC_BUTTON_SEARCH4)
		{
			cCodeID = IDC_EDIT_GENKOU4;
			nClassID = IDC_COMBO_SC4BGCLASS_C;
		}

		if( !SetBackgroundClassCode(cCodeID, nClassID) )
			return FALSE;
	}
	return TRUE;
}

