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

// AttrSegmentAttrDlg.cpp: CAttrSegmentAttrDlg クラスのインプリメンテーション

#include "stdafx.h"
#include "AttrSegmentAttrDlg.h"

using namespace sindy::schema;

/// ウィンドウマネージャ配置マップ（リンク内属性用）
BEGIN_WINDOW_MAP(SEGMENT_ATTR_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_CHECK1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT3)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC4)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_COMBO1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC5)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT4)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_SEGMENT_ATTR_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_SEGMENT_ATTR_EDIT5)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,10)
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
 * @retval CAttrSegmentAttrDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSegmentAttrDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSegmentAttrDlg::CAttrSegmentAttrDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrSegmentAttrDlg::CAttrSegmentAttrDlg()
	: ATTR_BASE_CLASS(SEGMENT_ATTR_MAP, IDD_ATTRSEGMENTATTRDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_SEGMENT_ATTR_CHECK1, CContainedWindow( _T("CHECKBOX"), this, IDC_SEGMENT_ATTR_CHECK1))
		(IDC_SEGMENT_ATTR_EDIT2,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT2 ))
		(IDC_SEGMENT_ATTR_EDIT3,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT3 ))
		(IDC_SEGMENT_ATTR_COMBO1, CContainedWindow( _T("COMBO"),    this, IDC_SEGMENT_ATTR_COMBO1 ))
		(IDC_SEGMENT_ATTR_EDIT4,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT4 ))
		(IDC_SEGMENT_ATTR_EDIT5,  CContainedWindow( _T("EDIT"),     this, IDC_SEGMENT_ATTR_EDIT5 ))
		(IDC_EDIT_SOURCE,         CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SOURCE ));
}

/** 
 * @fn HWND CAttrSegmentAttrDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrSegmentAttrDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSegmentAttrDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrSegmentAttrDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrSegmentAttrDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 2次メッシュを跨いで接しているリンク内属性が選択されている場合には、関連するリンク内属性も確保する
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrSegmentAttrDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 2次メッシュ境界のため追加
	m_pAttrLinkOnMeshList.clear();

	// 2次メッシュ境界跨いでいるかどうかフラグの初期化
	m_bIsRoadOnMesh = FALSE;

	IFeaturePtr	ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cTargetItems;
	cTargetItems.clear();

	// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続しているリンク内属性を取得
	if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), ipFeature, cTargetItems ) == TRUE )
	{
		// 2次メッシュを跨いでいるのでフラグ変更
		m_bIsRoadOnMesh = TRUE;

		// 自分の種別
		long attrCode = AheGetAttribute( GetCurrentFeatureDef()->GetFeature(), segment_attr::kLinkAttr ).lVal;

		// 選択された、2次メッシュ関連の道路を登録する
		for( const auto& onMesh : cTargetItems )
		{
			IFeaturePtr ipFeatureOnMesh = onMesh.GetFeature();

			// 同じ種別のものしか相手にしない（bug 11641）
			if( attrCode != AheGetAttribute(ipFeatureOnMesh, segment_attr::kLinkAttr).lVal )
				continue;

			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pAttrLinkOnMeshList.push_back(featDef);
		}
	}

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_SEGMENT_ATTR_COMBO1) );
	}

	// もしNAMEフィールドがないならエディットボックス等は非表示にする
	AdjustLayout();

	// 冠水注意地点以外ならNAMEフィールドは編集不可
	SetNameFieldEditable();

	return bRet;
}

/**
 * @fn void CAttrSegmentAttrDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrSegmentAttrDlg::CreateControlRelation()
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

		// リンク内属性用

		// 現地調査フラグ
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_CHECK1, TYPE_CHECKBOX, 0, 0, segment_attr::kFieldSurvey,NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY  );
		// 作業年度
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT2, TYPE_EDITSTRING, 0, 0, segment_attr::kWorkYear, NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY  );
		// 対応リンクID
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT3, TYPE_EDIT, 0, 0, segment_attr::kLinkID, NULL, segment_attr::kTableName, NULL, -1, FOLLOW_READONLY );
		// リンク内種別
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_COMBO1, TYPE_COMBO, 0, 0, segment_attr::kLinkAttr, NULL, segment_attr::kTableName );
		// 上下関係
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT4, TYPE_EDIT, 0, 0, segment_attr::kUnderOver, NULL, segment_attr::kTableName );
		// 名称
		m_cControlRel.SetControl( IDC_SEGMENT_ATTR_EDIT5, TYPE_EDITSTRING, 0, 0, segment_attr::kName, NULL, segment_attr::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, segment_attr::kSource, NULL, segment_attr::kTableName );
	}
}



void CAttrSegmentAttrDlg::SetLinkAttrComboBox()
{
	// コンボボックス取得
	CComboBox	cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );

	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return;

	// SEGMENTATTR_TABLE_NAME取得
	ITablePtr			ipTable;
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = segment_attr::kTableName;
	else
		strOwnerName.AppendFormat( _T(".%s"), segment_attr::kTableName );

	ipTable = AheOpenTableByTableName ( ipWorkspace, strOwnerName );

	if( !ipTable ) {
		AttrMessageBox( AheLoadString( IDS_FAILTOGETTABLE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// リンクID取得
	CFieldDef	*cFieldDef = GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kLinkID );
	if( !cFieldDef )
		return;
	LONG lLink_ID = cFieldDef->m_vaValue.lVal;
	// テーブルから検索
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	_ICursorPtr ipCursor;
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), segment_attr::kLinkID, lLink_ID );
	strSubFields.Format(_T("OBJECTID,Shape"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	if( !ipCursor )
		return;

	// Geometry取得
	IFeaturePtr	ipFeature;
	IGeometryPtr ipCurrentGeom;
	ipFeature = GetCurrentFeatureDef()->GetFeature();
	ipFeature->get_ShapeCopy( &ipCurrentGeom );
	LONG lCurrentOID;
	ipFeature->get_OID( &lCurrentOID );

	_IRowPtr ipRow;
    while( ipCursor->NextRow( &ipRow ) == S_OK )
    {
        if( !ipRow ) 
			break;

		IFeaturePtr ipFeatureOnSameLink = ipRow;
		LONG lOIDOnSameLink = 0;
		ipFeatureOnSameLink->get_OID( &lOIDOnSameLink );

		if( lCurrentOID == lOIDOnSameLink )
			continue;

		IGeometryPtr ipGeomOnSameLink;
		ipFeatureOnSameLink->get_ShapeCopy( &ipGeomOnSameLink );

		// オーバーラップしているリンク内属性のリンク種別をコンボボックスからはずす
		if( AheIsOverlap2( ipCurrentGeom, ipGeomOnSameLink ) )
		{
			IFieldsPtr ipFields;
			ipFeatureOnSameLink->get_Fields( &ipFields );

			LONG	lIndex = 0;
			CComVariant	vaValue;
			ipFields->FindField( CComBSTR( segment_attr::kLinkAttr ), &lIndex );
			ipFeatureOnSameLink->get_Value( lIndex, &vaValue );

			// 現在保持しているリストの中に対象のリンク内属性があれば、それと交換する
			// もしなければ、Cursorから拾ってきたものでよい
			for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
			{
				CString strTableName = (*it).GetTableName();
				CFieldDef* cFieldDefOID = (*it).GetFieldDef( segment_attr::kTableName, _T("OBJECTID") );
				// [Bug 5875]オーバーラップしているリンク内属性を選択すると落ちるバグ対応
				// リンク内属性以外をつかんでいる場合に落ちてしまっていた
				if( cFieldDefOID )
				{
					long lOID = cFieldDefOID->m_vaValue.lVal;
					if( strTableName == segment_attr::kTableName && lOID == lOIDOnSameLink )
					{
						// 現在の値と置き換える
						CFieldDef* cFieldDef = (*it).GetFieldDef( segment_attr::kTableName, segment_attr::kLinkAttr );
						vaValue = cFieldDef->m_vaValue;
					}
				}
			}

			LONG lLinkAttr_c = vaValue.lVal;
			LONG lCount = cCombo.GetCount();

			for( INT i = 0 ; i < lCount; i++ )
			{
				LONG lSetItemData = cCombo.GetItemData( i );

				if( lSetItemData == lLinkAttr_c )
					cCombo.DeleteString( i );
			}
		}
	}
}

/**
 * @fn BOOL CAttrSegmentAttrDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	INT		nClassID	[i]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrSegmentAttrDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
	// 以下の処理を行う
	if( m_bIsRoadOnMesh )
	{
		// とりあえず、現在保持しているリストすべてにおいてループを行う
		for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			// 現在選択中の2次メッシュを跨いでいる道路リンクに関連しているリストとの照合
			CFeatureDef *pFeatureDef = NULL;
			for( std::list<CFeatureDef>::iterator iAttr = m_pAttrLinkOnMeshList.begin(); iAttr != m_pAttrLinkOnMeshList.end(); iAttr++ )
			{
				if( iAttr->GetFeature() == it->GetFeature() )
				{
					pFeatureDef = &(*it);
					break;
				}
			}

			if( pFeatureDef )
			{
				// すべてのフィールド値をチェック
				for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
				{
					CString strFieldName = itField->m_strFieldName;

					// 以下内容をコピーする
					if( 0 == strFieldName.CompareNoCase( segment_attr::kLinkAttr ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kUnderOver ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kName ) ||
						0 == strFieldName.CompareNoCase( segment_attr::kSource ) )
					{
						// 上記のif文の条件以外はコピーする
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( segment_attr::kTableName, itField->m_strFieldName );
						VARIANT vaValue;
						pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
						if( itField->m_vaValue != vaValue )
						{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = TRUE;
						}
						else{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = FALSE;
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CAttrSegmentAttrDlg::ErrorCheck()
{
	// そもそもNAMEフィールドないなら何もしない(ASEANとか)
	if( !GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return TRUE;

	// 「冠水注意地点」以外が名称を持っていてはいけない
	// 将来的には他の種別でも名称持つようになるかも

	// コンボボックス取得
	CComboBox cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );
	int selectedIndex = cCombo.GetCurSel();
	DWORD selectedValue = cCombo.GetItemData( selectedIndex );
	if( (long)selectedValue == sindy::schema::segment_attr::link_attr::kFlooding )
	{
		return TRUE;
	}

	// 名称がNULLはOK
	CString name;
	GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ).GetWindowText( name );
	if( name.IsEmpty() )
	{
		return TRUE;
	}

	CComBSTR selectedText;
	cCombo.GetLBTextBSTR( selectedIndex, (BSTR&)selectedText );
	CString errorMessage;
	errorMessage.Format( AheLoadString( IDS_NAMENOTNULL ), selectedText );
	AheMessageBox( errorMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH );

	return FALSE;
}

void CAttrSegmentAttrDlg::SetNameFieldEditable()
{
	// そもそもNAMEフィールドないなら何もしない(ASEANとか)
	if( !GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return;

	if( IsNameFieldEditableAttr() )
	{
		::EnableWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), TRUE );
	}
	else
	{
		::EnableWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), FALSE );
	}
}

bool CAttrSegmentAttrDlg::IsNameFieldEditableAttr()
{
	// コンボボックス取得
	CComboBox cCombo( GetDlgItem( IDC_SEGMENT_ATTR_COMBO1) );
	int selectIndex = cCombo.GetCurSel();
	DWORD selectedValue = cCombo.GetItemData( selectIndex );

	// 冠水注意地点以外ならNAMEフィールドは編集不可
	// NAMEフィールド編集可の種別は将来的に増えるかも
	if( (long)selectedValue == sindy::schema::segment_attr::link_attr::kFlooding )
	{
		return true;
	}

	return false;
}

void CAttrSegmentAttrDlg::AdjustLayout()
{
	WINRECT *map = NULL; // WinMgr制御のコントロール取得用

	// NAMEフィールドがあるかどうか
	if( GetCurrentFeatureDef()->GetFieldDef( segment_attr::kTableName, segment_attr::kName ) )
		return;

	// フィールドがない時はエディットボックスとスタティックコントロールは非表示
	::ShowWindow( GetDlgItem( IDC_SEGMENT_ATTR_STATIC6 ), SW_HIDE );
	::ShowWindow( GetDlgItem( IDC_SEGMENT_ATTR_EDIT5 ), SW_HIDE );
	map = m_winMgr.FindRect( IDC_SEGMENT_ATTR_EDIT5 );

	if( map )
	{
		// 再配置
		m_winMgr.InitToFitSizeFromCurrent(this);
		m_winMgr.CalcLayout(this);
		m_winMgr.SetWindowPositions(this);	
	}
}
