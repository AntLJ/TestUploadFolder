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

// SindyVics.cpp: SindyVics クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyVics.h"
#include "SindyRoadNetwork.h"

SindyVics::SindyVics(SindyDBConnection *cDBConnection) :
SindyLinkQueue(cDBConnection, sindy::layer::INF_VICS, sindy::layer::LQ_VICS)
{

}

SindyVics::~SindyVics()
{

}
