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
 * @brief ����DB����w��̃h���C�����R�s�[���A���X�g�ɒǉ�����
 * @param input          [in]  ����DB�̃��[�N�X�y�[�X
 * @param domainInfoList [in]  �R�s�[����h���C�����ꗗ
 * @param result         [out] ���ʂ̒ǉ���
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
 * @brief ����DB����w��̃f�[�^�Z�b�g���R�s�[���A���X�g�ɒǉ�����
 * @param input           [in]  ����DB�̃��[�N�X�y�[�X
 * @param datasetInfoList [in]  �R�s�[����f�[�^�Z�b�g���ꗗ
 * @param result          [out] ���ʂ̒ǉ���
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
		// �f�[�^�Z�b�g��Clone�����Ȃ��悤�Ȃ̂ŋ�ԎQ�Ƃ����R�s�[
		ISpatialReferencePtr spRef;
		IGeoDatasetPtr(dataset)->get_SpatialReference(&spRef);

		product::ProductDatasetDef def;
		def.m_name = util::eraseOwnerName(datasetName.m_str);
		def.m_spRef = AheGetClone(spRef);
		result.push_back( def );
	}
}

/**
 * @brief �����C���̃t�B�[���h���R�s�[����
 * @warning �`��t�B�[���h�����͒ǉ��t�B�[���h�ǉ���ɒǉ�
 * @param result     [out] �t�B�[���h�R�s�[�������ʂ̊i�[��
 * @param shapeField [out] �`��t�B�[���h�̊i�[��
 * @param layer      [in]  �t�B�[���h�R�s�[�����C��
 * @return �R�s�[�����t�B�[���h�ꗗ
 */
std::vector<uh::tstring> copyField(IFieldsEditPtr& result, IFieldPtr& shapeField, const ITablePtr& layer)
{
	std::vector<uh::tstring> srcFieldNameList;

	// ���e�[�u���̃t�B�[���h��Ǎ���
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
 * @brief �t�B�[���h��ǉ�����
 * @warning �`��t�B�[���h�����͒ǉ��t�B�[���h�ǉ���ɒǉ�
 * @param result     [out] �t�B�[���h�̒ǉ���
 * @param addedField [in]  �ǉ�����t�B�[���h
 */
void addField(IFieldsEditPtr& result, const IFieldsEditPtr& addedField)
{
	if(!addedField) return;
	// ���e�[�u���̃t�B�[���h��Ǎ���
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
 * @brief �ǉ��t�B�[���h�������C���ɂ���Ȃ�A���������
 * @param fieldList   [in] �����C���̃t�B�[���h�ꗗ
 * @param addedField  [in] �ǉ��t�B�[���h
 * @param inputStr    [in] ����DB�ڑ�������
 * @param layerName   [in] �R�s�[�Ώۃ��C����
 * @return �����C���̃t�B�[���h���������ǉ��t�B�[���h
 * @retval nullptr addedField��null
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
 * @brief ���C���ꗗ�̏������Ƀt�B�[���h���쐬
 * @param inputDB       [in]  ����DB�̃��[�N�X�y�[�X
 * @param inputStr      [in]  ����DB�ڑ�������
 * @param layerInfoList [in]  �R�s�[���郌�C�����ꗗ
 * @param createDefList [out] �����t�B�[���h���X�g�̒ǉ���
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
			// �`��t�B�[���h�͍Ō�ɒǉ����邽�߁A�ޔ��p��
			IFieldPtr shapeFieldTmp;
			// �t�B�[���h���R�s�[�������C���̃t�B�[���h�ꗗ�擾
			auto srcFieldNameList = copyField( productLayerDef.m_fields, shapeFieldTmp, layer );
			// �ǉ��t�B�[���h��ǉ�
			IFieldsEditPtr fields = eraseDupliField(srcFieldNameList, layerInfo.m_addFields, inputStr, layerInfo.m_name);
			if(!fields)
				fields = IFieldsEditPtr(CLSID_Fields);
			if(shapeFieldTmp)
			{
				// �`��t�B�[���h�͍Ō�ɒǉ�(iPC�̃e���v���[�g���C���̌`���ɍ��킹�Ă���)
				fields->AddField(shapeFieldTmp);
			}
			addField(productLayerDef.m_fields, fields);
		}

		IUIDPtr clsid;
		layer->get_CLSID(&clsid);
		// CLSID�̎擾
		productLayerDef.m_srcCLSID = util::copyUID(clsid);
		// EXTCLSID�̎擾
		IUIDPtr extClsid;
		layer->get_EXTCLSID(&extClsid);
		productLayerDef.m_srcEXTCLSID= util::copyUID(extClsid);

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
			// �������ɂ̓��[�U�[�����s�v�Ȃ̂ō폜
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
