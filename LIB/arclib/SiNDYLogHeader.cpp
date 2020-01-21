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

// SiNDYLogHeader.cpp : SiNDYLogHeader �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <crd_cnv/crd_cnv.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include <CStringTokenizer.h>
#include "SiNDYLogHeader.h"
#include "SiNDYLogFunctionExecutor.h"
#include "SiNDYPlant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

// SiNDYLogHeader::LogRowInfo �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::LogRowInfo::LogRowInfo(LogRow& rLogRow, const SiNDYLogHeader& rHeader) :
m_rLogRow(rLogRow),
m_nObjectId(0),
m_cTableName(_T(""))
{
	initInfo(rHeader);
}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

void SiNDYLogHeader::LogRowInfo::clearInfo()
{
	m_nObjectId = 0;
	m_strWhereClause.Empty();
	m_ipGeometry = 0;
}

void SiNDYLogHeader::LogRowInfo::initInfo(const SiNDYLogHeader& rHeader)
{
	clearInfo();

	// ���C�����ƃI�u�W�F�N�gID���擾
	CString strLayerName;
	if((rHeader.m_nWhereClauseCol < 0 && rHeader.getObjectIdentifier(m_rLogRow, strLayerName, m_nObjectId)) ||
	   rHeader.getObjectsSelector(m_rLogRow, strLayerName, m_strWhereClause) ||
	   rHeader.getLayerName(m_rLogRow, strLayerName)) {
		m_cTableName = TableName(strLayerName);
	}

	if(rHeader.canGetLonLat()) {
		double lon = 0.0, lat = 0.0;
		rHeader.getLonLat(m_rLogRow, lon, lat);

		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutCoords(lon, lat);
		m_ipGeometry = ipPoint;
	}
	else if(rHeader.canGetMeshXY()) {
		int x = 0, y = 0;
		unsigned int meshcode = 0;
		rHeader.getMeshXY(m_rLogRow, meshcode, x, y);

		double lon = 0.0, lat = 0.0;
		AheMESH2LL(meshcode, x, y, lon, lat);

		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutCoords(lon, lat);
		m_ipGeometry = ipPoint;
	}
}

void SiNDYLogHeader::LogRowInfo::updateCellsString(const std::set<int>& rColSet, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet)
{
	for(std::set<int>::const_iterator it = rColSet.begin(); it != rColSet.end(); ++it) {
		updateCellString(*it, lpszNew, pUpdatedColSet);
	}
}

bool SiNDYLogHeader::LogRowInfo::updateCellString(int nCol, LPCTSTR lpszNew, std::set<int>* pUpdatedColSet)
{
	if(! lpszNew)
		return false;

	if(nCol >= static_cast<int>(m_rLogRow.size())) {
		for(int i = m_rLogRow.size(); i <= nCol; i++) {
			m_rLogRow.push_back(_T(""));
		}
	}

	if(updateCellString(m_rLogRow[nCol], lpszNew)) {
		if(pUpdatedColSet) {
			pUpdatedColSet->insert(nCol);
		}
	}

	return false;
}

bool SiNDYLogHeader::LogRowInfo::updateCellString(CString& rCell, LPCTSTR lpszNew)
{
	if(! lpszNew)
		return false;

	if(rCell != lpszNew) {
		rCell = lpszNew;
		return true;
	}
	else
		return false;
}

SiNDYLogHeader::LogRow::const_reference SiNDYLogHeader::LogRowInfo::operator [] (LogRow::size_type  n) const
{
	if(n >= m_rLogRow.size()) {
		for(LogRow::size_type i = m_rLogRow.size(); i <= n; i++) {
			m_rLogRow.push_back(_T(""));
		}
	}

	return m_rLogRow[n];
}

// SiNDYLogHeader::FieldValues �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

CString SiNDYLogHeader::FieldValues::getValuesString() const
{
	CString strResult;

	for(const_iterator it = begin(); it != end(); ++it) {
		if(it != begin()) {
			strResult += _T(";");
		}
		CString strValue = getValueOrDomainNameString(m_ipField, *it);
		strResult += strValue;
	}

	return strResult;
}

// SiNDYLogHeader::RowMap �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::RowMap::RowMap(LPCTSTR lpszTableName) :
m_cTableName(lpszTableName)
{
}

SiNDYLogHeader::RowMap::RowMap(const TableName& rTableName) :
m_cTableName(rTableName)
{
}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���҂������e�[�u���ł������ꍇ�̂݁A�I�u�W�F�N�g��ǉ�����
 *
 * @param rRowMap [in] �ǉ�����I�u�W�F�N�g
 * @retval true �����e�[�u���ł��������߁A�ǉ������B
 * @retval false �قȂ�e�[�u���ł��������߁A�ǉ����Ȃ������B
 */
bool SiNDYLogHeader::RowMap::insert(const RowMap& rRowMap)
{
	if(! m_cTableName.isSameTable(rRowMap.m_cTableName))
		return false;

	for(RowMap::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		static_cast<std::map<long, _IRowPtr>*>(this)->insert(*it);
	}

	return true;
}

// SiNDYLogHeader::Function �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::Function::~Function()
{
	for(FunctionMap::iterator it = m_cNextFunctionMap.begin(); it != m_cNextFunctionMap.end(); ++it) {
		delete it->second;
		it->second = 0;
	}
	m_cNextFunctionMap.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

bool SiNDYLogHeader::Function::addNextFunctions(int nCol, LPCTSTR lpszFunctionList)
{
	if(! lpszFunctionList)
		return false;

	LPCTSTR lpszIndex = ::_tcschr(lpszFunctionList, _T('|'));
	if(lpszIndex == lpszFunctionList) {
		lpszFunctionList++;
		lpszIndex = ::_tcschr(lpszFunctionList, _T('|'));
	}

	CString strFunction = lpszIndex ? CString(lpszFunctionList, lpszIndex - lpszFunctionList) : lpszFunctionList;
	strFunction.TrimLeft();
	strFunction.TrimRight();

	CStringTokenizer tkz(strFunction, _T(" "));
	if(! tkz.HasMoreTokens())
		return false;

	Function* pFunction = 0;

	CString strFunctionName = tkz.GetNextToken();
	if(strFunctionName.CompareNoCase(BelongFunction::className()) == 0) {
		if(! tkz.HasMoreTokens())
			return false;

		CString strLayerName = tkz.GetNextToken();
		CString strWhereClause = tkz.GetString();
		strWhereClause.TrimLeft();
		strWhereClause.TrimRight();

		pFunction = new BelongFunction(strLayerName, strWhereClause);
	}
	else if(strFunctionName.CompareNoCase(FieldFunction::className()) == 0) {
		if(! tkz.HasMoreTokens())
			return false;

		CString strFieldName = tkz.GetNextToken();
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new FieldFunction(strFieldName);
	}
	else if(strFunctionName.CompareNoCase(SubFieldFunction::className()) == 0) {
		if(! tkz.HasMoreTokens())
			return false;

		CString strFieldName = tkz.GetNextToken();
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new SubFieldFunction(strFieldName);
	}
	else if(strFunctionName.CompareNoCase(PointFunction::className()) == 0) {
		if(! tkz.HasMoreTokens())
			return false;

		CString strNo = tkz.GetNextToken();
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new PointFunction(strNo);
	}
	else if(strFunctionName.CompareNoCase(BufferFunction::className()) == 0) {
		if(! tkz.HasMoreTokens())
			return false;

		CString strNo = tkz.GetNextToken();
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new BufferFunction(strNo);
	}
	else if(strFunctionName.CompareNoCase(DistinctFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new DistinctFunction();
	}
	else if(strFunctionName.CompareNoCase(AvgFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new AvgFunction();
	}
	else if(strFunctionName.CompareNoCase(MaxFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new MaxFunction();
	}
	else if(strFunctionName.CompareNoCase(MinFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new MinFunction();
	}
	else if(strFunctionName.CompareNoCase(SumFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new SumFunction();
	}
	else if(strFunctionName.CompareNoCase(CountFunction::className()) == 0) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new CountFunction();
	}
	else if(
		strFunctionName.CompareNoCase(LongitudeFunction::className()) == 0 ||
		strFunctionName.CompareNoCase(LongitudeFunction::shortClassName()) == 0)
	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new LongitudeFunction();
	}
	else if(
		strFunctionName.CompareNoCase(LatitudeFunction::className()) == 0 ||
		strFunctionName.CompareNoCase(LatitudeFunction::shortClassName()) == 0)
	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new LatitudeFunction();
	}
	else if(
		strFunctionName.CompareNoCase(LengthFunction::className()) == 0 ||
		strFunctionName.CompareNoCase(LengthFunction::shortClassName()) == 0)
	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new LengthFunction();
	}
	else if(
		strFunctionName.CompareNoCase(BaseMeshFunction::className()) == 0 ||
		strFunctionName.CompareNoCase(BaseMeshFunction::shortClassName()) == 0
	) {
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new BaseMeshFunction();
	}
	else if(
		strFunctionName.CompareNoCase(MeshcodeFunction::className()) == 0 ||
		strFunctionName.CompareNoCase(MeshcodeFunction::shortClassName()) == 0)
	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new MeshcodeFunction();
	}
	else if(strFunctionName.CompareNoCase(MeshXFunction::className()) == 0)	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new MeshXFunction();
	}
	else if(strFunctionName.CompareNoCase(MeshYFunction::className()) == 0)	{
		if(tkz.HasMoreTokens())
			return false;
		pFunction = new MeshYFunction();
	}

	if(! pFunction)
		return false;

	Function* pNextFunction = addNextFunction(pFunction);
	if(pNextFunction != pFunction) {
		delete pFunction;
		pFunction = 0;

		if(pNextFunction == 0)
			return false;
	}

	if(! lpszIndex) {
		pNextFunction->m_cColSet.insert(nCol);
	}
	else
		return pNextFunction->addNextFunctions(nCol, lpszIndex + 1);

	return true;
}

SiNDYLogHeader::Function* SiNDYLogHeader::Function::addNextFunction(Function* pNextFunction)
{
	if(pNextFunction == 0)
		return 0;

	CString strKey = pNextFunction->getKey();
	FunctionMap::iterator it = m_cNextFunctionMap.find(pNextFunction->getKey());
	if(it == m_cNextFunctionMap.end()) {
		m_cNextFunctionMap[strKey] = pNextFunction;
		return pNextFunction;
	}
	else 
		return it->second;
}

void SiNDYLogHeader::Function::getUpdateColumnSet(ColumnSet& rColSet) const
{
	for(ColumnSet::const_iterator it = m_cColSet.begin(); it != m_cColSet.end(); ++it) {
		rColSet.insert(*it);
	}

	for(FunctionMap::const_iterator it = m_cNextFunctionMap.begin(); it != m_cNextFunctionMap.end(); ++it) {
		it->second->getUpdateColumnSet(rColSet);
	}
}

void SiNDYLogHeader::Function::fillAllCells(SiNDYLogHeader::LogRowInfo& rLogRow, LPCTSTR lpszNew, ColumnSet* pUpdatedColSet) const
{
	rLogRow.updateCellsString(columnSet(), lpszNew, pUpdatedColSet);
	for(FunctionMap::const_iterator it = m_cNextFunctionMap.begin(); it != m_cNextFunctionMap.end(); ++it) {
		it->second->fillAllCells(rLogRow, lpszNew, pUpdatedColSet);
	}
}

// SiNDYLogHeader::FieldFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::FieldFunction::FieldFunction(LPCTSTR lpszFieldName)
{
	if(lpszFieldName) {
		m_strFieldName = lpszFieldName;
		m_strFieldName.MakeUpper();
	}
}

// ���z�֐��̃I�[�o�[���C�h

CString SiNDYLogHeader::FieldFunction::getKey() const
{
	CString strKey;
	strKey.Format(_T("%s %s"), className(), m_strFieldName);
	return strKey;
}

// SiNDYLogHeader::PointFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::PointFunction::PointFunction(LPCTSTR lpszNo) :
m_nNo(0),
m_bLast(false)
{
	if(lpszNo) {
		if(::_tcsicmp(lpszNo, _T("FIRST")) == 0) {
			m_nNo = 0;
			m_bLast = false;
		}
		else if(::_tcsicmp(lpszNo, _T("LAST")) == 0) {
			m_nNo = 0;
			m_bLast = true;
		}
		else {
			m_nNo = ::_ttoi(lpszNo);
			m_bLast = false;
		}
	}
}

IPointPtr SiNDYLogHeader::PointFunction::getPoint(IPointCollection* ipPointCol) const
{
	if(! ipPointCol)
		return 0;

	long nPointCount;
	ipPointCol->get_PointCount(&nPointCount);
	if(! nPointCount)
		return 0;

	if(nPointCount <= m_nNo)
		return 0;

	IPointPtr ipPoint;
	ipPointCol->get_Point(m_bLast ? nPointCount - 1 : m_nNo, &ipPoint);
	return ipPoint;
}

// ���z�֐��̃I�[�o�[���C�h

CString SiNDYLogHeader::PointFunction::getKey() const
{
	CString strKey;
	if(m_bLast) {
		strKey.Format(_T("%s last"), className());
	}
	else {
		strKey.Format(_T("%s %d"), className(), m_nNo);
	}

	return strKey;
}

// SiNDYLogHeader::BufferFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::BufferFunction::BufferFunction(LPCTSTR lpszDistance) :
m_dDistance(lpszDistance ? ::_tcstod(lpszDistance, 0) : 0)
{
}

IGeometryPtr SiNDYLogHeader::BufferFunction::buffer(IGeometry* ipGeometry) const
{
	if(! ipGeometry)
		return 0;

	IClonePtr ipClone;
	IClonePtr(ipGeometry)->Clone(&ipClone);
	IGeometryPtr ipClonedGeometry(ipClone);
	if(ipClonedGeometry == 0)
		return 0;

	ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
	IProjectedCoordinateSystemPtr ipProjectedCoordinateSystem;
	ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan10, &ipProjectedCoordinateSystem);
	ipClonedGeometry->Project(ipProjectedCoordinateSystem);

	ITopologicalOperatorPtr ipTopoOp(ipClonedGeometry);
	if(ipTopoOp == 0)
		return 0;

	IGeometryPtr ipResult;
	ipTopoOp->Buffer(m_dDistance, &ipResult);

	return ipResult;
}

// ���z�֐��̃I�[�o�[���C�h

CString SiNDYLogHeader::BufferFunction::getKey() const
{
	CString strKey;
	strKey.Format(_T("%s %f"), className(), m_dDistance);
	return strKey;
}
// SiNDYLogHeader::SelectFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::SelectFunction::SelectFunction()
{
}

SiNDYLogHeader::SelectFunction::SelectFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause) :
m_cTableName(lpszLayerName)
{
	if(lpszWhereClause) {
		m_strWhereClause = lpszWhereClause;
	}
}

// SiNDYLogHeader::BelongFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::BelongFunction::BelongFunction(LPCTSTR lpszLayerName, LPCTSTR lpszWhereClause) :
SelectFunction(lpszLayerName, lpszWhereClause)
{
}

// ���z�֐��̃I�[�o�[���C�h

CString SiNDYLogHeader::BelongFunction::getKey() const
{
	CString strKey;
	strKey.Format(_T("%s %s %s"), className(), tableName().fullNameString(), whereClause());
	return strKey;
}

// SiNDYLogHeader::RootFunction::RootFunction �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

// ���z�֐��̃I�[�o�[���C�h

CString SiNDYLogHeader::RootFunction::getKey() const
{
	CString strKey;
	strKey.Format(_T("%s %s %s"), className(), tableName().fullNameString(), whereClause());
	return strKey;
}

// SiNDYLogHeader �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYLogHeader::SiNDYLogHeader()
{
	clear();
}

SiNDYLogHeader::~SiNDYLogHeader()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

void SiNDYLogHeader::clear()
{
	m_nLayerCol = -1;
	m_nObjectIdCol = -1;
	m_nVdbMeshcodeCol = -1;
	m_nVdbPrimitiveIdCol = -1;
	m_nWhereClauseCol = -1;
	m_nScaleCol = -1;
	m_nLatitudeCol = -1;
	m_nLongitudeCol = -1;
	m_nMeshcodeCol = -1;
	m_nXCol = -1;
	m_nYCol = -1;
	m_cFlagColSet.clear();
	m_cEditableColSet.clear();
}

/**
 * @brief �@�\�w�b�_��ݒ肷��
 *
 * @param strHeader [in] �w�b�_
 * @param sep [in] �f���~�^
 * @retval true �@�\�w�b�_�����݂���
 * @retval false �@�\�w�b�_�͑��݂��Ȃ�����
 */
bool SiNDYLogHeader::setHeader(const CString& strHeader, LPCTSTR sep)
{
	bool bResult = false;

	CStringTokenizer tkz(strHeader, sep ? sep : getSeps(), TOKEN_RET_EMPTY_ALL);
	for(int i = 0; tkz.HasMoreTokens(); i++) {
		bResult |= setColumnHeader(tkz.GetNextToken(), i);
	}

	return true;
}

/**
 * @brief �@�\�w�b�_��ݒ肷��
 *
 * @param rLogRow [in] �w�b�_�s
 * @retval true �@�\�w�b�_�����݂���
 * @retval false �@�\�w�b�_�͑��݂��Ȃ�����
 */
bool SiNDYLogHeader::setHeader(const LogRow& rLogRow)
{
	bool bResult = false;

	for(int i = 0; i < static_cast<int>(rLogRow.size()); i++) {
		bResult |= setColumnHeader(rLogRow[i], i);
	}

	return true;
}


/**
 * @brief �@�\�w�b�_���P�ݒ肷��
 *
 * @param rColumnHeader [in] �@�\�w�b�_
 * @param nCol [in] ��ԍ�
 * @retval true �@�\�w�b�_��ݒ肵��
 * @retval false �@�\�w�b�_�ł͂Ȃ�
 */
bool SiNDYLogHeader::setColumnHeader(const CString& rColumnHeader, int nCol)
{
	int nOpendParenthesis = rColumnHeader.Find(_T('{'));
	return setFunctionColumnHeader(nOpendParenthesis >= 0 ? rColumnHeader.Left(nOpendParenthesis) : rColumnHeader, nCol);
}

/**
 * @brief �@�\�w�b�_���P�ݒ肷��
 *
 * @param strColumnHeader [in] �@�\�w�b�_
 * @param nCol [in] ��ԍ�
 * @retval true �@�\�w�b�_��ݒ肵��
 * @retval false �@�\�w�b�_�ł͂Ȃ�
 */
bool SiNDYLogHeader::setFunctionColumnHeader(const CString& strColumnHeader, int nCol)
{
	if(strColumnHeader.IsEmpty())
		return false;

	// ���`��
	if(strColumnHeader[0] == _T('$')) {
		int nFind;

		if((nFind = strColumnHeader.Find(_T("$FIELD/"))) == 0) {
			CString strFunction;
			strFunction.Format(_T("|FIELD %s"), strColumnHeader.Mid(strColumnHeader.Find(_T('/')) + 1));
			return m_cRootFunction.addNextFunctions(nCol, strFunction);
		}

		// �������b�V���J����
		if((nFind = strColumnHeader.Find(_T("$BELONG/"))) == 0) {
			CStringTokenizer tkz(strColumnHeader, _T("/"));
			CString strLayerName;
			CString strFieldName;
			for(int i = 0; i < 3 && tkz.HasMoreTokens(); i++) {
				switch(i) {
				case 1:
					strLayerName = tkz.GetNextToken();
					break;
				case 2:
					strFieldName = tkz.GetNextToken();
					break;
				default:
					tkz.GetNextToken();
					break;
				}
			}

			if(! (strLayerName.IsEmpty() || strFieldName.IsEmpty())) {
				CString strFunction;
				strFunction.Format(_T("|BELONG %s|FIELD %s"), strLayerName, strFieldName); 
				return m_cRootFunction.addNextFunctions(nCol, strFunction);
			}
			else 
				return false;
		}
	}

	// �J��Ԃ�
	if(::_tcsncmp(static_cast<LPCTSTR>(strColumnHeader), _T("~|"), 2) == 0) {
		int nFind = m_strFunctionHeader.ReverseFind(_T('|'));
		if(nFind >= 2) {
			m_cRootFunction.addNextFunctions(nCol, m_strFunctionHeader.Left(nFind + 1) + strColumnHeader.Mid(2));
		}
	}

	// �p�C�v
	if(strColumnHeader[0] == _T('|')) {
		if(m_cRootFunction.addNextFunctions(nCol, static_cast<LPCTSTR>(strColumnHeader) + 1)) {
			m_strFunctionHeader = strColumnHeader;
			return true;
		}
		else
			return false;
	}

	// �R�����g�J����
	if(strColumnHeader.Find(_T("COMMENT")) == 0) {
		m_cEditableColSet.insert(nCol);

		LPCTSTR p = static_cast<LPCTSTR>(strColumnHeader) + 7;
		p += ::_tcsspn(p, _T(" "));
		if(*p) {
			CString strCombo;
			strCombo.Format(*p == _T('|') ? _T("%s") : _T("�@|%s"), p);
			m_cComboMap[nCol] = strCombo;
		}

		return true;
	}

	// �t���O�J����
	if(strColumnHeader.Find(_T("FLAG")) == 0) {
		m_cFlagColSet.insert(nCol);
		m_cEditableColSet.insert(nCol);
		return true;
	}

	// ���C�����J����
	if(m_nLayerCol < 0 && ! strColumnHeader.CompareNoCase(_T("LAYER"))) {
		m_nLayerCol = nCol;
		return true;
	}

	int nFind;

	// �I�u�W�F�N�gID�J����
	if(m_nObjectIdCol < 0) {
		if(! strColumnHeader.CompareNoCase(_T("OBJECTID"))) {
			m_nObjectIdCol = nCol;
			m_strLayerName.Empty();
			return true;
		}
		else {
			nFind = strColumnHeader.Find(_T("_OID"));
			if(nFind > 0) {
				m_nObjectIdCol = nCol;
				m_strLayerName = strColumnHeader.Left(nFind);
				return true;
			}
		}
	}

	// VDB���b�V���R�[�h�J����
	if(m_nVdbMeshcodeCol < 0 && ! (strColumnHeader.CompareNoCase(_T("VDB_MESHCODE")))) {
		m_nVdbMeshcodeCol = nCol;
		return true;
	}

	// VDB�v���~�e�B�uID
	if(m_nVdbPrimitiveIdCol < 0 && ! (strColumnHeader.CompareNoCase(_T("VDB_PRIMITIVEID")))) {
		m_nVdbPrimitiveIdCol = nCol;
		return true;
	}

	// Where��
	if(m_nWhereClauseCol < 0 && ! (strColumnHeader.CompareNoCase(_T("WHERECLAUSE")) && strColumnHeader.CompareNoCase(_T("WHERE")))) {
		m_nWhereClauseCol = nCol;
		return true;
	}

	// OPEN
	if(::_tcsnicmp(strColumnHeader, _T("OPEN"), 4) == 0) {
		// �O�u���̎擾
		LPCTSTR p = static_cast<LPCTSTR>(strColumnHeader) + 4;
		p += ::_tcsspn(p, _T(" "));
		m_cOpenColMap[nCol] = p;
		return true;
	}

	// �t�B�[���h
	if((nFind = strColumnHeader.Find(_T("_FIELD"))) > 0) {
		m_cFieldColMap[nCol] = strColumnHeader.Left(nFind);
		return true;
	}

	if(::_tcsnicmp(strColumnHeader, _T("FIELD"), 5) == 0) {
		CStringTokenizer tkz(strColumnHeader, _T(" "));
		tkz.GetNextToken();
		m_cFieldColMap[nCol] = tkz.GetNextToken();
		return true;
	}

	// �ܓx�J����
	if(m_nLatitudeCol < 0 && ! (strColumnHeader.CompareNoCase(_T("LATITUDE")) && strColumnHeader.CompareNoCase(_T("LAT")))) {
		m_nLatitudeCol = nCol;
		return true;
	}

	// �o�x�J����
	if(m_nLongitudeCol < 0 && ! (strColumnHeader.CompareNoCase(_T("LONGITUDE")) && strColumnHeader.CompareNoCase(_T("LON")))) {
		m_nLongitudeCol = nCol;
		return true;
	}
	
	// ���b�V���R�[�h�J����
	if(m_nMeshcodeCol < 0 && ! (strColumnHeader.CompareNoCase(_T("MESHCODE")) && strColumnHeader.CompareNoCase(_T("MESH")))) {
		m_nMeshcodeCol = nCol;
		return true;
	}

	// X���W�J����
	if(m_nXCol < 0 && ! strColumnHeader.CompareNoCase(_T("X"))) {
		m_nXCol = nCol;
		return true;
	}

	// Y���W�J����
	if(m_nYCol < 0 && ! strColumnHeader.CompareNoCase(_T("Y"))) {
		m_nYCol = nCol;
		return true;
	}

	// �X�P�[���J����
	if(m_nScaleCol < 0 && ! strColumnHeader.CompareNoCase(_T("SCALE"))) {
		m_nScaleCol = nCol;
		return true;
	}

	return false;
}

/**
 * @brief �I�u�W�F�N�g���擾���邽�߂�Where����쐬�ł���w�b�_�\����
 *
 * @retval true �\
 * @retval false �s�\
 */
bool SiNDYLogHeader::canSelectObjects() const
{
	if(canIdentifyObject() || canIdentifyVdbPrimitive())
		return true;

	if(! m_cFieldColMap.empty())
		return true;

	if(m_nWhereClauseCol >= 0)
		return true;

	return false;
}

/**
 * @brief �w�肵���s���w���������C�������擾����
 *
 * @param rRow [in] �s
 * @param rLayerName [in] ���C����
 * @retval false ���C�������擾�ł���
 * @retval false ���C�������擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getLayerName(const LogRow& rRow, CString& rLayerName) const
{
	if(m_nLayerCol >= 0 && (int)rRow.size() > m_nLayerCol) {
		rLayerName = rRow[m_nLayerCol];
	}
	else {
		rLayerName = m_strLayerName;
	}

	return ! rLayerName.IsEmpty();
}

/**
 * @brief �w�肵���s���w���������C�����ƃI�u�W�F�N�gID���擾����
 *
 * @param rRow [in] �s
 * @param rLayerName [in] ���C����
 * @param rObjectId [in] �I�u�W�F�N�gID
 * @retval false ���C�����ƃI�u�W�F�N�gID���擾�ł���
 * @retval false ���C�������I�u�W�F�N�gID���擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getObjectIdentifier(const LogRow& rRow, CString& rLayerName, long& rObjectId) const
{
	if(m_nObjectIdCol < 0 || (int)rRow.size() <= m_nObjectIdCol)
		return false;

	rObjectId = ::_tcstol(rRow[m_nObjectIdCol], NULL, 0);

	return getLayerName(rRow, rLayerName) && rObjectId != 0;
}

/**
 * @brief �w�肵���s���w������VDB���b�V���R�[�h���擾����
 *
 * @param rRow [in] �s
 * @param rVdbMeshcode [out] VDB���b�V���R�[�h
 * @retval true VDB���b�V���R�[�h���擾�ł���
 * @retval false VDB���b�V���R�[�h���擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getVdbMeshcode(const LogRow& rRow, long& rVdbMeshcode) const
{
	if(static_cast<int>(rRow.size()) <= m_nVdbMeshcodeCol)
		return false;

	rVdbMeshcode = ::_ttol(rRow[m_nVdbMeshcodeCol]);

	return true;
}

/**
 * @brief �w�肵���s���w���������C�����ƁA�I�u�W�F�N�g���擾���邽�߂�Where����擾����
 *
 * @param rRow [in] �s
 * @param rLayerName [in] ���C����
 * @param rWhereClause [in] Where��
 * @retval true ���C������Where����擾�ł���
 * @retval false ���C������Where����擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getObjectsSelector(const LogRow& rRow, CString& rLayerName, CString& rWhereClause) const
{
	if(! canSelectObjects())
		return false;

	if(! getLayerName(rRow, rLayerName))
		return false;

	CString (SiNDYLogHeader::*fpFuncs[])(const LogRow&) const = {
		&SiNDYLogHeader::getObjectsSelectorByObjectId,
		&SiNDYLogHeader::getObjectsSelectorByVdbPrimitiveId,
		&SiNDYLogHeader::getObjectsSelectorByField,
		&SiNDYLogHeader::getObjectsSelectorByWhereClause,
		0
	};

	for(int i = 0; fpFuncs[i]; i++) {
		CString strWhere = (this->*(fpFuncs[i]))(rRow);
		if(strWhere.IsEmpty())
			continue;

		if(! rWhereClause.IsEmpty()) {
			rWhereClause += _T(" AND ");
		}
		rWhereClause += strWhere;
	}

	return ! rWhereClause.IsEmpty();
}

CString SiNDYLogHeader::getObjectsSelectorByObjectId(const LogRow& rRow) const
{
	if(! canIdentifyObject())
		return _T("");

	long nObjectId = 0;
	CString strLayerName;
	if(! getObjectIdentifier(rRow, strLayerName, nObjectId))
		return _T("");

	CString strResult;
	strResult.Format(_T("(%s=%d)"), _T("OBJECTID"), nObjectId);
	return strResult;
}

CString SiNDYLogHeader::getObjectsSelectorByVdbPrimitiveId(const LogRow& rRow) const
{
	if(
		m_nVdbMeshcodeCol < 0 || static_cast<int>(rRow.size()) <= m_nVdbMeshcodeCol ||
		m_nVdbPrimitiveIdCol < 0 || static_cast<int>(rRow.size()) <= m_nVdbPrimitiveIdCol
	)
		return _T("");

	int nMeshCode = ::_tcstol(rRow[m_nVdbMeshcodeCol], NULL, 0);
	int nPrimitiveId = ::_tcstol(rRow[m_nVdbPrimitiveIdCol], NULL, 0);
	if(nMeshCode <= 0 || nPrimitiveId <= 0)
		return _T("");

	using namespace schema::tmp_info;

	CString strResult;
	strResult.Format(_T("(%s=%d AND %s=%d)"), kTmpMeshCode, nMeshCode, kTmpPrimID, nPrimitiveId);
	return strResult;
}

CString SiNDYLogHeader::getObjectsSelectorByField(const LogRow& rRow) const
{
	if(m_cFieldColMap.empty())
		return _T("");

	CString strResult;
	for(FieldColMap::const_iterator it = m_cFieldColMap.begin(); it != m_cFieldColMap.end(); ++it) {
		int nCol = it->first;
		const CString& rFieldName = it->second;

		if(nCol < 0 || static_cast<int>(rRow.size()) <= nCol)
			continue;

		CString strValue = rRow[nCol];
		if(strValue.IsEmpty())
			continue;

		if(! strResult.IsEmpty()) {
			strResult += _T(" AND ");
		}
		CString strWhere;

		if(strValue.Find(_T('%')) >= 0) {
			strWhere.Format(_T("(%s like %s)"), rFieldName, strValue);
		}
		else {
			strWhere.Format(_T("(%s=%s)"), rFieldName, strValue);
		}
		strResult += strWhere;
	}

	return strResult;
}

CString SiNDYLogHeader::getObjectsSelectorByWhereClause(const LogRow& rRow) const
{
	if(m_nWhereClauseCol < 0 || static_cast<int>(rRow.size()) <= m_nWhereClauseCol)
		return _T("");

	CString strResult;
	strResult.Format(_T("(%s)"), rRow[m_nWhereClauseCol]);
	return strResult;
}

/**
 * @brief �w�肵���s���w�������o�ܓx���擾����
 *
 * @param rRow [in] �s
 * @param rLongitude [out] �o�x
 * @param rLatitude [out] �ܓx
 * @retval true �o�ܓx���擾�ł���
 * @retval false �o�ܓx���擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getLonLat(const LogRow& rRow, double& rLongitude, double& rLatitude) const
{
	if(! canGetLonLat())
		return false;

	LPCTSTR lpszLongitude =
		(m_nLongitudeCol >= 0 && m_nLongitudeCol < static_cast<signed>(rRow.size())) ?
			static_cast<LPCTSTR>(rRow[m_nLongitudeCol]) : 0;

	LPCTSTR lpszLatitude =
		(m_nLatitudeCol >= 0 && m_nLatitudeCol < static_cast<signed>(rRow.size())) ?
			static_cast<LPCTSTR>(rRow[m_nLatitudeCol]) : 0;

	if(! (lpszLongitude && lpszLatitude))
		return false;

	LPTSTR p;
	rLongitude = ::_tcstod(lpszLongitude, &p);
	if(p == lpszLongitude)
		return false;

	rLatitude = ::_tcstod(lpszLatitude, &p);
	if(p == lpszLatitude)
		return false;

	return true;
}

/**
 * @brief �w�肵���s���w���������b�V���R�[�h�Ɛ��K�����W���擾����
 *
 * @param rRow [in] �s
 * @param rMeshcode [out] ���b�V���R�[�h
 * @param rX [out] X���W
 * @param rY [out] Y���W
 * @retval true �擾�ł���
 * @retval false �擾�ł��Ȃ�����
 */
bool SiNDYLogHeader::getMeshXY(const LogRow& rRow, unsigned int& rMeshcode, int& rX, int& rY) const
{
	if(! canGetMeshXY())
		return false;

	LPCTSTR lpszMeshCode =
		(m_nMeshcodeCol >= 0 && m_nMeshcodeCol < static_cast<signed>(rRow.size())) ?
			static_cast<LPCTSTR>(rRow[m_nMeshcodeCol]) : 0;

	LPCTSTR lpszX = (m_nXCol >= 0 && m_nXCol < static_cast<signed>(rRow.size())) ? static_cast<LPCTSTR>(rRow[m_nXCol]) : 0;
	LPCTSTR lpszY = (m_nYCol >= 0 && m_nYCol < static_cast<signed>(rRow.size())) ? static_cast<LPCTSTR>(rRow[m_nYCol]) : 0;

	if(! (lpszMeshCode && lpszX && lpszY))
		return false;

	LPTSTR p(NULL);
	rMeshcode = ::_tcstoul(lpszMeshCode, &p, 0);
	if(p == lpszMeshCode)
		return false;

	rX = ::_tcstol(lpszX, &p, 0);
	if(p == lpszX)
		return false;

	rY = ::_tcstol(lpszY, &p, 0);
	if(p == lpszY)
		return false;

	return true;
}

bool SiNDYLogHeader::getScale(const LogRow& rRow, double& rScale) const
{
	if(! haveScaleColumn())
		return false;

	if(static_cast<int>(rRow.size()) < m_nScaleCol)
		return false;

	rScale = ::_tcstod(rRow[m_nScaleCol], NULL);

	return rScale > 0.0;
}

_IRowPtr SiNDYLogHeader::getIdentifiedObject(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const
{
	if(! canIdentifyObject())
		return 0;

	CString strTableName;
	long nObjectId;
	if(! getObjectIdentifier(rRow, strTableName, nObjectId))
		return 0;

	return rTableCarrier.getRow(strTableName, nObjectId);
}

IFeaturePtr SiNDYLogHeader::getIdentifiedVdbPrimitive(const LogRow& rRow, SiNDYTableFinder& rTableCarrier) const
{
	if(! canIdentifyVdbPrimitive())
		return 0;

	CString strTableName;
	CString strWhereClause;
	if(! getObjectsSelector(rRow, strTableName, strWhereClause))
		return 0;

	long nVdbMeshcode;
	if(! getVdbMeshcode(rRow, nVdbMeshcode))
		return 0;

	IFeatureClassPtr ipFeatureClass = rTableCarrier.getTable(strTableName);
	if(ipFeatureClass == 0)
		return 0;

	IGeometryPtr ipGeometry = rTableCarrier.getBaseMeshGeometry(nVdbMeshcode);
	CComBSTR bstrWhereClause = strWhereClause;
	if(ipGeometry != 0) {
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		ipSpFilter->put_WhereClause(bstrWhereClause);
		ipSpFilter->putref_Geometry(ipGeometry);
		ipSpFilter->put_SpatialRel(esriSpatialRelIntersects);

		IFeatureCursorPtr ipFeatureCursor;
		ipFeatureClass->Search(IQueryFilterPtr(ipSpFilter), VARIANT_TRUE, &ipFeatureCursor);
		
		IFeaturePtr ipFeature;
		if(ipFeatureCursor->NextFeature(&ipFeature) == S_OK)
			return ipFeature;
	}

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	ipQueryFilter->put_WhereClause(bstrWhereClause);

	IFeatureCursorPtr ipFeatureCursor;
	ipFeatureClass->Search(ipQueryFilter, VARIANT_TRUE, &ipFeatureCursor);
	
	IFeaturePtr ipFeature;
	ipFeatureCursor->NextFeature(&ipFeature);

	return ipFeature;
}
