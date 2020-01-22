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

// ArrivedInfo.cpp: ArrivedInfo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrivedInfo.h"

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ArrivedInfo::ArrivedInfo()
{

}

ArrivedInfo::~ArrivedInfo()
{

}

//////////////////////////////////////////////////////////////////////
// 消去
//////////////////////////////////////////////////////////////////////

void ArrivedInfo::clear()
{
	m_cRoute.clear();
	m_cUturnRouteMap.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief ルートを設定する
 *
 * @param rRoute [in] 設定するルート
 * @return 設定したルート。指定したルートが既知のルートより効率的ではないため設定しなかった場合NULL。
 */
Route* ArrivedInfo::setRoute(const Route &rRoute)
{
	ATLASSERT(rRoute.isValid());

	// ルートが設定されていない場合、初期ルートとして設定する
	if(isNotArrived()) {
		if(rRoute.isNoRegulations()) {
			// 右左折禁止が設定されていない場合、無条件到着ルートに
			return &(m_cRoute = rRoute);
		}
		else {
			// 右左折禁止が設定されている場合、条件付到着ルートに
			Route* pResult = 0;

			if(rRoute.m_pLeftLink) {
				pResult = &(m_cUturnRouteMap[rRoute.m_pLeftLink] = rRoute);
			}

			return pResult;
		}
	}

	// 規制関係なしルートよりコストが高いルートはパス
	if(m_cRoute.isValid() && m_cRoute.m_dCost <= rRoute.m_dCost)
		return 0;

	if(rRoute.isNoRegulations()) {
		// 設定するルートに規制が設定されていなかった場合の処理
		return &(m_cRoute = rRoute);
	}
	else {
		Route* pResult = nullptr;

		return pResult;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// 取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 登録されているルートの数を取得する
 *
 * @return ルートの数
 */
int ArrivedInfo::countRoute() const
{
	return (m_cRoute.isEmpty() ? 0 : 1) + m_cUturnRouteMap.size() ;
}

/**
 * @brief 規制に絡んでいるものも含め、もっとも優れたルート
 *
 * @return 最短ルート。到達したルートが存在しない場合はNULL。
 */
Route* ArrivedInfo::bestRouteOfAll()
{
	Route* pResult = 0;
	
	if(m_cRoute.isValid()) {
		pResult = &m_cRoute;
	}

	Route* pRouteOfReg = bestRouteOfReg();

	if(! pResult || (pRouteOfReg && pRouteOfReg->m_dCost < pResult->m_dCost)) {
		pResult = pRouteOfReg;
	}

	return pResult;
}

const Route* ArrivedInfo::bestRouteOfAll() const
{
	const Route* pResult = 0;
	
	if(m_cRoute.isValid()) {
		pResult = &m_cRoute;
	}

	const Route* pRouteOfReg = bestRouteOfReg();

	if(! pResult || (pRouteOfReg && pRouteOfReg->m_dCost < pResult->m_dCost)) {
		pResult = pRouteOfReg;
	}

	return pResult;
}

/**
 * @brief 規制に絡んでいるもののなかで、もっとも優れたルート
 *
 * @return 規制がらみの最短ルート。存在しない場合はNULL。
 */
Route* ArrivedInfo::bestRouteOfReg()
{
	Route* pResult = 0;

	for(LinkKeyRouteMap::iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;
		
		if(rRoute.isValid() && (! pResult || rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

const Route* ArrivedInfo::bestRouteOfReg() const
{
	const Route* pResult = 0;

	for(LinkKeyRouteMap::const_iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		const Route& rRoute = itr->second;
		
		if(rRoute.isValid() && (! pResult || rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

/**
 * @brief 「出立フラグ」が立っていないものの中で、最も短いルート
 *
 * @return 最短ルート。存在しない場合はNULL。
 */
Route* ArrivedInfo::bestRouteOfYetDepartured()
{
	Route*	pResult = 0;
	
	if(m_cRoute.isValid() && ! m_cRoute.m_bDepartured) {
		pResult = &m_cRoute;
	}

	for(LinkKeyRouteMap::iterator itr = m_cUturnRouteMap.begin(); itr != m_cUturnRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;

		if(rRoute.isValid() && ! rRoute.m_bDepartured && ! pResult || (pResult && rRoute.m_dCost < pResult->m_dCost)) {
			pResult = &rRoute;
		}
	}

	return pResult;
}

//////////////////////////////////////////////////////////////////////
// 判定
//////////////////////////////////////////////////////////////////////

/**
 * @brief まだ一度も訪れたことがないか
 *
 * @retval true 未踏
 * @retval false 訪れたことが有る
 */
bool ArrivedInfo::isNotArrived() const
{
	return m_cRoute.isEmpty() && m_cUturnRouteMap.empty();
}

/**
 * @brief 規制に絡んでいるものも含め、どのルートよりもコストの少ないルートか
 *
 * @retval true 短い
 * @retval false 長い、あるいは同じ長さ
 */
bool ArrivedInfo::isBetterThanAll(const Route& rRoute) const
{
	const Route* pBestRoute = bestRouteOfAll();

	return ! pBestRoute || rRoute.m_dCost < pBestRoute->m_dCost ;
}

}
