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
#include "DataDef.h"

namespace index_def
{

std::vector<IndexInfo>& INDEXINFOMAP::getIndexInfo(const uh::tstring& name)
{
	auto it = std::find_if(m_valueMap.begin(), m_valueMap.end(),
		[&name](const VALUETYPE& elem)
		{
			return util::eq(elem.first, name);
		});
	if( m_valueMap.end() != it ) return it->second;
	m_valueMap.emplace_back(name, std::vector<IndexInfo>());
	return m_valueMap.back().second;
}

bool INDEXINFOMAP::isExist(const uh::tstring& name) const
{
	auto it = std::find_if(m_valueMap.begin(), m_valueMap.end(),
		[&name](const VALUETYPE& elem)
		{
			return util::eq(elem.first, name);
		});
	return  m_valueMap.end() != it;
}

} // index_def
