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

#include "common.h"
#include "LogSys.h"
#include <sindy/workspace.h>

namespace common_proc
{
	bool GetWorkspace(LPCTSTR connectInfo, IWorkspacePtr& ipWorkspace)
	{
		IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectInfo);
		if (!ipWorkspaceName) 
			return false;
		IUnknownPtr ipUnknown;
		if (FAILED(((INamePtr)ipWorkspaceName)->Open(&ipUnknown))) {
			CString errMsg;
			errMsg.Format(_T("%s�̐ڑ��Ɏ��s���܂���"), connectInfo);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		ipWorkspace = IWorkspacePtr(ipUnknown);
		return true;
	}

	bool GetTable(LPCTSTR tableName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipTable)
	{
		if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(tableName), &ipTable))) {
			CString errMsg;
			errMsg.Format(_T("%s�̃I�[�v���Ɏ��s���܂���"), tableName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		return true;
	}

	bool GetGeometryDef(LPCTSTR featureClassName, const IFeatureClassPtr& ipFeatureClass, IGeometryDefPtr &ipGeoDef)
	{
		// �t�B�[���h�Q�擾
		IFieldsPtr ipFields;
		if (FAILED(ipFeatureClass->get_Fields(&ipFields))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�Q�̎擾�Ɏ��s���܂���"), featureClassName);
			return false;
		}
		// Shape�t�B�[���h�擾
		long shapeIndex = -1;
		IFieldPtr ipField;
		if (FAILED(ipFields->FindField(CComBSTR(shp05::schema::kShape), &shapeIndex)) ||
			FAILED(ipFields->get_Field(shapeIndex, &ipField))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�̎擾�Ɏ��s���܂���"), featureClassName);
				return false;
		}
		// GeometryDef�擾
		if (FAILED(ipField->get_GeometryDef(&ipGeoDef))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("GeometryDef�̎擾�Ɏ��s���܂���"), featureClassName);
			return false;
		}
		return true;
	}

	bool GetFieldIndeces(LPCTSTR tableName, const ITablePtr& ipTable, FieldIndexMap* indeces)
	{
		CString errMsg;
		indeces->clear();
		IFieldsPtr ipFields;
		if (FAILED(ipTable->get_Fields(&ipFields))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�Q�擾�Ɏ��s���܂���"), tableName);
			return false;
		}
		// �t�B�[���h���擾
		long fieldCount;
		if (FAILED(ipFields->get_FieldCount(&fieldCount))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h���擾�Ɏ��s���܂���"), tableName);
			return false;
		}
		// �e�t�B�[���h���E�t�B�[���h�C���f�b�N�X�擾
		for (long i = 0; i < fieldCount; ++i) {
			// �t�B�[���h�擾
			IFieldPtr ipField;
			if (FAILED(ipFields->get_Field(i, &ipField))) {
				errMsg.Format(_T("�t�B�[���h�擾�Ɏ��s���܂����Bindex = %d"), i);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
				return false;
			}
			// �t�B�[���h���擾
			CComBSTR fieldName;
			if (FAILED(ipField->get_Name(&fieldName))) {
				errMsg.Format(_T("�t�B�[���h���擾�Ɏ��s���܂����B index = %d"), i);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
				return false;
			}
			// index�o�^
			indeces->insert(FieldIndexMap::value_type(CString(fieldName).MakeLower(), i));
		}
		return true;
	}

	bool GetRelFeatureCursor(LPCTSTR tableName, const IFeatureClassPtr& ipFeatureClass, const IGeometryPtr& ipGeometry, const esriSpatialRelEnum spRel, IFeatureCursorPtr& ipFeatureCursor, CString whereClause)
	{
		// �t�B���^�쐬
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		if (FAILED(ipSpFilter->putref_Geometry(ipGeometry)) ||
			FAILED(ipSpFilter->put_SpatialRel(spRel))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B���^�[�쐬�Ɏ��s���܂���(�����[�V�����t�^)"), tableName);
				return false;
		}
		if (! whereClause.IsEmpty()) {
			if (FAILED(ipSpFilter->put_WhereClause(CComBSTR(whereClause)))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B���^�[�쐬�Ɏ��s���܂���(Where��t�^)"), tableName);
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

	bool GetValue(const _IRowPtr& ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const FieldIndexMap& indeces, CComVariant* val)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		FieldIndexMap::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%s�t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		if (FAILED(ipRow->get_Value(fieldItr->second, val))) {
			errMsg.Format(_T("%s�t�B�[���h�̒l�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		return true;
	}

	bool GetMinMax(const IGeometryPtr ipGeometry, LONLAT* minLonLat, LONLAT* maxLonLat)
	{
		CString errMsg;
		// �G���x���[�v�쐬
		IEnvelopePtr ipEnv;
		if (FAILED(ipGeometry->get_Envelope(&ipEnv))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�G���x���[�v�̎擾�Ɏ��s���܂���"));
			return false;
		}
		// �ő�E�ŏ����W�擾
		if (FAILED(ipEnv->get_XMin(&minLonLat->e_lon)) || FAILED(ipEnv->get_YMin(&minLonLat->e_lat)) ||
			FAILED(ipEnv->get_XMax(&maxLonLat->e_lon)) || FAILED(ipEnv->get_YMax(&maxLonLat->e_lat))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�G���x���[�v�̍ő�E�ŏ����W�擾�Ɏ��s���܂���"));
			return false;
		}
		return true;
	}
}
