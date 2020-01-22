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
#include "stdafx.h"
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
Open(char *cFileName)
{

	char aBuffer[1000], aMeshCodeTmp[9];

	mMeshListFP = fopen(cFileName, "r");
	if(mMeshListFP == NULL) {
		fprintf(stderr, "[MeshList]オープンできません: %s\n", cFileName);
		return false;
	}

	while(1) {

		if(fgets(aBuffer, 1000, mMeshListFP) == NULL) {
			if(feof(mMeshListFP)) break;
			else {
				fprintf(stderr, "[MeshList]読み込みエラー\n");
				return false;
			}
		}

		char	*p = strchr(aBuffer, '\n');
		if(p != NULL) *p = '\0';	// 改行文字をヌル文字に置き換え
		strcpy(aMeshCodeTmp, aBuffer);
		MeshListCont	aMLCont(aMeshCodeTmp);
		insert(aMLCont);

	}

	return true;

}

bool MeshList::
Create(char *cFileName)
{

	mMeshListFP = fopen(cFileName, "w");
	if(mMeshListFP == NULL) {
		fprintf(stderr, "[MeshList]オープンできません: %s\n", cFileName);
		return false;
	}

	iterator	aItr;
	for(aItr = begin(); aItr != end(); aItr++) {
		fprintf(mMeshListFP, "%s\n", (*aItr).eMeshCode);
		fflush(mMeshListFP);
	}

	return true;

}

bool MeshList::
Add(char *cMeshCode)
{

	if(strlen(cMeshCode) != 8 && strlen(cMeshCode) != 6 && strlen(cMeshCode) != 4) return false;
	if(strspn(cMeshCode, "0123456789") != strlen(cMeshCode)) return false;

	MeshListCont	aMLCont(cMeshCode);
	insert(aMLCont);

	return true;

}

bool MeshList::
Add(LONG cMeshCode)
{

	if(cMeshCode < 1000 || 99999999 < cMeshCode) return false;

	char	aMeshCodeStr[9];
	sprintf(aMeshCodeStr, "%ld", cMeshCode);

	if(strlen(aMeshCodeStr) != 8 && strlen(aMeshCodeStr) != 6 && strlen(aMeshCodeStr) != 4) return false;

	MeshListCont	aMLCont(aMeshCodeStr);
	insert(aMLCont);

	return true;

}
