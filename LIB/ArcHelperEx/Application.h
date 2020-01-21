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
 * @file Application.h
 * @brief CApplicationクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <atl2/seal.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class CApplication
 * @brief <b>IApplicationラップクラス</b>\n
 * このクラスはCArcHelperに替わるアプリケーションラップクラスです。
 * このクラスでは、IApplicatonの全てのインターフェースをラップしています。
 * また、それ以外にも有用と思われるものについてはラップしてあります。\n\n
 * ISiNDYApplicationを使用する場合は、このクラスは使用しないで下さい。
 */
class CApplication : public atl2::seal<CComPtr<IApplication> >
{
public:
	CApplication() : m_bIsStartEditOperation( false )
	{
	}

	virtual ~CApplication()
	{
	}

	CApplication( IApplication* lp )
	{
		*this = lp;
	}

	CApplication& operator=( IApplication* lp )
	{
        if(*this!=lp)
    		AtlComPtrAssign((IUnknown**)&p, lp);

        return *this;
	}

	IApplication** Release()
	{
		CComPtr<IApplication>::Release();
		return &p;
	}

	//@{ @name IApplicationインターフェース
	IDocumentPtr GetDocument() const;
	IExtensionPtr FindExtensionByCLSID( IUID* ExtensionCLSID ) const;
	IExtensionPtr FindExtensionByName( LPCTSTR extensionName ) const;
	HWND GetHWND() const;
	bool IsDialogVisible() const;
	HRESULT LockCustomization( LPCTSTR Password, ICustomizationFilter* custFilter );
	HRESULT GetName( LPTSTR Name, int nLen ) const;
	HRESULT NewDocument( bool selectTemplate, LPCTSTR templatePath );
	HRESULT OpenDocument( LPCTSTR Path );
	HRESULT PrintDocument() const;
	HRESULT PrintPreview() const;
	HRESULT RefreshWindow() const;
	HRESULT SaveAsDocument( LPCTSTR saveAsPath, bool saveAsCopy ) const;
	HRESULT SaveDocument( LPCTSTR saveAsPath ) const;
	HRESULT ShowDialog( long dialogID, VARIANT& bShow, VARIANT& result ) const;
	IStatusBarPtr GetStatusBar() const;
	ITemplatesPtr GetTemplates() const;
	HRESULT UnlockCustomization( LPCTSTR Password );
	IDispatchPtr GetVBE() const;
	bool GetVisible() const;
	HRESULT PutVisible( bool Visible );
	//@}
	/**
	 * @brief <b>ProgIDからExtensionを探す</b>\n
	 * @param progID	[in]	ProgID（e.x. "esriCore.Editor"）
	 * @retval NULL		エラー又は存在しない
	 * @retval NULL以外	Extension
	 */
	IExtensionPtr FindExtensionByProgID( LPCTSTR progID ) const;

	//@{ @name IMxApplicationインターフェース
	IDisplayPtr GetDisplay() const;
	//@}

	/**
	 * @brief <b>TOCを構成するデータフレームを取得する</b>\n
	 * データフレームが複数ある場合は、常にアクティブなものを
	 * 取得します。
	 * @return IMapPtr
	 */
	IMapPtr GetFocusMap() const;

	/**
	 * @brief <b>フォーカスのある地図画面のスクリーンを取得する</b>\n
	 * @return IScreenDisplayPtr
	 */
	IScreenDisplayPtr GetFocusMapScreenDisplay() const;

	/**
	 * @brief <b>フォーカスのある地図画面のウィンドウハンドルを取得する</b>\n
	 * GetHWNDとは取得できるウィンドウハンドルが異なることに注意してください。
	 * @return HWND
	 */
	HWND GetFocusMapHWND() const;

	/**
	 * @brief <b>エディタコンポーネントを取得する</b>\n
	 * SiNDYEditorを優先的に取得します。SiNDYEditorを取得できない場合にのみ
	 * ArcMap標準のEditorを取得します。ArcMap標準かSiNDYEditorかどうかは、
	 * ISiNDYEditorインターフェースが取得できるかどうかで確認してください。
	 * @retval NULL		取得できない、又はエラー（未初期化など）
	 * @retval NULL以外	IEditorPtr
	 */
	IEditorPtr GetEditor() const;

	/**
	 * @brief <b>現在のビューオブジェクトを取得する</b>\n
	 * データビューの場合はMap、レイアウトビューの場合はPageLayout
	 * がそれぞれCoClassになりますので注意して下さい。
	 * @return IActiveViewPtr
	 */
	IActiveViewPtr GetActiveView() const;

	/**
	 * @brief <b>現在のビューの矩形を取得する</b>\n
	 * この関数で取得される矩形は、
	 * IActiveView::get_Extent()で得られるものと一緒
	 * です。
	 * @return ビュー矩形
	 */
	IEnvelopePtr GetViewBounds() const;

	/**
	 * @biref <b>アクティブデータフレームに読み込まれているテーブル・フィーチャクラスを取得する</b>\n
	 * アクティブデータフレームに読み込まれている場合に限り返します。参照用テーブルなど、
	 * アクティブデータフレームに読み込まれていない
	 * テーブルを取得したい場合はOpenTableByTableNameを使用してください。
	 * nFindCountの値が1以上の場合は、取り扱いに注意して下さい。
	 * @see AheGetTableByTableName
	 * @param lpcszOwnerTableName	[in]			オーナー.テーブル名
	 * @param nFindCount			[in]			見つかった件数（同じテーブルの場合はカウントしない）
	 * @param bEditableOnly			[in,optional]	編集可能なもののみ取得したい場合はtrue
	 * @return テーブル・フィーチャクラスが取得できない場合はNULL
	 */
	ITablePtr GetTableByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;

	/**
	 * @brief <b>アクティブデータフレームに読み込まれているテーブル・フィーチャクラスを取得する</b>\n
	 * この関数は内部でGetTableByTableNameを呼びますが、その後QueryInterfaceで
	 * IFeatureClassインターフェースを持つかどうかをチェックします。
	 * nFindCountの値が1以上の場合は、取り扱いに注意して下さい。
	 * @see GetTableByTableName
	 * @see GetTableByTableName
	 * @param lpcszOwnerTableName	[in]			オーナー.テーブル名
	 * @param nFindCount			[in]			見つかった件数（同じテーブルの場合はカウントしない）
	 * @param bEditableOnly			[in,optional]	編集可能なもののみ取得したい場合はtrue
	 * @return フィーチャクラスが取得できない場合はNULL
	 */
	IFeatureClassPtr GetFeatureClassByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;

	/**
	 * @brief <b>デバイス座標 -> マップ座標変換</b>\n
	 * この関数はIDisplayTransformation::ToMapPointのショートカットです。
	 * @param ipView	[in]	変換対象ビュー
	 * @param X			[in]	デバイス座標
	 * @param Y			[in]	デバイス座標
	 * @retval NULL		エラー
	 * @retval NULL以外 マップ座標
	 */
	IPointPtr ToMapPoint( IActiveView* ipView, long X, long Y ) const;

	/**
	 * @brief <b>Pixel -> マップ距離変換</b>\n
	 * @param ipView	[in]	変換対象ビュー
	 * @param nPixel	[in]	ピクセル数
	 * @retval 0以下	エラー
	 * @retval 上記以外	マップ距離
	 */
	double ToMapUnits( IActiveView* ipView, long nPixel ) const;

	/**
	 * @brief <b>キーコードでの各種画面操作を行う</b>
	 *
	 **/
	bool KeyZooMove( IActiveView* ipView, long lKeyCode, long lShift ) const;

	//@{ @name Editor系処理
	/**
	 * @brief <b>スナップする</b>
	 * @param ipPoint [in,out] スナップ対象ポイント
	 * @retval true スナップできた
	 * @retval false スナップしなかった
	 */
	bool Snap( IPoint* ipPoint ) const;

	/**
	 * @brief <b>編集開始状態を取得する</b>\n
	 * @retval esriStateEditing				アクティブなデータフレームで編集中である
	 * @retval esriStateNotEditing			編集開始されていない
	 * @retval esriStateEditingUnfocused	非アクティブなデータフレームで編集中である
	 */
	esriEditState GetEditState() const;

	HRESULT StartOperation();
	HRESULT StopOperation( LPCTSTR lpcszMenuText );
	HRESULT AbortOperation();
	//@}

	/**
	 * @brief <b>ArcMapかどうかを判断する</b>\n
	 * @retval true	ArcMapである
	 * @retval false ArcMapではない
	 */
	inline bool IsArcMap() const { CComBSTR bstrName; p->get_Name( &bstrName ); return ( 0 == wcscmp( bstrName, L"ArcMap" ) ) ? true : false; }

	/// ルールオブジェクトを取得する
	ISiNDYRulePtr GetRule() const;

	// StarEditOperationをしているかどうかを取得する
	// [Bug 10107]経路外道路に付与されたVICSリンク列が移動（伸縮）・削除できない
	// 関連で、SDEが10.1移行はStartEditOperationの2重掛けは失敗するようになったため
	// StartEditOperationのフラグ管理(m_bIsStartEditOperation)する
	inline bool IsStartEditOperation() const { return m_bIsStartEditOperation;};
protected:
	/**
	 * @brief <b>グループレイヤの中からテーブルを取得する</b>\n
	 * この関数はGetTableByTableNameから呼ばれます。
	 * nFindCountは関数内部で初期化されませんので、注意して下さい。
	 * @param ipCompLayer			[in]			グループレイヤ
	 * @param lpcszOwnerTableName	[in]			オーナー.テーブル名
	 * @param nFindCount			[in]			見つかった件数（同じテーブルの場合はカウントしない）
	 * @param bEditableOnly			[in,optional]	編集可能なもののみ取得したい場合はtrue
	 */
	ITablePtr GetTableByTableNameFromCompositeLayer( ICompositeLayerPtr ipCompLayer, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true ) const;
protected:
	mutable ISiNDYRulePtr m_ipRule; //!< ルールオブジェクトキャッシュ
private:
	bool m_bIsStartEditOperation; //!< StartEditOperationをしているかどうか
};

#endif // _APPLICATION_H_
