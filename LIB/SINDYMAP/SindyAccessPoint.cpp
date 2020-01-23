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

// SindyAccessPoint.cpp: SindyAccessPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyAccessPoint.h"
#include "SindyRoadNetwork.h"

SindyAccessPoint::SindyAccessPoint(SindyDBConnection *cDBConnection) :
SindyHighwayPoint(cDBConnection, sindy::layer::ACCESS_POINT)
{

}

SindyAccessPoint::~SindyAccessPoint()
{

}
