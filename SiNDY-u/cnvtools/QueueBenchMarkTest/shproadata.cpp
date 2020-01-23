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
 *	@file	shproaddata.cpp
 *	@brief
 *	@author	F.Abe
 *	@date	2004/10/03
 */


#include "stdafx.h"
#include "shproaddata.h"

bool ShpLink::read(const char *cFileName)
{
	if (! Entity::read(cFileName))
		return false;
	if (! SHPGetEntities2(&m_shphdl, &m_dbfhdl, &m_FromNodes, shape::road::road_link::kFromNodeId))
		return false;
	if (! SHPGetEntities2(&m_shphdl, &m_dbfhdl, &m_ToNodes, shape::road::road_link::kToNodeId))
		return false;
	m_nOID = DBFGetFieldIndex(&m_dbfhdl, shape::kObjectId);
	return true;
}

bool ShpLink::mk_linkid_table()
{
	long nFromId = DBFGetFieldIndex(&m_dbfhdl, shape::road::road_link::kFromNodeId);
	long nToId = DBFGetFieldIndex(&m_dbfhdl, shape::road::road_link::kToNodeId);

	for (SHPEntities::iterator it = m_entities.begin(); it != m_entities.end(); it++) {
		LinkId lid;
		lid.m_Id1 = it->second.m_vecAttributes[nFromId].lVal;
		lid.m_Id2 = it->second.m_vecAttributes[nToId].lVal;
		ShpEntity ent;
		ent.m_shpObject = it->second.m_shpObject;
		ent.m_vecAttributes = it->second.m_vecAttributes;
		m_LinkIds.insert(std::pair<LinkId, ShpEntity>(lid, ent));
		long temp = lid.m_Id1;
		lid.m_Id1 = lid.m_Id2;
		lid.m_Id2 = temp;
		m_LinkIds.insert(std::pair<LinkId, ShpEntity>(lid, ent));
	}

	return true;
}

bool ShpLink::find_link(long nId, ShpEntity *cObject)
{
	SHPEntities::iterator it = m_entities.find(nId);
	if (it == m_entities.end())
		return false;
	*cObject = it->second;
	return true;
}

bool ShpLink::find_link(long nId1, long nId2, ShpEntity *cObject)
{
	LinkId key;
	key.m_Id1 = nId1;
	key.m_Id2 = nId2;
	std::map<LinkId, ShpEntity, lessLinkId>::iterator it = m_LinkIds.find(key);
	if (it == m_LinkIds.end())
		return false;
	*cObject = it->second;
	return true;
}

bool ShpLink::find_link2(long nId1, long nId2, ShpEntity *cObject)
{
	Range range1 = m_FromNodes.equal_range(nId1);
	Range range2 = m_ToNodes.equal_range(nId2);
	if (range1.first != range1.second && range2.first == range2.second) {
		for (SHPEntities2::iterator i = range1.first; i != range2.second; i++) {
			for (SHPEntities2::iterator j = range2.first; j != range2.second; j++) {
				if (i->second.m_vecAttributes[m_nOID].lVal == j->second.m_vecAttributes[m_nOID].lVal) {
					*cObject = i->second;
					return true;
				}
			}
		}
	}
	range1 = m_FromNodes.equal_range(nId2);
	range2 = m_ToNodes.equal_range(nId1);
	if (range1.first != range1.second && range2.first == range2.second) {
		for (SHPEntities2::iterator i = range1.first; i != range2.second; i++) {
			for (SHPEntities2::iterator j = range2.first; j != range2.second; j++) {
				if (i->second.m_vecAttributes[m_nOID].lVal == j->second.m_vecAttributes[m_nOID].lVal) {
					*cObject = i->second;
					return true;
				}
			}
		}
	}
	return false;
}

bool ShpNode::find_node(long nId, ShpEntity *cObject)
{
	SHPEntities::iterator it = m_entities.find(nId);
	if (it == m_entities.end())
		return false;
	*cObject = it->second;
	return true;
}
