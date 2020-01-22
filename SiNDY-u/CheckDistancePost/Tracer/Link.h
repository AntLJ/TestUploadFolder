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

// Link.h: Link クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <set>

namespace sindy
{
class Link  
{
public:

// 構築/消滅
	Link();
	Link(const IFeaturePtr& ipFeature);
	virtual ~Link();

// 設定
	void setFeature(const IFeaturePtr& ipFeature);

	/**
	 * フィーチャから属性を取得する
	 *
	 * @param ipRow				[in]	フィーチャ（レコード）
	 * @param lpcszFieldName	[in]	取得するフィールド名
	 *
	 * @retval 取得できた値
	 */
	CComVariant GetAttribute( const _IRowPtr& ipRow, LPCTSTR lpcszFieldName );
	CComVariant GetAttribute( const _IRowPtr& ipRow, long lIndex );
	/*
	 * @brief フィールド名でフィールドインデックスを取得する
	 * @param ipFields [in] IFields
	 * @param lpcszFieldName [in] フィールド名
	 * フィールド名と検索文字列の大文字小文字が一致しない場合を考慮
	 * し、ループを回して一つずつフィールド名を確認します。
	 * 従って、IFields::FindFieldは使用しません。
	 * @retval -1 フィールド名が見つからない、又は引数がNULL
	 * @retval -1以外 フィールドインデックス
	 */
	long FindField( const IFieldsPtr& ipFields, LPCTSTR lpcszFieldName );

// オペレータ
	bool operator < (const Link& rLink) const { return m_nObjectId < rLink.m_nObjectId; }

// プロパティ
	long m_nObjectId; ///< オブジェクトID
	long m_nFromNodeId; ///< FromノードID
	long m_nToNodeId; ///< ToノードID

	long m_nRoadClass; ///< 道路種別
	long m_nNaviClass; ///< 経路種別
	long m_nRoadWidth; ///< 道路復員区分
	long m_nLinkClass; ///< リンク種別
	long m_nRoadNo; ///< 路線番号
	long m_nRoadCode; ///< 路線コード
	
	IGeometryPtr m_ipGeometry; ///< リンク形状（ハイライトに使用）
};

}
