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
 *	@file	shproaddata.h
 *	@brief
 *	@author	F.Abe
 *	@date	2004/10/03
 */

#ifndef	___SHP_ROAD_DATA_H___
#define	___SHP_ROAD_DATA_H___

#include "queue.h"

class ShpLink : public Entity
{
	SHPEntities2	m_FromNodes;
	SHPEntities2	m_ToNodes;
	long			m_nOID;

	struct LinkId {
		long		m_Id1;
		long		m_Id2;
	};
	struct lessLinkId : public std::binary_function<LinkId,LinkId,bool> {
		bool operator ()(const LinkId &l1, const LinkId &l2) const {
			return (l1.m_Id1 != l2.m_Id1) ? l1.m_Id1 < l2.m_Id1 : l1.m_Id2 < l2.m_Id2;
		}
	};

	std::map<LinkId, ShpEntity, lessLinkId> m_LinkIds;

	typedef std::pair<SHPEntities2::iterator,SHPEntities2::iterator> Range;
public:
	ShpLink() : Entity() {}
	virtual ~ShpLink() {}

	virtual bool read(const char *cFileName);
	virtual bool mk_linkid_table();

	virtual bool find_link(long nId, ShpEntity *cEntity);
	virtual bool find_link(long nId1, long nId2, ShpEntity *cEntity);
	virtual bool find_link2(long nId1, long nId2, ShpEntity *cEntity);	// ÉeÅ[ÉuÉãÇ»Çµ
};

class ShpNode : public Entity
{
public:
	ShpNode() : Entity() {}
	virtual ~ShpNode() {}

	virtual bool find_node(long nId, ShpEntity *cObject);
};

#endif	//___SHP_ROAD_DATA_H___
