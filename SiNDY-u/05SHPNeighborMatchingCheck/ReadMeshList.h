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
#include<set>

//指定されたデータセットのメッシュリストを読み込むコンテナに読み込むクラス
class CReadMeshList
{
public:
	CReadMeshList(std::wstring & rFilePath, std::wstring & rDataSetName);
	~CReadMeshList(void);
	bool ReadList(std::set<long> & rListSet);
	virtual bool CheckMesh(long aMesh);

private:
	std::wstring m_FilePath;
	std::wstring m_DataSetName;
};
