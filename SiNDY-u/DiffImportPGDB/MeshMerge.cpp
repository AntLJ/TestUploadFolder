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
#include "merge_target.h"

using namespace Reflect;
using namespace sindy::schema;

namespace{
	double ACCEPTABLE_X  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�o�x�j
	double ACCEPTABLE_Y  = 0.000001;	///< ���b�V�����E��̍��W���r���鎞�̌덷���e�Ɏg�p�����l�i�ܓx�j
}

// �R���X�g���N�^
CMeshMerge::CMeshMerge(void) : 
	m_FloorsIndex(),
	m_BGClsIndex(),
	m_AreaClsIndex(),
	m_AdmClsIndex()
{
	m_ExecInfo = _T("�s�v�\���_�폜�E���b�V�����E�}�[�W");
}

// �f�X�g���N�^
CMeshMerge::~CMeshMerge(void)
{
}

// �������֐�
bool CMeshMerge::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
					  const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	m_testMode = args.m_testMode;
	m_logFile = args.m_logFile;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		if( g_ShapeType[i] == esriGeometryPolygon || g_ShapeType[i] == esriGeometryPolyline )
		{
			//1�ł��|���S���E�|�����C��������Ύ��s����
			if(_tgetenv(_T("ACCEPTABLE")) == NULL)
			{
				_tprintf(_T("�}�[�W���덷�����e���܂���\n"));
				_ftprintf(stderr, _T("#�}�[�W���덷�����e���܂���\n"));
				ACCEPTABLE_X = ACCEPTABLE_Y = 0.0;
			}
			else
			{
				_tprintf(_T("�}�[�W���኱�̌덷�͋��e���܂�\n"));
				_ftprintf(stderr, _T("#�}�[�W���኱�̌덷�͋��e���܂�\n"));
			}

			//�w�i�p�̃}�[�W�Ώۃ��X�g��set�Ɋi�[
			//�w�i���C���p�̃}�[�W�Ώۃ��X�g��set�Ɋi�[
			//�s�s�G���A�p�̃}�[�W�Ώۃ��X�g��set�Ɋi�[
			if(CString(gEnvTable[g_DataIndex].m_Suffix).CompareNoCase(_T("frm")) == 0)
			{
				for( const auto& bgClass : CS_TARGET_LIST ){
					m_CSTarget.insert(bgClass);
				}
				for( const auto& bgClass : CL_TARGET_LIST ){
					m_CLTarget.insert(bgClass);
				}
				for( const auto& areaClass : AP_TARGET_LIST ){
					m_APTarget.insert(areaClass);
				}
			}

			//�s���E�p�̃}�[�W�Ώۃ��X�g��set�Ɋi�[
			if(CString(gEnvTable[g_DataIndex].m_Suffix).CompareNoCase(_T("adm")) == 0){
				for( const auto& admClass : ADM_TARGET_LIST ){
					m_ADMTarget.insert(admClass);
				}
			}

			return true;
		}
	}
	return false;
}

// ���b�V���P�ʂł̋��E��̕s�v�\���_�폜�Ƌ��E�}�[�W�֐�
bool CMeshMerge::fnExecMesh(COMMAND cCommand)
{
	_ftprintf(stderr, _T("#%d,%s\n"), g_CurrentMesh, _T("���b�V�����E�}�[�W"));
	//���b�V�����E�i�g�j���擾
	if(!fnGetMesh(true))
	{
		return false;
	}

	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		// City_Road�͒�����|���S�����ł���\���������̂Ń}�[�W�ΏۊO
		CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
		if( 0 == table.CompareNoCase(city_road::kTableName) )
		{
			_tprintf(_T("CITY_ROAD�̓}�[�W�ΏۊO\n"));
			continue;
		}

		// �|���S���͑S���`�F�b�N
		// TODO: ���Suffix���Ŕ��肵�āA���Ȃ��Ă������̂͌��Ȃ��悤�ɂ���
		if(g_ShapeType[i] == esriGeometryPolygon)
		{
			// �����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
			m_ipFeatureClass = m_ipFeatureClasses[i];

			// ����ȏ����E��r�����鑮��������΃C���f�b�N�X���擾���Ă���
			// �K���͍������ɂ��킹��
			// ��ʃR�[�h�̓t�B�[�`���N���X�ɂ��}�[�W���Ȃ��R�[�h������(merge_target.h�Q��)

			// �K���H
			long aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(building_step::kFloors), &aIndex);
			m_FloorsIndex[i] = aIndex;

			// �s�s�w�i�H
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_site::kBgClass), &aIndex);
			m_BGClsIndex[i] = aIndex;

			// �s�s�G���A�H
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_area::kAreaClass), &aIndex);
			m_AreaClsIndex[i] = aIndex;

			// �s���E�H
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_admin::kAreaClass), &aIndex);
			m_AdmClsIndex[i] = aIndex;

			// ���b�V�����E�̕s�v�\���_�폜
			fnDeleteDisusedPoint(i);

			// ���b�V�����E�}�[�W
			bool aLoop = true;
			while(aLoop){
				// ���b�V�����E���shape���}�[�W����
				aLoop = fnShapeMerge(i);
			}
		}
		else if(g_ShapeType[i] == esriGeometryPolyline)
		{
			// bug 10519 �Ń��C���n�t�B�[�`�����}�[�W����悤�ɂ���

			// Building_Line, City_Line�ȊO�̓}�[�W�ΏۊO
			CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
			if( 0 != table.CompareNoCase(building_line::kTableName) &&
				0 != table.CompareNoCase(city_line::kTableName) )
			{
				cout << (CStringA)table << "�F�}�[�W�ΏۊO" << endl;
				continue;
			}
			cout << (CStringA)table << "�F�}�[�W����" << endl;

			// �����Ώۃt�B�[�`���N���X�ݒ�i�������Ɏg�p�j
			m_ipFeatureClass = m_ipFeatureClasses[i];

			// City_Line�͗v��ʊm�F
			if( 0 == table.CompareNoCase(city_line::kTableName) )
			{
				long aIndex = -1;
				m_ipFeatureClass->FindField(CComBSTR(city_line::kBgClass), &aIndex);
				m_CLClsIndex[i] = aIndex;
			}

			// ���b�V�����E�}�[�W
			bool aLoop = true;
			while(aLoop){
				// ���b�V�����E���shape���}�[�W����
				aLoop = fnShapeMerge(i);
			}
		}

		for( auto& oid : m_deleteIDList )
		{
			IFeaturePtr ipDeleteTarget;
			m_ipFeatureClass->GetFeature( oid, &ipDeleteTarget );

			IGeometryPtr ipDeleteGeom;
			ipDeleteTarget->get_ShapeCopy( &ipDeleteGeom );

			if(FAILED(ipDeleteTarget->Delete()))
			{
				fnPrintLogStyle(error, i, oid, _T("�t�B�[�`���[�폜���s�B"));
				continue;
			}
		}
	}
	return true;
}

// ���C���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CMeshMerge::fnDeleteDisusedPoint(long cIndex)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom);
	if( !ipCursor ){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aResult &= fnCheckAndDelete(ipFeature, cIndex);
	}
	return aResult;
}

// �t�B�[�`���P�ʂ̃��b�V�����E��̕s�v�\���_�폜�֐�
bool CMeshMerge::fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex)
{
	long aObjID = 0;
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_ShapeCopy( &ipSrcShape );
	bool aResult = false;
	int aCount = 0;
	IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aCount);
	if(aCount > 0 && ipResult != NULL){
		if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
			fnSetCommonAttr(ipFeature, cIndex, NULL);
			if(SUCCEEDED(ipFeature->Store())){
				fnPrintLogStyle(info, cIndex, aObjID, _T("�s�v�\���_�폜�B"));
				aResult = true;
			}else{
				fnPrintComError();
				fnPrintLogStyle(error, cIndex, aObjID, _T("�s�v�\���_�폜�X�V���s�B"));
			}
		}else{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, aObjID, _T("�s�v�\���_�폜 Shape�ݒ莸�s�B"));
		}
	}else{
		aResult = true;
	}

	return aResult;
}

// �s�v�\���_���폜������̌`��ƍ폜�����|�C���g�������߂�֐�(�K�v�ɉ����ă|���S���������O�ɕ������čč\������)�B
IGeometryPtr CMeshMerge::fnCroppingShape(IGeometryPtr ipSrcGeom, int& cCount)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart = 0;
	ipGeoCol->get_GeometryCount(&aPart);
	
	cCount = 0;
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cCount);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_ipSpRef);
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
IGeometryPtr CMeshMerge::fnCroppingRing(IGeometryPtr ipSrcGeom, int& cCount)
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
bool CMeshMerge::fnShapeMerge(long cIndex/*, const IFeatureCursorPtr& ipCursor*/)
{
	// ���b�V�����E�ŕ�����}�[�W����K�v�̂���I�u�W�F�N�g�Ƃ����̂����X�L��
	// ������ A�ƃ}�[�W���`���蒼����B�ƃ}�[�W���E�E�E �Ə�������K�v�����邽�߁A
	// �J�[�\�������x����蒼���悤�ȏ����ɂȂ��Ă���B

	bool mergeSuccess = false;
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom);
	if( !ipCursor )
	{
		return false;
	}

	m_mergedIDList.clear();
	IFeaturePtr ipFeature;
	while( !mergeSuccess )
	{
		if( ipCursor->NextFeature(&ipFeature) != S_OK || !ipFeature )
		{
			break;
		}
		long aObjID = 0;
		ipFeature->get_OID(&aObjID);
		if(m_mergedIDList.find(aObjID) == m_mergedIDList.end())
		{
			//���X�g�ɂȂ���Ύ��s
			if( fnIsTargetKind(ipFeature, cIndex) )
			{
				if( fnSearchTargetsAndMerge(ipFeature, cIndex) )
				{
					mergeSuccess = true;
					break;
				}
			}
			else
			{
				// �}�[�W�ΏۊO�̑����ł���΃��X�g�ɒǉ�
				m_mergedIDList.insert(aObjID);
			}
		}
	}

	return mergeSuccess;
}

// �t�B�[�`�����Ƀ}�[�W�\�Ȃ��̂��}�[�W����֐�
bool CMeshMerge::fnSearchTargetsAndMerge(IFeaturePtr ipSrcFeature, long cIndex)
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_Shape( &ipGeom );

	long srcObjID = 0;
	ipSrcFeature->get_OID( &srcObjID );

	IFeatureCursorPtr ipFeatureCursor = fnSelectByShape( ipGeom );
	if( !ipFeatureCursor )
		return false;

	IFeaturePtr ipTgtFeature;
	bool aRet = false;
	std::map<long,CAdapt<IFeaturePtr>> targetFeatures; // key:oid val:IFeature
	while( ipFeatureCursor->NextFeature( &ipTgtFeature ) == S_OK && ipTgtFeature )
	{
		long tgtObjID = 0;
		ipTgtFeature->get_OID( &tgtObjID );
		if( srcObjID == tgtObjID )
			continue;

		int aResult = -1;
		aResult = fnIsNeedMerge( ipSrcFeature, ipTgtFeature, cIndex );
		switch( aResult )
		{
		case 2:
			{
				// �������̃��b�V���ł͂Ȃ����b�V�����E�ň�v���Ă���ꍇ(3���b�V�����炢�܂����ł���Ƃ��Ȃ�)
				aRet = false;
				break;
			}
		case 1:
			{
				targetFeatures[tgtObjID] = ipTgtFeature;
				break;
			}
		case 0:
			{
#if 0 // ��ʂɏo�邩����Ƀ��b�Z�[�W�͏o���Ȃ��ł���(���X�������Ă��݂�������)
				CString aMsg;
				aMsg.Format(_T("���b�V�����E�Ō`��s��v�I\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, tgtObjID, aMsg);
#endif
				break;
			}
		default:
			{
				CString aMsg;
				aMsg.Format(_T("�}�[�W�ł��܂���ł����I\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, tgtObjID, aMsg);
				aRet = false;
				break;
			}
		}
	}

	// ���C���n�̓��b�V�����E���1�_��3�ȏ�ڂ��Ă���G���[�ɂ��Ă����܂��ɂ���B
	esriGeometryType geomType = esriGeometryNull;
	ipGeom->get_GeometryType( &geomType );
	if( geomType == esriGeometryPolyline )
	{
		// targetFeatures ��2�ȏゾ�ƃ��b�V�����E��̂���1�_��3�{�ȏ�ڂ��Ă���
		if( targetFeatures.size() > 1 )
		{
			CString aMsg;
			aMsg.Format(_T("���b�V�����E�Őڂ��郉�C����3�{�ȏ゠�邽�߃}�[�W���܂���\t%d"), srcObjID);
			fnPrintLogStyle(info, cIndex, srcObjID, aMsg);

			// �܂Ƃ߂ă`�F�b�N�ς݈����ɂ���
			for( auto& target : targetFeatures )
			{
				m_mergedIDList.insert( target.first );
			}
			return false;
		}
	}

	for( auto& target : targetFeatures )
	{
		IFeaturePtr ipMergeFeature = target.second;
		if( fnCompAttr( ipSrcFeature, ipMergeFeature, cIndex ) )
		{
			bool aMergeResult = false;
			//ID�̏��������ɋz������B
			if( srcObjID < target.first )
			{
				aMergeResult = fnMerge( ipSrcFeature, ipMergeFeature, cIndex );
			}
			else
			{
				aMergeResult = fnMerge( ipMergeFeature, ipSrcFeature, cIndex );
			}

			if( aMergeResult )
			{
				CString aMsg;
				aMsg.Format(_T("�}�[�W���܂���\t%d"), srcObjID);
				fnPrintLogStyle(info, cIndex, target.first, aMsg);
				aRet = true;
			}
		}
		else
		{
			if( m_FloorsIndex[cIndex] != -1 )
			{
				//�K�����݂̂̈Ⴂ�͓��ʂɋz��
				long aFloors = 0;
				if( ( aFloors = fnCompAttrSp( ipSrcFeature, ipMergeFeature, cIndex ) ) > 0 )
				{
					bool aMergeResult = false;
					//ID�̏��������ɋz������B
					if( srcObjID < target.first )
					{
						//�K���͍����������
						ipSrcFeature->put_Value(m_FloorsIndex[cIndex], CComVariant(aFloors));
						aMergeResult= fnMerge(ipSrcFeature, ipMergeFeature, cIndex);
					}
					else
					{
						ipMergeFeature->put_Value(m_FloorsIndex[cIndex], CComVariant(aFloors));
						aMergeResult= fnMerge(ipMergeFeature, ipSrcFeature, cIndex);
					}

					if( aMergeResult )
					{
						CString aMsg;
						aMsg.Format(_T("�}�[�W���܂���\t%d"), srcObjID);
						fnPrintLogStyle(info, cIndex, target.first, aMsg);
						aRet = true;
					}
				}
				else
				{
					CString aMsg;
					aMsg.Format(_T("�אڂ��鑮�����Ⴄ�I�i�K���ȊO���j\t%d"), srcObjID);
					fnPrintLogStyle(error, cIndex, target.first, aMsg);
				}
			}
			else
			{
				CString aMsg;
				aMsg.Format(_T("�אڂ��鑮�����Ⴄ�I\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, target.first, aMsg);
			}
		}
	}

	return aRet;
}

// �`��I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i���b�V�����E��Ő��������L���Ă��邱�Ɓj
int CMeshMerge::fnIsNeedMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	IGeometryPtr ipSrcGeom, ipTgtGeom;

	long srcObjID = 0;
	ipSrcFeature->get_OID(&srcObjID);
	if(FAILED(ipSrcFeature->get_Shape( &ipSrcGeom )))
	{
		fnPrintLogStyle(error, cIndex, srcObjID, _T("SrcShape�̎擾���s�B"));
		return -1;
	}

	long tgtObjID = 0;
	ipTgtFeature->get_OID(&tgtObjID);
	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom )))
	{
		fnPrintLogStyle(error, cIndex, tgtObjID, _T("TgtShape�̎擾���s�B"));
		return -1;
	}

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	long aPointCountS = 0;
	long aPointCountT = 0;
	ipPointCollectionS->get_PointCount( &aPointCountS );
	ipPointCollectionT->get_PointCount( &aPointCountT );

	// �ꉞ�����̃W�I���g���^�C�v�m�F
	esriGeometryType srcGeomType = esriGeometryNull;
	esriGeometryType tgtGeomType = esriGeometryNull;
	ipSrcGeom->get_GeometryType( &srcGeomType );
	ipTgtGeom->get_GeometryType( &tgtGeomType );

	if( srcGeomType == esriGeometryPolygon && tgtGeomType == esriGeometryPolygon ){
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
	}else if( srcGeomType == esriGeometryPolyline && tgtGeomType == esriGeometryPolyline ){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for( auto srcPoint : aSrcPoints ){
			if( srcPoint.X == m_MinX ){
				// ���̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == m_MinX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.X == m_MaxX ){
				// �E�̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == m_MaxX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.Y == m_MinY ){
				// ���̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == m_MinY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.Y == m_MaxY ){
				// ��̃��b�V���̃I�u�W�F�N�g�ƃ}�[�W
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == m_MaxY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
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
bool CMeshMerge::fnCompAttr(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i)
	{
		FieldTable fieldTable = gEnvTable[g_DataIndex].m_Fields[cIndex][i];
		// ��r���Ȃ��ėǂ��t�B�[���h�Ȃ��΂�
		if( !fieldTable.m_Compare )
			continue;

		long index = 0;
		index = fieldTable.m_SindyIndex;
		CComVariant aSrcVal, aTgtVal;
		ipSrcFeature->get_Value(index, &aSrcVal);
		ipTgtFeature->get_Value(index, &aTgtVal);

		// GEOSPACE_ID�͓���Ή�
		if( 0 == CString(building::kGeospaceID).CompareNoCase(fieldTable.m_fieldName) )
		{
			long aSrcObjID = 0, aTgtObjID = 0;
			ipSrcFeature->get_OID(&aSrcObjID);
			ipTgtFeature->get_OID(&aTgtObjID);

			// ����NULL�Ȃ�}�[�W����
			if( aSrcVal.vt == VT_NULL && aTgtVal.vt == VT_NULL )
				return true;

			// �Е���NULL�Ȃ�}�[�W����(fnMerge()�̓���ς����炱����������������)
			// OBJECTID�Ⴂ�����c��̂ŁAOBJECTID���Ⴂ�ق���NULL�Ȃ�GEOSPACE_ID�����Ƃ��Ă��K�v������
			if( aSrcVal.vt != VT_NULL && aTgtVal.vt == VT_NULL )
			{
				if( aTgtObjID < aSrcObjID )
					ipTgtFeature->put_Value( index, aSrcVal );
				fnPrintLogStyle(info, cIndex, aTgtObjID, _T("GEOSPACE_ID��NULL�̌����ƃ}�[�W�B"));
				return true;
			}
			if( aSrcVal.vt == VT_NULL && aTgtVal.vt != VT_NULL )
			{
				if( aSrcObjID < aTgtObjID )
					ipSrcFeature->put_Value( index, aTgtVal );
				fnPrintLogStyle(info, cIndex, aSrcObjID, _T("GEOSPACE_ID��NULL�̌����ƃ}�[�W�B"));
				return true;
			}
			// �����l�������Ă��ē����l�ł���΃}�[�W����
			if( aSrcVal.vt != VT_NULL && aTgtVal.vt != VT_NULL )
			{
				// �l�Ⴄ�Ȃ烍�O�Ƀ��b�Z�[�W���o��
				if(aSrcVal != aTgtVal)
				{
					fnPrintLogStyle(info, cIndex, aTgtObjID, _T("GEOSPACE_ID���قȂ邽�߃}�[�W���܂���B"));
					return false;
				}
				return true;
			}
		}
		else
		{
			// GEOSPACE_ID�ȊO�͒P���ɔ�r
			if(aSrcVal != aTgtVal)
			{
				return false;
			}
		}
	}
	return true;
}

// �Е����}�[�W�����`��ɍX�V���A�Е����폜���āA�}�[�W��̏�Ԃ����֐��B
bool CMeshMerge::fnMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	long aObjID = 0;
	ipSrcFeature->get_OID(&aObjID);

	IGeometryPtr ipSrcGeom, ipTgtGeom;

	if(FAILED(ipSrcFeature->get_ShapeCopy( &ipSrcGeom )))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("SrcShape�̎擾���s�B"));
		return false;
	}

	if(FAILED(ipTgtFeature->get_ShapeCopy( &ipTgtGeom ))) // �ύX������킯�ł͖�������ǔO�̂���get_ShapeCopy���g��
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("TgtShape�̎擾���s�B"));
		return false;
	}

	ITopologicalOperatorPtr ipSrcTopo = ipSrcGeom;
	IGeometryPtr ipTgtGeom2;
	if( FAILED(ipSrcTopo->Union(ipTgtGeom, &ipTgtGeom2)) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("Union���s�B"));
		return false;
	}

	if( !ipTgtGeom2 )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("�}�[�W���s�B"));
		return false;
	}

	if( FAILED(ipTgtGeom2->putref_SpatialReference(m_ipSpRef)) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("putref_SpatialReference���s�B"));
		return false;
	}

	if(FAILED(((ITopologicalOperator2Ptr)ipTgtGeom2)->put_IsKnownSimple(VARIANT_FALSE)))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("put_IsKnownSimple���s�B"));
		return false;
	}

	if(FAILED(((ITopologicalOperatorPtr)ipTgtGeom2)->Simplify()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("Simplify���s�B"));
		return false;
	}

	if(FAILED(ipSrcFeature->putref_Shape(ipTgtGeom2)))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("�}�[�W��Shape�ݒ莸�s�B"));
		return false;
	}

	if( !fnSetCommonAttr(ipSrcFeature, cIndex, NULL) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("SetCommonAttr()���s�B"));
		return false;
	}

	if(FAILED(ipSrcFeature->Store()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("�t�B�[�`���[�X�V���s�B"));
		fnPrintComError();
		return false;
	}

	if(FAILED(ipTgtFeature->Delete()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("�t�B�[�`���[�폜���s�B"));
		return false;
	}

	return true;
}

// �����I�Ƀ}�[�W�ΏۂɂȂ邩�𔻒肷��֐��i������������ʈ����������́j
int CMeshMerge::fnCompAttrSp(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	int aResult = -1;
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i)
	{
		// i�Ԗڂ���r�Ώۃt�B�[���h�łȂ���Δ�΂�
		if( !gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Compare )
			continue;

		CComVariant vaValueSrc, vaValueTgt;
		ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &vaValueSrc);
		ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &vaValueTgt);

		// �K���ɈႢ������ꍇ�͍������ɍ��킹��
		if(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex == m_FloorsIndex[cIndex])
		{
			if(vaValueSrc.lVal > vaValueTgt.lVal)
			{
				aResult = vaValueSrc.lVal;
			}
			else
			{
				aResult = vaValueTgt.lVal;
			}
		}
		else
		{
			// �K���ȊO�͒P���ɔ�r
			if(vaValueSrc != vaValueTgt)
			{
				return -1;
			}
		}
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

// �}�[�W�Ώۑ������𔻒肷��֐�
bool CMeshMerge::fnIsTargetKind(const IFeaturePtr& ipFeature, const long cIndex)
{
	CString table = gEnvTable[g_DataIndex].m_OrgTableNames[cIndex];
	if(m_BGClsIndex[cIndex] >= 0 && table.CompareNoCase( city_site::kTableName ) == 0){
		// �w�i�|���S���Ȃ�
		CComVariant aVal;
		ipFeature->get_Value(m_BGClsIndex[cIndex], &aVal);
		if(m_CSTarget.find(aVal.lVal) == m_CSTarget.end()){
			// �}�[�W�Ώۃ��X�g�ɂȂ����
			return false;
		}
	}else if(m_AreaClsIndex[cIndex] >= 0 && table.CompareNoCase( city_area::kTableName ) == 0){
		// �s�s�G���A�Ȃ�
		CComVariant aVal;
		ipFeature->get_Value(m_AreaClsIndex[cIndex], &aVal);
		if(m_APTarget.find(aVal.lVal) == m_APTarget.end()){
			// �}�[�W�Ώۃ��X�g�ɂȂ����
			return false;
		}
	}else if(m_AdmClsIndex[cIndex] >= 0 && table.CompareNoCase( city_admin::kTableName ) == 0){
		// �s���E�Ȃ�
		CComVariant aVal;
		ipFeature->get_Value(m_AdmClsIndex[cIndex], &aVal);
		if(m_ADMTarget.find(aVal.lVal) == m_ADMTarget.end()){
			// �}�[�W�Ώۃ��X�g�ɂȂ����
			return false;
		}
	}else if(m_CLClsIndex[cIndex] >= 0 && table.CompareNoCase( city_line::kTableName ) == 0){
		// City_Line
		CComVariant aVal;
		ipFeature->get_Value(m_CLClsIndex[cIndex], &aVal);
		if(m_CLTarget.find(aVal.lVal) == m_CLTarget.end()){
			// �}�[�W�Ώۃ��X�g�ɂȂ����
			return false;
		}
	}
	// ��{�I�ɂ͑S���}�[�W
	return true;
}
