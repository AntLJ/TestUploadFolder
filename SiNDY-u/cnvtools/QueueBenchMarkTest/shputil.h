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
 *	@file	shputil.h
 *	@brief	
 *	@author	F.Abe
 *	@date	2004/10/03
 */

#ifndef	___SHAPE_UTIL_H___
#define	___SHAPE_UTIL_H___

#include "../sindy2shape/Names.h"

struct ShpEntity
{
	CSHPObject				m_shpObject;
	std::vector<VARIANT>	m_vecAttributes;

	ShpEntity() : m_shpObject(), m_vecAttributes() {}
	ShpEntity(CSHPObject &o, std::vector<VARIANT> &v) : m_shpObject(), m_vecAttributes() { m_shpObject = o; m_vecAttributes = v; }
	virtual ~ShpEntity() {}
};

typedef std::map<long, ShpEntity> SHPEntities;
typedef std::multimap<long, ShpEntity> SHPEntities2;
typedef	std::multimap< long, std::vector<VARIANT> > DBFAttributes;

long DBFGetFieldIndex(CDBFHandle *cHandle, const char *cFieldName);
bool DBFGetAttributes(CDBFHandle *cHandle, DBFAttributes *cAttributes, const char *cFieldName = shape::kObjectId);
bool SHPGetEntities(CSHPHandle *cShpHandle, CDBFHandle *cDbfHandle, SHPEntities *cEntities, const char *cFieldName = shape::kObjectId);
bool SHPGetEntities2(CSHPHandle *cShpHandle, CDBFHandle *cDbfHandle, SHPEntities2 *cEntities, const char *cFieldName = shape::kObjectId);

#endif	//___SHAPE_UTIL_H___
