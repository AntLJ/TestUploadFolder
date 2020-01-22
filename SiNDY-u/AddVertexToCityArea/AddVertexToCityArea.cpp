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

// AddVertexToCityArea.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include  <io.h>
#include  <stdio.h>
#include "AheGeometryOP.h"
#include "Workspace.h"

using namespace std;

CString m_strCityAreaWorkspace;
CString m_strMeshServerName;

bool Init(int argc, _TCHAR **argv);
int run();
IFeatureClassPtr GetTertiaryMesh( CString strConnectServer );


int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
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
	std::cerr << "�s�s�`��Ƀ��b�V���Ƃ̌�_�ɍ\���_���쐬����c�[������" << std::endl;
	std::cerr << "Usage  : AddVertexToCityArea -c <�s�s�`�󂪓����Ă���PGDB�̃t�@�C���p�X>" << std::endl;
	std::cerr << "                             -m <���b�V���`����擾����ڑ���>" << std::endl;
	std::cerr << std::endl;
	std::cerr << "�ڑ���̌`��:���[�U��/�p�X���[�h/�o�[�W����/�T�[�r�X/�T�[�o��" << std::endl;
	std::cerr << "��FREFERENCE/REFERENCE/SDE.DEFAULT/5151/SPINEL2.MR.IPC.PIONEER.CO.JP" << std::endl;
	return false;
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
			case 'c':
				m_strCityAreaWorkspace = *++argv;
				--argc;
				break;
			case 'm':
				m_strMeshServerName = *++argv;
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
	if( m_strCityAreaWorkspace.IsEmpty() || m_strMeshServerName.IsEmpty()  )
	{
		std::cerr << "�������s�����Ă��܂�" << std::endl;
		return false;
	}

	return true;
}

// �T�[�o�ɂȂ��AJIBAN�t�B�[�`���N���X���擾����֐�
IFeatureClassPtr GetTertiaryMesh( CString strConnectServer )
{
	USES_CONVERSION;
	sindy::CWorkspace cWorkspace;
	if( !cWorkspace.Connect( strConnectServer ) )
	{
		std::cerr << T2A( strConnectServer ) << "�ɐڑ��ł��܂���ł���" << std::endl;
		return false;
	}
	// �T�[�o�ڑ�
	IFeatureDatasetPtr ipFeatureDataset;
	if(((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_REF_MESH")), &ipFeatureDataset) == S_OK)
	{
		// ���łɂ���ꍇ�ɂ́A�Ώۂ̃t�B�[�`���N���X�擾���ďI��
		IEnumDatasetPtr ipSubSetsDataset;
		ipFeatureDataset->get_Subsets( &ipSubSetsDataset );
		ipSubSetsDataset->Reset();
		IDatasetPtr ipSubDataset;
		while( ipSubSetsDataset->Next( &ipSubDataset ) == S_OK )
		{
			CString strDatasetName = AheGetFeatureClassName( ipSubDataset );
			if( lstrcmpi( strDatasetName, _T("TERTIARYMESH")) == 0 )
			{
				return ipSubDataset;
			}
		}
	}

	std::cerr << "TERTIARYMESH���擾�ł��܂���ł���" << std::endl;
	return NULL;
}

// ���ۂ̎��s�֐�
int run()
{
	IFeatureClassPtr ipMeshClass = GetTertiaryMesh( m_strMeshServerName );
	if( !ipMeshClass )
		return false;

	USES_CONVERSION;
	// PGDB�I�[�v��
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( _bstr_t( m_strCityAreaWorkspace ), 0, &ipWorkspace );

	IEnumDatasetPtr ipEnumDataset;
	ipWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumDataset );
	IDatasetPtr ipDataset;
	while( ipEnumDataset->Next( &ipDataset ) == S_OK )
	{
		IFeatureClassPtr ipFeatureClass = ipDataset;
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		ipQuery->put_WhereClause( _bstr_t( _T("OBJECTID>0") ) );
		IFeatureCursorPtr ipFeatureCursor;
		ipFeatureClass->Update(ipQuery, VARIANT_TRUE, &ipFeatureCursor);
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
		{
			IGeometryPtr ipCityGeom;
			ipFeature->get_ShapeCopy( &ipCityGeom );

			IFeatureCursorPtr ipCursor;
			ipMeshClass->Search( AheInitSpatialFilter( NULL, ipCityGeom ), VARIANT_FALSE, &ipCursor );
			IFeaturePtr ipMeshFeature;
			while( ipCursor->NextFeature( &ipMeshFeature ) == S_OK )
			{
				IGeometryPtr ipMeshGeom;
				ipMeshFeature->get_ShapeCopy( &ipMeshGeom );
				// ���b�V���ƌ����_���擾
				IPointCollectionPtr ipAddPointCol = AheGetCrossOrTouchPoints( ipCityGeom, ipMeshGeom  );
				if( ipAddPointCol )
				{
					// �\���_�����܂����ƒǉ����Ă����֐�
					AheAddVertexesIfOnGeometry( ipCityGeom, (IGeometryPtr)ipAddPointCol );
				}
			}
			ipFeature->putref_Shape( ipCityGeom );
			HRESULT hr = ipFeature->Store();
			CComVariant vaValue;
			ipFeature->get_Value( 2, &vaValue );
			CString strName = vaValue.bstrVal;
			std::cerr << T2A(strName) << "�ɍ\���_�ǉ��I��" << std::endl;
		}
	}

	return 0;
}

