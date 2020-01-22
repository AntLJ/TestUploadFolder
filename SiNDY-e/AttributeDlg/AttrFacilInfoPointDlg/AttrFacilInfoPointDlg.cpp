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

// AttrFacilInfoPointDlg.cpp: CAttrFacilInfoPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "sindy/libschema.h"
#include "AttrFacilInfoPointDlg.h"

/// ウィンドウマネージャ配置マップ（高速・有料道路検索関係ポイント用）
BEGIN_WINDOW_MAP(FACIL_INFO_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADCLASS_REF)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADCLASS_REF)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADNAME_REF)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADNAME_REF)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROSENCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ROSENCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SEQUENCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SEQUENCE)
			ENDGROUP()
			RCSPACE(12)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILKANJI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FACILKANJI)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILKANA)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FACILKANA)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DUMMYFLAG)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_FACILCLASS)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SEARCHFACIL)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SEARCHFACIL)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_AUTOPOSITION_F)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DUMMY)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrFacilInfoPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrFacilInfoPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrFacilInfoPointDlg::CAttrFacilInfoPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化、さらにタブ位置、フォントタイプの初期化をしておきます。
 */
CAttrFacilInfoPointDlg::CAttrFacilInfoPointDlg()
	: ATTR_BASE_CLASS(FACIL_INFO_POINT_MAP, IDD_ATTRFACILINFOPOINTDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SEQUENCE ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_FACILKANJI ), 
		m_wnd4( _T("EDIT"), this, IDC_EDIT_FACILKANA ), 
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_DUMMYFLAG ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_FACILCLASS ),
		m_wnd7( _T("CHECKBOX"), this, IDC_CHECK_AUTOPOSITION_F ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd9( _T("CHECK"), this, IDC_CHECK_DUMMY),
		m_wnd10( _T("COMBOBOX"), this, IDC_COMBO_SEARCHFACIL )
{
	// フォント作成
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		AheLoadString(IDS_FONT));   // フォント種類
}


/** 
 * @fn HWND CAttrFacilInfoPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrFacilInfoPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrFacilInfoPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrFacilInfoPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn void CAttrFacilInfoPointDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASSにArcHelperを渡します、また、HwyHelperにもArcHelperをセットします。
 */
void CAttrFacilInfoPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
	m_cHwyHelper.SetArcHelper( ipApp ); 
}

/**
 * @fn BOOL CAttrFacilInfoPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrFacilInfoPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 路線コード、路線内シーケンスの初期値確保
	CComVariant		vaRoadSequence;
	CFieldDef*		pFieldDef;
	CComVariant		vaValue;	
 	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_SEQ") );
	vaValue= pFieldDef->m_vaValue;
	m_lOrgRoadSequence = vaValue.lVal;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;

	// 最初から路線コードを出すため
	SetLineClassList(m_lOrgRoadCode);

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_ROADCLASS_REF) );
	}

	return bRet;
}

/**
 * @fn void CAttrFacilInfoPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrFacilInfoPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 路線種別参照用
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF,	TYPE_COMBO,		0,	0 );
		// 路線名称参照用
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF,	TYPE_COMBO,		0,	0 );
		// 路線コード
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE,		TYPE_EDIT, 		0,	0, _T("ROAD_CODE"),	NULL, FACILINFO_TABLE_NAME , NULL, -1, FOLLOW_READONLY );
		// 路線内シーケンス参照用 
		m_cControlRel.SetControl( IDC_EDIT_SEQUENCE,		TYPE_EDIT,		0,	0, _T("ROAD_SEQ"),		NULL, FACILINFO_TABLE_NAME);
		// 施設漢字名称
		m_cControlRel.SetControl( IDC_EDIT_FACILKANJI,		TYPE_EDITSTRING,0,	0, _T("NAME_KANJI"),	NULL, FACILINFO_TABLE_NAME);
		// 施設カナ名称
		m_cControlRel.SetControl( IDC_EDIT_FACILKANA,		TYPE_EDITSTRING,0,	0, _T("NAME_YOMI"),		NULL, FACILINFO_TABLE_NAME);
		// ダミー料金所
		m_cControlRel.SetControl( IDC_CHECK_DUMMYFLAG,		TYPE_CHECKBOX,	0,	0, _T("DUMMYNAME_F"),	NULL, FACILINFO_TABLE_NAME);
		// 施設種別コード
		m_cControlRel.SetControl( IDC_COMBO_FACILCLASS,		TYPE_COMBO,		0,	0, _T("FACILCLASS_C"),	NULL, FACILINFO_TABLE_NAME);
		// ダミー施設種別コード
		m_cControlRel.SetControl( IDC_COMBO_SEARCHFACIL,	TYPE_COMBO,		0,	0, _T("SEARCHCLASS_C"),	NULL, FACILINFO_TABLE_NAME);
		// 施設代表ポイント自動計算フラグ
		m_cControlRel.SetControl( IDC_CHECK_AUTOPOSITION_F,	TYPE_CHECKBOX,	0,	0, _T("AUTOPOSITION_F"),NULL, FACILINFO_TABLE_NAME);
		// ダミー施設フラグ
		m_cControlRel.SetControl( IDC_CHECK_DUMMY,			TYPE_CHECKBOX,	0,	0, _T("DUMMYFACIL_F"),NULL, FACILINFO_TABLE_NAME);
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,0,	0, _T("SOURCE"),		NULL, FACILINFO_TABLE_NAME );
	}	
}

/**
 * @fn bool CAttrFacilInfoPointDlg::SetLineClassList(LONG nLineClass)
 * @brief ダイアログが呼ばれたときに路線種別参照用のコンボボックスに値をセットする
 *
 * @param nLineClass	[in]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::SetLineClassList(LONG nLineClass)
{
	// 路線種別リストのコンボボックス
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// アイテムのクリア
	cbLineClass.ResetContent();

	// 路線コードテーブルを取得していなければ、取得する
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// 与えられた路線コードの路線の路線種別コードを取得
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;
	if( ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursorに格納されたRoad_Code_Listのレコードのうち、最初の１つからLineClass_cを取得
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// 路線種別コードのコード値ドメインを取得
	ATLASSERT(ipRoadCodeListTable);
	IFieldsPtr ipFields(ipRoadCodeListTable->Fields);
	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::road_code_list::kLineClass))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// コンボボックスにアイテムを追加
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long aValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString aName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbLineClass.AddString(aName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbLineClass.SetItemData(nIndex, aValue);
			if(aValue == aLineClass)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

	// カーソルの設定
	cbLineClass.SetCurSel(nCurIdx);
	// 現在の路線種別の取得
	m_nLineClass = cbLineClass.GetItemData(nCurIdx);

	CFieldDef* pFieldDef;
	CComVariant		vaRoadCode;	
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	vaRoadCode = pFieldDef->m_vaValue;

	RedrawRoadNameList(vaRoadCode.lVal);
	return true;

}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetLineClass(int* pClass)
 * @brief 設定されている路線種別を取得する
 *
 * @param pClass	[out]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::GetLineClass(int* pClass)
{
	if( !pClass )
		return false;
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// インデックスの取得
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;
	// 値の取得
	*pClass = cbLineClass.GetItemData(nIndex);
	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::SetLineClass(int nClass)
 * @brief 路線種別の値からコンボボックスを設定
 *
 * @param nClass	[in]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::SetLineClass(int nClass)
{
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// インデックスの計算
	int i = 0;
	int nMaxCount = cbLineClass.GetCount();
	for( ; i < nMaxCount; ++i)
	{
		int nLineClass = cbLineClass.GetItemData(i);
		if(nLineClass == nClass)
			break;
	}
	if(i == nMaxCount)
		return false;
	// コンボボックスの設定
	cbLineClass.SetCurSel(i);

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief 路線名称コンボボックスの更新
 *
 * @param nRoadCode	[in]	路線コードの値
 *
 * @return	路線名称が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// 路線名称コンボボックス
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_ROADNAME_REF));
	// コンボボックスの情報のクリア
	cbRoadName.ResetContent();
	// カナ名称でマップの作成
	std::map<CString, int> cYomiMap;

	// 路線コード情報の再読み込み
	if(!GetRoadCodeSet(&m_cRoadCodeSet))
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOGETROUTECODE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return 0;
	}
	for(CHetRoadCodeSet::const_iterator aIter = m_cRoadCodeSet.begin(); aIter != m_cRoadCodeSet.end(); ++aIter)
	{
		if(m_nLineClass == 0 || aIter->second.m_nLineClass == m_nLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter->second.m_strNameYomi, aIter->second.m_nRoadCode));
	}

	// リストボックスにアイテムの追加
	int nCurIdx = 0;
	for(std::map<CString, int>::const_iterator aIter2 = cYomiMap.begin(); aIter2 != cYomiMap.end(); ++aIter2)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2->second);
		if(pRoadCode)
		{
			int nIndex = cbRoadName.AddString(pRoadCode->m_strNameKanji);
			if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
			{
				cbRoadName.SetItemData(nIndex, pRoadCode->m_nRoadCode);
				if(pRoadCode->m_nRoadCode == nRoadCode)
					nCurIdx = nIndex;
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTENAME ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
			}
		}
	}
	// 現在のカーソルの設定
	cbRoadName.SetCurSel(nCurIdx);
	// 現在の路線コードの取得
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
 * @brief 路線コードエディットボックスから路線コードの取得
 *
 * @param nRoadCode	[out]	路線コードの値
 *
 * @return	路線コードが取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
{
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の取得
	TCHAR lpszRoadCode[32];
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// 数値に変換
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
 * @brief 路線コードエディットボックスから路線コードの取得
 *
 * @param nRoadCode	[out]	路線コードの値
 *
 * @return	路線コードが取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrFacilInfoPointDlg::SetRoadCode(int nRoadCode)
{
	// 文字列変換
	TCHAR lpszRoadCode[8];
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の設定
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;	
		SetButton( Changed() );
	}
	return true;
}


/**
 * @fn BOOL CAttrFacilInfoPointDlg::ErrorCheck()
 * @brief エラーがないかどうか調べる
 *
 * ダイアログを閉じる際に、AttributeDlgObjから呼ばれる
 * 施設情報名称と施設情報カナ名称がちゃんと対になって両方入っているか、または入っていないかたしかめる
 *
 * @return	エラーがなかったらTRUE、エラーがあればFALSE
 */
BOOL CAttrFacilInfoPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == FACILINFO_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( FACILINFO_TABLE_NAME, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( FACILINFO_TABLE_NAME, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_INVALIDFACILKANA ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDFACILKANJI ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}


/**
* @brief	路線コードテーブル取得
*/
ITablePtr CAttrFacilInfoPointDlg::GetRoadCodeTable()
{
	ITablePtr ipRoadCodeListTable;
	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return NULL;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return NULL;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LISTテーブル取得
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

/**
* @brief	路線コード取得
*/
bool CAttrFacilInfoPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	// テーブルの取得
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;
	// 行取得のパラメータの設定
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// 行の取得
	_ICursorPtr ipCursor;
	HRESULT hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor))){
		::MessageBox(m_hWnd, AheLoadString( IDS_FAILTOGETROW ), AheLoadString( IDS_CAPTION ), MB_OK);
		return false;
	}
	// 各行から情報の取得
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CHetRoadCode cRoadCode(ipRow);
		pCodeSet->Append(cRoadCode);
	}
	return true;
}
