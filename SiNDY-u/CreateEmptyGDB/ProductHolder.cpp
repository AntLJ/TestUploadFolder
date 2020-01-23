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
#include "ProductHolder.h"
#include <sindy/workspace.h>

namespace
{

/**
 * @brief 入力DBから指定のドメインをコピーし、リストに追加する
 * @param input          [in]  入力DBのワークスペース
 * @param domainInfoList [in]  コピーするドメイン情報一覧
 * @param result         [out] 結果の追加先
 */
void domainCopy(const IWorkspacePtr& input, const std::set<create_info::CreateDomainInfo>& domainInfoList, std::vector<product::ProductDomainDef>& result)
{
	IWorkspaceDomainsPtr workspaceDomain(input);

	IEnumDomainPtr ipEnumDomain;
	workspaceDomain->get_Domains( &ipEnumDomain );
	ipEnumDomain->Reset();
	IDomainPtr ipDomain;
	while( ipEnumDomain->Next( &ipDomain ) == S_OK && ipDomain)
	{
		CComBSTR domainName;
		ipDomain->get_Name(&domainName);
		auto it = std::find_if(domainInfoList.cbegin(), domainInfoList.cend(),
			[&domainName](const create_info::CreateDomainInfo& elem){ return util::eq(elem.m_name, domainName.m_str); });
		if( domainInfoList.cend() == it )
			continue;
		product::ProductDomainDef def;
		def.m_name = domainName.m_str;
		def.m_domainPtr = AheGetClone(ipDomain);
		result.push_back( def );
	}
}

/**
 * @brief 入力DBから指定のデータセットをコピーし、リストに追加する
 * @param input           [in]  入力DBのワークスペース
 * @param datasetInfoList [in]  コピーするデータセット情報一覧
 * @param result          [out] 結果の追加先
 */
void datasetCopy(const IWorkspacePtr& input, const std::set<create_info::CreateDatasetInfo>& datasetInfoList, std::vector<product::ProductDatasetDef>& result)
{
	IEnumDatasetPtr featureEnumDataset = util::getDatasets(input, esriDTFeatureDataset);
	 if(!featureEnumDataset) return;
	 featureEnumDataset->Reset();
	IDatasetPtr dataset;
	while( featureEnumDataset->Next(&dataset) == S_OK && dataset )
	{
		CComBSTR datasetName;
		dataset->get_Name(&datasetName);

		auto it = std::find_if(datasetInfoList.cbegin(), datasetInfoList.cend(),
			[&datasetName](const create_info::CreateDatasetInfo& elem){ return util::eq(elem.m_name, datasetName.m_str); });
		if( datasetInfoList.end() == it )
			continue;
		// データセットはCloneが作れないようなので空間参照だけコピー
		ISpatialReferencePtr spRef;
		IGeoDatasetPtr(dataset)->get_SpatialReference(&spRef);

		product::ProductDatasetDef def;
		def.m_name = util::eraseOwnerName(datasetName.m_str);
		def.m_spRef = AheGetClone(spRef);
		result.push_back( def );
	}
}

/**
 * @brief 元レイヤのフィールドをコピーする
 * @warning 形状フィールドだけは追加フィールド追加後に追加
 * @param result     [out] フィールドコピーした結果の格納先
 * @param shapeField [out] 形状フィールドの格納先
 * @param layer      [in]  フィールドコピー元レイヤ
 * @return コピーしたフィールド一覧
 */
std::vector<uh::tstring> copyField(IFieldsEditPtr& result, IFieldPtr& shapeField, const ITablePtr& layer)
{
	std::vector<uh::tstring> srcFieldNameList;

	// 元テーブルのフィールドを読込み
	IFieldsPtr srcFields;
	layer->get_Fields(&srcFields);
	long lCount = 0;
	srcFields->get_FieldCount(&lCount);
	for(long index = 0; index<lCount; ++index)
	{
		IFieldPtr field;
		srcFields->get_Field(index, &field);
		CComBSTR name;
		field->get_Name(&name);
		srcFieldNameList.push_back( name.m_str );

		esriFieldType type = UNKOWN_FIELD_TYPE;
		field->get_Type(&type);
		switch(type)
		{
		default:
			result->AddField((IFieldPtr)AheGetClone(field));
			break;
		case esriFieldTypeGeometry:
			shapeField = (IFieldPtr)AheGetClone(field);
			continue;
			break;
		}
	}
	return srcFieldNameList;
}


/**
 * @brief フィールドを追加する
 * @warning 形状フィールドだけは追加フィールド追加後に追加
 * @param result     [out] フィールドの追加先
 * @param addedField [in]  追加するフィールド
 */
void addField(IFieldsEditPtr& result, const IFieldsEditPtr& addedField)
{
	if(!addedField) return;
	// 元テーブルのフィールドを読込み
	long lCount = 0;
	addedField->get_FieldCount(&lCount);
	for(long index = 0; index<lCount; ++index)
	{
		IFieldPtr field;
		addedField->get_Field(index, &field);
		result->AddField( IFieldEditPtr(field) );
	}
}

/**
 * @brief 追加フィールドが元レイヤにあるなら、それを除く
 * @param fieldList   [in] 元レイヤのフィールド一覧
 * @param addedField  [in] 追加フィールド
 * @param inputStr    [in] 入力DB接続文字列
 * @param layerName   [in] コピー対象レイヤ名
 * @return 元レイヤのフィールドを除いた追加フィールド
 * @retval nullptr addedFieldがnull
 */
IFieldsEditPtr eraseDupliField(
	const std::vector<uh::tstring>& fieldList
	, const IFieldsPtr& addedField
	, const uh::tstring& inputStr
	, const uh::tstring& layerName
	)
{
	if(!addedField) return nullptr;
	IFieldsEditPtr result(CLSID_Fields);

	long fieldCount = 0;
	addedField->get_FieldCount(&fieldCount);
	IFieldPtr field;
	for(long index=0; index < fieldCount; ++index)
	{
		addedField->get_Field(index, &field);
		CComBSTR name;
		field->get_Name(&name);
		auto it = std::find_if(fieldList.cbegin(), fieldList.cend(),
			[&name](const uh::tstring& fieldName){ return util::eq(fieldName, name.m_str); });
		if( fieldList.cend() != it )
		{
			RUNLOGGER.warn( IDS_EXIST_COLUMN_DEF, inputStr, layerName, *it );
			continue;
		}
		result->AddField( field );
	}
	return result;
}

/**
 * @brief レイヤ一覧の情報を元にフィールドを作成
 * @param inputDB       [in]  入力DBのワークスペース
 * @param inputStr      [in]  入力DB接続文字列
 * @param layerInfoList [in]  コピーするレイヤ情報一覧
 * @param createDefList [out] 生成フィールドリストの追加先
 */
bool copyLayer(
	IWorkspacePtr& inputDB
	, const uh::tstring& inputStr
	, const std::vector<create_info::CreateLayerInfo>& layerInfoList
	, std::vector<product::ProductLayerDef>& result
	)
{
	bool bSuccess = true;
	for(const auto& layerInfo : layerInfoList)
	{
		ITablePtr layer = AheOpenTableByTableName( inputDB, layerInfo.m_name.c_str() );
		if(!layer)
		{
			RUNLOGGER.fatal(IDS_FAILED_OPEN_TABLE, inputStr, layerInfo.m_name, _T(""));
			bSuccess = false;
			continue;
		}

		result.emplace_back();
		auto& productLayerDef = result.back();
		productLayerDef.m_name = layerInfo.m_name;
		productLayerDef.m_aliasName = layerInfo.m_aliasName.get();

		productLayerDef.m_fields = IFieldsEditPtr(CLSID_Fields);
		{
			// 形状フィールドは最後に追加するため、退避先用意
			IFieldPtr shapeFieldTmp;
			// フィールドをコピーしつつ元レイヤのフィールド一覧取得
			auto srcFieldNameList = copyField( productLayerDef.m_fields, shapeFieldTmp, layer );
			// 追加フィールドを追加
			IFieldsEditPtr fields = eraseDupliField(srcFieldNameList, layerInfo.m_addFields, inputStr, layerInfo.m_name);
			if(!fields)
				fields = IFieldsEditPtr(CLSID_Fields);
			if(shapeFieldTmp)
			{
				// 形状フィールドは最後に追加(iPCのテンプレートレイヤの形式に合わせている)
				fields->AddField(shapeFieldTmp);
			}
			addField(productLayerDef.m_fields, fields);
		}

		IUIDPtr clsid;
		layer->get_CLSID(&clsid);
		// CLSIDの取得
		productLayerDef.m_srcCLSID = util::copyUID(clsid);
		// EXTCLSIDの取得
		IUIDPtr extClsid;
		layer->get_EXTCLSID(&extClsid);
		productLayerDef.m_srcEXTCLSID= util::copyUID(extClsid);

		// フィーチャクラスの場合、所属するデータセットもコピー対象
		IFeatureClassPtr fc(layer);
		if(!fc) continue;
		// 後でまとめてコピーを作成するので、ここでは名称を取得するのみ
		IFeatureDatasetPtr fcDataset;
		fc->get_FeatureDataset(&fcDataset);
		if(fcDataset)
		{
			CComBSTR name;
			fcDataset->get_Name(&name);
			// 生成時にはユーザー名が不要なので削除
			productLayerDef.m_datasetName = util::eraseOwnerName(name.m_str);
		}

		esriFeatureType featureType = UNKOWN_FEATURE_TYPE;
		fc->get_FeatureType(&featureType);
		productLayerDef.m_srcFeatureType = featureType;

		CComBSTR shapeName;
		fc->get_ShapeFieldName(&shapeName);
		productLayerDef.m_shapeFieldName = shapeName.m_str;
	}
	return bSuccess;
}

} // namespace

bool CProductHolder::create()
{
	for(const auto& createInfo : m_createInfoMap)
	{
		IWorkspacePtr inputWorks = sindy::create_workspace( createInfo.first.c_str() );
		if(!inputWorks)
		{
			RUNLOGGER.fatal( IDS_CONNECT_INPUTDB, createInfo.first, _T(""), _T("") );
			return false;
		}

		if( !copyLayer( inputWorks, createInfo.first, createInfo.second.m_layerInfo, m_productDef.m_layerList ) )
			return false;
		domainCopy( inputWorks, createInfo.second.m_domainInfo, m_productDef.m_domainList );
		datasetCopy( inputWorks, createInfo.second.m_datasetInfo, m_productDef.m_datasetList );
	}
	return true;
}
