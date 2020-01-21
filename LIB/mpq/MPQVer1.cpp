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
**	MPQVer1
**
**		MPQ IO for format ver.1 class
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "MPQVer1.h"
#include "ZEndian.h"
#include "ZDigit.h"



/*
**	読み込み
*/

//! ファイル管理レイヤーの解析
bool MPQVer1::analyze(char* cBuffer, MPQ::Header* cHeader)
{
if(0 == cBuffer) cerr << "ERROR Header\n";
	if(0 == cBuffer) return false;

	char*	pPtr = cBuffer + 8;
	cHeader->eVer = ZEndian::UInt32B(pPtr);
//	if(getVer() != cHeader->eVer) return false;

	cHeader->eMesh   = ZDigit::BCDtoDEC(ZEndian::UInt32B(pPtr+4)) / 100;
	cHeader->eCreate = ZDigit::BCDtoDEC(ZEndian::UInt32B(pPtr+8));
	cHeader->eEdit   = ZDigit::BCDtoDEC(ZEndian::UInt32B(pPtr+12));
	cHeader->eIN     = ZDigit::BCDtoDEC(ZEndian::UInt32B(pPtr+16));

	return true;
}

//! 形状レイヤの解析
bool MPQVer1::analyze(char* cBuffer, vector<MPQ::Link*>& cLink)
{
	if(0 == cBuffer) return false;

	char*	pPtr   = cBuffer + 8;
	int		aCount = ZEndian::UInt32B(pPtr);
	pPtr += 4;
	for(int i=0 ; i<aCount ; ++i)
	{
		MPQ::Link*	pLink = new MPQ::Link();
		int			aPCnt = ZEndian::UInt32B(pPtr);
		pLink->eClass = ZEndian::UInt32B(pPtr+4);
		int	aValue = ZEndian::UInt32B(pPtr+8);
		switch(aValue & 0x00000003)
		{
			case 0:	pLink->eWay = MPQ::Link::Free;	break;
			case 1:	pLink->eWay = MPQ::Link::Ord;	break;
			case 2:	pLink->eWay = MPQ::Link::Stop;	break;
			default:pLink->eWay = MPQ::Link::Free;	break;
		}
		pLink->eValue = ZEndian::UInt32B(pPtr+12);
		pPtr += 16;
		for(int j=0 ; j<aPCnt ; ++j)
		{
			MPQ::Point	aPoint;
			aPoint.eX = ZEndian::UInt32B(pPtr);
			aPoint.eY = ZEndian::UInt32B(pPtr+4);
			unsigned	aTemp = ZEndian::UInt32B(pPtr+8);
			if(0xffffffff == aTemp)
			{
				aPoint.eID = 0;
				aPoint.eIN = true;
			}
			else if(0xfffffffe == aTemp)
			{
				aPoint.eID = 0;
				aPoint.eIN = false;
			}
			else
			{
				aPoint.eID = aTemp;
				aPoint.eIN = true;
			}
			map<int, MPQ::Attr*>::iterator	f = mAttr.find(ZEndian::UInt32B(pPtr+12));
			aPoint.eAttr = (mAttr.end()==f) ? 0 : f->second;
			pLink->eShape.push_back(aPoint);
			pPtr += 16;
		}
		cLink.push_back(pLink);
	}

	return 0 != cLink.size();
}

//! MapCube属性レイヤの解析
bool MPQVer1::analyze(char* cBuffer, vector<MPQ::Attr*>& cAttr)
{
	if(0 == cBuffer) return false;

	char*	pPtr   = cBuffer + 8;
	int		aCount = ZEndian::UInt32B(pPtr);
	int		aFixed = ZEndian::UInt32B(pPtr+4);
	pPtr += 8;
	for(int i=0 ; i<aCount ; ++i)
	{
		int		aID   = ZEndian::UInt32B(pPtr);
		char*	pNext = pPtr + ZEndian::UInt32B(pPtr+4);
		if(mAttr.end() != mAttr.find(aID))
		{
			pPtr = pNext;
			continue;
		}

		pPtr += 12;
		MPQ::Attr*	pAttr = new MPQ::Attr(aID);
		SetAttr(pPtr, pAttr, aFixed);

		mAttr.insert(map<int, MPQ::Attr*>::value_type(aID, pAttr));
		cAttr.push_back(pAttr);
		pPtr = pNext;
	}

	return 0 != cAttr.size();
}


/*
**	書き込み
*/

//! 管理レイヤの書き込み
bool MPQVer1::write(ostream& cStream, const MPQ::Header& cHeader)
{
	r_write32B(cStream, 0x00000000);	// レイヤＩＤ
	r_write32B(cStream, 28);			// レイヤサイズ

	r_write32B(cStream, getVer());	// フォーマットバージョン
	r_write32B(cStream, ZDigit::DECtoBCD(100*cHeader.eMesh));
	r_write32B(cStream, ZDigit::DECtoBCD(cHeader.eCreate));
	r_write32B(cStream, ZDigit::DECtoBCD(cHeader.eEdit));
	r_write32B(cStream, ZDigit::DECtoBCD(cHeader.eIN));

	return true;
}

//! 形状レイヤの書き込み
bool MPQVer1::write(ostream& cStream, const vector<MPQ::Link*>& cLink)
{
	if(0 == cLink.size()) return false;

	int	aPos = cStream.tellp();
	r_write32B(cStream, 0x00001000);	// レイヤＩＤ
	r_write32B(cStream, 0);			// レイヤサイズ

	int	aObjID = 1;
	mObjID.clear();
	r_write32B(cStream, cLink.size());	// データ個数
	for(vector<MPQ::Link*>::const_iterator i=cLink.begin() ; i!=cLink.end() ; ++i)
	{
		const MPQ::Link*	pLink = *i;
		r_write32B(cStream, pLink->eShape.size());	// ポイント個数
		r_write32B(cStream, pLink->eClass);			// 表示種別
		r_write32B(cStream, pLink->eWay);			// リンク列属性
		r_write32B(cStream, 0);
		for(vector<MPQ::Point>::const_iterator j=pLink->eShape.begin() ; j!=pLink->eShape.end() ; ++j)
		{
			r_write32B(cStream, (*j).eX);	// Ｘ座標
			r_write32B(cStream, (*j).eY);	// Ｙ座標
			if(! (*j).eIN)
			{
				r_write32B(cStream, 0xfffffffe);
			}
			else if(0 == (*j).eID)
			{
				r_write32B(cStream, 0xffffffff);
			}
			else
			{
				r_write32B(cStream, (*j).eID);	// ＩＮノードＩＤ
			}
			if(0 == (*j).eAttr)
			{
				r_write32B(cStream, 0xffffffff);
			} else
			{
				r_write32B(cStream, aObjID);
				mObjID.insert(map<int, int>::value_type((*j).eAttr->getID(), aObjID));
				++aObjID;
			}
		}
	}

	int	aSize = static_cast<int>(cStream.tellp()) - aPos;
	cStream.seekp(aPos+4, ios::beg);
	r_write32B(cStream, aSize);
	cStream.seekp(0, ios::end);

	return true;
}

//! MapCube 属性レイヤの書き込み
bool MPQVer1::write(ostream& cStream, const vector<MPQ::Attr*>& cAttr)
{
	if(0 == cAttr.size()) return false;

	int	aCount = 0;
	for(vector<MPQ::Attr*>::const_iterator i=cAttr.begin() ; i!=cAttr.end() ; ++i)
	{
		if(mObjID.end() != mObjID.find((*i)->getID())) ++aCount;
	}
	if(0 == aCount) return false;

	int	aPos = cStream.tellp();
	r_write32B(cStream, 0x00002000);	// レイヤＩＤ
	r_write32B(cStream, 0);			// レイヤサイズ

	r_write32B(cStream, aCount);
	r_write32B(cStream, 52);
	for(vector<MPQ::Attr*>::const_iterator i=cAttr.begin() ; i!=cAttr.end() ; ++i)
	{
		const MPQ::Attr*	pAttr = *i;
		map<int, int>::const_iterator	f = mObjID.find(pAttr->getID());
		if(mObjID.end() == f) continue;

		int	aSize = 12 + SizeAttr(*pAttr);
		r_write32B(cStream, f->second);
		r_write32B(cStream, aSize);
		r_write32B(cStream, 0);

		WriteAttr(cStream, *pAttr);
		--aCount;
	}
	assert(0 == aCount);

	int	aSize = static_cast<int>(cStream.tellp()) - aPos;
	cStream.seekp(aPos+4, ios::beg);
	r_write32B(cStream, aSize);
	cStream.seekp(0, ios::end);

	return true;
}


/*
**	MapCube属性データの書き込み(Ver.1)
*/

// 読み込み
int MPQVer1::SetAttr(char* cPtr, MPQ::Attr* cAttr, int cFixed)
{
	cAttr->eHeight = ZEndian::UInt32B(cPtr);
	cAttr->eSea    = ZEndian::UInt32B(cPtr+4);
	cAttr->eLane .eLeft  = ZEndian::UInt32B(cPtr+8);
	cAttr->eLine .eLeft  = ZEndian::UInt32B(cPtr+12);
	cAttr->eLane .eRight = ZEndian::UInt32B(cPtr+16);
	cAttr->eLine .eRight = ZEndian::UInt32B(cPtr+20);
	cAttr->eFence.eLeft  = ZEndian::UInt32B(cPtr+24);
	cAttr->eSide .eLeft  = ZEndian::UInt32B(cPtr+28);
	cAttr->eFence.eRight = ZEndian::UInt32B(cPtr+32);
	cAttr->eSide .eRight = ZEndian::UInt32B(cPtr+36);
	cAttr->eSurface = ZEndian::UInt32B(cPtr+40);
	cAttr->eLamp    = ZEndian::UInt32B(cPtr+44);
	cAttr->eCenter  = ZEndian::UInt32B(cPtr+48);
	cAttr->eValue   = 0;

	const char*	pPtr = cPtr + ((cFixed<=0)?52:cFixed);
	char	aTemp[1024];
	int		aBytes = ZEndian::UInt32B(pPtr);
	if(0 == aBytes)
	{
		pPtr += 4;
	} else
	{
		memcpy(aTemp, pPtr+4, aBytes);
		aTemp[aBytes] = 0;
		cAttr->eStruct = aTemp;
		pPtr += 4 + 4*((aBytes-1)/4+1);
	}

	aBytes = ZEndian::UInt32B(pPtr);
	if(0 == aBytes)
	{
		pPtr += 4;
	} else
	{
		memcpy(aTemp, pPtr+4, aBytes);
		aTemp[aBytes] = 0;
		cAttr->eMemo = aTemp;
		pPtr += 4 + 4*((aBytes-1)/4+1);
	}

	return int(pPtr) - int(cPtr);
}

// テキストの書き込み
static void WriteText(ostream& os, const string& cText)
{
	MPQVer1::r_write32B(os, cText.length());
	if(0 != cText.length())
	{
		static char	sZero[] = { 0, 0, 0, 0 };
		os.write(cText.c_str(), cText.length());
		int	aRem = cText.length() % 4;
		if(0 != aRem) os.write(sZero, 4-aRem);
	}
}

// 書き込み
int MPQVer1::WriteAttr(ostream& cStream, const MPQ::Attr& cAttr)
{
	int	aTop = cStream.tellp();

	// 固定長部
	r_write32B(cStream, cAttr.eHeight);
	r_write32B(cStream, cAttr.eSea);
	r_write32B(cStream, cAttr.eLane.eLeft);
	r_write32B(cStream, cAttr.eLine.eLeft);
	r_write32B(cStream, cAttr.eLane.eRight);
	r_write32B(cStream, cAttr.eLine.eRight);
	r_write32B(cStream, cAttr.eFence.eLeft);
	r_write32B(cStream, cAttr.eSide .eLeft);
	r_write32B(cStream, cAttr.eFence.eRight);
	r_write32B(cStream, cAttr.eSide .eRight);
	r_write32B(cStream, cAttr.eSurface);
	r_write32B(cStream, cAttr.eLamp);
	r_write32B(cStream, cAttr.eCenter);

	// 可変長部
	WriteText(cStream, cAttr.eStruct);
	WriteText(cStream, cAttr.eMemo);

	return static_cast<int>(cStream.tellp()) - aTop;
}

int MPQVer1::SizeAttr(const MPQ::Attr& cAttr)
{
	int	aSize = 52;
	aSize += (0==cAttr.eStruct.length()) ? 4 : (4+4*((cAttr.eStruct.length()-1)/4+1));
	aSize += (0==cAttr.eMemo  .length()) ? 4 : (4+4*((cAttr.eMemo  .length()-1)/4+1));

	return aSize;
}

//LITTLE_ENDIAN	→ BIG_ENDIANを考慮した書き込み
void MPQVer1::r_write32B(ostream& cStream,int cValue)
{
	uchar aTemp[4];

	aTemp[0] = (cValue&0xff000000) >> 24;
	aTemp[1] = (cValue&0x00ff0000) >> 16;
	aTemp[2] = (cValue&0x0000ff00) >> 8;
	aTemp[3] = (cValue&0x000000ff);

	cStream.write(reinterpret_cast<char*>(aTemp), 4);
}


