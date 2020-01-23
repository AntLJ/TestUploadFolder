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

#include "stdafx.h"
#include "LayerBase.h"

bool CRecodeBase::SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index)
{
	if(!SetData(strTableName, _IRowPtr(ipFeature), mapFieldIndex2Name, mapFieldName2Index, mapAttrName2Index))
		return false;

	// 形状取得はIFeaturePtrのときだけ
	IGeometryPtr ipGeom;
	if(!GetGeometry(strTableName, ipFeature, ipGeom))
		return false;
	if(!GetLonLatData(strTableName, ipGeom, eVecLonLat))
		return false;

	return true;
}

bool CRecodeBase::SetData(const CString& strTableName, _IRowPtr ipRow, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index, const CString& strOIDFieldName, const CString& strDivIDFieldName)
{
	// 各種属性値を確保
	for(const auto fieldindex : mapFieldIndex2Name){
		if(mapAttrName2Index.cend() == mapAttrName2Index.find(fieldindex.second))
			continue;
		CComVariant vaValue;
		if(!GetValue(strTableName, fieldindex.second, ipRow, fieldindex.first, vaValue))
			return false;
		eVecAttr.push_back(vaValue);
	}

	// ユニークキーをセット
	CComVariant vaOID;
	if(mapFieldName2Index.cend() != mapFieldName2Index.find(shp05::schema::kObjectId)){
		if(!GetValue(strTableName, shp05::schema::kObjectId, ipRow, mapFieldName2Index.at(shp05::schema::kObjectId), vaOID))
			return false;
		eObjectID.eID = vaOID.lVal;
	} else if(!strOIDFieldName.IsEmpty()){
		if(!GetValue(strTableName, shp05::schema::kObjectId, ipRow, mapFieldName2Index.at(strOIDFieldName), vaOID))
			return false;
		eObjectID.eID = vaOID.lVal;
	}

	if(mapFieldName2Index.cend() != mapFieldName2Index.find(shp05::schema::road::road_link_a::kDivId)){
		CComVariant vaDivID;
		if(!GetValue(strTableName, shp05::schema::road::road_link_a::kDivId, ipRow, mapFieldName2Index.at(shp05::schema::road::road_link_a::kDivId), vaDivID))
			return false;
		eObjectID.eDivID = vaDivID.lVal;
	} else if(!strDivIDFieldName.IsEmpty()){
		CComVariant vaDivID;
		if(!GetValue(strTableName, shp05::schema::road::road_link_a::kDivId, ipRow, mapFieldName2Index.at(strDivIDFieldName), vaDivID))
			return false;
		eObjectID.eDivID = vaDivID.lVal;
	}

	return true;
}

bool CTableBase::Init(bool bBefore, IWorkspacePtr ipWork, const CString& strTableName)
{
	m_bBefore = bBefore;

	if(!GetFeatureClass(strTableName, ipWork, m_ipFeatureClass))
		return false;

	// ログ出力用にbefore,afterの接頭文字を付ける
	if(m_bBefore)
		m_strTableName = _T("before_") + strTableName;
	else
		m_strTableName = _T("after_") + strTableName;

	if(!SetFieldInfo(strTableName, ITablePtr(m_ipFeatureClass)))
		return false;

	return true;
}

bool CTableBase::SetFieldInfo(const CString& strTableName, ITablePtr ipTable)
{
	IFieldsPtr ipFields;
	if(!GetFields(strTableName, ipTable, ipFields))
		return false;

	long lCount = 0;
	if(!GetFieldCount(strTableName, ipFields, lCount))
		return false;

	long n = 0;
	for(long l = 0; l < lCount; ++l){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		CString strName = ipField->GetName().GetBSTR();

		m_mapFieldIndex2Name.insert(std::make_pair(l, strName));
		m_mapFieldName2Index.insert(std::make_pair(strName, l));

		// 形状、ShapeのFID,OIDの場合はスルー
		if(0 == strName.CompareNoCase(shp05::schema::kShape) ||
			0 == strName.CompareNoCase(_T("FID")) ||
			0 == strName.CompareNoCase(_T("OID")) ||
			0 == strName.CompareNoCase(shp05::schema::kObjectId) ||
			0 == strName.CompareNoCase(shp05::schema::road::road_walk_link_a::kDivId) ||
			0 == strName.CompareNoCase(shp05::schema::road::road_walk_link_a::kFromNodeId) ||
			0 == strName.CompareNoCase(shp05::schema::road::road_walk_link_a::kFromDivId) ||
			0 == strName.CompareNoCase(shp05::schema::road::road_walk_link_a::kToNodeId) ||
			0 == strName.CompareNoCase(shp05::schema::road::road_walk_link_a::kToDivId))
			continue;

		m_mapAttrIndex2Name.insert(std::make_pair(n, strName));
		m_mapAttrName2Index.insert(std::make_pair(strName, n));

		n++;
	}

	return true;
}
