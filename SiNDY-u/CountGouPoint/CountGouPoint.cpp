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

// CountGouPoint.cpp: CountGouPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CountGouPoint.h"

char *strcpy(char *cDest, CComVariant *cSrc)
{

	USES_CONVERSION;
	return strcpy(cDest, OLE2T(cSrc->bstrVal));

}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CountGouPoint::CountGouPoint()
{

	char	*aTmpEV;

	mArgv			= NULL;

	mCityCode		= NULL;
	mFNCityList		= NULL;
	mFNOutput		= NULL;

	mFPOutput		= NULL;

	mMode			= kNone;
	m_iExeMode		= 0; 

#ifdef _DEBUG
//	_putenv("DB_ADDRESS=sindy2009a/sindy2009a/SDE.Current09A/5151/topaz2");
//	_putenv("DB_ADDRESS=beta9/beta9/beta9.B9_200305/5151/geo-as");
	_putenv("DB_ADDRESS=ARC93/ARC93/SDE.SNP_ARC93_10M/5151/despina");
#endif

	mDBCon_Address	= getenv("DB_ADDRESS");

	strcpy(mFCGouPoint,		FC_GOU_POINT);
	strcpy(mFCCityAdmin,	FC_CITY_ADMIN);
	strcpy(mFCBaseAdmin,	FC_BASE_ADMIN);

	if((aTmpEV = getenv("FC_GOU_POINT"		)) != NULL)	strcpy(mFCGouPoint,		aTmpEV);
	if((aTmpEV = getenv("FC_CITY_ADMIN"		)) != NULL)	strcpy(mFCCityAdmin,	aTmpEV);
	if((aTmpEV = getenv("FC_BASE_ADMIN"		)) != NULL)	strcpy(mFCBaseAdmin,	aTmpEV);

}

CountGouPoint::~CountGouPoint()
{

}

bool CountGouPoint::
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

		case 'a':
			mMode = kAllCity;
			break;

		case 'c':
			i++;
			mMode = kSingleCity;
			if(i < mArgc) mCityCode = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case 'l':
			i++;
			mMode = kCitiesInList;
			if(i < mArgc) mFNCityList = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case 'o':
			i++;
			if(i < mArgc) mFNOutput = mArgv[i];
			else {
				fprintf(stderr, "引数が足りません\n");
				return false;
			}
			break;

		case 'e':
			m_iExeMode |= ON_ElevenCode;
			break;

		case 'f':
			m_iExeMode |= ON_FiveCode;
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

	if(mFNOutput == NULL) {
		fprintf(stderr, "出力ファイル名が指定されていません\n");
		return false;
	}
	if(mMode == kNone) {
		fprintf(stderr, "オプション -a / -c / -l のいずれかが必要です\n");
		return false;
	} else if(mCityCode != NULL && strspn(mCityCode, "1234567890") != strlen(mCityCode)) {
		fprintf(stderr, "住所コードが不正です\n");
		return false;
	}

	if( m_iExeMode == 0 )
	{
		fprintf(stderr, "実行モードが指定されておりません(-e or -f)\n");
		return false;
	}
	else if( m_iExeMode & ON_ElevenCode && m_iExeMode & ON_FiveCode )
	{
		fprintf(stderr, "実行モードが複数指定されております(-e or -f)\n");
		return false;
	}

	if( m_iExeMode & ON_ElevenCode && mMode == kAllCity )
	{
		fprintf(stderr, "全国分をカウントする際は -e を指定してください\n");
		return false;
	}

	if( mCityCode != NULL && ((m_iExeMode & ON_ElevenCode && strlen(mCityCode) < 11) || (m_iExeMode & ON_FiveCode && strlen(mCityCode) < 5) ))
	{
		fprintf(stderr, "住所コードの桁数が不正です\n");
		return false;
	}

	return true;

}

void CountGouPoint::
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
		"\t・号ポイントの件数を５桁コードごとにカウントする\n\n"
		"【使用形式】\n\n"
		"\t%s <オプション>\n\n"
		"【オプション】\n\n"
		"\t-a\n"
		"\t\t全国分についてカウントする\n\n"
		"\t-c <市区町村コード>\n"
		"\t\t指定したコードの市区町村についてカウントする\n\n"
		"\t-l <ファイル名>\n"
		"\t\t市区町村コードリストの分についてカウントする\n\n"
		"\t-o <ファイル名>\n"
		"\t\t出力先ファイル名を指定\n\n"
		"\t-?\n"
		"\t\t使用法（このテキスト）を表示\n\n"
		"【環境変数】\n\n"
		"\tDB_ADDRESS\n"
		"\t\tGeoDB接続文字列（住所系サーバ）を指定\n"
		"\t\t形式: <user>/<password>/<version>/<instance>/<server>\n\n"
		"\tFC_GOU_POINT\n"
		"\t\t号ポイントレイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_CITY_ADMIN\n"
		"\t\t都市地図行政界レイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n"
		"\tFC_BASE_ADMIN\n"
		"\t\t中縮行政界レイヤのフィーチャクラス名\n"
		"\t\t（デフォルト: %s）\n\n",
		aExecFN, aExecFN, FC_GOU_POINT, FC_CITY_ADMIN, FC_BASE_ADMIN);

}

bool CountGouPoint::
fnMakeCityList(void)
{

	switch(mMode) {

	case kAllCity:

		{

//			char			aWhere[128];
			char			aSubField[128];
			_ICursorPtr		ipCur_CityAdmin;
			IQueryDefPtr	ipQDef_CityAdmin;

			sprintf(aSubField, "distinct(%s)", FD_CITYCODE);
//			sprintf(aWhere, "%s > '00000'", FD_CITYCODE);
			((IFeatureWorkspacePtr)ipWorkspace[mDBID_Address])->CreateQueryDef(&ipQDef_CityAdmin);
			ipQDef_CityAdmin->put_Tables(CComBSTR(mFCCityAdmin));
			ipQDef_CityAdmin->put_SubFields(CComBSTR(aSubField));
//			ipQDef_CityAdmin->put_WhereClause(CComBSTR(aWhere));
			ipQDef_CityAdmin->Evaluate(&ipCur_CityAdmin);

			_IRowPtr		ipR_CityAdmin;
			while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
				CComVariant		aVar_CityCode;
				char			aCityCode[6];
				ipR_CityAdmin->get_Value(0, &aVar_CityCode);
				strcpy(aCityCode, &aVar_CityCode);
				if(!mCityList.Add(aCityCode)) return false;
			}

		}

		break;

	case kCitiesInList:
		if(!mCityList.Open(mFNCityList)) return false;
		break;

	case kSingleCity:
		if(!mCityList.Add(mCityCode)) return false;
		break;

	default:
		break;

	}

	return true;

}

#ifdef _USE_SPCACHE_

bool CountGouPoint::
fnCreateCache(const char *cCityCode)
{

	fprintf(stderr, "%s: キャッシュ作成中...", cCityCode);

	// 中縮行政界ポリゴンの取得
	char	aWhere[512];
	long	aBACount;
	IFeatureCursorPtr	ipFCur_BaseAdmin;
	sprintf(aWhere, "%s = '%s'", FD_CITYCODE, cCityCode);
	ipFCur_BaseAdmin = SearchByWhereClause(aWhere, ipFC_BaseAdmin, &aBACount);

	if(aBACount == 0) {

		fprintf(stderr, "対応する中縮行政界ポリゴンがありません\n");
		return false;

	} else {

		double			aCacheArea, aMaxArea = 0;
		IFeaturePtr		ipF_BaseAdmin;
		IEnvelopePtr	ipE_BaseAdmin, ipE_ForCache;
		while(ipFCur_BaseAdmin->NextFeature(&ipF_BaseAdmin) == S_OK) {
			ipF_BaseAdmin->get_Extent(&ipE_BaseAdmin);
			((IAreaPtr)ipE_BaseAdmin)->get_Area(&aCacheArea);
			if(aCacheArea > aMaxArea) {
				aMaxArea = aCacheArea;
				ipE_ForCache = ipE_BaseAdmin;
			}
		}

		if(aMaxArea > 1) {
			fprintf(stderr, "エリアが巨大なのでキャッシュを作成しません\n");
			return false;
		} else {
			ipSpCacheMgr->FillCache(ipE_ForCache);
			fprintf(stderr, "完了\n");
		}

	}

	return true;

}

bool CountGouPoint::
fnCreateCache(IGeometryPtr ipG_TgtArea)
{

	VARIANT_BOOL	aIsFull;
	ipSpCacheMgr->get_CacheIsFull(&aIsFull);
	if(aIsFull) return true;	// すでにキャッシュが作成されている場合は抜ける

	IEnvelopePtr	ipE_TgtArea;
	ipG_TgtArea->get_Envelope(&ipE_TgtArea);
	ipSpCacheMgr->FillCache(ipE_TgtArea);

	return true;

}

#endif

bool CountGouPoint::
Execute(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnAnalyzeArg()) return false;

	if(mDBCon_Address == NULL) {
		fprintf(stderr, "環境変数 DB_ADDRESS が設定されていません\n");
		return false;
	}

	// 接続
	mDBID_Address	= Connect(_T(mDBCon_Address));	if(mDBID_Address	< 0) return false;

	// フィーチャクラスの取得
	ipFC_GouPoint	= GetFeatureClass(mDBID_Address,	mFCGouPoint);
	if(ipFC_GouPoint	== NULL)	return false;
	ipFC_CityAdmin	= GetFeatureClass(mDBID_Address,	mFCCityAdmin);
	if(ipFC_CityAdmin	== NULL)	return false;
	ipFC_BaseAdmin	= GetFeatureClass(mDBID_Address,	mFCBaseAdmin);
	if(ipFC_BaseAdmin	== NULL)	return false;

	// フィールド・インデックスの取得
	if(!GetFieldID(ipFC_GouPoint,	_T(FD_GOUTYPE),			&mFID_GouType))		return false;
	if(!GetFieldID(ipFC_CityAdmin,	_T(FD_CITYCODE),		&mFID_CityCode))	return false;
	if(!GetFieldID(ipFC_CityAdmin,	_T(FD_ADDRCODE),		&mFID_AddrCode))	return false;

	// 5桁リストor11桁リストを作成
	if(!fnMakeCityList()) {
		fprintf(stderr, "市区町村リストを生成できません\n");
		return false;
	}

	// 出力ファイルのオープン
	mFPOutput = fopen(mFNOutput, "w");
	if(mFPOutput == NULL) {
		fprintf(stderr, "ファイルを開けません: %s\n", mFNOutput);
		return false;
	}

	ipSpCacheMgr	= ipWorkspace[mDBID_Address];

	int aTotalGouCount = 0, aTotalTpgCount = 0, aTotalTotalCount = 0, aTotalAdmCount = 0;

	CityListSet_i	aCLItr;
	
	// 5桁実行モード
	if( m_iExeMode & ON_FiveCode )
	{
		// 5桁リストのループ
		for(aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			GCSet	aGCSet;

			fprintf(stderr, "%s 処理中 ...\n", (*aCLItr).eCityCode);

#ifdef _USE_SPCACHE_
			// キャッシュ作成
			fnCreateCache((*aCLItr).eCityCode);
#endif

			char	aWhere2[512];
			long	aCACount;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			sprintf(aWhere2, "%s = '%s'", FD_CITYCODE, (*aCLItr).eCityCode);
			ipFCur_CityAdmin = SearchByWhereClause(aWhere2, ipFC_CityAdmin, &aCACount);

			if(aCACount == 0) {
				fprintf(stderr, "%s: この市区町村コードを持つ都市地図行政界ポリゴンがありません\n", (*aCLItr).eCityCode);
				continue;
			}

			long			i = 0;

			fprintf(stderr, "号ポイントカウント中 ...\n");

			// 行政界ポリゴンのループ
			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

				int		aGouCount	= 0;
				int		aTpgCount	= 0;

				i++;
				fprintf(stderr, "\r%8ld / %8ld", i, aCACount);

	//			long	aOID_CityAdmin;
	//			ipF_CityAdmin->get_OID(&aOID_CityAdmin);
				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

#ifdef _USE_SPCACHE_
				// キャッシュ作成（中縮行政界単位のものが作成されていない場合のみ）
				fnCreateCache(ipG_CityAdmin);
#endif

				CComVariant		aVar_AddrCode;
				char			aAddrCode[12];
				sprintf(aAddrCode, "%s", (*aCLItr).eCityCode);
				ipF_CityAdmin->get_Value(mFID_AddrCode, &aVar_AddrCode);
				strcpy(&aAddrCode[5], &aVar_AddrCode);

				long	aGPCount;
				IFeatureCursorPtr	ipFCur_GouPoint;
				ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, ipFC_GouPoint, &aGPCount);

				if(aGPCount > 0) {
					// 号ポイントのループ
					IFeaturePtr	ipF_GouPoint;
					while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
						CComVariant	aVar_GouType;
						ipF_GouPoint->get_Value(mFID_GouType, &aVar_GouType);
						if(aVar_GouType.lVal == 1)	aGouCount++;
						else						aTpgCount++;
					}
				}

				GouCount	aGC(aAddrCode, aGouCount, aTpgCount, 1);
				GCSet::iterator	aGCItr = aGCSet.FindCode(aAddrCode);
				if(aGCItr == aGCSet.end()) {	// 見つからない
					aGCSet.insert(aGC);
				} else {						// 見つかった
					aGC.eGouCount += (*aGCItr).eGouCount;
					aGC.eTpgCount += (*aGCItr).eTpgCount;
					aGC.eAdmCount += (*aGCItr).eAdmCount;
					aGCSet.erase(aGCItr);
					aGCSet.insert(aGC);
				}

			}

#ifdef _USE_SPCACHE_
			ipSpCacheMgr->EmptyCache();
#endif

			fprintf(stderr, "\n完了\n");

			// ログ出力をタブ区切りにし、合計のまえに分かりやすい記号をいれる
			for(GCSet::iterator aItr = aGCSet.begin(); aItr != aGCSet.end(); aItr++) {
				int		aTotalCount	= (*aItr).eGouCount + (*aItr).eTpgCount;
				//fprintf(mFPOutput, "%s, %7ld, %7ld, %7ld, %4ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				fprintf(mFPOutput, "%s,\t%ld,\t%ld,\t%ld,\t%ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				aTotalGouCount += (*aItr).eGouCount;
				aTotalTpgCount += (*aItr).eTpgCount;
				aTotalAdmCount += (*aItr).eAdmCount;
			}
			fflush(mFPOutput);
		}
	}

	// 11桁実行モード
	else if( m_iExeMode & ON_ElevenCode )
	{
		// 11桁リストのループ
		for(aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			GCSet	aGCSet;

			fprintf(stderr, "%s%s 処理中 ...\n", (*aCLItr).eCityCode, (*aCLItr).eAddrCode);

			char	aWhere2[512];
			long	aCACount;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			sprintf(aWhere2, "%s = '%s' AND %s = '%s'", FD_CITYCODE, (*aCLItr).eCityCode, FD_ADDRCODE, (*aCLItr).eAddrCode);
			ipFCur_CityAdmin = SearchByWhereClause(aWhere2, ipFC_CityAdmin, &aCACount);

			if(aCACount == 0) {
				fprintf(stderr, "%s%s: この11桁コードを持つ都市地図行政界ポリゴンがありません\n", (*aCLItr).eCityCode, (*aCLItr).eAddrCode);
				continue;
			}

			long			i = 0;

			fprintf(stderr, "号ポイントカウント中 ...\n");

			// 行政界ポリゴンのループ
			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

				int		aGouCount	= 0;
				int		aTpgCount	= 0;

				i++;
				fprintf(stderr, "\r%8ld / %8ld", i, aCACount);


				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

#ifdef _USE_SPCACHE_
				// キャッシュ作成
				fnCreateCache(ipG_CityAdmin);
#endif

				CComVariant		aVar_AddrCode;
				char			aAddrCode[12];
				sprintf(aAddrCode, "%s", (*aCLItr).eCityCode);
				strcpy(&aAddrCode[5], (*aCLItr).eAddrCode);

				long	aGPCount;
				IFeatureCursorPtr	ipFCur_GouPoint;
				ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, ipFC_GouPoint, &aGPCount);

				if(aGPCount > 0) {
					// 号ポイントのループ
					IFeaturePtr	ipF_GouPoint;
					while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
						CComVariant	aVar_GouType;
						ipF_GouPoint->get_Value(mFID_GouType, &aVar_GouType);
						if(aVar_GouType.lVal == 1)	aGouCount++;
						else						aTpgCount++;
					}
				}

				GouCount	aGC(aAddrCode, aGouCount, aTpgCount, 1);
				GCSet::iterator	aGCItr = aGCSet.FindCode(aAddrCode);
				if(aGCItr == aGCSet.end()) {	// 見つからない
					aGCSet.insert(aGC);
				} else {						// 見つかった
					aGC.eGouCount += (*aGCItr).eGouCount;
					aGC.eTpgCount += (*aGCItr).eTpgCount;
					aGC.eAdmCount += (*aGCItr).eAdmCount;
					aGCSet.erase(aGCItr);
					aGCSet.insert(aGC);
				}

			}

#ifdef _USE_SPCACHE_
			ipSpCacheMgr->EmptyCache();
#endif

			fprintf(stderr, "\n完了\n");
			
			// ログ出力をタブ区切りにし、合計のまえに分かりやすい記号をいれる
			for(GCSet::iterator aItr = aGCSet.begin(); aItr != aGCSet.end(); aItr++) {
				int		aTotalCount	= (*aItr).eGouCount + (*aItr).eTpgCount;
				fprintf(mFPOutput, "%s,\t%ld,\t%ld,\t%ld,\t%ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				aTotalGouCount += (*aItr).eGouCount;
				aTotalTpgCount += (*aItr).eTpgCount;
				aTotalAdmCount += (*aItr).eAdmCount;
			}
			fflush(mFPOutput);
		}
	}

	// 合計算出
	aTotalTotalCount = aTotalGouCount + aTotalTpgCount;
	fprintf(mFPOutput, "--------------------------------------------------\n");
	fprintf(mFPOutput, "Total Count,\t%ld,\t%ld,\t%ld,\t%ld\n", aTotalGouCount, aTotalTpgCount, aTotalTotalCount, aTotalAdmCount);

	fclose(mFPOutput);

	return true;

}
