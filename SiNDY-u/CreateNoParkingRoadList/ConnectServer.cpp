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
#include "ConnectServer.h"
#include "MakeList.h"

extern "C"{
#include <wkf.h>
}

using namespace sindy;
using namespace sindy::schema;

CConnectServer::CConnectServer(void)
{
}

CConnectServer::~CConnectServer(void)
{
}

//////////////////////////////////////////////////////////////////////
// �w��T�[�o�ւ̐ڑ�
//////////////////////////////////////////////////////////////////////
bool CConnectServer::run(CString &rConf)
{
	ConfData aConfData;
	ReadConf(rConf, aConfData);

	if( aConfData.eSDEServer.IsEmpty() )
	{
		std::cout << "# ERROR\t�ڑ�����T�[�o�ݒ肪�L������Ă��܂���B\n";
		return false;
	}

	if( !aConfData.eOutputShape.IsEmpty() )
	{
		if( !PathIsDirectory( aConfData.eOutputShape ) )
		{
			std::cout << "# ERROR\t�o��shape�f�B���N�g���̃p�X���s���ł��B\n";
			return false;
		}
	}

	if( !aConfData.eOutputLink.IsEmpty() )
	{
		if( !PathIsDirectory( aConfData.eOutputShape ) )
		{
			std::cout << "# ERROR\t���ԋ֎~�����Ԃ̃��C���̏o��shape�f�B���N�g���̃p�X���s���ł��B\n";
			return false;
		}
	}

	// ���H�T�[�o�ɐڑ�
	const IWorkspacePtr ipSDEWorkspace = GetWorkspace( aConfData.eSDEServer, _T("���H�p�T�[�o") );
	if( !ipSDEWorkspace )
		return false;

	// �������C���_������
	// shape�̏ꍇ�͓s���{���ʓǍ��܂Ȃ��Ƃ����Ȃ��̂ł����̃f�B���N�g���ł͔��ʂł��Ȃ�
	// ���̂��߂��̃f�B���N�g���ɓs���{���̃f�B���N�g����ǋL����K�v������A�Ǝv��(2009/3/13)
	// PGDB�̏ꍇ��shape�̏ꍇ�ŏ����𕪂���i�Ƃ������IFeatureClass�̎擾���@���኱�ς���j
	bool bShape = true;		// �Ǎ��ݑΏۂ�shape���ǂ���
	if( !PathIsDirectory( aConfData.eNoParkingPath ) )
	{
		if( !PathFileExists( aConfData.eNoParkingPath ) )
		{
			std::cout << "# ERROR\t���֎���H���f�[�^�ݒ肪�s���ł��B\n";
			return false;
		}
		else
			bShape = false;
	}

	std::set<CString> setPerfecturePath;
	if( bShape )
	{
		// shape�̏ꍇ�͓s���{���f�B���N�g�����擾����
		if( !GetPerfecturePath( aConfData.eNoParkingPath, setPerfecturePath ) )
			return false;
	}
	else
		// PGDB�̏ꍇ�͂��̂܂ܒǉ�
		setPerfecturePath.insert( aConfData.eNoParkingPath );

	// ���֎���H���f�[�^�ɐڑ����A���֎���H���t�B�[�`���N���X���擾
	std::vector<CAdapt<IFeatureClassPtr>> vecFeatureClass;
	for( std::set<CString>::const_iterator it = setPerfecturePath.begin(); it != setPerfecturePath.end(); ++it )
	{
		// IWorkspace�擾
		const IWorkspacePtr ipNoParkingWorkspace = GetWorkspace( *it, _T("���֘H��") );
		if( ipNoParkingWorkspace )
		{
			IFeatureWorkspacePtr ipNoParkingFeatureWorkspace(ipNoParkingWorkspace);
			if( ipNoParkingFeatureWorkspace )
			{
				IFeatureClassPtr ipNoParkingRoadClass;
				ipNoParkingFeatureWorkspace->OpenFeatureClass(CComBSTR("NoParking_Road"), &ipNoParkingRoadClass);
			}
			else
			{
				std::cout << "# ERROR\tIFeatureWorkspace�̎擾�Ɏ��s\n";
				return false;
			}
			// IFeatureClass�̎擾��shape��PGDB���ŏ����𕪂���
			if( bShape )
			{
				// IFeatureClass���擾���A
				IFeatureClassPtr ipNoParkingRoadClass1, ipNoParkingRoadClass2;
				ipNoParkingRoadClass1 = GetFeatureClass( ipNoParkingWorkspace, _T("Reg_R_Imp") );
				ipNoParkingRoadClass2 = GetFeatureClass( ipNoParkingWorkspace, _T("Reg_R") );
				// ���X�g�֓˂�����
				if( ipNoParkingRoadClass1 && ipNoParkingRoadClass2 )
				{
					vecFeatureClass.push_back( ipNoParkingRoadClass1 );
					vecFeatureClass.push_back( ipNoParkingRoadClass2 );
				}
				else
				{
					if( !ipNoParkingRoadClass1 )
						std::cout << "# ERROR\t" << CT2A(*it) << "\tReg_R_Imp.shp ������܂񂹂�B\n";
					if( !ipNoParkingRoadClass2 )
						std::cout << "# ERROR\t" << CT2A(*it) << "\tReg_R.shp ������܂񂹂�B\n";
					return false;
				}
			}
			else
			{
				// IFeatureClass���擾���A���X�g�֓˂�����
				IFeatureClassPtr ipNoParkingRoadClass;
				ipNoParkingRoadClass = GetFeatureClass( ipNoParkingWorkspace, _T("NoParking_Road") );
				if( ipNoParkingRoadClass )
					vecFeatureClass.push_back( ipNoParkingRoadClass );
				else
				{
					std::cout << "# ERROR\t" << CT2A(*it) <<"�t�B�[�`���N���X(NoParking_Road) ������܂񂹂�B\n";
					return false;
				}
			}
		}
		else
			return false;
	}

	// ���H�����N�N���X���擾
	IFeatureClassPtr ipLinkClass = GetFeatureClass( ipSDEWorkspace, _T("Road_Link") );
	if( !ipLinkClass )
		return false;

	// �o�͐惍�O�̍쐬
	std::ofstream ofp(static_cast<CT2A>(aConfData.eOutput));
	if( ofp.fail() )
	{
		std::cout << "# ERROR\t�G���[���O�t�@�C���u" << CT2A(aConfData.eOutput);
		std::cout << "�v�쐬���s" << std::endl;
		return false;
	}
	CMakeList aMakeList;
	aMakeList.create( ipLinkClass, vecFeatureClass, bShape, ofp, aConfData.eOutputShape, aConfData.eOutputLink );

	ofp.close();

	// SJIS��EUC�����R�[�h�ϊ�
	char szOutputTmpFile[L_tmpnam];
	::tmpnam(szOutputTmpFile);
	FILE *ifp = fopen(CT2A(aConfData.eOutput), "r");
	FILE *ofp2 = fopen(szOutputTmpFile, "wb");
	wkfConvertKanjiCodeOfFile(KC_SJIS, ifp, KC_EUC, ofp2);
    fclose(ifp);
    fclose(ofp2);
	remove( CT2A(aConfData.eOutput) );
	rename( szOutputTmpFile, CT2A(aConfData.eOutput) );

	return true;
}

//////////////////////////////////////////////////////////////////////
// �ݒ�t�@�C���̓ǂݍ���
//////////////////////////////////////////////////////////////////////
void CConnectServer::ReadConf(CString &rConf, ConfData &rConfData)
{
	std::ifstream ifs(rConf);
	char rStrLine[1024] ={0};
	while( ifs.getline( rStrLine, sizeof(rStrLine) ) )
	{
		CString csLine = rStrLine;
		if( csLine[0] == '#' )
			continue;

		if( csLine.Find(_T("SDESERVER") ) >= 0 )
		{
			csLine.Delete(0,9);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eSDEServer = csLine;
		}

		if( csLine.Find(_T("NOPARKINGDATA") ) >= 0 )
		{
			csLine.Delete(0,13);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eNoParkingPath = csLine;
		}

		if( csLine.Find(_T("OUTPUTFILE") ) >= 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutput = csLine;
		}

		if( csLine.Find(_T("OUTPUTSHAPE") ) >= 0 )
		{
			csLine.Delete(0,11);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutputShape = csLine;
		}
		if( csLine.Find(_T("OUTPUTBASE") ) >= 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutputLink = csLine;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// shape�̓s���{���f�B���N�g�����擾
//////////////////////////////////////////////////////////////////////
bool CConnectServer::GetPerfecturePath( LPCTSTR lpcszNoParkingPath, std::set<CString>& setPerfecturePath )
{
	setPerfecturePath.clear();

	WIN32_FIND_DATA fd;
	CString strFindDir = CString(lpcszNoParkingPath) + _T("\\*");
	HANDLE hFind = FindFirstFile( strFindDir, &fd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// ���[�g�p�X�ȉ��̃f�B���N�g���T��
		do
		{
			if( fd.cFileName[0] == '.' )
				continue;
			// NOPARKINGDIR�Ŏw�肵���f�B���N�g���ȉ��ɂ͓s���{���ʂ̃f�B���N�g�������Ȃ��͂�������
			// �u.shp�v�����邩���������Ĕ��ʂ���悤�ɂ���
			CString strPerfecturePath = CString(lpcszNoParkingPath) + _T("\\") + fd.cFileName;
			if( PathIsDirectory( strPerfecturePath ) )
			{
				WIN32_FIND_DATA fd2;
				CString strFindDir2 = strPerfecturePath + _T("\\*");
				HANDLE hFind2 = FindFirstFile( strFindDir2, &fd2 );
				if( hFind2 != INVALID_HANDLE_VALUE )
				{
					// ���[�g�p�X�ȉ��̃f�B���N�g���T��
					do
					{
						if( fd2.cFileName[0] == '.' )
							continue;
						// �����f�B���N�g�����Ɂu.shp�v������Γs���{���f�B���N�g���ƌ��Ȃ�
						if( _tcsicmp( CString(fd2.cFileName).Right(4), _T(".shp") ) == 0 )
						{
							setPerfecturePath.insert( strPerfecturePath );
							break;
						}
					} while( FindNextFile( hFind2, &fd2 ) );
					FindClose(hFind2);
				}
				else
				{
					// �����ɗ��邱�Ƃ͂Ȃ��Ǝv�����ǁA�ꉞ
					std::cout << "# ERROR\t���֎���H���f�[�^�������s2\n";
					FindClose(hFind);
					return false;
				}
			}
		} while( FindNextFile( hFind, &fd ) );
		FindClose(hFind);
	}
	else
	{
		std::cout << "# ERROR\t���֎���H���f�[�^�������s\n";
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// IWorkspace���擾
//////////////////////////////////////////////////////////////////////
IWorkspacePtr CConnectServer::GetWorkspace( LPCTSTR lpcszDBPath, LPCTSTR lpcszMessage )
{
	IWorkspacePtr ipWorkspace(INamePtr(create_workspace_name(lpcszDBPath))->_Open());
	if( ipWorkspace )
		std::cerr << CT2A(lpcszMessage) << "�F" << sindy::workspace_address(ipWorkspace) << "�ɐڑ�\n";
	else
		std::cout << "# ERROR\t" << CT2A(lpcszDBPath) << "\tIWorkspace�擾���s\n";

	return ipWorkspace;
}

//////////////////////////////////////////////////////////////////////
// IFeatureClass���擾
//////////////////////////////////////////////////////////////////////
IFeatureClassPtr CConnectServer::GetFeatureClass( IWorkspace* ipWorkspace, LPCTSTR lpcszFeatureClassName )
{
	IFeatureClassPtr ipFeatureClass;
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	if( ipFeatureWorkspace )
	{
		ipFeatureWorkspace->OpenFeatureClass( CComBSTR(lpcszFeatureClassName), &ipFeatureClass );
		if( !ipFeatureClass )
			std::cout << "# ERROR\tIFeatureClass�̎擾�Ɏ��s\n";
	}
	else

		std::cout << "# ERROR\tIFeatureWorkspace�̎擾�Ɏ��s\n";

	return ipFeatureClass;
}
