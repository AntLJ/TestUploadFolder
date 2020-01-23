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
#include "SDEBase.h"

using namespace Reflect;
using namespace sindy::schema;
namespace{
	LPCTSTR PROG_NAME = _T("DiffImportRoad");
}

// �R���X�g���N�^
CSDEBase::CSDEBase(void)
: m_ExecInfo(_T(""))
, m_MaxX(0)
, m_MaxY(0)
, m_MinX(0)
, m_MinY(0)
, m_LayerManage(NULL)
{
}

// �f�X�g���N�^
CSDEBase::~CSDEBase(void)
{
}

// �������֐�
bool CSDEBase::init(void)
{
	return true;
}

// �ҏW�J�n�֐�
bool CSDEBase::fnStartEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->StartEditing( VARIANT_FALSE ))){	// Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
			_ftprintf(stderr, _T("#ERROR �ҏW�J�n�ł��܂���ł���(StartEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR �ҏW�J�n�ł��܂���ł���(StartEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(FAILED(ipWorkspaceEdit->StartEditOperation())){
			_ftprintf(stderr, _T("#ERROR �ҏW�J�n�ł��܂���ł���(StartEditOperation),%d,%s\n"), g_CurrentMesh,  cMsg);
			_tprintf(_T("ERROR �ҏW�J�n�ł��܂���ł���(StartEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}

// �ҏW�I���E�ۑ��֐�
bool CSDEBase::fnStopEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->StopEditOperation())){
			_ftprintf(stderr, _T("#ERROR,�ҏW�I�����s(StopEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,�ҏW�I�����s(StopEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){;	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
			_ftprintf(stderr, _T("#�ۑ�����,%d,%s\n"), g_CurrentMesh, cMsg);
		}else{
			_ftprintf(stderr, _T("#ERROR,�ۑ����s(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,�ۑ����s(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}

// �ҏW�I���E�j���֐�
bool CSDEBase::fnAbortEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->AbortEditOperation())){
			_ftprintf(stderr, _T("#ERROR,�ҏW�I�����s(AbortEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,�ҏW�I�����s(AbortEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_FALSE ))){;	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
			_ftprintf(stderr, _T("#�j������,%d,%s\n"), g_CurrentMesh, cMsg);
		}else{
			_ftprintf(stderr, _T("#ERROR,�j�����s(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,�ۑ����s(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}


// ���ԕۑ��֐�
bool CSDEBase::fnMidSave(LPCTSTR cMsg) const
{
	if(fnStopEdit(cMsg)){
		return fnStartEdit(cMsg);
	}
	return false;
}

// �����J�n�̃��b�V���擾�i���f�����炻�̎�����j�֐�
MESH_SET::iterator CSDEBase::fnGetFirstMesh(void) const
{
	MESH_SET::iterator aMesh = g_MeshSet.find(g_CurrentMesh);
	if(aMesh == g_MeshSet.end()){
		aMesh = g_MeshSet.begin();
	}else{
		++aMesh;
	}
	return aMesh;
}

/*
	�S���b�V�������֐�
	�����F���ԕۑ��܂ł̏������b�V����
	�@�@�@-1�͕ۑ��Ȃ��i�I�[�o�[�t���[���邭�炢�񂹂΂��ۑ�����邯�ǁj
*/
bool CSDEBase::executeALL(int cMidSave)
{
	CString aMsg =m_ExecInfo + _T("�J�n");
	g_IsContinue =false;
	if(!fnStartEdit(aMsg)){
		return false;
	}
	int aCount = 0;
	aMsg = _T("");
	MESH_SET::iterator aMesh;
	for(aMesh = fnGetFirstMesh(); aMesh  != g_MeshSet.end(); ++aMesh){
		g_CurrentMesh = *aMesh;
		_tprintf(_T("Now %s,%d\n"), m_ExecInfo,  g_CurrentMesh);

		if(!fnExecMesh()){
			//���b�V�����̏����ŏ������s�s�\�ȃG���[����������A�j�����ďI��
			aMsg = m_ExecInfo + _T("�G���[");
			fnAbortEdit(aMsg);
			return false;
		}
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				_tprintf(_T("�����͒��~����܂���\n"));
				_ftprintf(stderr, _T("#�����͒��~����܂���,%d\n"), g_CurrentMesh);
				aMsg = _T("�蓮���~");
				break;
			}else if(aKey == 'P'){
				//���f
				_tprintf(_T("�����͈ꎞ���f����܂���\n"));
				_ftprintf(stderr, _T("#�����͈ꎞ���f����܂���,%d\n"), g_CurrentMesh);
				aMsg = _T("�ꎞ��~");
				g_IsContinue = true;
				break;
			}
		}
		if(++aCount == cMidSave){
			aMsg = m_ExecInfo + _T("����");
			if(!fnMidSave(aMsg)){
				return false;
			}
			aCount = 0;
		}
	}
	if(aMesh == g_MeshSet.end()){
		//�Ō�܂ōs������
		g_CurrentMesh = -1;
		g_NowProccess++;
		aMsg = m_ExecInfo + _T("�I��");
	}
	if(!fnStopEdit((aMsg))){	//�ۑ����ďI��
		return false;
	}
	return true;
}

// ���b�V���|���S�����̓��b�V�����E���C���擾�֐�
// ���E��̌����Ȕ�r�����邽�߂̍ő�ŏ��l���擾
bool CSDEBase::fnGetMesh(void)
{
	IFeatureClassPtr ipFeatureClass = m_LayerManage->findTable(basemesh::kTableName);
	if(!ipFeatureClass){
		//�o�O�̂͂�������
		fnPrintComError();
		return false;
	}
	CString aWhere;
	aWhere.Format(_T("%s = %d"), basemesh::kMeshCode, g_CurrentMesh);
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, aWhere);

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
	IFeaturePtr ipFeature;
	ipCursor->NextFeature(&ipFeature);
	if(!ipFeature){
		fnPrintComError();
		return false;
	}
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		fnPrintComError();
		return false;
	}
	m_ipMeshPoly = ipGeom;
	//���E�́u�g�v���擾
	ITopologicalOperatorPtr ipTopo(m_ipMeshPoly);
	ipTopo->get_Boundary(&m_ipMeshBoundary);
	m_ipMeshBoundary->putref_SpatialReference( m_LayerManage->get_SpRef() );
	m_ipMeshBoundary->SnapToSpatialReference();

	IPointCollectionPtr ipPointCollection(m_ipMeshPoly);
	ATLASSERT( ipPointCollection != NULL );

	long aPointCount = 0;
	ipPointCollection->get_PointCount( &aPointCount );
	std::vector<WKSPoint> pPt( aPointCount );
	ipPointCollection->QueryWKSPoints( 0, aPointCount, &pPt.at(0) );


	double aMinX, aMinY, aMaxX, aMaxY;
	aMinX = aMinY = 1000000.0;
	aMaxX = aMaxY = -1000000.0;
	for( int j = 0; j < aPointCount; j++ ) {
		if(aMinX > pPt[j].X)
			aMinX = pPt[j].X;
		if(aMinY > pPt[j].Y)
			aMinY = pPt[j].Y;
		if(aMaxX < pPt[j].X)
			aMaxX = pPt[j].X;
		if(aMaxY < pPt[j].Y)
			aMaxY = pPt[j].Y;
	}

	m_MaxX = aMaxX;
	m_MaxY = aMaxY;
	m_MinX = aMinX;
	m_MinY = aMinY;
	return true;
}

// COM�̃G���[�o�͊֐�
void CSDEBase::fnPrintComError(void) const
{
	IErrorInfoPtr	ipErrorInfo;
	::GetErrorInfo(0, &ipErrorInfo);
	if(ipErrorInfo != NULL){
		CComBSTR	aMsg;
		ipErrorInfo->GetDescription(&aMsg);
		_ftprintf(stderr, _T("#ERROR %s,%d\n"), CString(aMsg), g_CurrentMesh);
		_tprintf(_T("ERROR %s,%d\n"), aMsg, g_CurrentMesh);
	}else{
		_ftprintf(stderr, _T("#UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
		_tprintf(_T("UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
	}
}

// ���ʑ��������̃R�s�[and/or�Z�b�g�֐�
bool CSDEBase::fnSetCommonAttr(_IRowPtr& ipFeature, INDEX_MAP& cSDEIndexMap, INDEX_MAP& cPGDBIndexMap, const _IRowPtr& ipSrcFeature) const
{
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kOperator].m_Index, CComVariant(g_Operator)))){
		fprintf(stderr, "#�ŏI�X�V�҃Z�b�g���s\n");
		return false;
	}
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kUpdateType].m_Index, CComVariant(5)))){ //��C�C���|�[�g
		fprintf(stderr, "#�ŏI�X�V���e�Z�b�g���s\n");
		return false;
	}

	if(cSDEIndexMap.find(ipc_table::kUserClaim) != cSDEIndexMap.end() && cPGDBIndexMap.find(ipc_table::kUserClaim) != cPGDBIndexMap.end()){
		if(ipSrcFeature != NULL){
			CComVariant aClaim, aOrgClaim;
			ipSrcFeature->get_Value(cPGDBIndexMap[ipc_table::kUserClaim].m_Index, &aClaim);
			ipFeature->get_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, &aOrgClaim);
			if(aClaim != aOrgClaim){
				ipFeature->put_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, aClaim);
			}
			//���\�[�X�͏o�͎��ɏo���Ă��Ȃ��̂Ŕ��f���Ȃ�
			//CComVariant aSource, aOrgSource;
			//ipSrcFeature->get_Value(g_SourceIndex[cIndex], &aSource);
			//ipFeature->get_Value(g_SourceIndex[cIndex], &aOrgSource);
			//if(aSource != aOrgSource){
			//	ipFeature->put_Value(cSDEIndexMap[cIndex], aSource);
			//}
		}else {
			ipFeature->put_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, CComVariant(0));
			ipFeature->put_Value(cSDEIndexMap[ipc_table::kPurpose].m_Index, CComVariant(0));
		}
	}
	//���삳��̃p�N��
	CString aStr = _T("");
	SYSTEMTIME stSystemTime;
	//���삳��̃p�N���@�����܂�

	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kModifyProgName].m_Index, CComVariant(PROG_NAME)))){
		_ftprintf(stderr, _T("#ERROR,�v���O�������Z�b�g���s\n"));
		return false;
	}
	//���삳��̃p�N��
	::GetLocalTime( &stSystemTime);
	aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	//���삳��̃p�N���@�����܂�
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kProgModifyDate].m_Index, CComVariant(aStr)))){
		fprintf(stderr, "#�X�V�����Z�b�g���s\n");
		return false;
	}
	return true;
}

// ���O�n���h���i�W���^�C�v�j�p�̃��b�Z�[�W�o�͊֐�
void CSDEBase::fnPrintLogStyre(int cType, LPCTSTR cLayerName, int cObjID, LPCTSTR cMsg, LPCTSTR cMsg2, REF_LAYER cRefLayer, IGeometryPtr ipGeom) const
{
	static LPCTSTR aErrType[] = {
		_T("INFO"),
		_T("WARNING"),
		_T("ERROR"),
		_T("CRITICAL")
	};
	CString aLayerName;
	switch(cRefLayer){
		case TGT_LAYER:
			aLayerName = m_LayerManage->findTableName(cLayerName);	// �T�[�o����擾�������O�i���[�U���t���j
			break;
		case FIX_LAYER:
			aLayerName = g_FixUser + _T(".") + cLayerName;		// �w�肵��FIX���[�U��+�^�O�I�Ɏg�p���Ă��閼��
			break;
		case PGDB_LAYER:
			aLayerName = cLayerName;	// �^�O�I�Ɏg�p���Ă��閼�́i���[�U���Ȃ��j
			break;
		default:
			break;
	}
	if(ipGeom){
		double aX = -1.0, aY = -1.0;
		esriGeometryType aType = esriGeometryNull;
		ipGeom->get_GeometryType(&aType);
		switch(aType){
			case esriGeometryPolygon:
				{
					IPointPtr ipPoint;
					((IAreaPtr)ipGeom)->get_LabelPoint(&ipPoint);
					ipPoint->QueryCoords(&aX, &aY);
				}
				break;
			case esriGeometryPolyline:
				{
					IPointPtr ipPoint;
					((IPolylinePtr)ipGeom)->get_FromPoint(&ipPoint);
					ipPoint->QueryCoords(&aX, &aY);
				}
				break;
			case esriGeometryPoint:
				((IPointPtr)ipGeom)->QueryCoords(&aX, &aY);
				break;
			default:
				//�����ɂ͗��Ȃ��͂�������
				break;
		}
		if(cMsg2){
			_ftprintf(stderr, _T("0\t%s\t%d\t%.12f\t%.12f\t%s\t0\t%s\t%d\t%s\n"), aLayerName, cObjID, aX, aY, aErrType[cType], cMsg, g_CurrentMesh, cMsg2);
		}else{
			_ftprintf(stderr, _T("0\t%s\t%d\t%.12f\t%.12f\t%s\t0\t%s\t%d\n"), aLayerName, cObjID, aX, aY, aErrType[cType], cMsg, g_CurrentMesh);
		}
	}else{
		if(cMsg2){
			_ftprintf(stderr, _T("0\t%s\t%d\t\t%\t%s\t0\t%s\t%d\t%s\n"), aLayerName, cObjID, aErrType[cType], cMsg, g_CurrentMesh, cMsg2);
		}else{
			_ftprintf(stderr, _T("0\t%s\t%d\t\t%\t%s\t0\t%s\t%d\n"), aLayerName, cObjID, aErrType[cType], cMsg, g_CurrentMesh);
		}
	}
	if(cType > 2){
		//��ʂɂ��o��
		_tprintf(_T("%s\t%d\t\t\t%s\t0\t%s\t%d\n"), aLayerName, cObjID,aErrType[cType], cMsg, g_CurrentMesh);
	}
#ifdef _DEBUG
		fflush(stderr);
#endif
}

// ��Ԍ����֐�
IFeatureCursorPtr CSDEBase::fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum cSpatialRel) const
{
	if(!m_ipTable[0]){
		_ftprintf(stderr, _T("#ERROR,�����Ώۃt�B�[�`���N���X��NULL�ł�,%d\n"), g_CurrentMesh);
		return NULL;
	}
	IFeatureCursorPtr ipCursor;
	CComBSTR          bstrFieldName;
	((IFeatureClassPtr)m_ipTable[0])->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, cSpatialRel );
	
	if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		_ftprintf(stderr, _T("#ERROR,�֘A����t�B�[�`���[�擾���s,%d\n"), g_CurrentMesh);
		return NULL;
	}
	return ipCursor;
}
