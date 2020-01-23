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
#include "OutputMeshRecord.h"

#include <algorithm>
#include <sindy/workspace.h>
#include <boost/filesystem.hpp>
// TODO: enumerate_elementsを使用できるように改修するときにコメントアウトを外す
//#include <TDC/useful_headers/enumerate_elements.h>

using namespace sindy::schema;

namespace
{
/**
 * @brief メッシュレイヤ内のメッシュコード一覧を取得
 * @param meshLayer [in]  検索対象のレイヤ
 * @param result    [out] メッシュコード一覧の格納先
 * @retval true  処理に成功
 * @retval false 検索カーソルの生成に失敗
 */
bool makeAllMeshcodeList(const IFeatureClassPtr& meshLayer, std::vector<unsigned long>& result)
{
	if( !meshLayer) return false;

	result.clear();
	IFeatureCursorPtr cursor;
	if(FAILED(meshLayer->Search(AheInitQueryFilter(nullptr, meshcode_class::kMeshCode), VARIANT_TRUE, &cursor))
		|| !cursor)
	{
		return false;
	}
	long meshcodeIndex = -1;
	cursor->FindField(CComBSTR(meshcode_class::kMeshCode), &meshcodeIndex);
	IFeaturePtr feature;
	while(S_OK==cursor->NextFeature(&feature) && feature)
	{
		CComVariant value;
		feature->get_Value(meshcodeIndex, &value);
		result.push_back( value.ulVal );
	}
	return true;
}

}

bool COutputMeshRecord::output()
{
	if(m_meshDB.empty()) return true;

	IWorkspacePtr works = sindy::create_workspace(m_meshDB.c_str());
	if(!works)
	{
		RUNLOGGER.fatal(IDS_CONNECT_INPUTDB, m_meshDB, _T(""), _T(""));
		return false;
	}
	ITablePtr layer =  AheOpenTableByTableName( works, m_meshLayerName.c_str() );
	if( !layer )
	{
		RUNLOGGER.fatal(IDS_FAILED_OPEN_TABLE, m_meshDB, m_meshLayerName, _T(""));
		return false;
	}

	long inputShapeIndex = -1L;
	auto inputLayerFieldMap = util::createFieldMap(layer, inputShapeIndex);
	switch(m_outputType)
	{
	case SDE:
	case LOCAL:
		return outputAllTargetMesh(*m_targetPathList.begin(), layer, inputLayerFieldMap, inputShapeIndex);
		break;
	case FOLDER:
		{
			bool bSuccess = true;
			for(const auto& targetDB : m_targetPathList)
				bSuccess &= outputAllTargetMesh(targetDB, layer, inputLayerFieldMap, inputShapeIndex);
			return bSuccess;
		}
		break;
	default:
		assert(false);
		return false;
		break;
	}
}

bool COutputMeshRecord::outputAllTargetMesh(
	const uh::tstring& path, const IFeatureClassPtr& inputLayer
	, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex)
{
	IWorkspacePtr outDB = sindy::create_workspace(path.c_str());
	if(!outDB)
	{
		RUNLOGGER.error(IDS_CONNECT_OUTPUTDB, path, _T(""), _T(""));
		return false;
	}
	IFeatureClassPtr outLayer = AheOpenTableByTableName( outDB, m_meshLayerName.c_str() );
	if( !outLayer )
	{
		RUNLOGGER.error(IDS_FAILED_OPEN_TABLE, path, m_meshLayerName, _T(""));
		return false;
	}
	long outputShapeIndex = -1L;
	auto outputFM = util::createFieldMap(outLayer, outputShapeIndex);

	// 出力先のメッシュコード一覧を作成
	std::vector<unsigned long> outMeshcodeList;
	if( !makeAllMeshcodeList(outLayer, outMeshcodeList) )
	{
		RUNLOGGER.error(IDS_FAILED_SEARCH_OUTDB_MESH, path, m_meshLayerName, _T(""));
		return false;
	}

	if(!databaseUtility::startEdit(outDB))
		return false;

	bool bHasError = false; //!< 致命的ではなく、一部保存はできるがエラーが発生した
	bool bHasFatal = false; //!< 致命的エラーが発生して保存できない
	try
	{
		// メッシュリストから出力先に存在するメッシュコードを除いたリストを取得
		auto targetMeshList = makeTargetMeshList(path, outMeshcodeList);
		// 上記のメッシュリストからinputLayer向けのメッシュ検索条件群を生成
		auto whereList = makeWhereList(targetMeshList);

		// メッシュを検索・出力し、出力DBに作成されたメッシュコード一覧を取得
		std::vector<CString> createdMeshList;
		bHasFatal = !outputRecord(outLayer, path, whereList
			, inputLayer, inputFM, inputShapeIndex
			, outputFM, outputShapeIndex
			, createdMeshList);

		if(!bHasFatal)
		{
			// 検索条件として指定されていたが、入力DBからメッシュを検索できなかったものを出力
			std::vector<CString> diffList;
			std::set_difference(targetMeshList.begin(), targetMeshList.end(),
				createdMeshList.begin(), createdMeshList.end(),
				std::back_inserter(diffList));
			if(!diffList.empty())
			{
				tstringstream ss;
				ss << AheLoadString(IDS_FAILED_FIND_MESH).GetString() << _T(" : ")
					<< path
					<< _T(" LAYER[") << m_meshLayerName << _T("]")
					<< _T(" MESH[") << uh::str_util::join(diffList,_T(",")).GetString() << _T("]");
				RUNLOGGER.error(ss.str().c_str());
				bHasError = true;
			}
		}
	}
	catch(const _com_error& e)
	{
		RUNLOGGER.error(atl2::stringize(e).c_str());
		bHasFatal = true;
	}

	if(bHasFatal)
	{
		databaseUtility::abortEdit(outDB);
		databaseUtility::stopEdit(outDB, false);
		return false;
	}

	if(!databaseUtility::stopEdit(outDB, true))
		return false;
	return !bHasError;
}

bool COutputMeshRecord::outputRecord(
	IFeatureClassPtr& outputLayer
	, const uh::tstring& outputStr
	, const std::vector<CString>& strWhereList
	, const IFeatureClassPtr& inputLayer
	, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex
	, const std::map<uh::tstring,long>& outputFM, long outputShapeIndex
	, std::vector<CString>& createdMeshList)
{
	if( strWhereList.empty() ) return true;

	// コピー対象として除外するフィールド群
	static const std::vector<uh::tstring> exclusionFields = boost::assign::list_of
		(sindy::schema::ipc_feature::kObjectID)
		(_T("SHAPE_LENGTH"))
		(_T("SHAPE_AREA"))
		(_T("SHAPE.LEN"))
		(_T("SHAPE.AREA"));

	IFeatureCursorPtr inserter;
	if( FAILED(outputLayer->Insert(VARIANT_TRUE, &inserter))
		|| !inserter)
	{
		RUNLOGGER.error(IDS_FAILED_CREATE_CURSOR, outputStr, m_meshLayerName, _T(""));
		return false;
	}
	IFeatureBufferPtr featureBuffer;
	if( FAILED(outputLayer->CreateFeatureBuffer(&featureBuffer))
		|| !featureBuffer)
	{
		RUNLOGGER.error(IDS_FAILED_CREATE_FCBUFFER, outputStr, m_meshLayerName, _T(""));
		return false;
	}

	std::vector<CString> result;
	// 出力に成功、あるいは、すでに出力先に存在したメッシュコード一覧
	for(const auto& strWhere : strWhereList)
	{
		IFeatureCursorPtr cursor;
		if( FAILED(inputLayer->Search(AheInitQueryFilter(nullptr, nullptr, strWhere), VARIANT_TRUE, &cursor))
			|| !cursor)
		{
			tstringstream ss;
			ss << _T("SEARCH[")	<< strWhere.GetString() << _T("]");
			RUNLOGGER.error(IDS_FAILED_SEARCH_MESH, outputStr, m_meshLayerName, _T(""), ss.str());
			return false;
		}

		IFeaturePtr feature;
		while(S_OK==cursor->NextFeature(&feature) && feature)
		{
			for(const auto& elem : inputFM)
			{
				// コピー対象外のフィールドは飛ばす
				// フィールドマップのフィールド名は大文字に変換されているため、これで検索できる
				auto it = std::find(exclusionFields.cbegin(), exclusionFields.cend(), elem.first);
				if( exclusionFields.cend() != it )
					continue;

				if(inputShapeIndex==elem.second)
				{
					IGeometryPtr shape;
					feature->get_ShapeCopy(&shape);
					featureBuffer->putref_Shape(shape);
					continue;
				}
				CComVariant value;
				feature->get_Value(elem.second, &value);
				
				auto outputIt = outputFM.find(elem.first);
				if( outputFM.end()==outputIt )
				{
					RUNLOGGER.error(IDS_FAILED_OUTPUT_INDEX, outputStr, m_meshLayerName, elem.first);
					return false;
				}
				featureBuffer->put_Value(outputIt->second , value);
			}

			CComVariant id;
			HRESULT re = inserter->InsertFeature(featureBuffer, &id);
			if(S_OK!=re)
			{
				tstringstream ss;
				ss << _T(" ERROR[") << re << _T("]");
				RUNLOGGER.error(IDS_FAILED_CREATE_RECORD, outputStr, m_meshLayerName, _T(""), ss.str());
				return false;
			}
			CComVariant meshcode;
			featureBuffer->get_Value(outputFM.at(meshcode_class::kMeshCode), &meshcode);
			createdMeshList.push_back( uh::str_util::ToString(meshcode, _T("%.0f")) );
		}
	}
	if(FAILED(inserter->Flush()))
	{
		RUNLOGGER.error(IDS_FAILED_CURSOR_FLUSH, outputStr, m_meshLayerName, _T(""));
		return false;
	}
	std::sort(createdMeshList.begin(), createdMeshList.end());
	return true;
}

std::vector<CString> COutputMeshRecord::makeTargetMeshList(
	const uh::tstring& path
	, const std::vector<unsigned long>& outputMeshList)
{
	if(FOLDER==m_outputType)
	{
		boost::filesystem::path bPath(path);
		if( !util::isNumber(bPath.stem().c_str()) )
		{
			assert(false);
			RUNLOGGER.error(IDS_DONT_ASSUME_FILE, path, _T(""), _T(""));
			return std::vector<CString>();
		}
		unsigned long meshcode = std::stoul(bPath.stem().c_str());
		auto it = std::find(outputMeshList.cbegin(), outputMeshList.cend(), meshcode);
		if( outputMeshList.cend() != it )
		{
			tstringstream ss;
			ss << _T("MESH[") << meshcode << _T("]");
			RUNLOGGER.error(IDS_WARN_EXIST_MESH, path, _T(""), _T(""), ss.str());
			return std::vector<CString>();
		}
		std::vector<CString> result;
		result.push_back(bPath.stem().c_str());
		return result;
	}

	if(LOCAL==m_outputType)
	{
		// uh::tstringをCStringに変換
		std::vector<CString> result;
		std::transform( m_meshlist.cbegin(), m_meshlist.cend(), std::back_inserter(result),
			[&](const uh::tstring& meshcode) { return meshcode.c_str(); } );

		// 出力先に存在するものは除外
		auto itNew = std::remove_if(result.begin(), result.end(),
			[&](const CString& meshcodeStr)
			{
				unsigned long meshcode = std::stoul(meshcodeStr.GetString());
				auto it = std::find(outputMeshList.cbegin(), outputMeshList.cend(), meshcode);
				if( outputMeshList.cend() != it )
				{
					tstringstream ss;
					ss << _T("MESH[") << meshcode << _T("]");
					RUNLOGGER.error(IDS_WARN_EXIST_MESH, path, _T(""), _T(""), ss.str());
					return true;
				}
				return false;
			});
		result.erase(itNew, result.end() );
		result.shrink_to_fit();
		return result;
	}
	// ここにくることはないはず
	assert(false);
	return std::vector<CString>();

}

std::vector<CString> COutputMeshRecord::makeWhereList(const std::vector<CString>& targetMeshList)
{
	// TODO: enumerate_elementsを使用できるように改修
	const unsigned int SQL_INLimit = 1000; //!< SQL文のIN()の要素の限界数
	// SQL_INLimit ごとに分割
	std::vector<std::vector<CString>> meshParLimitList;
	auto processStart = targetMeshList.cbegin();
	while( processStart!=targetMeshList.cend() )
	{
		auto processEnd = targetMeshList.cend();
		if( std::distance(processStart, targetMeshList.cend()) > SQL_INLimit )
		{
			processEnd = processStart+SQL_INLimit;
			std::advance(processStart, SQL_INLimit );
		}
		std::vector<CString> meshParLimit;
		std::copy(processStart, processEnd, std::back_inserter(meshParLimit));
		meshParLimitList.push_back( meshParLimit );
		processStart = processEnd;
	}
	std::vector<CString> strWhereList;
	for(const auto& meshParLimit : meshParLimitList)
	{
		CString strWhere;
		strWhere.Format(_T("%s IN (%s)"),
			sindy::schema::category::mesh_code_table::kMeshCode
			, uh::str_util::join( meshParLimit, _T(",")));
		strWhereList.push_back( strWhere );
	}
	return strWhereList;
}
