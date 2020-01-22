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

// Dijkstra.h: Dijkstra �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LinkTraceSpace.h"
#include "LinkArrivedInfo.h"

namespace sindy
{
class Dijkstra  
{
public:
	typedef std::map<long, LinkArrivedInfo> LinkArrivedInfoMap;
	typedef std::multimap<double, Route*> CostKeyRoutePtrMap;

// �\�z/����
	Dijkstra();
	virtual ~Dijkstra();

// ����
	virtual void clear();

// �ݒ�
	virtual bool setLinks(const LinkTraceSpace& rSpace, long nStartLinkId);
	virtual bool setEndLinkId(long nEndLinkId);

// �擾
	virtual long startLinkId() const { return m_itrStartLinkArrivedInfo != m_cLinkArrivedInfoMap.end() ? m_itrStartLinkArrivedInfo->first : 0; }

// �擾����
	virtual const Route* getRouteToEndLink();
	virtual const Route* getNextTraceRoute();

// ����
	virtual bool trace();

// �v���p�e�B
	LinkArrivedInfoMap m_cLinkArrivedInfoMap; ///< �����N�A�y�т��̓��B���ꗗ

private:
// ����
	void setRoutes(Route::CostKeyRouteMap& rCostKeyRouteMap);
	bool setRoute(Route& rRoute);

// �v���p�e�B
	LinkArrivedInfoMap::const_iterator m_itrStartLinkArrivedInfo; ///< �ǐՎn�[�����N
	LinkArrivedInfoMap::const_iterator m_itrEndLinkArrivedInfo; ///< �ǐՏI�[�����N
	CostKeyRoutePtrMap m_cCostKeyRoutePtrMap; ///< �R�X�g�����[�g�|�C���^�ꗗ
};

}

