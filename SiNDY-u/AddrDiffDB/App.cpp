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

// ワークスペース取得
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

// 使用法
void CApp::printUsage()
{
	std::cerr << "【20桁住所コード差分抽出ツール】\n"
			  << "(Usage)AddrDiffDB.exe [各種オプション]\n"
	  		  << "・オプションについて\n"
			  << "■チェックするメッシュ関連オプション(いずれか必須)\n"
			  << "  -m  <メッシュコード>\n"
 			  << "  -l  <メッシュリスト>\n"
			  << "  -lg <メッシュリスト> ... メッシュリスト内のメッシュを１つのグループとして差分を取るモード\n"
			  << "■ログ出力先オプション(必須)\n"
			  << "  -o  <出力ファイル名>\n"
			  << "■無視する住所11桁コードリスト(任意)\n"
			  << "  -n  <11桁住所コードリスト>\n"
			  << "・環境変数について\n"
			  << "  PGDB_MODE		     SiNDY-c用チェックモード ON/OFF(何か設定されていれば、ONと判断する)\n"
			  << "  DB_ADDRESS_NEW     ＤＢ接続プロパティ(新) SDE or PGDB\n"
			  << "  FC_CITY_ADMIN_NEW  都市地図行政界フィーチャクラス名(新)\n"
			  << "  FC_GOU_POINT_NEW   号ポイントフィーチャクラス名(新)\n"
			  << "  DB_ADDRESS_OLD     ＤＢ接続プロパティ(旧) SDE or PGDB\n"
			  << "  FC_CITY_ADMIN_OLD  都市地図行政界フィーチャクラス名(旧)\n"
			  << "  FC_GOU_POINT_OLD   号ポイントフィーチャクラス名(旧)\n"	
			  << "  FC_CITYMESH        都市地図メッシュフィーチャクラス名\n"
			  << std::endl;
}

// 初期化
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( argc < 5 ){
		std::cerr << "#ERROR 引数の数が正しくありません" << std::endl;
		return false;
	}

	// オプション解析
	for( int i=1;i<argc-1;i++ )
	{
		if( _tcscmp(argv[i], _T("-m")) == 0 )		// 単メッシュ
			m_vecMesh.push_back( atol(argv[++i]) );
		else if( _tcscmp(argv[i], _T("-l")) == 0 || _tcscmp(argv[i], _T("-lg")) == 0 )	// メッシュリスト
		{
			if( _tcscmp(argv[i], _T("-lg")) == 0 )
				m_bGroupMeshMode = true;

			if( !loadMeshList(argv[++i]) )
			{
				std::cerr << "#ERROR メッシュリストの読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}
		}		
		else if( _tcscmp(argv[i], _T("-o")) == 0 ){	// ログファイル

			// 先にファイル存在チェック
			m_bIsAddMode = (_access(argv[++i], 0) != -1)? true : false;
			int nOpenMode = m_bIsAddMode? ios::out|ios::app : ios::out;

			m_fout.open( static_cast<CT2CW>(argv[i]), nOpenMode );
			if( !m_fout ){
				std::cerr << "#ERROR ログファイルのオープンに失敗 : " << argv[i] << std::endl;
				return false;
			}
			m_fout.precision(12);
		}
		else if( _tcscmp(argv[i], _T("-n")) == 0 ){	// OKコードリスト

			if( !loadOKAddrCodeList(argv[++i]) ){
				std::cerr << "#ERROR OKコードリストの読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}
		}
		else{
			std::cerr << "#ERROR 不正なオプション : " << argv[i] << std::endl;
			return false;
		}
	}

	if( !m_fout.is_open() ){
		std::cerr << "#ERROR 出力先を指定してください( -o オプション)" << std::endl;
		return false;
	}

	return getEnv();	// 環境変数取得
}

// 実行
bool CApp::execute()
{
	// ログヘッダー出力
	printHeader();

	CODEMAP NewCodeMap, OldCodeMap;
	for( m_itrMesh=m_vecMesh.begin();m_itrMesh!=m_vecMesh.end();++m_itrMesh )
	{
		// ＤＢから情報取得
		if( !setFromDB(*m_itrMesh) ){
			std::cerr << *m_itrMesh << " : DBから必要な情報が取得できませんでした" << std::endl;
			std::cout << *m_itrMesh << " : DBから必要な情報が取得できませんでした" << std::endl;
			continue;
		}

		// メッシュジオメトリ取得
		IFeaturePtr ipMeshFeature = getMeshFeature( *m_itrMesh );
		if( NULL == ipMeshFeature ){
			std::cerr << "#ERROR メッシュフィーチャの取得に失敗 : " << *m_itrMesh << std::endl;
			std::cout << "#ERROR メッシュフィーチャの取得に失敗 : " << *m_itrMesh << std::endl;
			continue;
		}
		IGeometryPtr ipMeshGeo;
		ipMeshFeature->get_Shape( &ipMeshGeo );

		makeCodeList( m_ipNewCAFC, m_ipNewGouFC, ipMeshGeo, *m_itrMesh, m_ipNewWorkspace, NewCodeMap );
		makeCodeList( m_ipOldCAFC, m_ipOldGouFC, ipMeshGeo, *m_itrMesh, m_ipOldWorkspace, OldCodeMap );

		// 1メッシュごとに差分取る場合は、ここで抽出
		if( !m_bGroupMeshMode )
		{
			findDeleteCode( NewCodeMap, OldCodeMap );	// 削除住所コード検出
			NewCodeMap.clear();
			OldCodeMap.clear();
			std::cout << *m_itrMesh << " : チェック完了" << std::endl;
		}
		else
			std::cout << *m_itrMesh << " : 住所コード抽出完了" << std::endl;
	}

	// メッシュリストをグループ化して差分取る場合、ここで抽出
	if( m_bGroupMeshMode )
	{
		std::cout << "差分抽出を開始します ... ";
		findDeleteCode( NewCodeMap, OldCodeMap );
		std::cout << "完了" << std::endl;
	}

	return true;
}

/////////////////////////////////
//	内部関数
/////////////////////////////////

// 環境変数
bool CApp::getEnv()
{
	CString strError;

	if( NULL == (m_lpszNewDBProp = _tgetenv(_T("DB_ADDRESS_NEW"))) )
		strError += _T("#ERROR 環境変数 DB_ADDRESS_NEW が設定されていない\n");
	if( NULL == (m_lpszOldDBProp = _tgetenv(_T("DB_ADDRESS_OLD"))) )
		strError += _T("#ERROR 環境変数 DB_ADDRESS_OLD が設定されていない\n");
	if( NULL == (m_lpszNewCAFName = _tgetenv(_T("FC_CITY_ADMIN_NEW"))) )
		strError += _T("#ERROR 環境変数 FC_CITY_ADMIN_NEW が設定されていない\n");
	if( NULL == (m_lpszOldCAFName = _tgetenv(_T("FC_CITY_ADMIN_OLD"))) )
		strError += _T("#ERROR 環境変数 FC_CITY_ADMIN_OLD が設定されていない\n");
	if( NULL == (m_lpszNewGouFName = _tgetenv(_T("FC_GOU_POINT_NEW"))) )
		strError += _T("#ERROR 環境変数 FC_GOU_POINT_NEW が設定されていない\n");
	if( NULL == (m_lpszOldGouFName = _tgetenv(_T("FC_GOU_POINT_OLD"))) )
		strError += _T("#ERROR 環境変数 FC_GOU_POINT_OLD が設定されていない\n");
	if( NULL == (m_lpszMeshFName = _tgetenv(_T("FC_CITYMESH"))) )
		strError += _T("#ERROR 環境変数 FC_CITYMESH が設定されていない\n");

	if( !strError.IsEmpty() ){
		std::cerr << strError << std::endl;
		return false;
	}
	else
		return true;
}

// メッシュリスト読み込み
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

// OKコードリスト
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

// ＤＢから必要情報取得
bool CApp::setFromDB( LONG lMeshcode )
{
	if( !isPGDBMode() )
	{
		if( m_ipNewWorkspace && m_ipOldWorkspace )	
			return true;

		if( NULL == (m_ipNewWorkspace = getWorkspace(m_lpszNewDBProp)) )
		{
			cerr << "#ERROR 接続に失敗(新) : " << m_lpszNewDBProp << endl;
			return false;
		}
		if( NULL == (m_ipOldWorkspace = getWorkspace(m_lpszOldDBProp)) )
		{
			cerr << "#ERROR 接続に失敗(旧) : " << m_lpszOldDBProp << endl;
			return false;
		}

		cout << "【SDE接続情報(新)】 : " << m_lpszNewDBProp << endl;
		cout << "【SDE接続情報(旧)】 : " << m_lpszOldDBProp << endl;
	}
	else
	{
		// 先にメッシュコードから、PGDBファイル名を取得
		CString strPGDBFile;
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_lpszNewDBProp, lMeshcode/10000, lMeshcode );
		if( NULL == (m_ipNewWorkspace = getWorkspace(strPGDBFile)) )
		{
			cerr << "#ERROR PGDBファイル(新)のオープンに失敗 : " << strPGDBFile << endl;
			return false;
		}
		strPGDBFile.Format( _T("%s\\%d\\%d.mdb"), m_lpszOldDBProp, lMeshcode/10000, lMeshcode );
		if( NULL == (m_ipOldWorkspace = getWorkspace(strPGDBFile)) )
		{
			cerr << "#ERROR PGDBファイル(旧)のオープンに失敗 : " << strPGDBFile << endl;
			return false;
		}
	}

	CString strError;

	// 行政界・号ポイント
	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszNewCAFName), &m_ipNewCAFC );
	if( NULL == m_ipNewCAFC )
		strError.AppendFormat( _T("#ERROR 都市地図行政界(新)フィーチャクラスの取得に失敗 : %s\n"), m_lpszNewCAFName );

	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszNewGouFName), &m_ipNewGouFC );
	if( NULL == m_ipNewGouFC )
		strError.AppendFormat( _T("#ERROR 号ポイント(新)フィーチャクラスの取得に失敗 : %s\n"), m_lpszNewGouFName );

	((IFeatureWorkspacePtr)m_ipOldWorkspace)->OpenFeatureClass( CComBSTR(m_lpszOldCAFName), &m_ipOldCAFC );
	if( NULL == m_ipOldCAFC )
		strError.AppendFormat( _T("#ERROR 都市地図行政界(旧)フィーチャクラスの取得に失敗 : %s\n"), m_lpszOldCAFName );

	((IFeatureWorkspacePtr)m_ipOldWorkspace)->OpenFeatureClass( CComBSTR(m_lpszOldGouFName), &m_ipOldGouFC );
	if( NULL == m_ipOldGouFC )
		strError.AppendFormat( _T("#ERROR 号ポイント(旧)フィーチャクラスの取得に失敗 : %s\n"), m_lpszOldGouFName );

	// メッシュフィーチャクラス取得
	((IFeatureWorkspacePtr)m_ipNewWorkspace)->OpenFeatureClass( CComBSTR(m_lpszMeshFName), &m_ipMeshFC );
	if( NULL == m_ipMeshFC )
		strError.AppendFormat( _T("#ERROR メッシュフィーチャクラスの取得に失敗 : %s\n"), m_lpszMeshFName );

	if( !strError.IsEmpty() ){
		std::cerr << strError << std::endl;
		return false;
	}
	else{
		// フィールドインデックス取得
		m_cCityAdminFID.set(m_ipNewCAFC);
		m_cGouPointFID.set(m_ipNewGouFC);
		return true;
	}

}

// メッシュフィーチャ取得
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

// 20桁住所コードリスト作成
void CApp::makeCodeList( IFeatureClass* ipAdminFC, IFeatureClass* ipGouFC, IGeometry* ipMeshGeo, long lMesh, IWorkspace* ipWorkspace, CODEMAP& codeMap )
{
	// ToDo:要詳細調査
	// PGDBが対象の場合に空間キャッシュを使うとWorkspaceが開放されなくなり、大量に開いた場合に
	// 例外が発生するっぽいのでSDEモード限定にする（bug 9304, 9127）
	ISpatialCacheManagerPtr ipCache(ipWorkspace);
	if( !isPGDBMode() )
	{
		IEnvelopePtr ipEnv;
		ipMeshGeo->get_Envelope( &ipEnv );
		if( FAILED(ipCache->FillCache(ipEnv)) )
		{
			std::cerr << "#ERROR キャッシュの構築に失敗 : " << lMesh << std::endl;
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

			// [Bug 6426]街区符号がNullかつ整備済みなら無視
			if( cCityAdmin.m_lGaikuFugo == 0 
				&& (cCityAdmin.m_lAddrClass == city_admin::addr_class::kChibanMaintenance || cCityAdmin.m_lAddrClass == city_admin::addr_class::kResidenceIndicationMaintenance) )
				continue;

			IGeometryPtr ipAdminGeo( cCityAdmin.getGeo() );

			// SDEモードならば、メッシュでクリップしてから
			if( !isPGDBMode() )
			{
				ITopologicalOperatorPtr ipTopoOpe( ipAdminGeo );
				IEnvelopePtr ipEnv;
				ipMeshGeo->get_Envelope( &ipEnv );
				ipTopoOpe->Clip( ipEnv );
			}

			// 行政界内の号ポイントを取得
			IFeatureCursorPtr ipGouCursor;
			SelectByShapeWithCount( ipAdminGeo, ipGouFC, &ipGouCursor,  esriSpatialRelIntersects, !isPGDBMode()? strWhereGou : NULL, strSubFields );
			if( ipGouCursor )
			{				
				IFeaturePtr ipGouFeature;
				while( ipGouCursor->NextFeature(&ipGouFeature) == S_OK )
				{
					CGouPoint cGouPoint(ipGouFeature, m_cGouPointFID);
					cGouPoint.m_lMeshCode = *m_itrMesh;					// グループ化モード時必要なので、常に設定することにする
					CString strAddrCode = getAddrCode( cCityAdmin, cGouPoint );	// 行政界・号ポイントから20桁住所コード生成
					if( 20 == strAddrCode.GetLength() )
					{
						codeMap.insert( std::pair<CString, CGouPoint>(strAddrCode, cGouPoint) );
					}
				}
			}
		}
	}
}

// 20桁住所コード生成
CString CApp::getAddrCode( const CCityAdmin& cCityAdmin, const CGouPoint& cGouPoint )
{
	CString strAddrCode(_T(""));

	// 拡張号番号フラグチェックし、行政界からどのレベルまで取得するか決める
	if( 1 == cGouPoint.m_lExGouNoF )
		strAddrCode = cCityAdmin.getAddrCode(11);	/// 街区は、号ポイントから取得
	else{
		if( 0 == cCityAdmin.m_lGaikuFugo )
			strAddrCode = cCityAdmin.getAddrCode(11);
		else
			strAddrCode = cCityAdmin.getAddrCode(16);
	}

	// 号番号から、街区以下のコードを作成
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

	if( -1 != nHaihunSPos && -1 == nHaihunEPos ){	// 「-」があってかつ1つだけのとき
		
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
	else if( nHaihunSPos != nHaihunEPos ){	// 「-」が2つ以上あるとき

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
	else{	// 「-」がないとき

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

// 削除コード検索
void CApp::findDeleteCode( const CODEMAP& NewCodeList, const CODEMAP& OldCodeList )
{
	// 旧コードリストを元に削除されているか見つける
	for( CODEMAP::const_iterator itrM=OldCodeList.begin();itrM!=OldCodeList.end();++itrM )
	{
		// OK11桁住所コードリストならばチェックしない
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
			 * 枝番なし号ポイントが枝番つき号ポイントへ変更されているものは削除としない
			 * Ex) 号ポイント「101」→「101-2」へ変更
			 */
			bool bFindBranch = false;
			if( itrM->first.Mid(16) == _T("0000") )	/// 号番号が[0000]のパターン
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
				printDeleteGou( itrM->second, itrM->first );	// 削除された住所コード(号ポイント)
		}
	}
}

// 号番号から、不要な文字除去
void CApp::filterGouNo( CString& strGou )
{
	CString strTmp(_T(""));

	int nStrLen = strGou.GetLength();

	// 数字と「-」以外は、無視
	for( int i=0;i<nStrLen;i++ )
	{
		if( _istdigit(strGou[i]) || '-' == strGou[i] )
			strTmp += strGou[i];
	}

	// 語尾が「-」になっていたら、それ削除「#454-イ」とかのとき考えられる
	if( strTmp.Find(_T("-")) == strTmp.GetLength()-1 )
		strTmp = strTmp.Left(strTmp.GetLength()-1 );

	strGou = strTmp;
}

// 削除されたポイント出力
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

	m_fout << "\t\t号ポイントが削除されました\t" << strAddrCode << "\t" << cGouPoint.m_strGouNo << "\t" << cGouPoint.m_lMeshCode << std::endl;
}

////////////////////////////////////////
//	汎用関数
////////////////////////////////////////

// 空間・属性検索(件数も取得版)
LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
{
	/// クエリフィルタ作成
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;	
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeo );
	ipFilter->put_SpatialRel( spatialRel );
	
	// Where句指定されていたら、セットする
	if( lpszWhereClause )
	{
		ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
		ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
	}

	// SubFields指定されていたら、セットする
	if( lpszSubFields )
		ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

	/// 検索結果が 0 ならリターン
	long lHitCount = 0;
	if( isPGDBMode() )
	{
		// PGDBを空間キャッシュ後に検索するとFeatureCountが遅い問題の回避コード
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

	/// 検索
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
	if( NULL == *ppFeatureCursor )
		return 0;
			
	return lHitCount;
}
