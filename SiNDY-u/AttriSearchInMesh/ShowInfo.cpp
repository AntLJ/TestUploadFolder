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

// ShowInfo.cpp: ShowInfo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShowInfo.h"
#include <io.h>

// 使用法
void CShowInfo::printUsage()
{
	std::cerr << "【メッシュ内属性検索ツール使用法】\n"
			  << "(Usage)AttriSearchInMesh.exe [各種オプション] (> 進捗ログ)\n"
			  << "・オプションについて\n"
			  << "■チェックするメッシュ関連オプション(いずれか必須)\n"
			  << "-l  <メッシュリスト>\n"
			  << "-m  <メッシュコード>\n"
			  << "■ログ出力先オプション(必須)\n"
			  << "-o  <出力ファイル名>\n"
			  << "■その他任意のオプション"
			  << "-i  <出力フィールド設定ファイル> ... -c オプション指定時は無効\n"
			  << "-w  <Where句> ... 指定しなかったときは、「OBJECT IS NOT NULL」です\n"
			  << "-c     : メッシュ内のオブジェクト数を出力モード\n"
			  << "-nodup : カウントするとき、メッシュ境界上のポイントを考慮する(重複カウントされない)\n"
			  << "  ※-nodup オプションは、現在SDE版でかつポイントデータのみ対応しています。\n"
			  << "・環境変数について\n"
			  << "  PGDB_MODE   SiNDY-c用チェックモード ON/OFF(何か設定されていれば、ONと判断する)\n"
			  << "  DB_ANY      DB接続プロパティ(SDE PGDB共用)\n"
			  << "  FC_TARGET   検出対象レイヤ名\n"
			  << "  FC_{CITY|BASE|MIDDLE}MESH   検出対象スケールのメッシュフィーチャクラス名" << std::endl;
}

// 初期化
bool CShowInfo::init( int argc, char* argv[] )
{
	if( argc < 5 )
	{
		std::cerr << "#ERROR 引数の数が足りません(必須オプション -l or -m と -o)" << std::endl;
		return false;
	}

	for( int i = 1; i < argc-1; ++i )
	{
		if( strcmp(argv[i], "-m") == 0 )				// 単メッシュ
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( strcmp(argv[i], "-l") == 0 )			// メッシュリスト
		{
			if( !loadMeshList(argv[++i]) )
			{
				std::cerr << "#ERROR メッシュリストの読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}
		}
		else if( strcmp(argv[i], "-o") == 0 )	// ログファイル
		{
			// 先にファイル存在チェック
			m_bIsAddMode = _access(argv[++i], 0) != -1;
			int nOpenMode = m_bIsAddMode? std::ios::out|std::ios::app : std::ios::out;
			m_fout.open( static_cast<CT2CW>(argv[i]), nOpenMode );
			if( !m_fout )
			{
				std::cerr << "#ERROR ログファイルのオープンに失敗 : " << argv[i] << std::endl;
				return false;
			}
			m_fout.precision(12);
		}
		else if( strcmp(argv[i], "-i") == 0 )	// 出力フィールド設定ファイル
		{
			// 設定ファイルロード
			if( !loadInitFile(argv[++i]) )
			{
				std::cerr << "#ERROR 設定ファイルの読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}
		}
		else if( strcmp(argv[i], "-w") == 0 )
			m_strWhereClause = argv[++i];	// Where句
		else if( strcmp(argv[i], "-c") == 0 )
			m_emLogMode = kCount;			// オブジェクトカウントモード
		else if( strcmp(argv[i], "-nodup") == 0 )
			m_bIsNoDupMode = true;
		else
		{
			std::cerr << "#ERROR 不正なオプションです : " << argv[i] << std::endl;
			return false;
		}
	}

	// 一番最後に-c or -nodupを指定している可能性も考慮(ちょっと強引)
	if( strcmp(argv[argc-1], "-c") == 0 )
		m_emLogMode = kCount;
	else if( strcmp(argv[argc-1], "-nodup") == 0 )
		m_bIsNoDupMode = true;

	if( m_bIsNoDupMode && gf::IsPGDBMode() )
	{
		std::cerr << "#ERROR -nodup オプションは、PGDB版では無効です。" << std::endl;
		return false;
	}

	if( !m_fout.is_open() )
	{
		std::cerr << "#ERROR 出力先を指定してください( -o オプション)" << std::endl;
		return false;
	}

	if( m_vecMesh.empty() )
	{
		std::cerr << "#ERROR チェックするメッシュが設定されていません" << std::endl;
		return false;
	}

	return getEnv();
}

// 実行
bool CShowInfo::execute()
{
	for( std::vector<long>::const_iterator itr = m_vecMesh.begin(); itr != m_vecMesh.end(); ++itr )
	{
		// ＤＢから情報取得
		if( ! setFromDB(*itr) )
		{
			return false;
		}

		// 初回メッシュ時ヘッダー出力
		if( itr == m_vecMesh.begin() )
		{
			if( ! printHeader() )
			{
				std::cerr << "#ERROR ヘッダ情報出力に失敗。メッシュコード : " << *itr << std::endl;
				return false;
			}
		}

		// 指定メッシュのフィーチャを取得
		IFeaturePtr ipMeshFeature;
		if( ! getMeshFeature( *itr, &ipMeshFeature ) )
		{
			std::cerr << "#ERROR メッシュフィーチャ取得に失敗。メッシュコード : " << *itr << std::endl;
			return false;
		}
		if( NULL == ipMeshFeature )
		{
			std::cout << *itr << " : このメッシュコードのメッシュはありません" << std::endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_ShapeCopy( &ipMeshGeo );
		if( NULL == ipMeshGeo )
		{
			std::cerr << "#ERROR 図形の取得に失敗。メッシュコード : " << *itr << std::endl;
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

// メッシュリスト読み込み
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

// 設定ファイル読み込み
bool CShowInfo::loadInitFile( LPCTSTR lpcszFile )
{
	if( kCount == m_emLogMode )
	{
		std::cerr << "オブジェクトカウントモードのため、設定ファイル読み込みは無視します" << std::endl;
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

		// コメントまたは、改行だけの時無視
		if( str.IsEmpty() || str.Find('#') == 0 )
			continue;

		m_vecField.push_back( str.MakeUpper() );	// フィールドベクタにも格納(大文字変換)
	}

	return true;
}

// 環境変数取得
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

	// 接続プロパティ
	if( !m_strDBProp.GetEnvironmentVariable(_T("DB_ANY")) )
	{
		std::cerr << "#ERROR 環境変数 DB_ANY の取得に失敗" << std::endl;
		return false;
	}
	// 検出対象フィーチャクラス名
	if( !m_strTargetFCName.GetEnvironmentVariable(_T("FC_TARGET")) )
	{
		std::cerr << "#ERROR 環境変数 FC_TARGET の取得に失敗" << std::endl;
		return false;
	}
	// メッシュフィーチャクラス名
	CString strEnvName;
	if( m_vecMesh[0] > 1000 && m_vecMesh[0] < 10000 )				strEnvName = _T("FC_MIDDLEMESH");
	else if( m_vecMesh[0] > 100000 && m_vecMesh[0] < 1000000 )		strEnvName = _T("FC_BASEMESH");
	else if( m_vecMesh[0] > 10000000 && m_vecMesh[0] < 100000000 )	strEnvName = _T("FC_CITYMESH");
	else
	{
		std::cerr << "#ERROR メッシュリストの1行目が不正なメッシュです。スケールが判定できません : " << m_vecMesh[0] << std::endl;
		return false;
	}

	if( !m_strMeshFCName.GetEnvironmentVariable(strEnvName) )
	{
		std::cerr << "#ERROR 環境変数" << CT2CA(strEnvName) << " の取得に失敗" << std::endl;
		return false;
	}

	return true;
}

// ＤＢから必要情報取得
bool CShowInfo::setFromDB( long lMeshcode )
{
	if( !gf::IsPGDBMode() )
	{
		if( m_ipWorkspace )		return true;

		if( NULL == (m_ipWorkspace = gf::GetWorkspace(m_strDBProp)) )
		{
			std::cerr << "#ERROR SDE接続に失敗 : " << CT2CA(m_strDBProp) << std::endl;
			return false;
		}

		std::cout << "【SDE接続情報】 : " << CT2CA(m_strDBProp) << std::endl;
	}
	else
	{
		// 先にメッシュコードから、PGDBファイル名を取得
		CString strPGDBFile;
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_strDBProp, lMeshcode/10000, lMeshcode );

		if( NULL == (m_ipWorkspace = gf::GetWorkspace(strPGDBFile)) )
		{
			std::cerr << "#ERROR PGDBファイルのオープンに失敗 : " << CT2CA(strPGDBFile) << std::endl;
			return false;
		}
	}

	IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( CComBSTR(m_strTargetFCName), &m_ipTargetFC );
	if( NULL == m_ipTargetFC )
	{
		std::cerr << "#ERROR ターゲットフィーチャクラスの取得に失敗 : " << CT2CA(m_strTargetFCName) << std::endl;
		return false;
	}
	// ターゲットフィーチャの形状タイプを取得しておく
	if( FAILED(m_ipTargetFC->get_ShapeType( &m_esriTargetGeoType )) )
	{
		std::cerr << "#ERROR ターゲットフィーチャクラスの図形タイプの取得に失敗 : " << CT2CA(m_strTargetFCName) << std::endl;
		return false;
	}

	IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( CComBSTR(m_strMeshFCName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
	{
		std::cerr << "#ERROR メッシュフィーチャクラスの取得に失敗 : " << CT2CA(m_strMeshFCName) << std::endl;
		return false;
	}

	// フィールドIDマップ作成
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
			std::cerr << "#ERROR ターゲットフィーチャクラス内にフィールドが存在しない : " << CT2CA(*itr) << std::endl;
			return false;
		}
	}

	return true;
}

// ヘッダー情報出力
bool CShowInfo::printHeader()
{
	// カウントモードなら出力しない
	if( m_emLogMode != kNormal )
		return true;

	// 追記モードなら出力しないがドメインマップのみ取得
	if( ! m_bIsAddMode ) {

		m_fout << "#SINDYSTDLOG\n"
			   << "#対応結果\tレイヤ名\tOID\tX(経度)\tY(緯度)\tエラーレベル\tエラーコード\tエラーメッセージ\tメッシュコード\t";

		// 設定フィールド空ならコメント出力しておしまい
		if( m_vecField.empty() )
			m_fout << "フリーコメント" << std::endl;
	}

	// 次に設定フィールドのエイリアス名をヘッダーに表示
	IFieldsPtr ipFields;
	//ドメイン名を取得するためにフィールドを一つ取得(何でもいい)
	if( FAILED(m_ipTargetFC->get_Fields( &ipFields )) )
	{
		std::cerr << "#ERROR ターゲットフィーチャクラス内のフィールドの取得に失敗" << std::endl;
		return false;
	}

	long lFieldCount = 0;
	if( FAILED(ipFields->get_FieldCount( &lFieldCount )) )
	{
		std::cerr << "#ERROR ターゲットフィーチャクラス内のフィールド件数の取得に失敗" << std::endl;
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
				std::cerr << "#ERROR ターゲットフィーチャクラス内のフィールドの取得に失敗 : index=" << i << std::endl;
				return false;
			}
			if( FAILED(ipField->get_Name( &bstrField )) )
			{
				std::cerr << "#ERROR ターゲットフィーチャクラス内のフィールド名の取得に失敗 : index=" << i << std::endl;
				return false;
			}
			if( *itr == CString(bstrField).MakeUpper() )	// 大文字にそろえて比較
			{
				BSTR bstrAlias;
				if( FAILED( ipField->get_AliasName( &bstrAlias )) )
				{
					std::cerr << "#ERROR ターゲットフィーチャクラス内のエリアス名の取得に失敗 : index=" << i << std::endl;
					return false;
				}
				// 追記モードなら出力しない
				if( ! m_bIsAddMode ) {
					m_fout << COLE2T(bstrAlias) << "\t";
				}

				// ドメインマップvectorもここで作成してしまう
				IDomainPtr ipDomain;
				if( FAILED(ipField->get_Domain( &ipDomain )) )
				{
					std::cerr << "#ERROR ターゲットフィーチャクラス内のドメインの取得に失敗 : index=" << i << std::endl;
					return false;
				}
				DOMAINMAP map;
				if( ! gf::GetDomainMap(m_ipTargetFC, *itr, &map) )
				{
					std::cerr << "#ERROR ターゲットフィーチャクラス内のドメインマップの取得に失敗 : index=" << i << std::endl;
					return false;
				}
				m_mapDM.insert( std::pair<CString, DOMAINMAP>(*itr, map) );
				::SysFreeString( bstrAlias );
			}
			::SysFreeString( bstrField );
		}
	}
	// 追記モードなら出力しない
	if( ! m_bIsAddMode ) {
		m_fout << "フリーコメント" << std::endl;
	}
	return true;
}

// 標準出力モード処理メイン
bool CShowInfo::execNormalLog( long lMeshcode, IGeometry* ipMeshGeo )
{		
	// 指定メッシュ内にあり尚且つWhere句で指定したものに一致するフィーチャを取得(空間＋属性検索)
	long lFeatureCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipMeshGeo, m_ipTargetFC, ipFeatureCursor, &lFeatureCount,
									  esriSpatialRelIntersects, m_strWhereClause ))
	{
		std::cerr << "#ERROR 空間・属性検索に失敗 : メッシュコード=" << lMeshcode << std::endl;
		return false;
	}
	if( lFeatureCount > 0 )
	{
		// 重複許可しないモード+ポイントならば、その重複対象のオブジェクトＩＤフィーチャを先に取得する
		std::set<long>	setDupID;
		if( m_bIsNoDupMode && m_esriTargetGeoType == esriGeometryPoint )
		{
			if( ! makeDupIDSet( lMeshcode, ipMeshGeo, setDupID ) )
			{
				std::cerr << "#ERROR メッシュ境界上オブジェクトＩＤ一覧作成に失敗 : メッシュコード=" << lMeshcode << std::endl;
				return false;
			}
		}

		// 表示
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			if( setDupID.size() > 0 )
			{
				long lOID;
				if( FAILED(ipFeature->get_OID( &lOID )) )
				{
					std::cerr << "#ERROR OID取得に失敗 : メッシュコード=" << lMeshcode << std::endl;
					return false;
				}
				if( setDupID.find(lOID) != setDupID.end() )
					continue;
			}

			if( ! printData( ipFeature, lMeshcode ) )
			{
				std::cerr << "#ERROR データ出力に失敗 : メッシュコード=" << lMeshcode << std::endl;
				return false;
			}
		}

		std::cout << lMeshcode << " : 出力完了" << std::endl;
	}
	else
		std::cout << lMeshcode << " : 指定した条件にフィーチャはありません" << std::endl;

	return true;
}

// オブジェクトカウント処理
bool CShowInfo::execObjectCount( long lMeshcode, IGeometry* ipMeshGeo )
{
	// 指定メッシュ内にあり尚且つWhere句で指定したものに一致するフィーチャを取得(空間＋属性検索)
	long lFeatureCount =gf::SelectByShapeOnlyCount( ipMeshGeo, m_ipTargetFC, esriSpatialRelIntersects, m_strWhereClause );
	if( lFeatureCount < 0 )
	{
		std::cerr << "#ERROR 空間・属性検索の件数取得に失敗 : メッシュコード=" << lMeshcode << std::endl;
		return false;
	}

	// 重複許可しないモード(ポイントだけ対応した)
	if( m_bIsNoDupMode && m_esriTargetGeoType == esriGeometryPoint )
	{
		int objCount = checkOnMeshObject( lMeshcode, ipMeshGeo );
		if( objCount < 0 )
		{
			std::cerr << "#ERROR メッシュ境界上オブジェクトの件数取得に失敗 : メッシュコード=" << lMeshcode << std::endl;
			return false;
		}
		lFeatureCount -= objCount;
	}
	m_fout << lMeshcode << "\t" << lFeatureCount << std::endl;	// メッシュ内オブジェクト数表示

	std::cout << lMeshcode << " : 出力完了" << std::endl;
	return true;
}

// メッシュ境界上オブジェクトＩＤ一覧作成(標準ログモード時に使う)
bool CShowInfo::makeDupIDSet( long lMeshcode, IGeometry* ipBaseMeshGeo, std::set<LONG>& setID )
{
	ITopologicalOperatorPtr ipTopoOpe( ipBaseMeshGeo );
	if( ! ipTopoOpe )
	{
		std::cerr << "#ERROR TopologicalOperator生成に失敗" << std::endl;
		return false;
	}

	IGeometryPtr ipBound;
	ipTopoOpe->get_Boundary( &ipBound );
	if( ! ipBound )
	{
		std::cerr << "#ERROR 境界取得に失敗" << std::endl;
		return false;
	}

	long lOnMeshCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipBound, m_ipTargetFC, ipFeatureCursor, &lOnMeshCount,
									  esriSpatialRelIntersects, m_strWhereClause ) )
	{
		std::cerr << "#ERROR 空間・属性検索に失敗" << std::endl;
		return false;
	}
	if( lOnMeshCount > 0 )
	{
		IPointCollectionPtr ipMeshPointCol( ipBaseMeshGeo );
		IPointPtr ipMeshLUPoint;
		// メッシュ左下座標取得
		if( FAILED(ipMeshPointCol->get_Point( 0, &ipMeshLUPoint )) )
		{
			std::cerr << "#ERROR ポイントコレクション取得に失敗" << std::endl;
			return false;
		}
		double dLUX, dLUY;
		if( FAILED(ipMeshLUPoint->QueryCoords( &dLUX, &dLUY )) )
		{
			std::cerr << "#ERROR ポイントコレクションのX,Y座標取得に失敗" << std::endl;
			return false;
		}

		int dDupCount = 0;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			// オブジェクトＩＤ取得
			long lOID;
			if( FAILED(ipFeature->get_OID( &lOID )) )
			{
				std::cerr << "#ERROR OID取得に失敗" << std::endl;
				return false;
			}

			// 取得したフィーチャのポイントがメッシュの下or左側なら対象メッシュ内ではカウントしない
			IGeometryPtr ipGeo;
			if( FAILED(ipFeature->get_ShapeCopy( &ipGeo )) )
			{
				std::cerr << "#ERROR 図形取得に失敗" << std::endl;
				return false;
			}
			IPointPtr ipPoint(ipGeo);
			if( NULL == ipPoint )
			{
				std::cerr << "#ERROR 図形取得(ポイント)に失敗" << std::endl;
				return false;
			}

			double dX, dY;
			if( FAILED(ipPoint->QueryCoords( &dX, &dY )) )
			{
				std::cerr << "#ERROR ポイントのX,Y座標取得に失敗" << std::endl;
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

// メッシュ境界上オブジェクトチェック(オブジェクトカウント時使用)
int CShowInfo::checkOnMeshObject( long lMeshcode, IGeometry* ipBaseMeshGeo )
{
	ITopologicalOperatorPtr ipTopoOpe( ipBaseMeshGeo );
	if( ! ipTopoOpe )
	{
		std::cerr << "#ERROR TopologicalOperator生成に失敗" << std::endl;
		return -1;
	}

	IGeometryPtr ipBound;
	ipTopoOpe->get_Boundary( &ipBound );
	if( ! ipBound )
	{
		std::cerr << "#ERROR 境界取得に失敗" << std::endl;
		return -1;
	}

	long lOnMeshCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( ! gf::SelectByShapeWithCount( ipBound, m_ipTargetFC, ipFeatureCursor, &lOnMeshCount,
									  esriSpatialRelIntersects, m_strWhereClause ) )
	{
		std::cerr << "#ERROR 空間・属性検索に失敗" << std::endl;
		return -1;
	}
	if( lOnMeshCount > 0 )
	{
		IPointCollectionPtr ipMeshPointCol( ipBaseMeshGeo );
		IPointPtr ipMeshLUPoint;
		// メッシュ左下座標取得
		if( FAILED(ipMeshPointCol->get_Point( 0, &ipMeshLUPoint )) )
		{
			std::cerr << "#ERROR ポイントコレクション取得に失敗" << std::endl;
			return -1;
		}
		double dLUX, dLUY;
		if( FAILED(ipMeshLUPoint->QueryCoords( &dLUX, &dLUY )) )
		{
			std::cerr << "#ERROR ポイントコレクションのX,Y座標取得に失敗" << std::endl;
			return -1;
		}

		int nDupCount = 0;
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			// 取得したフィーチャのポイントがメッシュの下or左側なら対象メッシュ内ではカウントしない
			IGeometryPtr ipGeo;
			if( FAILED(ipFeature->get_ShapeCopy( &ipGeo )) )
			{
				std::cerr << "#ERROR 図形取得に失敗" << std::endl;
				return -1;
			}
			IPointPtr ipPoint(ipGeo);
			if( NULL == ipPoint )
			{
				std::cerr << "#ERROR 図形取得(ポイント)に失敗" << std::endl;
				return -1;
			}

			double dX, dY;
			if( FAILED(ipPoint->QueryCoords( &dX, &dY )) )
			{
				std::cerr << "#ERROR ポイントのX,Y座標取得に失敗" << std::endl;
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

// メッシュフィーチャ取得
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

// ポイント情報表示
bool CShowInfo::getDispPoint( IFeature* ipFeature, WKSPoint& point )
{
	IGeometryPtr ipGeom;
	if( FAILED(ipFeature->get_ShapeCopy( &ipGeom )) )
		return false;

	IPointPtr ipPoint;
		
	// 形状ごとに表示ポイント取得
	esriGeometryType esriGeoType;
	if( FAILED(ipGeom->get_GeometryType( &esriGeoType )) )
		return false;

	if( esriGeoType == esriGeometryPoint )
	{
		ipPoint = ipGeom;
	}
	else if( esriGeoType == esriGeometryPolygon )	// ポリゴンなら、重心表示
	{
		IAreaPtr ipArea(ipGeom);
		if( FAILED(ipArea->get_Centroid( &ipPoint )) )	// [Bug5934] LabelPoint→Centroidに変更
			return false;
	}
	else if( esriGeoType == esriGeometryPolyline )	// ラインなら、先頭の点を表示
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

// データ表示
bool CShowInfo::printData( IFeature* ipFeature, long lMeshcode )
{
	// OID取得
	long lOID = 0;
	if( FAILED(ipFeature->get_OID( &lOID )) )
	{
		std::cerr << "#ERROR OID取得に失敗" << std::endl;
		return false;
	}

	// 表示ポイント取得
	WKSPoint point;
	if( ! getDispPoint( ipFeature, point ))
	{
		std::cerr << "#ERROR 表示ポイント情報取得に失敗" << std::endl;
		return false;
	}

	m_fout << 0 << "\t" << m_strTargetFCName << "\t" << lOID << "\t" << point.X << "\t" << point.Y << "\t"	// 対応状況・レイヤ名・OID・経度・緯度
		   << "\t\t\t" << lMeshcode << "\t";	// エラーレベル・エラーコード・エラーメッセージ・メッシュコード

	for( std::vector<CString>::const_iterator itrF = m_vecField.begin(); itrF != m_vecField.end(); ++itrF )
	{
		CComVariant vaValue;
		FIELDMAP::const_iterator itr = m_mapFieldID.find( *itrF );
		if( itr != m_mapFieldID.end() )
			if( FAILED(ipFeature->get_Value( itr->second, &vaValue )) )
			{
				std::cerr << "#ERROR 値の取得に失敗" << std::endl;
				return false;
			}
						
		// ドメインあればドメインを表示・なければフィールド値を表示
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

	m_fout << std::endl;	//フリーコメント

	return true;
}

// CComVariantからstring型に変換
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
