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

// AttrHeightLogDlg.cpp : DLL エクスポートの実装です。


#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "resource.h"
#include "AttrHeightLogDlg.h"
#include "winlib/winutil.h"

BEGIN_WINDOW_MAP(HEIGHT_LOG_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_Z10)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_Z10)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ZORIG)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_ZORIG)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_RUNWAY)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_RUNWAY)
				ENDGROUP()
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(-1 )
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_UPDOWN)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_UPDOWN)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ONOFF)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_ONOFF)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_ROADNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_ROADNAME)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_SPOTNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_SPOTNAME)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_MASTERNAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_MASTERNAME)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_FILENAME)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_FILENAME)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_CONTAIN)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_CONTAIN)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_TOFIT, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_MEASURE)
					RCTOFIT(-1)
					RCREST(IDC_COMBO_MEASURE)
				ENDGROUP()
			RCSPACE(5)
				BEGINCOLS(WRCT_PCT, 0, 50)
					RCTOFIT(IDC_STATIC_DATE)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_DATE)
				ENDGROUP()
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS( WRCT_REST, 0, RCMARGINS(2,2) )
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_REMARKS)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_REMARKS)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrHeightLogDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHeightLogDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightLogDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHeightLogDlg::CAttrHeightLogDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrHeightLogDlg::CAttrHeightLogDlg()
	: ATTR_BASE_CLASS(HEIGHT_LOG_MAP, IDD_ATTRHEIGHTLOGDLG)
{
}

/** 
 * @fn HWND CAttrHeightLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrHeightLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHeightLogDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrHeightLogDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrHeightLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrHeightLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	SetDlgStatus( IDC_COMBO_RUNWAY, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_ROADNAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_SPOTNAME, FALSE, TRUE );
	SetDlgStatus( IDC_COMBO_UPDOWN, FALSE, FALSE );
	SetDlgStatus( IDC_COMBO_ONOFF, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_MASTERNAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_FILENAME, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_CONTAIN, FALSE, TRUE );
	SetDlgStatus( IDC_COMBO_MEASURE, FALSE, FALSE );
	SetDlgStatus( IDC_EDIT_DATE, FALSE, TRUE );
	SetDlgStatus( IDC_EDIT_REMARKS, FALSE, TRUE );
	SetMasterValue();
	return bRet;
}

/**
 * @fn void CAttrHeightLogDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrHeightLogDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_EDIT_Z10,		TYPE_EDITSTRING, 	0,	0,	_T("Z_10"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_EDIT_ZORIG,	TYPE_EDITSTRING, 	0,	0,	_T("Z_ORIG"),	NULL, m_strFeatureClassName );
	}
}

void CAttrHeightLogDlg::SetMasterValue()
{
	_IRowPtr ipRow = GetCurrentFeatureDef()->GetFeature();
	// つないでいるワークスペースのテーブルを取得する
	if( ipRow )
	{
		IWorkspacePtr ipWorkspace = AheGetWorkspace( ipRow );
		ITablePtr ipTable = AheOpenTableByTableName( ipWorkspace, _T("HEIGHT_LOG_MASTER") );
		if( ipTable )
		{
			_IRowPtr ipMasterRow;
			ipTable->GetRow( GetCurrentFeatureDef()->GetFieldDef( m_strFeatureClassName, _T("MASTER_ID") )->m_vaValue.lVal, &ipMasterRow );
			if( ipMasterRow )
			{
				SetValueToCombo( IDC_COMBO_RUNWAY, GetDisplayValue( ipMasterRow, _T("RUNWAY_C") ) );
				SetValueToEdit( IDC_EDIT_ROADNAME, GetDisplayValue( ipMasterRow, _T("ROAD_NAME") ) );
				SetValueToEdit( IDC_EDIT_SPOTNAME, GetDisplayValue( ipMasterRow, _T("SPOT_NAME") ) );
				SetValueToCombo( IDC_COMBO_UPDOWN, GetDisplayValue( ipMasterRow, _T("UP_DOWN_C") ) );
				SetValueToCombo( IDC_COMBO_ONOFF, GetDisplayValue( ipMasterRow, _T("ON_OFF_C") ) );
				SetValueToEdit( IDC_EDIT_MASTERNAME, GetDisplayValue( ipMasterRow, _T("MASTER_NAME") ) );
				SetValueToEdit( IDC_EDIT_FILENAME, GetDisplayValue( ipMasterRow, _T("FILENAME") ) );
				SetValueToEdit( IDC_EDIT_CONTAIN, GetDisplayValue( ipMasterRow, _T("CONTAIN") ) );
				SetValueToCombo( IDC_COMBO_MEASURE, GetDisplayValue( ipMasterRow, _T("MEASURE_C") ) );
				SetValueToEdit( IDC_EDIT_DATE, GetDisplayValue( ipMasterRow, _T("DELIVERY_DATE") ) );
				SetValueToEdit( IDC_EDIT_REMARKS, GetDisplayValue( ipMasterRow, _T("REMARKS") ) );
			}
		}
	}
}

void CAttrHeightLogDlg::SetValueToCombo( int nID, CString strValue )
{
	CComboBox cCombo = GetDlgItem( nID );
	cCombo.ResetContent();
	cCombo.SetCurSel( cCombo.AddString( strValue ) );
}

void CAttrHeightLogDlg::SetValueToEdit( int nID, CString strValue )
{
	CEdit cEdit = GetDlgItem( nID );
	cEdit.SetWindowText( strValue );
}

CString CAttrHeightLogDlg::GetDisplayValue( _IRowPtr ipRow, CString strFieldName )
{
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );
	long lIndex = 0;
	ipFields->FindField( _bstr_t( strFieldName ), &lIndex );
	CComVariant vaValue;
	ipRow->get_Value( lIndex, &vaValue );

	IFieldPtr ipField;
	ipFields->get_Field( lIndex, &ipField );
	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	CString strValue;
	if( ipDomain )
	{
		ICodedValueDomainPtr ipCodedValueDomain = ipDomain;
		long lCount = 0;
		ipCodedValueDomain->get_CodeCount( &lCount );
		for( int i = 0; i < lCount; ++i )
		{
			CComVariant vaCodeValue;
			ipCodedValueDomain->get_Value( i, &vaCodeValue );
			if( vaCodeValue.lVal == vaValue.lVal )
			{
				CComBSTR bstrValue;
				ipCodedValueDomain->get_Name( i, &bstrValue );
				strValue = bstrValue;
				break;
			}
		}
	}
	else
	{
		if( vaValue.vt == VT_BSTR )
			strValue = vaValue.bstrVal;
		else if( vaValue.vt == VT_DATE )
		{
			vaValue.ChangeType( VT_BSTR );
			strValue = vaValue.bstrVal;
		}
	}
	return strValue;
}
