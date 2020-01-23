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

// SindyPointClass.h: SindyPointClass クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYPOINTCLASS_H__144652B3_3D96_4641_95D7_BE3AED5C757F__INCLUDED_)
#define AFX_SINDYPOINTCLASS_H__144652B3_3D96_4641_95D7_BE3AED5C757F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyFeatureClass.h"

struct lessPoint : public std::binary_function<IPointPtr,IPointPtr,bool>
{
	bool operator ()(IPointPtr ipPoint1, IPointPtr ipPoint2) const
	{
		HRESULT hr;
		const char *func = "lessPoint::operator()";

		double aX1 = 0.0, aX2 = 0.0, aY1 = 0.0, aY2 = 0.0;
		if (FAILED(hr = ipPoint1->get_X(&aX1)))
			(void)GDBERROR(hr, func);
		if (FAILED(hr = ipPoint1->get_Y(&aY1)))
			(void)GDBERROR(hr, func);
		if (FAILED(hr = ipPoint2->get_X(&aX2)))
			(void)GDBERROR(hr, func);
		if (FAILED(hr = ipPoint2->get_Y(&aY2)))
			(void)GDBERROR(hr, func);

		if (aX1 != aX2) return aX1 < aX2;
		return aY1 < aY2;
	}
};

typedef std::multimap<IPointPtr,IFeaturePtr,lessPoint>	GDBPNTLIST;
typedef GDBPNTLIST::iterator							GDBPNTITER;
typedef std::pair<GDBPNTITER,GDBPNTITER>				GDBPNTRANGE;
typedef std::pair<IPointPtr,IFeaturePtr>				GDBPNTSET;

class SindyPointClass : public SindyFeatureClass  
{
public:
	SindyPointClass(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyPointClass();

	virtual HRESULT LoadByPolygon(IPolygonPtr ipPolygon);
	virtual HRESULT SearchByPoint(IPointPtr ipPoint, GDBPNTRANGE *cRange);

protected:
	GDBPNTLIST mPointList;
};

#endif // !defined(AFX_SINDYPOINTCLASS_H__144652B3_3D96_4641_95D7_BE3AED5C757F__INCLUDED_)
