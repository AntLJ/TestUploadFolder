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

// CreateOrthoArea.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include  <io.h>
#include  <stdio.h>
#include <math.h>

using namespace std;

CString m_strRasterWorkspace;
CString m_strMeshListDir;
CString m_strOutputDir;
CString m_strTemplateOrthoArea;
CString m_strLogFileName;
ISpatialReferencePtr m_ipSpRef;
long m_lAreaIndex = -1;
long m_lPhotoNoIndex = -1;
long m_lZoomLevel = 19;

crd_cnv g_cnv;

bool Init(int argc, _TCHAR **argv);
int run();
void WriteLog(LPCTSTR lpcszErr);

double XToLongitude(int x, int zoom)
{
    return (double)x / pow((double)2, (double)zoom) * 360.0 - 180.0;
}

double YToLatitude(int y, int zoom)
{
    double tile_p = pow(M_E, (double)((0.5 - (y / pow((double)2, (double)zoom))) * 4.0 * M_PI));
    return asin((tile_p - 1.0) / (tile_p + 1.0)) * 180.0 / M_PI;
} 


int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try {
		arctl::coinitializer initializer;
		{
			if( Init( argc, argv) )
				retval = run();
			else
				return retval;
		}
	} catch (...) {
		retval = 2;
	}

	switch (retval) {
	case 0: std::cerr << "����ɏI�����܂���" << std::endl; break;
	case 1: std::cerr << "�ُ�I�����܂���" << std::endl; break;
	case 2: std::cerr << "��O���������܂���" << std::endl; break;
	default: break;
	}
	return retval;
}


// �g�p���@�o�͊֐�
bool Usage()
{
	std::cerr << "�I���\�f�[�^����I���\�G���A���쐬����c�[��" << std::endl;
	std::cerr << "Usage  : CreateOrthoArea -r <�I���\�̓����Ă���t�H���_>" << std::endl;
	std::cerr << "                         -m <���b�V�����X�g�̓����Ă���t�H���_>" << std::endl;
	std::cerr << "                         -o <�A�E�g�v�b�g��>" << std::endl;
	std::cerr << "                         -t <�e���v���[�g�I���\�̃e���v���[�g�t�@�C����>" << std::endl;
	std::cerr << "                         -l <���O�t�@�C����>" << std::endl;
	std::cerr << "                         -z <�Y�[�����x���i�f�t�H���g19�j>" << std::endl;

	return false;
}

// �p�X�̍Ō��\\���Ȃ��ꍇ�ɂ͑���
CString CheckLastWord( CString strString )
{
	CString strLastWord = strString.Right( 1 );
	if( strLastWord != _T("\\") )
		strString = strString + _T("\\");
	return strString;
}

// �������֐�
bool Init(int argc, _TCHAR **argv)
{
	if (argc <= 1)
		return Usage();
	while (--argc)
	{
		_TCHAR *s = *++argv;
		switch (*s)
		{
		case '-':
			switch (*++s)
			{
			case 'r':
				m_strRasterWorkspace = *++argv;
				m_strRasterWorkspace = CheckLastWord( m_strRasterWorkspace );
				--argc;
				break;
			case 'm':
				m_strMeshListDir = *++argv;
				m_strMeshListDir = CheckLastWord( m_strMeshListDir );
				--argc;
				break;
			case 'o':
				m_strOutputDir = *++argv;
				m_strOutputDir = CheckLastWord( m_strOutputDir );
				--argc;
				break;
			case 't':
				m_strTemplateOrthoArea = *++argv;
				{
					// �ŏ��ɃI���\�G���A�̋�ԎQ�Ǝ擾���Ă���
					IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
					IWorkspacePtr ipWorkspace;
					ipWorkspaceFactory->OpenFromFile( _bstr_t( m_strTemplateOrthoArea ), 0, &ipWorkspace );
					IEnumDatasetPtr ipEnumDataset;
					ipWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumDataset );
					IDatasetPtr ipDataset;
					while( ipEnumDataset->Next( &ipDataset ) == S_OK )
					{
						IGeoDatasetPtr ipGeoDataset = ipDataset;
						ipGeoDataset->get_SpatialReference( &m_ipSpRef );
					}
				}
				--argc;
				break;
			case 'l':
				m_strLogFileName = *++argv;
				{
					FILE* fStream = NULL;
					_wfopen_s( &fStream, m_strLogFileName, _T("a") );

					if ( !fStream )
					{
						std::cerr << "���O�t�@�C�������m�F���Ă��������B���O�t�@�C�����쐬���邱�Ƃ��ł��܂���ł���" << std::endl;
						return false;
					}
					fclose( fStream );
				}
				--argc;
				break;
			case 'z':
				m_lZoomLevel = _ttol( *++argv );
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

	// �l�͑S�����܂��Ă��Ȃ��Ƃ����Ȃ�
	if( m_strRasterWorkspace.IsEmpty() || m_strMeshListDir.IsEmpty() || m_strOutputDir.IsEmpty() || m_strTemplateOrthoArea.IsEmpty() || 
		m_strLogFileName.IsEmpty() || !m_ipSpRef )
	{
		std::cerr << "�������s�����Ă��邩�A�I���\�G���A�̋�ԎQ�Ƃ��擾�ł��܂���ł���" << std::endl;
		return false;
	}

	return true;
}

// �t�H���_�̒��ɂ���txt�t�@�C���������Ă���
void FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, LPCTSTR lpFormat, std::list<CString> &listFileName)
{
	do {
		if (lstrcmp(fd.cFileName, _T(".")) == 0 || lstrcmp(fd.cFileName, _T("..")) == 0) {
			// �������Ȃ�
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			CString cstrPath;
			cstrPath.Format(_T("%s%s"), lpPath, fd.cFileName);
			CString cstrFind = cstrPath + CString(_T("\\*"));

			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile(hFind2, fd2, cstrPath, lpFormat, listFileName);
			FindClose(hFind2);

		} else {
			CString cstrName;
			cstrName.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrExtension = PathFindExtension(cstrName);
			if (cstrExtension == CString(lpFormat))
				listFileName.push_back(cstrName);
		}
	} while (FindNextFile(hFind, &fd));
}

// �t�H���_���̃t�@�C����������
void CreateOrtholist( std::list<CString> &listOrthoName )
{
	WIN32_FIND_DATA		ffd;
	CString	strDirPath = m_strRasterWorkspace + _T("*");
	HANDLE hFile = FindFirstFile( strDirPath, &ffd );

	if ( hFile == INVALID_HANDLE_VALUE )
		return;	// �t�@�C�������݂��Ȃ��H
	// �s�s�n�}���b�V�����X�g�������Ă���
	FindFile( hFile, ffd, m_strRasterWorkspace, _T(".jpg"), listOrthoName );
	FindClose(hFile);
}

// ���b�V�����X�g�ǂݍ��݂��s���A�I���\��U�蕪����t�H���_�\���쐬�֐�
void CreateMeshFloder(map< unsigned int, CString> &mapMeshCity )
{
	std::list< CString > listCityName;
	// �t�H���_���̃t�@�C����������
	WIN32_FIND_DATA		ffd;
	CString	strDirPath = m_strMeshListDir + _T("*");
	HANDLE hFile = FindFirstFile( strDirPath, &ffd );

	if ( hFile == INVALID_HANDLE_VALUE )
		return;	// �t�@�C�������݂��Ȃ��H
	// �s�s�n�}���b�V�����X�g�������Ă���
	FindFile( hFile, ffd, m_strMeshListDir, _T(".txt"), listCityName );
	FindClose(hFile);

	CString strOutDir = m_strOutputDir;
	for( list< CString >::iterator it = listCityName.begin(); it != listCityName.end(); ++it )
	{
		std::ifstream file(*it);
		if (! file.is_open())
			return;

		CString strCity = *it;
		strCity.Replace( m_strMeshListDir, _T("") );
		strCity.Replace( _T(".txt"), _T("") );
		strCity.Replace( _T("\\"), _T("") );
		while (file.good())
		{
			unsigned int m = 0;
			file >> m;
			if (! m) continue;
			// ���b�V���R�[�h�Ɠs�s�G���A�̊֌W���m��
			mapMeshCity[ m ] = strCity;

			CString strMesh;
			strMesh.Format( _T("%lu"), m );
			CString strDir = strOutDir;
			if ( !PathFileExists(strDir) )
				CreateDirectory( strDir, NULL );
			strDir = strDir + strCity;
			if ( !PathFileExists(strDir) )
				CreateDirectory( strDir, NULL );
			strDir = strDir + _T("\\") + strMesh;
			if ( !PathFileExists(strDir) )
				CreateDirectory( strDir, NULL );
		}
		// OrthoAreaPGDB���R�s�[����
		CString strPGDBName = m_strOutputDir + strCity + _T("\\") + strCity + _T(".mdb");
		CopyFile( m_strTemplateOrthoArea, strPGDBName, TRUE );
		// ���łɁA�t�B�[���h�C���f�b�N�X�擾
		if( m_lAreaIndex == -1 )
		{
			IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
			IWorkspacePtr ipOrthoAreaWorkspace;
			ipWorkspaceFactory->OpenFromFile( _bstr_t( m_strTemplateOrthoArea ), 0, &ipOrthoAreaWorkspace );
			IEnumDatasetPtr ipEnumDataset;
			ipOrthoAreaWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumDataset );
			IDatasetPtr ipDataset;
			IFeatureClassPtr ipFeatureClass;
			while( ipEnumDataset->Next( &ipDataset ) == S_OK )
				ipFeatureClass = ipDataset;
			IFieldsPtr ipFields;
			ipFeatureClass->get_Fields( &ipFields );
			ipFields->FindField( _bstr_t(_T("AREA")), &m_lAreaIndex );
			ipFields->FindField( _bstr_t(_T("PHOTONO")), &m_lPhotoNoIndex );
		}
	}
}

// ���ۂ̎��s�֐�
int run()
{
#ifndef _FOR_ENGLISH
	::setlocale(LC_ALL, "Japanese");
#endif 

	SYSTEMTIME stSystemTime = {0};
	::GetLocalTime( &stSystemTime);
	CString strLog;
	strLog.Format( _T("***************************************************\n�J�n����:%d/%02d/%02d %02d:%02d:%02d\n"), 
		stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond );
	WriteLog( strLog );

	// �t�H���_���̃t�@�C����������
	std::list< CString > listOrthoName;
	CreateOrtholist( listOrthoName );

	// �摜�t�@�C���̃R�s�[��t�H���_���쐬
	map< unsigned int, CString > mapMeshCity;
	CreateMeshFloder( mapMeshCity );

	map< CString, CAdapt< IFeatureClassPtr > > mapFeatureClass;
	map< CString, CAdapt< IFeatureCursorPtr > > mapFeatureCursor;
	USES_CONVERSION;
	for( list< CString >::iterator it = listOrthoName.begin(); it != listOrthoName.end(); ++it )
	{
		CString strOrthoFullPath = *it;
		CString strFileName = strOrthoFullPath.Right( strOrthoFullPath.GetLength() - strOrthoFullPath.ReverseFind( '\\' ) - 1 );
		CString strFolderPath = strOrthoFullPath.Left( strOrthoFullPath.ReverseFind( '\\' ) );
		CString strFolderName = strFolderPath.Right( strFolderPath.GetLength() - strFolderPath.ReverseFind( '\\' ) - 1 );
		CString strX, strY;
		strX = strFolderName;
		strY = strFileName.Left( strFileName.ReverseFind( '.' ) );
		CString strPasteFileName = strX + _T("_") + strY + _T(".jpg");

		long lX = _ttol( strX );
		long lY = _ttol( strY );

		double dX1, dY1, dX2, dY2, dX3, dY3, dX4, dY4;
		// ����
		dX1 = XToLongitude( lX, m_lZoomLevel );
		dY1 = YToLatitude( lY, m_lZoomLevel );
		// ����
		dX2 = XToLongitude( lX, m_lZoomLevel );
		dY2 = YToLatitude( lY+1, m_lZoomLevel ); 
		// �E��
		dX3 = XToLongitude( lX+1, m_lZoomLevel );
		dY3 = YToLatitude( lY+1, m_lZoomLevel ); 
		// �E��
		dX4 = XToLongitude( lX+1, m_lZoomLevel );
		dY4 = YToLatitude( lY, m_lZoomLevel ); 

		IPointCollectionPtr ipPointCollection(CLSID_Ring);
		WKSPoint points[5];
		points[0].X = dX1; points[0].Y = dY1;
		points[1].X = dX2; points[1].Y = dY2;
		points[2].X = dX3; points[2].Y = dY3;
		points[3].X = dX4; points[3].Y = dY4;
		points[4].X = dX1; points[4].Y = dY1;
		ipPointCollection->AddWKSPoints(5, points);
		IGeometryCollectionPtr ipGeometryCollection(CLSID_Polygon);
		ipGeometryCollection->AddGeometry((IGeometryPtr)ipPointCollection);
		((IGeometryPtr)ipGeometryCollection)->putref_SpatialReference( m_ipSpRef );
		((IGeometryPtr)ipGeometryCollection)->SnapToSpatialReference();

		unsigned int	meshcode1 = 0, meshcode2 = 0, meshcode3 = 0, meshcode4 = 0;
		int nX1 = 0, nY1 = 0, nX2 = 0, nY2 = 0, nX3 = 0, nY3 = 0, nX4 = 0, nY4 = 0;
		int Ret1 = g_cnv.LLtoMesh_World( dX1, dY1, 64, crd_cnv::SJ_ORIGIN, &meshcode1, &nX1, &nY1, 0 );
		int Ret2 = g_cnv.LLtoMesh_World( dX2, dY2, 64, crd_cnv::SJ_ORIGIN, &meshcode2, &nX2, &nY2, 0 );
		int Ret3 = g_cnv.LLtoMesh_World( dX3, dY3, 64, crd_cnv::SJ_ORIGIN, &meshcode3, &nX3, &nY3, 0 );
		int Ret4 = g_cnv.LLtoMesh_World( dX4, dY4, 64, crd_cnv::SJ_ORIGIN, &meshcode4, &nX4, &nY4, 0 );

		list< unsigned int > listRasterMeshCode;
		listRasterMeshCode.push_back( meshcode1 );
		listRasterMeshCode.push_back( meshcode2 );
		listRasterMeshCode.push_back( meshcode3 );
		listRasterMeshCode.push_back( meshcode4 );
		listRasterMeshCode.sort();
		listRasterMeshCode.unique();

		bool bCreate = false;
		for( list< unsigned int >::iterator it = listRasterMeshCode.begin(); it != listRasterMeshCode.end(); ++it )
		{
			CString strCity = mapMeshCity[ *it ];
			if( !strCity.IsEmpty() )
			{
				CString strMeshCode;
				strMeshCode.Format( _T("%lu"), *it );
				CString strCopyFolder = m_strOutputDir + strCity + _T("\\") + strMeshCode + _T("\\");
				CString strPasteFullFileName = strCopyFolder + strPasteFileName;
				// �㏑���R�s�[�͂��Ȃ����Ƃɂ���(��3����=TRUE)
				CopyFile( strOrthoFullPath, strPasteFullFileName, TRUE );

				// �悤�₭�����Ō`��쐬����
				// �S���摜�U�蕪������ŁA�I���\�G���A
				CString strOrthoAreaPGDBName = m_strOutputDir + strCity + _T("\\") + strCity + _T(".mdb");
				IFeatureClassPtr ipFeatureClass = mapFeatureClass[ strOrthoAreaPGDBName ];
				IFeatureCursorPtr ipFeatureCursor = mapFeatureCursor[ strOrthoAreaPGDBName ];
				if( !ipFeatureClass )
				{
					IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
					IWorkspacePtr ipOrthoAreaWorkspace;
					ipWorkspaceFactory->OpenFromFile( _bstr_t( strOrthoAreaPGDBName ), 0, &ipOrthoAreaWorkspace );
					IEnumDatasetPtr ipEnumDataset;
					ipOrthoAreaWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumDataset );
					IDatasetPtr ipDataset;
					while( ipEnumDataset->Next( &ipDataset ) == S_OK )
						ipFeatureClass = ipDataset;

					mapFeatureClass[ strOrthoAreaPGDBName ] = ipFeatureClass;
					ipFeatureClass->Insert(VARIANT_FALSE, &ipFeatureCursor);
					mapFeatureCursor[ strOrthoAreaPGDBName ] = ipFeatureCursor;
				}

				IFeatureBufferPtr ipFeature;
				ipFeatureClass->CreateFeatureBuffer(&ipFeature);
				HRESULT hr = ipFeature->put_Value( m_lAreaIndex, _variant_t( _bstr_t( strCity ) ) );
				CString strBrowseName = strPasteFileName;
				strBrowseName = strBrowseName.Left( strBrowseName.Find( _T(".") ) );
				// PHOTONO�́A10���R�[�h�{�t�@�C�����i�g���q�Ȃ��j�ƂȂ�
				CString strFileName = strMeshCode + _T("\\") + strBrowseName;
				hr = ipFeature->put_Value( m_lPhotoNoIndex, _variant_t( _bstr_t( strFileName ) ) );
				hr = ipFeature->putref_Shape( (IGeometryPtr)ipGeometryCollection );
				// PGDB�̏ꍇ�ɂ�InsertFeature����Δ��f����Ă��܂�
				CComVariant vaID;
				hr = ipFeatureCursor->InsertFeature(ipFeature, &vaID);
				// ������Flush���Ă��悢���A�K�v�Ȃ���
				// �I���\�G���A���ЂƂ쐬�����΂����̂ŁA1�ڂ�break
				bCreate =true;
				break;
			}
		}

		// �����͈͊O�̂��߁A�I���\�G���A�쐬�ł��Ȃ�����
		if( !bCreate )
		{
			strLog.Format( _T("�����G���A�͈͊O�̉摜:%s\n"), strOrthoFullPath );
			WriteLog(strLog);
		}
	}

	::GetLocalTime( &stSystemTime);
	strLog.Format( _T("�I������:%d/%02d/%02d %02d:%02d:%02d\n***************************************************\n"),
		stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond );
	WriteLog( strLog );

	return 0;
}

// ���O���������ފ֐�
void WriteLog(LPCTSTR lpcszErr)
{
	FILE* fStream = NULL;
	_wfopen_s( &fStream, m_strLogFileName, _T("a") );

 	if( fStream )
	{
		fwprintf( fStream, lpcszErr );
		fclose( fStream );
	}
}
