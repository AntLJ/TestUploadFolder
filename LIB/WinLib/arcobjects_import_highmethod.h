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
#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer") rename("ISet", "_ISet")
#import <esriSystemUI.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment") rename("ISet","_ISet")
#import <esriDisplay.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment") rename("ISet","_ISet")
#import <esriServer.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriOutput.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("IRelationship", "_IRelationship") rename("ISet", "_ISet")
#import <esriGISClient.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriDataSourcesFile.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriDataSourcesGDB.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesOleDB.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriDataSourcesRaster.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor") rename("IRow","_IRow")
#import <esriGeoDatabaseDistributed.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriCarto.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("ISet","_ISet")  rename("ITableDefinition", "_ITableDefinition" )
#import <esriCartoUI.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriLocation.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriNetworkAnalysis.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriControls.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriGeoAnalyst.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ICursor","_ICursor")
#import <esri3DAnalyst.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriGlobeCore.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("IGPFeatureRecordSetLayer","IFeatureParameterLayer")
#import <esriSpatialAnalyst.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("IGeoDataset","IMathOp","IExtractionOp","IExtractionOp2")
#import <esriFramework.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids function_mapping exclude("OLE_COLOR","OLE_HANDLE","ICursorPtr","VARTYPE","UINT_PTR") rename("ICommand","_ICommand") rename("IColorPtr","_IColorPtr") rename("ISet","_ISet")
#import <esriGeoDatabaseUI.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ICursor","_ICursor") rename("VersionManager","ESRIVersionManager")
#import <esriDisplayUI.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriCatalog.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids
#import <esriCatalogUI.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") exclude("IGxBrowser")
#import <esriGeoDatabaseExtensions.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor")
#import <esriArcMapUI.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids function_mapping rename("ICommand","_ICommand") exclude("IMxApplication","IMxApplication2","IDataConnectionPropertyPage2","IDataGraphWizard")
#import <esriEditor.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids rename("IRow","_IRow")
#import <esriGeoprocessing.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ICursor","_ICursor")
#import <esriGeoStatisticalAnalyst.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" high_method_prefix("_") raw_method_prefix("") no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
