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
#include "ShapeMaker.h"


ShapeMaker::ShapeMaker(const CString& outRootDir, bool isTest, const IGeometryDefPtr ipGeometryDef)
	:m_outRootDir(outRootDir), m_isTest(isTest),m_ipGeometryDef(ipGeometryDef)
{
}

ShapeMaker::~ShapeMaker(void)
{
}

bool ShapeMaker::makeShape(const IDRecordMap& cnvInfo, long cityMesh)
{
	// �o�̓t�@�C����
	CString shpName;
	shpName.Format(_T("%d_%s"), cityMesh, shp05::schema::citymap::bld_name::kSuffix);
	// �o�̓t�H���_�쐬&�����t�@�C���폜
	CString outputDir;
	if (! makeDirAndDelFile(cityMesh, shpName, &outputDir))
		return false;
	// ���[�N�X�y�[�X�̎擾
	IWorkspacePtr ipWorkspace;
	if (! common_proc::GetWorkspace(outputDir, ipWorkspace))
		return false;
	// test���[�h�̏ꍇ�̂݌`��t���ō쐬
	if (! m_isTest) {
		// Shape�쐬
		ITablePtr ipTable;
		if (! makeVacuityDbf(shpName, ipWorkspace, ipTable))
			return false;
		// ���R�[�h�쐬
		if (! importRecord(shpName, cnvInfo, ipTable))
			return false;
	} else {
		// Shape�쐬
		IFeatureClassPtr ipFeatureClass;
		if (! makeVacuityShape(shpName, ipWorkspace, ipFeatureClass))
			return false;
		// ���R�[�h�쐬
		if (! importRecord(shpName, cnvInfo, ipFeatureClass))
			return false;
	}
	return true;	
}

bool ShapeMaker::makeDirAndDelFile(long cityMesh, LPCTSTR shpName, CString* outputDir)
{
	// �f�B���N�g���쐬
	long secondMesh = cityMesh/100;
	long firstMesh = secondMesh/100;
	outputDir->Format(_T("%s%d\\%d\\%d\\"), m_outRootDir, firstMesh, secondMesh, cityMesh);
	if (! MakeSureDirectoryPathExists(CT2CA(*outputDir))) {
		CString errMsg;
		errMsg.Format(_T("�f�B���N�g���̍쐬�Ɏ��s���܂����B:%s"), *outputDir);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �t�@�C�������݂��Ă��ꍇ�폜
	CString aExt[4] = { _T("shp"), _T("shx"), _T("dbf"), _T("prj")};
	CString fullFileName;
	for (int i = 0; i < 4; ++i) {
		fullFileName.Format(_T("%s\\%s.%s"),*outputDir, shpName, aExt[i]);
		if (PathFileExists(fullFileName)) {
			if (DeleteFile(fullFileName) == 0) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�@�C���폜�Ɏ��s"), fullFileName);
				return false;
			}
		}
	}
	return true;
}

bool ShapeMaker::makeVacuityDbf(LPCTSTR dbfName, const IWorkspacePtr ipWorkspace, ITablePtr& ipTable)
{
	CString errMsg;
	// �����t�B�[���h�쐬
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! makeAttrFields(dbfName, ipFields))
		return false;
	// DBF�쐬
	if (FAILED(IFeatureWorkspacePtr(ipWorkspace)->CreateTable(CComBSTR(dbfName), ipFields, 0, 0, CComBSTR(_T("")), &ipTable))){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("dbf(��)�쐬�Ɏ��s���܂���"), dbfName);
		return false;
	}
	return true;

}

bool ShapeMaker::makeVacuityShape(LPCTSTR shpName, const IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass)
{
	CString errMsg;
	// �����t�B�[���h�쐬
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! makeAttrFields(shpName, ipFields))
		return false;
	// �`��t�B�[���h�쐬
	IFieldPtr ipGeoField;
	if (! makeShapeField(shpName, shp05::schema::kShape, esriFieldTypeGeometry, ipGeoField))
		return false;
	if (FAILED(ipFields->AddField(ipGeoField))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ǉ��Ɏ��s���܂���"), shpName);
		return false;
	}
	// Shape�쐬
	if (FAILED(IFeatureWorkspacePtr(ipWorkspace)->CreateFeatureClass(CComBSTR(shpName), ipFields,0, 0, esriFTSimple, CComBSTR(shp05::schema::kShape), CComBSTR(_T("")), &ipFeatureClass))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("shape(��)�쐬�Ɏ��s���܂���"), shpName);
		return false;
	}
	return true;
}

bool ShapeMaker::makeAttrFields(LPCTSTR fileName, IFieldsEditPtr& ipFields)
{
	CString errMsg;
	// ����ID
	IFieldPtr ipBldIDField;
	if (! makeAttrField(fileName, shp05::schema::citymap::bld_name::kBldId, esriFieldTypeInteger, VARIANT_TRUE, VARIANT_FALSE, 9, ipBldIDField))
		return false;
	if (FAILED(ipFields->AddField(ipBldIDField))) {
		errMsg.Format(_T("�t�B�[���h�ǉ��Ɏ��s���܂���:%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, fileName);
		return false;
	}
	// ��������
	IFieldPtr ipBldNameField;
	if (! makeAttrField(fileName, shp05::schema::citymap::bld_name::kBldName, esriFieldTypeString, VARIANT_TRUE, VARIANT_FALSE, 254, ipBldNameField))
		return false;
	if (FAILED(ipFields->AddField(ipBldNameField))) {
		errMsg.Format(_T("�t�B�[���h�ǉ��Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, fileName);
		return false;
	}
	return true;
}

bool ShapeMaker::makeShapeField(LPCTSTR shpFileName, LPCTSTR fieldName, esriFieldType fieldType, IFieldPtr &ipGeoField)
{
	IFieldEditPtr ipGeoFieldEdit(CLSID_Field);

	if (FAILED(ipGeoFieldEdit->put_Name(CComBSTR(fieldName))) ||
		FAILED(ipGeoFieldEdit->put_Type(esriFieldTypeGeometry)) ||
		FAILED(ipGeoFieldEdit->put_IsNullable(VARIANT_FALSE))) {
			return false;
	}
	// �ϊ���GeometryDef������擾
	esriGeometryType geometryType;
	long gridCount = 0;

	if (FAILED(m_ipGeometryDef->get_GeometryType(&geometryType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�p��GometryType�擾�Ɏ��s���܂���"), shpFileName);
		return false;
	}
	if (FAILED(m_ipGeometryDef->get_GridCount(&gridCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�p��gridcount�擾�Ɏ��s���܂���"), shpFileName);
		return false;
	}
	std::vector<double> gridSizes;
	for (long n = 0; n < gridCount; ++n) {
		double temp = 0.0;
		if (FAILED(m_ipGeometryDef->get_GridSize(n, &temp))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�p��gridsize�擾�Ɏ��s���܂���"), shpFileName);
			return false;
		}
		gridSizes.push_back(temp);
	}
	// �`��t�B�[���h�쐬
	IGeometryDefEditPtr ipGeometryDefEdit(CLSID_GeometryDef);
	if (FAILED(ipGeometryDefEdit->put_GeometryType(geometryType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ւ�GeometryType�t�^�Ɏ��s���܂���"), shpFileName);
		return false;
	}
	if (gridCount > 0) {
		if (FAILED(ipGeometryDefEdit->put_GridCount(gridCount))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ւ�GridCount�t�^�Ɏ��s���܂���"), shpFileName);
			return false;
		}
		for (long i = 0; i < gridCount; ++i) {
			if (FAILED(ipGeometryDefEdit->put_GridSize(i, gridSizes[i]))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ւ�GridSize�t�^�Ɏ��s���܂���"), shpFileName);
				return false;
			}
		}
	}
	// prj���o�͂��Ȃ��悤�ɁASpatialReference��CLSID_UnknownCoordinateSystem��^����
	IUnknownCoordinateSystemPtr ipUnkSys(CLSID_UnknownCoordinateSystem);
	if (FAILED(ipGeometryDefEdit->putref_SpatialReference(ipUnkSys))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ւ̋�ԎQ�ƍ폜�����Ɏ��s���܂���"), shpFileName);
		return false;
	}
	if (FAILED(ipGeoFieldEdit->putref_GeometryDef(ipGeometryDefEdit))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ւ�GeometryDef�t�^�Ɏ��s���܂���"), shpFileName);
		return false;
	}
	ipGeoField = IFieldPtr(ipGeoFieldEdit);
	return true;
}

bool ShapeMaker::makeAttrField(LPCTSTR shpName, LPCTSTR fieldName, esriFieldType fieldType, VARIANT_BOOL editable, VARIANT_BOOL nullable, long fieldLength, IFieldPtr &ipField)
{
	IFieldEditPtr ipFieldEdit(CLSID_Field);

	// [Arc10.3.1�Ή�] Shape�t�B�[���h�����ّΉ�
	//   Arc10.1�܂Ńt�B�[���h���ݒ��text�t�B�[���h�݂̂ɐݒ肪�L����put_Length()�����{���Ă������߁A
	//   Integer�t�B�[���h�͎w��l�ł͂Ȃ��f�t�H���g����9�ɐݒ肳��Ă����B
	//   Arc10.3.1����d�l�ύX�ɂ��AInteger�t�B�[���h�̃f�t�H���g����9��10�ɕύX���ꂽ���߁A
	//   Integer�t�B�[���h�̏ꍇ��Integer�t�B�[���h���ݒ�ɗL����put_Precision()�ɂ�
	//   Arc10.1�܂łƓ����l�ɂȂ�t�B�[���h�����w�肵�Đݒ肷��B
	if (fieldType == esriFieldTypeInteger) {
		if (FAILED(ipFieldEdit->put_Name(CComBSTR(fieldName))) ||
			FAILED(ipFieldEdit->put_Type(fieldType)) ||
			FAILED(ipFieldEdit->put_Editable(editable)) ||
			FAILED(ipFieldEdit->put_Precision(fieldLength)) ||
			FAILED(ipFieldEdit->put_IsNullable(nullable))) {
			CString errMsg;
			errMsg.Format(_T("�t�B�[���h�쐬�Ɏ��s�F%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
	}
	else {
		if (FAILED(ipFieldEdit->put_Name(CComBSTR(fieldName))) ||
			FAILED(ipFieldEdit->put_Type(fieldType)) ||
			FAILED(ipFieldEdit->put_Editable(editable)) ||
			FAILED(ipFieldEdit->put_Length(fieldLength)) ||
			FAILED(ipFieldEdit->put_IsNullable(nullable))) {
			CString errMsg;
			errMsg.Format(_T("�t�B�[���h�쐬�Ɏ��s�F%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
	}
	ipField = IFieldPtr(ipFieldEdit);
	return true;
}

bool ShapeMaker::importRecord(LPCTSTR dbfName, const IDRecordMap& cnvInfo, ITablePtr& ipTable)
{
	CString errMsg;
	// �t�B�[���h�C���f�b�N�X�擾
	long indexBldID = -1, indexBldName = -1;
	if (FAILED(ipTable->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldId), &indexBldID))) {
		errMsg.Format(_T("index�擾�Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
		return false;
	}
	if (FAILED(ipTable->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldName), &indexBldName))) {
		errMsg.Format(_T("index�擾�Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
		return false;
	}
	// ���R�[�h�o�^
	_ICursorPtr ipCursor;
	if (FAILED(ipTable->Insert(VARIANT_TRUE, &ipCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�J�[�\���C���T�[�g�Ɏ��s���܂���"), dbfName);
		return false;
	}
	for (const auto record : cnvInfo) {
		IRowBufferPtr ipRowBuffer;
		if (FAILED(ipTable->CreateRowBuffer(&ipRowBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���[�o�b�t�@�̍쐬�Ɏ��s���܂���"), dbfName);
			return false;
		}
		// �����t�^
		if (FAILED(ipRowBuffer->put_Value(indexBldID, CComVariant(record.second.e_bldID)))) {
			errMsg.Format(_T("�����t�^�Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldId);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
			return false;
		}
		if (FAILED(ipRowBuffer->put_Value(indexBldName, CComVariant(record.second.e_bldName)))) {
			errMsg.Format(_T("�����t�^�Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
			return false;
		}
		// �C���T�[�g
		CComVariant id;
		if (FAILED(ipCursor->InsertRow(ipRowBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���R�[�h�ǉ��Ɏ��s���܂���"), dbfName);
			return false;
		}
	}
	// �t���b�V��
	if (FAILED(ipCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���R�[�h�̃t���b�V���Ɏ��s���܂���"), dbfName);
		return false;
	}
	return true;
}

bool ShapeMaker::importRecord(LPCTSTR shpName, const IDRecordMap& cnvInfo, IFeatureClassPtr& ipFeatureClass)
{
	CString errMsg;
	// �t�B�[���h�C���f�b�N�X�擾
	long indexBldID = -1, indexBldName = -1;
	if (FAILED(ipFeatureClass->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldId), &indexBldID))) {
		errMsg.Format(_T("index�擾�Ɏ��s���܂���:%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
		return false;
	}
	if (FAILED(ipFeatureClass->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldName), &indexBldName))) {
		errMsg.Format(_T("index�擾�Ɏ��s���܂����F%s"), shp05::schema::citymap::bld_name::kBldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
		return false;
	}
	// ���R�[�h�o�^
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(ipFeatureClass->Insert(VARIANT_TRUE, &ipFeatureCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�J�[�\���C���T�[�g�Ɏ��s���܂���"), shpName);
		return false;
	}
	for (const auto record :cnvInfo) {
		IFeatureBufferPtr ipFeatureBuffer;
		if (FAILED(ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���o�b�t�@�̍쐬�Ɏ��s���܂���"), shpName);
			return false;
		}
		// �����t�^
		if (FAILED(ipFeatureBuffer->put_Value(indexBldID, CComVariant(record.second.e_bldID)))) {
			errMsg.Format(_T("�����t�^�Ɏ��s���܂���:%s"), shp05::schema::citymap::bld_name::kBldId);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
		if (FAILED(ipFeatureBuffer->put_Value(indexBldName, CComVariant(record.second.e_bldName)))) {
			errMsg.Format(_T("�����t�^�Ɏ��s���܂���:%s"), shp05::schema::citymap::bld_name::kBldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
		// �`��t�^
		if (FAILED(ipFeatureBuffer->putref_Shape(record.second.e_ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�^�Ɏ��s���܂���"), shpName);
			return false;
		}
		// �C���T�[�g
		CComVariant id;
		if (FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���R�[�h�ǉ��Ɏ��s���܂���"), shpName);
			return false;
		}
	}
	// �t���b�V��
	if (FAILED(ipFeatureCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���R�[�h�̃t���b�V���Ɏ��s���܂���"), shpName);
		return false;
	}
	return true;
}
