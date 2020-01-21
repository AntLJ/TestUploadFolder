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
	strReason.Format(_T("! �֐�\"%s\"��\"%s\"�͓��͂ł��Ȃ��B"), lpszKey, lpszInput);
	setReason(strReason);
}

Executor::OverflowException::OverflowException(const TableName& rTableName, int nCount)
{
	CString strReason;
	strReason.Format(_T("! \"%s\"�������ɁA�I�u�W�F�N�g�W����%d���ɒB�����B"), rTableName.fullNameString(), nCount);
	setReason(strReason);
}

Executor::LayerNotFoundException::LayerNotFoundException(const TableName& rTableName)
{
	CString strReason;
	strReason.Format(_T("! \"%s\"���J���Ȃ��B"), rTableName.fullNameString());
	setReason(strReason);
}

Executor::FieldNotFoundException::FieldNotFoundException(const CString& rFieldName)
{
	CString strReason;
	strReason.Format(_T("! �����t�B�[���h�t�B�[���h\"%s\"�����݂��Ȃ�"), rFieldName);
	setReason(strReason);
}

Executor::CantCalcException::CantCalcException(LPCTSTR lpszFieldName)
{
	CString strReason;
	if(lpszFieldName) {
		strReason.Format(_T("! \"%s\"�ɑ΂��Ďw��̌v�Z���s�����Ƃ��o���Ȃ�"), lpszFieldName);
	}
	else {
		strReason.Format(_T("! �w��̌v�Z���s�����Ƃ��o���Ȃ�"));
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

// Executor �N���X�̃C���v�������e�[�V����
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

// FieldExecutor �N���X�̃C���v�������e�[�V����
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

// �ŗL����

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

// SubFieldExecutor �N���X�̃C���v�������e�[�V����
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

// PointExecutor �N���X�̃C���v�������e�[�V����
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
			throw NotImplementException(getKey(), _T("�_��`��ȊO�̑����l"));

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

// BufferExecutor �N���X�̃C���v�������e�[�V����
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
			throw NotImplementException(getKey(), _T("�`��ȊO�̑����l"));

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

// LengthExecutor �N���X�̃C���v�������e�[�V����
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
 * @brief �Q�_�Ԃ̋����i���[�g���j�����߂�
 *
 * @param dX1 [in] �_�P�̌o�x
 * @param dX2 [in] �_�P�̈ܓx
 * @param dY1 [in] �_�Q�̌o�x
 * @param dY2 [in] �_�Q�̈ܓx
 * @return �Q�_�Ԃ̋����i���[�g���j
 */
double LengthExecutor::getLength(double dX1, double dY1, double dX2, double dY2)
{
	return crd_cnv().GetDist(dX1, dY1, dX2, dY2);
}

// LonLatExecutor �N���X�̃C���v�������e�[�V����
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

// LongitudeExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

CComVariant LongitudeExecutor::getValue(IPoint* ipPoint)
{
	if(! ipPoint)
		return CComVariant();

	double dX;
	ipPoint->get_X(&dX);

	return dX;
}

// LatitudeExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

CComVariant LatitudeExecutor::getValue(IPoint* ipPoint)
{
	if(! ipPoint)
		return CComVariant();

	double dY;
	ipPoint->get_Y(&dY);

	return dY;
}

// MeshExecutor �N���X�̃C���v�������e�[�V����
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
		throw NotImplementException(getKey(), _T("�_�`��ȊO�̃t�B�[�`��"));

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	return getMeshXY(IPointPtr(ipGeometry));
}

LogFE::MeshXY MeshExecutor::getMeshXY(const VARIANT& rVariant)
{
	if(rVariant.vt != VT_UNKNOWN)
		throw NotImplementException(getKey(), _T("�_�`��ȊO�̑����l"));

	return getMeshXY(IPointPtr(rVariant.punkVal));
}

LogFE::MeshXY MeshExecutor::getMeshXY(IPoint* ipPoint)
{
	if(! ipPoint)
		throw NotImplementException(getKey(), _T("�_�`��ȊO�̌`��"));

	double dX;
	double dY;
	ipPoint->get_X(&dX);
	ipPoint->get_Y(&dY);

	return getMeshXY(dX, dY);
}

// BaseMeshExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �o�ܓx���Q�����b�V��XY���W�i100�����W�n�j�ɕϊ�����
 *
 * @param dLongitude [in] �o�x
 * @param dLatitude [in] �ܓx
 * @return �Q�����b�V��XY���W
 */
LogFE::MeshXY BaseMeshExecutor::getMeshXY(double dLongitude, double dLatitude)
{
	int x = 0, y = 0;
	unsigned int meshcode = AheLL2MESH( dLongitude, dLatitude, 2, x, y );

	return LogFE::MeshXY(meshcode, x, y);
}

// MeshXYExecutor �N���X�̃C���v�������e�[�V����
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

// DistinctExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

// ���z�֐��̃I�[�o�[���C�h

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

// AvgExecutor �N���X�̃C���v�������e�[�V����
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

// MaxExecutor �N���X�̃C���v�������e�[�V����
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

// MinExecutor �N���X�̃C���v�������e�[�V����
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

// SumExecutor �N���X�̃C���v�������e�[�V����
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

// CountExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

// ���z�֐��̃I�[�o�[���C�h

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

// SelectExecutor �N���X�̃C���v�������e�[�V����
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

// BelongExecutor �N���X�̃C���v�������e�[�V����
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

// �ŗL����

void BelongExecutor::belong(_IRow* ipRow, RowMap& rResultRowMap)
{
	if(! ipRow)
		return;

	// �����e�[�u���Ԃ�BELONG���邱�Ƃ͕s�\
	TableName cTableName(ipRow);
	if(cTableName.isSameTable(rResultRowMap.m_cTableName))
		return;

	// �e�[�u���̎�ނɉ����ď�����U�蕪��
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

	// �����e�[�u���Ԃ�BELONG���邱�Ƃ͕s�\
	if(rRowMap.m_cTableName.isSameTable(rResultRowMap.m_cTableName))
		return;

	// �e�[�u���̎�ނɉ����ď�����U�蕪��
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
			throw NotImplementException(getKey(), _T("�`��ȊO�̑����l"));

		belongByGeometry(ipGeometry, rResultRowMap);
	}
}

void BelongExecutor::belongByInf(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// Inf����Lq���擾
	RowMap cLqRowMap(static_cast<LPCTSTR>(rRowMap.m_cTableName.getLqFullNameString()));
	getLqByInf(rRowMap, cLqRowMap);

	// Lq�����߂悤�Ƃ��Ă����ꍇ�A�����ŏI��
	if(rResultRowMap.insert(cLqRowMap))
		return;

	// Lq����...
	belongByLq(cLqRowMap, rResultRowMap);
}

void BelongExecutor::belongByLq(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// Lq����Inf���擾
	TableName cInfTableName(rRowMap.m_cTableName.getInfFullNameString());
	if(cInfTableName.isSameTable(rResultRowMap.m_cTableName)) {
		getInfByLq(rRowMap, rResultRowMap);
		return;
	}

	// Lq���瓹�H�����N���擾
	RowMap cLinkMap(schema::road_link::kTableName);
	getRoadLinkByLq(rRowMap, cLinkMap);

	// ���H�����N�����߂Ă����ꍇ�A�����ŏI��
	if(rResultRowMap.insert(cLinkMap))
		return;

	// ���H�����N����...
	belongByRoadLink(cLinkMap, rResultRowMap);
}

void BelongExecutor::belongByRoadLink(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// Inf��Lq�����߂悤�Ƃ��Ă����ꍇ�ALq�����߂āA�K�؂Ȋ֐��ɏ������ϑ�����
	if(rResultRowMap.m_cTableName.isLqTableName() || rResultRowMap.m_cTableName.isInfTableName()) {
		RowMap cLqRowMap(TableName(rResultRowMap.m_cTableName.getLqFullNameString()));
		getLqByRoadLink(rRowMap, cLqRowMap);
		if(rResultRowMap.insert(cLqRowMap))
			return;

		belongByLq(cLqRowMap, rResultRowMap);
		return;
	}

	// ���H�����N�e�X�ɑ΂�����
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		belongByRoadLink(rRowMap.m_cTableName, IFeaturePtr(it->second), rResultRowMap);
	}
}

void BelongExecutor::belongByFeature(const RowMap& rRowMap, RowMap& rResultRowMap)
{
	// �t�B�[�`���e�X�ɑ΂�����
	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		belongByFeature(IFeaturePtr(it->second), rResultRowMap);
	}
}

void BelongExecutor::belongByInf(const TableName& rInfTableName, _IRow* ipInfRow, RowMap& rResultRowMap)
{
	if(! ipInfRow)
		return;

	// Inf����Lq���擾
	RowMap cLqRowMap(TableName(rInfTableName.getLqFullNameString()));
	getLqByInf(ipInfRow, cLqRowMap);

	// Lq�����߂Ă����ꍇ�A�����ŏI��
	if(rResultRowMap.insert(cLqRowMap))
		return;

	using namespace sindy::schema;

	// ���H�����N�擾
	CComBSTR bstrLinkId(link_queue::kLinkID);

	std::map<long, long> cLinkIdSequence;
	for(RowMap::const_iterator it = cLqRowMap.begin(); it != cLqRowMap.end(); ++it) {
		CComVariant va = getValue(it->second, bstrLinkId);
		cLinkIdSequence[va.lVal] = it->first;
	}

	RowMap cLinkMap(TableName(cLqRowMap.m_cTableName.complementOwnerName(road_link::kTableName)));
	getRoadLinkByLinkIdSequence(cLinkIdSequence, cLinkMap);

	// ���H�����N�����߂Ă����ꍇ�A�����ŏI��
	if(rResultRowMap.insert(cLinkMap))
		return;

	// ���H�����N�̌`�����������
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

	// �`�󂩂�đI�����s��
	belongByGeometry(IGeometryPtr(ipTopoOp), rResultRowMap);
}


void BelongExecutor::belongByLq(const TableName& rLqTableName, _IRow* ipRow, RowMap& rResultRowMap)
{
	if(! ipRow)
		return;

	// Lq����Inf���擾
	TableName cInfTableName(rLqTableName.getInfFullNameString());
	if(cInfTableName.isSameTable(rResultRowMap.m_cTableName)) {
		getInfByLq(ipRow, rResultRowMap);
		return;
	}

	// Lq���瓹�H�����N���擾
	RowMap cLinkMap(TableName(rLqTableName.complementOwnerName(schema::road_link::kTableName)));
	getRoadLinkByLq(ipRow, cLinkMap);

	// ���H�����N�����߂悤�Ƃ��Ă����ꍇ�A�����ŏI��
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

	// Inf��Lq�����߂悤�Ƃ��Ă����ꍇ�ALq�����߂āA�K�؂Ȋ֐��ɏ������ϑ�����
	if(rResultRowMap.m_cTableName.isLqTableName() || rResultRowMap.m_cTableName.isInfTableName()) {
		RowMap cLqRowMap(TableName(rResultRowMap.m_cTableName.getLqFullNameString()));
		getLqByRoadLink(ipFeature, cLqRowMap);
		if(rResultRowMap.insert(cLqRowMap))
			return;

		belongByLq(cLqRowMap, rResultRowMap);
		return;
	}

	// �t�B�[�`�����ʏ�����
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

	// �`��P�̌`���擾
	esriGeometryType geoType1;
	ipGeometry1->get_GeometryType(&geoType1);
	bool bGeoType1IsPoint = (geoType1 == esriGeometryPoint || geoType1 == esriGeometryMultipoint);

	ITablePtr ipTable = getTable(rResultRowMap.m_cTableName);
	if(ipTable == 0)
		return;

	using namespace sindy::schema;

	// �`��ɑ΂���BELONG�́A�t�B�[�`���N���X�ɑ΂��Ă݈̂Ӗ�������
	IFeatureClassPtr ipFeatureClass = ipTable;
	bool bDelegate = false;
	CString strDelegateWhereClause;
	if(ipFeatureClass == 0) {
		// �������AInf��Lq�Ɋւ��ẮA���H�����N�����̑���Ƃ��邱�Ƃ��ł���
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

	// �`��Q�̌`���擾
	esriGeometryType geoType2;
	ipFeatureClass->get_ShapeType(&geoType2);
	bool bGeoType2IsPoint = (geoType2 == esriGeometryPoint || geoType2 == esriGeometryMultipoint);

	// �ǂ�����_�`�󂩊m�F
	bool bEachGeoTypeIsPoint = (bGeoType1IsPoint && bGeoType2IsPoint);

	// ��ԃt�B���^�p��
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

	// �֌W��
	static const CComBSTR bstrPointRelDescription[] = { _T("T********"), _T("") };
	static const CComBSTR bstRelDescription[] = { _T("*T*******"), _T("***T*****"), _T("**F******"), _T("******F**"), _T("") };

	// �_�`�󓯎m�̏ꍇ�́u�����Ɠ����v����B
	// ����ȊO�̏ꍇ�́A�u�ǂ��炩�̋��E����������̓����ɑ��݂���v���A�u�ǂ��炩�̓������A��������̊O���ɑ��݂��Ȃ��v���𔻒�B
	// �f�[�^������ł���΁A�ȉ��̂��̂��u��������v�Ɣ��肷��͂��ł���B
	// �E�w�肵�����b�V�����ɑ��݂���`��
	// �E�w�肵�����b�V�����E�ɑ��݂���_�`��
	// �E�w�肵���`�󂪑��݂��Ă��郁�b�V��
	// �E�w�肵�������N��ɑ��݂��郊���N�������i�X�΁j
	// �E�w�肵�������N�������i�X�΁j�����݂��郊���N
	// �E�w�肵�������N�ɐڑ�����m�[�h
	// �E�w�肵���m�[�h�ɐڑ����郊���N
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
 * @brief Inf����Lq���擾����
 *
 * @param rRowMap [in] Inf�ꗗ
 * @param rResultRowMap [out] Lq�ꗗ
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
 * @brief Lq���瓹�H�����N���擾����
 *
 * @param rRowMap [in] Lq�ꗗ
 * @param rResultRowMap [out] ���H�����N�ꗗ
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
 * @brief ���H�����N����Lq���擾����
 * @param rRowMap [in] ���H�����N�ꗗ
 * @param rResultRowMap [out] Lq�ꗗ
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
 * @brief Lq����Inf���擾����
 *
 * @param rRowMap [in] Lq�ꗗ
 * @param rResultRowMap [out] Inf�ꗗ
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

// RootExecutor �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// ����
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
			// �e�[�u�����擾
			ITablePtr ipTable = tableFinder().getTable(strTableName);
			if(ipTable == 0)
				return false;

			if(logRow().m_nObjectId) {
				// �I�u�W�F�N�gID����`����Ă����Ȃ烆�j�[�N�I�u�W�F�N�g
				_IRowPtr ipRow;
				if(ipTable->GetRow(logRow().m_nObjectId, &ipRow) != S_OK || ipRow == 0)
					return false;
				m_bChainSubField = false;
				chain(ipRow);
			}
			else {
				// Where�傪��`����Ă����Ȃ�I�u�W�F�N�g
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
