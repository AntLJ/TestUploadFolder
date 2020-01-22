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

// CheckBaseIsles.cpp: CheckBaseIsles クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CheckBaseIsles.h"
#include <sindy/workspace.h>
#include <sindy/libschema.h>
#include <sindy/schema.h>

using namespace sindy;

char	gMessage[1024];



// クエリフィルタ設定用関数
IQueryFilterPtr AheInitQueryFilter( IQueryFilter* ipFilter/*			= NULL*/,
								 LPCTSTR lpcszSubFields/*				= NULL*/,
								 LPCTSTR lpcszWhereClause/*				= NULL*/,... )
{
	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp.CreateInstance( CLSID_QueryFilter );
	else
		ipFilterTmp = ipFilter;

	if( ipFilterTmp )
	{
		ipFilterTmp->put_SubFields( _bstr_t(lpcszSubFields) );
		if( lpcszWhereClause )
		{
			va_list args;
			va_start( args, lpcszWhereClause );
			CString strWhere;
			strWhere.FormatV( lpcszWhereClause, args );
			ipFilterTmp->put_WhereClause( CComBSTR((LPCTSTR)strWhere) );
			va_end(args);
		}
	}
	else
		_ASSERTE( ipFilterTmp );

	return ipFilterTmp;
}

// 空間クエリフィルタ設定用関数
ISpatialFilterPtr AheInitSpatialFilter( ISpatialFilter* ipFilter/*		= NULL*/,
									IGeometry* pGeom/*					= NULL*/,
									LPCTSTR lpcszGeomField/*			= _T("Shape")*/,
									esriSearchOrder order/*				= esriSearchOrderSpatial*/,
									esriSpatialRelEnum SpatialRel/*		= esriSpatialRelIntersects*/,
									LPCTSTR lpcszSpatialRelDescription/*= NULL*/ )
{
	ISpatialFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp.CreateInstance( CLSID_SpatialFilter );
	else
		ipFilterTmp = ipFilter;

	_ASSERTE( ipFilterTmp );
	ipFilterTmp->putref_Geometry( pGeom );
	ipFilterTmp->put_GeometryField( _bstr_t(lpcszGeomField) );
	ipFilterTmp->put_SearchOrder( order );
	ipFilterTmp->put_SpatialRel( SpatialRel );
	ipFilterTmp->put_SpatialRelDescription( _bstr_t(lpcszSpatialRelDescription) );

	return ipFilterTmp;
}

// フィールドインデックスを取得する
long FindFieldIndex( IFeatureClass* ipClass, LPCTSTR lpcszField )
{
	IFieldsPtr ipFields;
	long nIndex = -1;
	ipClass->FindField( CComBSTR(lpcszField), &nIndex );
	return nIndex;
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CheckBaseIsles::CheckBaseIsles()
{

	mMeshCode	= NULL;
	mMeshListFN	= NULL;
	mErrorLogFN	= NULL;

	mFCN_CityAdmin	= schema::city_admin::kTableName;
	mFCN_BaseSite	= schema::base_site::kTableName;
	mFCN_BaseMesh	= schema::basemesh::kTableName;

}

CheckBaseIsles::~CheckBaseIsles()
{

}

bool CheckBaseIsles::
fnGetEnvVar(void)
{

	char	*aTmp;

#ifdef _DEBUG
	_putenv("DB_BASE=trial/trial/ETC_DEVELOP_TES/5151/coral2");
	_putenv("DB_ADDRESS=trial/trial/ETC_DEVELOP_TES/5151/coral2");
	_putenv("FC_CITYADMIN=TRIAL.City_Admin");
	_putenv("FC_BASESITE=TRIAL.Base_Site");
	//_putenv("DB_BASE=ronly/ronly/SDE.DEFAULT/5151/onyx");
	//_putenv("DB_ADDRESS=ronly/ronly/SDE.DEFAULT/5151/topaz2");
	//_putenv("FC_CITYADMIN=FIX201111.City_Admin");
	//_putenv("FC_BASESITE=FIX201111.Base_Site");
	_putenv("FC_BASEMESH=REFERENCE.BaseMesh");
	_putenv("LOG_FILE=log.txt");
#endif

	mDBCon_Base		= getenv("DB_BASE");
	mDBCon_Address	= getenv("DB_ADDRESS");

	if(mDBCon_Base == NULL) {
		fprintf(stderr, "環境変数 DB_BASE が設定されていません\n");
		return false;
	}
	if(mDBCon_Address == NULL) {
		fprintf(stderr, "環境変数 DB_ADDRESS が設定されていません\n");
		return false;
	}

	aTmp	= getenv("FC_CITYADMIN");	if(aTmp != NULL) mFCN_CityAdmin	= aTmp;
	aTmp	= getenv("FC_BASESITE");	if(aTmp != NULL) mFCN_BaseSite	= aTmp;
	aTmp	= getenv("FC_BASEMESH");	if(aTmp != NULL) mFCN_BaseMesh	= aTmp;

	mErrorLogFN = getenv("LOG_FILE");

	return true;

}

bool CheckBaseIsles::
fnAnalyzeArg(void)
{

	int	i;

	if(mArgc < 2) {
		fprintf(stderr, "引数が足りません\n");
		return false;
	}

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {

		switch(mArgv[i][1]) {

		case 'a':
			mCheckMode = kCheckAllMesh;
			break;

		case 'm':
			i++;
			mCheckMode = kCheckSingleMesh;
			if(i < mArgc) mMeshCode = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case 'l':
			i++;
			mCheckMode = kCheckMeshesInList;
			if(i < mArgc) mMeshListFN = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case 'o':
			i++;
			if(i < mArgc) mErrorLogFN = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case '?':
			fnPrintUsage();
			return false;
			break;

		default:
			fprintf(stderr, "不正なオプションです: %s\n", mArgv[i]);
			return false;

		}

	}

	if(mErrorLogFN == NULL) {
		fprintf(stderr, "ログファイルが指定されていません\n");
		return false;
	}
	if(mCheckMode == kCheckNone) {
		fprintf(stderr, "オプション -a / -m / -l のいずれかが必要です\n");
		return false;
	} else if(mCheckMode == kCheckSingleMesh && (mMeshCode == NULL || strlen(mMeshCode) != 6 || strspn(mMeshCode, "1234567890") != strlen(mMeshCode))) {
		fprintf(stderr, "メッシュコードが不正です\n");
		return false;
	} else if(mCheckMode == kCheckMeshesInList && mMeshListFN == NULL) {
		fprintf(stderr, "メッシュリストが指定されていません\n");
		return false;
	}

	return true;

}

void CheckBaseIsles::
fnPrintUsage(void)
{

	char	*aExecFN;
	aExecFN = strrchr(mArgv[0], '\\');
	if(aExecFN == NULL) aExecFN = mArgv[0];
	else aExecFN++;

	fprintf(stderr,
		"\n【コマンド名称】\n\n"
		"\t%s\n\n"
		"【処理内容】\n\n"
		"\t中縮背景の陸地部分が都市地図行政界ポリゴンの海以外のポリゴンに\n"
		"\t内包されるかどうかチェックする\n\n"
		"【使用形式】\n\n"
		"\t%s <オプション>\n\n"
		"【オプション】\n\n"
		"\t-a\n"
		"\t\t全中縮メッシュポリゴンについてチェックする\n"
		"\t\t（メッシュリストは自動生成）\n\n"
		"\t-m <メッシュコード>\n"
		"\t\t指定したコードのメッシュについてチェックを実施\n\n"
		"\t-l <ファイル名>\n"
		"\t\tチェック対象のメッシュリストファイルを指定\n\n"
		"\t-o <ファイル名>\n"
		"\t\tログ出力先ファイル名を指定\n"
		"\t\t（環境変数 LOG_FILE でも指定可）\n\n"
		"\t-?\n"
		"\t\t使用法（このテキスト）を表示\n\n"
		"【環境変数】\n\n"
		"\tLOG_FILE\n"
		"\t\tログ出力先ファイル名を指定\n\n"
		"\tDB_BASE\n"
		"\t\tGeoDB接続文字列（中縮サーバ）を指定\n"
		"\t\t形式: <user>/<password>/<version>/<instance>/<server>\n\n"
		"\tDB_ADDRESS\n"
		"\t\tGeoDB接続文字列（住所系サーバ）を指定\n"
		"\t\t形式: （同上）\n\n"
		"\tFC_CITY_ADMIN\n"
		"\t\t都市地図行政界レイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_BASE_SITE\n"
		"\t\t中縮背景レイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_BASEMESH\n"
		"\t\t中縮メッシュポリゴンレイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n",
		aExecFN, aExecFN, schema::city_admin::kTableName, schema::base_site::kTableName, schema::basemesh::kTableName);

}

bool CheckBaseIsles::
fnMakeMeshList(void)
{

	switch(mCheckMode) {

	case kCheckAllMesh:

		// 全都市地図メッシュのリストを生成
		{
			long				aCount;
			IFeatureCursorPtr	ipFCur_BaseMesh;
			ipFC_BaseMesh->Search( AheInitQueryFilter( NULL, schema::basemesh::kMeshCode, NULL ), VARIANT_TRUE, &ipFCur_BaseMesh );

			IFeaturePtr			ipF_BaseMesh;
			while(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) == S_OK) {
				CComVariant		aVar_MeshCode;
				ipF_BaseMesh->get_Value(mFID_MeshCode, &aVar_MeshCode);
				mMeshList.Add(aVar_MeshCode.lVal);
			}
		}

		break;

	case kCheckMeshesInList:
		// メッシュリストファイルを開く
		mMeshList.Open(mMeshListFN);
		break;

	case kCheckSingleMesh:
		// 指定メッシュひとつをリストへ追加
		mMeshList.Add(mMeshCode);
		break;

	default:
		break;

	}

	return true;

}

bool CheckBaseIsles::
Execute(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnGetEnvVar())	return false;
	if(!fnAnalyzeArg())	return false;

	// 接続
	CString strErr( _T("ワークスペースが開けません") );
	m_ipBaseWS = create_workspace( mDBCon_Base ); if( !m_ipBaseWS ) strErr.AppendFormat( _T("\n %s"), mDBCon_Base);
	m_ipAddrWS = create_workspace( mDBCon_Address ); if( !m_ipAddrWS ) strErr.AppendFormat( _T("\n %s"), mDBCon_Address);
	if( !(m_ipBaseWS && m_ipAddrWS) )
	{
		std::cerr << CStringA(strErr) << std::endl;
		return false;
	}

	// フィーチャクラス取得
	strErr = _T("フィーチャクラスが開けません");
	((IFeatureWorkspacePtr)m_ipBaseWS)->OpenFeatureClass( CComBSTR(mFCN_BaseSite), &ipFC_BaseSite ); if( !ipFC_BaseSite ) strErr.AppendFormat( _T("\n %s"), mFCN_BaseSite);
	((IFeatureWorkspacePtr)m_ipAddrWS)->OpenFeatureClass( CComBSTR(mFCN_CityAdmin), &ipFC_CityAdmin ); if( !ipFC_CityAdmin ) strErr.AppendFormat( _T("\n %s"), mFCN_CityAdmin);
	((IFeatureWorkspacePtr)m_ipAddrWS)->OpenFeatureClass( CComBSTR(mFCN_BaseMesh), &ipFC_BaseMesh ); if( !ipFC_BaseMesh ) strErr.AppendFormat( _T("\n %s"), mFCN_BaseMesh);
	if( !(ipFC_BaseSite && ipFC_CityAdmin &&ipFC_BaseMesh) )
	{
		std::cerr << CStringA(strErr) << std::endl;
		return false;
	}

	// フィールドインデックス取得
	mFID_AreaClass_c = FindFieldIndex( ipFC_CityAdmin, schema::city_admin::kAreaClass );
	mFID_Sc1BgClass_c = FindFieldIndex( ipFC_BaseSite, schema::base_site::kSC1BgClass );
	mFID_Sc2BgClass_c = FindFieldIndex( ipFC_BaseSite, schema::base_site::kSC2BgClass );
	mFID_Sc3BgClass_c = FindFieldIndex( ipFC_BaseSite, schema::base_site::kSC3BgClass );
	mFID_Sc4BgClass_c = FindFieldIndex( ipFC_BaseSite, schema::base_site::kSC4BgClass );
	mFID_MeshCode = FindFieldIndex( ipFC_BaseMesh, schema::basemesh::kMeshCode );

	// メッシュリスト作成
	fprintf(stderr, "メッシュリスト作成中...");
	if(!fnMakeMeshList()) {
		fprintf(stderr, "失敗\n");
		return false;
	}
	fprintf(stderr, "完了\n");

	// ログファイルのオープン
	if(!mErrLog.Open(mErrorLogFN)) {
		fprintf(stderr, "ログファイルが開けません\n");
		return false;
	}
	if(!mErrLog.SetProgramID(PROGRAM_ID)) {
		fprintf(stderr, "プログラムＩＤを設定できません\n");
		return false;
	}
	if(!mErrLog.SetErrCodeTable(gECTable, ECT_MAX_RECORD)) {
		fprintf(stderr, "エラーコードテーブルを設定できません\n");
		return false;
	}

	mErrLog.OutputComment("行政界・中縮背景整合チェックツール by H.Hyodo");

	sprintf(gMessage, "DB_BASE: %s", mDBCon_Base);
	mErrLog.OutputComment(gMessage);
	sprintf(gMessage, "DB_ADDRESS: %s", mDBCon_Address);
	mErrLog.OutputComment(gMessage);

	SYSTEMTIME	aStartTime;
	GetLocalTime(&aStartTime);
	sprintf(gMessage, "開始時刻: %04d/%02d/%02d %02d:%02d:%02d",
		aStartTime.wYear, aStartTime.wMonth, aStartTime.wDay, aStartTime.wHour, aStartTime.wMinute, aStartTime.wSecond);
	mErrLog.OutputComment(gMessage);
	mErrLog.Flush();

	ISpatialCacheManagerPtr		ipSCM_Base(m_ipBaseWS), ipSCM_Address(m_ipAddrWS);

	ISpatialReferencePtr	ipSpRef;

	// 中縮メッシュのループ
	MeshListSet_i	aMLItr;
	for(aMLItr = mMeshList.begin(); aMLItr != mMeshList.end(); aMLItr++) {

		mErrLog.OutputComment((*aMLItr).eMeshCode);

		// キャッシュの作成
		IFeatureCursorPtr	ipFCur_BaseMesh;
		IFeaturePtr			ipF_BaseMesh;
		IEnvelopePtr		ipE_BaseMesh;
		IGeometryPtr		ipG_BaseMesh;
		long	aBMCount = 0;
		fprintf(stderr, "%s: キャッシュ作成中...", (*aMLItr).eMeshCode);

		IQueryFilterPtr ipQuery( AheInitQueryFilter( NULL, NULL, _T("%s = %s"), schema::basemesh::kMeshCode, (*aMLItr).eMeshCode ) );
		ipFC_BaseMesh->FeatureCount( ipQuery, &aBMCount );
		if(aBMCount == 0) {
			fprintf(stderr, "メッシュポリゴンがありません\n", (*aMLItr).eMeshCode);
			continue;
		}
		ipFC_BaseMesh->Search( ipQuery, VARIANT_TRUE, &ipFCur_BaseMesh );
		
		if(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) != S_OK) {
			fprintf(stderr, "%s: キャッシュの設定に失敗しました\n", (*aMLItr).eMeshCode);
			goto EXIT_CACHE_CREATION;
		}
		ipF_BaseMesh->get_Extent(&ipE_BaseMesh);
		ipSCM_Base->FillCache(ipE_BaseMesh);
		if(ipSCM_Base != ipSCM_Address) {
			ipSCM_Address->FillCache(ipE_BaseMesh);
		}
		ipF_BaseMesh->get_Shape(&ipG_BaseMesh);
		fprintf(stderr, "完了\n");

EXIT_CACHE_CREATION:

		long	aOID_BaseMesh;
		ipF_BaseMesh->get_OID(&aOID_BaseMesh);

		// 中縮背景から海ポリゴンを取得
		IPolygonPtr			ipP_BaseSea(CLSID_Polygon);
		IGeometryPtr		ipG_BaseSea;
		IFeatureCursorPtr	ipFCur_BaseSite;
		IFeaturePtr			ipF_BaseSite;

		IQueryFilterPtr ipBaseSeaQuery( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipG_BaseMesh, _T("Shape"), esriSearchOrderSpatial, esriSpatialRelIntersects, NULL ) , NULL, NULL ) );
		long	aBSCount = 0;
		ipFC_BaseSite->FeatureCount(ipBaseSeaQuery, &aBSCount );
		ipFC_BaseSite->Search(ipBaseSeaQuery, VARIANT_TRUE, &ipFCur_BaseSite );
		while(ipFCur_BaseSite->NextFeature(&ipF_BaseSite) == S_OK) {
			long	aOID_BaseSite;
			IGeometryPtr	ipG_BaseSite;
			CComVariant	aVar_BGClass1, aVar_BGClass2, aVar_BGClass3, aVar_BGClass4;
			ipF_BaseSite->get_OID(&aOID_BaseSite);
			ipF_BaseSite->get_Value(mFID_Sc1BgClass_c, &aVar_BGClass1);
			ipF_BaseSite->get_Value(mFID_Sc2BgClass_c, &aVar_BGClass2);
			ipF_BaseSite->get_Value(mFID_Sc3BgClass_c, &aVar_BGClass3);
			ipF_BaseSite->get_Value(mFID_Sc4BgClass_c, &aVar_BGClass4);
//			if(aVar_BGClass1.lVal == 101001 || aVar_BGClass2.lVal == 101001 ||
//				aVar_BGClass3.lVal == 101001 || aVar_BGClass4.lVal == 101001) {		// いずれかのスケールで「海」の場合
			if(aVar_BGClass1.lVal == 101001 && aVar_BGClass2.lVal == 101001 &&
				aVar_BGClass3.lVal == 101001 && aVar_BGClass4.lVal == 101001) {		// すべてのスケールで「海」の場合
				ipF_BaseSite->get_Shape(&ipG_BaseSite);

				// Arc931対応で追加(空間参照が無いとジオメトリ演算がおかしくなる) 2010/06/11
				ipG_BaseSite->get_SpatialReference( &ipSpRef );	// 空間参照を取得	
				ipP_BaseSea->putref_SpatialReference( ipSpRef );

				VARIANT_BOOL	aTouches;
				((IRelationalOperatorPtr)ipG_BaseSite)->Touches(ipG_BaseMesh, &aTouches);
				if(!aTouches) {
					if(FAILED(((ITopologicalOperatorPtr)ipP_BaseSea)->Union(ipG_BaseSite, &ipG_BaseSea))) {
						sprintf(gMessage, "ITopologicalOperator::Union");
						mErrLog.OutputErr(mFCN_BaseSite, aOID_BaseSite, ERR_FAILED_AO_METHOD, gMessage);
					}
					ipP_BaseSea = ipG_BaseSea;
				} else {
					// 実際にはメッシュ矩形内にはない（外接している）ものは弾く（bug 9125）
					--aBSCount;
				}
			}
		}
		if(aBSCount == 0) {
			fprintf(stderr, "中縮背景ポリゴンが一つもありません\n");
			continue;
		}

		// 「海」ポリゴン→「陸」ポリゴン生成
		IGeometryPtr	ipG_BaseLand;
		if(FAILED(((ITopologicalOperatorPtr)ipG_BaseMesh)->Difference(ipP_BaseSea, &ipG_BaseLand))) {
			sprintf(gMessage, "ITopologicalOperator::Difference");
			mErrLog.OutputErr(mFCN_BaseMesh, aOID_BaseMesh, ERR_FAILED_AO_METHOD, gMessage);
		}
		VARIANT_BOOL	aEmpty;
		ipG_BaseLand->get_IsEmpty(&aEmpty);
		if(aEmpty) {
			fprintf(stderr, "メッシュ全域が海です\n");
			continue;
		}

		// メッシュ内の行政界ポリゴン(海以外)を統合
		IPolygonPtr			ipP_AdminLand(CLSID_Polygon);
	
		// Arc931対応で追加(空間参照が無いとジオメトリ演算がおかしくなる) 2010/06/11
		if( !ipSpRef )
			ipG_BaseMesh->get_SpatialReference( &ipSpRef );
		ipP_AdminLand->putref_SpatialReference( ipSpRef );

		IGeometryPtr		ipG_AdminLand;
		IFeatureCursorPtr	ipFCur_CityAdmin;
		IFeaturePtr			ipF_CityAdmin;
		long	aCACount = 0;
		IQueryFilterPtr ipCityAdmQuely = AheInitQueryFilter( AheInitSpatialFilter( NULL, ipG_BaseMesh, _T("Shape"), esriSearchOrderSpatial, esriSpatialRelIntersects, NULL ) , NULL, NULL );
		ipFC_CityAdmin->FeatureCount( ipCityAdmQuely, &aCACount );
		if(aCACount == 0) {
			mErrLog.OutputErr(mFCN_BaseMesh, aOID_BaseMesh, ERR_NO_ADMIN_POLY, NULL);
			continue;
		}

		ipFC_CityAdmin->Search( ipCityAdmQuely, VARIANT_TRUE, &ipFCur_CityAdmin );
		while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {
			long	aOID_CityAdmin;
			IGeometryPtr	ipG_CityAdmin;
			CComVariant		aVar_AreaClass;
			ipF_CityAdmin->get_OID(&aOID_CityAdmin);
			ipF_CityAdmin->get_Value(mFID_AreaClass_c, &aVar_AreaClass);
			if(aVar_AreaClass.lVal != 2) {	// 海でない
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

//				((IAreaPtr)ipG_CityAdmin)->get_Area(&aArea);
//				fprintf(stderr, "[%8d] %15.12f\n", aOID_CityAdmin, aArea);
				if(FAILED(((ITopologicalOperatorPtr)ipP_AdminLand)->Union(ipG_CityAdmin, &ipG_AdminLand))) {
					sprintf(gMessage, "ITopologicalOperator::Union");
					mErrLog.OutputErr(mFCN_CityAdmin, aOID_CityAdmin, ERR_FAILED_AO_METHOD, gMessage);
				}
				ipP_AdminLand = ipG_AdminLand;
			}
		}

//		((IAreaPtr)ipP_AdminLand)->get_Area(&aArea);
//		fprintf(stderr, "[total---] %15.12f\n", aArea);

		// 「陸」ポリゴン群のループ
		long	aMaxRing;
		((IGeometryCollectionPtr)ipG_BaseLand)->get_GeometryCount(&aMaxRing);
		for(int i = 0; i < aMaxRing; i++) {
			if( i == 264 )
				cout << i << endl;
			IPolygonPtr		ipP_Ring(CLSID_Polygon);

			// Arc931対応で追加(空間参照が無いとジオメトリ演算がおかしくなる) 2010/06/11
			ipP_Ring->putref_SpatialReference( ipSpRef );

			IGeometryPtr	ipG_Ring;
			((IGeometryCollectionPtr)ipG_BaseLand)->get_Geometry(i, &ipG_Ring);	// ポリゴン群から１つのポリゴン（リング）を取得

			IPointPtr		ipP_temp;
			double		aX_temp, aY_temp;
			((IAreaPtr)ipG_Ring)->get_Centroid(&ipP_temp);
			ipP_temp->QueryCoords(&aX_temp, &aY_temp);
			if( aX_temp > 136.61868395 && aX_temp < 136.61868396 )
				cout << i << endl;

			VARIANT_BOOL	aIsExterior;
			((IRingPtr)ipG_Ring)->get_IsExterior(&aIsExterior);
			if(!aIsExterior) continue;	// exterior ring のみチェック
			((IGeometryCollectionPtr)ipP_Ring)->AddGeometry(ipG_Ring);

			VARIANT_BOOL	aDisjoint, aDisjoint2;
			VARIANT_BOOL	aOverlaps, aOverlaps2;
			VARIANT_BOOL	aContains2;
			VARIANT_BOOL	aIsPrecisionEqual;
			CComBSTR		bstrAdm, bstrSite;
			HRESULT			HL;
			VARIANT_BOOL	aIsEmpAdm, aIsEmpSite;
			ISpatialReferencePtr	ipSpRefAdm, ipSpRefSite;
			HL = ipP_AdminLand->get_SpatialReference( &ipSpRefAdm );
			HL = ipP_Ring->get_SpatialReference( &ipSpRefSite );
			ipSpRefAdm->get_Name( &bstrAdm );
//			ipSpRefSite->get_Name( &bstrSite );
			((IRelationalOperatorPtr)ipP_AdminLand)->Overlaps(ipP_Ring, &aOverlaps);
			((IRelationalOperatorPtr)ipP_AdminLand)->Disjoint(ipP_Ring, &aDisjoint);
			((IRelationalOperatorPtr)ipP_Ring)->Overlaps(ipP_AdminLand, &aOverlaps2);
			((IRelationalOperatorPtr)ipP_Ring)->Disjoint(ipP_AdminLand, &aDisjoint2);
			((IRelationalOperatorPtr)ipP_Ring)->Contains(ipP_AdminLand, &aContains2);
//			IEnvelopePtr	ipEnvAdm;
//			IEnvelopePtr	ipEnvSite;
//			ipP_AdminLand->get_Envelope( &ipEnvAdm );
//			ipP_Ring->get_Envelope( &ipEnvSite );

			VARIANT_BOOL	aContains;
			((IRelationalOperatorPtr)ipP_AdminLand)->Contains(ipP_Ring, &aContains);
			if(!aContains) {
				IPointPtr	ipP_Centroid;
				double		aX, aY, aArea;
				((IAreaPtr)ipG_Ring)->get_Area(&aArea);
				if(aArea < 0.00001) {	// 面積が小さいとき→重心の座標を出力
					((IAreaPtr)ipG_Ring)->get_Centroid(&ipP_Centroid);
					ipP_Centroid->QueryCoords(&aX, &aY);
					mErrLog.OutputErr(mFCN_CityAdmin, 0, aX, aY, ERR_ILLEGAL_ADMIN_POLY, NULL);
					mErrLog.Flush();
				} else {				// 面積が大きいとき→不適切な行政界と重複している部分の重心の座標を出力（複数）
					IGeometryPtr	ipG_BaseLand2;
					((ITopologicalOperatorPtr)ipP_Ring)->Difference(ipP_AdminLand, &ipG_BaseLand2);
					long	aMaxRing2;
					((IGeometryCollectionPtr)ipG_BaseLand2)->get_GeometryCount(&aMaxRing2);
					for(int j = 0; j < aMaxRing2; j++) {
						IPolygonPtr		ipP_Ring2(CLSID_Polygon);

						// Arc931対応で追加(空間参照が無いとジオメトリ演算がおかしくなる) 2010/06/11
						ipP_Ring2->putref_SpatialReference( ipSpRef );

						IGeometryPtr	ipG_Ring2;
						((IGeometryCollectionPtr)ipG_BaseLand2)->get_Geometry(j, &ipG_Ring2);
						VARIANT_BOOL	aIsExterior2;
						((IRingPtr)ipG_Ring2)->get_IsExterior(&aIsExterior2);
						if(!aIsExterior2) continue;	// exterior ring のみチェック
						((IAreaPtr)ipG_Ring2)->get_Centroid(&ipP_Centroid);
						ipP_Centroid->QueryCoords(&aX, &aY);
						mErrLog.OutputErr(mFCN_CityAdmin, 0, aX, aY, ERR_ILLEGAL_ADMIN_POLY, NULL);
						mErrLog.Flush();
					}
				}
			}
		}

		fprintf(stderr, "%s:\n", (*aMLItr).eMeshCode);
		mErrLog.Flush();

	}

	// 終了時刻をログへ出力
	SYSTEMTIME	aEndTime;
	GetLocalTime(&aEndTime);
	sprintf(gMessage, "終了時刻: %04d/%02d/%02d %02d:%02d:%02d",
		aEndTime.wYear, aEndTime.wMonth, aEndTime.wDay, aEndTime.wHour, aEndTime.wMinute, aEndTime.wSecond);
	mErrLog.OutputComment(gMessage);

	mErrLog.Close();

	return true;

}
