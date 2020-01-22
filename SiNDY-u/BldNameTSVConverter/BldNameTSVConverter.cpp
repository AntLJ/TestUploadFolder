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

#include "stdafx.h"
#include "BldNameTSVConverter.h"
#include "TsvWriter.h"
#include "MultiTranslateListWriter.h"
#include <crd_cnv/crd_cnv.h>
#include <boost/assign.hpp>
#include <TDC/useful_headers/str_util.h>
#include <regex>

crd_cnv gCrdCnv;

namespace po = boost::program_options;

LPCSTR g_strKey_INI			= "ini";
LPCSTR g_strKey_RUN_LOG		= "run_log";
LPCSTR g_strKey_ERR_LOG		= "err_log";
LPCSTR g_strKey_HELP		= "help";

LPCSTR g_strKey_BASE_DB		= "BASE_DB";
LPCSTR g_strKey_MAP_DB		= "MAP_DB";
LPCSTR g_strKey_ADDR_DB		= "ADDR_DB";
LPCSTR g_strKey_AM_DB		= "AM_DB";
LPCSTR g_strKey_GRP_CODE	= "GRP_CODE";
LPCSTR g_strKey_OUTPUT_DIR	= "OUTPUT_DIR";
LPCSTR g_strKey_TRANSLATE   = "TRANSLATE";
LPCSTR g_strKey_ADDCOLUMN   = "ADD_COLUMN";
LPCSTR g_strKey_MAX_REC_NUM = "MAX_REC_NUM";

LPCSTR g_strFile_BLD001		= "BLD001.tsv.bld";
LPCSTR g_strFile_MSTBLA		= "MSTBLA.tsv.bld";

const long g_lClassNameMaxBytes	= 40;
const long g_lBldgNameMaxBytes	= 80;
const long g_lBldgYomiMaxBytes	= 160;
const long g_lBldgNameSepMaxBytes	= 120;
const long g_lBldgYomiSepMaxBytes	= 200;

const long g_lBldgOIDRange		= 500000;

BldNameTSVConverter::BldNameTSVConverter() : 
	m_conAddrDB( __uuidof(ADODB::Connection) ), m_mode(ExecuteMode::ReleaseTsv)
{
}

BldNameTSVConverter::~BldNameTSVConverter()
{
}

bool BldNameTSVConverter::init( int argc, _TCHAR* argv[] )
{
	// �R�}���h�����̃`�F�b�N
	if( !checkArg( argc, argv ) ) return false;

	// �ݒ�t�@�C���̓ǂݍ���
	if( !loadIniFile() ) return false;

	// ���s���[�h��킸�A�����ŕK�v�ȏ��������܂����{

	// DB�̃I�[�v��
	IWorkspacePtr ipWSBase, ipWSMap, ipWSAddr;
	if( !common_proc::getWorkspace( m_strDBBase, ipWSBase ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + m_strDBBase );
		return false;
	}

	// �t�B�[�`���N���X�̎擾
	if( !common_proc::getFeatureClass( sindy::schema::sj::buildingname_point::kTableName, ipWSBase, m_ipFCBldgNamePoint ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + CString( sindy::schema::sj::buildingname_point::kTableName ) );
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�}�b�v�̎擾
	if( !common_proc::getFieldIndexes( m_ipFCBldgNamePoint, m_mapIndexesBldgNamePoint ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[���h�C���f�b�N�X�}�b�v���擾�ł��܂��� : ") + CString( sindy::schema::sj::buildingname_point::kTableName ) );
		return false;
	}

	// �u������|�󃊃X�g�쐬�v���[�h���̏�����
	if( ExecuteMode::TranslateList == m_mode )
	{
		// ������|�󃊃X�g�ǉ��J�����ݒ�t�@�C���Ǎ���
		std::ifstream ifs( m_strAddColumn );
		if( !ifs.is_open() )
		{
			LogMgr::getInstance().writeRunLog( _T("������|�󃊃X�g�ǉ��J�����ݒ�t�@�C�����I�[�v���ł��܂��� : ") + m_strAddColumn );
			return false;
		}

		std::string line;
		while(std::getline(ifs, line))
		{
			// ��s�ƃR�����g�s�͎擾�ΏۊO
			if( line.empty() || line.at(0) == _T('#') )
				continue;

			m_addcolumnList.push_back( CString(line.c_str()) );
		}

		return true;
	}

	// �u���������[�X�f�[�^�쐬�v���[�h���݂̂ŕK�v�ȏ����������{

	// DB�̃I�[�v��
	if( !common_proc::getWorkspace( m_strDBMap, ipWSMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + m_strDBMap );
		return false;
	}
	if( !common_proc::getWorkspace( m_strDBAddr, ipWSAddr ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + m_strDBAddr );
		return false;
	}

	// �t�B�[�`���N���X�̎擾
	if( !common_proc::getFeatureClass( sindy::schema::building::kTableName, ipWSMap, m_ipFCBuilding ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + CString( sindy::schema::building::kTableName ) );
		return false;
	}
	if( !common_proc::getFeatureClass( sindy::schema::city_admin::kTableName, ipWSAddr, m_ipFCCityAdmin ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + CString( sindy::schema::city_admin::kTableName ) );
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�}�b�v�̎擾
	if( !common_proc::getFieldIndexes( m_ipFCCityAdmin, m_mapIndexesCityAdmin ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[���h�C���f�b�N�X�}�b�v���擾�ł��܂��� : ") + CString( sindy::schema::city_admin::kTableName ) );
		return false;
	}

	// �Z�����ԃe�[�u���i�[DB�̃I�[�v��
	if( !openAddrDB() ) return false;

	return true;
}

bool BldNameTSVConverter::execCreateReleaseTsv()
{
	// ������ʃR�[�h�}�X�^�̐���
	if( !createBldgClassMaster() ) return false;

	// ���������[�X�f�[�^�iTSV�j�̐���
	if( !createBldgNameTSV() ) return false;

	return true;
}

bool BldNameTSVConverter::execCreateMultiTranslateList()
{
	// ������|�󃊃X�g�̐���
	return createMultiTranslateList();
}

bool BldNameTSVConverter::exec()
{
	// ���s���[�h�ɉ���������
	switch(m_mode)
	{
	case ExecuteMode::ReleaseTsv : // ���������[�X�f�[�^�쐬
		return execCreateReleaseTsv();
		break;
	case ExecuteMode::TranslateList: // ������|�󃊃X�g�쐬
		return execCreateMultiTranslateList();
		break;
	}
	return true;
}

bool BldNameTSVConverter::checkArg( int argc, _TCHAR* argv[] )
{
	po::options_description descOption( "�I�v�V����" );
	descOption.add_options()
		( g_strKey_INI,	          po::value<std::string>(),	"�ݒ�t�@�C����" )
		( g_strKey_RUN_LOG,	      po::value<std::string>(),	"���s���O�t�@�C����" )
		( g_strKey_ERR_LOG,	      po::value<std::string>(),	"�G���[���O�t�@�C����" )
		( g_strKey_HELP,                                    "�w���v��\��" );
	po::variables_map varMap;
	po::store( po::parse_command_line( argc, argv, descOption ), varMap );
	po::notify( varMap );

	// �w���v�\��
	if( varMap.count( g_strKey_HELP ) || 0 == varMap.size() )
	{
		std::cout << descOption << std::endl;
		return false;
	}

	// �K�{�J�����ꗗ�i���s���[�h�Ɋ֌W�Ȃ����ʂ̂��̂�����`�j
	std::vector<std::pair<LPCSTR, CString*>> paramList = boost::assign::pair_list_of
		(g_strKey_RUN_LOG, &m_strRunLog)
		(g_strKey_ERR_LOG, &m_strErrLog)
		(g_strKey_INI, &m_strIniFile)
		;

	// �K�{�I�v�V��������
	for(const auto& param : paramList)
	{
		if( !getArgParam( varMap, param.first, *(param.second) ) )
		{
			LogMgr::getInstance().writeRunLog( uh::str_util::format(_T("%s�I�v�V�������w�肳��Ă��܂���"), (CString)param.first) );
			return false;
		}
	}

	// ���s���O�t�@�C����
	if( !LogMgr::getInstance().setRunLog( m_strRunLog, &varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("���s���O�t�@�C�����I�[�v���ł��܂��� : ") + m_strRunLog );
		return false;
	}

	// �G���[���O�t�@�C����
	if( !LogMgr::getInstance().setErrLog( m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�G���[���O�t�@�C�����I�[�v���ł��܂��� : ") + m_strErrLog );
		return false;
	}
	LogMgr::getInstance().writeErrLog( _T("# FREESTYLELOG") );
	LogMgr::getInstance().writeErrLog( _T("FLAG\tERR_LV\tLAYER\tOBJECTID\tERR_MSG") );

	return true;
}

bool BldNameTSVConverter::getArgParam( const boost::program_options::variables_map& varMap, const std::string& strName, CString& strParam )
{
	if( 0 == varMap.count( strName ) ) return false;
	strParam = CString( varMap[strName].as<std::string>().c_str() );
	if( strParam.IsEmpty() ) return false;
	return true;
}

bool BldNameTSVConverter::loadIniFile()
{
	po::options_description descIniFile( "�ݒ�t�@�C��" );
	descIniFile.add_options()
		( g_strKey_BASE_DB,		po::value<std::string>(), "�x�[�XDB" )
		( g_strKey_MAP_DB,		po::value<std::string>(), "�n�}DB" )
		( g_strKey_ADDR_DB,		po::value<std::string>(), "�Z��DB" )
		( g_strKey_AM_DB,		po::value<std::string>(), "�Z�����ԃe�[�u���i�[DB" )
		( g_strKey_GRP_CODE,	po::value<std::string>(), "�f�[�^���ރR�[�h" )
		( g_strKey_OUTPUT_DIR,	po::value<std::string>(), "�o�̓f�B���N�g��" )
		( g_strKey_TRANSLATE, 	                          "������|�󃊃X�g�쐬���[�h" )
		( g_strKey_ADDCOLUMN,   po::value<std::string>(), "������|�󃊃X�g�ǉ��J�����ݒ�t�@�C��" )
		( g_strKey_MAX_REC_NUM, po::value<std::string>(), "������|�󃊃X�g���ő僌�R�[�h��" );


	std::ifstream ifs( m_strIniFile );
	if( !ifs.is_open() )
	{
		LogMgr::getInstance().writeRunLog( _T("�ݒ�t�@�C�����I�[�v���ł��܂���") );
		return false;
	}
	po::variables_map varMap;
	po::store( po::parse_config_file( ifs, descIniFile ), varMap );
	po::notify( varMap );

	// �uTRANSLATE�v���ڂ�����A�l���utrue�v�̏ꍇ�A���s���[�h���u������|�󃊃X�g�쐬�v�ɕύX
	if( 0 != varMap.count( g_strKey_TRANSLATE ) && 
			0 == CString( varMap[ g_strKey_TRANSLATE ].as<std::string>().c_str() ).CompareNoCase( _T("true") ) )
			m_mode = ExecuteMode::TranslateList;

	// �K�{�ݒ荀�ڈꗗ�i���������[�X�f�[�^�������j
	const std::vector<std::pair<LPCSTR, CString*>> paramReleaseTsvList = boost::assign::pair_list_of
		( g_strKey_BASE_DB, &m_strDBBase )
		( g_strKey_MAP_DB, &m_strDBMap )
		( g_strKey_ADDR_DB, &m_strDBAddr )
		( g_strKey_AM_DB, &m_strDBAM )
		( g_strKey_GRP_CODE, &m_strGrpCode )
		( g_strKey_OUTPUT_DIR, &m_strOutDir )
		;

	// �K�{�ݒ荀�ڈꗗ�i������|�󃊃X�g�쐬���j
	const std::vector<std::pair<LPCSTR, CString*>> paramTranslateList = boost::assign::pair_list_of
		( g_strKey_BASE_DB, &m_strDBBase )
		( g_strKey_GRP_CODE, &m_strGrpCode )
		( g_strKey_OUTPUT_DIR, &m_strOutDir )
		( g_strKey_ADDCOLUMN, &m_strAddColumn )	
		( g_strKey_MAX_REC_NUM, &m_maxRecNum )	
		;

	auto paramList = ( m_mode == ExecuteMode::ReleaseTsv ) ?
							paramReleaseTsvList : paramTranslateList;

	LogMgr::getInstance().writeParamInfos( varMap );

	// �K�{�ݒ荀�ڌ���
	for(const auto& param : paramList)
	{
		if( !getArgParam( varMap, param.first, *(param.second) ) )
		{
			LogMgr::getInstance().writeRunLog( _T("�ݒ荀�� ") + CString( param.first ) + _T(" ���ݒ肳��Ă��܂���") );
			return false;
		}
	}

	if( ExecuteMode::TranslateList == m_mode )
	{
		// ������|�󃊃X�g���R�[�h�ő�o�͌����l�̌����i1�ȏ�̐����ł��邩�j
		std::wregex maxRecNumRegex(L"^[1-9]{1}[0-9]{0,}$");
		if( 0 != varMap.count(g_strKey_MAX_REC_NUM) && !std::regex_match( CStringW(m_maxRecNum).GetString(), maxRecNumRegex ) )
		{
			LogMgr::getInstance().writeRunLog( _T("������|�󃊃X�g���R�[�h�ő�o�͌�����1�ȏ�̐����ł͂���܂��� : ") + m_maxRecNum );
			return false;
		}
	}

	ifs.close();

	return true;
}

bool BldNameTSVConverter::openAddrDB()
{
	// �ڑ������񂩂烆�[�U�E�T�[�r�X�����擾
	int nPos = m_strDBAM.Find( _T('@') );
	if( 0 >= nPos )
	{
		LogMgr::getInstance().writeRunLog( _T("�Z�����ԃe�[�u���i�[DB�ւ̐ڑ������񂪖����ł� : ") + m_strDBAM );
		return false;
	}
	CString strDBUser = m_strDBAM.Left( nPos );
	CString strDBSrvc = m_strDBAM.Mid( nPos + 1 );
	if( strDBUser.IsEmpty() || strDBSrvc.IsEmpty() )
	{
		LogMgr::getInstance().writeRunLog( _T("�Z�����ԃe�[�u���i�[DB�ւ̐ڑ������񂪖����ł� : ") + m_strDBAM );
		return false;
	}

	// DB�ɐڑ�
	CString strConnect;
	strConnect.Format( _T(" Provider=OraOLEDB.Oracle; Data Source=%s; User ID=%s; Password=%s; "), strDBSrvc, strDBUser, strDBUser );
	m_conAddrDB->ConnectionString = _bstr_t( strConnect );
	try
	{
		if( FAILED( m_conAddrDB->Open( "", "", "", ADODB::adConnectUnspecified ) ) )
		{
			LogMgr::getInstance().writeRunLog( _T("�Z�����ԃe�[�u���i�[DB�ɐڑ��ł��܂��� : ") + m_strDBAM );
			return false;
		}
	}
	catch( const _com_error& e )
	{
		LogMgr::getInstance().writeRunLog( _T("�Z�����ԃe�[�u���i�[DB�ڑ����ɗ�O���� : ") + m_strDBAM );
		throw e;
	}

	return true;
}

void BldNameTSVConverter::outputErrLog( LogMgr::EErrLevel errLv, const CString& strLayer, long lOID, const CString& strMsg )
{
	CString strTmp;
	strTmp.Format( _T("0\t%s\t%s\t%ld\t%s"), LogMgr::getErrLvString( errLv ), strLayer, lOID, strMsg );
	LogMgr::getInstance().writeErrLog( strTmp );
}

bool BldNameTSVConverter::createBldgClassMaster()
{
	std::cout << "������ʃR�[�h�}�X�^�o�͊J�n" << std::endl;

	// �}�X�^�t�@�C�����J���i���s�R�[�h��LF�ƂȂ�悤�o�C�i�����[�h�ŊJ���j
	TsvWriter writer;
	CString file = uh::str_util::format( _T("%s\\%s"), m_strOutDir, (CString)g_strFile_MSTBLA );
	if( !writer.open( file, Encoding::Euc, LineFeed::Lf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("������ʃR�[�h�}�X�^�t�@�C�����I�[�v���ł��܂��� : ") + file );
		return false;
	}

	// �R�[�h�l�h���C���擾
	std::map< long, CString > mapValues;
	if( !common_proc::getCodedValues( m_ipFCBldgNamePoint, sindy::schema::sj::buildingname_point::kBldgClass1, mapValues ) )
	{
		LogMgr::getInstance().writeRunLog( _T("������ʃR�[�h�̃R�[�h�l�h���C�����擾�ł��܂��� : ") + CString( sindy::schema::sj::buildingname_point::kBldgClass1 ) );
		return false;
	}

	// �}�X�^�t�@�C���ɃR�[�h�l�h���C���̈ꗗ���o��
	for( const auto& rec : mapValues )
	{
		// ����������̃o�C�g�����`�F�b�N
		if( common_proc::convSJIS2EUC( std::string( CT2A( rec.second ) ) ).size() > g_lClassNameMaxBytes )
		{
			CString strTmp;
			strTmp.Format( _T("������ʃR�[�h�̐��������� %ld �o�C�g�𒴂��Ă��� : [%ld] %s"), g_lClassNameMaxBytes, rec.first, rec.second );
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, 0, strTmp );
		}

		std::vector<CString> valueList = boost::assign::list_of
			( uh::str_util::ToString(rec.first) )
			( rec.second )
			;

		CString joinStr = uh::str_util::join(valueList, _T("\t"));
		std::string writeStr = CT2A( joinStr + LINE_FEED_CODE_LF );

		// EUC�ŏo��
		if( !writer.writeRecord( valueList ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, 0, _T("������ʃR�[�h�}�X�^�t�@�C���ɏ������߂Ȃ�") );
			continue;
		}
	}

	// �}�X�^�t�@�C�������
	writer.close();

	std::cout << "������ʃR�[�h�}�X�^�o�͊���" << std::endl;

	return true;
}

bool BldNameTSVConverter::searchBldNamePoint(IFeatureCursorPtr& ipFeatureCur, long& lCount)
{
	// �����r�����̃|�C���g�̃J�[�\�����擾
	CString strWhere( CString( sindy::schema::sj::buildingname_point::kYomiSeparated ) + _T(" IS NOT NULL") );
	CString strPostfix( _T("ORDER BY ") + CString( sindy::schema::sj::buildingname_point::kBuildingOID ) );
	if( !common_proc::searchByWhereClause( m_ipFCBldgNamePoint, ipFeatureCur, lCount, strWhere, strPostfix ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�����r�����̃|�C���g�̌����Ɏ��s���܂��� : ") + strWhere );
		return false;
	}
	return true;
}

bool BldNameTSVConverter::getCommonAttrValue(const IFeaturePtr& ipFeature, BldNamePointRec& rec )
{
	// ���̃|�C���g��OID���擾
	ipFeature->get_OID( &rec.bldNamePointOID );

	// �����|���S��OID���擾
	CComVariant varBldgOID;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kBuildingOID, m_mapIndexesBldgNamePoint, varBldgOID ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("�����|���S��OID���擾�ł��Ȃ�") );
		return false;
	}
	rec.bldgOID = varBldgOID.lVal;

	// ���̂��擾
	CComVariant varName;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kName, m_mapIndexesBldgNamePoint, varName ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("���̂��擾�ł��Ȃ�") );
		return false;
	}
	rec.name = ( VT_BSTR == varName.vt ) ? varName.bstrVal : _T("");

	// ���̂̃o�C�g�����`�F�b�N
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.name ) ) ).size() > g_lBldgNameMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("���̂̃T�C�Y�� %ld �o�C�g�𒴂��Ă��� : %s"), g_lBldgNameMaxBytes, rec.name );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// ���́i�����j���擾
	CComVariant varNameSep;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kNameSeparated, m_mapIndexesBldgNamePoint, varNameSep ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("���́i�����j���擾�ł��Ȃ�") );
		return false;
	}
	rec.nameSep = ( VT_BSTR == varNameSep.vt ) ? varNameSep.bstrVal : _T("");

	// ���́i�����j�̃o�C�g�����`�F�b�N
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.nameSep ) ) ).size() > g_lBldgNameSepMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("���́i�����j�̃T�C�Y�� %ld �o�C�g�𒴂��Ă��� : %s"), g_lBldgNameSepMaxBytes, rec.nameSep );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// �ǂ݁i�����j���擾
	CComVariant varYomiSep;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kYomiSeparated, m_mapIndexesBldgNamePoint, varYomiSep ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("���~�i�����j���擾�ł��Ȃ�") );
		return false;
	}
	rec.yomiSep = ( VT_BSTR == varYomiSep.vt ) ? varYomiSep.bstrVal : _T("");

	// �ǂ݂̃o�C�g�����`�F�b�N
	// �i�ǂ݁i�����j�̒l���番�������������������́j
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.getRemovedSepStrYomi() ) ) ).size() > g_lBldgYomiMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("�ǂ݂̃T�C�Y�� %ld �o�C�g�𒴂��Ă��� : %s"), g_lBldgYomiMaxBytes, rec.getRemovedSepStrYomi() );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// �ǂ݁i�����j�̃o�C�g�����`�F�b�N
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.yomiSep ) ) ).size() > g_lBldgYomiSepMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("�ǂ݁i�����j�̃T�C�Y�� %ld �o�C�g�𒴂��Ă��� : %s"), g_lBldgYomiSepMaxBytes, rec.yomiSep );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	return true;
}

bool BldNameTSVConverter::createBldgNameTSV()
{
	std::cout << "�����r�����̌��������[�X�f�[�^�o�͊J�n" << std::endl;

	// ���������[�X�f�[�^�t�@�C�����J��
	TsvWriter writer;
	CString file = uh::str_util::format( _T("%s\\%s"), m_strOutDir, (CString)g_strFile_BLD001 );
	if( !writer.open( file, Encoding::Euc, LineFeed::Lf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("���������[�X�f�[�^�t�@�C�����I�[�v���ł��܂��� : ") + file );
		return false;
	}
	
	// �����r�����̃|�C���g������
	long lCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( !searchBldNamePoint( ipFeatureCursor, lCount ) )
		return false;

	std::cout << "�o�͑Ώی��������[�X�f�[�^���� : " << std::setw( 7 ) << lCount << " ��" << std::endl;

	// �����r�����̃|�C���g�̃��[�v
	IFeaturePtr ipFeature;
	long i = 0;
	while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
	{
		++i;
		if( 0 == i % 100 || i == lCount )
		{
			std::cout << "���������[�X�f�[�^�o�͒� [ " << std::setw( 7 ) << i << " / " << std::setw( 7 ) << lCount << " ]\r";
		}

		BldNamePointRec rec;

		// ���ʑ����l�擾
		if( !getCommonAttrValue( ipFeature, rec ) ) 
			continue;

		// ������ʃR�[�h1���擾
		CComVariant varBlgdClass1;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kBldgClass1, m_mapIndexesBldgNamePoint, varBlgdClass1 ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("������ʃR�[�h1���擾�ł��Ȃ�") );
			continue;
		}
		rec.bldgClass1 = ( VT_I4 == varBlgdClass1.vt ) ? CString( std::to_string( varBlgdClass1.lVal ).c_str() ) : _T("");

		// �K�����擾
		CComVariant varFloors;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kFloors, m_mapIndexesBldgNamePoint, varFloors ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("�K�����擾�ł��Ȃ�") );
			continue;
		}
		rec.floors = ( VT_I4 == varFloors.vt ) ? CString( std::to_string( varFloors.lVal ).c_str() ) : _T("");

		// �R���e���c�R�[�h���擾
		CComVariant varContentsCode;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kContentsCode, m_mapIndexesBldgNamePoint, varContentsCode ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("���LPOI�R���e���c�R�[�h���擾�ł��Ȃ�") );
			continue;
		}
		rec.contentsCode = ( VT_I4 == varContentsCode.vt ) ? varContentsCode.lVal : 0L;

		// �r�����̑Ó��t���O���擾
		CComVariant varSuitable;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kSuitable, m_mapIndexesBldgNamePoint, varSuitable ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T( "�r�����̑Ó��t���O���擾�ł��Ȃ�" ) );
			continue;
		}
		rec.suitable = ( VT_I4 == varSuitable.vt ) ? CString( std::to_string( varSuitable.lVal ).c_str() ) : _T( "0" );

		// �Z���R�[�h��ݒ�
		if( !getAddrCode( rec.bldNamePointOID, rec.bldgOID, ipFeature, rec.addrcode ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("�Z���R�[�h���擾�ł��Ȃ�") );
			continue;
		}

		// ��\�_��ݒ�
		IPointPtr ipPoint;
		if( !getBldgRepPoint( rec.bldNamePointOID, rec.bldgOID, ipFeature, ipPoint ) ) continue;

		// ���W�l��2�����b�V��XY�ɕϊ�
		double dLon = 0.0, dLat = 0.0;
		if( FAILED( ipPoint->QueryCoords( &dLon, &dLat ) ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, rec.bldgOID, _T("�����|���S���̑�\�_���W���擾�ł��Ȃ�") );
			continue;
		}
		if( 0 != gCrdCnv.LLtoMesh( dLon, dLat, 2, &rec.meshcode, &rec.x, &rec.y, 1 ) )
		{
			outputErrLog(
					LogMgr::kError,
					sindy::schema::sj::buildingname_point::kTableName,
					rec.bldgOID,
					_T("�����|���S���̑�\�_���W��2�����b�V��XY�ɕϊ��ł��Ȃ�") );
			continue;
		}

		// �o�̓t�@�C���ɏo�͂���l���i�[
		std::vector<CString> valueList = boost::assign::list_of
			( m_strGrpCode )
			( uh::str_util::ToString(rec.bldgOID) )
			( uh::str_util::ToString(rec.meshcode) )
			( uh::str_util::ToString(rec.x) )
			( uh::str_util::ToString(rec.y) )
			( rec.addrcode.Mid(  0, 2 ) )
			( rec.addrcode.Mid(  2, 3 ) )
			( rec.addrcode.Mid(  5, 3 ) )
			( rec.addrcode.Mid(  8, 3 ) )
			( rec.addrcode.Mid( 11, 5 ) )
			( rec.addrcode.Mid( 16, 4 ) )
			( rec.name )
			( rec.getRemovedSepStrYomi() )
			( rec.nameSep )
			( rec.yomiSep )
			( rec.bldgClass1 )
			( rec.floors )
			( rec.getGrpCode() )
			( rec.getChainCode() )
			( rec.suitable )
			;

		// EUC�ŏo��
		if( !writer.writeRecord( valueList ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, rec.bldgOID, _T("���������[�X�f�[�^�t�@�C���ɏ������߂Ȃ�") );
			continue;
		}
	}

	// ���������[�X�f�[�^�t�@�C�������
	writer.close();

	std::cout << std::endl << "�����r�����̌��������[�X�f�[�^�o�͊���" << std::endl;

	return true;
}

bool BldNameTSVConverter::createMultiTranslateList()
{
	std::cout << "������|�󃊃X�g�o�͊J�n" << std::endl;

	// ���������[�X�f�[�^�t�@�C�����J���iUTF-8(BOM�t���j�j
	MultiTranslateListWriter writer( m_strOutDir, _tstol(m_maxRecNum), m_strGrpCode, m_addcolumnList );
	if( !writer.open() )
		return false;

	// �����r�����̃|�C���g������
	long lCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( !searchBldNamePoint( ipFeatureCursor, lCount ) )
		return false;

	std::cout << "������|�󃊃X�g�o�͑Ώۃf�[�^���� : " << std::setw( 7 ) << lCount << " ��" << std::endl;

	// �����r�����̃|�C���g�̃��[�v
	IFeaturePtr ipFeature;
	long i = 0;
	while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
	{
		++i;
		if( 0 == i % 100 || i == lCount )
		{
			std::cout << "������|�󃊃X�g�f�[�^�o�͒� [ " << std::setw( 7 ) << i << " / " << std::setw( 7 ) << lCount << " ]\r";
		}

		BldNamePointRec rec;

		// ���ʑ����l�擾
		if( !getCommonAttrValue(ipFeature, rec) ) 
			continue;

		// �o�̓t�@�C���ɏo�͂���l���i�[
		std::vector<CString> valueList = boost::assign::list_of
			( m_strGrpCode )
			( uh::str_util::ToString(rec.bldgOID) )
			( rec.name )
			( rec.getRemovedSepStrYomi() )
			( rec.getReplacedSepName() )
			( rec.getReplacedSepYomi() )
			;

		// ������|�󃊃X�g�ǉ��J�����ݒ�t�@�C���ɋL�ڂ��ꂽ�w�b�_�����A����ǉ�
		for( const auto& column : m_addcolumnList )
			valueList.push_back( _T("") );

		writer.writeRecord( valueList );
	}

	// ���������[�X�f�[�^�t�@�C�������
	writer.close();

	std::cout << std::endl << "������|�󃊃X�g�o�͊���" << std::endl;

	return true;
}

bool BldNameTSVConverter::getAddrCode( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, CString& strAddrCode )
{
	strAddrCode.Empty();

	// �����|���S��OID���L�[�ɏZ�����ԃe�[�u����茟��
	if( !getAddrCodeFromIntermediate( lBldgOID, strAddrCode )  ) return false;
	if( !strAddrCode.IsEmpty() ) return true;

	// ���̃|�C���g�̃W�I���g�����擾
	IGeometryPtr ipGeometry;
	if( FAILED( ipFeature->get_Shape( &ipGeometry ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("�W�I���g���̎擾�Ɏ��s") );
		return false;
	}

	// ���Y�|�C���g������s�s�n�}�s���E�|���S�����Z���R�[�h���擾
	if( !getAddrCodeFromAdmin( lOID, ipGeometry, strAddrCode ) ) return false;

	return true;
}

bool BldNameTSVConverter::getAddrCodeFromIntermediate( long lBldgOID, CString& strAddrCode )
{
	// �����|���S��OID���Z���R�[�h�Ή��\���쐬
	if( !createMapBldgOID2AddrCode( lBldgOID ) ) return false;

	// �Ή��\���������AOID������ΏZ���R�[�h��Ԃ�
	const auto itr = m_mapBldgOID2AddrCode.find( lBldgOID );
	if( m_mapBldgOID2AddrCode.end() != itr ) strAddrCode = itr->second;

	return true;
}

bool BldNameTSVConverter::createMapBldgOID2AddrCode( long lBldgOID )
{
	// �Ή��\��OID�͈�
	static long lOIDMin = -1, lOIDMax = -1;

	// �Ή��\��OID�͈͂ɓ��YOID���܂܂�Ă����炻�̂܂ܔ�����
	if( lOIDMin <= lBldgOID && lBldgOID <= lOIDMax ) return true;

	// ���YOID���܂�OID�͈͂�ݒ�
	// g_lBldgOIDRange = 500000 �̏ꍇ : [ 0, 499999 ] �� [ 500000, 999999 ] �� [ 1000000, 1499999 ] �� �c �Ɛ��ڂ���͂�
	lOIDMin = static_cast<long>( lBldgOID / g_lBldgOIDRange ) * g_lBldgOIDRange;
	lOIDMax = lOIDMin + g_lBldgOIDRange - 1;

	// �Ή��\�̃N���A
	m_mapBldgOID2AddrCode.clear();

	// �N�G���ݒ�
	// rep_flag �̗����Ă��� polygon_id �� gp_view �� cs_view �Ŕ��Ȃ����߁A�P����UNION�Ŗ��Ȃ�
	CString strSQL;
	strSQL.Format(
		_T( "SELECT polygon_id, addr_code FROM gp_view WHERE rep_flag = '1' AND polygon_id BETWEEN %ld AND %ld UNION " ) \
		_T( "SELECT polygon_id, addr_code FROM cs_view WHERE rep_flag = '1' AND polygon_id BETWEEN %ld AND %ld" ),
		lOIDMin, lOIDMax, lOIDMin, lOIDMax );

	try
	{
		// �N�G���̃I�[�v��
		ADODB::_RecordsetPtr pRecs( __uuidof( ADODB::Recordset ) );
		pRecs->PutRefActiveConnection( m_conAddrDB );
		if( FAILED( pRecs->Open( _bstr_t( strSQL ), vtMissing, ADODB::adOpenKeyset, ADODB::adLockReadOnly, ADODB::adCmdText ) ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T( "�����|���S��OID���Z���R�[�h�Ή��\�̐����Ɏ��s" ) );
			return false;
		}
		// ���R�[�h�Q��Ή��\�Ɋi�[
		while( !pRecs->adoEOF )
		{
			m_mapBldgOID2AddrCode.insert( std::make_pair( pRecs->Fields->GetItem( 0L )->Value.lVal, pRecs->Fields->GetItem( 1L )->Value.bstrVal ) );
			pRecs->MoveNext();
		}
	}
	catch( const _com_error& e )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T( "�����|���S��OID���Z���R�[�h�Ή��\�̐������ɗ�O���� : " ) + CString( e.ErrorMessage() ) );
		return false;
	}

	return true;
}

bool BldNameTSVConverter::getAddrCodeFromAdmin( long lOID, const IGeometryPtr& ipGeometry, CString& strAddrCode )
{
	// �Z���R�[�h�����
	strAddrCode.Empty();

	// ���Y�|�C���g������s�s�n�}�s���E�|���S������Ԍ���
	// �iOID�����Ō��ʂ��擾���A�����Ԃ����ꍇ�͍ŏ���1�t�B�[�`����I���j
	IFeatureCursorPtr ipFCurCityAdmin;
	CString strPostfix( _T("ORDER BY ") + CString( sindy::schema::city_admin::kObjectID ) );
	long lCount = 0;
	if( !common_proc::searchByGeometry( m_ipFCCityAdmin, ipFCurCityAdmin, lCount, ipGeometry, esriSpatialRelIntersects, strPostfix ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("�s���E�|���S���̋�Ԍ����Ɏ��s") );
		return false;
	}

	IFeaturePtr ipFCityAdmin;
	if( FAILED( ipFCurCityAdmin->NextFeature( &ipFCityAdmin ) ) || !ipFCityAdmin )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("�s���E�|���S���t�B�[�`���̎擾�Ɏ��s") );
		return false;
	}

	// �s�s�n�}�s���E�|���S����OID�擾
	long lOIDCityAdmin = 0;
	ipFCityAdmin->get_OID( &lOIDCityAdmin );

	// �s�s�n�}�s���E�|���S���̊e�푮���l�擾
	CComVariant varCityCode, varAddrCode, varGaikuFugo;
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kCityCode, m_mapIndexesCityAdmin, varCityCode ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("�s���E�|���S���t�B�[�`���̎s�撬���R�[�h�̎擾�Ɏ��s") );
		return false;
	}
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kAddrCode, m_mapIndexesCityAdmin, varAddrCode ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("�s���E�|���S���t�B�[�`���̏Z���R�[�h�̎擾�Ɏ��s") );
		return false;
	}
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kGaikuFugo, m_mapIndexesCityAdmin, varGaikuFugo ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("�s���E�|���S���t�B�[�`���̊X�敄���̎擾�Ɏ��s") );
		return false;
	}
	CString strCityCodeTmp( ( VT_BSTR == varCityCode.vt ) ? varCityCode.bstrVal : _T("") );
	CString strAddrCodeTmp( ( VT_BSTR == varAddrCode.vt ) ? varAddrCode.bstrVal : _T("") );
	long lGaikuFugo( ( VT_I4 == varGaikuFugo.vt ) ? varGaikuFugo.lVal : -1 );

	// �Z���R�[�h���s��
	if( strCityCodeTmp.IsEmpty() || _T("00000") == strCityCodeTmp || strAddrCodeTmp.IsEmpty() || _T("000000") == strAddrCodeTmp )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("���Y�|�C���g������s���E�|���S���̏Z���R�[�h���s���ł� : ") + strCityCodeTmp + strAddrCodeTmp );
		return false;
	}

	// �܂�5�����i�[
	strAddrCode = strCityCodeTmp;

	// �厚�E�ʏ́`���E���ڃR�[�h���u999999�v�łȂ��Ȃ�
	if( _T("999999") != strAddrCodeTmp )
	{
		// 11���܂Ŋi�[
		strAddrCode += strAddrCodeTmp;
		// �X�敄����5���܂ł̐��̐����Ȃ�16���܂Ŋi�[
		if( 0 < lGaikuFugo && lGaikuFugo < 100000 ) strAddrCode.AppendFormat( _T("%05ld"), lGaikuFugo );
	}

	return true;
}

bool BldNameTSVConverter::getBldgRepPoint( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, IPointPtr& ipPoint )
{
	// �����|���S���t�B�[�`�����擾
	IFeaturePtr ipFBuilding;
	if( FAILED( m_ipFCBuilding->GetFeature( lBldgOID, &ipFBuilding ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("�����|���S���t�B�[�`�����擾�ł��Ȃ� : ") + CString( std::to_string( lBldgOID ).c_str() ) );
		return false;
	}

	// �����|���S���`����擾
	IGeometryPtr ipGBuilding;
	if( FAILED( ipFBuilding->get_Shape( &ipGBuilding ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T("�����|���S���`����擾�ł��Ȃ�") );
		return false;
	}

	// ��\�|�C���g���擾
	if( !getRepPoint( ipGBuilding, ipPoint ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T("��\�|�C���g���擾�ł��Ȃ�") );
		return false;
	}

	return true;
}

bool BldNameTSVConverter::getRepPoint( const IPolygon4Ptr& ipPolygon, IPointPtr& ipPoint )
{
	// ��ԎQ�Ƃ̎擾
	ISpatialReferencePtr ipSpRef;
	if( FAILED( ipPolygon->get_SpatialReference( &ipSpRef ) ) ) return false;

	// �G�N�X�e���A�����O�̌����i1�������ǂ����j
	long lExteriorRings = 0;
	if( FAILED( ipPolygon->get_ExteriorRingCount( &lExteriorRings ) ) ) return false;
	if( 1 != lExteriorRings ) return false;

	// �G�N�X�e���A�����O�̎擾
	IRingPtr ipRingExterior;
	if( FAILED( ipPolygon->QueryExteriorRingsEx( 1, &ipRingExterior ) ) ) return false;

	// �C���e���A�����O���̎擾
	long lInteriorRings = 0;
	if( FAILED( ipPolygon->get_InteriorRingCount( ipRingExterior, &lInteriorRings ) ) ) return false;

	// �G�N�X�e���A�����O�\���_���̎擾
	long lExteriorPoints = 0;
	if( FAILED( ( (IPointCollectionPtr)ipRingExterior )->get_PointCount( &lExteriorPoints ) ) ) return false;

	// �i�C���e���A�����O�������Ȃ��j�O�p�`�̏ꍇ
	if( 0 == lInteriorRings && 4 == lExteriorPoints )
	{
		// �d�S���擾���ďI��
		if( FAILED( ( (IAreaPtr)ipRingExterior )->get_Centroid( &ipPoint ) ) ) return false;
		return true;
	}

	// �Ίp���Q�̎擾�i�����̍~���ŃR���e�i�Ɋi�[�j
	DiagonalLenMap mapDiagonals;

	// �����O�̃��[�v
	long lParts = 0;
	if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_GeometryCount( &lParts ) ) ) return false;
	for( long lRingIndex1 = 0; lRingIndex1 < lParts; ++lRingIndex1 )
	{
		// �����O�@�擾
		IGeometryPtr ipGRing1;
		if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_Geometry( lRingIndex1, &ipGRing1 ) ) ) return false;

		// �����O�@�̍\���_���擾
		long lPoints1 = 0;
		if( FAILED( ( (IPointCollectionPtr)ipGRing1 )->get_PointCount( &lPoints1 ) ) ) return false;

		// �G�N�X�e���A�����O�̏ꍇ�A�܂��͎������O���ł̑Ίp���𒊏o
		VARIANT_BOOL vbIsExterior = VARIANT_FALSE;
		if( FAILED( ( (IRingPtr)ipGRing1 )->get_IsExterior( &vbIsExterior ) ) ) return false;
		if( vbIsExterior )
		{
			// �n�_�̃��[�v
			for( long i = 0; i < lPoints1 - 2; ++i )
			{
				// �I�_�̃��[�v
				for( long j = i + 2; j < lPoints1 - 1; ++j )
				{
					// �ׂ荇���\���_���m�̏ꍇ�̓X�L�b�v
					if( 0 == i && lPoints1 - 2 == j ) continue;

					// �Ίp���̐����ƃR���e�i�ւ̒ǉ�
					if( !createDiagonals( ipPolygon, ipGRing1, i, ipGRing1, j, ipSpRef, mapDiagonals ) ) return false;
				}
			}
		}

		// �������O�Ƃ̑Ίp�������o
		for( long lRingIndex2 = lRingIndex1 + 1; lRingIndex2 < lParts; ++lRingIndex2 )
		{
			// �����O�A�擾
			IGeometryPtr ipGRing2;
			if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_Geometry( lRingIndex2, &ipGRing2 ) ) ) return false;

			// �����O�A�̍\���_���擾
			long lPoints2 = 0;
			if( FAILED( ( (IPointCollectionPtr)ipGRing2 )->get_PointCount( &lPoints2 ) ) ) return false;

			// �n�_�̃��[�v
			for( long i = 0; i < lPoints1 - 1; ++i )
			{
				// �I�_�̃��[�v
				for( long j = 0; j < lPoints2 - 1; ++j )
				{
					// �Ίp���̐����ƃR���e�i�ւ̒ǉ�
					if( !createDiagonals( ipPolygon, ipGRing1, i, ipGRing2, j, ipSpRef, mapDiagonals ) ) return false;
				}
			}
		}
	}

	// �Ίp���Q�i�[�R���e�i����̏ꍇ�ُ͈�I��
	if( 0 == mapDiagonals.size() ) return false;

	// �Ίp���̒����ق����珇�ɒ��_���擾�A���̓_���炠�鋗���i臒l�j���Ƀ|���S���̋��E�������݂��Ȃ���΂�����\�_�Ƃ��ĕԂ�
	// �i��\�_���|���S���̋��E�t�߂ɐݒ肳��邱�Ƃ�h���j
	IGeometryPtr ipGBoundary;	// �|���S�����E��
	if( FAILED( ( (ITopologicalOperatorPtr)ipPolygon )->get_Boundary( &ipGBoundary ) ) ) return false;
	double dArea = 0.0;			// �|���S���ʐ�
	if( FAILED( ( (IAreaPtr)ipPolygon )->get_Area( &dArea ) ) ) return false;
	double dThreshold = sqrt( dArea ) / 20.0;	// 臒l: �|���S���ʐς̕�������5%
	IPointPtr ipPointMid( CLSID_Point ), ipPointOnEdge( CLSID_Point );
	for( const auto& rec : mapDiagonals )
	{
		if( FAILED( rec.second->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPointMid ) ) ) return false;
		double dDistAlongEdge = 0.0, dDistFromEdge = 0.0;
		VARIANT_BOOL vbIsRight = VARIANT_FALSE;
		if( FAILED( ( ( (IPolylinePtr)ipGBoundary )->QueryPointAndDistance( esriNoExtension, ipPointMid, VARIANT_TRUE, ipPointOnEdge, &dDistAlongEdge, &dDistFromEdge, &vbIsRight ) ) ) ) return false;
		if( dDistFromEdge > dThreshold )
		{
			ipPoint = ipPointMid;
			return true;
		}
	}

	// �i�����܂łő�\�_������ł��Ȃ��ꍇ�j�Œ��̑Ίp����蒆�_��I��
	const auto itr = mapDiagonals.begin();
	if( mapDiagonals.end() == itr ) return false;
	ipPoint = IPointPtr( CLSID_Point );
	if( FAILED( (*itr).second->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPoint ) ) ) return false;

	return true;
}

bool BldNameTSVConverter::createDiagonals( const IPolygon4Ptr& ipPolygon, const IRingPtr& ipRing1, long lIndex1, const IRingPtr& ipRing2, long lIndex2, const ISpatialReferencePtr& ipSpRef, DiagonalLenMap& mapDiagonals )
{
	// �\���_���擾
	IPointPtr ipPt1, ipPt2;
	if( FAILED( ( (IPointCollectionPtr)ipRing1 )->get_Point( lIndex1, &ipPt1 ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipRing2 )->get_Point( lIndex2, &ipPt2 ) ) ) return false;

	// �Ίp������
	ILinePtr ipLine( CLSID_Line );
	if( FAILED( ipLine->putref_SpatialReference( ipSpRef ) ) ) return false;
	if( FAILED( ipLine->PutCoords( ipPt1, ipPt2 ) ) ) return false;

	// �Ίp���̒������擾
	double dLength = 0.0;
	if( FAILED( ipLine->get_Length( &dLength ) ) ) return false;

	// �|�����C���Ƃ��Đ����i���C���̂܂܂ł� IRelationalOperator �����p�ł��Ȃ��̂Łj
	IPolylinePtr ipPolyline( CLSID_Polyline );
	if( FAILED( ipPolyline->putref_SpatialReference( ipSpRef ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipPolyline )->AddPoint( ipPt1 ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipPolyline )->AddPoint( ipPt2 ) ) ) return false;

	// �|���S���Ƃ̋�ԓI�֌W���`�F�b�N
	VARIANT_BOOL vbContains;
	if( FAILED( ( (IRelationalOperatorPtr)ipPolygon )->Contains( ipPolyline, &vbContains ) ) ) return false;

	// �Ίp�����|���S���Ɋ܂܂�Ă���ꍇ
	if( vbContains )
	{
		// �Ίp�����R���e�i�ɓo�^
		mapDiagonals.insert( DiagonalLenMap::value_type( dLength, ipLine ) );
	}

	return true;
}
