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

// AttrHeightNode.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrHeightNode.h"

using namespace sindy::schema;

BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		RCSPACE(20)
		BEGINROWS(WRCT_REST,0,0)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_COMBO_LAYER1)
				RCTOFIT(IDC_EDIT_ID1)
				RCREST(IDC_EDIT_WALKCLASS1)
				RCSPACE(20)
				RCTOFIT(IDC_COMBO_HEIGHT1)
				RCSPACE(20)
			ENDGROUP()
			RCSPACE(5)
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_COMBO_LAYER2)
				RCTOFIT(IDC_EDIT_ID2)
				RCREST(IDC_EDIT_WALKCLASS2)
				RCSPACE(20)
				RCTOFIT(IDC_COMBO_HEIGHT2)
				RCSPACE(20)
			ENDGROUP()
			RCSPACE(5)
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
 * @retval CAttrHeightNodeを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrHeightNode();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHeightNode::CAttrHeightNode()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrHeightNode::CAttrHeightNode() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_HEIGHT_NODE_DLG),
	m_focusedHeight( linkNo::unkown ),
	m_forceCancel( false )
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_LAYER1, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LAYER1 ) )
		( IDC_EDIT_ID1, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ID1 ) )
		( IDC_COMBO_HEIGHT1, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_HEIGHT1 ) )
		( IDC_COMBO_LAYER2, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_LAYER2 ) )
		( IDC_EDIT_ID2, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ID2 ) )
		( IDC_COMBO_HEIGHT2, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_HEIGHT2 ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

/** 
 * @fn HWND CAttrHeightNode::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrHeightNode::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrHeightNode::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrHeightNode::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

// 「OK」ボタン制御
void CAttrHeightNode::SetButton( BOOL changed )
{
	// CAttrBaseDlg::SetButton( changed ) では「OK」が潰せないケースがある。
	// 親ダイアログ（AttributeDlg）側のput_EditType()で新規作成時などに強制的にOKをEnableにするモードに設定されるようだ。
	//
	// ここでは、不正な場所に作成されるのを防ぐため、それは邪魔以外の何物でもないが、子ダイアログ側から
	// 解除する方法が見つからないので、強引に親のコントロールを拾ってきて直接操作する。。
	if( m_forceCancel )
	{
		CWindow(::GetDlgItem( m_hAttributeDlgWnd, IDOK )).EnableWindow( FALSE );
		return;
	}

	ATTR_BASE_CLASS::SetButton( changed );
}

// コントロール背景色変更
LRESULT CAttrHeightNode::OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 論理チェックでエラーとなった状態では「OK」を押させたくないのだが、SketchToolから呼ばれた際には
	// 子ダイアログの初期化の後に更に強制的にEnableにされてしまう。
	// その後、こちらから更に無効にするタイミング（イベント）が他にないので、しょうがないから常に呼ばれるここでやる。。
	// これをやらないと、SketchToolで作成して開いた直後に「OK」できてしまう
	SetButton( ATTR_BASE_CLASS::Changed() );
	return ATTR_BASE_CLASS::OnCtlColorEdit( uMsg, wParam, lParam, bHandled );
}

void CAttrHeightNode::EnableControls( bool editing )
{
	// 常に編集不可のコントロール
	const static std::vector<UINT> ctrls = boost::assign::list_of
		(IDC_COMBO_LAYER1)
		(IDC_EDIT_ID1)
		(IDC_EDIT_WALKCLASS1)
		(IDC_COMBO_LAYER2)
		(IDC_EDIT_ID2)
		(IDC_EDIT_WALKCLASS2);

	for( const auto& id : ctrls )
	{
		// エディットボックスならReadOnly
		CEdit edit( GetDlgItem(id) );
		edit.SetReadOnly( TRUE );

		// コンボならDisable...めんどくせーな
		if ( !(edit.GetStyle() & ES_READONLY) )
			edit.EnableWindow( FALSE );
	}
	
	// 編集開始で編集可能な場合のみ
	const static std::vector<UINT> heights = boost::assign::list_of
		(IDC_COMBO_HEIGHT1)
		(IDC_COMBO_HEIGHT2)
		(IDC_EDIT_SOURCE);

	for( const auto& id : heights )
	{
		GetDlgItem( id ).EnableWindow( bool2BOOL( editing ) );
	}

}

// 歩行者種別反映
void CAttrHeightNode::SetWalkclassToCtrl()
{
	if( !m_relativeLinks.IsExist() )
		return;

	static const std::map<linkNo::eCode, UINT> ctrlIDs = boost::assign::map_list_of
		( linkNo::link1, IDC_EDIT_WALKCLASS1 )
		( linkNo::link2, IDC_EDIT_WALKCLASS2 );

	for( const auto& id : ctrlIDs )
	{
		GetDlgItem( id.second ).SetWindowText( 
			m_relativeLinks[ id.first ].m_walkclass );
	}
}

// コンボボックのスリスト作成
void CAttrHeightNode::CreateHeightCombo( UINT id, long height )
{
	// 2 ～ -2 の連番作成
	// TODO: ちゃんとやるならレンジドメイン見るべき
	std::vector<long> vals(5);
	std::iota( vals.rbegin(), vals.rend(), -2 );

	CComboBox cb( GetDlgItem(id) );
	cb.ResetContent();
	for( const long val : vals )
	{
		int idx = cb.AddString( uh::str_util::ToString( val ) );

		// 選択すべき高さだったら選択しておく
		if( val == height )
			cb.SetCurSel( idx );

		// リストのアイテムに高さを紐付けておく
		cb.SetItemData( idx, val );
	}

}

// 自分の管理する高さ情報を各コントロールに反映する
void CAttrHeightNode::ApplyToCtrl()
{
	// コンボボックスリストのアイテムを選択する
	auto selectComboItem = []( HWND hwnd, const CString& item )
	{
		CComboBox cb( hwnd );
		int num = cb.GetCount();
		for( int i=0; i<num; ++i )
		{
			CComBSTR bstr;
			cb.GetLBTextBSTR( i, (BSTR&)bstr );
			if( item == bstr )
			{
				cb.SetCurSel( i );
				return;
			}
		}
	};

	// タプルはあんまり使いたくないが、超局所的なのでクラス作るよりはいいかな。。。
	typedef boost::tuple< linkNo::eCode, UINT, CString> linkno_ctrl_field;
	static const std::vector< linkno_ctrl_field > tupleFields = boost::assign::tuple_list_of
		( linkNo::link1, IDC_COMBO_LAYER1, height_node::kLinkID1Table  )
		( linkNo::link2, IDC_COMBO_LAYER2, height_node::kLinkID2Table  )
		( linkNo::link1, IDC_EDIT_ID1, height_node::kLinkID1  )
		( linkNo::link2, IDC_EDIT_ID2, height_node::kLinkID2 );

	// コントロールに値をつっこむ
	for( auto& fields : tupleFields )
	{
		// タプルを展開
		linkNo::eCode linkNo = linkNo::unkown;
		UINT ctrl = 0;
		CString field;
		boost::tie( linkNo, ctrl, field ) = fields;

		// 対象のControlDefを探す
		auto it = boost::find_if( m_cControlRel, [&field](CControlDef& def){
			return (field.CompareNoCase( def.GetFieldName() ) == 0);
		});

		if( it == m_cControlRel.end() )
		{
			_ASSERT( false );
			continue;
		}
		
		// ダイアログに値をセット
		const auto& linkInfo = m_relativeLinks[linkNo];
		switch (it->m_type)
		{
		case TYPE_EDIT:
			GetDlgItem( ctrl ).SetWindowText( uh::str_util::ToString( linkInfo.m_id ) );
			break;
		case TYPE_COMBO:
			selectComboItem( GetDlgItem( ctrl ), linkInfo.m_layer );
			break;
		default:
			break;
		}

		// ControlDefにも反映
		it->SetValueFromControl();
	}
	
	// 高さは別で。。
	SetWalkclassToCtrl();
}

/**
 * @fn BOOL CAttrHeightNode::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrHeightNode::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	bool editing = IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable;
	if( editing )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_HEIGHT1) );
	}

	// 高さ以外はDisableにしておく
	EnableControls( editing );

	// 相対高さコンボボックス
	auto& getHeight = [&](const CString& field){
		return GetCurrentFeatureDef()->GetFieldDef( height_node::kTableName, field )->m_vaValue.lVal;
	};
	CreateHeightCombo( IDC_COMBO_HEIGHT1, getHeight( height_node::kLinkID1Height ) );
	CreateHeightCombo( IDC_COMBO_HEIGHT2, getHeight( height_node::kLinkID2Height ) );

	// リンクの情報を取得
	const auto& heightNode = GetCurrentFeatureDef()->GetFeature();
	CString err;
	if( !m_relativeLinks.Reset( heightNode, m_cArcHelper, err ) ||
		!m_relativeLinks.CheckLogic(err) )
	{
		if( editing )
		{
			// [キャンセル] しかさせない
			m_forceCancel = true;
			::MessageBox( nullptr, err, AheLoadString( IDS_DIALOG_NAME ), MB_OK | MB_ICONERROR );
			EnableControls(false);
			return FALSE;
		}
	}

	// 新規もの（SketchToolで作成された）だったらコントロールに値を反映
	if( m_relativeLinks.IsCreated() )
	{
		ApplyToCtrl();
	}
	
	// 歩行者種別は別テーブルの情報なので自前で。。
	SetWalkclassToCtrl();
	
	return bRet;
}

/**
 * @fn void CAttrHeightNode::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrHeightNode::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// リンク１のテーブル
		m_cControlRel.SetControl( IDC_COMBO_LAYER1, TYPE_COMBO, 0, 0, height_node::kLinkID1Table, 0, height_node::kTableName );
		// リンク１のID
		m_cControlRel.SetControl( IDC_EDIT_ID1, TYPE_EDIT, 0, 0, height_node::kLinkID1, 0, height_node::kTableName );
		// リンク１の高さ レンジドメインがウマくないのでTYPE_OTHERにしておく
		m_cControlRel.SetControl( IDC_COMBO_HEIGHT1, TYPE_OTHER, 0, 0, height_node::kLinkID1Height, 0, height_node::kTableName );
		// リンク2のテーブル
		m_cControlRel.SetControl( IDC_COMBO_LAYER2, TYPE_COMBO, 0, 0, height_node::kLinkID2Table, 0, height_node::kTableName );
		// リンク2のID
		m_cControlRel.SetControl( IDC_EDIT_ID2, TYPE_EDIT, 0, 0, height_node::kLinkID2, 0, height_node::kTableName );
		// リンク2の高さ
		m_cControlRel.SetControl( IDC_COMBO_HEIGHT2, TYPE_OTHER, 0, 0, height_node::kLinkID2Height, 0, height_node::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, height_node::kTableName );
	}
}

namespace highlite_color{
	const COLORREF flat = RGB(0,255,0);
	const COLORREF lower = RGB(0,0,255);
	const COLORREF heiger = RGB(255,0,0);
}

// 再描画
LRESULT CAttrHeightNode::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	if( !m_relativeLinks.IsExist() )
		return 0;

	// ハイライトのラインを描画
	auto highliteLink = [&]( const IFeaturePtr& link, COLORREF color, bool outline )
	{
		auto geom = AheGetShapeCopy( link );
		AheDrawPolyline(m_cArcHelper.GetDisplay(), geom, color, 5, esriSLSSolid, TRUE, FALSE);
		if( outline )
		{
			AheDrawFeatureXOR( m_cArcHelper.GetDisplay(), link, AheConvertPixelsToMapUnits( m_cArcHelper.GetDisplay(), 10 ), FALSE );
		}
	};

	for( const auto& link : m_relativeLinks )
	{
		// フォーカスがあたってればアウトライン表示
		bool outline = (link.first == m_focusedHeight);

		if( !link.second.m_feature )
			continue;

		// 高低によってハイライト色を分ける
		COLORREF color = 0;
		switch( m_relativeLinks.GetRelativeHeight( link.first ) )
		{
		case height_info::flat:
			color = highlite_color::flat; break;
		case height_info::lower:
			color = highlite_color::lower; break;
		case height_info::higher:
			color = highlite_color::heiger; break;
		default:
			break;
		}

		highliteLink( link.second.m_feature, color, outline );
	}
	
	SetButton( ATTR_BASE_CLASS::Changed() );

	return 0;
}

// 高さが変更された
LRESULT CAttrHeightNode::OnHeightChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// コンボボックスによってフィールドが異なる
	CString field;
	linkNo::eCode linkNo = linkNo::unkown;
	switch (wID)
	{
	case IDC_COMBO_HEIGHT1:
		field = height_node::kLinkID1Height;
		linkNo = linkNo::link1;
		break;
	case IDC_COMBO_HEIGHT2:
		field = height_node::kLinkID2Height;
		linkNo = linkNo::link2;
		break;
	default:
		break;
	}
	
	// コンボボックスから新しい値を取得
	CComboBox cb(hWndCtl);
	long newVal = cb.GetItemData( cb.GetCurSel() );

	// ダイアログ管理用の値を更新
	GetCurrentFeatureDef()->GetFieldDef( height_node::kTableName, field )->SetNewValue( newVal, hWndCtl );

	// 高さ管理用の値を更新
	m_relativeLinks.UpdateHeight( linkNo, newVal );

	// ハイライト再描画
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);

	SetButton( ATTR_BASE_CLASS::Changed() );

	return 0;
}

// フォーカス取得時
LRESULT CAttrHeightNode::OnHeightFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// アウトラインハイライトフラグを立てておく
	switch (wID)
	{
	case IDC_COMBO_HEIGHT1:
		m_focusedHeight = linkNo::link1; break;
	case IDC_COMBO_HEIGHT2:
		m_focusedHeight = linkNo::link2; break;
	default:
		m_focusedHeight = linkNo::unkown; break;
	}

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}

// フォーカスが外れるとき
LRESULT CAttrHeightNode::OnHeightKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_focusedHeight = linkNo::unkown;	
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}
