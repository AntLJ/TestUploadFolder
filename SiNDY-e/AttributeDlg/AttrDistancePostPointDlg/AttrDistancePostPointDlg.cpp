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

// AttrDistancePostPointDlg.cpp: CAttrDistancePostPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "sindy/libschema.h"
#include "AttrDistancePostPointDlg.h"

/// ウィンドウマネージャ配置マップ（高速・有料道路検索関係ポイント用）
BEGIN_WINDOW_MAP(DISTANCE_POST_POINT_MAP)
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
				RCTOFIT(IDC_STATIC_DIRECTIONCODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_DIRECTIONCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KILOPOST)
				RCTOFIT(-1)
				RCFIXED(IDC_EDIT_KILOPOST2,20)
				RCTOFIT(IDC_EDIT_KILOPOST)
				RCTOFIT(IDC_STATIC_KILOPOST2)
			ENDGROUP()
			RCSPACE(5)
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
 * @retval CAttrDistancePostPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrDistancePostPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrDistancePostPointDlg::CAttrDistancePostPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化、さらにタブ位置、フォントタイプの初期化をしておきます。
 */
CAttrDistancePostPointDlg::CAttrDistancePostPointDlg()
	: ATTR_BASE_CLASS(DISTANCE_POST_POINT_MAP, IDD_ATTRDISTANCEPOSTPOINTDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_DIRECTIONCODE ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KILOPOST ), 
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_KILOPOST2 ), 
		m_lOrgRoadCode(-1),
		m_lOrgDirCode(-1),
		m_nLineClass(-1),
		m_nRoadCode(-1),
		m_nDirCode(-1),
		m_hChildWnd(NULL)
{
}

// 子ダイアログを作成します。
HWND CAttrDistancePostPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

// 作成した子ダイアログを削除します。
void CAttrDistancePostPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

// ATTR_BASE_CLASSにArcHelperを渡します、また、HwyHelperにもArcHelperをセットします。
void CAttrDistancePostPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

// 現在表示されるべきフィーチャのFeatureDefインデックスを渡します
BOOL CAttrDistancePostPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 路線コードの初期値確保
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	CComVariant vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;

	// 方向コードの初期値確保
	CFieldDef* pFieldDef2 = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_direction_table::kDirection );
	CComVariant vaValueDir = pFieldDef2->m_vaValue;
	m_lOrgDirCode = vaValueDir.lVal;

	// 最初から路線コードを出すため
	SetLineClassList(m_lOrgRoadCode);
	SetDirList(m_lOrgDirCode);

	// 路線コードの設定
	if(! SetRoadCode(m_nRoadCode))
		return 0;

	CFieldDef* pFieldDef3 = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::distance_post_point::kDistance );
	CComVariant vaDistance = pFieldDef3->m_vaValue;
	CEdit edtDistance = CEdit(GetDlgItem(IDC_EDIT_KILOPOST));

	CString strTmp;
	strTmp.Format( _T("%.1f"), vaDistance.fltVal );
	edtDistance.SetWindowText(strTmp);

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_ROADCLASS_REF) );
	}

	return bRet;
}

// エラーがないかどうか調べる
BOOL CAttrDistancePostPointDlg::ErrorCheck()
{
	// コントロールの内容チェック
	for ( const auto& ctl : m_cControlRel )
	{
		if ( IDC_EDIT_KILOPOST == ctl.m_nID )
		{
			CString strVal;
			::GetWindowText( ::GetDlgItem(m_hWnd, ctl.m_nID), strVal.GetBuffer(1026), 1026);
			strVal.ReleaseBuffer();

			CString strRet = strVal.SpanIncluding(_T("0123456789."));
			if(strVal != strRet)
			{
				CString strMessage;
				LONG lOID = 0;
				ctl.m_pFeatureDef->GetFeature()->get_OID( &lOID );
				strMessage.Format( AheLoadString( IDS_ERROR_DISTANCE2 ), lOID );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	// 格納されている内容もチェック
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == sindy::schema::distance_post_point::kTableName )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaDistance;
			pFieldDef = it->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::distance_post_point::kDistance );
			vaDistance = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaDistance.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERROR_DISTANCE ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else
			{
				CString strTmp;
				strTmp.Format( _T("%.1f"), vaDistance.fltVal );
				pFieldDef->m_vaValue = CComVariant( (float)_tstof(strTmp) );
			}
		}
	}

	return TRUE;
}

// 子ダイアログを実際に作成します。
void CAttrDistancePostPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// 路線種別参照用
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF, TYPE_COMBO, 0, 0 );
		// 路線名称参照用
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF, TYPE_COMBO, 0, 0 );
		// 路線コード
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE, TYPE_EDIT, 0,	0, sindy::schema::category::highway_road_relational_table::kRoadCode, NULL, sindy::schema::distance_post_point::kTableName, NULL, -1, FOLLOW_READONLY );
		// 方向コード
		m_cControlRel.SetControl( IDC_COMBO_DIRECTIONCODE, TYPE_COMBO, 0, 0, sindy::schema::category::highway_direction_table::kDirection, NULL, sindy::schema::distance_post_point::kTableName );
		// 距離標値
		m_cControlRel.SetControl( IDC_EDIT_KILOPOST, TYPE_EDITFLOAT, 0, 0, sindy::schema::distance_post_point::kDistance, NULL, sindy::schema::distance_post_point::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, sindy::schema::ipc_table::kSource, NULL, sindy::schema::distance_post_point::kTableName );
		// 距離標値(文字列)
		m_cControlRel.SetControl( IDC_EDIT_KILOPOST2, TYPE_EDITSTRING, 0, 0, sindy::schema::distance_post_point::kDistanceMark, NULL, sindy::schema::distance_post_point::kTableName );
	}
}

// ダイアログが呼ばれたときに路線種別参照用のコンボボックスに値をセットする
bool CAttrDistancePostPointDlg::SetLineClassList(int nLineClass)
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
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("%s = %ld"), sindy::schema::category::highway_road_relational_table::kRoadCode, nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );

	_ICursorPtr ipCursor;
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	long aLineClass = -1;
	if( ipCursor )
	{
		_IRowPtr ipRow;
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursorに格納されたRoad_Code_Listのレコードのうち、最初の１つからLineClass_cを取得
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}
	// TODO:aLineClassの値を確認する処理を追加する

	// 路線種別コードのコード値ドメインを取得
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

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	CComVariant vaRoadCode = pFieldDef->m_vaValue;

	RedrawRoadNameList(vaRoadCode.lVal);
	return true;
}

// 設定されている路線種別を取得する
bool CAttrDistancePostPointDlg::GetLineClass(int* pClass)
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

// 路線種別の値からコンボボックスを設定
bool CAttrDistancePostPointDlg::SetLineClass(int nClass)
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

// ダイアログが呼ばれたときに方向コード用のコンボボックスに値をセットする
void CAttrDistancePostPointDlg::SetDirList(int nDir)
{
	// 路線種別リストのコンボボックス
	CComboBox cbDir = CComboBox(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
	// アイテムのクリア
	cbDir.ResetContent();

	// 方向コードのコード値ドメインを取得
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return;

	IFieldsPtr ipFields;
	ipCurrentFeature->get_Fields(&ipFields);

	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::highway_node::kDirection))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// コンボボックスにアイテムを追加
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long nValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString sName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbDir.AddString(sName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbDir.SetItemData(nIndex, nValue);
			if(nValue == nDir)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return;
		}
	}

	// カーソルの設定
	cbDir.SetCurSel(nCurIdx);
	// 現在の路線種別の取得
	m_nDirCode = cbDir.GetItemData(nCurIdx);
}

// 路線名称コンボボックスの更新
bool CAttrDistancePostPointDlg::RedrawRoadNameList(int nRoadCode)
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

	for(const auto& aIter : m_cRoadCodeSet)
	{
		if(m_nLineClass == 0 || aIter.second.m_nLineClass == m_nLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter.second.m_strNameYomi, aIter.second.m_nRoadCode));
	}

	// リストボックスにアイテムの追加
	int nCurIdx = 0;
	for(const auto& aIter2 : cYomiMap)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2.second);
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
				// TODO: ここでreturnしなくて大丈夫か確認
			}
		}
	}

	// 現在のカーソルの設定
	cbRoadName.SetCurSel(nCurIdx);
	// 現在の路線コードの取得
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);

	return true;
}

// 路線コードエディットボックスから路線コードの取得
bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
{
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の取得
	TCHAR lpszRoadCode[32] = {};
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// 数値に変換
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}

// 路線コードエディットボックスから路線コードの取得
bool CAttrDistancePostPointDlg::SetRoadCode(int nRoadCode)
{
	// 文字列変換
	TCHAR lpszRoadCode[8] = {};
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の設定
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	pFieldDef->m_vaValue = nRoadCode;
	if(nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;	
		SetButton( Changed() );
	}
	return true;
}

// 路線コードテーブル取得
ITablePtr CAttrDistancePostPointDlg::GetRoadCodeTable()
{
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return NULL;

	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );

	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return NULL;

	IWorkspacePtr ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LISTテーブル取得
	// TODO: AheOpenSameOwnerTable()を使うようにする
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

// 路線コード取得
bool CAttrDistancePostPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	// テーブルの取得
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// 行取得のパラメータの設定
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);

	// 行の取得
	_ICursorPtr ipCursor;
	HRESULT hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_TRUE, &ipCursor))){
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
