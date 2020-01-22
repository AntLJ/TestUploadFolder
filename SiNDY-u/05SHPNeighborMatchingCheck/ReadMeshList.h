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

//�w�肳�ꂽ�f�[�^�Z�b�g�̃��b�V�����X�g��ǂݍ��ރR���e�i�ɓǂݍ��ރN���X
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
