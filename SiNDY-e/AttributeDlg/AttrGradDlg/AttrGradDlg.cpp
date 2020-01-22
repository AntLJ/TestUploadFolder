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

// AttrGradDlg.cpp: CAttrGradDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrGradDlg.h"

/// ウィンドウマネージャ配置マップ（傾斜用）
BEGIN_WINDOW_MAP(GRADMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_GRAD_STATIC1)
				RCTOFIT(-1)
				RCREST(IDC_GRAD_EDIT1)
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
 * @retval CAttrGradDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrGradDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrGradDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrGradDlg::CAttrGradDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrGradDlg::CAttrGradDlg() 
	: ATTR_BASE_CLASS(GRADMAP, IDD_ATTRGRADDLG),
		m_wnd0( _T("EDIT"), this, IDC_GRAD_EDIT1 ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrGradDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrGradDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrGradDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrGradDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrGradDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 2次メッシュを跨いで接している傾斜が選択されている場合には、関連する傾斜も確保する
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrGradDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 2次メッシュ境界のため追加
	m_pGradOnMeshList.clear();

	// 2次メッシュ境界跨いでいるかどうかフラグの初期化
	m_bIsRoadOnMesh = FALSE;

	IFeaturePtr	ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cTargetItems;
	cTargetItems.clear();

	// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続している傾斜を取得
	if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), ipFeature, cTargetItems ) )
	{
		// 2次メッシュを跨いでいるのでフラグ変更
		m_bIsRoadOnMesh = TRUE;

		// 選択された、2次メッシュ関連の道路を登録する
		for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
		{
			IFeaturePtr ipFeatureOnMesh = it->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pGradOnMeshList.push_back(featDef);
		}
	}

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_GRAD_EDIT1) );
	}

    return bRet;
}

/**
 * @fn void CAttrGradDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrGradDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 傾斜用

		// 傾斜値
		m_cControlRel.SetControl( IDC_GRAD_EDIT1,		TYPE_EDIT,		0,	0,	_T("GRAD_VALUE"),	NULL, GRAD_TABLE_NAME );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, GRAD_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrGradDlg::CheckGradValue()
 * @brief 傾斜値が正しくセットされているかチェック
 *
 * 傾斜値は-40,-30,-20,-10,0,10,20,30,40のいずれかでないといけません
 *
 * @return	正しい傾斜値であったらTRUE, 不正な傾斜値であったらFALSE 
 */
BOOL CAttrGradDlg::CheckGradValue()
{
	CEdit	cEditGrad( GetDlgItem(IDC_GRAD_EDIT1) );
	int		iLineLength = cEditGrad.LineLength();

	LPTSTR lpszGradValue = (LPTSTR)_alloca( ( iLineLength + 1 ) * sizeof(int) );
	cEditGrad.GetWindowText( lpszGradValue, iLineLength + 1 );

	LONG lGradValue = _ttol( lpszGradValue );

	CFeatureDef	*pFeatureDef;
	pFeatureDef = GetCurrentFeatureDef();
	
	if( lGradValue == -40 || lGradValue == -30 || lGradValue == -20 || lGradValue == -10 || lGradValue == 0 ||
		lGradValue == 10 || lGradValue == 20 || lGradValue == 30 || lGradValue == 40)
	{
		pFeatureDef->m_bError = FALSE;
		return TRUE;
	}

	m_bErrorCheck = TRUE;

	AttrMessageBox( AheLoadString( IDS_GRADRANGE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, WARNLEVEL_HIGH );

	m_bErrorCheck = FALSE;

	pFeatureDef->m_bError = TRUE;
	return FALSE;
}

/**
 * @fn BOOL CAttrGradDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	INT		nClassID	[i]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrGradDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
	// このコピーだけはやるしかないのでこれを消しては絶対にいけない！
	CopyMeshGrad();

	if( !CheckGradValue() )
		return FALSE;
	return TRUE;
}
void CAttrGradDlg::CopyMeshGrad()
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
			for( std::list<CFeatureDef>::iterator iAttr = m_pGradOnMeshList.begin(); iAttr != m_pGradOnMeshList.end(); iAttr++ )
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
					if( strFieldName == _T("GRAD_VALUE") || strFieldName == _T("SOURCE") )
					{
						// 上記のif文の条件以外はコピーする
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( GRAD_TABLE_NAME, itField->m_strFieldName );
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
}
/**
 * @fn BOOL CAttrGradDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESCキーが押された時の動作
 *
 * m_bErrorCheckをTRUEにしておかないと、ESCキーを押してダイアログを閉じるときに、エラーチェックが入ってしまうためにここで制御
 * 
 * @param	nClassID	[in]		ESCキーが押された時のItemID
 *
 * @return	必ずTRUEが返る（意味のない値）
 */
BOOL CAttrGradDlg::CheckEscKeyItem(INT nClassID)
{
	m_bErrorCheck = TRUE;	
	return TRUE;
}

/**
 * @fn BOOL CAttrRoadLinkDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときに道路リンクのエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrGradDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == GRAD_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaGradValue;
			pFieldDef = it->GetFieldDef( GRAD_TABLE_NAME, _T("GRAD_VALUE") );

			LONG lGradValue = pFieldDef->m_vaValue.lVal;

			// フィーチャとオブジェクトID取得
			IFeaturePtr ipFeature(it->GetFeature());
			LONG lOID = 0;
			ipFeature->get_OID( &lOID );

			if( !(lGradValue == -40 || lGradValue == -30 || lGradValue == -20 || lGradValue == -10 || lGradValue == 0 ||
				lGradValue == 10 || lGradValue == 20 || lGradValue == 30 || lGradValue == 40) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_GRADRANGE2 ) ,lOID );
				AttrMessageBox( strMessage,  AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, WARNLEVEL_HIGH );
				return FALSE;
			}
		}
	}
	
	return TRUE;
}
