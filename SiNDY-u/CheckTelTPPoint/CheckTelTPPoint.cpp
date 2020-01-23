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

// CheckTelTPPoint.cpp:	CCheckTelTPPoin�@�N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include "CheckTelTPPoint.h"
#include "GlobalFunc.h"
#include <iterator>
#include <crd_cnv.h>
#include <WinLib/VersionInfo.h>
#include "sindy/schema/map.h"
#include <boost/assign.hpp>
using namespace boost::assign;
using namespace std;
using namespace sindy::schema;

const std::set<LONG> CCheckTelTPPoint::m_CBGClass_NG = 
	list_of
	(city_site::bg_class::kSands)                    // ���n
	(city_site::bg_class::kMarsh)                    // ���n
	(city_site::bg_class::kRiver)                    // �͐�
	(city_site::bg_class::kPool)                     // �v�[��
	(city_site::bg_class::kSea)                      // �C(Bug 5707�Ή� 07/11/30�ǉ�)
	(city_site::bg_class::kLake)                     // �r�E��
	(city_site::bg_class::kInterCityTollExpressway)  // �s�s�ԍ������i�L���j
	(city_site::bg_class::kUrbanTollExpressway)      // �s�s�����i�L���j
	(city_site::bg_class::kTollRoad)                 // �L�����H
	(city_site::bg_class::kNationalHighway)          // ����
	(city_site::bg_class::kPrincipalPrefecturalRoad) // ��v�n����
	(city_site::bg_class::kPrefectureNormalRoad)     // ��ʓs���{����
	(city_site::bg_class::kOtherArterialRoad)        // ���̑��������H
	(city_site::bg_class::kSideway)                  // ����
	(city_site::bg_class::kGardenPath)               // �뉀�H
	(city_site::bg_class::kCenterDivider)            // ���������сi�W���j
	(city_site::bg_class::kInterCityFreeExpressway)  // �s�s�ԍ������i�����j
	(city_site::bg_class::kUrbanFreeExpressway)      // �s�s�����i�����j
	(city_site::bg_class::kPublicRoadStairs)         // �����K�i
	(city_site::bg_class::kRunway)                   // �����H
	(city_site::bg_class::kUnderRepairRoad)          // �H�������H
	(city_site::bg_class::kTollGateLane)             // ���������[��
	;
	// 2010/01/26 �_����OK�ƂȂ������߁ANG���X�g����폜
//	45	// �_��

const std::set<LONG> CCheckTelTPPoint::m_BBGClass_NG = 
	list_of
	(base_site::bg_class::kSea)          // �C
	(base_site::bg_class::kRiver)        // �͐�E��E�J
	(base_site::bg_class::kLake)         // �΁E�r�E��
	(base_site::bg_class::kSands)        // ���n
	(base_site::bg_class::kMarsh)        // ���n
	(base_site::bg_class::kSwimmingPool) // �v�[��
	;
	// 2010/01/26 �_����OK�ƂȂ������߁ANG���X�g����폜
//	801001  // �_��


CCheckTelTPPoint::CCheckTelTPPoint( CEnviron& rEnv ) : 
	m_rEnv(rEnv),
	m_ipTPWorkspace(NULL)
{
	m_dMeter = -1; 
}

CCheckTelTPPoint::~CCheckTelTPPoint()
{

}

// �g�p�@
void CCheckTelTPPoint::printUsage()
{
	cerr 
		<< "�yTelTPPoint�`�F�b�N�c�[���z\n"
		<< "(Usage)CheckTelTPPoint.exe [option] [logfile]\n"
		<< "�E�`�F�b�N�͈̓I�v�V����(�w��Ȃ��̂Ƃ��́A�S���`�F�b�N)\n"
		<< "  -m [meshcode]  ���b�V���R�[�h�w��\n"
		<< "  -l [meshlist]  ���b�V�����X�g�w��\n"
		<< "  -d [circle value] POI�̒T�����a(�P�ʂ�10�i�x)\n"		// Bug 5701�Ή� 07/11/30�ǉ�
		<< "�E���ϐ��ɂ���\n"
		<< "  DB_TP          TP�nSDE�ڑ��v���p�e�B\n"
		<< "  DB_ADDRESS     �Z���nSDE�ڑ��v���p�e�B\n"
		<< "  DB_MAP         �s�s�n�}�nSDE�ڑ��v���p�e�B\n"
		<< "  DB_BASE        ���k�nSDE�ڑ��v���p�e�B\n"
		<< "  DB_POI         POI�nSDE�ڑ��v���p�e�B\n"				// Bug 5701�Ή� 07/11/30�ǉ� 
		<< "  FC_TelTP_POINT �d�b�ԍ������|�C���g�t�B�[�`���N���X\n"
		<< "  FC_CITY_ADMIN  �s�s�n�}�s���E�t�B�[�`���N���X\n"
		<< "  FC_CITY_SITE   �s�s�n�}�w�i�t�B�[�`���N���X\n"
		<< "  FC_BASE_SITE   ���k�w�i�t�B�[�`���N���X\n" 
		<< "  FC_POI         POI�t�B�[�`���N���X��\n"				// Bug 5701�Ή� 07/11/30�ǉ�
		<< endl;
}

// ������
bool CCheckTelTPPoint::init( int argc, char* argv[] )
{
	if( !checkArg(argc, argv) )
		return false;

	if( !setInfoFromSDE() )
		return false;

	// ���O�t�@�C��
	if( !m_cError.open(argv[argc-1]) )
	{
		cerr << "#Error ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[argc-1] << endl;
		return false;
	}
	m_cError.setFCName( m_rEnv.m_strTelTPFName );

	// ���O�w�b�_�[�o��
	CVersion cVer;
	CString strVersion;
	strVersion.Format( _T("%s FILEVERSION %s PRODUCTVERSION %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_cError.printHeader( strVersion );

	return true;
}

//���s
void CCheckTelTPPoint::execute()
{
	
	if( m_vecMesh.empty() )	checkMain();
	else					checkMainByMesh();
}

//�������
bool CCheckTelTPPoint::checkArg( int argc, char *argv[] )
{
	for( int i=1;i<argc-2;++i )
	{
		if( strcmp(argv[i], "-m") == 0 ){
			m_vecMesh.push_back( atol(argv[++i]) );
		}
		if( strcmp(argv[i], "-l") == 0 ){
			if( !loadMeshlist(argv[++i]) )
				return false;
		}	
		if( strcmp(argv[i], "-d") == 0 ){		// Bug 5701�Ή� 07/11/30�ǉ�
			m_dMeter = atof(argv[++i]);
		}
	}
	if(m_dMeter < 0){
		cerr << "#Error 臒l�̐ݒ肪�Ԉ���Ă��܂�" << endl;
		return false;
	}
	return true;
}


// ���ϐ��擾���A�K�v���擾
bool CCheckTelTPPoint::setInfoFromSDE()
{
	// �Z���n�ڑ�(TelTP�p)
	if( NULL == (m_ipTPWorkspace = gf::getWorkspace(m_rEnv.m_strTPDBProp)) )
	{
		cerr << "#Error �Z���nDB(TelTP)�ւ̐ڑ��Ɏ��s : " << m_rEnv.m_strTPDBProp << endl;
		return false;
	}
	cout << "�y�Z���nDB(TelTP)�ڑ��z" << m_rEnv.m_strTPDBProp << endl;

	// �Z���n�ڑ�(�s���E�p)
	const IWorkspacePtr ipAddrWorkspace = gf::getWorkspace( m_rEnv.m_strAddrDBProp );
	if( NULL == ipAddrWorkspace )
	{
		cerr << "#Error �Z���nDB(�s���E)�ւ̐ڑ��Ɏ��s : " << m_rEnv.m_strAddrDBProp << endl;
		return false;
	}
	cout << "�y�Z���nDB(�s���E)�ڑ��z" << m_rEnv.m_strAddrDBProp << endl;

	// �n�}�n�ڑ�(�s�s�n�})
	const IWorkspacePtr ipCityWorkspace = gf::getWorkspace( m_rEnv.m_strCityDBProp );
	if( NULL == ipCityWorkspace )
	{
		cerr << "#Error �n�}�nDB(�s�s�n�})�ւ̐ڑ��Ɏ��s : " << m_rEnv.m_strCityDBProp << endl;
		return false;
	}
	cout << "�y�n�}�nDB(�s�s�n�})�ڑ��z" << m_rEnv.m_strCityDBProp << endl;

	// �n�}�n�ڑ�(���k)
	const IWorkspacePtr ipBaseWorkspace = gf::getWorkspace( m_rEnv.m_strBaseDBProp );
	if( NULL == ipBaseWorkspace )
	{
		cerr << "#Error �n�}�nDB(���k)�ւ̐ڑ��Ɏ��s : " << m_rEnv.m_strBaseDBProp << endl;
		return false;
	}
	cout << "�y�n�}�nDB(���k)�ڑ��z" << m_rEnv.m_strBaseDBProp << endl;

	// POI�ڑ�(Bug 5701�Ή� 07/11/30�ǉ�)
	if( NULL == (m_ipPOIWorkspace = gf::getWorkspace(m_rEnv.m_strPOIDBProp)) )
	{
		cerr << "#Error POI�ւ̐ڑ��Ɏ��s : " << m_rEnv.m_strPOIDBProp << endl;
		return false;
	}
	cout << "�yPOI�ڑ��z" << m_rEnv.m_strPOIDBProp << endl;

	// �t�B�[�`���N���X�擾
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strTelTPFName), &m_ipTelTPFC );
	if( NULL == m_ipTelTPFC )
	{
		cerr << "#Error �d�b�ԍ������|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << m_rEnv.m_strTelTPFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strCityAdminFName), &m_ipCityAdminFC );
	if( NULL == m_ipCityAdminFC )
	{
		cerr << "#Error �s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : " << m_rEnv.m_strCityAdminFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipCityWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strCitySiteFName), &m_ipCitySiteFC );
	if( NULL == m_ipCitySiteFC )
	{
		cerr << "#Error �s�s�n�}�w�i�t�B�[�`���N���X�̎擾�Ɏ��s : " << m_rEnv.m_strCitySiteFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipBaseWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strBaseSiteFName), &m_ipBaseSiteFC );
	if( NULL == m_ipBaseSiteFC )
	{
		cerr << "#Error ���k�w�i�t�B�[�`���N���X�̎擾�Ɏ��s : " << m_rEnv.m_strBaseSiteFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(_T("REFERENCE.CityMesh")), &m_ipCityMeshFC );
	if( NULL == m_ipCityMeshFC )
	{
		cerr << "#Error �s�s�n�}���b�V���t�B�[�`���N���X�̎擾�Ɏ��s : REFERENCE.CityMesh" << endl;
		return false;
	}
	// POI�t�B�[�`���N���X�擾(Bug 5701�Ή� 07/11/30�ǉ�)
	IFeatureWorkspacePtr(m_ipPOIWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strPOIFName), &m_ipPOIFC );
	if( NULL == m_ipPOIFC )
	{
		cerr << "#Error POI�t�B�[�`���N���X�̎擾�Ɏ��s : " << m_rEnv.m_strPOIFName << endl;
		return false;
	}
	
	// �t�B�[���h�C���f�b�N�X
	if( FAILED(m_ipCitySiteFC->FindField(CComBSTR(sindy::schema::city_site::kBgClass), &m_lCBGClassC_FID)) 
		|| FAILED(m_ipBaseSiteFC->FindField(CComBSTR(sindy::schema::base_site::kSC4BgClass), &m_lBSC4BGClassC_FID)) )
	{
		cerr << "#Error �w�i��ʃt�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	return true;
}


// ���b�V�����X�g�ǂݍ���
bool CCheckTelTPPoint::loadMeshlist( LPCTSTR lpszFile )
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

// ���b�V���t�B�[�`���擾
IFeaturePtr CCheckTelTPPoint::getMeshFeature( long lMeshCode )
{
	CString strWhere;
	strWhere.Format( _T("%s=%d"), sindy::schema::citymesh::kMeshCode, lMeshCode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor == NULL )
		return NULL;

	IFeaturePtr ipMeshFeature;
	return( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ? ipMeshFeature : NULL );
}

// �`�F�b�N���C��
void CCheckTelTPPoint::checkMain()
{
	LONG lAllCount = 0;
	m_ipTelTPFC->FeatureCount( NULL, &lAllCount );
	IFeatureCursorPtr ipFeatureCursor;
	m_ipTelTPFC->Search( NULL, VARIANT_FALSE, &ipFeatureCursor );
	if( NULL == ipFeatureCursor )
	{
		std::cerr << "TelTP�|�C���g�͂���܂���" << std::endl;
		return;
	}

	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
	{
		LONG OID;						/// 12/7 ���������ב΍�̂��ߏC��
		ipFeature->get_OID( &OID );		/// ���i�x�N�^�[��CTelTP�I�u�W�F�N�g���i�[���Ă������̂��A�I�u�W�F�N�gID�̂݊i�[����悤�ɕύX�j

		m_vecTelTP.push_back( OID );	/// �S�����́AORA-1555�΍�̂��߁A�������Ɋi�[
	}
	LONG lCount = 1;
	for( std::vector<LONG>::const_iterator itr=m_vecTelTP.begin();itr!=m_vecTelTP.end();++itr )
	{
		// �I�u�W�F�N�gID����CTelTP���쐬
		IFeaturePtr ipFeature;
		m_ipTelTPFC->GetFeature( *itr, &ipFeature ); 
		CTelTP cTelTP( ipFeature );

		checkTelTPAndAdmin( cTelTP );

		checkTelTPAndSite( cTelTP );

		checkTelTPPoints( cTelTP );

		cerr << lCount++ << " / " << lAllCount << " ������\r";
	}
}

// ���b�V�����ƂɃ`�F�b�N
void CCheckTelTPPoint::checkMainByMesh()
{
	// ���b�V���t�B�[�`���N���X�̎擾(�擪�ɂ���f�[�^���璷������)
	CString strMeshFName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::middlemesh::kTableName );
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::basemesh::kTableName );
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::citymesh::kTableName );
	else
	{
		cerr << "#Error ���b�V���R�[�h���A�s���ł�" << endl;
		return;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(strMeshFName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
	{
		cerr << "#Error ���b�V���t�B�[�`���N���X�̎擾�Ɏ��s" << endl;
		return;
	}

	//1���b�V�����Ƃ̃��[�v
	for( vector<long>::iterator itr=m_vecMesh.begin();itr!=m_vecMesh.end();++itr )
	{
		// ���b�V���t�B�[�`���̎擾
		IFeaturePtr ipMeshFeature = getMeshFeature(*itr);
		if( NULL == ipMeshFeature )
		{
			cout << *itr << " : ���b�V���t�B�[�`���̎擾�Ɏ��s" << endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_Shape( &ipMeshGeo );
		
		// Arc10.1�Ή��ŃL���b�V�����g�p���Ȃ��悤�C��		
		IFeatureCursorPtr ipFeatureCursor;
		LONG lAllCount = gf::SelectByShapeWithCount( ipMeshGeo, m_ipTelTPFC, &ipFeatureCursor, esriSpatialRelIntersects );
		if( lAllCount < 1 )
		{
			cout << *itr << " : ���b�V������TelTP�|�C���g�͂���܂���" << endl;
			continue;
		}
		LONG lCount = 1;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			CTelTP ob(ipFeature);

			checkTelTPAndAdmin( ob );
				
			checkTelTPAndSite( ob );

			checkTelTPPoints( ob );

			cerr << lCount++ << " / " << lAllCount << " ������\r";
		}
		cout << *itr << " : �`�F�b�N����" << endl;
	}
}

// �s�s�n�}�s���E�Ƃ̐����`�F�b�N
void CCheckTelTPPoint::checkTelTPAndAdmin( const CTelTP& cTelTP )
{
	// TelTP�|�C���g���ʒu����s�s�n�}�s���E�擾
	LONG lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature ){
		m_cError.printLog( cTelTP, E_DUP_OR_NOTON_ADMIN );	// E8
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	CComVariant vaValue;

	switch( cTelTP.m_ePointStatC )
	{
	case GAITOU_SISETU:		// �Y���{�݂���

		if( cTelTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
			m_cError.printLog( cTelTP, W_GAITOUSISETU_CITYCODE );	//W4
		}

		if(cTelTP.getAddrCode(11).compare(5, 6, "000000") != 0 ){
			if( cTelTP.getAddrCode(8) != cAdmin.getAddrCode(8) ){
				m_cError.printLog( cTelTP, W_GAITOUSISETU_OAZACODE );	//W3
			}
		
			switch( cAdmin.m_eAddrClassC )
			{
			case OAZA_NOTSEIBI:			// �����ڊE������
			case JUKYO_TIBAN_NOTSEIBI:	// �Z���\���E�n�Ԗ�����
				if( cTelTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
					m_cError.printLog( cTelTP, W_GAITOUSISETU_ADDRCODE_NOTSEIBI );	// W1
				}
				break;
			default:
				if( cTelTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
					m_cError.printLog( cTelTP, W_GAITOUSISETU_ADDRCODE_SEIBI );	// W2
				}
				break;
			}
		}
		break;
	case HORYU:				// �ۗ�
		m_cError.printLog( cTelTP, N_HORYU_POINTSTAT );	// N5
		break;

	case UNKNOWN_POS:		// �ʒu�s��
		break;

	case NOSURVEY:			// ������
		
		// �ŏI�X�V�҂��usindy�v���`�F�b�N
		vaValue = cTelTP.getValue( sindy::schema::teltp_point::kOperator );
		if( vaValue.vt != VT_BSTR || strcmp(COLE2T(vaValue.bstrVal), "sindy") != 0 ){
			m_cError.printLog( cTelTP, E_NOSURVEY_OPERATOR );	// E6
		}
		break;

	default:
		m_cError.printLog( cTelTP, E_UNKNOWN_POINTSTAT );	// E7
		break;
	}
}


// �w�i�Ƃ̐����`�F�b�N
void CCheckTelTPPoint::checkTelTPAndSite( const CTelTP& cTelTP )
{
	// �������ʃR�[�h���u�Y���{�݂���v�̏ꍇ�̂�
	if( cTelTP.m_ePointStatC == GAITOU_SISETU )
	{
		// �w�i����ɍi��Ȃ��Ƃ��́A�`�F�b�N���Ȃ�
		LONG lCount = 0;
		
		// TelTP�|�C���g���ʒu����s�s�n�}�w�i�擾
		IFeaturePtr ipCitySiteFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'C' );
		if(lCount > 1){
			if( NULL == ipCitySiteFeature ){
				m_cError.printLog( cTelTP, E_DUP_OR_NOTON_CITYSITE);	// E12
				return;
			}
		}
		else if(lCount == 1 ){
			CSite cCitySite( ipCitySiteFeature, 'C' );
			if( isNGCitySite(cCitySite.m_lBGClassC) )
				m_cError.printLog( cTelTP, E_ON_NGCITYSITE , &(cCitySite.getBGClassDomain()) );	// E10
		}
		else{
			//�s�s���b�V���|���S�����Ȃ��ꍇ�͔w�i�Ȃ��G���[���o�͂��Ȃ��B
			CComVariant vaValue;
			IFeatureCursorPtr ipFeatureCursor;
			IFeaturePtr ipMeshFeature;

			//�s�s���b�V���|���S���������ꍇ�ACREATE_YEAR��NULL�������Ă���B����ꍇ�͍쐬�N�B
			LONG lMCount = gf::SelectByShapeWithCount( cTelTP.getGeo(), m_ipCityMeshFC, &ipFeatureCursor, esriSpatialRelIntersects );
			LONG m_nCreate_year = 0;
			m_ipCityMeshFC->FindField(CComBSTR(sindy::schema::citymesh::kCreateYear), &m_nCreate_year); 
			while( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ){
				ipMeshFeature->get_Value( m_nCreate_year, &vaValue );
				if( vaValue.vt != VT_NULL ){
					string string_NULL = "�w�i�Ȃ�[�׊X�H]";
					m_cError.printLog( cTelTP, E_ON_NGCITYSITE, &string_NULL);	// E10
					break;
				}
			}
		}

		lCount = 0;

		// TelTP�|�C���g���ʒu���钆�k�w�i�擾
		IFeaturePtr ipBaseSiteFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'B' );
		if( NULL == ipBaseSiteFeature && lCount > 1 ){
			m_cError.printLog( cTelTP, E_DUP_OR_NOTON_BASESITE);	// E13
			return;
		}

		if( lCount == 1 ){
			CSite cBaseSite( ipBaseSiteFeature, 'B' );
			if( isNGBaseSite(cBaseSite.m_lBGClassC) )
				m_cError.printLog( cTelTP, E_ON_NGBASESITE, &(cBaseSite.getBGClassDomain()) );	// E11
		}
	}
}


//����ܓx�o�x�̃|�C���g�̒������ʃR�[�h�`�F�b�N
void CCheckTelTPPoint::checkTelTPPoints( const CTelTP& cTelTP )
{
	IFeatureCursorPtr ipFeaturePointCursor;

	// �����W�I���g���ɑ���TelTP�����邩�ǂ�������
	LONG lPointCount = gf::SelectByShapeWithCount( cTelTP.getGeo(), m_ipTelTPFC, &ipFeaturePointCursor, esriSpatialRelContains );
	if(lPointCount > 1)
	{
 		IFeaturePtr ipFeaturePoint;
		while( ipFeaturePointCursor->NextFeature( &ipFeaturePoint ) == S_OK)
		{
			// �������ʃR�[�h�`�F�b�N
			CTelTP ob_Point( ipFeaturePoint );
			if(ob_Point.m_ePointStatC != cTelTP.m_ePointStatC )
			{
				m_cError.printLog( cTelTP, E_PLURAL_OPERATOR );	// E14
				break;
			}
		}
	}
	// �������ʃR�[�h���u�Y���{�݂���v�̏ꍇ�̂�
	if(cTelTP.m_ePointStatC == GAITOU_SISETU){
		
		// �d�b�ԍ��̃t���[�_�C�����`�F�b�N
		if( cTelTP.m_strTelNum.find("0120") == 0 )
			m_cError.printLog( cTelTP, W_TELNUM_FREECALL );		// W9

		// IPCODE�`�F�b�N(BUG 5701�Ή� 07/11/30�ǉ�)
		CheckIPCODE( cTelTP );

		// ����V�[�P���X�d���`�F�b�N�iBug9295 �Ή��j
		CheckSameSequence( cTelTP );
	}
}


// �|�C���g������Ă���|���S�����擾
IFeaturePtr CCheckTelTPPoint::getFeatureWithinPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch )
{
	IFeatureCursorPtr ipFeatureCursor;
	switch( szSwitch )
	{
	case 'A':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityAdminFC, &ipFeatureCursor, esriSpatialRelWithin );
		break;
	case 'C':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCitySiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'B':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipBaseSiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'M':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipMeshFC, &ipFeatureCursor, esriSpatialRelWithin );
		break;
	default:
		return NULL;
	}

	// �w�i�|���S����ɍi��Ȃ��Ƃ��́ANULL��Ԃ�
	if( *lpCount == 1 ){
		IFeaturePtr ipFeature;
		return ( SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature))? ipFeature : NULL );
	}
	else
		return NULL;
}

// TelTP�̎��͂�POI��IP�R�[�h&�d�b�ԍ��`�F�b�N(Bug 5701�Ή� 07/11/30�ǉ�)�@
void CCheckTelTPPoint::CheckIPCODE( const CTelTP& cTelTP )
{
	IGeometryPtr ipGeoBuffer;			// TelTP�̎��͂̋�Ԃ̃W�I���g���p
	IFeatureCursorPtr ipPOICursor;		
	IEnvelopePtr ipEnvelop;
	cTelTP.getGeo()->get_Envelope( &ipEnvelop );

	// �����Ŏw�肳�ꂽ�l�̕�����TelTP�̃W�I���g�����L���� �� TelTP�̎��́�m�̋�Ԃ̃W�I���g���쐬
	ipEnvelop->Expand(m_dMeter, m_dMeter, VARIANT_FALSE );
	{
		// TelTP�̎��͂̃W�I���g���ɑ΂���POI�̋�ԒT���������Ȃ������ʎ擾
		LONG POICount = gf::SelectByShapeWithCount(ipEnvelop, m_ipPOIFC, &ipPOICursor, esriSpatialRelIntersects);
		if( POICount > 0 )
		{
			IFeaturePtr ipPOIFeature;		
			IGeometryPtr ipPOIGeo;			// TelTP�̎��͂ɂ���POI�P�̂̃W�I���g��
			while( ipPOICursor->NextFeature( &ipPOIFeature ) == S_OK )
			{
				// POI�̃t�B�[���h�擾�����i���gCFeature::getValue����assert�𔲂������́j
				ipPOIFeature->get_Shape( &ipPOIGeo );
				IFieldsPtr ipPOIFields;
				ipPOIFeature->get_Fields( &ipPOIFields );
				LONG lFID = 0;
				CComVariant vaValue;				
			
				// POI��IPCODE�t�B�[���h�擾
				ipPOIFields->FindField( CComBSTR(_T("IPCODE")), &lFID );
				ipPOIFeature->get_Value( lFID, &vaValue );
				string strIPCODE;
				if( vaValue.vt == VT_BSTR )
					strIPCODE = COLE2T( vaValue.bstrVal );
				vaValue.Clear();
				
				// �����Ёi�O�����A�Ղ�E�C�x���g�A�ԉ΁j �ˁ@�h�o�R�[�h=�u02�v
				// �h�l�i���o�C�����ԏ�@�ˁ@�h�o�R�[�h=�u52�v
				// ��i�@�ˁ@�h�o�R�[�h=�u67�v
				if( strIPCODE == "02" || strIPCODE == "52" || strIPCODE == "67" )
				{
					// POI��TEL�t�B�[���h�擾
					lFID = 0;
					ipPOIFields->FindField( CComBSTR(_T("TEL")), &lFID );
					ipPOIFeature->get_Value( lFID, &vaValue );
					string strTELNUM;
					if( vaValue.vt == VT_BSTR )
						strTELNUM = COLE2T( vaValue.bstrVal );
					vaValue.Clear();

					// ��LIP�R�[�h������POI��TelTP�̎��͂ɂ���A����̓d�b�ԍ������ꍇ�G���[�o��
					if(strTELNUM == cTelTP.m_strTelNum){
						
						// �������擾���邽�߂̊O���N���X
						crd_cnv	a_cCrd;
						
						// TelTP��(x,y)���W���擾
						double dX_TelTP, dY_TelTP;
						IPointPtr ipTelTPPoint( cTelTP.getGeo() );
						ipTelTPPoint->get_X( &dX_TelTP );
						ipTelTPPoint->get_Y( &dY_TelTP );

						// POI��(x,y)���W���擾
						double dX_POI, dY_POI;
						IPointPtr ipPOIPoint( ipPOIGeo );
						ipPOIPoint->get_X( &dX_POI );
						ipPOIPoint->get_Y( &dY_POI );

						m_cError.printLog( cTelTP, E_POI_TABOO_IPCODE, a_cCrd.GetDist(dX_POI, dY_POI, dX_TelTP, dY_TelTP));	// E15
					}
				}
			}
		}
	}
}

// ����V�[�P���X�����|�C���g���������݂��Ȃ����`�F�b�N�iBug9295 �ǉ��j
void CCheckTelTPPoint::CheckSameSequence( const CTelTP& cTelTP )
{
	// ����V�[�P���X�������́A���g�ł͂Ȃ����̂̌���
	CString strWhere;
	strWhere.Format( _T("%s=%d and OBJECTID<>%d"), sindy::schema::teltp_point::kSequence, cTelTP.m_lSequence, cTelTP.getOID() );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipTelTPFC->Search( ipQuery, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
		return;

	IFeaturePtr ipFeature;
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
	{
		// ����V�[�P���X������TelTP������
		CTelTP ob(ipFeature);

		// ��������TelTP�̒������ʃR�[�h���u�Y���{�݂���v�̏ꍇ�̓G���[
		if( GAITOU_SISETU == ob.m_ePointStatC )
		{
			m_cError.printLog( cTelTP, E_SAME_SEQUENCE );	// E16
			break;
		}
	}
}























