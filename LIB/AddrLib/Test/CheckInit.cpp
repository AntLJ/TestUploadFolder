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

#include "stdafx.h"
#include "CheckInit.h"

#include <arctl/coinitializer.h>
#include <AddrLib/Initializer.h>

std::mutex CheckInit::s_mutex;

CheckInit::CheckInit()
{
	const arctl::coinitializer coInitializer;

	// 住所マスタはReleaseユーザ(その時点で最新のメジャー住所マスタ)から
	// 取得するので、いずれ住所名称が変わることが考えられる。
	// テストに失敗した場合は、その辺りもチェックしてみること。
	addr::Init_AreaCodeHelper();
	addr::Init_CodeConverter(verificationDatDir + _T("CodeConvTable.mdb"));
	addr::Init_YomiConverter(verificationDatDir + _T("YomiTable.mdb"));
}

CheckInit& CheckInit::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_mutex);

	static CheckInit instance;
	return instance;
};
