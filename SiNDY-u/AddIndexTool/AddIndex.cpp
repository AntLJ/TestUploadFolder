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

// ツールで生成したインデックスの末尾に付与する文字列
const uh::tstring ADDINDEX_SUFFIX = _T("_INDEX");

/**
 * @brief 対象レイヤのスキーマに排他的ロックがかかっていないか判定する
 * @param layer [in] チェックするレイヤ
 * @retval true  ロックされていない
 * @retval false ロックされている
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
 * @brief インデックスが付与可能か判定する
 * @param fieldName   [in] 対象のフィールド名
 * @param existingMap [in] 出力先に存在するインデックス名と
 * それに関わるフィールド名リストの対応表
 * @retval true  付与可能
 * @retval false 付与不可
 */
bool checkCanAddIndex(const uh::tstring& fieldName
	, const std::map<uh::tstring, std::vector<uh::tstring> >& existingMap)
{
	// インデックスがすでに付与されているフィールドなら、
	// このツールでは、付与は禁止
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
 * @brief 削除可能なインデックスの名称リストを返す
 * @param fieldName   [in] 対象のフィールド名
 * @param existingMap [in] 出力先に存在するインデックス名と
 * それに関わるフィールド名リストの対応表
 * @return インデックス削除可能か判定
 */
bool checkCanDeleteIndex(
	const uh::tstring& fieldName
	, const std::map<uh::tstring, std::vector<uh::tstring> >& existingMap)
{
	bool existTarget = false;
	for(const auto& elem : existingMap)
	{
		const auto& fieldNameList = elem.second;

		// 複数フィールドが関わるインデックスはこのツールでは操作できないこととする
		// (SDEだと複数フィールド関わるものも張れる)
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
		// 排他的ロックがかかっている場合は処理しない
		if( !checkLock(layer) )
		{
			RUNLOGGER.error(IDS_LAYER_IS_LOCKED, m_db, layerInfo.first, _T(""));
			continue;
		}
		if( !lockSchema(layer, layerInfo.first) )
			continue;

		// 以降、処理が中断されるか、完了したらスキーマのロックを解除する
		uh::scope_guard scope( std::bind(&CAddIndex::releaseSchema, this, layer, layerInfo.first ) );
		// インデックス付与操作
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
	// 出力先のインデックス付与済みフィールド名を取得
	auto existingIndexMap = loadIndexes(layer);

	// フィールドマップ<フィールド名, インデックス>
	std::map<uh::tstring, long> fieldMap;
	for(const auto& indexInfo : indexInfoList)
	{
		auto result = fieldMap.emplace(indexInfo.m_name, 0);
		// 初登場のフィールドはインデックスを確保しておく
		if( result.second )
		{
			result.first->second = -1;
			layer->FindField(CComBSTR(indexInfo.m_name.c_str()), &result.first->second);
		}
		// フィールドインデックスを取得できない = レイヤにないフィールドなので飛ばす
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
				// 関わるフィールド取得
				IFieldPtr field = util::getField( indexInfo.m_name, layer);

				// インデックス名は <フィールド名>_INDEX とする
				const uh::tstring addIndexName = indexInfo.m_name + ADDINDEX_SUFFIX;

				// インデックスを作成
				IIndexEditPtr indexEdit(CLSID_Index);
				indexEdit->put_Name( CComBSTR( addIndexName.c_str() ) );
				indexEdit->put_IsAscending(indexInfo.m_order_asc? VARIANT_TRUE:VARIANT_FALSE);
				indexEdit->put_IsUnique(indexInfo.m_unique? VARIANT_TRUE:VARIANT_FALSE);

				// インデックスを付与するフィールドを設定
				IFieldsEditPtr putFields(CLSID_Fields);
				putFields->AddField(field);
				indexEdit->putref_Fields(putFields);

				// レイヤにインデックスを作成
				IIndexesPtr indexes = util::getIndexes(layer);
				if(FAILED(layer->AddIndex(indexEdit)))
				{
					RUNLOGGER.error(IDS_CANT_ADD_INDEX_TO_DB, m_db, layerName, indexInfo.m_name);
					continue;
				}
				// メモリ上のインデックス付与済みフィールドも更新
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
					// メモリ上のインデックス付与済みフィールドも更新
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
