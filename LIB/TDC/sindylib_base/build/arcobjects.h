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

#pragma once

////////////////////////////
// ArcObjects
////////////////////////////
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#pragma warning(disable : 4042)
#pragma warning(disable : 4278)
#import<esriSystem.olb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("XMLSerializer", "_XMLSerializer") rename("ISet", "_ISet")
#import<esriSystemUI.olb>			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IProgressDialog", "IESRIProgressDialog") rename("ICommand", "_ICommand")
#import<esriGeometry.olb>			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("ISegment", "_ISegment")
#import<esriDisplay.olb>			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import<esriGeoDatabase.olb>		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#import<esriGeoDatabaseUI.olb>      raw_interfaces_only raw_native_types no_namespace
#import<esriDataSourcesFile.olb>	raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import<esriCarto.olb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "UINT_PTR") rename("ITableDefinition", "_ITableDefinition")
#import<esriArcMapUI.olb>           raw_interfaces_only raw_native_types no_namespace named_guids
#import<esriFramework.olb>          raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("ICommand", "_ICommand")
#import<esriEditor.olb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import "tlb/sindyeCore.tlb"		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)
