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

// VerDiff.cpp: VerDiff �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerDiff.h"

namespace{ 
	LPCTSTR DEF_MESHFIELD = _T("MeshCode");

	LPCTSTR DEF_CITY_CODE_FIELD = _T("CITYCODE");
	LPCTSTR DEF_ADDR_CODE_FIELD = _T("ADDRCODE");

	CString DEF_BUILD_LAYER = _T("Building");
}
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

VerDiff::VerDiff()
{
	m_MeshIndex = -1;
	m_CodeSet.clear();
}

VerDiff::~VerDiff()
{
	m_CodeSet.clear();
}

TypeTbl VerDiff::m_Table[] = {
	{_T("�ǉ�"), esriDifferenceTypeInsert},			//�V�̂�
	{_T("�폜"), esriDifferenceTypeDeleteNoChange},	//���̂�
	{_T("�X�V"), esriDifferenceTypeUpdateNoChange},	//����
	{_T("�X�V+�X�V"), esriDifferenceTypeUpdateUpdate},	//����
	{_T("�X�V+�폜"), esriDifferenceTypeUpdateDelete},	//�V�̂�
	{_T("�폜+�X�V"), esriDifferenceTypeDeleteUpdate},	//���̂�
	{NULL, (esriDifferenceType)-1}
};

bool VerDiff::init(IWorkspacePtr ipWorkspace, IWorkspacePtr ipAdrWorkspace, LPCTSTR cIndexLayer, LPCTSTR cOldVersion, LPCTSTR cOldAdmVersion, int cMode)
{
	m_ipWorkspace = ipWorkspace;
	m_Mode = cMode;
	m_OldVersion = cOldVersion;
	CString aOldAdmVersion;
	if(CString(cOldAdmVersion) != _T("")){
		aOldAdmVersion = cOldAdmVersion;
	}else{
		aOldAdmVersion = cOldVersion;
	}
	if(cMode == 1 || cMode == 3){
		if(cIndexLayer != NULL){
			IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
			ATLASSERT( ipFeatureWorkspace != NULL );
			ipFeatureWorkspace->OpenFeatureClass(CComBSTR(cIndexLayer), &m_ipMeshFeatureClass);
			if(m_ipMeshFeatureClass == NULL){
				_ftprintf(stderr, _T("%s���J���܂���B\n"), cIndexLayer);
				return false;
			}

			m_ipMeshFeatureClass->FindField(CComBSTR(DEF_MESHFIELD), &m_MeshIndex);
			if(m_MeshIndex < 0){
				_ftprintf(stderr, _T("�t�B�[���h�������s[%s]\n"),	DEF_MESHFIELD);
				return false;
			}
			if(cMode == 3){
				LPCTSTR aEnv = _tgetenv(_T("FC_BUILDING"));
				if(aEnv != NULL){
					DEF_BUILD_LAYER = aEnv;
				}
				//�����ł�ipAdrWorkspace�͌����p�ɂȂ��Ă���͂�
				((IFeatureWorkspacePtr)ipAdrWorkspace)->OpenFeatureClass(CComBSTR(DEF_BUILD_LAYER), &m_ipBuildFeatureClass);
				if(m_ipBuildFeatureClass == NULL){
					_ftprintf(stderr, _T("%s���J���܂���B\n"), static_cast<LPCTSTR>(DEF_BUILD_LAYER));
					return false;
				}
			}
		}else{
			fprintf(stderr, "���b�V�����X�g���쐬���鎞�́A���b�V���|���S�����w�肵�ĉ������B\n");
			return false;
		}
	}else if(m_Mode == 2){
		if(cIndexLayer != NULL){
			IVersionedWorkspacePtr ipAdrVerWorkspace(ipAdrWorkspace);
			ATLASSERT( ipAdrVerWorkspace != NULL );
			IVersionPtr ipAdrNewVersion(ipAdrVerWorkspace);
			IVersionPtr ipAdrOldVersion;
			ipAdrVerWorkspace->FindVersion(CComBSTR(aOldAdmVersion), &ipAdrOldVersion);
//			ipAdrVerWorkspace->FindVersion(CComBSTR("SDE.DEFAULT"), &ipAdrOldVersion);
			if(ipAdrOldVersion == NULL){
				_ftprintf(stderr, _T("�Z���p���o�[�W����[%s]��������܂���B\n"), static_cast<LPCTSTR>(aOldAdmVersion));
				return false;
			}
			IFeatureWorkspacePtr ipAdrNewFeatureWorkspace(ipAdrNewVersion);
			ATLASSERT( ipAdrNewFeatureWorkspace != NULL );
			IFeatureWorkspacePtr ipAdrOldFeatureWorkspace(ipAdrOldVersion);
			ATLASSERT( ipAdrOldFeatureWorkspace != NULL );
			ipAdrOldFeatureWorkspace->OpenFeatureClass(CComBSTR(cIndexLayer), &m_ipOldAdrFeatureClass);
			if(m_ipOldAdrFeatureClass == NULL){
				_ftprintf(stderr, _T("%s���J���܂���B(%s)\n"), static_cast<LPCTSTR>(cIndexLayer), static_cast<LPCTSTR>(m_OldVersion));
				return false;
			}
			ipAdrNewFeatureWorkspace->OpenFeatureClass(CComBSTR(cIndexLayer), &m_ipNewAdrFeatureClass);
			if(m_ipNewAdrFeatureClass == NULL){
				_ftprintf(stderr, _T("%s���J���܂���B(�J�����g)\n"), static_cast<LPCTSTR>(cIndexLayer));
				return false;
			}
			m_ipNewAdrFeatureClass->FindField(CComBSTR(DEF_CITY_CODE_FIELD), &m_CityCodeIndex);
			if(m_CityCodeIndex < 0){
				_ftprintf(stderr, _T("�t�B�[���h�������s[%s]\n"), DEF_CITY_CODE_FIELD);
				return false;
			}
			m_ipNewAdrFeatureClass->FindField(CComBSTR(DEF_ADDR_CODE_FIELD), &m_AddrCodeIndex);
			if(m_AddrCodeIndex < 0){
				_ftprintf(stderr, _T("�t�B�[���h�������s[%s]\n"), DEF_ADDR_CODE_FIELD);
				return false;
			}
		}else{
			_ftprintf(stderr, _T("�Z�����X�g���쐬���鎞�́A�s���E�|���S�����w�肵�ĉ������B\n"));
			return false;
		}
	}

	return true;
}

bool VerDiff::execDiff(LPCTSTR cLayer)
{
	IVersionedWorkspacePtr ipVerWorkspace(m_ipWorkspace);
	ATLASSERT( ipVerWorkspace != NULL );
	IVersionPtr ipNewVersion(ipVerWorkspace);
	IVersionPtr ipOldVersion;
	ipVerWorkspace->FindVersion(CComBSTR(m_OldVersion), &ipOldVersion);
	if(ipOldVersion == NULL){
		_ftprintf(stderr, _T("�o�[�W����[%s]��������܂���B\n"), static_cast<LPCTSTR>(m_OldVersion));
		return false;
	}
	IFeatureWorkspacePtr ipNewFeatureWorkspace(ipNewVersion);
	ATLASSERT( ipNewFeatureWorkspace != NULL );
	IFeatureWorkspacePtr ipOldFeatureWorkspace(ipOldVersion);
	ATLASSERT( ipOldFeatureWorkspace != NULL );

	ipOldFeatureWorkspace->OpenFeatureClass(CComBSTR(cLayer), &m_ipOldFeatureClass);
	if(m_ipOldFeatureClass == NULL){
		_ftprintf(stderr, _T("%s���J���܂���B(%s)\n"), cLayer, static_cast<LPCTSTR>(m_OldVersion));
		return false;
	}
	ipNewFeatureWorkspace->OpenFeatureClass(CComBSTR(cLayer), &m_ipNewFeatureClass);
	if(m_ipNewFeatureClass == NULL){
		_ftprintf(stderr, _T("%s���J���܂���B�i�J�����g�j\n"),  cLayer);
		return false;
	}
	bool aKlinkTgt;
	CString aLayer = cLayer;
	if(aLayer.CompareNoCase(_T("Gou_Point")) == 0 || aLayer.CompareNoCase(_T("CS_Addr_Point")) == 0 || aLayer.CompareNoCase(_T("Klink_Point")) == 0){
		aKlinkTgt = true;
	}else{
		aKlinkTgt = false;
	}
	for(int i = 0; m_Table[i].m_Msg != NULL; i++){
		_tprintf(_T("%s�́u%s�v�����������Ă��܂��B\n"), cLayer, m_Table[i].m_Msg);
		fnCheckDiff(i, aKlinkTgt);
	}
	return true;
}

bool VerDiff::fnCheckDiff(int cIndex, bool cKlinkTgt)
{
	IVersionedTablePtr ipVerVTable(m_ipNewFeatureClass);
	IDifferenceCursorPtr ipCursor;
	if(FAILED(ipVerVTable->Differences((ITablePtr)m_ipOldFeatureClass, m_Table[cIndex].m_Type, NULL, &ipCursor))){
		_ftprintf(stderr, _T("%s�������s\n"), m_Table[cIndex].m_Msg);
		return false;
	}
	_IRowPtr ipRow;
	LONG aObjID;
	IFeatureClassPtr ipFeatureClass;
	IFeatureClassPtr ipAdmFeatureClass;
	if(cIndex == 1 || cIndex == 5){
		//�폜����Ă��邩��A�I�u�W�F�N�g�͌Â���������
		ipFeatureClass = m_ipOldFeatureClass;
		ipAdmFeatureClass = m_ipOldAdrFeatureClass;
	}else{
		ipFeatureClass = m_ipNewFeatureClass;
		ipAdmFeatureClass = m_ipNewAdrFeatureClass;
	}
	CComBSTR aLayerName;
	((IDatasetPtr)ipFeatureClass)->get_Name(&aLayerName);

	while(ipCursor->Next(&aObjID, &ipRow) == S_OK){
		if(m_Mode == 0){
			_tprintf(_T("%s�F%s,%d\n"), m_Table[cIndex].m_Msg, static_cast<LPCTSTR>(CString(aLayerName)), aObjID);
		}else if(m_Mode == 2){
			//�Z���R�[�h���X�g
			if(cIndex == 2 || cIndex == 3){
				fnSearchAddr(aObjID, m_ipOldFeatureClass, m_ipOldAdrFeatureClass);
				fnSearchAddr(aObjID, m_ipNewFeatureClass, m_ipNewAdrFeatureClass);
			}else{
				fnSearchAddr(aObjID, ipFeatureClass, ipAdmFeatureClass);
			}
		}else if(m_Mode == 3 && cKlinkTgt){
			//���b�V�����X�g�i�g���j
			if(cIndex == 2 || cIndex == 3){
				fnSearchMeshEx(aObjID, m_ipOldFeatureClass);
				fnSearchMeshEx(aObjID, m_ipNewFeatureClass);
			}else{
				fnSearchMeshEx(aObjID, ipFeatureClass);
			}
		}else{
			//���b�V�����X�g
			if(cIndex == 2 || cIndex == 3){
				fnSearchMesh(aObjID, m_ipOldFeatureClass);
				fnSearchMesh(aObjID, m_ipNewFeatureClass);
			}else{
				fnSearchMesh(aObjID, ipFeatureClass);
			}
		}
	}
	return true;
}

bool VerDiff::fnSearchMesh(IFeaturePtr ipFeature)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if( !ipGeom )
		return true;
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipMeshFeatureClass->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	IFeatureCursorPtr ipCursor;

	if(FAILED(m_ipMeshFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		fprintf(stderr, "���b�V���������s�B,%d\n", aObjID);
		return false;
	}
	IFeaturePtr ipMeshFeature;
	IRelationalOperatorPtr ipRel(ipGeom);
	while(ipCursor->NextFeature(&ipMeshFeature) == S_OK){
		IGeometryPtr ipTgtGeom;
		ipMeshFeature->get_Shape(&ipTgtGeom);
		VARIANT_BOOL aTouch;
		ipRel->Touches(ipTgtGeom, &aTouch);
		if(!aTouch){
			CComVariant aMeshCode;
			ipMeshFeature->get_Value(m_MeshIndex, &aMeshCode);
			CString aTmpMesh;
			aTmpMesh.Format(_T("%d"), aMeshCode.lVal);
			m_CodeSet.insert(aTmpMesh);
//			if(aTmpMesh == "53395505"){
//				printf("%d\n", cObjID);
//			}
	//		printf("%d\n", aMeshCode.lVal);
		}
	}
	return true;
}

bool VerDiff::fnSearchMesh(LONG cObjID, IFeatureClassPtr ipFeatureClass)
{
	IFeaturePtr ipFeature;
	ipFeatureClass->GetFeature(cObjID, &ipFeature);
	if(ipFeature == NULL){
		fprintf(stderr, "�t�B�[�`����������Ȃ��B,%d\n", cObjID);
		return false;
	}
	return fnSearchMesh(ipFeature);
}

bool VerDiff::fnSearchMeshEx(LONG cObjID, IFeatureClassPtr ipFeatureClass)
{
	IFeaturePtr ipFeature;
	ipFeatureClass->GetFeature(cObjID, &ipFeature);
	if(ipFeature == NULL){
		fprintf(stderr, "�t�B�[�`����������Ȃ��B,%d\n", cObjID);
		return false;
	}
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	// NULL�V�F�C�v�̓X�L�b�v
	if( !ipGeom )
		return true;
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipBuildFeatureClass->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelWithin );
	IFeatureCursorPtr ipCursor;
	if(FAILED(m_ipBuildFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		fprintf(stderr, "�_���܂ތ����������s�B,%d\n", aObjID);
		return false;
	}

	IFeaturePtr ipBuildFeature;
	if(ipCursor->NextFeature(&ipBuildFeature) == S_OK){
		return fnSearchMesh(ipBuildFeature);
	}
	return fnSearchMesh(ipFeature);
}

bool VerDiff::resultOut(LPCTSTR cOutFile)
{
	FILE * pFile;

	if((pFile = _tfopen(cOutFile, _T("wt"))) != NULL){
		CODE_SET::iterator aCode;
		for(aCode = m_CodeSet.begin(); aCode != m_CodeSet.end(); aCode++){
			_ftprintf(pFile, _T("%s\n"), static_cast<LPCTSTR>(*aCode));
		}
		fclose(pFile);
	}else{
		_ftprintf(stderr, _T("�o�̓t�@�C�����J���Ȃ��B:%s\n"), cOutFile);
	}
	return true;
}


bool VerDiff::fnSearchAddr(LONG cObjID, IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipAddrFeatureClass)
{
	IFeaturePtr ipFeature;
	ipFeatureClass->GetFeature(cObjID, &ipFeature);
	if(ipFeature == NULL){
		fprintf(stderr, "�t�B�[�`����������Ȃ��B,%d\n", cObjID);
		return false;
	}
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	// NULL�V�F�C�v�̓X�L�b�v
	if( !ipGeom )
		return true;
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipOldAdrFeatureClass->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	IFeatureCursorPtr ipCursor;

	if(FAILED(ipAddrFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fprintf(stderr, "�Z���������s�B,%d\n", cObjID);
		return false;
	}
	IFeaturePtr ipAdrFeature;
	IRelationalOperatorPtr ipRel(ipGeom);
	while(ipCursor->NextFeature(&ipAdrFeature) == S_OK){
		IGeometryPtr ipTgtGeom;
		ipAdrFeature->get_Shape(&ipTgtGeom);
		VARIANT_BOOL aTouch;
		ipRel->Touches(ipTgtGeom, &aTouch);
		if(!aTouch){
			CComVariant aCityCode;
			ipAdrFeature->get_Value(m_CityCodeIndex, &aCityCode);
			if(aCityCode.vt != VT_NULL){
				CComVariant aAddrCode;
				ipAdrFeature->get_Value(m_AddrCodeIndex, &aAddrCode);
				if(aAddrCode.vt != VT_NULL){
					CString aTmpStr = CString(aCityCode.bstrVal) + CString(aAddrCode.bstrVal);
					m_CodeSet.insert(aTmpStr);
				}
			}
		}
	}
	return true;
}

bool VerDiff::execAddrDiff()
{
	CComBSTR aLayerName;
	m_ipOldAdrFeatureClass->get_AliasName(&aLayerName);
	for(int i = 0; m_Table[i].m_Msg != NULL; i++){
		_tprintf(_T("%s�́u%s�v�����������Ă��܂��B\n"), static_cast<LPCTSTR>(CString(aLayerName)), m_Table[i].m_Msg);
		IVersionedTablePtr ipVerVTable(m_ipNewAdrFeatureClass);
		IDifferenceCursorPtr ipCursor;
		if(FAILED(ipVerVTable->Differences((ITablePtr)m_ipOldAdrFeatureClass, m_Table[i].m_Type, NULL, &ipCursor))){
			_ftprintf(stderr, _T("%s�������s\n"), m_Table[i].m_Msg);
			return false;
		}
		_IRowPtr ipRow;
		LONG aObjID;

		while(ipCursor->Next(&aObjID, &ipRow) == S_OK){
			if(i != 0 && i != 4){
				IFeaturePtr ipFeature;
				m_ipOldAdrFeatureClass->GetFeature(aObjID, &ipFeature);
				if(ipFeature != NULL){
					CComVariant aCityCode;
					ipFeature->get_Value(m_CityCodeIndex, &aCityCode);
					if(aCityCode.vt != VT_NULL){
						CComVariant aAddrCode;
						ipFeature->get_Value(m_AddrCodeIndex, &aAddrCode);
						if(aAddrCode.vt != VT_NULL){
							CString aTmpStr = CString(aCityCode.bstrVal) + CString(aAddrCode.bstrVal);
//							if(strchr(aTmpStr, '?') != NULL){
//								printf("�_��:%s\n", aTmpStr);
//							}
							m_CodeSet.insert(aTmpStr);
						}
					}
				}else{
					fprintf(stderr, "�t�B�[�`�����擾�ł��Ȃ��B,%d\n", aObjID);
				}
			}
			if(i != 1 && i != 5){
				IFeaturePtr ipFeature;
				m_ipNewAdrFeatureClass->GetFeature(aObjID, &ipFeature);
				if(ipFeature != NULL){
					CComVariant aCityCode;
					ipFeature->get_Value(m_CityCodeIndex, &aCityCode);
					if(aCityCode.vt != VT_NULL){
						CComVariant aAddrCode;
						ipFeature->get_Value(m_AddrCodeIndex, &aAddrCode);
						if(aAddrCode.vt != VT_NULL){
							CString aTmpStr = CString(aCityCode.bstrVal) + CString(aAddrCode.bstrVal);
//							if(strchr(aTmpStr, '?') != NULL){
//								printf("�_��:%s\n", aTmpStr);
//							}
							m_CodeSet.insert(aTmpStr);
						}
					}
				}else{
					fprintf(stderr, "�t�B�[�`�����擾�ł��Ȃ��B,%d\n", aObjID);
				}
			}
		}
	}
	return true;
}

bool VerDiff::execCSDiff(LPCTSTR cLayer)
{
	IVersionedWorkspacePtr ipVerWorkspace(m_ipWorkspace);
	ATLASSERT( ipVerWorkspace != NULL );
	IVersionPtr ipNewVersion(ipVerWorkspace);
	IVersionPtr ipOldVersion;
	ipVerWorkspace->FindVersion(CComBSTR(m_OldVersion), &ipOldVersion);
	if(ipOldVersion == NULL){
		_ftprintf(stderr, _T("�o�[�W����[%s]��������܂���B\n"), static_cast<LPCTSTR>(m_OldVersion));
		return false;
	}
	IFeatureWorkspacePtr ipNewFeatureWorkspace(ipNewVersion);
	ATLASSERT( ipNewFeatureWorkspace != NULL );
	IFeatureWorkspacePtr ipOldFeatureWorkspace(ipOldVersion);
	ATLASSERT( ipOldFeatureWorkspace != NULL );

	ipOldFeatureWorkspace->OpenFeatureClass(CComBSTR(cLayer), &m_ipOldFeatureClass);
	if(m_ipOldFeatureClass == NULL){
		_ftprintf(stderr, _T("%s���J���܂���B(%s)\n"), cLayer, static_cast<LPCTSTR>(m_OldVersion));
		return false;
	}
	ipNewFeatureWorkspace->OpenFeatureClass(CComBSTR(cLayer), &m_ipNewFeatureClass);
	if(m_ipNewFeatureClass == NULL){
		_ftprintf(stderr, _T("%s���J���܂���B�i�J�����g�j\n"),  cLayer);
		return false;
	}
	LONG aTwentyIndex;
	m_ipNewFeatureClass->FindField(CComBSTR(DEF_ADDR_CODE_FIELD), &aTwentyIndex);
	if(aTwentyIndex < 0){
		fprintf(stderr, "20���R�[�h�t�B�[���h�������s\n");
		return false;
	}
	for(int i = 0; m_Table[i].m_Msg != NULL; i++){
		_tprintf(_T("%s�́u%s�v�����������Ă��܂�(%s���g)�B\n"), cLayer, m_Table[i].m_Msg, cLayer);
		IVersionedTablePtr ipVerVTable(m_ipNewFeatureClass);
		IDifferenceCursorPtr ipCursor;
		if(FAILED(ipVerVTable->Differences((ITablePtr)m_ipOldFeatureClass, m_Table[i].m_Type, NULL, &ipCursor))){
			_ftprintf(stderr, _T("%s�������s\n"), m_Table[i].m_Msg);
			return false;
		}
		_IRowPtr ipRow;
		LONG aObjID;
		while(ipCursor->Next(&aObjID, &ipRow) == S_OK){
			if(i != 0 && i != 4){
				IFeaturePtr ipFeature;
				m_ipOldFeatureClass->GetFeature(aObjID, &ipFeature);
				if(ipFeature != NULL){
					CComVariant aTwentyCode;
					ipFeature->get_Value(aTwentyIndex, &aTwentyCode);
					if(aTwentyCode.vt != VT_NULL){
						CString aTmpStr = aTwentyCode.bstrVal;
						m_CodeSet.insert(aTmpStr.Left(11));
					}
				}else{
					fprintf(stderr, "�t�B�[�`�����擾�ł��Ȃ��B,%d\n", aObjID);
				}
			}
			if(i != 1 && i != 5){
				IFeaturePtr ipFeature;
				m_ipNewFeatureClass->GetFeature(aObjID, &ipFeature);
				if(ipFeature != NULL){
					CComVariant aTwentyCode;
					ipFeature->get_Value(aTwentyIndex, &aTwentyCode);
					if(aTwentyCode.vt != VT_NULL){
						CString aTmpStr = aTwentyCode.bstrVal;
						m_CodeSet.insert(aTmpStr.Left(11));
					}
				}else{
					fprintf(stderr, "�t�B�[�`�����擾�ł��Ȃ��B,%d\n", aObjID);
				}
			}
		}
	}
	return true;
}

