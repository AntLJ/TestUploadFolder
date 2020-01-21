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

// DRMALoad2.h: DRMALoad2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMALOAD2_H__97D7DE2C_3319_48E8_82E6_E7B974B54193__INCLUDED_)
#define AFX_DRMALOAD2_H__97D7DE2C_3319_48E8_82E6_E7B974B54193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMALayer2.h"
#include "DRMALoad.h"

typedef DRMALoadT<
	DRMABaseRoadNodeLayer2,
	DRMABaseRoadLinkLayer2,
	DRMABaseRoadLinkAttrLayer2,
	DRMARoadLinkRelationLayer2,
	DRMABeaconLocationLayer2,
	DRMAAllRoadNodeLayer2,
	DRMAAllRoadLinkLayer2,
	DRMASiteLayer2,
	DRMAAdminLocationLayer2,
	DRMARailwayLocationLayer2,
	DRMAFacilLocationLayer2,
	DRMAFacilShapeLayer2,
	DRMAPlaceNameLocationLayer2,
	DRMAAllRoadLinkAttrLayer2
> _DRMALoad2;

/**
 * @brief DRMALoad拡張クラス
 *
 * @note DRMA
 */
class DRMALoad2 : public _DRMALoad2
{
public:

	virtual bool load(const char* cDirectoryName, int cMeshCode, const char* cSuffix = ".mt") { return _DRMALoad2::load(cDirectoryName, cMeshCode, cSuffix); }
	virtual bool load(const char* cFileName) { return _DRMALoad2::load(cFileName); }
	virtual bool load(std::istream& cStream);

protected:
	virtual void clear();
};

#endif // !defined(AFX_DRMALOAD2_H__97D7DE2C_3319_48E8_82E6_E7B974B54193__INCLUDED_)
