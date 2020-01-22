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

// LinkCostList.h: LinkCostList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

namespace sindy
{
class LinkCostList  
{
public:
// ��`
	typedef std::map<long, double> RoadClassCostMap;
	typedef std::map<long, double> NaviClassCostMap;
	typedef std::map<long, double> RoadWidthCostMap;
	typedef std::map<long, double> LinkClassCostMap;

// �\�z/����
	LinkCostList();
	virtual ~LinkCostList();

// �v���p�e�B
	static RoadClassCostMap m_cRoadClassCostMap;
	static NaviClassCostMap m_cNaviClassCostMap;
	static RoadWidthCostMap m_cRoadWidthCostMap;
	static LinkClassCostMap m_cLinkClassCostMap;
};

}

