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

/**
 * @file    uh_guid.h
 */

#include <atlstr.h>
#include "tstring.h"

#include <rpc.h>
#pragma comment(lib, "rpcrt4")

namespace uh {
	// 2016/05/25 ADAM2のGUIDフィールド値の格納用
	// 2016/06/06 「-」の削除オプションを追加
	/**
	 * @brief	GUIDを文字列形式で生成
	 * @param eraseHypen [in] GUIDから「-」を削除するか。trueで削除、falseで削除しない
	 * @return	GUID文字列
	 */
	inline tstring GetGUID(bool eraseHyphen = true) {
		std::wstring wsUuid;

		UUID uuid;
		if (::UuidCreate(&uuid) != RPC_S_OK)
			return tstring();

		RPC_WSTR szUuid = NULL;
		if (::UuidToStringW(&uuid, &szUuid) != RPC_S_OK)
			return tstring();

		wsUuid = (BSTR)szUuid;
		::RpcStringFreeW(&szUuid);

		uh::tstring tsUuid = toTStr(wsUuid);

		// ｢-｣の削除指定がなければ、ここで返す
		if(!eraseHyphen)
			return tsUuid;

		static uh::tstring hyphen(_T("-")); 
		uh::tstring::size_type pos(tsUuid.find(hyphen));

		while(pos != uh::tstring::npos)
		{
			tsUuid.replace(pos, hyphen.size(), _T(""));
			pos = tsUuid.find(hyphen);
		}

		return tsUuid;
	}
}
