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

/*
**	MPQJoint
**
**		MapCube-Network joint info handling class
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "MPQJoint.h"
#include "MPQVer1.h"
#include "ZEndian.h"
#include "ZFile.h"
#include "ZWaste.h"


/*
**	データの取り出し
*/

MPQ::Attr* MPQJoint::getAttr(int cID)
{
	if(cID <= 0) return 0;
	if(cID > int(mAttr.size())) return 0;

	return mAttr[cID-1];
}


/*
**	データのクリア
*/

void MPQJoint::clear(void)
{
	for(vector<MPQ::Attr*>::iterator i=mAttr.begin() ; i!=mAttr.end() ; ++i) delete *i;
	mAttr .clear();
	mJoint.clear();
}


/*
**	読み込み
*/

bool MPQJoint::read(const char* cDir, int cMesh)
{
	char	aFilename[256];
	sprintf(aFilename, "%s/%d/%d.%s", cDir, cMesh/100, cMesh, mExt);
	ifstream	aStream(aFilename, ios::binary | ios::in);
	if(! aStream) return false;

	char	aBuffer[2*sizeof(Info)];
	Info	aInfo[2];
	aStream.read(aBuffer, 2*sizeof(Info));
	aInfo[0].eAddr  = ZEndian::UInt32B(aBuffer);
	aInfo[0].eSize  = ZEndian::UInt32B(aBuffer+4);
	aInfo[0].eCount = ZEndian::UInt32B(aBuffer+8);
	aInfo[1].eAddr  = ZEndian::UInt32B(aBuffer+12);
	aInfo[1].eSize  = ZEndian::UInt32B(aBuffer+16);
	aInfo[1].eCount = ZEndian::UInt32B(aBuffer+20);

	clear();
	if(! readAttr (aStream, aInfo[0])) return false;
	if(! readJoint(aStream, aInfo[1])) return false;
	aStream.close();

	return true;
}

//! MapCube 属性データの読み込み
bool MPQJoint::readAttr(istream& is, const Info& cInfo)
{
	if(0 == cInfo.eSize)  return false;
	if(0 == cInfo.eCount) return false;

	vector<char>	aBuffer;
	aBuffer.resize(cInfo.eSize);
	is.seekg(cInfo.eAddr, ios::beg);
	is.read (&aBuffer[0], cInfo.eSize);

	char*	pPtr = &aBuffer[0];
	for(int i=0 ; i<cInfo.eCount ; ++i)
	{
		MPQ::Attr*	pAttr = new MPQ::Attr(i+1);
		pPtr += MPQVer1::SetAttr(pPtr, pAttr);
		mAttr.push_back(pAttr);
	}

	return true;
}

//! リンク接続情報の読み込み
bool MPQJoint::readJoint(istream& is, const Info& cInfo)
{
	if(0 == cInfo.eSize)  return false;
	if(0 == cInfo.eCount) return false;

	vector<char>	aBuffer;
	aBuffer.resize(cInfo.eSize);
	is.seekg(cInfo.eAddr, ios::beg);
	is.read (&aBuffer[0], cInfo.eSize);

	char*	pPtr = &aBuffer[0];
	for(int i=0 ; i<cInfo.eCount ; ++i)
	{
		Joint	aJoint;
		aJoint.eID1  = ZEndian::UInt32B(pPtr);
		aJoint.eID2  = ZEndian::UInt32B(pPtr+4);
		aJoint.eFrom = ZEndian::UInt32B(pPtr+8);
		aJoint.eTo   = ZEndian::UInt32B(pPtr+12);
		aJoint.eAttr = ZEndian::UInt32B(pPtr+16);
		pPtr += 20;
		mJoint.insert(aJoint);
	}

	return true;
}


/*
**	書き込み
*/

bool MPQJoint::write(const char* cDir, int cMesh)
{
	char	aFilename[256];
	sprintf(aFilename, "%s/%d", cDir, cMesh/100);
	if(! ZFile::CreateDir(aFilename))
	{
		ZWaste	aWaste;
		while(aWaste.getTime() < 0.1) { }
	}
	if(! ZFile::CreateDir(aFilename)) return false;

	sprintf(aFilename, "%s/%d/%d.%s", cDir, cMesh/100, cMesh, mExt);
	ofstream	aStream(aFilename, ios::binary | ios::out);
	if(! aStream) return false;

	Info	aInfo[2];
	aStream.write(reinterpret_cast<char*>(aInfo), 2*sizeof(Info));
	if(! writeAttr (aStream, &aInfo[0])) return false;
	if(! writeJoint(aStream, &aInfo[1])) return false;

	aStream.seekp(0, ios::beg);
	ZEndian::Write32B(aStream, aInfo[0].eAddr);
	ZEndian::Write32B(aStream, aInfo[0].eSize);
	ZEndian::Write32B(aStream, aInfo[0].eCount);
	ZEndian::Write32B(aStream, aInfo[1].eAddr);
	ZEndian::Write32B(aStream, aInfo[1].eSize);
	ZEndian::Write32B(aStream, aInfo[1].eCount);
	aStream.close();

	return true;
}

//! MapCube 属性の書き込み
bool MPQJoint::writeAttr(ostream& os, Info* cInfo)
{
	if(0 == mAttr.size()) return false;

	cInfo->eAddr  = os.tellp();
	cInfo->eCount = 0;
	for(vector<MPQ::Attr*>::iterator i=mAttr.begin() ; i!=mAttr.end() ; ++i)
	{
		MPQVer1::WriteAttr(os, **i);
		++cInfo->eCount;
	}
	cInfo->eSize = static_cast<int>(os.tellp()) - cInfo->eAddr;

	return true;
}

//! リンク接続情報の書き込み
bool MPQJoint::writeJoint(ostream& os, Info* cInfo)
{
	if(0 == mJoint.size()) return false;

	cInfo->eAddr  = os.tellp();
	cInfo->eCount = 0;
	for(multiset<Joint>::iterator i=mJoint.begin() ; i!=mJoint.end() ; ++i)
	{
		ZEndian::Write32B(os, (*i).eID1);
		ZEndian::Write32B(os, (*i).eID2);
		ZEndian::Write32B(os, (*i).eFrom);
		ZEndian::Write32B(os, (*i).eTo);
		ZEndian::Write32B(os, (*i).eAttr);
		++cInfo->eCount;
	}
	cInfo->eSize = static_cast<int>(os.tellp()) - cInfo->eAddr;

	return true;
}

