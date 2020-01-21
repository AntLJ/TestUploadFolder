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

// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#include <atlbase.h>
#include <atlwin.h>

#pragma warning( push )
#pragma warning( disable : 6011 )
#include <atlapp.h>
#pragma warning( pop )

#import <esriSystem.olb> raw_interfaces_only raw_native_types no_namespace exclude("OLE_HANDLE", "OLE_COLOR")
#import <esriGeometry.olb> raw_interfaces_only raw_native_types no_namespace rename("ISegment", "_ISegment")
#import <esriGeoDatabase.olb> raw_interfaces_only raw_native_types no_namespace
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace

#include <atlctrls.h>

#pragma warning( disable : 4561 )
