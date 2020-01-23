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
	// ワークスペース取得
	inline IWorkspacePtr GetWorkspace( const CString& dbConnectString, const CString& message )
	{
		INamePtr ipName( sindy::create_workspace_name(dbConnectString) );
		if( !ipName )
		{
			wcerr << L"#Error " << CT2CW(message) << L"へのSDE接続に失敗 : " << CT2CW(dbConnectString) << endl;
			return nullptr;
		}

		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		if( !ipUnknown )
		{
			wcerr << L"#Error " << CT2CW(message) << L"へのSDE接続に失敗 : " << CT2CW(dbConnectString) << endl;
		}
		else
		{
			wcerr << L"#" << CT2CW(message) << L"SDE接続 : " << CT2CW(dbConnectString) << endl;
		}
		return ipUnknown;
	}

	// フィーチャクラス取得
	inline IFeatureClassPtr GetFeatureClass( const IWorkspacePtr& ipWorkspace, const CString& featureClassName )
	{
		IFeatureClassPtr ipFeatureClass;
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(featureClassName), &ipFeatureClass );
		if( !ipFeatureClass )
		{
			wcerr << L"#Error フィーチャクラスの取得に失敗 : " << CT2CW(featureClassName) << endl;
		}
		return ipFeatureClass;
	}

	// (ポイント)フィーチャからMESHXYを取得
	inline void GetMeshXY( const IFeaturePtr& feature, int& mesh, int& x, int& y )
	{
		IGeometryPtr ipGeo;
		feature->get_Shape( &ipGeo ); // 本当はポイントかどうかチェックしたほうが良い
		WKSPoint wksPoint;
		((IPointPtr)ipGeo)->QueryCoords( &wksPoint.X, &wksPoint.Y );
		g_cCrdCnv.LLtoMesh( wksPoint.X, wksPoint.Y, 2, &mesh, &x, &y, 1 );
	}

	// 現在の時間
	inline CString GetCurTime()
	{
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		return strTime;
	}
}

// 初期化
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	CArguments arg;
	if( !arg.parse( argc, argv ) )
	{
		return false;
	}

	// SDE接続
	IWorkspacePtr ipPOIWorkspace( gf::GetWorkspace(arg.m_poiDB.c_str(), _T("POIデータ")) );
	if( ! ipPOIWorkspace )
	{
		return false;
	}

	IWorkspacePtr ipAddrWorkspace( gf::GetWorkspace(arg.m_addrDB.c_str(), _T("住所データ")) );
	if( ! ipAddrWorkspace )
	{
		return false;
	}

	IWorkspacePtr ipMapWorkspace( gf::GetWorkspace(arg.m_mapDB.c_str(), _T("地図データ")) );
	if( ! ipMapWorkspace )
	{
		return false;
	}

	// POI_POINT取得
	m_ipPOIPointFC = gf::GetFeatureClass( ipPOIWorkspace, arg.m_poiPoint.c_str() );
	if( ! m_ipPOIPointFC )
	{
		return false;
	}
	
	// POI_SUB_POINT取得
	m_ipPOISubPointFC = gf::GetFeatureClass( ipPOIWorkspace, arg.m_poiSubPoint.c_str() );
	if( ! m_ipPOISubPointFC )
	{
		return false;
	}

	// CITY_ADMIN取得
	m_ipCityAdminFC = gf::GetFeatureClass( ipAddrWorkspace, arg.m_cityAdmin.c_str() );
	if( ! m_ipCityAdminFC )
	{
		return false;
	}

	// BUILDING取得
	m_ipBuildingFC = gf::GetFeatureClass( ipMapWorkspace, arg.m_building.c_str() );
	if( ! m_ipBuildingFC )
	{
		return false;
	}

	// フィールドインデックス取得
	if( FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kContentsCode), &m_lContentsCode_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kContentsSeq), &m_lSeq_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kName), &m_lName_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kTel), &m_lTel_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kACCCode), &m_lAcc_FID ))
		|| 	FAILED(m_ipPOIPointFC->FindField( _bstr_t(sindyk::poi_point_org::kDelete), &m_lDelete_FID )) )
	{
		wcerr << L"#Error POI_POINTフィールドインデックスの取得に失敗" << endl;
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
		wcerr << L"#Error POI_SUB_POINTフィールドインデックスの取得に失敗" << endl;
		return false;
	}

	if( FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kCityCode), &m_lCityCode_FID ))
		|| 	FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kAddrCode), &m_lAddrCode_FID ))
		|| 	FAILED(m_ipCityAdminFC->FindField( _bstr_t(city_admin::kAddrCode2), &m_lAddrCode2_FID )) )
	{
		wcerr << L"#Error CITY_ADMINフィールドインデックスの取得に失敗" << endl;
		return false;
	}

	// CONTENTS_MASTER テーブル名取得
	m_cDBCache.m_strContentsMaster = arg.m_contents.c_str();

	// 出力対象種別表リスト読み込み
	if( !m_cListCache.Create(arg.m_listPath.c_str()) )
	{
		wcerr << L"#Error 出力対象種別表リストの読み込み失敗 : " << arg.m_listPath << endl;
		return false;
	}
	wcerr << L"#出力対象種別表リスト読み込み成功 : " << arg.m_listPath << endl;

	// 出力ファイルオープン
	m_outPOI.Open( arg.m_outFile );
	if( !m_outPOI.IsOpen() )
	{
		wcerr << L"#Error 出力ファイルのオープンに失敗 : " << arg.m_outFile << endl;
		return false;
	}
	m_outPOISub.Open( arg.m_outFileSub );
	if( !m_outPOISub.IsOpen() )
	{
		wcerr << L"#Error 出力ファイルのオープンに失敗 : " << arg.m_outFileSub << endl;
		return false;
	}

	// マスタ情報系キャッシュ
	if( !m_cDBCache.Create(ipPOIWorkspace) )
	{
		wcerr << L"#Error マスタ情報キャッシュに失敗" << endl;
		return false;
	}
	wcerr << L"#マスタ情報キャッシュ作成完了" << endl;

	// 処理対象IDNCODE
	m_idnCode = arg.m_idnCode.c_str();
	const auto& idncodes = uh::str_util::split( m_idnCode, _T(",") );
	for( const auto& idncode : idncodes )
	{
		m_idnCodeCounter[_ttol(idncode)] = 0;
	}

	// ログヘッダー
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
					  _T("#開始時間 %s"),
					  cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), gf::GetCurTime() );
	wcout << CT2CW(strHeader) << endl;

	return true;
}

// 実行
void CApp::Run()
{
	// まずPOI_SUB_POINTを取得(IDNCODE、DELETE_Cで絞る)
	if( !CollectPoiSubPointInfo() )
		return;

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	long lCount = 1;

	// ORDER BY句追加（検索順に依存しないようにするため）[bug12624]
	CString strPostfix;
	strPostfix.Format(_T("ORDER BY %s, %s"), sindyk::poi_point::kContentsCode, sindyk::poi_point::kContentsSeq); // CONTENTS_CODE, CONTENTS_SEQの昇順

	// Contents_Masterから、Contents_Type<0,1>のContents_codeを使って、POI_POINTから該当Contents_codeのものだけを取得（Bug9215対応）
	vector<wstring>	vecWstrWhere = m_cDBCache.CreateWhereVecFromContentCode( sindyk::poi_point::kContentsCode );

	// vectorで返ってくるため、中身の数分実行
	for( const auto& whereClause : vecWstrWhere )
	{
		// Contents_code in (hoge, hoge2, ・・・)なWhere句
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
				wcout << L"#Fatal Error マスタ情報がありません : " << contentsCode.lVal << endl;
				++lCount;
				continue;
			}

			const POIUSABLEINFO* pUsableInfo = m_cListCache.GetPOIMasterInfo(pDBInfo->first, pDBInfo->second);
			if( !pUsableInfo )
			{
				wcout << L"#Warning マスタと一致する情報がリストにありません(情報が古いか確認) : " << pDBInfo->first << L"," << pDBInfo->second << endl;
				++lCount;
				continue;
			}

			// 重要コンテンツフラグ採用ならば、無条件にレコード格納
			// 位置フラグ or PPフラグのどちらかが採用なら、こちらもレコード格納（精度['1S', '3A', '1A', 'UK']かどうかはSetPOI内で判定して落とす）
			if( pUsableInfo->bImportantF || pUsableInfo->bQualityF || pUsableInfo->bPPRateF )
			{
				SetPOI( ipPOIFeature, pDBInfo, pUsableInfo );
			}
			++lCount;
		}
		wcerr << lCount << L" 件読み込み完了\r";
	}

	// データ出力
	wcerr << L"#データ出力開始 ... ";
	m_outPOI.POIPointOutput(m_setImpPOI);
	m_outPOI.POIPointOutput(m_setPOI);
	m_outPOISub.POISubPointOutput(m_setPOISub);
	wcerr << L"完了" << endl;

	// IDNCODEごとの出力件数チェック
	for( const auto& errorIDNCodeCount : m_idnCodeCounter )
	{
		if( 0 == errorIDNCodeCount.second )
		{
			wcout << L"#Warning IDNCODE : " << errorIDNCodeCount.first << L" の出力件数が0件です" << endl;
		}
	}

	// ログフッター
	wcout << L"#終了時間 " << (const wchar_t*)gf::GetCurTime() << endl;
}

// ---------------------------------- Private ------------------------------------

// レコード格納
void CApp::SetPOI( IFeaturePtr ipFeature, const POIMASTERDBINFO* pMaster, const POIUSABLEINFO* pUsable  )
{
	POIPointRecord cPoi;

	ipFeature->get_OID( &cPoi.m_lOID );		// OID
	cPoi.m_lGrpCode = pMaster->first;		// 分類コード
	cPoi.m_lChainCode = pMaster->second;	// チェーンコード

	CComVariant vaValue;

	// 精度コード
	ipFeature->get_Value( m_lAcc_FID, &vaValue );
	cPoi.m_strAccCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// シーケンス(文字列)
	ipFeature->get_Value( m_lSeq_FID, &vaValue );
	cPoi.m_strSeq = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// 名称
	ipFeature->get_Value( m_lName_FID, &vaValue );
	cPoi.m_strName = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	vaValue.Clear();

	// 電話番号(ハイフン除外する)
	ipFeature->get_Value( m_lTel_FID, &vaValue );
	cPoi.m_strTel = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
	cPoi.m_strTel.Replace( _T("-"), _T("") );
	vaValue.Clear();

	// 削除コード
	ipFeature->get_Value( m_lDelete_FID, &vaValue );
	cPoi.m_deleteCode = vaValue.lVal;
	vaValue.Clear();

	// Bug9215対応：重要コンテンツ以外は、ここで精度で落とすように変更
	// ここで出力対象かチェック(TELがNULLでないか・精度向上利用対象か・重要コンテンツでなければ精度は['1S', '3A', '1A', 'UK']か)
	if( cPoi.m_strTel.IsEmpty() )
		return;

	if( pUsable->bImportantF || 
		( (pUsable->bQualityF || pUsable->bPPRateF) && cPoi.GetPriorValueFromAcc() < 4 ) )
	{
		// プロット位置の行政界から情報取得
		IGeometryPtr ipGeo;
		ipFeature->get_Shape( &ipGeo );

		CODEINFO cCodeInfo;
		if( !GetCityAdminInfo(ipGeo, cCodeInfo) )
		{
			wcout << L"#Error 行政界情報取得できない : " << pMaster->first << L"-" << pMaster->second << L"-" << (const wchar_t*)cPoi.m_strSeq << endl;
			return;
		}

		cPoi.m_strAddrCode1 = cCodeInfo.first;	// 住所コード1
		cPoi.m_strAddrCode2 = cCodeInfo.second;	// 住所コード2

		// ２次メッシュXY取得
		gf::GetMeshXY( ipFeature, cPoi.m_nMeshCode, cPoi.m_nX, cPoi.m_nY );

		// 家形ID取得 (15春向けから)
		// 家形に乗ってないことも多々あるので、乗ってなくてもメッセージは出さない
		long buildingID = -1; // POIが乗ってる建物のOID
		GetBuildingInfo(ipGeo, buildingID);
		cPoi.m_buildingID = buildingID;

		// マッチング種別を設定して格納
		// 重要コンテンツ：0、位置品質向上：1、ピンポイント率向上：2（Bug9215対応：コード値変更）
		if( pUsable->bImportantF )
		{
			cPoi.m_nAddXYC	= 0;
			m_setImpPOI.insert( cPoi );	// レコード格納
		}
		else
		{
			cPoi.m_nAddXYC = pUsable->bQualityF? 1 : 2;
			m_setPOI.insert( cPoi );	// レコード格納
		}

		// ここまできたらSUB_POINT情報も取得
		SetPOISub( cPoi.m_lOID );
	}
}

// Poi_Sub_Pointレコード格納
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

		m_setPOISub.insert( cPoiSub ); // レコード格納

		feature->get_Value( m_idnCode_FID, &idn );
		++m_idnCodeCounter[idn.lVal];
	}
}

// 行政界情報取得
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

			// 市区町村コード
			ipFeature->get_Value( m_lCityCode_FID, &vaValue );
			rCodeInfo.first = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
			vaValue.Clear();

			// 住所コード1
			ipFeature->get_Value( m_lAddrCode_FID, &vaValue );
			rCodeInfo.first += (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
			vaValue.Clear();

			// 住所コード2
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

	// 1回普通に空間検索してみる。だいたいこっちで拾って終わり
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

	// ポイントが中抜き部分に落ちてるかもしれないので、一度ポイントにバッファかけて検索して、検索候補を作る
	IGeometryPtr ipSearchGeom;
	((ITopologicalOperatorPtr)ipGeom)->Buffer( 0.001, &ipSearchGeom ); // 100mくらいバッファ

	// 候補を取ってくる
	ISpatialFilterPtr ipFilter2( CLSID_SpatialFilter );
	ipFilter2->putref_Geometry( ipSearchGeom );
	ipFilter2->put_SpatialRel( esriSpatialRelIntersects );
	ipFilter2->put_SubFields( CComBSTR( subFields ) );
	IFeatureCursorPtr ipCursor2;
	m_ipBuildingFC->Search( ipFilter2, VARIANT_TRUE, &ipCursor2 );
	if( !ipCursor2 )
		return false;

	// 改めてポイントが乗るかどうかをチェック
	IFeaturePtr ipFeature2;
	while( S_OK == ipCursor2->NextFeature( &ipFeature2 ) && ipFeature2 )
	{
		// Exteriorを取る(=中抜き埋めた形状)
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

			// ポイントが乗るか？
			// Disjointの方が早いけど、上手く家形が取れないのでWithinでやる
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
	// TODO:IDNCODEのチェックは別でやって、DELETE_CもWhere句に含める
	IQueryFilterPtr ipPOISubQuery( CLSID_QueryFilter );
	CString poiSubWhereClause;
	poiSubWhereClause.Format( _T("%s in (%s)"), sindyk::poi_sub_point_org::kIDNCode, m_idnCode );
	ipPOISubQuery->put_WhereClause( CComBSTR(poiSubWhereClause) );
	IFeatureCursorPtr ipSubPOICursor;
	m_ipPOISubPointFC->Search( ipPOISubQuery, VARIANT_FALSE, &ipSubPOICursor );
	if( !ipSubPOICursor )
	{
		wcerr << L"#Error IDNCODEでの絞り込みに失敗：IDNCODE " << (CT2CW)m_idnCode << endl;
		return false;
	}

	long deleteCodeIndex = -1;
	m_ipPOISubPointFC->FindField( CComBSTR(sindyk::poi_sub_point_org::kDelete), &deleteCodeIndex );
	if( deleteCodeIndex < 0 )
	{
		wcerr << L"#Error POI_SUB_POINTフィールドインデックスの取得に失敗" << endl;
		return false;
	}

	// DELETE_C=0 のレコードを格納
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
