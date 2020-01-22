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

// ChangeCodedValueDomain.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <arctl/coinitializer.h>

typedef std::map< long, CString > CODEDVALUEMAP; // �R�[�h�l�Ɛ����̃}�b�v

CString m_strServer; //!< �R�[�h�l�h���C���o�͂������T�[�o��
CString m_strOutputFile; //!< �R�[�h�l�h���C���o�͂���t�@�C����
CString m_strFolder; //!< �R�[�h�l�h���C����ύX�������t�@�C���������Ă���t�H���_�� 
CString m_strChangeDomainServer; //!< �R�[�h�l�h���C����ύX�������T�[�o��
CString m_strChangeCodedValueFile; //!< �ύX����R�[�h�l�h���C���̃t�@�C����
std::map< CString, CODEDVALUEMAP > m_mapChangedDomain; //!< �h���C�������L�[�Ƃ����R�[�h�l�h���C����ύX������e

namespace return_code {
	enum ECode {
		unknown	= -1,		//!< �������
		ok	= 0,			//!< ����I��
		tool_error = 1,		//!< �c�[�����o���G���[
		exception_error = 2,//!< ��O�G���[
		licence_error = 3,	//!< ���C�Z���X�F�؎��s
	};
}

// �t�H���_�̒��ɂ���PGDB�AFGDB�t�@�C���������Ă���
void FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName)
{
	do {
		if (lstrcmp(fd.cFileName, _T(".")) == 0 || lstrcmp(fd.cFileName, _T("..")) == 0) {
			// �������Ȃ�
		} else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			CString cstrPath;
			cstrPath.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString strExtention = cstrPath.Right( 4 );

			if (strExtention.CompareNoCase( _T(".gdb") ) == 0  )
			{
				listFileName.push_back(cstrPath);
			}
			else
			{
				CString cstrFind = cstrPath + CString(_T("\\*"));

				WIN32_FIND_DATA fd2;
				HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
				FindFile(hFind2, fd2, cstrPath, listFileName);
				FindClose(hFind2);
			}
		} else {
			CString cstrName;
			cstrName.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrExtension = PathFindExtension(cstrName);
			if (cstrExtension.CompareNoCase( _T(".mdb") ) == 0  )
				listFileName.push_back(cstrName);
		}

	} while (FindNextFile(hFind, &fd));
}

// �g�p���@�o�͊֐�
bool Usage()
{
	std::cerr << "�R�[�h�l�h���C���̐������o�́E�ϊ�����c�[��" << std::endl;
	std::cerr << "Usage  : ChangeCodedValueDomain"  << std::endl;
	std::cerr << "		-s <�R�[�h�l�h���C���o�͂������T�[�o���>"  << std::endl;
	std::cerr << "		-o <�R�[�h�l�h���C���o�͂���t�@�C����>"  << std::endl;
	std::cerr << "      -f <�R�[�h�l�h���C����ϊ��������t�@�C���������Ă���t�H���_��>" << std::endl;
	std::cerr << "      -d <�R�[�h�l�h���C����ϊ��������T�[�o��> " << std::endl;
	std::cerr << "      -c <�ϊ�����h���C���̑Ή��\> " << std::endl;
	return false;
}

// ������
int Init( int argc, _TCHAR* argv[] )
{
	if (argc <= 1)
		return Usage();

	//TODO ���̂�����boost::program_options�ł��ׂ�
	while (--argc)
	{
		_TCHAR *s = *++argv;
		switch (*s)
		{
		case '-':
			switch (*++s)
			{
			case 's':
				m_strServer = *++argv;
				--argc;
				break;
			case 'o':
				m_strOutputFile = *++argv;
				--argc;
				break;
			case 'f':
				m_strFolder = *++argv;
				--argc;
				break;
			case 'd':
				m_strChangeDomainServer = *++argv;
				--argc;
				break;
			case 'c':
				m_strChangeCodedValueFile = *++argv;
				--argc;
				break;
			default:
				return Usage();
			}
			break;
		default:
			break;
		}
	}

	if( !m_strServer.IsEmpty() )
	{
		if( m_strOutputFile.IsEmpty() )
		{
			std::cerr << "�R�[�h�l�h���C���o�̓��[�h�̏ꍇ�ɂ́A-o�I�v�V�����ŏo�̓t�@�C�������w�肵�Ă�������" << std::endl;
			return false;			
		}
	}

	if( !m_strFolder.IsEmpty() && !m_strChangeDomainServer.IsEmpty() )
	{
		std::cerr << "�R�[�h�l�h���C���ϊ����[�h�̏ꍇ�ɂ́A-f�A-d�̃I�v�V�����ǂ��炩���w�肵�Ă�������" << std::endl;
		return false;			
	}

	return true;
}

// �R�[�h�l��ϊ�����t�@�C���̓��e�ǂݎ��
bool ReadChangeDomainFile()
{
	std::ifstream file(m_strChangeCodedValueFile);
	if (! file.is_open())
	{
		std::cerr << "�t�@�C�����J���܂���ł���" << std::endl;
		return false;
	}

	while (file.good())
	{
		char szBuffer[2001];
		file.getline( szBuffer, 2000 );
		// �J���}��؂�ł��邱�ƑO��
		CString strLine = szBuffer;
		CString strDomainName = strLine.Left( strLine.Find( _T(",") ) );
		strLine = strLine.Right( strLine.GetLength() - strLine.Find( _T(",") ) -1 );
		CString strDomainValue = strLine.Left( strLine.Find( _T(",") ) );
		strLine = strLine.Right( strLine.GetLength() - strLine.Find( _T(",") ) -1 );
		CString strNewExpalin = strLine;

		if( strDomainName.IsEmpty() || strDomainValue.IsEmpty() || strNewExpalin.IsEmpty() )
			continue;

		CODEDVALUEMAP mapNewDomainMap = m_mapChangedDomain[ strDomainName ];
		mapNewDomainMap[ _ttol( strDomainValue ) ] = strNewExpalin;

		m_mapChangedDomain[ strDomainName ] = mapNewDomainMap;
	}
	return true;
}

// ����
int _tmain(int argc, _TCHAR* argv[])
{
	int retcode = return_code::unknown;

	try{
		const arctl::coinitializer aCoInitializer;
		::setlocale(LC_ALL, "Japanese");
		
		try
		{
			if( !Init( argc, argv ) )
				return return_code::tool_error;

			// �R�[�h�l���o�͂��郂�[�h
			if( !m_strServer.IsEmpty() )
			{
				// �R�[�h�l�h���C���̒l�����ׂďo�͂���ɂ́A���̃R�����g�A�E�g�O��
				// ���݂̃R�[�h�l�h���C���o�͂��郂�[�h�쐬���ׂ�����
				FILE *fStream = NULL;
				_wfopen_s( &fStream, m_strOutputFile, _T("a") );

				IWorkspacePtr ipWorkspace = sindy::create_workspace( m_strServer );
				IWorkspaceDomains3Ptr ipWorkspaceDomain = ipWorkspace;
				IEnumDomainPtr ipEnumDomain;
				ipWorkspaceDomain->get_Domains( &ipEnumDomain );
				IDomainPtr ipDomain;
				long l = 0;
				while( ipEnumDomain->Next( &ipDomain ) == S_OK )
				{
					CComBSTR bstrDomainName;
					ipDomain->get_Name( &bstrDomainName );
					std::map< long, CString > mapDomain = m_mapChangedDomain[ CString( bstrDomainName ) ];

					ICodedValueDomain2Ptr ipCodedValueDomain = ipDomain;
					if( ipCodedValueDomain )
					{
						long lCount = 0;
						ipCodedValueDomain->get_CodeCount( &lCount );

						for( int i = 0; i < lCount; ++i )
						{
							CComVariant vaValue;
							CComBSTR bstrName;
							ipCodedValueDomain->get_Name( i, &bstrName );
							ipCodedValueDomain->get_Value( i, &vaValue );
							CString strValue;
							strValue.Format( _T("%s,%ld,%s\n"), bstrDomainName, vaValue.lVal, bstrName );
							fwprintf( fStream, strValue );

						}
					}
				}
				fclose( fStream );
			}
			// �R�[�h�l��ϊ����郂�[�h
			else
			{
				// �ύX����R�[�h�l�h���C���t�@�C���ݒ�̓ǂݍ���
				if( !ReadChangeDomainFile() )
					return return_code::tool_error;

				std::list< CString > listChangeServerName;
				// PGDB�Ƃ�FGDB�Ƃ��t�H���_���̂��̂��ꊇ�ł��ꍇ
				if( m_strChangeDomainServer.IsEmpty() )
				{
 					// �t�H���_���̃t�@�C����������
					WIN32_FIND_DATA		ffd;
					CString				strDirPath;
					strDirPath = m_strFolder + CString( _T("\\*") );

					HANDLE hFile = FindFirstFile( strDirPath, &ffd );
					if ( hFile == INVALID_HANDLE_VALUE )
						return return_code::tool_error;	// �t�@�C�������݂��Ȃ��H
					// obj�t�@�C�������Ă���
					FindFile( hFile, ffd, m_strFolder, listChangeServerName );
					FindClose(hFile);
				}
				// �T�[�o�̏ꍇ�̓T�[�o�ւ̐ڑ������X�g�ɓ˂�����ł���
				else
					listChangeServerName.push_back( m_strChangeDomainServer );

				for( std::list< CString >::const_iterator it = listChangeServerName.begin(); it != listChangeServerName.end(); ++it )
				{
					// ���[�N�X�y�[�X�擾���āA�R�[�h�l�h���C�����ׂĎ擾
					IWorkspacePtr ipWorkspace = sindy::create_workspace( *it );
					IWorkspaceDomains3Ptr ipWorkspaceDomain = ipWorkspace;
					IEnumDomainPtr ipEnumDomain;
					ipWorkspaceDomain->get_Domains( &ipEnumDomain );
					IDomainPtr ipDomain;
					while( ipEnumDomain->Next( &ipDomain ) == S_OK )
					{
						CComBSTR bstrDomainName;
						ipDomain->get_Name( &bstrDomainName );
						// �ϊ��Ώۂ̃R�[�h�l�h���C�����擾����
						std::map< CString, CODEDVALUEMAP >::const_iterator mapDomain = m_mapChangedDomain.find( CString( bstrDomainName ) );
						if( mapDomain == m_mapChangedDomain.end() )
							continue;

						ICodedValueDomain2Ptr ipCodedValueDomain = ipDomain;
						if( ipCodedValueDomain )
						{
							// �R�[�h�l����������폜
							// �t�@�C���Ŏw�肵�Ă�����̂����Ȃ̂ŁA�w�肵�Ă��Ȃ����̂ɑ΂��Ă͕ϊ��͂�����Ȃ�
							for( CODEDVALUEMAP::const_iterator it = mapDomain->second.begin(); it != mapDomain->second.end(); ++it )
							{
								HRESULT hr = ipCodedValueDomain->DeleteCode( _variant_t( it->first )  );
							}
							// �t�@�C���Ŏw�肵���R�[�h�l��ǉ�����
							for( CODEDVALUEMAP::const_iterator it = mapDomain->second.begin(); it != mapDomain->second.end(); ++it )
							{
								HRESULT hr = ipCodedValueDomain->AddCode( _variant_t( it->first ), _bstr_t( it->second ) );
							}

							// �����Ń\�[�g���Ă���
							ipCodedValueDomain->SortByValue( VARIANT_FALSE );
							// �R�[�h�l�X�V
							HRESULT hr = ipWorkspaceDomain->AlterDomain( (IDomainPtr)ipCodedValueDomain  );
						}
					}
				}
			}
			retcode = return_code::ok;

		}
		catch (const _com_error& com_err)
		{
			// �c�[����O�G���[
			std::cerr << CT2A( com_err.ErrorMessage() ) << std::endl;
			retcode = return_code::exception_error;
		}
	}
	catch(const std::exception &e)
	{
		// ���C�Z���X�F�؎��s
		std::cerr << e.what() << std::endl;
		retcode = return_code::licence_error;
	}

	if( retcode == return_code::ok )
		std::cerr << "����I��"  << std::endl;
	else
		std::cerr << "�ُ�I��"  << std::endl;

	return retcode;
}

