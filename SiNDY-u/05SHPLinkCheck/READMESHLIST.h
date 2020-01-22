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
#include<vector>

class CREADMESHLIST
{
public:
	CREADMESHLIST(_TCHAR* pFilePath);
	~CREADMESHLIST(void);
	bool ReadMeshList(std::vector<long>& rMeshList);       //メッシュリストを読み込み、vectorに格納する関数

private:
	std::wstring m_FilePath;

	//メッシュコードが正しいかチェックする関数
	bool MeshCheck(long aMeshCode);
};
