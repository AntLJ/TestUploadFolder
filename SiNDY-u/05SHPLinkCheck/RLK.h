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
#include<vector>
#include"Def.h"

class CRLK
{
public:
	CRLK(IFeatureClassPtr ipFeatureClass);
	~CRLK(void);

	/**
	 * @brief リンク情報をvectorに格納
	 * @param rLinkSet			[in/out]	vectorコンテナ
	 * @param rLinkeFileName	[in]		ノードファイル名
	 * @param bWalkOpt			[in]		-wオプション有無
	 * @retval true							処理成功
	 * @retval false						処理失敗
	 */
	bool GetInfo(std::vector<LINK> & rLinkSet, std::wstring rLinkeFileName, bool bWalkOpt);

	
private:
    IFeatureClassPtr m_ipFeatureClass;			//ﾌｨｰﾁｬｰｸﾗｽ
	

};


