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
 *	@file	queue.h
 *	@brief	
 *	@author	F.Abe
 *	@date	2004/10/03
 */

#ifndef	___QUEUE_H___
#define	___QUEUE_H___

#include "shputil.h"

class Queue
{
public:
	CDBFHandle		m_lqhdl;
	CDBFHandle		m_infhdl;
	DBFAttributes	m_queue;
	DBFAttributes	m_attributes;
	DBFAttributes	m_map;

public:
	Queue() : m_lqhdl(), m_infhdl(), m_queue(), m_attributes() {}
	virtual ~Queue() {}

	virtual bool read(const char *cQFileName, const char *cIFileName, const char *cFieldName = 0);	// 拡張子抜きのファイル名を指定
};

class Entity
{
public:
	CSHPHandle		m_shphdl;
	CDBFHandle		m_dbfhdl;
	SHPEntities		m_entities;

public:
	Entity() : m_shphdl(), m_dbfhdl(), m_entities() {}
	virtual ~Entity() {}

	virtual bool read(const char *cFileName);	// 拡張子抜きのファイル名を指定
};

#endif	//___QUEUE_H___
