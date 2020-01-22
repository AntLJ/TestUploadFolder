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

/**
 * @class	CCSHelper
 * @brief	CSポイントヘルパー検索クラス
 * @note	CSポイントに対する色々な処理を行う
 */
class CCSHelper
{
public:
	CCSHelper( CFeatureClass& cCSPointFC, const CFeatureClass& cCityAdminFC) : 
		m_cCSPointFC(cCSPointFC),
		m_cCityAdminFC(cCityAdminFC)
	{}
	~CCSHelper(void){};

	/**
	 * @brief	CSポイント検索
	 * @param	cRec		[in,out]	リストレコード
	 * @param	bAddMode	[in]		付与モード？
	 * @return	EResultStat(チェック結果ステータス)
	 */
	EResultStat findPoint( CLRec& cRec, bool bAddMode );

	/**
	 * @brief	CSポイント移動
	 * @param	cRec	[in, out]	リストレコード
	 * @return	EResultStat(移動処理結果ステータス)
	 */
	EResultStat movePoint( const CLRec& cRec );

	/**
	 * @brief	CSポイント追加
	 * @param	cRec	[in, out]	リストレコード
	 * @return	EResultStat(移動処理結果ステータス)
	 */
	EResultStat insertPoint( CLRec& cRec );

private:

	CFeatureClass	m_cCSPointFC;		//!< CSポイントフィーチャクラス
	const CFeatureClass	m_cCityAdminFC;		//!< 都市地図行政界フィーチャクラス
};
