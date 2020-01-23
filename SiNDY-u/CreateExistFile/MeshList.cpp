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

// MeshList.cpp: MeshList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "MeshList.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MeshList::MeshList()
{

	mMeshListFP	= NULL;

}

MeshList::~MeshList()
{

	if(mMeshListFP != NULL)	fclose(mMeshListFP);

}

bool MeshList::
Open(const char *cFileName)
{

	char aBuffer[1000];

	mMeshListFP = fopen(cFileName, "r");
	if(mMeshListFP == NULL) {
		cout << "[MeshList]オープンできません: " << cFileName << endl;
		return false;
	}

	while(1) {

		if(fgets(aBuffer, 1000, mMeshListFP) == NULL) {
			if(feof(mMeshListFP)) break;
			else {
				cout << "[MeshList]読み込みエラー" << endl;
				return false;
			}
		}

		string	aMCTmp(aBuffer, 8);
		MeshListCont	aMLCont(aMCTmp);
		insert(aMLCont);

	}

	return true;

}

bool MeshList::
Add(const char *cMeshCode)
{

	if(strlen(cMeshCode) != 8) return false;
	if(strspn(cMeshCode, "0123456789") != strlen(cMeshCode)) return false;

	MeshListCont	aMLCont(cMeshCode);
	insert(aMLCont);

	return true;

}

bool MeshList::
Add(long cMeshCode)
{

	if(cMeshCode < 10000000 || 99999999 < cMeshCode) return false;

	char	aMeshCodeStr[9];
	sprintf_s(aMeshCodeStr, 9, "%8d", cMeshCode);

	MeshListCont	aMLCont(aMeshCodeStr);
	insert(aMLCont);

	return true;

}
