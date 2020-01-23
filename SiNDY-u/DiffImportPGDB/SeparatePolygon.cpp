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

#include "StdAfx.h"
#include "SeparatePolygon.h"

using namespace Reflect;

CSeparatePolygon::CSeparatePolygon(void)
{
	m_ExecInfo = _T("�}���`�p�[�g�|���S������");
}

CSeparatePolygon::~CSeparatePolygon(void)
{
}

bool CSeparatePolygon::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
							const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	m_testMode = args.m_testMode;
	m_logFile = args.m_logFile;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		if(g_ShapeType[i] == esriGeometryPolygon){
			//1�ł��|���S��������Ύ��s����
			return true;
		}
	}
	return false;
}

bool CSeparatePolygon::fnExecMesh(COMMAND cCommand)
{
	//���b�V���|���S�����擾
	if(!fnGetMesh(false)){
		return false;
	}
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; i++){
		if(g_ShapeType[i] == esriGeometryPolygon){
			//�����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
			m_ipFeatureClass = m_ipFeatureClasses[i];
			//���b�V�����E�̕s�v�\���_�폜
			fnSeparate(i);
		}
	}
	return true;
}

bool CSeparatePolygon::fnSeparate(long cIndex)
{
	bool aResult = false;
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom);
	if(ipCursor == NULL){
		return aResult;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature){
		fnCheckAndSeparate(ipFeature, cIndex);
	}
	return true;;
}

bool CSeparatePolygon::fnCheckAndSeparate(IFeaturePtr ipFeature, long cIndex)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount;
	if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
		fnPrintComError();
		return false;
	}
	if(aCount > 1){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, aObjID, (string)CT2A(g_LayerName[cIndex]).m_psz, 0, stderr)){
			aCount = aPolUtil.Get_Part_Num();
			if(aCount > 1){
				IFeatureCursorPtr ipInsertCursor;
				if(FAILED(m_ipFeatureClass->Insert(VARIANT_FALSE, &ipInsertCursor))){
					fnPrintComError();
					return false;
				}
				int iPart;
				int aReNew = 0;
				double aMaxArea = 0;
				for(iPart = 0; iPart < aCount; iPart++){
					IPolygonPtr ipSubPoly = aPolUtil.Get_Part_Polygon(iPart+1);
					IAreaPtr ipArea = ipSubPoly;
					if(ipArea != NULL){
						double aArea;
						ipArea->get_Area(&aArea);
						if(aArea > aMaxArea){
							aMaxArea = aArea;
							aReNew = iPart;
						}
					}
				}
				for(iPart = 0; iPart < aCount; iPart++){
					if(iPart == aReNew){
						continue;
					}
					IFeatureBufferPtr ipBuffer;
					if(FAILED(m_ipFeatureClass->CreateFeatureBuffer(&ipBuffer))){
						fnPrintComError();
						return false;
					}
					IPolygonPtr ipSubPoly = aPolUtil.Get_Part_Polygon(iPart+1);
					if(FAILED(ipBuffer->putref_Shape(ipSubPoly))){
						fnPrintComError();
						return false;
					}
					//fnSetChgAttr(ipBuffer);
					fnCopyAttr(ipBuffer, ipFeature, cIndex);
					//fnSetCommonAttr(ipBuffer, cIndex, NULL);
					fnSetCommonAttr(ipBuffer, cIndex, ipFeature);
					CComVariant vIDVariant;
					if(FAILED(ipInsertCursor->InsertFeature(ipBuffer, &vIDVariant))){
						fnPrintComError();
						return false;
					}
					fnPrintLogStyle(info, cIndex, vIDVariant.lVal, _T("�}���`�����ǉ��B"));
				}
				if(FAILED(ipInsertCursor->Flush())){
					fnPrintComError();
					return false;
				}
				//fnSetChgAttr(ipFeature);
				fnSetCommonAttr(ipFeature, cIndex, NULL);
				ipGeom = aPolUtil.Get_Part_Polygon(aReNew+1);
				ipFeature->putref_Shape(ipGeom);
				ipFeature->Store();
				fnPrintLogStyle(info, cIndex, aObjID, _T("�}���`�����X�V�B"));
			}
		}else{
			fnPrintLogStyle(error, cIndex, aObjID, _T("�|���S���̕��͎��s�B"));
			return false;
		}
	}
	return true;
}

bool CSeparatePolygon::fnCopyAttr(IFeatureBufferPtr ipBuffer, IFeaturePtr ipFeature, long cIndex)
{
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; i++){
		CComVariant aSrcVal;
		ipFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &aSrcVal);
		ipBuffer->put_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, aSrcVal);
	}
	return false;
}
