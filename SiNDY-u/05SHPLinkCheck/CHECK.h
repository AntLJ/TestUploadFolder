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
#include<map>
#include<vector>
#include"Def.h"

/*
typedef struct LinkInfo{
	long ObjectID,FromNodeID,ToNodeID;		//オブジェクトID、始点ノードID、終点ノードID 
	WKSPoint FirstPoint,EndPoint;           //始点座標、終点座標
}LINK;
*/
class CCHECK
{
public:
	CCHECK(void);
	~CCHECK(void);

	/**
	 * @brief リンク正常性チェック
	 * @param rNodeSet		[in]	ノード情報
	 * @param rLinkSort		[in]	リンク情報
	 * @param rSecondMesh	[in]	メッシュコード
	 * @param bWalkOpt		[in]	-wオプション有無
	 * @retval true					処理成功
	 * @retval false				処理失敗
	 */
	bool CheckLinkSHP(std::map<std::pair<long, long>, WKSPoint> & rNodeSet, std::vector<LINK> & rLinkSort,long rSecondMesh, bool bWalkOpt);
};
