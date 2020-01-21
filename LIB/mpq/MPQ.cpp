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
**	MPQ
**
**		MapCube Handling library
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "MPQ.h"
#include "MPQIO.h"
#include "ZEndian.h"
#include "ZFile.h"
#include "ZDigit.h"


/*
**	コンストラクタ＆デストラクタ
*/

MPQ::MPQ(MPQIO* cIO)
	: mIO(cIO)
{
}

MPQ::~MPQ()
{
	for(vector<Link*>::iterator i=mLink.begin() ; i!=mLink.end() ; ++i) delete *i;
	mLink.clear();

	for(vector<Attr*>::iterator i=mAttr.begin() ; i!=mAttr.end() ; ++i) delete *i;
	mAttr.clear();
}


/**
**	読み込み
*/

bool MPQ::read(const char* cDir, int cMesh)
{
	char	aFilename[256];
	sprintf(aFilename, "%s/%d/%d.mpq", cDir, cMesh/100, cMesh);
	ifstream	aStream(aFilename, ios::binary | ios::in);
	if(! aStream) return false;

	aStream.seekg(0, ios::end);
	int	aSize = aStream.tellg();
	if(0 == aSize) return false;

	vector<char>	aBuffer;
	aBuffer.resize(aSize);
	aStream.seekg(0, ios::beg);
	aStream.read (&aBuffer[0], aSize);
	aStream.close();

	char*	pPtr = &aBuffer[0];
	char*	pL0000 = 0;
	char*	pL1000 = 0;
	char*	pL2000 = 0;
	while(aSize > 0)
	{
		int	aLID  = ZEndian::UInt32B(pPtr);
		int	aPart = ZEndian::UInt32B(pPtr+4);
		if(     0x00000000 == aLID) pL0000 = pPtr;
		else if(0x00001000 == aLID) pL1000 = pPtr;
		else if(0x00002000 == aLID) pL2000 = pPtr;
		pPtr  += aPart;
		aSize -= aPart;
	}
	if(0 == pL0000) return false;
	if(0 == pL1000) return false;

	// 管理レイヤ
	if(! mIO->analyze(pL0000, &mHeader)) return false;

	// MapCube 属性情報（存在しなくてもエラーではない）
	if(0 != pL2000) mIO->analyze(pL2000, mAttr);

	// 形状レイヤ
	if(! mIO->analyze(pL1000, mLink)) return false;

	return true;
}
bool MPQ::read(const char* cDir)
{
//	char	aFilename[256];
//	sprintf(aFilename, "%s/%d/%d.mpq", cDir, cMesh/100, cMesh);
	ifstream	aStream(cDir, ios::binary | ios::in);
	if(! aStream) return false;

	aStream.seekg(0, ios::end);
	int	aSize = aStream.tellg();
	if(0 == aSize) return false;

	vector<char>	aBuffer;
	aBuffer.resize(aSize);
	aStream.seekg(0, ios::beg);
	aStream.read (&aBuffer[0], aSize);
	aStream.close();

	char*	pPtr = &aBuffer[0];
	char*	pL0000 = 0;
	char*	pL1000 = 0;
	char*	pL2000 = 0;
	while(aSize > 0)
	{
		int	aLID  = ZEndian::UInt32B(pPtr);
		int	aPart = ZEndian::UInt32B(pPtr+4);
		if(     0x00000000 == aLID) pL0000 = pPtr;
		else if(0x00001000 == aLID) pL1000 = pPtr;
		else if(0x00002000 == aLID) pL2000 = pPtr;
		pPtr  += aPart;
		aSize -= aPart;
	}
	if(0 == pL0000) return false;
	if(0 == pL1000) return false;

	// 管理レイヤ
	if(! mIO->analyze(pL0000, &mHeader)) return false;

	// MapCube 属性情報（存在しなくてもエラーではない）
	if(0 != pL2000) mIO->analyze(pL2000, mAttr);

	// 形状レイヤ
	if(! mIO->analyze(pL1000, mLink)) return false;

	return true;
}


/**
**	書き込み
*/

bool MPQ::write(const char* cDir, int cMesh)
{
	char	aFilename[256];
	sprintf(aFilename, "%s/%d", cDir, cMesh/100);
	if(! ZFile::CreateDir(aFilename)) return false;

	sprintf(aFilename, "%s/%d/%d.mpq", cDir, cMesh/100, cMesh);
	ofstream	aStream(aFilename, ios::binary | ios::out);
	if(! aStream) return false;

	if(! mIO->write(aStream, mHeader)) return false;
	if(! mIO->write(aStream, mLink)) return false;
	if(! mIO->write(aStream, mAttr)) return false;
	aStream.close();

	return true;
}


/*
**	for Attr
*/

//! コンストラクタ
MPQ::Attr::Attr(int cID)
	: mID(cID)
{
	eHeight  = INVALID;
	eSea     = INVALID;
	eSurface = INVALID;
	eLamp    = INVALID;
	eCenter  = INVALID;
	eValue   = 0;
}


/*
**	コピー
*/

MPQ::Attr* MPQ::cloneAttr(const Attr& cAttr)
{
	Attr*	pAttr = newAttr();
	pAttr->eHeight  = cAttr.eHeight;
	pAttr->eSea     = cAttr.eSea;
	pAttr->eLane    = cAttr.eLane;
	pAttr->eLine    = cAttr.eLine;
	pAttr->eFence   = cAttr.eFence;
	pAttr->eSide    = cAttr.eSide;
	pAttr->eSurface = cAttr.eSurface;
	pAttr->eLamp    = cAttr.eLamp;
	pAttr->eCenter  = cAttr.eCenter;
	pAttr->eValue   = cAttr.eValue;
	pAttr->eStruct  = cAttr.eStruct;
	pAttr->eMemo    = cAttr.eMemo;

	return pAttr;
}


/*
**	MapCube 属性の有効性検証
*/

bool MPQ::Attr::valid(void) const
{
	if(INVALID != eHeight)  return true;
	if(INVALID != eSea)     return true;
	if(INVALID != eSurface) return true;
	if(INVALID != eLamp)    return true;
	if(INVALID != eCenter)  return true;

	if(eLane .valid()) return true;
	if(eLine .valid()) return true;
	if(eFence.valid()) return true;
	if(eSide .valid()) return true;

	if(0 != eStruct.length()) return true;
	if(0 != eMemo  .length()) return true;

	return false;
}


/*
**	for Point
*/

//! コンストラクタ
MPQ::Point::Point(void)
{
	eX    = 0;
	eY    = 0;
	eID   = 0;
	eIN   = false;
	eAttr = 0;
}

//! コンストラクタ
MPQ::Point::Point(int cX, int cY)
	: eX(cX), eY(cY)
{
	eID   = 0;
	eIN   = false;
	eAttr = 0;
}


/*
**	for Link
*/

//! コンストラクタ
MPQ::Link::Link(void)
{
	eClass = -1;
	eWay   = Free;
	eValue = 0;
}

