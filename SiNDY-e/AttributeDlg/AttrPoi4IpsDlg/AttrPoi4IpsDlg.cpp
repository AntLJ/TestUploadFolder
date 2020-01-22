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

// AttrPoi4IpsDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrPoi4IpsDlg.h"
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <TDC/useful_headers/registry_util.h>
#include "TDC/sindylib_core/FieldMap.h"
#include "AccTable.h"
#include <fstream>

using namespace sindy::schema;

// 定数定義
namespace
{
	// レジストリ関連
	const CString REG_PATH = _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\AttributeTool\\AttrPoi4IpsDlg");
	const CString REG_CONTENTS_MASTER = _T("ContentsMasterFilePath");
	const CString REG_INFOPROVIDE_MASTER = _T("InfoProvideMasterFilePath");
	
	// コントロール関連
	const CString DELETE_BUTTON_STR = _T("削除");
	const CString DELETE_BUTTON_REDO_STR = _T("削除解除");

	// エラーメッセージ関連
	const CString TOOL_NAME = _T("iPS向けPOI注記整備ツール");
	const CString REG_KEY_ERR = _T("レジストリ設定が不正です");
	const CString FILE_PATH_ERR = _T("レジストリに設定されているファイルパスが不正です");

	// 設定値関連

	// 閉鎖理由　閉鎖・廃止
	const CString CLOSE_C_CLOSE = _T("a");
	// 情報提供元コード　インクリメントP（株）
	const CString PST_IP_C_CLOSE_INCREMENT_P = _T("00");
	// 精度　都市地図レベル 家形
	const CString ACC_C_WALL = _T("1S");
}

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(CPOIPOINT_MAP)
	BEGINROWS(WRCT_FIXED,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_FIXED,5,5)
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CONTENTS_CODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CONTENTS_CODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CONTENTS_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CONTENTS_NAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_PP_FOUND)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PST_IP_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_PST_IP_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_AAC_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_AAC_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_DELETE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLOSE_C)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_CLOSE_C)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_DELETE_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DELETE_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ANNO_DISP_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ANNO_DISP_C)
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_FIXED, 0, RCMARGINS(10,0))
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
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
 * @retval CAttrPoi4IpsDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrPoi4IpsDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrPoi4IpsDlg::CAttrPoi4IpsDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrPoi4IpsDlg::CAttrPoi4IpsDlg() 
	: ATTR_BASE_CLASS(CPOIPOINT_MAP, IDD_POIPOINTDLG),
	m_isDelete(true), m_contentsType(LONG_MAX), m_isNewlyPoi(false)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_EDIT_CONTENTS_CODE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CONTENTS_CODE ) )
		( IDC_EDIT_CONTENTS_NAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CONTENTS_NAME ) )
		( IDC_EDIT_NAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAME ) )
		( IDC_BUTTON_PP_FOUND, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_PP_FOUND ) )
		( IDC_COMBO_PST_IP_C, CContainedWindow( _T("COMBO"), this, IDC_COMBO_PST_IP_C ) )
		( IDC_COMBO_AAC_C, CContainedWindow( _T("COMBO"), this, IDC_COMBO_AAC_C ) )
		( IDC_BUTTON_DELETE, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_DELETE ) )
		( IDC_EDIT_CLOSE_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CLOSE_C ) )
		( IDC_EDIT_DELETE_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_DELETE_C ) )
		( IDC_EDIT_ANNO_DISP_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_ANNO_DISP_C ) );
}

/** 
 * @fn HWND CAttrPoi4IpsDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrPoi4IpsDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrPoi4IpsDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrPoi4IpsDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrPoi4IpsDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrPoi4IpsDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// read only
	CEdit(GetDlgItem(IDC_EDIT_CONTENTS_CODE)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_CONTENTS_NAME)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_NAME)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_CLOSE_C)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_DELETE_C)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_ANNO_DISP_C)).SetReadOnly(TRUE);

	// 選択されているフィーチャの情報を取得しメンバ変数にセット
	getCurrentFeatureInfo();
	// 情報提供元コードコンボボックス作成
	createPstIpCombo();
	// 精度コンボボックス作成
	createAcc();
	// 本ダイアログで使用するドメインを作成しメンバ変数にセット
	createStrKeyDomain();
	
	// GRPCODE-CHAINCODE-ContentsSeqの形でコンテンツコードを表示
	GetDlgItem(IDC_EDIT_CONTENTS_CODE).SetWindowText( uh::str_util::format(_T("%s-%s-%s"), m_grpcode, m_chainCode, m_contentsSeq));
	// コンテンツ名称を表示
	GetDlgItem(IDC_EDIT_CONTENTS_NAME).SetWindowText( uh::str_util::format(_T("%s"), m_contentsName ));

	// 自前でで管理しているEditBox群の初期表示を実行
	setEditBoxByDomainVal(IDC_EDIT_CLOSE_C, sindyk::poi_point::kCloseCode);
	setEditBoxByDomainVal(IDC_EDIT_DELETE_C, sindyk::poi_point::kDelete );
	setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, sindyk::poi_point::kAnnoDispCode);

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );

		// 正位置判明ボタンアクティブ化制御
		// コンテンツタイプが兼用・ＰＯＩ専用の場合、ボタン、情報提供元コード、精度をアクティブとする。
		GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(TRUE);
		if ( !( m_contentsType == sindyk::contents_master::contents_type_code::kCommon ||
			m_contentsType == sindyk::contents_master::contents_type_code::kPOI )
			)
		{
			GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(FALSE);
		}

		// 削除ボタンアクティブ化制御
		// コンテンツタイプが以下の場合アクティブとする。
		// ・兼用
		// ・注記専用かつ新規POI以外
		GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(TRUE);
		m_isDelete = judgeDeleteFlg();
		if (m_isDelete)
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_STR);
		else
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_REDO_STR);

		if ( !( m_contentsType == sindyk::contents_master::contents_type_code::kCommon ||
			    (m_contentsType == sindyk::contents_master::contents_type_code::kAnno && !m_isNewlyPoi))
			)
		{
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_STR);
			GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(FALSE);
	}

	return bRet;
}

/**
 * @fn void CAttrPoi4IpsDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrPoi4IpsDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( !m_cControlRel.empty() )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ControlRelにフィールド名とテーブル名を登録
		// 名称
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, sindyk::poi_point::kName, NULL, sindyk::poi_point::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, sindyk::poi_point::kTableName );

		// ※情報提供元コード、精度、閉鎖理由、削除コード、注記表示コードは自前で行う
	}
}

// テーブルの中からidx番目の値を取得
std::pair<CString, CString> getValByIdx(const StringMasterTbl& tbl , int idx)
{
	ASSERT(tbl.size() < idx);
	int cnt(0);
	for( const auto&  rec :  tbl)
	{
		if (cnt == idx)
			return rec.second;

		++cnt;
	}

	return std::pair<CString, CString>();
}

// コンボボックスのアイテムをFieldDefにセット
void CAttrPoi4IpsDlg::setFieldDef4Combo(const StringMasterTbl& tbl, UINT id,  const LPCTSTR lpszFieldName)
{
	int idx = CComboBox(GetDlgItem(id)).GetCurSel();
	std::pair<CString, CString> pair = getValByIdx( tbl, idx);
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, lpszFieldName );
	if( 0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, pair.first) )
	{
		pFieldDef->m_vaValue = pair.first;
		pFieldDef->m_bChanged = TRUE;
	}

	// 更新情報変更
	updateChange(pFieldDef);

	// 更新情報付与
	UpdateHistory(GetCurrentFeatureDef());
}

LRESULT  CAttrPoi4IpsDlg::setPstIPCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 情報提供元コードコンボボックスの値でFieldDefを変更
	setFieldDef4Combo(m_pstIpCodeMaster, IDC_COMBO_PST_IP_C, sindyk::poi_point::kPSTIPCode);

	return 0;
}

LRESULT  CAttrPoi4IpsDlg::setAccCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 精度コンボボックスの値でFieldDefを変更
	setFieldDef4Combo(m_AccCodeMaster, IDC_COMBO_AAC_C, sindyk::poi_point::kACCCode);

	return 0;
}

// 任意の文字列の添え字を取得する
size_t getIdx(std::vector<CString> src, const CString target )
{
	std::vector<CString>::iterator iter = std::find(src.begin(), src.end(), target);
	return std::distance(src.begin(), iter);
}

// レジストリ値を取得
bool CAttrPoi4IpsDlg::getRegKey(const CString& path , const CString& key, CString& out)
{
	// パスを取得
	out = uh::reg_util::read( key, path );

	// キーがない、または値が空なら許可
	if( out.IsEmpty() )
	{
		CString msg = uh::str_util::format( _T("%s\nレジストリパス:%s"), REG_KEY_ERR.GetString(), key.GetString() );
		AttrMessageBox( msg.GetString(), TOOL_NAME.GetString(), MB_OK, NULL);
		return false;
	}

	return true;
}

// 現在の選択されているフィーチャから必要な情報を取得する
void CAttrPoi4IpsDlg::getCurrentFeatureInfo()
{
	// 選択されているフィーチャを取得
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	// フィールドマップ作成しメンバ変数にセット
	m_fieldMap = sindy::CFieldMap( feature->GetFields() );

	//コンテンツマスタが読み込まれていない場合はリターン
	if(m_contentsMaster.empty())
		return;

	// GRPCODE,CHAINCODE,コンテンツ名称,コンテンツタイプの位置を特定
	long grpcodeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kGroupCode );
	long chainCodeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kChainCode );
	long contentsTypeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kContentsType);
	long nameIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kName);

	// コンテンツコードからGRPCODE,CHAINCODEを取得
	CString tmpContents = feature->GetValue( m_fieldMap.FindField( sindyk::poi_point::kContentsCode ) );
	long tmpGrpcode = _ttoi( tmpContents.Left( 4 ) ); // コンテンツコードの上4桁がGRPCODE
	long tmpChainCode = _ttoi( tmpContents.Right( 5 ) ); // コンテンツコードの上5桁がCHAINCODE

	// コンテンツマスタからコンテンツコードに紐づくレコードを特定
	bool isHeader = true;
	bool isExist = false;
	for( const auto&  line :  m_contentsMaster )
	{

		// ヘッダは省略
		if ( isHeader )
		{
			isHeader = false;
			continue;
		}

		// コンテンツコードに紐づくレコードを取得する
		if ( _ttoi(line[grpcodeIdx]) == tmpGrpcode && _ttoi(line[chainCodeIdx]) == tmpChainCode )
		{
			isExist = true;
			// GRPCODEをセット
			m_grpcode = line[grpcodeIdx];
			// CHAINCODEをセット
			m_chainCode = line[chainCodeIdx];
			// コンテンツタイプをセット
			m_contentsType = _ttoi(line[contentsTypeIdx]);
			// コンテンツ名称をセット
			m_contentsName = line[nameIdx];

			break;
		}
	}

	if (!isExist)
		return;

	// ContentsSeqをセット
	m_contentsSeq = feature->GetValue( m_fieldMap.FindField( sindyk::poi_point::kContentsSeq ) );

	// 新規POIかを判定
	m_isNewlyPoi = m_contentsSeq.Find(_T("-")) != -1;
}

// コンボボックス作成(汎用)
void CAttrPoi4IpsDlg::createComboBox(UINT id, const StringMasterTbl& tbl, LPCTSTR lpszTableName, LPCTSTR lpszFieldName)
{

	// 初期選択の為にFieldDefを取得しておく
	CFieldDef *pFieldDef = GetCurrentFeatureDef()->GetFieldDef( lpszTableName, lpszFieldName );

	// コンボボックスにアイテムを追加
	CComboBox cCombo = CComboBox(GetDlgItem(id));
	cCombo.ResetContent();
	int hitPos = INT_MAX;
	for ( const auto& line : tbl )
	{
		const std::pair<CString, CString>& masterRec = line.second;

		// 表示文字列を作成
		CString disp;
		disp.Format(_T("%s(%s)"), masterRec.second , masterRec.first);

		INT iCurPos = cCombo.AddString( disp );
		cCombo.SetItemData( iCurPos, line.first );

		// 選択されているフィーチャの値だった場合、その位置を選択行とする
		if( pFieldDef->m_vaValue == masterRec.first )
			cCombo.SetCurSel( iCurPos );
	}
}

// 情報提供元コードコンボボックス作成
void CAttrPoi4IpsDlg::createPstIpCombo()
{
	// コンボボックスに入れれる項目が存在しない場合作成
	if ( m_pstIpCodeMaster.empty() )
	{
		// レジストリから情報提供元コードマスタのパスを取得
		CString regString;
		if( !getRegKey( REG_PATH, REG_INFOPROVIDE_MASTER, regString) )
			return;

		// 情報提供元コードマスタ読み込み
		const std::vector<std::vector<CString>>& infoProvideMaster =  parseTsvFile( regString );
		if (infoProvideMaster.empty())
			return;

		// IP_CODE,NAME,の位置を特定
		long ipCodeIdx = getIdx(infoProvideMaster[0], sindyk::infoprovide_master::kIPCode );
		long nameIdx = getIdx(infoProvideMaster[0],  sindyk::infoprovide_master::kName );
		long mntPriorityIdx = getIdx(infoProvideMaster[0],  sindyk::infoprovide_master::kMNTPriority );

		// 情報提供元コードマスタからIP_CODE,NAMEを取得
		bool isHeader = true;
		for( const auto&  line : infoProvideMaster )
		{
			// ヘッダは省略
			if ( isHeader )
			{
				isHeader = false;
				continue;
			}

			// 整備優先順位が-1は省略
			if ( _ttoi( line[mntPriorityIdx] ) == -1 )
				continue;

			m_pstIpCodeMaster[ _ttoi(line[mntPriorityIdx]) ] = std::pair<CString,CString>(line[ipCodeIdx],line[nameIdx]);
		}
	}

	// コンボボックス作成
	createComboBox(IDC_COMBO_PST_IP_C, m_pstIpCodeMaster, sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode);
}

// 精度コンボボックス作成
void CAttrPoi4IpsDlg::createAcc()
{
	// コンボボックス作成
	createComboBox(IDC_COMBO_AAC_C, m_AccCodeMaster, sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode);
}

// フィールドの設定値をドメインの値に変換しエディットボックスに入れる
void CAttrPoi4IpsDlg::setEditBoxByDomainVal(int id, LPCTSTR lpszFieldName)
{
	using namespace sindyk::poi_point;

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, lpszFieldName );
	if ( pFieldDef->m_vaValue.vt != VT_NULL )
	{
		CString dsp;
		dsp.Format(_T("%s(%s)"), m_strKeyDomainMap[lpszFieldName][CString(pFieldDef->m_vaValue)], CString(pFieldDef->m_vaValue));
		CEdit(GetDlgItem(id)).SetWindowText( dsp.GetString() );
	}
	else
		CEdit(GetDlgItem(id)).SetWindowText(_T("") );
}

// 削除ボタンが押下された場合の処理
LRESULT CAttrPoi4IpsDlg::onPressDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	using namespace sindyk::contents_master::contents_type_code;
	using namespace sindyk::poi_point;

	// 削除の場合
	if ( m_isDelete )
	{
		// 兼用の場合
		if( m_contentsType == kCommon )
		{
			//「注記表示コード」に「非表示」を設定する。
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
			pFieldDef->m_vaValue =  annodisp_code::kNone;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, kAnnoDispCode);
		}
		// 注記専用かつ新規POI以外
		else
		{
			//「削除コード」に「1」、「閉鎖理由」に「閉鎖・廃止」を設定する。
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
			pFieldDef->m_vaValue =  delete_code::kDead ;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal(IDC_EDIT_DELETE_C, kDelete);
			
			pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
			pFieldDef->m_vaValue =CLOSE_C_CLOSE;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal( IDC_EDIT_CLOSE_C, kCloseCode );
		}
	} 
	// 削除解除の場合
	else
	{
		// 兼用の場合
		if( m_contentsType == kCommon )
		{
			//「注記表示コード」に「標準」を設定する。
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
			pFieldDef->m_vaValue =  annodisp_code::kNomal;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, kAnnoDispCode);

		}
		// 注記専用かつ新規POI以外
		else
		{
			//「削除コード」に「0」、「閉鎖理由」に「NULL」を設定する。
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
			pFieldDef->m_vaValue =  delete_code::kAlive ;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal(IDC_EDIT_DELETE_C, kDelete);

			pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
			pFieldDef->m_vaValue.vt = VT_NULL;
			// 更新フラグ変更
			updateChange(pFieldDef);
			// 画面表示更新
			setEditBoxByDomainVal( IDC_EDIT_CLOSE_C, kCloseCode );
		}
	}

	//ボタン判定用フラグを反転
	m_isDelete = !m_isDelete;

	// 削除ボタンの名称を変更
	CButton cButton = CButton(GetDlgItem(IDC_BUTTON_DELETE));
	if (m_isDelete)
		cButton.SetWindowText(DELETE_BUTTON_STR);
	else
		cButton.SetWindowText(DELETE_BUTTON_REDO_STR);

	// 更新情報付与
	UpdateHistory(GetCurrentFeatureDef());
	// ダイアログを更新領域に追加します
	InvalidateRect( NULL, FALSE );
	// 変更された場合はボタンを Enable に
	SetButton( Changed() );

	return 0;
}

// 正位置判明ボタンが押下された場合の処理
LRESULT CAttrPoi4IpsDlg::onPressPpFound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 情報提供元コードを「00 インクリメントP（株）に変更
	CComboBox pstIpCombo = CComboBox( GetDlgItem(IDC_COMBO_PST_IP_C) );
	pstIpCombo.SetCurSel( getComboIdx(PST_IP_C_CLOSE_INCREMENT_P,  m_pstIpCodeMaster) );
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode );
	if(0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, PST_IP_C_CLOSE_INCREMENT_P))
	{
		pFieldDef->m_vaValue = PST_IP_C_CLOSE_INCREMENT_P;
		// 更新フラグ変更
		updateChange(pFieldDef);
	}

	// 「精度」を「1S」に変更
	CComboBox accCombo = CComboBox( GetDlgItem(IDC_COMBO_AAC_C) );
	accCombo.SetCurSel( getComboIdx(ACC_C_WALL,  m_AccCodeMaster) );
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode );
	if(0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, ACC_C_WALL))
	{
		pFieldDef->m_vaValue = ACC_C_WALL;
		// 更新フラグ変更
		updateChange(pFieldDef);
	}

	// 更新情報付与
	UpdateHistory(GetCurrentFeatureDef());

	// ダイアログを更新領域に追加します
	InvalidateRect( NULL, FALSE );
	// 変更された場合はボタンを Enable に
	SetButton( Changed() );

	return 0;
}

// マスタからコンボボックスのシーケンスを取得する
INT CAttrPoi4IpsDlg::getComboIdx(const CString& ipCode, const StringMasterTbl& tbl)
{
	// コンボボックスのシーケンスはmapのソート順(すなわち表示優先順位順)のはず
	UINT ret(UINT_MAX);
	UINT cnt(0);
	for( const auto& line : tbl )
	{
		if (line.second.first == ipCode )
		{
			ret = cnt;
			break;
		}
		++cnt;
	}
	return ret;
}

// 引数を区切り文字で分解
std::vector<CString> spritStr(CString str, CString delim)
{
	std::vector<CString> ret;

	CString st;
	int indexback = 0;
	int i;

	// 一文字ずつ解析
	for( i = 0; i < str.GetLength(); i++)
	{
		// 区切り文字が見つかったら
		if (str.GetAt(i) == delim.GetAt(0))
		{
			if ((i - indexback) == 0)
				ret.push_back(_T(""));
			else
			{
				// 区切り文字から前の文字列を取得
				st = str.Mid(indexback, i - indexback);
				ret.push_back(st);
			}
			indexback = i + 1;
		}
	}

	if ((i - indexback) != 0)
	{
		st = str.Mid(indexback, i - indexback);
		ret.push_back(st);
	}

	return ret;
}

// TSVファイルを2次元配列で返却する
std::vector<std::vector<CString>> CAttrPoi4IpsDlg::parseTsvFile(const CString& file)
{
	std::vector< std::vector<CString> > ret;
	std::list<CString> lists;
	if( !uh::isFile(file.GetString()) )
	{
		CString msg = uh::str_util::format( _T("%s\nファイルパス:%s"),FILE_PATH_ERR.GetString(), file.GetString() );
		AttrMessageBox( msg.GetString(), TOOL_NAME.GetString(), MB_OK, NULL);

		return ret;
	}

	// ファイルから1行ずつ読み込んで格納
	std::ifstream ifs(file.GetString());
	if( !(ifs && ifs.good()) )
		return ret;

	std::string buf;
	while(std::getline(ifs, buf))
	{
		lists.push_back( uh::toTStr(buf).c_str() );
	}

	for( const auto&  line : lists)
	{
		// タブ区切りをばらして格納
		ret.push_back( spritStr( line,_T("\t")));
	}
	return ret;
}

// 更新情報を付与
bool CAttrPoi4IpsDlg::UpdateHistory(CFeatureDef* featureDef)
{
	//　 更新情報をいったん初期化する

	// WORK_DATEを元に戻す
	CFieldDef* pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorkDate );
	pFieldDef->m_vaValue = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;
	// WORK_DATEを元に戻す
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifyDate );
	pFieldDef->m_vaValue  = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;

	// WORKERを元に戻す
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorker );
	pFieldDef->m_vaValue  = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;
	// VERIFIERを元に戻す
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifier );
	pFieldDef->m_vaValue = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;

	// 初期化後変更があった場合更新を行う
	if (featureDef->Changed())
	{
		// 最終プログラム更新日時
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

		// WORK_DATEを更新
		CFieldDef* pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorkDate );
		pFieldDef->m_vaValue = strTime;
		pFieldDef->m_bChanged = TRUE;
		//VERIFY_DATEを更新
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifyDate );
		pFieldDef->m_vaValue = strTime;
		pFieldDef->m_bChanged = TRUE;

		// 更新者
		CComBSTR bstrOperator;
		m_ipRule->get_OperatorName( &bstrOperator );
		// WORKERを更新
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorker );
		pFieldDef->m_vaValue = bstrOperator;
		pFieldDef->m_bChanged = TRUE;
		// VERIFIERを更新
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifier );
		pFieldDef->m_vaValue = bstrOperator;
		pFieldDef->m_bChanged = TRUE;
	}

	return true;
}

// 削除ボタンフラグを現在の状況から判断する
bool CAttrPoi4IpsDlg::judgeDeleteFlg()
{
	using namespace sindyk::contents_master::contents_type_code;
	using namespace sindyk::poi_point;

	// 兼用の場合
	if( m_contentsType == kCommon )
	{
		//「注記表示コード」に「非表示」以外か？
		CFieldDef* annoDispCodeDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
		if (annoDispCodeDef->m_vaValue.lVal != annodisp_code::kNone)
			return true;
	}
	// 注記専用かつ新規POI以外
	else
	{
		//「削除コード」に「1」以外 または「閉鎖理由」が「閉鎖・廃止」以外か？
		CFieldDef* deleteCDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
		CFieldDef* closeCDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
		if (deleteCDef->m_vaValue.lVal != delete_code::kDead ||
			!( closeCDef->m_vaValue.vt != VT_NULL && 0 == _tcscmp( closeCDef->m_vaValue.bstrVal,CLOSE_C_CLOSE))
			)
			return true;
	}

	return false;
}

// 更新フラグを変更する
void CAttrPoi4IpsDlg::updateChange(CFieldDef* pFieldDef)
{
	if ( pFieldDef->m_vaValue == pFieldDef->m_vaInitValue)
		pFieldDef->m_bChanged = FALSE;
	else
		pFieldDef->m_bChanged = TRUE;
}

// キーがStringであるドメインを取得する
std::map< CString, CString> CAttrPoi4IpsDlg::getStrKeyDomein(const LPCTSTR lpszFieldName)
{
	std::map< CString, CString> ret;
	IDomainPtr ipDomain;

	GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, lpszFieldName )->m_ipField->get_Domain( &ipDomain );

	ICodedValueDomainPtr ipCodedValueDomain( ipDomain );

	long lCodeNum = 0;
	ipCodedValueDomain->get_CodeCount( &lCodeNum );

	for( int i = 0; i < lCodeNum; ++i )
	{
		CComVariant vaValue;
		CComBSTR bstrName;
		ipCodedValueDomain->get_Value( i, &vaValue );
		ipCodedValueDomain->get_Name( i, &bstrName );
		ret[ CString(vaValue) ] = CString(bstrName);
	}

	return ret;
}

// 本ダイアログで使用するドメインを取得する
void CAttrPoi4IpsDlg::createStrKeyDomain()
{
	using namespace sindyk::poi_point;
	
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kAnnoDispCode) )
		m_strKeyDomainMap[ kAnnoDispCode ] = getStrKeyDomein( kAnnoDispCode );
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kDelete) )
		m_strKeyDomainMap[ kDelete ] = getStrKeyDomein( kDelete );
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kCloseCode) )
		m_strKeyDomainMap[ kCloseCode ] = getStrKeyDomein( kCloseCode );

}

// コンテンツマスタ読み込み
bool  CAttrPoi4IpsDlg::readContentsMaster()
{
	// レジストリからコンテンツマスタのパスを取得
	CString regString;
	if( !getRegKey(REG_PATH, REG_CONTENTS_MASTER, regString) )
		return false;

	// コンテンツマスタ読み込み
	m_contentsMaster = parseTsvFile( regString );
	if ( m_contentsMaster.empty() )
		return false;

	return true;
}
