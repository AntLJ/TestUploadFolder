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
#include"Definition.h"
#include<map>
#include<set>

class CMerge
{
public:
	CMerge(void);
	~CMerge(void);
	//���ڂ��郁�b�V���Ɨאڃ��b�V���̃��X�g�ŃL�[����v������̂����o�����X�g�����
	bool MakeList(std::multimap<Key,Info> & rMainMap, std::multimap<Key,Info> & rNeighborMap,std::set<CheckKey> & rMeregeSet,long aShapeType);
	bool MakeListAllowError(std::multimap<Key,Info> & rMainMap, std::multimap<Key,Info> & rNeighborMap,std::set<CheckKey> & rMeregeSet,long aShapeType);


};
