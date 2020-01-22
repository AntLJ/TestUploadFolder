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

// AttrZipcodePolygonDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrZipCodeAreaDlg.h"
#include <boost/assign/list_of.hpp>
#include <regex>

using namespace sindy::schema;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(ZIPCODEAREA_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE2)
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
 * @retval CAttrZipcodePolygonDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrZipCodeAreaDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrZipcodePolygonDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrZipcodePolygonDlg::CAttrZipcodePolygonDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrZipCodeAreaDlg::CAttrZipCodeAreaDlg() 
	: ATTR_BASE_CLASS(ZIPCODEAREA_MAP, IDD_ZIPCODEDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_EDIT_ZIPCODE1, CContainedWindow(_T("EDIT"), this, IDC_EDIT_ZIPCODE1))
		(IDC_EDIT_ZIPCODE2, CContainedWindow(_T("EDIT"), this, IDC_EDIT_ZIPCODE2))
		(IDC_EDIT_SOURCE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE));
}

/** 
 * @fn HWND CAttrZipcodePolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrZipCodeAreaDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrZipcodePolygonDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrZipCodeAreaDlg::Delete()
{
	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrZipcodePolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * また、原稿番号の値も初期化して、ルールから取得しておく
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrZipCodeAreaDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex(lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable);

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_ZIPCODE1));
	}

	return bRet;
}

/**
 * @fn void CAttrZipcodePolygonDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrZipCodeAreaDlg::CreateControlRelation()
{
	ATLASSERT(IsWindow() == TRUE);

	if (m_cControlRel.size() > 0)
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND(m_hWnd);
	}
	else
	{
		m_cControlRel.SetHWND(m_hWnd);

		// 郵便番号1
		m_cControlRel.SetControl(
			IDC_EDIT_ZIPCODE1, TYPE_EDITSTRING, 0, 0,
			zipcode_area::kZipCode1, 0, zipcode_area::kTableName);
		// 郵便番号2
		m_cControlRel.SetControl(
			IDC_EDIT_ZIPCODE2, TYPE_EDITSTRING, 0, 0,
			zipcode_area::kZipCode2, 0, zipcode_area::kTableName);
		// 情報ソース
		m_cControlRel.SetControl(
			IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0,
			ipc_table::kSource, 0, zipcode_area::kTableName);
	}
}

BOOL CAttrZipCodeAreaDlg::ErrorCheck()
{
	// 7桁の数字かどうかをチェックする
	// 郵便番号として適切かどうかは別途チェックツールで対応
	std::regex re("[0-9]{7}");
	std::smatch match;

	CString zipCode1;
	GetDlgItem(IDC_EDIT_ZIPCODE1).GetWindowText(zipCode1);
	if (zipCode1.IsEmpty())
	{
		// zipcode1がNULLはエラー
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE1_NULL), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	std::string s = (CStringA)zipCode1;
	if (!std::regex_match(s, match, re))
	{
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE1_DIGIT), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	CString zipCode2;
	GetDlgItem(IDC_EDIT_ZIPCODE2).GetWindowText(zipCode2);
	if (zipCode2.IsEmpty())
	{
		// zipcode2がNULLはOK
		return TRUE;
	}

	std::string s2 = (CStringA)zipCode2;
	if (!std::regex_match(s2, match, re))
	{
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE2_DIGIT), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	// zipcode1とzipcode2が同じ値になっているのはNG
	if (0 == zipCode1.CompareNoCase(zipCode2))
	{
		MessageBox(AheLoadString(IDS_ERR_SAME_ZIPCODE), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	return TRUE;
}
