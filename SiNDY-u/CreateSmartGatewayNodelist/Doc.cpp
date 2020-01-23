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
#include "Doc.h"

CDoc::CDoc(void)
{
}

CDoc::~CDoc(void)
{
	Close();
	m_ofstream.close();
}

// �C���|�[�g�ݒ�̎擾
bool CDoc::getenv(const char* strConfigFile)
{
	FILE *fp;
	if((fp = fopen(strConfigFile, "r"))){
		char    aStr[256];
		while(fgets(aStr, 256, fp) != NULL){
			if( aStr[strlen(aStr)-1] == '\n' )
				aStr[strlen(aStr)-1] = '\0';

			if(aStr[0] == '#')	continue;
			int		i=0;
			char	*s;
			char	aWork[256],aVal[256],aInBuff[1024];
			strcpy(aInBuff,aStr);
			for( s=strtok(aInBuff,"\t"); s!=NULL; s=strtok(NULL,"\t")){
				if( !s ) continue;
				strcpy(aWork, s );
				s=strtok(NULL,"\t");
				if( s ) strcpy(aVal, s );
				//< # �ΏۃT�[�o��
				if(( !strcmp("SERVER",aWork))&&(strlen(aVal))){
					m_strServer = aVal;
				}
				//< # �ΏۃC���X�^���X
				else if(( !strcmp("INSTANCE",aWork))&&(strlen(aVal))){
					m_strInstance = aVal;		
				}
				//< # �Ώۃ��[�U
				else if(( !strcmp("USER",aWork))&&(strlen(aVal))){
					m_strUser = aVal;		
				}
				//< # �Ώۃp�X���[�h
				else if(( !strcmp("PASSWORD",aWork))&&(strlen(aVal))){
					m_strPassword = aVal;		
				}
				//< # �Ώۃo�[�W����
				else if(( !strcmp("VERSION",aWork))&&(strlen(aVal))){
					m_strVersion = aVal;		
				}
				//< # �o�̓f�B���N�g��
				else if(( !strcmp("OUTPUT",aWork))&&(strlen(aVal))){
					m_strOutputDir = aVal;	
				}
			}
        }
		(void)fclose(fp);
	}
	else{
		cerr << "�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s:\t" << strConfigFile;
		return false;
	}
	if( m_strServer.GetLength() == 0 )
		cerr << "�T�[�o�����͂���Ă��܂���BSERVER�œ��͂��Ă�������" << endl;
	if( m_strInstance.GetLength() == 0 )
		cerr << "�C���X�^���X�����͂���Ă��܂���BINSTANCE�œ��͂��Ă�������" << endl;
	if( m_strUser.GetLength() == 0 )
		cerr << "���[�U�������͂���Ă��܂���BUSER�œ��͂��Ă�������" << endl;
	if( m_strPassword.GetLength() == 0 )
		cerr << "�p�X���[�h�����͂���Ă��܂���BPASSWORD�œ��͂��Ă�������" << endl;
	if( m_strVersion.GetLength() == 0 )
		cerr << "�o�[�W���������͂���Ă��܂���BVERSION�œ��͂��Ă�������" << endl;
	if( m_strOutputDir.GetLength() == 0 )
		cerr << "�o�̓f�B���N�g�������͂���Ă��܂���BOUTPUT�œ��͂��Ă�������" << endl;
	
	if( m_strServer.GetLength() == 0		|| m_strInstance.GetLength() == 0
	 || m_strUser.GetLength() == 0			|| m_strPassword.GetLength() == 0
	 || m_strVersion.GetLength() == 0		|| m_strOutputDir.GetLength() == 0 )
		return false;

	return true;
}

//���C���̏���
bool CDoc::Proc()
{
	//SDE�̃I�[�v��
	if(! OpenSDE()) return false;

	//FACIL_INFO_POINT
	vector<FacilInfo> eFacilInfoVector;

	if(! GetSmartICPoint(eFacilInfoVector)){
		cerr << _T("FACIL_INFO_POINT������̎擾�Ɏ��s") << endl;
		return false;
	}		
	//HIGHWAY_NODE
	vector<atl2::seal<IGeometryPtr>> HwyNodeGeomVector;

	if(! GetHwyNodeGeom(eFacilInfoVector, HwyNodeGeomVector)){
		cerr << "HIGHWAY_NODE�C���̏����Ɏ��s" << endl;
		return false;
	}
	//ROAD_NODE
	set<long> RoadNodeOIDSet;

	if(! GetRoadNodeOID(HwyNodeGeomVector, RoadNodeOIDSet)){
		cerr << "ROAD_NODE�̏����Ɏ��s" << endl;
		return false;
	}	
	//�t�@�C���o��
	if(! SetOfstream()) return false;

	set<long>::iterator	aIter = RoadNodeOIDSet.begin();
	for( ; aIter != RoadNodeOIDSet.end(); aIter++){
		m_ofstream << *aIter << endl;
	}
	return true;
}

//�X�}�[�gIC��Road_Code��Road_Seq�̎擾
bool CDoc::GetSmartICPoint(vector<FacilInfo>& r_eFacilInfoVector)
{
	ITablePtr	ipTable;
	if(! m_cSDEManager.OpenTargetTable(_T("FACIL_INFO_POINT"), m_ipWorkspace, &ipTable)) return false;

	_ICursorPtr ipCursor;
	if(! m_cSDEManager.GetCursor(ipTable, _T("FACILCLASS_C = 7"), &ipCursor)) return false;

	FacilInfo	eFacilInfo;
	_IRowPtr	ipRow;

	while( S_OK==ipCursor->NextRow(&ipRow)){
		eFacilInfo.eRoad_Code	= m_cSDEManager.GetValue(ipRow, _T("ROAD_CODE")).lVal;
		eFacilInfo.eRoad_Seq	= m_cSDEManager.GetValue(ipRow, _T("ROAD_SEQ")).lVal;
		
		r_eFacilInfoVector.push_back(eFacilInfo);
	}	
	return true;
}

//Highway_Node�̃W�I���g���擾
bool CDoc::GetHwyNodeGeom(const vector<FacilInfo>& r_eFacilInfoVector, vector<atl2::seal<IGeometryPtr>>& r_GeomVector)
{
	IFeatureClassPtr ipFeatureClass;
	if(! m_cSDEManager.OpenTargetLayer(_T("HIGHWAY_NODE"), m_ipWorkspace, &ipFeatureClass)) return false;
	
	for(int i=0; i < r_eFacilInfoVector.size(); i++){

		CString strWhereClause;
		strWhereClause.Format(_T("TOLLCLASS_C = 4 AND ROAD_CODE = %ld AND ROAD_SEQ = %ld AND INOUT_C IN (1,2)"),r_eFacilInfoVector[i].eRoad_Code, r_eFacilInfoVector[i].eRoad_Seq);
		
		IFeatureCursorPtr ipFeatureCursor = NULL;
		if(! m_cSDEManager.GetFeatureCursor(ipFeatureClass, strWhereClause, &ipFeatureCursor)) return false;

		IFeaturePtr ipFeature = NULL;
		while( S_OK==ipFeatureCursor->NextFeature(&ipFeature)){
			IGeometryPtr ipGeometry = NULL;
			ipFeature->get_ShapeCopy(&ipGeometry);

			r_GeomVector.push_back(ipGeometry);
		}
	}
	return true;
}

//Road_Node�̃W�I���g���擾
bool CDoc::GetRoadNodeOID(const vector<atl2::seal<IGeometryPtr> >& r_GeomVector, set<long>& r_ObjectIDSet)
{
	ITablePtr	ipTable;
	if(! m_cSDEManager.OpenTargetTable(_T("ROAD_NODE"), m_ipWorkspace, &ipTable)) return false;

	for(int i=0; i < r_GeomVector.size(); i++){
		ISpatialFilterPtr	ipSpatialFilter(CLSID_SpatialFilter);
							ipSpatialFilter->putref_Geometry(r_GeomVector[i]);
							ipSpatialFilter->put_SpatialRel(esriSpatialRelIntersects);
							ipSpatialFilter->put_SubFields(CComBSTR(_T("OBJECTID,SHAPE")));

		_ICursorPtr	ipCursor = NULL;
		if( FAILED( ipTable->Search( ipSpatialFilter, VARIANT_FALSE, &ipCursor ))){
			cerr << "�s�̌����Ɏ��s" << endl;
			return false;
		}

		_IRowPtr	ipRow = NULL;

		while( S_OK==ipCursor->NextRow(&ipRow)){
			long lOID = 0;
			ipRow->get_OID(&lOID);
			r_ObjectIDSet.insert(lOID);
		}
	}
	return true;
}

//SDE�I�[�v��
bool CDoc::OpenSDE(void)
{
	// ���������Ή�
	IUnknownPtr().CreateInstance("{a318a696-3ed1-4775-a922-147e8c36d79d}");

	// �C���|�[�g�Ώۃ��[�N�X�y�[�X
	if( ! m_cSDEManager.ConnectSDE(m_strUser,m_strPassword,m_strInstance,m_strServer,m_strVersion,m_ipWorkspace)){
		return false;
	}
	return true;
}

//SDE�̃N���[�Y
void CDoc::Close()
{
	m_ipWorkspace = NULL;
}

//�o�̓X�g���[���̐ݒ�
bool CDoc::SetOfstream()
{
	//�t�@�C���o�͗p
	CString strOutputFileName = m_strOutputDir + _T("\\") + m_strUser + _T("_SmartNodeList.txt");

	m_ofstream.open(strOutputFileName);	
	if( !m_ofstream ){
		cerr << LPCTSTR(strOutputFileName) << "�̏������݂Ɏ��s" << endl;
		return false;
	}
	return true;
}
