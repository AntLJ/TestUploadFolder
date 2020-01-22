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

// CheckExTPPoint.cpp: CCheckExTPPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckExTPPoint.h"
#include "GlobalFunc.h"
#include <iterator>
#include "sindy/schema/map.h"
#include <boost/assign.hpp>
using namespace boost::assign;

using namespace std;
using namespace sindy::schema;
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const std::set<long> CCheckExTPPoint::m_CBGClass_NG = 
	list_of
	(city_site::bg_class::kSands)                    // 砂地
	(city_site::bg_class::kMarsh)                    // 湿地
	(city_site::bg_class::kRiver)                    // 河川
	(city_site::bg_class::kPool)                     // プール
	(city_site::bg_class::kSea)                      // 海
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
	(city_site::bg_class::kDamGuideNotExist)         // ダム（便覧記載なし）
	(city_site::bg_class::kDamGuideExist)            // ダム（便覧記載あり）
	(city_site::bg_class::kRunway)                   // 滑走路
	(city_site::bg_class::kUnderRepairRoad)          // 工事中道路
	(city_site::bg_class::kTollGateLane)             // 料金所レーン
	;
const set<long> CCheckExTPPoint::m_BBGClass_NG = 
	list_of
	(base_site::bg_class::kSea)          // 海
	(base_site::bg_class::kRiver)        // 河川・沢・谷
	(base_site::bg_class::kLake)         // 湖・池・沼
	(base_site::bg_class::kSands)        // 砂地
	(base_site::bg_class::kMarsh)        // 湿地
	(base_site::bg_class::kSwimmingPool) // プール
	(base_site::bg_class::kDam);         // ダム

CCheckExTPPoint::CCheckExTPPoint() : 
	m_emCheckMode(kNormal),
	m_lCBGClassC_FID(-1),
	m_lBSC4BGClassC_FID(-1),
	m_lCreateYear_FID(-1)
{
}

CCheckExTPPoint::~CCheckExTPPoint()
{

}

// 使用法
void CCheckExTPPoint::printUsage()
{
	cerr 
		<< "【拡張電話番号検索ポイントチェックツール】\n"
		<< "(Usage)CheckEXTPPoint.exe [option] [logfile]\n"
		<< "・チェックモードオプション(指定なしのときは、通常チェック)\n"
		<< "  -e normal/alps/acc4/ips/hlp    normal : 通常, alps : アルプスのみ, acc4 : iPC(精度4)のみ, ips : iPSチェックモード, hlp : HLP\n"
		<< "・チェック範囲オプション(指定なしのときは、全件チェック)\n"
		<< "  -m [meshcode]  メッシュコード指定\n"
		<< "  -l [meshlist]  メッシュリスト指定\n"
		<< "・環境変数について\n"
		<< "  DB_TP         TP系DB接続プロパティ\n"
		<< "  DB_ADDRESS    住所系DB接続プロパティ\n"
		<< "  DB_MAP        都市地図系DB接続プロパティ\n"
		<< "  DB_BASE       中縮系DB接続プロパティ\n"
		<< "  DB_MESH       メッシュ系DB接続プロパティ(CityMeshで使用)\n"
		<< "  FC_EXTP_POINT 拡張電話番号検索ポイントフィーチャクラス\n"
		<< "  FC_CITY_ADMIN 都市地図行政界フィーチャクラス\n"
		<< "  FC_CITY_SITE  都市地図背景フィーチャクラス\n"
		<< "  FC_BASE_SITE  中縮背景フィーチャクラス\n"
		<< "  FC_CITYMESH   都市地図メッシュフィーチャクラス\n" << endl;
}

// 初期化
bool CCheckExTPPoint::init( int argc, char* argv[] )
{
	for( int i = 1; i < argc-2; ++i )
	{
		if( strcmp(argv[i], "-e") == 0 )
		{
			++i;
			if( strcmp(argv[i], "normal") == 0 )
				m_emCheckMode = kNormal;
			else if( strcmp(argv[i], "alps") == 0 )
				m_emCheckMode = kAlps;
			else if( strcmp(argv[i], "acc4") == 0 )
				m_emCheckMode = kAcc4;
			else if( strcmp(argv[i], "ips") == 0 )
				m_emCheckMode = kiPS;
			else if( strcmp(argv[i], "hlp") == 0 )
				m_emCheckMode = kHlp;
			else
				EXE_ERRORLOG( "#Error -e オプションのときはnormal/alps/acc4/ips/hlpのいずれかしか指定できません", argv[i], false );
		}
		else if( strcmp(argv[i], "-m") == 0 )
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( strcmp(argv[i], "-l") == 0 )
		{
			if( !loadMeshlist(argv[++i]) )
				EXE_ERRORLOG( "#Error メッシュリストの読み込みに失敗", argv[i], false );
		}
	}

	// ログファイル
	if( !m_cError.open(argv[argc-1]) )
		EXE_ERRORLOG( "#Error ログファイルのオープンに失敗", argv[argc-1], false );

	if( !setInfoFromDB() )
		return false;

	m_cError.setFCName( m_strExTPFCName );
	m_cError.printHeader();

	return true;
}

// 実行
void CCheckExTPPoint::execute()
{
	// Where句設定
	switch( m_emCheckMode )
	{
	case kNormal:
		m_strWhereClause = _T("INFOSRC_C=1");
		cerr << "通常チェックモードで実行します\n" << endl;
		break;
	case kAlps:
		m_strWhereClause = _T("INFOSRC_C=2");
		cerr << "アルプスチェックモードで実行します\n" << endl;
		break;
	case kAcc4:
		m_strWhereClause = _T("INFOSRC_C=3");
		cerr << "iPC(精度4)チェックモードで実行します\n" << endl;
		break;
	case kHlp:
		m_strWhereClause = _T("INFOSRC_C=4");
		cerr << "HLPチェックモードで実行します\n" << endl;
		break;
	case kiPS:
		m_strWhereClause = _T("INFOSRC_C IS NOT NULL");
		cerr << "iPSチェックモードで実行します\n" << endl;
		break;
	default:
		return;
	}

	// iPSチェックモード時は、常に全件
	if( m_vecMesh.empty() || kiPS == m_emCheckMode )
		checkMain();
	else
		checkMainByMesh();
}

/**
 * 内部関数
 */

// 環境変数取得し、必要情報取得
bool CCheckExTPPoint::setInfoFromDB()
{
	// 先に環境変数取得
#ifdef _DEBUG
	_tputenv( _T("DB_TP=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_ADDRESS=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_MAP=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_BASE=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("DB_MESH=D:\\temp\\GEOSPACE\\51357375.mdb") );
	_tputenv( _T("FC_EXTP_POINT=ExTP_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_CITY_SITE=City_Site") );
	_tputenv( _T("FC_BASE_SITE=Base_Site") );
	_tputenv( _T("FC_CITYMESH=CityMesh") );
#endif

	CString strEnv;

	// 住所系接続(ExTP用)
	if( !strEnv.GetEnvironmentVariable(_T("DB_TP")) )
		EXE_ERRORLOG( "#Error 環境変数 DB_TP の取得に失敗", NULL, false );
	if( NULL == (m_ipTPWorkspace = gf::getWorkspace(strEnv)) )
		EXE_ERRORLOG( "#Error 住所系DB(ExTP)への接続に失敗", strEnv, false );
	cout << "【住所系DB(ExTP)接続】" << strEnv << endl;

	if( !m_strExTPFCName.GetEnvironmentVariable(_T("FC_EXTP_POINT")) )
			EXE_ERRORLOG( "#Error 環境変数 FC_EXTP_POINT の取得に失敗", NULL, false );
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(m_strExTPFCName), &m_ipExTPFC );
	if( NULL == m_ipExTPFC )
		EXE_ERRORLOG( "#Error 拡張電話番号検索ポイントフィーチャクラスの取得に失敗", m_strExTPFCName, false );

	// 住所系接続(行政界用)
	if( !strEnv.GetEnvironmentVariable(_T("DB_ADDRESS")) )
		EXE_ERRORLOG( "#Error 環境変数 DB_ADDRESS の取得に失敗", NULL, false );

	const IWorkspacePtr ipAddrWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipAddrWorkspace )
		EXE_ERRORLOG( "#Error 住所系DB(行政界)への接続に失敗", strEnv, false );
	cout << "【住所系DB(行政界)接続】" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
			EXE_ERRORLOG( "#Error 環境変数 FC_CITY_ADMIN の取得に失敗", NULL, false );
	IFeatureWorkspacePtr(ipAddrWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCityAdminFC );
	if( NULL == m_ipCityAdminFC )
		EXE_ERRORLOG( "#Error 都市地図行政界フィーチャクラスの取得に失敗", strEnv, false );

	// 地図系接続(都市地図)
	if( !strEnv.GetEnvironmentVariable(_T("DB_MAP")) )
		EXE_ERRORLOG( "#Error 環境変数 DB_MAP の取得に失敗", NULL, false );
	const IWorkspacePtr ipCityWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipCityWorkspace )
		EXE_ERRORLOG( "#Error 地図系DB(都市地図)への接続に失敗", strEnv, false );
	cout << "【地図系DB(都市地図)接続】" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITY_SITE")) )
			EXE_ERRORLOG( "#Error 環境変数 FC_CITY_SITE の取得に失敗", NULL, false );
	IFeatureWorkspacePtr(ipCityWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCitySiteFC );
	if( NULL == m_ipCitySiteFC )
		EXE_ERRORLOG( "#Error 都市地図背景フィーチャクラスの取得に失敗", strEnv, false );

	if( FAILED(m_ipCitySiteFC->FindField(CComBSTR(_T("BGCLASS_C")), &m_lCBGClassC_FID)) )
			EXE_ERRORLOG( "#Error 都市地図背景種別フィールドインデックスの取得に失敗", NULL, false );

	// メッシュ系接続
	if( !strEnv.GetEnvironmentVariable(_T("DB_MESH")) )
		EXE_ERRORLOG( "#Error 環境変数 DB_MESH の取得に失敗", NULL, false );
	const IWorkspacePtr ipMeshWorkspace = gf::getWorkspace( strEnv );
	if( NULL == ipMeshWorkspace )
		EXE_ERRORLOG( "#Error メッシュ系DB(都市地図)への接続に失敗", strEnv, false );
	cout << "【メッシュ系DB接続】" << strEnv << endl;

	if( !strEnv.GetEnvironmentVariable(_T("FC_CITYMESH")) )
			EXE_ERRORLOG( "#Error 環境変数 FC_CITYMESH の取得に失敗", NULL, false );
	IFeatureWorkspacePtr(ipMeshWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipCityMeshFC );
	if( NULL == m_ipCityMeshFC )
		EXE_ERRORLOG( "#Error 都市地図メッシュフィーチャクラスの取得に失敗", strEnv, false );

	if( FAILED(m_ipCityMeshFC->FindField(CComBSTR(_T("CREATE_YEAR")), &m_lCreateYear_FID)) )
			EXE_ERRORLOG( "#Error 都市地図メッシュ作成年度フィールドインデックスの取得に失敗", NULL, false );

	// [Bug6320]iPSチェックモード時は無視
	if( kiPS != m_emCheckMode )
	{
		// 地図系接続(中縮)	
		if( !strEnv.GetEnvironmentVariable(_T("DB_BASE")) )
			EXE_ERRORLOG( "#Error 環境変数 DB_BASE の取得に失敗", NULL, false );

		const IWorkspacePtr ipBaseWorkspace = gf::getWorkspace( strEnv );
		if( NULL == ipBaseWorkspace )
			EXE_ERRORLOG( "#Error 地図系DB(中縮)への接続に失敗", strEnv, false );
		cout << "【地図系DB(中縮)接続】" << strEnv << endl;

		if( !strEnv.GetEnvironmentVariable(_T("FC_BASE_SITE")) )
			EXE_ERRORLOG( "#Error 環境変数 FC_BASE_SITE の取得に失敗", NULL, false );

		IFeatureWorkspacePtr(ipBaseWorkspace)->OpenFeatureClass( CComBSTR(strEnv), &m_ipBaseSiteFC );
		if( NULL == m_ipBaseSiteFC )
			EXE_ERRORLOG( "#Error 中縮背景フィーチャクラスの取得に失敗", strEnv, false );

		if( FAILED(m_ipBaseSiteFC->FindField(CComBSTR(_T("SC4BGCLASS_C")), &m_lBSC4BGClassC_FID)) )
			EXE_ERRORLOG( "#Error 中縮背景種別フィールドインデックスの取得に失敗", NULL, false );
	}

	return true;
}

// メッシュリスト読み込み
bool CCheckExTPPoint::loadMeshlist( LPCTSTR lpszFile )
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
IFeaturePtr CCheckExTPPoint::getMeshFeature( long lMeshCode )
{
	CString strWhere;
	strWhere.Format( _T("MESHCODE=%d"), lMeshCode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipTargetMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor == NULL )
		return NULL;

	IFeaturePtr ipMeshFeature;
	return( ipFeatureCursor->NextFeature(&ipMeshFeature) == S_OK ? ipMeshFeature : NULL );
}

// チェックメイン
void CCheckExTPPoint::checkMain()
{
	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(m_strWhereClause) );

	long lAllCount = 0;
	m_ipExTPFC->FeatureCount( ipQuery, &lAllCount );
	IFeatureCursorPtr ipFeatureCursor;
	m_ipExTPFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor )
	{
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			m_vecExTP.push_back( CExTP(ipFeature) );	/// 全件時は、ORA-1555対策のため、メモリに格納
		}

		long lCount = 1;
		for( std::vector<CExTP>::const_iterator itr=m_vecExTP.begin();itr!=m_vecExTP.end();++itr )
		{
			switch( m_emCheckMode )
			{
			case kNormal:
			case kAcc4:
			case kiPS:
			case kHlp:
				checkExTPAndAdmin( *itr );
				break;
			case kAlps:
				checkAlpsAndAdmin( *itr );
				break;
			}

			checkExTPAndSite( *itr );
			checkSame20Addrcode( *itr );

			cerr << lCount++ << " / " << lAllCount << " 件完了\r";
		}
		cerr << endl;
	}
}

// メッシュごとにチェック
void CCheckExTPPoint::checkMainByMesh()
{
	// メッシュフィーチャクラスの取得(先頭にあるデータから長さ識別)
	CString strMeshFName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strMeshFName = _T("REFERENCE.MiddleMesh");
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strMeshFName = _T("REFERENCE.BaseMesh");
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strMeshFName = _T("REFERENCE.CityMesh");
	else
	{
		cerr << "#Error メッシュコードが、不正です" << endl;
		return;
	}
	IFeatureWorkspacePtr(m_ipTPWorkspace)->OpenFeatureClass( CComBSTR(strMeshFName), &m_ipTargetMeshFC );
	if( NULL == m_ipTargetMeshFC )
	{
		cerr << "#Error メッシュフィーチャクラスの取得に失敗" << endl;
		return;
	}

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
		
		IFeatureCursorPtr ipFeatureCursor;
		LONG lAllCount = gf::SelectByShapeWithCount( ipMeshGeo, m_ipExTPFC, &ipFeatureCursor, esriSpatialRelIntersects, m_strWhereClause );
		if( lAllCount < 1 )
		{
			cout << *itr << " : メッシュ内にExTPポイントはありません" << endl;
			continue;
		}
		LONG lCount = 1;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			CExTP ob(ipFeature);
			switch( m_emCheckMode )
			{
			case kNormal:
			case kAcc4:
			case kHlp:
				checkExTPAndAdmin( ob );	break;
			case kAlps:		checkAlpsAndAdmin( ob );	break;
			}

			checkExTPAndSite( ob );
			checkSame20Addrcode( ob );

			cerr << lCount++ << " / " << lAllCount << " 件完了\r";
		}

		cout << *itr << " : チェック完了" << endl;
	}
}

// 都市地図行政界との整合チェック
void CCheckExTPPoint::checkExTPAndAdmin( const CExTP& cExTP )
{
	// ExTPポイントが位置する都市地図行政界取得
	long lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature )
	{
		m_cError.printLog( cExTP, E_DUP_OR_NOTON_ADMIN );	// E19
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	switch( cExTP.m_ePointStatC )
	{
	case GOU_SEIBI:		// GOU整備
	case GOU_EXSEIBI:	// GOU拡張整備
	case REFNUM_DIFF:	// 資料番号と不一致
	case NUM_NOTKEISAI:	// 番号未掲載

		switch( cAdmin.m_eAddrClassC )
		{
		case OAZA_NOTSEIBI:			// 町丁目界未整備
		case JUKYO_TIBAN_NOTSEIBI:	// 住居表示・地番未整備
			if( cExTP.m_ePointStatC == GOU_EXSEIBI ){
				m_cError.printLog( cExTP, E_EXGOU_NOTSEIBI );	// E4
			}				
			else if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
				m_cError.printLog( cExTP, E_MISMATCH_ADDRCODE11_NOTSEIBI );	// N1, N7, W13, N10 の大分類
			}
			break;
		default:
			if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){
				m_cError.printLog( cExTP, E_MISMATCH_ADDRCODE11_SEIBI );	// W2, W5, W8, W11 の大分類
			}
			else{
				switch( cExTP.m_ePointStatC )
				{
				case GOU_SEIBI:		// GOU整備
				case REFNUM_DIFF:	// 資料番号と不一致
				case NUM_NOTKEISAI:	// 番号未掲載
					if( cExTP.getAddrCode(16) != cAdmin.getAddrCode(16) ){
						m_cError.printLog( cExTP, E_MISMATCH_GAIKU );	// W3, N9, N12 の大分類
					}
					break;
				case GOU_EXSEIBI:	// GOU拡張整備
					if( cExTP.getAddrCode(16) == cAdmin.getAddrCode(16) ){
						m_cError.printLog( cExTP, E_MATCH_GAIKU );	// E24 の大分類
					}
					break;
				default:
					break;
				}
			}
			break;
		}
		break;

	case ADMIN_DIFF:	// 行政界不一致

		if( cExTP.getAddrCode(11) == cAdmin.getAddrCode(11) ){
			if( cAdmin.m_eAddrClassC == OAZA_NOTSEIBI || cAdmin.m_eAddrClassC == JUKYO_TIBAN_NOTSEIBI ){
				m_cError.printLog( cExTP, W_ADMIN_ADDRCODE_NOTSEIBI );	// W13
			}
			else{
				m_cError.printLog( cExTP, W_ADMIN_ADDRCODE_SEIBI );	// W14
			}
		}
		break;

	case NOSURVEY:	// 未調査
		{
			CComVariant vaValue = cExTP.getValue( _T("OPERATOR") );
			if( vaValue.vt != VT_BSTR || strcmp(COLE2T(vaValue.bstrVal), "sindy") != 0 )
				m_cError.printLog( cExTP, E_NOSURVEY_OPERATOR );	// E15

		}
		break;
	case TPG_SEIBI:		// TPG整備
		m_cError.printLog( cExTP, E_TPG );	// E6
		break;
	case UNKNOWN_POS:		// 位置不明
	case UNDER_NUM:			// 地下街番号
	case SISETU_NOTKEISAI:	// 施設未掲載
		break;
	default:
		m_cError.printLog( cExTP, E_UNKNOWN_POINTSTAT );	// E18
		break;
	}

	// 5桁コードチェック([位置不明][未調査]のものはチェック対象外)
	if( (cExTP.m_ePointStatC != NOSURVEY && cExTP.m_ePointStatC != UNKNOWN_POS)
		&& cExTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
		m_cError.printLog( cExTP, W_MISMATCH_CITYCODE );	// W16
	}

	// 8桁コードチェック([GOU整備][GOU拡張整備]のみ実施)
	if( cExTP.m_ePointStatC == GOU_SEIBI || cExTP.m_ePointStatC == GOU_EXSEIBI )
	{
		if( cExTP.getAddrCode(8) != cAdmin.getAddrCode(8) )
			m_cError.printLog( cExTP, (cExTP.m_ePointStatC == GOU_SEIBI)? W_GOU_OAZACODE_SEIBI : W_EXGOU_OAZACODE_SEIBI );	// W25 or W26
	}

	// 拡張街区符号上かチェック
	if( !cAdmin.m_strExGaikuFugo.empty() )
		m_cError.printLog( cExTP, E_ON_EXGAIKUFUGO );	// E17

}

// 都市地図行政界との整合チェック(アルプス用)
void CCheckExTPPoint::checkAlpsAndAdmin( const CExTP& cExTP )
{
	// ExTPポイントが位置する都市地図行政界取得
	long lCount = 0;
	IFeaturePtr ipAdminFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'A' );
	if( NULL == ipAdminFeature ){
		m_cError.printLog( cExTP, E_DUP_OR_NOTON_ADMIN );	// E19
		return;
	}
	CCityAdmin cAdmin( ipAdminFeature );

	// 5桁コード
	if( cExTP.getAddrCode(5) != cAdmin.getAddrCode(5) ){
		m_cError.printLog( cExTP, W_ALPS_CITYCODE );	// W27
		return;
	}

	// 8桁コード
	if( cExTP.getAddrCode(8) != cAdmin.getAddrCode(8) ){
		m_cError.printLog( cExTP, W_ALPS_OAZACODE );	// W28
		return;
	}

	// 11桁コード
	if( cExTP.getAddrCode(11) != cAdmin.getAddrCode(11) ){

		switch( cAdmin.m_eAddrClassC  )
		{
		case JUKYO_SEIBI:			/// 住居表示整備済み
		case TIBAN_SEIBI:			/// 地番整備済み
			m_cError.printLog( cExTP, N_ALPS_ADDRCODE_SEIBI );	// N29
			break;
		default:
			m_cError.printLog( cExTP, N_ALPS_ADDRCODE_NOTSEIBI );	// N30
			break;
		}
	}
	else{
		// 11桁等しいなら街区チェック
		if( cExTP.getAddrCode(16) != cAdmin.getAddrCode(16) ){
			switch( cAdmin.m_eAddrClassC  )
			{
			case JUKYO_SEIBI:			/// 住居表示整備済み
			case TIBAN_SEIBI:			/// 地番整備済み
				m_cError.printLog( cExTP, N_ALPS_GAIKU_SEIBI );	// N31
				break;
			default:
				break;
			}
		}
	}
}

// 背景との整合チェック
void CCheckExTPPoint::checkExTPAndSite( const CExTP& cExTP )
{
	// チェックモードがアルプスでなく、調査結果コードが[位置不明][未調査]のものはチェック対象外
	if( m_emCheckMode != kAlps
		&& cExTP.m_ePointStatC == NOSURVEY || cExTP.m_ePointStatC == UNKNOWN_POS )
		return;

	// ExTPポイントが位置する都市地図背景取得
	long lCount = 0;
	IFeaturePtr ipCitySiteFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'C' );
	if( lCount == 0 )
	{
		// [Bug 5622]都市地図がある場所ならばエラー出力
		IFeatureCursorPtr ipMeshCursor;
		long lMeshCount = gf::SelectByShapeWithCount( cExTP.getGeo(), m_ipCityMeshFC, &ipMeshCursor, esriSpatialRelIntersects, NULL, _T("CREATE_YEAR") );
		if( lMeshCount > 0 )
		{
			IFeaturePtr ipMeshFeature;
			while( ipMeshCursor->NextFeature(&ipMeshFeature) == S_OK )
			{
				CComVariant vaValue;
				ipMeshFeature->get_Value( m_lCreateYear_FID, &vaValue );
				if( vaValue.vt != VT_NULL )
				{
					m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGCITYSITE : E_ALPS_ON_NGCITYSITE, "背景なし[細街路]" );	// E20 or E32
					break;
				}
			}
		}
	}
	else if( lCount == 1 )
	{
		CSite cCitySite( ipCitySiteFeature, 'C' );
		if( isNGCitySite(cCitySite.m_lBGClassC) )
			m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGCITYSITE : E_ALPS_ON_NGCITYSITE, cCitySite.getBGClassDomain() );	// E20 or E32
	}
	else if(lCount > 1 )	// 複数あり
	{
		m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_DUP_OR_NOTON_CITYSITE : E_ALPS_DUP_OR_NOTON_CITYSITE );	// E22 or E34
	}
	
	// [Bug 6320]中縮背景との整合チェックは、iPSチェックモードでないとき実行
	if( kiPS != m_emCheckMode )
	{
		// ExTPポイントが位置する中縮背景取得
		lCount = 0;
		IFeaturePtr ipBaseSiteFeature = getFeatureIntersectsPoint( cExTP.getGeo(), &lCount, 'B' );
		if( lCount == 1 )
		{
			CSite cBaseSite( ipBaseSiteFeature, 'B' );
			if( isNGBaseSite(cBaseSite.m_lBGClassC) )
				m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_ON_NGBASESITE : E_ALPS_ON_NGBASESITE, cBaseSite.getBGClassDomain() );	// E21 or E33
		}
		else if( lCount > 1 )
		{
			m_cError.printLog( cExTP, (m_emCheckMode != kAlps)? E_DUP_OR_NOTON_BASESITE : E_ALPS_DUP_OR_NOTON_BASESITE );	// E23 or E35
		}
	}
}

// ポイントが乗っているポリゴンを取得
IFeaturePtr CCheckExTPPoint::getFeatureIntersectsPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch )
{
	IFeatureCursorPtr ipFeatureCursor;
	switch( szSwitch )
	{
	case 'A':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityAdminFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'C':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCitySiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'B':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipBaseSiteFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	case 'M':
		*lpCount = gf::SelectByShapeWithCount( ipGeo, m_ipCityMeshFC, &ipFeatureCursor, esriSpatialRelIntersects );
		break;
	default:
		return NULL;
	}

	// 背景ポリゴン一つに絞れないときは、NULLを返す
	if( *lpCount == 1 )
	{
		IFeaturePtr ipFeature;
		return ( SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature))? ipFeature : NULL );
	}
	else
		return NULL;
}

// 同一20桁住所コードを持つポイントが複数存在しないか確認（Bug9296 追加）
void	CCheckExTPPoint::checkSame20Addrcode( const CExTP& cExTP )
{
	// 自身の調査結果コードが「位置不明」「未調査」であれば、チェック対象外
	if( NOSURVEY == cExTP.m_ePointStatC || UNKNOWN_POS == cExTP.m_ePointStatC )
		return;

	// 同一20桁住所コードを持つもの、自身ではないものの検索
	CString strWhere;
	strWhere.Format( _T("%s='%s' and OBJECTID<>%d"), sindy::schema::extp_point::kAddrCode, cExTP.m_strAddrCode.c_str(), cExTP.getOID() );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipExTPFC->Search( ipQuery, VARIANT_TRUE, &ipCursor );

	if( !ipCursor )
		return;

	IFeaturePtr ipFeature;
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature)
	{
		// 同一20桁住所コードを持つExTPが存在
		CExTP ob(ipFeature);

		// 見つかったExTPの調査結果コードが「位置不明」「未調査」以外はエラー
		if( NOSURVEY != ob.m_ePointStatC && UNKNOWN_POS != ob.m_ePointStatC )
		{
			m_cError.printLog( cExTP, E_SAME_ADDRCODE, ob.getOID() );	// E36
		}
	}
}
