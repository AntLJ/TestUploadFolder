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
	// 出力ファイル名
	CString shpName;
	shpName.Format(_T("%d_%s"), cityMesh, shp05::schema::citymap::bld_name::kSuffix);
	// 出力フォルダ作成&既存ファイル削除
	CString outputDir;
	if (! makeDirAndDelFile(cityMesh, shpName, &outputDir))
		return false;
	// ワークスペースの取得
	IWorkspacePtr ipWorkspace;
	if (! common_proc::GetWorkspace(outputDir, ipWorkspace))
		return false;
	// testモードの場合のみ形状付きで作成
	if (! m_isTest) {
		// Shape作成
		ITablePtr ipTable;
		if (! makeVacuityDbf(shpName, ipWorkspace, ipTable))
			return false;
		// レコード作成
		if (! importRecord(shpName, cnvInfo, ipTable))
			return false;
	} else {
		// Shape作成
		IFeatureClassPtr ipFeatureClass;
		if (! makeVacuityShape(shpName, ipWorkspace, ipFeatureClass))
			return false;
		// レコード作成
		if (! importRecord(shpName, cnvInfo, ipFeatureClass))
			return false;
	}
	return true;	
}

bool ShapeMaker::makeDirAndDelFile(long cityMesh, LPCTSTR shpName, CString* outputDir)
{
	// ディレクトリ作成
	long secondMesh = cityMesh/100;
	long firstMesh = secondMesh/100;
	outputDir->Format(_T("%s%d\\%d\\%d\\"), m_outRootDir, firstMesh, secondMesh, cityMesh);
	if (! MakeSureDirectoryPathExists(CT2CA(*outputDir))) {
		CString errMsg;
		errMsg.Format(_T("ディレクトリの作成に失敗しました。:%s"), *outputDir);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// ファイルが存在してた場合削除
	CString aExt[4] = { _T("shp"), _T("shx"), _T("dbf"), _T("prj")};
	CString fullFileName;
	for (int i = 0; i < 4; ++i) {
		fullFileName.Format(_T("%s\\%s.%s"),*outputDir, shpName, aExt[i]);
		if (PathFileExists(fullFileName)) {
			if (DeleteFile(fullFileName) == 0) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ファイル削除に失敗"), fullFileName);
				return false;
			}
		}
	}
	return true;
}

bool ShapeMaker::makeVacuityDbf(LPCTSTR dbfName, const IWorkspacePtr ipWorkspace, ITablePtr& ipTable)
{
	CString errMsg;
	// 属性フィールド作成
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! makeAttrFields(dbfName, ipFields))
		return false;
	// DBF作成
	if (FAILED(IFeatureWorkspacePtr(ipWorkspace)->CreateTable(CComBSTR(dbfName), ipFields, 0, 0, CComBSTR(_T("")), &ipTable))){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("dbf(器)作成に失敗しました"), dbfName);
		return false;
	}
	return true;

}

bool ShapeMaker::makeVacuityShape(LPCTSTR shpName, const IWorkspacePtr ipWorkspace, IFeatureClassPtr& ipFeatureClass)
{
	CString errMsg;
	// 属性フィールド作成
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! makeAttrFields(shpName, ipFields))
		return false;
	// 形状フィールド作成
	IFieldPtr ipGeoField;
	if (! makeShapeField(shpName, shp05::schema::kShape, esriFieldTypeGeometry, ipGeoField))
		return false;
	if (FAILED(ipFields->AddField(ipGeoField))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド追加に失敗しました"), shpName);
		return false;
	}
	// Shape作成
	if (FAILED(IFeatureWorkspacePtr(ipWorkspace)->CreateFeatureClass(CComBSTR(shpName), ipFields,0, 0, esriFTSimple, CComBSTR(shp05::schema::kShape), CComBSTR(_T("")), &ipFeatureClass))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("shape(器)作成に失敗しました"), shpName);
		return false;
	}
	return true;
}

bool ShapeMaker::makeAttrFields(LPCTSTR fileName, IFieldsEditPtr& ipFields)
{
	CString errMsg;
	// 建物ID
	IFieldPtr ipBldIDField;
	if (! makeAttrField(fileName, shp05::schema::citymap::bld_name::kBldId, esriFieldTypeInteger, VARIANT_TRUE, VARIANT_FALSE, 9, ipBldIDField))
		return false;
	if (FAILED(ipFields->AddField(ipBldIDField))) {
		errMsg.Format(_T("フィールド追加に失敗しました:%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, fileName);
		return false;
	}
	// 建物名称
	IFieldPtr ipBldNameField;
	if (! makeAttrField(fileName, shp05::schema::citymap::bld_name::kBldName, esriFieldTypeString, VARIANT_TRUE, VARIANT_FALSE, 254, ipBldNameField))
		return false;
	if (FAILED(ipFields->AddField(ipBldNameField))) {
		errMsg.Format(_T("フィールド追加に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldName);
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
	// 変換元GeometryDefから情報取得
	esriGeometryType geometryType;
	long gridCount = 0;

	if (FAILED(m_ipGeometryDef->get_GeometryType(&geometryType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド用のGometryType取得に失敗しました"), shpFileName);
		return false;
	}
	if (FAILED(m_ipGeometryDef->get_GridCount(&gridCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド用のgridcount取得に失敗しました"), shpFileName);
		return false;
	}
	std::vector<double> gridSizes;
	for (long n = 0; n < gridCount; ++n) {
		double temp = 0.0;
		if (FAILED(m_ipGeometryDef->get_GridSize(n, &temp))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールド用のgridsize取得に失敗しました"), shpFileName);
			return false;
		}
		gridSizes.push_back(temp);
	}
	// 形状フィールド作成
	IGeometryDefEditPtr ipGeometryDefEdit(CLSID_GeometryDef);
	if (FAILED(ipGeometryDefEdit->put_GeometryType(geometryType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールドへのGeometryType付与に失敗しました"), shpFileName);
		return false;
	}
	if (gridCount > 0) {
		if (FAILED(ipGeometryDefEdit->put_GridCount(gridCount))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールドへのGridCount付与に失敗しました"), shpFileName);
			return false;
		}
		for (long i = 0; i < gridCount; ++i) {
			if (FAILED(ipGeometryDefEdit->put_GridSize(i, gridSizes[i]))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールドへのGridSize付与に失敗しました"), shpFileName);
				return false;
			}
		}
	}
	// prjを出力しないように、SpatialReferenceにCLSID_UnknownCoordinateSystemを与える
	IUnknownCoordinateSystemPtr ipUnkSys(CLSID_UnknownCoordinateSystem);
	if (FAILED(ipGeometryDefEdit->putref_SpatialReference(ipUnkSys))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールドへの空間参照削除処理に失敗しました"), shpFileName);
		return false;
	}
	if (FAILED(ipGeoFieldEdit->putref_GeometryDef(ipGeometryDefEdit))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状フィールドへのGeometryDef付与に失敗しました"), shpFileName);
		return false;
	}
	ipGeoField = IFieldPtr(ipGeoFieldEdit);
	return true;
}

bool ShapeMaker::makeAttrField(LPCTSTR shpName, LPCTSTR fieldName, esriFieldType fieldType, VARIANT_BOOL editable, VARIANT_BOOL nullable, long fieldLength, IFieldPtr &ipField)
{
	IFieldEditPtr ipFieldEdit(CLSID_Field);

	// [Arc10.3.1対応] Shapeフィールド長差異対応
	//   Arc10.1までフィールド長設定はtextフィールドのみに設定が有効なput_Length()を実施していたため、
	//   Integerフィールドは指定値ではなくデフォルト長の9に設定されていた。
	//   Arc10.3.1から仕様変更により、Integerフィールドのデフォルト長が9→10に変更されたため、
	//   Integerフィールドの場合はIntegerフィールド長設定に有効なput_Precision()にて
	//   Arc10.1までと同じ値になるフィールド長を指定して設定する。
	if (fieldType == esriFieldTypeInteger) {
		if (FAILED(ipFieldEdit->put_Name(CComBSTR(fieldName))) ||
			FAILED(ipFieldEdit->put_Type(fieldType)) ||
			FAILED(ipFieldEdit->put_Editable(editable)) ||
			FAILED(ipFieldEdit->put_Precision(fieldLength)) ||
			FAILED(ipFieldEdit->put_IsNullable(nullable))) {
			CString errMsg;
			errMsg.Format(_T("フィールド作成に失敗：%s"), fieldName);
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
			errMsg.Format(_T("フィールド作成に失敗：%s"), fieldName);
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
	// フィールドインデックス取得
	long indexBldID = -1, indexBldName = -1;
	if (FAILED(ipTable->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldId), &indexBldID))) {
		errMsg.Format(_T("index取得に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
		return false;
	}
	if (FAILED(ipTable->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldName), &indexBldName))) {
		errMsg.Format(_T("index取得に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
		return false;
	}
	// レコード登録
	_ICursorPtr ipCursor;
	if (FAILED(ipTable->Insert(VARIANT_TRUE, &ipCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("カーソルインサートに失敗しました"), dbfName);
		return false;
	}
	for (const auto record : cnvInfo) {
		IRowBufferPtr ipRowBuffer;
		if (FAILED(ipTable->CreateRowBuffer(&ipRowBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ローバッファの作成に失敗しました"), dbfName);
			return false;
		}
		// 属性付与
		if (FAILED(ipRowBuffer->put_Value(indexBldID, CComVariant(record.second.e_bldID)))) {
			errMsg.Format(_T("属性付与に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldId);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
			return false;
		}
		if (FAILED(ipRowBuffer->put_Value(indexBldName, CComVariant(record.second.e_bldName)))) {
			errMsg.Format(_T("属性付与に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, dbfName);
			return false;
		}
		// インサート
		CComVariant id;
		if (FAILED(ipCursor->InsertRow(ipRowBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("レコード追加に失敗しました"), dbfName);
			return false;
		}
	}
	// フラッシュ
	if (FAILED(ipCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("レコードのフラッシュに失敗しました"), dbfName);
		return false;
	}
	return true;
}

bool ShapeMaker::importRecord(LPCTSTR shpName, const IDRecordMap& cnvInfo, IFeatureClassPtr& ipFeatureClass)
{
	CString errMsg;
	// フィールドインデックス取得
	long indexBldID = -1, indexBldName = -1;
	if (FAILED(ipFeatureClass->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldId), &indexBldID))) {
		errMsg.Format(_T("index取得に失敗しました:%s"), shp05::schema::citymap::bld_name::kBldId);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
		return false;
	}
	if (FAILED(ipFeatureClass->FindField(CComBSTR(shp05::schema::citymap::bld_name::kBldName), &indexBldName))) {
		errMsg.Format(_T("index取得に失敗しました：%s"), shp05::schema::citymap::bld_name::kBldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
		return false;
	}
	// レコード登録
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(ipFeatureClass->Insert(VARIANT_TRUE, &ipFeatureCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("カーソルインサートに失敗しました"), shpName);
		return false;
	}
	for (const auto record :cnvInfo) {
		IFeatureBufferPtr ipFeatureBuffer;
		if (FAILED(ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィーチャバッファの作成に失敗しました"), shpName);
			return false;
		}
		// 属性付与
		if (FAILED(ipFeatureBuffer->put_Value(indexBldID, CComVariant(record.second.e_bldID)))) {
			errMsg.Format(_T("属性付与に失敗しました:%s"), shp05::schema::citymap::bld_name::kBldId);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
		if (FAILED(ipFeatureBuffer->put_Value(indexBldName, CComVariant(record.second.e_bldName)))) {
			errMsg.Format(_T("属性付与に失敗しました:%s"), shp05::schema::citymap::bld_name::kBldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, shpName);
			return false;
		}
		// 形状付与
		if (FAILED(ipFeatureBuffer->putref_Shape(record.second.e_ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状付与に失敗しました"), shpName);
			return false;
		}
		// インサート
		CComVariant id;
		if (FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("レコード追加に失敗しました"), shpName);
			return false;
		}
	}
	// フラッシュ
	if (FAILED(ipFeatureCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("レコードのフラッシュに失敗しました"), shpName);
		return false;
	}
	return true;
}
