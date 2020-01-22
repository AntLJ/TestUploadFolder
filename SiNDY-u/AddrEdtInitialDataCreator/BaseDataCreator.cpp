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
#include "BaseDataCreator.h"
#include "Logger.h"
#include "Utility.h"

#include <sindy/workspace.h>
#include <TDC/useful_headers/str_util.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace Utility;

BaseDataCreator::BaseDataCreator(const Arguments& arg) : m_args(arg)
{
	m_commonAttrMap[ipc_table::kOperator] = _T("sindy");
	m_commonAttrMap[ipc_table::kPurpose] = 0;
	m_commonAttrMap[ipc_table::kModifyDate] = _T("");
	m_commonAttrMap[ipc_table::kUpdateType] = ipc_table::update_type::kDefault;
	SYSTEMTIME time; GetLocalTime(&time);
	m_commonAttrMap[ipc_table::kProgModifyDate] = uh::str_util::ToString(&time);
	m_commonAttrMap[ipc_table::kModifyProgName] = _T("AddrEdtInitialDataCreator");
	m_commonAttrMap[ipc_table::kUserClaim] = 0;
	m_commonAttrMap[ipc_table::kSource] = _T("");
}

bool BaseDataCreator::initBase()
{
	// 実行ログオープン
	if( !RUNLOGGER.Initialize( (uh::tstring)m_args.m_runLog) )
	{
		cerr << (CT2A)uh::str_util::format(_T("FATAL\t実行ログが開けません:%s"), m_args.m_runLog) << endl;
		return false;
	}

	// エラーログオープン
	if( !ERRLOGGER.Initialize( (uh::tstring)m_args.m_errLog) )
	{
		cerr << (CT2A)uh::str_util::format(_T("FATAL\tエラーログが開けません:%s"), m_args.m_errLog) << endl;
		return false;
	}

	// ヘッダ書込み
	CVersion ver;
	ERRLOGGER.Log(_T("#%s FILEVERSION %s\n"), ver.GetInternalName(), ver.GetFileVersion());
	ERRLOGGER.Log(_T("#%s\n"), uh::str_util::join(ERRLOG_HEADER_LIST, _T("\t")));

	return true;
}

IFeatureCursorPtr BaseDataCreator::searchByWhereClause(LayerInfoManager& layerManager, const CString& whereClause)
{
	IQueryFilterPtr queryFilter(CLSID_QueryFilter);
	queryFilter->put_WhereClause(CComBSTR(whereClause));

	IFeatureCursorPtr cursor;
	if (FAILED(layerManager.getFeatureClass()->Search(queryFilter, VARIANT_FALSE, &cursor)))
	{
		return nullptr;
	}

	return cursor;
}

bool BaseDataCreator::putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, const CString& value)
{
	CComVariant va;
	if( value.IsEmpty() )
	{
		if( S_OK != va.ChangeType(VT_NULL) )
			return false;
	}
	else
		va = value;

	if( FAILED(featureBuf->put_Value(fieldIdx, va)) )
		return false;
	
	return true;
}

bool BaseDataCreator::getValue(const IFeaturePtr& feature, long fieldIdx, CComVariant& value)
{
	if( FAILED(feature->get_Value(fieldIdx, &value)) )
		return false;
	
	return true;
}

bool BaseDataCreator::getValue(const _IRowPtr& row, long fieldIdx, CComVariant& value)
{
	if( FAILED(row->get_Value(fieldIdx, &value)) )
		return false;
	
	return true;
}

bool BaseDataCreator::putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, int value, bool isNull)
{
	CComVariant va;
	if( isNull )
	{
		if( S_OK != va.ChangeType(VT_NULL) )
			return false;
	}
	else
		va = value;

	if( FAILED(featureBuf->put_Value(fieldIdx, CComVariant(va))) )
		return false;
	
	return true;
}

bool BaseDataCreator::putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, CComVariant& value)
{
	if( FAILED(featureBuf->put_Value(fieldIdx, value)) )
		return false;
	
	return true;
}

bool BaseDataCreator::startEdit()
{
	auto state = StartEditing(VARIANT_FALSE);
	if( S_NON_VERSIONED != state && S_VERSIONED != state )
		return false;

	if (!StartEditOperation())
	{
		return false;
	}
	return true;
}

BaseDataCreator::State BaseDataCreator::StartEditing(bool bRedoUndo)
{
	if(!m_edtWorkspace) return S_ERROR;

	//Checking the session whether it is editing
	VariantBool vb;
	if(FAILED(m_edtWorkspace->IsBeingEdited(&vb)) || vb)
		return S_ERROR;

	IMultiuserWorkspaceEditPtr ipMWorkspace(m_edtWorkspace);	// Get IMultiuserWorkspaceEditPtr object
	//Supporting Non versioned
	if(ipMWorkspace && SUCCEEDED(ipMWorkspace->SupportsMultiuserEditSessionMode(esriMultiuserEditSessionMode::esriMESMNonVersioned, &vb)) && vb)
	{
		unsigned int nVer(0), nNonVer(0);
		for(const auto& edtLayerInfo : m_edtLayerInfos)
		{
			IVersionedObjectPtr ipVer(edtLayerInfo.second.getTable());
			if(!ipVer) return S_ERROR;
			if(FAILED(ipVer->get_IsRegisteredAsVersioned(&vb))) return S_ERROR;
			if(vb)
				++nVer;
			else
				++nNonVer;
		}
		if(nNonVer == m_edtLayerInfos.size())
		{
			m_isVersioned = false;
			return SUCCEEDED(ipMWorkspace->StartMultiuserEditing(esriMultiuserEditSessionMode::esriMESMNonVersioned)) ? S_NON_VERSIONED : S_ERROR;
		}
		else if(nVer != m_edtLayerInfos.size())	//Not at all opened tables are versioned
		{
			return S_ERROR_MIXED;
		}
	}

	//Start it with the versioned session
	if( SUCCEEDED( m_edtWorkspace->StartEditing(VariantBool(bRedoUndo)) ))
	{
		m_isVersioned = true;
		return S_VERSIONED;
	}

	return S_ERROR;
}

bool BaseDataCreator::stopEdit(bool isSave)
{
	if (!StopEditOperation() || !StopEditing(isSave) )
		return false;

	return true;
}

bool BaseDataCreator::abort()
{
	if (!AbortEditOperation() || !StopEditing(false) )
		return false;

	return true;
}

bool BaseDataCreator::tableAnalyze(const CString& layerName)
{
	IDatasetAnalyzePtr datasetAnalyze(m_edtLayerInfos[layerName].getFeatureClass());

	if(!datasetAnalyze)
		return true;

	// B,F,Sテーブルの統計情報を取得
	if( FAILED(datasetAnalyze->Analyze(
					(int)esriTableComponents::esriBusinessTable
					+ (int)esriTableComponents::esriFeatureTable)) )
	{
		return false;
	}
	return true;
}

bool BaseDataCreator::StopEditing(bool bSave)
{
	if(!m_edtWorkspace) return false;

	return SUCCEEDED(m_edtWorkspace->StopEditing(VariantBool(bSave)));
}

bool BaseDataCreator::StartEditOperation()
{
	if(!m_edtWorkspace) return false;
	
	if(!m_isVersioned) return true;

	if( FAILED( m_edtWorkspace->StartEditOperation() ) ) {
		m_edtWorkspace->StopEditing(false);
		return false;
	}

	return true;
}

bool BaseDataCreator::StopEditOperation()
{
	if(!m_edtWorkspace) return false;

	if(!m_isVersioned) return true;

	if( FAILED( m_edtWorkspace->StopEditOperation()) ) {
		m_edtWorkspace->StopEditing(false);
		return false;
	}

	return true;
}

bool BaseDataCreator::AbortEditOperation()
{
	if(!m_edtWorkspace) return false;

	return SUCCEEDED(m_edtWorkspace->AbortEditOperation());
}

