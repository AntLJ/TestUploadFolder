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
#include "AdamLinkPointChecker.h"

bool sindy::adam2::format::CSimpleLinkPointChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && indexes.size() == 2);
	if (!(sCALLBACK && sALL_FEATURE && indexes.size() == 2)) { return false; }

	// 車線リンク-車線ポイントの専用チェックのため、インデックスは決め打ちでよいかも
	const auto& kLinkInfo = kFeatureFileList[indexes[0]];
	const auto& kLinkData = (*sALL_FEATURE)[indexes[0]];
	const auto& kPointInfo = kFeatureFileList[indexes[1]];
	const auto& kPointData = (*sALL_FEATURE)[indexes[1]];

	sCALLBACK->callback(_T("[INFO][%s-%s]論理チェック"), kLinkInfo.alias_name, kPointInfo.alias_name);

	std::map<long, const Feature*> dicNodeOID;
	fBuildDic(kPointData, CLaneNodeChecker::NODE_ID, dicNodeOID, false);

	bool err = false;
	for (const auto& itr : kLinkData)
	{
		long objectid = GetLong<CLaneLinkChecker::OBJECTID>(itr);
		long from_node = GetLong<CLaneLinkChecker::FROM_NODE_ID>(itr);
		long to_node = GetLong<CLaneLinkChecker::TO_NODE_ID>(itr);
		long tunnel = GetLong<CLaneLinkChecker::TUNNEL_F>(itr);

		if (tunnel)
		{
			// TODO: ここは assert じゃなくてエラーハンドリングした方がいいかも
			assert(dicNodeOID.count(from_node));
			assert(dicNodeOID.count(to_node));

			if (!GetLong<CLaneNodeChecker::COVER_F>(*dicNodeOID[from_node])) {
				sCALLBACK->callback(_T("[WARNING][%s.%d]トンネルフラグがONに対して覆いものフラグがOFFになっています.[NODE=%d]."),
					kLinkInfo.alias_name,
					objectid,
					from_node);
				err = true;
			}
			if (!GetLong<CLaneNodeChecker::COVER_F>(*dicNodeOID[to_node])) {
				sCALLBACK->callback(_T("[WARNING][%s.%d]トンネルフラグがONに対して覆いものフラグがOFFになっています.[NODE=%d]."),
					kLinkInfo.alias_name,
					objectid,
					to_node);
				err = true;
			}
		}
	}

	return !err;
}
