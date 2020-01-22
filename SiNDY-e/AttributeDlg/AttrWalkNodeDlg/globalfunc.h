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

#pragma once

namespace gf { // グローバル関数

	/**
	* @brief 指定フィーチャの周辺～(m)の範囲で指定フィーチャクラスのフィーチャを検索する
	* @warning 検索結果には指定範囲外のものも含まれるので注意
	* @param ipFeature				[in]			検索範囲の中心になるフィーチャ
	* @param ipFeatClass			[in]			検索対象のフィーチャクラス
	* @param dRange					[in]			検索範囲の半径（単位：[m]）
	* @param strWhereClause			[in]			検索条件（デフォルト：NULL）
	* @return IFeatureCursorPtr
	*/
	IFeatureCursorPtr AroundSearch( IFeature* ipFeature, IFeatureClass* ipFeatClass, double dRange, LPCTSTR strWhereClause=NULL );

	/**
	* @brief フィーチャを点滅させる
	*
	* @note    ポイントフィーチャのみ対応
	* @warning sleepTime×blinkCountフレームの時間、入力を受け付けなくなる
	*
	* @param ipScDisp            [in]   スクリーンディスプレイ
	* @param lOID                [in]   対象フィーチャのオブジェクトID
	* @param ipFeatClass         [in]   対象フィーチャのフィーチャクラス
	* @param sleepTime           [in]   点滅後の入力を受け付けない時間 (Sleep関数に渡される)
	* @param blinkCount          [in]   点滅させる回数
	*/
	void BlinkFeature( IScreenDisplay* ipScDisp, LONG lOID, IFeatureClass* ipFeatClass, DWORD sleepTime=100, unsigned int blinkCount=4 );

	/**
	 * @brief CComVariantをCString型の文字列に変換する
	 * @param variant [in] CStringに変換するCComVariant
	 * @param strNull [in] variantが文字列型でない場合に表示する文字列
	 * @retval null     CComVariantのvtがVT_NULLだった
	 * @retval null以外 CComVariantの保持していた文字列
	 */
	CString VariantToCString( const CComVariant& variant, LPCTSTR strNull );

} // namespace gf
