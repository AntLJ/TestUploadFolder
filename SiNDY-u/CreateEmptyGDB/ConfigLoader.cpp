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
#include "ConfigLoader.h"
#include <sindy/workspace.h>
#include <ArcHelperEx/AheLayerFunctions.h>

#include "ParseFunction.h"

namespace
{

/**
 * @brief IEnumDatasetPtrに所属するレイヤ名一覧を取得
 * @param dateSet [in] レイヤを保持するデータセット
 * @param ownerName [in] オーナー名
 * @return 名前一覧
 */
std::vector<CString> getLayerNameList(IEnumDatasetPtr dataSet, const CString& ownerName)
{
	std::vector<CString> result;
	dataSet->Reset();
	IDatasetPtr data;
	while( dataSet->Next(&data) == S_OK && data )
	{
		if( 0!=ownerName.CompareNoCase( AheGetFeatureClassOwnerName( data ) ) )
			continue;
		CComBSTR name;
		data->get_Name(&name);
		result.push_back( util::eraseOwnerName(name.m_str).c_str() );
	}
	return result;
}

/**
 * @brief データセットに所属するレイヤ名一覧を取得
 * @param dateSet   [in] レイヤを保持するデータセット
 * @param ownerName [in] オーナー名
 * @return 名前一覧
 */
std::vector<CString> getLayerNameListInDataset(IEnumDatasetPtr dataSet, const CString& ownerName)
{
	std::vector<CString> result;
	dataSet->Reset();
	IDatasetPtr data;
	while( dataSet->Next(&data) == S_OK && data )
	{
		IEnumDatasetPtr subData;
		data->get_Subsets( &subData );
		auto layerList = getLayerNameList( subData, ownerName );
		std::copy(layerList.begin(), layerList.end(), std::back_inserter(result));
	}
	return result;
}

/**
 * @brief レイヤ一覧のフィールドを読込み
 * @param inputDB    [in]  入力DBのワークスペース
 * @param inputStr   [in]  入力DBの接続文字列
 * @param layerList  [in]  レイヤ一覧
 * @param createInfo [out] レイヤ・ドメイン・フィーチャデータセットの情報の追加先
 * @param checkFlag  [in]  定義済みレイヤには処理を行わないようにチェックするか
 * @return 処理の成否
 */
bool makeCreateInfo(
	IWorkspacePtr& inputDB
	, const uh::tstring& inputStr
	, const std::vector<CString>& layerList
	, create_info::CreateInfo& createInfo
	, bool checkFlag)
{
	for(auto& layerName : layerList)
	{
		ITablePtr layer =  AheOpenTableByTableName( inputDB, layerName );
		if(!layer)
		{
			RUNLOGGER.fatal(IDS_FAILED_OPEN_TABLE, inputStr, layerName.GetString(), _T(""));
			return false;
		}

		auto itInfo = std::find_if(createInfo.m_layerInfo.begin(), createInfo.m_layerInfo.end(),
			[&layerName](create_info::CreateLayerInfo& cDef)
			{ return util::eq(layerName.GetString(),cDef.m_name); } );

		// 定義済みの場合(設定ファイルで指定されたレイヤである場合)は
		// 定義を作成せずにドメイン・データセット取得のみ
		if( createInfo.m_layerInfo.end()!=itInfo )
		{
			if(checkFlag)
			{
				// 設定ファイルでmesh_layer_nameで指定したレイヤを生成対象に指定した状態で、
				// メッシュ生成機能を実行しているとここにくる
				// この場合、設定ファイルでの設定を優先するので、警告文だけ出して次へ。
				RUNLOGGER.warn(IDS_WARN_EXIST_TABLE_DEF, inputStr, layerName.GetString(), _T(""));
				continue;
			}
		}
		else
		{
			// 未定義の場合は定義を作成
			createInfo.m_layerInfo.emplace_back();
			createInfo.m_layerInfo.back().m_name = layerName.GetString();
			itInfo = createInfo.m_layerInfo.end()-1;
		}
		// 使用するドメインの調査のため、元テーブルのフィールドを読込み
		IFieldsPtr srcFields;
		layer->get_Fields(&srcFields);
		long lCount = 0;
		srcFields->get_FieldCount(&lCount);
		for(long index = 0; index<lCount; ++index)
		{
			IFieldPtr field;
			srcFields->get_Field(index, &field);

			IDomainPtr domain;
			field->get_Domain(&domain);
			if(!domain) continue;

			CComBSTR domainName;
			domain->get_Name(&domainName);
			createInfo.m_domainInfo.emplace(domainName.m_str);
		}
		// エイリアスが設定ファイルで設定されている場合、取得しない
		if(!itInfo->m_aliasName)
		{
			CComBSTR fcAliasName;
			IObjectClassPtr(layer)->get_AliasName(&fcAliasName);
			itInfo->m_aliasName = fcAliasName.m_str;
		}

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
			createInfo.m_datasetInfo.emplace(name.m_str);
		}
	}
	return true;
}


/**
 * @brief inputの指定データ一覧を読取り、レイヤ・ドメイン・フィーチャデータセット生成に
 * 必要な情報を取得
 * @param inputDB        [in]  入力DBのワークスペース
 * @param inputStr       [in]  入力DBの接続文字列
 * @param type           [in]  データタイプ
 * @param createDefList  [out] レイヤ・ドメイン・フィーチャデータセットの情報の追加先
 */
bool cnvTableToCreateList(
	IWorkspacePtr& inputDB, const uh::tstring& inputStr, esriDatasetType type, create_info::CreateInfo& createInfoList)
{
	IEnumDatasetPtr ipEnumDataset = util::getDatasets(inputDB, type);
	std::vector<CString> layerList;
	if( esriDTFeatureDataset==type )
		layerList = getLayerNameListInDataset(ipEnumDataset, AheGetConnectUserName(inputDB));
	else
		layerList = getLayerNameList(ipEnumDataset, AheGetConnectUserName(inputDB));
	return makeCreateInfo( inputDB, inputStr, layerList, createInfoList, false );
}

} // namespace

bool CConfigLoader::loadConfig(const uh::tstring& filepath, const uh::tstring& inputStr)
{
	IWorkspacePtr inputDB = sindy::create_workspace( inputStr.c_str() );
	if( !inputDB )
	{
		RUNLOGGER.fatal(IDS_CONNECT_INPUTDB, inputStr, _T(""), _T("") );
		return false;
	}

	create_info::CreateInfo createInfo;
	if(filepath.empty())
	{
		if( !cnvTableToCreateList( inputDB, inputStr, esriDTFeatureDataset, createInfo ) )
			return false;
		if( !cnvTableToCreateList( inputDB, inputStr, esriDTFeatureClass, createInfo ) )
			return false;
		if( !cnvTableToCreateList( inputDB, inputStr, esriDTTable, createInfo ) )
			return false;
	}
	else
	{
		if( !parser::xmlToCreateDef( filepath, createInfo ) )
			return false;

		std::vector<CString> layerList;
		std::transform(createInfo.m_layerInfo.cbegin(), createInfo.m_layerInfo.cend(), std::back_inserter(layerList),
			[&](const create_info::CreateLayerInfo& cDef )
			{
				return cDef.m_name.c_str();
			});
		if( !makeCreateInfo( inputDB, inputStr, layerList, createInfo, false ) )
			return false;
	}
	m_createInfoMap.emplace_back(inputStr, createInfo);
	return true;
}

bool CConfigLoader::loadMesh(const uh::tstring& meshDBStr, const uh::tstring& layerName)
{
	if( meshDBStr.empty() ) return true;

	IWorkspacePtr meshWorks = sindy::create_workspace( meshDBStr.c_str() );
	if(!meshWorks)
	{
		RUNLOGGER.fatal(IDS_CONNECT_INPUTDB, meshDBStr, _T(""), _T(""));
		return false;
	}

	// TODO: メッシュDB名ではなく接続情報を保持してと同一のDBか判定する
	// (現状でも途中で異常終了するので動くことは動く)
	create_info::CreateInfo* createInfo = nullptr;
	auto itDefined = std::find_if(m_createInfoMap.begin(), m_createInfoMap.end(),
		[&](const std::pair<uh::tstring, create_info::CreateInfo>& elem)
		{
			return util::eq(elem.first, meshDBStr);
		});
	if( m_createInfoMap.cend() == itDefined )
	{
		m_createInfoMap.emplace_back(meshDBStr, create_info::CreateInfo());
		createInfo = &m_createInfoMap.back().second;
	}
	else
	{
		createInfo = &itDefined->second;
	}

	std::vector<CString> layerNameList;
	layerNameList.push_back(layerName.c_str());
	if( !makeCreateInfo( meshWorks, meshDBStr, layerNameList, *createInfo, true ) )
		return false;
	
	return true;
}
