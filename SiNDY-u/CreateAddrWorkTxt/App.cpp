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

#include <SiNDYLib/Workspace.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <WinLib/VersionInfo.h>

#include "CS2WorkTxt.h"
#include "GP2WorkTxt.h"
#include "KP2WorkTxt.h"

/**
 * @brief	���݂̎�����Ԃ�
 * @return	����������(yyyy/mm/dd hh/mm/ss)
 */
inline CString GetCurTime()
{
	SYSTEMTIME	st;
	GetLocalTime( &st );
	CString strTime;
	strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
}

// �g�p�@
void CApp::Usage()
{
	std::cerr << "[�Z�����ԃf�[�^�쐬�c�[��]\n" 
			  << "(Usage)CreateAddrWorkTxt.exe [�쐬���[�h�I�v�V����] [�e��I�v�V����]\n"
			  << "���쐬���[�h�I�v�V���� ... �����ꂩ�K�{�w��\n"
			  << " -gpw ... ���|�C���g���ԃf�[�^�쐬���[�h\n"
			  << " -kpw ... �ƌ`�����N�|�C���g���ԃf�[�^�쐬���[�h\n"
			  << " -csw ... CS�|�C���g���ԃf�[�^�쐬���[�h\n"
			  << "���e��I�v�V����\n"
			  << " -out ... �o�͐�(�K�{) ���E�ƌ`�����N->�o�̓f�B���N�g�� CS->�o�̓t�@�C����\n"
			  << " -log ... ���O�t�@�C����(�K�{)\n"
			  << " -jdb ... �Z��DB�ڑ���(�ƌ`�����N���̂ݕK�{)\n"
			  << " �E���s�G���A�w��I�v�V����(���w��́A�S��)\n"
			  << "     -cl <�s�撬���R�[�h���X�g>\n"
			  << "     -cs <�s�撬���R�[�h>\n"
			  << "     -cr <�J�n�s�撬���R�[�h>-<�I���s�撬���R�[�h>\n"
			  << "�����ϐ��ɂ���(�S�ĕK�{)\n"
			  << " DB_ADDRESS      ... �Z���nSDE�ڑ��v���p�e�B\n"
			  << " FC_TARGET_POINT ... �^�[�Q�b�g�Z���|�C���g�e�[�u����(�� or �ƌ`�����N or CS)\n"
			  << " FC_CITY_ADMIN   ... �s�s�n�}�s���E�e�[�u����\n"
			  << std::endl;
}

// ������
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	if( argc < 5 )
	{
		std::cerr << "#Error �����̐����s���ł�" << std::endl;
		return false;
	}

	// �쐬���[�h�m�F
	if( _tcscmp(argv[1], _T("-gpw")) == 0 )		m_emMode = kGPMode;
	else if( _tcscmp(argv[1], _T("-kpw")) == 0 )	m_emMode = kKPMode;
	else if( _tcscmp(argv[1], _T("-csw")) == 0 )	m_emMode = kCSMode;
	else
	{
		std::cerr << "#Error �s���ȍ쐬���[�h�I�v�V���� : " << CT2CA(argv[1]) << std::endl;
		return false;
	}

	// �������
	for( int i = 2; i < argc - 1; ++i )
	{
		if( _tcscmp(argv[i], _T("-out")) == 0 )
		{
			boost::filesystem::wpath out( argv[++i] );
			switch( m_emMode )
			{
			case kGPMode:
			case kKPMode:
				if( !boost::filesystem::is_directory(out) )
				{
					std::cerr << "#Error ���E�ƌ`�����N�̏ꍇ�́A�o�͐�y�f�B���N�g���z���w�肵�ĉ����� : " << CT2CA(argv[i]) << std::endl;
					return false;
				}
				m_cParam.m_strOutDir = out.string().c_str();
				break;
			case kCSMode:
				if( boost::filesystem::exists(out) && boost::filesystem::is_directory(out) )
				{
					std::cerr << "#Error CS�̏ꍇ�́A�o�́y�t�@�C���z���w�肵�ĉ����� : " << CT2CA(argv[i]) << std::endl;
					return false;
				}
				m_cParam.m_strOutFile = out.string().c_str();
				break;
			default:
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-log")) == 0 )
		{			
			m_ofs.open( static_cast<CT2CW>(argv[++i]) );
			if( !m_ofs.is_open() )
			{
				std::cerr << "#Error ���O�t�@�C���̃I�[�v���Ɏ��s : " << CT2CA(argv[i]) << std::endl;
				return false;
			}
			m_ofs.imbue( std::locale(std::locale("Japanese"), "C", std::locale::numeric) );			
		}
		else if( _tcscmp(argv[i], _T("-cl")) == 0 || _tcscmp(argv[i], _T("-cr")) == 0 || _tcscmp(argv[i], _T("-cs")) == 0 )
		{
			m_cParam.m_pairArea.first = argv[i];
			m_cParam.m_pairArea.second = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-jdb")) == 0 )
		{
			m_cParam.m_strJDBProp = argv[++i];
		}
	}

	// �o�͐�w��`�F�b�N
	if( m_cParam.m_strOutDir.IsEmpty() && m_cParam.m_strOutFile.IsEmpty() )
	{
		std::cerr << "#Error �f�[�^�o�͐�(-out)�����w��" << std::endl;
		return false;
	}

	// ���O�t�@�C���w��`�F�b�N
	if( !m_ofs.is_open() )
	{
		std::cerr << "#Error ���O�t�@�C��(-log)�����w��" << std::endl;
		return false;
	}

	// �Z��DB�w��`�F�b�N(�ƌ`�����N��)
	if( kKPMode == m_emMode && m_cParam.m_strJDBProp.IsEmpty() )
	{
		std::cerr << "#Error �Z��DB�ڑ���(-jdb)�����w��" << std::endl;
		return false;
	}

	// SDE�ڑ�
	CString strEnv;
	if( !strEnv.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		std::cerr << "#Error ���ϐ� DB_ADDRESS ���ݒ肳��Ă��Ȃ�" << std::endl;
		return false;
	}
	
	CWorkspace cWorkspace;
	if( !cWorkspace.Connect(strEnv) )
	{
		std::cerr << "#Error SDE�ڑ��Ɏ��s : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	// �^�[�Q�b�g�Z���|�C���g�e�[�u���I�[�v��
	if( !strEnv.GetEnvironmentVariable(_T("FC_TARGET_POINT")) )
	{
		std::cerr << "#Error ���ϐ� FC_TARGET_POINT ���ݒ肳��Ă��Ȃ�" << std::endl;
		return false;
	}

	m_cParam.m_cPointFC.SetObject( cWorkspace.OpenTable(strEnv) );
	if( !m_cParam.m_cPointFC )
	{
		std::cerr << "#Error �Z���|�C���g�t�B�[�`���N���X�̃I�[�v�����s : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	// �s�s�n�}�s���E�t�B�[�`���N���X�I�[�v��
	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
	{
		std::cerr << "#Error ���ϐ� FC_CITY_ADMIN ���ݒ肳��Ă��Ȃ�" << std::endl;
		return false;
	}

	m_cParam.m_cCityAdminFC.SetObject( cWorkspace.OpenTable(strEnv) );
	if( !m_cParam.m_cCityAdminFC )
	{
		std::cerr << "#Error �s�s�n�}�s���E�t�B�[�`���N���X�̃I�[�v�����s : " << CT2CA(strEnv) << std::endl;
		return false;
	}

	return true;
}

// ���s
bool CApp::Run()
{
	// ���O�w�b�_�[�o��
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#SINDYSTDLOG\n#%s FILEVERSION %s PRODUCTVERSION %s\n#�J�n���� %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), GetCurTime() );
	m_ofs << CT2CA(strHeader) << std::endl;

	// ���ԃf�[�^�����C���X�^���X�擾
	std::tr1::shared_ptr<CWorkTxtImpl> pWorkTxt;
	switch( m_emMode )
	{
	case kCSMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CCS2WorkTxt(m_cParam, m_ofs));	break;
	case kGPMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CGP2WorkTxt(m_cParam, m_ofs));	break;
	case kKPMode:	pWorkTxt = std::tr1::shared_ptr<CWorkTxtImpl>(new CKP2WorkTxt(m_cParam, m_ofs));	break;
	default:
		return false;
	}

	// ������
	if( pWorkTxt->Init() )
	{
		// �o�͊J�n
		if( !pWorkTxt->WriteData() )
		{
			std::cerr << "#Error �f�[�^�o�͒��ɃG���[���������܂���" << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "#Error �o�̓N���X�̏������Ɏ��s" << std::endl;
		return false;
	}


	// �I������
	m_ofs << "#�I������ : " << CT2CA(GetCurTime()) << std::endl;

	return true;
}
