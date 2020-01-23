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
 * @file WorkPurpose.h
 * @brief <b>CWorkPurposeクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _WORKPURPOSE_H_
#define _WORKPURPOSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace workpurpose {
/**
 * @class CWorkPurpose
 * @brief <b>作業目的管理レコードクラス</b>\n
 * このクラスは、REFERENCE.WORKPURPOSEテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CWorkPurpose : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CWorkPurpose, workpurpose )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>作業目的名を取得する</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszPurpose	[out]	作業目的名を格納するバッファ
	 * @retval LPCTSTR 作業目的名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetPurpose( int nLen, LPTSTR lpszPurpose ) const {
		return GetStringValueByFieldName( schema::workpurpose::kPurpose, nLen, lpszPurpose );
	}

	/**
	 * @brief <b>作業目的IDを取得する</b>\n
	 * @retval 0以上 作業目的ID
	 * @retval 0未満 エラー又はNULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::workpurpose::kPurposeID, -1 );
	}

	/**
	 * @brief <b>MXDファイル名を取得する</b>\n
	 * @param nLen			[in]			lpszMXDの収納可能文字列数
	 * @param lpszMXD		[out]			取得文字列を格納するバッファ
	 * @retval LPCTSTR MXDファイル名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetMXD( int nLen, LPTSTR lpszMXD ) const {
		return GetStringValueByFieldName( schema::workpurpose::kMXD, nLen, lpszMXD );
	}

	/**
	 * @brief <b>自動属性付与値を取得する</b>\n
	 * @param nLen					[in]			lpszAutoAttrGivingの収納可能文字列数
	 * @param lpszAutoAttrGiving	[out]			取得文字列を格納するバッファ
	 * @retval LPCTSTR 自動属性付与値
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetAutoAttrGiving( int nLen, LPTSTR lpszAutoAttrGiving ) const {
		return GetStringValueByFieldName( schema::workpurpose::kAutoAttrGiving, nLen, lpszAutoAttrGiving );
	}

	/**
	 * @brief <b>作業目的種類を取得する</b>\n
	 * @retval 0以上 作業目的種類
	 * @retval -1 エラー又はNULL
	 */
	long GetWorkKind() const {
		return GetLongValueByFieldName( schema::workpurpose::kWorkKind, -1 );
	}

	/**
	 * @brief <b>現地調査反映用かどうかのフラグを取得する</b>\n
	 * @note 現地調査反映用の作業目的で編集を行うと、ライブラリ内で自動的に
	 * 現地調査フラグにチェックを入れます。
	 * @retval true 現地調査反映用
	 * @retval false エラー又はNULL又は現地調査反映用ではない
	 */
	bool GetGencho() const {
		return GetBoolValueByFieldName( schema::workpurpose::kGencho, false );
	}

	/**
	 * @brief <b>クレーム修正用かどうかのフラグを取得する</b>\n
	 * @note クレーム修正用の作業目的で編集を行うと、ライブラリ内で自動的に
	 * クレームフラグにチェックを入れます。
	 * @retval true クレーム修正用
	 * @retval false エラー又はNULL又はクレーム修正用ではない
	 */
	bool GetClaim() const {
		return GetBoolValueByFieldName( schema::workpurpose::kClaim, false );
	}

	/**
	 * @brief <b>注記用かどうかのフラグを取得する</b>\n
	 * @note 注記用の作業では使用してはいけないツールに関しては、ここの値をチェックして
	 * ボタンのEnable/Disableを操作する必要があります。
	 * @retval true 注記用
	 * @retval false エラー又はNULL又は注記用ではない
	 */
	bool GetChuki() const {
		return GetBoolValueByFieldName( schema::workpurpose::kChuki, false );
	}

	/**
	 * @brief <b>削除済みかどうかのフラグを取得する</b>\n
	 * @note 作業目的レコードは実際には削除されることはなく、替わりにこのフラグが立ちます。
	 * @retval true 削除済み
	 * @retval false エラー又はNULL又は削除されていない
	 */
	bool GetDelete() const {
		return GetBoolValueByFieldName( schema::workpurpose::kDelete, false );
	}

	/**
	 * @brief <b>テスト用かどうかのフラグを取得する</b>\n
	 * @note このフラグは普段は表に出ません。
	 * @retval true テスト用
	 * @retval false エラー又はNULL又はテスト用ではない
	 */
	bool GetTest() const {
		return GetBoolValueByFieldName( schema::workpurpose::kTest, false );
	}

	/**
	 * @brief <b>カテゴリ名を取得する</b>\n
	 * @param nLen			[in]			lpszCategoryの収納可能文字列数
	 * @param lpszCategory	[out]			カテゴリ名を格納するバッファ
	 * @retval LPCTSTR カテゴリ名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetCategory( int nLen, LPTSTR lpszCategory ) const {
		return GetStringValueByFieldName( schema::workpurpose::kCategory, nLen, lpszCategory );
	}

	/**
	 * @brief <b>レイヤファイルパスを取得する</b>\n
	 * @note 以前はバッチで処理していましたが、ルールに取り込みました。
	 * @param nLen			[in]			lpszLayerFilePathの収納可能文字列数
	 * @param lpszLayerFilePath	[out]			レイヤファイルパスを格納するバッファ
	 * @retval LPCTSTR レイヤファイルパス
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetLayerFilePath( int nLen, LPTSTR lpszLayerFilePath ) const {
		return GetStringValueByFieldName( schema::workpurpose::kLayerFilePath, nLen, lpszLayerFilePath );
	}

	/**
	 * @brief <b>保存可不可フラグを取得する</b>\n
	 * @note 以前はマクロで実現していましたが、ルールに取り込みました。
	 * @retval true 保存可能な作業目的（デフォルト）
	 * @retval false 保存不可能な作業目的
	 */
	bool GetCanSave() const {
		return GetBoolValueByFieldName( schema::workpurpose::kCanSave, true );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>作業目的名を設定する</b>\n
	 * @param lpcszPurpose	[out]	作業目的名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurpose( LPCTSTR lpcszPurpose ) {
		return SetStringValueByFieldName( schema::workpurpose::kPurpose, lpcszPurpose );
	}

	/**
	 * @brief <b>MXDファイル名を設定する</b>\n
	 * @param lpcszMXD		[in]			MXDファイル名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMXD( LPCTSTR lpcszMXD ) {
		return SetStringValueByFieldName( schema::workpurpose::kMXD, lpcszMXD );
	}

	/**
	 * @brief <b>MXDファイル名を設定する</b>\n
	 * @param lpcszMXD		[in]			MXDファイル名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoAttrGiving( LPCTSTR lpcszAutoAttrGiving ) {
		return SetStringValueByFieldName( schema::workpurpose::kAutoAttrGiving, lpcszAutoAttrGiving );
	}

	/**
	 * @brief <b>作業目的種類を設定する</b>\n
	 * @param lKind			[in]	作業目的種類
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetWorkKind( long lKind ) {
		return SetLongValueByFieldName( schema::workpurpose::kWorkKind, lKind );
	}

	/**
	 * @brief <b>現地調査反映用かどうかのフラグを設定する</b>\n
	 * @note 現地調査反映用の作業目的で編集を行うと、ライブラリ内で自動的に
	 * 現地調査フラグにチェックを入れます。
	 * @param bFlag		[in]	現地調査反映用かどうかのフラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetGencho( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kGencho, bFlag );
	}

	/**
	 * @brief <b>クレーム修正用かどうかのフラグを設定する</b>\n
	 * @note クレーム修正用の作業目的で編集を行うと、ライブラリ内で自動的に
	 * クレームフラグにチェックを入れます。
	 * @param bFlag		[in]	クレーム修正用かどうかのフラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClaim( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kClaim, bFlag );
	}

	/**
	 * @brief <b>注記用かどうかのフラグを設定する</b>\n
	 * @note 注記用の作業では使用してはいけないツールに関しては、ここの値をチェックして
	 * ボタンのEnable/Disableを操作する必要があります。
	 * @param bFlag		[in]	注記用かどうかのフラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetChuki( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kChuki, bFlag );
	}

	/**
	 * @brief <b>作業目的IDを設定する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::workpurpose::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>削除済みかどうかのフラグを設定する</b>\n
	 * @note 作業目的レコードは実際には削除されることはなく、替わりにこのフラグが立ちます。
	 * @param bFlag		[in]	削除済みかどうかのフラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDelete( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kDelete, bFlag );
	}

	/**
	 * @brief <b>テスト用かどうかのフラグを設定する</b>\n
	 * @note このフラグは普段は表に出ません。
	 * @param bFlag		[in]	テスト用かどうかのフラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetTest( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kTest, bFlag );
	}

	/**
	 * @brief <b>カテゴリ名を設定する</b>\n
	 * @param lpcszCategory	[in]		カテゴリ名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetCategory( LPCTSTR lpcszCategory ) {
		return SetStringValueByFieldName( schema::workpurpose::kCategory, lpcszCategory );
	}
	//@}
};

} // workpurpose

} // sindy

#endif // _WORKPURPOSE_H_
