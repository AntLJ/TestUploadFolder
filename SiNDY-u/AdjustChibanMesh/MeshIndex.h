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

// MeshIndex.h: MeshIndex クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_)
#define AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MeshIndex  
{
public:
	/** 
  	 * @brief コンストラクタ 
  	 */ 
	MeshIndex();

	/** 
  	 * @brief コンストラクタ 
	 *
	 * @param ipFeature	[out]	メッシュポリゴンフィーチャクラス
	 *
  	 */ 
	MeshIndex(IFeatureClassPtr ipFeature);

	/** 
  	 * @brief デストラクタ 
  	 */ 
	~MeshIndex();

	/**
	 * @brief 初期化。（条件で指定）
	 *
	 * @param cMeshField	[in]	メッシュコードフィールド名
	 * @param cWhere		[in]	対象抽出用SQLのWHERE文部分
	 *
	 * @retval true		成功
	 * @retval false	失敗
	 */
	bool init(LPCTSTR cMeshField, const CString &cWhere);

	/**
	 * @brief 初期化。（メッシュコードで指定）
	 *
	 * @param cMeshCode		[in]	メッシュコード
	 * @param cMeshField	[in]	メッシュコードフィールド名
	 *
	 * @retval true		成功
	 * @retval false	失敗
	 */
	bool init(LONG cMeshCode, LPCTSTR cMeshField);

	/**
	 * @brief （次の）対象メッシュポリゴンを取得。
	 *
	 * @param cpMeshCode	[out]	メッシュコード
	 *
	 * @retval NULL以外	メッシュポリゴンのフィーチャ
	 * @retval NULL	終了（エラーで抜けるかもしれない）
	 */
	IFeaturePtr nextFeature(LONG* cpMeshCode);

private:
	LONG m_MeshIndex;	///< メッシュコードフィールドインデックス
	IFeatureCursorPtr m_ipCursor;	///< 条件で選ぶ時用のフィーチャカーソル
	IFeatureClassPtr	m_ipFeature;	///< メッシュポリゴンフィーチャクラス（名前が紛らわしい）
};

#endif // !defined(AFX_MESHINDEX_H__232CA619_D99C_4293_A06B_FBC0DA339045__INCLUDED_)
