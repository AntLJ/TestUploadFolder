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

/**
* @file ArcHelper.h
*
* @brief IApplicationに関連する処理を行うArcHelperの定義ファイル
*/
#if !defined(AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_)
#define AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_	//!< 重複インクルード防止

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Application.h"

/**
* @class CArcHelper
*
* @brief IApplication を包括管理するクラス
*/
class CArcHelper : public CApplication
{
public:
	CArcHelper();
	virtual ~CArcHelper();

	//////////////////////////////////////////////////////////////////////
	// 初期化に関する関数
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief 初期化
	*
	* @note 渡されたインターフェイスを判定してクラスを初期化する。
	*
	* @param hook [in] オブジェクトのインスタンス
	*/
	void Init( IDispatchPtr hook );

	/**
	* @brief 初期化
	*
	* @note 渡されたインターフェイスで初期化する。
	*
	* @param ipApp [in] IApplicationのインスタンス
	*/
	void Init( IApplicationPtr ipApp );

	/**
	* @brief 初期化
	*
	* @note 渡されたインターフェイスで初期化する。
	*
	* @param ipEditor [in] IEditor のインスタンス
	*/
	void Init( IEditorPtr ipEditor );

	//////////////////////////////////////////////////////////////////////
	// 初期化方法の判定用関数
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief 初期化済み確認
	*
	* @note このクラスが初期化済みであるかを確認する。
	* @note 基本的にこの関数は、メソッドの有効性をテストするため、内部で適宜利用される。
	*
	* @retval TRUE 初期化済み
	* @retval FALSE 未初期化
	*/
	BOOL CheckInit() const;

	/**
	* @brief 初期化済み確認
	*
	* @note このクラスが IApplication で初期化済みであるかを確認する。
	* @note 基本的にこの関数は、メソッドの有効性をテストするため、内部で適宜利用される。
	*
	* @retval TRUE 初期化済み
	* @retval FALSE 未初期化
	*/
	BOOL CheckApp() const { return CheckInit(); }

	/**
	* @brief 初期化済み確認
	*
	* @note このクラスが IEditor で初期化済みであるかを確認する。
	* @note 基本的にこの関数は、メソッドの有効性をテストするため、内部で適宜利用される。
	*
	* @retval TRUE 初期化済み
	* @retval FALSE 未初期化
	*/
	BOOL CheckEditor() const;

	//////////////////////////////////////////////////////////////////////
	// オブジェクト取得関数
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief IApplication のインスタンス取得
	*
	* @note クラスが管理している IApplication のインスタンスを返す。
	*
	* @return IApplication のインスタンス
	*/
	IApplicationPtr GetApp() const { return (IApplication*)*this; }

	/**
	* @brief IActiveView のインスタンス取得
	*
	* @param bMapDisplay	[in,optional]	地図ビューの ScreenDisplay を取る場合は TRUE（デフォルト）、カレントを取る場合は FALSE
	*
	* @return IActiveViewPtr
	*/
	IActiveViewPtr GetActiveView(  BOOL bMapDisplay = TRUE ) const
	{
		//return ( bMapDisplay ) ? GetFocusMap() : CApplication::GetActiveView(); <- 3項演算子でスマートポインタを返すと最終項が必ずNULLになる？？（bug 5797）
		if( bMapDisplay )
			return GetFocusMap();
		else
			return CApplication::GetActiveView();
	}

	/**
	* @brief IMap のインスタンス取得
	*
	* @note クラスが管理しているインスタンスに応じて、そこから取得した IMap のインスタンスを返す。
	*
	* @return IMap のインスタンス
	*/
	IMapPtr GetMap() const { return GetFocusMap(); }

	/**
	* @brief IScreenDisplay のインスタンス取得
	*
	* @param bMapDisplay	[in,optional]	地図ビューの ScreenDisplay を取る場合は TRUE（デフォルト）、カレントを取る場合は FALSE
	*
	* @return IScreenDisplayPtr
	*/
	IScreenDisplayPtr GetScreenDisplay( BOOL bMapDisplay = TRUE ) const;

	/**
	* @brief IGraphicsContainer のインスタンス取得
	*
	* @param bMapDisplay	[in,optional]	地図ビューの ScreenDisplay を取る場合は TRUE（デフォルト）、カレントを取る場合は FALSE
	*
	* @return IGraphicsContainerPtr
	*/
	IGraphicsContainerPtr GetGraphicsContainer( BOOL bMapDisplay = TRUE ) const;

	/**
	* @brief TOCから現在選択されているレイヤを取得する
	*
	* @note 基本的にTOCで現在選択しているレイヤを返す。
	*		グループレイヤ選択されている場合には、そのなかの編集可能されているレイヤを取得してくる
	*		そのグループレイヤの中に複数の編集可能レイヤがある場合には、選択しているグループレイヤを返します。
	*
	* @param ppiLayer [out] 選択されているレイヤのインスタンス
	* @param bIsEditable [in]　編集開始されているレイヤをグループレイヤの下のレイヤから拾ってくるか？(デフォルト：true)
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	ILayerPtr GetSelectedLayer( bool bIsEditable = true ) const;

	//////////////////////////////////////////////////////////////////////
	// その他の関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief センタリングと拡大率変更
	*
	* @note bScale に０(Zero)以下を設定すると拡大率の変更は行われない。
	*
	* @param piPoint [in] 移動位置
	* @param dScale [in,optional] 拡大率
	* @param bMapDisplay [in,optional] GetActiveView() の引数 bMapDisplay と同義。
	*/
	void SetCenter( IPointPtr piPoint, DOUBLE dScale = -1.0, BOOL bMapDisplay = TRUE ) const;

	/// TOC を再描画する
	void RefreshContentsView() const;
	//////////////////////////////////////////////////////////////////////
	// 編集に関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief 編集処理開始
	*
	* @note 全てのレイヤに対し、編集処理を開始する。
	*
	* @retval TRUE 編集処理開始
	* @retval FALSE 編集処理は開始できない
	*/
	BOOL StartEditOperation( void){ return SUCCEEDED(StartOperation()) ? TRUE : FALSE; }

	/**
	* @brief 編集処理終了
	*
	* @note 現在行っている編集処理を終了する。
	*
	* @param 編集終了マーク文字列
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL StopEditOperation( LPCTSTR lpcszEditName ){ return SUCCEEDED(StopOperation(lpcszEditName)) ? TRUE : FALSE; }

	/**
	* @brief 編集処理中断
	*
	* @note 現在行っている編集処理を中断する。全ての処理は無効になる。
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL AbortEditOperation(){ return SUCCEEDED(AbortOperation()) ? TRUE : FALSE; }

	/**
	 * @brief 編集開始されているかどうか
	 *
	 * @retval TRUE 編集開始されている
	 * @retval FALSE 編集開始されていない
	 */
	BOOL IsStartEditing() const { return GetEditState() == esriStateEditing ? TRUE : FALSE; }
	//////////////////////////////////////////////////////////////////////
	// スナップに関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief スナップポイントを取得する
	*
	* @note スナップポイントを取得します。
	* @note カスタムアプリケーションでは使用できません。
	*
	* @param piPoint [in,retval] マウスポインタの位置
	*
	* @retval TURE スナップした。
	* @retval FALSE スナップしていない
	*/
	BOOL Snap( IPointPtr piPoint) const;
	/**
	* @brief スナップポイントを取得する
	*
	* @note スナップポイントを取得します。
	* @note スナップした点が複数個あった場合、一番近い距離のものを優先したければ、 bFirstOnly に FALSE を指定してください（まだ実装していません）。
	*
	* @param piPoint         [in]       マウスポインタの位置
	* @param ipSnappedPoint  [retval]   スナップしたポイント
	* @param bFirstOnly      [optional] 複数個あった場合に距離を無視するかどうか
	*
	* @retval TURE スナップした。
	* @retval FALSE スナップしていない
	*/
	BOOL Snap( IPointPtr ipPoint, IPoint** ipSnappedPoint, BOOL bFirstOnly = TRUE ) const;

	/**
	* @brief スナップポイントを取得する
	*
	* @note スナップポイントを取得します。カスタムアプリケーション用のコードは未完成です。
	* @note bCheckClass が FALSE なら SnapType は sindyeSnapNone、sindyeSnapRoadIntersection、sindyeSnapAny のいずれかになります。
	* @note bCheckHitType を TRUE にする場合は HitType は NULL を指定しないでください。
	*
	* @param piPoint         [in]       マウスポインタの位置
	* @param ipSnappedPoints [retval]   スナップしたポイントのコレクション
	* @param SnapType        [retval]   スナップしたフィーチャのタイプ
	* @param bCheckClass     [optional] スナップしたフィーチャのクラスを特定するかどうか
	* @param bCheckHitType   [optional] スナップしたポイントのタイプを特定するかどうか
	* @param HitType         [optional] スナップしたポイントのタイプ
	*
	* @retval TURE スナップした。
	* @retval FALSE スナップしていない
	*/
	BOOL Snap( IPointPtr ipPoint, IPointCollection** ipSnappedPoints,
		sindyeSnappingType* type, BOOL bCheckClass = FALSE,
		BOOL bCheckHitType = FALSE, esriGeometryHitPartType* HitType = NULL ) const;
	//////////////////////////////////////////////////////////////////////
	// イベント接続に関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief IActiveView に接続
	*
	* @param piUnk [in] 接続するオブジェクトインスタンス
	* @param dwCookie [in/out] 接続検証のクッキー値
	*
	* @return クッキー値
	*/	
	DWORD ConnectToActiveView( IUnknownPtr piUnk, DWORD& dwCookie ) const;

	/**
	* @brief IActiveView の接続解除
	*
	* @param dwCookie [in/out] 接続時のクッキー値
	*
	* @return クッキー値
	*/	
	DWORD DisconnectFromActiveView( DWORD& dwCookie ) const;
	//////////////////////////////////////////////////////////////////////
	// その他の関数
	//////////////////////////////////////////////////////////////////////
	/**
	 * @brief マウス位置の XY から地図上の座標に変換する
	 *
	 * @note 現在の地理参照をセットします
	 *
	 * @return IPointPtr
	 */
	IPointPtr QueryMapPoint( LONG X, LONG Y ) const { return ToMapPoint( (IActiveViewPtr)GetFocusMap(), X, Y ); }

	/**
	 * @brief ピクセルからマップユニットに変換します
	 *
	 * @return 現在のマップユニットでの長さ
	 */
	DOUBLE ConvertPixelsToMapUnits( INT nPixels ) const { return ToMapUnits( (IActiveViewPtr)GetFocusMap(), nPixels ); }

	BOOL IsArcMapBase() const { return IsArcMap(); }

protected:
	mutable IWorkspacePtr m_ipReferenceWorkspace;	//!< REFERENCE への接続用
};

#endif // !defined(AFX_ARCHELPER_H__E819DB32_9167_4CF3_9DF4_FF0DF22918B0__INCLUDED_)
