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

// SindyLineClass.h: SindyLineClass クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLINECLASS_H__3375EB9C_3A9D_43B2_A5DB_0C7B9A1016B6__INCLUDED_)
#define AFX_SINDYLINECLASS_H__3375EB9C_3A9D_43B2_A5DB_0C7B9A1016B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyFeatureClass.h"

struct SindyLine
{
	IPointPtr ipFrom;
	IPointPtr ipTo;
	SindyLine() : ipFrom(NULL), ipTo(NULL) {}
	SindyLine(IPointPtr ipPoint1, IPointPtr ipPoint2) : ipFrom(ipPoint1), ipTo(ipPoint2) {}
	virtual ~SindyLine() {}
};

struct lessLine : public std::binary_function<SindyLine,SindyLine,bool>
{
	bool operator ()(const SindyLine &cLine1, const SindyLine &cLine2) const
	{
		HRESULT hr;
		const char *func = "lessLine::operator()";

		double hX1 = 0.0, hY1 = 0.0, hX2 = 0.0, hY2 = 0.0, tX1 = 0.0, tY1 = 0.0, tX2 = 0.0, tY2 = 0.0;

		if (FAILED(hr = cLine1.ipFrom->get_X(&hX1))) (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine1.ipFrom->get_Y(&hY1))) (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine1.ipTo->get_X(&tX1)))   (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine1.ipTo->get_Y(&tY1)))   (void)GDBERROR(hr, func);

		if (FAILED(hr = cLine2.ipFrom->get_X(&hX2))) (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine2.ipFrom->get_Y(&hY2))) (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine2.ipTo->get_X(&tX2)))   (void)GDBERROR(hr, func);
		if (FAILED(hr = cLine2.ipTo->get_Y(&tY2)))   (void)GDBERROR(hr, func);

		double cX1 = (hX1 + tX1) / 2.0;
		double cY1 = (hY1 + tY1) / 2.0;
		double cX2 = (hX2 + tX2) / 2.0;
		double cY2 = (hY1 + tY2) / 2.0;

		if (cX1 != cX2) return cX1 < cX2;
		if (cY1 != cY2) return cY1 < cY2;
		if (hX1 != hX2) return hX1 < hX2;
		if (hY1 != hY2) return hY1 < hY2;
		if (tX1 != tX2) return tX1 < tX2;
		return tY1 < tY2;
	}
};

typedef std::multimap<SindyLine,IFeaturePtr,lessLine>	GDBLINELIST;
typedef GDBLINELIST::iterator							GDBLINEITER;
typedef std::pair<GDBLINEITER,GDBLINEITER>				GDBLINERANGE;
typedef std::pair<SindyLine,IFeaturePtr>				GDBLINESET;

class SindyLineClass : public SindyFeatureClass  
{
public:
	SindyLineClass(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyLineClass();

	virtual HRESULT LoadByPolygon(IPolygonPtr ipPolygon);
	virtual HRESULT SearchByLine(IPointPtr ipFrom, IPointPtr ipTo, GDBLINERANGE *cRange);

protected:
	GDBLINELIST mLineList;
};

#endif // !defined(AFX_SINDYLINECLASS_H__3375EB9C_3A9D_43B2_A5DB_0C7B9A1016B6__INCLUDED_)
