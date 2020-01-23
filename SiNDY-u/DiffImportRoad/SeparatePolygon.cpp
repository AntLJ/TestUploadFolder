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

using namespace sindy::schema;
using namespace Reflect;

// �R���X�g���N�^
CSeparatePolygon::CSeparatePolygon(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("�}���`�p�[�g�|���S������");
}

// �f�X�g���N�^
CSeparatePolygon::~CSeparatePolygon(void)
{
}

// �������֐�
bool CSeparatePolygon::init(void)
{
	bool aResult = false;
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
		//1�ł��|���S��������Ύ��s����
		IFeatureClassPtr ipClass = m_LayerManage->findTable(aEnv->m_OrgTable);
		if(ipClass){
			esriGeometryType aGeometryType;
			ipClass->get_ShapeType(&aGeometryType);
			if(aGeometryType == esriGeometryPolygon){
				//�Ώۂ����X�g�Ǘ�����i���󒆏k�w�i�݂̂����j
				m_TgtList.push_back(aEnv->m_OrgTable);
				aResult = true;
			}
		}
	}
	return aResult;
}

// ���b�V���P�ʂł̃}���`�p�[�g�|���S�������֐�
bool CSeparatePolygon::fnExecMesh(void)
{
	//���b�V���|���S��,�g���擾
	if(!fnGetMesh()){
		return false;
	}
	foreach(const CString& aLayer, m_TgtList){
		EnvTable *aEnv = m_LayerManage->findEnvTable(aLayer);
		//�����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
		m_ipTable[0] = m_LayerManage->findTable(aLayer);
		fnSeparate(*aEnv);
	}
	return true;
}

// ���C�����̃}���`�p�[�g�����֐�
bool CSeparatePolygon::fnSeparate(EnvTable& cEnv)
{
	bool aResult = false;
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshPoly);
	if(!ipCursor){
		return aResult;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature){
		fnCheckAndSeparate(ipFeature, cEnv);
	}
	return true;;
}

// �}���`�p�[�g�ł��邩�̃`�F�b�N�ƕ����֐�
bool CSeparatePolygon::fnCheckAndSeparate(IFeaturePtr& ipFeature, EnvTable& cEnv)
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
		if(aPolUtil.Init(ipGeom, aObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			aCount = aPolUtil.Get_Part_Num();
			if(aCount > 1){
				IFeatureCursorPtr ipInsertCursor;
				if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Insert(VARIANT_FALSE, &ipInsertCursor))){
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
					if(FAILED(((IFeatureClassPtr)m_ipTable[0])->CreateFeatureBuffer(&ipBuffer))){
						fnPrintComError();
						return false;
					}
					IPolygonPtr ipSubPoly = aPolUtil.Get_Part_Polygon(iPart+1);
					if(FAILED(ipBuffer->putref_Shape(ipSubPoly))){
						fnPrintComError();
						return false;
					}
					fnCopyAttr(ipBuffer, ipFeature, cEnv.m_SDEIndexMap);
					fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
					CComVariant vIDVariant;
					if(FAILED(ipInsertCursor->InsertFeature(ipBuffer, &vIDVariant))){
						fnPrintComError();
						return false;
					}
					fnPrintLogStyre(0, cEnv.m_OrgTable, vIDVariant.lVal, _T("�}���`�����ǉ�"), NULL, TGT_LAYER);
				}
				if(FAILED(ipInsertCursor->Flush())){
					fnPrintComError();
					return false;
				}
				fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
				ipGeom = aPolUtil.Get_Part_Polygon(aReNew+1);
				ipFeature->putref_Shape(ipGeom);
				ipFeature->Store();
				fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("�}���`�����X�V"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�|���S���̕��͎��s"), NULL, TGT_LAYER);
			return false;
		}
	}
	return true;
}

// �p�����鑮���̃R�s�[�֐�
bool CSeparatePolygon::fnCopyAttr(IFeatureBufferPtr& ipBuffer, const IFeaturePtr& ipFeature, INDEX_MAP& cSDEIndex)
{
	for(INDEX_MAP::const_iterator it = cSDEIndex.begin(); it != cSDEIndex.end(); ++it){
		//�ŏI�X�V���t�͔��f�ΏۊO�A�ŏI�X�V�҂͌�Ŏw��l�œ��ꂷ��̂ŏ��O�AINF_ID�͕ʓr�X�V����̂ŏ��O
		//��ƖړI�A�X�V���e�i�H�j���ΏۊO�i����v�]�j
		//�������r�́A�R���e�i�ɓ����ۂɑ啶���ɂ��Ă���̂ƁA�X�L�[�}���啶������̂͂��Ȃ̂Łu==�v�ł����͂�
		if(it->first == ipc_table::kOperator || it->first == ipc_table::kModifyDate || it->first == link_queue::kInfID
			|| it->first == ipc_table::kPurpose || it->first == ipc_table::kUpdateType){
			continue;
		}
		CComVariant aVal;
		ipFeature->get_Value(cSDEIndex[it->first].m_Index, &aVal);
		ipBuffer->put_Value(it->second.m_Index, aVal);
	}
	return true;
}
