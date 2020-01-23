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
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"

// �ÓI�����o�ϐ��̎���
crd_cnv CAddr2TsvBase::m_cCrdCnv;

CAddr2TsvBase::CAddr2TsvBase(void)
{
}

CAddr2TsvBase::~CAddr2TsvBase(void)
{
}

bool CAddr2TsvBase::init(void)
{
	if (!m_ipWorkspace || m_strTableName.IsEmpty())
		return false;	// �o�O�ȊO���肦�Ȃ����[�g

	CString fullTableName(m_strOwner.IsEmpty() ? m_strTableName : m_strOwner + _T(".") + m_strTableName);
	if (!CDBUtils::openTable(m_ipWorkspace, fullTableName, m_ipTable, m_fieldIndex)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errOpenTable, _T("�e�[�u���I�[�v�����s [%s]"), fullTableName);
		return false;
	}

	return true;
}

bool CAddr2TsvBase::execute(const bool bDispProgress, const CString& strWhere)
{
	// �i�����o�́i�W���o�́j
	long lEndCount = 0;
	long lPrePercentage = 0;
	CString strLogMsg;
	if (bDispProgress) {
		strLogMsg.Format(_T("  %s 0%%\r"), m_strProgressLogPrefix);
		std::cout << CT2CA(strLogMsg);
	}

	// �����擾
	long lCount = 0;
	if (!CDBUtils::getRowCount(m_ipTable, strWhere, lCount)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errSelectCount, _T("���R�[�h�����擾���s [%s]"), m_strTableName);
		return false;
	}

	// ����
	_ICursorPtr ipCursor;
	if (!CDBUtils::getCursor(m_ipTable, strWhere, m_strSortField, ipCursor)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errSelectTable, _T("���R�[�h�������s [%s]"), m_strTableName);
		return false;
	}

	_IRowPtr ipRow;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow) {
		// ���R�[�h�ϊ�
		if (!convRecord(ipRow))
			return false;

		// �i�����o�́i�W���o�́j
		if (bDispProgress) {
			++lEndCount;
			long lPercentage = (long)floor((((double)lEndCount / (double)lCount) * 100.0));
			if (lPrePercentage != lPercentage) {
				strLogMsg.Format(_T("  %s %ld%%\r"), m_strProgressLogPrefix, lPercentage);
				std::cout << CT2CA(strLogMsg);
			}
			lPrePercentage = lPercentage;
		}
	}
	// ���ʏo�́irun���O�ƕW���o�́j
	if (bDispProgress) {
		strLogMsg.Format(_T("  %s ... OK\n"), m_strProgressLogPrefix);
		CLogSys::GetInstance().WriteRunLog(true, false, strLogMsg);
	}

	return true;
}

bool CAddr2TsvBase::getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index) const
{
	if (!CDBUtils::getFieldIndex(fieldIndex, strFieldName, index)) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errGetIndex, _T("�e�[�u���̃C���f�b�N�X�擾���s [%s.%s]"), m_strTableName, strFieldName);
		return false;
	}
	return true;
}

bool CAddr2TsvBase::getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal) const
{
	CComVariant vaVal;
	if (!CDBUtils::getValue(ipRow, strFieldName, index, strVal)) {
		long oid = 0;
		ipRow->get_OID(&oid);
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, m_strTableName, oid, err_code::errGetValue, _T("�����擾���s [%s]"), strFieldName);
		return false;
	}
	return true;
}

bool CAddr2TsvBase::getMeshXY(const IFeaturePtr& ipFeature, int& mesh, int &x, int& y) const
{
	long oid = 0;
	ipFeature->get_OID(&oid);

	IGeometryPtr ipGeom;
	if (FAILED(ipFeature->get_Shape(&ipGeom))) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, m_strTableName, oid, err_code::errGetShape, _T("�`��擾���s"));
		return false;
	}
	VARIANT_BOOL bIsEmpty = VARIANT_TRUE;
	if (FAILED(ipGeom->get_IsEmpty(&bIsEmpty)) || bIsEmpty == VARIANT_TRUE) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, m_strTableName, oid, err_code::errGetShape, _T("�`��擾���s"));
		return false;
	}
	double lon = 0.0, lat = 0.0;
	if (FAILED(((IPointPtr)ipGeom)->QueryCoords(&lon, &lat))) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, m_strTableName, oid, err_code::errGetXY, _T("XY���W�擾���s"));
		return false;
	}
	if (m_cCrdCnv.LLtoMesh(lon, lat, 2, &mesh, &x, &y) == -1) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, m_strTableName, oid, err_code::errLLtoMesh, _T("���b�V��XY�ϊ����s"));
		return false;
	}

	return true;
}
