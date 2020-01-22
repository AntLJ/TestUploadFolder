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

// AttrCDSCommonDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrCdsCommonDlg.h"
#include "DlgMap.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCdsCommonDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCdsCommonDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCdsCommonDlg::CAttrCdsCommonDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCdsCommonDlg::CAttrCdsCommonDlg() 
	: ATTR_BASE_CLASS(CITY_SITE_MAP, IDD_CDS_COMMON_DLG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_CLASS, CContainedWindow( _T("COMBO"), this, IDC_COMBO_CLASS ) )
		( IDC_EDIT_MSNO, CContainedWindow( _T("EDIT"), this, IDC_EDIT_MSNO ) )
		( IDC_BUTTON_SEARCH_MSNO, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_SEARCH_MSNO ) )
		( IDC_CHECK_UNDERGROUND, CContainedWindow( _T("CHECK"), this, IDC_CHECK_UNDERGROUND ) )
		( IDC_CHECK_CORRIDOR, CContainedWindow( _T("CHECK"), this, IDC_CHECK_CORRIDOR ) )
		( IDC_CHECK_NOWALL, CContainedWindow( _T("CHECK"), this, IDC_CHECK_NOWALL ) )
		( IDC_EDIT_BLDNAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_BLDNAME ) )
		( IDC_EDIT_BLDADDR, CContainedWindow( _T("EDIT"), this, IDC_EDIT_BLDADDR ) )
		( IDC_EDIT_GEOID, CContainedWindow( _T("EDIT"), this, IDC_EDIT_GEOID ) )
		( IDC_EDIT_NAMESTRING1, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAMESTRING1 ) )
		( IDC_EDIT_NAMESTRING2, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAMESTRING2 ) )
		( IDC_COMBO_DISPTYPE, CContainedWindow( _T("COMBO"), this, IDC_COMBO_DISPTYPE ) )
		( IDC_CHECK_HIMAWARI, CContainedWindow( _T("CHECK"), this, IDC_CHECK_HIMAWARI ) );

	// 原稿番号入力除外テーブル一覧設定
	m_msNoExceptTblList = boost::assign::list_of
		( city_railway_gs::kTableName )
		( city_station_gs::kTableName );
}

/** 
 * @fn HWND CAttrCdsCommonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCdsCommonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	// レイヤに応じて配置マップを変える
	// TODO: どこかで列挙型を定義して判定した方が良い
	if( 0 == m_strClassName.CompareNoCase( city_site_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_SITE_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_LINE_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_RAILWAY_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(BUILDING_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_ANNO_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_STATION_MAP);
	}

	HWND hWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);

	// レイヤに応じて、不要なコントロールの非表示とキャプションの変更を行う
	// CITY_SITE_GS
	if( 0 == m_strClassName.CompareNoCase(city_site_gs::kTableName) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_BG));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_BG));
	}

	// CITY_LINE_GS
	if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_LINE_BG));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_LINE_BG));
	}
	
	// CITY_RAILWAY_GS
	if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_RAILWAY_CLASS));
	}
	else // CITY_RAILWAY_GS以外
	{
		::ShowWindow( GetDlgItem( IDC_CHECK_UNDERGROUND ), SW_HIDE );
	}

	// BUILDING_GS
	if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_BLDG_CLASS));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_BLDG_CLASS));
	}
	else // BUILDING_GS以外
	{
		::ShowWindow( GetDlgItem( IDC_CHECK_CORRIDOR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_CHECK_NOWALL ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_BLDNAME ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_BLDNAME ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_BLDADDR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_BLDADDR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_GEOID ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_GEOID ), SW_HIDE );
	}

	// CITY_ANNOTATION_GS
	if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_ANNO_CLASS));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_ANNO_CLASS));
	}
	else // CITY_ANNOTATION_GS以外
	{
		::ShowWindow( GetDlgItem( IDC_STATIC_NAMESTRING1 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_NAMESTRING1 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_NAMESTRING2 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_NAMESTRING2 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_DISPTYPE ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_COMBO_DISPTYPE ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_CHECK_HIMAWARI ), SW_HIDE );
	}

	// CITY_STATION_GS
	if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_STATION_CLASS));
	}

	// 原稿番号入力関連
	auto findTargetTbl = std::find_if( 
				m_msNoExceptTblList.begin(),
				m_msNoExceptTblList.end(),
				[this](const CString& tableName)
					{ return 0 == tableName.CompareNoCase(this->m_strClassName); } );
	// 原稿番号入力対象外のテーブルであれば、コントロールを隠す
	if( m_msNoExceptTblList.end() != findTargetTbl )
	{
		::ShowWindow( GetDlgItem( IDC_STATIC_MSNO ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_MSNO ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCH_MSNO ), SW_HIDE );
	}
	
	return hWnd;
}

/**
 * @fn void CAttrCdsCommonDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCdsCommonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCdsCommonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCdsCommonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowMSNOByClass();
	UpdateDispTypeList();
	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_COMBO_CLASS) );
	}

	// GEOSPACE_IDは編集されないのでDISABLEに
	SetDlgStatus( IDC_EDIT_GEOID, FALSE, TRUE );

	return bRet;
}

/**
 * @fn void CAttrCdsCommonDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCdsCommonDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( !m_cControlRel.empty() )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		if( 0 == m_strClassName.CompareNoCase( city_site_gs::kTableName ) )
		{
			// 背景種別
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_site_gs::kBgClass, 0, city_site_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
		{
			// 背景ライン種別
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_line_gs::kBgClass, 0, city_line_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
		{
			// 鉄道種別, 地下フラグ
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_railway_gs::kRailwayClass, 0, city_railway_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_UNDERGROUND, TYPE_CHECKBOX, 0, 0, city_railway_gs::kUnderground, 0, city_railway_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
		{
			// 建物種別, 渡り廊下フラグ, 無壁フラグ, 建物名称, 建物住所, GEOSPACE_ID
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, building_gs::kBldClass, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_CORRIDOR, TYPE_CHECKBOX, 0, 0, building_gs::kCorridor, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_NOWALL, TYPE_CHECKBOX, 0, 0, building_gs::kNoWall, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_BLDNAME, TYPE_EDITSTRING, 0, 0, building_gs::kBldName, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_BLDADDR, TYPE_EDITSTRING, 0, 0, building_gs::kBldAddr, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_GEOID, TYPE_EDITSTRING, 0, 0, building_gs::kGeospaceID, 0, building_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
		{
			// 注記種別, 注記文字列１,２, 注記記号, ひまわりフラグ
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_annotation_gs::kAnnoClass, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_NAMESTRING1, TYPE_EDITSTRING, 0, 0, city_annotation_gs::kNameString1, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_NAMESTRING2, TYPE_EDITSTRING, 0, 0, city_annotation_gs::kNameString2, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_COMBO_DISPTYPE, TYPE_COMBO, 0, 0, city_annotation_gs::kDispType, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_HIMAWARI, TYPE_CHECKBOX, 0, 0, city_annotation_gs::kHimawari, 0, city_annotation_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
		{
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_station_gs::kStationClass, 0 , city_station_gs::kTableName );
		}

		// 原稿番号入力
		auto findTargetTbl = std::find_if( 
					m_msNoExceptTblList.begin(),
					m_msNoExceptTblList.end(),
					[this](const CString& tableName)
						{ return 0 == tableName.CompareNoCase(this->m_strClassName); } );
		// 原稿番号入力対象のテーブルの場合、コントロールを設定
		if( m_msNoExceptTblList.end() == findTargetTbl )
		{
			m_cControlRel.SetControl( IDC_EDIT_MSNO,	TYPE_EDIT,	0,	0 );
			m_cControlRel.SetControl( IDC_BUTTON_SEARCH_MSNO,	TYPE_BUTTON,	0,	0 );
		}

		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, m_strClassName );
	}
}

/**
 * @fn BOOL CAttrCdsCommonDlg::SetClassByMSNO(INT nCodeID, INT nClassID)
 * @brief 種別コードをコンボボックスに設定する
 *
 * 原稿番号EditBoxに入力された原稿番号から各種別コードを取得し、各種別コードComboBoxに設定する
 *
 * @param nCodeID			[in]	原稿番号が入っているEditBoxのコードID
 * @param nClassID			[in]	指定した種別のComboBoxのコードID
 *
 * @return 種別コードをセットすることができたらTRUE,できなかったらFALSE
 */
BOOL CAttrCdsCommonDlg::SetClassByMSNO(INT nCodeID, INT nClassID)
{
	CEdit edit( GetDlgItem( nCodeID ) );
	CString strMSNO;
	edit.GetWindowText( strMSNO );

	// 原稿番号を半角に変換
	strMSNO = AheConvertToGenkouNoStr( strMSNO );

	// 原稿番号から種別コードを取得
	LONG lSearchClassID = -1;
	if( !strMSNO.IsEmpty() )
		lSearchClassID = GetClassByMSNO( strMSNO );

	if( lSearchClassID >= 0 )
	{
		// 種別コンボボックスにセット
		SetComboBoxSel(nClassID, lSearchClassID);

		// 原稿番号をセット
		SetDlgItemText(nCodeID, strMSNO);

		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}
	else
	{
		// 原稿番号を元に戻す
		ShowMSNOByClass();
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
		return FALSE;
	}

	return TRUE;
}

/**
 * @fn void CAttrCdsCommonDlg::ShowMSNOByClass()
 * @brief 現在選択中の種別コードの原稿番号を、原稿番号フィールドに表示
 */
void CAttrCdsCommonDlg::ShowMSNOByClass()
{
	CWindow	cWMsno(GetDlgItem(IDC_EDIT_MSNO));	// 原稿番号テキストボックス
	LONG lClassID = -1;
	GetClassID( &lClassID );
	cWMsno.SetWindowText( GetMSNOByClass( lClassID ) );
	return;
}

/**
 * @fn LONG CAttrCdsCommonDlg::GetClassID(LONG *lClassID)
 * @brief 現在選択中の種別IDを取得
 *
 * @param	lClassID	[out]		選択中の種別のID
 */
void CAttrCdsCommonDlg::GetClassID(LONG *lClassID)
{
	CComboBox	cWComboClass( GetDlgItem( IDC_COMBO_CLASS ) );
	LONG		lSelectIndex = cWComboClass.GetCurSel();
	*lClassID = (LONG)cWComboClass.GetItemData(lSelectIndex);
}

/**
 * @brief リターンキーが押された時の動作
 *
 * @param	nClassID	[in]		リターンキーが押された時のItemID
 *
 * @retval  TRUE  リターンキーとしての動作(適用処理)可
 * @retval  FALSE リターンキーとしての動作不可 
 */
BOOL CAttrCdsCommonDlg::CheckReturnKeyItem(INT nClassID)
{
	BOOL bIsOK = TRUE; 
	if ((nClassID == IDC_EDIT_MSNO) || (nClassID == IDC_BUTTON_SEARCH_MSNO)) {		// 原稿番号エディットボックス or 検索ボタン
		if( !SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS) )
			return FALSE;

		// 注記記号を更新
		UpdateDispTypeList();
	}
	else
	{
		bIsOK = ErrorCheck();
		if( bIsOK && (nClassID == IDC_EDIT_NAMESTRING1 || nClassID == IDC_EDIT_NAMESTRING2) )
			AutoUpdateStringNum( nClassID );
	}
	return bIsOK;
}

/**
 * @brief GEOSPACE_IDが変更された時の動作
 *
 * @return なし
 */
void CAttrCdsCommonDlg::SetGeospaceID()
{
	// GEOSPACE_IDをエディットボックスから取得
	CEdit cEdit( GetDlgItem( IDC_EDIT_GEOID ) );
	CString strEdit;
	cEdit.GetWindowText( strEdit );

	// GEOSPACE_IDを半角に変換(原稿番号の変換ロジック使用)
	// ひらがな等は取り除かれる ex. 123あいうabcDEF → 123ABCDEF
	CString strGeoID;
	strGeoID = AheConvertToGenkouNoStr( strEdit );	// 半角,英字OK,マイナスNO

	// 変換したGEOSPACE_IDをセット
	cEdit.SetWindowText( strEdit );
}

/**
 * @brief 原稿番号と種別の対応表を作成する
 *
 * @return なし
 */
void CAttrCdsCommonDlg::LoadBackgroundRule()
{
	// CITY_ANNOTATION_GSの場合は、AnnotationClassTable.hから
	// それ以外は、BackgroundClassTable.hから対応表を作成する
	if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
#define ANNO_MAP( msno, code, domain, disp, layer ) { BACKGROUND_RULE br = {0}; \
	                                                  br.lClass = code; \
 	                                                  br.strClass_Name = domain; \
 	                                                  br.strMS_NO = msno; \
 									                  m_BackgroundRule.emplace_back(br); \
                                                      m_mapAnnoDispType[code] = layer::disp_type::disp; }
#include "AnnotationClassTable.h"
#undef ANNO_MAP
	}
	else
	{
#define BG_MAP( msno, code, domain, layer ) if( 0 == m_strClassName.CompareNoCase(layer::kTableName) ) \
	                                          { BACKGROUND_RULE br = {0}; \
	                                            br.lClass = code; \
	                                            br.strClass_Name = domain; \
	                                            br.strMS_NO = msno; \
									            m_BackgroundRule.emplace_back(br); }
#include "BackgroundClassTable.h"
#undef BG_MAP
	}
}

/**
 * @brief エラーチェックを行う
 *
 * @retval  TRUE  エラーなし
 * @retval  FALSE エラーあり
 */
BOOL CAttrCdsCommonDlg::ErrorCheck()
{
	BOOL bRet = TRUE; // 返り値
	CString strMsg; // ダイアログに表示するメッセージ

	// 選択した複数のフィーチャに対してエラーチェック
	for( auto& featureDef : *m_pFeatureDefList )
	{
		CString strErr; // エラーメッセージ

		// 注記文字列のチェック
		CheckAnnoNameString( featureDef, strErr );

		// 文字列がフィールドサイズを超えていないかチェック
		CheckExceedFieldSize( featureDef, strErr );

		// エラーメッセージがなければ次のフィーチャへ
		if( strErr.IsEmpty() )
			continue;

		// エラーメッセージがあれば、表示用のメッセージに成形
		LONG lOID = 0;
		featureDef.GetFeature()->get_OID(&lOID);
		CString strTmpMsg;
		strTmpMsg.Format( AheLoadString(IDS_ERR_INVALID_ATTR), featureDef.GetTableName(), lOID );
		INT nCurPos = 0;
		CString strToken( strErr.Tokenize( _T(","), nCurPos ) );
		while( -1 != nCurPos )
		{
			strTmpMsg += _T("\n") + strToken;
			strToken = strErr.Tokenize(_T(","), nCurPos );
		}
		strMsg += strTmpMsg + _T("\n");

		bRet = FALSE;
	}

	// エラーがあればメッセージを出力
	if( !bRet )
		AttrMessageBox( strMsg, AheLoadString( IDS_ERRCHK ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
	
	return bRet;
}

/**
 * @brief 注記文字列に不正な文字が含まれていないかチェックする
 *
 * @note  下記の文字以外が含まれる場合は、エラーとする
 *        全角漢字、全角ひらがな、全角カタカナ、
 *        「０」～「９」、「ａ」～「Ｚ」、全角ギリシア文字
 *        全角利用可能特殊記号（0x8141（「、」）～0x81fc（「◯」））
 *        「．」「～」「々」「〆」「・」「（」「）」「ー」「、」「－」
 *        ※第三水準の文字は許容しない
 *        また、チェックするのは注記文字列1のみ
 *
 * @param	featureDef	[in]		チェック対象のフィーチャ
 * @param	strErr		[out]		エラーメッセージ
 *
 * @retval  TRUE  対象のレイヤではない、または不正文字が含まれていない
 * @retval  FALSE 不正な文字が含まれている
 */
BOOL CAttrCdsCommonDlg::CheckAnnoNameString( CFeatureDef& featureDef, CString& strErr )
{
	// 注記以外では確認不要
	CString strTableName( featureDef.GetTableName() );
	if( 0 != strTableName.CompareNoCase(city_annotation_gs::kTableName) )
		return TRUE;
	
	// 注記文字列1をチェック		
	CFieldDef* pDef = featureDef.GetFieldDef( strTableName, city_annotation_gs::kNameString1 );
	CString strOrgStr( ( pDef && VT_BSTR == pDef->m_vaValue.vt ) ? pDef->m_vaValue : _T("") );

	// 注記文字列を適正な文字列に変換
	CString strErrChar; // 不正な文字
	CString strCorrectStr = AheConvertToAnnoJustStr(strOrgStr, TRUE, FALSE, FALSE, &strErrChar );

	// 変換前後で注記文字列に変化がなければリターン
	// 変化があれば、不正な文字が含まれている
	if( 0 == strCorrectStr.Compare(strOrgStr) )
		return TRUE;
	
	// 不正があれば、メッセージボックスを出して保存させない
	strErr += AheLoadString( IDS_MSG_INVALID_NAMESTRING );

	return FALSE;
}

/**
 * @brief 注記文字列を変更した際に注記文字数を更新する
 *
 * @retern  なし
 */
void CAttrCdsCommonDlg::AutoUpdateStringNum(UINT nTargetControl)
{
	if( ! GetCurrentFeatureDef() )
		return;

	CString strStringNumFieldName, strStringFieldName; // 文字数・文字列フィールド名（nTargetControlで変わる）
	switch( nTargetControl )
	{
	case IDC_EDIT_NAMESTRING1: strStringNumFieldName = city_annotation_gs::kStringNum1; strStringFieldName = city_annotation_gs::kNameString1; break;
	case IDC_EDIT_NAMESTRING2: strStringNumFieldName = city_annotation_gs::kStringNum2; strStringFieldName = city_annotation_gs::kNameString2; break;
		default: _ASSERTE(false); break;
	}

	CFieldDef* pStringFieldDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, strStringFieldName );

	CString strText;
	if( pStringFieldDef )
	{
		CComVariant vaNameString( pStringFieldDef->m_vaValue );
		if( VT_BSTR == vaNameString.vt )
			strText = vaNameString.bstrVal;
	}

	// 文字数計算（ANSIでのバイト数であることに注意！！ bug 2746）
	LONG lStringNumber = strlen( CT2CA( strText ) );
	// UTF-8でのバイト数計算 一応残しておく
	//lStringNumber = ::WideCharToMultiByte( CP_UTF8, 0, strText, -1, NULL, 0, NULL, NULL ) - 1;

	// FieldDefにセット
	CFieldDef* pStringNumFieldDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, strStringNumFieldName );
	pStringNumFieldDef->SetNewValue( lStringNumber, m_hWnd );
}

/**
 * @brief 注記種別を変更した際に注記記号のコンボボックスリストを更新する
 *
 * @retern  なし
 */
void CAttrCdsCommonDlg::UpdateDispTypeList()
{
	// CITY_ANNOTATION_GS以外、または編集開始していなければリターン
	if( 0 != m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) || !m_cArcHelper.IsStartEditing() )
		return;

	// 注記種別を取得
	LONG lCode = -1;
	GetClassID( &lCode );

	// 注記記号のコンボボックスリストを更新し、注記種別に対応する注記記号をセット
	CComboBox cWDispType(GetDlgItem(IDC_COMBO_DISPTYPE));
	cWDispType.ResetContent();

	CFieldDef* pDispTypeDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, city_annotation_gs::kDispType );

	// 注記種別に対応する注記記号のみを選択できるよう、リストを更新
	// 対応するものがない場合は、すべて選択できるようにする
	auto mapDispTypeDomain = pDispTypeDef->GetDomain();
	auto it = m_mapAnnoDispType.find( lCode );
	for(const auto& domain : *mapDispTypeDomain)
	{
		if(m_mapAnnoDispType.end() == it || domain.idx == it->second)
			cWDispType.SetItemData( cWDispType.AddString( domain.code_discription ), domain.idx );
	}

	// 種別に対応する記号があればその注記記号を、
	// なければフィールドにセットされている注記記号を、コンボボックスから選択
	LONG lDispType = -1;
	if( m_mapAnnoDispType.end() != it )
		lDispType = it->second;
	else
		lDispType = ( ( pDispTypeDef && VT_I4 == pDispTypeDef->m_vaValue.vt ) ? pDispTypeDef->m_vaValue.lVal : -1 );

	SetComboBoxSel(IDC_COMBO_DISPTYPE, lDispType);

	InvalidateRect( NULL, FALSE );
	// 変更された場合はボタンを Enable に
	SetButton( Changed() );
}

/**
 * @brief 指定したコンボボックスのアイテムを変更
 *
 * @param	nComboID	[in]		コンボボックスのItemID
 * @param	lVal	    [in]		表示するアイテムに対応する値
 *
 * @return  なし 
 */
void CAttrCdsCommonDlg::SetComboBoxSel(INT nComboID, LONG lVal)
{
	CComboBox	cWComboBox( GetDlgItem(nComboID) );
	LONG		lClassNum = cWComboBox.GetCount();
	LONG		lComboBoxItemNum = 0;
	for (; lComboBoxItemNum < lClassNum ; ++lComboBoxItemNum)
	{
		DWORD dwComboBoxValue = cWComboBox.GetItemData(lComboBoxItemNum);
		LONG lComboBoxValue = (LONG)dwComboBoxValue;
		if (lComboBoxValue == lVal)
			break;
	}
	cWComboBox.SetCurSel(lComboBoxItemNum);

	m_cControlRel.ControlChanged( nComboID );
}

/**
 * @brief 入力された文字列がフィールドサイズを超えていないかチェックする
 *
 * @param	featureDef	[in]		チェック対象のフィーチャ
 * @param	strErr		[out]		エラーメッセージ
 *
 * @retval  TRUE  フィールドサイズを超えていない
 * @retval  FALSE フィールドサイズを超えている
 */
BOOL CAttrCdsCommonDlg::CheckExceedFieldSize( CFeatureDef& featureDef, CString& strErr )
{
	CString strTableName( featureDef.GetTableName() );

	// チェック対象のフィールド一覧(べた書き)
	std::vector<CString> vecTgtFieldName;
	{
		// 情報ソース(各レイヤ共通)
		vecTgtFieldName.emplace_back(ipc_table::kSource);

		if( 0 == strTableName.CompareNoCase(city_annotation_gs::kTableName) )
		{
			// 注記文字列1, 注記文字列2(CITY_ANNOTATION_GS)
			vecTgtFieldName.emplace_back(city_annotation_gs::kNameString1);
			vecTgtFieldName.emplace_back(city_annotation_gs::kNameString2);
		}
		else if( 0 == strTableName.CompareNoCase(building_gs::kTableName) )
		{
			// 建物名称, 建物住所(BUILDING_GS)
			vecTgtFieldName.emplace_back(building_gs::kBldName);
			vecTgtFieldName.emplace_back(building_gs::kBldAddr);
		}
	}

	BOOL bIsOK = TRUE;

	// 入力された文字列がフィールドサイズを超えていないかチェック
	for( const auto& fieldName : vecTgtFieldName )
	{
		CFieldDef* pDef = featureDef.GetFieldDef(strTableName, fieldName);

		LONG lFieldSize = -1;
		pDef->m_ipField->get_Length(&lFieldSize);

		CString strInput = ( VT_BSTR == pDef->m_vaValue.vt ) ? pDef->m_vaValue : _T("");
		// UTF-8であることに注意
		INT nInputSize = ::WideCharToMultiByte( CP_UTF8, 0, strInput, -1, NULL, 0, NULL, NULL ) - 1;

		if( lFieldSize >= nInputSize )
			continue;

		CString strTmp;
		strTmp.Format(AheLoadString(IDS_MSG_EXCEED_FIELD_SIZE), pDef->m_strFieldAliasName);
		strErr += strTmp;
		bIsOK = FALSE;
	}

	return bIsOK;
}
