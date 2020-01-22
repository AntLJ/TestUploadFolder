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

// AttrSJPolygonDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrSJPolygonDlg.h"
#include <boost/assign/list_of.hpp>

const double  EARTH_R  = 6370.0;

using namespace sindy::schema;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINROWS(WRCT_REST,0,0)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANDCOVER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANDCOVER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANDUSE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANDUSE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_ROADFLAG)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCREST(IDC_STATIC_AREA)
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
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrSJPolygonDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSJPolygonDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSJPolygonDlg::CAttrSJPolygonDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrSJPolygonDlg::CAttrSJPolygonDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_SINDYCHILDDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_LANDCOVER, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LANDCOVER ) )
		( IDC_COMBO_LANDUSE, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LANDUSE ) )
		( IDC_CHECK_ROADFLAG, CContainedWindow(  _T("CHECK"), this, IDC_CHECK_ROADFLAG ) );
}

/** 
 * @fn HWND CAttrSJPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrSJPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrSJPolygonDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrSJPolygonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrSJPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrSJPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ROAD_POLYGONならば、ROAD_Fのチェックボックス表示、それ以外ならば非表示
	CWindow cRoadCheckBox( GetDlgItem( IDC_CHECK_ROADFLAG ) );
	if( m_strClassName == sj::kRoadTableName )
		cRoadCheckBox.ShowWindow( SW_SHOW );
	else
		cRoadCheckBox.ShowWindow( SW_HIDE );

	// RAILWAY_POLYGONならば必ずコンボボックスDISABLE
	if( m_strClassName == sj::kRailwayTableName )
	{
		SetDlgStatus( IDC_COMBO_LANDCOVER, false );
		SetDlgStatus( IDC_COMBO_LANDUSE, false );
	}

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// ROAD_POLYGONならLCCLS、LUCLSのコンボボックスの制御
		if( m_strClassName == sj::kRoadTableName )
			SetRoadFlag( (bool)(GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kRoadF )->m_vaValue.lVal), false );

		// 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// 面積表示（ArcHelperあたりにあってもいいかも）
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if( ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		if( ipGeom )
		{
			double dArea = 0;
			((IAreaPtr)ipGeom)->get_Area(&dArea);
			IPointPtr ipPoint(CLSID_Point);
			((IAreaPtr)ipGeom)->QueryCentroid(ipPoint);
			ipPoint->putref_SpatialReference( AheGetSpatialReference( ipGeom ) );
			double dY = 0;
			ipPoint->get_Y(&dY);
			// 現在の緯度と赤道との割合を求める
			double dRatio = EARTH_R * sin(M_PI / 2.0 - dY * M_PI / 180.0) * 2.0 * M_PI/360.0;
			dArea *= fabs(dRatio * EARTH_R * 2.0 * M_PI / 360.0);
			dArea *= 1000000.0;
			CString strArea;
			strArea.Format( _T("AERA : %.2lfm2"), dArea );
			CStatic cStatic( GetDlgItem( IDC_STATIC_AREA ) );
			cStatic.SetWindowText(strArea );
		}
	}
    return bRet;
}

/**
 * @fn void CAttrSJPolygonDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrSJPolygonDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// LAND_COVER
		m_cControlRel.SetControl( IDC_COMBO_LANDCOVER, TYPE_COMBO, 0, 0, sj::kLCCLS, 0, m_strClassName );
		// LAND_USE
		m_cControlRel.SetControl( IDC_COMBO_LANDUSE, TYPE_COMBO, 0, 0, sj::kLUCLS, 0, m_strClassName );
		// ROAD_F
		if( m_strClassName == sj::kRoadTableName )
			m_cControlRel.SetControl( IDC_CHECK_ROADFLAG, TYPE_CHECKBOX, 0, 0, sj::kRoadF, 0, m_strClassName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, m_strClassName );
	}
}

// 道路フラグのチェックボックス管理関数
void CAttrSJPolygonDlg::SetRoadFlag( bool bCheck, bool bReset )
{
	// ROAD_Fが1の場合には、LCCLS、LUCLSの変更はできない
	// ROAD_Fが0の場合には、LCCLS、LUCLSの変更ができる
	SetDlgStatus( IDC_COMBO_LANDCOVER, !bCheck );
	SetDlgStatus( IDC_COMBO_LANDUSE, !bCheck );

	if( bReset )
	{
		// リセットする場合には、カーソル0にする（0は初期値）
		CComboBox cComboLC( GetDlgItem( IDC_COMBO_LANDCOVER ) ), cComboLU( GetDlgItem( IDC_COMBO_LANDUSE ) );
		cComboLC.SetCurSel( 0 );
		cComboLU.SetCurSel( 0 );
	}
}

// エラーチェック
BOOL CAttrSJPolygonDlg::ErrorCheck()
{
	// ROAD_POLYGONで、ROAD_F=1であればよし
	// 正確には、ROAD_F=1の場合にはこの子ダイアログで、LAND_COVER=0に無理やりしているので、エラーにはならない
	if( m_strClassName == sj::kRoadTableName )
	{
		if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kRoadF )->m_vaValue.lVal == 1 )
			return TRUE;
	}

	// LAND_COVER=0はエラー
	if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sj::kLCCLS )->m_vaValue.lVal == 0L )
	{
		long lOID = 0;
		GetCurrentFeatureDef()->GetFeature()->get_OID(&lOID);
		CString strMessage;
		strMessage.Format( AheLoadString( IDS_UNDEFINEDERROR ), m_strClassName, lOID );
		AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH);
		return FALSE;
	}

	return TRUE;
}
