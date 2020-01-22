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

	// SDE接続（住所）
	CWorkspace cWorkspaceAdr;
	if( cWorkspaceAdr.Connect( m_strConnectAdr ) )
	{
		// 号ポイントフィーチャクラス
		m_cFCGouPoint.SetObject( cWorkspaceAdr.OpenTable( schema::gou_point::kTableName ) );
		if( m_cFCGouPoint )
		{
			m_cFMGouPoint = m_cFCGouPoint.GetFieldMap();
			m_cTNGouPoint = m_cFCGouPoint.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL フィーチャクラスを開けません : ") + CString(schema::gou_point::kTableName) );
			bIsOK = false;
		}

		// 都市行政界フィーチャクラス
		m_cFCCityAdmin.SetObject( cWorkspaceAdr.OpenTable( schema::city_admin::kTableName ) );
		if( m_cFCCityAdmin )
		{
			m_cFMCityAdmin = m_cFCCityAdmin.GetFieldMap();
			m_cTNCityAdmin = m_cFCCityAdmin.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL フィーチャクラスを開けません : ") + CString(schema::city_admin::kTableName) );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL 接続できません : ") + m_strConnectAdr );
		bIsOK = false;
	}

	// SDE接続（家形）
	CWorkspace cWorkspaceBld;
	if( cWorkspaceBld.Connect( m_strConnectBld ) )
	{
		// 家形ポリゴンフィーチャクラス
		m_cFCBuilding.SetObject( cWorkspaceBld.OpenTable( schema::building::kTableName ) );
		if( m_cFCBuilding )
		{
			m_cFMBuilding = m_cFCBuilding.GetFieldMap();
			m_cTNBuilding = m_cFCBuilding.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL フィーチャクラスを開けません : ") + CString(schema::building::kTableName) );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL 接続できません : ") + m_strConnectBld );
		bIsOK = false;
	}

	// SDE接続（GEOSPACE）
	CWorkspace cWorkspaceGeo;
	if( cWorkspaceGeo.Connect( m_strConnectGeo ) )
	{
		// GEOSPACE家形ポリゴンフィーチャクラス
		m_cFCBuildingGeo.SetObject( cWorkspaceGeo.OpenTable( _T("G0013") ) );
		if( m_cFCBuildingGeo )
		{
			m_cFMBuildingGeo = m_cFCBuildingGeo.GetFieldMap();
			m_cTNBuildingGeo = m_cFCBuildingGeo.GetNameString();
		}
		else
		{
			WriteLog( _T("#FATAL フィーチャクラスを開けません : G0013") );
			bIsOK = false;
		}
	}
	else
	{
		WriteLog( _T("#FATAL 接続できません : ") + m_strConnectGeo );
		bIsOK = false;
	}

	// 住所DB接続
	if(! ConnectJDB( m_strConnectJDB,	m_cDBSourceJDB	) ) { bIsOK = false; }
	if(! ConnectJDB( m_strConnectJGDC,	m_cDBSourceJGDC	) ) { bIsOK = false; }

	return bIsOK;
}

bool CCheckAddrUsingGSObj::Execute()
{
	_TCHAR aBuf[64];
	int	nCount = 0;
	int nTotal = m_listOID_GouPoint.size();

	// 号ポイントのループ
	foreach( int nOID, m_listOID_GouPoint )
	{
		CLogInfo cLogInfo( nOID );

		// 進捗表示
		nCount++;
		if( nCount % 100 == 0 || nCount == nTotal )
		{
			CString strTmp;
			strTmp.Format( _T("[ %8ld / %8ld ] 件目をチェック中...\r"), nCount, nTotal );
			cout << CT2A(strTmp);
		}

		// OIDに対応する号ポイントを取得
		IFeaturePtr ipF_GouPoint = m_cFCGouPoint.GetFeature( nOID );
		if(! ipF_GouPoint )
		{
			WriteLogInfo( cLogInfo, _T("対応する号ポイントが見つかりません") );
			continue;
		}

		// 号ポイント属性取得
		CFeature cF_GouPoint( ipF_GouPoint, sindy::sindyTableType::gou_point, false, m_cFMGouPoint, m_cTNGouPoint );
		CString	strGouNo	= cF_GouPoint.GetStringValueByFieldName( sindy::schema::gou_point::kGouNo, 64, aBuf );	// 号番号
		long	lGouType	= cF_GouPoint.GetLongValueByFieldName( sindy::schema::gou_point::kGouType,  0 );		// 号タイプ
		long	lExpGouNo	= cF_GouPoint.GetLongValueByFieldName( sindy::schema::gou_point::kExpGouNo, 0 );		// 拡張号番号フラグ

		// 当該位置の行政界
		IFeaturePtr ipF_CityAdmin = SearchAdmin( ipF_GouPoint );
		if(! ipF_CityAdmin )
		{
			WriteLogInfo( cLogInfo, _T("対応する行政界ポリゴンが見つかりません") );
			continue;
		}

		// 行政界から11桁コードを取得
		CFeature cF_CityAdmin( ipF_CityAdmin, sindy::sindyTableType::city_admin, false, m_cFMCityAdmin, m_cTNCityAdmin );
		CString strAddrCode =
			CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kCityCode, 64, aBuf ) ) +
			CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kAddrCode, 64, aBuf ) );

		// 11桁コードを国地協コードに変換
		CString strAddrCodeJGDC = ConvAddrCode2JGDC( strAddrCode );

		// 行政界から街区符号を取得
		// まずは拡張街区符号（文字列）を取得
		CString strGaikuFugo = CString( cF_CityAdmin.GetStringValueByFieldName( sindy::schema::city_admin::kExtGaikuFugo, 64, aBuf ) );
		if( strGaikuFugo.IsEmpty() )
		{
			// なければ街区符号（数値）を取得し文字列化
			long aGaikuFugo = cF_CityAdmin.GetLongValueByFieldName( sindy::schema::city_admin::kGaikuFugo, -1 );
			if( aGaikuFugo > 0 )
			{
				strGaikuFugo.Format( _T("%ld"), aGaikuFugo );
			}
		}
		::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, strGaikuFugo, -1, aBuf, 64 );	// 全角化
		strGaikuFugo = aBuf;

		// 地番・号文字列を生成
		CString strChibanGou = ( strGouNo == _T("＊") ) ? strGaikuFugo : ( ( strGaikuFugo.IsEmpty() || lExpGouNo ) ? strGouNo : ( strGaikuFugo + _T("－") + strGouNo ) );
		::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, strChibanGou, -1, aBuf, 64 );	// 全角化
		strChibanGou = aBuf;

		// 住所文字列生成
		cLogInfo.e_strAddrName = GetAddrName( strAddrCode ) + strChibanGou;

		// 当該位置のiPC家形を取得
		IFeaturePtr ipF_Building = SearchBuilding( ipF_GouPoint );
		if(! ipF_Building )
		{
			// 家形がない
			WriteLogInfo( cLogInfo, _T("号ポイントを含む家形がありません") );
			continue;
		}

		// 家形のGeospaceIDを取得
		CFeature cF_Building( ipF_Building, sindy::sindyTableType::building, false, m_cFMBuilding, m_cTNBuilding );
		CString strGeospaceID = cF_Building.GetStringValueByFieldName( sindy::schema::building::kGeospaceID, 64, aBuf );	strGeospaceID.TrimRight();
		if( strGeospaceID.IsEmpty() )
		{
			// GeospaceIDが振られていない
			WriteLogInfo( cLogInfo, _T("GeospaceIDが家形に付与されていません") );
			continue;
		}

		// iPC家形に付与されているGeospaceIDでGEOSPACE家形を検索
		IFeaturePtr ipF_BuildingGeo = SearchBuildingGeo( strGeospaceID );
		if(! ipF_BuildingGeo )
		{
			// GEOSPACE家形がない
			WriteLogInfo( cLogInfo, _T("GeospaceIDに対応するGEOSPACE家形がありません") );
			continue;
		}

		// GEOSPACE家形の住所を取得
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
			// 住所（11桁コード）がない
			WriteLogInfo( cLogInfo, _T("GEOSPACE家形に11桁住所コードがありません") );
			continue;
		}
		CString strChibanGouGeo;
		if(! strAddress1.IsEmpty() )
		{
			strChibanGouGeo = strAddress1;
			if(! strAddress2.IsEmpty() )
			{
				strChibanGouGeo += _T("－") + strAddress2;
				if(! strAddress3.IsEmpty() )
				{
					strChibanGouGeo += _T("－") + strAddress3;
				}
			}
		}
		cLogInfo.e_strAddrNameGeo = strPrefNameGeo + strCityNameGeo + strAdr1NameGeo + strAdr2NameGeo + strChibanGouGeo;
		cLogInfo.e_strAddrNameGeo.Remove( _T('　') );

		// 11桁住所コードを比較
		if( strAddrCodeJGDC != strAddrCodeGeo )
		{
			// 住所が違う
			WriteLogInfo( cLogInfo, _T("11桁住所コード（国地協）が違います") );
			continue;
		}

		// 地番以降を比較
		if( strChibanGou != strChibanGouGeo )
		{
			// 住所が違う
			WriteLogInfo( cLogInfo, _T("地番以降住所文字列が違います") );
			continue;
		}

		// 住所が一致
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

	options_description option("オプション");
	option.add_options()
		( "connect_adr",	value<string>(),	"SDE接続文字列（住所）" )
		( "connect_bld",	value<string>(),	"SDE接続文字列（家形）" )
		( "connect_geo",	value<string>(),	"SDE接続文字列（GEOSPACE）" )
		( "connect_jdb",	value<string>(),	"住所DB接続文字列（住所文字列取得）" )
		( "connect_jgdc",	value<string>(),	"住所DB接続文字列（加除→国地協テーブル）" )
		( "log_input",		value<string>(),	"古川さんツール出力ログファイル名" )
		( "log_output",		value<string>(),	"出力ログファイル名" )
		( "help,?",								"ヘルプを表示" );
	store( parse_command_line( argc, argv, option ), m_variable_map );
	notify( m_variable_map );

	// ヘルプ表示
	if( m_variable_map.count("help") || m_variable_map.size() == 0 )
	{
		cout << option << endl;
		return false;
	}

	// ログファイル
	if( m_variable_map.count("log_output") )
	{
		m_strLogOutput = m_variable_map["log_output"].as<string>().c_str();
		if(! OpenLogFile( m_strLogOutput ) ) { bIsOK = false; }
	}
	else
	{
		WriteLog( _T("#FATAL ログファイル名の指定が必要です。--log_output オプションを指定してください。") );
		bIsOK = false;
	}

	// 入力ファイル
	if( m_variable_map.count("log_input") )
	{
		m_strLogInput = m_variable_map["log_input"].as<string>().c_str();
		if(! OpenInputFile( ) ) { bIsOK = false; }
	}
	else
	{
		WriteLog( _T("#FATAL 入力ファイル名の指定が必要です。--log_input オプションを指定してください。") );
		bIsOK = false;
	}

	// SDE接続文字列（住所）
	if( m_variable_map.count("connect_adr") )
	{
		m_strConnectAdr = m_variable_map["connect_adr"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE接続文字列（住所）の指定が必要です。--connect_adr オプションを指定してください。") );
		bIsOK = false;
	}

	// SDE接続文字列（家形）
	if( m_variable_map.count("connect_bld") )
	{
		m_strConnectBld = m_variable_map["connect_bld"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE接続文字列（住所）の指定が必要です。--connect_bld オプションを指定してください。") );
		bIsOK = false;
	}

	// SDE接続文字列（GEOSPACE）
	if( m_variable_map.count("connect_geo") )
	{
		m_strConnectGeo = m_variable_map["connect_geo"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL SDE接続文字列（GEOSPACE）の指定が必要です。--connect_geo オプションを指定してください。") );
		bIsOK = false;
	}

	// 住所DB接続文字列（住所文字列取得）
	if( m_variable_map.count("connect_jdb") )
	{
		m_strConnectJDB = m_variable_map["connect_jdb"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL 住所DB接続文字列（住所文字列取得）の指定が必要です。--connect_jdb オプションを指定してください。") );
		bIsOK = false;
	}

	// 住所DB接続文字列（加除→国地協テーブル）
	if( m_variable_map.count("connect_jgdc") )
	{
		m_strConnectJGDC = m_variable_map["connect_jgdc"].as<string>().c_str();
	}
	else
	{
		WriteLog( _T("#FATAL 住所DB接続文字列（加除→国地協テーブル）の指定が必要です。--connect_jgdc オプションを指定してください。") );
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
		WriteLog( _T("#FATAL 入力ファイルを開けませんでした。 : ") + m_strLogInput );
		return false;
	}

	string strTmp;
	while( fInput && getline( fInput, strTmp ) )
	{
		CString strBuf( CA2T( strTmp.c_str() ) );
		if( strBuf.IsEmpty() || strBuf[0] == _T('#') ) continue;

		std::vector< CString > vecFields;
		Split( strBuf, vecFields );						// レコードをフィールド列に分解
		if( vecFields[2] == _T("OBJECTID") ) continue;	// ヘッダ行の場合はスキップ
		long lOID = _ttol( vecFields[2] );				// OIDを取得
		if( lOID <= 0 )
		{
			WriteLog( _T("#FATAL 入力ファイルからOID列を取得できませんでした : ") + m_strLogInput );
			return false;
		}
		// コメントが「移動処理」（「移動処理（強制）」を除く）の場合はOIDをリストに追加
		if( vecFields[23].Left( 4 ) == _T("移動処理") && vecFields[23] != _T("移動処理（強制）") )
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
		WriteLog( _T("#FATAL 接続文字列が不正です : ") + strConnect );
		return false;
	}
	else
	{
		CString strUser		= strConnect.Mid( 0, nPos1 );
		CString strPass		= strConnect.Mid( nPos1 + 1, nPos2 - nPos1 - 1 );
		CString strDBName	= strConnect.Mid( nPos2 + 1 );
		if( OSUCCESS != cDB.Open( CT2CA(strDBName), CT2CA(strUser), CT2CA(strPass) ) )
		{
			WriteLog( _T("#FATAL 接続できません : ") + strConnect );
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
		// キャッシュにない場合 → DBから検索
		// 複数候補がある場合はとりあえず最小値としているが…
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
		// キャッシュにある場合 → キャッシュの内容を返す
		return itr->second;
	}

	return _T("00000000000");
}

CString CCheckAddrUsingGSObj::GetAddrName( const CString& strAddrCode )
{
	map<CString, CString>::const_iterator itr = m_mapAddrName.find( strAddrCode );

	if( itr == m_mapAddrName.end() )
	{
		// キャッシュにない場合
		CString strQuery;
		strQuery.Format( _T("select replace( ken_kanji || shi_kanji || oaza_kanji || aza_kanji, '　') from areacode_master where ken_code = '%s' and shi_code = '%s' and oaza_code = '%s' and aza_code = '%s'"),
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
		// キャッシュにある場合 → キャッシュの内容を返す
		return itr->second;
	}

	return _T("");
}
