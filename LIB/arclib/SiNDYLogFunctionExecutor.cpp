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

// SiNDYLogFunctionExecutor.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <crd_cnv/crd_cnv.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include "SiNDYLogFunctionExecutor.h"
#include "SiNDYTableFinder.h"
#include "SiNDYPlant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

CString MeshXYList::getString() const
{
	CString strResult;
	for(const_iterator it = begin(); it != end(); ++it) {
		const LogFE::MeshXY& rMeshXY = *it;

		if(! strResult.IsEmpty()) {
			strResult += _T(';');
		}

		CString strMeshXY;
		strMeshXY.Format(_T("%u(%d,%d)"), rMeshXY.m_nMeshcode, rMeshXY.m_nX, rMeshXY.m_nY);
		strResult += strMeshXY;
	}

	if(strResult.IsEmpty()) {
		strResult = _T("<empty>");
	}

	return strResult;
}

Executor::NotImplementException::NotImplementException(LPCTSTR lpszKey, LPCTSTR lpszInput)
{
	CString strReason;
	strReason.Format(_T("! 関数\"%s\"に\"%s\"は入力できない。"), lpszKey, lpszInput);
	setReason(strReason);
}

Executor::OverflowException::OverflowException(const TableName& rTableName, int nCount)
{
	CString strReason;
	strReason.Format(_T("! \"%s\"検索時に、オブジェクト集合が%d件に達した。"), rTableName.fullNameString(), nCount);
	setReason(strReason);
}

Executor::LayerNotFoundException::LayerNotFoundException(const TableName& rTableName)
{
	CString strReason;
	strReason.Format(_T("! \"%s\"を開けない。"), rTableName.fullNameString());
	setReason(strReason);
}

Executor::FieldNotFoundException::FieldNotFoundException(const CString& rFieldName)
{
	CString strReason;
	strReason.Format(_T("! 属性フィールドフィールド\"%s\"が存在しない"), rFieldName);
	setReason(strReason);
}

Executor::CantCalcException::CantCalcException(LPCTSTR lpszFieldName)
{
	CString strReason;
	if(lpszFieldName) {
		strReason.Format(_T("! \"%s\"に対して指定の計算を行うことが出来ない"), lpszFieldName);
	}
	else {
		strReason.Format(_T("! 指定の計算を行うことが出来ない"));
	}
	setReason(strReason);
}

Executor::SQLException::SQLException()
{
	CComBSTR	bstrError;

	IErrorInfoPtr ipError;
	::GetErrorInfo(0, &ipError);
	if(ipError != 0) {
		ipError->GetDescription(&bstrError);
	}

	setReason(CString(bstrError));
}

// Executor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

Executor::Executor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, std::set<int>* pUpdatedColSet) :
m_rTableFinder(rTableFinder),
m_rLogRow(rLogRow),
m_pUpdatedColSet(pUpdatedColSet)
{
}

ITablePtr Executor::getTable(const TableName& rTableName)
{
	ITablePtr ipTable = m_rTableFinder.getTable(rTableName.fullNameString());
	if(ipTable == 0)
		throw LayerNotFoundException(rTableName);

	return ipTable;
}

void Executor::update(const FieldValues& rFieldValues)
{
	update(static_cast<LPCTSTR>(rFieldValues.getValuesString()));
}

void Executor::update(const MeshXYList& rMeshXYList)
{
	update(static_cast<LPCTSTR>(rMeshXYList.getString()));
}

void Executor::chain(IFieldPtr ipField, const CComVariant& va)
{
	FieldValues cValues(ipField);
	cValues.push_back(va);
	chain(cValues);
}

void Executor::update(LPCTSTR lpszValue)
{
	logRow().updateCellsString(_function().columnSet(), lpszValue, updatedColSet());
}

// FieldExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool FieldExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	IFieldsPtr ipFields;
	if(ipRow->get_Fields(&ipFields) != S_OK)
		return false;

	long nFieldIndex;
	if(ipFields->FindField(CComBSTR(function().fieldName()), &nFieldIndex) != S_OK || nFieldIndex < 0)
		throw FieldNotFoundException(function().fieldName());

	IFieldPtr ipField;
	if(ipFields->get_Field(nFieldIndex, &ipField) != S_OK)
		return false;

	FieldValues cFieldValues(ipField);
	getFieldValues(ipRow, nFieldIndex, cFieldValues);
	chain(cFieldValues);

	return true;
}

bool FieldExecutor::execute(const RowMap& rRowMap)
{
	if(rRowMap.empty())
		return false;

	RowMap::const_iterator it = rRowMap.begin();
	if(it == rRowMap.end())
		return false;

	IFieldsPtr ipFields;
	if(it->second->get_Fields(&ipFields) != S_OK)
		return false;

	long nFieldIndex;
	if(ipFields->FindField(CComBSTR(function().fieldName()), &nFieldIndex) != S_OK || nFieldIndex < 0)
		throw FieldNotFoundException(function().fieldName());

	IFieldPtr ipField;
	if(ipFields->get_Field(nFieldIndex, &ipField) != S_OK)
		return false;

	FieldValues cFieldValues(ipField);
	getFieldValues(rRowMap, nFieldIndex, cFieldValues);
	chain(cFieldValues);

	return true;
}

// 固有処理

void _FieldExecutor::getFieldValues(_IRow* ipRow, long nFieldIndex, FieldValues& rFieldValues)
{
	if(! ipRow)
		return;

	CComVariant va;
	ipRow->get_Value(nFieldIndex, &va);

	rFieldValues.push_back(va);
}

void _FieldExecutor::getFieldValues(const RowMap& rRowMap, long nFieldIndex, FieldValues& rFieldValues)
{
	if(rRowMap.empty())
		return;

	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		getFieldValues(it->second, nFieldIndex, rFieldValues);
	}
}

// SubFieldExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool SubFieldExecutor::execute(const RowMapWithFieldMap& rRowMap)
{
	if(rRowMap.empty())
		return false;

	std::map<CString, IFieldPtr>::const_iterator it = rRowMap.m_FieldMap.find(function().fieldName());
	if(it == rRowMap.m_FieldMap.end() || it->second == 0)  
		throw FieldNotFoundException(function().fieldName());

	FieldValues cFieldValues(it->second);
	getFieldValues(rRowMap, std::distance(rRowMap.m_FieldMap.begin(), it), cFieldValues);
	chain(cFieldValues);

	return true;
}

// PointExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool PointExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	IPointPtr ipPoint = getPoint(IFeaturePtr(ipRow));
	if(ipPoint == 0)
		throw CantCalcException(getKey());

	chain(0, static_cast<IUnknown*>(ipPoint));

	return true;
}

bool PointExecutor::execute(const RowMap& rRowMap)
{
	FieldValues cValues;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		IPointPtr ipPoint = getPoint(IFeaturePtr(it->second));
		if(ipPoint == 0)
			throw CantCalcException(getKey());

		cValues.push_back(static_cast<IUnknown*>(ipPoint));
	}

	chain(cValues);

	return true;
}

bool PointExecutor::execute(const FieldValues& rFieldValues)
{
	FieldValues cValues;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		IPointCollectionPtr ipPointCol;
		const CComVariant& va = *it;
		if(va.vt != VT_UNKNOWN || ((ipPointCol = va.punkVal) == 0))
			throw NotImplementException(getKey(), _T("点列形状以外の属性値"));

		IPointPtr ipPoint = getPoint(ipPointCol);
		if(ipPoint == 0)
			throw CantCalcException(getKey());

		cValues.push_back(static_cast<IUnknown*>(ipPoint));
	}

	chain(cValues);

	return true;
}

IPointPtr PointExecutor::getPoint(IFeature* ipFeature)
{
	if(! ipFeature)
		return 0;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);
	return getPoint(IPointCollectionPtr(ipGeometry));
}

IPointPtr PointExecutor::getPoint(IPointCollection* ipPointCol)
{
	if(! ipPointCol)
		return 0;

	return function().getPoint(ipPointCol);
}

// BufferExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool BufferExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	IGeometryPtr ipGeometry(buffer(IFeaturePtr(ipRow)));
	if(ipGeometry == 0)
		throw CantCalcException(getKey());

	chain(0, static_cast<IUnknown*>(ipGeometry));

	return true;
}

bool BufferExecutor::execute(const RowMap& rRowMap)
{
	FieldValues cValues;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		IGeometryPtr ipGeometry(buffer(IFeaturePtr(it->second)));
		if(ipGeometry == 0)
			throw CantCalcException(getKey());

		cValues.push_back(static_cast<IUnknown*>(ipGeometry));
	}

	chain(cValues);

	return true;
}

bool BufferExecutor::execute(const FieldValues& rFieldValues)
{
	FieldValues cValues;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		IGeometryPtr ipGeometry;
		if(it->vt != VT_UNKNOWN || ((ipGeometry = it->punkVal) == 0))
			throw NotImplementException(getKey(), _T("形状以外の属性値"));

		ipGeometry = buffer(ipGeometry);
		if(ipGeometry == 0)
			throw CantCalcException(getKey());

		cValues.push_back(static_cast<IUnknown*>(ipGeometry));
	}

	chain(cValues);

	return true;
}

IGeometryPtr BufferExecutor::buffer(IFeature* ipFeature)
{
	if(! ipFeature)
		return 0;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);
	return buffer(ipGeometry);
}

IGeometryPtr BufferExecutor::buffer(IGeometry* ipGeometry)
{
	return ipGeometry ? function().buffer(ipGeometry) : 0;
}

// LengthExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool LengthExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	IFeaturePtr ipFeature = ipRow;
	if(ipFeature == 0)
		return false;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	chain(0, getLength(IPointCollectionPtr(ipGeometry)));

	return true;
}

bool LengthExecutor::execute(const RowMap& rRowMap)
{
	FieldValues cValues;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		cValues.push_back(getLength(IFeaturePtr(it->second)));
	}

	chain(cValues);

	return true;
}

bool LengthExecutor::execute(const FieldValues& rFieldValues)
{
	FieldValues cValues;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		const CComVariant& va = *it;
		if(va.vt != VT_UNKNOWN)
			continue;

		cValues.push_back(getLength(IPointCollectionPtr(va.punkVal)));
	}

	chain(cValues);

	return true;
}

double LengthExecutor::getLength(IFeature* ipFeature)
{
	if(! ipFeature)
		return 0.0;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	return getLength(IPointCollectionPtr(ipGeometry));
}

double LengthExecutor::getLength(IPointCollection* ipPointCol)
{
	if(! ipPointCol)
		return 0.0;

	double dDistance = 0.0;
	long nCount;
	ipPointCol->get_PointCount(&nCount);
	for(int i = 0; i < nCount - 1; i++) {
		IPointPtr ipPoint1, ipPoint2;
		ipPointCol->get_Point(i, &ipPoint1);
		ipPointCol->get_Point(i + 1, &ipPoint2);

		double dX1, dY1;
		double dX2, dY2;
		ipPoint1->get_X(&dX1);
		ipPoint1->get_Y(&dY1);
		ipPoint2->get_X(&dX2);
		ipPoint2->get_Y(&dY2);

		dDistance += getLength(dX1, dY1, dX2, dY2);
	}

	return dDistance;
}

/**
 * @brief ２点間の距離（メートル）を求める
 *
 * @param dX1 [in] 点１の経度
 * @param dX2 [in] 点１の緯度
 * @param dY1 [in] 点２の経度
 * @param dY2 [in] 点２の緯度
 * @return ２点間の距離（メートル）
 */
double LengthExecutor::getLength(double dX1, double dY1, double dX2, double dY2)
{
	return crd_cnv().GetDist(dX1, dY1, dX2, dY2);
}

// LonLatExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool LonLatExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	FieldValues cValues;
	cValues.push_back(getValue(IFeaturePtr(ipRow)));
	chain(cValues);

	return true;
}

bool LonLatExecutor::execute(const RowMap& rRowMap)
{
	if(rRowMap.empty())
		return false;

	FieldValues cValues;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		cValues.push_back(getValue(IFeaturePtr(it->second)));
	}
	chain(cValues);

	return true;
}

bool LonLatExecutor::execute(const FieldValues& rFieldValues)
{
	if(rFieldValues.empty())
		return false;

	FieldValues cValues;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		cValues.push_back(getValue(*it));
	}
	chain(cValues);

	return true;
}

CComVariant LonLatExecutor::getValue(IFeature* ipFeature)
{
	if(! ipFeature)
		return CComVariant();

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	return getValue(IPointPtr(ipGeometry));
}

CComVariant LonLatExecutor::getValue(const VARIANT& rVariant)
{
	if(rVariant.vt != VT_UNKNOWN)
		return CComVariant();

	return getValue(IPointPtr(rVariant.punkVal));
}

// LongitudeExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

CComVariant LongitudeExecutor::getValue(IPoint* ipPoint)
{
	if(! ipPoint)
		return CComVariant();

	double dX;
	ipPoint->get_X(&dX);

	return dX;
}

// LatitudeExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

CComVariant LatitudeExecutor::getValue(IPoint* ipPoint)
{
	if(! ipPoint)
		return CComVariant();

	double dY;
	ipPoint->get_Y(&dY);

	return dY;
}

// MeshExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool MeshExecutor::execute(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	MeshXYList cMeshXYList;
	cMeshXYList.push_back(getMeshXY(IFeaturePtr(ipRow)));
	chain(cMeshXYList);

	return true;
}

bool MeshExecutor::execute(const RowMap& rRowMap)
{
	if(rRowMap.empty())
		return false;

	MeshXYList cMeshXYList;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		cMeshXYList.push_back(getMeshXY(IFeaturePtr(it->second)));
	}
	chain(cMeshXYList);

	return true;
}

bool MeshExecutor::execute(const FieldValues& rFieldValues)
{
	if(rFieldValues.empty())
		return false;

	MeshXYList cMeshXYList;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		cMeshXYList.push_back(getMeshXY(*it));
	}
	chain(cMeshXYList);

	return true;
}

LogFE::MeshXY MeshExecutor::getMeshXY(IFeature* ipFeature)
{
	if(! ipFeature)
		throw NotImplementException(getKey(), _T("点形状以外のフィーチャ"));

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	return getMeshXY(IPointPtr(ipGeometry));
}

LogFE::MeshXY MeshExecutor::getMeshXY(const VARIANT& rVariant)
{
	if(rVariant.vt != VT_UNKNOWN)
		throw NotImplementException(getKey(), _T("点形状以外の属性値"));

	return getMeshXY(IPointPtr(rVariant.punkVal));
}

LogFE::MeshXY MeshExecutor::getMeshXY(IPoint* ipPoint)
{
	if(! ipPoint)
		throw NotImplementException(getKey(), _T("点形状以外の形状"));

	double dX;
	double dY;
	ipPoint->get_X(&dX);
	ipPoint->get_Y(&dY);

	return getMeshXY(dX, dY);
}

// BaseMeshExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

/**
 * @brief 経緯度を２次メッシュXY座標（100万座標系）に変換する
 *
 * @param dLongitude [in] 経度
 * @param dLatitude [in] 緯度
 * @return ２次メッシュXY座標
 */
LogFE::MeshXY BaseMeshExecutor::getMeshXY(double dLongitude, double dLatitude)
{
	int x = 0, y = 0;
	unsigned int meshcode = AheLL2MESH( dLongitude, dLatitude, 2, x, y );

	return LogFE::MeshXY(meshcode, x, y);
}

// MeshXYExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool MeshXYExecutor::execute(const MeshXYList& rMeshXYList)
{
	if(rMeshXYList.empty())
		return false;

	FieldValues cFieldValues;
	for(MeshXYList::const_iterator it = rMeshXYList.begin(); it != rMeshXYList.end(); ++it) {
		cFieldValues.push_back(getValue(*it));
	}
	chain(cFieldValues);

	return true;
}

// DistinctExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

// 仮想関数のオーバーライド

bool DistinctExecutor::execute(const FieldValues& rFieldValues)
{
	if(rFieldValues.empty())
		return false;

	std::set<CComVariant, CComVariantLess> cVariantSet;
	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		cVariantSet.insert(*it);
	}

	FieldValues cDistinctedValues(rFieldValues.m_ipField);
	for(std::set<CComVariant, CComVariantLess>::const_iterator it = cVariantSet.begin(); it != cVariantSet.end(); ++it) {
		cDistinctedValues.push_back(*it);
	}

	chain(cDistinctedValues);

	return true;
}

// AvgExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool AvgExecutor::execute(const FieldValues& rFieldValues)
{
	CComVariant va;
	va.vt = VT_EMPTY;

	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		const CComVariant& rVariant = *it;
		if(rVariant.vt == VT_NULL)
			continue;

		if(va.vt == VT_EMPTY) {
			va = rVariant;
			continue;
		}

		switch(rVariant.vt) {
		case VT_I4:
			va.lVal += rVariant.lVal;
			break;
		case VT_R8:
			va.dblVal += rVariant.dblVal;
			break;
		default:
			if(rFieldValues.m_ipField != 0) {
				CComBSTR bstrName;
				rFieldValues.m_ipField->get_Name(&bstrName);
				throw CantCalcException(CString(bstrName));
			}
			else
				throw CantCalcException();
		}
	}

	if(! rFieldValues.empty()) {
		switch(va.vt) {
		case VT_I4:
			va.lVal /= rFieldValues.size();
			break;
		case VT_R8:
			va.dblVal /= rFieldValues.size();
			break;
		}
	}

	chain(rFieldValues.m_ipField, va);

	return true;
}

// MaxExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool MaxExecutor::execute(const FieldValues& rFieldValues)
{
	CComVariant va;
	va.vt = VT_EMPTY;

	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		const CComVariant& rVariant = *it;
		if(rVariant.vt == VT_NULL)
			continue;

		if(va.vt == VT_EMPTY) {
			va = rVariant;
			continue;
		}

		if(CComVariantLess()(va, rVariant)) {
			va = rVariant;
		}
	}

	chain(rFieldValues.m_ipField, va);
	
	return true;
}

// MinExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool MinExecutor::execute(const FieldValues& rFieldValues)
{
	CComVariant va;
	va.vt = VT_EMPTY;

	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		const CComVariant& rVariant = *it;
		if(rVariant.vt == VT_NULL)
			continue;

		if(va.vt == VT_EMPTY) {
			va = rVariant;
			continue;
		}

		if(CComVariantLess()(rVariant, va)) {
			va = rVariant;
		}
	}

	chain(rFieldValues.m_ipField, va);

	return true;
}

// SumExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool SumExecutor::execute(const FieldValues& rFieldValues)
{
	CComVariant va;
	va.vt = VT_EMPTY;

	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		const CComVariant& rVariant = *it;
		if(rVariant.vt == VT_NULL)
			continue;

		if(va.vt == VT_EMPTY) {
			va = rVariant;
			continue;
		}

		switch(rVariant.vt) {
		case VT_I4:
			va.lVal += rVariant.lVal;
			break;
		case VT_R8:
			va.dblVal += rVariant.dblVal;
			break;
		default:
			if(rFieldValues.m_ipField != 0) {
				CComBSTR bstrName;
				rFieldValues.m_ipField->get_Name(&bstrName);
				throw CantCalcException(CString(bstrName));
			}
			else
				throw CantCalcException();
		}
	}

	chain(rFieldValues.m_ipField, va);

	return true;
}

// CountExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

// 仮想関数のオーバーライド

bool CountExecutor::execute(_IRow* ipRow)
{
	chain(0, CComVariant(ipRow ? 1 : 0, VT_I4));
	return true;
}

bool CountExecutor::execute(const RowMap& rRowMap)
{
	chain(0, CComVariant(long(rRowMap.size()), VT_I4));
	return true;
}

bool CountExecutor::execute(const FieldValues& rFieldValues)
{
	chain(0, CComVariant(long(rFieldValues.size()), VT_I4));
	return true;
}

// SelectExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

SelectExecutor::SelectExecutor(SiNDYTableFinder& rTableFinder, LogRowInfo& rLogRow, std::set<int>* pUpdatedColSet) :
Executor(rTableFinder, rLogRow, pUpdatedColSet)
{
}

_ICursorPtr SelectExecutor::getCursor(ITable* ipTable, IQueryFilter* ipQueryFilter)
{
	if(! ipTable)
		return 0;

	long nRowCount;
	if(ipTable->RowCount(ipQueryFilter, &nRowCount) != S_OK)
		return 0;

	if(nRowCount > 100000)
		throw OverflowException(ipTable, nRowCount);

	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor)))
		throw SQLException();

	return ipCursor;
}

IFeatureCursorPtr SelectExecutor::getFeatureCursor(IFeatureClass* ipFeatureClass, IQueryFilter* ipQueryFilter)
{
	if(! ipFeatureClass)
		return 0;

	long nRowCount;
	if(ipFeatureClass->FeatureCount(ipQueryFilter, &nRowCount) != S_OK)
		return 0;

	if(nRowCount > 100000)
		throw OverflowException(TableName(ITablePtr(ipFeatureClass)), nRowCount);

	IFeatureCursorPtr ipFeatureCursor;
	if(FAILED(ipFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)))
		throw SQLException();

	return ipFeatureCursor;
}

void SelectExecutor::getRowMap(ITable* ipTable, BSTR bstrFieldName, const std::set<long> rValueSet, RowMap& rRowMap, BSTR bstrSortFieldName)
{
	for(std::set<long>::const_iterator it = rValueSet.begin(); it != rValueSet.end();) {
		CComBSTR bstrWhereClause;
		bstrWhereClause += bstrFieldName;
		bstrWhereClause += _T(" IN (");
		for(int i = 0; i < 1000 && it != rValueSet.end(); i++, ++it) {
			if(i != 0) {
				bstrWhereClause += _T(",");
			}
			CString strBuffer;
			strBuffer.Format(_T("%d"), *it);
			bstrWhereClause += static_cast<LPCTSTR>(strBuffer);
		}
		bstrWhereClause += _T(")");

		if(! selectFunction().whereClause().IsEmpty() && rRowMap.m_cTableName.isSameTable(selectFunction().tableName())) {
			bstrWhereClause += _T(" AND ");
			bstrWhereClause += static_cast<LPCTSTR>(selectFunction().whereClause());
		}

		getRowMap(ipTable, bstrWhereClause, rRowMap, bstrSortFieldName);
	}
}

void SelectExecutor::getRowMap(ITable* ipTable, BSTR bstrWhereClause, RowMap& rRowMap, BSTR bstrSortFieldName)
{
	if(bstrWhereClause) {
		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		ipQueryFilter->put_WhereClause(bstrWhereClause);
		getRowMap(ipTable, ipQueryFilter, rRowMap, bstrSortFieldName);
	}
	else {
		getRowMap(ipTable, static_cast<IQueryFilter*>(NULL), rRowMap, bstrSortFieldName);
	}
}

void SelectExecutor::getRowMap(ITable* ipTable, IQueryFilter* ipQueryFilter, RowMap& rRowMap, BSTR bstrSortFieldName)
{
	if(! ipTable)
		return;

	_ICursorPtr ipCursor = getCursor(ipTable, ipQueryFilter);
	if(ipCursor == 0)
		return;

	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK) {
		if(bstrSortFieldName) {
			CComVariant va = getValue(ipRow, bstrSortFieldName);
			va.ChangeType(VT_I4);
			rRowMap[va.lVal] = ipRow;
		}
		else {
			long nObjectId;
			ipRow->get_OID(&nObjectId);
			rRowMap[nObjectId] = ipRow;
		}

		if(rRowMap.size() > 100000)
			throw OverflowException(ipTable, rRowMap.size());
	}
}

void SelectExecutor::getRowMap(BSTR bstrTables, BSTR bstrWhereClause, const std::set<CString>& rFieldNameSet, RowMapWithFieldMap& rRowMap)
{
	CComBSTR bstrSubFields(_T("*"));

	if(! rFieldNameSet.empty()) {
		bstrSubFields.Empty();
		CComBSTR bstrCountSubFields;

		for(std::set<CString>::const_iterator it = rFieldNameSet.begin(); it != rFieldNameSet.end(); ++it) {
			const CString& rFieldName = *it;

			if(bstrSubFields.Length() != 0) {
				bstrSubFields += _T(",");
			}

			bstrSubFields += CComBSTR(rFieldName);

			if(rFieldName.Find(_T('(')) == -1 || rFieldName.Find(_T("DISTINCT")) == 0) {
				if(bstrCountSubFields.Length() != 0) {
					bstrCountSubFields += _T(",");
				}
				bstrCountSubFields += _T("COUNT(");
				bstrCountSubFields += CComBSTR(rFieldName);
				bstrCountSubFields += _T(")");
			}
		}

		if(bstrCountSubFields.Length() != 0) {
			_ICursorPtr ipCursor = getCursor(bstrCountSubFields, bstrTables, bstrWhereClause);
			_IRowPtr ipRow;
			if(ipCursor == 0 || ipCursor->NextRow(&ipRow) != S_OK || ipRow == 0)
				return;

			IFieldsPtr ipFields;
			ipRow->get_Fields(&ipFields);

			long nFieldCount;
			ipFields->get_FieldCount(&nFieldCount);
			for(int i = 0; i < nFieldCount; i++) {
				CComVariant va;
				ipRow->get_Value(i, &va);
				if(va.lVal > 100000)
					throw OverflowException(TableName(CString(bstrTables)), va.lVal);
			}
		}
	}

	_ICursorPtr ipCursor = getCursor(bstrSubFields, bstrTables, bstrWhereClause);
	if(ipCursor == 0)
		return;


	IFieldsPtr ipFields;
	ipCursor->get_Fields(&ipFields);
	if(ipFields != 0) {
		IFieldPtr ipField;
		long nIndex = 0;
		for(std::set<CString>::const_iterator it = rFieldNameSet.begin(); it != rFieldNameSet.end(); ++it, nIndex++) {
			ipFields->get_Field(nIndex, &ipField);
			rRowMap.m_FieldMap[*it] = ipField;
		}
	}

	int i = 0;
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK) {
		rRowMap[i++] = ipRow;
	}
}

_ICursorPtr SelectExecutor::getCursor(BSTR bstrSubFields, BSTR bstrTables, BSTR bstrWhereClause)
{
	USES_CONVERSION;

	IFeatureWorkspacePtr ipFeatureWorkspace = tableFinder().getWorkspace(OLE2CT(bstrTables));
	if(ipFeatureWorkspace == 0)
		return 0;

	IQueryDefPtr ipQueryDef;
	ipFeatureWorkspace->CreateQueryDef(&ipQueryDef);

	if(bstrSubFields) {
		ipQueryDef->put_SubFields(bstrSubFields);
	}

	if(bstrTables) {
		ipQueryDef->put_Tables(bstrTables);
	}

	if(bstrWhereClause) {
		ipQueryDef->put_WhereClause(bstrWhereClause);
	}

	_ICursorPtr ipCursor;
	if(FAILED(ipQueryDef->Evaluate(&ipCursor)))
		throw SQLException();

	return ipCursor;
}

// BelongExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

bool BelongExecutor::execute(_IRow* ipRow)
{
	RowMap cResultRowMap(function().tableName());
	belong(ipRow, cResultRowMap);
	chain(cResultRowMap);

	return true;
}

bool BelongExecutor::execute(const RowMap& rRowMap)
{
	RowMap cResultRowMap(function().tableName());
	belong(rRowMap, cResultRowMap);
	chain(cResultRowMap);

	return true;
}

bool BelongExecutor::execute(const FieldValues& rFieldValues)
{
	RowMap cResultRowMap(function().tableName());
	belong(rFieldValues, cResultRowMap);
	chain(cResultRowMap);

	return true;
}

// 固有処理

void BelongExecutor::belong(_IRow* ipRow, RowMap& rResultRowMap)
{
	if(! ipRow)
		return;

	// 同じテーブル間でBELONGすることは不可能
	TableName cTableName(ipRow);
	if(cTableName.isSameTable(rResultRowMap.m_cTableName))
		return;

	// テーブルの種類に応じて処理を振り分け
	if(cTableName.isInfTableName()) {
		belongByInf(cTableName, ipRow, rResultRowMap);
	}
	else if(cTableName.isLqTableName()) {
		belongByLq(cTableName, ipRow, rResultRowMap);
	}
	else if(cTableName.isRoadLinkTableName()) {
		belongByRoadLink(cTableName, IFeaturePtr(ipRow), rResultRowMap);
	}
	else {
		belongByFeature(IFeaturePtr(ipRow), rResultRowMap);
	}
}

void BelongExecutor::belong(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	if(rRowMap.empty())
		return;

	// 同じテーブル間でBELONGすることは不可能
	if(rRowMap.m_cTableName.isSameTable(rResultRowMap.m_cTableName))
		return;

	// テーブルの種類に応じて処理を振り分け
	if(rRowMap.m_cTableName.isInfTableName()) {
		belongByInf(rRowMap, rResultRowMap);
	}
	else if(rRowMap.m_cTableName.isLqTableName()) {
		belongByLq(rRowMap, rResultRowMap);
	}
	else if(rRowMap.m_cTableName.isRoadLinkTableName()) {
		belongByRoadLink(rRowMap, rResultRowMap);
	}
	else {
		belongByFeature(rRowMap, rResultRowMap);
	}
}

void BelongExecutor::belong(const FieldValues& rFieldValues, RowMap& rResultRowMap)
{
	if(rFieldValues.empty())
		return;

	for(FieldValues::const_iterator it = rFieldValues.begin(); it != rFieldValues.end(); ++it) {
		IGeometryPtr ipGeometry;
		const CComVariant& va = *it;
		if(va.vt != VT_UNKNOWN || ((ipGeometry = va.punkVal) == 0))
			throw NotImplementException(getKey(), _T("形状以外の属性値"));

		belongByGeometry(ipGeometry, rResultRowMap);
	}
}

void BelongExecutor::belongByInf(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// InfからLqを取得
	RowMap cLqRowMap(static_cast<LPCTSTR>(rRowMap.m_cTableName.getLqFullNameString()));
	getLqByInf(rRowMap, cLqRowMap);

	// Lqを求めようとしていた場合、ここで終了
	if(rResultRowMap.insert(cLqRowMap))
		return;

	// Lqから...
	belongByLq(cLqRowMap, rResultRowMap);
}

void BelongExecutor::belongByLq(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// LqからInfを取得
	TableName cInfTableName(rRowMap.m_cTableName.getInfFullNameString());
	if(cInfTableName.isSameTable(rResultRowMap.m_cTableName)) {
		getInfByLq(rRowMap, rResultRowMap);
		return;
	}

	// Lqから道路リンクを取得
	RowMap cLinkMap(schema::road_link::kTableName);
	getRoadLinkByLq(rRowMap, cLinkMap);

	// 道路リンクを求めていた場合、ここで終了
	if(rResultRowMap.insert(cLinkMap))
		return;

	// 道路リンクから...
	belongByRoadLink(cLinkMap, rResultRowMap);
}

void BelongExecutor::belongByRoadLink(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// InfかLqを求めようとしていた場合、Lqを求めて、適切な関数に処理を委託する
	if(rResultRowMap.m_cTableName.isLqTableName() || rResultRowMap.m_cTableName.isInfTableName()) {
		RowMap cLqRowMap(TableName(rResultRowMap.m_cTableName.getLqFullNameString()));
		getLqByRoadLink(rRowMap, cLqRowMap);
		if(rResultRowMap.insert(cLqRowMap))
			return;

		belongByLq(cLqRowMap, rResultRowMap);
		return;
	}

	// 道路リンク各々に対し処理
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		belongByRoadLink(rRowMap.m_cTableName, IFeaturePtr(it->second), rResultRowMap);
	}
}

void BelongExecutor::belongByFeature(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// フィーチャ各々に対し処理
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		belongByFeature(IFeaturePtr(it->second), rResultRowMap);
	}
}

void BelongExecutor::belongByInf(const TableName& rInfTableName, _IRow* ipInfRow, RowMap& rResultRowMap)
{
	if(! ipInfRow)
		return;

	// InfからLqを取得
	RowMap cLqRowMap(TableName(rInfTableName.getLqFullNameString()));
	getLqByInf(ipInfRow, cLqRowMap);

	// Lqを求めていた場合、ここで終了
	if(rResultRowMap.insert(cLqRowMap))
		return;

	using namespace sindy::schema;

	// 道路リンク取得
	CComBSTR bstrLinkId(link_queue::kLinkID);

	std::map<long, long> cLinkIdSequence;
	for(RowMap::const_iterator it = cLqRowMap.begin(); it != cLqRowMap.end(); ++it) {
		CComVariant va = getValue(it->second, bstrLinkId);
		cLinkIdSequence[va.lVal] = it->first;
	}

	RowMap cLinkMap(TableName(cLqRowMap.m_cTableName.complementOwnerName(road_link::kTableName)));
	getRoadLinkByLinkIdSequence(cLinkIdSequence, cLinkMap);

	// 道路リンクを求めていた場合、ここで終了
	if(rResultRowMap.insert(cLinkMap))
		return;

	// 道路リンクの形状をくっつける
	ITopologicalOperatorPtr ipTopoOp;
	for(RowMap::const_iterator it = cLinkMap.begin(); it != cLinkMap.end(); ++it) {
		IFeaturePtr ipLink = it->second;
		if(ipLink == 0)
			continue;

		IGeometryPtr ipGeom;
		if(ipLink->get_ShapeCopy(&ipGeom) != S_OK)
			break;

		if(ipTopoOp == 0) {
			ipTopoOp = ipGeom;
		}
		else {
			IGeometryPtr ipUnionedGeom;
			if(ipTopoOp->Union(ipGeom, &ipUnionedGeom) != S_OK || ipUnionedGeom == 0)
				break;

			ipTopoOp = ipUnionedGeom;
		}
	}

	// 形状から再選択を行う
	belongByGeometry(IGeometryPtr(ipTopoOp), rResultRowMap);
}


void BelongExecutor::belongByLq(const TableName& rLqTableName, _IRow* ipRow, RowMap& rResultRowMap)
{
	if(! ipRow)
		return;

	// LqからInfを取得
	TableName cInfTableName(rLqTableName.getInfFullNameString());
	if(cInfTableName.isSameTable(rResultRowMap.m_cTableName)) {
		getInfByLq(ipRow, rResultRowMap);
		return;
	}

	// Lqから道路リンクを取得
	RowMap cLinkMap(TableName(rLqTableName.complementOwnerName(schema::road_link::kTableName)));
	getRoadLinkByLq(ipRow, cLinkMap);

	// 道路リンクを求めようとしていた場合、ここで終了
	if(! rResultRowMap.insert(cLinkMap))
		return;

	belongByFeature(IFeaturePtr(ipRow), rResultRowMap);
}

#pragma warning(push)
#pragma warning(disable : 4100)
void BelongExecutor::belongByRoadLink(const TableName& rLinkTableName, IFeature* ipFeature, RowMap& rResultRowMap)
{
	if(! ipFeature)
		return;

	// InfかLqを求めようとしていた場合、Lqを求めて、適切な関数に処理を委託する
	if(rResultRowMap.m_cTableName.isLqTableName() || rResultRowMap.m_cTableName.isInfTableName()) {
		RowMap cLqRowMap(TableName(rResultRowMap.m_cTableName.getLqFullNameString()));
		getLqByRoadLink(ipFeature, cLqRowMap);
		if(rResultRowMap.insert(cLqRowMap))
			return;

		belongByLq(cLqRowMap, rResultRowMap);
		return;
	}

	// フィーチャ共通処理へ
	belongByFeature(ipFeature, rResultRowMap);
}
#pragma warning(pop)

void BelongExecutor::belongByFeature(IFeature* ipFeature, RowMap& rResultRowMap)
{
	if(! ipFeature)
		return;

	IGeometryPtr ipGeometry;
	if(ipFeature->get_Shape(&ipGeometry) != S_OK || ipGeometry == 0)
		return;

	belongByGeometry(ipGeometry, rResultRowMap);
}

void BelongExecutor::belongByGeometry(IGeometry* ipGeometry1, RowMap& rResultRowMap)
{
	if(! ipGeometry1)
		return;

	// 形状１の形状種取得
	esriGeometryType geoType1;
	ipGeometry1->get_GeometryType(&geoType1);
	bool bGeoType1IsPoint = (geoType1 == esriGeometryPoint || geoType1 == esriGeometryMultipoint);

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName);
	if(ipTable == 0)
		return;

	using namespace sindy::schema;

	// 形状に対するBELONGは、フィーチャクラスに対してのみ意味を持つ
	IFeatureClassPtr ipFeatureClass = ipTable;
	bool bDelegate = false;
	CString strDelegateWhereClause;
	if(ipFeatureClass == 0) {
		// ただし、InfやLqに関しては、道路リンクをその代役とすることができる
		if(rResultRowMap.m_cTableName.isInfTableName() || rResultRowMap.m_cTableName.isLqTableName()) {
			ipFeatureClass = getTable(rResultRowMap.m_cTableName.complementOwnerName(road_link::kTableName));
			if(ipFeatureClass == 0)
				return;
			strDelegateWhereClause.Format(_T("%s=1"), rResultRowMap.m_cTableName.getLqRfFieldNameString());
			bDelegate = true;
		}
		else
			return;
	}

	// 形状２の形状種取得
	esriGeometryType geoType2;
	ipFeatureClass->get_ShapeType(&geoType2);
	bool bGeoType2IsPoint = (geoType2 == esriGeometryPoint || geoType2 == esriGeometryMultipoint);

	// どちらも点形状か確認
	bool bEachGeoTypeIsPoint = (bGeoType1IsPoint && bGeoType2IsPoint);

	// 空間フィルタ用意
	ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
	ipSpFilter->put_SpatialRel(esriSpatialRelRelation);
	ipSpFilter->putref_Geometry(ipGeometry1);

	if(! function().whereClause().IsEmpty()) {
		if(strDelegateWhereClause.IsEmpty()) {
			ipSpFilter->put_WhereClause(CComBSTR(function().whereClause()));
		}
		else {
			strDelegateWhereClause.Format(_T("(%s) AND (%s)"), strDelegateWhereClause, function().whereClause());
			ipSpFilter->put_WhereClause(CComBSTR(strDelegateWhereClause));
		}
	}

	// 関係式
	static const CComBSTR bstrPointRelDescription[] = { _T("T********"), _T("") };
	static const CComBSTR bstRelDescription[] = { _T("*T*******"), _T("***T*****"), _T("**F******"), _T("******F**"), _T("") };

	// 点形状同士の場合は「内側と内側」判定。
	// それ以外の場合は、「どちらかの境界がもう一方の内側に存在する」か、「どちらかの内側が、もう一方の外側に存在しない」かを判定。
	// データが正常であれば、以下のものを「所属する」と判定するはずである。
	// ・指定したメッシュ内に存在する形状
	// ・指定したメッシュ境界に存在する点形状
	// ・指定した形状が存在しているメッシュ
	// ・指定したリンク上に存在するリンク内属性（傾斜）
	// ・指定したリンク内属性（傾斜）が存在するリンク
	// ・指定したリンクに接続するノード
	// ・指定したノードに接続するリンク
	const CComBSTR* pRelDescription = (bEachGeoTypeIsPoint ? bstrPointRelDescription : bstRelDescription);

	RowMap cFeatureMap(bDelegate ? rResultRowMap.m_cTableName.complementOwnerName(road_link::kTableName) : rResultRowMap.m_cTableName);
	for(int i = 0; pRelDescription && pRelDescription[i].Length(); i++) {
		ipSpFilter->put_SpatialRelDescription(pRelDescription[i]);

		IFeatureCursorPtr ipFeatureCursor = getFeatureCursor(ipFeatureClass, IQueryFilterPtr(ipSpFilter));
		if(ipFeatureCursor == 0)
			continue;

		IFeaturePtr ipFeature2;
		while(ipFeatureCursor->NextFeature(&ipFeature2) == S_OK) {
			long nObjectId;
			ipFeature2->get_OID(&nObjectId);
			cFeatureMap[nObjectId] = ipFeature2;
		}
	}

	if(bDelegate) {
		belongByRoadLink(cFeatureMap, rResultRowMap);
	}
	else {
		rResultRowMap.insert(cFeatureMap);
	}
}

void BelongExecutor::getLqByInf(_IRow* ipRow, RowMap& rResultRowMap)
{
	if(ipRow == 0)
		return;

	long nObjectId;
	ipRow->get_OID(&nObjectId);

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName.fullNameString());
	if(ipTable == 0)
		return;

	using namespace sindy::schema;

	CString strWhereClause;
	if(! function().whereClause().IsEmpty() && rResultRowMap.m_cTableName.isSameTable(function().tableName())) {
		strWhereClause.Format(_T("%s AND %s=%d"), function().whereClause(), link_queue::kInfID, nObjectId);
	}
	else {
		strWhereClause.Format(_T("%s=%d"), link_queue::kInfID, nObjectId);
	}

	getRowMap(ipTable, CComBSTR(strWhereClause), rResultRowMap, CComBSTR(link_queue::kSequence));
}

void BelongExecutor::getInfByLq(_IRow* ipRow, RowMap& rResultRowMap)
{
	if(ipRow == 0)
		return;

	using namespace sindy::schema;

	CComVariant va = getValue(ipRow, CComBSTR(link_queue::kInfID));
	long nInfId = va.lVal;

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName);
	if(ipTable == 0)
		return;

	if(! function().whereClause().IsEmpty() && rResultRowMap.m_cTableName.isSameTable(function().tableName())) {
		CString strWhereClause;
		strWhereClause.Format(_T("%s AND %s=%d"), function().whereClause(), _T("OBJECTID"), nInfId);
		getRowMap(ipTable, CComBSTR(strWhereClause), rResultRowMap);

	}
	else {
		_IRowPtr ipInfRow;
		if(ipTable->GetRow(nInfId, &ipInfRow) != S_OK)
			return;

		rResultRowMap[nInfId] = ipInfRow;
	}
}

void BelongExecutor::getRoadLinkByLq(_IRow* ipRow, RowMap& rResultRowMap)
{
	if(ipRow == 0)
		return;

	using namespace sindy::schema;

	CComVariant va = getValue(ipRow, CComBSTR(link_queue::kLinkID));
	long nLinkId = va.lVal;

	IFeatureClassPtr ipFeatureClass = getTable(rResultRowMap.m_cTableName);
	if(ipFeatureClass == 0)
		return;

	if(! function().whereClause().IsEmpty() && rResultRowMap.m_cTableName.isSameTable(function().tableName())) {
		CString strWhereClause;
		strWhereClause.Format(_T("%s AND %s=%d"), function().whereClause(), _T("OBJECTID"), nLinkId);
		getRowMap(ITablePtr(ipFeatureClass), CComBSTR(strWhereClause), rResultRowMap);
	}
	else {
		IFeaturePtr ipFeature;
		if(ipFeatureClass->GetFeature(nLinkId, &ipFeature) != S_OK)
			return;

		rResultRowMap[nLinkId] = ipFeature;
	}
}

void BelongExecutor::getLqByRoadLink(IFeature* ipFeature, RowMap& rResultRowMap)
{
	if(ipFeature == 0)
		return;

	long nObjectId;
	ipFeature->get_OID(&nObjectId);

	ITablePtr ipLqTable = getTable(rResultRowMap.m_cTableName);
	if(ipLqTable == 0)
		return;

	using namespace sindy::schema;

	CString strWhereClause;
	if(! function().whereClause().IsEmpty() && rResultRowMap.m_cTableName.isSameTable(function().tableName())) {
		strWhereClause.Format(_T("%s AND %s=%d"), function().whereClause(), link_queue::kLinkID, nObjectId);
	}
	else {
		strWhereClause.Format(_T("%s=%d"), link_queue::kLinkID, nObjectId);
	}

	getRowMap(ipLqTable, CComBSTR(strWhereClause), rResultRowMap);
}

void BelongExecutor::getRoadLinkByLinkIdSequence(const std::map<long, long>& cLinkIdSequence, RowMap& rResultRowMap)
{
	using namespace sindy::schema::link_queue;

	if(cLinkIdSequence.empty())
		return;

	IFeatureClassPtr ipFeatureClass = getTable(rResultRowMap.m_cTableName);
	if(ipFeatureClass == 0)
		return;

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	CString strWhereClause;
	strWhereClause.Format(_T("%s IN ("), _T("OBJECTID"));
	for(std::map<long, long>::const_iterator it = cLinkIdSequence.begin(); it != cLinkIdSequence.end(); ++it) {
		if(it != cLinkIdSequence.begin()) {
			strWhereClause += _T(",");
		}

		CString strId;
		strId.Format(_T("%d"), it->first);
		strWhereClause += strId;
	}
	strWhereClause += _T(")");

	if(! function().whereClause().IsEmpty() && rResultRowMap.m_cTableName.isSameTable(function().tableName())) {
		strWhereClause += _T(" AND ");
		strWhereClause += function().whereClause();
	}

	ipQueryFilter->put_WhereClause(CComBSTR(strWhereClause));

	IFeatureCursorPtr ipFeatureCursor = getFeatureCursor(ipFeatureClass, ipQueryFilter);
	if(ipFeatureCursor == 0)
		return;

	std::map<long, _IRowPtr> cLinkMap;
	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
		long nObjectId;
		ipFeature->get_OID(&nObjectId);
		std::map<long, long>::const_iterator it = cLinkIdSequence.find(nObjectId);
		if(it != cLinkIdSequence.end()) {
			rResultRowMap[it->second] = ipFeature;
		}
	}
}


/**
 * @brief InfからLqを取得する
 *
 * @param rRowMap [in] Inf一覧
 * @param rResultRowMap [out] Lq一覧
 */
void BelongExecutor::getLqByInf(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	using namespace sindy::schema::link_queue;

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName);
	if(ipTable == 0)
		return;

	std::set<long> cInfIdSet;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		_IRowPtr ipRow = it->second;
		long nObjectId;
		ipRow->get_OID(&nObjectId);
		cInfIdSet.insert(nObjectId);
	}

	getRowMap(ipTable, CComBSTR(kInfID), cInfIdSet, rResultRowMap);
}

/**
 * @brief Lqから道路リンクを取得する
 *
 * @param rRowMap [in] Lq一覧
 * @param rResultRowMap [out] 道路リンク一覧
 */
void BelongExecutor::getRoadLinkByLq(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	using namespace sindy::schema::link_queue;

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName);
	if(ipTable == 0)
		return;

	CComBSTR bstrLinkId(kLinkID);

	std::set<long> cLinkIdSet;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		_IRowPtr ipRow = it->second;
		CComVariant va = getValue(ipRow, bstrLinkId);
		cLinkIdSet.insert(va.lVal);
	}

	getRowMap(ipTable, CComBSTR(_T("OBJECTID")), cLinkIdSet, rResultRowMap);
}

/**
 * @brief 道路リンクからLqを取得する
 * @param rRowMap [in] 道路リンク一覧
 * @param rResultRowMap [out] Lq一覧
 */
void BelongExecutor::getLqByRoadLink(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	using namespace sindy::schema::link_queue;

	ITablePtr ipLqTable = getTable(rResultRowMap.m_cTableName);
	if(ipLqTable == 0)
		return;

	CComBSTR bstrLqRfFieldName = rResultRowMap.m_cTableName.getLqRfFieldNameString();
	if(bstrLqRfFieldName.Length() == 0)
		return;

	std::set<long> cLinkIdSet;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		long nLinkId = it->first;
		_IRowPtr ipLink = it->second;

		CComVariant va = getValue(ipLink, bstrLqRfFieldName);
		if(va.lVal == 0)
			continue;

		cLinkIdSet.insert(nLinkId);
	}

	getRowMap(ipLqTable, CComBSTR(kLinkID), cLinkIdSet, rResultRowMap);
}

/**
 * @brief LqからInfを取得する
 *
 * @param rRowMap [in] Lq一覧
 * @param rResultRowMap [out] Inf一覧
 */
void BelongExecutor::getInfByLq(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	using namespace sindy::schema::link_queue;

	ITablePtr ipInfTable = getTable(rResultRowMap.m_cTableName);
	if(ipInfTable == 0)
		return;

	CComBSTR bstrInfId(kInfID);

	std::set<long> cInfIdSet;
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		_IRowPtr ipRow = it->second;
		CComVariant va = getValue(ipRow, bstrInfId);
		cInfIdSet.insert(va.lVal);
	}

	getRowMap(ipInfTable, CComBSTR(_T("OBJECTID")), cInfIdSet, rResultRowMap);
}

// RootExecutor クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

bool RootExecutor::execute()
{
	try {
		CString strTableName(logRow().m_cTableName.fullNameString());

		if(logRow().m_ipGeometry != 0) {
			FieldValues aFieldValues;
			aFieldValues.push_back(logRow().m_ipGeometry.GetInterfacePtr());
			chain(aFieldValues);
		}
		else {
			// テーブルを取得
			ITablePtr ipTable = tableFinder().getTable(strTableName);
			if(ipTable == 0)
				return false;

			if(logRow().m_nObjectId) {
				// オブジェクトIDが定義されていたならユニークオブジェクト
				_IRowPtr ipRow;
				if(ipTable->GetRow(logRow().m_nObjectId, &ipRow) != S_OK || ipRow == 0)
					return false;
				m_bChainSubField = false;
				chain(ipRow);
			}
			else {
				// Where句が定義されていたならオブジェクト
				const FunctionMap& rFunctionMap = nextFunctionMap();
				std::set<CString> cFieldNameSet;
				bool bNotSubFeilds = false;
				for(FunctionMap::const_iterator it = rFunctionMap.begin(); it != rFunctionMap.end(); ++it) {
					const SiNDYLogHeader::Function* pFunction = it->second;
					if(::_tcscmp(pFunction->getClassName(), SiNDYLogHeader::SubFieldFunction::className()) == 0) {
						cFieldNameSet.insert(static_cast<const SiNDYLogHeader::FieldFunction*>(pFunction)->fieldName());
					}
					else 
						bNotSubFeilds = true;
				}

				CComBSTR bstrWhereClause(logRow().m_strWhereClause.IsEmpty() ? _T("") : logRow().m_strWhereClause);
				if(! cFieldNameSet.empty()) {
					RowMapWithFieldMap cRowMap(logRow().m_cTableName);
					m_bChainSubField = true;
					try {
						getRowMap(CComBSTR(strTableName), bstrWhereClause, cFieldNameSet, cRowMap);
						chain(cRowMap);
					}
					catch(const Exception& e) {
						const FunctionMap& rFunctionMap = nextFunctionMap();
						for(FunctionMap::const_iterator it = rFunctionMap.begin(); it != rFunctionMap.end(); ++it) {
							const SiNDYLogHeader::Function* pFunction = it->second;
							if(::_tcscmp(pFunction->getClassName(), SiNDYLogHeader::SubFieldFunction::className()) == 0) {
								pFunction->fillAllCells(logRow(), e.what(), updatedColSet());
							}
						}
					}
				}

				if(bNotSubFeilds) {
					RowMap cRowMap(logRow().m_cTableName);
					m_bChainSubField = false;
					getRowMap(ipTable, bstrWhereClause, cRowMap);
					chain(cRowMap);
				}
			}
		}
	}
	catch(const Exception& e) {
		function().fillAllCells(logRow(), e.what(), updatedColSet());
		return false;
	}

	return true;
}

bool RootExecutor::onChain(const SiNDYLogHeader::Function& rFunction) const
{
	return m_bChainSubField ^ (::_tcscmp(rFunction.getClassName(), SiNDYLogHeader::SubFieldFunction::className()) != 0);
}
