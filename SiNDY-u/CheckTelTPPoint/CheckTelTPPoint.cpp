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

// CheckTelTPPoint.cpp:	CCheckTelTPPoin　クラスのインプリメンテーション
//

#include "stdafx.h"
#include "CheckTelTPPoint.h"
#include "GlobalFunc.h"
#include <iterator>
#include <crd_cnv.h>
#include <WinLib/VersionInfo.h>
#include "sindy/schema/map.h"
#include <boost/assign.hpp>
using namespace boost::assign;
using namespace std;
using namespace sindy::schema;

const std::set<LONG> CCheckTelTPPoint::m_CBGClass_NG = 
	list_of
	(city_site::bg_class::kSands)                    // 砂地
	(city_site::bg_class::kMarsh)                    // 湿地
	(city_site::bg_class::kRiver)                    // 河川
	(city_site::bg_class::kPool)                     // プール
	(city_site::bg_class::kSea)                      // 海(Bug 5707対応 07/11/30追加)
	(city_site::bg_class::kLake)                     // 池・湖
	(city_site::bg_class::kInterCityTollExpressway)  // 都市間高速道（有料）
	(city_site::bg_class::kUrbanTollExpressway)      // 都市高速（有料）
	(city_site::bg_class::kTollRoad)                 // 有料道路
	(city_site::bg_class::kNationalHighway)          // 国道
	(city_site::bg_class::kPrincipalPrefecturalRoad) // 主要地方道
	(city_site::bg_class::kPrefectureNormalRoad)     // 一般都道府県道
	(city_site::bg_class::kOtherArterialRoad)        // その他幹線道路
	(city_site::bg_class::kSideway)                  // 歩道
	(city_site::bg_class::kGardenPath)               // 庭園路
	(city_site::bg_class::kCenterDivider)            // 中央分離帯（標準）
	(city_site::bg_class::kInterCityFreeExpressway)  // 都市間高速道（無料）
	(city_site::bg_class::kUrbanFreeExpressway)      // 都市高速（無料）
	(city_site::bg_class::kPublicRoadStairs)         // 公道階段
	(city_site::bg_class::kRunway)                   // 滑走路
	(city_site::bg_class::kUnderRepairRoad)          // 工事中道路
	(city_site::bg_class::kTollGateLane)             // 料金所レーン
	;
	// 2010/01/26 ダム上OKとなったため、NGリストから削除
//	45	// ダム

const std::set<LONG> CCheckTelTPPoint::m_BBGClass_NG = 
	list_of
	(base_site::bg_class::kSea)          // 海
	(base_site::bg_class::kRiver)        // 河川・沢・谷
	(base_site::bg_class::kLake)         // 湖・池・沼
	(base_site::bg_class::kSands)        // 砂地
	(base_site::bg_class::kMarsh)        // 湿地
	(base_site::bg_class::kSwimmingPool) // プール
	;
	// 2010/01/26 ダム上OKとなったため、NGリストから削除
//	801001  // ダム


CCheckTelTPPoint::CCheckTelTPPoint( CEnviron& rEnv ) : 
	m_rEnv(rEnv),
	m_ipTPWorkspace(NULL)
{
	m_dMeter = -1; 
}

CCheckTelTPPoint::~CCheckTelTPPoint()
{

}

// 使用法
void CCheckTelTPPoint::printUsage()
{
	cerr 
		<< "【TelTPPointチェックツール】\n"
		<< "(Usage)CheckTelTPPoint.exe [option] [logfile]\n"
		<< "・チェック範囲オプション(指定なしのときは、全件チェック)\n"
		<< "  -m [meshcode]  メッシュコード指定\n"
		<< "  -l [meshlist]  メッシュリスト指定\n"
		<< "  -d [circle value] POIの探索半径(単位は10進度)\n"		// Bug 5701対応 07/11/30追加
		<< "・環境変数について\n"
		<< "  DB_TP          TP系SDE接続プロパティ\n"
		<< "  DB_ADDRESS     住所系SDE接続プロパティ\n"
		<< "  DB_MAP         都市地図系SDE接続プロパティ\n"
		<< "  DB_BASE        中縮系SDE接続プロパティ\n"
		<< "  DB_POI         POI系SDE接続プロパティ\n"				// Bug 5701対応 07/11/30追加 
		<< "  FC_TelTP_POINT 電話番号検索ポイントフィーチャクラス\n"
		<< "  FC_CITY_ADMIN  都市地図行政界フィーチャクラス\n"
		<< "  FC_CITY_SITE   都市地図背景フィーチャクラス\n"
		<< "  FC_BASE_SITE   中縮背景フィーチャクラス\n" 
		<< "  FC_POI         POIフィーチャクラス名\n"				// Bug 5701対応 07/11/30追加
		<< endl;
}

// 初期化
bool CCheckTelTPPoint::init( int argc, char* argv[] )
{
	if( !checkArg(argc, argv) )
		return false;

	if( !setInfoFromSDE() )
		return false;

	// ログファイル
	if( !m_cError.open(argv[argc-1]) )
	{
		cerr << "#Error ログファイルのオープンに失敗 : " << argv[argc-1] << endl;
		return false;
	}
	m_cError.setFCName( m_rEnv.m_strTelTPFName );

	// ログヘッダー出力
	CVersion cVer;
	CString strVersion;
	strVersion.Format( _T("%s FILEVERSION %s PRODUCTVERSION %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_cError.printHeader( strVersion );

	return true;
}

//実行
void CCheckTelTPPoint::execute()
{
	
	if( m_vecMesh.empty() )	checkMain();
	else					checkMainByMesh();
}

//引数解析
bool CCheckTelTPPoint::checkArg( int argc, char *argv[] )
{
	for( int i=1;i<argc-2;++i )
	{
		if( strcmp(argv[i], "-m") == 0 ){
			m_vecMesh.push_back( atol(argv[++i]) );
		}
		if( strcmp(argv[i], "-l") == 0 ){
			if( !loadMeshlist(argv[++i]) )
				return false;
		}	
		if( strcmp(argv[i], "-d") == 0 ){		// Bug 5701対応 07/11/30追加
			m_dMeter = atof(argv[++i]);
		}
	}
	if(m_dMeter < 0){
		cerr << "#Error 閾値の設定が間違っています" << endl;
		return false;
	}
	return true;
}


// 環境変数取得し、必要情報取得
bool CCheckTelTPPoint::setInfoFromSDE()
{
	// 住所系接続(TelTP用)
	if( NULL == (m_ipTPWorkspace = gf::getWorkspace(m_rEnv.m_strTPDBProp)) )
	{
		cerr << "#Error 住所系DB(TelTP)への接続に失敗 : " << m_rEnv.m_strTPDBProp << endl;
		return false;
	}
	cout << "【住所系DB(TelTP)接続】" << m_rEnv.m_strTPDBProp << endl;

	// 住所系接続(行政界用)
	const IWorkspacePtr ipAddrWorkspace = gf::getWorkspace( m_rEnv.m_strAddrDBProp );
	if( NULL == ipAddrWorkspace )
	{
		cerr << "#Error 住所系DB(行政界)への接続に失敗 : " << m_rEnv.m_strAddrDBProp << endl;
		return false;
	}
	cout << "【住所系DB(行政界)接続】" << m_rEnv.m_strAddrDBProp << endl;

	// 地図系接続(都市地図)
	const IWorkspacePtr ipCityWorkspace = gf::getWorkspace( m_rEnv.m_strCityDBProp );
	if( NULL == ipCityWorkspace )
	{
		cerr << "#Error 地図系DB(都市地図)への接続に失敗 : " << m_rEnv.m_strCityDBProp << endl;
		return false;
	}
	cout << "【地図系DB(都市地図)接続】" << m_rEnv.m_strCityDBProp << endl;

	// 地図系接続(中縮)
	const IWorkspacePtr ipBaseWorkspace = gf::getWorkspace( m_rEnv.m_strBaseDBProp );
	if( NULL == ipBaseWorkspace )
	{
		cerr << "#Error 地図系DB(中縮)への接続に失敗 : " << m_rEnv.m_strBaseDBProp << endl;
		return false;
	}
	cout << "【地図系DB(中縮)接続】" << m_rEnv.m_strBaseDBProp << endl;

	// POI接続(Bug 5701対応 07/11/30追加)
	if( NULL == (m_ipPOIWorkspace = gf::getWorkspace(m_rEnv.m_strPOIDBProp)) )
	{
		cerr << "#Error POIへの接続に失敗 : " << m_rEnv.m_strPOIDBProp << endl;
		return false;
	}
	cout << "【POI接続】" << m_rEnv.m_strPOIDBProp << endl;

	// フィーチャクラス取得
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strTelTPFName), &m_ipTelTPFC );
	if( NULL == m_ipTelTPFC )
	{
		cerr << "#Error 電話番号検索ポイントフィーチャクラスの取得に失敗 : " << m_rEnv.m_strTelTPFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strCityAdminFName), &m_ipCityAdminFC );
	if( NULL == m_ipCityAdminFC )
	{
		cerr << "#Error 都市地図行政界フィーチャクラスの取得に失敗 : " << m_rEnv.m_strCityAdminFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipCityWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strCitySiteFName), &m_ipCitySiteFC );
	if( NULL == m_ipCitySiteFC )
	{
		cerr << "#Error 都市地図背景フィーチャクラスの取得に失敗 : " << m_rEnv.m_strCitySiteFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(ipBaseWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strBaseSiteFName), &m_ipBaseSiteFC );
	if( NULL == m_ipBaseSiteFC )
	{
		cerr << "#Error 中縮背景フィーチャクラスの取得に失敗 : " << m_rEnv.m_strBaseSiteFName << endl;
		return false;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(_T("REFERENCE.CityMesh")), &m_ipCityMeshFC );
	if( NULL == m_ipCityMeshFC )
	{
		cerr << "#Error 都市地図メッシュフィーチャクラスの取得に失敗 : REFERENCE.CityMesh" << endl;
		return false;
	}
	// POIフィーチャクラス取得(Bug 5701対応 07/11/30追加)
	IFeatureWorkspacePtr(m_ipPOIWorkspace)->OpenFeatureClass( CComBSTR(m_rEnv.m_strPOIFName), &m_ipPOIFC );
	if( NULL == m_ipPOIFC )
	{
		cerr << "#Error POIフィーチャクラスの取得に失敗 : " << m_rEnv.m_strPOIFName << endl;
		return false;
	}
	
	// フィールドインデックス
	if( FAILED(m_ipCitySiteFC->FindField(CComBSTR(sindy::schema::city_site::kBgClass), &m_lCBGClassC_FID)) 
		|| FAILED(m_ipBaseSiteFC->FindField(CComBSTR(sindy::schema::base_site::kSC4BgClass), &m_lBSC4BGClassC_FID)) )
	{
		cerr << "#Error 背景種別フィールドインデックスの取得に失敗" << endl;
		return false;
	}

	return true;
}


// メッシュリスト読み込み
bool CCheckTelTPPoint::loadMeshlist( LPCTSTR lpszFile )
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

// メッシュフィーチャ取得
IFeaturePtr CCheckTelTPPoint::getMeshFeature( long lMeshCode )
{
	CString strWhere;
	strWhere.Format( _T("%s=%d"), sindy::schema::citymesh::kMeshCode, lMeshCode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor == NULL )
		return NULL;

	IFeaturePtr ipMeshFeature;
	return( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ? ipMeshFeature : NULL );
}

// チェックメイン
void CCheckTelTPPoint::checkMain()
{
	LONG lAllCount = 0;
	m_ipTelTPFC->FeatureCount( NULL, &lAllCount );
	IFeatureCursorPtr ipFeatureCursor;
	m_ipTelTPFC->Search( NULL, VARIANT_FALSE, &ipFeatureCursor );
	if( NULL == ipFeatureCursor )
	{
		std::cerr << "TelTPポイントはありません" << std::endl;
		return;
	}

	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
	{
		LONG OID;						/// 12/7 メモリ負荷対策のため修正
		ipFeature->get_OID( &OID );		/// ↑（ベクターにCTelTPオブジェクトを格納していたものを、オブジェクトIDのみ格納するように変更）

		m_vecTelTP.push_back( OID );	/// 全件時は、ORA-1555対策のため、メモリに格納
	}
	LONG lCount = 1;
	for( std::vector<LONG>::const_iterator itr=m_vecTelTP.begin();itr!=m_vecTelTP.end();++itr )
	{
		// オブジェクトIDからCTelTPを作成
		IFeaturePtr ipFeature;
		m_ipTelTPFC->GetFeature( *itr, &ipFeature ); 
		CTelTP cTelTP( ipFeature );

		checkTelTPAndAdmin( cTelTP );

		checkTelTPAndSite( cTelTP );

		checkTelTPPoints( cTelTP );

		cerr << lCount++ << " / " << lAllCount << " 件完了\r";
	}
}

// メッシュごとにチェック
void CCheckTelTPPoint::checkMainByMesh()
{
	// メッシュフィーチャクラスの取得(先頭にあるデータから長さ識別)
	CString strMeshFName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::middlemesh::kTableName );
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::basemesh::kTableName );
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strMeshFName.Format( _T("REFERENCE.%s"), sindy::schema::citymesh::kTableName );
	else
	{
		cerr << "#Error メッシュコードが、不正です" << endl;
		return;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(strMeshFName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
	{
		cerr << "#Error メッシュフィーチャクラスの取得に失敗" << endl;
		return;
	}

	//1メッシュごとのループ
	for( vector<long>::iterator itr=m_vecMesh.begin();itr!=m_vecMesh.end();++itr )
	{
		// メッシュフィーチャの取得
		IFeaturePtr ipMeshFeature = getMeshFeature(*itr);
		if( NULL == ipMeshFeature )
		{
			cout << *itr << " : メッシュフィーチャの取得に失敗" << endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_Shape( &ipMeshGeo );
		
		// Arc10.1対応でキャッシュを使用しないよう修正		
		IFeatureCursorPtr ipFeatureCursor;
		LONG lAllCount = gf::SelectByShapeWithCount( ipMeshGeo, m_ipTelTPFC, &ipFeatureCursor, esriSpatialRelIntersects );
		if( lAllCount < 1 )
		{
			cout << *itr << " : メッシュ内にTelTPポイントはありません" << endl;
			continue;
		}
		LONG lCount = 1;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			CTelTP ob(ipFeature);

			checkTelTPAndAdmin( ob );
				
			checkTelTPAndSite( ob );

			checkTelTPPoints( ob );

			cerr << lCount++ << " / " << lAllCount << " 件完了\r";
		}
		cout << *itr << " : チェック完了" << endl;
	}
}

// 都市地図行政界との整合チェック
void CCheckTelTPPoint::checkTelTPAndAdmin( const CTelTP& cTelTP )
{
	// TelTPポイントが位置する都市地図行政界取得
	LONG lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature ){
		m_cError.printLog( cTelTP, E_DUP_OR_NOTON_ADMIN );	// E8
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	CComVariant vaValue;

	switch( cTelTP.m_ePointStatC )
	{
	case GAITOU_SISETU:		// 該当施設あり

		if( cTelTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
			m_cError.printLog( cTelTP, W_GAITOUSISETU_CITYCODE );	//W4
		}

		if(cTelTP.getAddrCode(11).compare(5, 6, "000000") != 0 ){
			if( cTelTP.getAddrCode(8) != cAdmin.getAddrCode(8) ){
				m_cError.printLog( cTelTP, W_GAITOUSISETU_OAZACODE );	//W3
			}
		
			switch( cAdmin.m_eAddrClassC )
			{
			case OAZA_NOTSEIBI:			// 町丁目界未整備
			case JUKYO_TIBAN_NOTSEIBI:	// 住居表示・地番未整備
				if( cTelTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
					m_cError.printLog( cTelTP, W_GAITOUSISETU_ADDRCODE_NOTSEIBI );	// W1
				}
				break;
			default:
				if( cTelTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
					m_cError.printLog( cTelTP, W_GAITOUSISETU_ADDRCODE_SEIBI );	// W2
				}
				break;
			}
		}
		break;
	case HORYU:				// 保留
		m_cError.printLog( cTelTP, N_HORYU_POINTSTAT );	// N5
		break;

	case UNKNOWN_POS:		// 位置不明
		break;

	case NOSURVEY:			// 未調査
		
		// 最終更新者が「sindy」かチェック
		vaValue = cTelTP.getValue( sindy::schema::teltp_point::kOperator );
		if( vaValue.vt != VT_BSTR || strcmp(COLE2T(vaValue.bstrVal), "sindy") != 0 ){
			m_cError.printLog( cTelTP, E_NOSURVEY_OPERATOR );	// E6
		}
		break;

	default:
		m_cError.printLog( cTelTP, E_UNKNOWN_POINTSTAT );	// E7
		break;
	}
}


// 背景との整合チェック
void CCheckTelTPPoint::checkTelTPAndSite( const CTelTP& cTelTP )
{
	// 調査結果コードが「該当施設あり」の場合のみ
	if( cTelTP.m_ePointStatC == GAITOU_SISETU )
	{
		// 背景が一つに絞れないときは、チェックしない
		LONG lCount = 0;
		
		// TelTPポイントが位置する都市地図背景取得
		IFeaturePtr ipCitySiteFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'C' );
		if(lCount > 1){
			if( NULL == ipCitySiteFeature ){
				m_cError.printLog( cTelTP, E_DUP_OR_NOTON_CITYSITE);	// E12
				return;
			}
		}
		else if(lCount == 1 ){
			CSite cCitySite( ipCitySiteFeature, 'C' );
			if( isNGCitySite(cCitySite.m_lBGClassC) )
				m_cError.printLog( cTelTP, E_ON_NGCITYSITE , &(cCitySite.getBGClassDomain()) );	// E10
		}
		else{
			//都市メッシュポリゴンがない場合は背景なしエラーを出力しない。
			CComVariant vaValue;
			IFeatureCursorPtr ipFeatureCursor;
			IFeaturePtr ipMeshFeature;

			//都市メッシュポリゴンが無い場合、CREATE_YEARはNULLが入っている。ある場合は作成年。
			LONG lMCount = gf::SelectByShapeWithCount( cTelTP.getGeo(), m_ipCityMeshFC, &ipFeatureCursor, esriSpatialRelIntersects );
			LONG m_nCreate_year = 0;
			m_ipCityMeshFC->FindField(CComBSTR(sindy::schema::citymesh::kCreateYear), &m_nCreate_year); 
			while( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ){
				ipMeshFeature->get_Value( m_nCreate_year, &vaValue );
				if( vaValue.vt != VT_NULL ){
					string string_NULL = "背景なし[細街路]";
					m_cError.printLog( cTelTP, E_ON_NGCITYSITE, &string_NULL);	// E10
					break;
				}
			}
		}

		lCount = 0;

		// TelTPポイントが位置する中縮背景取得
		IFeaturePtr ipBaseSiteFeature = getFeatureWithinPoint( cTelTP.getGeo(), &lCount, 'B' );
		if( NULL == ipBaseSiteFeature && lCount > 1 ){
			m_cError.printLog( cTelTP, E_DUP_OR_NOTON_BASESITE);	// E13
			return;
		}

		if( lCount == 1 ){
			CSite cBaseSite( ipBaseSiteFeature, 'B' );
			if( isNGBaseSite(cBaseSite.m_lBGClassC) )
				m_cError.printLog( cTelTP, E_ON_NGBASESITE, &(cBaseSite.getBGClassDomain()) );	// E11
		}
	}
}


//同一緯度経度のポイントの調査結果コードチェック
void CCheckTelTPPoint::checkTelTPPoints( const CTelTP& cTelTP )
{
	IFeatureCursorPtr ipFeaturePointCursor;

	// 同じジオメトリに他のTelTPがあるかどうか検索
	LONG lPointCount = gf::SelectByShapeWithCount( cTelTP.getGeo(), m_ipTelTPFC, &ipFeaturePointCursor, esriSpatialRelContains );
	if(lPointCount > 1)
	{
 		IFeaturePtr ipFeaturePoint;
		while( ipFeaturePointCursor->NextFeature( &ipFeaturePoint ) == S_OK)
		{
			// 調査結果コードチェック
			CTelTP ob_Point( ipFeaturePoint );
			if(ob_Point.m_ePointStatC != cTelTP.m_ePointStatC )
			{
				m_cError.printLog( cTelTP, E_PLURAL_OPERATOR );	// E14
				break;
			}
		}
	}
	// 調査結果コードが「該当施設あり」の場合のみ
	if(cTelTP.m_ePointStatC == GAITOU_SISETU){
		
		// 電話番号のフリーダイヤルチェック
		if( cTelTP.m_strTelNum.find("0120") == 0 )
			m_cError.printLog( cTelTP, W_TELNUM_FREECALL );		// W9

		// IPCODEチェック(BUG 5701対応 07/11/30追加)
		CheckIPCODE( cTelTP );

		// 同一シーケンス重複チェック（Bug9295 対応）
		CheckSameSequence( cTelTP );
	}
}


// ポイントが乗っているポリゴンを取得
IFeaturePtr CCheckTelTPPoint::getFeatureWithinPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch )
{
	IFeatureCursorPtr ipFeatureCursor;
	switch( szSwitch )
	{
	case 'A':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityAdminFC, &ipFeatureCursor, esriSpatialRelWithin );
		break;
	case 'C':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCitySiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'B':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipBaseSiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'M':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipMeshFC, &ipFeatureCursor, esriSpatialRelWithin );
		break;
	default:
		return NULL;
	}

	// 背景ポリゴン一つに絞れないときは、NULLを返す
	if( *lpCount == 1 ){
		IFeaturePtr ipFeature;
		return ( SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature))? ipFeature : NULL );
	}
	else
		return NULL;
}

// TelTPの周囲のPOIのIPコード&電話番号チェック(Bug 5701対応 07/11/30追加)　
void CCheckTelTPPoint::CheckIPCODE( const CTelTP& cTelTP )
{
	IGeometryPtr ipGeoBuffer;			// TelTPの周囲の空間のジオメトリ用
	IFeatureCursorPtr ipPOICursor;		
	IEnvelopePtr ipEnvelop;
	cTelTP.getGeo()->get_Envelope( &ipEnvelop );

	// 引数で指定された値の分だけTelTPのジオメトリを広げる ＝ TelTPの周囲○mの空間のジオメトリ作成
	ipEnvelop->Expand(m_dMeter, m_dMeter, VARIANT_FALSE );
	{
		// TelTPの周囲のジオメトリに対してPOIの空間探索をおこなった結果取得
		LONG POICount = gf::SelectByShapeWithCount(ipEnvelop, m_ipPOIFC, &ipPOICursor, esriSpatialRelIntersects);
		if( POICount > 0 )
		{
			IFeaturePtr ipPOIFeature;		
			IGeometryPtr ipPOIGeo;			// TelTPの周囲にあるPOI単体のジオメトリ
			while( ipPOICursor->NextFeature( &ipPOIFeature ) == S_OK )
			{
				// POIのフィールド取得準備（中身CFeature::getValueからassertを抜いたもの）
				ipPOIFeature->get_Shape( &ipPOIGeo );
				IFieldsPtr ipPOIFields;
				ipPOIFeature->get_Fields( &ipPOIFields );
				LONG lFID = 0;
				CComVariant vaValue;				
			
				// POIのIPCODEフィールド取得
				ipPOIFields->FindField( CComBSTR(_T("IPCODE")), &lFID );
				ipPOIFeature->get_Value( lFID, &vaValue );
				string strIPCODE;
				if( vaValue.vt == VT_BSTR )
					strIPCODE = COLE2T( vaValue.bstrVal );
				vaValue.Clear();
				
				// 昭文社（グルメ、祭り・イベント、花火） ⇒　ＩＰコード=「02」
				// ＩＭＪモバイル駐車場　⇒　ＩＰコード=「52」
				// 夜景　⇒　ＩＰコード=「67」
				if( strIPCODE == "02" || strIPCODE == "52" || strIPCODE == "67" )
				{
					// POIのTELフィールド取得
					lFID = 0;
					ipPOIFields->FindField( CComBSTR(_T("TEL")), &lFID );
					ipPOIFeature->get_Value( lFID, &vaValue );
					string strTELNUM;
					if( vaValue.vt == VT_BSTR )
						strTELNUM = COLE2T( vaValue.bstrVal );
					vaValue.Clear();

					// 上記IPコードを持つPOIがTelTPの周囲にあり、同一の電話番号を持つ場合エラー出力
					if(strTELNUM == cTelTP.m_strTelNum){
						
						// 距離を取得するための外部クラス
						crd_cnv	a_cCrd;
						
						// TelTPの(x,y)座標を取得
						double dX_TelTP, dY_TelTP;
						IPointPtr ipTelTPPoint( cTelTP.getGeo() );
						ipTelTPPoint->get_X( &dX_TelTP );
						ipTelTPPoint->get_Y( &dY_TelTP );

						// POIの(x,y)座標を取得
						double dX_POI, dY_POI;
						IPointPtr ipPOIPoint( ipPOIGeo );
						ipPOIPoint->get_X( &dX_POI );
						ipPOIPoint->get_Y( &dY_POI );

						m_cError.printLog( cTelTP, E_POI_TABOO_IPCODE, a_cCrd.GetDist(dX_POI, dY_POI, dX_TelTP, dY_TelTP));	// E15
					}
				}
			}
		}
	}
}

// 同一シーケンスを持つポイントが複数存在しないかチェック（Bug9295 追加）
void CCheckTelTPPoint::CheckSameSequence( const CTelTP& cTelTP )
{
	// 同一シーケンスを持つもの、自身ではないものの検索
	CString strWhere;
	strWhere.Format( _T("%s=%d and OBJECTID<>%d"), sindy::schema::teltp_point::kSequence, cTelTP.m_lSequence, cTelTP.getOID() );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipTelTPFC->Search( ipQuery, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
		return;

	IFeaturePtr ipFeature;
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
	{
		// 同一シーケンスを持つTelTPが存在
		CTelTP ob(ipFeature);

		// 見つかったTelTPの調査結果コードが「該当施設あり」の場合はエラー
		if( GAITOU_SISETU == ob.m_ePointStatC )
		{
			m_cError.printLog( cTelTP, E_SAME_SEQUENCE );	// E16
			break;
		}
	}
}























