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

/**
* @file MapHelper.cpp
*
* @brief IMapに関連する処理を行うCMapHelperの実装ファイル
*/
#include "stdafx.h"
#include "MapHelper.h"
#include "AheGlobals.h"
#include "ArcHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMapHelper::CMapHelper( CArcHelper& cArcHelper)
{
	m_piMap = cArcHelper.GetMap();
}

CMapHelper::CMapHelper()
{
}

CMapHelper::~CMapHelper()
{
}

CMapHelper& CMapHelper::operator=( CMapHelper& cMapHelper)
{
	m_piMap = cMapHelper.m_piMap;

	return *this;
}

CMapHelper& CMapHelper::operator=( CArcHelper& cArcHelper)
{
	m_piMap = cArcHelper.GetMap();

	return *this;
}
