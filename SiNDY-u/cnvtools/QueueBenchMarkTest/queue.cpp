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

/**
 *	@file	queue.cpp
 *	@brief	
 *	@author	F.Abe
 *	@date	2004/10/03
 */


#include "stdafx.h"
#include "queue.h"

bool Queue::read(const char *cQFileName, const char *cIFileName, const char *cFieldName)
{
	if (! cQFileName || ! cIFileName)
		return false;
	if (! m_lqhdl.open(cQFileName, "r"))
		return false;
	if (! m_infhdl.open(cIFileName, "r"))
		return false;
	if (! DBFGetAttributes(&m_lqhdl, &m_queue))
		return false;
	if (! DBFGetAttributes(&m_infhdl, &m_attributes))
		return false;
	if (cFieldName) {
		if (! DBFGetAttributes(&m_lqhdl, &m_map, cFieldName))
			return false;
	}
	return true;
}

bool Entity::read(const char *cFileName)
{
	if (! cFileName)
		return false;
	if (! m_shphdl.open(cFileName, "r"))
		return false;
	if (! m_dbfhdl.open(cFileName, "r"))
		return false;
	if (! SHPGetEntities(&m_shphdl, &m_dbfhdl, &m_entities))
		return false;
	return true;
}
