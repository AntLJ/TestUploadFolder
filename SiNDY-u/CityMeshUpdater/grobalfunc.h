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
#include <map>
#include <vector>

namespace gf {

	// サーバに接続してワークスペースを取得する
	IWorkspacePtr OpenWorkSpace( LPCTSTR strServername, LPCTSTR strUsername, LPCTSTR strPassword, LPCTSTR strVersionname );


	/**
	* @brief	あるバージョンに子バージョンを作成し、そのワークスペースを返す
	*
	* @note		既に同名のバージョンがあった場合は消してから新たに作り直す
	*
	* @param	ipWorkspace			[in]	親バージョンのワークスペース
	* @param	strVersionName		[in]	作るバージョンのバージョン名
	* @return	新しく作った子バージョンのワークスペース
	*/
	IWorkspacePtr CreateNewVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName );

	// テーブルのバージョン間における属性差分マップを取得する
	bool DiffMapBetweenVersion( IWorkspacePtr ipCurrentWorkspace, LPCTSTR strTableName, MESHCODEMAP& DiffMap );

	// テーブルを全行読み込んでマップに格納する
	MESHCODEMAP Table2Map( IWorkspace* ipWorkspace, LPCTSTR strTableName, LPCTSTR strKey );

	/**
	* @brief	2つマップを比較して重複するキーは削除する
	*
	* @note		TargetMapとCompareMapを比較して同じキーがあれば、TargetMap側では削除します。
	*
	* @param	TargetMap			[in]	実際に削除される
	* @param	CompareMap			[in]	比較に使われる
	* @return	削除した行数
	*/
	int DeleteDuplicateKeys( MESHCODEMAP& TargetMap, const MESHCODEVECMAP& CompareMap );

	MESHCODEMAP CreateMapFromQuery( IFeatureClass* ipFeatClass, const MESHCODEMAP& mapTmpCityMesh );
	
	int UpdateTable( const MESHCODEMAP& SrcMap, MESHCODEMAP& DstMap );

	int UpdateTable( const MESHCODEVECMAP& SrcMap, const std::vector<CString>& vecFdield, MESHCODEMAP& DstMap );
	
	std::vector<CString> Tokenize2Vector( const CString & strSrc, LPCTSTR lpcszTorkens, LPCTSTR lpcszTrim );

}
