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

// CheckExTPPoint.cpp: CCheckExTPPoint �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckExTPPoint.h"
#include "GlobalFunc.h"
#include <iterator>
#include "sindy/schema/map.h"
#include <boost/assign.hpp>
using namespace boost::assign;

using namespace std;
using namespace sindy::schema;
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

const std::set<long> CCheckExTPPoint::m_CBGClass_NG = 
	list_of
	(city_site::bg_class::kSands)                    // ���n
	(city_site::bg_class::kMarsh)                    // ���n
	(city_site::bg_class::kRiver)                    // �͐�
	(city_site::bg_class::kPool)                     // �v�[��
	(city_site::bg_class::kSea)                      // �C
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
	(city_site::bg_class::kDamGuideNotExist)         // �_���i�֗��L�ڂȂ��j
	(city_site::bg_class::kDamGuideExist)            // �_���i�֗��L�ڂ���j
	(city_site::bg_class::kRunway)                   // �����H
	(city_site::bg_class::kUnderRepairRoad)          // �H�������H
	(city_site::bg_class::kTollGateLane)             // ���������[��
	;
const set<long> CCheckExTPPoint::m_BBGClass_NG = 
	list_of
	(base_site::bg_class::kSea)          // �C
	(base_site::bg_class::kRiver)        // �͐�E��E�J
	(base_site::bg_class::kLake)         // �΁E�r�E��
	(base_site::bg_class::kSands)        // ���n
	(base_site::bg_class::kMarsh)        // ���n
	(base_site::bg_class::kSwimmingPool) // �v�[��
	(base_site::bg_class::kDam);         // �_��

CCheckExTPPoint::CCheckExTPPoint() : 
	m_emCheckMode(kNormal),
	m_lCBGClassC_FID(-1),
	m_lBSC4BGClassC_FID(-1),
	m_lCreateYear_FID(-1)
{
}

CCheckExTPPoint::~CCheckExTPPoint()
{

}

// �g�p�@
void CCheckExTPPoint::printUsage()
{
	cerr 
		<< "�y�g���d�b�ԍ������|�C���g�`�F�b�N�c�[���z\n"
		<< "(Usage)CheckEXTPPoint.exe [option] [logfile]\n"
		<< "�E�`�F�b�N���[�h�I�v�V����(�w��Ȃ��̂Ƃ��́A�ʏ�`�F�b�N)\n"
		<< "  -e normal/alps/acc4/ips/hlp    normal : �ʏ�, alps : �A���v�X�̂�, acc4 : iPC(���x4)�̂�, ips : iPS�`�F�b�N���[�h, hlp : HLP\n"
		<< "�E�`�F�b�N�͈̓I�v�V����(�w��Ȃ��̂Ƃ��́A�S���`�F�b�N)\n"
		<< "  -m [meshcode]  ���b�V���R�[�h�w��\n"
		<< "  -l [meshlist]  ���b�V�����X�g�w��\n"
		<< "�E���ϐ��ɂ���\n"
		<< "  DB_TP         TP�nDB�ڑ��v���p�e�B\n"
		<< "  DB_ADDRESS    �Z���nDB�ڑ��v���p�e�B\n"
		<< "  DB_MAP        �s�s�n�}�nDB�ڑ��v���p�e�B\n"
		<< "  DB_BASE       ���k�nDB�ڑ��v���p�e�B\n"
		<< "  DB_MESH       ���b�V���nDB�ڑ��v���p�e�B(CityMesh�Ŏg�p)\n"
		<< "  FC_EXTP_POINT �g���d�b�ԍ������|�C���g�t�B�[�`���N���X\n"
		<< "  FC_CITY_ADMIN �s�s�n�}�s���E�t�B�[�`���N���X\n"
		<< "  FC_CITY_SITE  �s�s�n�}�w�i�t�B�[�`���N���X\n"
		<< "  FC_BASE_SITE  ���k�w�i�t�B�[�`���N���X\n"
		<< "  FC_CITYMESH   �s�s�n�}���b�V���t�B�[�`���N���X\n" << endl;
}

// ������
bool CCheckExTPPoint::init( int argc, char* argv[] )
{
	for( int i = 1; i < argc-2; ++i )
	{
		if( strcmp(argv[i], "-e") == 0 )
		{
			++i;
			if( strcmp(argv[i], "normal") == 0 )
				m_emCheckMode = kNormal;
			else if( strcmp(argv[i], "alps") == 0 )
				m_emCheckMode = kAlps;
			else if( strcmp(argv[i], "acc4") == 0 )
				m_emCheckMode = kAcc4;
			else if( strcmp(argv[i], "ips") == 0 )
				m_emCheckMode = kiPS;
			else if( strcmp(argv[i], "hlp") == 0 )
				m_emCheckMode = kHlp;
			else
				EXE_ERRORLOG( "#Error -e �I�v�V�����̂Ƃ���normal/alps/acc4/ips/hlp�̂����ꂩ�����w��ł��܂���", argv[i], false );
		}
		else if( strcmp(argv[i], "-m") == 0 )
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( strcmp(argv[i], "-l") == 0 )
		{
			if( !loadMeshlist(argv[++i]) )
				EXE_ERRORLOG( "#Error ���b�V�����X�g�̓ǂݍ��݂Ɏ��s", argv[i], false );
		}
	}

	// ���O�t�@�C��
	if( !m_cError.open(argv[argc-1]) )
		EXE_ERRORLOG( "#Error ���O�t�@�C���̃I�[�v���Ɏ��s", argv[argc-1], false );

	if( !setInfoFromDB() )
		return false;

	m_cError.setFCName( m_strExTPFCName );
	m_cError.printHeader();

	return true;
}

// ���s
void CCheckExTPPoint::execute()
{
	// Where��ݒ�
	switch( m_emCheckMode )
	{
	case kNormal:
		m_strWhereClause = _T("INFOSRC_C=1");
		cerr << "�ʏ�`�F�b�N���[�h�Ŏ��s���܂�\n" << endl;
		break;
	case kAlps:
		m_strWhereClause = _T("INFOSRC_C=2");
		cerr << "�A���v�X�`�F�b�N���[�h�Ŏ��s���܂�\n" << endl;
		break;
	case kAcc4:
		m_strWhereClause = _T("INFOSRC_C=3");
		cerr << "iPC(���x4)�`�F�b�N���[�h�Ŏ��s���܂�\n" << endl;
		break;
	case kHlp:
		m_strWhereClause = _T("INFOSRC_C=4");
		cerr << "HLP�`�F�b�N���[�h�Ŏ��s���܂�\n" << endl;
		break;
	case kiPS:
		m_strWhereClause = _T("INFOSRC_C IS NOT NULL");
		cerr << "iPS�`�F�b�N���[�h�Ŏ��s���܂�\n" << endl;
		break;
	default:
		return;
	}

	// iPS�`�F�b�N���[�h���́A��ɑS��
	if( m_vecMesh.empty() || kiPS == m_emCheckMode )
		checkMain();
	else
		checkMainByMesh();
}

/**
 * �����֐�
 */

// ���ϐ��擾���A�K�v���擾
bool CCheckExTPPoint::setInfoFromDB()
{
	// ��Ɋ��ϐ��擾
#ifdef _DEBUG
	_tputenv( _T("DB_TP=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_ADDRESS=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_MAP=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_BASE=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_MESH=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("FC_EXTP_POINT=ExTP_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_CITY_SITE=City_Site") );
	_tputenv( _T("FC_BASE_SITE=Base_Site") );
	_tputenv( _T("FC_CITYMESH=CityMesh") );
#endif

	CString strEnv;

	// �Z���n�ڑ�(ExTP�p)
	if( !strEnv.GetEnvironmentVariable(_T("DB_TP")) )
		EXE_ERRORLOG( "#Error ���ϐ� DB_TP �̎擾�Ɏ��s", NULL, false );
	if( NULL == (m_ipTPWorkspace = gf::getWorkspace(strEnv)) )
		EXE_ERRORLOG( "#Error �Z���nDB(ExTP)�ւ̐ڑ��Ɏ��s", strEnv, false );
	cout << "�y�Z���nDB(ExTP)�ڑ��z" << strEnv << endl;

	if( !m_strExTPFCName.GetEnvironmentVariable(_T("FC_EXTP_POINT")) )
			EXE_ERRORLOG( "#Error ���ϐ� FC_EXTP_POINT �̎擾�Ɏ��s", NULL, false );
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(m_strExTPFCName), &m_ipExTPFC );
	if( NULL == m_ipExTPFC )
		EXE_ERRORLOG( "#Error �g���d�b�ԍ������|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s", m_strExTPFCName, false );

	// �Z���n�ڑ�(�s���E�p)
	if( !strEnv.GetEnvironmentVariable(_T("DB_ADDRESS")) )
		EXE_ERRORLOG( "#Error ���ϐ� DB_ADDRESS �̎擾�Ɏ��s", NULL, false );

	const IWorkspacePtr ipAddrWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipAddrWorkspace )
		EXE_ERRORLOG( "#Error �Z���nDB(�s���E)�ւ̐ڑ��Ɏ��s", strEnv, false );
	cout << "�y�Z���nDB(�s���E)�ڑ��z" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
			EXE_ERRORLOG( "#Error ���ϐ� FC_CITY_ADMIN �̎擾�Ɏ��s", NULL, false );
	IFeatureWorkspacePtr(ipAddrWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCityAdminFC );
	if( NULL == m_ipCityAdminFC )
		EXE_ERRORLOG( "#Error �s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s", strEnv, false );

	// �n�}�n�ڑ�(�s�s�n�})
	if( !strEnv.GetEnvironmentVariable(_T("DB_MAP")) )
		EXE_ERRORLOG( "#Error ���ϐ� DB_MAP �̎擾�Ɏ��s", NULL, false );
	const IWorkspacePtr ipCityWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipCityWorkspace )
		EXE_ERRORLOG( "#Error �n�}�nDB(�s�s�n�})�ւ̐ڑ��Ɏ��s", strEnv, false );
	cout << "�y�n�}�nDB(�s�s�n�})�ڑ��z" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_SITE")) )
			EXE_ERRORLOG( "#Error ���ϐ� FC_CITY_SITE �̎擾�Ɏ��s", NULL, false );
	IFeatureWorkspacePtr(ipCityWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCitySiteFC );
	if( NULL == m_ipCitySiteFC )
		EXE_ERRORLOG( "#Error �s�s�n�}�w�i�t�B�[�`���N���X�̎擾�Ɏ��s", strEnv, false );

	if( FAILED(m_ipCitySiteFC->FindField(CComBSTR(_T("BGCLASS_C")), &m_lCBGClassC_FID)) )
			EXE_ERRORLOG( "#Error �s�s�n�}�w�i��ʃt�B�[���h�C���f�b�N�X�̎擾�Ɏ��s", NULL, false );

	// ���b�V���n�ڑ�
	if( !strEnv.GetEnvironmentVariable(_T("DB_MESH")) )
		EXE_ERRORLOG( "#Error ���ϐ� DB_MESH �̎擾�Ɏ��s", NULL, false );
	const IWorkspacePtr ipMeshWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipMeshWorkspace )
		EXE_ERRORLOG( "#Error ���b�V���nDB(�s�s�n�})�ւ̐ڑ��Ɏ��s", strEnv, false );
	cout << "�y���b�V���nDB�ڑ��z" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITYMESH")) )
			EXE_ERRORLOG( "#Error ���ϐ� FC_CITYMESH �̎擾�Ɏ��s", NULL, false );
	IFeatureWorkspacePtr(ipMeshWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCityMeshFC );
	if( NULL == m_ipCityMeshFC )
		EXE_ERRORLOG( "#Error �s�s�n�}���b�V���t�B�[�`���N���X�̎擾�Ɏ��s", strEnv, false );

	if( FAILED(m_ipCityMeshFC->FindField(CComBSTR(_T("CREATE_YEAR")), &m_lCreateYear_FID)) )
			EXE_ERRORLOG( "#Error �s�s�n�}���b�V���쐬�N�x�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s", NULL, false );

	// [Bug6320]iPS�`�F�b�N���[�h���͖���
	if( kiPS != m_emCheckMode )
	{
		// �n�}�n�ڑ�(���k)	
		if( !strEnv.GetEnvironmentVariable(_T("DB_BASE")) )
			EXE_ERRORLOG( "#Error ���ϐ� DB_BASE �̎擾�Ɏ��s", NULL, false );

		const IWorkspacePtr ipBaseWorkspace = gf::getWorkspace( strEnv );
		if( NULL == ipBaseWorkspace )
			EXE_ERRORLOG( "#Error �n�}�nDB(���k)�ւ̐ڑ��Ɏ��s", strEnv, false );
		cout << "�y�n�}�nDB(���k)�ڑ��z" << strEnv << endl;

		if( !strEnv.GetEnvironmentVariable(_T("FC_BASE_SITE")) )
			EXE_ERRORLOG( "#Error ���ϐ� FC_BASE_SITE �̎擾�Ɏ��s", NULL, false );

		IFeatureWorkspacePtr(ipBaseWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipBaseSiteFC );
		if( NULL == m_ipBaseSiteFC )
			EXE_ERRORLOG( "#Error ���k�w�i�t�B�[�`���N���X�̎擾�Ɏ��s", strEnv, false );

		if( FAILED(m_ipBaseSiteFC->FindField(CComBSTR(_T("SC4BGCLASS_C")), &m_lBSC4BGClassC_FID)) )
			EXE_ERRORLOG( "#Error ���k�w�i��ʃt�B�[���h�C���f�b�N�X�̎擾�Ɏ��s", NULL, false );
	}

	return true;
}

// ���b�V�����X�g�ǂݍ���
bool CCheckExTPPoint::loadMeshlist( LPCTSTR lpszFile )
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
IFeaturePtr CCheckExTPPoint::getMeshFeature( long lMeshCode )
{
	CString strWhere;
	strWhere.Format( _T("MESHCODE=%d"), lMeshCode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipTargetMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor == NULL )
		return NULL;

	IFeaturePtr ipMeshFeature;
	return( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ? ipMeshFeature : NULL );
}

// �`�F�b�N���C��
void CCheckExTPPoint::checkMain()
{
	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(m_strWhereClause) );

	long lAllCount = 0;
	m_ipExTPFC->FeatureCount( ipQuery, &lAllCount );
	IFeatureCursorPtr ipFeatureCursor;
	m_ipExTPFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor )
	{
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			m_vecExTP.push_back( CExTP(ipFeature) );	/// �S�����́AORA-1555�΍�̂��߁A�������Ɋi�[
		}

		long lCount = 1;
		for( std::vector<CExTP>::const_iterator itr=m_vecExTP.begin();itr!=m_vecExTP.end();++itr )
		{
			switch( m_emCheckMode )
			{
			case kNormal:
			case kAcc4:
			case kiPS:
			case kHlp:
				checkExTPAndAdmin( *itr );
				break;
			case kAlps:
				checkAlpsAndAdmin( *itr );
				break;
			}

			checkExTPAndSite( *itr );
			checkSame20Addrcode( *itr );

			cerr << lCount++ << " / " << lAllCount << " ������\r";
		}
		cerr << endl;
	}
}

// ���b�V�����ƂɃ`�F�b�N
void CCheckExTPPoint::checkMainByMesh()
{
	// ���b�V���t�B�[�`���N���X�̎擾(�擪�ɂ���f�[�^���璷������)
	CString strMeshFName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strMeshFName = _T("REFERENCE.MiddleMesh");
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strMeshFName = _T("REFERENCE.BaseMesh");
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strMeshFName = _T("REFERENCE.CityMesh");
	else
	{
		cerr << "#Error ���b�V���R�[�h���A�s���ł�" << endl;
		return;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(strMeshFName), &m_ipTargetMeshFC );
	if( NULL == m_ipTargetMeshFC )
	{
		cerr << "#Error ���b�V���t�B�[�`���N���X�̎擾�Ɏ��s" << endl;
		return;
	}

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
		
		IFeatureCursorPtr ipFeatureCursor;
		LONG lAllCount = gf::SelectByShapeWithCount( ipMeshGeo, m_ipExTPFC, &ipFeatureCursor, esriSpatialRelIntersects, m_strWhereClause );
		if( lAllCount < 1 )
		{
			cout << *itr << " : ���b�V������ExTP�|�C���g�͂���܂���" << endl;
			continue;
		}
		LONG lCount = 1;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			CExTP ob(ipFeature);
			switch( m_emCheckMode )
			{
			case kNormal:
			case kAcc4:
			case kHlp:
				checkExTPAndAdmin( ob );	break;
			case kAlps:		checkAlpsAndAdmin( ob );	break;
			}

			checkExTPAndSite( ob );
			checkSame20Addrcode( ob );

			cerr << lCount++ << " / " << lAllCount << " ������\r";
		}

		cout << *itr << " : �`�F�b�N����" << endl;
	}
}

// �s�s�n�}�s���E�Ƃ̐����`�F�b�N
void CCheckExTPPoint::checkExTPAndAdmin( const CExTP& cExTP )
{
	// ExTP�|�C���g���ʒu����s�s�n�}�s���E�擾
	long lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature )
	{
		m_cError.printLog( cExTP, E_DUP_OR_NOTON_ADMIN );	// E19
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	switch( cExTP.m_ePointStatC )
	{
	case GOU_SEIBI:		// GOU����
	case GOU_EXSEIBI:	// GOU�g������
	case REFNUM_DIFF:	// �����ԍ��ƕs��v
	case NUM_NOTKEISAI:	// �ԍ����f��

		switch( cAdmin.m_eAddrClassC )
		{
		case OAZA_NOTSEIBI:			// �����ڊE������
		case JUKYO_TIBAN_NOTSEIBI:	// �Z���\���E�n�Ԗ�����
			if( cExTP.m_ePointStatC == GOU_EXSEIBI ){
				m_cError.printLog( cExTP, E_EXGOU_NOTSEIBI );	// E4
			}				
			else if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
				m_cError.printLog( cExTP, E_MISMATCH_ADDRCODE11_NOTSEIBI );	// N1, N7, W13, N10 �̑啪��
			}
			break;
		default:
			if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
				m_cError.printLog( cExTP, E_MISMATCH_ADDRCODE11_SEIBI );	// W2, W5, W8, W11 �̑啪��
			}
			else{
				switch( cExTP.m_ePointStatC )
				{
				case GOU_SEIBI:		// GOU����
				case REFNUM_DIFF:	// �����ԍ��ƕs��v
				case NUM_NOTKEISAI:	// �ԍ����f��
					if( cExTP.getAddrCode(16) != cAdmin.getAddrCode(16) ){
						m_cError.printLog( cExTP, E_MISMATCH_GAIKU );	// W3, N9, N12 �̑啪��
					}
					break;
				case GOU_EXSEIBI:	// GOU�g������
					if( cExTP.getAddrCode(16) == cAdmin.getAddrCode(16) ){
						m_cError.printLog( cExTP, E_MATCH_GAIKU );	// E24 �̑啪��
					}
					break;
				default:
					break;
				}
			}
			break;
		}
		break;

	case ADMIN_DIFF:	// �s���E�s��v

		if( cExTP.getAddrCode(11) == cAdmin.getAddrCode(11) ){
			if( cAdmin.m_eAddrClassC == OAZA_NOTSEIBI || cAdmin.m_eAddrClassC == JUKYO_TIBAN_NOTSEIBI ){
				m_cError.printLog( cExTP, W_ADMIN_ADDRCODE_NOTSEIBI );	// W13
			}
			else{
				m_cError.printLog( cExTP, W_ADMIN_ADDRCODE_SEIBI );	// W14
			}
		}
		break;

	case NOSURVEY:	// ������
		{
			CComVariant vaValue = cExTP.getValue( _T("OPERATOR") );
			if( vaValue.vt != VT_BSTR || strcmp(COLE2T(vaValue.bstrVal), "sindy") != 0 )
				m_cError.printLog( cExTP, E_NOSURVEY_OPERATOR );	// E15

		}
		break;
	case TPG_SEIBI:		// TPG����
		m_cError.printLog( cExTP, E_TPG );	// E6
		break;
	case UNKNOWN_POS:		// �ʒu�s��
	case UNDER_NUM:			// �n���X�ԍ�
	case SISETU_NOTKEISAI:	// �{�ݖ��f��
		break;
	default:
		m_cError.printLog( cExTP, E_UNKNOWN_POINTSTAT );	// E18
		break;
	}

	// 5���R�[�h�`�F�b�N([�ʒu�s��][������]�̂��̂̓`�F�b�N�ΏۊO)
	if( (cExTP.m_ePointStatC != NOSURVEY && cExTP.m_ePointStatC != UNKNOWN_POS)
		&& cExTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
		m_cError.printLog( cExTP, W_MISMATCH_CITYCODE );	// W16
	}

	// 8���R�[�h�`�F�b�N([GOU����][GOU�g������]�̂ݎ��{)
	if( cExTP.m_ePointStatC == GOU_SEIBI || cExTP.m_ePointStatC == GOU_EXSEIBI )
	{
		if( cExTP.getAddrCode(8) != cAdmin.getAddrCode(8) )
			m_cError.printLog( cExTP, (cExTP.m_ePointStatC == GOU_SEIBI)? W_GOU_OAZACODE_SEIBI : W_EXGOU_OAZACODE_SEIBI );	// W25 or W26
	}

	// �g���X�敄���ォ�`�F�b�N
	if( !cAdmin.m_strExGaikuFugo.empty() )
		m_cError.printLog( cExTP, E_ON_EXGAIKUFUGO );	// E17

}

// �s�s�n�}�s���E�Ƃ̐����`�F�b�N(�A���v�X�p)
void CCheckExTPPoint::checkAlpsAndAdmin( const CExTP& cExTP )
{
	// ExTP�|�C���g���ʒu����s�s�n�}�s���E�擾
	long lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature ){
		m_cError.printLog( cExTP, E_DUP_OR_NOTON_ADMIN );	// E19
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	// 5���R�[�h
	if( cExTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
		m_cError.printLog( cExTP, W_ALPS_CITYCODE );	// W27
		return;
	}

	// 8���R�[�h
	if( cExTP.getAddrCode(8) != cAdmin.getAddrCode(8) ){
		m_cError.printLog( cExTP, W_ALPS_OAZACODE );	// W28
		return;
	}

	// 11���R�[�h
	if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){

		switch( cAdmin.m_eAddrClassC  )
		{
		case JUKYO_SEIBI:			/// �Z���\�������ς�
		case TIBAN_SEIBI:			/// �n�Ԑ����ς�
			m_cError.printLog( cExTP, N_ALPS_ADDRCODE_SEIBI );	// N29
			break;
		default:
			m_cError.printLog( cExTP, N_ALPS_ADDRCODE_NOTSEIBI );	// N30
			break;
		}
	}
	else{
		// 11���������Ȃ�X��`�F�b�N
		if( cExTP.getAddrCode(16) != cAdmin.getAddrCode(16) ){
			switch( cAdmin.m_eAddrClassC  )
			{
			case JUKYO_SEIBI:			/// �Z���\�������ς�
			case TIBAN_SEIBI:			/// �n�Ԑ����ς�
				m_cError.printLog( cExTP, N_ALPS_GAIKU_SEIBI );	// N31
				break;
			default:
				break;
			}
		}
	}
}

// �w�i�Ƃ̐����`�F�b�N
void CCheckExTPPoint::checkExTPAndSite( const CExTP& cExTP )
{
	// �`�F�b�N���[�h���A���v�X�łȂ��A�������ʃR�[�h��[�ʒu�s��][������]�̂��̂̓`�F�b�N�ΏۊO
	if( m_emCheckMode != kAlps
		&& cExTP.m_ePointStatC == NOSURVEY || cExTP.m_ePointStatC == UNKNOWN_POS )
		return;

	// ExTP�|�C���g���ʒu����s�s�n�}�w�i�擾
	long lCount = 0;
	IFeaturePtr ipCitySiteFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'C' );
	if( lCount == 0 )
	{
		// [Bug 5622]�s�s�n�}������ꏊ�Ȃ�΃G���[�o��
		IFeatureCursorPtr ipMeshCursor;
		long lMeshCount = gf::SelectByShapeWithCount( cExTP.getGeo(), m_ipCityMeshFC, &ipMeshCursor, esriSpatialRelIntersects, NULL, _T("CREATE_YEAR") );
		if( lMeshCount > 0 )
		{
			IFeaturePtr ipMeshFeature;
			while( ipMeshCursor->NextFeature(&ipMeshFeature) == S_OK )
			{
				CComVariant vaValue;
				ipMeshFeature->get_Value( m_lCreateYear_FID, &vaValue );
				if( vaValue.vt != VT_NULL )
				{
					m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGCITYSITE : E_ALPS_ON_NGCITYSITE, "�w�i�Ȃ�[�׊X�H]" );	// E20 or E32
					break;
				}
			}
		}
	}
	else if( lCount == 1 )
	{
		CSite cCitySite( ipCitySiteFeature, 'C' );
		if( isNGCitySite(cCitySite.m_lBGClassC) )
			m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGCITYSITE : E_ALPS_ON_NGCITYSITE, cCitySite.getBGClassDomain() );	// E20 or E32
	}
	else if(lCount > 1 )	// ��������
	{
		m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_DUP_OR_NOTON_CITYSITE : E_ALPS_DUP_OR_NOTON_CITYSITE );	// E22 or E34
	}
	
	// [Bug 6320]���k�w�i�Ƃ̐����`�F�b�N�́AiPS�`�F�b�N���[�h�łȂ��Ƃ����s
	if( kiPS != m_emCheckMode )
	{
		// ExTP�|�C���g���ʒu���钆�k�w�i�擾
		lCount = 0;
		IFeaturePtr ipBaseSiteFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'B' );
		if( lCount == 1 )
		{
			CSite cBaseSite( ipBaseSiteFeature, 'B' );
			if( isNGBaseSite(cBaseSite.m_lBGClassC) )
				m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGBASESITE : E_ALPS_ON_NGBASESITE, cBaseSite.getBGClassDomain() );	// E21 or E33
		}
		else if( lCount > 1 )
		{
			m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_DUP_OR_NOTON_BASESITE : E_ALPS_DUP_OR_NOTON_BASESITE );	// E23 or E35
		}
	}
}

// �|�C���g������Ă���|���S�����擾
IFeaturePtr CCheckExTPPoint::getFeatureIntersectsPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch )
{
	IFeatureCursorPtr ipFeatureCursor;
	switch( szSwitch )
	{
	case 'A':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityAdminFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'C':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCitySiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'B':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipBaseSiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'M':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityMeshFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	default:
		return NULL;
	}

	// �w�i�|���S����ɍi��Ȃ��Ƃ��́ANULL��Ԃ�
	if( *lpCount == 1 )
	{
		IFeaturePtr ipFeature;
		return ( SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature))? ipFeature : NULL );
	}
	else
		return NULL;
}

// ����20���Z���R�[�h�����|�C���g���������݂��Ȃ����m�F�iBug9296 �ǉ��j
void	CCheckExTPPoint::checkSame20Addrcode( const CExTP& cExTP )
{
	// ���g�̒������ʃR�[�h���u�ʒu�s���v�u�������v�ł���΁A�`�F�b�N�ΏۊO
	if( NOSURVEY == cExTP.m_ePointStatC || UNKNOWN_POS == cExTP.m_ePointStatC )
		return;

	// ����20���Z���R�[�h�������́A���g�ł͂Ȃ����̂̌���
	CString strWhere;
	strWhere.Format( _T("%s='%s' and OBJECTID<>%d"), sindy::schema::extp_point::kAddrCode, cExTP.m_strAddrCode.c_str(), cExTP.getOID() );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipExTPFC->Search( ipQuery, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
		return;

	IFeaturePtr ipFeature;
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
	{
		// ����20���Z���R�[�h������ExTP������
		CExTP ob(ipFeature);

		// ��������ExTP�̒������ʃR�[�h���u�ʒu�s���v�u�������v�ȊO�̓G���[
		if( NOSURVEY != ob.m_ePointStatC && UNKNOWN_POS != ob.m_ePointStatC )
		{
			m_cError.printLog( cExTP, E_SAME_ADDRCODE, ob.getOID() );	// E36
		}
	}
}
