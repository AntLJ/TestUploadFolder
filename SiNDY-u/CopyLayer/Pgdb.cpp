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

// Pgdb.cpp: Pgdb クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pgdb.h"

namespace {
	LPCTSTR DATASET_NAME = "city";

	LPCTSTR gFrmTables[] = {"City_Site", "City_Line", "City_Area", "City_Tunnel", "City_Road", NULL};
	LPCTSTR gHigTables[] = {"Building", "Building_Line", "Building_Step", "Building_Roof", NULL};
	LPCTSTR gRalTables[] = {"City_Station", "City_Railway", NULL};
	LPCTSTR gAdmTables[] = {"City_Admin", "Gou_Point", "CS_Addr_Point", NULL};
	LPCTSTR gSvyTables[] = {"City_Survey_Point",  NULL};
	LPCTSTR gAnoTables[] = {"City_Annotation", "City_Disp_Line", NULL};
	LPCTSTR gShoTables[] = {"Mapple_Chiban", "Mapple_HAL", NULL};

	struct EnvTable {
		LPCTSTR	m_Suffix;
		LPCTSTR	*m_OrgTables;
	};

	const EnvTable gEnvTable[] = {
		{"hig", gHigTables},
		{"frm", gFrmTables},
		{"ral", gRalTables},
		{"adm", gAdmTables},
		{"svy", gSvyTables},
		{"avc", gAnoTables},
		{"sho", gShoTables},
		{NULL, NULL}
	};

	//出力情報テーブル用(ExportPgdbと同じにすること)
	LPCTSTR HISTRY_TABLE_NAME = "EXPORT_INFO";

	LPCTSTR OBJECTID_NAME = "OBJECTID";
	LPCTSTR NAME_NAME = "NAME";
	LPCTSTR FEATURECLASS_NAME = "FEATURECLASS";
	LPCTSTR	CONNECT_INFO_NAME = "CONNECT_INFO";
	LPCTSTR	EXP_USER_NAME = "USER";
	LPCTSTR	EXP_MACHINE_NAME = "MACHINE";
	LPCTSTR	EXP_DATE_NAME = "EXP_DATE";
	LPCTSTR	EXP_OPERATION_NAME = "OPETATION";

	struct CommonNewFieldTable {
		LPCTSTR m_FieldName;
		esriFieldType m_FieldType;
		LONG m_Length;
	};

	CommonNewFieldTable gHistryField[] = {
		{OBJECTID_NAME, esriFieldTypeOID, 32},
		{NAME_NAME, esriFieldTypeString, 32},
		{FEATURECLASS_NAME, esriFieldTypeString, 32},
		{CONNECT_INFO_NAME, esriFieldTypeString, 64},
		{EXP_USER_NAME, esriFieldTypeString, 16},
		{EXP_MACHINE_NAME, esriFieldTypeString, 16},
		{EXP_DATE_NAME, esriFieldTypeDate, 4},
		{EXP_OPERATION_NAME, esriFieldTypeString, 4},
		{NULL, esriFieldType(0), 0}
	};
}
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Pgdb::Pgdb()
{

}

Pgdb::~Pgdb()
{

}

bool Pgdb::init(LPCTSTR cSrcDir, LPCTSTR cTgtDir, LPCTSTR cSuffix, bool cForceMode)
{
	m_DataIndex = -1;
	for(int i = 0; gEnvTable[i].m_Suffix != NULL; i++){
		if(CString(cSuffix) == gEnvTable[i].m_Suffix){
			m_DataIndex = i;
			break;
		}
	}
	if(m_DataIndex < 0){
		CString aSuffList = gEnvTable[0].m_Suffix;
		for(int i = 1; gEnvTable[i].m_Suffix != NULL; i++){
			aSuffList += ",";
			aSuffList += gEnvTable[i].m_Suffix;
		}
		fprintf(stderr, "拡張子には %s のいずれかを指定して下さい。\n", static_cast<LPCTSTR>(aSuffList));
		return false;
	}
	m_SrcDir = cSrcDir;
	m_TgtDir = cTgtDir;
	m_ForceMode = cForceMode;

	if(m_SrcDir == m_TgtDir){
		printf("Error,コピー元とコピー先のディレクトリが同じ。,%s\n", cSrcDir);
		fprintf(stderr, "Error,コピー元とコピー先のディレクトリが同じ。,%s\n", cSrcDir);
		return false;
	}
	if(::_access(cSrcDir, 4) == -1){
		printf("Error,コピー元ディレクトリが存在しないか、アクセス権が足りません。,%s\n", cSrcDir);
		fprintf(stderr, "Error,コピー元ディレクトリが存在しないか、アクセス権が足りません。,%s\n", cSrcDir);
		return false;
	}

	if(::_access(cTgtDir, 6) == -1){
		printf("Error,コピー先ディレクトリが存在しないか、アクセス権が足りません。,%s\n", cTgtDir);
		fprintf(stderr, "Error,コピー先ディレクトリが存在しないか、アクセス権が足りません。,%s\n", cTgtDir);
		return false;
	}
	return true;
}

bool Pgdb::execCopyLayer(LONG cMeshCode)
{
	m_MeshCode = cMeshCode;
	IWorkspacePtr ipSrcWorkspace = fnOpenLocalDatabase(m_SrcDir);
	if(ipSrcWorkspace == NULL){
		return false;
	}
	IWorkspacePtr ipTgtWorkspace = fnOpenLocalDatabase(m_TgtDir);
	if(ipTgtWorkspace == NULL){
		return false;
	}

	//Create the source workspace name from the source workspace object.
	IDatasetPtr sourceWorkspaceDataset = (IDatasetPtr)ipSrcWorkspace;
	INamePtr ipSrcName;
	sourceWorkspaceDataset->get_FullName(&ipSrcName);
	IWorkspaceNamePtr sourceWorkspaceName = ipSrcName;

	//Create the target workspace name from the target workspace object.
	IFeatureDatasetPtr ipDataSet;
	IDatasetPtr targetWorkspaceDataset;
	if(SUCCEEDED(((IFeatureWorkspacePtr)ipTgtWorkspace)->OpenFeatureDataset(CComBSTR(DATASET_NAME), &ipDataSet))){
		targetWorkspaceDataset = ipDataSet;
	}else{
		printf("Warning,データセットが開けません。\n");
		fprintf( stderr, "Warning,データセットが開けません。\n");
		targetWorkspaceDataset = (IDatasetPtr)ipTgtWorkspace;
	}

	INamePtr ipTgtName;
	targetWorkspaceDataset->get_FullName(&ipTgtName);
//	IWorkspaceNamePtr targetWorkspaceName = ipTgtName;

    IEnumNamePtr fromName(CLSID_NamesEnumerator);
	for(int i = 0; gEnvTable[m_DataIndex].m_OrgTables[i] != NULL; i++){
		IFeatureWorkspacePtr ipSrcFeatureWorkspace = ipSrcWorkspace;
		IFeatureClassPtr ipSrcFeatureClass;
		if(ipSrcFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gEnvTable[m_DataIndex].m_OrgTables[i]))
			, &ipSrcFeatureClass) != S_OK){
			fprintf(stderr, "Error,元フィーチャクラスが開けません[%s].。,%d\n", gEnvTable[m_DataIndex].m_OrgTables[i], m_MeshCode);
			return false;				
		}
		IFeatureWorkspacePtr ipTgtFeatureWorkspace = ipTgtWorkspace;
		IFeatureClassPtr ipTgtFeatureClass;
		if(ipTgtFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(gEnvTable[m_DataIndex].m_OrgTables[i]))
			, &ipTgtFeatureClass) == S_OK){
			if(m_ForceMode){
				if(FAILED(((IDatasetPtr)ipTgtFeatureClass)->Delete())){
					fprintf(stderr, "Error,既存フィーチャクラス削除失敗[%s],%d\n", gEnvTable[m_DataIndex].m_OrgTables[i], m_MeshCode);
					return false;				
				}
				ipTgtFeatureClass = NULL;
			}else{
				fprintf(stderr, "Error,フィーチャクラスが存在します[%s].。,%d\n", gEnvTable[m_DataIndex].m_OrgTables[i], m_MeshCode);
				return false;				
			}
		}
		IDatasetNamePtr datasetName(CLSID_FeatureClassName);
		datasetName->putref_WorkspaceName(sourceWorkspaceName);
		datasetName->put_Name(CComBSTR(gEnvTable[m_DataIndex].m_OrgTables[i]));
        IEnumNameEditPtr editFromName;
        editFromName = (IEnumNameEditPtr)fromName;
        editFromName->Add((INamePtr) datasetName);
	}
    INamePtr toName = ipTgtName;
	IGeoDBDataTransferPtr geoDBDataTransfer(CLSID_GeoDBDataTransfer);
    IEnumNameMappingPtr fromNameMapping;
	VARIANT_BOOL targetObjectNameExists;
	if(FAILED(geoDBDataTransfer->GenerateNameMapping(fromName, toName, &fromNameMapping, &targetObjectNameExists))){
		fprintf(stderr, "Error,GenerateNameMapping失敗,%d\n", m_MeshCode);
		return false;						
	}
	if(targetObjectNameExists){
		fprintf(stderr, "Error,フィーチャクラスが存在します。,%d\n", m_MeshCode);
		return false;						
	}
	if(FAILED(geoDBDataTransfer->Transfer(fromNameMapping, toName))){
		fprintf(stderr, "Error,コピー失敗。,%d\n", m_MeshCode);
		return false;						
	}
	//最後にエクスポート情報の更新
	ITablePtr ipSrcInfoTable = fnOpenExportInfoTable(ipSrcWorkspace, 0);
	ITablePtr ipTgtInfoTable = fnOpenExportInfoTable(ipTgtWorkspace, 1);
	for(int i = 0; gEnvTable[m_DataIndex].m_OrgTables[i] != NULL; i++){
		fnSetExportInfo(ipSrcInfoTable, ipTgtInfoTable, i);
	}
	return true;
}

IWorkspacePtr Pgdb::fnOpenLocalDatabase(LPCTSTR cBasePath)
{
	CString aDataBase;
	aDataBase.Format("%s\\%04d\\%08d.mdb", cBasePath, m_MeshCode/10000, m_MeshCode);

	// パーソナルジオDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;
	// オープンしてみる
	ipWorkspaceFactory->OpenFromFile( CComBSTR( aDataBase ), 0, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		printf("Error,%s が開けません。\n", static_cast<LPCTSTR>(aDataBase));
		fprintf( stderr, "Error,%s が開けません。\n", static_cast<LPCTSTR>(aDataBase));
		return NULL;
	}
	return ipWorkspace;

}

ITablePtr Pgdb::fnOpenExportInfoTable(IWorkspacePtr ipWorkspace, int cMode)
{
	//cMode = 0:コピー元用
	//cMode = 1:コピー先用
	IFeatureWorkspacePtr ipFeatureWorkspace = ipWorkspace;
	ITablePtr ipOutTable;

	if(ipFeatureWorkspace->OpenTable(CComBSTR(HISTRY_TABLE_NAME), &ipOutTable) != S_OK && cMode == 1){
		// フィールド作成
		IFieldsEditPtr  ipFieldsEd(CLSID_Fields);
		for(int i = 0; gHistryField[i].m_FieldName != NULL; i++){
			IFieldEditPtr ipFieldEdit(CLSID_Field);
			ipFieldEdit->put_Name(CComBSTR(gHistryField[i].m_FieldName));
			ipFieldEdit->put_Type(gHistryField[i].m_FieldType);
			ipFieldEdit->put_Required( VARIANT_TRUE );
			ipFieldEdit->put_IsNullable(VARIANT_FALSE);
			ipFieldEdit->put_DomainFixed( VARIANT_FALSE );
			ipFieldEdit->put_Length(gHistryField[i].m_Length);
			// フィールド追加
			if(FAILED(ipFieldsEd->AddField(ipFieldEdit))){
				fprintf(stderr, "Error,情報テーブル作成時のフィールド追加失敗。,%d\n", m_MeshCode);
				return NULL;
			}
		}

		// テーブル作成
		if(FAILED(ipFeatureWorkspace->CreateTable( CComBSTR(HISTRY_TABLE_NAME), ipFieldsEd, NULL, NULL, CComBSTR(_T("")), &ipOutTable ))){
			fprintf(stderr, "Error,情報テーブル作成失敗。,%d\n", m_MeshCode);
			return NULL;
		}
	}
	if(ipOutTable != NULL){
		LONG aID;
		if(FAILED(((IClassSchemaEditPtr)ipOutTable)->RegisterAsObjectClass (CComBSTR(OBJECTID_NAME), CComBSTR(""), &aID))){
			fprintf(stderr, "Error,情報テーブル登録失敗。,%d\n", m_MeshCode);
		}
		ipOutTable->FindField(CComBSTR(NAME_NAME), &m_NameIndex[cMode]);
		ipOutTable->FindField(CComBSTR(FEATURECLASS_NAME), &m_ClassIndex[cMode]);
		ipOutTable->FindField(CComBSTR(CONNECT_INFO_NAME), &m_ConnectIndex[cMode]);
		ipOutTable->FindField(CComBSTR(EXP_USER_NAME), &m_UserIndex[cMode]);
		ipOutTable->FindField(CComBSTR(EXP_MACHINE_NAME), &m_MachineIndex[cMode]);
		ipOutTable->FindField(CComBSTR(EXP_DATE_NAME), &m_DateIndex[cMode]);
		ipOutTable->FindField(CComBSTR(EXP_OPERATION_NAME), &m_OperationIndex[cMode]);
	}
	return ipOutTable;
}

bool Pgdb::fnSetExportInfo(ITablePtr ipSrcTbl, ITablePtr ipTgtTbl, int cIndex)
{
	_IRowPtr ipSrcRow = fnGetRow(ipSrcTbl, gEnvTable[m_DataIndex].m_OrgTables[cIndex]);
	_IRowPtr ipTgtRow = fnGetRow(ipTgtTbl, gEnvTable[m_DataIndex].m_OrgTables[cIndex]);
	if(ipTgtRow != NULL){
		fnCopyInfo(ipTgtRow, ipSrcRow);
		ipTgtRow->Store();
	}else{
		IRowBufferPtr ipBuffer;
		ipTgtTbl->CreateRowBuffer(&ipBuffer);
		fnCopyInfo(ipBuffer, ipSrcRow);
		//名前はセットされない
		ipBuffer->put_Value(m_NameIndex[1], CComVariant(gEnvTable[m_DataIndex].m_OrgTables[cIndex]));
		_ICursorPtr ipCursor;
		ipTgtTbl->Insert(VARIANT_FALSE, &ipCursor);
		CComVariant aID;
		if(FAILED(ipCursor->InsertRow(ipBuffer, &aID))){
			fprintf(stderr, "Error,情報テーブル追加失敗。,%d\n", m_MeshCode);
			return false;
		}		
	}

	return false;
}

_IRowPtr Pgdb::fnGetRow(ITablePtr ipTable, LPCTSTR cLayerName)
{
	if(ipTable == NULL){
		return NULL;
	}
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("%s = '%s'", NAME_NAME, cLayerName);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	_ICursorPtr ipCursor;
	ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(ipCursor == NULL){
		return NULL;
	}

	_IRowPtr ipRow;
	ipCursor->NextRow(&ipRow);
	return ipRow;
}

bool Pgdb::fnCopyInfo(_IRowPtr ipTgtRow, _IRowPtr ipSrcRow)
{
	if(ipSrcRow != NULL){
		CComVariant aFeatureClass, aUser, aConnect, aMachine, aDate;
		ipSrcRow->get_Value(m_ClassIndex[0], &aFeatureClass);
		ipTgtRow->put_Value(m_ClassIndex[1], aFeatureClass);

		ipSrcRow->get_Value(m_UserIndex[0], &aUser);
		ipTgtRow->put_Value(m_UserIndex[1], aUser);

		ipSrcRow->get_Value(m_ConnectIndex[0], &aConnect);
		ipTgtRow->put_Value(m_ConnectIndex[1], aConnect);

		ipSrcRow->get_Value(m_MachineIndex[0], &aMachine);
		ipTgtRow->put_Value(m_MachineIndex[1], aMachine);

		ipSrcRow->get_Value(m_DateIndex[0], &aDate);
		ipTgtRow->put_Value(m_DateIndex[1], aDate);
	}
	ipTgtRow->put_Value(m_OperationIndex[1], CComVariant("CPY"));
	return true;
}
