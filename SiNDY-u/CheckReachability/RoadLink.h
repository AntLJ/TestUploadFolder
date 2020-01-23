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

#include <arctl/field.h>
#include <sindy/schema.h>

ARCTL_FIELD_VALUE_CLASS(
	RoadLink,
	((arctl::field::oid_t))
	(2(sindy::schema::road_link::kFromNodeID, long))
	(2(sindy::schema::road_link::kToNodeID, long))
	(2(sindy::schema::road_link::kNoPassage, long))
	(2(sindy::schema::road_link::kOneway, long))
);
