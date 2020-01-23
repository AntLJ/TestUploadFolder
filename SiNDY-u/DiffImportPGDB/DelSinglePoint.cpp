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
#include "DelSinglePoint.h"

using namespace Reflect;

// �R���X�g���N�^
CDelSinglePoint::CDelSinglePoint(void)
{
	m_ExecInfo = _T("�s�v�\���_���̂Q�폜");
}

// �f�X�g���N�^
CDelSinglePoint::~CDelSinglePoint(void)
{
}

// �������֐�
bool CDelSinglePoint::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
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

// ���b�V���P�ʂł̕s�v�\���_�폜�i���̂Q�j�֐�
bool CDelSinglePoint::fnExecMesh(COMMAND cCommand)
{
	//���b�V�����E�i�g�j���擾
	if(!fnGetMesh(true)){
		return false;
	}
	bool aResult = true; 
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		if(g_ShapeType[i] == esriGeometryPolygon){
			//�����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
			m_ipFeatureClass = m_ipFeatureClasses[i];
			aResult &= fnDeleteDisusedPoint(i);
		}
	}
	return aResult;
}

// ���C���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CDelSinglePoint::fnDeleteDisusedPoint(long cIndex)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom, esriSpatialRelOverlaps);
	if(ipCursor== NULL){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature){
		aResult &= fnCheckAndDelete(ipFeature, cIndex);
	}
	return aResult;
}

// �t�B�[�`���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CDelSinglePoint::fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex)
{
	long aObjID;
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_Shape( &ipSrcShape );
	POINT_SET aCheckPoints;

	//�폜���_�Ƃ��̑O��̓_�̏����擾
	bool aResult = fnGetCheckPoints(ipSrcShape, aCheckPoints);
	if(aCheckPoints.size() > 0){
		//�폜��₪��������A�ڂ���|���S��������
		IFeatureCursorPtr ipCursor = fnSelectByShape(ipSrcShape);
		if(ipCursor== NULL){
			return false;
		}
		IFeaturePtr ipTgtFeature;
		POINT_SET aDelPoints;
		while(ipCursor->NextFeature(&ipTgtFeature) == S_OK && ipTgtFeature){
			long aTgtObjID;	
			ipTgtFeature->get_OID(&aTgtObjID);
			if(aObjID == aTgtObjID){
				//������������͂�
				continue;
			}
			//���̑O�ォ��Ȃ郉�C���������Ă��邩�`�F�b�N
			fnFindPairLine(ipTgtFeature, aCheckPoints, aDelPoints);
		}
		if(aDelPoints.size() > 0){
			if(!m_testMode){
				//�|�C���g�폜
				IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aDelPoints, aObjID, cIndex);
				if(ipResult != NULL){
					if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
						fnSetCommonAttr(ipFeature, cIndex, NULL);
						if(SUCCEEDED(ipFeature->Store())){
							POINT_SET::const_iterator aCmp;
							for(aCmp = aDelPoints.begin(); aCmp != aDelPoints.end(); aCmp++){
								fnPrintLogStyle(info, cIndex, aObjID, _T("�s�v�\���_�폜�B"));
							}
							aResult = true;
						}else{
							fnPrintLogStyle(error, cIndex, aObjID, _T("�X�V���s�B"));
						}
					}else{
						fnPrintLogStyle(error, cIndex, aObjID, _T("Shape�ݒ莸�s�I�B"));
					}
				}else{
					aResult = true;
				}
			}else{
				POINT_SET::const_iterator aCmp;
				for(aCmp = aDelPoints.begin(); aCmp != aDelPoints.end(); aCmp++){
					fnPrintLogStyle(info, cIndex, aObjID, _T("�s�v�\���_�폜�\�B"));
				}
				aResult = true;
			}
		}
	}
	return aResult;
}

// �|���S���̍폜���_�Ƃ��̑O��̓_�̏����擾����֐�
bool CDelSinglePoint::fnGetCheckPoints(IGeometryPtr ipGeom, POINT_SET& cCheckPoints)
{
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	if(aPart == 1){
		fnCheckRing(ipGeom, cCheckPoints);
	}else{
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			fnCheckRing(ipSubGeom, cCheckPoints);
		}
	}
	return true;
}

// �폜���|�C���g�̑O�ォ��Ȃ郉�C���������Ă��邩�`�F�b�N���āA�폜�Ώۃ|�C���g�����߂�֐�
bool CDelSinglePoint::fnFindPairLine(IFeaturePtr ipFeature, const POINT_SET& cCheckPoints, POINT_SET& cDeletePoints)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	IPointCollectionPtr ipPointCollection = ipGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount(&aPointCount);
	if(aPointCount > 4){
		std::vector<WKSPoint> aPoint( aPointCount );
		ipPointCollection->QueryWKSPoints( 0, aPointCount, &aPoint.at(0) );
		for(int i = 0; i < aPointCount - 1; i++){
			POINT_SET::const_iterator aCmp;
			for(aCmp = cCheckPoints.begin(); aCmp != cCheckPoints.end(); aCmp++){
				if((fnIsEqual(aCmp->m_Left , aPoint[i]) && fnIsEqual(aCmp->m_Right , aPoint[i+1]))
				|| (fnIsEqual(aCmp->m_Left , aPoint[i+1]) && fnIsEqual(aCmp->m_Right , aPoint[i]))){
					cDeletePoints.insert(*aCmp);
					break;
				}
#if 0 //�K�������ǂ����ʂɂȂ�Ȃ��̂ňꎞ�ۗ�
				if(i < aPointCount - 2 && !fnIsEqual(aCmp->m_Center , aPoint[i+1]) && (aPoint[i+1].X == m_MinX || aPoint[i+1].X == m_MaxX || aPoint[i+1].Y == m_MinY || aPoint[i+1].Y == m_MaxY)){
					if((fnIsEqual(aCmp->m_Left , aPoint[i]) && fnIsEqual(aCmp->m_Right , aPoint[i+2]))
					|| (fnIsEqual(aCmp->m_Left , aPoint[i+2]) && fnIsEqual(aCmp->m_Right , aPoint[i]))){
						cDeletePoints.insert(*aCmp);
						break;
					}					
				}
#endif
			}
		}
	}
	return false;
}

// �s�v�\���_���폜������̌`������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)
IGeometryPtr CDelSinglePoint::fnCroppingShape(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cDeletePoints, cObjID, cIndex);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_ipSpRef);
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = fnCroppingRing(ipSubGeom, cDeletePoints, cObjID, cIndex);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// �i�|���S���𕪉������j�����O�̍폜���_�Ƃ��̑O��̓_�̏����擾����֐�
bool CDelSinglePoint::fnCheckRing(IGeometryPtr ipGeom,  POINT_SET& cCheckPoints)
{
	IPointCollectionPtr ipPointCollection = ipGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount(&aPointCount);
	if(aPointCount > 4){
		//4�_�ȉ��i��������4�_�̎������Ȃ��j�͂���ȏ�폜�ł��Ȃ����猩�Ȃ�
		std::vector<WKSPoint> aPoint( aPointCount );
		ipPointCollection->QueryWKSPoints( 0, aPointCount, &aPoint.at(0) );
		for(int i = 0; i < aPointCount - 1; i++){
			if(aPoint[i].X == m_MinX || aPoint[i].X == m_MaxX
					|| aPoint[i].Y == m_MinY || aPoint[i].Y == m_MaxY){
				int aSt;
				if(i == 0){
					aSt = aPointCount - 2;
				}else{
					aSt = i-1;
				}
				//���b�V�����E��̓_�ɑ΂��A���̑O��̓_���������E��ł͂Ȃ��A���ʂ̃��b�V���Ɋ܂܂�Ă�����̂����Ƃ��đI��
				//���X�꒼�����������̂��A�����E�}�[�W�ɂ���č\���_���ł��Ă��܂����������E������
				//�ׂ荇���I�u�W�F�N�g�������悤�ɕ����E�}�[�W����ĐV���ɓ_���ł����Ȃ�A�����͂Ƃ�Ă���̂ŁA�����Ȃ��̂ŁA
				//���̎��_�ł́A�u�폜���v�ł���
				//�����ɂ܂�����ꍇ��3�_��Y���W�������ꍇ�A��k�ɂ܂�����ꍇ��3�_��X���W�������ꍇ�ɘR���o�O������
				if(aPoint[i].X != aPoint[aSt].X && aPoint[i].X != aPoint[i+1].X
					&& aPoint[i].Y != aPoint[aSt].Y && aPoint[i].Y != aPoint[i+1].Y
					&& fnCalcMeshCode(aPoint[aSt]) != fnCalcMeshCode(aPoint[i+1])){
					CheckPoint aChk;
					aChk.m_Center = aPoint[i];
					aChk.m_Left = aPoint[aSt];
					aChk.m_Right = aPoint[i+1];
					cCheckPoints.insert(aChk);
				}
			}
		}
	}
	return false;
}

// 2�_��WKSPoint��v�m�F�֐�
bool CDelSinglePoint::fnIsEqual(const WKSPoint& aPoint1, const WKSPoint& aPoint2)
{
	if(aPoint1.X == aPoint2.X && aPoint1.Y == aPoint2.Y){
		return true;
	}else{
		return false;
	}
}

// �s�v�\���_���폜������̌`������߂�֐�(�����O�P��)
IGeometryPtr CDelSinglePoint::fnCroppingRing(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex)
{
	IGeometryPtr ipResultGeom = ipSrcGeom;
	bool aDel = false;
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	IPointPtr ipPoint;
	ipPointCollection->get_PointCount( &aPointCount );
	double aBaseX, aBaseY;

	if(aPointCount > 4){
		//4�_�ȉ��i��������4�_�̎������Ȃ��j�͂���ȏ�폜�ł��Ȃ����猩�Ȃ�
		for( long j = 0; j < aPointCount; j++) {
			ipPointCollection->get_Point( j, &ipPoint);
			ipPoint->QueryCoords(&aBaseX, &aBaseY);
			POINT_SET::const_iterator aCmp;
			for(aCmp = cDeletePoints.begin(); aCmp != cDeletePoints.end(); aCmp++){
				if(aBaseX == aCmp->m_Center.X && aBaseY == aCmp->m_Center.Y){
					ipPointCollection->RemovePoints(j, 1);
					if(j == 0){
						aDel = true;
						CString aMsg;
						aMsg.Format(_T("�n�_�폜�I(%d)"), j);
						fnPrintLogStyle(info, cIndex, cObjID, aMsg);
					}else{
						CString aMsg;
						aMsg.Format(_T("�폜�I(%d)"), j);
						fnPrintLogStyle(info, cIndex, cObjID, aMsg);
					}
					aPointCount--;
					j--;
					break;
				}
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
	return ipResultGeom;
}

// �|�C���g�i���W�j����s�s�n�}�̃��b�V���R�[�h���v�Z����֐�
long CDelSinglePoint::fnCalcMeshCode(const WKSPoint& cPoint)
{
	//���W�ϊ����C�u�������甲���{��
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_amari1	= 0.0,	lon_amari1	= 0.0;
	double	lat_amari2	= 0.0,	lon_amari2	= 0.0;
	int		up_1st		= 0,	low_1st		= 0;
	int		up_2nd		= 0,	low_2nd		= 0;
	int		up_64		= 0,	low_64		= 0;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	wid64		= wid2 / 8.0;
	hei64		= hei2 / 8.0;

	
	up_1st		= int(cPoint.Y * 1.5);
	low_1st		= int(cPoint.X - 100);

	lat_amari1	= cPoint.Y - (up_1st / 1.5) ;
	lon_amari1	= cPoint.X - (low_1st + 100);

	up_2nd		= int(lat_amari1 / hei2);
	low_2nd		= int(lon_amari1 / wid2);
	lat_amari2	= lat_amari1 - up_2nd * hei2;
	lon_amari2	= lon_amari1 - low_2nd * wid2;
	up_64		= int(lat_amari2 / hei64);
	low_64		= int(lon_amari2 / wid64);

	long meshcode	= up_1st * 1000000 + low_1st * 10000 +
					up_2nd * 1000 + low_2nd * 100 +
					up_64 * 10 + low_64;
	return meshcode;
}
