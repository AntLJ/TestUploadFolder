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

// AttrSpotSurveyPointDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "shellapi.h" // ShellExecute
#include "AttrSpotSurveyPointDlg.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "DrawFunctions.h"
#include "genericFunction.h"
#include <boost/assign/list_of.hpp>
#include <GdiPlus.h>
#pragma comment (lib, "Gdiplus.lib")
#undef min
#undef max

using namespace sindy::schema;
using namespace sindy::schema::sindym;

static const long FileNameMaxSize = 255;
static const long DirectoryMaxSize = 511;
static const long FullPathMaxSize = 2047;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(SPOT_SURVEY_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			// 調査ポイント情報部分
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_MANAGEMENT_ID, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_MANAGEMENT_ID, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_TARGETTYPE, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_TARGETTYPE, 35)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_SEASON, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SEASON, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_SURVEYTYPE, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SURVEYTYPE, 35)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCPERCENT(IDC_STATIC_COMPANYNAME, 15)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_COMPANYNAME, 30)
				RCSPACE(20)
				RCPERCENT(IDC_STATIC_SURVEYATTR, 20)
				RCTOFIT(-1)
				RCPERCENT(IDC_EDIT_SURVEYATTR, 35)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT,0,0)
			// 画像表示部分
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,5))
				RCREST(IDC_STATIC_VAR1)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CHANGECODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CHANGECODE)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAMEORG)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAMEORG)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYCOMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SURVEYCOMMENT)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYDATE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SURVEYDATE)
				RCSPACE(20)
				RCTOFIT(IDC_CHK_PIC_IMP_F)
				RCSPACE(20)
				RCTOFIT(IDC_CHK_IPC_F)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				BEGINCOLS(WRCT_FIXED, 0, 0)
					BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
						BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_BUTTON_ILLUST)
						ENDGROUP()
						BEGINROWS(WRCT_FIXED, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_BUTTON_POINT)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, 0)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCREST(IDC_STATIC_PIC)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCREST(IDC_SLIDER_PIC)
							RCTOFIT(-1)
							RCTOFIT(IDC_STATIC_PICNUM)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT,0,0)
			// 現調会社コメント部分
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,5))
				RCREST(IDC_STATIC_VAR2)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				BEGINCOLS(WRCT_REST, 0, 0)
					RCTOFIT(IDC_STATIC_REQUEST)
					RCTOFIT(-1)
					RCREST(IDC_EDIT_REQUEST)
				ENDGROUP()
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_COMPANYCOMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_COMPANYCOMMENT)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REMARKS1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_REMARKS1)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REMARKS2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_REMARKS2)
			ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NG_ATTR)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_NG_ATTR)
			ENDGROUP()
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NG_COMMENT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NG_COMMENT)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrSpotSurveyPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSpotSurveyPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSpotSurveyPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSpotSurveyPointDlg::CAttrSpotSurveyPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrSpotSurveyPointDlg::CAttrSpotSurveyPointDlg() 
	: ATTR_BASE_CLASS(SPOT_SURVEY_POINT_MAP, IDD_SPOTSURVEYPOINTDLG)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_MANAGEMENT_ID,  CContainedWindow( _T("EDIT"),     this, IDC_EDIT_MANAGEMENT_ID ) )
		( IDC_EDIT_TARGETTYPE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_TARGETTYPE ) )
		( IDC_EDIT_SEASON,         CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SEASON ) )
		( IDC_EDIT_SURVEYTYPE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYTYPE ) )
		( IDC_EDIT_COMPANYNAME,    CContainedWindow( _T("EDIT"),     this, IDC_EDIT_COMPANYNAME ) )
		( IDC_EDIT_SURVEYATTR,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYATTR ) )
		( IDC_EDIT_REQUEST,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REQUEST ) )
		( IDC_COMBO_CHANGECODE,    CContainedWindow( _T("COMBO"),    this, IDC_COMBO_CHANGECODE ) )
		( IDC_EDIT_NAME,           CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NAME ) )
		( IDC_EDIT_NAMEORG,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NAMEORG ) )
		( IDC_EDIT_SURVEYCOMMENT,  CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYCOMMENT ) )
		( IDC_EDIT_SURVEYDATE,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_SURVEYDATE ) )
		( IDC_CHK_PIC_IMP_F,       CContainedWindow( _T("CHECKBOX"), this, IDC_CHK_PIC_IMP_F ) )
		( IDC_CHK_IPC_F,           CContainedWindow( _T("CHECKBOX"), this, IDC_CHK_IPC_F ) )
		( IDC_BUTTON_ILLUST,       CContainedWindow( _T("BUTTON"),   this, IDC_BUTTON_ILLUST ) )
		( IDC_BUTTON_POINT,        CContainedWindow( _T("BUTTON"),   this, IDC_BUTTON_POINT ) )
		( IDC_EDIT_COMPANYCOMMENT, CContainedWindow( _T("EDIT"),     this, IDC_EDIT_COMPANYCOMMENT ) )
		( IDC_EDIT_REMARKS1,       CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REMARKS1 ) )
		( IDC_EDIT_REMARKS2,       CContainedWindow( _T("EDIT"),     this, IDC_EDIT_REMARKS2 ) )
		( IDC_EDIT_NG_ATTR,        CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NG_ATTR ) )
		( IDC_EDIT_NG_COMMENT,     CContainedWindow( _T("EDIT"),     this, IDC_EDIT_NG_COMMENT ) );

	m_objectID = 0;
	m_slider = 0;
	m_directionIdx = -1;
	m_selectedPicSeq = -1;
}

/** 
 * @fn HWND CAttrSpotSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrSpotSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSpotSurveyPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrSpotSurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	SetNull2DrawGeometry();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	if( IsWindow() )
		DestroyWindow();
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrSpotSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrSpotSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	// 描画用ジオメトリ変数初期化
	SetNull2DrawGeometry();
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	m_objectID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	((IFeaturePtr)GetCurrentFeatureDef()->GetFeature())->get_Shape( &m_surveyPointGeom );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// 移動前ポイント探すついでに移動前後でポイントが等しいかどうかチェック
	// 等しければボタン無効
	m_orgPointIsDrawed = false;
	CButton cPointButton( GetDlgItem( IDC_BUTTON_POINT ) );
	cPointButton.EnableWindow( LoadSurveyPointOrg() ? FALSE : TRUE );

	// 画像探す
	m_slider = 0;
	CTrackBarCtrl( GetDlgItem( IDC_SLIDER_PIC ) ).SetPos(0);
	m_directionIdx = -1;
	m_selectedPicSeq = -1;
	m_picPointVec.clear();
	FindPicture();
	DrawPicturePoint();

	// 既にビューアを開いているときは表示画像を切り替える
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	attr->updatePictureViewer( 0, GetCurrentFeatureDef()->GetFeature(), VARIANT_FALSE );
	RefreshPicturePointAllow();

	// イラスト描画
	m_illustIsDrawed = false;
	DrawIllustGeometry();
	CButton cIllustButton( GetDlgItem( IDC_BUTTON_ILLUST ) );
	if( !m_illustPointGeom && !m_illustLineGeom && !m_illustPolygonGeom )
	{
		m_illustIsDrawed = false;
		cIllustButton.EnableWindow( FALSE );
	}
	else
	{
		cIllustButton.SetWindowText( _T("イラストOFF") );
	}

	return bRet;
}

LRESULT CAttrSpotSurveyPointDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// コントロール
	CreateControlRelation();
	// サブクラス化
	for( auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}
	// レジストリの設定取得
	GetRegCode();
	// ウィンドウマネージャによる再配置
	// これないとエディットボックス一度隠すと(縦方向が)潰れてしまう
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
}

/**
 * @fn void CAttrSpotSurveyPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrSpotSurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		using namespace spot_survey_point;
		m_cControlRel.SetHWND( m_hWnd );
		// 管理ID
		m_cControlRel.SetControl( IDC_EDIT_MANAGEMENT_ID, TYPE_EDITSTRING, 0, 0, kManagementId, NULL, kTableName );
		// 時期データ
		m_cControlRel.SetControl( IDC_EDIT_SEASON, TYPE_EDITSTRING, 0, 0, kSeason, NULL, kTableName );
		// 現調会社名
		m_cControlRel.SetControl( IDC_EDIT_COMPANYNAME, TYPE_EDITSTRING, 0, 0, kCompanyName, NULL, kTableName );
		// 対象種別
		m_cControlRel.SetControl( IDC_EDIT_TARGETTYPE, TYPE_EDITSTRING, 0, 0, kTargetType, NULL, kTableName );
		// 現調種別
		m_cControlRel.SetControl( IDC_EDIT_SURVEYTYPE, TYPE_EDITSTRING, 0, 0, kSurveyType, NULL, kTableName );
		// 調査対象属性
		m_cControlRel.SetControl( IDC_EDIT_SURVEYATTR, TYPE_EDITSTRING, 0, 0, kSurveyAttribute, NULL, kTableName );
		// 依頼内容
		m_cControlRel.SetControl( IDC_EDIT_REQUEST, TYPE_EDITSTRING, 0, 0, kSurveyRequestType, NULL, kTableName );
		// 変更内容
		m_cControlRel.SetControl( IDC_COMBO_CHANGECODE, TYPE_COMBO, 0, 0, kChangedCode, NULL, kTableName );
		// 名称
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, kName, NULL, kTableName );
		// 修正前名称
		m_cControlRel.SetControl( IDC_EDIT_NAMEORG, TYPE_EDITSTRING, 0, 0, kNameOrg, NULL, kTableName );
		// 調査コメント
		m_cControlRel.SetControl( IDC_EDIT_SURVEYCOMMENT, TYPE_EDITSTRING, 0, 0, kSurveyComment, NULL, kTableName );
		// 調査日
		m_cControlRel.SetControl( IDC_EDIT_SURVEYDATE, TYPE_EDITSTRING, 0, 0, kSurveyDate, NULL, kTableName );
		// 撮影不可F
		m_cControlRel.SetControl( IDC_CHK_PIC_IMP_F, TYPE_CHECKBOX, 0, 0, kPictureImpossibleF, NULL, kTableName );
		// iPC作成F
		m_cControlRel.SetControl( IDC_CHK_IPC_F, TYPE_CHECKBOX, 0, 0, kCreatedByIPCF, NULL, kTableName );
		// 現調会社コメント
		m_cControlRel.SetControl( IDC_EDIT_COMPANYCOMMENT, TYPE_EDITSTRING, 0, 0, kCompanyComment, NULL, kTableName );
		// 備考1
		m_cControlRel.SetControl( IDC_EDIT_REMARKS1, TYPE_EDITSTRING, 0, 0, kRemarks1, NULL, kTableName );
		// 備考2
		m_cControlRel.SetControl( IDC_EDIT_REMARKS2, TYPE_EDITSTRING, 0, 0, kRemarks2, NULL, kTableName );
		// NG種別
		m_cControlRel.SetControl( IDC_EDIT_NG_ATTR, TYPE_EDITSTRING, 0, 0, kNgAttribute, NULL, kTableName );
		// NGコメント
		m_cControlRel.SetControl( IDC_EDIT_NG_COMMENT, TYPE_EDITSTRING, 0, 0, kNgComment, NULL, kTableName );
	}
}

LRESULT CAttrSpotSurveyPointDlg::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( !m_bFinishInit )
		return 0;

	int msg = HIWORD(wParam); // 操作メッセージ
	int nTargetControl = (int)LOWORD(wParam); // 操作対象コントロール

	// エディットボックスの時に全選択にする
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );

	// コンボボックスのリストボックスの長さを調節
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged( (int)LOWORD(wParam) );
		// ダイアログを更新領域に追加します
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}
	return 0;
}

// 現調画像サムネイル描画
LRESULT CAttrSpotSurveyPointDlg::OnPaint( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 描画開始
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);

	if( m_baseDir.IsEmpty() )
	{
		EndPaint( &ps );
		return 0;
	}

	// 描画範囲の取得
	HWND hPicWnd = GetDlgItem( IDC_STATIC_PIC );
	CWindow cPicWindow( hPicWnd );
	long lWidth = 0, lHeight = 0, lPicWidth = 0, lPicHeight = 0;
	RECT rect;
	cPicWindow.GetClientRect( &rect );
	lWidth = rect.right - rect.left;
	lHeight = rect.bottom - rect.top;

	// 描画枚数を決める
	long maxPaintNum = 0;
	maxPaintNum = (long)m_picPath.size();

	// 0ならここで終わり
	if( maxPaintNum == 0 )
	{
		EndPaint( &ps );
		return 0;
	}

	// 最大4枚まで
	if( maxPaintNum > 4 )
		maxPaintNum = 4;

	// 描画範囲を等分する
	lWidth = ( lWidth / maxPaintNum ) - maxPaintNum + 1;
	RECT PicRect;
	long lPicNo = m_slider;

	// サムネイルの下に枚数を表示するために文字列フォーマットを設定
    Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter );
    stringFormat.SetLineAlignment( Gdiplus::StringAlignmentCenter );
	// 描画用にフォントとブラシを作成
	Gdiplus::Font strFont(_T("MS UI Gothic"), 15);
    Gdiplus::SolidBrush strBrush( Gdiplus::Color::Black );

	// 1枚あたりの縦横の比率で描画サイズ変える
	CRect pictRect(0,0,0,0);
	cPicWindow.GetClientRect(pictRect);

	HDC picDC = cPicWindow.GetDC();
	Gdiplus::Graphics graphic(picDC);

	// 背景をクリア
	// これをしないとスライダー移動したときに枚数表示が重なる
	graphic.FillRectangle(
		&Gdiplus::SolidBrush( Gdiplus::Color(240, 240, 240) ),
		Gdiplus::Rect(pictRect.left, pictRect.top, pictRect.Width(), pictRect.Height()) );

	auto thumbRects = getThumbSize();
	for(auto& elem : thumbRects)
	{
		// firstには画像格納順が入る
		// 必要なのはvectorのインデックス番号なので、ここで変換
		const int index = elem.first - 1;
		if( index >= m_pictureVec.size() )
			continue;
		m_pictureVec[index].draw(hPicWnd, elem.second);

		// 枚数描画用の矩形を用意
		Gdiplus::RectF strRect(
			static_cast<Gdiplus::REAL>(elem.second.left)
			, static_cast<Gdiplus::REAL>(elem.second.bottom)
			, static_cast<Gdiplus::REAL>(elem.second.Width())
			, static_cast<Gdiplus::REAL>(pictRect.Height() - elem.second.Height())
			);

		graphic.DrawString(
			CString(std::to_wstring(elem.first).c_str()) + _T("枚目")
			, -1
            , &strFont
            , strRect
            , &stringFormat
            , &strBrush
        );
	}
	ReleaseDC(picDC);
	// 描画終了
	EndPaint( &ps );
	return 0;
}

LRESULT CAttrSpotSurveyPointDlg::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 撮影画像ポイント矢印の再描画
	RefreshPicturePointAllow();

	// 移動前現調ポイント
	if( m_orgPointIsDrawed )
	{
		if( m_orgSurveyPointGeom )
		{
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_orgSurveyPointGeom, FALSE, 5, 0, 0, 0, esriSMSCircle );
		}
	}

	// イラスト
	if( m_illustIsDrawed )
	{
		if( m_illustPointGeom )
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_illustPointGeom, FALSE, 5, 0, 200, 50, esriSMSCircle );
		if( m_illustLineGeom )
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(), m_illustLineGeom, RGB( 0, 200, 50 ), 2, esriSLSSolid, FALSE );
		if( m_illustPolygonGeom )
			AheDrawPolygon( m_cArcHelper.GetScreenDisplay(), m_illustPolygonGeom, RGB( 0, 200, 50 ), 2, esriSLSSolid, FALSE );
	}

	// TODO: こちらはハイライトの仕様が確定したら直すこと
	// 暫定対応である。
	// AfterDrawが呼ばれる順番が、
	// 画像ビューアのハイライト処理→この処理となり、ハイライト処理が上書きされるため、
	// 改めて、画像ビューア側のハイライト処理を呼び出している
	// （つまり、画像ビューアのハイライト処理→この処理→画像ビューアのハイライト処理、というように、同じ処理が二回呼び出されている）
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	IActiveViewEventsPtr(user)->AfterDraw(m_cArcHelper.GetScreenDisplay(), esriViewDrawPhase::esriViewForeground);

	return 0;
}

// 左クリックで撮影ポイントハイライト
LRESULT CAttrSpotSurveyPointDlg::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_picPath.empty() ) return 0;

	// 子ダイアログ内のクリック位置
	int pictSeq = getClickedPictureSeq( LOWORD(lParam), HIWORD(lParam) );
	if( pictSeq <= 0 ) return 0;

	m_selectedPicSeq = pictSeq;
	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	attr->updatePictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature(), VARIANT_FALSE );
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	return 0;
}

// ダブルクリックで画像開く
LRESULT CAttrSpotSurveyPointDlg::OnLButtonDblClk( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_picPath.empty() ) return 0;

	// 子ダイアログ内のクリック位置
	int pictSeq = getClickedPictureSeq( LOWORD(lParam), HIWORD(lParam) );
	if( pictSeq <= 0 ) return 0;

	m_selectedPicSeq = pictSeq;

	auto dock = generic::getDockableWindow( CLSID_SpotAttributeWindow, m_cArcHelper.GetApp() );
	auto user = generic::castUserDataToIUnknownPtr(dock);
	auto attr = static_cast<ISpotAttributeWindowPtr>(user);
	// Shift押されてたら強制して作る
	if( wParam&MK_SHIFT)
		attr->createPictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature() );
	else
		attr->updatePictureViewer( pictSeq, GetCurrentFeatureDef()->GetFeature(), VARIANT_TRUE );

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	return 0;
}

// スライダー動かした時のイベント
void CAttrSpotSurveyPointDlg::OnHScroll( int nCode, short nPos, HWND hWnd )
{
	CTrackBarCtrl trackbar = hWnd;
	m_slider = trackbar.GetPos();
	InvalidateRect( NULL, FALSE );
}

IGeometryPtr CAttrSpotSurveyPointDlg::GetIllustGeometry( const IWorkspacePtr& ipWorkspace, const CString& owner, const CString& table, const long id )
{
	CString tableName;
	if( owner.IsEmpty() )
		tableName = table;
	else
		tableName = owner + _T(".") + table;

	ITablePtr ipTable;
	ipTable = AheOpenTableByTableName( ipWorkspace, tableName );
	IFeatureClassPtr ipFeatureClass( ipTable );

	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( AheInitQueryFilter( NULL, NULL, _T("%s = %d"), spot_survey_point_relational_table::kSurveyPointId, id ), VARIANT_FALSE, &ipCursor );

	IGeometryPtr ipRetGeom;
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		if( !ipRetGeom )
		{
			ipRetGeom = ipGeom;
		}
		else
		{
			// 複数もありうる
			IGeometryPtr ipTmpGeom = ipRetGeom;
			((ITopologicalOperatorPtr)ipTmpGeom)->Union( ipGeom, &ipRetGeom );
		}
	}

	return ipRetGeom;
}

IWorkspacePtr CAttrSpotSurveyPointDlg::GetWorkspace()
{
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );

	IWorkspacePtr ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );

	return ipWorkspace;
}

// イラスト表示ボタン
LRESULT CAttrSpotSurveyPointDlg::OnClickedIllustButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	CString str;
	CButton cButton( GetDlgItem( IDC_BUTTON_ILLUST ) );
	if( m_illustIsDrawed )
		str = _T("イラストON");
	else
		str = _T("イラストOFF");

	cButton.SetWindowText( str );
	DrawIllustGeometry();
	return 0;
}

void CAttrSpotSurveyPointDlg::DrawIllustGeometry()
{
	if( m_illustIsDrawed )
	{
		m_illustIsDrawed = false;
		m_illustPointGeom = nullptr;
		m_illustLineGeom = nullptr;
		m_illustPolygonGeom = nullptr;
	}
	else
	{
		m_illustIsDrawed = true;

		// イラスト取ってきてハイライト
		if( !m_illustPointGeom && !m_illustLineGeom && !m_illustPolygonGeom )
		{
			IWorkspacePtr ipWorkspace = GetWorkspace();
			CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
			m_illustPointGeom = GetIllustGeometry( ipWorkspace, owner, illustration_point::kTableName, m_objectID );
			m_illustLineGeom = GetIllustGeometry( ipWorkspace, owner, illustration_line::kTableName, m_objectID );
			m_illustPolygonGeom = GetIllustGeometry( ipWorkspace, owner, illustration_polygon::kTableName, m_objectID );
		}
	}
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}

// 移動前後確認ボタン
LRESULT CAttrSpotSurveyPointDlg::OnClickedPointButton( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	CButton cButton( GetDlgItem( IDC_BUTTON_POINT ) );
	CString str;
	if( m_orgPointIsDrawed )
		str = _T("移動前確認");
	else
		str = _T("移動後確認");

	cButton.SetWindowText( str );
	DrawOrgPointGeometry();
	return 0;
}

bool CAttrSpotSurveyPointDlg::LoadSurveyPointOrg()
{
	if( !m_orgSurveyPointGeom )
	{
		IWorkspacePtr ipWorkspace = GetWorkspace();
		CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
		m_orgSurveyPointGeom = GetIllustGeometry( ipWorkspace, owner, spot_survey_point_org::kTableName, m_objectID );
	}

	if( m_orgSurveyPointGeom )
	{
		double x = 0, y = 0, orgx = 0, orgy = 0;
		((IPointPtr)m_surveyPointGeom)->QueryCoords( &x, &y );
		((IPointPtr)m_orgSurveyPointGeom)->QueryCoords( &orgx, &orgy );
		// 移動前後が同じ座標の場合、移動前はないものとみなす
		return ( x == orgx && y == orgy );
	}
	return true;
}

void CAttrSpotSurveyPointDlg::DrawOrgPointGeometry()
{
	IEnvelopePtr ipExtent;
	m_cArcHelper.GetActiveView()->get_Extent( &ipExtent );
	if( m_orgPointIsDrawed )
	{
		m_orgPointIsDrawed = false;
		ipExtent->CenterAt( (IPointPtr)m_surveyPointGeom );
	}
	else
	{
		m_orgPointIsDrawed = true;
		ipExtent->CenterAt( (IPointPtr)m_orgSurveyPointGeom );
	}
	m_cArcHelper.GetActiveView()->put_Extent( ipExtent );
	m_cArcHelper.GetActiveView()->Refresh();
}

void CAttrSpotSurveyPointDlg::DrawPicturePoint()
{
	IWorkspacePtr ipWorkspace = GetWorkspace();
	CString owner = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );

	CString tableName;
	if( owner.IsEmpty() )
		tableName = picture_point::kTableName;
	else
		tableName = owner + _T(".") + picture_point::kTableName;

	ITablePtr ipTable;
	ipTable = AheOpenTableByTableName( ipWorkspace, tableName );
	IFeatureClassPtr ipFeatureClass( ipTable );
	if( !ipFeatureClass )
		return;

	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( AheInitQueryFilter( NULL, NULL, _T("%s = %d"), picture_point::kSurveyPointId, m_objectID ), VARIANT_FALSE, &ipCursor );

	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		// TODO:このへんPicPointだけで完結するようにしたい
		if( m_pictureSeqIdx < 0 || m_directionIdx < 0 )
		{
			IFieldsPtr ipFields;
			ipFeature->get_Fields( &ipFields );
			ipFields->FindField( CComBSTR(picture_point::kPictureSeq), &m_pictureSeqIdx );
			ipFields->FindField( CComBSTR(picture_point::kDirection), &m_directionIdx );
		}
		m_picPointVec.push_back( PicPoint( ipFeature, m_pictureSeqIdx, m_directionIdx ) );
	}
	// 画像格納順でソート
	std::sort( m_picPointVec.begin(), m_picPointVec.end(), []( PicPoint& left, PicPoint& right ){return left.m_pictureSeq < right.m_pictureSeq;});

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}

void CAttrSpotSurveyPointDlg::FindPicture()
{
	m_picPath.clear();
	m_pictureVec.clear();
	m_slider = 0;

	// 現調種別
	CEdit cEditType(GetDlgItem( IDC_EDIT_SURVEYTYPE ) );
	LPTSTR lpszType = (LPTSTR)_alloca( ( cEditType.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditType.GetWindowText( lpszType, cEditType.GetWindowTextLength() + 1 );
	CString surveyType = lpszType;

	// 時期データ
	CEdit cEditSeason(GetDlgItem( IDC_EDIT_SEASON ) );
	LPTSTR lpszSeason = (LPTSTR)_alloca( ( cEditSeason.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditSeason.GetWindowText( lpszSeason, cEditSeason.GetWindowTextLength() + 1 );
	CString seasonData = lpszSeason;

	// OID
	CString strOid;
	strOid.Format( _T("%d"), m_objectID );

	// フォルダ内のファイル検索する
	// TODO:uh::getFileNames使う
	HANDLE hFile; // 検索用ハンドル
	WIN32_FIND_DATA findData; // 検索用構造体
	CString dirPath; // 検索するディレクトリ
	long itemCount = 0;
	dirPath = m_baseDir + _T("\\") + surveyType + _T("\\") + seasonData + _T("\\") + strOid + _T("\\") + _T("*.JPG");

	// 検索用ハンドルと最初のファイル取得
	hFile = FindFirstFile( dirPath, &findData );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		// TODO:「画像なし」と「画像見つからない」の区別を付けられるなら、「画像見つからない」場合はエラーメッセージを出すようにする。
		SetDlgItemText( IDC_STATIC_PICNUM, _T("0") );
		return;
	}

	// 画像パスを格納
	// 最初のファイル(findData)取得済みなのでdo～whileで処理
	do
	{
		// *.jpgで検索してるので問題ないと思うが、一応ファイル名等のチェック
		// ディレクトリ「.」「..」は見なくて良いので飛ばす
		if( findData.cFileName[0] == '.' )
			continue;

		// サブディレクトリは無視
		if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;

		// 得られるファイル名はフルパスなので分解
		TCHAR strDrive[FileNameMaxSize], strDir[DirectoryMaxSize], strFileName[FileNameMaxSize], strExt[FileNameMaxSize];
		_tsplitpath( findData.cFileName, strDrive, strDir, strFileName, strExt );
		// jpg以外は無視
		if( ((CString)strExt).CompareNoCase( _T(".JPG") ) != 0 )
			continue;

		// 格納
		CString strParameters;
		strParameters += m_baseDir + _T("\\") + surveyType + _T("\\") + seasonData + _T("\\") + strOid + _T("\\") + strFileName + _T(".JPG");
		m_picPath.push_back( strParameters );
	} while( FindNextFile( hFile, &findData ) );

	// 検索終わったら検索用ハンドルを閉じなくてはいけない
	FindClose( hFile );

	// ファイル名の頭はシーケンス番号なのでsortしてやればDB格納順になるはず
	std::sort( m_picPath.begin(), m_picPath.end() );

	// 画像をロードして格納
	for( auto& path : m_picPath )
	{
		CJpeg2 cJpeg;
		if( !cJpeg.load( path ) )
		{
#ifdef _DEBUG
			CString msg;
			msg.Format( _T("ロード失敗：%s"), path );
			MessageBox( msg, _T("FindPicture()"), MB_OK );
#endif
		}
		m_pictureVec.push_back( cJpeg );
	}

	// 取得した画像の枚数に合わせてスライダー他を設定する
	long picNum = 0;
	picNum = (long)m_picPath.size();
	CString strPicNum;
	strPicNum.Format( _T("%d"), picNum );
	SetDlgItemText( IDC_STATIC_PICNUM, strPicNum );

	// デフォルトで4枚まで表示させるので4枚未満ならスライダーいらない
	CTrackBarCtrl cTrackBar( GetDlgItem( IDC_SLIDER_PIC ) );
	if( picNum > 4 )
	{
		::ShowWindow( cTrackBar, true );
		cTrackBar.SetRange( 0, picNum-4 );
	}
	else
	{
		::ShowWindow( cTrackBar, false );
	}
}

// クリックされたサムネイルの元画像パス取得
int CAttrSpotSurveyPointDlg::getClickedPictureSeq(long clickedX, long clickedY)
{
	CWindow parentWnd( m_hWnd );
	CWindow staticPict( GetDlgItem( IDC_STATIC_PIC ) );

	CRect parentRect(0,0,0,0), picRect(0,0,0,0);
	parentWnd.GetWindowRect( parentRect );
	staticPict.GetWindowRect( picRect );

	// 子ダイアログのウィンドウ位置からクリックしポイントを求める
	// つまりこのX、YがIDC_STATIC_PICのウィンドウ範囲に入っていればよい
	const int nStaticPicX = parentRect.left + clickedX;
	const int nStaticPixY = parentRect.top + clickedY;
	if( nStaticPicX < picRect.left || nStaticPicX > picRect.right ||
		nStaticPixY < picRect.top || nStaticPixY > picRect.bottom )
		return -1;

	// 領域のどこをクリックしたか？
	auto thumbSizes = getThumbSize();
	for(auto& thumbnail : thumbSizes)
	{
		// この矩形の位置はIDC_STATIC_PICからの相対位置になっているので、
		// IDC_STATIC_PICの絶対座標に移動する
		CRect& collisionRect = thumbnail.second;
		collisionRect.OffsetRect(picRect.left, picRect.top);
		if( collisionRect.left <= nStaticPicX
			&& nStaticPicX <= collisionRect.right
			&& collisionRect.top <= nStaticPixY
			&& nStaticPixY <= collisionRect.bottom)
		{
			return thumbnail.first;
		}
	}
	return -1;
}

std::map<long,CRect> CAttrSpotSurveyPointDlg::getThumbSize()
{
	// 描画範囲としてIDC_STATIC_PICコントロールの描画領域を取得
	CRect staticArea(0,0,0,0);
	CWindow(GetDlgItem( IDC_STATIC_PIC )).GetClientRect( staticArea );

	// サムネイルとして描画可能な枚数は最大で4枚とする
	const long maxThumbNum = std::min(m_picPath.size(), 4U );
	// サムネイル画像の最大高さは、IDC_STATIC_PIC領域の7割の大きさとする
	const long thumbMaxHeight = static_cast<long>(staticArea.Height() * 0.7);
	// サムネイル画像の最大幅は、IDC_STATIC_PIC領域を、描画枚数で等分した大きさとする
	const long thumbMaxWidth  = staticArea.Width() / maxThumbNum;

	// サムネイル画像の最大高さ・幅に収まり、且つ、アスペクト比が4:3が保たれる
	// 画像幅・高さを計算する
	// double計算の過程で多少誤差が出る場合もあるが、厳密に求める必要はない
	double thumbHeight = thumbMaxWidth * 3.0/4.0;
	if( thumbHeight > thumbMaxHeight )
		thumbHeight = thumbMaxHeight;
	double thumbWidth = thumbHeight * 4.0 / 3.0;

	// サムネイル左の余白
	const long leftMargin = static_cast<long>((thumbMaxWidth - thumbWidth)/2.0);
	// サムネイル右の余白
	const long rightMargin = leftMargin;
	// サムネイル上の余白(レイアウトの都合で下の余白はとらない)
	const long topMargin = static_cast<long>((thumbMaxHeight - thumbHeight)/2.0);

	std::map<long,CRect> thumbRects;
	// dispIndexは、描画したときに左から何番目の画像になるかを表す
	// サムネイル領域の矩形を定義
	CRect thumbArea(
		0,          // 左辺X相対座標
		topMargin,  // 上辺Y相対座標
		thumbWidth, // 幅
		thumbHeight // 高さ
		);
	for(long dispIndex = 0; dispIndex < maxThumbNum; ++dispIndex)
	{
		thumbArea.OffsetRect( leftMargin, 0 );
		thumbRects.emplace( dispIndex+m_slider+1, thumbArea );
		thumbArea.OffsetRect( thumbWidth+rightMargin, 0 );
	}
	return thumbRects;
}

void CAttrSpotSurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\SpotViewer" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if( hOpenKey )
	{
		TCHAR strBaseDir[DirectoryMaxSize];

		if( EmxGetRegString( _T("SurveyDir"), strBaseDir, DirectoryMaxSize, hOpenKey ) )
			m_baseDir = strBaseDir;

		RegCloseKey( hOpenKey );
	}
}

void CAttrSpotSurveyPointDlg::DrawPicturePointAllow( IDisplayPtr piScreenDisplay, IPointPtr ipPoint, BOOL bXOR, long size, long red, long green, long blue, long angle, bool bDoStartStop )
{
	// シンボル作成
	IArrowMarkerSymbolPtr ipArrowMarkerSymbol( CLSID_ArrowMarkerSymbol );
	ipArrowMarkerSymbol->put_Style( esriAMSPlain );

	IColorPtr ipColor( AheGetColor( RGB( red, green, blue ) ) );
	ipArrowMarkerSymbol->put_Color( ipColor );
	ipArrowMarkerSymbol->put_Size( size );
	ipArrowMarkerSymbol->put_Angle( angle );

	ISymbolPtr ipArrowSymbol( ipArrowMarkerSymbol );
	if( bXOR )
	{
		ipArrowSymbol->put_ROP2( esriROPXOrPen );
	}

	// 描画
	double dScale = 0;
	IDisplayTransformationPtr ipTrans;
	piScreenDisplay->get_DisplayTransformation( &ipTrans );
	ipTrans->get_ReferenceScale( &dScale );
	ipTrans->put_ReferenceScale( 0.0 );
	if( bDoStartStop )
	{
		UINT hDC = 0;
		piScreenDisplay->get_hDC( &hDC );
		piScreenDisplay->StartDrawing( hDC, esriNoScreenCache );
	}
	piScreenDisplay->SetSymbol( ipArrowSymbol );
	piScreenDisplay->DrawPoint( ipPoint );
	if( bDoStartStop )
	{
		piScreenDisplay->FinishDrawing();
	}
	ipTrans->put_ReferenceScale( dScale );
}

void CAttrSpotSurveyPointDlg::SetNull2DrawGeometry()
{
	m_surveyPointGeom = nullptr;
	m_orgSurveyPointGeom = nullptr;
	m_illustPointGeom = nullptr;
	m_illustLineGeom = nullptr;
	m_illustPolygonGeom = nullptr;
}

// 撮影画像ポイント矢印の再描画
void CAttrSpotSurveyPointDlg::RefreshPicturePointAllow()
{
	// 撮影ポイント(全部)
	for( auto& picPoint : m_picPointVec )
	{
		// NULLもあり得るので、その時は飛ばす
		if( picPoint.m_x == 0 && picPoint.m_y == 0 )
			continue;

		// 未設定(撮影向きが-1)のときも飛ばす
		if( picPoint.m_angle < 0 )
			continue;

		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( picPoint.m_x, picPoint.m_y );
		DrawPicturePointAllow( m_cArcHelper.GetScreenDisplay(), ipPoint, FALSE, 12, 255, 0, 0, picPoint.m_angle );
		}

	// 撮影ポイント(選択中)
	if( m_selectedPicSeq > 0 )
	{
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( m_picPointVec[m_selectedPicSeq-1].m_x, m_picPointVec[m_selectedPicSeq-1].m_y );
		DrawPicturePointAllow( m_cArcHelper.GetScreenDisplay(), ipPoint, FALSE, 12, 0, 0, 255, m_picPointVec[m_selectedPicSeq-1].m_angle );
		m_selectedPicSeq = -1;
	}
}
