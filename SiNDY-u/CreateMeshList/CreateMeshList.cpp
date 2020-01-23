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

// CreateMeshList.cpp: CreateMeshList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CreateMeshList.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CreateMeshList::CreateMeshList()
{

	char	*aTmpEnv;

	mAllAdmin	= false;

	strcpy(mFCCityMesh,		FC_CITYMESH);
	strcpy(mFCBaseMesh,		FC_BASEMESH);
	strcpy(mFCCityAdmin,	FC_CITYADMIN);
	strcpy(mFDCityMeshCode,	FD_CITYMESHCODE);
	strcpy(mFDCompleteF,	FD_COMPLETE_F);

	if((aTmpEnv = getenv("FC_CITYMESH"))		!= NULL) strcpy(mFCCityMesh,		aTmpEnv);
	if((aTmpEnv = getenv("FC_BASEMESH"))		!= NULL) strcpy(mFCBaseMesh,		aTmpEnv);
	if((aTmpEnv = getenv("FC_CITYADMIN"))		!= NULL) strcpy(mFCCityAdmin,		aTmpEnv);

#ifdef _DEBUG
	_putenv("DB_ADDRESS=sindy/sindy/SDE.SiNDY200308/5151/topaz");
//	_putenv("DB_ADDRESS=sindy/sindy/SDE.DEFAULT/5151/topaz");
//	_putenv("DB_ADDRESS=trial/trial/SDE.DEFAULT/5151/topaz");
#endif

	mDBCon_Address = getenv("DB_ADDRESS");

}

CreateMeshList::~CreateMeshList()
{

}

bool CreateMeshList::
fnAnalyzeArg(void)
{

	int	i;

	if(mArgc < 2) {
		fprintf(stderr, "引数が足りません\n");
		fnPrintUsage();
		return false;
	}

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {

		switch(mArgv[i][1]) {

		case 'A':
			mAllAdmin = true;
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

	if(i < mArgc - 1) {
		fprintf(stderr, "引数が足りません\n");
		fnPrintUsage();
		return false;
	}

	mOutFN = mArgv[i];

	if(mDBCon_Address == NULL) {
		fprintf(stderr, "環境変数 DB_ADDRESS が指定されていません\n");
		return false;
	}

	if(mOutFN == NULL) {
		fprintf(stderr, "出力ファイル名が指定されていません\n");
		return false;
	}

	return true;

}

void CreateMeshList::
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
		"\tメッシュリスト（都市地図）を作成する\n\n"
		"【使用形式】\n\n"
		"\t%s [オプション] <出力メッシュリストファイル名>\n\n"
		"【オプション】\n\n"
		"\t-A\n"
		"\t\tメッシュリストの作成対象を行政界ポリゴンの整備エリアとする\n"
		"\t\t（行政界ポリゴンを１つでも含むメッシュをリストに出力する）\n\n"
		"\t-?\n"
		"\t\t使用法（このテキスト）を表示\n\n"
		"【環境変数】\n\n"
		"\tDB_ADDRESS\n"
		"\t\tGeoDB接続文字列（住所系サーバ）を指定\n"
		"\t\t形式: <user>/<password>/<version>/<instance>/<server>\n\n"
		"\tFC_CITYMESH\n"
		"\t\t都市地図メッシュレイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_BASEMESH\n"
		"\t\t中縮メッシュレイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_CITYADMIN\n"
		"\t\t都市地図行政界レイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n",
		aExecFN, aExecFN, FC_CITYMESH, FC_BASEMESH, FC_CITYADMIN);

}

bool CreateMeshList::
Create(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnAnalyzeArg()) return false;

	// 接続
	if(!Connect(mDBCon_Address)) return false;

	// フィーチャクラス取得
	ipFC_CityMesh	= GetFeatureClass(mFCCityMesh);		if(ipFC_CityMesh	== NULL) return false;
	ipFC_BaseMesh	= GetFeatureClass(mFCBaseMesh);		if(ipFC_BaseMesh	== NULL) return false;
	if(mAllAdmin) {
		ipFC_CityAdmin	= GetFeatureClass(mFCCityAdmin);	if(ipFC_CityAdmin	== NULL) return false;
	}

	// フィールドインデックス取得
	LONG	aFID_MeshCode;
	if(!GetFieldID(ipFC_CityMesh, mFDCityMeshCode, &aFID_MeshCode)) {
		printf("%s のフィールドインデックスを取得できませんでした\n", mFDCityMeshCode);
		return false;
	}

	// フィーチャカーソル取得
	LONG	aCount;
	IFeatureCursorPtr	ipFCur_CityMesh;
	if(mAllAdmin) {
		ipFCur_CityMesh = SearchByWhereClause(NULL, ipFC_CityMesh, &aCount);
	} else {
		char	aWhereClause[300];
		sprintf(aWhereClause, "%s is not null", mFDCompleteF);
		ipFCur_CityMesh = SearchByWhereClause(aWhereClause, ipFC_CityMesh, &aCount);
	}

	fprintf(stderr, "全メッシュ取得中...");
	// フィーチャカーソルのループ
	int		i = 0;
	IFeaturePtr	ipF_CityMesh;
	while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

		long	aOID_CityMesh;
		ipF_CityMesh->get_OID(&aOID_CityMesh);

		CComVariant	aVar_MeshCode;
		ipF_CityMesh->get_Value(aFID_MeshCode, &aVar_MeshCode);

		MeshListCont	aMLCont(aVar_MeshCode.lVal, aOID_CityMesh);
		mMeshList.insert(aMLCont);

	}
	fprintf(stderr, "完了\n");

	if(mAllAdmin) {

		fprintf(stderr, "行政界ポリゴンの有無確認中...\n");

		ISpatialCacheManagerPtr		ipCacheMgr(ipWorkspace);
		char	aBaseMeshCode[7];
		memset(aBaseMeshCode, 0, 7);

		// メッシュリストのループ
		long	i = 0, aMLSize = mMeshList.size();
		MeshListSet_i	aMLItr;
		for(aMLItr = mMeshList.begin(); aMLItr != mMeshList.end();) {

			i++;
			fprintf(stderr, "%7ld/%7ld\r", i, aMLSize);

			if(strncmp(aBaseMeshCode, (*aMLItr).eMeshCode, 6) != 0) {
				// キャッシュ入れ替え
				IFeatureCursorPtr	ipFCur_BaseMesh;
				long	aBMCount;
				char	aWhere[64];
				sprintf(aBaseMeshCode, "%.6s", (*aMLItr).eMeshCode);
				sprintf(aWhere, "%s = %s", FD_BASEMESHCODE, aBaseMeshCode);
				ipFCur_BaseMesh = SearchByWhereClause(aWhere, ipFC_BaseMesh, &aBMCount);
				if(aBMCount != 0) {
					IFeaturePtr		ipF_BaseMesh;
					if(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) == S_OK) {
						IEnvelopePtr	ipE_BaseMesh;
						ipF_BaseMesh->get_Extent(&ipE_BaseMesh);
						ipCacheMgr->FillCache(ipE_BaseMesh);
					} else {
						fprintf(stderr, "キャッシュの作成に失敗(継続します): %s\n", aBaseMeshCode);
					}
				} else {
					fprintf(stderr, "キャッシュの作成に失敗(継続します): %s\n", aBaseMeshCode);
				}
			}

			// OIDからフィーチャを取得
			IFeaturePtr		ipF_CityMesh;
			ipFC_CityMesh->GetFeature((*aMLItr).eObjectID, &ipF_CityMesh);

			// エンベロープ取得
			IEnvelopePtr	ipE_CityMesh;
			ipF_CityMesh->get_Extent(&ipE_CityMesh);

			// 空間検索
			long	aCACount=0;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			ipFCur_CityAdmin = SearchByShapeAndDesc(ipE_CityMesh,"T********", ipFC_CityAdmin, &aCACount);

			// 引っかかる行政界ポリゴンがない場合 → メッシュリストから除去
			if(aCACount == 0) {
				mMeshList.erase(aMLItr++);
			} else {
				aMLItr++;
			}

		}

		fprintf(stderr, "\n完了\n");

	}

/*
	// フィーチャカーソルのループ
	int		i = 0;
	IFeaturePtr	ipF_CityMesh;
	while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

		i++;
		fprintf(stderr, "%7ld/%7ld\r", i, aCount);

		CComVariant	aVar_MeshCode;
		ipF_CityMesh->get_Value(aFID_MeshCode, &aVar_MeshCode);

		if(mAllAdmin) {
			IEnvelopePtr	ipE_CityMesh;
			ipF_CityMesh->get_Extent(&ipE_CityMesh);
			IFeatureCursorPtr	ipFCur_CityAdmin;
			long	aCACount;
//			ipFCur_CityAdmin = SearchByShape(ipE_CityMesh, esriSpatialRelIntersects, ipFC_CityAdmin, &aCACount);
			ipFCur_CityAdmin = SearchByShape(ipE_CityMesh, "T********", ipFC_CityAdmin, &aCACount);
			if(aCACount == 0) continue;

			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {
				long	aOID_CityAdmin;
				ipF_CityAdmin->get_OID(&aOID_CityAdmin);
				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);
				VARIANT_BOOL	aTouches;
				if(FAILED(((IRelationalOperatorPtr)ipE_CityMesh)->Touches(ipG_CityAdmin, &aTouches))) {
					fprintf(stdout, "%8ld, %9ld, IRelationalOperator::Touches 失敗\n", aVar_MeshCode.lVal, aOID_CityAdmin);
					continue;
				}
				if(aTouches) aCACount--;
			}
			if(aCACount == 0) continue;

		}

		MeshListCont	aMLCont(aVar_MeshCode.lVal);
		mMeshList.insert(aMLCont);
		fprintf(stderr, "----- %8ld -----\n", aVar_MeshCode.lVal);

	}
*/
	// メッシュコードの出力
	if(!mMeshList.Create(mOutFN)) {
		fprintf(stderr, "出力ファイルを作成できませんでした: %s\n", mOutFN);
		return false;
	}

	fprintf(stderr, "全処理終了\n");

	return true;

}

// SpatialRelDescriptionを利用したshapeによる検索
IFeatureCursorPtr CreateMeshList::SearchByShapeAndDesc(IGeometryPtr		ipGeometry,
			  LPCTSTR			cSpatialRelDesc,
			  IFeatureClassPtr	ipFeatureClass,
			  LONG				*cCount)
{

	IFeatureCursorPtr	ipFeatureCursor;

	if(!ipFeatureClass) {
		std::cout << "[SearchByShapeAndDesc] フィーチャクラスが NULL です" << std::endl;
		return nullptr;
	}

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR	bstrFieldName;

	/// クエリフィルタ作成
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeometry );
	ipFilter->put_SpatialRel( esriSpatialRelRelation );
	ipFilter->put_SpatialRelDescription( CComBSTR(cSpatialRelDesc) );

	/// 検索結果が 0 ならリターン
	ipFeatureClass->FeatureCount( ipFilter, cCount );
	if( *cCount == 0 ) {
		return nullptr;
	}

	/// 検索
	if(FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor ))) {
		std::cout << "[SearchByShapeAndDesc] Search に失敗しました" << std::endl;
		*cCount = 0;
		return nullptr;
	}

	return ipFeatureCursor;

}