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
#include "ErrorCode.h"

using namespace sindy_error::walk_node;

void ErrorCodeKeeper::Reset()
{
	m_errorCodes.clear();
}

void ErrorCodeKeeper::Push(ErrorCode code)
{
	m_errorCodes.push_back(code);
}

bool ErrorCodeKeeper::IsError()
{
	// àÍÇ¬Ç≈Ç‡ÉGÉâÅ[Ç™Ç†ÇÈÇ»ÇÁtrueÇï‘Ç∑
	return !m_errorCodes.empty();
}

std::vector<CString> ErrorCodeKeeper::GetStringList()
{
	std::vector<CString> reList; //!< ï‘ÇËílÇäiî[
	for(const auto& elem : m_errorCodes)
	{
		switch(elem)
		{
		case ErrorCode::ROADNODEID_INPUT_ERROR:
			reList.emplace_back(AheLoadString(IDS_ROADNODE_INPUT_ERROR));
			break;
		case ErrorCode::STATIONGATE_INPUT_ERROR:
			reList.emplace_back(AheLoadString(IDS_STATIONGATE_INPUT_ERROR));
			break;
		case ErrorCode::STATIONGATE_NONE_INPUT_ERROR:
			reList.emplace_back(AheLoadString(IDS_STATIONGATE_NONE_INPUT_ERROR));
			break;
		case ErrorCode::UNDERGROUNDGATE_INPUT_ERROR:
			reList.emplace_back(AheLoadString(IDS_UNDERGROUNDGATE_INPUT_ERROR));
			break;
		case ErrorCode::UNDERGROUNDGATE_NONE_INPUT_ERROR:
			reList.emplace_back(AheLoadString(IDS_UNDERGROUNDGATE_NONE_INPUT_ERROR));
			break;
		default:
			continue;
			break;
		}
	}
	return reList;
}
