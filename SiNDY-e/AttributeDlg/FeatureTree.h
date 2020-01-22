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

/*! \file FeatureTree.h
        \brief CFeatureTree クラスのインターフェイス
        \author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
        \date 2002-02-01
*/
#if !defined(AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_)
#define AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureDef.h"
#include "LQRowDef.h"

const unsigned int WM_TREESELECTCHANGE = RegisterWindowMessage(_T("WM_TREESELECTCHANGE"));      //!< ツリー上で選択状態が変化した時に発生するメッセージ
const unsigned int WM_REFRESHFEATURES  = RegisterWindowMessage(_T("WM_REFRESHFEATURES"));       //!< 再描画の依頼を受けるためのメッセージ
const unsigned int WM_CHANGECURRENTFEATURE = RegisterWindowMessage(_T("WM_CHANGECURRENTFEATURE"));	//!< 選択されているフィーチャを変更依頼を受けるためのメッセージ

/*!     \class CFeatureTree
        \brief 選択されたフィーチャをレイヤ、フィーチャ、外部テーブル、構成点でツリー上に表示するクラス

        初期化する場合は以下のメソッドを呼んで下さい。

        Init()
        SetFeatures()

        選択されたフィーチャや構成点の強調表示を再描画するには、 WM_REFRESHFEATURES をポストしてください。

        \b Example:

        ::SendMessage( m_pTreeCtrl, WM_REFRESHFEATURES, NULL, NULL );

        選択状態が変わった場合には WM_TREESELECTCHANGE メッセージを出します。
*/
class CFeatureTree : 
        public CWindowImpl<CFeatureTree, CTreeViewCtrlEx, CControlWinTraits>
{
public:
    DECLARE_WND_SUPERCLASS( _T("CFeatureTree"), CTreeViewCtrl::GetWndClassName() )
    CFeatureTree();
    virtual ~CFeatureTree();

    /**
     * ツリーに登録されているフィーチャの種類
     * @see ItemKind()
     */
    enum ITEMKINDS {
		NOKINDS,
		LAYERITEM,
		FEATUREITEM,
		OUTERTABLEITEM,
		OUTERROWITEM,
		FEATUREPOINTITEM
    };

/////////////////////////////////////////////////////////////////////////////
//
// CFeatureTree メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CFeatureTree)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown )
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown )
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDoubleClick )
    MESSAGE_HANDLER(WM_REFRESHFEATURES, OnRefresh)
	MESSAGE_HANDLER(WM_CHANGECURRENTFEATURE, OnSetCurrentFeature )
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// ツリービューイベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
    LRESULT OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnLButtonDoubleClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnSetCurrentFeature( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
/////////////////////////////////////////////////////////////////////////////
//
// メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * @brief ArcObjects ヘルパークラスをセットする
	 *
	 * @param pHelper ArcObjects ヘルパークラスのポインタ
	 */
    void Init( IApplication* ipApp );

	/**
	 * @brief フィーチャ定義クラスのリストをツリーにセットする
	 *
	 * @param pFeatureDefList       [in]        ツリーにセットするフィーチャ定義クラス
	 *
	 * @note 複数回呼ぶとその分追加され、その場合の動作は保証しません。
	 */
    void SetFeatures( std::list<CFeatureDef>* pFeatureDefList );

	/**
	 * @brief リンク列定義クラスのリストをツリーにセットする
	 *
	 * @param pFeatureDefList       [in]        ツリーにセットするフィーチャ定義クラス
	 *
	 * @note 複数回呼ぶとその分追加され、その場合の動作は保証しません。
	 */
	void SetLQs( std::list<CLQRowDef>* pFeatureDefList );

	/**
	 * @brief 現在選択されているアイテムを _ISet に入れて返す
	 *
	 * @param ipSet [out]        _ISet
	 *
	 * @note コンテナ内に入るのは IFeature 又は IRow になります。
	 */
    void GetSelectedFeatures( _ISet** ipSet );

	/**
	 * @brief 現在選択されているアイテムからインデックス番号を取得する
	 *
	 * @param lFeatureIndex	[out]        フィーチャ定義クラスのインデックス番号
	 * @param lTableIndex	[out]        テーブル定義クラスのインデックス番号
	 * @param lRowIndex		[out]        レコード定義クラスのインデックス番号
	 *
	 * @note 選択されているアイテムがフィーチャの場合は、lTableIndex と lRowIndex はそれぞれ -1 になります。
	 */
	void GetSelectedFeatures( long& lFeatureIndex, long& lTableIndex, long& lRowIndex );

	/**
	 * @brief ツリーコントロール上のアイテムの種類を判定する
	 *
	 * @param item  [in]        判定したいアイテム
	 *
	 * @return 判定結果（@see ITEMKINDS）
	 */
    INT ItemKind( HTREEITEM item );

	/**
	 * @brief HTREEITEM 構造体から CRowDef を特定して返す
	 *
	 * @param item          [in]        ツリー上のアイテム
	 *
	 * @return 存在しなければ NULL、あればそのポインタを返す
	 */
    CRowDef* GetRowDef( HTREEITEM item );

	/**
	 * @brief HTREEITEM 構造体から CLQRowDef を特定して返す
	 *
	 * @param item		[in]	ツリー上のアイテム
	 *
	 * @return 存在しなければ NULL、あればそのポインタを返す
	 */
	CLQRowDef* GetLQRowDef( HTREEITEM item );

	/**
	 * @brief HTREEITEM 構造体から CFeatureDef を特定して返す
	 *
	 * @param item          [in]        ツリー上のアイテム
	 *
	 * @return 存在しなければ NULL、あればそのポインタを返す
	 */
	CFeatureDef* GetFeatureDef( HTREEITEM item );

	/**
	 * @brief ハイライト用のコレクションなどを消す
	 *
	 * @note ダイアログの終了時に呼んでください
	 */
    void Clear();
private:
	/**
	 * @brief 選択されているフィーチャをハイライトする
	 *
	 * @param bRefresh      [in]	単純な再描画の場合は FALSE（デフォルト）、ハイライトするフィーチャが変わるときは TRUE を渡す
	 */
    void HilitSelectedFeatures( BOOL bRefresh = FALSE );

	/**
	 * @brief 選択されているリンク列をハイライトする
	 *
	 * @param ipGeom		[in]	リンク列の形状を渡す	
	 * @param bRefresh      [in]	単純な再描画の場合は FALSE（デフォルト）、ハイライトするフィーチャが変わるときは TRUE を渡す
	 */
	void HilitSelectedLQ( IGeometryPtr ipGeom,  BOOL bRefresh = FALSE );

	/**
	 * @brief 選択したフィーチャのTOCに表示されているレイヤ名を取得する
	 *
	 * @param ipFeature		[in]	フィーチャ
	 * @param bstrLayerName	[out]	TOCで表示されているレイヤ名
	 */
	void GetLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName);

	/**
	 * @brief TOCで選択しているレイヤ名を取得する
	 *
	 * @note 複数のレイヤを選択している場合は何も返さない。またIGeoFeatureLayerのときのみ値を返す
	 *
	 * @param ipFeature		[in]	属性表示したいフィーチャ
	 * @param bstrLayerName	[out]	TOCで表示されているレイヤ名
	 */
	void GetSelectedLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName);

	/**
	 * @brief 渡されたレイヤ名とツリーのテキストが同じかどうかの判定
	 *
	 * @param bstrLayerName		[in]	レイヤ名
	 * @param item				[in]	判定するアイテム
	 *
	 * @return 同じなら TRUE、違うなら FALSE
	 */
	BOOL IsSameText( CComBSTR bstrLayerName, HTREEITEM item );

	/**
	 * @brief 同じレイヤ名を持つアイテムを探す
	 *
	 * @param bstrLayerName		[in]	探すレイヤ名
	 *
	 * @return あれば該当するレイヤのアイテムを返す。なければ NULL を返す
	 */
	HTREEITEM SearchLayerName(  CComBSTR lpcszLayerName  );

	/**
	 * @brief レイヤ名とOBJECTIDからアイテムを探す
	 *
	 * @note ここで言うレイヤ名とはツリー上で表示されているレイヤ名です
	 *
	 * @param lpcszLayerName	[in]	レイヤ名
	 * @param lOID				[in]	OBJECTID
	 *
	 * @return あれば該当するレイヤのアイテムを返す。なければ NULL を返す
	 */
	HTREEITEM SearchOID( LPCTSTR lpcszLayerName, LONG lOID );

	/**
	 * @brief フィーチャをツリーに登録する
	 *
	 * レイヤ			0x8000000
	 * フィーチャ		0x4000000 + フィーチャカウント
	 * テーブル			0x2000000 + テーブルカウント
	 * レコード			0x1000000 + レコードカウント
	 * 構成点ラベル		0x800000  + フィーチャカウント
	 * 構成点			0x400000  + 構成点カウント
	 *
	 * @param pFeatureDef	[in]	登録するフィーチャ定義クラスのリスト
	 */
	void Add( std::list<CFeatureDef>* pFeatureDef );

	/**
	 * @brief リンク列をツリーに登録する
	 *
	 * レイヤ		0x8000000
	 * フィーチャ	0x4000000 + フィーチャカウント
	 * テーブル		0x2000000 + テーブルカウント
	 * レコード		0x1000000 + レコードカウント
	 * 構成点ラベル	0x800000  + フィーチャカウント
	 * 構成点		0x400000  + 構成点カウント
	 *
	 * @param pFeatureDef	[in]	登録するフィーチャ定義クラスのリスト
	 */
	void AddLQ( std::list<CLQRowDef>* pFeatureDef );

	/**
	 * @brief アイテムをツリーに登録する
	 *
	 * @param bIsRoot			[in]	親アイテム（レイヤ）の場合は TRUE、それ以外は FALSE
	 * @param lpItem			[in]	親になるアイテム
	 * @param lptstrNodeName	[in]	追加するアイテムのテキスト
	 *
	 * @return 追加したアイテム
	 */
    HTREEITEM AddItem( bool bIsRoot, HTREEITEM lpItem, LPCTSTR lptstrNodeName );

	/**
	 * @brief 外部テーブルをツリーに登録する
	 *
	 * @param item				[in]	親に当たるフィーチャのツリーアイテム
	 * @param pRelTableDef		[in]	登録する外部テーブル定義クラスのポインタ
	 * @param lTableIndex		[in]	pRelTableDef のインデックス番号
	 */
	void AddOuterTable( HTREEITEM& item, CRelationalTableDef* pRelTableDef, LONG lTableIndex );

	/**
	 * @brief マウスボタンのクリックを処理する関数
	 *
	 * @param uMsg          [in]        UINT
	 * @param wParam        [in]        WPARAM
	 * @param lParam        [in]        LPARAM
	 * @param bHandled      [out]        BOOL
	 * @param bDlbClick     [in]        ダブルクリックのイベントの場合は TRUE
	 *
	 * @return LONG
	 */
	LONG OnMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick );

	/**
	 * @brief リンク列専用マウスボタンのクリックを処理する
	 *
	 * @param uMsg		[in]	UINT
	 * @param wParam	[in]	WPARAM
	 * @param lParam	[in]	LPARAM
	 * @param bHandled	[out]	BOOL
	 * @param bDlbClick	[in]	ダブルクリックのイベントの場合は TRUE
	 *
	 * @return LONG
	 */
	LONG OnLQMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick );

	/**
	 * @brief ツリー上で右クリックを押したときにコピーのメニューを出します
	 *
	 * @param uMsg          [in]        UINT
	 * @param wParam        [in]        WPARAM ツリーの指定番号(上から)
	 * @param lParam        [in]        LPARAM
	 * @param bHandled      [out]       BOOL
	 *
	 * @return BOOL
	 */
	BOOL ShowMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief ツリー上で右クリックを押したときにコピーのメニューを出します
	 *
	 * @param bstrTree	[in]	CComBSTR 選択したOID
	 */
    void  WriteClipBoard(CComBSTR bstrTree);

	/**
	* @brief ポイントのXYZ座標情報を文字列で取得する
	* @param point XYZを取得したいポイント
	* @return XYZ座標の文字列 (X=* Y=* Z=*)
	*/
	CString getXYZAsString(IPointPtr point) const;

/////////////////////////////////////////////////////////////////////////////
//
// メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	BOOL m_bIsLineHilight;			//!< ラインをハイライトするかのフラグ
	BOOL m_bIsVertexHilight;		//!< 構成点をハイライトするかのフラグ
private:
    CArcHelper m_cArcHelper;					//!< ヘルパークラス
    std::list<CFeatureDef>* m_pFeatureDefList;	//!< フィーチャ保存用
	std::list<CLQRowDef>*	m_pLQDefList;		//!< フィーチャ保存用

    _ISetPtr		m_arrayHilitFeatures;	//!< ハイライトするフィーチャ保存用
    IPointPtr	m_ipHilitPoint;			//!< ハイライトするポイント保存用
	IGeometryPtr	m_ipHilightGeometry;//!< ハイライトする形状
	bool		m_bIsLQAttribute;		//!< リンク列を表示中かどうか				
    bool		m_bIsRButtonClick;		//!< 右クリックがおされたかどうかのフラグ		
};

#endif // !defined(AFX_FEATURETREE_H__5A8AAB20_068E_4C6C_B8E8_F7EFD0B9370A__INCLUDED_)

