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
#include "CreateEmptyLayer.h"
#include <sindy/workspace.h>
#include <ArcHelperEx/AheLayerFunctions.h>

#include "Utility.h"

namespace
{
template <typename TPtr>
inline bool createFeatureClass(
	TPtr& dst, 
	const product::ProductLayerDef& layerDef,
	const uh::tstring& dbPath
	)
{
	IFeatureClassPtr fcTmp;
	HRESULT re = dst->CreateFeatureClass(
		CComBSTR(layerDef.m_name.c_str()),
		layerDef.m_fields,
		layerDef.m_srcCLSID,
		layerDef.m_srcEXTCLSID,
		layerDef.m_srcFeatureType,
		CComBSTR(layerDef.m_shapeFieldName.c_str()),
		CComBSTR(_T("")),
		&fcTmp );
	if( S_OK!= re )
	{
		tstringstream ss;
		ss << _T("ERROR[") << re << _T("]");
		RUNLOGGER.fatal(IDS_FAILED_CREATE_LAYER, dbPath, layerDef.m_name, ss.str());
		return false;
	}
	// 以下、付随する情報を設定
	util::SchemaGuard guard(fcTmp);
	IClassSchemaEditPtr(fcTmp)->AlterAliasName(CComBSTR(layerDef.m_aliasName.c_str()));
	return true;
}

inline bool createLayer(
	IFeatureWorkspacePtr& dst, 
	const product::ProductLayerDef& layerDef,
	const uh::tstring& dbPath
	)
{
	// 形状フィールドが空 = テーブルと判断
	if( layerDef.m_shapeFieldName.empty() )
	{
		ITablePtr table;
		return SUCCEEDED(dst->CreateTable(
				CComBSTR(layerDef.m_name.c_str()),
				layerDef.m_fields, layerDef.m_srcCLSID, layerDef.m_srcEXTCLSID,
				CComBSTR(_T("")),
				&table ));
	}
	return createFeatureClass(dst, layerDef, dbPath);
}
} // namespace

bool CCreateEmptyLayer::create()
{
	for(const auto& targetDB : m_targetPathList)
	{
		IWorkspacePtr outDB = sindy::create_workspace(targetDB.c_str());
		if(!outDB)
		{
			RUNLOGGER.fatal(IDS_CONNECT_OUTPUTDB, targetDB, _T(""), _T(""));
			continue;
		}

		// データセット作成
		{
			IFeatureWorkspacePtr featureWorks((IWorkspace*)outDB);
			for( const auto& datasetDef : m_productDef.m_datasetList )
			{
				// 既存がすでにあるなら作成しない
				IFeatureDatasetPtr datasetTmp;
				if(SUCCEEDED(featureWorks->OpenFeatureDataset(CComBSTR(datasetDef.m_name.c_str()), &datasetTmp)))
				{
					ISpatialReferencePtr dstSpRef;
					IGeoDatasetPtr(datasetTmp)->get_SpatialReference(&dstSpRef);
					if( !util::isEqual(datasetDef.m_spRef, dstSpRef) )
					{
						tstringstream ss;
						ss << _T("DATASET[") << datasetDef.m_name << _T("]");
						RUNLOGGER.error(IDS_EXIST_DIFF_DATASET, targetDB, _T(""), _T(""), ss.str());
					}
					continue;
				}
				if( S_OK!=featureWorks->CreateFeatureDataset(CComBSTR(datasetDef.m_name.c_str()), datasetDef.m_spRef, &datasetTmp) )
				{
					tstringstream ss;
					ss << _T("DATASET[") << datasetDef.m_name << _T("]");
					RUNLOGGER.error(IDS_FAILED_CREATE_DATASET, targetDB, _T(""), _T(""), ss.str());
					continue;
				}
			}

		}

		// ドメイン作成
		{
			IWorkspaceDomainsPtr domainWorks((IWorkspace*)outDB);
			for( const auto& domain : m_productDef.m_domainList )
			{
				// 作成先に同名のドメインがすでに存在する場合、作成しない
				CComBSTR name;
				domain.m_domainPtr->get_Name(&name);

				IDomainPtr outDomain;
				domainWorks->get_DomainByName( name, &outDomain );
				if( outDomain )
				{
					tstringstream ss;
					ss << _T("DOMAIN[") << domain.m_name << _T("]");
					RUNLOGGER.warn(IDS_WARN_EXIST_DOMAIN, targetDB, _T(""), _T(""), ss.str());
					continue;
				}
				long lIndex = 0;
				domainWorks->AddDomain( domain.m_domainPtr, &lIndex );
			}
		}

		for(const auto& createDef : m_productDef.m_layerList)
		{
			// すでに存在するなら作成しない
			ITablePtr targetLayer =  AheOpenTableByTableName(outDB, createDef.m_name.c_str());
			if( targetLayer )
			{
				RUNLOGGER.warn(IDS_WARN_EXIST_LAYER, targetDB, createDef.m_name, _T(""));
				continue;
			}


			if( createDef.m_datasetName.empty() )
			{
				createLayer( IFeatureWorkspacePtr((IWorkspace*)outDB), createDef, targetDB);
				continue;
			}

			IFeatureWorkspacePtr dstWorks((IWorkspace*)outDB);
			IFeatureDatasetPtr datasetTmp;
			if(FAILED(dstWorks->OpenFeatureDataset(CComBSTR(createDef.m_datasetName.c_str()), &datasetTmp)))
			{
				tstringstream ss;
				ss << _T("DATASET[") << createDef.m_datasetName << _T("]");
				RUNLOGGER.error(IDS_FAILED_OPEN_DATASET, targetDB, createDef.m_name, _T(""), ss.str());
				continue;
			}
			createFeatureClass( datasetTmp, createDef, targetDB);
		}
	}
	return true;
}
