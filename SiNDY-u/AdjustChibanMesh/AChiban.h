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

// AChiban.h: AChiban クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_)
#define AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** 
    * @brief 注記フラグ付きの地番代表点のうちメッシュ境界上にあるものを検出及びフラグをオフするクラス 
    */ 
class AChiban  
{
public:
	/** 
  	 * @brief コンストラクタ 
  	 */ 
	AChiban();

	/** 
  	 * @brief デストラクタ 
  	 */ 
	~AChiban();

	/**
	 * @brief 初期化。
	 *
	 * @param ipChibanClass	[in]	地番代表点フィーチャクラス
	 * @param cChgMode		[in]	変更フラグ
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init(IFeatureClassPtr ipChibanClass, bool cChgMode);

	/**
	 * @brief メッシュ境界上の地番代表点の注記フラグをオフにする。
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool execAdjust(IFeaturePtr ipMesh, LONG cMeshCode);

private:
	CString m_LayerName;	///< フィーチャクラス名（ログ用）
	LONG m_MeshCode;		///< 実行中のメッシュコード（ログ用）
	LONG m_AnnofIndex;		///< 注記フラグのフィールドインデックス
	IFeatureClassPtr m_ipRep;	///< 地番代表点フィーチャクラス
	bool m_ChgMode;			///< 変更フラグ

	/**
	 * @brief 指定ジオメトリ上の地番代表点で注記フラグが立っているものを検出する。変更モードの時はオフにする。
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnCheck(IGeometryPtr ipBoundary);
};

#endif // !defined(AFX_ACHIBAN_H__22170C45_A46A_40CD_B561_307E37C448FF__INCLUDED_)
