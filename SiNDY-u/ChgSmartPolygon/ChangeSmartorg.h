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

typedef std::list<CAdapt<IPointPtr>> LIST_POINTS;
typedef std::list<LIST_POINTS> LIST_LOOP;

class CChangeSmart
{
public:
	CChangeSmart(void);
	~CChangeSmart(void);

	bool ExecSmart(IFeatureClassPtr ipFeatureClass);
	bool ToSmart(IFeatureClassPtr ipFeatureClass, std::map<long, long>& mapFeatureChk);

	void SetOffset(double dOffset) { m_dAllowOffset = dOffset; };
	IPolygonPtr GetMergePolygon(LIST_POINTS& listOuter, LIST_LOOP& listInner, bool bOuter, IPathPtr ipPath);
	bool GetLoop(IPolygonPtr ipPolygon, IPointPtr ipFromPoint, LIST_POINTS& listOuter, LIST_LOOP& listInner, bool& bOuter);
	bool SortTarget(LIST_POINTS& listPoints, IPointPtr ipPoint);
	IPolygonPtr CreatePolygon(LIST_POINTS& listOuter, LIST_LOOP& listInner);
	void ChangeLoop(LIST_POINTS& listPoints, IPathPtr ipPath);

private:
	double m_dAllowOffset;
};
