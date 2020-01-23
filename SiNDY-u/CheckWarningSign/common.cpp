#include "common.h"
#include "LogSys.h"
#include <sindy/workspace.h>

namespace common_proc {
	bool getWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace)
	{
		IWorkspacePtr ipWork = sindy::create_workspace(connectInfo);
		if( ! ipWork){
			CString errMsg;
			errMsg.Format(_T("%s�̐ڑ��Ɏ��s���܂���"), connectInfo);
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, errMsg);
			return false;
	}
		ipWorkspace = ipWork;
		return true;
	}

	bool getFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR tableName, const CString& schemaName, IFeatureClassPtr& ipFeatureClass)
	{
		CString fullTableName = tableName;
		if (! schemaName.IsEmpty()) {
			fullTableName.Format(_T("%s.%s"), schemaName, tableName);
		}
		if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(fullTableName), &ipFeatureClass))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�t�B�[�`���N���X�̃I�[�v���Ɏ��s���܂���"), fullTableName);
			return false;
		}
		return true;
	}

	bool getFeatureCursor(const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& whereClause, IFeatureCursorPtr& ipFeatureCursor)
	{
		// �t�B���^�쐬
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		if (ipGeometry) {
			if (FAILED(ipSpFilter->putref_Geometry(ipGeometry)) ||
				FAILED(ipSpFilter->put_SpatialRel(spRel)) ||
				FAILED(ipSpFilter->put_GeometryField(CComBSTR(_T("SHAPE")))) ) {
					LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�t�B���^�[�̍쐬(��Ԍ����̐ݒ�)�Ɏ��s���܂���"), tableName);
					return false;
			}
		}

		if (! whereClause.IsEmpty()) {
			if (FAILED(ipSpFilter->put_WhereClause(CComBSTR(whereClause)))) {
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�t�B���^�[�̍쐬(���������̐ݒ�)�Ɏ��s���܂���"), tableName);
				return false;
			}
		}
		// �J�[�\���擾
		if (FAILED(ipFeatureClass->Search(ipSpFilter, VARIANT_FALSE, &ipFeatureCursor))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�J�[�\���擾�Ɏ��s���܂���"), tableName);
			return false;
		}

		return true;
	}

	bool getFields(LPCTSTR tableName, IFeatureClassPtr& ipFeatureClass, IFieldsPtr& ipFields)
	{
		if(S_OK != ipFeatureClass->get_Fields(&ipFields)){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�t�B�[���h�Q�̎擾�Ɏ��s���܂���"), tableName);
			return false;
		}
		return true;
	}

	bool getField(LPCTSTR tableName, IFieldsPtr& ipFields, long lIndex, IFieldPtr& ipField)
	{
		if(S_OK != ipFields->get_Field(lIndex, &ipField)){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�t�B�[���h�̎擾�Ɏ��s���܂���"), tableName);
			return false;
		}
		return true;
	}
}