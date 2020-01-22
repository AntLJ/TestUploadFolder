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
#include "AddIndex.h"
#include <sindy/workspace.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/useful_headers/scope_guard.h>

namespace
{

// �c�[���Ő��������C���f�b�N�X�̖����ɕt�^���镶����
const uh::tstring ADDINDEX_SUFFIX = _T("_INDEX");

/**
 * @brief �Ώۃ��C���̃X�L�[�}�ɔr���I���b�N���������Ă��Ȃ������肷��
 * @param layer [in] �`�F�b�N���郌�C��
 * @retval true  ���b�N����Ă��Ȃ�
 * @retval false ���b�N����Ă���
 */
bool checkLock(ITablePtr layer)
{
	IEnumSchemaLockInfoPtr lockList;
	ISchemaLockPtr(layer)->GetCurrentSchemaLocks(&lockList);

	ISchemaLockInfoPtr lockInfo;
	lockList->Reset();
	while( S_OK==lockList->Next(&lockInfo) && lockInfo )
	{
		esriSchemaLock lockType = esriSharedSchemaLock;
		lockInfo->get_SchemaLockType(&lockType);
		if( esriExclusiveSchemaLock==lockType )
			return false;
	}
	return true;
}

/**
 * @brief �C���f�b�N�X���t�^�\�����肷��
 * @param fieldName   [in] �Ώۂ̃t�B�[���h��
 * @param existingMap [in] �o�͐�ɑ��݂���C���f�b�N�X����
 * ����Ɋւ��t�B�[���h�����X�g�̑Ή��\
 * @retval true  �t�^�\
 * @retval false �t�^�s��
 */
bool checkCanAddIndex(const uh::tstring& fieldName
	, const std::map<uh::tstring, std::vector<uh::tstring> >& existingMap)
{
	// �C���f�b�N�X�����łɕt�^����Ă���t�B�[���h�Ȃ�A
	// ���̃c�[���ł́A�t�^�͋֎~
	for(const auto& elem : existingMap)
	{
		const auto& fieldNameList = elem.second;
		auto itFieldName = std::find(fieldNameList.cbegin(), fieldNameList.cend(), fieldName);
		if(fieldNameList.cend()!=itFieldName)
			return false;
	}
	return true;
}

/**
 * @brief �폜�\�ȃC���f�b�N�X�̖��̃��X�g��Ԃ�
 * @param fieldName   [in] �Ώۂ̃t�B�[���h��
 * @param existingMap [in] �o�͐�ɑ��݂���C���f�b�N�X����
 * ����Ɋւ��t�B�[���h�����X�g�̑Ή��\
 * @return �C���f�b�N�X�폜�\������
 */
bool checkCanDeleteIndex(
	const uh::tstring& fieldName
	, const std::map<uh::tstring, std::vector<uh::tstring> >& existingMap)
{
	bool existTarget = false;
	for(const auto& elem : existingMap)
	{
		const auto& fieldNameList = elem.second;

		// �����t�B�[���h���ւ��C���f�b�N�X�͂��̃c�[���ł͑���ł��Ȃ����ƂƂ���
		// (SDE���ƕ����t�B�[���h�ւ����̂������)
		if(1 != fieldNameList.size())
			return false;

		auto itFieldName = std::find(fieldNameList.cbegin(), fieldNameList.cend(), fieldName);
		if(fieldNameList.cend()!=itFieldName)
			existTarget = true;
	}
	return existTarget;
}

} // namespace

bool CAddIndex::execute()
{
	IWorkspacePtr targetWorks = sindy::create_workspace(m_db.c_str());
	if(!targetWorks)
	{
		RUNLOGGER.fatal(IDS_FAILED_CONNECT_INPUT_DB, m_db, _T(""), _T(""));
		return false;
	}

	for(const auto& layerInfo : m_indexInfoMap)
	{
		ITablePtr layer = AheOpenTableByTableName(targetWorks, layerInfo.first.c_str());
		if(!layer)
		{
			RUNLOGGER.error(IDS_FAILED_OPEN_LAYER, m_db, layerInfo.first, _T(""));
			continue;
		}
		// �r���I���b�N���������Ă���ꍇ�͏������Ȃ�
		if( !checkLock(layer) )
		{
			RUNLOGGER.error(IDS_LAYER_IS_LOCKED, m_db, layerInfo.first, _T(""));
			continue;
		}
		if( !lockSchema(layer, layerInfo.first) )
			continue;

		// �ȍ~�A���������f����邩�A����������X�L�[�}�̃��b�N����������
		uh::scope_guard scope( std::bind(&CAddIndex::releaseSchema, this, layer, layerInfo.first ) );
		// �C���f�b�N�X�t�^����
		addIndex(layer, layerInfo.first, layerInfo.second);
	}
	return true;
}

bool CAddIndex::lockSchema(ITablePtr& layer, const uh::tstring& layerName)
{
	HRESULT reLock = ISchemaLockPtr(layer)->ChangeSchemaLock(esriExclusiveSchemaLock);
	if(FAILED(reLock))
	{
		tstringstream ss;
		ss << _T("ERROR[") << reLock << _T("]");
		RUNLOGGER.error(IDS_FAILED_LOCK, m_db, layerName, _T(""), ss.str());
		return false;
	}
	return true;
}

void CAddIndex::releaseSchema(ITablePtr& layer, const uh::tstring& layerName)
{
	HRESULT reLock = ISchemaLockPtr(layer)->ChangeSchemaLock(esriSharedSchemaLock);
	if(FAILED(reLock))
	{
		tstringstream ss;
		ss << _T("ERROR[") << reLock << _T("]");
		RUNLOGGER.fatal(IDS_FAILED_LOCK_RELEASE, m_db, layerName, _T(""), ss.str());
	}
}

void CAddIndex::addIndex(
	ITablePtr& layer
	, const uh::tstring& layerName
	, const std::vector<index_def::IndexInfo>& indexInfoList)
{
	// �o�͐�̃C���f�b�N�X�t�^�ς݃t�B�[���h�����擾
	auto existingIndexMap = loadIndexes(layer);

	// �t�B�[���h�}�b�v<�t�B�[���h��, �C���f�b�N�X>
	std::map<uh::tstring, long> fieldMap;
	for(const auto& indexInfo : indexInfoList)
	{
		auto result = fieldMap.emplace(indexInfo.m_name, 0);
		// ���o��̃t�B�[���h�̓C���f�b�N�X���m�ۂ��Ă���
		if( result.second )
		{
			result.first->second = -1;
			layer->FindField(CComBSTR(indexInfo.m_name.c_str()), &result.first->second);
		}
		// �t�B�[���h�C���f�b�N�X���擾�ł��Ȃ� = ���C���ɂȂ��t�B�[���h�Ȃ̂Ŕ�΂�
		if( result.first->second < 0 )
		{
			RUNLOGGER.error(IDS_NONE_EXIST_COLUMN, m_db, layerName, indexInfo.m_name);
			continue;
		}


		switch(indexInfo.m_type)
		{
		case index_def::ADD:
			if(!checkCanAddIndex(indexInfo.m_name, existingIndexMap))
			{
				RUNLOGGER.error(IDS_CANT_ADD_INDEX, m_db, layerName, indexInfo.m_name);
				continue;
			}
			{
				// �ւ��t�B�[���h�擾
				IFieldPtr field = util::getField( indexInfo.m_name, layer);

				// �C���f�b�N�X���� <�t�B�[���h��>_INDEX �Ƃ���
				const uh::tstring addIndexName = indexInfo.m_name + ADDINDEX_SUFFIX;

				// �C���f�b�N�X���쐬
				IIndexEditPtr indexEdit(CLSID_Index);
				indexEdit->put_Name( CComBSTR( addIndexName.c_str() ) );
				indexEdit->put_IsAscending(indexInfo.m_order_asc? VARIANT_TRUE:VARIANT_FALSE);
				indexEdit->put_IsUnique(indexInfo.m_unique? VARIANT_TRUE:VARIANT_FALSE);

				// �C���f�b�N�X��t�^����t�B�[���h��ݒ�
				IFieldsEditPtr putFields(CLSID_Fields);
				putFields->AddField(field);
				indexEdit->putref_Fields(putFields);

				// ���C���ɃC���f�b�N�X���쐬
				IIndexesPtr indexes = util::getIndexes(layer);
				if(FAILED(layer->AddIndex(indexEdit)))
				{
					RUNLOGGER.error(IDS_CANT_ADD_INDEX_TO_DB, m_db, layerName, indexInfo.m_name);
					continue;
				}
				// ��������̃C���f�b�N�X�t�^�ς݃t�B�[���h���X�V
				std::vector<uh::tstring> fieldNameList;
				fieldNameList.push_back(indexInfo.m_name);
				existingIndexMap.emplace(addIndexName.c_str(), fieldNameList);
			}
			break;
		case index_def::DEL:
			if(!checkCanDeleteIndex(indexInfo.m_name, existingIndexMap))
			{
				RUNLOGGER.error(IDS_CANT_DEL_INDEX, m_db, layerName, indexInfo.m_name);
				continue;
			}
			{
				IIndexesEditPtr editIndexes(util::getIndexes(layer));

				IEnumIndexPtr enumIndex;
				editIndexes->FindIndexesByFieldName(CComBSTR(indexInfo.m_name.c_str()), &enumIndex);
				enumIndex->Reset();
				IIndexPtr index;
				while(S_OK==enumIndex->Next(&index) && index)
				{
					if(FAILED(layer->DeleteIndex(index)))
					{
						RUNLOGGER.error(IDS_CANT_DEL_INDEX_TO_DB, m_db, layerName, indexInfo.m_name);
						continue;
					}
					CComBSTR name;
					index->get_Name(&name);
					// ��������̃C���f�b�N�X�t�^�ς݃t�B�[���h���X�V
					existingIndexMap.erase(existingIndexMap.find(name.m_str));
				}
			}
			break;
		default:
			assert(false);
			break;
		}
	}
}


std::map<uh::tstring, std::vector<uh::tstring> > CAddIndex::loadIndexes(const ITablePtr& layer)
{
	std::map<uh::tstring, std::vector<uh::tstring> > result;

	IIndexesPtr indexes;
	layer->get_Indexes(&indexes);
	long indexCount = 0;
	indexes->get_IndexCount(&indexCount);
	for(long index=0; index<indexCount; ++index)
	{
		IIndexPtr iIndexElem;
		indexes->get_Index(index, &iIndexElem);
		IFieldsPtr fields;
		iIndexElem->get_Fields(&fields);
		long fieldCount = 0;
		fields->get_FieldCount(&fieldCount);

		std::vector<uh::tstring> fieldNameList;
		for(long fieldIndex=0; fieldIndex<fieldCount; ++fieldIndex)
		{
			IFieldPtr field;
			fields->get_Field(fieldIndex, &field);
			CComBSTR fieldName;
			field->get_Name(&fieldName);
			fieldNameList.push_back(fieldName.m_str);
		}

		CComBSTR indexName;
		iIndexElem->get_Name(&indexName);
		result.emplace(indexName.m_str, fieldNameList); 
	}
	return result;
}
