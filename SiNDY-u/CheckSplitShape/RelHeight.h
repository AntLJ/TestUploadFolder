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

#include "LayerBase.h"

#define MAPHEIGHTINFO std::map<long,std::map<long,std::pair<long,long>>> // linkid,divid,fromlevel,tolevel

class CRelHeight : public CRecodeBase {
public:
	CRelHeight(){}
	~CRelHeight(){}
};

class CRelHeightAll: public CTableBase {
public:
	CRelHeightAll(){}
	~CRelHeightAll(){}

public:
	bool Init(ITablePtr ipRelHeightTable, const std::set<long>& setUsedLinkID);
	virtual bool LoadData();

private:

public:
	std::map<UNIQID,CRelHeight> m_mapData;   //!< DIVID�P�ʂ̃f�[�^���iLINKID�Ƃ���DIVID��UNIQID�j
	std::set<long> m_setLinkID;              //!< �����m�[�h�Ŏg�p���������NID
	MAPHEIGHTINFO m_mapHeight;               //!< SDE���̍����m�[�h�̃X�L�[�}�ɍ��킹���`�Ńf�[�^���m�ۂ�����
};
