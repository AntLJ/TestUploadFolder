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
#include"Definition.h"
#include<map>
#include<set>

class CGetLayerInfo
{
public:
	CGetLayerInfo(long aMeshCode,std::wstring aAllow);
	~CGetLayerInfo(void);
	//着目メッシュと上下左右隣接するメッシュの境界線上の情報を一つにまとめる。
	bool Merge5MeshInfo( std::wstring & rDirName, std::wstring & rLayerName, std::set<CheckKey> & rMergeVector);
	long Merge5MiddleMesh( std::wstring & rDirName, std::wstring & rLayerName);
	long Merge5BaseMesh( std::wstring & rDirName, std::wstring & rLayerName);
	long Merge5CityMesh( std::wstring & rDirName, std::wstring & rLayerName);
private:
	long m_MeshCode;
	std::multimap<Key,Info> m_MainMap, m_NorthMap,m_SouthMap,m_EastMap,m_WestMap;
	std::wstring m_Allow;

};
