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

// SindyRoadLink.h: SindyRoadLink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYROADLINK_H__A77D5DB5_8BAF_4C5A_A1A1_403A58F22CAE__INCLUDED_)
#define AFX_SINDYROADLINK_H__A77D5DB5_8BAF_4C5A_A1A1_403A58F22CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyFeatureClass.h"

class SindyRoadLink : public SindyFeatureClass  
{
public:
	SindyRoadLink(SindyDBConnection *cDBConnection);
	SindyRoadLink(SindyDBConnection *cDBConnection, const std::string& subFields);
	virtual ~SindyRoadLink();

	virtual HRESULT LoadByPolygon(IPolygonPtr ipPolygon);
	virtual HRESULT SearchByNodeID(long cNodeID, long cRelation, GDBOBJLIST *cLinkList);
	virtual HRESULT SearchLink(long cFromNodeID, long cToNodeID, IFeature **ipLink);
	virtual HRESULT SearchByGeometry(IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor);
	/**
	 * @brief SubFieldsを取得
	 * @retval SubFieldsの文字列
	 */
	const std::string& GetSubFields() const { return mSubFields; }
	/**
	 * @brief SubFieldsを設定する
	 */
	void SetSubFields(const std::string& strSubFields) { mSubFields = strSubFields; }

protected:
	GDB2OBJLIST mFrom;
	GDB2OBJLIST mTo;

	std::string mSubFields; //!< ROAD_LINKに必要なフィールドを定義すること(defaultは設定なし)
};

#endif // !defined(AFX_SINDYROADLINK_H__A77D5DB5_8BAF_4C5A_A1A1_403A58F22CAE__INCLUDED_)
