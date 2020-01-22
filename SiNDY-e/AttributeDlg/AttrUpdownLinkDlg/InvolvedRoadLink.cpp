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
#include "InvolvedRoadLink.h"


InvolvedRoadLink::InvolvedRoadLink(void)
:m_roadID(0),
m_directionC(sindy::schema::adam_v2::rel_road_link_updown_link::roadlink_dir_code::kUnset)
{}

InvolvedRoadLink::InvolvedRoadLink(IFeaturePtr roadLink, _IRowPtr relRow ):
	m_roadLink(roadLink),
	m_relRow(relRow)
{
	m_roadID = AheGetOID(roadLink);
	m_directionC = AheGetAttribute(relRow,
		sindy::schema::adam_v2::rel_road_link_updown_link::kRoadLinkDirectionC).intVal;
}

InvolvedRoadLink::InvolvedRoadLink(IFeaturePtr roadLink):
	m_roadLink(roadLink),
	m_directionC(sindy::schema::adam_v2::rel_road_link_updown_link::roadlink_dir_code::kUnset)

{
	m_roadID = AheGetOID(roadLink);
}