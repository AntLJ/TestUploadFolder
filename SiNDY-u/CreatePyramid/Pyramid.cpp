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
#include "Pyramid.h"
using namespace std;

namespace{
	const long PATH_LEN = 1024;
}

// �R���X�g���N�^
CPyramid::CPyramid(void)
	: m_bBatch(false), m_lDir(0), m_lExtension(0), m_lMode(1){
		char szCurrent[PATH_LEN] = {};
		::GetCurrentDirectory( PATH_LEN, szCurrent );
		m_strCurrent.Format(_T("%s"), szCurrent);
		m_strIniPath.Format(_T("%s\\Common.ini"), szCurrent);
		m_strMeshlist.Format(_T("meshlist.txt"));
}

// ���b�V�����X�g�w��ŏ�������
void CPyramid::TargetMeshList( bool bOption )
{
	char szDir[PATH_LEN] = {};
	char szMeshPath[PATH_LEN] = {};
	char szExtension[16] = {};
	CString strExtension;
	cout<<"\n�g���q���w�肵�Ă��������B�i��F�ujpg�v�upng�v�j\n\t-> ";
	if( bOption )
	{
		switch( m_lExtension )
		{
		case 0:
			strExtension.Format(_T("jpg"));
			break;
		case 1:
			strExtension.Format(_T("png"));
			break;
		case 2:
			strExtension.Format(_T("bmp"));
			break;
		default:
			strExtension.Format(_T("jpg"));
			break;
		}
		cout<<strExtension<<endl;
	}
	else
	{
		cin>>szExtension;
		strExtension.Format(_T("%s"), szExtension);
	}

	cout<<"���b�V�����X�g�t�@�C�����w�肵�Ă��������B\n\t->";
	char* pMeshPath = m_strCurrent.GetBuffer( m_strCurrent.GetLength() + m_strMeshlist.GetLength() + 1 );
	m_strCurrent.ReleaseBuffer();
	CString strMeshPath;
	if( bOption )
	{
		// ��΃p�X�����΃p�X��
		if( strMeshPath.Left(3).Find(_T(":\\")) || strMeshPath.Left(3).Find(_T("\\\\")) || strMeshPath.Left(3).Find(_T(":/")) || strMeshPath.Left(3).Find(_T("//")) )
		{
			strMeshPath = m_strMeshlist;
		}
		else
		{
			PathAppend( pMeshPath, m_strMeshlist );
			strMeshPath.Format(_T("%s"), pMeshPath);
		}
		cout<<m_strMeshlist<<endl;
	}
	else
	{
		cin>>szMeshPath;
		strMeshPath.Format(_T("%s"), szMeshPath);
		// ��΃p�X�����΃p�X��
		if( strMeshPath.Left(3).Find(_T(":\\")) || strMeshPath.Left(3).Find(_T("\\\\")) || strMeshPath.Left(3).Find(_T(":/")) || strMeshPath.Left(3).Find(_T("//")) )
		{
		}
		else
		{
			PathAppend( pMeshPath, szMeshPath );
			strMeshPath.Format(_T("%s"), pMeshPath);
		}
	}

	if( !PathFileExists( strMeshPath ) )
	{
		cout<<strMeshPath<<" �͂���܂���B"<<endl;
		return;
	}

	char szDirStruct[PATH_LEN] = {0};
	CString strDirStruct;
	cout<<"�f�B���N�g���\��"<<endl;
	cout<<"  [ 1 ]�u./4��/8��.jpg�v�\�� or �u./4��/6��.jpg�v"<<endl;
	cout<<"  [ 2 ]�u./8��.jpg�v�\�� or �u./6��.jpg�v�\��"<<endl;
	cout<<"  [ 3 ]�u./6��/10��.jpg�v�\��"<<endl;
	cout<<"\t 1 or 2 or 3 ??  : ";
	if( bOption )
	{
		if(m_lDir ==  1)
			cout<<"2"<<endl;
		else if(m_lDir == 0)
			cout<<"1"<<endl;
		else if(m_lDir == 2)
			cout<<"3"<<endl;
	}
	else
	{
		cin>>szDirStruct;
		strDirStruct.Format(_T("%s"), szDirStruct);
		if( strDirStruct.Left(1) == _T("1") )
			m_lDir = 0;
		else if( strDirStruct.Left(1) == _T("2") )
			m_lDir = 1;
		else if( strDirStruct.Left(1) == _T("3") )
			m_lDir = 2;
	}

	if( !PathFileExists( m_strIniPath ) )
	{
		cout<<"�ݒ�t�@�C��:"<<m_strIniPath<<"�͂���܂���B"<<endl;
		return;
	}

	switch( m_lDir ){
		case 0:
			::GetPrivateProfileString( _T("1"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		case 1:
			::GetPrivateProfileString( _T("3"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		case 2:
			::GetPrivateProfileString( _T("4"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		default:
			break;
	}

	FILE *fin;
	fopen_s( &fin, strMeshPath, "r" );
	if( fin == NULL )
	{
		cout<<"\n!! ���b�V�����X�g�t�@�C���͑��݂��܂����A�I�[�v�����邱�Ƃ��o���܂���ł����B"<<endl;
		return;
	}
	char szMesh[PATH_LEN] = {};
	std::list<CString> listMesh;
	while( fgets( szMesh, PATH_LEN, fin ) != NULL )
	{
		unsigned int iMesh = _atoi64(szMesh);
		CString strMesh;
		strMesh.Format(_T("%u"), iMesh);
		listMesh.push_back(strMesh);
	}
	fclose(fin);

	PathAddBackslash( szDir );

	cout<<"�@���s���~�b�h�t�@�C���쐬��"<<endl;

	IWorkspaceFactoryPtr ipWF;
	if( SUCCEEDED( ipWF.CreateInstance( CLSID_RasterWorkspaceFactory)))
	{
		// ���X�^�t�@�C���̂���t�H���_���w��
		IWorkspacePtr ipWS;
		if( SUCCEEDED( ipWF->OpenFromFile( CComBSTR( szDir ), NULL, &ipWS)))
		{
			// ���X�^�t�@�C���̓ǂݍ���
			IRasterWorkspacePtr ipRasterWS(ipWS);
			IRasterDatasetPtr ipRasterDataset;

			int i = 0;

			for( std::list<CString>::iterator it = listMesh.begin(); it != listMesh.end(); it++ )
			{
				CString strMesh, strBase;
				strMesh = *it;
				if( strMesh.IsEmpty() )
					continue;

				if(m_lDir==2)
					strBase = strMesh.Left(6);
				else
					strBase = strMesh.Left(4);
				i++;
				list<CString> listFindFile;
				CString strTmp;
				CString strFindFile;
				switch( m_lDir ){
					case 0:
						strFindFile.Format(_T("%s\\%s.%s"), strBase, strMesh, strExtension);
						listFindFile.push_back( strFindFile );
						break;
					case 1:
						strFindFile.Format(_T("%s.%s"), strMesh, strExtension);
						listFindFile.push_back( strFindFile );
						break;
					case 2:
						for( int j = 0; j != 4; ++j ){
							strFindFile.Format(_T("%s\\%s_%ld.%s"), strBase, strMesh, j+1, strExtension);
							listFindFile.push_back( strFindFile );
						}
						break;
					default:
						break;
				}
				m_strRasterDir = szDir;

				for( list< CString >::iterator it = listFindFile.begin(); it != listFindFile.end(); ++it )
					CreatePyramidFile( ipRasterWS, *it, *it, i );
			}
		}
		else
			cout<<"\t"<<szDir<<" �͂���܂���B"<<endl;
	}
	cout<<"�@���s���~�b�h�t�@�C���쐬�I��\n\n";
}

// �f�B���N�g���w��ŏ�������
void CPyramid::TargetPathList( bool bOption )
{
	char szDir[PATH_LEN] = {};
	char szExtension[16] = {};
	CString strExtension;
	cout<<"\n�g���q���w�肵�Ă��������B�i��F�ujpg�v�upng�v�j\n\t-> ";
	if( bOption )
	{
		switch( m_lExtension )
		{
		case 0:
			strExtension.Format(_T("jpg"));
			break;
		case 1:
			strExtension.Format(_T("png"));
			break;
		case 2:
			strExtension.Format(_T("bmp"));
			break;
		default:
			strExtension.Format(_T("jpg"));
			break;
		}
		cout<<strExtension<<endl;
	}
	else
	{
		cin>>szExtension;
		strExtension.Format(_T("%s"), szExtension);
	}

	::GetPrivateProfileString( _T("2"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
	if( !PathFileExists( szDir ) )
	{
		cout<<szDir<<" �͂���܂���B"<<endl;
		return;
	}
	PathAddBackslash( szDir );
	m_strRasterDir = szDir;

	cout<<"�@���s���~�b�h�t�@�C���쐬��"<<endl;
	int i = 0;

	IWorkspaceFactoryPtr ipWF;
	if( SUCCEEDED( ipWF.CreateInstance( CLSID_RasterWorkspaceFactory ) ) )
	{
		// ���X�^�t�@�C���̂���t�H���_���w��
		IWorkspacePtr ipWS;
		if( SUCCEEDED( ipWF->OpenFromFile( CComBSTR( szDir ), NULL, &ipWS)))
		{
			// ���X�^�t�@�C���̓ǂݍ���
			IRasterWorkspacePtr ipRasterWS(ipWS);
			IRasterDatasetPtr ipRasterDataset;

			CString strFindFile;
			strFindFile.Format(_T("%s*.%s"), szDir, strExtension);

			WIN32_FIND_DATA wfd = {0};
			HANDLE hFind = FindFirstFile( strFindFile, &wfd );
			if( hFind != INVALID_HANDLE_VALUE )
			{
				do{
					i++;
					CreatePyramidFile( ipRasterWS, wfd.cFileName, wfd.cFileName, i );
				} while( FindNextFile( hFind, &wfd ) );
			}
			FindClose( hFind );
		}
		else
			cout<<"\t�~ "<<szDir<<" �͂���܂���B"<<endl;
	}

	cout<<"�@���s���~�b�h�t�@�C���쐬�I��\n\n";
}

// �s���~�b�h�t�@�C�����쐬����
void CPyramid::CreatePyramidFile( IRasterWorkspacePtr ipRasterWS, LPCSTR szFildFile, LPCSTR szMesh, int i )
{
	cout<<"\t["<<i<<"]\t"<<szMesh;

	IRasterDatasetPtr ipRasterDataset;
	if( SUCCEEDED( ipRasterWS->OpenRasterDataset( CComBSTR( szFildFile ), &ipRasterDataset)))
	{
		IRasterLayerPtr ipRasterLayer;
		if( SUCCEEDED( ipRasterLayer.CreateInstance( CLSID_RasterLayer )))
		{
			if( SUCCEEDED( ipRasterLayer->CreateFromDataset( ipRasterDataset )))
			{
				// Arc10.3.1��IRasterLayer->get_PyramidPresent�̌��ʂ�10.1�ƈقȂ�
				// ��������킹�邽�߂Ƀ��X�^�t�@�C���Ɠ����t�@�C�����̃s���~�b�h�t�@�C��(*.ovr)�̑��݊m�F������

				// �����Ώۂ̃s���~�b�h�t�@�C��������
				CString strFindPyramid(szFildFile);
				strFindPyramid = m_strRasterDir + strFindPyramid + _T(".ovr");
				// �s���~�b�h�t�@�C��������
				bool existsPyramid = false;
				WIN32_FIND_DATA wfd = {0};
				HANDLE hFind = FindFirstFile(strFindPyramid, &wfd );
				// ����������true
				if( hFind != INVALID_HANDLE_VALUE )
					existsPyramid = true;
				FindClose( hFind );

				try{
					// �s���~�b�h�t�@�C�����Ȃ��ꍇ�̏���
					if(!existsPyramid)
					{
						IRasterPyramidPtr ipRasterPyramid( ipRasterDataset );
						ipRasterPyramid->Create();
						VARIANT_BOOL vaBool = VARIANT_FALSE;
						ipRasterPyramid->get_Present(&vaBool);
						cout<<"\t�쐬����!"<<endl;
					}
					// �s���~�b�h�t�@�C��������ꍇ�̏���
					else
						cout<<"\t�쐬��!"<<endl;
				}
				catch(...){
					cout<<"\t�쐬���s!!!"<<endl;
				}
			}
		}
		else
			cout<<"\t�w�胁�b�V���͂���܂���B"<<endl;
	}
	else
		cout<<"\t�w�胁�b�V���͂���܂���B"<<endl;
}

// �J�n����(�������[�h�󂯕t��)
int CPyramid::StartFunction( bool bOption )
{
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"�s���~�b�h�t�@�C�������쐬�c�[��"<<endl;
	cout<<"�y 1 �z���X�g����s���~�b�h�t�@�C�����쐬"<<endl;
	cout<<"�y 2 �z�f�B���N�g�����w�肵�A�s���~�b�h�t�@�C�����쐬"<<endl;
	char szMode[PATH_LEN] = {};
	cout<<"\t 1 or 2 ??  : ";

	if( bOption )
	{
		cout<<m_lMode<<endl;

		return m_lMode;
	}
	else
	{
		cin>>szMode;

		CString strMode;
		strMode.Format(_T("%s"), szMode);
		strMode.Left(1);

		return atoi(strMode);
	}
}

// �I������
int CPyramid::EndFunction()
{
	char szYesNo[PATH_LEN] = {0};
	cout<<"�����ď��� [y] / �I��[n]\n\t-> ";
	cin>>szYesNo;

	CString strYesNo;
	strYesNo.Format(_T("%s"), szYesNo);

	if( strYesNo.Left(1) == _T("y") || strYesNo.Left(1) == _T("Y") )
		return 1;
	else if( strYesNo.Left(1) == _T("n") || strYesNo.Left(1) == _T("N") )
		return 2;
	else
		return 0;
}

// �I�v�V��������͂��Đݒ肷��
bool CPyramid::InitOption( std::list<CString> listOption )
{
	long lePriority = 0;	//!< �D��x�p�����[�^
	long llPriority = 0;	//!< �D��x�p�����[�^
	for( std::list<CString>::iterator it = listOption.begin(); it != listOption.end(); it++ )
	{
		CString strTmp;
		strTmp = *it;

		// �W�����͂��ǂ���
		if( strTmp == _T("/c") )
			m_bBatch = true;

		// �g���q�ݒ�
		if( strTmp == _T("/j") && lePriority < 10 ){//jpg
			m_lExtension = 0;
			lePriority = 10;
		}else if( strTmp == _T("/p") && lePriority < 5 ){//png
			m_lExtension = 1;
			lePriority = 5;
		}else if( strTmp == _T("/b") && lePriority < 1 ){//bmp
			m_lExtension = 2;
			lePriority = 1;
		}

		// ���b�V�����X�g���f�B���N�g����
		if( strTmp == _T("/m") && llPriority < 5 ){//���b�V�����X�g
			m_lMode = 1;
			llPriority = 5;
		}else if( strTmp == _T("/d") && llPriority < 1 ){//�f�B���N�g��
			m_lMode = 2;
			llPriority = 1;
		}

		// �Ǎ��݃��X�g�f�B���N�g���ύX
		if( strTmp == _T("/l")  )	//�f�B���N�g��
			m_lDir = 1;
		if( strTmp == _T("/s")  )	//�f�B���N�g��
			m_lDir = 2;

		// ���b�V�����X�g�t�@�C��
		if( strTmp.Find(_T(".txt")) >= 0 )
			m_strMeshlist = strTmp;

		if( strTmp.Find(_T(".exe")) >= 0 )
		{
			char cFullPath[PATH_LEN] = {};
			PathSearchAndQualify( strTmp, cFullPath, PATH_LEN );
			PathRemoveFileSpec( cFullPath );
			m_strCurrent.Format(_T("%s"), cFullPath);
			m_strIniPath.Format(_T("%s\\Common.ini"), cFullPath);
		}
	}

	return m_bBatch;
}
