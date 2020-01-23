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

// SindyGuide.cpp: SindyGuide クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyGuide.h"
#include "SindyRoadNetwork.h"

SindyGuide::SindyGuide(SindyDBConnection *cDBConnection) :
SindyLinkQueue(cDBConnection, sindy::layer::INF_GUIDE, sindy::layer::LQ_GUIDE)
{

}

SindyGuide::~SindyGuide()
{

}
