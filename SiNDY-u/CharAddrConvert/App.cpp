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
#include "App.h"
#include <io.h>
#include <conio.h>
#include "DataOutputter.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

CCharAddrConvert g_cYomiHelper;

CApp::CApp(void) : 
	m_emListMode(kAll)
{
}

CApp::~CApp(void)
{
}

// �g�p�@
void CApp::PrintUsage()
{
	std::cerr << "[�����t���Z���f�[�^�ϊ��c�[��]\n"
			  << "CharAddrConvert.exe [�e��I�v�V����]\n"
			  << "�E�I�v�V�����ɂ���\n"
			  << "  -i [�ݒ�t�@�C��] ... �ݒ�t�@�C�����w��(�K�{)\n"
			  << "  (���s�G���A�w��I�v�V����) ... �f�t�H���g�́A�S��)\n"
			  << "  -ca ... �S��\n"
			  << "  -cr [�J�n�s�撬���R�[�h-�I���s�撬���R�[�h] ... �G���A�w��\n"
			  << "  -cl [�s�撬�����X�g�t�@�C��] ... ���X�g�w��\n"
			  << "  -cs [�s�撬���R�[�h] ... �P��\n"
			  << "�����O�́A�W���G���[�o�͂���擾���ĉ�����" << std::endl;
}

// ������
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	// �R�}���h���C�����
	if( argc < 3 )
		return progress_out::ProgressError<bool>( false, kError, "�����̐����s���ł�" );

	CString strInitFile;
	for( int i = 1; i < argc - 1; ++i )
	{
		if( _tcscmp(argv[i], _T("-ca")) == 0 )
		{
			m_emListMode = kAll;
		}
		else if( _tcscmp(argv[i], _T("-cr")) == 0 )
		{
			m_emListMode = kRange;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-cl")) == 0 )
		{
			m_emListMode = kList;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-cs")) == 0 )
		{
			m_emListMode = kSingle;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-i")) == 0 )
		{
			strInitFile = argv[++i];
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "�s���ȃI�v�V���� : %s", argv[i] );
	}

	return LoadInitFile( strInitFile );	// �Ō�ɐݒ�t�@�C���ǂݍ���
}

// ���s
void CApp::Run()
{
	progress_out::ProgressTime( _T("�����J�n") );
	object_out::LogHeader();	// ���O�w�b�_�[�o��

	// �s�撬�����ƂɃf�[�^�ϊ��J�n
	for( std::set<CString>::const_iterator itr = m_setCode.begin(); itr != m_setCode.end(); ++itr )
	{
		object_out::g_strCurrentCityCode = *itr;
		progress_out::Progress( _T("%s : �ϊ��J�n"), *itr );

		// �w��5���ȉ��̓s�s�n�}�s���E�擾
		CString strWhere;
		strWhere.Format( _T("%s='%s'"), city_admin::kCityCode, *itr );
		long lAdminCount = 0;
		IFeatureCursorPtr ipFeatureCursor = gf::SelectByAttr( m_ipCityAdminFC, &lAdminCount, strWhere );
		if( ipFeatureCursor )
		{
			PNTMAP PntMap;	// Pnt���R�[�h�i�[�R���e�i
			GOUMAP GouMap;	// Gou���R�[�h�i�[�R���e�i

			long lIndex = 1;
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
			{
				CAdminInfo cAdminInfo( ipFeature, m_cAdminFID );

				// 11���L�����`�F�b�N
				if( m_cMakeAddrRec.IsValidAddrCode(cAdminInfo.m_strAddrCode) )
				{
					if( !cAdminInfo.m_strExtGaiku.IsEmpty() )
					{
						// ���~���擾�ł��Ă��Ȃ��Ȃ�A���Y�s���E���̍��|�C���g���f�[�^�����Ȃ�
						if( cAdminInfo.m_strExtGaikuYomi.IsEmpty() )
						{
							object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "�g���X�敄�������邪�A���~�̎擾�Ɏ��s�����̂Ńf�[�^���ł��Ȃ�\t%s", cAdminInfo.m_strExtGaiku );
							++lIndex;
							continue;
						}

						// �Z��������ʎ擾
						switch( cAdminInfo.m_lAddrClass_C )
						{
						case city_admin::addr_class::kResidenceIndicationMaintenance:	// �Z���\�������ς�
							m_cMakeAddrRec.MakePntRecordByAdmin( cAdminInfo, PntMap );	// �s���E�����\�_�𐶐�
							break;
						case city_admin::addr_class::kAzaUndeveloped:
						case city_admin::addr_class::kChibanMaintenance:
							object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "�g���X�敄���������āA�Z��������ʂ�[�����ڊE������|�n�Ԑ����ς�]��NG�ł�" );
							++lIndex;
							continue;
						default:
							break;
						}
					}

					// �s���E�ȉ��ɂ��鍆�|�C���g��20����(�����t���Ɍ���)
					IFeatureCursorPtr ipGouCursor = gf::SelectByShape( cAdminInfo.GetGeometry(), m_ipGouPointFC, NULL, esriSpatialRelIntersects );
					if( ipGouCursor )
					{
						IFeaturePtr ipGouFeature;					
						while( ipGouCursor->NextFeature(&ipGouFeature) == S_OK )
						{
							m_cMakeAddrRec.MakeGouRecordByDB( cAdminInfo, CGouInfo(ipGouFeature, m_cGouFID), GouMap );
						}
					}
				}
				else
					object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "11���R�[�h���Z���}�X�^�ɑ��݂��Ȃ� or �p�~�R�[�h�ł�\t%s", cAdminInfo.m_strAddrCode );

				std::cerr << lIndex++ << " / " << lAdminCount << " �� �s���E���̕ϊ��I��\r";
			}

			std::cerr << std::endl;

			// �n�ԃf�[�^����
			m_cMakeAddrRec.MakePntRecordByGou( GouMap, PntMap );

			// ���f�[�^����(����20�������\�_�𐶐�����)
			GOUMAP RepGouMap;
			m_cMakeAddrRec.SelectRepGouRecord( GouMap, RepGouMap );

			// �f�[�^�����o��
			if( !WriteData(*itr, PntMap, RepGouMap) )
				return;
		}

		progress_out::Progress( _T("%s : �ϊ��I��"), *itr );

		// �啶��[Q]������Ă�����r�����f
		if( _kbhit() && 'Q' == _getch() )
		{
			progress_out::Progress( _T("[Q]��������܂����B�ϊ��𒆒f���܂��B") );
			break;
		}
	}

	progress_out::ProgressTime( _T("�����I��") );
}

/////////////////////////////////////////
// �����֐�
/////////////////////////////////////////

// �ݒ�t�@�C���ǂݍ���
bool CApp::LoadInitFile( LPCTSTR lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return progress_out::ProgressError<bool>( false, kError, "�ݒ�t�@�C�����I�[�v���ł��܂���ł��� : %s", lpcszFile );

	po::options_description config_file( "�ݒ�t�@�C��" );
	config_file.add_options()
		("CONV_MODE",        po::value<int>()->default_value(kNormal),                  "�f�[�^�o�̓��[�h")
		("DB_ADDRESS",       po::value<std::string>(&m_cParam.m_strAddrDBProp),         "�Z���nSDE�ڑ��v���p�e�B")
		("DB_MAP",           po::value<std::string>(&m_cParam.m_strMapDBProp),          "�n�}�nSDE�ڑ��v���p�e�B")
		("FC_CITY_ADMIN",    po::value<std::string>(&m_cParam.m_strCityAdminFCName),    "�s�s�n�}�s���E�t�B�[�`���N���X��")
		("FC_GOU_POINT",     po::value<std::string>(&m_cParam.m_strGouPointFCName),     "���|�C���g�t�B�[�`���N���X��")
		("FC_CITY_SITE",     po::value<std::string>(&m_cParam.m_strCitySiteFCName),     "�s�s�n�}�w�i�t�B�[�`���N���X��")
		("FC_BUILDING",      po::value<std::string>(&m_cParam.m_strBuildingFCName),     "�s�s�n�}�����t�B�[�`���N���X��")
		("FC_BUILDING_ROOF", po::value<std::string>(&m_cParam.m_strBuildingRoofFCName), "�s�s�n�}����|���S���t�B�[�`���N���X��")
		("FC_CITYMESH",      po::value<std::string>(&m_cParam.m_strCityMeshFCName),     "�s�s�n�}���b�V���t�B�[�`���N���X��")
		("DB_CODECONV",      po::value<std::string>(&m_cParam.m_strCodeConvDB),         "�Z���R�[�h�ϊ�DB")
		("DB_YOMICONV",      po::value<std::string>(&m_cParam.m_strYomiConvDB),         "�Z���R�[�h�ϊ�DB")
		("JDB_CONNECT",      po::value<std::string>(&m_cParam.m_strJusyoDB),            "�Z��DB�ڑ��v���p�e�B")
		("PNT_DATA_DIR",     po::value<std::string>(&m_cParam.m_strPntDir),             "pntex�f�[�^�o�͐�")
		("GOU_DATA_DIR",     po::value<std::string>(&m_cParam.m_strGouDir),             "gouex�f�[�^�o�͐�");

	po::variables_map vm;
	try
	{
		po::store( po::parse_config_file(ifs, config_file), vm );
	}
	catch( std::exception& e )
	{
		return progress_out::ProgressError<bool>( false, kError, "�ݒ�t�@�C���̉�͂Ɏ��s [%s]", e.what() );
	}

	po::notify( vm );

	// �ϊ����[�h(Bug7599)
	if( ! (0 < vm["CONV_MODE"].as<int>() && vm["CONV_MODE"].as<int>() < 3) )
		return progress_out::ProgressError<bool>( false, kError, "�ϊ����[�h�����e�O�̒l : %d", vm["CONV_MODE"].as<int>() );
	m_cParam.m_emConvMode = static_cast<emConvMode>(vm["CONV_MODE"].as<int>());

	progress_out::Progress( "�ϊ����[�h : %s", (kNormalConv == m_cParam.m_emConvMode? "�i�r����" : "�O������") );

	// DB�ڑ��E�t�B�[�`���N���X�擾
	m_ipAddrWorkspace = gf::GetWorkspace( m_cParam.m_strAddrDBProp.c_str() );
	if( NULL == m_ipAddrWorkspace )
		return progress_out::ProgressError<bool>( false, kError, "�Z���nSDE�ւ̐ڑ��Ɏ��s : %s", m_cParam.m_strAddrDBProp.c_str() );

	progress_out::Progress( "�Z���nSDE�֐ڑ� : %s", m_cParam.m_strAddrDBProp.c_str() );

	// �t�B�[�`���N���X�擾
	((IFeatureWorkspacePtr)m_ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCityAdminFCName.c_str()), &m_ipCityAdminFC );
	if( m_ipCityAdminFC )
		m_cAdminFID.CreateFieldMap( (ITablePtr)m_ipCityAdminFC );
	else
		return progress_out::ProgressError<bool>( false, kError, "�s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strCityAdminFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strGouPointFCName.c_str()), &m_ipGouPointFC );
	if( m_ipGouPointFC )
		m_cGouFID.CreateFieldMap( (ITablePtr)m_ipGouPointFC );
	else
		return progress_out::ProgressError<bool>( false, kError, "���|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strGouPointFCName.c_str() );

	m_ipMapWorkspace = gf::GetWorkspace( m_cParam.m_strMapDBProp.c_str() );
	if( !m_ipMapWorkspace )
		return progress_out::ProgressError<bool>( false, kError, "�n�}�nSDE�ւ̐ڑ��Ɏ��s : %s", m_cParam.m_strMapDBProp.c_str() );

	progress_out::Progress( "�n�}�nSDE�֐ڑ� : %s", m_cParam.m_strMapDBProp.c_str() );

	// �t�B�[�`���N���X�擾
	IFeatureClassPtr ipCitySiteFC, ipBuildingFC, ipBuildingRoofFC, ipCityMeshFC;
	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCitySiteFCName.c_str()), &ipCitySiteFC );
	if( !ipCitySiteFC )
		return progress_out::ProgressError<bool>( false, kError, "�s�s�n�}�w�i�|���S���t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strCitySiteFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strBuildingFCName.c_str()), &ipBuildingFC );
	if( !ipBuildingFC )
		return progress_out::ProgressError<bool>( false, kError, "�s�s�n�}�����|���S���t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strBuildingFCName.c_str() );
	
	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strBuildingRoofFCName.c_str()), &ipBuildingRoofFC );
	if( !ipBuildingRoofFC )
		return progress_out::ProgressError<bool>( false, kError, "�s�s�n�}����|���S���t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strBuildingRoofFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCityMeshFCName.c_str()), &ipCityMeshFC );
	if( !ipCityMeshFC )
		return progress_out::ProgressError<bool>( false, kError, "�s�s�n�}���b�V���|���S���t�B�[�`���N���X�̎擾�Ɏ��s : %s", m_cParam.m_strCityMeshFCName.c_str() );

	// �R�[�h�ϊ��N���X������
	if( !m_cMakeAddrRec.Init(m_ipCityAdminFC, ipCitySiteFC, ipBuildingFC, ipBuildingRoofFC, ipCityMeshFC, m_cParam.m_strCodeConvDB.c_str(), m_cParam.m_strJusyoDB.c_str()) )
		return false;

	// ���~�ϊ��N���X������
	std::multimap<long, CString> mapError = g_cYomiHelper.Init( m_cParam.m_strYomiConvDB.c_str() );
	if( !mapError.empty() )
	{
		// �G���[�L��
		for( std::multimap<long, CString>::const_iterator itr = mapError.begin(); itr != mapError.end(); ++itr )
			progress_out::ProgressError<int>( 0, kError, "%s : %s ", itr->second, m_cParam.m_strYomiConvDB.c_str() );

		return false;
	}

	// �s�撬���R�[�h���X�g����
	if( !MakeCityCodeList() )
		return false;

	// pntex�f�[�^�o�͐�̑��݊m�F(�Ȃ���΍쐬)
	if( _access_s(m_cParam.m_strPntDir.c_str(), 0) != 0 )
	{
		if( !CreateDirectory(m_cParam.m_strPntDir.c_str(), NULL) )
			return progress_out::ProgressError<bool>( false, kError, "pntex�o�̓f�B���N�g�����쐬�ł��܂��� : %s", m_cParam.m_strPntDir.c_str() );	
	}

	// gouex�f�[�^�o�͐�̑��݊m�F(�Ȃ���΍쐬)
	if( _access_s(m_cParam.m_strGouDir.c_str(), 0) != 0 )
	{
		if( !CreateDirectory(m_cParam.m_strGouDir.c_str(), NULL) )
			return progress_out::ProgressError<bool>( false, kError, "gouex�o�̓f�B���N�g�����쐬�ł��܂��� : %s", m_cParam.m_strGouDir.c_str() );
	}
	
	return true;
}

// �s�撬���R�[�h���X�g�쐬
bool CApp::MakeCityCodeList()
{
	switch( m_emListMode )
	{
	case kAll:
	case kRange:
		{
			CString strWhere;
			if( kAll == m_emListMode )
				strWhere.Format( _T("%s != '00000'"), city_admin::kCityCode );
			else
			{
				// �͈͂��擾
				int nPos = m_strListInfo.Find('-');
				if( -1 != nPos )
					strWhere.Format( _T("%s between '%s' and '%s'"), city_admin::kCityCode, m_strListInfo.Left(nPos), m_strListInfo.Mid(nPos+1) );
				else
					return progress_out::ProgressError<bool>( false, kError, "�s�撬���͈͎w����@����������(<code>-<code>) : %s", m_strListInfo );
			}

			// �s���E����
			IQueryDefPtr	ipQueryDef;
			IFeatureWorkspacePtr(m_ipAddrWorkspace)->CreateQueryDef( &ipQueryDef );
			ipQueryDef->put_Tables( _bstr_t(_T(m_cParam.m_strCityAdminFCName.c_str())) );
			ipQueryDef->put_WhereClause( _bstr_t(strWhere) );
			CString strSubFields;
			strSubFields.Format( _T("distinct(%s)"), city_admin::kCityCode );
			ipQueryDef->put_SubFields( CComBSTR(strSubFields) );

			_ICursorPtr ipCursor;
			if( SUCCEEDED(ipQueryDef->Evaluate(&ipCursor)) && ipCursor )
			{
				_IRowPtr ipRow;
				while( ipCursor->NextRow(&ipRow) == S_OK )
				{
					CComVariant vaValue;
					ipRow->get_Value( 0, &vaValue );
					if( VT_BSTR == vaValue.vt )
						m_setCode.insert( CString(vaValue.bstrVal) );
				}
			}
		}
		break;
	case kList:
		{
			// ���X�g����s�撬���R�[�h�擾
			std::ifstream ifs( static_cast<CT2CW>(m_strListInfo) );
			if( ifs )
			{
				std::string strBuff;
				while( std::getline(ifs, strBuff) ){ m_setCode.insert( strBuff.c_str() ); }
			}
			else
				return progress_out::ProgressError<bool>( false, kError, "�s�撬�����X�g�t�@�C���̓ǂݍ��݂Ɏ��s : %s", m_strListInfo );
		}
		break;
	case kSingle:
		m_setCode.insert( m_strListInfo );
		break;
	}
	return true;
}

// �f�[�^�����o��
bool CApp::WriteData( LPCTSTR lpcszCityCode, const PNTMAP& rPntList, const GOUMAP& rGouList )
{
	// pntex
	CString strFile;
	if( !rPntList.empty() )
	{
		strFile.Format( _T("%s\\pntex%s.txt"), m_cParam.m_strPntDir.c_str(), lpcszCityCode );
		std::ofstream ofspnt( static_cast<CT2CW>(strFile), std::ios::out | std::ios::binary );
		if( ofspnt )
		{
			// �ϊ����[�h�ŏꍇ�킯(Bug7599)
			if( kNormalConv == m_cParam.m_emConvMode )
				data_output::OutPntFile( ofspnt, rPntList );
			else
				data_output::OutPntFile_Outside( ofspnt, rPntList );
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "�o�͗ppntex�t�@�C���̃I�[�v���Ɏ��s : %s", strFile );
	}

	// gouex
	if( !rGouList.empty() )
	{
		strFile.Format( _T("%s\\gouex%s.txt"), m_cParam.m_strGouDir.c_str(), lpcszCityCode );
		std::ofstream ofsgou( static_cast<CT2CW>(strFile), std::ios::out | std::ios::binary );
		if( ofsgou )
		{
			// �ϊ����[�h�ŏꍇ�킯(Bug7599)
			if( kNormalConv == m_cParam.m_emConvMode )
				data_output::OutGouFile( ofsgou, rGouList );
			else
				data_output::OutGouFile_Outside( ofsgou, rGouList );
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "�o�͗pgouex�t�@�C���̃I�[�v���Ɏ��s : %s", strFile );
	}
	return true;
}
