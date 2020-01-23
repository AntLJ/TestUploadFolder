#include "stdafx.h"
#include "common.h"
#include "LogSys.h"
#include <sindy/workspace.h>

namespace common_proc {
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace)
	{
		IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectInfo);
		IUnknownPtr ipUnknown;
		if (FAILED(INamePtr(ipWorkspaceName)->Open(&ipUnknown))) {
			CString errMsg;
			errMsg.Format(_T("%s�̐ڑ��Ɏ��s���܂���"), connectInfo);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		ipWorkspace = IWorkspacePtr(ipUnknown);
		return true;
	}

	bool GetFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass)
	{
		CString fullTableName = tableName;
		if (! schemaName.IsEmpty()) {
			fullTableName.Format(_T("%s.%s"), schemaName, tableName);
		}
		if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(fullTableName), &ipFeatureClass))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���N���X�̃I�[�v���Ɏ��s���܂���"), fullTableName);
			return false;
		}
		return true;
	}

	bool GetFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor)
	{
		// �t�B���^�쐬
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		if (ipGeometry) {
			if (FAILED(ipSpFilter->putref_Geometry(ipGeometry)) ||
				FAILED(ipSpFilter->put_SpatialRel(spRel))) {
					LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B���^�[�̍쐬(��Ԍ����̐ݒ�)�Ɏ��s���܂���"), tableName);
					return false;
			}
		}
		if (! whereClause.IsEmpty()) {
			if (FAILED(ipSpFilter->put_WhereClause(CComBSTR(whereClause)))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B���^�[�̍쐬(���������̐ݒ�)�Ɏ��s���܂���"), tableName);
				return false;
			}
		}
		// �J�[�\���擾
		if (FAILED(ipFeatureClass->Search(ipSpFilter, VARIANT_FALSE, &ipFeatureCursor))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�J�[�\���擾�Ɏ��s���܂���"), tableName);
			return false;
		}
		return true;
	}

	bool GetValue(_IRowPtr ipRow, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val)
	{
		CString errMsg;
		if (FAILED(ipRow->get_Value(index, &val))) {
			errMsg.Format(_T("%s�t�B�[���h�̒l�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, layerName);
			return false;
		}
		return true;
	}

	bool PutValue(IRowBufferPtr ipRowBuffer, LPCTSTR layerName, LPCTSTR fieldName, long index, CComVariant& val)
	{
		CString errMsg;
		if (FAILED(ipRowBuffer->put_Value(index, val))) {
			errMsg.Format(_T("%s�t�B�[���h�̑����t�^�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, layerName);
			return false;
		}
		return true;
	}

}