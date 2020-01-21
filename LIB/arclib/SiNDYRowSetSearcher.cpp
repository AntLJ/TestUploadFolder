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

#include "stdafx.h"
#pragma warning (disable : 4290)
#include "SiNDYRowSetSearcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

#if(0)
HRESULT (RowSetSearcher::*(_row_set_searcher_traits<RowSetSearcher>::accessor<ICursor*>::get))(ICursor**) =
	RowSetSearcher::SearchNext;

HRESULT (FeatureSetSearcher::*(_row_set_searcher_traits<FeatureSetSearcher>::accessor<IFeatureCursor*>::get))(IFeatureCursor**) =
	FeatureSetSearcher::SearchNext;
#endif
