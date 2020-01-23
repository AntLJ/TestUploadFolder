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

// SindyHighwayFacil.h: SindyHighwayFacil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYHIGHWAYFACIL_H__57FEC406_11AA_4D22_921C_4F0411AA5250__INCLUDED_)
#define AFX_SINDYHIGHWAYFACIL_H__57FEC406_11AA_4D22_921C_4F0411AA5250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyDBConnection.h"

class SindyHighwayFacil  
{
public:
	long eRoadCode;
	long eRoadSeq;

public:
	SindyHighwayFacil();
	SindyHighwayFacil(long cRoadCode, long cRoadSeq);
	virtual ~SindyHighwayFacil();

	bool operator ==(const SindyHighwayFacil &cFacil);
	HRESULT SetFacil(_IRowPtr ipRow);
};

struct lessHwyFacil : public std::binary_function<SindyHighwayFacil,SindyHighwayFacil,bool>
{
	bool operator ()(const SindyHighwayFacil &cFacil1, const SindyHighwayFacil &cFacil2) const
	{
		return (cFacil1.eRoadCode != cFacil2.eRoadCode) ? cFacil1.eRoadCode < cFacil2.eRoadCode : cFacil1.eRoadSeq < cFacil2.eRoadSeq;
	}
};

typedef std::multimap<SindyHighwayFacil,_IRowPtr,lessHwyFacil>	GDBHWYFACILLIST;
typedef GDBHWYFACILLIST::iterator								GDBHWYFACILITER;
typedef std::pair<GDBHWYFACILITER,GDBHWYFACILITER>				GDBHWYFACILRANGE;
typedef std::pair<SindyHighwayFacil,_IRowPtr>					GDBHWYFACILSET;

#endif // !defined(AFX_SINDYHIGHWAYFACIL_H__57FEC406_11AA_4D22_921C_4F0411AA5250__INCLUDED_)
