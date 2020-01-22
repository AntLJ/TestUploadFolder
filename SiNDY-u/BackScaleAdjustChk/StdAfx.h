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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8E678AC7_A957_4E00_BEAE_0AFA7BBCCADB__INCLUDED_)
#define AFX_STDAFX_H__8E678AC7_A957_4E00_BEAE_0AFA7BBCCADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlbase.h"

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#ifdef	_ARC_9_1_
#import "c:\Program Files\ArcGIS\com\esriSystem.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriGeometry.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriGeoDatabase.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriDataSourcesGDB.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriDisplay.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#import "c:\Program Files\ArcGIS\com\esriDataSourcesFile.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") 
#else
#import "c:\ArcGIS\arcexe82\Bin\esriCore.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids , exclude ("OLE_COLOR", "OLE_HANDLE")
#endif
#pragma warning(pop)

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8E678AC7_A957_4E00_BEAE_0AFA7BBCCADB__INCLUDED_)
