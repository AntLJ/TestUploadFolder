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
#include "SurveyPointManager.h"
#include "common.h"
#include "LogSys.h"

using namespace sindy::schema::sj;

SurveyPointRec::SurveyPointRec() :
	e_objectID( 0 ), e_srcType( 0 ), e_bldOID( 0 )
{
}

SurveyPointRec::~SurveyPointRec()
{
}

bool SurveyPointRec::setGeometry( const IFeaturePtr& ipFeature )
{
	IGeometryPtr ipGeometry;
	if( FAILED( ipFeature->get_Shape( &ipGeometry ) ) ) return false;
	e_ipPoint = ipGeometry;

	return true;
}

SurveyPointManager::SurveyPointManager(
	const IFeatureClassPtr& ipCityMesh,
	const IFeatureClassPtr& ipCityAdmin,
	const std::map<CString, long>& mapCityMeshIndexes,
	const std::map<CString, long>& mapCityAdminIndexes,
	const std::map<CString, long>& mapBldgSrcPtIndexes,
	const std::map<CString, long>& mapTownpageIndexes,
	const CString& sourceName,
	const CString& sourceDate,
	const ADODB::_ConnectionPtr& conAddrDB )
	: m_ipCityMesh( ipCityMesh ), m_ipCityAdmin( ipCityAdmin ), m_mapCityMeshIndexes( mapCityMeshIndexes ),
	m_mapCityAdminIndexes( mapCityAdminIndexes ), m_mapBldgSrcPtIndexes( mapBldgSrcPtIndexes ), m_mapTownpageIndexes( mapTownpageIndexes ),
	m_sourceName( sourceName ), m_sourceDate( sourceDate ), m_conAddrDB( conAddrDB )
{
	// フィールドリストの登録
	m_mapFields[kObjectID]		= ::buildingname_src_point::kObjectID;
	m_mapFields[kName]			= ::buildingname_src_point::kName;
	m_mapFields[kOrgName1]		= ::buildingname_src_point::kOrgName1;
	m_mapFields[kOrgName2]		= ::buildingname_src_point::kOrgName2;
	m_mapFields[kKana]			= ::buildingname_src_point::kKana;
	m_mapFields[kAddr]			= ::buildingname_src_point::kAddr;
	m_mapFields[kOrgAddr]		= ::buildingname_src_point::kOrgAddr;
	m_mapFields[kAddrCode]		= ::buildingname_src_point::kAddrCode;
	m_mapFields[kAddrLon]		= ::buildingname_src_point::kAddrLon;
	m_mapFields[kAddrLat]		= ::buildingname_src_point::kAddrLat;
	m_mapFields[kRepType]		= ::buildingname_src_point::kRepType;
	m_mapFields[kAddrLv]		= ::buildingname_src_point::kAddrLevel;
	m_mapFields[kPinPoint]		= ::buildingname_src_point::kPinPoint;
	m_mapFields[kMultiSameBldg]	= ::buildingname_src_point::kMultiSameBldg;
	m_mapFields[kSameAddr]		= ::buildingname_src_point::kSameAddr;
	m_mapFields[kSameAddrCount]	= ::buildingname_src_point::kSameAddrCount;
	m_mapFields[kAddrVer]		= ::buildingname_src_point::kAddrVer;
	m_mapFields[kSrcType]		= ::buildingname_src_point::kSourceType;
	m_mapFields[kSrcName]		= ::buildingname_src_point::kSourceName;
	m_mapFields[kSrcDate]		= ::buildingname_src_point::kSourceDate;
	m_mapFields[kSrcID]			= ::buildingname_src_point::kSourceID;
	m_mapFields[kBldg1Div1]		= ::buildingname_src_point::kBuilding1Div1;
	m_mapFields[kBldg1Div2]		= ::buildingname_src_point::kBuilding1Div2;
	m_mapFields[kBldg2Div1]		= ::buildingname_src_point::kBuilding2Div1;
	m_mapFields[kBldg2Div2]		= ::buildingname_src_point::kBuilding2Div2;
	m_mapFields[kHeight]		= ::buildingname_src_point::kHeight;
	m_mapFields[kFloors]		= ::buildingname_src_point::kFloors;
	m_mapFields[kBaseFloors]	= ::buildingname_src_point::kBasementFloors;
	m_mapFields[kBldgArea]		= ::buildingname_src_point::kBuildingArea;
	m_mapFields[kAllArea]		= ::buildingname_src_point::kAllArea;
	m_mapFields[kSrcStartDate]	= ::buildingname_src_point::kSrcStartDate;
	m_mapFields[kSrcUpdateDate]	= ::buildingname_src_point::kSrcUpdateDate;
	m_mapFields[kUpdateDate]	= ::buildingname_src_point::kUpdateDate;
	m_mapFields[kAdoption]		= ::buildingname_src_point::kAdoption;
}

SurveyPointManager::~SurveyPointManager()
{
}

bool SurveyPointManager::open( const CString& fileName )
{
	m_ofsOutput.open( fileName );
	if( !m_ofsOutput.is_open() ) return false;

	// ヘッダ情報の出力
	for( const auto& rec : m_mapFields ) m_ofsOutput << CT2A( rec.second ) << "\t";
	m_ofsOutput << "X\tY\tBLDID\tMESHCODE\tADDRSTR" << std::endl;

	return true;
}

void SurveyPointManager::close()
{
	m_ofsOutput.close();
}

bool SurveyPointManager::addFromBldgNameSrc( long srcOID, long bldOID, const IFeaturePtr& ipFeature )
{
	SurveyPointRec surveyPtRec;
	surveyPtRec.e_objectID = srcOID;
	surveyPtRec.e_bldOID = bldOID;
	CComVariant varSrcType;
	if( !::common_proc::GetValue( (_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_mapBldgSrcPtIndexes, &varSrcType ) )
		return false;
	surveyPtRec.e_srcType = varSrcType.lVal;

	// ポイントジオメトリの取得
	if( !surveyPtRec.setGeometry( ipFeature ) )
	{
		LogSys::GetInstance().WriteFreeStyleLog( true, err_level::err, _T("ジオメトリの取得に失敗"), ::buildingname_src_point::kTableName, surveyPtRec.e_objectID );
		return false;
	}

	// フィールドリストに則った処理
	for( const auto& rec : m_mapFields )
	{
		CComVariant varTmp;
		if( !::common_proc::GetValue( (_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, rec.second, m_mapBldgSrcPtIndexes, &varTmp ) )
			return false;
		switch( varTmp.vt )
		{
		case VT_EMPTY:
		case VT_NULL:
			surveyPtRec.push_back( _T("") );
			break;
		default:
			varTmp.ChangeType( VT_BSTR );
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		}
	}

	(*this).insert( surveyPtRec );

	return true;
}

bool SurveyPointManager::addFromTownpage( long twnOID, long bldOID, const IFeaturePtr& ipFeature )
{
	SurveyPointRec surveyPtRec;
	surveyPtRec.e_objectID = twnOID;
	surveyPtRec.e_bldOID = bldOID;
	surveyPtRec.e_srcType = SRC_TYPE_TOWN;

	// ポイントジオメトリの取得
	if( !surveyPtRec.setGeometry( ipFeature ) )
	{
		LogSys::GetInstance().WriteFreeStyleLog( true, err_level::err, _T("ジオメトリの取得に失敗"), sindy::schema::townpage::kTableName, surveyPtRec.e_objectID );
		return false;
	}

	// フィールドリストに則った処理
	for( const auto& rec : m_mapFields )
	{
		CComVariant varTmp;
		switch( rec.first )
		{
		case kObjectID:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kObjectID, m_mapTownpageIndexes, &varTmp ) )
				return false;
			varTmp.ChangeType( VT_BSTR );
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kName:
		case kOrgName1:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kName, m_mapTownpageIndexes, &varTmp ) )
				return false;
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kKana:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kYomi, m_mapTownpageIndexes, &varTmp ) )
				return false;
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kAddrCode:
			{
				CString strAddrCode;
				if( !getAddrCodeFromTownpage( ipFeature, strAddrCode ) ) return false;
				surveyPtRec.push_back( strAddrCode );
				break;
			}
		case kRepType:
		case kAddrLv:
		case kMultiSameBldg:
		case kSameAddr:
		case kSameAddrCount:
		case kAdoption:
			surveyPtRec.push_back( _T("0") );
			break;
		case kPinPoint:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kPinPointFlag, m_mapTownpageIndexes, &varTmp ) )
				return false;
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kSrcType:
			surveyPtRec.push_back( std::to_string( SRC_TYPE_TOWN ).c_str() );
			break;
		case kSrcName:
			surveyPtRec.push_back( m_sourceName );
			break;
		case kSrcDate:
			surveyPtRec.push_back( m_sourceDate );
			break;
		case kSrcID:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kDataSeq, m_mapTownpageIndexes, &varTmp ) )
				return false;
			varTmp.ChangeType( VT_BSTR );
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kBldg1Div1:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kNttGrpCode, m_mapTownpageIndexes, &varTmp ) )
				return false;
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kBldg1Div2:
			if( !::common_proc::GetValue( (_IRowPtr)ipFeature, sindy::schema::townpage::kTableName, sindy::schema::townpage::kNttCode, m_mapTownpageIndexes, &varTmp ) )
				return false;
			surveyPtRec.push_back( CString( varTmp ) );
			break;
		case kUpdateDate:
			surveyPtRec.push_back( CString( GetYYYYMMDD( true ).c_str() ) );
			break;
		default:
			surveyPtRec.push_back( _T("") );
			break;
		}
	}

	(*this).insert( surveyPtRec );

	return true;
}

bool SurveyPointManager::output()
{
	// ファイルに出力
	for( const auto& rec : *this )
	{
		for( const auto& fld : rec )
		{
			m_ofsOutput << CT2A( fld ) << "\t";
		}
		double dx = 0.0, dy = 0.0;
		rec.e_ipPoint->QueryCoords( &dx, &dy );
		m_ofsOutput << std::fixed << std::setprecision( 12 ) << dx << "\t";
		m_ofsOutput << std::fixed << std::setprecision( 12 ) << dy << "\t";
		m_ofsOutput << rec.e_bldOID << "\t";
		m_ofsOutput << CT2A( getMeshCode( rec.e_ipPoint ) ) << "\t";
		m_ofsOutput << CT2A( getAddrName( rec.e_ipPoint ) ) << std::endl;
	}

	// 出力したレコード群をクリア
	(*this).clear();

	return true;
}

CString SurveyPointManager::getMeshCode( const IPointPtr& ipPoint )
{
	CString strMeshCode( _T("(取得失敗)") );

	// 都市地図メッシュポリゴンを空間検索し、都市地図メッシュコードを取得する
	IFeatureCursorPtr ipCurCityMesh;
	if( !::common_proc::SearchByGeometry( m_ipCityMesh, ipCurCityMesh, ipPoint, esriSpatialRelIntersects ) )
		return strMeshCode;
	IFeaturePtr ipFCityMesh;
	if( FAILED( ipCurCityMesh->NextFeature( &ipFCityMesh ) ) || !ipFCityMesh )
		return strMeshCode;
	CComVariant varTmp;
	if( !::common_proc::GetValue( (_IRowPtr)ipFCityMesh, sindy::schema::citymesh::kTableName, sindy::schema::citymesh::kMeshCode, m_mapCityMeshIndexes, &varTmp ) )
		return strMeshCode;
	strMeshCode.Format( _T("%d"), varTmp.lVal );

	return strMeshCode;
}

CString SurveyPointManager::getAddrName( const IPointPtr& ipPoint )
{
	CString strAddrName( _T("(取得失敗)") );

	// 都市地図行政界を空間検索し、11桁住所コードを取得する
	IFeatureCursorPtr ipCurCityAdmin;
	if( !::common_proc::SearchByGeometry( m_ipCityAdmin, ipCurCityAdmin, ipPoint, esriSpatialRelIntersects ) )
		return strAddrName;
	IFeaturePtr ipFCityAdmin;
	if( FAILED( ipCurCityAdmin->NextFeature( &ipFCityAdmin ) ) || !ipFCityAdmin )
		return strAddrName;
	CComVariant varCityCode, varAddrCode;
	if( !::common_proc::GetValue( (_IRowPtr)ipFCityAdmin, sindy::schema::city_admin::kTableName, sindy::schema::city_admin::kCityCode, m_mapCityAdminIndexes, &varCityCode ) )
		return strAddrName;
	if( !::common_proc::GetValue( (_IRowPtr)ipFCityAdmin, sindy::schema::city_admin::kTableName, sindy::schema::city_admin::kAddrCode, m_mapCityAdminIndexes, &varAddrCode ) )
		return strAddrName;
	CString strAddrCode = CString( varCityCode.bstrVal ) + CString( varAddrCode.bstrVal );

	// 住所マスタを利用して11桁住所コードから住所文字列を取得する
	CString strSQL;
	strSQL.Format(
		_T("SELECT REPLACE( ken_kanji || shi_kanji || oaza_kanji || aza_kanji, '　' ) FROM areacode_master ") \
		_T("WHERE ken_code = '%s' AND shi_code = '%s' AND oaza_code = '%s' AND aza_code = '%s'"),
		strAddrCode.Mid( 0, 2 ), strAddrCode.Mid( 2, 3 ), strAddrCode.Mid( 5, 3 ), strAddrCode.Mid( 8, 3 ) );
	try
	{
		ADODB::_RecordsetPtr pRecs( __uuidof( ADODB::Recordset ) );
		pRecs->PutRefActiveConnection( m_conAddrDB );
		if( FAILED( pRecs->Open( _bstr_t( strSQL ), vtMissing, ADODB::adOpenKeyset, ADODB::adLockReadOnly, ADODB::adCmdText ) ) )
			return strAddrName;
		if( !pRecs->adoEOF )
		{
			strAddrName = pRecs->Fields->GetItem( 0L )->Value.bstrVal;
		}
		pRecs->Close();
	}
	catch( const _com_error& e )
	{
		LogSys::GetInstance().WriteFreeStyleLog( true, err_level::err, _T("住所DB問合せ時に例外が発生") );
		throw e;
	}

	return strAddrName;
}

bool SurveyPointManager::getAddrCodeFromTownpage( const IFeaturePtr& ipFeature, CString& strAddrCode )
{
	using namespace sindy::schema;

	// prefcode, citycode, adrcode1, adrcode2, chibancode, jukyocode取得
	CComVariant varPref, varCity, varAdr1, varAdr2, varChiban, varJukyo;
	if( (! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kPrefCode,		m_mapTownpageIndexes, &varPref) ) ||
		(! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kCityCode,		m_mapTownpageIndexes, &varCity) ) ||
		(! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kAdrCode1,		m_mapTownpageIndexes, &varAdr1) ) ||
		(! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kAdrCode2,		m_mapTownpageIndexes, &varAdr2) ) ||
		(! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kChibanCode,	m_mapTownpageIndexes, &varChiban) ) ||
		(! ::common_proc::GetValue( (_IRowPtr)ipFeature, townpage::kTableName, townpage::kJukyoCode,	m_mapTownpageIndexes, &varJukyo) ) ) {
			return false;
	}
	strAddrCode.Format(_T("%s%s%s%s%s%s"), CString(varPref.bstrVal), CString(varCity.bstrVal), CString(varAdr1.bstrVal), CString(varAdr2.bstrVal), CString(varChiban.bstrVal), CString(varJukyo.bstrVal));

	return true;
}
