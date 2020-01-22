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

// CheckAdminMatch.cpp: CheckAdminMatch クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CheckAdminMatch.h"
#include <atlstr.h>
#include <WinLib/VersionInfo.h>
char	gMessage[1024];

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CheckAdminMatch::CheckAdminMatch()
{

	mDBCon_Address	= NULL;

	mFC_CityAdmin	= NULL;
	mFC_BaseAdmin	= NULL;

	mFN_ChkLog		= NULL;

	mFN_CityList	= NULL;
	mCityCode		= NULL;

	mRangeMin		= -1;
	mRangeMax		= -1;

	mDist1			= 0;		// Base_Admin - City_Admin
	mDist2			= 0;		// City_Admin - City_Admin ８桁
	mDist3			= 0;		// City_Admin - City_Admin 11桁
	mDist4			= 0;		// City_Admin - City_Admin 街区符号界

	mOID_BaseAdmin	= 0;

	mCLType			= kCLNone;

	m_iExeMode		= 0;

}

// デストラクタ
CheckAdminMatch::~CheckAdminMatch()
{

}

// Usageメッセージ表示
void	CheckAdminMatch::printUsage()
{
	cerr
		<< "【行政界整合チェックツール】\n"
		<< "(Usage)CheckAdminMatch.exe [option]\n"
		<< "・チェック対象オプション\n"
		<< " -ca                全件チェック\n"
		<< " -cl [listfile]     リスト指定（市区町村リスト）\n"
		<< " -cs [code]         単一指定（市区町村コード）\n"
		<< " -cr [code-code]    エリア指定（市区町村コード-市区町村コード）\n"
		<< " -o  [filename]     ログ出力ファイル名指定\n"
		<< "・実行モードについて\n"
		<< " -f                 5桁コード実行モード(-ca, -crを指定したときはこちら)\n"
		<< " -e                 11桁コード実行モード\n"
		<< "\n"
		<< "・環境変数について\n"
		<< " DB_ADDRESS         接続先SDE\n"
		<< " FC_CITY_ADMIN      都市地図行政界フィーチャクラス\n"
		<< " FC_BASE_ADMIN      ベース行政界フィーチャクラス\n"
		<< " JDB_CONNECT		住所DB接続先\n"
		<< " PGDB_OUT           分散ポリゴン作成先PGDBファイル名\n"
		<< " FC_OUT             分散ポリゴン作成先フィーチャクラス名\n"
		<< endl;
}

// 環境変数取得
bool CheckAdminMatch::
fnGetEnvVar()
{

	mDBCon_Address	= getenv("DB_ADDRESS");
	if( mDBCon_Address == NULL ) {
		cerr << "環境変数 DB_ADDRESS が設定されていません" << endl;
		return false;
	}

	mPGDB_OUT		= getenv("PGDB_OUT");
	if( mPGDB_OUT == NULL )
	{
		cerr << "環境変数 PGDB_OUT が設定されていません" << endl;
		return false;
	}

	mFC_OUT			= getenv("FC_OUT");
	if( mFC_OUT == NULL )
	{
		cerr << "環境変数 FC_OUT が設定されていません" << endl;
		return false;
	}

	mFC_CityAdmin	= getenv("FC_CITY_ADMIN");
	if( mFC_CityAdmin == NULL ) {
		cerr << "環境変数 FC_CITY_ADMIN が設定されていません" << endl;
		return false;
	}

	mFC_BaseAdmin	= getenv("FC_BASE_ADMIN");
	if( mFC_BaseAdmin == NULL ) {
		cerr << "環境変数 FC_BASE_ADMIN が設定されていません" << endl;
		return false;
	}

	return true;

}

// 引数解析
bool CheckAdminMatch::
fnAnalyzeArg()
{

	int		i;
	char	*pRange, *pHyphen;

	if(mArgc < 2) {
		cerr << "引数が足りません" << endl;
		return false;
	}

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {
		switch(mArgv[i][1]) {
		case 'c':
			switch(mArgv[i][2]) {
			case 'a':
				mCLType	= kCLAll;
				break;
			case 'l':
				mCLType	= kCLList;
				mFN_CityList	= mArgv[++i];
				break;
			case 's':
				mCLType	= kCLSingle;
				mCityCode	= mArgv[++i];
				break;
			case 'r':
				mCLType	= kCLRange;
				pRange	= mArgv[++i];
				mRangeMin	= atol(pRange);
				pHyphen	= strchr(pRange, '-');
				if(pHyphen != NULL) {
					mRangeMax	= atol(++pHyphen);
				}
				break;
			default:
				cerr << "不正なオプションです: " << mArgv[i] << endl;
				return false;
			}
			break;
		case 'o':
			mFN_ChkLog	= mArgv[++i];
			break;
		case 'd':
			switch(mArgv[i][2]) {
			case '1':
				mDist1	= atof(mArgv[++i]);
				break;
			case '2':
				mDist2	= atof(mArgv[++i]);
				break;
			case '3':
				mDist3	= atof(mArgv[++i]);
				break;
			case '4':
				mDist4	= atof(mArgv[++i]);
				break;
			default:
				cerr << "不正なオプションです: " << mArgv[i] << endl;
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
			return false;
		default:
			cerr << "不正なオプションです: " << mArgv[i] << endl;
			return false;
		}
	}

	if( m_iExeMode == 0 )
	{
		cerr << "実行モードが指定されておりません(-e or -f)" << endl;
		return false;
	}
	else if( m_iExeMode & ON_ElevenCode && m_iExeMode & ON_FiveCode )
	{
		cerr << "実行モードが複数指定されております(-e or -f)" << endl;
		return false;
	}

	switch(mCLType) {
	case kCLNone:
		cerr << "オプション -c* が指定されていません" << endl;
		return false;
	case kCLAll:
		if( m_iExeMode & ON_ElevenCode )
		{
			cerr << "全国チェックの際は -f(5桁モード) を指定してください" << endl;
			return false;
		}
		break;
	case kCLList:
		if(mFN_CityList == NULL) {
			cerr << "オプション -cl で市区町村リストファイルが指定されていません" << endl;
			return false;
		}
		break;
	case kCLSingle:
		if(mCityCode == NULL) {
			cerr << "オプション -cs で市区町村コードが指定されていません" << endl;
			return false;
		}
		if( mCityCode != NULL && ((m_iExeMode & ON_ElevenCode && strlen(mCityCode) < 11) || (m_iExeMode & ON_FiveCode && strlen(mCityCode) < 5) ))
		{
			cerr << "住所コードの桁数が不正です" << endl;
			return false;
		}
		break;
	case kCLRange:
		if(mRangeMin < 0 || mRangeMax < 0) {
			cerr << "オプション -cr で市区町村コードの範囲が正しく指定されていません" << endl;
			return false;
		}
		if( m_iExeMode & ON_ElevenCode )
		{
			cerr << "範囲チェックの際は -f(5桁モード) を指定してください" << endl;
			return false;
		}
		break;
	}

	if(mFN_ChkLog == NULL) {
		cerr << "チェックログファイル名が指定されていません" << endl;
		return false;
	}

	return true;

}

// 出力用PGDB作成
bool CheckAdminMatch::
fnCreateTmpFC()
{
	// [Bug 6193]対応 PGDB作成 存在する場合は既存のものを開く
	IWorkspaceFactoryPtr	ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr		ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( CComBSTR(mPGDB_OUT), NULL, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		std::string		temp = mPGDB_OUT;

		int				iPos = temp.rfind( "\\" );
		std::string		temp_Dir, temp_File;
		if( iPos != -1 )
		{
			temp_Dir	= temp.substr( 0, iPos );
			temp_File	= temp.substr( iPos );
		}
		else
		{
			temp_Dir	= ".";
			temp_File	= temp;
		}
		IPropertySetPtr		ipPropertySet( CLSID_PropertySet );
		IWorkspaceNamePtr	ipWorkspaceName;
		if( ipWorkspaceFactory->Create( CComBSTR( temp_Dir.c_str() ), CComBSTR( temp_File.c_str() ), ipPropertySet, NULL, &ipWorkspaceName ) != S_OK )
		{
			cout << "#PGDBファイルの作成に失敗！" << endl;
			return false;
		}
		if( ipWorkspaceName )
		{
			IUnknownPtr		ipUnknown;
			( (INamePtr)ipWorkspaceName )->Open( &ipUnknown );
			ipWorkspace = ipUnknown;
		}
		else
			return false;
	}


	IFieldsPtr	ipFlds_TmpFC(CLSID_Fields);
	IFieldPtr	ipFld_Shape(CLSID_Field), ipFld_OID(CLSID_Field), ipFld_CityCode(CLSID_Field), ipFld_AddrCode(CLSID_Field);
	IGeometryDefPtr	ipGD_TmpFC(CLSID_GeometryDef);
	ISpatialReferencePtr	ipSR_TmpFC;
	IUIDPtr		ipUID_ClsID, ipUID_ExtClsID;

	ipFC_CityAdmin->get_CLSID(&ipUID_ClsID);
	ipFC_CityAdmin->get_EXTCLSID(&ipUID_ExtClsID);
	((IGeoDatasetPtr)ipFC_CityAdmin)->get_SpatialReference(&ipSR_TmpFC);

	// GeometryDef 設定
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GeometryType(esriGeometryPolygon);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridCount(1);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridSize(0, 0.1);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_HasM(VARIANT_FALSE);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_HasZ(VARIANT_FALSE);
	((IGeometryDefEditPtr)ipGD_TmpFC)->putref_SpatialReference(ipSR_TmpFC);

	// SHAPE フィールド
	((IFieldEditPtr)ipFld_Shape)->put_Name(CComBSTR("SHAPE"));
	((IFieldEditPtr)ipFld_Shape)->put_Type(esriFieldTypeGeometry);
	((IFieldEditPtr)ipFld_Shape)->putref_GeometryDef(ipGD_TmpFC);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_Shape);

	// ObjectID フィールド
	((IFieldEditPtr)ipFld_OID)->put_Name(CComBSTR("OBJECTID"));
	((IFieldEditPtr)ipFld_OID)->put_Type(esriFieldTypeOID);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_OID);

	// AddrCode フィールド
	((IFieldEditPtr)ipFld_AddrCode)->put_Name(CComBSTR("AddrCode"));
	((IFieldEditPtr)ipFld_AddrCode)->put_Type(esriFieldTypeString);
	((IFieldEditPtr)ipFld_AddrCode)->put_Length(11);
	((IFieldEditPtr)ipFld_AddrCode)->put_IsNullable(VARIANT_FALSE);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_AddrCode);

	mFC_TmpPoly = mFC_OUT;	// [bug 9651]対応で修正

	// フィーチャクラス作成
	((IFeatureWorkspacePtr)ipWorkspace)->CreateFeatureClass( CComBSTR(mFC_OUT), ipFlds_TmpFC, ipUID_ClsID, ipUID_ExtClsID, esriFTSimple, CComBSTR("SHAPE"), NULL, &ipFC_TmpPoly );
	if( ipFC_TmpPoly == NULL )
	{
		cerr << "フィーチャクラス " << mFC_TmpPoly << " の作成に失敗しました" << endl;
		return false;
	}

	// フィールドインデックス取得
	if(!GetFieldID(ipFC_TmpPoly,		FD_ADDR_CODE,			&mFID_AddrCode_TP))		return false;

	return true;

}

// 実行5桁or11桁リスト作成
bool CheckAdminMatch::
fnMakeCityList()
{

	IFeatureCursorPtr	ipFCur_CityAdmin;
	IFeaturePtr			ipF_CityAdmin;
	_ICursorPtr		ipCur_CityAdmin;
	IQueryDefPtr	ipQDef_CityAdmin;
	_IRowPtr			ipR_CityAdmin;
	char	aWhere[128], aRange[64], aSubField[64];
	string	aTmpStr;
	sprintf(aWhere, "%s > '00000'", FD_CITY_CODE);
	sprintf(aSubField, "distinct(%s)", FD_CITY_CODE);

	switch(mCLType) {
	case kCLRange:
		sprintf(aRange, " and %s between '%05d' and '%05d'", FD_CITY_CODE, mRangeMin, mRangeMax);
		strcat(aWhere, aRange);
	case kCLAll:
		((IFeatureWorkspacePtr)ipWorkspace[mDBID_Address])->CreateQueryDef(&ipQDef_CityAdmin);
		ipQDef_CityAdmin->put_Tables(CComBSTR(mFC_CityAdmin));
		ipQDef_CityAdmin->put_SubFields(CComBSTR(aSubField));
		ipQDef_CityAdmin->put_WhereClause(CComBSTR(aWhere));
		ipQDef_CityAdmin->Evaluate(&ipCur_CityAdmin);
		while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
			CComVariant		aVar_CityCode;
			ipR_CityAdmin->get_Value(0, &aVar_CityCode);
			aTmpStr	= ExtractStr(aVar_CityCode);
			if(!mCityList.Add(aTmpStr.c_str())) return false;
		}
		break;
	case kCLList:
		if(!mCityList.Open(mFN_CityList)) return false;
		break;
	case kCLSingle:
		if(!mCityList.Add(mCityCode)) return false;
		break;
	}

	return true;

}

// 中縮行政界取得
void CheckAdminMatch::
fnSetBaseAdminPoly(const char *cCityCode)
{

	char	aWhere[128];
	sprintf(aWhere, "CityCode = '%s'", cCityCode);

	IPolygonPtr	ipP_Result(CLSID_Polygon);

	long	aCount;
	IFeatureCursorPtr	ipFCur_BaseAdmin;
	ipFCur_BaseAdmin = SearchByWhereClause(aWhere, ipFC_BaseAdmin, &aCount);

	if(aCount < 1) {

		ipP_BaseAdmin->SetEmpty();
		mOID_BaseAdmin	= 0;

		// 住所コードから住所名称取得（Bug8610対応）
		CString	AddrKanji;
		mJDBAccess.CodeToKanji( cCityCode, AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

		cerr << cCityCode << ": 中縮行政界ポリゴン(Base_Admin)がありません" << endl;
		sprintf(gMessage, "0\t\t\t中縮行政界がない\t%s\t\t\t%s\t\t", cCityCode,
			AddrKanji );
		mChkLog.OutputDirect(gMessage);

	} else {

		IFeaturePtr	ipF_BaseAdmin;
		double	aArea, aAreaMax = 0;
		while(ipFCur_BaseAdmin->NextFeature(&ipF_BaseAdmin) == S_OK) {

			long	aOID;
			ipF_BaseAdmin->get_OID(&aOID);

			IGeometryPtr	ipG_BaseAdmin;
			IGeometryPtr	ipG_Union;
			ipF_BaseAdmin->get_Shape(&ipG_BaseAdmin);

			((IAreaPtr)ipG_BaseAdmin)->get_Area(&aArea);
			if(aArea > aAreaMax) {
				aAreaMax		= aArea;
				mOID_BaseAdmin	= aOID;
			}

			((ITopologicalOperatorPtr)ipP_Result)->Union(ipG_BaseAdmin, &ipG_Union);
			ipP_Result	= ipG_Union;

		}
		ipP_BaseAdmin = ipP_Result;
	}

	return;

}

// 実行リスト作成
bool CheckAdminMatch::
fnMakeACList(const char *cCityCode)
{

	mACSet.clear();

	_ICursorPtr		ipCur_CityAdmin;
	char	aWhere[128];
	string	aTmpStr;

	sprintf(aWhere, FD_CITY_CODE " = '%s'", cCityCode);
	IFeatureCursorPtr	ipFCur_CityAdmin;
	long	aCount;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	if(aCount < 1) {
		cerr << cCityCode << ": 行政界ポリゴンがありません" << endl;
		return false;
	}

	IFeaturePtr	ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

		CityListCont Cont_temp;
		long	aOID_CityAdmin;
		ipF_CityAdmin->get_OID(&aOID_CityAdmin);

		// ８・11桁コードの取得・リストへの追加
		CComVariant	aTmpVar;
		string	aACStr11;
		ipF_CityAdmin->get_Value(mFID_CityCode, &aTmpVar);	aTmpStr = ExtractStr(aTmpVar);	aTmpVar.Clear();	aACStr11 = aTmpStr;
		strcpy( Cont_temp.eCityCode, aTmpStr.c_str());
		ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);	aTmpStr = ExtractStr(aTmpVar);	aTmpVar.Clear();	aACStr11 += aTmpStr;
		strcpy( Cont_temp.eAddrCode, aTmpStr.c_str());
		// 11桁モードの場合は、入力リストと照らし合わせ、無いものは実行リストに追加しない
		if( m_iExeMode & ON_ElevenCode && mCityList.find( Cont_temp ) == mCityList.end() )
			continue;
		mACSet.Add(aACStr11, aOID_CityAdmin);

		// 市区町村行政界との整合チェック
		VARIANT_BOOL	aEmpty;
		ipP_BaseAdmin->get_IsEmpty(&aEmpty);
		if(!aEmpty) {
			IGeometryPtr	ipG_CityAdmin;
			ipF_CityAdmin->get_Shape(&ipG_CityAdmin);
			VARIANT_BOOL	aDisjoint;
			((IRelationalOperatorPtr)ipP_BaseAdmin)->Disjoint(ipG_CityAdmin, &aDisjoint);
			if(aDisjoint) {
				IPointPtr	ipPt_Centroid;
				double		aX, aY;
				((IAreaPtr)ipG_CityAdmin)->get_Centroid(&ipPt_Centroid);
				ipPt_Centroid->QueryCoords(&aX, &aY);

				// 住所コードから住所名称取得（Bug8610対応）
				CString	AddrKanji;
				mJDBAccess.CodeToKanji( aACStr11.c_str(), AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

				sprintf(gMessage, "0\t%s\t%ld\t適切な中縮行政界内にない\t%s\t%16.12f\t%15.12f\t%s\t\t",
					mFC_CityAdmin, aOID_CityAdmin, aACStr11.c_str(), aX, aY,
					AddrKanji);
				mChkLog.OutputDirect(gMessage);
			}
		}

	}

	return true;

}

// ログファイルオープン&必要情報書き込み
bool CheckAdminMatch::
fnOpenChkLog()
{

	if(!mChkLog.Open(mFN_ChkLog, FREESTYLELOG)) {
		fprintf(stderr, "ログファイルが開けません\n");
		return false;
	}

	// ツール名をログへ出力
	mChkLog.OutputComment("行政界整合チェックツール");
	CVersion cVer;
	char szMsg[100] = {0};
	sprintf_s( szMsg, sizeof(szMsg), "%s FILEVERSION %s PRODUCTVERSION %s",	cVer.GetOriginalFilename(), cVer.GetFileVersion(),cVer.GetProductVersion() );
	mChkLog.OutputComment(szMsg);

	// 開始時刻をログへ出力
	mChkLog.OutputTime();

	// ヘッダを出力
	mChkLog.OutputDirect("FLAG\tLAYER\tOBJECTID\t情報\t住所コード\t重心経度\t重心緯度\t住所名称\tCOMMENT ＯＫ|修正|保留\tCOMMENT");
	mChkLog.Flush();

	return true;

}

// 都市地図行政界ポリゴン単位のリスト作成
bool CheckAdminMatch::
fnAdminProc(IFeaturePtr ipF_CityAdmin, const char *cCityCode)
{

	CComVariant	aTmpVar;
	AdminAttr	aAdmAttr;
	AddrPolyRec	aAPRec;
	long		aOID_CityAdmin;
	IGeometryPtr	ipG_CityAdmin;

	aAdmAttr.eAddrCode	= cCityCode;

	// ObjectID 取得
	ipF_CityAdmin->get_OID(&aOID_CityAdmin);

	// AddrCode 取得
	string	aTmpStr;
	ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);
	aTmpStr = ExtractStr(aTmpVar);
	aAdmAttr.eAddrCode += aTmpStr;
	aTmpVar.Clear();

	// 別荘地などの特殊コードは無視
	if(aAdmAttr.eAddrCode.substr(5, 6) == "999999") {
		return true;
	}

	// AreaClass_c 取得
	ipF_CityAdmin->get_Value(mFID_AreaClassC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 0:		aAdmAttr.eAreaClassC = '0'; break;
	case 1:		aAdmAttr.eAreaClassC = '1'; break;
	case 2:		aAdmAttr.eAreaClassC = '2'; break;
	default:	return false;
	}

	// AddrClass_c 取得
	ipF_CityAdmin->get_Value(mFID_AddrClassC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 0:		aAdmAttr.eAddrClassC = '0'; break;
	case 1:		aAdmAttr.eAddrClassC = '1'; break;
	case 2:		aAdmAttr.eAddrClassC = '2'; break;
	case 3:		aAdmAttr.eAddrClassC = '3'; break;
	default:	return false;
	}

	// GaikuFugo 取得
	ipF_CityAdmin->get_Value(mFID_GaikuFugo, &aTmpVar);
	if(aTmpVar.vt == VT_NULL) {
		if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2') return false;	// 整備済み → 号の20桁化はしない
		aAdmAttr.eGaikuFugo = 0;
	} else {
		if(aTmpVar.lVal > 99999) return false;
		aAdmAttr.eGaikuFugo = aTmpVar.lVal;
	}

	// ジオメトリ取得
	if(FAILED(ipF_CityAdmin->get_ShapeCopy(&ipG_CityAdmin))) return false;

	// ポリゴンリストへ追加
	aAPRec.eOID			= aOID_CityAdmin;
	aAPRec.ipPolygon	= ((IPolygonPtr)ipG_CityAdmin);
	
	// 11桁モードの場合は、別途8桁ポリゴンリストを作成するため、ここでは追加しない
	if( m_iExeMode & ON_FiveCode )
		mAddrPoly8.push_back(aAPRec);
	mAddrPoly11.push_back(aAPRec);
	if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2')	mAddrPolyB.push_back(aAPRec);	// 整備済み → 街区リストにも追加

	return true;
}

// ポリゴン状態チェック
bool CheckAdminMatch::
fnCheckAPDist(const string &cAddrCode, const AddrPoly &cAddrPoly, double cDist)
{
	// 同一11桁ポリゴン群の隣接グループ作成
	AddrGroup	aAGroup;
	for(AddrPoly::const_iterator aAPItr1 = cAddrPoly.begin(); aAPItr1 != cAddrPoly.end(); aAPItr1++) {
		AddrGroup::iterator aAGTmp = aAGroup.end();
		for(AddrGroup::iterator aAGItr = aAGroup.begin(); aAGItr != aAGroup.end(); aAGItr++) {
			for(AddrPoly::iterator aAPItr2 = (*aAGItr).begin(); aAPItr2 != (*aAGItr).end(); aAPItr2++) {
				// 隣接ポリゴンかどうかチェック
				VARIANT_BOOL	aDisjoint;
				((IRelationalOperatorPtr)(*aAPItr1).ipPolygon)->Disjoint((*aAPItr2).ipPolygon, &aDisjoint);
				if(!aDisjoint) {	// 隣接
					if(aAGTmp != aAGroup.end()) {	// 既に他のグループに属している
						(*aAGTmp).splice((*aAGTmp).end(), *aAGItr);
						aAGItr = aAGroup.erase(aAGItr);
						aAGItr--;
					} else {						// まだ他のグループに属していない
						(*aAGItr).push_back(*aAPItr1);
						aAGTmp = aAGItr;
					}
					break;
				}
			}
		}
		if(aAGTmp == aAGroup.end()) {	// 隣接がなかった
			AddrPoly	aAPoly;
			aAPoly.push_back(*aAPItr1);
			aAGroup.push_back(aAPoly);
		}
	}

	// 同一11桁ポリゴン群の距離チェック
	if(aAGroup.size() > 1) {	// グループが複数
		IPolygonPtr	ipP_TmpPoly(CLSID_Polygon);

		for(AddrPoly::const_iterator aAPItr1 = cAddrPoly.begin(); aAPItr1 != cAddrPoly.end(); aAPItr1++) {
			IGeometryPtr	ipG_Result;

			// Arc931対応で追加(空間参照が無いとジオメトリ演算がおかしくなる) 2010/06/11
			ISpatialReferencePtr	ipSpRef;
			(aAPItr1->ipPolygon)->get_SpatialReference( &ipSpRef );	// 空間参照を行政界のジオメトリから取得	
			ipP_TmpPoly->putref_SpatialReference( ipSpRef );

			((ITopologicalOperatorPtr)ipP_TmpPoly)->Union((*aAPItr1).ipPolygon, &ipG_Result);
			ipP_TmpPoly	= ipG_Result;
		}

		IPointPtr	ipPt_Centroid;
		double		aX, aY;
		if(FAILED(((IAreaPtr)ipP_TmpPoly)->get_Centroid(&ipPt_Centroid))) {
			aX	= 0;
			aY	= 0;
		} else {
			ipPt_Centroid->QueryCoords(&aX, &aY);
		}

		// 出力用フィーチャカーソル取得
		IFeatureCursorPtr	ipFCur_TmpPoly;
		ipFC_TmpPoly->Insert(VARIANT_TRUE, &ipFCur_TmpPoly);

		IFeatureBufferPtr	ipFBuf_TmpPoly;
		ipFC_TmpPoly->CreateFeatureBuffer(&ipFBuf_TmpPoly);

		ipFBuf_TmpPoly->putref_Shape(ipP_TmpPoly);

		CComVariant	aVar_AddrCode = cAddrCode.c_str();
		ipFBuf_TmpPoly->put_Value(mFID_AddrCode_TP, aVar_AddrCode);

		// 住所コードから住所名称取得（Bug8610対応）
		CString	AddrKanji;
		mJDBAccess.CodeToKanji( cAddrCode.c_str(), AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

		CComVariant	aOID_TP;
		if(FAILED(ipFCur_TmpPoly->InsertFeature(ipFBuf_TmpPoly, &aOID_TP)) || FAILED(ipFCur_TmpPoly->Flush())) {
			sprintf(gMessage, "0\t\t\t行政界ポリゴンが分散\t%s\t%16.12f\t%15.12f\t%s\t\t",
				cAddrCode.c_str(), aX, aY,
				AddrKanji);
		} else {
			sprintf(gMessage, "0\t%s\t%ld\t行政界ポリゴンが分散\t%s\t%16.12f\t%15.12f\t%s\t\t",
				mFC_TmpPoly.c_str(), aOID_TP.lVal, cAddrCode.c_str(), aX, aY,
				AddrKanji);
		}
		mChkLog.OutputDirect(gMessage);
	}
	mChkLog.Flush();

	return true;
}

// 8桁単位の行政界ポリゴンキャッシュ作成
bool CheckAdminMatch::
fnCreateCache8(const string &cAddrCode)
{
	ipSpCacheMgr->EmptyCache();

	char	aWhere[1024];
	sprintf(aWhere, FD_CITY_CODE " = '%s' and " FD_ADDR_CODE " like '%s___'",
		cAddrCode.substr(0, 5).c_str(), cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	if(aCount < 1) {
		return false;
	}

	double	aXMin = 180, aYMin = 180, aXMax = -180, aYMax = -180;

	IFeaturePtr		ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {
		double	aX1, aY1, aX2, aY2;
		IEnvelopePtr	ipE_CityAdmin;
		ipF_CityAdmin->get_Extent(&ipE_CityAdmin);
		ipE_CityAdmin->QueryCoords(&aX1, &aY1, &aX2, &aY2);
		if(aX1 < aXMin) aXMin = aX1;
		if(aY1 < aYMin) aYMin = aY1;
		if(aX2 > aXMax) aXMax = aX2;
		if(aY2 > aYMax) aYMax = aY2;
	}

	IEnvelopePtr	ipE_CityAdmin8(CLSID_Envelope);
	ipE_CityAdmin8->PutCoords(aXMin, aYMin, aXMax, aYMax);
	ipE_CityAdmin8->Expand(0.001, 0.001, VARIANT_FALSE);
	ipSpCacheMgr->FillCache(ipE_CityAdmin8);

	return true;
}

// 8桁単位の行政界ポリゴンリスト作成(11桁モード用)
bool CheckAdminMatch::
fnCreatePolyList8(const string &cAddrCode)
{
	char	aWhere[1024];
	sprintf(aWhere, FD_CITY_CODE " = '%s' and " FD_ADDR_CODE " like '%s___'",
		cAddrCode.substr(0, 5).c_str(), cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	// ポリゴン間チェック用なので、1個以下の場合は作成しない
	if(aCount < 2) {
		return false;
	}

	IFeaturePtr		ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) 
	{
		CComVariant	aTmpVar;
		AdminAttr	aAdmAttr;
		AddrPolyRec	aAPRec;
		long		aOID_CityAdmin;
		IGeometryPtr	ipG_CityAdmin;

		aAdmAttr.eAddrCode	= cAddrCode.substr(0, 5).c_str();

		// ObjectID 取得
		ipF_CityAdmin->get_OID(&aOID_CityAdmin);

		// AddrCode 取得
		string	aTmpStr;
		ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);
		aTmpStr = ExtractStr(aTmpVar);
		aAdmAttr.eAddrCode += aTmpStr;
		aTmpVar.Clear();

		// 別荘地などの特殊コードは無視
		if(aAdmAttr.eAddrCode.substr(5, 6) == "999999") {
			continue;
		}

		// AreaClass_c 取得
		ipF_CityAdmin->get_Value(mFID_AreaClassC, &aTmpVar);
		switch(aTmpVar.lVal) {
		case 0:		aAdmAttr.eAreaClassC = '0'; break;
		case 1:		aAdmAttr.eAreaClassC = '1'; break;
		case 2:		aAdmAttr.eAreaClassC = '2'; break;
		default:	continue;
		}

		// AddrClass_c 取得
		ipF_CityAdmin->get_Value(mFID_AddrClassC, &aTmpVar);
		switch(aTmpVar.lVal) {
		case 0:		aAdmAttr.eAddrClassC = '0'; break;
		case 1:		aAdmAttr.eAddrClassC = '1'; break;
		case 2:		aAdmAttr.eAddrClassC = '2'; break;
		case 3:		aAdmAttr.eAddrClassC = '3'; break;
		default:	continue;
		}

		// GaikuFugo 取得
		ipF_CityAdmin->get_Value(mFID_GaikuFugo, &aTmpVar);
		if(aTmpVar.vt == VT_NULL) {
			if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2') continue;	// 整備済み → 号の20桁化はしない
			aAdmAttr.eGaikuFugo = 0;
		} else {
			if(aTmpVar.lVal > 99999) continue;
			aAdmAttr.eGaikuFugo = aTmpVar.lVal;
		}

		// ジオメトリ取得
		if(FAILED(ipF_CityAdmin->get_ShapeCopy(&ipG_CityAdmin))) continue;

		// ポリゴンリストへ追加
		aAPRec.eOID			= aOID_CityAdmin;
		aAPRec.ipPolygon	= ((IPolygonPtr)ipG_CityAdmin);
		mAddrPoly8.push_back(aAPRec);
	}

	return true;
}

// メイン部
bool CheckAdminMatch::
Execute(int cArgc, char **cArgv)
{
	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnGetEnvVar()) return false;
	if(!fnAnalyzeArg()) return false;

	// GeoDBへ接続
	mDBID_Address	= Connect(mDBCon_Address);	if(mDBID_Address	< 0) return false;

	// フィーチャクラス取得
	if((ipFC_CityAdmin		= GetFeatureClass(mDBID_Address, _T(mFC_CityAdmin)))	== NULL) return false;
	if((ipFC_BaseAdmin		= GetFeatureClass(mDBID_Address, _T(mFC_BaseAdmin)))	== NULL) return false;

	// フィールドインデックス取得
	if(!GetFieldID(ipFC_CityAdmin,		FD_CITY_CODE,			&mFID_CityCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_ADDR_CODE,			&mFID_AddrCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_GAIKU_FUGO,			&mFID_GaikuFugo))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_EXT_GAIKU_FUGO,		&mFID_ExtGaikuFugo))	return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_AREA_CLASS_C,		&mFID_AreaClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_ADDR_CLASS_C,		&mFID_AddrClassC))		return false;

	// チェック結果格納先フィーチャクラス作成
	if(!fnCreateTmpFC()) return false;

	// 住所DBへの接続
	if( !mJDBAccess.Connect() )
	{
		cerr << "住所DBへの接続に失敗" << endl;
		return false;
	}

	// 市区町村リストの生成
	cerr << "市区町村リスト作成中...";
	if(!fnMakeCityList()) return false;
	cerr << "完了" << endl;

	// ログファイルのオープン
	if(!fnOpenChkLog()) return false;

	ipSpCacheMgr = ipWorkspace[mDBID_Address];

	char	temp_addr[6];
	memset( temp_addr, '\0', 6 );
	try
	{
		// 市区町村コードリストのループ
		for(CityListSet_i aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			if( m_iExeMode & ON_ElevenCode && strcmp( temp_addr, (*aCLItr).eCityCode ) == 0 )
				continue;
			strcpy( temp_addr, (*aCLItr).eCityCode );
			cerr << (*aCLItr).eCityCode << ": チェック中..." << endl;

			// 市区町村ポリゴンの設定
			fnSetBaseAdminPoly((*aCLItr).eCityCode);

			// ８・11桁リストの作成／行政界のチェック
			if(!fnMakeACList((*aCLItr).eCityCode)) continue;
			mChkLog.Flush();

			// ８桁リストのループ
			ACSet::iterator	aACItr;
			for(aACItr = mACSet.begin(); aACItr != mACSet.end(); aACItr++) {

				cerr << (*aACItr).eAC8 << ": チェック中..." << endl;

				// キャッシュ作成
				fnCreateCache8((*aACItr).eAC8);

				// 11桁リストのループ
				AC11Set::iterator	aAC11Itr;
				for(aAC11Itr = (*aACItr).eAC11Set.begin(); aAC11Itr != (*aACItr).eAC11Set.end(); aAC11Itr++) {

					cerr << (*aAC11Itr).eAC11 << ": チェック中..." << endl;

					// 行政界ポリゴンのループ
					set<long, less<long> >::iterator	aItr;
					for(aItr = (*aAC11Itr).eOIDSet.begin(); aItr != (*aAC11Itr).eOIDSet.end(); aItr++) {
						IFeaturePtr		ipF_CityAdmin;
						ipFC_CityAdmin->GetFeature(*aItr, &ipF_CityAdmin);
						if(!fnAdminProc(ipF_CityAdmin, (*aCLItr).eCityCode)) {
							mChkLog.Flush();
						}
					}
					fnCheckAPDist((*aAC11Itr).eAC11, mAddrPoly11, mDist3);
					fnCheckAPDist((*aAC11Itr).eAC11, mAddrPolyB, mDist4);

					mAddrPoly11.clear();
					mAddrPolyB.clear();
				}
				if( m_iExeMode & ON_FiveCode )
				{
					if((*aACItr).eAC11Set.size() > 1) {		// 11桁が複数存在する場合（ログの重複を防ぐ）
						fnCheckAPDist((*aACItr).eAC8, mAddrPoly8, mDist2);
					}
				}
				else
				{
					// 8桁単位のポリゴンリスト作成
					if(fnCreatePolyList8((*aACItr).eAC8) && (*aACItr).eAC11Set.size() > 1)
					{
						fnCheckAPDist((*aACItr).eAC8, mAddrPoly8, mDist2);
					}
				}
				mAddrPoly8.clear();
			}
		}
	}
	catch(std::exception & e_std)
	{
		char aMessage[1024];
		std::cerr << "std::exception::what : " << e_std.what() << std::endl;
		sprintf(aMessage, "std::exception::what : ",e_std.what() );
		mChkLog.OutputDirect(aMessage);
		return false;
	}
	catch(_com_error &e_com)
	{
		char aMessage[1024];
		std::cerr << "com_error : " << e_com.ErrorMessage() << std::endl;
		sprintf(aMessage, "com_error : ",e_com.ErrorMessage() );
		mChkLog.OutputDirect(aMessage);
		return false;
	}

	// 終了時刻をログへ出力
	mChkLog.OutputTime();

	return true;
}
