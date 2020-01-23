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
#include <ArcHelperEx/GlobalFunctions.h>
#include <coord_converter.h>
#include "Pgdb.h"

namespace {

	//�����Ώۃe�[�u��
	LPCTSTR gBuildTable[] = {
		_T("BUILDING"),
		_T("BUILDING_STEP"),
		_T("BUILDING_LINE")
	};

	crd_cnv cnv;	///< ���W�ϊ��N���X
}

// �R���X�g���N�^
CPgdb::CPgdb(void)
: m_pFile(NULL)
, m_TargetPath(_T(""))
, m_MarkPath(_T(""))
, m_Word(_T(""))
, m_BuildIndex(-1)
{
}

// �f�X�g���N�^
CPgdb::~CPgdb(void)
{
}

// �������E���O����
bool CPgdb::init(LPCTSTR cTargetPath, LPCTSTR cMarkPath, LPCTSTR cDelWord, LPCTSTR cDate1, LPCTSTR cDate2, FILE *pFile)
{
	if(pFile){
		m_pFile = pFile;
	}else{
		//�ʏ킱���ɂ͗��Ȃ��݌v������
		m_pFile = stderr;
	}
	if(_tcslen(cDelWord) < 1){
		_ftprintf(m_pFile, _T("#ERROR �폜�������[�h���s�K�؂ł��B,�u%s�v\n"), cDelWord);
		_tprintf( _T("ERROR �폜�������[�h���s�K�؂ł��B,�u%s�v\n"), cDelWord);
		return false;
	}
	m_Word = cDelWord;
	
	//�t�H���_�̑��݂����`�F�b�N
	if(_taccess(cTargetPath, 0x04) == 0){
		m_TargetPath = cTargetPath;
	}else{
		_ftprintf(m_pFile, _T("#ERROR �^�[�Q�b�g�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), cTargetPath);
		_tprintf(_T("ERROR �^�[�Q�b�g�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), cTargetPath);
		return false;
	}
	if(_taccess(cMarkPath, 0x04) == 0){
		m_MarkPath = cMarkPath;
	}else{
		_ftprintf(m_pFile, _T("#ERROR �}�[�L���O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), cMarkPath);
		_tprintf(_T("ERROR �}�[�L���O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), cMarkPath);
		return false;
	}

	//���t���������t�^��
	if(_tcslen(cDate1) > 1){
		m_Date[0] = cDate1;
		m_Date[0].ChangeType(VT_DATE);
		_ftprintf(m_pFile, _T("#�������ƌ`�p�̓��t����F,%s\n"), cDate1);
	}else{
		m_Date[0].vt = VT_NULL;
		_ftprintf(m_pFile, _T("#�������ƌ`�p�̓��t����͐ݒ肳��Ă��܂���B\n"));
	}
	if(_tcslen(cDate2) > 1){
		m_Date[1] = cDate2;
		m_Date[1].ChangeType(VT_DATE);
		_ftprintf(m_pFile, _T("#�����t���ƌ`�p�̓��t����F,%s\n"), cDate2);
	}else{
		m_Date[1].vt = VT_NULL;
		_ftprintf(m_pFile, _T("#�����t���ƌ`�p�̓��t����͐ݒ肳��Ă��܂���B\n"));
	}
	return true;
}

// �}�[�N�ƌ`�폜�̃��C����
bool CPgdb::execDeleteKakei(LPCTSTR cMeshList)
{
	FILE *pFile;
	if((pFile = _tfopen(cMeshList, _T("rt"))) == NULL){
		_ftprintf(m_pFile, _T("#ERROR ���b�V�����X�g���J���܂���B,%s\n"), cMeshList);
		_tprintf( _T("ERROR ���b�V�����X�g���J���܂���B,%s\n"), cMeshList);
		return false;
	}
	char aBuf[32];
	while(fgets(aBuf, 32, pFile) != NULL){
		if(aBuf[0] != '#'){	//�@�擪#�̓R�����g����
			fnDeleteKakeiMesh(atol(aBuf));
		}
	}
	fclose(pFile);
	return true;
}

// ���b�V���P�ʂőΏۉƌ`���폜
bool CPgdb::fnDeleteKakeiMesh(long cMeshCode)
{
	_tprintf(_T("Now %d\n"), cMeshCode);
	IWorkspacePtr ipTgtWorkspace = fnOpenLocalDataBase(m_TargetPath, cMeshCode);
	if(!ipTgtWorkspace){
		return false;
	}
	IWorkspacePtr ipMarkWorkspace = fnOpenLocalDataBase(m_MarkPath, cMeshCode);
	if(!ipMarkWorkspace){
		return false;
	}
	IFeatureClassPtr ipGou = fnOpenFeatureClass(ipMarkWorkspace, _T("GOU_POINT"));
	if(!ipGou){
		return false;
	}
	for(int i = 0; i < 3; ++i){
		m_ipBuild[i] = fnOpenFeatureClass(ipTgtWorkspace, gBuildTable[i]);
		if(!m_ipBuild[i]){
			return false;
		}
		m_ipBuild[i]->FindField(CComBSTR(_T("ORG_MODIFYDATE")), &m_ModDateIndex[i]);
		if(m_ModDateIndex[i] < 0){
			_ftprintf(m_pFile, _T("#ERROR �t�B�[���h��������܂���uORG_MODIFYDATE�v�B,%d,%s\n"), cMeshCode, gBuildTable[i]);
			return false;
		}
	}

	m_ipBuild[0]->FindField(CComBSTR(_T("BLDCLASS_C")), &m_BuildIndex);
	if(m_BuildIndex < 0){
		_ftprintf(m_pFile, _T("#ERROR �t�B�[���h��������܂���uBLDCLASS_C�v�B,%d,%s\n"), cMeshCode, gBuildTable[0]);
		return false;
	}

	//��ɑΏۂ̒��o����
	std::vector<ID_SET> aIds(3);
	if(!fnGetDeleteTarget(ipGou, aIds)){
		return false;
	}

	//���ۂ̍폜
	for(int i = 0; i < 3; ++i){
		fnDeleteObject(aIds[i], i);
	}
	return true;
}

// �w�胁�b�V���R�[�h��PGDB�I�[�v��
IWorkspacePtr CPgdb::fnOpenLocalDataBase(LPCTSTR cBaseDir, long cMeshCode)
{
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBaseDir, cMeshCode/10000, cMeshCode); 

	// �p�[�\�i���W�IDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// �I�[�v�����Ă݂�
	ipWorkspaceFactory->OpenFromFile( CComBSTR( aDatabase ), 0, &ipWorkspace );
	if( !ipWorkspace )
	{
		_ftprintf( m_pFile, _T("#ERROR,%s ���J���܂���B\n"), static_cast<LPCTSTR>(aDatabase));
	}
	return ipWorkspace;
}

// �w��t�B�[�`���N���X�I�[�v��
IFeatureClassPtr CPgdb::fnOpenFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName)
{
	IFeatureClassPtr ipFeatureClass;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(	CComBSTR(cLayerName), &ipFeatureClass) != S_OK || !ipFeatureClass){
		_ftprintf(m_pFile, _T("#ERROR,�t�B�[�`���[�N���X���J���܂���B,%s\n"), cLayerName);
		return NULL;
	}
	return ipFeatureClass;
}

//  �폜�Ώہi�����𖞂����j��ID�񒊏o
bool CPgdb::fnGetDeleteTarget(const IFeatureClassPtr& ipGou, std::vector<ID_SET>& cIds)
{
#ifdef PART_COMP	//������v�Ȃ�
	//���|�C���g�Ɂu�w�肵���L�[���[�h�v�܂܂�鍆�|�C���g������
	CString aWhere;
	aWhere.Format(_T("GOU_NO LIKE '*%s*'"), m_Word);
	aWhere.Replace(_T("#"), _T("[#]"));	//#��SQL�Łu�C�ӂ̔��p�����v���������ꕶ�������̂悤�Ȃ̂ŁA�d���Ȃ�
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("OBJECTID, SHAPE"), aWhere);
#else
	//���S��v�Ȃ�
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("OBJECTID, SHAPE"), _T("GOU_NO = '%s'"), m_Word);
#endif
	IFeatureCursorPtr ipCursor;

	HRESULT hr = ipGou->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,���|�C���g�̌����Ɏ��s�B(%d)\n"), hr);
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		//1�̍��|�C���g����3���C�����𒊏o�B���t�ɂ��������l��
		//�ƌ`�͒��ڋ�Ԍ����A����Ɋ֘A����X�e�b�v�ƃ��C���̏��Ō��Č��߂�
		fnFindTarget(ipFeature, cIds);
	}
	return true;
}

// �P�̍��|�C���g����A�폜�ΏۂƂȂ錚���E�i���E���C����ID��̒��o
bool CPgdb::fnFindTarget(const IFeaturePtr& ipGouFeature, std::vector<ID_SET>& cIds)
{
	IGeometryPtr ipGeom;
	ipGouFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		_ftprintf(m_pFile, _T("#ERROR,���̌`�󂪎��܂���i�ʏ�o�Ă͂����Ȃ��G���[�j�B\n"));
		return false;
	}
	CComBSTR          bstrFieldName;
	m_ipBuild[0]->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, esriSpatialRelWithin );
	//�܂��͉ƌ`����
	IFeatureCursorPtr ipCursor;
	HRESULT hr = m_ipBuild[0]->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,�ƌ`�̌����Ɏ��s�B(%d)\n"), hr);
		return false;
	}
	IFeaturePtr ipFeature;
	//�ő�ł�1����������Ȃ��͂�
	if(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aVal;
		ipFeature->get_Value(m_BuildIndex, &aVal);

		bool aHaveKind = (aVal.vt != VT_NULL && aVal.lVal != 0);

		if(!fnDateCheck(ipFeature, 0, aHaveKind)){
			//���t�������N���A���Ă��Ȃ�������ΏۊO
			fnPrintLog(gBuildTable[0], ipFeature, 1, _T("�w����ȍ~�ɕҏW���ꂽ�ƌ`�̂��ߍ폜���܂���B"));
			return true;
		}
		//���t�������N���A���Ă�����
		//�ƌ`�Ɋ܂܂��i���i�X�e�b�v�j������
		IGeometryPtr ipBuildGeom;
		ipFeature->get_Shape(&ipBuildGeom);
		if(!ipBuildGeom){
			_ftprintf(m_pFile, _T("#ERROR,�ƌ`�̌`�󂪎��܂���i�ʏ�o�Ă͂����Ȃ��G���[�j�B\n"));
			return false;
		}
		if(!fnFindInner(ipBuildGeom, cIds[1], 1,aHaveKind)){
			//���t�������N���A���Ă��Ȃ����̂�1�ł�����ΑΏۊO
			fnPrintLog(gBuildTable[0], ipFeature, 1, _T("�w����ȍ~�ɕҏW���ꂽ�i�����܂܂�Ă��邽�ߍ폜���܂���B"));
			return true;
		}
		//���t�������N���A���Ă�����
		//�ƌ`�Ɋ܂܂�郉�C������
		fnFindInner(ipBuildGeom, cIds[2], 2, aHaveKind);
		long aObjID;
		ipFeature->get_OID(&aObjID);
		cIds[0].insert(aObjID);
	}else{
		long aObjID;
		ipGouFeature->get_OID(&aObjID);
		fnPrintLog(_T("GOU_POINT"), ipGouFeature, 1, _T("�Ή�����ƌ`��������܂���ł����B"));
	}
	return true;
}

// ���t�����̃`�F�b�N
bool CPgdb::fnDateCheck(const IFeaturePtr& ipFeature, int cIndex, bool cHaveKind)
{
	int aDataTarget = cHaveKind?1:0;
	if(m_Date[aDataTarget].vt == VT_NULL){
		return true;
	}
	CComVariant aVal;
	ipFeature->get_Value(m_ModDateIndex[cIndex], &aVal);
	if(aVal.vt != VT_NULL && aVal > m_Date[aDataTarget]){
		return false;
	}
	return true;
}

// �ƌ`�Ɋ܂܂��i���E���C���̂����폜�Ώ�ID��𒊏o
bool CPgdb::fnFindInner(const IGeometryPtr& ipGeom, ID_SET& cIds, int cIndex, bool cHaveKind)
{
	CComBSTR          bstrFieldName;
	m_ipBuild[cIndex]->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, esriSpatialRelContains );

	IFeatureCursorPtr ipCursor;
	HRESULT hr = m_ipBuild[cIndex]->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,�ƌ`�Ɋ܂܂��I�u�W�F�N�g�����Ɏ��s�B(%d)\n"), hr);
		return false;
	}
	bool aCanDelete = true;
	ID_SET aTmpIds;
	IFeaturePtr ipFeature;
	//�ő�ł�1����������Ȃ��͂�
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		if(fnDateCheck(ipFeature, cIndex, cHaveKind)){
			//���t�������N���A���Ă�����
			long aObjID;
			ipFeature->get_OID(&aObjID);
			aTmpIds.insert(aObjID);
		}else{
			//1�ł����t�����𖞂����Ă��Ȃ�������S�đΏۊO
			aCanDelete = false;
			break;
		}
	}
	if(aCanDelete){
		//�S��OK�ȏꍇ�����폜�Ώ�
		foreach(const long aObjID, aTmpIds){
			cIds.insert(aObjID);
		}
	}
	return aCanDelete;
}

// ID��w��ŃI�u�W�F�N�g�̍폜
bool CPgdb::fnDeleteObject(const ID_SET& cIds, int cIndex)
{
	foreach(const long aObjID, cIds){
		IFeaturePtr ipFeature;
		m_ipBuild[cIndex]->GetFeature(aObjID, &ipFeature);
		if(ipFeature){
#ifndef _DEBUG
			ipFeature->Delete();
#endif
			fnPrintLog(gBuildTable[cIndex], ipFeature, 0, _T("�|���S���폜"));
		}else{
			_ftprintf(m_pFile, _T("#ERROR,ID�ɂ��I�u�W�F�N�g�擾�Ɏ��s�B�i�o�Ă͂����Ȃ��j,%s,%d\n"), gBuildTable[cIndex], aObjID);
			return false;
		}
	}
	return true;
}

// ���b�V���EXY�`���̃��O�o��
void CPgdb::fnPrintLog(LPCTSTR cLayerName, const IFeaturePtr& ipFeature, int cErrLv, LPCTSTR cMsg)
{
	static LPCTSTR aErrMsg[] = {
		_T("INFO"),
		_T("WARNING"),
		_T("ERROR")
	};
	long aObjID;
	ipFeature->get_OID(&aObjID);
	int aMesh = -1, aX = -1, aY = -1;
	fnGetMXY(ipFeature, aMesh, aX, aY);
	_ftprintf(m_pFile, _T("%d\t%d\t%d\t%s\t%ld\t%s\t%s\n"), aMesh, aX, aY, cLayerName, aObjID, aErrMsg[cErrLv], cMsg);
}

// �t�B�[�`�����烍�O�o�͗p�̃��b�V���EXY���v�Z
bool CPgdb::fnGetMXY(const IFeaturePtr& ipFeature, int& cMesh, int& cX, int& cY)
{
	cMesh = cX = cY = 0;
	//�|�C���g�A�|���S���A���C���ʂɃ��b�V���AX,Y���擾
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		return false;
	}
	esriGeometryType aType;
	ipGeom->get_GeometryType(&aType);
	switch(aType){
		case esriGeometryPoint:
			{
				//�|�C���g�͂��̓_
				double aX, aY;
				((IPointPtr)ipGeom)->QueryCoords(&aX, &aY);
				cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
			}
			break;
		case esriGeometryPolygon:
			{
				//�|���S���́A���x���|�C���g���͎n�_
				IAreaPtr ipArea = ipGeom;
				if(!ipArea){
					//�ɏ����Ƃ��肦����ۂ�
					IPointPtr ipPoint;
					((IPointCollectionPtr)ipGeom)->get_Point(0, &ipPoint);
					if(!ipPoint){
						_ftprintf(m_pFile, _T("#ERROR,�n�_����\�_�����Ȃ��|���S���B�i�o�Ă͂����Ȃ��͂��j\n"));
						return false;
					}
					double aX, aY;
					ipPoint->QueryCoords(&aX, &aY);
					cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
				}else{
					IPointPtr ipPoint;
					ipArea->get_LabelPoint(&ipPoint);
					if(!ipPoint){
						((IPointCollectionPtr)ipGeom)->get_Point(0, &ipPoint);
						if(!ipPoint){
							_ftprintf(m_pFile, _T("#ERROR,�n�_����\�_�����Ȃ��|���S���B�i�o�Ă͂����Ȃ��͂��j\n"));
							return false;
						}
					}
					double aX, aY;
					ipPoint->QueryCoords(&aX, &aY);
					cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
				}
			}
			break;
		case esriGeometryPolyline:
			{
				//���C���́A�\���_�I�ɐ^�񒆕t�߂̓_�i2�_���Ǝn�_�j
				long aCount;
				((IPointCollectionPtr)ipGeom)->get_PointCount(&aCount);
				aCount /= 2;
				--aCount;
				IPointPtr ipPoint;
				((IPointCollectionPtr)ipGeom)->get_Point(aCount, &ipPoint);
				if(!ipPoint){
					_ftprintf(m_pFile, _T("#ERROR,���ԓ_�����Ȃ��|�����C���B�i�o�Ă͂����Ȃ��͂��j\n"));
					return false;
				}
				double aX, aY;
				ipPoint->QueryCoords(&aX, &aY);
				cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
			}
			break;
		default:
			{
				_ftprintf(m_pFile, _T("#ERROR,���Ή���ShapeType�B�i�o�Ă͂����Ȃ��j\n"));
				return false;
			}
			break;
	}
	return true;
}
