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


class CRND
{
public:
	CRND(IFeatureClassPtr ipFeatureClass);  
	~CRND(void);

	/**
	 * @brief ノード情報をmapに格納
	 * @param rSortSet		[in/out]	キーをOBJECTID・DIVID、要素を座標とするmapコンテナ
	 * @param rNodeFileName	[in]		ノードファイル名
	 * @param bWalkOpt		[in]		-wオプション有無
	 * @retval true						処理成功
	 * @retval false					処理失敗
	 */
	bool Point2Map(std::map<std::pair<long, long>, WKSPoint>& rSortSet, std::wstring rNodeFileName, bool bWalkOpt);

private:
	IFeatureClassPtr m_ipFeatureClass;			//ﾌｨｰﾁｬｰｸﾗｽ
};
