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
#include "MeshMerge.h"

using namespace sindy::schema;
using namespace Reflect;

namespace{
	double ACCEPTABLE_X  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�o�x�j
	double ACCEPTABLE_Y  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�ܓx�j
	long BG_EXCEPT_LIST[] = {101001, 101002, 101003, 102003, -1};	///< �w�i�̃}�[�W���O��ʁi�C�A�͐�E��E�J�A�΁E�r�E���A��O�n�j
}

// �R���X�g���N�^
CMeshMerge::CMeshMerge(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("�s�v�\���_�폜�E���b�V�����E�}�[�W");
}

// �f�X�g���N�^
CMeshMerge::~CMeshMerge(void)
{
}

// �������֐�
bool CMeshMerge::init(void)
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
	if(aResult){
		//1�ł��|���S��������Ύ��s����
		if(_tgetenv(_T("ACCEPTABLE")) == NULL){
			_tprintf(_T("�}�[�W���덷�����e���܂���\n"));
			_ftprintf(stderr, _T("#�}�[�W���덷�����e���܂���\n"));
			ACCEPTABLE_X = ACCEPTABLE_Y = 0.0;
		}else{
			_tprintf(_T("�}�[�W���኱�̌덷�͋��e���܂�\n"));
			_ftprintf(stderr, _T("#�}�[�W���኱�̌덷�͋��e���܂�\n"));
		}
		//�w�i�p�̃}�[�W���O���X�g��set�Ɋi�[
		for(int i = 0; BG_EXCEPT_LIST[i] > 0; ++i){
			m_BGTarget.insert(BG_EXCEPT_LIST[i]);
		}
	}
	return aResult;
}

// ���b�V���P�ʂł̋��E��̕s�v�\���_�폜�Ƌ��E�}�[�W�֐�
bool CMeshMerge::fnExecMesh(void)
{
	//���b�V�����E�i�g�j���擾
	if(!fnGetMesh()){
		return false;
	}
	foreach(const CString& aLayer, m_TgtList){
		EnvTable *aEnv = m_LayerManage->findEnvTable(aLayer);
		//�����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
		m_ipTable[0] = m_LayerManage->findTable(aLayer);

		//���b�V�����E�̕s�v�\���_�폜
		fnDeleteDisusedPoint(*aEnv);

		//���b�V�����E�}�[�W
		bool aLoop = true; 
		while(aLoop){
			// ���b�V�����E���shape���}�[�W����
			aLoop = fnShapeMerge(*aEnv);
		}
		m_IDList.clear();
	}
	return true;
}

// ���C���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CMeshMerge::fnDeleteDisusedPoint(EnvTable& cEnv)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshBoundary);
	if(ipCursor== NULL){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aResult &= fnCheckAndDelete(ipFeature, cEnv);
	}
	return aResult;
}

// �t�B�[�`���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CMeshMerge::fnCheckAndDelete(IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	long aObjID;	
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_ShapeCopy( &ipSrcShape );
	bool aResult = false;
	int aCount = 0;
	IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aCount);
	if(aCount > 0 && ipResult != NULL){
		if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
			fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
			if(SUCCEEDED(ipFeature->Store())){
				fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("�s�v�\���_�폜"), NULL, TGT_LAYER);
				aResult = true;
			}else{
				fnPrintComError();
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�s�v�\���_�폜�X�V���s"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�s�v�\���_�폜 Shape�ݒ莸�s"), NULL, TGT_LAYER);
		}
	}else{
		aResult = true;
	}
	return aResult;
}

// �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
IGeometryPtr CMeshMerge::fnCroppingShape(const IGeometryPtr& ipSrcGeom, int& cCount)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	cCount = 0;
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cCount);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_LayerManage->get_SpRef());
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = fnCroppingRing(ipSubGeom, cCount);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐��i�����O�P�ʁj�B
IGeometryPtr CMeshMerge::fnCroppingRing(const IGeometryPtr& ipSrcGeom, int& cCount)
{
	//���b�V���̃R�[�i�[�̓_�́A�ꌩ�s�v�����A�}�[�W�O�ɂ͕K�v�ɂȂ�ꍇ��
	//���邽�߁A�c���悤�ɕύX�B
	//�c���͉���3���b�V���ȏ�ɂ킽��|���S���̏ꍇ�A���b�V���R�[�i�[��
	//��������������Ȃ��߁A�e���b�V���͈͓̔��݂̂�ΏۂƂ���悤�ύX
	IGeometryPtr ipResultGeom = ipSrcGeom;
	bool aDel = false;
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	IPointPtr ipPoint;
	ipPointCollection->get_PointCount( &aPointCount );
	if(aPointCount > 4){
		for( int j = 0; j < aPointCount - 2; ++j) {
			double aBaseX, aBaseY;
			double aNextX1, aNextX2, aNextY1, aNextY2;
			ipPointCollection->get_Point( j, &ipPoint);
			ipPoint->QueryCoords(&aBaseX, &aBaseY);
			if(aBaseX == m_MinX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MinX && aNextX2 == m_MinX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
						ipPointCollection->RemovePoints(j+1, 1);
						_tprintf(_T("�폜�I(%d)\n"), j+1);
						++cCount;
						--aPointCount;
						j--;
						continue;
					}
				}
			}else if(aBaseX == m_MaxX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("�폜�I(%d)\n"), j+1);
						++cCount;
						--j;
						continue;
					}
				}
			}
			if(aBaseY == m_MinY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MinY && aNextY2 == m_MinY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						++cCount;
						_tprintf(_T("�폜�I(%d)\n"), j+1);
						--j;
					}
				}
			}else if(aBaseY == m_MaxY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("�폜�I(%d)\n"), j+1);
						++cCount;
						--j;
					}
				}
			}
		}
	}
	//�n�_,�I�_���폜�ΏۂƂȂ鎞�̏���
	if(aPointCount > 4){
		double aBaseX, aBaseY;
		double aNextX1, aNextX2, aNextY1, aNextY2;
		ipPointCollection->get_Point( 0, &ipPoint);
		ipPoint->QueryCoords(&aBaseX, &aBaseY);
		if(aBaseX == m_MinX){
			ipPointCollection->get_Point( 1, &ipPoint); //�n�_�̎�
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);	//�I�_��1�O
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MinX && aNextX2 == m_MinX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("�폜�I(�I�_)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("�폜�I(�n�_)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseX == m_MaxX){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("�폜�I(�I�_)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("�폜�I(�n�_)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aBaseY == m_MinY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MinY && aNextY2 == m_MinY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("�폜�I(�I�_)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("�폜�I(�n�_)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseY == m_MaxY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//�������b�V���̃R�[�i�[���������b�V���͈͓��Ȃ�
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("�폜�I(�I�_)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("�폜�I(�n�_)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aDel){
			IRingPtr ipRing = ipResultGeom;
			if(ipRing != NULL){
				ipRing->Close();
			}else{
				IPolygonPtr ipPolygon = ipResultGeom;
				if(ipPolygon != NULL){
					ipPolygon->Close();
					ipPolygon->SimplifyPreserveFromTo();
				}else{
					ipResultGeom = NULL;
				}
			}
		}
	}
	return ipResultGeom;
}

// ���C���P�ʂŃ��b�V�����E�̃}�[�W������֐�
bool CMeshMerge::fnShapeMerge(EnvTable& cEnv)
{
	bool aMerge = false;
	IFeatureCursorPtr	ipCursor;
	if((ipCursor = fnSelectByShape(m_ipMeshBoundary)) == NULL){
		return false;
	}

	IFeaturePtr ipFeature;
	while(!aMerge){
		if(ipCursor->NextFeature(&ipFeature) != S_OK){
			break;
		}
		long aObjID;
		ipFeature->get_OID(&aObjID);
		if(m_IDList.find(aObjID) == m_IDList.end()){
			//���X�g�ɂȂ���Ύ��s
			if(fnIsTargetKind(ipFeature, cEnv) && fnSerchTargetsAndMerge(ipFeature, cEnv)){
				aMerge = true;
				break;
			}else{
				//�}�[�W�ΏۊO�̑������A����ȏ�}�[�W������̂��Ȃ���΃��X�g�ɒǉ�
				m_IDList.insert(aObjID);
			}
		}
	}
	return aMerge;
}

// �t�B�[�`�����Ƀ}�[�W�\�Ȃ��̂��}�[�W����֐�
bool CMeshMerge::fnSerchTargetsAndMerge(IFeaturePtr& ipSrcFeature, EnvTable& cEnv)
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_Shape( &ipGeom );

	long aSrcObjID;
	ipSrcFeature->get_OID(&aSrcObjID);
	IFeatureCursorPtr ipFeatureCursor = fnSelectByShape(ipGeom);
	if(ipFeatureCursor == NULL){
		return false;
	}
	long aObjID;
	IFeaturePtr ipTgtFeature;
	int aResult;
	bool aRet = false;
	while(ipFeatureCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aObjID);
		if(aSrcObjID == aObjID){
			continue;
		}
		if((aResult = fnIsNeedMerge(ipSrcFeature, ipTgtFeature, cEnv)) == 1){
			if(fnCompAttr(ipSrcFeature, ipTgtFeature, cEnv)){
				bool aMergeResult;
				//ID�̏��������ɋz������B
				if(aSrcObjID < aObjID){
					aMergeResult= fnMerge(ipSrcFeature, ipTgtFeature, cEnv);
				}else{
					aMergeResult= fnMerge(ipTgtFeature, ipSrcFeature, cEnv);
				}
				if(aMergeResult){
					CString aMsg;
					aMsg.Format(_T("%d"), aSrcObjID);
					fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("�}�[�W���܂���"), aMsg, TGT_LAYER);
					aRet = true;
					break;
				}
			}else{
				CString aMsg;
				aMsg.Format(_T("%d"), aSrcObjID);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�אڂ��鑮�����Ⴄ"), aMsg, TGT_LAYER);
			}
		}else if(aResult == 0){
//			printf("Not Merge:%d,%d\n", cObjID, aObjID);
		}else{
			CString aMsg;
			aMsg.Format(_T("%d"), aSrcObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�}�[�W�ł��܂���ł���"), aMsg, TGT_LAYER);
			aRet = false;
			break;
		}
	}
	return aRet;
}

// �`��I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���b�V�����E��Ő��������L���Ă��邱�Ɓj
int CMeshMerge::fnIsNeedMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
	//�|���S���̎������Ă΂�Ȃ�
	IGeometryPtr ipSrcGeom, ipTgtGeom;

	if(FAILED(ipSrcFeature->get_Shape( &ipSrcGeom ))){
		long aObjID;
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("SrcShape�̎擾���s"), NULL, TGT_LAYER);
		return -1;
	}
	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom ))){
		long aObjID;
		ipTgtFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("TgtShape�̎擾���s"), NULL, TGT_LAYER);
		return -1;
	}

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	long aPointCountS = 0;
	long aPointCountT = 0;
	ipPointCollectionS->get_PointCount( &aPointCountS );
	ipPointCollectionT->get_PointCount( &aPointCountT );

	if(aPointCountS > 3 && aPointCountT > 3){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for(int i = 0; i < aPointCountS -1; i++){
			if(aSrcPoints[i].X == m_MinX && aSrcPoints[i+1].X == m_MinX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MinX  && aTgtPoints[j+1].X == m_MinX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].X == m_MaxX && aSrcPoints[i+1].X == m_MaxX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MaxX  && aTgtPoints[j+1].X == m_MaxX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MinY && aSrcPoints[i+1].Y == m_MinY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MinY  && aTgtPoints[j+1].Y == m_MinY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MaxY && aSrcPoints[i+1].Y == m_MaxY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MaxY  && aTgtPoints[j+1].Y == m_MaxY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

//�@�����I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���߂�ꂽ��������v���邱�Ɓj
// (���k�w�i�Œ�ɂȂ��Ă��܂����̂ŁA�Ώۂ��������璲�����K�v)
bool CMeshMerge::fnCompAttr(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
#if 0
	//�ėp�I�ɂ���ɂ́A����Ȋ����̕����������E�E�i�s�s�n�}�p�Ȃ̂ŁA���̂܂܂ł̓G���[�j
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Name != NULL; i++){
		if(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Compare){
			CComVariant aSrcVal, aTgtVal;
			ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SinIndex, &aSrcVal);
			ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SinIndex, &aTgtVal);
			if(aSrcVal != aTgtVal){
				return false;
			}
		}
	}
#else
	CComVariant aSrcVal, aTgtVal;
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}

#endif
	return true;
}

// �Е����}�[�W�����`��ɍX�V���A�Е����폜���āA�}�[�W��̏�Ԃ����֐��B
bool CMeshMerge::fnMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
	long aObjID;
	IGeometryPtr ipSrcGeom, ipTgtGeom;
	bool aResult = false;
	if(FAILED(ipSrcFeature->get_ShapeCopy(&ipSrcGeom))){
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("SrcShape�̎擾���s"), NULL, TGT_LAYER);
		return false;
	}

	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom ))){
		ipTgtFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("TgtShape�̎擾���s"), NULL, TGT_LAYER);
		return false;
	}
	ITopologicalOperatorPtr  ipSrcTopo = ipSrcGeom;
	IGeometryPtr ipTgtGeom2;
	if(ipSrcTopo->Union(ipTgtGeom, &ipTgtGeom2) ==  S_OK){
		if(ipTgtGeom2 != NULL){
			ipTgtGeom2->putref_SpatialReference(m_LayerManage->get_SpRef());
			((ITopologicalOperator2Ptr)ipTgtGeom2)->put_IsKnownSimple(VARIANT_FALSE);
			((ITopologicalOperatorPtr)ipTgtGeom2)->Simplify();
			if(SUCCEEDED(ipSrcFeature->putref_Shape(ipTgtGeom2))){
				fnSetCommonAttr((_IRowPtr)ipSrcFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
				if(SUCCEEDED(ipSrcFeature->Store())){
					if(SUCCEEDED(ipTgtFeature->Delete())){
						aResult = true;
					}else{
						ipTgtFeature->get_OID(&aObjID);
						fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�t�B�[�`���[�폜���s"), NULL, TGT_LAYER);
					}
				}else{
					ipSrcFeature->get_OID(&aObjID);
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�t�B�[�`���[�X�V���s"), NULL, TGT_LAYER);
					fnPrintComError();
				}
			}else{
				ipSrcFeature->get_OID(&aObjID);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�}�[�W��Shape�ݒ莸�s"), NULL, TGT_LAYER);
			}
		}else{
			ipSrcFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�}�[�W���s"), NULL, TGT_LAYER);
		}
	}else{
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("Union���s"), NULL, TGT_LAYER);
	}
	return aResult;
}

// ���e�͈͂������������W�i�l�j�̈�v�m�F�֐�
bool CMeshMerge::fnCompXY(double cPoint1, double cPoint2, double cAcceptable)
{
	if(fabs(cPoint1 - cPoint2) <= cAcceptable){
		return true;
	}else{
		return false;
	}
}

// �}�[�W�Ώۑ������𔻒肷��֐�(���k�w�i�Œ�ɂȂ��Ă��܂����̂ŁA�Ώۂ��������璲�����K�v)
bool CMeshMerge::fnIsTargetKind(const IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	if(!fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index)){
			return false;
	}
	return true;
}

// �w��t�B�[�`���̎w�葮�������O�}�[�W�������𔻒肷��֐��i���k�w�i����j
bool CMeshMerge::fnIsTargetBaseKind(const IFeaturePtr& ipFeature, long cIndex)
{
	CComVariant aVal;
	ipFeature->get_Value(cIndex, &aVal);
	if(m_BGTarget.find(aVal.lVal) != m_BGTarget.end()){
		//�}�[�W���O���X�g�ɂ����
		return false;
	}
	return true;
}
