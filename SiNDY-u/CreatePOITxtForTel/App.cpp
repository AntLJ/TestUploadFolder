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

#include "sindy/workspace.h"
#include "sindy/schema/map.h"
#include "sindy/schema/sindyk.h"
#include "crd_cnv/coord_converter.h"
#include "WinLib/VersionInfo.h"
#include "TDC/useful_headers/str_util.h"

static crd_cnv g_cCrdCnv;
using namespace sindy::schema;
using namespace std;

namespace gf
{
	// ���[�N�X�y�[�X�擾
	inline IWorkspacePtr GetWorkspace( const CString& dbConnectString, const CString& message )
	{
		INamePtr ipName( sindy::create_workspace_name(dbConnectString) );
		if( !ipName )
		{
			wcerr << L"#Error " << CT2CW(message) << L"�ւ�SDE�ڑ��Ɏ��s : " << CT2CW(dbConnectString) << endl;
			return nullptr;
		}

		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		if( !ipUnknown )
		{
			wcerr << L"#Error " << CT2CW(message) << L"�ւ�SDE�ڑ��Ɏ��s : " << CT2CW(dbConnectString) << endl;
		}
		else
		{
			wcerr << L"#" << CT2CW(message) << L"SDE�ڑ� : " << CT2CW(dbConnectString) << endl;
		}
		return ipUnknown;
	}

	// �t�B�[�`���N���X�擾
	inline IFeatureClassPtr GetFeatureClass( const IWorkspacePtr& ipWorkspace, const CString& featureClassName )
	{
		IFeatureClassPtr ipFeatureClass;
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(featureClassName), &ipFeatureClass );
		if( !ipFeatureClass )
		{
			wcerr << L"#Error �t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CW(featureClassName) << endl;
		}
		return ipFeatureClass;
	}

	// (�|�C���g)�t�B�[�`������MESHXY���擾
	inline void GetMeshXY( const IFeaturePtr& feature, int& mesh, int& x, int& y )
	{
		IGeometryPtr ipGeo;
		feature->get_Shape( &ipGeo ); // �{���̓|�C���g���ǂ����`�F�b�N�����ق����ǂ�
		WKSPoint wksPoint;
		((IPointPtr)ipGeo)->QueryCoords( &wksPoint.X, &wksPoint.Y );
		g_cCrdCnv.LLtoMesh( wksPoint.X, wksPoint.Y, 2, &mesh, &x, &y, 1 );
	}

	// ���݂̎���
	inline CString GetCurTime()
	{
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		return strTime;
	}
}

// ������
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	CArguments arg;
	if( !arg.parse( argc, argv ) )
	{
		return false;
	}

	// SDE�ڑ�
	IWorkspacePtr ipPOIWorkspace( gf::GetWorkspace(arg.m_poiDB.c_str(), _T("POI�f�[�^")) );
	if( ! ipPOIWorkspace )
	{
		return false;
	}

	IWorkspacePtr ipAddrWorkspace( gf::GetWorkspace(arg.m_addrDB.c_str(), _T("�Z���f�[�^")) );
	if( ! ipAddrWorkspace )
	{
		return false;
	}

	IWorkspacePtr ipMapWorkspace( gf::GetWorkspace(arg.m_mapDB.c_str(), _T("�n�}�f�[�^")) );
	if( ! ipMapWorkspace )
	{
		return false;
	}

	// POI_POINT�擾
	m_ipPOIPointFC = gf::GetFeatureClass( ipPOIWorkspace, arg.m_poiPoint.c_str() );
	if( ! m_ipPOIPointFC )
	{
		return false;
	}
	
	// POI_SUB_POINT�擾
	m_ipPOISubPointFC = gf::GetFeatureClass( ipPOIWorkspace, arg.m_poiSubPoint.c_str() );
	if( ! m_ipPOISubPointFC )
	{
		return false;
	}

	// CITY_ADMIN�擾
	m_ipCityAdminFC = gf::GetFeatureClass( ipAddrWorkspace, arg.m_cityAdmin.c_str() );
	if( ! m_ipCityAdminFC )
	{
		return false;
	}

	// BUILDING�擾
	m_ipBuildingFC = gf::GetFeatureClass( ipMapWorkspace, arg.m_building.c_str() );
	if( ! m_ipBuildingFC )
	{
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	if( FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kContentsCode), &m_lContentsCode_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kContentsSeq), &m_lSeq_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kName), &m_lName_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kTel), &m_lTel_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kACCCode), &m_lAcc_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kDelete), &m_lDelete_FID )) )
	{
		wcerr << L"#Error POI_POINT�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	if( FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kFuncSeq), &m_funcSeq_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kPSTAttCode), &m_pstAttCode_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kPriority), &m_priority_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kPriorityAttCode), &m_priorityAttCode_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kTollRoad), &m_tollRoad_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kPOIPointID), &m_poiPointId_FID ))
		|| 	FAILED(m_ipPOISubPointFC->FindField( _bstr_t(sindyk::poi_sub_point_org::kIDNCode), &m_idnCode_FID )) )
	{
		wcerr << L"#Error POI_SUB_POINT�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	if( FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kCityCode), &m_lCityCode_FID ))
		|| 	FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kAddrCode), &m_lAddrCode_FID ))
		|| 	FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kAddrCode2), &m_lAddrCode2_FID )) )
	{
		wcerr << L"#Error CITY_ADMIN�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	// CONTENTS_MASTER �e�[�u�����擾
	m_cDBCache.m_strContentsMaster = arg.m_contents.c_str();

	// �o�͑Ώێ�ʕ\���X�g�ǂݍ���
	if( !m_cListCache.Create(arg.m_listPath.c_str()) )
	{
		wcerr << L"#Error �o�͑Ώێ�ʕ\���X�g�̓ǂݍ��ݎ��s : " << arg.m_listPath << endl;
		return false;
	}
	wcerr << L"#�o�͑Ώێ�ʕ\���X�g�ǂݍ��ݐ��� : " << arg.m_listPath << endl;

	// �o�̓t�@�C���I�[�v��
	m_outPOI.Open( arg.m_outFile );
	if( !m_outPOI.IsOpen() )
	{
		wcerr << L"#Error �o�̓t�@�C���̃I�[�v���Ɏ��s : " << arg.m_outFile << endl;
		return false;
	}
	m_outPOISub.Open( arg.m_outFileSub );
	if( !m_outPOISub.IsOpen() )
	{
		wcerr << L"#Error �o�̓t�@�C���̃I�[�v���Ɏ��s : " << arg.m_outFileSub << endl;
		return false;
	}

	// �}�X�^���n�L���b�V��
	if( !m_cDBCache.Create(ipPOIWorkspace) )
	{
		wcerr << L"#Error �}�X�^���L���b�V���Ɏ��s" << endl;
		return false;
	}
	wcerr << L"#�}�X�^���L���b�V���쐬����" << endl;

	// �����Ώ�IDNCODE
	m_idnCode = arg.m_idnCode.c_str();
	const auto& idncodes = uh::str_util::split( m_idnCode, _T(",") );
	for( const auto& idncode : idncodes )
	{
		m_idnCodeCounter[_ttol(idncode)] = 0;
	}

	// ���O�w�b�_�[
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
					  _T("#�J�n���� %s"),
					  cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), gf::GetCurTime() );
	wcout << CT2CW(strHeader) << endl;

	return true;
}

// ���s
void CApp::Run()
{
	// �܂�POI_SUB_POINT���擾(IDNCODE�ADELETE_C�ōi��)
	if( !CollectPoiSubPointInfo() )
		return;

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	long lCount = 1;

	// ORDER BY��ǉ��i�������Ɉˑ����Ȃ��悤�ɂ��邽�߁j[bug12624]
	CString strPostfix;
	strPostfix.Format(_T("ORDER BY %s, %s"), sindyk::poi_point::kContentsCode, sindyk::poi_point::kContentsSeq); // CONTENTS_CODE, CONTENTS_SEQ�̏���

	// Contents_Master����AContents_Type<0,1>��Contents_code���g���āAPOI_POINT����Y��Contents_code�̂��̂������擾�iBug9215�Ή��j
	vector<wstring>	vecWstrWhere = m_cDBCache.CreateWhereVecFromContentCode( sindyk::poi_point::kContentsCode );

	// vector�ŕԂ��Ă��邽�߁A���g�̐������s
	for( const auto& whereClause : vecWstrWhere )
	{
		// Contents_code in (hoge, hoge2, �E�E�E)��Where��
		ipQuery->put_WhereClause( CComBSTR(whereClause.c_str()) );
		if( FAILED(IQueryFilterDefinitionPtr(ipQuery)->put_PostfixClause(CComBSTR(strPostfix))) )
			continue;

		IFeatureCursorPtr ipPOICursor;
		m_ipPOIPointFC->Search( ipQuery, VARIANT_FALSE, &ipPOICursor );
		if( !ipPOICursor )
			continue;

		IFeaturePtr ipPOIFeature;
		while( ipPOICursor->NextFeature(&ipPOIFeature) == S_OK && ipPOIFeature )
		{
			CComVariant contentsCode;
			ipPOIFeature->get_Value( m_lContentsCode_FID, &contentsCode );

			const POIMASTERDBINFO* pDBInfo = m_cDBCache.GetPOIMasterInfo( contentsCode.lVal );
			if( !pDBInfo )
			{
				wcout << L"#Fatal Error �}�X�^��񂪂���܂��� : " << contentsCode.lVal << endl;
				++lCount;
				continue;
			}

			const POIUSABLEINFO* pUsableInfo = m_cListCache.GetPOIMasterInfo(pDBInfo->first, pDBInfo->second);
			if( !pUsableInfo )
			{
				wcout << L"#Warning �}�X�^�ƈ�v�����񂪃��X�g�ɂ���܂���(��񂪌Â����m�F) : " << pDBInfo->first << L"," << pDBInfo->second << endl;
				++lCount;
				continue;
			}

			// �d�v�R���e���c�t���O�̗p�Ȃ�΁A�������Ƀ��R�[�h�i�[
			// �ʒu�t���O or PP�t���O�̂ǂ��炩���̗p�Ȃ�A����������R�[�h�i�[�i���x['1S', '3A', '1A', 'UK']���ǂ�����SetPOI���Ŕ��肵�ė��Ƃ��j
			if( pUsableInfo->bImportantF || pUsableInfo->bQualityF || pUsableInfo->bPPRateF )
			{
				SetPOI( ipPOIFeature, pDBInfo, pUsableInfo );
			}
			++lCount;
		}
		wcerr << lCount << L" ���ǂݍ��݊���\r";
	}

	// �f�[�^�o��
	wcerr << L"#�f�[�^�o�͊J�n ... ";
	m_outPOI.POIPointOutput(m_setImpPOI);
	m_outPOI.POIPointOutput(m_setPOI);
	m_outPOISub.POISubPointOutput(m_setPOISub);
	wcerr << L"����" << endl;

	// IDNCODE���Ƃ̏o�͌����`�F�b�N
	for( const auto& errorIDNCodeCount : m_idnCodeCounter )
	{
		if( 0 == errorIDNCodeCount.second )
		{
			wcout << L"#Warning IDNCODE : " << errorIDNCodeCount.first << L" �̏o�͌�����0���ł�" << endl;
		}
	}

	// ���O�t�b�^�[
	wcout << L"#�I������ " << (const wchar_t*)gf::GetCurTime() << endl;
}

// ---------------------------------- Private ------------------------------------

// ���R�[�h�i�[
void CApp::SetPOI( IFeaturePtr ipFeature, const POIMASTERDBINFO* pMaster, const POIUSABLEINFO* pUsable  )
{
	POIPointRecord cPoi;

	ipFeature->get_OID( &cPoi.m_lOID );		// OID
	cPoi.m_lGrpCode = pMaster->first;		// ���ރR�[�h
	cPoi.m_lChainCode = pMaster->second;	// �`�F�[���R�[�h

	CComVariant vaValue;

	// ���x�R�[�h
	ipFeature->get_Value( m_lAcc_FID, &vaValue );
	cPoi.m_strAccCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// �V�[�P���X(������)
	ipFeature->get_Value( m_lSeq_FID, &vaValue );
	cPoi.m_strSeq = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// ����
	ipFeature->get_Value( m_lName_FID, &vaValue );
	cPoi.m_strName = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// �d�b�ԍ�(�n�C�t�����O����)
	ipFeature->get_Value( m_lTel_FID, &vaValue );
	cPoi.m_strTel = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	cPoi.m_strTel.Replace( _T("-"), _T("") );
	vaValue.Clear();

	// �폜�R�[�h
	ipFeature->get_Value( m_lDelete_FID, &vaValue );
	cPoi.m_deleteCode = vaValue.lVal;
	vaValue.Clear();

	// Bug9215�Ή��F�d�v�R���e���c�ȊO�́A�����Ő��x�ŗ��Ƃ��悤�ɕύX
	// �����ŏo�͑Ώۂ��`�F�b�N(TEL��NULL�łȂ����E���x���㗘�p�Ώۂ��E�d�v�R���e���c�łȂ���ΐ��x��['1S', '3A', '1A', 'UK']��)
	if( cPoi.m_strTel.IsEmpty() )
		return;

	if( pUsable->bImportantF || 
		( (pUsable->bQualityF || pUsable->bPPRateF) && cPoi.GetPriorValueFromAcc() < 4 ) )
	{
		// �v���b�g�ʒu�̍s���E������擾
		IGeometryPtr ipGeo;
		ipFeature->get_Shape( &ipGeo );

		CODEINFO cCodeInfo;
		if( !GetCityAdminInfo(ipGeo, cCodeInfo) )
		{
			wcout << L"#Error �s���E���擾�ł��Ȃ� : " << pMaster->first << L"-" << pMaster->second << L"-" << (const wchar_t*)cPoi.m_strSeq << endl;
			return;
		}

		cPoi.m_strAddrCode1 = cCodeInfo.first;	// �Z���R�[�h1
		cPoi.m_strAddrCode2 = cCodeInfo.second;	// �Z���R�[�h2

		// �Q�����b�V��XY�擾
		gf::GetMeshXY( ipFeature, cPoi.m_nMeshCode, cPoi.m_nX, cPoi.m_nY );

		// �ƌ`ID�擾 (15�t��������)
		// �ƌ`�ɏ���ĂȂ����Ƃ����X����̂ŁA����ĂȂ��Ă����b�Z�[�W�͏o���Ȃ�
		long buildingID = -1; // POI������Ă錚����OID
		GetBuildingInfo(ipGeo, buildingID);
		cPoi.m_buildingID = buildingID;

		// �}�b�`���O��ʂ�ݒ肵�Ċi�[
		// �d�v�R���e���c�F0�A�ʒu�i������F1�A�s���|�C���g������F2�iBug9215�Ή��F�R�[�h�l�ύX�j
		if( pUsable->bImportantF )
		{
			cPoi.m_nAddXYC	= 0;
			m_setImpPOI.insert( cPoi );	// ���R�[�h�i�[
		}
		else
		{
			cPoi.m_nAddXYC = pUsable->bQualityF? 1 : 2;
			m_setPOI.insert( cPoi );	// ���R�[�h�i�[
		}

		// �����܂ł�����SUB_POINT�����擾
		SetPOISub( cPoi.m_lOID );
	}
}

// Poi_Sub_Point���R�[�h�i�[
void CApp::SetPOISub( const long poiPointId )
{
	if( 0 == m_mapPoiSub.count(poiPointId) )
		return;

	for( const auto& feature : m_mapPoiSub[poiPointId] )
	{
		POISubPointRecord cPoiSub;
		cPoiSub.m_poiPointId = poiPointId;

		CComVariant seq, pst, pri, pat, tor, idn;
		feature->get_Value( m_funcSeq_FID, &seq );
		cPoiSub.m_funcSeq = seq;
		feature->get_Value( m_pstAttCode_FID, &pst );
		cPoiSub.m_pstAttCode = pst;
		feature->get_Value( m_priority_FID, &pri );
		cPoiSub.m_priority = pri.intVal;
		feature->get_Value( m_priorityAttCode_FID, &pat );
		cPoiSub.m_priorityAttCode = pat;
		feature->get_Value( m_tollRoad_FID, &tor );
		cPoiSub.m_tollRoadFlag = tor.intVal;

		gf::GetMeshXY( feature, cPoiSub.m_meshCode, cPoiSub.m_x, cPoiSub.m_y );

		m_setPOISub.insert( cPoiSub ); // ���R�[�h�i�[

		feature->get_Value( m_idnCode_FID, &idn );
		++m_idnCodeCounter[idn.lVal];
	}
}

// �s���E���擾
bool CApp::GetCityAdminInfo( IGeometryPtr ipGeo, CODEINFO& rCodeInfo )
{
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	long lCount = 0;
	m_ipCityAdminFC->FeatureCount( ipFilter, &lCount );

	IFeatureCursorPtr ipCursor;
	m_ipCityAdminFC->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( lCount == 1 && ipCursor )
	{
		IFeaturePtr ipFeature;
		if( ipCursor->NextFeature(&ipFeature) == S_OK )
		{
			CComVariant vaValue;

			// �s�撬���R�[�h
			ipFeature->get_Value( m_lCityCode_FID, &vaValue );
			rCodeInfo.first = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
			vaValue.Clear();

			// �Z���R�[�h1
			ipFeature->get_Value( m_lAddrCode_FID, &vaValue );
			rCodeInfo.first += (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
			vaValue.Clear();

			// �Z���R�[�h2
			ipFeature->get_Value( m_lAddrCode2_FID, &vaValue );
			CString strAddrCode2 = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
			vaValue.Clear();

			if( strAddrCode2 != _T("000000") )
				rCodeInfo.second = rCodeInfo.first.Left(5) + strAddrCode2;
		}
		return true;
	}
	else
		return false;
}

bool CApp::GetBuildingInfo( const IGeometryPtr& ipGeom, long& buildingID )
{
	CString subFields;
	subFields.Format( _T("%s, %s"), sindy::schema::ipc_table::kObjectID, sindy::schema::ipc_feature::kShape );

	// 1�񕁒ʂɋ�Ԍ������Ă݂�B���������������ŏE���ďI���
	ISpatialFilterPtr ipFilter1( CLSID_SpatialFilter );
	ipFilter1->putref_Geometry( ipGeom );
	ipFilter1->put_SpatialRel( esriSpatialRelIntersects );
	ipFilter1->put_SubFields( CComBSTR( subFields ) );
	IFeatureCursorPtr ipCursor1;
	m_ipBuildingFC->Search( ipFilter1, VARIANT_TRUE, &ipCursor1 );
	if( ipCursor1 )
	{
		IFeaturePtr ipFeature1;
		if( ipCursor1->NextFeature(&ipFeature1) == S_OK && ipFeature1 )
		{
			ipFeature1->get_OID( &buildingID );
			return true;
		}
	}

	// �|�C���g�������������ɗ����Ă邩������Ȃ��̂ŁA��x�|�C���g�Ƀo�b�t�@�����Č������āA�����������
	IGeometryPtr ipSearchGeom;
	((ITopologicalOperatorPtr)ipGeom)->Buffer( 0.001, &ipSearchGeom ); // 100m���炢�o�b�t�@

	// ��������Ă���
	ISpatialFilterPtr ipFilter2( CLSID_SpatialFilter );
	ipFilter2->putref_Geometry( ipSearchGeom );
	ipFilter2->put_SpatialRel( esriSpatialRelIntersects );
	ipFilter2->put_SubFields( CComBSTR( subFields ) );
	IFeatureCursorPtr ipCursor2;
	m_ipBuildingFC->Search( ipFilter2, VARIANT_TRUE, &ipCursor2 );
	if( !ipCursor2 )
		return false;

	// ���߂ă|�C���g����邩�ǂ������`�F�b�N
	IFeaturePtr ipFeature2;
	while( S_OK == ipCursor2->NextFeature( &ipFeature2 ) && ipFeature2 )
	{
		// Exterior�����(=���������߂��`��)
		IGeometryPtr ipBldGeometry;
		ipFeature2->get_Shape(&ipBldGeometry);
		IPolygon4Ptr ipBldPolygon( ipBldGeometry );

		IGeometryBagPtr ipExtRingBag;
		ipBldPolygon->get_ExteriorRingBag( &ipExtRingBag );
		IGeometryCollectionPtr ipExRingColl( ipExtRingBag );

		long ringCount = 0;
		ipExRingColl->get_GeometryCount( &ringCount );

		for( long i = 0; i < ringCount; ++i )
		{
			IGeometryPtr ipExtGeom;
			ipExRingColl->get_Geometry( i, &ipExtGeom );

			// �|�C���g����邩�H
			// Disjoint�̕����������ǁA��肭�ƌ`�����Ȃ��̂�Within�ł��
			IRelationalOperatorPtr ipRelOpe = ipGeom;
			VARIANT_BOOL within = VARIANT_FALSE;
			ipRelOpe->Within( ipExtGeom, &within );
			if( within )
			{
				ipFeature2->get_OID( &buildingID );
				return true;
			}
		}
	}
	return false;
}

bool CApp::CollectPoiSubPointInfo()
{
	// TODO:IDNCODE�̃`�F�b�N�͕ʂł���āADELETE_C��Where��Ɋ܂߂�
	IQueryFilterPtr ipPOISubQuery( CLSID_QueryFilter );
	CString poiSubWhereClause;
	poiSubWhereClause.Format( _T("%s in (%s)"), sindyk::poi_sub_point_org::kIDNCode, m_idnCode );
	ipPOISubQuery->put_WhereClause( CComBSTR(poiSubWhereClause) );
	IFeatureCursorPtr ipSubPOICursor;
	m_ipPOISubPointFC->Search( ipPOISubQuery, VARIANT_FALSE, &ipSubPOICursor );
	if( !ipSubPOICursor )
	{
		wcerr << L"#Error IDNCODE�ł̍i�荞�݂Ɏ��s�FIDNCODE " << (CT2CW)m_idnCode << endl;
		return false;
	}

	long deleteCodeIndex = -1;
	m_ipPOISubPointFC->FindField( CComBSTR(sindyk::poi_sub_point_org::kDelete), &deleteCodeIndex );
	if( deleteCodeIndex < 0 )
	{
		wcerr << L"#Error POI_SUB_POINT�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	// DELETE_C=0 �̃��R�[�h���i�[
	IFeaturePtr ipSubPOIFeature;
	while( S_OK == ipSubPOICursor->NextFeature(&ipSubPOIFeature) && ipSubPOIFeature )
	{
		CComVariant deleteCode;
		ipSubPOIFeature->get_Value( deleteCodeIndex, &deleteCode );
		if( deleteCode.lVal != 0 )
			continue;

		CComVariant pointID;
		ipSubPOIFeature->get_Value( m_poiPointId_FID, &pointID );
		m_mapPoiSub[pointID.lVal].push_back(ipSubPOIFeature);
	}
	return true;
}
