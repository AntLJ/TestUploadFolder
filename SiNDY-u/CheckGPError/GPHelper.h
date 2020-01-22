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

#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/GouPoints.h>

using namespace addr;

/**
 * @class	CGPHelper
 * @brief	号ポイントヘルパー検索クラス
 * @note	号ポイントに対する色々な処理を行う
 */
class CGPHelper
{
public:
	CGPHelper( CFeatureClass& cGouPointFC, CFeatureClass& cGouPointFC_Ref, const CFeatureClass& cCityAdminFC, const CFeatureClass& cCityAdminFC_Ref ) : 
		m_cGouPointFC(cGouPointFC),
		m_cCityAdminFC(cCityAdminFC),
		m_cGous(cCityAdminFC),
		m_cGouPointFC_Ref(cGouPointFC_Ref),
		m_cCityAdminFC_Ref(cCityAdminFC_Ref)
	{}
	~CGPHelper(void){}

	/**
	 * @brief	号ポイント検索
	 * @param	cRec		[in,out]		リストレコード
	 * @param	bAddMode	[in]			付与モード？
	 * @return	EResultStat(チェック結果ステータス)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	号ポイント移動
	 * @param	cRec	[in, out]	リストレコード
	 * @param	bPriorF	[in]		移動ポイントは、優先フラグ立ってる？
	 * @return	EResultStat(移動処理結果ステータス)
	 */
	EResultStat movePoint( CLRec& cRec, bool bPriorF );

	/**
	 * @brief	号ポイント追加
	 * @param	cRec		[in, out]	リストレコード
	 * @return	EResultStat(移動処理結果ステータス)
	 */
	EResultStat insertPoint( CLRec& cRec );

private:

	/**
	 * @brief	チェックタイプごとの行政界Where句作成
	 * @param	cRec	[in]	リストレコード
	 * @param	emCType	[in]	チェックタイプ
	 * @return	Where句
	 */
	CString MakeWhereClause( const CLRec& cRec, ECheckType emCType );

	/**
	 * @brief	指定行政界内の号ポイント確認
	 * @param	cRec	[in]	リストレコード
	 * @param	rGou	[out]	号ポイント情報
	 * @param	emCType	[in]	チェックタイプ
	 * @retval	kO_NOT_FOUND      : 見つからない(一時ステータス)
	 * @retval	kO_MATCH_PRIOR_GP : Gouと優先フラグありポイントで一致(一時ステータス)
	 * @retval	kO_MATCH_GP       : Gouと優先フラグなしポイントで一致(一時ステータス)
	 * @retval	kW_GP_DUPLI_PRIOR : 【要確認】同一20桁コードで優先フラグ立っているものが複数あり	
	 */
	EResultStat findGPByAdmin( const CLRec& cRec, CSPGouPoint& rGou, ECheckType emCType );

	/**
	 * @brief	レコードと一致した号ポイントの関係から詳細ステータス取得
	 * @param	cRec		[in,out]	レコード
	 * @param	pGou		[in]		一致した号ポイント
	 * @param	bAddMode	[in]		付与モード？
	 * @return	EResultStat(チェック結果ステータス)
	 */
	EResultStat getDetailStatus( CLRec& cRec, const CSPGouPoint& pGou, bool bAddMode );
	
	/**
	 * @brief	移動・追加予定ポイントと参照DBのポイントとの比較
	 * @param	cRec		[in]	レコード
	 * @param	pGou		[in]	移動・追加予定のポイント
	 * @param   bIsMovePoint	[in]	pGouが移動ポイントかどうか
	 * @return	移動・追加の可否 (true：可)
	 */
	bool checkGPByReferenceDB( CLRec& cRec, CCityAdmin& cAdmin, CGouPoint& cGou, bool bIsMovePoint );

private:

	CGouPoints			m_cGous;			//!< 号ポイントリスト管理クラス

	CFeatureClass		m_cGouPointFC;		//!< 号ポイントフィーチャクラス
	CFeatureClass		m_cGouPointFC_Ref;	//!< 参照用号ポイントフィーチャクラス(bug9300)
	const CFeatureClass	m_cCityAdminFC;		//!< 都市地図行政界フィーチャクラス
	const CFeatureClass	m_cCityAdminFC_Ref;	//!< 参照用都市地図行政界フィーチャクラス(bug9300)
};
