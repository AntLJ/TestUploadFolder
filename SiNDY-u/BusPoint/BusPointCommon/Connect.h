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

#pragma once
#include "stdafx.h"
#include "../FieldMap.h"
#include <sindy/workspace.h>

namespace DB
{
	/**
	 * @brief   ワークスペースオープン
	 * @param   name           [in]  接続名
	 * @param   ipWorkspace    [in]  ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
	{
		ipWorkspace = sindy::create_workspace( name );
		if( ! ipWorkspace ){
			std::cerr << "#FATAL DB Connect Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   ワークスペースオープン
	 * @param   name            [in]  接続名
	 * @param   ipEditWorkspace [in]  ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool OpenWorkspace( const CString& name, IWorkspaceEditPtr& ipEditWorkspace )
	{
		IWorkspacePtr ipWorkspace;
		if( ! OpenWorkspace( name, ipWorkspace ) )
			return false;
		ipEditWorkspace = ipWorkspace;
		return true;
	};

	/**
	 * @brief   フィーチャクラスオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipFeatureClass [out] 取得するフィーチャクラス
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
		if( ! ipFeatureClass ){
			std::cerr << "#FATAL FC Open Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		// フィールドマップ作成
		fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

		return true;
	};

	/**
	 * @brief   テーブルオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipTable        [out] 取得するテーブル
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
	{
		// オープン
		((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
		if( ! ipTable ){
			std::cerr << "#FATAL Table Open Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		// フィールドマップ作成
		fieldMap.CreateFieldMap( ipTable->GetFields() );
	
		return true;
	};

	/**
	 * @brief   編集開始
	 * @param   ipEditWorkspace [in]  ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool StartEdit( IWorkspaceEditPtr& ipEditWorkspace )
	{
		// 編集開始（Undo Redo がいらないため、false）
		if( S_OK != ipEditWorkspace->StartEditing( VARIANT_FALSE ) || S_OK != ipEditWorkspace->StartEditOperation() ){
			std::cerr << "#ERROR Start Edit Fail" << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   編集保存＆終了
	 * @param   ipEditWorkspace [in]  ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	inline bool StopEdit( IWorkspaceEditPtr& ipEditWorkspace )
	{
		// 編集確定
		if( S_OK != ipEditWorkspace->StopEditOperation() ){
			std::cerr << "#ERROR Stop Edit Operation Fail" << std::endl;
			ipEditWorkspace->AbortEditOperation();
			return false;
		}
	
		// 編集終了（保存する必要があるため、true）
		if( S_OK != ipEditWorkspace->StopEditing( VARIANT_TRUE ) ){
			std::cerr << "#ERROR Stop Edit Fail" << std::endl;
			return false;
		}
		return true;
	};



};
