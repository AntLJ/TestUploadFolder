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
 * @brief IEnumDatasetPtr�ɏ������郌�C�����ꗗ���擾
 * @param dateSet [in] ���C����ێ�����f�[�^�Z�b�g
 * @param ownerName [in] �I�[�i�[��
 * @return ���O�ꗗ
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
 * @brief �f�[�^�Z�b�g�ɏ������郌�C�����ꗗ���擾
 * @param dateSet   [in] ���C����ێ�����f�[�^�Z�b�g
 * @param ownerName [in] �I�[�i�[��
 * @return ���O�ꗗ
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
 * @brief ���C���ꗗ�̃t�B�[���h��Ǎ���
 * @param inputDB    [in]  ����DB�̃��[�N�X�y�[�X
 * @param inputStr   [in]  ����DB�̐ڑ�������
 * @param layerList  [in]  ���C���ꗗ
 * @param createInfo [out] ���C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�̏��̒ǉ���
 * @param checkFlag  [in]  ��`�ς݃��C���ɂ͏������s��Ȃ��悤�Ƀ`�F�b�N���邩
 * @return �����̐���
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

		// ��`�ς݂̏ꍇ(�ݒ�t�@�C���Ŏw�肳�ꂽ���C���ł���ꍇ)��
		// ��`���쐬�����Ƀh���C���E�f�[�^�Z�b�g�擾�̂�
		if( createInfo.m_layerInfo.end()!=itInfo )
		{
			if(checkFlag)
			{
				// �ݒ�t�@�C����mesh_layer_name�Ŏw�肵�����C���𐶐��ΏۂɎw�肵����ԂŁA
				// ���b�V�������@�\�����s���Ă���Ƃ����ɂ���
				// ���̏ꍇ�A�ݒ�t�@�C���ł̐ݒ��D�悷��̂ŁA�x���������o���Ď��ցB
				RUNLOGGER.warn(IDS_WARN_EXIST_TABLE_DEF, inputStr, layerName.GetString(), _T(""));
				continue;
			}
		}
		else
		{
			// ����`�̏ꍇ�͒�`���쐬
			createInfo.m_layerInfo.emplace_back();
			createInfo.m_layerInfo.back().m_name = layerName.GetString();
			itInfo = createInfo.m_layerInfo.end()-1;
		}
		// �g�p����h���C���̒����̂��߁A���e�[�u���̃t�B�[���h��Ǎ���
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
		// �G�C���A�X���ݒ�t�@�C���Őݒ肳��Ă���ꍇ�A�擾���Ȃ�
		if(!itInfo->m_aliasName)
		{
			CComBSTR fcAliasName;
			IObjectClassPtr(layer)->get_AliasName(&fcAliasName);
			itInfo->m_aliasName = fcAliasName.m_str;
		}

		// �t�B�[�`���N���X�̏ꍇ�A��������f�[�^�Z�b�g���R�s�[�Ώ�
		IFeatureClassPtr fc(layer);
		if(!fc) continue;
		// ��ł܂Ƃ߂ăR�s�[���쐬����̂ŁA�����ł͖��̂��擾����̂�
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
 * @brief input�̎w��f�[�^�ꗗ��ǎ��A���C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g������
 * �K�v�ȏ����擾
 * @param inputDB        [in]  ����DB�̃��[�N�X�y�[�X
 * @param inputStr       [in]  ����DB�̐ڑ�������
 * @param type           [in]  �f�[�^�^�C�v
 * @param createDefList  [out] ���C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�̏��̒ǉ���
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

	// TODO: ���b�V��DB���ł͂Ȃ��ڑ�����ێ����ĂƓ����DB�����肷��
	// (����ł��r���ňُ�I������̂œ������Ƃ͓���)
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
