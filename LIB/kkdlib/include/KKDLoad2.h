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

// KKDLoad2.h: KKDLoad2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLOAD2_H__CCBA4A85_1792_455E_976D_62D58E0B7F85__INCLUDED_)
#define AFX_KKDLOAD2_H__CCBA4A85_1792_455E_976D_62D58E0B7F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KKDLayer2.h"
#include "KKDLoad.h"

typedef KKDLoadT<
	KKDRegulationRoadNodeLayer2,
	KKDRegulationRoadLinkLayer2,
	KKDLaneRegulationRoadNodeLayer2,
	KKDLaneRegulationRoadLinkLayer2,
	KKDLaneRegulationIntersectionNodeLayer2
> _KKDLoad2;

class KKDLoad2 : public _KKDLoad2
{
public:
	virtual bool load(const char* cDirectoryName, int cMeshCode, const char* cSuffix = 0) { return _KKDLoad2::load(cDirectoryName, cMeshCode, cSuffix); }
	virtual bool load(const char* cFileName) { return _KKDLoad2::load(cFileName); }
	virtual bool load(std::istream& cStream);

protected:
	virtual void clear();
};

#endif // !defined(AFX_KKDLOAD2_H__CCBA4A85_1792_455E_976D_62D58E0B7F85__INCLUDED_)
