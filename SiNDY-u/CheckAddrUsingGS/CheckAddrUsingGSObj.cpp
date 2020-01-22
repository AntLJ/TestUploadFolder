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
#include "CheckAddrUsingGSObj.h"
#include <sindy/workspace.h>
#include <SiNDYLib/Feature.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;
using namespace boost::program_options;
using namespace sindy;

CCheckAddrUsingGSObj::CCheckAddrUsingGSObj(void)
{
	OStartup();
}

CCheckAddrUsingGSObj::~CCheckAddrUsingGSObj(void)
{
	m_cDBSourceJDB.Close();
	m_cDBSourceJGDC.Close();
	OShutdown();
}

bool CCheckAddrUsingGSObj::Init( int argc, _TCHAR* argv[] )
{
	bool bIsOK = true;

	if(! CheckArg( argc, argv ) )
		return false;

	// SDE�ڑ��i�Z���j
	CWorkspace cWorkspaceAdr;
	if( cWorkspaceAdr.Connect( m_strConnectAdr ) )
	{
		// ���|�C���g�t�B�[�`���N���X
		m_cFCGouPoint.SetObject( cWorkspaceAdr.OpenTable( schema::gou_point::kTableName ) );
		if( m_cFCGouPoint )
		{
			m_cFMGouPoint = m_cFCGouPoint.GetFieldMap();
			m_cTNGouPoint = m_cFCGouPoint.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL �t�B�[�`���N���X���J���܂��� : ") + CString(schema::gou_point::kTableName) );
			bIsOK = false;
		}

		// �s�s�s���E�t�B�[�`���N���X
		m_cFCCityAdmin.SetObject( cWorkspaceAdr.OpenTable( schema::city_admin::kTableName ) );
		if( m_cFCCityAdmin )
		{
			m_cFMCityAdmin = m_cFCCityAdmin.GetFieldMap();
			m_cTNCityAdmin = m_cFCCityAdmin.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL �t�B�[�`���N���X���J���܂��� : ") + CString(schema::city_admin::kTableName) );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL �ڑ��ł��܂��� : ") + m_strConnectAdr );
		bIsOK = false;
	}

	// SDE�ڑ��i�ƌ`�j
	CWorkspace cWorkspaceBld;
	if( cWorkspaceBld.Connect( m_strConnectBld ) )
	{
		// �ƌ`�|���S���t�B�[�`���N���X
		m_cFCBuilding.SetObject( cWorkspaceBld.OpenTable( schema::building::kTableName ) );
		if( m_cFCBuilding )
		{
			m_cFMBuilding = m_cFCBuilding.GetFieldMap();
			m_cTNBuilding = m_cFCBuilding.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL �t�B�[�`���N���X���J���܂��� : ") + CString(schema::building::kTableName) );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL �ڑ��ł��܂��� : ") + m_strConnectBld );
		bIsOK = false;
	}

	// SDE�ڑ��iGEOSPACE�j
	CWorkspace cWorkspaceGeo;
	if( cWorkspaceGeo.Connect( m_strConnectGeo ) )
	{
		// GEOSPACE�ƌ`�|���S���t�B�[�`���N���X
		m_cFCBuildingGeo.SetObject( cWorkspaceGeo.OpenTable( _T("G0013") ) );
		if( m_cFCBuildingGeo )
		{
			m_cFMBuildingGeo = m_cFCBuildingGeo.GetFieldMap();
			m_cTNBuildingGeo = m_cFCBuildingGeo.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL �t�B�[�`���N���X���J���܂��� : G0013") );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL �ڑ��ł��܂��� : ") + m_strConnectGeo );
		bIsOK = false;
	}

	// �Z��DB�ڑ�
	if(! ConnectJDB( m_strConnectJDB,	m_cDBSourceJDB	) ) { bIsOK = false; }
	if(! ConnectJDB( m_strConnectJGDC,	m_cDBSourceJGDC	) ) { bIsOK = false; }

	return bIsOK;
}

bool CCheckAddrUsingGSObj::Execute()
{
	_TCHAR aBuf[64];
	int	nCount = 0;
	int nTotal = m_listOID_GouPoint.size();

	// ���|�C���g�̃��[�v
	foreach( int nOID, m_listOID_GouPoint )
	{
		CLogInfo cLogInfo( nOID );

		// �i���\��
		nCount++;
		if( nCount % 100 == 0 || nCount == nTotal )
		{
			CString strTmp;
			strTmp.Format( _T("[ %8ld / %8ld ] ���ڂ��`�F�b�N��...\r"), nCount, nTotal );
			cout << CT2A(strTmp);
		}

		// OID�ɑΉ����鍆�|�C���g���擾
		IFeaturePtr ipF_GouPoint = m_cFCGouPoint.GetFeature( nOID );
		if(! ipF_GouPoint )
		{
			WriteLogInfo( cLogInfo, _T("�Ή����鍆�|�C���g��������܂���") );
			continue;
		}

		// ���|�C���g�����擾
		CFeature cF_GouPoint( ipF_GouPoint, sindy::sindyTableType::gou_point, false, m_cFMGouPoint, m_cTNGouPoint );
		CString	strGouNo	= cF_GouPoint.GetStringValueByFieldName( sindy::schema::gou_point::kGouNo, 64, aBuf );	// ���ԍ�
		long	lGouType	= cF_GouPoint.GetLongValueByFieldName( sindy::schema::gou_point::kGouType,  0 );		// ���^�C�v
		long	lExpGouNo	= cF_GouPoint.GetLongValueByFieldName( sindy::schema::gou_point::kExpGouNo, 0 );		// �g�����ԍ��t���O

		// ���Y�ʒu�̍s���E
		IFeaturePtr ipF_CityAdmin = SearchAdmin( ipF_GouPoint );
		if(! ipF_CityAdmin )
		{
			WriteLogInfo( cLogInfo, _T("�Ή�����s���E�|���S����������܂���") );
			continue;
		}

		// �s���E����11���R�[�h���擾
		CFeature cF_CityAdmin( ipF_CityAdmin, sindy::sindyTableType::city_admin, false, m_cFMCityAdmin, m_cTNCityAdmin );
		CString strAddrCode =
			CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kCityCode, 64, aBuf ) ) +
			CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kAddrCode, 64, aBuf ) );

		// 11���R�[�h�����n���R�[�h�ɕϊ�
		CString strAddrCodeJGDC = ConvAddrCode2JGDC( strAddrCode );

		// �s���E����X�敄�����擾
		// �܂��͊g���X�敄���i������j���擾
		CString strGaikuFugo = CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kExtGaikuFugo, 64, aBuf ) );
		if( strGaikuFugo.IsEmpty() )
		{
			// �Ȃ���ΊX�敄���i���l�j���擾��������
			long aGaikuFugo = cF_CityAdmin.GetLongValueByFieldName( sindy::schema::city_admin::kGaikuFugo, -1 );
			if( aGaikuFugo > 0 )
			{
				strGaikuFugo.Format( _T("%ld"), aGaikuFugo );
			}
		}
		::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, strGaikuFugo, -1, aBuf, 64 );	// �S�p��
		strGaikuFugo = aBuf;

		// �n�ԁE��������𐶐�
		CString strChibanGou = ( strGouNo == _T("��") ) ? strGaikuFugo : ( ( strGaikuFugo.IsEmpty() || lExpGouNo ) ? strGouNo : ( strGaikuFugo + _T("�|") + strGouNo ) );
		::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, strChibanGou, -1, aBuf, 64 );	// �S�p��
		strChibanGou = aBuf;

		// �Z�������񐶐�
		cLogInfo.e_strAddrName = GetAddrName( strAddrCode ) + strChibanGou;

		// ���Y�ʒu��iPC�ƌ`���擾
		IFeaturePtr ipF_Building = SearchBuilding( ipF_GouPoint );
		if(! ipF_Building )
		{
			// �ƌ`���Ȃ�
			WriteLogInfo( cLogInfo, _T("���|�C���g���܂މƌ`������܂���") );
			continue;
		}

		// �ƌ`��GeospaceID���擾
		CFeature cF_Building( ipF_Building, sindy::sindyTableType::building, false, m_cFMBuilding, m_cTNBuilding );
		CString strGeospaceID = cF_Building.GetStringValueByFieldName( sindy::schema::building::kGeospaceID, 64, aBuf );	strGeospaceID.TrimRight();
		if( strGeospaceID.IsEmpty() )
		{
			// GeospaceID���U���Ă��Ȃ�
			WriteLogInfo( cLogInfo, _T("GeospaceID���ƌ`�ɕt�^����Ă��܂���") );
			continue;
		}

		// iPC�ƌ`�ɕt�^����Ă���GeospaceID��GEOSPACE�ƌ`������
		IFeaturePtr ipF_BuildingGeo = SearchBuildingGeo( strGeospaceID );
		if(! ipF_BuildingGeo )
		{
			// GEOSPACE�ƌ`���Ȃ�
			WriteLogInfo( cLogInfo, _T("GeospaceID�ɑΉ�����GEOSPACE�ƌ`������܂���") );
			continue;
		}

		// GEOSPACE�ƌ`�̏Z�����擾
		CFeature cF_BuildingGeo( ipF_BuildingGeo, sindy::sindyTableType::unknown, false, m_cFMBuildingGeo, m_cTNBuildingGeo );
		CString strAddrCodeGeo	= cF_BuildingGeo.GetStringValueByFieldName( _T("ADCD"), 64, aBuf );		strAddrCodeGeo.TrimRight();
		CString	strPrefNameGeo	= cF_BuildingGeo.GetStringValueByFieldName( _T("Kanj_Tod"), 64, aBuf );	strPrefNameGeo.TrimRight();
		CString	strCityNameGeo	= cF_BuildingGeo.GetStringValueByFieldName( _T("Kanj_Shi"), 64, aBuf );	strCityNameGeo.TrimRight();
		CString	strAdr1NameGeo	= cF_BuildingGeo.GetStringValueByFieldName( _T("Kanj_Ooa"), 64, aBuf );	strAdr1NameGeo.TrimRight();
		CString	strAdr2NameGeo	= cF_BuildingGeo.GetStringValueByFieldName( _T("Kanj_Aza"), 64, aBuf );	strAdr2NameGeo.TrimRight();
		CString	strAddress1		= cF_BuildingGeo.GetStringValueByFieldName( _T("Address1"), 64, aBuf );	strAddress1.TrimRight();
		CString	strAddress2		= cF_BuildingGeo.GetStringValueByFieldName( _T("Address2"), 64, aBuf );	strAddress2.TrimRight();
		CString	strAddress3		= cF_BuildingGeo.GetStringValueByFieldName( _T("Address3"), 64, aBuf );	strAddress3.TrimRight();
		if( strAddrCodeGeo.IsEmpty() )
		{
			// �Z���i11���R�[�h�j���Ȃ�
			WriteLogInfo( cLogInfo, _T("GEOSPACE�ƌ`��11���Z���R�[�h������܂���") );
			continue;
		}
		CString strChibanGouGeo;
		if(! strAddress1.IsEmpty() )
		{
			strChibanGouGeo = strAddress1;
			if(! strAddress2.IsEmpty() )
			{
				strChibanGouGeo += _T("�|") + strAddress2;
				if(! strAddress3.IsEmpty() )
				{
					strChibanGouGeo += _T("�|") + strAddress3;
				}
			}
		}
		cLogInfo.e_strAddrNameGeo = strPrefNameGeo + strCityNameGeo + strAdr1NameGeo + strAdr2NameGeo + strChibanGouGeo;
		cLogInfo.e_strAddrNameGeo.Remove( _T('�@') );

		// 11���Z���R�[�h���r
		if( strAddrCodeJGDC != strAddrCodeGeo )
		{
			// �Z�����Ⴄ
			WriteLogInfo( cLogInfo, _T("11���Z���R�[�h�i���n���j���Ⴂ�܂�") );
			continue;
		}

		// �n�Ԉȍ~���r
		if( strChibanGou != strChibanGouGeo )
		{
			// �Z�����Ⴄ
			WriteLogInfo( cLogInfo, _T("�n�Ԉȍ~�Z�������񂪈Ⴂ�܂�") );
			continue;
		}

		// �Z������v
		WriteLogInfo( cLogInfo, _T("OK") );

	}

	cout << endl;

	m_bIsOK = true;
	return true;
}

void CCheckAddrUsingGSObj::WriteLogHeader()
{
	WriteLog( _T("#SINDYSTDLOG"), false );
}

bool CCheckAddrUsingGSObj::CheckArg( int argc, _TCHAR* argv[] )
{
	bool bIsOK = true;

	options_description option("�I�v�V����");
	option.add_options()
		( "connect_adr",	value<string>(),	"SDE�ڑ�������i�Z���j" )
		( "connect_bld",	value<string>(),	"SDE�ڑ�������i�ƌ`�j" )
		( "connect_geo",	value<string>(),	"SDE�ڑ�������iGEOSPACE�j" )
		( "connect_jdb",	value<string>(),	"�Z��DB�ڑ�������i�Z��������擾�j" )
		( "connect_jgdc",	value<string>(),	"�Z��DB�ڑ�������i���������n���e�[�u���j" )
		( "log_input",		value<string>(),	"�Ð삳��c�[���o�̓��O�t�@�C����" )
		( "log_output",		value<string>(),	"�o�̓��O�t�@�C����" )
		( "help,?",								"�w���v��\��" );
	store( parse_command_line( argc, argv, option ), m_variable_map );
	notify( m_variable_map );

	// �w���v�\��
	if( m_variable_map.count("help") || m_variable_map.size() == 0 )
	{
		cout << option << endl;
		return false;
	}

	// ���O�t�@�C��
	if( m_variable_map.count("log_output") )
	{
		m_strLogOutput = m_variable_map["log_output"].as<string>().c_str();
		if(! OpenLogFile( m_strLogOutput ) ) { bIsOK = false; }
	}
	else
	{
		WriteLog( _T("#FATAL ���O�t�@�C�����̎w�肪�K�v�ł��B--log_output �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// ���̓t�@�C��
	if( m_variable_map.count("log_input") )
	{
		m_strLogInput = m_variable_map["log_input"].as<string>().c_str();
		if(! OpenInputFile( ) ) { bIsOK = false; }
	}
	else
	{
		WriteLog( _T("#FATAL ���̓t�@�C�����̎w�肪�K�v�ł��B--log_input �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// SDE�ڑ�������i�Z���j
	if( m_variable_map.count("connect_adr") )
	{
		m_strConnectAdr = m_variable_map["connect_adr"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE�ڑ�������i�Z���j�̎w�肪�K�v�ł��B--connect_adr �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// SDE�ڑ�������i�ƌ`�j
	if( m_variable_map.count("connect_bld") )
	{
		m_strConnectBld = m_variable_map["connect_bld"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE�ڑ�������i�Z���j�̎w�肪�K�v�ł��B--connect_bld �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// SDE�ڑ�������iGEOSPACE�j
	if( m_variable_map.count("connect_geo") )
	{
		m_strConnectGeo = m_variable_map["connect_geo"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE�ڑ�������iGEOSPACE�j�̎w�肪�K�v�ł��B--connect_geo �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// �Z��DB�ڑ�������i�Z��������擾�j
	if( m_variable_map.count("connect_jdb") )
	{
		m_strConnectJDB = m_variable_map["connect_jdb"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL �Z��DB�ڑ�������i�Z��������擾�j�̎w�肪�K�v�ł��B--connect_jdb �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	// �Z��DB�ڑ�������i���������n���e�[�u���j
	if( m_variable_map.count("connect_jgdc") )
	{
		m_strConnectJGDC = m_variable_map["connect_jgdc"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL �Z��DB�ڑ�������i���������n���e�[�u���j�̎w�肪�K�v�ł��B--connect_jgdc �I�v�V�������w�肵�Ă��������B") );
		bIsOK = false;
	}

	return bIsOK;
}

bool CCheckAddrUsingGSObj::OpenInputFile()
{
	ifstream fInput;

	fInput.open( m_strLogInput, ios::in );
	if(! fInput.is_open() )
	{
		WriteLog( _T("#FATAL ���̓t�@�C�����J���܂���ł����B : ") + m_strLogInput );
		return false;
	}

	string strTmp;
	while( fInput && getline( fInput, strTmp ) )
	{
		CString strBuf( CA2T( strTmp.c_str() ) );
		if( strBuf.IsEmpty() || strBuf[0] == _T('#') ) continue;

		std::vector< CString > vecFields;
		Split( strBuf, vecFields );						// ���R�[�h���t�B�[���h��ɕ���
		if( vecFields[2] == _T("OBJECTID") ) continue;	// �w�b�_�s�̏ꍇ�̓X�L�b�v
		long lOID = _ttol( vecFields[2] );				// OID���擾
		if( lOID <= 0 )
		{
			WriteLog( _T("#FATAL ���̓t�@�C������OID����擾�ł��܂���ł��� : ") + m_strLogInput );
			return false;
		}
		// �R�����g���u�ړ������v�i�u�ړ������i�����j�v�������j�̏ꍇ��OID�����X�g�ɒǉ�
		if( vecFields[23].Left( 4 ) == _T("�ړ�����") && vecFields[23] != _T("�ړ������i�����j") )
		{
			m_listOID_GouPoint.push_back( lOID );
		}
	}

	return true;
}

void CCheckAddrUsingGSObj::Split(const CString& strBuf, std::vector< CString >& vecFields )
{
	int nBeg = 0;
	while( 1 )
	{
		int nEnd = strBuf.Find( _T('\t'), nBeg );
		CString strTmp = ( nEnd == -1 ) ? strBuf.Mid( nBeg ) : strBuf.Mid( nBeg, nEnd - nBeg );
		vecFields.push_back( strTmp );
		if( nEnd == -1 ) break;
		nBeg = nEnd + 1;
	}
}

void CCheckAddrUsingGSObj::WriteLogInfo( const CLogInfo& cLogInfo, const CString& strMsg )
{
	CString strTmp;
	strTmp.Format( _T("0\t%s\t%ld\t\t\tERROR\t\t%s\t%s\t%s"), sindy::schema::gou_point::kTableName, cLogInfo.e_lOID, strMsg, cLogInfo.e_strAddrName, cLogInfo.e_strAddrNameGeo );
	WriteLog( strTmp, false );
}

IFeaturePtr CCheckAddrUsingGSObj::SearchPolygon( IPointPtr ipPoint, CFeatureClass cFeatureClass )
{
	assert( ipPoint );
	assert( cFeatureClass );

	ISpatialFilterPtr ipSpFilter( AheInitSpatialFilter( nullptr, ipPoint, _T("Shape"), esriSearchOrderSpatial, esriSpatialRelWithin, nullptr ) );

	long lCount = cFeatureClass.FeatureCount( ipSpFilter );
	if( lCount == 1 )
	{
		IFeatureCursorPtr ipFeatureCursor = cFeatureClass.Search( ipSpFilter, VARIANT_FALSE );

		IFeaturePtr ipFeature;
		if( ipFeatureCursor && ( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) ) )
		{
			return ipFeature;
		}
	}

	return nullptr;
}

bool CCheckAddrUsingGSObj::ConnectJDB( const CString& strConnect, ODatabase& cDB )
{
	int nPos1 = strConnect.Find( _T('/') );
	int nPos2 = strConnect.Find( _T('@'), nPos1 );
	if( nPos1 == -1 || nPos2 == -1 )
	{
		WriteLog( _T("#FATAL �ڑ������񂪕s���ł� : ") + strConnect );
		return false;
	}
	else
	{
		CString strUser		= strConnect.Mid( 0, nPos1 );
		CString strPass		= strConnect.Mid( nPos1 + 1, nPos2 - nPos1 - 1 );
		CString strDBName	= strConnect.Mid( nPos2 + 1 );
		if( OSUCCESS != cDB.Open( CT2CA(strDBName), CT2CA(strUser), CT2CA(strPass) ) )
		{
			WriteLog( _T("#FATAL �ڑ��ł��܂��� : ") + strConnect );
			return false;
		}
	}

	return true;
}

IFeaturePtr CCheckAddrUsingGSObj::SearchAdmin( IFeaturePtr ipF_Point )
{
	assert( ipF_Point );

	IGeometryPtr ipG_Point;
	ipF_Point->get_Shape( &ipG_Point );

	IFeaturePtr ipF_CityAdmin = SearchPolygon( ipG_Point, m_cFCCityAdmin );

	return ipF_CityAdmin;
}

IFeaturePtr CCheckAddrUsingGSObj::SearchBuilding( IFeaturePtr ipF_Point )
{
	assert( ipF_Point );

	IGeometryPtr ipG_Point;
	ipF_Point->get_Shape( &ipG_Point );

	IFeaturePtr ipF_Building = SearchPolygon( ipG_Point, m_cFCBuilding );

	return ipF_Building;
}

IFeaturePtr CCheckAddrUsingGSObj::SearchBuildingGeo( const CString& strGeospaceID )
{
	IQueryFilterPtr ipQFilter( AheInitQueryFilter( nullptr, nullptr, _T("B_FID = '%s'"), strGeospaceID ) );

	IFeatureCursorPtr ipFeatureCursor = m_cFCBuildingGeo.Search( ipQFilter, VARIANT_FALSE );

	IFeaturePtr ipFeature;
	if( ipFeatureCursor && ( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) ) )
	{
		return ipFeature;
	}

	return nullptr;
}

CString CCheckAddrUsingGSObj::ConvAddrCode2JGDC( const CString& strAddrCode )
{
	map<CString, CString>::const_iterator itr = m_mapConvAddr.find( strAddrCode );

	if( itr == m_mapConvAddr.end() )
	{
		// �L���b�V���ɂȂ��ꍇ �� DB���猟��
		// ������₪����ꍇ�͂Ƃ肠�����ŏ��l�Ƃ��Ă��邪�c
		CString strQuery = _T("select min(kokuti_code) from kajyo2kokuti_list where kajyo_code = '") + strAddrCode + _T("'");

		ODynaset cDynaset( m_cDBSourceJGDC, CT2A(strQuery) );
		long lCount = cDynaset.GetRecordCount();
		if( lCount > 0 )
		{
			OValue cValue;
			cDynaset.GetFieldValue( 0, &cValue );
			CString strRet = CA2T( (LPCSTR)cValue ); 
			m_mapConvAddr[strAddrCode] = strRet;
			return strRet;
		}
	}
	else
	{
		// �L���b�V���ɂ���ꍇ �� �L���b�V���̓��e��Ԃ�
		return itr->second;
	}

	return _T("00000000000");
}

CString CCheckAddrUsingGSObj::GetAddrName( const CString& strAddrCode )
{
	map<CString, CString>::const_iterator itr = m_mapAddrName.find( strAddrCode );

	if( itr == m_mapAddrName.end() )
	{
		// �L���b�V���ɂȂ��ꍇ
		CString strQuery;
		strQuery.Format( _T("select replace( ken_kanji || shi_kanji || oaza_kanji || aza_kanji, '�@') from areacode_master where ken_code = '%s' and shi_code = '%s' and oaza_code = '%s' and aza_code = '%s'"),
			strAddrCode.Mid(0, 2), strAddrCode.Mid(2, 3), strAddrCode.Mid(5, 3), strAddrCode.Mid(8, 3) );

		ODynaset cDynaset( m_cDBSourceJDB, CT2A(strQuery) );
		long lCount = cDynaset.GetRecordCount();
		if( lCount > 0 )
		{
			OValue cValue;
			cDynaset.GetFieldValue( 0, &cValue );
			CString strRet = CA2T( (LPCSTR)cValue ); 
			m_mapAddrName[strAddrCode] = strRet;
			return strRet;
		}
	}
	else
	{
		// �L���b�V���ɂ���ꍇ �� �L���b�V���̓��e��Ԃ�
		return itr->second;
	}

	return _T("");
}
