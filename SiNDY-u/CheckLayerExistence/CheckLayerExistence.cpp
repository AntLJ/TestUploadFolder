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


#include <stdio.h>

#include "stdafx.h"
#include "CheckLayerExistence.h"
#include <iterator>

using namespace std;


CCheckLayerExistence::CCheckLayerExistence()
{
	m_MeshSuccessFlag = 0;
	m_ModeFlag = 0;	
	m_ExeModeFlag = 0;
}

CCheckLayerExistence::~CCheckLayerExistence()
{

}

bool CCheckLayerExistence::init(int argc, char *argv[])
{
	if( !checkArg(argc, argv) )					// �������
		return false;

	// ���O�t�@�C��
	if( !m_cError.open(argv[argc-1]) )
	{
		cerr << "#Error ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[argc-1] << endl;
		return false;
	}

	CString strVersion;
	strVersion.Format( _T("%s %s"), PROGNAME, TOOL_VERSION );
	m_cError.printHeader( strVersion, m_FileProp );

	return true;
}



void CCheckLayerExistence::printUsage()
{
	cerr
		<< "�yPGDB�����݃��C���`�F�b�N�c�[��" << TOOL_VERSION << "�z\n"
		<< "(Usage)CheckLayerExistence.exe [option] [logfile]\n"
		<< "�E�`�F�b�N�ΏۃI�v�V����\n"
		<< " -m [meshcode]		���b�V���R�[�h�w��\n"
		<< " -l [meshlist]		���b�V�����X�g�w��\n"
		<< " -n [filenamelist]	�t�@�C�������X�g�w��\n"
		<< " -s [layername]		���݃`�F�b�N���郌�C�����w��\n"
		<< " -f [filename]		���݃`�F�b�N���郌�C�����X�g���w��\n"
		<< " -c []			���C�����́��~�݂̂̏o�̓��[�h�w��\n"
		<< " -d []			���݂��Ȃ��������C�����o�̓��[�h�w��\n"
		<< "\n"
		<< "�E���ϐ��ɂ���\n"
		<< " DB_ANY			PGDB�t�@�C���i�[�t�H���_�̃p�X\n"
		<< endl;
}

void CCheckLayerExistence::execute()
{
	if( m_ExeModeFlag == 2 )
		Mode_Meshcode();

	else if( m_ExeModeFlag == 3 )
		Mode_Filename();

}



void CCheckLayerExistence::Mode_Meshcode()
{
	CString ErrorMsg;
	for( vector<string>::iterator itr=m_vecMesh.begin(); itr!=m_vecMesh.end() ; ++itr )
	{
		m_FileName = (*itr).c_str();						// string�^�̃��b�V���R�[�h��CString�^�ɕϊ�
		unsigned long meshnum = strtoul(m_FileName, NULL, 10);			// ���b�V���ԍ���unsigned long�^�ɕϊ��������̂�ۊ�
		m_FileName += ".mdb";								// [���b�V���ԍ�].mdb

		// ���b�V���R�[�h����n�}�̎�ނ𔻕ʂ���
		CString strMeshFName;
		if( meshnum > 1000 && meshnum < 10000 )
		{
			strMeshFName = _T("MIDDLEMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName;		// PGDB�t�@�C���̐�΃p�X�쐬
		}
		else if( meshnum > 100000 && meshnum < 1000000 )
		{
			strMeshFName = _T("BASEMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(4) + "\\" + m_FileName;	// PGDB�t�@�C���̐�΃p�X�쐬
		}
		else if( meshnum > 10000000 && meshnum < 100000000 )
		{
			strMeshFName = _T("CITYMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(4) + "\\" + m_FileName;	// PGDB�t�@�C���̐�΃p�X�쐬
		}
		else if( meshnum > 999999999 && meshnum < 4000000000 )
		{
			//���ۓs�s�n�}�Ή�
			strMeshFName = _T("TERTIARYMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(6) + "\\" + m_FileName.Left(8) + "\\" + m_FileName;	// PGDB�t�@�C���̐�΃p�X�쐬
		}
		else
		{
			ErrorMsg = "���b�V���R�[�h���s���ł��I";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FileName, ErrorMsg);
			continue;
		}

		// �t�@�C��������K�v�����擾
		if( !setInfoFromPGDB() )
		{
			ErrorMsg = "PGDB�t�@�C�������݂��Ȃ����A�A�h���X���Ԉ���Ă��܂��I";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FilePass, ErrorMsg);
			continue;
		}
		
		// ���ۂ̃��C���L���`�F�b�N����
		LayerCheck();
	}
}

void CCheckLayerExistence::Mode_Filename()
{
	CString ErrorMsg;
	for( vector<string>::iterator itr=m_vecMesh.begin(); itr!=m_vecMesh.end() ; ++itr )
	{
		m_FileName = (*itr).c_str();						// string�^�̃��b�V���R�[�h��CString�^�ɕϊ�

		m_FilePass = m_FileProp + "\\" + m_FileName;

		// �t�@�C��������K�v�����擾
		if( !setInfoFromPGDB() )
		{
			ErrorMsg = "PGDB�t�@�C�������݂��Ȃ����A�A�h���X���Ԉ���Ă��܂��I";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FilePass, ErrorMsg);
			continue;
		}
		
		// ���ۂ̃��C���L���`�F�b�N����
		LayerCheck();
	}
}


bool CCheckLayerExistence::checkArg(int argc, char *argv[])
{
	for( int i=1 ; i<argc ; i++ )
	{
		if( strcmp(argv[i], "-m") == 0 )
		{
			m_ExeModeFlag += 2;
			string temp( argv[++i] );
			m_vecMesh.push_back( temp );
		}
		if( strcmp(argv[i], "-l") == 0 )
		{
			m_ExeModeFlag += 2;
			if( !loadMeshlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-n") == 0 )
		{
			m_ExeModeFlag += 3;
			if( !loadMeshlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-s") == 0 )
		{
			string temp( argv[++i] );
			m_vecLayerName.push_back( temp );
		}
		if( strcmp(argv[i], "-f") == 0 )
		{
			if( !loadLayerlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-c") == 0 )
		{
			m_ModeFlag = 1;
		}
		if( strcmp(argv[i], "-d") == 0 )
		{
			m_ModeFlag = 2;
		}

	}
	if( m_vecMesh.empty() )
	{
		cerr << "PGDB���X�g��t�@�C�����w�肳��Ă��܂���" << endl;
		return false;
	}
	if( m_vecLayerName.empty() )
	{
		cerr << "�T�����C�����w�肳��Ă��܂���" << endl;
		return false;
	}
	if( m_ModeFlag != 1 && m_ModeFlag != 2 )
	{
		cerr << "�o�̓��[�h���w�肳��Ă��܂���" << endl;
		return false;
	}
	if( m_ExeModeFlag == 0 )
	{
		cerr << "���s���[�h���w�肳��Ă��܂���" << endl;
		return false;
	}
	if( m_ExeModeFlag != 2 && m_ExeModeFlag != 3 )
	{
		cerr << "���s���[�h�w�肪�s���ł�" << endl;
		return false;
	}

	return true;
}

bool CCheckLayerExistence::loadMeshlist(LPCTSTR lpszFile)
{
	m_MeshlistName = lpszFile;
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	
	// string�^�Ń��b�V���R�[�h���i�[
	copy(
		std::istream_iterator<string>(ifs),
		std::istream_iterator<string>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

bool CCheckLayerExistence::loadLayerlist(LPCTSTR lpszFile)
{

	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	
	copy(
		std::istream_iterator<string>(ifs),
		std::istream_iterator<string>(),
		std::back_inserter(m_vecLayerName)
		);
	
	return ifs.eof();

}

bool CCheckLayerExistence::getEnv()
{

#ifdef _DEBUG
	_tputenv( _T("DB_ANY=C:\\svn\\SiNDY-u\\CheckLayerExistence\\test") );
	
#endif

	CString strError(_T(""));
	if(!m_FileProp.GetEnvironmentVariable( _T("DB_ANY")) )
		strError += _T("#Error ���ϐ� DB_ANY �̎擾�Ɏ��s\n");
	
	if( !strError.IsEmpty() )
	{
		std::cerr << strError;
		return false;
	}
	else
		return true;
}

bool CCheckLayerExistence::setInfoFromPGDB()
{
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	USES_CONVERSION;

	// �ڑ���̕\��
	CComVariant vaPGDB( m_FilePass );
	cerr << "\t�ڑ�PGDB �F " << OLE2T( vaPGDB.bstrVal ) << endl;

	// �ڑ��m�F
	cerr << "\t�ڑ����E�E�E";
	if( ipWorkspaceFactory->OpenFromFile( vaPGDB.bstrVal, NULL, &m_ipPGDBWorkspace ) != S_OK){
		cerr << "�ڑ��ł��܂���ł���" << endl;
		return false;
	}
	cerr << "�ڑ����܂���" << endl;


	return true;
}

void CCheckLayerExistence::LayerCheck()
{
	for(vector<string>::iterator itr = m_vecLayerName.begin() ; itr != m_vecLayerName.end() ; ++itr)
	{
		// ���[�U�̃��C�����啶���ɕϊ�����

		// ���ۂɃ��C���I�[�v�������݂邱�ƂŁA���C�������݂��邩���Ȃ����𔻕ʂ���
		CString temp = (*itr).c_str();
		IFeatureWorkspacePtr(m_ipPGDBWorkspace)->OpenFeatureClass( CComBSTR(temp), &m_PGDBFC );  
		if( m_PGDBFC == NULL )
		{
			if(m_ModeFlag == 2)
				m_cError.printLog( m_FileName, *itr );
			if(m_ModeFlag == 1)
			{
				m_MeshSuccessFlag =1;
				break;
			}
		}
	}
	if(m_ModeFlag == 1)
		m_cError.printLog( m_FileName, m_MeshSuccessFlag );

	m_MeshSuccessFlag = 0;
}


