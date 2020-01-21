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
	// 2016/05/25 ADAM2��GUID�t�B�[���h�l�̊i�[�p
	// 2016/06/06 �u-�v�̍폜�I�v�V������ǉ�
	/**
	 * @brief	GUID�𕶎���`���Ő���
	 * @param eraseHypen [in] GUID����u-�v���폜���邩�Btrue�ō폜�Afalse�ō폜���Ȃ�
	 * @return	GUID������
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

		// �-��̍폜�w�肪�Ȃ���΁A�����ŕԂ�
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
