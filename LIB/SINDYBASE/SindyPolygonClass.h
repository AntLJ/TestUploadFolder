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

// SindyPolygonClass.h: SindyPolygonClass クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYPOLYGONCLASS_H__0F5EC6FC_97AE_4BAF_80CA_818368A0DAE9__INCLUDED_)
#define AFX_SINDYPOLYGONCLASS_H__0F5EC6FC_97AE_4BAF_80CA_818368A0DAE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyFeatureClass.h"

class SindyPolygonClass : public SindyFeatureClass  
{
public:
	SindyPolygonClass(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyPolygonClass();

	virtual HRESULT LoadByPolygon(IPolygonPtr ipPolygon);
	virtual HRESULT SearchByGeometry(IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor);
	virtual HRESULT SearchPrefPoly(IGeometryPtr ipGeometry, const std::string &cKeyField, IFeature **ipFeature);
};

#endif // !defined(AFX_SINDYPOLYGONCLASS_H__0F5EC6FC_97AE_4BAF_80CA_818368A0DAE9__INCLUDED_)
