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
#include "CreateIllustNameTable.h"
#include "common.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/Workspace.h>
#include <sindy/schema/illust.h>
#include <TDC/illustlib/IllustLinkTable.h>
#include <TDC/illustlib/IllustNameCreator.h>
#include <TDC/illustlib/IllustOutputInfo.h>

using namespace sindy;
using namespace schema;
namespace po = boost::program_options;

CreateIllustNameTable::CreateIllustNameTable() : m_nErrCounter( 0 )
{
}

CreateIllustNameTable::~CreateIllustNameTable()
{
}

bool CreateIllustNameTable::init( int argc, _TCHAR* argv[] )
{
	// �R�}���h�����̃`�F�b�N
	if( !checkArg( argc, argv ) ) return false;

	// �C���X�g�t�@�C�����Ή��\�t�@�C���̃I�[�v��
	if( !m_cIllustNameTable.open( m_strOutFile, m_varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�C���X�g�t�@�C�����Ή��\�t�@�C�����I�[�v���ł��܂��� : ") + m_strOutFile );
		return false;
	}

	// DB�̃I�[�v��
	IWorkspacePtr ipWorkspace;
	if( !common_proc::getWorkspace( m_strDBConnect, ipWorkspace ) )
	{
		LogMgr::getInstance().writeRunLog( _T("DB�ɐڑ��ł��܂��� : ") + m_strDBConnect );
		return false;
	}

	// ILLUST_LINK �t�B�[�`���N���X�̎擾
	ITablePtr ipIllustLinkTable;
	if( !common_proc::getTable( illust_link::kTableName, ipWorkspace, ipIllustLinkTable ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�t�B�[�`���N���X���I�[�v���ł��܂��� : ") + CString( illust_link::kTableName ) );
		return false;
	}
	// �e�[�u���t�@�C���_�ɒǉ�
	m_ipTableFinder.InitCollection( ipIllustLinkTable, CModel() );

	// �������ݒ�t�@�C���̓ǂݍ���
	if( !m_cIllustSettings.load( m_strIniFile, ipIllustLinkTable ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�������ݒ�t�@�C���𐳏�ɓǂݍ��߂܂��� : ") + m_strIniFile );
		return false;
	}

	return true;
}

bool CreateIllustNameTable::execute()
{
	// �������ݒ�t�@�C�����R�[�h�̃��[�v
	for( auto rec : m_cIllustSettings )
	{
		const auto& table = CAST_ILLUSTLINKTABLE( m_ipTableFinder.FindTable( illust_link::kTableName ) );
		if( !table ) return false;

		// ILLUST_LINK ����C���X�g��ʃR�[�h�{�d����R�[�h����v���郌�R�[�h�Q���擾
		CString strWhereClause;
		strWhereClause.Format( _T("%s = %d and %s = %d"), illust_link::kIllustClass, rec.first.first, illust_link::kCustomerCode, rec.first.second );
		table->_Select( AheInitQueryFilter( nullptr, nullptr, strWhereClause ), true );
		table->CreateCache();

		// �C���X�g�t�@�C���������N���X�̏�����
		CIllustNameCreator cCreator( rec.second.m_strPrefix, _T(""), rec.second.m_vecNamingRule );

		int nIllustLinks = 0;				// �C���X�g�����N���J�E���g�p
		std::set<CString> setIllustFiles;	// �C���X�g�t�@�C�����J�E���g�p

		// ILLUST_LINK ���R�[�h�̃��[�v
		for( const auto& row : *table )
		{
			const auto& illustRow = CAST_ILLUSTLINKROW( row );

			// �o�͏��O�����ɍ��v����ꍇ�̓X�L�b�v
			if( CIllustOutputInfo::IsException( illustRow, rec.second.m_cOutputInfo ) )
				continue;

			++nIllustLinks;

			// �W���o�͂ɐi�����o��
			std::cout << "[" << rec.first.first << "] [" << rec.first.second << "] : " << std::setw(5) << nIllustLinks << " / " << std::setw(5) << table->size() << "\r";

			// �C���X�g�t�@�C�����i�����p�j�擾
			CString strFileName = cCreator.GetIllustName( illustRow );
			if( strFileName.IsEmpty() )
			{
				outputErrLog( illustRow, LogMgr::EErrLevel::kError, _T(""), _T("�C���X�g�t�@�C�������擾�ł��܂���") );
				continue;
			}
			setIllustFiles.insert( strFileName );

			// �C���X�g�t�@�C���̑��݃`�F�b�N
			CString strFilePath = rec.second.m_strIllustDir + _T("\\") + strFileName + rec.second.m_strExtension;
			if( !checkFileExistence( strFilePath ) )
			{
				outputErrLog( illustRow, LogMgr::EErrLevel::kError, _T(""), _T("�C���X�g�t�@�C�������݂��܂���"), strFilePath );
				continue;
			}

			// �C���X�g�t�@�C�����Ή��\�Ƀ��R�[�h��ǉ�
			m_cIllustNameTable.insert( IllustNameRec( rec.first.first, rec.first.second, illustRow->GetOID(), strFileName ) );
		}

		// �L���b�V�����N���A
		table->DeleteCache();
		table->clear();

		// ���s���O�ɏ��������o��
		CString strLog;
		strLog.Format( _T("[%d] [%d] : %5d link(s), %5d file(s)"), rec.first.first, rec.first.second, nIllustLinks, setIllustFiles.size() );
		LogMgr::getInstance().writeRunLog( strLog );
	}

	// �C���X�g�t�@�C�����Ή��\�t�@�C�����o��
	m_cIllustNameTable.write();

	// �G���[������ꍇ�� false ��߂�
	return ( 0 == m_nErrCounter );
}

bool CreateIllustNameTable::checkArg( int argc, _TCHAR* argv[] )
{
	po::options_description option("�I�v�V����");

	option.add_options()
		( "db,d",				po::value<std::string>(),	"DB�ڑ�������" )
		( "illust_settings,i",	po::value<std::string>(),	"�������ݒ�t�@�C�����i���́j" )
		( "output_list,o",		po::value<std::string>(),	"�C���X�g�t�@�C�����Ή��\�t�@�C�����i�o�́j" )
		( "runlog,r",			po::value<std::string>(),	"���s���O�t�@�C����" )
		( "errlog,e",			po::value<std::string>(),	"�G���[���O�t�@�C����" )
		( "help,?",											"�w���v��\��" );
	po::store( po::parse_command_line( argc, argv, option ), m_varMap );
	po::notify( m_varMap );

	// �w���v�\��
	if( m_varMap.count( "help" ) || 0 == m_varMap.size() )
	{
		std::cout << option << std::endl;
		return false;
	}

	// ���s���O�t�@�C����
	if( !getArgParam( "runlog", m_strRunLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("���s���O�t�@�C�������w�肳��Ă��܂���") );
		return false;
	}
	if( !LogMgr::getInstance().setRunLog( m_strRunLog, &m_varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("���s���O�t�@�C�����I�[�v���ł��܂��� : ") + m_strRunLog );
		return false;
	}

	// �G���[���O�t�@�C����
	if( !getArgParam( "errlog", m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�G���[���O�t�@�C�������w�肳��Ă��܂���") );
		return false;
	}
	if( !LogMgr::getInstance().setErrLog( m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�G���[���O�t�@�C�����I�[�v���ł��܂��� : ") + m_strErrLog );
		return false;
	}

	// DB�ڑ�������
	if( !getArgParam( "db", m_strDBConnect ) )
	{
		LogMgr::getInstance().writeRunLog( _T("DB�ڑ������񂪎w�肳��Ă��܂���") );
		return false;
	}

	// �C���X�g�t�@�C�����Ή��\�t�@�C����
	if( !getArgParam( "output_list", m_strOutFile ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�C���X�g�t�@�C�����Ή��\�t�@�C�������w�肳��Ă��܂���") );
		return false;
	}

	// �������ݒ�t�@�C����
	if( !getArgParam( "illust_settings", m_strIniFile ) )
	{
		LogMgr::getInstance().writeRunLog( _T("�������ݒ�t�@�C�������w�肳��Ă��܂���") );
		return false;
	}

	return true;
}

bool CreateIllustNameTable::getArgParam( const std::string& strName, CString& strParam )
{
	if( 0 == m_varMap.count( strName ) ) return false;
	strParam = CString( m_varMap[strName].as<std::string>().c_str() );
	if( strParam.IsEmpty() ) return false;
	return true;
}

bool CreateIllustNameTable::checkFileExistence( const CString& strFileName )
{
	std::ifstream ifs( strFileName, std::ios::binary | std::ios::in );
	if( !ifs.is_open() ) return false;
	ifs.close();
	return true;
}

void CreateIllustNameTable::outputErrLog( const sindy::CSPIllustLinkRow& illustLinkRow, LogMgr::EErrLevel errLv, const CString& strErrCode, const CString& strErrMsg, const CString& strOption /* = _T("") */ )
{
	// �ŏ���1���ڂ̏ꍇ�A���O�^�C�v���o��
	if( 0 == m_nErrCounter )
	{
		LogMgr::getInstance().writeErrLog( _T("# SINDYSTDLOG") );
	}
	++m_nErrCounter;

	// �W�����O�t�H�[�}�b�g�ŏo��
	CString strOut;
	strOut.Format( _T("0\t%s\t%ld\t\t\t%s\t%s\t%s\t%s"), sindy::schema::illust_link::kTableName, illustLinkRow->GetOID(), LogMgr::getErrLvString( errLv ), strErrCode, strErrMsg, strOption );
	LogMgr::getInstance().writeErrLog( strOut );
}
