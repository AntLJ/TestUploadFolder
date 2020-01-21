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

// KKDLayer.h: KKDLayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLAYER_H__86724C39_6B21_4857_949A_46116C2A7DF7__INCLUDED_)
#define AFX_KKDLAYER_H__86724C39_6B21_4857_949A_46116C2A7DF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DRMALayer.h>
#include "KKDAdmin.h"
#include "KKDRegulationRoadNode.h"
#include "KKDRegulationRoadLink.h"
#include "KKDLaneRegulationRoadNode.h"
#include "KKDLaneRegulationRoadLink.h"
#include "KKDLaneRegulationIntersectionNode.h"

typedef DRMALayerTemplate<KKDAdmin> KKDAdminLayer;
typedef DRMAContinualLayerTemplate<KKDRegulationRoadNode> KKDRegulationRoadNodeLayer;
typedef DRMAContinualLayerTemplate<KKDRegulationRoadLink> KKDRegulationRoadLinkLayer;
typedef DRMAContinualLayerTemplate<KKDLaneRegulationRoadNode> KKDLaneRegulationRoadNodeLayer;
typedef DRMAContinualLayerTemplate<KKDLaneRegulationRoadLink> KKDLaneRegulationRoadLinkLayer;
typedef DRMAContinualLayerTemplate<KKDLaneRegulationIntersectionNode> KKDLaneRegulationIntersectionNodeLayer;

#endif // !defined(AFX_KKDLAYER_H__86724C39_6B21_4857_949A_46116C2A7DF7__INCLUDED_)
