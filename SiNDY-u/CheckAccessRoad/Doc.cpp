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

// Doc.cpp: CDoc �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include "Doc.h"
#include <fstream>
#include <algorithm>
#include <sindy/workspace.h>
#include <map.h>
#include <sindyk.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CDoc::CDoc():m_strLogFile(_T("accessroad_err.txt")),
	m_mkFileDir(_T(".")),
	m_CheckDist(100),
	m_DebugMode(false),
	m_FacilListWrite(false)
{
	m_Fp = NULL;
	m_ipWorkspace = NULL;
	m_ipLinkFeatureClass = NULL;
	m_ipNodeFeatureClass = NULL;
	m_ipAdmFeatureClass = NULL;
	m_ipBaseSiteFeatureClass = NULL;
	m_ipBaseLineFeatureClass = NULL;
	m_ipLQ_Route = NULL;
	m_ipINF_Route = NULL;
}
CDoc::~CDoc()
{
	deleteMap();
}
/**
* �{�ݏ��|�C���g�R���e�i���
* @note �{�ݏ��|�C���g�R���e�i��������܂��B
* @param void
* @return	void
*/
void CDoc::deleteMap(void)
{
	m_FacilPoints.clear();
}
/**
* �{�ݏ��|�C���g�t�@�C�����AShapefile�ɕϊ�
* @note �{�ݏ��|�C���g�t�@�C�����AShapeFile�ɕϊ����܂��B
* @param �{�ݏ��o�̓t�@�C����
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
bool CDoc::convertFacilPoint(LPCTSTR sShapeFile)
{
	//�{�ݏ��|�C���g��Shape�ϊ�
	if( makeFacilPoint(sShapeFile))
	{
		//�{�ݏ��|�C���g�����G���A��Shape�ϊ�
		if( makeCheckArea(sShapeFile))
		{
			return true;
		}
		else
		{
			std::cerr << "�{�ݏ��|�C���g�����G���A��Shape�ϊ����s" << std::endl;
			return false;
		}
	} 
	else 
		return false;
}

// �{�ݏ����擾�����A�̗�������s
bool CDoc::getFacilPoint(void)
{
	deleteMap();

	std::vector<long> aContentsVec;

	if( !readContentsList(aContentsVec) ){
		std::cerr << "�R���e���c���X�g�̓ǂݍ��݂Ɏ��s" << std::endl;
		return false;
	}
	HRESULT hr;
	IWorkspacePtr ipPOIWorkspace = sindy::_create_workspace(m_strPOIDB, hr );
	if( FAILED(hr) ){
		std::cerr << "POIDB�̃��[�N�X�y�[�X�̎擾�Ɏ��s" << std::endl;
		return false;
	}
	IFeatureWorkspacePtr ipFeatureWorkspace(ipPOIWorkspace);

	IFeatureClassPtr ipPOIPointFeatureClass, ipPOISubPointFeatureClass;
	if( !OpenFeatureClass(ipFeatureWorkspace, sindy::schema::sindyk::poi_point::kTableName, &ipPOIPointFeatureClass) 
		|| !OpenFeatureClass(ipFeatureWorkspace, sindy::schema::sindyk::poi_sub_point::kTableName, &ipPOISubPointFeatureClass) ){
			std::cerr << "POIDB�̃t�B�[�`���N���X�̎擾�Ɏ��s���܂���" << std::endl;
			return false;
	}

	using namespace sindy::schema::sindyk;

	BOOST_FOREACH( const long& rContentsID, aContentsVec){
		cout << "\r�{�݃|�C���g�̎擾��...";
		// POI_POINT������Ă���
		CString aWhere1;
		aWhere1.Format(_T("%s = %d AND %s = %d"), poi_point::kContentsCode, rContentsID, poi_point::kDelete, poi_point::delete_code::kAlive );
		std::vector< CAdapt<IFeaturePtr> > aPOIPointFeatureVec;
		SelectByWhereClause(ipPOIPointFeatureClass, aWhere1, aPOIPointFeatureVec);

		// ����Ă���POI_POINT������
		BOOST_FOREACH( const CAdapt<IFeaturePtr>& rAFeature1, aPOIPointFeatureVec){
			const IFeaturePtr& rFeature1 = rAFeature1.m_T;

			// �Ƃ肠�����{�݃|�C���g��ݒ�
			CFacilPoint aFacilPoint;
			WKSPoint aWKSPoint1 = {0,0};
			if( !GetFeaturePoint(rFeature1, aWKSPoint1) ){
				continue;
			}
			std::pair<int, int> aLL256Pair1 = getLL256(aWKSPoint1);
			aFacilPoint.setLon(aLL256Pair1.first);
			aFacilPoint.setLat(aLL256Pair1.second);
			aFacilPoint.setKanji(GetValue( rFeature1, poi_point::kName).bstrVal);

			CString aContentSeq=CString(GetValue( rFeature1, poi_point::kContentsSeq ).bstrVal);
			CString aCode; aCode.Format(_T("%d-%d-%s"), long(rContentsID/100000), rContentsID%100000, aContentSeq);
			aFacilPoint.setCode(aCode);

			// POI_POINT��ID�ɑΉ����A�폜�t���O���u0:�����v��SUBPOINT������Ă���
			long aPOIPointID = -1;	rFeature1->get_OID(&aPOIPointID);

			CString aWhere2;
			aWhere2.Format(_T("%s = %d AND %s = %d"), poi_sub_point::kPOIPointID, aPOIPointID, poi_sub_point::kDelete, poi_sub_point::delete_code::kAlive );
			std::vector< CAdapt<IFeaturePtr> > aPOISubPointFeatureVec;
			long aCount = SelectByWhereClause(ipPOISubPointFeatureClass, aWhere2, aPOISubPointFeatureVec);

			// SUBPOINT���Ȃ���΁A���̂܂ܓo�^
			if( aCount == 0 ){
				aFacilPoint.setFacilPointXY();
				m_FacilPoints.push_back(aFacilPoint);
				cout << "\r�{�݃|�C���g�̎擾��...\t" << m_FacilPoints.size();
			}
			else{
				BOOST_FOREACH( const CAdapt<IFeaturePtr>& rAFeature2, aPOISubPointFeatureVec){
					const IFeaturePtr& rFeature2 = rAFeature2.m_T;

					WKSPoint aWKSPoint2 = {0,0};
					if( !GetFeaturePoint(rFeature2, aWKSPoint2) ){
						continue;
					}
					CFacilPoint aSubFacilPoint(aFacilPoint);
					std::pair<int, int> aLL256Pair2 = getLL256(aWKSPoint2);
					aSubFacilPoint.setLon(aLL256Pair2.first);
					aSubFacilPoint.setLat(aLL256Pair2.second);
					aSubFacilPoint.setFacilPointXY();
					m_FacilPoints.push_back(aSubFacilPoint);
					cout << "\r�{�݃|�C���g�̎擾��...\t" << m_FacilPoints.size();
				}
			}
		}
	}
	cout << "\r�{�݃|�C���g�̎擾�I��\t" << m_FacilPoints.size() << endl;

	if( m_FacilListWrite ){
		cout << "\r�{�݃��X�g�̏������ݒ�";
		if( !writeFacilList() ){
			return false;
		}
		cout << "\r�{�݃��X�g�̏������ݏI��\t" << m_FacilPoints.size() << std::endl;
	}

	return true;
}

// �{�ݏ���XML(Excel�̃X�v���b�h�V�[�g�`��)���o�͂���
bool CDoc::writeFacilList()
{
	CString strOutFileName;
	strOutFileName.Format("%s\\access_list.xml", m_mkFileDir);

	ofstream aOutStream(strOutFileName);
	if( !aOutStream ){
		cout << (LPCSTR)CStringA(strOutFileName) << "�̏������݂Ɏ��s" << endl;
		return false;
	}
	aOutStream << "<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>" << endl;
	aOutStream << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\">" << endl;
	aOutStream << "<Worksheet ss:Name=\"access_list\">" << endl;
	aOutStream << "<Table>" << endl;

	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// �ܓx�̕���60(���ۂɂ�80�ɒu����������)
	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// �o�x�̕���60(���ۂɂ�80�ɒu����������)
	aOutStream << "<Column ss:Width=\"360\"/>" << endl;	// �{�ݖ��̂̕���360(���ۂɂ�480�ɒu����������)
	aOutStream << "<Column ss:Width=\"240\"/>" << endl;	// �V�[�P���X�ԍ��̕���240(���ۂɂ�320�ɒu����������)
	aOutStream << "<Column ss:Width=\"60\"/>" << endl;	// ���l���̕���60(���ۂɂ�80�ɒu����������)

	CString aHeaderColumn[5] = { _T("�o�x"), _T("�ܓx"), _T("�{�ݖ���"), _T("�V�[�P���X�ԍ�"), _T("���l") };
	writeExcelRow(aHeaderColumn, 5, &aOutStream);

	BOOST_FOREACH( const CFacilPoint& rFacilPoint, m_FacilPoints){
		CString aColumn[5];
		aColumn[0].Format("%d", rFacilPoint.getLon());
		aColumn[1].Format("%d", rFacilPoint.getLat());
		aColumn[2] = rFacilPoint.getKanji();
		aColumn[3] = rFacilPoint.getCode();
		writeExcelRow( aColumn, 5, &aOutStream );
	}

	aOutStream << "</Table>" << endl;
	aOutStream << "</Worksheet>" << endl;
	aOutStream << "</Workbook>" << endl;

	aOutStream.close();

	return true;
}

// Excel�̍s������
void CDoc::writeExcelRow(const CString* rStrAry, const int& rColNum, ofstream* pOfstream) const
{
	*pOfstream << "\t<Row>";
	for(int i=0; i<rColNum; ++i){
		writeExcelCell(rStrAry[i], pOfstream);
	}
	*pOfstream << "</Row>" << endl;
}

// Excel�̃Z���̒��g������
void CDoc::writeExcelCell(LPCTSTR strValue, ofstream* pOfstream) const
{
	*pOfstream << "<Cell><Data ss:Type=\"String\">" << (LPCSTR)CStringA(strValue) << "</Data></Cell>";
}

// �R���e���c���X�g��ǂݍ���
bool CDoc::readContentsList(std::vector<long>& rContentsVec)
{
	ifstream aStream;
	aStream.open(m_strContentsList);
	if( !aStream )
	{
		CString	aStr; aStr.Format(" Open Error [%s]",m_strContentsList);
		AfxMessageBox(aStr,MB_ICONHAND);
		return false;
	}

	std::string aLine;
	while( getline( aStream, aLine ) )
	{
		std::string::size_type aPos = aLine.find_first_of("-");
		if( std::string::npos != aPos )
		{
			std::string aZen = aLine.substr(0, aPos);
			std::string aKou = aLine.substr(aPos+1);
			long aZenNum = atol(aZen.c_str());
			long aKouNum = atol(aKou.c_str());
			rContentsVec.push_back(aZenNum*100000+aKouNum);
		}
	}
	std::sort(rContentsVec.begin(), rContentsVec.end());
	std::unique(rContentsVec.begin(), rContentsVec.end());

	aStream.close();

	return true;
}

/**
* �{�ݏ��R���e�i���AShapefile�ɕϊ�
* @note �{�ݏ��R���e�i���AShapeFile�ɕϊ����܂��B
* @param �{�ݏ��o�̓t�@�C����
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
bool CDoc::makeFacilPoint(LPCTSTR sFname)
{
	int	aFID[10];
	char *aFStr[] = {"KANJI","SEQCODE","MESHCODE","XPOS","YPOS","TYPE","LON","LAT"};
	// SHP�f�[�^�̐V�K�쐬
	if( !m_SHPHdl.create(sFname, SHPT_POINT)){
		cerr << sFname << " :ShpFile Create ERROR." << endl;
		return false ;
	}
	// DBF�f�[�^�̐V�K�쐬
	if( !m_DBFHdl.create(sFname)){
		cerr << sFname << " :DbfFile Create ERROR." << endl;
		return false ;
	}
	// DBF�̃t�B�[���h�̍쐬
	for( int i=0,j=0; i<8; i++ ){
		if( i < 2 ){
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTString,128,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
		else{
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTInteger,8,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
	}
	//�{�ݏ��|�C���g�̕ϊ�
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it ++, i++ ){
		double		aX = it->getFacilDX();
		double		aY = it->getFacilDY();
		if( i%10==0 )
			cout << "\r�{�݃|�C���g��Shape�ϊ���...\t" << i+1;

		// shape�`��̓o�^
		CSHPPoint	aPoint(aX,aY);
		CSHPObj		aArcObj(SHPT_POINT, aPoint);
		int aShapeID = m_SHPHdl.writeObject(aArcObj, -1);
		if(aShapeID < 0){
			cerr << it->getMeshcode() << "\t[" << it->getKanji() << "]";
			cerr << "\t�I�u�W�F�N�g�̏������ݎ��s:"<< std::endl;
			continue;
		}
		int j = 0;
		// �{�ݖ��́i�����j
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getKanji().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �V�[�P���X�R�[�h
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getCode().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// ���b�V���R�[�h
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getMeshcode())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// ���K���w���W
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getX())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// ���K���x���W
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getY())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �{�ݎ�ʃR�[�h
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getType())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �o�x
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getLon())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �ܓx
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getLat())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
	}
	cout << "\r�{�݃|�C���g��Shape�ϊ��I��\t\t" << m_FacilPoints.size() << endl;
	return true;
}
/**
* �{�ݏ��|�C���g�����G���A��Shape�ϊ�
* @note �{�ݏ��R���e�i���A�����Ώۋ�`ShapeFile�ɕϊ����܂��B
* @param �{�ݏ�񌟍��G���A�o�̓t�@�C����
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
bool CDoc::makeCheckArea(LPCTSTR sFname)
{
	if( !m_FacilPoints.size())	//�f�[�^�Ȃ�
		return false;
	int		aFID[10];
	char   *aFStr[] = {"KANJI","SEQCODE","MESHCODE","TYPE"};
	char	aFname[256];
	sprintf(aFname,"%s_Area",sFname);
	if( !m_SHPHdl.create(aFname, SHPT_POLYGON)){
		cerr << sFname << " :ShpFile Create ERROR." << endl;
		return false ;
	}
	// DBF�f�[�^�̐V�K�쐬
	if( !m_DBFHdl.create(aFname)){
		cerr << sFname << " :DbfFile Create ERROR." << endl;
		return false ;
	}
	// DBF�̃t�B�[���h�̍쐬
	for( int i=0; i<4; i++ ){
		if( i < 2 ){
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTString,128,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
		else{
			if((aFID[i] = m_DBFHdl.addField(aFStr[i],FTInteger,8,0)) < 0){
				cerr << aFStr[i] << " :Field Create ERROR." << endl;
				return false ;
			}
		}
	}
	//�{�ݏ��|�C���g�̕ϊ�
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it++, i++ ){
		double		aDX = it->getFacilDX();
		double		aDY = it->getFacilDY();
		int			aX  = it->getX();
		int			aY  = it->getY();
		int			aBaseMesh = it->getMeshcode();

		//�{�݃|�C���g�𒆐S�ɋ�`���쐬���鋗��
		// �~�P�D�Q�@�w�苗�����]���Ƀo�b�t�@���������邽��
		// �~�P�O�O�@2�����b�V���R�[�h��100���~100���̐��K��XY���W�ŕ\�����ꍇ��
		//			 X�AY�Ƃ���ӂ̒�����10km�Ɖ��肷��ƁA1���W������0.01m�ƂȂ邽�߁A100�{����
		int			aDist = m_CheckDist*1.2*100;		//�{�݃|�C���g�𒆐S�G���A�����i���j�ɕϊ�

		//�����G���A�������W
		CMeshPos	aSPos(aBaseMesh,aX,aY);
		aSPos.SetLeftDownPos(aBaseMesh,aX,aY,aDist);
		//�����G���A�E����W
		CMeshPos	aEPos(aBaseMesh,aX,aY);
		aEPos.SetRightTopPos(aBaseMesh,aX,aY,aDist);
		if( !aSPos.GetMeshcode() || !aEPos.GetMeshcode()){
			continue;
		}
		// �|�C���g�z��ɓo�^
		CSHPPointArray aPointArray; // �`��N���X
		aPointArray.append(aSPos.getLon(), aSPos.getLat());	//(1)�œ쐼		   2���|�|�|�|�|��3
		aPointArray.append(aSPos.getLon(), aEPos.getLat());	//(2)�Ŗk��         | �A�N�Z�X��|
		aPointArray.append(aEPos.getLon(), aEPos.getLat());	//(3)�Ŗk��         | �`�F�b�N��|
		aPointArray.append(aEPos.getLon(), aSPos.getLat());	//(4)�œ쓌         |    �͈�   |
		aPointArray.append(aSPos.getLon(), aSPos.getLat());	//(5)�œ쐼      1,5���|�|�|�|�|��4

		// shape�I�u�W�F�N�g�̍쐬
		CSHPObj aPolyObj(SHPT_POLYGON, aPointArray);

		// (���̂Ƃ��A�C���f�b�N�X��-1�ɂ���ƍŌ�ɒǉ�)
		int aShapeID = m_SHPHdl.writeObject(aPolyObj, -1);
		if( aShapeID < 0 ){ // 0�����̓G���[
			cerr << "Shape�I�u�W�F�N�g�̒ǉ��Ɏ��s:" << endl;
			continue;
		}
		//�`�F�b�N�͈͂�o�^
		it->setCheckArea(aSPos.getLon(), aSPos.getLat(), aEPos.getLon(), aEPos.getLat());

		int j = 0;
		// �{�ݖ��́i�����j
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getKanji().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �V�[�P���X�R�[�h
		if( !m_DBFHdl.writeStringAttribute(aShapeID, aFID[j], it->getCode().GetBuffer(256))){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// ���b�V���R�[�h
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getMeshcode())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
		// �{�ݎ�ʃR�[�h
		if( !m_DBFHdl.writeIntegerAttribute(aShapeID, aFID[j], it->getType())){
			cerr << it->getMeshcode() <<"\tERROR " << aFStr[j] <<" Write.";
		} j++;
	}
	return true;
}
/**
* �A�N�Z�X���H�`�F�b�N���ʃt�@�C���I�[�v��
* @note �A�N�Z�X���H�`�F�b�N���ʃt�@�C�����I�[�v�����܂��B
* @param �Ȃ�
* @return	�Ȃ�
*/
void CDoc::CheckStart(void)
{
	//�G���[���O�t�@�C���̎w��
	char	sAccessLogFile[256];
	sprintf(sAccessLogFile,"%s\\%s",getLogDirStr(),getLogFileStr());
	if( NULL!=(m_Fp=fopen(sAccessLogFile,"w"))){
		CheckStop();
		if( NULL==(m_Fp=fopen(sAccessLogFile,"a"))){
			cerr << "ERROR " << sAccessLogFile <<" Open.";
		}
	}
}
/**
* �A�N�Z�X���H�`�F�b�N���ʃt�@�C���N���[�Y
* @note �A�N�Z�X���H�`�F�b�N���ʃt�@�C�����N���[�Y���܂��B
* @param �Ȃ�
* @return	�Ȃ�
*/
void CDoc::CheckStop(void)
{
	if( m_Fp ){
		(void)fclose(m_Fp);
		m_Fp = NULL;
	}
}
/**
* �{�ݏ��|�C���g�t�@�C�����AShapefile�ɕϊ�
* @note �{�ݏ��|�C���g�t�@�C�����AShapeFile�ɕϊ����܂��B
* @return	0�F����I���A1:�ϊ����s�A3�F�ϊ�����Shape�ɃG���[����
*/
int CDoc::checkFacilPoint(void)
{
	int iRET=0;
	//�{�ݏ��|�C���g�ƃA�N�Z�X���H�̔���J�n
	CheckStart();
	CString	aStr;
	int i=0;
	for( FacilArray::iterator it = m_FacilPoints.begin(); it!=m_FacilPoints.end(); it++, i++ ){
		//�`�F�b�N�i�s��
		cout << "\rcheck�� " << i+1 << "/" << m_FacilPoints.size();
		cout << "  " << it->getMeshcode();
		cout << "  " << it->getKanji().GetBuffer(256) << flush;

		//�`�F�b�N�������Z�b�g(�w�苗����10�{�ɂ��āA�w�苗���ȏ�̏ꍇ�ł������v�����o����悤�ɂ��Ă���)
		it->setDistance(m_CheckDist*10);

		//�`�F�b�N�p��`�|���S���쐬
		CNwArea	aNwArea = it->getCheckArea();
		IEnvelopePtr ipNwAreaEnv( CLSID_Envelope );
		ipNwAreaEnv->PutCoords(aNwArea.getSx(), aNwArea.getSy(), aNwArea.getEx(), aNwArea.getEy());

		//-------------------------------------------------------------------------------------
		//���̎{�݂ɑ΂���A�`�F�b�N�G���A���̍ŋߖT�����N���擾
		//-------------------------------------------------------------------------------------
		IPointPtr ipFacilPoint(CLSID_Point);
		ipFacilPoint->PutCoords(it->getFacilDX(), it->getFacilDY());

		//�{�݃|�C���g�̓o�^
		it->setFacilPoint(ipFacilPoint);

		using namespace sindy::schema;

		IFeatureCursorPtr	ipLinkFeatureCursor;
		esriSpatialRelEnum aSpatialRel = esriSpatialRelIntersects;					//����{�N���X
		long lCount = SelectByShape(ipNwAreaEnv, m_ipLinkFeatureClass, &ipLinkFeatureCursor, aSpatialRel);
		if( lCount > 0 ){
			IFeaturePtr		ipLinkFeature;
			while(( S_OK==ipLinkFeatureCursor->NextFeature( &ipLinkFeature ))&&(ipLinkFeature != NULL)){
				if( !isNaviLink(VIEW_ARC_FERRY,ipLinkFeature))
					continue;
				IGeometryPtr	ipLinkGeom;
				if( SUCCEEDED( ipLinkFeature->get_ShapeCopy( &ipLinkGeom ))){
					CComVariant vaVal = GetValue(ipLinkFeature,_T("OBJECTID"));
					CComVariant vaSID = GetValue(ipLinkFeature,road_link::kFromNodeID);
					CComVariant vaEID = GetValue(ipLinkFeature,road_link::kToNodeID);					
					if( vaVal.vt==VT_NULL )
						continue;					
					//�������铹�H��ʂ��ǂ������`�F�b�N����
					if( isIgnoreLink(ipLinkFeature)){
						continue;
					}
					//�A�N�Z�X�|�C���g�ƍŋߖT�����N�̋������v�Z
					IPointPtr ipNearestPoint;
					double	aNearDistance = GetNearestDist(ipLinkGeom, ipFacilPoint, ipNearestPoint);
					//�������������Ȃ��ꍇ�́A�ǐՑΏۃ����N�ȊO�͏��O����
					if(aNearDistance == it->getDistance() ){
						//��{���H�����Ɉ�v���邩�ǂ���
						if(! isNaviLink(VIEW_ARC_NAVI, ipLinkFeature)){
							//��{���H�ł͂Ȃ��ꍇ�ARouteLqrf���`�F�b�N
							if(! isLinkQueRoute(ipLinkFeature)){
								continue;
							}
						}
					}
					//�����𔻕�
					if( aNearDistance <= it->getDistance()){
						//�ŋߖT�̃����N������ێ�����(�w�苗���ȏ�ł��ŋߖT�����N��ێ�����)
						it->setDistance(aNearDistance);
						//�w�苗�������̏ꍇ�͓o�^����
						if(aNearDistance <= m_CheckDist){
							it->getNearLink().setOID(vaVal.intVal);
							it->getNearLink().setSNID(vaSID.intVal);
							it->getNearLink().setENID(vaEID.intVal);
							it->getNearLink().setFeature(ipLinkFeature);
							it->getNearLink().setGeometry(ipLinkGeom);
							it->setNearestPoint(ipNearestPoint);
						}
					}
				}
			}
			//�ڎ��`�F�b�N�p���C��(CheckLine)�@����Ő��n�`�F�b�N�ɂ��g�p����
			if(! CreateCheckLineShape(&m_FacilPoints[i])){
				std::cerr << "�ڎ��`�F�b�N�p���C���̍쐬�Ɏ��s���܂���" << std::endl;
				return 1;
			}
			//-------------------------------------------------------------------------------------
			//�ŋߖT�����N���A��{���H�i����5.5���ȏ�̓��H�j�ɓ��B�ł��邩����
			//-------------------------------------------------------------------------------------
			IFeaturePtr	 ipCheckFeature = it->getNearLink().getFeature();
			IGeometryPtr ipCheckGeom    = it->getNearLink().getGeometry();
			LinkArray aTraceLinkArray;
			aTraceLinkArray.insert(it->getNearLink().getOID());
			if( ipCheckFeature!=NULL ){
				it->setAccessSts(CheckAccess(ipCheckFeature, ipCheckGeom, aTraceLinkArray));
			}
		}
		//�ŋߖT�����N�ǐՌ��ʂ��󂯂āA�ǉ��̃`�F�b�N�����s
		switch(it->getAccessSts()){
		case 0:		//�ŋߖT�����N��������Ȃ������ꍇ
			break;
		case 1:
		case 2:
			//�ǐՐ��������ꍇ�͐��n�`�F�b�N
			if(isCrossSuikei(&m_FacilPoints[i])){
				//���n�G���[�ɕύX
				it->setAccessSts(5);
			}
			break;
		default:
			//�ŋߖT�����N�ɐڑ����Ă��郊���N���`�F�b�N�Ώۂɂ���
			IGeometryPtr ipNerestGeom = it->getNearLink().getGeometry();
			//����ʍs���l������
			int aOneway = GetValue(it->getNearLink().getFeature(), road_link::kOneway).intVal;
			//�����Ȃ�����ʍs�̏ꍇ�́A�I�_ or �n�_�̂ݒǐՂ���
			if(isOnewayLink(aOneway)){
				//�����N���|�C���g��
				IPointCollectionPtr ipPointCol(it->getNearLink().getGeometry());
				IPointPtr ipTmpPoint;
				if(aOneway==1){
					//�������̏ꍇ�̓����N�̎n�_
					ipPointCol->get_Point(0, &ipTmpPoint);
				}
				else{
					//�t�����̏ꍇ�̓����N�̏I�_
					long aPointCount=0;
					ipPointCol->get_PointCount(&aPointCount);
					ipPointCol->get_Point(aPointCount - 1, &ipTmpPoint);
				}
				ipNerestGeom = ipTmpPoint;
			}
			//�ڑ������N���`�F�b�N����
			if(CheckConnectLink(it->getFacilPoint(), ipNerestGeom, it->getNearLink().getOID())){
				//�ŋߖT�ɐڑ����Ă��郊���N���A�������A�N�Z�X���H�̏ꍇ�́yWARNING�z
				it->setAccessSts(10);
			}
			else{
				//Bug:1569 �Ή�	2003/5/19	�����̓`�F�b�N�ΏۂƂ��Ȃ� �������p���Ń`�F�b�N�ꏊ�ύX
				//�ǐՂɐ������Ȃ������ꍇ�͗����t���O�`�F�b�N
				CComVariant vaVal = GetValue(it->getNearLink().getFeature(),road_link::kIsland);
				if( vaVal.intVal==1 ){
					it->setAccessSts(7);
				}
			}
			break;
		}
		//-------------------------------------------------------------------------------------
		//�A�N�Z�X���H�`�F�b�N���ʂ̏o��
		//-------------------------------------------------------------------------------------
		if (! showAccessRoad(i+1,&m_FacilPoints[i]))
			iRET = 3;
	}
	cout << "                                                                                   \r";
	cout << "\r�A�N�Z�X���H�`�F�b�N�I��\t\t" << m_FacilPoints.size() << endl;
	deleteMap();
	CheckStop();
	return iRET;
}

/**
* @brief	���B�\�`�F�b�N
* @note	�Y�������N���o�H�Ώۓ��H�ɓ��B�ł��邩�`�F�b�N
* @param	piFeature	[in]	���B�\�����`�F�b�N�������t�B�[�`��
* @param	piGeometry	[in]	���B�\�����`�F�b�N�������t�B�[�`���̃W�I���g��
* @param	rLink		[out]	�ǐՍς݊m�F�p
* @return	int	�A�N�Z�X�R�[�h
*/
int CDoc::CheckAccess(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink)
{
	//�ŋߖT�����N�̃����N�������Ȃ��̒ʍs�֎~�̏ꍇ�̓G���[
	if(isNopassLink(piFeature)){
		//�ʍs�֎~�G���[
		return 9;
	}
	//�ŋߖT�����N�́A��{���H�{���H����5.5m�ȏォ����
	else if(isNaviLink(VIEW_ARC_NAVI, piFeature)){
		return IsTraceSuccessLink(piFeature, piGeometry, rLink);
	}
	//�ŋߖT�����N���A�N�Z�X���H�����N�̏ꍇ
	else if(isLinkQueRoute(piFeature)){
		int aStat=0;
		switch( LinkTrace(piFeature,piGeometry, rLink)){
		case 1:				//�ySUCCESS�z��{���H�ɐڑ�
			aStat = 1; 
			break;
		case 2:				//�ySUCCESS�z���[�g�����N��o�R�Ŋ�{���H�ɐڑ�
			aStat = 2; 
			break;
		case 8:				// �v�ڎ��m�F����ǉ� ADD 2006/9/30
			aStat = 8;
			break;
		default:		    //�yERROR�z�{�݂܂œ��B�ł��Ȃ�
			aStat = 3;
			break;
		}
		return aStat;
	}
	else{
		//�ŋߖT�����N���ǂ��ɂ��ǐՂł��Ȃ��P�[�X
		return 3; //�yERROR�z�{�݂܂œ��B�ł��Ȃ�
	}
}

//�{�݃|�C���g�ƍŋߖT�|�C���g�𒼐��Ō���SHAPE������
bool CDoc::CreateCheckLineShape(CFacilPoint *cFacil)
{
	//OID=0�͑ΏۂƂ��Ȃ�
	if(cFacil->getNearLink().getOID()){
		//�{�݃|�C���g�ƍŋߖT�����N�̃|�C���g�𒼐��Ō���
		//�{�݃|�C���g
		IPointPtr ipPoint1 = cFacil->getFacilPoint();
		//�ŋߖT�|�C���g
		IPointPtr ipPoint2 = cFacil->getNearestPoint();
		//�����̍쐬
		IPolylinePtr	ipCheckLine( CLSID_Polyline );
		ipCheckLine->SetEmpty();
		((IPointCollectionPtr)ipCheckLine)->AddPoint( (IPointPtr)ipPoint1 );
		((IPointCollectionPtr)ipCheckLine)->AddPoint( (IPointPtr)ipPoint2 );
		//�o�^����
		cFacil->setCheckLine(ipCheckLine);

		double dX1=.0, dY1=.0, dX2=.0, dY2=.0;
		ipPoint1->get_X(&dX1);	ipPoint1->get_Y(&dY1);
		ipPoint2->get_X(&dX2);	ipPoint2->get_Y(&dY2);

		//���n�����m�F�p�A���C��Shape�쐬
		static	bool bStart = true;
		static  int aHdl;
		if( bStart ){
			char	sShapeFileStr[256];
			sprintf(sShapeFileStr,"%s\\%s",getLogDirStr(),"CheckLine");
			if( !m_SHPHdl.create(sShapeFileStr, SHPT_ARC)){
				cerr << " :ShpFile Create ERROR." << endl;	return false ;
			}
			if( !m_DBFHdl.create(sShapeFileStr)){
				cerr << " :DbfFile Create ERROR." << endl;	return false ;
			}
			if((aHdl = m_DBFHdl.addField("HitTestID",FTInteger,8,0)) < 0){
				cerr << " :Field Create ERROR." << endl;	return false ;
			}
		}
		bStart = false;
		CSHPPointArray aPointArray;	aPointArray.append(dX1,dY1);
		aPointArray.append(dX2,dY2);
		CSHPObj aArcObj(SHPT_ARC, aPointArray);
		int aShapeID = m_SHPHdl.writeObject(aArcObj, -1);
		if(aShapeID < 0){
			cerr << "\t�I�u�W�F�N�g�̏������ݎ��s:"<< std::endl;
			return false;
		}
		m_DBFHdl.writeIntegerAttribute(aShapeID,aHdl,cFacil->getMeshcode());
	}
	return true;
}

/**
* @brief	�ڑ������N�̃`�F�b�N
* @note	�Y�������N�ɐڑ����Ă��郊���N���A���B�\�����N�����`�F�b�N
* @param	piFacilPoint		[in]	�`�F�b�N�������{�݃|�C���g
* @param	piNearestGeometry	[in]	�ڐG���肷��W�I���g��
* @param	cTracedLinkOID		[out]	�ŋߖT�����N��OID
* @return	true	�ڑ������N����ʓ��H�ɐڑ�
false	�ڑ������N����ʓ��H�ɐڑ����Ȃ�
*/
bool CDoc::CheckConnectLink(const IPointPtr piFacilPoint, const IGeometryPtr piNearestGeometry, long cTracedLinkOID)
{
	//�ڑ������N�̎擾
	IFeatureCursorPtr ipConnectFeatureCursor;
	esriSpatialRelEnum	aSpatialRel = esriSpatialRelTouches;	//�ڐG
	long lCount = SelectByShape(piNearestGeometry, m_ipLinkFeatureClass, &ipConnectFeatureCursor, aSpatialRel);
	if( lCount > 0 ){
		IFeaturePtr		ipConnectFeature;
		while(S_OK==ipConnectFeatureCursor->NextFeature(&ipConnectFeature)){
			//�������铹�H��ʂ��ǂ������`�F�b�N
			if(isIgnoreLink(ipConnectFeature)){
				continue;
			}
			//�W�I���g���̎擾
			IGeometryPtr	ipConnectGeometry;
			ipConnectFeature->get_ShapeCopy(&ipConnectGeometry);
			IPointPtr ipNearestPoint;
			double aDist = GetNearestDist(ipConnectGeometry, piFacilPoint, ipNearestPoint);
			//�w�苗�����߂����𔻒�
			if(aDist <= m_CheckDist){
				//�߂��ꍇ�͒ǐՂ��s��
				long aOID=0;
				ipConnectFeature->get_OID(&aOID);
				//�ǐՍς݂̃����NID�ƈ�v�����ꍇ�́A���O����
				//��ʂ��l�������ꍇ�ɁA�ŋߖT�����N���ēx��������邱�Ƃ�����̂�
				if(cTracedLinkOID == aOID){
					continue;
				}
				//�ǐՍς݃����N�Ƃ��ēo�^
				LinkArray aTraceLinkArray;
				aTraceLinkArray.insert(aOID);
				bool aIsSuccess=false;
				switch(CheckAccess(ipConnectFeature, ipConnectGeometry, aTraceLinkArray))
				{
					//1 or 2�̏ꍇ�́A�ǐ�OK
				case 1:	//�ySUCCESS�z��{���H�ɐڑ�
				case 2:	//�ySUCCESS�z���[�g�����N��o�R�Ŋ�{���H�ɐڑ�
					aIsSuccess = true;	
					break;
					//��L�ȊO�͒ǐ�NG
				default:
					break;
				}
				//���������ꍇ�́Atrue��Ԃ��@//�o�H�Ώۂɓ��B�ł��Ȃ��ꍇ�́A���̐ڑ������N���`�F�b�N
				if(aIsSuccess){
					return true;
				}
			}
		}
	}
	//�ڑ������N�S�Ă��o�H�Ώۓ��H�ɓ��B�ł��Ȃ�
	return false;
}
/**
* �A�N�Z�X���H�̃����N�ǐ�
* @note �w��A�N�Z�X���H���A��{���H�ɐڑ�����܂Ń����N�ǐՂ��s���܂��B
* @param IFeaturePtr	(I)	�����N�t�B�[�`���ւ̃|�C���^
* @param IGeometryPtr	(I)	�����N�W�I���g���ւ̃|�C���^
* @param LinkArray		(I)	�{�݃|�C���g���ӂ̓��H�����N�R���e�i
* @return	true:��{���H�܂ŒǐՐ����Afalse�F��{���H�܂ŒǐՎ��s
*/
int	CDoc::LinkTrace(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink)
{
	//-------------------------------------------------------------------------------------
	//�����N�ǐՂ��������N���A��{���H�ɓ��B�ł��邩����
	//-------------------------------------------------------------------------------------
	int		isOver = 3;	//�f�t�H���g�́yERROR�z�{�݂ɓ��B�ł��Ȃ� ���Z�b�g

	using namespace sindy::schema;

	esriSpatialRelEnum	aSpatialRel = esriSpatialRelTouches;	//�ڐG
	IFeatureCursorPtr	ipChkFeatureCursor;
	CComVariant vaVa = GetValue(piFeature,_T("OBJECTID"));

	//����ʍs���l�����邽�߁A����ʍs�R�[�h���擾���Ă���
	int aPriorLinkOneway = GetValue(piFeature, road_link::kOneway).intVal;

	long lCount = SelectByShape(piGeometry, m_ipLinkFeatureClass, &ipChkFeatureCursor, aSpatialRel);
	if( lCount > 0 ){
		IFeaturePtr		ipNextFeature;
		IGeometryPtr	ipNextGeometry;
		while(( S_OK==ipChkFeatureCursor->NextFeature( &ipNextFeature ))&&(ipNextFeature!=NULL)){
			//Add 2006/9/30 SHIMANO
			//�ǐՔ͈͂��L��T�C�Y�ɂȂ�����ǐՏI��
			if( rLink.size() > 100 ){ //���ۖ،J��Ԃ��񐔂��Ď�
				isOver = 8;
				break;
			}
			//�������铹�H��ʂ��ǂ������`�F�b�N����
			if( isIgnoreLink(ipNextFeature)){
				continue;
			}
			//�ǐՂ̏ꍇ�A�����Ȃ��̒ʍs�֎~�����N�͒ǐՑΏۊO
			if( isNopassLink(ipNextFeature)){
				continue;
			}
			//�����Ȃ�����ʍs���l�����ĒǐՂł��邩?
			if( isOnewayLink(aPriorLinkOneway)){
				//�Ώێ{�݂ɓ��B�ł��邩�ǂ����Ȃ̂ŁA����ʍs�ɋt�����Ă��郊���N�ł����OK
				//�Ώێ{�݂���́A����ʍs�ŋA��Ȃ��Ă�OK�Ƃ����`�F�b�N�d�l
				long aCommonNode=0;	//���ʂ��Ď����Ă���ׂ��m�[�hID
				if(aPriorLinkOneway==1){
					//�������̏ꍇ��FromNode�����ʂ̃m�[�h�Ƃ��Ă��Ȃ���΂����Ȃ�
					aCommonNode = GetValue(piFeature, road_link::kFromNodeID).lVal;
				}
				else{
					//�t�����̏ꍇ��ToNode�����ʂ̃m�[�h�Ƃ��Ă��Ȃ���΂����Ȃ�
					aCommonNode = GetValue(piFeature, road_link::kToNodeID).lVal;
				}
				long aFromNode = GetValue(ipNextFeature, road_link::kFromNodeID).lVal;
				long aToNode = GetValue(ipNextFeature, road_link::kToNodeID).lVal;
				//FromTo�ǂ���Ƃ���v���Ȃ���ΒǐՑΏۊO
				if( aFromNode != aCommonNode && aToNode != aCommonNode){
					continue;
				}
			}
			//���̃����N�͒ǐՍρH
			if( !isTraceOverLink(ipNextFeature,rLink)){
				if( SUCCEEDED( ipNextFeature->get_ShapeCopy( &ipNextGeometry ))){
					CComVariant vaVal = GetValue(ipNextFeature,_T("OBJECTID"));
					//�ŋߖT�����N�͊�{���H�{���H����5.5m�ȏォ����
					if( isNaviLink(VIEW_ARC_NAVI,ipNextFeature)){
						//�ǐՂɐ����������ǂ����B����ʍs���l�����ĒǐՂł��Ȃ��ꍇ������
						isOver = IsTraceSuccessLink(ipNextFeature, ipNextGeometry, rLink);
						//���������ꍇ�́A����ȏ�ǐՂ��Ȃ�
						if(isOver == 1 || isOver == 2){
							break;
						}
					}
					//�ڑ����Ă��铹�H��S�Ĕ���
					else if( isLinkQueRoute(ipNextFeature)){
						isOver = LinkTrace(ipNextFeature, ipNextGeometry, rLink);
						if( isOver == 1 || isOver == 2 ){
							//�ySUCCESS�z�A�N�Z�X���H�o�R�Ŋ�{���H�ɐڑ�
							isOver = 2;
							break;
						}
					}
				}
			}
		}
	}
	return( isOver );
}
/**
* �A�N�Z�X���H�����F���̃����N�͒ǐՍς݂�����
* @note �w��A�N�Z�X���H�ǐՂŁA���̃����N�͒ǐՍς݂����肵�܂�
* @param IFeaturePtr	(I)	�����N�t�B�[�`���ւ̃|�C���^
* @param LinkArray		(I)	�{�݃|�C���g���ӂ̓��H�����N�R���e�i
* @return	true:�ǐՍςݓ��H�ł���Afalse�F�ǐՍςݓ��H�ł͂Ȃ�
*/
bool CDoc::isTraceOverLink(IFeaturePtr piFeature,LinkArray& rLink)
{
	long aBaseLinkID =  GetValue(piFeature,_T("OBJECTID")).lVal;
	if( rLink.find(aBaseLinkID) != rLink.end()){
		return true;
	}

	rLink.insert(aBaseLinkID);

	return false;
}
/**
* �{�݂ƃA�N�Z�X���H�̊Ԃɐ��n�����邩����
* @note �{�݂ƃA�N�Z�X���H�̊Ԃɐ��n�����邩������s���܂��B
* @param CFacilPoint	(I)	�{�݃A�N�Z�X���H���
* @return	true:���n����Afalse�F���n�Ȃ�
*/
bool CDoc::isCrossSuikei(CFacilPoint *cFacil)
{
	using namespace sindy::schema;

	bool aSts=false;
	if( cFacil!=NULL ){
		//�`�F�b�N�p���C��(�{�݃|�C���g�ƍŋߖT�����N�̍ŒZ�_�𒼐��Ō��񂾃��C��)
		IPolylinePtr ipCheckLine = cFacil->getCheckLine();

		//���n�̃q�b�g�e�X�g(�����Ƀq�b�g���邩�ǂ���)
		IFeatureCursorPtr	ipCrossFeatureCursor;
		esriSpatialRelEnum	aSpatialRel = esriSpatialRelCrosses;		//�N���X
		//���n�Ƃ̌�������
		long lCount = SelectByShape(ipCheckLine, m_ipBaseSiteFeatureClass, &ipCrossFeatureCursor, aSpatialRel);
		if( lCount > 0 ){
			IFeaturePtr		ipChkFeature;
			while(( S_OK==ipCrossFeatureCursor->NextFeature( &ipChkFeature ))&&(ipChkFeature != NULL)){
				IGeometryPtr	ipChkGeom;
				if( SUCCEEDED( ipChkFeature->get_ShapeCopy( &ipChkGeom ))){
					CComVariant vaVal = GetValue(ipChkFeature,base_site::kSC4BgClass);
					switch( vaVal.intVal ){
					case base_site::bg_class::kSea		:	//�C
					case base_site::bg_class::kRiver	:	//�͐�E��E�J
					case base_site::bg_class::kLake		:	//�΁E�r�E��
					case base_site::bg_class::kMarsh	:	//���n
						aSts = true;	//���n�ƌ������Ă���		
						break;
					default:
						break;
					}
				}
			}
		}
		//BaseSite�Ő��n�ƃq�b�g���Ă��Ȃ���΁ABaseLine���`�F�b�N
		if(!aSts){
			lCount = SelectByShape(ipCheckLine, m_ipBaseLineFeatureClass, &ipCrossFeatureCursor, aSpatialRel);
			if( lCount > 0 ){
				IFeaturePtr		ipChkFeature;
				while(( S_OK==ipCrossFeatureCursor->NextFeature( &ipChkFeature ))&&(ipChkFeature != NULL)){
					IGeometryPtr	ipChkGeom;
					if( SUCCEEDED( ipChkFeature->get_ShapeCopy( &ipChkGeom ))){
						CComVariant vaOID = GetValue(ipChkFeature,_T("OBJECTID"));
						CComVariant vaVal = GetValue(ipChkFeature,base_line::kBgClass);
						switch( vaVal.intVal ){
						case base_line::bg_class::kRiver		:	//�͐�E��E�J
						case base_line::bg_class::kBreakwater	:	//�h�g��
							aSts = true;	//���n�ƌ������Ă���		
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
	return( aSts );
}
/**
*	@brief	�������郊���N���ǂ���
*	@note	���H���(�׊X�HL3,�t�F���[�q�H,���z�����N,�u���b�W�����N,�{�ݏo����/�������N,�e�Ќ������ꃊ���N) 
*	@retval	true	�������郊���N
false	�������Ȃ������N
*/
bool CDoc::isIgnoreLink(_IRowPtr ipRow)
{
	using namespace sindy::schema;

	int aRoad_C = GetValue(ipRow, road_link::kRoadClass).intVal;
	if( aRoad_C==road_link::road_class::kNarrowL3
		|| aRoad_C==road_link::road_class::kFerryNoDisp
		|| aRoad_C==road_link::road_class::kFerryS2
		|| aRoad_C==road_link::road_class::kFerryS3
		|| aRoad_C==road_link::road_class::kFerryS4
		|| aRoad_C==road_link::road_class::kVirtual
		||	aRoad_C==road_link::road_class::kBridge
		|| aRoad_C==road_link::road_class::kFacilityEntrance
		|| aRoad_C==road_link::road_class::kParkingEntrance
		|| aRoad_C==road_link::road_class::kParking
		|| aRoad_C==road_link::road_class::kMatching
		|| aRoad_C==road_link::road_class::kMatchingBT )
	{
		//�������郊���N
		return true;
	}
	return false;
}

/**
*	@brief	�ŋߖT�|�C���g�Ƃ̋����擾
*	@param	piTargetGeom	[in]	�v���������W�I���g��
*	@param	piSorcePoint	[in]	��ƂȂ�|�C���g
*	@param	rNearestPoint	[out]	�ŋߖT�|�C���g
*	@retval	����
*/
double CDoc::GetNearestDist(const IGeometryPtr piTargetGeom, const IPointPtr piSorcePoint, IPointPtr& rNearestPoint)
{
	//�\�[�X�|�C���g��XY���擾
	WKSPoint aSorceWKS = {0,0};;
	piSorcePoint->get_X(&aSorceWKS.X);
	piSorcePoint->get_Y(&aSorceWKS.Y);

	//IProximityOperator�ōł��߂��|�C���g���擾
	IProximityOperatorPtr ipProximity(piTargetGeom);
	ipProximity->ReturnNearestPoint(piSorcePoint, esriNoExtension, &rNearestPoint);
	//�ŋߖT�|�C���g��XY���擾
	WKSPoint aNearestWKS = {0,0};
	rNearestPoint->get_X(&aNearestWKS.X);
	rNearestPoint->get_Y(&aNearestWKS.Y);

	//�����v�Z
	CMeshPos	aMeshPos;
	return aMeshPos.GetDist(aSorceWKS.X, aSorceWKS.Y, aNearestWKS.X, aNearestWKS.Y);	
}

/**
*	@brief	�����Ȃ��̒ʍs�֎~�����N���ǂ���
*	@note	�ʍs�֎~��ʃR�[�h(�����Ȃ�, �ʍs�s��, �����Ȃ��ʍs�֎~(�֌W�҈ȊO))
*	@retval	true	�����Ȃ��̒ʍs�֎~
false	�����Ȃ��̒ʍs�֎~�ȊO
*/
bool CDoc::isNopassLink(_IRowPtr ipRow)
{
	using namespace sindy::schema;

	int aNopass_C = GetValue(ipRow, road_link::kNoPassage).intVal;
	if( aNopass_C==road_link::no_passage::kNoPassage
		|| aNopass_C==road_link::no_passage::kInpassable
		|| aNopass_C==road_link::no_passage::kRestricted )
	{
		//�����Ȃ��ʍs�֎~
		return true;
	}
	return false;
}

/**
*	@brief	�����Ȃ��̈���ʍs���ǂ���
*	@note	����ʍs��ʎ�ʃR�[�h(�����Ȃ�����ʍs(������/�t����))
*	@retval	true	�����Ȃ�����ʍs
false	�����Ȃ�����ʍs�ł͂Ȃ�
*/
bool CDoc::isOnewayLink(int cOneway)
{
	using namespace sindy::schema;

	if( cOneway==road_link::oneway::kFore
		|| cOneway==road_link::oneway::kReverse)
	{
		//�����Ȃ�����ʍs
		return true;
	}
	return false;
}

/** 
*	@brief	�ǐՂ̖��ɂ��ǂ蒅���������N��ǐՂ���
*	@note	�ǐՂ̖��ɓ��B���������N������ʍs�̏ꍇ�͈���ʍs�ł͂Ȃ���{���H�ɓ��B����܂ŒǐՂ���
*	@param	ipFeature	[in]	�ǐՃt�B�[�`��
*	@param	ipGeom	[in]	�ǐՃW�I���g��
*	@param	cLink [in] �{�݃|�C���g���ӂ̓��H�����N�R���e�i
*	@retval	�ǐՃR�[�h
*/
int CDoc::IsTraceSuccessLink(IFeaturePtr ipFeature, IGeometryPtr ipGeom, LinkArray& rLink)
{
	using namespace sindy::schema;

	//�ǐՂɐ����������ǂ����B
	//����ʍs���l�����ĒǐՂł��Ȃ��ꍇ���l������
	int isOver=1;
	//����ʍs�̏ꍇ�� ����ʍs�ł͂Ȃ���{���H�{���H����5.5m�ȏナ���N�ɐڑ�����܂ŒǐՂ���
	int aOneway = GetValue(ipFeature, road_link::kOneway).intVal;
	if(isOnewayLink(aOneway)){
		isOver = LinkTrace(ipFeature, ipGeom, rLink);
	}
	return isOver;
}

/**
* �A�N�Z�X���H�`�F�b�N���ʂ̏o��
* @note �A�N�Z�X���H�`�F�b�N���ʂ̏o�͂��s���܂��B
* @param int			(I)	�{�ݏ��ԍ��i1�`�j
* @param CFacilPoint	(I)	�{�݃A�N�Z�X���H���
* @return	bool
*/

bool CDoc::showAccessRoad(int cNo,CFacilPoint *cFacil)
{	
	bool bRET=true;
	IFeaturePtr ipFeature = cFacil->getNearLink().getFeature();

	//�������A�N�Z�X���H�́A���O�o�͂��Ȃ�
	if(( cFacil->getAccessSts()==1 )||( cFacil->getAccessSts()==2 ))
		return true;

	using namespace sindy::schema;

	CString	aMsg1("");
	CString	aMsg2("");
	CString	aDouClassStr("");
	CString	aDouWidthStr("");
	GetValueStringWithDomain(ipFeature, road_link::kRoadClass, &aDouClassStr);
	GetValueStringWithDomain(ipFeature, road_link::kRoadWidth, &aDouWidthStr);
	aMsg1.Format("%d\t%d\t%d",cFacil->getMeshcode(),cFacil->getX(),cFacil->getY());

	//�G���[���O�Ɏ{�ݏ��̃V�[�P���X�ԍ����o�͂����悤��,
	//�ecase�� cFacil->getCode()); ��ǉ� 2007/3/26 masuda
	switch( cFacil->getAccessSts()){
	case 1:	//�i����j�A�N�Z�X���H����y��{���H�ŗU���z
		aMsg2.Format("\tNAVI\t%7.3f\t%ld\t%ld\t�������A�N�Z�X��\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		break;
	case 2:	//�i����j�A�N�Z�X���H����yILQ_Route�ŗU���z
		aMsg2.Format("\tROUTE\t%7.3f\t%ld\t%ld\t�������A�N�Z�X��\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		break;
	case 3:	//�i�G���[�j�{�݂܂œ��B�ł��Ȃ�
		aMsg2.Format("\tFATAL\t%7.3f\t%ld\t%ld\t��ʐڑ����H�֓��B�ł��Ȃ�\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		cout << "\rERROR�F�{�݂܂œ��B�ł��Ȃ�... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	case 5:	//�i���[�j���O�j���n�ƌ������Ă���
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t�{�݂Ɠ��H�̊Ԃɐ��n������\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode()); 
		cout << "\rWARNING�F���n�ƌ������Ă���... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 7:	//�i���[�j���O�j���������N
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t���������N\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING�F���������N... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 8:	//�i���[�j���O�j�v�ڎ��m�F
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t�v�ڎ��m�F\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING�F�v�ڎ��m�F... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	case 9: //(�G���[)�@�ŋߖT�����N���ʍs�֎~
		aMsg2.Format("\tERROR\t%7.3f\t%ld\t%ld\t�ŋߖT�����N�������Ȃ��̒ʍs�֎~\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rERROR�F�ʍs�֎~�����N... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	case 10: // (���[�j���O) �ŋߖT�̐ڑ������N���������A�N�Z�X��
		aMsg2.Format("\tWARNING\t%7.3f\t%ld\t%ld\t�ŋߖT�ɐڑ����Ă��郊���N���������A�N�Z�X��\t%s\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			aDouClassStr,
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rWARNING�F�ŋߖT�̐ڑ������N��������... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		break;
	default: //�i�G���[�j�A�N�Z�X���H�Ȃ�
		aMsg2.Format("\tFATAL\t%7.3f\t%ld\t%ld\t�A�N�Z�X���H�Ȃ�\t\t%s\t%s",
			cFacil->getDistance(),
			cFacil->getNearNode().getOID(),
			cFacil->getNearLink().getOID(),
			cFacil->getKanji(),
			cFacil->getCode());
		cout << "\rERROR�F�A�N�Z�X���H�Ȃ�... \t" << cNo << "\t" << cFacil->getKanji().GetBuffer(256);
		bRET=false;
		break;
	}
	//�G���[���O�t�@�C���֏�������
	if( m_Fp ){	fprintf(m_Fp,"%s%s\n",aMsg1,aMsg2);	}
	ATLTRACE(TEXT("%s%s\n"),aMsg1,aMsg2);

	return bRET;
}
/**
* ��Ԍ����G���W��
* @note �w��t�B�[�`���N���X�Ɛ؂�o���p�W�I���g�����A��Ԍ������s���܂��B
* @param IGeometryPtr		(I)	�؂�o���p�W�I���g��
* @param IFeatureClassPtr	(I)	�����Ώۃt�F�[�`���N���X
* @param IFeatureCursor	(O)	�t�B�[�`���J�[�\���ւ̃|�C���^
* @param esriSpatialRelEnum(I)	��Ԍ������Z�I�y���[�^	
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
long CDoc::SelectByShape(IGeometryPtr       ipGeom, 
						 IFeatureClassPtr   ipFeatureClass, 
						 IFeatureCursor**   ipFeatureCursor,
						 esriSpatialRelEnum spatialRel )
{
	LONG              lFeatureCount = 0;

	/// �N�G���t�B���^�쐬
	if( ipFeatureClass != NULL ){
		CComBSTR			bstrFieldName;
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ISpatialFilterPtr	ipFilter( CLSID_SpatialFilter );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeom );
		ipFilter->put_SpatialRel( spatialRel );

		/// �������ʂ� 0 �Ȃ烊�^�[��
		ipFeatureClass->FeatureCount( ipFilter, &lFeatureCount );
		if( lFeatureCount < 1 ) {
			return 0;
		}
		/// ����
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ipFeatureCursor );
	}
	return lFeatureCount;
}

long CDoc::SelectByWhereClause(IFeatureClassPtr& r_ipFeatureClass, LPCTSTR strWhereClause, std::vector< CAdapt<IFeaturePtr> >& rFeatureVec ) const
{
	rFeatureVec.clear();

	IQueryFilterPtr	ipFilter( CLSID_QueryFilter );
	ipFilter->put_WhereClause(CComBSTR(strWhereClause));

	IFeatureCursorPtr ipFeatureCursor;

	/// ����
	r_ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor );

	// OBJECTID�Ń\�[�g���������߁Amap�����
	// �\�[�g����̂́A�������Ă����ʂ��ς��Ȃ��悤�ɂ��邽��
	// (�����Ɉ��������������ł͏������邽�тɕ��т��ς���Ă��܂�)
	typedef std::pair<long, CAdapt<IFeaturePtr> > IDFPair;
	std::map< long, CAdapt<IFeaturePtr> > aTmpMap;

	IFeaturePtr ipFeature;
	while(S_OK==ipFeatureCursor->NextFeature(&ipFeature)){
		long lObjectID = -1;
		ipFeature->get_OID(&lObjectID);
		aTmpMap.insert( IDFPair(lObjectID, ipFeature) );
	}

	// map�Ɋi�[���ꂽ����vector�ɒǉ����Ă���
	BOOST_FOREACH( const IDFPair& rIDFPair, aTmpMap){
		rFeatureVec.push_back(rIDFPair.second);
	}

	return rFeatureVec.size();
}

bool CDoc::GetFeaturePoint(const IFeaturePtr& r_ipFeature, WKSPoint& rWKSPoint) const
{
	IGeometryPtr ipGeometry;
	if( FAILED( r_ipFeature->get_Shape(&ipGeometry) ) ){
		return false;
	}
	IPointPtr ipPoint(ipGeometry);
	ipPoint->QueryCoords(&(rWKSPoint.X), &(rWKSPoint.Y));

	return true;
}

std::pair<int, int> CDoc::getLL256(const WKSPoint& rWKSPoint) const
{
	std::pair<int, int> aPair;
	static crd_cnv crd;
	crd.LLtoLL256(rWKSPoint.X, rWKSPoint.Y, &(aPair.first), &(aPair.second) );
	return aPair;
}

bool CDoc::OpenFeatureClass(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strFeatureClassName, IFeatureClass** ppFeatureClass)
{
	if( FAILED(r_ipFeatureWorkspace->OpenFeatureClass( CComBSTR(strFeatureClassName), ppFeatureClass) ) ){
		std::cerr << (LPCSTR)CStringA(strFeatureClassName) << "\t�̃I�[�v���Ɏ��s" << std::endl;
		return false;
	}
	return true;
}

bool CDoc::OpenTable(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strTableName, ITable** ppTable)
{
	if( FAILED(r_ipFeatureWorkspace->OpenTable( CComBSTR(strTableName) , ppTable) ) ){
		std::cerr << (LPCSTR)CStringA(strTableName) << "\t�̃I�[�v���Ɏ��s" << std::endl;
		return false;
	}
	return true;
}

/**
* �{�ݏ��|�C���g�t�@�C�����AShapefile�ɕϊ�
* @note �{�ݏ��|�C���g�t�@�C�����AShapeFile�ɕϊ����܂��B
* @param �{�ݏ��o�̓t�@�C����
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
bool CDoc::makeAccessRoadList(void)
{
	CString	 aStr;
	cout << "\r�A�N�Z�X���H���X�g�쐬�J�n." << endl;
	if(( m_ipINF_Route==NULL )||( m_ipLQ_Route==NULL )){
		return false;
	}
	std::set<long>			aVRouteInfID;
	std::set<long>			aVRouteLinkID;
	std::set<CAccessRoad>	SetAccessRoad;
	//�A�N�Z�X�����h�c�o�^
	IQueryFilterPtr		piQueryFilter;
	if( SUCCEEDED(piQueryFilter.CreateInstance(CLSID_QueryFilter))){
		if( SUCCEEDED(piQueryFilter->put_WhereClause(CComBSTR("ROUTECLASS_C=1")))){
			_ICursorPtr	piCursor;
			long lCount=0;
			m_ipINF_Route->RowCount(piQueryFilter, &lCount);
			if( S_OK == m_ipINF_Route->Search(piQueryFilter, VARIANT_FALSE, &piCursor)){
				_IRowPtr piRow;
				while( S_OK == piCursor->NextRow( &piRow )){
					CComVariant vaVal;
					vaVal = GetValue(piRow,_T("OBJECTID"));
					if( vaVal.vt!=VT_NULL ){
						long aInfID = vaVal.intVal;
						aVRouteInfID.insert(aInfID);
						cout << "\r�A�N�Z�X���H���h�c�o�^��... "<<  aVRouteInfID.size();
					}
				}
			}
		}
	}
	//�A�N�Z�X�������N�h�c�o�^
	_ICursorPtr	piCursor;
	m_ipLQ_Route->Search(0, VARIANT_FALSE, &piCursor);
	_IRowPtr piRow;
	while( S_OK == piCursor->NextRow( &piRow )){
		_variant_t vaVal;
		vaVal = GetValue(piRow,"Inf_ID");	
		if(aVRouteInfID.find((long)vaVal) != aVRouteInfID.end()) {
			vaVal = GetValue(piRow, "LINK_ID");
			aVRouteLinkID.insert((long)vaVal);
			cout << "\r�A�N�Z�X���H�����N�h�c�o�^��... " << aVRouteLinkID.size();
		}
	}
	cout << "                                                                           \r";	//�N���A

	using namespace sindy::schema;

	//�A�N�Z�X���H�i�o�H�ΏۊO or ���H����5.5�������j�����N�����t�@�C���o��
	CString	aFileName1;
	CString	aFileName2;
	aFileName1.Format("%s\\%s.txt",getLogDirStr(),"access_road");
	aFileName2.Format("%s\\%s_debug.txt",getLogDirStr(),"access_road");
	if( aVRouteLinkID.size()){

		//		cout << "\rSDE��Ԍ�����.." << endl;
		FILE	*fp1=NULL,*fp2=NULL;	int i=0;		
		if( NULL!=(fp1=fopen(aFileName1,"wb"))){			//�����[�X�p
			if( m_DebugMode )  fp2=fopen(aFileName2,"w");	//�f�B�o�O�p
			IFeatureCursorPtr	ipFeatureCursor;
			aStr.Format("ROUTE_LQRF=1");
			CComBSTR	aComBStr = aStr;
			IQueryFilterPtr	ipQueryFilter(CLSID_QueryFilter);
			ipQueryFilter->put_WhereClause(aComBStr);
			if( S_OK==m_ipLinkFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)&&(ipFeatureCursor != NULL)){
				IFeaturePtr ipFeatures;
				while( S_OK==ipFeatureCursor->NextFeature(&ipFeatures)){
					_variant_t vaValLinkID = GetValue(ipFeatures,"OBJECTID");	
					if(aVRouteLinkID.find((long)vaValLinkID) != aVRouteLinkID.end()) {
						CComVariant vaVal[4];
						vaVal[0] =GetValue(ipFeatures,road_link::kTmpMeshCode);
						vaVal[1] =GetValue(ipFeatures,road_link::kFromNodeID);
						vaVal[2] =GetValue(ipFeatures,road_link::kToNodeID);
						vaVal[3] =GetValue(ipFeatures,_T("OBJECTID"));

						//�������铹�H��ʂ��ǂ������`�F�b�N����
						if( isIgnoreLink(ipFeatures)){
							continue;
						}

						if(( vaVal[1].vt!=VT_NULL )&&( vaVal[2].vt!=VT_NULL )&&( vaVal[3].vt!=VT_NULL )){
							int  aMeshcode = 0;
							long aSNodeID  = vaVal[1].intVal;
							long aENodeID  = vaVal[2].intVal;
							long aLinkID   = vaVal[3].intVal;
							//							if( !aMeshcode )	//Del 2004/11/16
							{
								IGeometryPtr ipGeometry;
								if( SUCCEEDED( ipFeatures->get_ShapeCopy(&ipGeometry))){
									IPointCollectionPtr ipPointCollection(ipGeometry);
									long	aPointCount=0;
									ipPointCollection->get_PointCount(&aPointCount);
									CMapDPoint dPs[3];
									//									if( aPointCount>0 )	//�C�� 2004/11/15
									if( aPointCount>1 )
									{
										int			aX=0,aY=0, aIdx=1;
										IPointPtr	ipPoint[2];
										ipPointCollection->get_Point(0, &ipPoint[0]);
										ipPoint[0]->get_X(&dPs[0].eX);
										ipPoint[0]->get_Y(&dPs[0].eY);
										if( aPointCount>2 ){
											aIdx=2;		//�C�����b�V�����E���N�I�_�̓��H��z��
										}
										ipPointCollection->get_Point(aPointCount-aIdx, &ipPoint[1]);
										ipPoint[1]->get_X(&dPs[1].eX);
										ipPoint[1]->get_Y(&dPs[1].eY);
										dPs[2].eX = (double)(dPs[0].eX+dPs[1].eX)/2.0;
										dPs[2].eY = (double)(dPs[0].eY+dPs[1].eY)/2.0;

										CMeshPos	aMP;
										aMP.LLtoMesh(dPs[2].eX,dPs[2].eY,
											2,				//���b�V���R�[�h���x��
											&aMeshcode,	//���b�V���R�[�h
											&aX, &aY);		//���K�����W
									}
								}
							}
							if( fp1 ){
								CAccessRoad	aRoad(aMeshcode,aSNodeID,aENodeID);
								SetAccessRoad.insert(aRoad);
								//								fprintf( fp1,"%d\t0x%x\t0x%x\n",aMeshcode,aSNodeID,aENodeID);
							}
							if( fp2 ){
								CString	aRoadStr;
								CString	aNaviStr;
								CString	aWidthStr;
								GetValueStringWithDomain(ipFeatures, road_link::kRoadClass, &aRoadStr);
								GetValueStringWithDomain(ipFeatures, road_link::kNaviClass, &aNaviStr);
								GetValueStringWithDomain(ipFeatures, road_link::kRoadWidth, &aWidthStr);
								//								fprintf( fp2,"%d\t%ld\t%ld\t%ld\t%s\t%s\t%s\n",
								//											aMeshcode,aSNodeID,aENodeID,aLinkID,aRoadStr,aNaviStr,aWidthStr);
								fprintf( fp2,"0\tROAD_LINK\t%ld\t\t\t�A�N�Z�X���H\t%d\t%s\t%s\t%s\n",
									aLinkID,aMeshcode,aRoadStr,aNaviStr,aWidthStr);
							}
							cout << "\r�A�N�Z�X���H���X�g�쐬��... " << ++i << "/" << aVRouteLinkID.size();
						}
						else{
							TRACE("ERROR	ROAD_LINK Attr is NULL\n");	}
					}
				}
			}
		}
		//���בւ������H���o��
		i=0;
		for( set<CAccessRoad>::iterator it = SetAccessRoad.begin(); it!=SetAccessRoad.end(); it++){
			CAccessRoad	aRoad = *it;
#ifdef	_DEBUG	
			fprintf( fp1,"%d\t%ld\t%ld\n",aRoad.getMeshcode(),aRoad.getFromId(),aRoad.getToId());
#else
			fprintf( fp1,"%d\t0x%x\t0x%x\n",aRoad.getMeshcode(),aRoad.getFromId(),aRoad.getToId());
#endif
			cout << "\r�A�N�Z�X���H���X�g����... " << ++i << "/" << SetAccessRoad.size();
		}
		if( fp1 )(void)fclose(fp1);
		if( fp2 )(void)fclose(fp2);
	}
	cout << "                                                                           \r";	//�N���A
	cout << "\r�A�N�Z�X���H���X�g�쐬�I��\t" << aVRouteLinkID.size() << endl;
	cout << "FILE:" << aFileName1.GetBuffer(256) << endl;

	aVRouteInfID.clear();
	aVRouteLinkID.clear();
	SetAccessRoad.clear();

	return true;
}
/**
* �r�c�d�ڑ�
* @note �r�c�d�ւ̐ڑ��A�t�B�[�`���|�C���^���擾���܂��B
* @param �Ȃ�
* @return	true�F�ϊ������Afalse�F�ϊ����s
*/
bool CDoc::Init(void)
{
	//�n�}�؂�o���pSDE�ڑ�
	HRESULT hr;
	m_ipWorkspace = sindy::_create_workspace(m_strRoadDB, hr );
	if( FAILED(hr) ){
		std::cerr << "�n�}�؂�o���pDB���[�N�X�y�[�X�擾���s" << std::endl;
		return false;
	}

	using namespace sindy::schema;

	//�Ώۃt�B�[�`���N���X�|�C���^�̎擾
	IFeatureWorkspacePtr ipFeatureWorkspace(m_ipWorkspace);	
	if( !OpenFeatureClass( ipFeatureWorkspace, road_link::kTableName, &m_ipLinkFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, road_node::kTableName, &m_ipNodeFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, base_site::kTableName, &m_ipBaseSiteFeatureClass)
		|| !OpenFeatureClass( ipFeatureWorkspace, base_line::kTableName, &m_ipBaseLineFeatureClass)
		|| !OpenTable( ipFeatureWorkspace, lq_route::kTableName, &m_ipLQ_Route)
		|| !OpenTable( ipFeatureWorkspace, inf_route::kTableName, &m_ipINF_Route) ){
			std::cerr << "�n�}�؂�o���p�t�B�[�`���N���X�擾���s" << std::endl;
			return false;
	}
	return true;
}
/**
* SDE�ڑ��I��
* @note	SDE���[�N�X�y�[�X�C���^�[�t�F�[�X�Q�ƃJ�E���^�̉��
* @param	�Ȃ�
* @return	�Ȃ�
*/
void CDoc::Close(void)
{
	m_ipWorkspace=NULL;
	m_ipLinkFeatureClass=NULL;
	m_ipNodeFeatureClass=NULL;
	m_ipAdmFeatureClass=NULL;
	m_ipBaseSiteFeatureClass=NULL;
	m_ipBaseLineFeatureClass=NULL;
	m_ipLQ_Route = NULL;
	m_ipINF_Route = NULL;
}
/**
* �w��t�F�[�`���̃t�B�[���h�l�h���C�����擾�yUTIL�z
* @note �w��t�F�[�`���̃t�B�[���h�l�h���C�����擾���܂�
* @param ipRow				[in]	IRow�C���^�t�F�[�X�ւ̃|�C���^
* @param lpcszFieldName	[in]	�t�B�[���h��
* @param coString			[out]	�R�[�h�l�h���C����
* @return	�w��t�B�[���h�l��Ԃ�
*/
void CDoc::GetValueStringWithDomain(const IFeaturePtr ipFeatures, LPCTSTR lpcszFieldName, CString* coString)
{
	*coString=_T("");
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipFeatures == NULL )
		return;

	USES_CONVERSION;

	IFieldsPtr	ipFields;
	ipFeatures->get_Fields( &ipFields );
	long		lFieldIndex;
	ipFields->FindField( CComBSTR(lpcszFieldName), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipFeatures->get_Value( lFieldIndex, &vaValue );
		IDomainPtr	ipDomain;
		IFieldPtr	ipField;
		ipFields->get_Field(lFieldIndex, &ipField);
		if( ipField!=NULL && SUCCEEDED(ipField->get_Domain(&ipDomain)) && ipDomain != 0) {
			// �h���C�����擾�ł�����h���C������Ԃ�
			BSTR aDomainStr;
			char tmpStr[256];
			if(variantToDomainName(ICodedValueDomainPtr(ipDomain), vaValue, &aDomainStr)) {
				CString strAlias = OLE2T(aDomainStr);	//Add 2006/9/29 SHIMANO
				strcpy(tmpStr, strAlias);
				*coString = tmpStr;
			}
		}
	}	
	return;
}
/**
* �w��t�F�[�`���̃t�B�[���h�l�h���C�����擾�yUTIL�z
* @note �R�[�h�l�h���C���C���^�t�F�[�X�ɖ₢���킹�A�w�肵��VARIANT�l�ɑΉ�����h���C�������擾���܂�
* @param ipCodedValueDomain	[in]	ICodedValueDomain�C���^�t�F�[�X�ւ̃|�C���^
* @param cVariant				[in]	�t�B�[���h��
* @param coDomainName			[out]	�R�[�h�l�h���C����
* @return	true:�擾����   false:�擾���s
*/
/// �R�[�h�l�h���C���C���^�t�F�[�X�ɖ₢���킹�A�w�肵��VARIANT�l�ɑΉ�����h���C�������擾
bool CDoc::variantToDomainName(ICodedValueDomain* ipCodedValueDomain, const _variant_t& cVariant, BSTR* coDomainName)
{
	if( !ipCodedValueDomain) {
		ATLTRACE(_T("�R�[�h�l�h���C���C���^�t�F�[�X��NULL\n"));
		return false;
	}
	long aCodeCount;
	ipCodedValueDomain->get_CodeCount(&aCodeCount);

	for(int i=0; i<aCodeCount; ++i) {
		_variant_t aVar;
		ipCodedValueDomain->get_Value(i, &aVar);
		if(cVariant == aVar) {
			ipCodedValueDomain->get_Name(i, coDomainName);
			return true;
		}
	}
	return false;
}
/**
* �w��t�F�[�`���̃t�B�[���h�l��Ԃ��yUTIL�z
* @note �w��t�F�[�`���C���^�t�F�[�X�́A�w��t�B�[���h�����A�t�B�[���h�l��Ԃ��܂�
* @param ipRow				[in]	IRow�C���^�t�F�[�X�ւ̃|�C���^ 
* @param lpcszFieldName	[in]	�t�B�[���h��
* @return	�w��t�B�[���h�l��Ԃ�
*/
CComVariant CDoc::GetValue(const _IRowPtr ipRow, const LPCTSTR lpcszFieldName)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipRow == NULL ) return vaValue;

	IFieldsPtr	ipFields;	ipRow->get_Fields( &ipFields );
	LONG		lFieldIndex;
	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	long lFieldCount;
	ipFields->get_FieldCount( &lFieldCount );

	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}	
	return vaValue;
}
/**
* �w��t�F�[�`���̃t�B�[���h�l��Ԃ�
* @note �w��t�F�[�`���C���^�t�F�[�X�́A�w��t�B�[���h�����A�t�B�[���h�l��Ԃ��܂�
* @param ipFeatures		[in]	�t�B�[�`���C���^�t�F�[�X�ւ̃|�C���^
* @param lpcszFieldName	[in]	�t�B�[���h��
* @return	�w��t�B�[���h�l��Ԃ�
*/
CComVariant CDoc::GetValue(const IFeaturePtr ipFeatures, const LPCTSTR lpcszFieldName)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;
	if( ipFeatures == NULL ) return vaValue;

	IFieldsPtr	ipFields;	ipFeatures->get_Fields( &ipFields );
	LONG		lFieldIndex;
	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipFeatures->get_Value( lFieldIndex, &vaValue );
	}	
	return vaValue;
}
/**
* �w�蓹�HOBJECTID���A�A�N�Z�X��������
* @note �w�蓹�H�����R�[�h���A�A�N�Z�X�������肵�܂�
* @param ���H�����R�[�h
* @return	true�F�A�N�Z�X���H�ł���Afalse�F�A�N�Z�X���H�ł͂Ȃ�
*/
bool CDoc::isLinkQueRoute(IFeaturePtr ipFeature)
{
	using namespace sindy::schema;

	if( 1 == GetValue(ipFeature,road_link::kRouteLQRF).intVal){
		return true;
	}
	return false;
}
/**
* �w�蓹�H�����R�[�h���A�A�N�Z�X��������
* @note �w�蓹�H�����R�[�h���A�A�N�Z�X�������肵�܂�
* @param ���H�����R�[�h
* @return	true�F�A�N�Z�X���H�ł���Afalse�F�A�N�Z�X���H�ł͂Ȃ�
*/
bool CDoc::isNaviRoadWidth(const int cWidth, const int cRoadClass)
{
	using namespace sindy::schema;

	bool aSts = false;
	switch( cRoadClass ){
	case road_link::road_class::kHighway:				//���������ԓ��H	
	case road_link::road_class::kCityHighway:			//�s�s�������H
	case road_link::road_class::kCountry:				//��ʍ���	
	case road_link::road_class::kPrefectureMain:		//��v�n����(�s�{��)
	case road_link::road_class::kCityMain:				//��v�n����(�w��s)
	case road_link::road_class::kPrefectureNormal:		//��ʓs���{����
	case road_link::road_class::kCityNormal:			//�w��s�̈�ʎs��
	case road_link::road_class::kTollCountry:			//��ʍ���(�L��)
	case road_link::road_class::kTollPrefectureMain:	//��v�n����(�s�{��)(�L��)
	case road_link::road_class::kTollCityMain:			//��v�n����(�w��s)(�L��)
	case road_link::road_class::kTollPrefectureNormal:	//��ʓs���{����(�L��)
	case road_link::road_class::kTollCityNormal:		//�w��s�̈�ʎs��(�L��)
	case road_link::road_class::kTollOther:				//���̑��̓��H(�L��)
		//		case road_link::road_class::kOther:					//���̑��̓��H
		//		case road_link::road_class::kOtherNoDisp:			//�i�������̑���
		aSts = true;
		break;
	default:
		break;
	}
	//���̑����ȏ�Ȃ�A���H�������肵�Ȃ�
	if( aSts ){
		return( aSts );
	}
	switch( cWidth ){
	case road_link::road_width::kUnsurveyed:		//�������E��ƒ�
	case road_link::road_width::kBetween30And55:	//����3.0m�ȏ�5.5m����
	case road_link::road_width::kUnder30:			//����3.0m����
		break;
	case road_link::road_width::kOver130:			//����13.0m�ȏ�
	case road_link::road_width::kBetween55And130:	//����5.5m�ȏ�13.0m����
		aSts = true;
		break;
	default:
		break;
	}
	return( aSts );
}
/**
* ���H��ʃR�[�h���A��ʓs���{�����ȏ�̓��H������
* @note	���H��ʃR�[�h���A��ʓs���{�����ȏ�̓��H�����肵�܂�
* @param	���H�����N�t�B�[�`���ւ̃|�C���^
* @return	true�F�����ȏ㓹�H�ł���Afalse�F�����ȏ�̓��H�ł͂Ȃ�
*/
bool CDoc::isKendoClassLink(const IFeaturePtr piFeature)
{
	bool	aSts=false;

	using namespace sindy::schema;

	if( piFeature!=NULL ){
		CComVariant vaVal[2];
		vaVal[0] = GetValue(piFeature,_T("OBJECTID"));
		vaVal[1] = GetValue(piFeature,road_link::kRoadClass);
		switch( vaVal[1].intVal ){
		case road_link::road_class::kHighway:				//���������ԓ��H
		case road_link::road_class::kCityHighway:			//�s�s�������H
		case road_link::road_class::kCountry:				//��ʍ���
		case road_link::road_class::kTollCountry:			//��ʍ���(�L��)
		case road_link::road_class::kPrefectureMain:		//��v�n����(�s�{��)
		case road_link::road_class::kTollPrefectureMain:	//��v�n����(�s�{��)(�L��)
		case road_link::road_class::kCityMain:				//��v�n����(�w��s)
		case road_link::road_class::kTollCityMain:			//��v�n����(�w��s)(�L��)
		case road_link::road_class::kPrefectureNormal:		//��ʓs���{����
		case road_link::road_class::kTollPrefectureNormal:	//��ʓs���{����(�L��)
		case road_link::road_class::kCityNormal:			//�w��s�̈�ʎs��
		case road_link::road_class::kTollCityNormal:		//�w��s�̈�ʎs��(�L��)
			aSts = true;
			break;
		default: break;
		}
	}
	return( aSts );
}
/**
* �w�蓹�H��ʃR�[�h���A��{���H������
* @note �w�蓹�H��ʃR�[�h���A��{���H�����肵�܂�
* @param ���H��ʃR�[�h
* @return	true�F��{���H�ł���Afalse�F��{���H�ł͂Ȃ�
*/
bool CDoc::isNaviLink(const int cViewMode,const IFeaturePtr piFeature)
{
	if( piFeature == NULL )
		return false;

	using namespace sindy::schema;

	CComVariant vaVal[4];
	vaVal[0] = GetValue(piFeature,_T("OBJECTID"));
	vaVal[1] = GetValue(piFeature,road_link::kRoadClass);
	vaVal[2] = GetValue(piFeature,road_link::kRoadWidth);
	vaVal[3] = GetValue(piFeature,road_link::kNaviClass);
	int aRoadClass = vaVal[1].intVal;
	int aRoadWidth = vaVal[2].intVal;
	int aNaviClass = vaVal[3].intVal;

	bool	aSts = false;
	switch( cViewMode ){
	case VIEW_ARC_FERRY:
		switch( aRoadClass ){
		case road_link::road_class::kFerryNoDisp:	//�t�F���[�q�H(S4�j���\����)
		case road_link::road_class::kFerryS2:		//�t�F���[�q�H(S3�j���\����)
		case road_link::road_class::kFerryS3:		//�t�F���[�q�H(S2�j���\����)
		case road_link::road_class::kFerryS4:		//�t�F���[�q�H(��\����)
			break;
		default:
			aSts = true;
			break;
		}
		break;
	case VIEW_ARC_DISP :
		switch( aRoadClass ){
		case road_link::road_class::kHighway:				//���������ԓ��H	
		case road_link::road_class::kCityHighway:			//�s�s�������H
		case road_link::road_class::kFreeHighway:			//��������
		case road_link::road_class::kCountry:				//��ʍ���
		case road_link::road_class::kTollCountry:			//��ʍ���(�L��)
		case road_link::road_class::kPrefectureMain:		//��v�n����(�s�{��)
		case road_link::road_class::kTollPrefectureMain:		//��v�n����(�s�{��)(�L��)
		case road_link::road_class::kCityMain:				//��v�n����(�w��s)
		case road_link::road_class::kTollCityMain:			//��v�n����(�w��s)(�L��)
		case road_link::road_class::kPrefectureNormal:		//��ʓs���{����
		case road_link::road_class::kTollPrefectureNormal:	//��ʓs���{����(�L��)
		case road_link::road_class::kCityNormal:			//�w��s�̈�ʎs��
		case road_link::road_class::kTollCityNormal:		//�w��s�̈�ʎs��(�L��)
		case road_link::road_class::kOther:					//���̑��̓��H
		case road_link::road_class::kTollOther:				//���̑��̓��H(�L��)
		case road_link::road_class::kOtherNoDisp:			//�i�������̑���
			aSts = true;
			break;
		case road_link::road_class::kParking:				// ���ԏ�����H
		default: break;
		}
		break;
	case VIEW_ARC_NAVI :
		switch( aNaviClass ){
		case road_link::navi_class::kHighway:				//���������ԓ��H
		case road_link::navi_class::kCityHighway:			//�s�s�������H
		case road_link::navi_class::kCountry:				//��ʍ���
		case road_link::navi_class::kPrefectureMain:		//��v�n����(�s�{��)
		case road_link::navi_class::kCityMain:				//��v�n����(�w��s)
		case road_link::navi_class::kPrefectureNormal:		//��ʓs���{����
		case road_link::navi_class::kCityNormal:			//�w��s�̈�ʎs��
		case road_link::navi_class::kOther:					//���̑����H
		case road_link::navi_class::kLinkCountry:		 	//�ڑ�����
		case road_link::navi_class::kTollCountry:			//��ʍ���(�L��)
		case road_link::navi_class::kTollPrefectureMain:	//��v�n����(�s�{��)(�L��)
		case road_link::navi_class::kTollCityMain:			//��v�n����(�w��s)(�L��)
		case road_link::navi_class::kTollPrefectureNormal:	//��ʓs���{����(�L��)
		case road_link::navi_class::kTollCityNormal:		//�w��s�̈�ʎs��(�L��)
		case road_link::navi_class::kTollOther:				//���̑����H(�L��)
			aSts = true;
			break;
		default:			break;							//�o�H�ΏۊO���H
		}
		break;
	}
	//�o�H�Ώۓ��H�ŁA���H�������A�T�D�T�������̏ꍇ�̓A�N�Z�X���H�Ɣ��肵�Ȃ�
	if( cViewMode != VIEW_ARC_FERRY ){
		if( aSts ){
			//			if( !isNaviRoadWidth(aRoadWidth,aRoadClass))	//Del Ver.1.2 2003/9/29
			if(( aRoadClass==road_link::navi_class::kOther )||( aRoadClass==road_link::road_class::kOtherNoDisp )){
				if( !isNaviRoadWidth(aRoadWidth,aNaviClass)){
					aSts = false;
				}
			}
		}
	}
	return aSts;
}
//------------------------------[ end of file ]------------------------------
