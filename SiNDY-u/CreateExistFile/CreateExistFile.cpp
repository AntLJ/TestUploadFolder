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

// CreateExistFile.cpp: CreateExistFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CreateExistFile.h"
//#include "..\..\LIB\TDC\GeomtoShapeFile\GeomtoShapeFile.h" 動作確認用1/15

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CreateExistFile::CreateExistFile()
{

	char	*aTmpStr;

	mArgc		= 0;
	mArgv		= NULL;

	if((aTmpStr = getenv("EXIST_DIR"))		!= NULL) mOutDir		= aTmpStr;

	mCreateMode	= kCreateNone;

	mDivNum		= 32;

	mFCCityMesh	= FC_CITYMESH;
	mFCBaseMesh	= FC_BASEMESH;
	mFCCitySite	= FC_CITYSITE;

	if((aTmpStr = getenv("DB_MAP"))			!= NULL) mDBMap			= aTmpStr;

	if((aTmpStr = getenv("FC_CITYMESH"))	!= NULL) mFCCityMesh	= aTmpStr;
	if((aTmpStr = getenv("FC_BASEMESH"))	!= NULL) mFCBaseMesh	= aTmpStr;
	if((aTmpStr = getenv("FC_CITYSITE"))	!= NULL) mFCCitySite	= aTmpStr;

	ExistError = false;

}

CreateExistFile::~CreateExistFile()
{

}

bool CreateExistFile::
fnAnalyzeArg()
{

	int		i;

	if(mArgc < ARG_MIN + 1) goto ARG_LACK;

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {

		switch(mArgv[i][1]) {

		case 'a':
			mCreateMode = kCreateAllMesh;
			break;

		case 'l':
			i++;
			mCreateMode = kCreateMeshesInList;
			if(i < mArgc) mFNMeshList = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'm':
			i++;
			mCreateMode = kCreateSingleMesh;
			if(i < mArgc) mMeshCode = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'o':
			i++;
			if(i < mArgc) mOutDir = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'd':
			i++;
			if(i < mArgc) mDivNum = atoi(mArgv[i]);
			else goto ARG_LACK;
			break;

		default:
			cout << "不正なオプションです: " << mArgv[i] << endl;
			return false;

		}

	}

	if(mOutDir.size() == 0) {
		cout << "出力先ディレクトリが指定されていません" << endl;
		return false;
	}

	switch(mCreateMode) {

	case kCreateNone:
		cout << "オプション -a / -l / -m が必要です" << endl;
		return false;

	case kCreateAllMesh:
		break;

	case kCreateMeshesInList:
		if(mFNMeshList.size() == 0) {
			cout << "メッシュリストが指定されていません" << endl;
			return false;
		}
		break;

	case kCreateSingleMesh:
		if(mMeshCode.size() != 8 || mMeshCode.find_first_not_of("1234567890") != string::npos) {
			cout << "メッシュコードが不正です" << endl;
			return false;
		}
		break;

	}

	if(mDivNum < 2 || DIV_MAX < mDivNum) {
		cout << "分割数が不正です（範囲: 2～" << DIV_MAX << "）: " << mDivNum << endl;
		return false;
	}

	return true;

ARG_LACK:

	cout << "引数が足りません" << endl;
	fnPrintUsage();
	return false;

}

void CreateExistFile::
fnPrintUsage()
{

	string	aExecFN(mArgv[0]);
	string::size_type	p = aExecFN.find_last_of('\\') + 1;
	if(p != string::npos) aExecFN.erase(0, p);

	cout << endl;
	cout << "【コマンド名称】" << endl << endl;
	cout << "\t" << aExecFN << endl << endl;
	cout << "【処理内容】" << endl << endl;
	cout << "\t存在判定ファイルを生成する" << endl << endl;
	cout << "【使用形式】" << endl << endl;
	cout << "\t" << aExecFN << " <オプション>" << endl << endl;
	cout << "【オプション】" << endl << endl;
	cout << "\t-a" << endl;
	cout << "\t\t全都市地図メッシュについて生成" << endl;
	cout << "\t\t（メッシュリストは都市地図メッシュレイヤから自動生成）" << endl << endl;
	cout << "\t-l <メッシュリスト>" << endl;
	cout << "\t\tメッシュリストファイルで生成対象メッシュを指定" << endl << endl;
	cout << "\t-m <メッシュコード>" << endl;
	cout << "\t\t指定コードの都市地図メッシュのみ生成" << endl << endl;
	cout << "\t-d <分割数>" << endl;
	cout << "\t\t存在判定を行うエリア単位を，都市地図メッシュの分割数で指定" << endl;
	cout << "\t\tデフォルトは32" << endl << endl;
	cout << "\t-o <ディレクトリ>" << endl;
	cout << "\t\t存在判定ファイルの出力先ディレクトリ指定" << endl;
	cout << "\t\t（環境変数 EXIST_DIR でも指定可）" << endl << endl;
	cout << "\t※ オプションは -a / -l / -m が必須" << endl << endl << endl;

}

bool CreateExistFile::
fnMakeMeshList()
{

	switch(mCreateMode) {

	case kCreateAllMesh:

		// SearchByWhereClause を使い "Complete_f is not null" の条件でカーソル取得 → メッシュリストを生成
		{
			long	aCount;
			string	aWhere;
			IFeatureCursorPtr	ipFCur_CityMesh;

			aWhere = FD_COMPLETE_F " is not null";
			ipFCur_CityMesh = SearchByWhereClause(_T(aWhere.c_str()), ipFC_CityMesh, &aCount);

			if(aCount == 0) {
				cout << "都市地図メッシュが１つもありません" << endl;
				return false;
			}

			IFeaturePtr			ipF_CityMesh;
			while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {
				CComVariant		aVar_MeshCode;
				ipF_CityMesh->get_Value(mFID_CityMeshCode, &aVar_MeshCode);
				mMeshList.Add(aVar_MeshCode.lVal);
			}
		}

		break;

	case kCreateMeshesInList:

		// メッシュリストファイルを開く
		if(!mMeshList.Open(mFNMeshList.c_str())) {
			cout << "メッシュリストファイルを開けません: " << mFNMeshList << endl;
			return false;
		}
		break;

	case kCreateSingleMesh:
		// 指定メッシュひとつをリストへ追加
		if(!mMeshList.Add(mMeshCode.c_str())) {
			cout << "メッシュコードを追加できません: " << mMeshCode << endl;
			return false;
		}
		break;

	default:
		break;

	}

	return true;

}

bool CreateExistFile::
fnCreateCache(const string &cMeshCode)
{

	static string aBMCode;

	if(aBMCode == cMeshCode.substr(0, 6)) return true;

	cout << "キャッシュ作成中..." << flush;

	aBMCode	= cMeshCode.substr(0, 6);

	ISpatialCacheManagerPtr	ipCacheMgr(ipWorkspace);

	string	aWhere;
	long	aCount;
	IFeatureCursorPtr	ipFCur_BaseMesh;
	aWhere	= FD_BASEMESHCODE " = " + aBMCode;
	ipFCur_BaseMesh = SearchByWhereClause(aWhere.c_str(), ipFC_BaseMesh, &aCount);
	if(aCount == 0) {
		ipCacheMgr->EmptyCache();
		cout << "失敗" << endl;
		return false;
	}

	IFeaturePtr	ipF_BaseMesh;
	if(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) == S_OK) {
		IEnvelopePtr	ipE_BaseMesh;
		if (FAILED(ipF_BaseMesh->get_Extent(&ipE_BaseMesh))) {
			cout << "失敗" << endl;
			return false;
		}
		if (FAILED(ipCacheMgr->FillCache(ipE_BaseMesh))) {
			cout << "失敗" << endl;
			return false;
		}
	}

	cout << "完了" << endl;
	return true;

}

IPolygonPtr CreateExistFile::
fnCreateIncompletePoly(IEnvelopePtr ipE_CityMesh, long *cCount)
{

	// メッシュ内背景ポリゴンの取得
	long	aCount;
	IFeatureCursorPtr	ipFCur_CitySite;
	ipFCur_CitySite = SearchByShape(ipE_CityMesh, esriSpatialRelIntersects, ipFC_CitySite, &aCount);
	if(aCount == 0) {
		cout << "背景ポリゴンが１つもない" << endl;
		*cCount = -1;
		return NULL;
	}

	IPolygonPtr	ipPoly_Incomplete(CLSID_Polygon);
	IGeometryBagPtr	a_ipGeomBag(CLSID_GeometryBag);
	IGeometryCollectionPtr	ipGCol_Incomplete(a_ipGeomBag);

	ISpatialReferencePtr	a_ipSpRef;	// 空間参照

	// 背景ポリゴンのループ
	*cCount = 0;
	IFeaturePtr	ipF_CitySite;
	while(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK) {

		// 不完全ポリゴンのみを抽出
		CComVariant	aTmpVar;
		if (FAILED(ipF_CitySite->get_Value(mFID_BGClass_C, &aTmpVar)) ) {
			cout << "BGCLASS_Cの取得に失敗" << endl;
			*cCount = -1;
			return NULL;
		}
		if(aTmpVar.lVal != 60) continue;

		IGeometryPtr	ipG_CitySite;
		ipF_CitySite->get_ShapeCopy(&ipG_CitySite);

		// メッシュ矩形にタッチしているだけのものは除外
		VARIANT_BOOL	aTouches;
		if (FAILED(((IRelationalOperatorPtr)ipE_CityMesh)->Touches(ipG_CitySite, &aTouches))) {
			cout << "タッチの判定に失敗" << endl;
			*cCount = -1;
			return NULL;
		}
		if(aTouches) continue;

		if((*cCount) == 0 ) {
			if (FAILED(ipG_CitySite->get_SpatialReference ( &a_ipSpRef ))) {
				cout << "SpatialReference取得に失敗" << endl;
				*cCount = -1;
				return NULL;
			}
		}

		(*cCount)++;
		if (FAILED(ipGCol_Incomplete->AddGeometry(ipG_CitySite))) {
			cout << "Geometry追加に失敗" << endl;
			*cCount = -1;
			return NULL;
		}
	}

	IEnumGeometryPtr	ipEG_Incomplete(ipGCol_Incomplete);
	long	aIncompCount;
	if (FAILED(ipEG_Incomplete->get_Count(&aIncompCount))) {
		cout << "ポリゴン数の取得に失敗" << endl;
		*cCount = -1;
		return NULL;
	}
	if(aIncompCount == 0) {		// 不完全ポリゴンが１つもない
		*cCount = 0;
		return NULL;
	}

	if (FAILED(a_ipGeomBag->putref_SpatialReference ( a_ipSpRef ))) {
		cout << "SpatialReferenceの適用に失敗" << endl;
		*cCount = 0;
		return NULL;
	}

	// 不完全ポリゴンを統合（ポリゴン or マルチポリゴン）
	if (FAILED(((ITopologicalOperatorPtr)ipPoly_Incomplete)->ConstructUnion(ipEG_Incomplete))) {
		cout << "不完全ポリゴンの統合に失敗" << endl;
		*cCount = 0;
		return NULL;
	}
	return ipPoly_Incomplete;

}

bool CreateExistFile::
Create(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(mDBMap.size() == 0) {
		cout << "環境変数 DB_MAP が設定されていません" << endl;
		return false;
	}

	if(!fnAnalyzeArg()) return false;

	string aExistLine(mDivNum, 'o');

	// 接続
	if(!Connect(mDBMap.c_str())) {
		return false;
	}

	// フィーチャクラス取得
	ipFC_CityMesh = GetFeatureClass(mFCCityMesh.c_str());	if(ipFC_CityMesh == NULL)	return false;
	ipFC_BaseMesh = GetFeatureClass(mFCBaseMesh.c_str());	if(ipFC_BaseMesh == NULL)	return false;
	ipFC_CitySite = GetFeatureClass(mFCCitySite.c_str());	if(ipFC_CitySite == NULL)	return false;

	// フィールドインデックス取得...
	if(!GetFieldID(ipFC_CityMesh,	_T(FD_CITYMESHCODE),	&mFID_CityMeshCode))	return false;
	if(!GetFieldID(ipFC_BaseMesh,	_T(FD_BASEMESHCODE),	&mFID_BaseMeshCode))	return false;
	if(!GetFieldID(ipFC_CityMesh,	_T(FD_COMPLETE_F),		&mFID_CompleteFlag))	return false;
	if(!GetFieldID(ipFC_CitySite,	_T(FD_BGCLASS_C),		&mFID_BGClass_C))		return false;

	// メッシュリストの取得
	if(!fnMakeMeshList()) return false;

	// デバッグ用
	//string	a_sFName	= "C:\\kudo\\SVN\\SiNDY-u\\CreateExistFile\\test\\test.shp";
	//GeomtoShapeFile	a_gGeoToShp;
	//a_gGeoToShp.Init ( a_sFName, esriGeometryPolygon );

	// メッシュリストのループ
	MeshListSet_i	aMLItr;
	for(aMLItr = mMeshList.begin(); aMLItr != mMeshList.end(); aMLItr++) {

		// キャッシュ設定
//		if (! fnCreateCache((*aMLItr).eMeshCode.c_str()))
//			return false;

		// 都市地図メッシュのカーソル取得
		IFeatureCursorPtr	ipFCur_CityMesh;
		long	aMeshCount;
		string	aWhere;

		aWhere	= FD_CITYMESHCODE " = " + (*aMLItr).eMeshCode;

		ipFCur_CityMesh	= SearchByWhereClause(aWhere.c_str(), ipFC_CityMesh, &aMeshCount);
		if(aMeshCount == 0) {
			cout << (*aMLItr).eMeshCode << ": 都市地図メッシュが取得できません" << endl;
			ExistError = true;
			continue;
		}

		// 都市地図メッシュのループ（１回のみ）
		IFeaturePtr		ipF_CityMesh;
		while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

			int		i, j;
			FILE	*fp;
			char	aCurDirName[_MAX_DIR], aFileName[_MAX_FNAME];

			if(ipF_CityMesh == NULL) break;

			// 都市地図メッシュのジオメトリ→エンベロープ取得
			IEnvelopePtr	ipE_CityMesh;
			if (FAILED(ipF_CityMesh->get_Extent(&ipE_CityMesh))) {
				cout << (*aMLItr).eMeshCode << ":都市地図メッシュのエンベロープ取得に失敗" << endl;
				return false;
			}

			// メッシュコード，完全／不完全フラグの取得
			CComVariant	aVar_MeshCode, aVar_CompleteFlag;
			if (FAILED(ipF_CityMesh->get_Value(mFID_CityMeshCode, &aVar_MeshCode)) ||
				FAILED(ipF_CityMesh->get_Value(mFID_CompleteFlag, &aVar_CompleteFlag))) {
					cout << (*aMLItr).eMeshCode << ":メッシュコード or 完全/不完全フラグの取得に失敗" << endl;
					return false;
			}

			// カレントディレクトリ設定
			sprintf_s(aCurDirName, _MAX_DIR, "%s\\%4d", mOutDir.c_str(), aVar_MeshCode.lVal / 10000);
			if(_chdir(aCurDirName) == -1) {
				cout << "ディレクトリ新規作成: " << aCurDirName << " ..." << endl;
				if(_mkdir(aCurDirName) == -1) {
					cout << "作成できません" << endl;
					return false;
				}
				cout << "完了" << endl;
				if(_chdir(aCurDirName) == -1) {
					cout << "カレントディレクトリを変更できません" << endl;
					return false;
				}
			}

			// existファイルオープン
			sprintf_s(aFileName, _MAX_FNAME, "%8ld.exi", aVar_MeshCode.lVal);
			cout << aFileName << " 作成中..." << flush;
			fp = fopen(aFileName, "wb");
			if(fp == NULL) {
				cout << "オープンできません" << endl;
				return false;
			}

			// 不完全ポリゴン取得
			IPolygonPtr		ipPoly_Incomplete;
			long	aIncompCount;
			ipPoly_Incomplete = fnCreateIncompletePoly(ipE_CityMesh, &aIncompCount);
			if(aIncompCount < 0) {			// 背景ポリゴンが１つも無い
				ExistError = true;
				continue;
			} else if(aIncompCount == 0) {	// 不完全エリアが無い
				// ヘッダ書き出し
				fprintf(fp, "%8ld\tO\t%d\t%d\n", aVar_MeshCode.lVal, mDivNum, mDivNum);

				// データ書き出し
				for(i = 0; i < mDivNum; i++) {
					fprintf(fp, "%s\n", aExistLine.c_str());
				}
			} else {

				// ヘッダ書き出し
				fprintf(fp, "%8ld\tV\t%d\t%d\n", aVar_MeshCode.lVal, mDivNum, mDivNum);

				// 都市地図メッシュの四隅座標取得
				double	aXMin, aXMax, aYMin, aYMax, aWidth, aHeight;
				if (FAILED(ipE_CityMesh->get_XMin(&aXMin)) ||
					FAILED(ipE_CityMesh->get_XMax(&aXMax)) ||
					FAILED(ipE_CityMesh->get_YMin(&aYMin)) ||
					FAILED(ipE_CityMesh->get_YMax(&aYMax)) ||
					FAILED(ipE_CityMesh->get_Width(&aWidth)) ||
					FAILED(ipE_CityMesh->get_Height(&aHeight))) {
						cout << (*aMLItr).eMeshCode << ":メッシュ座標取得失敗" << endl;
						fclose(fp);
						return false;
				}

				ISpatialReferencePtr	ipSpRef;
				ipE_CityMesh->get_SpatialReference ( &ipSpRef );

				// 完全／不完全判定・データ書き出し（セルのループ）
				for(i = 0; i < mDivNum; i++) {

					for(j = 0; j < mDivNum; j++) {

						/**** セルの設定・開始 ****/

						// セルの座標計算
						double	aX_Cell[2], aY_Cell[2];
						aX_Cell[0]	= aXMin + aWidth	* j			/ mDivNum;	// XMin
						aX_Cell[1]	= aXMin + aWidth	* (j + 1)	/ mDivNum;	// XMax
						aY_Cell[0]	= aYMax - aHeight	* (i + 1)	/ mDivNum;	// YMin
						aY_Cell[1]	= aYMax - aHeight	* i			/ mDivNum;	// YMax

						// 点列 → Ring
						IRingPtr			ipR_Cell(CLSID_Ring);
						IPointCollectionPtr	ipPtCol_Cell(ipR_Cell);
						for(int n = 0; n < 5; n++) {
							IPointPtr	ipPoint(CLSID_Point);
							if (FAILED(ipPoint->put_X(aX_Cell[(int)((n + 1)/ 2) % 2])) ||
								FAILED(ipPoint->put_Y(aY_Cell[(int)((n + 2)/ 2) % 2]))) {
									cout << "セルの座標付与に失敗" << endl;
									fclose(fp);
									return false;
							}
							if (FAILED(ipPtCol_Cell->AddPoint(ipPoint))) {
								cout << "セルの点追加に失敗" << endl;
								fclose(fp);
								return false;
							}
						}

						// Ring → Polygon
						IPolygonPtr				ipPgn_Cell(CLSID_Polygon);
						IGeometryCollectionPtr	ipGCol_Cell(ipPgn_Cell);
						if (FAILED(ipGCol_Cell->AddGeometry(ipR_Cell))) {
							cout << "セルの追加に失敗" << endl;
							fclose(fp);
							return false;
						}

						if (FAILED(ipPgn_Cell->putref_SpatialReference ( ipSpRef ))) {
							cout << "SpatialReference適用に失敗" << endl;
							fclose(fp);
							return false;
						}

						/**** セルの設定・終了 ****/

						IRelationalOperatorPtr	ipROp_Cell(ipPgn_Cell);
						VARIANT_BOOL	aWithin, aOverlap;

						if(FAILED(ipROp_Cell->Within(ipPoly_Incomplete, &aWithin))) {
							cerr << "Within 失敗: " << (*aMLItr).eMeshCode << " (" << setw(3) << j << ", " << setw(3) << i << ")" << endl;
							ExistError = true;
							aWithin = VARIANT_FALSE;
						}
						if(aWithin) {
							fprintf(fp, "x");
							continue;
						}

						if(FAILED(ipROp_Cell->Overlaps(ipPoly_Incomplete, &aOverlap))) {
							cerr << "Overlaps 失敗: " << (*aMLItr).eMeshCode << " (" << setw(3) << j << ", " << setw(3) << i << ")" << endl;
							ExistError = true;
							aOverlap = VARIANT_FALSE;
						}
						if(aOverlap) {
							fprintf(fp, "v");
							continue;
						}

						fprintf(fp, "o");

					}

					fprintf(fp, "\n");

				}

			}
		//	a_gGeoToShp.OuttoShapeFile ( (IGeometryPtr)ipPoly_Incomplete );
			cout << "完了" << endl;

			fclose(fp);

		}

	}
	if (ExistError)
		return false;
	else
		return true;

}
