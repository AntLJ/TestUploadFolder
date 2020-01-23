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

#include "StdAfx.h"
#include "DBReplicater.h"
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_core/sindymacroutil.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace sindy;
using namespace errorcode;
using namespace schema;

DBReplicater::DBReplicater()
{
}

DBReplicater::~DBReplicater()
{
}

// テーブルをコピーする
bool DBReplicater::CopyTable2( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new )
{
	IWorkspacePtr ipSourceWorkspace((IWorkspace*)work);
	IWorkspacePtr ipTargetWorkspace((IWorkspace*)work_new);
	esriWorkspaceType esriSourceDataType, esriTargetDataType;
	ipSourceWorkspace->get_Type(&esriSourceDataType);
	ipTargetWorkspace->get_Type(&esriTargetDataType);
	esriDatasetType esriDataType = esriDTFeatureClass;

	if ((esriSourceDataType == esriFileSystemWorkspace) || (esriTargetDataType == esriFileSystemWorkspace))
		return false; // Should be a throw

	// コピー元
	IDatasetPtr ipSourceWorkspaceDataset(ipSourceWorkspace);
	INamePtr ipSourceName;
	ipSourceWorkspaceDataset->get_FullName(&ipSourceName);
	IWorkspaceNamePtr ipSourceWorkspaceName(ipSourceName);

	// ペースト先
	IDatasetPtr ipTargetWorkspaceDataset(ipTargetWorkspace);
	INamePtr ipTargetName;
	ipTargetWorkspaceDataset->get_FullName(&ipTargetName);
	IWorkspaceNamePtr ipTargetWorkspaceName(ipTargetName);

	// Set the name of the object to be copied
	ITablePtr ipSourceTable( work.OpenTable(name.c_str()) );
	INamePtr ipDatasetName;
	((IDatasetPtr)ipSourceTable)->get_FullName(&ipDatasetName);

	//Setup mapping for copy/paste
	IEnumNamePtr ipFromName(CLSID_NamesEnumerator);
	IEnumNameEditPtr ipEditFromName(ipFromName);
	ipEditFromName->Add((INamePtr)ipDatasetName);

	// Generate name mapping
	IEnumNameMappingPtr ipFromNameMapping;
	IGeoDBDataTransferPtr ipGeoDBDataTransfer(CLSID_GeoDBDataTransfer);
	VARIANT_BOOL vaIsConflicts = VARIANT_FALSE;
	if( FAILED( ipGeoDBDataTransfer->GenerateNameMapping(ipFromName, (INamePtr)ipTargetWorkspaceName, &ipFromNameMapping, &vaIsConflicts) ) )
		return false; // 何かがおかしい

	// ターゲットレイヤ名の変更
	INameMappingPtr ipNameMapping;
	ipFromNameMapping->Reset();
	while( S_OK == ipFromNameMapping->Next( &ipNameMapping ) && ipNameMapping )
	{
		CComBSTR bstrName;
		ipNameMapping->get_TargetName(&bstrName);
		if( 0 == _wcsicmp( bstrName, uh::toWStr(name).c_str() ) )
			ipNameMapping->put_TargetName(CComBSTR(name_new.c_str()));
	}

	// Copy/Pate the data
	if( FAILED( ipGeoDBDataTransfer->Transfer(ipFromNameMapping, ipTargetName) ) )
	{
		std::cerr << "# ERROR: テーブル[" << uh::toStr(name) << _T("] -> [") << uh::toStr(name_new) << "]にコピー失敗 理由：" <<  (LPCSTR)CStringA(GETCOMERRORSTRING()) << std::endl;
		_ASSERTE( false );
		return false;
	}
	return true;
}

// テーブルをコピーする
bool DBReplicater::CopyTable( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new )
{
	if( work.GetNameString()->IsSDE() )
		return false; // SDE上では動作させない

	IDatasetPtr table = work.OpenTable( name.c_str() );
	VARIANT_BOOL canCopy = VARIANT_FALSE;
	table->CanCopy(&canCopy);
	if( !canCopy )
		return CopyTable2( work, name, work_new, name_new  );

	IDatasetPtr table_new;
	if( FAILED( table->Copy( CComBSTR(uh::toWStr(name_new).c_str()), (IWorkspace*)work, &table_new ) ) )
	{
		std::cerr << "# ERROR: テーブル[" << uh::toStr(name) << "] -> [" << uh::toStr(name_new) << "]にコピー失敗 理由：" <<   (LPCSTR)CStringA(GETCOMERRORSTRING()) << std::endl;
		_ASSERTE( false );
		return false;
	}

	return true;
}

// 条件に合致するもののみレコード追加する
bool DBReplicater::InsertTable( IQueryFilterPtr filter, IWorkspacePtr srcWorkspace, const uh::tstring& srcTable, IWorkspacePtr destWorkspace, const uh::tstring& destTable, const std::list<uh::tstring>& listColumns, bool all_columns/* = false*/ )
{
	ITablePtr srcT;
	((IFeatureWorkspacePtr)srcWorkspace)->OpenTable( CComBSTR(srcTable.c_str()), &srcT );

	ITablePtr destT;
	((IFeatureWorkspacePtr)destWorkspace)->OpenTable( CComBSTR(destTable.c_str()), &destT );

	CString strSubFields;
	bool has_shape = false;
	IFeatureClassPtr ipClass(srcT);
	if( ipClass )
		has_shape = true;

#ifdef _DEBUG
	long count = 0;
	srcT->RowCount( filter, &count );
#endif // ifdef _DEBUG

	_ICursorPtr cur;
	if( FAILED( srcT->Search( filter, VARIANT_TRUE, &cur ) ) )
	{
		CString msg;
		msg.Format(_T("テーブル[%s] -> [%s]にコピー失敗 : 理由:%s"), srcTable.c_str(), destTable.c_str(), GETCOMERRORSTRING());
		_ASSERT_EXPR( false, msg );
		return false;
	}

	_ICursorPtr dest_cur;
	if( FAILED( destT->Insert( VARIANT_TRUE, &dest_cur ) ) )
	{
		CString msg;
		msg.Format(_T("テーブル[%s] -> [%s]にコピー失敗 : 理由:%s"), srcTable.c_str(), destTable.c_str(), GETCOMERRORSTRING());
		_ASSERT_EXPR( false, msg );
		return false;
	}
	IRowBufferPtr buf;
	destT->CreateRowBuffer( &buf );

	// カラムインデックス番号取得
	std::list<std::pair<long,long>> idxs;
	IFieldsPtr ipSrcFields, ipDestFields;
	cur->get_Fields( &ipSrcFields );
	destT->get_Fields( &ipDestFields );
	CFieldMap srcFields(ipSrcFields), destFields(ipDestFields);
	if( all_columns )
	{
		for( const CFieldMap::_FIELD& field : srcFields )
		{
			long srcIdx = srcFields.FindField(field.name);
			long destIdx = destFields.HasField(field.name);
			if( 0 <= srcIdx && 0 <= destIdx )
				idxs.push_back( std::make_pair( srcIdx, destIdx ) );
		}
	}
	else {
		for( const auto& col : listColumns )
		{
			long srcIdx = srcFields.FindField(col.c_str());
			long destIdx = destFields.FindField(col.c_str());
			if( 0 <= srcIdx && 0 <= destIdx )
				idxs.push_back( std::make_pair( srcIdx, destIdx ) );
		}
	}

	_IRowPtr row;
	while( S_OK == cur->NextRow( &row ) && row )
	{
		// フィーチャクラスの場合は形状はコピー対象とする
		if( has_shape )
			((IFeatureBufferPtr)buf)->putref_Shape( AheGetShapeCopy((IFeaturePtr)row) );

		for( const auto& idx : idxs )
		{
			CComVariant va;
			row->get_Value( idx.first, &va );
			// 半角空文字の場合は、NULLとして突っ込む（bug 10563）
			if( VT_BSTR == va.vt && 0 == wcscmp( L" ", va.bstrVal ) && destFields.IsNullable(idx.second) )
			{
				va.Clear();
				va.vt = VT_NULL;
			}
			buf->put_Value( idx.second, va );
		}
		if( FAILED( dest_cur->InsertRow( buf, &VARIANT() ) ) )
		{
			CString msg;
			msg.Format(_T("テーブル[%s] -> [%s]にコピー失敗 : 理由:%s"), srcTable.c_str(), destTable.c_str(), GETCOMERRORSTRING());
			_ASSERT_EXPR( false, msg );
			return false;
		}
	}
	return true;
}
