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
#pragma	warning(disable : 4192)
#pragma	warning(disable : 4278)
#import <esriSystem.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer") rename("ISet", "_ISet")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment") rename("ISet","_ISet") rename("Path", "_Path")
#import <esriDisplay.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment") rename("ISet","_ISet")
#import <esriServer.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriOutput.olb> raw_interfaces_only no_namespace named_guids
#import <esriGeoDatabase.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("ItemInfo","_ItemInfo") rename("IRelationship","_IRelationship") rename("ISet", "_ISet")
#import <esriGISClient.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriDataSourcesFile.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesOleDB.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesRaster.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor")  rename("ISet", "_ISet")
#import <esriGeoDatabaseDistributed.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriCarto.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("PropertyInfo", "_PropertyInfo")  rename("ITableDefinition", "_ITableDefinition" ) rename("ISet","_ISet")
#import <esriCartoUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriLocation.olb> raw_interfaces_only no_namespace named_guids
#import <esriNetworkAnalysis.olb> raw_interfaces_only no_namespace named_guids rename("ISet","_ISet")
#import <esriControls.olb> raw_interfaces_only raw_native_types no_namespace named_guids rename("ISet","_ISet")
#import <esriGeoAnalyst.olb> raw_interfaces_only no_namespace named_guids rename("ICursor","_ICursor")
#import <esri3DAnalyst.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriGlobeCore.olb> raw_interfaces_only no_namespace named_guids exclude("IGPFeatureRecordSetLayer","IFeatureParameterLayer")
#import <esriSpatialAnalyst.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IGeoDataset","IMathOp","IExtractionOp","IExtractionOp2")
#import <esriFramework.olb> raw_interfaces_only no_namespace named_guids function_mapping exclude("OLE_COLOR","OLE_HANDLE","ICursorPtr","VARTYPE","UINT_PTR") rename("ICommand","_ICommand") rename("IColorPtr","_IColorPtr") rename("ISet","_ISet")
#import <esriGeoDatabaseUI.olb> raw_interfaces_only no_namespace named_guids rename("ICursor","_ICursor") rename("VersionManager","ESRIVersionManager")
#import <esriDisplayUI.olb> raw_interfaces_only no_namespace named_guids
#import <esriCatalog.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriCatalogUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") exclude("IGxBrowser") rename("ISet","_ISet")
#import <esriGeoDatabaseExtensions.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor")
#import <esriArcMapUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids function_mapping rename("ICommand","_ICommand") exclude("IMxApplication2","IDataConnectionPropertyPage2","IDataGraphWizard") rename("ISet","_ISet")
#import <esriEditor.olb> raw_interfaces_only raw_native_types no_namespace named_guids rename("IRow","_IRow") rename("ISet","_ISet")
#import <esriGeoprocessing.olb> raw_interfaces_only no_namespace named_guids rename("ICursor","_ICursor") rename("EnumBSTR","_EnumBSTR")
#import <esriGeoStatisticalAnalyst.olb> raw_interfaces_only no_namespace named_guids
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
