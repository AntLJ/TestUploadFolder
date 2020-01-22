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

// ArrivedInfo.h: ArrivedInfo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Route.h"

namespace sindy
{
class ArrivedInfo  
{
public:
	typedef std::map<const LinkBranch*, Route> LinkKeyRouteMap;

// 構築/消滅
	ArrivedInfo();
	virtual ~ArrivedInfo();

// 消去
	void clear();

// 設定
	Route* setRoute(const Route &rRoute);

// 取得処理
	int countRoute() const;
	Route* bestRouteOfAll();
	const Route* bestRouteOfAll() const;
	Route* bestRouteOfReg();
	const Route* bestRouteOfReg() const;
	Route* bestRouteOfYetDepartured();

// 判定
	bool isNotArrived() const;
	bool isBetterThanAll(const Route& rRoute) const;

private:
// プロパティ
	Route m_cRoute; ///< ここまでのルート
	LinkKeyRouteMap m_cUturnRouteMap; ///< Ｕターンしそうなルート
};

}
