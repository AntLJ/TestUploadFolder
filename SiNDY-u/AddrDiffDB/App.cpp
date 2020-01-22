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

#include <io.h>
#include <sindy/workspace.h>

// ���[�N�X�y�[�X�擾
inline IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
{
	INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
	if( NULL == ipName )
		return NULL;
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	return ipUnknown;
}

CApp::CApp(void) : 
	m_bIsAddMode(false),
	m_bGroupMeshMode(false)
{
}

CApp::~CApp(void)
{
}

// �g�p�@
void CApp::printUsage()
{
	std::cerr << "�y20���Z���R�[�h�������o�c�[���z\n"
			  << "(Usage)AddrDiffDB.exe [�e��I�v�V����]\n"
	  		  << "�E�I�v�V�����ɂ���\n"
			  << "���`�F�b�N���郁�b�V���֘A�I�v�V����(�����ꂩ�K�{)\n"
			  << "  -m  <���b�V���R�[�h>\n"
 			  << "  -l  <���b�V�����X�g>\n"
			  << "  -lg <���b�V�����X�g> ... ���b�V�����X�g���̃��b�V�����P�̃O���[�v�Ƃ��č�������郂�[�h\n"
			  << "�����O�o�͐�I�v�V����(�K�{)\n"
			  << "  -o  <�o�̓t�@�C����>\n"
			  << "����������Z��11���R�[�h���X�g(�C��)\n"
			  << "  -n  <11���Z���R�[�h���X�g>\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  PGDB_MODE		     SiNDY-c�p�`�F�b�N���[�h ON/OFF(�����ݒ肳��Ă���΁AON�Ɣ��f����)\n"
			  << "  DB_ADDRESS_NEW     �c�a�ڑ��v���p�e�B(�V) SDE or PGDB\n"
			  << "  FC_CITY_ADMIN_NEW  �s�s�n�}�s���E�t�B�[�`���N���X��(�V)\n"
			  << "  FC_GOU_POINT_NEW   ���|�C���g�t�B�[�`���N���X��(�V)\n"
			  << "  DB_ADDRESS_OLD     �c�a�ڑ��v���p�e�B(��) SDE or PGDB\n"
			  << "  FC_CITY_ADMIN_OLD  �s�s�n�}�s���E�t�B�[�`���N���X��(��)\n"
			  << "  FC_GOU_POINT_OLD   ���|�C���g�t�B�[�`���N���X��(��)\n"	
			  << "  FC_CITYMESH        �s�s�n�}���b�V���t�B�[�`���N���X��\n"
			  << std::endl;
}

// ������
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( argc < 5 ){
		std::cerr << "#ERROR �����̐�������������܂���" << std::endl;
		return false;
	}

	// �I�v�V�������
	for( int i=1;i<argc-1;i++ )
	{
		if( _tcscmp(argv[i], _T("-m")) == 0 )		// �P���b�V��
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( _tcscmp(argv[i], _T("-l")) == 0 || _tcscmp(argv[i], _T("-lg")) == 0 )	// ���b�V�����X�g
		{
			if( _tcscmp(argv[i], _T("-lg")) == 0 )
				m_bGroupMeshMode = true;

			if( !loadMeshList(argv[++i]) )
			{
				std::cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
		}		
		else if( _tcscmp(argv[i], _T("-o")) == 0 ){	// ���O�t�@�C��

			// ��Ƀt�@�C�����݃`�F�b�N
			m_bIsAddMode = (_access(argv[++i], 0) != -1)? true : false;
			int nOpenMode = m_bIsAddMode? ios::out|ios::app : ios::out;

			m_fout.open( static_cast<CT2CW>(argv[i]), nOpenMode );
			if( !m_fout ){
				std::cerr << "#ERROR ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
			m_fout.precision(12);
		}
		else if( _tcscmp(argv[i], _T("-n")) == 0 ){	// OK�R�[�h���X�g

			if( !loadOKAddrCodeList(argv[++i]) ){
				std::cerr << "#ERROR OK�R�[�h���X�g�̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
		}
		else{
			std::cerr << "#ERROR �s���ȃI�v�V���� : " << argv[i] << std::endl;
			return false;
		}
	}

	if( !m_fout.is_open() ){
		std::cerr << "#ERROR �o�͐���w�肵�Ă�������( -o �I�v�V����)" << std::endl;
		return false;
	}

	return getEnv();	// ���ϐ��擾
}

// ���s
bool CApp::execute()
{
	// ���O�w�b�_�[�o��
	printHeader();

	CODEMAP NewCodeMap, OldCodeMap;
	for( m_itrMesh=m_vecMesh.begin();m_itrMesh!=m_vecMesh.end();++m_itrMesh )
	{
		// �c�a������擾
		if( !setFromDB(*m_itrMesh) ){
			std::cerr << *m_itrMesh << " : DB����K�v�ȏ�񂪎擾�ł��܂���ł���" << std::endl;
			std::cout << *m_itrMesh << " : DB����K�v�ȏ�񂪎擾�ł��܂���ł���" << std::endl;
			continue;
		}

		// ���b�V���W�I���g���擾
		IFeaturePtr ipMeshFeature = getMeshFeature( *m_itrMesh );
		if( NULL == ipMeshFeature ){
			std::cerr << "#ERROR ���b�V���t�B�[�`���̎擾�Ɏ��s : " << *m_itrMesh << std::endl;
			std::cout << "#ERROR ���b�V���t�B�[�`���̎擾�Ɏ��s : " << *m_itrMesh << std::endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_Shape( &ipMeshGeo );

		makeCodeList( m_ipNewCAFC, m_ipNewGouFC, ipMeshGeo, *m_itrMesh, m_ipNewWorkspace, NewCodeMap );
		makeCodeList( m_ipOldCAFC, m_ipOldGouFC, ipMeshGeo, *m_itrMesh, m_ipOldWorkspace, OldCodeMap );

		// 1���b�V�����Ƃɍ������ꍇ�́A�����Œ��o
		if( !m_bGroupMeshMode )
		{
			findDeleteCode( NewCodeMap, OldCodeMap );	// �폜�Z���R�[�h���o
			NewCodeMap.clear();
			OldCodeMap.clear();
			std::cout << *m_itrMesh << " : �`�F�b�N����" << std::endl;
		}
		else
			std::cout << *m_itrMesh << " : �Z���R�[�h���o����" << std::endl;
	}

	// ���b�V�����X�g���O���[�v�����č������ꍇ�A�����Œ��o
	if( m_bGroupMeshMode )
	{
		std::cout << "�������o���J�n���܂� ... ";
		findDeleteCode( NewCodeMap, OldCodeMap );
		std::cout << "����" << std::endl;
	}

	return true;
}

/////////////////////////////////
//	�����֐�
/////////////////////////////////

// ���ϐ�
bool CApp::getEnv()
{
	CString strError;

	if( NULL == (m_lpszNewDBProp = _tgetenv(_T("DB_ADDRESS_NEW"))) )
		strError += _T("#ERROR ���ϐ� DB_ADDRESS_NEW ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszOldDBProp = _tgetenv(_T("DB_ADDRESS_OLD"))) )
		strError += _T("#ERROR ���ϐ� DB_ADDRESS_OLD ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszNewCAFName = _tgetenv(_T("FC_CITY_ADMIN_NEW"))) )
		strError += _T("#ERROR ���ϐ� FC_CITY_ADMIN_NEW ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszOldCAFName = _tgetenv(_T("FC_CITY_ADMIN_OLD"))) )
		strError += _T("#ERROR ���ϐ� FC_CITY_ADMIN_OLD ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszNewGouFName = _tgetenv(_T("FC_GOU_POINT_NEW"))) )
		strError += _T("#ERROR ���ϐ� FC_GOU_POINT_NEW ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszOldGouFName = _tgetenv(_T("FC_GOU_POINT_OLD"))) )
		strError += _T("#ERROR ���ϐ� FC_GOU_POINT_OLD ���ݒ肳��Ă��Ȃ�\n");
	if( NULL == (m_lpszMeshFName = _tgetenv(_T("FC_CITYMESH"))) )
		strError += _T("#ERROR ���ϐ� FC_CITYMESH ���ݒ肳��Ă��Ȃ�\n");

	if( !strError.IsEmpty() ){
		std::cerr << strError << std::endl;
		return false;
	}
	else
		return true;
}

// ���b�V�����X�g�ǂݍ���
bool CApp::loadMeshList( const char* lpszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	std::copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

// OK�R�[�h���X�g
bool CApp::loadOKAddrCodeList( LPCTSTR lpszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;

	while( !ifs.eof() )
	{
		std::string str;
		std::getline( ifs, str );
		if( str.empty() )
			continue;

		m_setOKCode.insert( str.c_str() );
	}

	return true;
}

// �c�a����K�v���擾
bool CApp::setFromDB( LONG lMeshcode )
{
	if( !isPGDBMode() )
	{
		if( m_ipNewWorkspace && m_ipOldWorkspace )	
			return true;

		if( NULL == (m_ipNewWorkspace = getWorkspace(m_lpszNewDBProp)) )
		{
			cerr << "#ERROR �ڑ��Ɏ��s(�V) : " << m_lpszNewDBProp << endl;
			return false;
		}
		if( NULL == (m_ipOldWorkspace = getWorkspace(m_lpszOldDBProp)) )
		{
			cerr << "#ERROR �ڑ��Ɏ��s(��) : " << m_lpszOldDBProp << endl;
			return false;
		}

		cout << "�ySDE�ڑ����(�V)�z : " << m_lpszNewDBProp << endl;
		cout << "�ySDE�ڑ����(��)�z : " << m_lpszOldDBProp << endl;
	}
	else
	{
		// ��Ƀ��b�V���R�[�h����APGDB�t�@�C�������擾
		CString strPGDBFile;
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_lpszNewDBProp, lMeshcode/10000, lMeshcode );
		if( NULL == (m_ipNewWorkspace = getWorkspace(strPGDBFile)) )
		{
			cerr << "#ERROR PGDB�t�@�C��(�V)�̃I�[�v���Ɏ��s : " << strPGDBFile << endl;
			return false;
		}
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_lpszOldDBProp, lMeshcode/10000, lMeshcode );
		if( NULL == (m_ipOldWorkspace = getWorkspace(strPGDBFile)) )
		{
			cerr << "#ERROR PGDB�t�@�C��(��)�̃I�[�v���Ɏ��s : " << strPGDBFile << endl;
			return false;
		}
	}

	CString strError;

	// �s���E�E���|�C���g
	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszNewCAFName), &m_ipNewCAFC );
	if( NULL == m_ipNewCAFC )
		strError.AppendFormat( _T("#ERROR �s�s�n�}�s���E(�V)�t�B�[�`���N���X�̎擾�Ɏ��s : %s\n"), m_lpszNewCAFName );

	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszNewGouFName), &m_ipNewGouFC );
	if( NULL == m_ipNewGouFC )
		strError.AppendFormat( _T("#ERROR ���|�C���g(�V)�t�B�[�`���N���X�̎擾�Ɏ��s : %s\n"), m_lpszNewGouFName );

	((IFeatureWorkspacePtr)m_ipOldWorkspace)->OpenFeatureClass( CComBSTR(m_lpszOldCAFName), &m_ipOldCAFC );
	if( NULL == m_ipOldCAFC )
		strError.AppendFormat( _T("#ERROR �s�s�n�}�s���E(��)�t�B�[�`���N���X�̎擾�Ɏ��s : %s\n"), m_lpszOldCAFName );

	((IFeatureWorkspacePtr)m_ipOldWorkspace)->OpenFeatureClass( CComBSTR(m_lpszOldGouFName), &m_ipOldGouFC );
	if( NULL == m_ipOldGouFC )
		strError.AppendFormat( _T("#ERROR ���|�C���g(��)�t�B�[�`���N���X�̎擾�Ɏ��s : %s\n"), m_lpszOldGouFName );

	// ���b�V���t�B�[�`���N���X�擾
	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszMeshFName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
		strError.AppendFormat( _T("#ERROR ���b�V���t�B�[�`���N���X�̎擾�Ɏ��s : %s\n"), m_lpszMeshFName );

	if( !strError.IsEmpty() ){
		std::cerr << strError << std::endl;
		return false;
	}
	else{
		// �t�B�[���h�C���f�b�N�X�擾
		m_cCityAdminFID.set(m_ipNewCAFC);
		m_cGouPointFID.set(m_ipNewGouFC);
		return true;
	}

}

// ���b�V���t�B�[�`���擾
IFeaturePtr CApp::getMeshFeature( LONG lMeshcode )
{
	CString strWhere;

	if( !isPGDBMode() )	strWhere.Format( _T("MESHCODE=%d"), lMeshcode );
	else			strWhere.Format( _T("OBJECTID IS NOT NULL") );

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause( CComBSTR(strWhere) );
	
	IFeatureCursorPtr ipFeatureCursor;
	IFeaturePtr ipFeature;
	
	m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );

	if( NULL == ipFeatureCursor || ipFeatureCursor->NextFeature(&ipFeature) != S_OK )
		return NULL;
	
	return ipFeature;
}

// 20���Z���R�[�h���X�g�쐬
void CApp::makeCodeList( IFeatureClass* ipAdminFC, IFeatureClass* ipGouFC, IGeometry* ipMeshGeo, long lMesh, IWorkspace* ipWorkspace, CODEMAP& codeMap )
{
	// ToDo:�v�ڍג���
	// PGDB���Ώۂ̏ꍇ�ɋ�ԃL���b�V�����g����Workspace���J������Ȃ��Ȃ�A��ʂɊJ�����ꍇ��
	// ��O������������ۂ��̂�SDE���[�h����ɂ���ibug 9304, 9127�j
	ISpatialCacheManagerPtr ipCache(ipWorkspace);
	if( !isPGDBMode() )
	{
		IEnvelopePtr ipEnv;
		ipMeshGeo->get_Envelope( &ipEnv );
		if( FAILED(ipCache->FillCache(ipEnv)) )
		{
			std::cerr << "#ERROR �L���b�V���̍\�z�Ɏ��s : " << lMesh << std::endl;
		}
	}

	IFeatureCursorPtr ipAdminCursor;
	long lAdminCount = SelectByShapeWithCount( ipMeshGeo, ipAdminFC, &ipAdminCursor, esriSpatialRelIntersects );

	if( !isPGDBMode() )
	{
		ipCache->EmptyCache();
	}

	if( lAdminCount > 0 )
	{
		CString strWhereGou;
		strWhereGou.Format( _T("%s=1"), gou_point::kGouType );
		CString strSubFields;
		strSubFields.Format( _T("%s,%s"), gou_point::kGouNo, gou_point::kExpGouNo );
		IFeaturePtr ipAdminFeature;
		while( ipAdminCursor->NextFeature(&ipAdminFeature) == S_OK )
		{
			CCityAdmin cCityAdmin( ipAdminFeature, m_cCityAdminFID );

			// [Bug 6426]�X�敄����Null�������ς݂Ȃ疳��
			if( cCityAdmin.m_lGaikuFugo == 0 
				&& (cCityAdmin.m_lAddrClass == city_admin::addr_class::kChibanMaintenance || cCityAdmin.m_lAddrClass == city_admin::addr_class::kResidenceIndicationMaintenance) )
				continue;

			IGeometryPtr ipAdminGeo( cCityAdmin.getGeo() );

			// SDE���[�h�Ȃ�΁A���b�V���ŃN���b�v���Ă���
			if( !isPGDBMode() )
			{
				ITopologicalOperatorPtr ipTopoOpe( ipAdminGeo );
				IEnvelopePtr ipEnv;
				ipMeshGeo->get_Envelope( &ipEnv );
				ipTopoOpe->Clip( ipEnv );
			}

			// �s���E���̍��|�C���g���擾
			IFeatureCursorPtr ipGouCursor;
			SelectByShapeWithCount( ipAdminGeo, ipGouFC, &ipGouCursor,  esriSpatialRelIntersects, !isPGDBMode()? strWhereGou : NULL, strSubFields );
			if( ipGouCursor )
			{				
				IFeaturePtr ipGouFeature;
				while( ipGouCursor->NextFeature(&ipGouFeature) == S_OK )
				{
					CGouPoint cGouPoint(ipGouFeature, m_cGouPointFID);
					cGouPoint.m_lMeshCode = *m_itrMesh;					// �O���[�v�����[�h���K�v�Ȃ̂ŁA��ɐݒ肷�邱�Ƃɂ���
					CString strAddrCode = getAddrCode( cCityAdmin, cGouPoint );	// �s���E�E���|�C���g����20���Z���R�[�h����
					if( 20 == strAddrCode.GetLength() )
					{
						codeMap.insert( std::pair<CString, CGouPoint>(strAddrCode, cGouPoint) );
					}
				}
			}
		}
	}
}

// 20���Z���R�[�h����
CString CApp::getAddrCode( const CCityAdmin& cCityAdmin, const CGouPoint& cGouPoint )
{
	CString strAddrCode(_T(""));

	// �g�����ԍ��t���O�`�F�b�N���A�s���E����ǂ̃��x���܂Ŏ擾���邩���߂�
	if( 1 == cGouPoint.m_lExGouNoF )
		strAddrCode = cCityAdmin.getAddrCode(11);	/// �X��́A���|�C���g����擾
	else{
		if( 0 == cCityAdmin.m_lGaikuFugo )
			strAddrCode = cCityAdmin.getAddrCode(11);
		else
			strAddrCode = cCityAdmin.getAddrCode(16);
	}

	// ���ԍ�����A�X��ȉ��̃R�[�h���쐬
	CString strGouNo( cGouPoint.m_strGouNo );
	if( -1 != strGouNo.Find('#') )
		filterGouNo(strGouNo);

	int nHaihunSPos = strGouNo.Find('-');
	int nHaihunEPos = strGouNo.Find('-', nHaihunSPos+1);
	int nCurCodeLevel = strAddrCode.GetLength();
	if( 11 != nCurCodeLevel && 16 != nCurCodeLevel )
		return _T("");

	CString strGaiku(_T("")), strGou(_T(""));
	int nGaiku = 0, nGou = 0;

	if( -1 != nHaihunSPos && -1 == nHaihunEPos ){	// �u-�v�������Ă���1�����̂Ƃ�
		
		switch(nCurCodeLevel)
		{
		case 11:
			nGaiku = _ttoi( strGouNo.Left(nHaihunSPos) );
			nGou = _ttoi( strGouNo.Mid(nHaihunSPos+1) );
			if( nGaiku > 99999 || nGou > 9999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%05d%04d"), nGaiku, nGou );
			break;
		case 16:
			nGou = _ttoi(strGouNo.Left(nHaihunSPos));
			if( nGou > 9999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%04d"), nGou );
			break;
		}
	}
	else if( nHaihunSPos != nHaihunEPos ){	// �u-�v��2�ȏ゠��Ƃ�

		switch( nCurCodeLevel )
		{
		case 11:
			nGaiku = _ttoi( strGouNo.Left(nHaihunSPos) );
			nGou = _ttoi( strGouNo.Mid(nHaihunSPos+1, nHaihunEPos-nHaihunSPos-1) );
			if( nGaiku > 99999 || nGou > 9999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%05d%04d"), nGaiku, nGou );
			break;
		case 16:
			nGou = _ttoi( strGouNo.Left(nHaihunSPos) );
			if( nGou > 9999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%04d"), nGou );
			break;
		}
	}
	else{	// �u-�v���Ȃ��Ƃ�

		switch( nCurCodeLevel )
		{
		case 11:
			nGaiku = _ttoi( strGouNo );
			if( nGaiku > 99999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%05d0000"), nGaiku );
			break;
		case 16:
			nGou = _ttoi( strGouNo );
			if( nGou > 9999 )
				return _T("");
			strAddrCode.AppendFormat( _T("%04d"), nGou );
			break;
		}
	}

	return strAddrCode;
}

// �폜�R�[�h����
void CApp::findDeleteCode( const CODEMAP& NewCodeList, const CODEMAP& OldCodeList )
{
	// ���R�[�h���X�g�����ɍ폜����Ă��邩������
	for( CODEMAP::const_iterator itrM=OldCodeList.begin();itrM!=OldCodeList.end();++itrM )
	{
		// OK11���Z���R�[�h���X�g�Ȃ�΃`�F�b�N���Ȃ�
		if( !m_setOKCode.empty() )
		{
			std::set<CString>::const_iterator itrOK = m_setOKCode.find( itrM->first.Left(11) );
			if( itrOK != m_setOKCode.end() )
				continue;
		}

		CODEMAP::const_iterator itrFind = NewCodeList.find( itrM->first );
		if( itrFind == NewCodeList.end() )
		{
			/**
			 * [Bug 5449]
			 * �}�ԂȂ����|�C���g���}�Ԃ����|�C���g�֕ύX����Ă�����͍̂폜�Ƃ��Ȃ�
			 * Ex) ���|�C���g�u101�v���u101-2�v�֕ύX
			 */
			bool bFindBranch = false;
			if( itrM->first.Mid(16) == _T("0000") )	/// ���ԍ���[0000]�̃p�^�[��
			{
				for( CODEMAP::const_iterator itrNew = NewCodeList.begin(); itrNew != NewCodeList.end(); ++itrNew )
				{
					if( itrM->first.Left(16) == itrNew->first.Left(16) )
					{
						bFindBranch = true;
						break;
					}
				}
			}

			if( !bFindBranch )
				printDeleteGou( itrM->second, itrM->first );	// �폜���ꂽ�Z���R�[�h(���|�C���g)
		}
	}
}

// ���ԍ�����A�s�v�ȕ�������
void CApp::filterGouNo( CString& strGou )
{
	CString strTmp(_T(""));

	int nStrLen = strGou.GetLength();

	// �����Ɓu-�v�ȊO�́A����
	for( int i=0;i<nStrLen;i++ )
	{
		if( _istdigit(strGou[i]) || '-' == strGou[i] )
			strTmp += strGou[i];
	}

	// ������u-�v�ɂȂ��Ă�����A����폜�u#454-�C�v�Ƃ��̂Ƃ��l������
	if( strTmp.Find(_T("-")) == strTmp.GetLength()-1 )
		strTmp = strTmp.Left(strTmp.GetLength()-1 );

	strGou = strTmp;
}

// �폜���ꂽ�|�C���g�o��
void CApp::printDeleteGou( const CGouPoint& cGouPoint, const CString& strAddrCode )
{
	m_fout << "0\t" << m_lpszOldGouFName << "\t" << cGouPoint.getOID() << "\t";

	IPointPtr ipPoint( cGouPoint.getGeo() );
	if( NULL != ipPoint )
	{
		WKSPoint point;
		ipPoint->QueryCoords( &(point.X), &(point.Y) );
		m_fout << point.X << "\t" << point.Y << "\t";
	}
	else
		m_fout << "\t\t";

	m_fout << "\t\t���|�C���g���폜����܂���\t" << strAddrCode << "\t" << cGouPoint.m_strGouNo << "\t" << cGouPoint.m_lMeshCode << std::endl;
}

////////////////////////////////////////
//	�ėp�֐�
////////////////////////////////////////

// ��ԁE��������(�������擾��)
LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// �N�G���t�B���^�쐬
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where��w�肳��Ă�����A�Z�b�g����
	if( lpszWhereClause )
	{
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
	}

	// SubFields�w�肳��Ă�����A�Z�b�g����
	if( lpszSubFields )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// �������ʂ� 0 �Ȃ烊�^�[��
	long lHitCount = 0;
	if( isPGDBMode() )
	{
		// PGDB����ԃL���b�V����Ɍ��������FeatureCount���x�����̉���R�[�h
		_ICursorPtr ipCursor;
		((ITablePtr)ipFeatureClass)->Search( ipFilter, VARIANT_FALSE, &ipCursor );
		if( ipCursor )
		{
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK ){ ++lHitCount; }
		}
	}
	else
		ipFeatureClass->FeatureCount( ipFilter, &lHitCount );

	if( lHitCount < 1 )
		return 0;

	/// ����
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	if( NULL == *ppFeatureCursor )
		return 0;
			
	return lHitCount;
}
