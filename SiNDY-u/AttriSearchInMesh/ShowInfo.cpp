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

// ShowInfo.cpp: ShowInfo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShowInfo.h"
#include <io.h>

// �g�p�@
void CShowInfo::printUsage()
{
	std::cerr << "�y���b�V�������������c�[���g�p�@�z\n"
			  << "(Usage)AttriSearchInMesh.exe [�e��I�v�V����] (> �i�����O)\n"
			  << "�E�I�v�V�����ɂ���\n"
			  << "���`�F�b�N���郁�b�V���֘A�I�v�V����(�����ꂩ�K�{)\n"
			  << "-l  <���b�V�����X�g>\n"
			  << "-m  <���b�V���R�[�h>\n"
			  << "�����O�o�͐�I�v�V����(�K�{)\n"
			  << "-o  <�o�̓t�@�C����>\n"
			  << "�����̑��C�ӂ̃I�v�V����"
			  << "-i  <�o�̓t�B�[���h�ݒ�t�@�C��> ... -c �I�v�V�����w�莞�͖���\n"
			  << "-w  <Where��> ... �w�肵�Ȃ������Ƃ��́A�uOBJECT IS NOT NULL�v�ł�\n"
			  << "-c     : ���b�V�����̃I�u�W�F�N�g�����o�̓��[�h\n"
			  << "-nodup : �J�E���g����Ƃ��A���b�V�����E��̃|�C���g���l������(�d���J�E���g����Ȃ�)\n"
			  << "  ��-nodup �I�v�V�����́A����SDE�łł��|�C���g�f�[�^�̂ݑΉ����Ă��܂��B\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  PGDB_MODE   SiNDY-c�p�`�F�b�N���[�h ON/OFF(�����ݒ肳��Ă���΁AON�Ɣ��f����)\n"
			  << "  DB_ANY      DB�ڑ��v���p�e�B(SDE PGDB���p)\n"
			  << "  FC_TARGET   ���o�Ώۃ��C����\n"
			  << "  FC_{CITY|BASE|MIDDLE}MESH   ���o�ΏۃX�P�[���̃��b�V���t�B�[�`���N���X��" << std::endl;
}

// ������
bool CShowInfo::init( int argc, char* argv[] )
{
	if( argc < 5 )
	{
		std::cerr << "#ERROR �����̐�������܂���(�K�{�I�v�V���� -l or -m �� -o)" << std::endl;
		return false;
	}

	for( int i = 1; i < argc-1; ++i )
	{
		if( strcmp(argv[i], "-m") == 0 )				// �P���b�V��
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( strcmp(argv[i], "-l") == 0 )			// ���b�V�����X�g
		{
			if( !loadMeshList(argv[++i]) )
			{
				std::cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
		}
		else if( strcmp(argv[i], "-o") == 0 )	// ���O�t�@�C��
		{
			// ��Ƀt�@�C�����݃`�F�b�N
			m_bIsAddMode = _access(argv[++i], 0) != -1;
			int nOpenMode = m_bIsAddMode? std::ios::out|std::ios::app : std::ios::out;
			m_fout.open( static_cast<CT2CW>(argv[i]), nOpenMode );
			if( !m_fout )
			{
				std::cerr << "#ERROR ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
			m_fout.precision(12);
		}
		else if( strcmp(argv[i], "-i") == 0 )	// �o�̓t�B�[���h�ݒ�t�@�C��
		{
			// �ݒ�t�@�C�����[�h
			if( !loadInitFile(argv[++i]) )
			{
				std::cerr << "#ERROR �ݒ�t�@�C���̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
		}
		else if( strcmp(argv[i], "-w") == 0 )
			m_strWhereClause = argv[++i];	// Where��
		else if( strcmp(argv[i], "-c") == 0 )
			m_emLogMode = kCount;			// �I�u�W�F�N�g�J�E���g���[�h
		else if( strcmp(argv[i], "-nodup") == 0 )
			m_bIsNoDupMode = true;
		else
		{
			std::cerr << "#ERROR �s���ȃI�v�V�����ł� : " << argv[i] << std::endl;
			return false;
		}
	}

	// ��ԍŌ��-c or -nodup���w�肵�Ă���\�����l��(������Ƌ���)
	if( strcmp(argv[argc-1], "-c") == 0 )
		m_emLogMode = kCount;
	else if( strcmp(argv[argc-1], "-nodup") == 0 )
		m_bIsNoDupMode = true;

	if( m_bIsNoDupMode && gf::IsPGDBMode() )
	{
		std::cerr << "#ERROR -nodup �I�v�V�����́APGDB�łł͖����ł��B" << std::endl;
		return false;
	}

	if( !m_fout.is_open() )
	{
		std::cerr << "#ERROR �o�͐���w�肵�Ă�������( -o �I�v�V����)" << std::endl;
		return false;
	}

	if( m_vecMesh.empty() )
	{
		std::cerr << "#ERROR �`�F�b�N���郁�b�V�����ݒ肳��Ă��܂���" << std::endl;
		return false;
	}

	return getEnv();
}

// ���s
bool CShowInfo::execute()
{
	for( std::vector<long>::const_iterator itr = m_vecMesh.begin(); itr != m_vecMesh.end(); ++itr )
	{
		// �c�a������擾
		if( ! setFromDB(*itr) )
		{
			return false;
		}

		// ���񃁃b�V�����w�b�_�[�o��
		if( itr == m_vecMesh.begin() )
		{
			if( ! printHeader() )
			{
				std::cerr << "#ERROR �w�b�_���o�͂Ɏ��s�B���b�V���R�[�h : " << *itr << std::endl;
				return false;
			}
		}

		// �w�胁�b�V���̃t�B�[�`�����擾
		IFeaturePtr ipMeshFeature;
		if( ! getMeshFeature( *itr, &ipMeshFeature ) )
		{
			std::cerr << "#ERROR ���b�V���t�B�[�`���擾�Ɏ��s�B���b�V���R�[�h : " << *itr << std::endl;
			return false;
		}
		if( NULL == ipMeshFeature )
		{
			std::cout << *itr << " : ���̃��b�V���R�[�h�̃��b�V���͂���܂���" << std::endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_ShapeCopy( &ipMeshGeo );
		if( NULL == ipMeshGeo )
		{
			std::cerr << "#ERROR �}�`�̎擾�Ɏ��s�B���b�V���R�[�h : " << *itr << std::endl;
			return false;
		}

		switch( m_emLogMode )
		{
		case kNormal:
			if( ! execNormalLog( *itr, ipMeshGeo ) )
				return false;
			break;
			
		case kCount:
			if( ! execObjectCount( *itr, ipMeshGeo ) )
				return false;
			break;
		}
	}

	return true;
}

// ���b�V�����X�g�ǂݍ���
bool CShowInfo::loadMeshList( LPCTSTR lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return false;
	std::copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

// �ݒ�t�@�C���ǂݍ���
bool CShowInfo::loadInitFile( LPCTSTR lpcszFile )
{
	if( kCount == m_emLogMode )
	{
		std::cerr << "�I�u�W�F�N�g�J�E���g���[�h�̂��߁A�ݒ�t�@�C���ǂݍ��݂͖������܂�" << std::endl;
		return true;
	}

	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return false;

	while( !ifs.eof() )
	{
		TCHAR szBuff[1024] = {0};
		ifs.getline( szBuff, sizeof(szBuff), '\n' );
		CString str( szBuff );

		// �R�����g�܂��́A���s�����̎�����
		if( str.IsEmpty() || str.Find('#') == 0 )
			continue;

		m_vecField.push_back( str.MakeUpper() );	// �t�B�[���h�x�N�^�ɂ��i�[(�啶���ϊ�)
	}

	return true;
}

// ���ϐ��擾
bool CShowInfo::getEnv()
{
#ifdef _DEBUG
	_tputenv( _T("DB_ANY=ronly/ronly/SDE.Current08A/5151/ruby2") );
//	_tputenv( _T("DB_ANY=data") );
	_tputenv( _T("FC_TARGET=SiNDY2008A.City_Annotation") );
	_tputenv( _T("FC_CITYMESH=REFERENCE.CityMesh") );
	_tputenv( _T("FC_BASEMESH=REFERENCE.BaseMesh") );
	_tputenv( _T("FC_MIDDLEMESH=REFERENCE.MiddleMesh") );
#endif _DEBUG

	// �ڑ��v���p�e�B
	if( !m_strDBProp.GetEnvironmentVariable(_T("DB_ANY")) )
	{
		std::cerr << "#ERROR ���ϐ� DB_ANY �̎擾�Ɏ��s" << std::endl;
		return false;
	}
	// ���o�Ώۃt�B�[�`���N���X��
	if( !m_strTargetFCName.GetEnvironmentVariable(_T("FC_TARGET")) )
	{
		std::cerr << "#ERROR ���ϐ� FC_TARGET �̎擾�Ɏ��s" << std::endl;
		return false;
	}
	// ���b�V���t�B�[�`���N���X��
	CString strEnvName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strEnvName = _T("FC_MIDDLEMESH");
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strEnvName = _T("FC_BASEMESH");
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strEnvName = _T("FC_CITYMESH");
	else
	{
		std::cerr << "#ERROR ���b�V�����X�g��1�s�ڂ��s���ȃ��b�V���ł��B�X�P�[��������ł��܂��� : " << m_vecMesh[0] << std::endl;
		return false;
	}

	if( !m_strMeshFCName.GetEnvironmentVariable(strEnvName) )
	{
		std::cerr << "#ERROR ���ϐ�" << CT2CA(strEnvName) << " �̎擾�Ɏ��s" << std::endl;
		return false;
	}

	return true;
}

// �c�a����K�v���擾
bool CShowInfo::setFromDB( long lMeshcode )
{
	if( !gf::IsPGDBMode() )
	{
		if( m_ipWorkspace )		return true;

		if( NULL == (m_ipWorkspace = gf::GetWorkspace(m_strDBProp)) )
		{
			std::cerr << "#ERROR SDE�ڑ��Ɏ��s : " << CT2CA(m_strDBProp) << std::endl;
			return false;
		}

		std::cout << "�ySDE�ڑ����z : " << CT2CA(m_strDBProp) << std::endl;
	}
	else
	{
		// ��Ƀ��b�V���R�[�h����APGDB�t�@�C�������擾
		CString strPGDBFile;
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_strDBProp, lMeshcode/10000, lMeshcode );

		if( NULL == (m_ipWorkspace = gf::GetWorkspace(strPGDBFile)) )
		{
			std::cerr << "#ERROR PGDB�t�@�C���̃I�[�v���Ɏ��s : " << CT2CA(strPGDBFile) << std::endl;
			return false;
		}
	}

	IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( CComBSTR(m_strTargetFCName), &m_ipTargetFC );
	if( NULL == m_ipTargetFC )
	{
		std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(m_strTargetFCName) << std::endl;
		return false;
	}
	// �^�[�Q�b�g�t�B�[�`���̌`��^�C�v���擾���Ă���
	if( FAILED(m_ipTargetFC->get_ShapeType( &m_esriTargetGeoType )) )
	{
		std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X�̐}�`�^�C�v�̎擾�Ɏ��s : " << CT2CA(m_strTargetFCName) << std::endl;
		return false;
	}

	IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( CComBSTR(m_strMeshFCName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
	{
		std::cerr << "#ERROR ���b�V���t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(m_strMeshFCName) << std::endl;
		return false;
	}

	// �t�B�[���hID�}�b�v�쐬
	m_mapFieldID.clear();
	for( std::vector<CString>::const_iterator itr = m_vecField.begin(); itr != m_vecField.end(); ++itr )
	{
		long lFieldID = -1;
		if( SUCCEEDED(m_ipTargetFC->FindField(CComBSTR(*itr), &lFieldID)) && lFieldID != -1 )
		{
			m_mapFieldID.insert( std::pair<CString, long>(*itr, lFieldID) );
		}
		else
		{
			std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���Ƀt�B�[���h�����݂��Ȃ� : " << CT2CA(*itr) << std::endl;
			return false;
		}
	}

	return true;
}

// �w�b�_�[���o��
bool CShowInfo::printHeader()
{
	// �J�E���g���[�h�Ȃ�o�͂��Ȃ�
	if( m_emLogMode != kNormal )
		return true;

	// �ǋL���[�h�Ȃ�o�͂��Ȃ����h���C���}�b�v�̂ݎ擾
	if( ! m_bIsAddMode ) {

		m_fout << "#SINDYSTDLOG\n"
			   << "#�Ή�����\t���C����\tOID\tX(�o�x)\tY(�ܓx)\t�G���[���x��\t�G���[�R�[�h\t�G���[���b�Z�[�W\t���b�V���R�[�h\t";

		// �ݒ�t�B�[���h��Ȃ�R�����g�o�͂��Ă����܂�
		if( m_vecField.empty() )
			m_fout << "�t���[�R�����g" << std::endl;
	}

	// ���ɐݒ�t�B�[���h�̃G�C���A�X�����w�b�_�[�ɕ\��
	IFieldsPtr ipFields;
	//�h���C�������擾���邽�߂Ƀt�B�[���h����擾(���ł�����)
	if( FAILED(m_ipTargetFC->get_Fields( &ipFields )) )
	{
		std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃t�B�[���h�̎擾�Ɏ��s" << std::endl;
		return false;
	}

	long lFieldCount = 0;
	if( FAILED(ipFields->get_FieldCount( &lFieldCount )) )
	{
		std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃t�B�[���h�����̎擾�Ɏ��s" << std::endl;
		return false;
	}
	
	for( std::vector<CString>::const_iterator itr = m_vecField.begin(); itr != m_vecField.end(); ++itr )
	{
		for( long i = 0; i < lFieldCount; i++ )
		{
			BSTR bstrField;
			IFieldPtr ipField;
			if( FAILED(ipFields->get_Field(i, &ipField)) )
			{
				std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃t�B�[���h�̎擾�Ɏ��s : index=" << i << std::endl;
				return false;
			}
			if( FAILED(ipField->get_Name( &bstrField )) )
			{
				std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃t�B�[���h���̎擾�Ɏ��s : index=" << i << std::endl;
				return false;
			}
			if( *itr == CString(bstrField).MakeUpper() )	// �啶���ɂ��낦�Ĕ�r
			{
				BSTR bstrAlias;
				if( FAILED( ipField->get_AliasName( &bstrAlias )) )
				{
					std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃G���A�X���̎擾�Ɏ��s : index=" << i << std::endl;
					return false;
				}
				// �ǋL���[�h�Ȃ�o�͂��Ȃ�
				if( ! m_bIsAddMode ) {
					m_fout << COLE2T(bstrAlias) << "\t";
				}

				// �h���C���}�b�vvector�������ō쐬���Ă��܂�
				IDomainPtr ipDomain;
				if( FAILED(ipField->get_Domain( &ipDomain )) )
				{
					std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃h���C���̎擾�Ɏ��s : index=" << i << std::endl;
					return false;
				}
				DOMAINMAP map;
				if( ! gf::GetDomainMap(m_ipTargetFC, *itr, &map) )
				{
					std::cerr << "#ERROR �^�[�Q�b�g�t�B�[�`���N���X���̃h���C���}�b�v�̎擾�Ɏ��s : index=" << i << std::endl;
					return false;
				}
				m_mapDM.insert( std::pair<CString, DOMAINMAP>(*itr, map) );
				::SysFreeString( bstrAlias );
			}
			::SysFreeString( bstrField );
		}
	}
	// �ǋL���[�h�Ȃ�o�͂��Ȃ�
	if( ! m_bIsAddMode ) {
		m_fout << "�t���[�R�����g" << std::endl;
	}
	return true;
}

// �W���o�̓��[�h�������C��
bool CShowInfo::execNormalLog( long lMeshcode, IGeometry* ipMeshGeo )
{		
	// �w�胁�b�V�����ɂ��菮����Where��Ŏw�肵�����̂Ɉ�v����t�B�[�`�����擾(��ԁ{��������)
	long lFeatureCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipMeshGeo, m_ipTargetFC, ipFeatureCursor, &lFeatureCount,
									  esriSpatialRelIntersects, m_strWhereClause ))
	{
		std::cerr << "#ERROR ��ԁE���������Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
		return false;
	}
	if( lFeatureCount > 0 )
	{
		// �d�������Ȃ����[�h+�|�C���g�Ȃ�΁A���̏d���Ώۂ̃I�u�W�F�N�g�h�c�t�B�[�`�����Ɏ擾����
		std::set<long>	setDupID;
		if( m_bIsNoDupMode && m_esriTargetGeoType == esriGeometryPoint )
		{
			if( ! makeDupIDSet( lMeshcode, ipMeshGeo, setDupID ) )
			{
				std::cerr << "#ERROR ���b�V�����E��I�u�W�F�N�g�h�c�ꗗ�쐬�Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
				return false;
			}
		}

		// �\��
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			if( setDupID.size() > 0 )
			{
				long lOID;
				if( FAILED(ipFeature->get_OID( &lOID )) )
				{
					std::cerr << "#ERROR OID�擾�Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
					return false;
				}
				if( setDupID.find(lOID) != setDupID.end() )
					continue;
			}

			if( ! printData( ipFeature, lMeshcode ) )
			{
				std::cerr << "#ERROR �f�[�^�o�͂Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
				return false;
			}
		}

		std::cout << lMeshcode << " : �o�͊���" << std::endl;
	}
	else
		std::cout << lMeshcode << " : �w�肵�������Ƀt�B�[�`���͂���܂���" << std::endl;

	return true;
}

// �I�u�W�F�N�g�J�E���g����
bool CShowInfo::execObjectCount( long lMeshcode, IGeometry* ipMeshGeo )
{
	// �w�胁�b�V�����ɂ��菮����Where��Ŏw�肵�����̂Ɉ�v����t�B�[�`�����擾(��ԁ{��������)
	long lFeatureCount =gf::SelectByShapeOnlyCount( ipMeshGeo, m_ipTargetFC, esriSpatialRelIntersects, m_strWhereClause );
	if( lFeatureCount < 0 )
	{
		std::cerr << "#ERROR ��ԁE���������̌����擾�Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
		return false;
	}

	// �d�������Ȃ����[�h(�|�C���g�����Ή�����)
	if( m_bIsNoDupMode && m_esriTargetGeoType == esriGeometryPoint )
	{
		int objCount = checkOnMeshObject( lMeshcode, ipMeshGeo );
		if( objCount < 0 )
		{
			std::cerr << "#ERROR ���b�V�����E��I�u�W�F�N�g�̌����擾�Ɏ��s : ���b�V���R�[�h=" << lMeshcode << std::endl;
			return false;
		}
		lFeatureCount -= objCount;
	}
	m_fout << lMeshcode << "\t" << lFeatureCount << std::endl;	// ���b�V�����I�u�W�F�N�g���\��

	std::cout << lMeshcode << " : �o�͊���" << std::endl;
	return true;
}

// ���b�V�����E��I�u�W�F�N�g�h�c�ꗗ�쐬(�W�����O���[�h���Ɏg��)
bool CShowInfo::makeDupIDSet( long lMeshcode, IGeometry* ipBaseMeshGeo, std::set<LONG>& setID )
{
	ITopologicalOperatorPtr ipTopoOpe( ipBaseMeshGeo );
	if( ! ipTopoOpe )
	{
		std::cerr << "#ERROR TopologicalOperator�����Ɏ��s" << std::endl;
		return false;
	}

	IGeometryPtr ipBound;
	ipTopoOpe->get_Boundary( &ipBound );
	if( ! ipBound )
	{
		std::cerr << "#ERROR ���E�擾�Ɏ��s" << std::endl;
		return false;
	}

	long lOnMeshCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipBound, m_ipTargetFC, ipFeatureCursor, &lOnMeshCount,
									  esriSpatialRelIntersects, m_strWhereClause ) )
	{
		std::cerr << "#ERROR ��ԁE���������Ɏ��s" << std::endl;
		return false;
	}
	if( lOnMeshCount > 0 )
	{
		IPointCollectionPtr ipMeshPointCol( ipBaseMeshGeo );
		IPointPtr ipMeshLUPoint;
		// ���b�V���������W�擾
		if( FAILED(ipMeshPointCol->get_Point( 0, &ipMeshLUPoint )) )
		{
			std::cerr << "#ERROR �|�C���g�R���N�V�����擾�Ɏ��s" << std::endl;
			return false;
		}
		double dLUX, dLUY;
		if( FAILED(ipMeshLUPoint->QueryCoords( &dLUX, &dLUY )) )
		{
			std::cerr << "#ERROR �|�C���g�R���N�V������X,Y���W�擾�Ɏ��s" << std::endl;
			return false;
		}

		int dDupCount = 0;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			// �I�u�W�F�N�g�h�c�擾
			long lOID;
			if( FAILED(ipFeature->get_OID( &lOID )) )
			{
				std::cerr << "#ERROR OID�擾�Ɏ��s" << std::endl;
				return false;
			}

			// �擾�����t�B�[�`���̃|�C���g�����b�V���̉�or�����Ȃ�Ώۃ��b�V�����ł̓J�E���g���Ȃ�
			IGeometryPtr ipGeo;
			if( FAILED(ipFeature->get_ShapeCopy( &ipGeo )) )
			{
				std::cerr << "#ERROR �}�`�擾�Ɏ��s" << std::endl;
				return false;
			}
			IPointPtr ipPoint(ipGeo);
			if( NULL == ipPoint )
			{
				std::cerr << "#ERROR �}�`�擾(�|�C���g)�Ɏ��s" << std::endl;
				return false;
			}

			double dX, dY;
			if( FAILED(ipPoint->QueryCoords( &dX, &dY )) )
			{
				std::cerr << "#ERROR �|�C���g��X,Y���W�擾�Ɏ��s" << std::endl;
				return false;
			}

			if( dLUX == dX && dLUY == dY && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, -1, -1)) )
				setID.insert( lOID );
			else if( dLUX == dX && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, -1, 0)) )
				setID.insert( lOID );
			else if( dLUY == dY && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, 0, -1)) )
				setID.insert( lOID );
		}
	}

	return true;
}

// ���b�V�����E��I�u�W�F�N�g�`�F�b�N(�I�u�W�F�N�g�J�E���g���g�p)
int CShowInfo::checkOnMeshObject( long lMeshcode, IGeometry* ipBaseMeshGeo )
{
	ITopologicalOperatorPtr ipTopoOpe( ipBaseMeshGeo );
	if( ! ipTopoOpe )
	{
		std::cerr << "#ERROR TopologicalOperator�����Ɏ��s" << std::endl;
		return -1;
	}

	IGeometryPtr ipBound;
	ipTopoOpe->get_Boundary( &ipBound );
	if( ! ipBound )
	{
		std::cerr << "#ERROR ���E�擾�Ɏ��s" << std::endl;
		return -1;
	}

	long lOnMeshCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipBound, m_ipTargetFC, ipFeatureCursor, &lOnMeshCount,
									  esriSpatialRelIntersects, m_strWhereClause ) )
	{
		std::cerr << "#ERROR ��ԁE���������Ɏ��s" << std::endl;
		return -1;
	}
	if( lOnMeshCount > 0 )
	{
		IPointCollectionPtr ipMeshPointCol( ipBaseMeshGeo );
		IPointPtr ipMeshLUPoint;
		// ���b�V���������W�擾
		if( FAILED(ipMeshPointCol->get_Point( 0, &ipMeshLUPoint )) )
		{
			std::cerr << "#ERROR �|�C���g�R���N�V�����擾�Ɏ��s" << std::endl;
			return -1;
		}
		double dLUX, dLUY;
		if( FAILED(ipMeshLUPoint->QueryCoords( &dLUX, &dLUY )) )
		{
			std::cerr << "#ERROR �|�C���g�R���N�V������X,Y���W�擾�Ɏ��s" << std::endl;
			return -1;
		}

		int nDupCount = 0;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			// �擾�����t�B�[�`���̃|�C���g�����b�V���̉�or�����Ȃ�Ώۃ��b�V�����ł̓J�E���g���Ȃ�
			IGeometryPtr ipGeo;
			if( FAILED(ipFeature->get_ShapeCopy( &ipGeo )) )
			{
				std::cerr << "#ERROR �}�`�擾�Ɏ��s" << std::endl;
				return -1;
			}
			IPointPtr ipPoint(ipGeo);
			if( NULL == ipPoint )
			{
				std::cerr << "#ERROR �}�`�擾(�|�C���g)�Ɏ��s" << std::endl;
				return -1;
			}

			double dX, dY;
			if( FAILED(ipPoint->QueryCoords( &dX, &dY )) )
			{
				std::cerr << "#ERROR �|�C���g��X,Y���W�擾�Ɏ��s" << std::endl;
				return -1;
			}

			if( dLUX == dX && dLUY == dY && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, -1, -1)) )
				++nDupCount;
			else if( dLUX == dX && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, -1, 0)) )
				++nDupCount;
			else if( dLUY == dY && isExistMesh(m_cCrdCnv.GetMeshCode(lMeshcode, 0, -1)) )
				++nDupCount;
		}
		return nDupCount;
	}

	return 0;
}

// ���b�V���t�B�[�`���擾
bool CShowInfo::getMeshFeature( LONG lMeshcode, IFeature** ipFeature )
{
	CString strWhere;
	strWhere.Format( _T("MESHCODE=%d"), lMeshcode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	if( ! ipQuery )
		return false;

	if( FAILED(ipQuery->put_WhereClause( CComBSTR(strWhere) )) )
		return false;

	IFeatureCursorPtr ipFeatureCursor;
	
	if( FAILED(m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor )) )
		return false;

	if( NULL == ipFeatureCursor || ipFeatureCursor->NextFeature(ipFeature) != S_OK )
		return false;
	
	return true;
}

// �|�C���g���\��
bool CShowInfo::getDispPoint( IFeature* ipFeature, WKSPoint& point )
{
	IGeometryPtr ipGeom;
	if( FAILED(ipFeature->get_ShapeCopy( &ipGeom )) )
		return false;

	IPointPtr ipPoint;
		
	// �`�󂲂Ƃɕ\���|�C���g�擾
	esriGeometryType esriGeoType;
	if( FAILED(ipGeom->get_GeometryType( &esriGeoType )) )
		return false;

	if( esriGeoType == esriGeometryPoint )
	{
		ipPoint = ipGeom;
	}
	else if( esriGeoType == esriGeometryPolygon )	// �|���S���Ȃ�A�d�S�\��
	{
		IAreaPtr ipArea(ipGeom);
		if( FAILED(ipArea->get_Centroid( &ipPoint )) )	// [Bug5934] LabelPoint��Centroid�ɕύX
			return false;
	}
	else if( esriGeoType == esriGeometryPolyline )	// ���C���Ȃ�A�擪�̓_��\��
	{
		IPointCollectionPtr ipPointCol( ipGeom );
		if( ! ipPointCol )
			return false;

		if( FAILED(ipPointCol->get_Point( 0, &ipPoint )) )
			return false;
	}

	if( ipPoint )
	{
		if( FAILED(ipPoint->QueryCoords( &(point.X), &(point.Y) )) )
			return false;
	}
	return true;
}

// �f�[�^�\��
bool CShowInfo::printData( IFeature* ipFeature, long lMeshcode )
{
	// OID�擾
	long lOID = 0;
	if( FAILED(ipFeature->get_OID( &lOID )) )
	{
		std::cerr << "#ERROR OID�擾�Ɏ��s" << std::endl;
		return false;
	}

	// �\���|�C���g�擾
	WKSPoint point;
	if( ! getDispPoint( ipFeature, point ))
	{
		std::cerr << "#ERROR �\���|�C���g���擾�Ɏ��s" << std::endl;
		return false;
	}

	m_fout << 0 << "\t" << m_strTargetFCName << "\t" << lOID << "\t" << point.X << "\t" << point.Y << "\t"	// �Ή��󋵁E���C�����EOID�E�o�x�E�ܓx
		   << "\t\t\t" << lMeshcode << "\t";	// �G���[���x���E�G���[�R�[�h�E�G���[���b�Z�[�W�E���b�V���R�[�h

	for( std::vector<CString>::const_iterator itrF = m_vecField.begin(); itrF != m_vecField.end(); ++itrF )
	{
		CComVariant vaValue;
		FIELDMAP::const_iterator itr = m_mapFieldID.find( *itrF );
		if( itr != m_mapFieldID.end() )
			if( FAILED(ipFeature->get_Value( itr->second, &vaValue )) )
			{
				std::cerr << "#ERROR �l�̎擾�Ɏ��s" << std::endl;
				return false;
			}
						
		// �h���C������΃h���C����\���E�Ȃ���΃t�B�[���h�l��\��
		std::map<CString, DOMAINMAP>::const_iterator itrDM = m_mapDM.find( itr->first );
		if( itrDM == m_mapDM.end() )
			m_fout << VariantToString( vaValue ) << "\t";
		else
		{
			DOMAINMAP::const_iterator itrDomain = itrDM->second.find( vaValue.lVal );
			if( itrDomain != itrDM->second.end() )
				m_fout << itrDomain->second << "\t";
			else
				m_fout << VariantToString(vaValue) << "\t";
		}
	}

	m_fout << std::endl;	//�t���[�R�����g

	return true;
}

// CComVariant����string�^�ɕϊ�
const CString VariantToString( CComVariant vaTarget )
{
	switch( vaTarget.vt )
	{
	case VT_NULL:		return _T("");
	case VT_EMPTY:		return _T("");
	case VT_UNKNOWN:	return _T("");
	default:
		{
			vaTarget.ChangeType( VT_BSTR );
			CString strRet( vaTarget.bstrVal );
			::SysFreeString( vaTarget.bstrVal );
			return strRet;
		}
	}
}
