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

// CheckJusyoDist.cpp: CheckJusyoDist クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CheckJusyoDist.h"
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/str_util.h>

DWORD	gTS, gTC, gTE, gTP[10], gTT;

using namespace uh;
using namespace sindy::schema;

/**
 * @brief	半角数字文字列？
 * @param	strData	[in]	データ
 * @return	bool
 */
bool IsNumStr( const CString& strData )
{
	int iLen = strData.GetLength();
	for( int i = 0; i < iLen; ++i )
	{
		if( !_istdigit(strData.GetAt(i)) )
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CheckJusyoDist::CheckJusyoDist()
{
	mDBCon_Address	= NULL;
	mPGDB_Out	= NULL;

	mFC_CityAdmin	= NULL;
	mFC_GouPoint	= NULL;
	mFC_CSAddrPoint	= NULL;
	mFC_KlinkPoint	= NULL;

	mIgnoreTPG		= false;

	mFN_ChkLog		= NULL;

	mFN_CityList	= NULL;
	mCityCode		= NULL;
	mAddr8List      = NULL;

	mRangeMin		= -1;
	mRangeMax		= -1;

	mDistErr		= 800;		// ポイント群 - ポイント群
	mDistNbr		= 100;		// ポイント・グループ化閾値

	mCLType			= kCLNone;

	m_iExeMode		= 0;

	// 実行モードリストの初期化
	m_exeModeList.insert(ON_FiveCode);
	m_exeModeList.insert(ON_ElevenCode);
	m_exeModeList.insert(ON_Addrcode8);

}

CheckJusyoDist::~CheckJusyoDist()
{

}

// 使用法
void CheckJusyoDist::Usage()
{
	cerr << "[新距離チェックツール]\n"
		 << "(Usage) CheckJusyoDist.exe [実行エリアオプション] [その他オプション]\n"
		 << "・実行エリアオプションについて\n"
		 << "  -ca ... 全件\n"
		 << "  -cr [開始市区町村コード-終了市区町村コード] ... エリア指定\n"
		 << "  -cl [市区町村リストファイル] ... リスト指定\n"
		 << "  -cs [市区町村コード] ... 単発\n"
		 << "  -adr8list [住所8桁コードリスト] ... リスト指定\n"
		 << "・実行モードについて\n"
		 << "  -f  ... 5桁コード実行モード(-ca, -crを指定したときはこちら)\n"
		 << "  -e  ... 11桁コード実行モード\n"
		 << "  -adr8  ... 住所8桁コード＋地番 実行モード\n"
		 << "・その他オプションについて\n"
		 << "  -o  [ログファイル] ... 必須指定\n"
		 << "  -t                 ... TPG除外モード(デフォルトは、Off)\n"
		 << "  -dn [グループ距離] ... グループ化の距離(デフォルトは、100m)\n"
		 << "  -de [エラーとするグループ間距離] ... 各グループ間のエラーとなる距離(デフォルトは、800m)\n"
		 << "・環境変数について\n"
		 << "  DB_ADDRESS    ... 住所系SDE接続先\n"
		 << "  FC_GOU_POINT  ... 号ポイントフィーチャクラス名\n"
		 << "  FC_CS_ADDR_POINT ... CSポイントフィーチャクラス\n"
		 << "  FC_KLINK_POINT ... 家形リンクポイントフィーチャクラス\n"
		 << "  FC_CITY_ADMIN ... 都市地図行政界フィーチャクラス名\n"
		 << "  JDB_CONNECT ... 住所DB接続先\n"
		 << "  PGDB_OUT ... 出力先のPGDB\n"
		 << "  FC_OUT   ... 出力先のフィーチャクラス名\n" << std::endl;
}

// 環境変数取得
bool CheckJusyoDist::fnGetEnvVar()
{
	mDBCon_Address	= getenv("DB_ADDRESS");
	if(!mDBCon_Address) {
		cerr << "環境変数 DB_ADDRESS が設定されていません" << endl;
		return false;
	}

	mFC_GouPoint	= getenv("FC_GOU_POINT");
	if(!mFC_GouPoint) {
		cerr << "環境変数 FC_GOU_POINT が設定されていません" << endl;
		return false;
	}

	mFC_CityAdmin	= getenv("FC_CITY_ADMIN");
	if(!mFC_CityAdmin) {
		cerr << "環境変数 FC_CITY_ADMIN が設定されていません" << endl;
		return false;
	}

	mFC_CSAddrPoint	= getenv("FC_CS_ADDR_POINT");
	if(!mFC_CSAddrPoint) {
		cerr << "環境変数 FC_CS_ADDR_POINT が設定されていません" << endl;
		return false;
	}

	mFC_KlinkPoint	= getenv("FC_KLINK_POINT");
	if(!mFC_KlinkPoint) {
		cerr << "環境変数 FC_KLINK_POINT が設定されていません" << endl;
		return false;
	}

	mPGDB_Out	= getenv("PGDB_OUT");
	if(!mPGDB_Out) {
		cerr << "環境変数 PGDB_OUT が設定されていません" << endl;
		return false;
	}

	mFC_Target = getenv("FC_OUT");
	if(!mFC_Target) {
		cerr << "環境変数 FC_OUT が設定されていません" << endl;
		return false;
	}

	return true;
}

// 引数解析
bool CheckJusyoDist::fnAnalyzeArg( int argc, char** argv )
{
	char	*pRange, *pHyphen;

	if( argc < 4 )
	{
		cerr << "引数が足りません" << endl;
		return false;
	}

	for( int i = 1; i < argc && argv[i][0] == '-'; i++ )
	{
		uh::tstring arg(argv[i]);

		// 実行モード（住所8桁コード＋地番コード）のオプション確認
		if(arg == _T("-adr8"))
		{
			m_iExeMode |= ON_Addrcode8;
			continue;
		}
		
		// 実行エリア（住所8桁コードリスト）のオプション確認
		if(arg == _T("-adr8list"))
		{
			mCLType = kAdr8List;
			mAddr8List = ( i+1 < argc ) ? argv[++i] : NULL;
			continue;
		}

		switch( argv[i][1] )
		{
		case 'c':
			switch( argv[i][2] )
			{
			case 'a':
				mCLType	= kCLAll;
				break;
			case 'l':
				mCLType	= kCLList;
				mFN_CityList	= argv[++i];
				break;
			case 's':
				mCLType	= kCLSingle;
				mCityCode	= argv[++i];
				break;
			case 'r':
				mCLType	= kCLRange;
				pRange	= argv[++i];
				mRangeMin	= atol(pRange);
				pHyphen	= strchr(pRange, '-');
				if(pHyphen != NULL) {
					mRangeMax	= atol(++pHyphen);
				}
				break;
			default:
				cerr << "不正なオプションです: " << argv[i] << endl;
				return false;
			}
			break;
		case 'o':	// ログファイル
			mFN_ChkLog	= argv[++i];
			break;
		case 't':
			mIgnoreTPG	= true;
			break;
		case 'd':
			switch( argv[i][2] )
			{
			case 'e':
				mDistErr	= atof(argv[++i]);
				break;
			case 'n':
				mDistNbr	= atof(argv[++i]);
				break;
			default:
				cerr << "不正なオプションです: " << argv[i] << endl;
				return false;
			}
			break;
		case 'e':
			m_iExeMode |= ON_ElevenCode;
			break;
		case 'f':
			m_iExeMode |= ON_FiveCode;
			break;
		default:
			cerr << "不正なオプションです: " << argv[i] << endl;
			return false;
		}
	}

	// 実行モードが未指定でないかチェック
	if( m_iExeMode == 0 )
	{
		cerr << "実行モードが指定されておりません(-e or -f or -adr8)" << endl;
		return false;
	}

	// 実行モードが複数指定されていないかチェック
	int count = 0;
	for(const auto& mode : m_exeModeList)
	{
		if(m_iExeMode & mode)
		{
			++count;
		}
	}

	if(count > 1)
	{
		cerr << "実行モードが複数指定されております" << endl;
		return false;
	}

	// 実行モード（住所8桁コード＋地番コード）指定時の関連オプションのチェック
	if( m_iExeMode & ON_Addrcode8)
	{
		if(mCLType != kAdr8List)
		{
			cerr << "オプション -adr8list が指定されていません" << endl;
			return false;
		}

		if(mAddr8List == NULL)
		{
			cerr << "オプション -adr8list で住所8桁コードリストファイルが指定されていません" << endl;
			return false;
		}
	}

	switch( mCLType )
	{
	case kCLNone:
		cerr << "オプション -c* が指定されていません" << endl;
		return false;
	case kCLAll:
		if( (m_iExeMode & ON_ElevenCode) || (m_iExeMode & ON_Addrcode8) )
		{
			cerr << "全国チェックの際は -f(5桁モード) を指定してください" << endl;
			return false;
		}
		break;
	case kCLList:
		if( mFN_CityList == NULL )
		{
			cerr << "オプション -cl で市区町村リストファイルが指定されていません" << endl;
			return false;
		}
		break;
	case kCLSingle:
		if( mCityCode == NULL )
		{
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
		if( mRangeMin < 0 || mRangeMax < 0 )
		{
			cerr << "オプション -cr で市区町村コードの範囲が正しく指定されていません" << endl;
			return false;
		}
		if( (m_iExeMode & ON_ElevenCode) || (m_iExeMode & ON_Addrcode8) )
		{
			cerr << "範囲チェックの際は -f(5桁モード) を指定してください" << endl;
			return false;
		}
		break;
	}

	if( mFN_ChkLog == NULL )
	{
		cerr << "チェックログファイル名が指定されていません" << endl;
		return false;
	}

	if( mDistErr == 0 )
	{
		cerr << "[エラーとするグループ間距離]を正しく指定してください" << endl;
		return false;
	}
	if( mDistNbr == 0 )
	{
		cerr << "[グループ化距離]を正しく指定してください" << endl;
		return false;
	}
	
	return true;

}


bool CheckJusyoDist::fnCreateTmpFC()
{
	// [Bug 6194]先にPGDB取得(なければ作成)
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( CComBSTR(mPGDB_Out), NULL, &ipWorkspace );
	if( NULL == ipWorkspace )
	{
		std::string strTmp = mPGDB_Out;

		// ディレクトリとファイルに分割
		int nPos = strTmp.rfind( "\\" );
		std::string strDir = (-1 != nPos)? strTmp.substr( 0, nPos ) : ".";
		std::string strFile = (-1 != nPos)? strTmp.substr( nPos+1 ) : strTmp;

		IPropertySetPtr ipPropertySet( CLSID_PropertySet );
		IWorkspaceNamePtr ipWorkspaceName;
		ipWorkspaceFactory->Create( CComBSTR(strDir.c_str()), CComBSTR(strFile.c_str()), ipPropertySet, NULL, &ipWorkspaceName );
		if( ipWorkspaceName )
		{
			IUnknownPtr ipUnknown;
			((INamePtr)ipWorkspaceName)->Open( &ipUnknown );
			ipWorkspace = ipUnknown;
		}
	}

	IFieldsPtr	ipFlds_TmpFC(CLSID_Fields);
	IFieldPtr	ipFld_Shape(CLSID_Field), ipFld_OID(CLSID_Field), ipFld_AddrCode(CLSID_Field);
	IGeometryDefPtr	ipGD_TmpFC(CLSID_GeometryDef);
	ISpatialReferencePtr	ipSR_TmpFC;
	IUIDPtr		ipUID_ClsID, ipUID_ExtClsID;

	ipFC_GouPoint->get_CLSID( &ipUID_ClsID );
	ipFC_GouPoint->get_EXTCLSID( &ipUID_ExtClsID );
	((IGeoDatasetPtr)ipFC_GouPoint)->get_SpatialReference( &ipSR_TmpFC );

	// GeometryDef 設定
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GeometryType(esriGeometryMultipoint);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridCount(1);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridSize(0, 0.05);
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
	((IFieldEditPtr)ipFld_AddrCode)->put_Length(16);
	((IFieldEditPtr)ipFld_AddrCode)->put_IsNullable(VARIANT_FALSE);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_AddrCode);

	// フィーチャクラス作成
	((IFeatureWorkspacePtr)ipWorkspace)->CreateFeatureClass( CComBSTR(mFC_Target), ipFlds_TmpFC, ipUID_ClsID, ipUID_ExtClsID, esriFTSimple, CComBSTR("SHAPE"), NULL, &ipFC_TmpPoints );
	if( NULL == ipFC_TmpPoints )
	{
		cerr << "フィーチャクラス " << mFC_Target << " の作成に失敗しました" << endl;
		return false;
	}

	// フィールドインデックス取得
	if(!GetFieldID(ipFC_TmpPoints,		FD_ADDR_CODE,			&mFID_AddrCode_TP))		return false;

	return true;
}

bool CheckJusyoDist::fnMakeCityList()
{
	IFeatureCursorPtr	ipFCur_CityAdmin;
	IFeaturePtr			ipF_CityAdmin;
	_ICursorPtr		ipCur_CityAdmin;
	IQueryDefPtr	ipQDef_CityAdmin;
	_IRowPtr			ipR_CityAdmin;
	char	aWhere[128], aRange[64], aSubField[64];
	string	aTmpStr;
	sprintf(aWhere, "%s > '00000'", city_admin::kCityCode);
	sprintf(aSubField, "distinct(%s)", city_admin::kCityCode);

	switch(mCLType) {
	case kCLRange:
		sprintf(aRange, " and %s between '%05d' and '%05d'", city_admin::kCityCode, mRangeMin, mRangeMax);
		strcat(aWhere, aRange);
	case kCLAll:
		((IFeatureWorkspacePtr)ipWorkspace)->CreateQueryDef(&ipQDef_CityAdmin);
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
	// 住所8桁コードリスト指定
	case kAdr8List:
		// 住所8桁コードリスト読込み
		if(!mCityList.Open(mAddr8List))
		{
			return false;
		}
		break;
	}

	return true;
}

// 行政界リスト生成
bool CheckJusyoDist::fnMakeACList(const char *cCityCode)
{
	mACSet.clear();

	using namespace sindy::schema;

	CString whereCaluse;
	whereCaluse.Format(_T("%s = '%s' and %s = %d"), 
		city_admin::kCityCode, cCityCode,
		city_admin::kAddrClass, city_admin::addr_class::kResidenceIndicationAndChibanUndeveloped);

	long	aCount;
	IFeatureCursorPtr ipFCur_CityAdmin;
	if(!SearchByWhereClause(whereCaluse, ipFC_CityAdmin, aCount, ipFCur_CityAdmin))
	{
		return false;
	}

	if(aCount < 1)
	{
		cerr << cCityCode << ": 行政界ポリゴンがありません" << endl;
		return false;
	}

	IFeaturePtr	ipF_CityAdmin;
	while( S_OK == ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) && ipF_CityAdmin )
	{
		long lOID = 0;
		ipF_CityAdmin->get_OID( &lOID );

		// ８・11桁コードの取得・リストへの追加
		CComVariant	vaValue;
		string	strAddrCode;
		ipF_CityAdmin->get_Value(mFID_CityCode, &vaValue);
		strAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
		vaValue.Clear();

		ipF_CityAdmin->get_Value(mFID_AddrCode, &vaValue);
		strAddrCode += (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
		vaValue.Clear();

		mACSet.Add( strAddrCode, lOID );

	}
	return true;
}

bool CheckJusyoDist::fnOpenChkLog()
{
	if( !mChkLog.Open(mFN_ChkLog, FREESTYLELOG) )
	{
		fprintf(stderr, "ログファイルが開けません\n");
		return false;
	}

	// ツール名・バージョン出力
	CVersion cVer;
	char szMsg[100] = {0};
	sprintf_s( szMsg, sizeof(szMsg), "%s FILEVERSION %s PRODUCTVERSION %s", cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	mChkLog.OutputComment( szMsg );

	// 開始時刻をログへ出力
	mChkLog.OutputTime();

	// ヘッダを出力
	mChkLog.OutputDirect("FLAG\tLAYER\tOBJECTID\tポイント数\t情報\t住所コード\t文字地番\t重心経度\t重心緯度\t都道府県名\t市区町村名\t大字・町名\t字・町目\tCOMMENT ＯＫ|修正|保留\tCOMMENT");
	mChkLog.Flush();

	return true;
}

void CheckJusyoDist::getCityAdmin(const IFeaturePtr& ipCityAdmin, AdminAttr& adminAttr, IGeometryPtr& ipCityAdminGeo)
{
	CComVariant	vaValue;

	// CityCode取得
	ipCityAdmin->get_Value( mFID_CityCode, &vaValue );
	adminAttr.eAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "00000";
	vaValue.Clear();

	// AddrCode 取得
	ipCityAdmin->get_Value(mFID_AddrCode, &vaValue);
	adminAttr.eAddrCode += (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// AddrClass_c 取得
	ipCityAdmin->get_Value(mFID_AddrClassC, &vaValue);
	adminAttr.eAddrClassC = vaValue.lVal;

	// AddrCode2 取得
	ipCityAdmin->get_Value( mFID_AddrCode2, &vaValue );
	adminAttr.eAddrCode2 = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// GaikuFugo 取得
	ipCityAdmin->get_Value(mFID_GaikuFugo, &vaValue);
	adminAttr.eGaikuFugo = (VT_I4 == vaValue.vt)? vaValue.lVal : 0;

	// 拡張街区符号取得
	ipCityAdmin->get_Value(mFID_ExtGaikuFugo, &vaValue);
	adminAttr.eExtGaiku = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// ジオメトリ取得
	ipCityAdmin->get_ShapeCopy( &ipCityAdminGeo );
}

// 指定行政化の号ポイント20桁化
bool CheckJusyoDist::fnAdminProc(const IFeaturePtr& ipFeature )
{
	// 都市地図行政界ポリゴンの属性値を取得
	AdminAttr adminAttr;
	IGeometryPtr ipCityAdminGeo;
	getCityAdmin(ipFeature, adminAttr, ipCityAdminGeo);

	// 別荘地などの特殊コードは無視
	if( adminAttr.eAddrCode.substr(5, 6) == _T("999999") )
	{
		return true;
	}

	// 街区符号なしで整備済み → 号の20桁化はしない
	if( adminAttr.eExtGaiku.empty() && adminAttr.eGaikuFugo == 0 && 
		(adminAttr.eAddrClassC == 1 || adminAttr.eAddrClassC == 2) )
		return true;

	// 拡張街区ありで町丁目未整備|地番整備済み → 号の20桁化しない
	if( !adminAttr.eExtGaiku.empty() && 
		adminAttr.eAddrClassC == 0 || adminAttr.eAddrClassC == 2 )
		return true;	

	// 指定都市地図行政界ポリゴン上に存在する号ポイントを取得
	long gouPointCount = 0;
	IFeatureCursorPtr ipGouPointFeaCur;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_GouPoint, gouPointCount, ipGouPointFeaCur))
	{
		return false;
	}

	if( gouPointCount != 0 )
	{
		IFeaturePtr	ipF_GouPoint;
		while(S_OK == ipGouPointFeaCur->NextFeature(&ipF_GouPoint) && ipF_GouPoint)
		{
			fnGetGPInfo(ipF_GouPoint, adminAttr, true);
		}
	}

	// KP空間検索
	long aKPCount = 0;
	IFeatureCursorPtr	ipFCur_KlinkPoint;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_KlinkPoint, aKPCount, ipFCur_KlinkPoint))
	{
		return false;
	}

	if( aKPCount == 0 )
	{
		return true;
	}

	IFeaturePtr	ipF_KlinkPoint;
	while(S_OK == ipFCur_KlinkPoint->NextFeature(&ipF_KlinkPoint) && ipF_KlinkPoint)
	{
		if(!fnGetKPInfo(ipF_KlinkPoint, adminAttr))
		{
			continue;
		}
	}

	return true;
}

bool CheckJusyoDist::getGouPoints(const IFeaturePtr& ipFeature )
{
	// 都市地図行政界ポリゴンの属性値を取得
	AdminAttr adminAttr;
	IGeometryPtr ipCityAdminGeo;
	getCityAdmin(ipFeature, adminAttr, ipCityAdminGeo);

	// 別荘地などの特殊コードは無視
	if( adminAttr.eAddrCode.substr(5, 6) == _T("999999") )
	{
		return true;
	}

	// 指定都市地図行政界ポリゴン上に存在する号ポイントを検索
	long gouPointCount = 0;
	IFeatureCursorPtr ipGouPointFeaCur;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_GouPoint, gouPointCount, ipGouPointFeaCur))
	{
		return false;
	}

	// 該当する号ポイントが0件の場合は、以降の処理は行わない
	if( gouPointCount == 0)
	{
		return true;
	}

	// 該当する号ポイントの情報を取得
	IFeaturePtr	ipGouPointFea;
	while(S_OK == ipGouPointFeaCur->NextFeature(&ipGouPointFea) && ipGouPointFea)
	{
		fnGetGPInfo(ipGouPointFea, adminAttr, false);
	}

	return true;
}

// 号ポイント情報取得(20桁化)
void CheckJusyoDist::fnGetGPInfo(const IFeaturePtr& ipFeature, const AdminAttr &cAdmAttr, bool isAddAdr2Point )
{
	CComVariant vaValue;
	AddrTmpRec		aATRec;

	// 号番号の取得
	ipFeature->get_Value(mFID_GouNo, &vaValue);
	string strGouNo = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";

	// 号番号空または[*]ならチェック対象外
	if( strGouNo.empty() || strGouNo.find('*') == 0 )
		return;
	vaValue.Clear();

	// 拡張号番号フラグの取得
	ipFeature->get_Value(mFID_ExpGouNoF, &vaValue);
	bool bExGouF = (vaValue.vt != VT_NULL && vaValue.lVal == 1);

	// 住所整備種別コードの取得
	aATRec.eAddrClass	= cAdmAttr.eAddrClassC;

	// ObjectID 取得
	ipFeature->get_OID(&aATRec.ePointID);

	// [Bug6516]20桁生成
	CreateAddrCode( cAdmAttr, strGouNo, bExGouF, aATRec );
	if( aATRec.eAddrCode.length() == 20 )
	{
		// 号タイプの取得
		ipFeature->get_Value(mFID_GouType, &vaValue);
		aATRec.eGouType = vaValue.lVal;

		// 座標の取得
		IGeometryPtr ipG_GouPoint;
		ipFeature->get_Shape(&ipG_GouPoint);
		((IPointPtr)ipG_GouPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

		// 追加
		mAddrTmpSet.insert(aATRec);

		// CITY_ADMINのADDRCODE2の値とisAddAdr2Pointの値をみて、以降の処理の実施有無を判断
		// （isAddAdr2Point値は実行モードにより異なる）
		if( cAdmAttr.eAddrCode2 != "000000" && isAddAdr2Point )
		{
			aATRec.eAddrCode = aATRec.eAddrCode.substr(0, 5) + cAdmAttr.eAddrCode2.c_str() + aATRec.eAddrCode.substr(11, 9);
			mAddrTmpSet.insert(aATRec);
		}
	}
}

// [Bug6516]20桁生成関数
void CheckJusyoDist::CreateAddrCode( const AdminAttr& cAdmAttr, const string& strGouNo, bool bExGouF, AddrTmpRec& rRec )
{
	static CTmpCodeCreator cTmpCodeCreator;	//!< [Bug 6516]一時コード作成テーブル

	// 最初に文字付き住所になるかならないかで場合わけ(16桁が通常・文字付きで異なるケースは、文字付きを優先する為)
	bool bCharChibanDomariF = false;	// 文字付き地番で地番止まり住所か？
	std::string strCharPntName;	// 文字付き住所地番名称格納用

	// 号番号を[-]で分解しておく
	std::list<CString> listToken;
	CString strData( strGouNo.c_str() );
	strData.Replace( "#", "" );	// 先頭#は除去
	int iCurPos = 0;
	CString strToken( strData.Tokenize( "-", iCurPos ) );
	while( !strToken.IsEmpty() )
	{
		listToken.push_back( strToken );
		strToken = strData.Tokenize( "-", iCurPos );
	}

	// CITY_ADMINの街区符号が未設定（null）の場合
	if( cAdmAttr.eGaikuFugo == 0 )
	{

		// 拡張街区符号が未設定（null）で、号番号の値が「#」＋数値以外の値である場合
		if( cAdmAttr.eExtGaiku.empty() && strGouNo.find('#') == 0 && !IsNumStr(*(listToken.begin())) )	// 文字地番 = 号番号次第
		{
			strCharPntName = *(listToken.begin());
			bCharChibanDomariF = (listToken.size() == 1);
		}
		else
		{
			if( !bExGouF )	// 拡張号番号OFF 文字地番 = 街区符号
			{
				if( !cAdmAttr.eExtGaiku.empty() )
				{
					// [bug11616]対応
					// [Bug6516再修正]では、拡張街区符号（eExtGaiku）＋号番号（のハイフンより前の値）で、
					// 文字付き地番名称を生成していたが、文字付き住所データの生成ロジックをみるかぎり、
					// 文字付き地番名称には拡張街区符号のみ使用する方が適切なため、修正
					strCharPntName = cAdmAttr.eExtGaiku;
					bCharChibanDomariF = false;
				}
			}
			else if( !IsNumStr(*(listToken.begin())) )	// 拡張号番号ON  文字地番 = 号番号次第
			{
				strCharPntName = *(listToken.begin());
				bCharChibanDomariF = (listToken.size() == 1);
			}
		}
	}
	else // CITY_ADMINの街区符号に何かしらの値がある場合
	{
		// 拡張号番号フラグがTrueで、号番号の値が「#」＋数値以外の値である場合
		if(bExGouF && strGouNo.find('#') == 0 && !IsNumStr(*(listToken.begin())))
		{
			strCharPntName = *(listToken.begin());
			bCharChibanDomariF = (listToken.size() == 1);
		}
	}

	if( !strCharPntName.empty() )
	{
		// 文字付きの際、擬似コードを発行する(地番止まりでないときは、下4桁を9999にする)
		std::string strPntCode = cTmpCodeCreator.GetTmpCode( cAdmAttr.eAddrCode, strCharPntName );
		std::string strGouCode = bCharChibanDomariF? "0000" : "9999";
		rRec.eCharPntName = strCharPntName;
		rRec.eAddrCode = (!strPntCode.empty())? cAdmAttr.eAddrCode + strPntCode + strGouCode : "";
	}
	else
	{
		long	aCode1 = 0, aCode2 = 0;
		char* p = strpbrk( (char*)strGouNo.c_str(), "0123456789-");				// まず、数字列 or ハイフンを探す
		if( p )
		{								// 発見（なければ終了）
			if( *p == '-' )
				return;				// ハイフンだったらポイントとして取得しない
			else
			{
				aCode1 = atoi(p);					// 数値化①
				p = strchr(p, '-');					// ハイフンを探す
				if( p )
				{						// 発見（なければ終了）
					p = strpbrk(p, "0123456789");	// 数字列を探す
					if( p )
						aCode2 = atoi(p);			// 数値化②
				}
			}
		}

		long	aChiban, aBango;
		if(!bExGouF && cAdmAttr.eGaikuFugo > 0) {	// 街区符号あり
			aChiban	= cAdmAttr.eGaikuFugo;
			aBango	= aCode1;
			if( aBango > 9999 ) return;
		} else {						// 街区符号なし
			aChiban	= aCode1;
			aBango	= aCode2;
			if(aChiban == 0 || aChiban > 99999 || aBango > 9999) return;
		}

		char szTmpCode[21] = "";
		sprintf_s( szTmpCode, sizeof(szTmpCode), "%s%05d%04d", cAdmAttr.eAddrCode.c_str(), aChiban, aBango );
		rRec.eAddrCode = szTmpCode;
	}
}

// 家形リンク情報取得
bool CheckJusyoDist::fnGetKPInfo(const IFeaturePtr& ipF_KlinkPoint, const AdminAttr& cAdmAttr)
{
	static string	aACOrg, aACNew;

	CComVariant		aTmpVar;
	AddrTmpRec		aATRec;
	
	// 情報提供元コードが「1」（家形リンクデータ）以外は無視
	ipF_KlinkPoint->get_Value(mFID_InfoSrcC, &aTmpVar);
	if(aTmpVar.lVal != 1) return false;

	// 入力用座標精度種別コードが「1」「2」「4」「5」「6」「7」以外は無視
	ipF_KlinkPoint->get_Value(mFID_InputSeidoC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 1: case 2: case 4: case 5: case 6: case 7:
		break;
	default:
		return false;
	}

	// 住所整備種別コードの取得
	aATRec.eAddrClass	= cAdmAttr.eAddrClassC;

	// ObjectID 取得
	ipF_KlinkPoint->get_OID(&aATRec.ePointID);

	// 住所コード(20桁)の取得
	char	aAddrCode[23];
	ipF_KlinkPoint->get_Value(mFID_AddrCode_KP, &aTmpVar);
	aATRec.eAddrCode = ExtractStr(aTmpVar);
	aTmpVar.Clear();

	aATRec.eAddrCode	= aATRec.eAddrCode.substr(0, 20);

	if(aATRec.eAddrCode.substr(11, 5) == "00000") return false;

	// 11桁コードを最新コードに変更
	if(aATRec.eAddrCode.substr(0, 11) == aACOrg) {
		aATRec.eAddrCode	= aACNew + aATRec.eAddrCode.substr(11, 9);
	} else {
		aACOrg	= aATRec.eAddrCode.substr(0, 11);
		mJDBAccess.GetNewestCode(aATRec.eAddrCode.c_str(), aAddrCode);
		aATRec.eAddrCode	= aAddrCode;
		aACNew	= aATRec.eAddrCode.substr(0, 11);
	}

	if("900" < cAdmAttr.eAddrCode.substr(8, 3)) {
		// 11桁が有効なら使用
		bool	aValid;
		mJDBAccess.IsValid(aATRec.eAddrCode.substr(0, 11).c_str(), &aValid);
		if(!aValid) return false;
	} else {
		if("000" < cAdmAttr.eAddrCode.substr(8, 3) && cAdmAttr.eAddrCode.substr(8, 3) < "100") {
			// 11桁一致なら使用
			if(aATRec.eAddrCode.substr(0, 11) != cAdmAttr.eAddrCode) return false;
		} else {
			// ８桁一致なら使用
			if(aATRec.eAddrCode.substr(0, 8) != cAdmAttr.eAddrCode.substr(0, 8)) return false;
		}
	}

	// 号タイプ
	aATRec.eGouType		= 3;

	// 座標の取得
	IGeometryPtr ipG_KlinkPoint;
	ipF_KlinkPoint->get_Shape(&ipG_KlinkPoint);
	((IPointPtr)ipG_KlinkPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

	// 追加
	mAddrTmpSet.insert(aATRec);

	if( cAdmAttr.eAddrCode2 != "000000" ) {
		aATRec.eAddrCode = aATRec.eAddrCode.substr(0, 5) + cAdmAttr.eAddrCode2.c_str() + aATRec.eAddrCode.substr(11, 9);
		mAddrTmpSet.insert(aATRec);
	}

	return true;
}

// CSポイント情報取得
void CheckJusyoDist::fnCSAddrProc(const IFeaturePtr& ipFeature )
{
	AddrTmpRec		aATRec;

	// OIDの取得
	ipFeature->get_OID(&aATRec.ePointID);

	aATRec.eGouType		= 0;

	// 住所コード(20桁)の取得
	CComVariant vaValue;
	ipFeature->get_Value(mFID_AddrCode_CS, &vaValue);
	aATRec.eAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// 座標の取得
	IGeometryPtr	ipG_CSAddrPoint;
	ipFeature->get_Shape(&ipG_CSAddrPoint);
	((IPointPtr)ipG_CSAddrPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

	mAddrTmpSet.insert( aATRec );	// 追加
}

void CheckJusyoDist::fnConvDataSet16()
{
	// 特定条件を満たす住所ポイントを除外
	excludingPoints(true);

	// 住所16桁コードでグループ化
	makeAddrcodeGroupsByAdr16();

}

void CheckJusyoDist::fnConvDataSet8Chiban()
{
	// 特定条件を満たす住所ポイントを除外
	excludingPoints(false);

	// 住所8桁コード＋地番コード（文字付き住所の場合は、住所8桁コード＋地番名称）でグループ化
	makeAddrcodeGroupsByAdr8Chiban();

}

void CheckJusyoDist::excludingPoints(bool isExcludeKakeiPoint)
{
	// CSP/GOUに同一20桁が存在するKLP/TPG(-t指定時)を除外
	AddrTmpSet::iterator aATItr;
	string	aACTmp;
	long	aGTTmp = 0;
	for( aATItr = mAddrTmpSet.begin(); aATItr != mAddrTmpSet.end(); aATItr++ )
	{
		if( aACTmp != (*aATItr).eAddrCode )
		{
			aACTmp	= (*aATItr).eAddrCode;
			aGTTmp	= (*aATItr).eGouType;
			
			continue;
		}

		if( aGTTmp < (*aATItr).eGouType )
		{
			// 家形リンクポイントを除外対象とするかどうかは、isExcludeKakeiPoint次第
			if( (mIgnoreTPG && (*aATItr).eGouType == 2) || ( (*aATItr).eGouType == 3 && isExcludeKakeiPoint ) )	// TPG(-t指定時) or KLP
			{
				AddrTmpSet::iterator	aATTmp = aATItr;
				aATItr--;
				mAddrTmpSet.erase(aATTmp);
			}
		}
	}

	// CSP/GOUに同一16桁が存在する地番止まりTPGを除外(-t指定時)
	if( !mIgnoreTPG )
		return;

	AddrTmpSet::iterator	aATItr1, aATItr2;
	for( aATItr1 = mAddrTmpSet.begin(); aATItr1 != mAddrTmpSet.end(); aATItr1++ )
	{
		if( (*aATItr1).eAddrCode.substr(16, 4) == "0000" && (*aATItr1).eGouType == 2 )	// 地番止まり＆TPG
		{
			for( aATItr2 = aATItr1, aATItr2++; aATItr2 != mAddrTmpSet.end() && (*aATItr1).eAddrCode.substr(0, 16) == (*aATItr2).eAddrCode.substr(0, 16); aATItr2++ )
			{
				if( (*aATItr2).eGouType < 2 )
				{
					AddrTmpSet::iterator	aATTmp = aATItr1;
					if( aATItr1 == mAddrTmpSet.begin() )
					{
						mAddrTmpSet.erase(aATTmp);
						aATItr1 = mAddrTmpSet.begin();
					}
					else
					{
						aATItr1--;
						mAddrTmpSet.erase(aATTmp);
					}
					break;
				}
			}
		}
	}
}

void CheckJusyoDist::makeAddrcodeGroupsByAdr16()
{
	// グループ化前にリストを空にする
	mAddrList.clear();

	AddrTmpSet::iterator aATItr = mAddrTmpSet.begin();
	while( aATItr != mAddrTmpSet.end() )
	{
		if( (*aATItr).eAddrClass != 3 )	// 整備済み → 無視
		{
			aATItr++;
			continue;
		}

		AddrRec	aAddrRec;
		aAddrRec.eAddrCode	= aATItr->eAddrCode.substr(0, 16);
		aAddrRec.eCharPntName = aATItr->eCharPntName;
		while( aATItr != mAddrTmpSet.end() && aAddrRec.eAddrCode == (*aATItr).eAddrCode.substr(0, 16) )
		{
			PointRec	aPtRec;
			aPtRec.ePointID		= (*aATItr).ePointID;
			aPtRec.eLongitude	= (*aATItr).eLongitude;
			aPtRec.eLatitude	= (*aATItr).eLatitude;
			aPtRec.eGouType		= (*aATItr).eGouType;
			aAddrRec.ePoints.push_back(aPtRec);
			aATItr++;
		}

		mAddrList.push_back(aAddrRec);
	}
}

void CheckJusyoDist::makeAddrcodeGroupsByAdr8Chiban()
{
	// グループ化前にリストを空にする
	mAddrList.clear();

	for(const auto& addrTmpRec : mAddrTmpSet)
	{
		// 「住所8桁コード＋地番（通常住所：地番コード、文字付き地番：文字付き地番名称）」の単位でグループ化を行なう仕様だが、
		// 実装上は、住所16桁コード＋地番名称の単位でグループ化するようにしたいため、
		// 　・小字コード（住所コードの8～11桁目）は一律「000」
		// 　・文字付き住所の場合、地番コードは一律「A0000」、文字付き住所でない場合は、元々の地番コードのまま
		// とした上で、グループ化の処理を行なう

		AddrRec tempAddrRec;

		// 住所11桁コードをまず格納（その際、小字コード（住所コードの8～11桁目）は一律「000」にする）
		tempAddrRec.eAddrCode = addrTmpRec.eAddrCode.substr(0, 8) + _T("000");

		// 地番コードについて、文字付き住所の場合（文字付き地番名称が空の場合）、
		// 元々格納されている地番コードを、そうでなければ「A0000」を格納
		tempAddrRec.eAddrCode += (addrTmpRec.eCharPntName.empty()) ? addrTmpRec.eAddrCode.substr(11, 5) : _T("A0000");

		// 文字付き地番名称は元々の値をそのまま格納
		tempAddrRec.eCharPntName = addrTmpRec.eCharPntName;

		PointRec pointRec;
		pointRec.ePointID = addrTmpRec.ePointID;
		pointRec.eLongitude = addrTmpRec.eLongitude;
		pointRec.eLatitude = addrTmpRec.eLatitude;
		pointRec.eGouType = addrTmpRec.eGouType;

		// 処理対象の住所ポイント（addrTmpRec）がグループ化リスト（mAddrList）に存在するか検索
		// 検索としては、同じ16桁コード＋文字付き地番名称の値を持つAddrRecが存在するかを探す形となる
		auto& itr = find(mAddrList.begin(), mAddrList.end(), tempAddrRec);

		// 存在しなければ、新規グループ（AddrRec）としてリストに追加
		if(itr == mAddrList.end())
		{
			tempAddrRec.ePoints.push_back(pointRec);
			mAddrList.push_back(tempAddrRec);

			continue;
		}

		// 存在すれば、既にmAddrListに追加済みのグループ（AddrRec）のePointsへ追加
		itr->ePoints.push_back(pointRec);
	}
}

void CheckJusyoDist::fnCheckDataSet16()
{
	AddrList::iterator	aALItr;
	for( aALItr = mAddrList.begin(); aALItr != mAddrList.end(); aALItr++ )
	{
		// グループリストの作成
		GroupList	aGList;
		for( PointList::iterator aPItr1 = (*aALItr).ePoints.begin(); aPItr1 != (*aALItr).ePoints.end(); aPItr1++ )
		{
			GroupList::iterator	aGItrTmp = aGList.end();
			for( GroupList::iterator	aGItr = aGList.begin(); aGItr != aGList.end(); aGItr++ )
			{
				for( PointList::iterator aPItr2 = (*aGItr).ePoints.begin(); aPItr2 != (*aGItr).ePoints.end(); aPItr2++ )
				{
					// 「近隣」ポイントかどうかのチェック
					double	aDist = mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);

					// 「近隣」である
					if(aDist < mDistNbr)
					{
						if( aGItrTmp != aGList.end() )	// 既に他のグループに属している
						{
							(*aGItrTmp).ePoints.splice((*aGItrTmp).ePoints.end(), (*aGItr).ePoints);
							aGItr = aGList.erase(aGItr);
							aGItr--;
						}
						else	// まだ他のグループに属していない
						{	
							(*aGItr).ePoints.push_back(*aPItr1);
							aGItrTmp = aGItr;
						}
						break;
					}
				}
			}

			// 「近隣」がなかった
			if( aGItrTmp == aGList.end() )
			{
				GroupRec	aGRec;
				aGRec.ePoints.push_back(*aPItr1);
				aGList.push_back(aGRec);
			}
		}

		// グループが一つ → 距離チェックの必要なし
		if(aGList.size() == 1) continue;

		// グループ内で代表点を決定
		for( GroupList::iterator	aGItr = aGList.begin(); aGItr != aGList.end(); aGItr++ )
		{
			double	aMinDist = DBL_MAX;
			PointList::iterator	aPItrTmp = (*aGItr).ePoints.begin();
			for( PointList::iterator aPItr1 = (*aGItr).ePoints.begin(); aPItr1 != (*aGItr).ePoints.end(); aPItr1++ )
			{
				double	aTotalDist = 0;
				for( PointList::iterator aPItr2 = (*aGItr).ePoints.begin(); aPItr2 != (*aGItr).ePoints.end(); aPItr2++ )
				{
					aTotalDist += mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);
				}
				if( aTotalDist < aMinDist )
				{
					aMinDist	= aTotalDist;
					aPItrTmp	= aPItr1;
				}
			}
			(*aGItr).eRepPoint	= *aPItrTmp;
		}

		// グループ間で距離チェック
		bool aErr = false;
		for( GroupList::iterator	aGItr1 = aGList.begin(); aGItr1 != aGList.end(); aGItr1++ )
		{
			GroupList::iterator	aGItr2;
			for( aGItr2 = aGItr1, aGItr2++; aGItr2 != aGList.end(); aGItr2++ )
			{
				double	aMinDist = DBL_MAX;
				for( PointList::iterator aPItr1 = (*aGItr1).ePoints.begin(); aPItr1 != (*aGItr1).ePoints.end(); aPItr1++ )
				{
					for( PointList::iterator aPItr2 = (*aGItr2).ePoints.begin(); aPItr2 != (*aGItr2).ePoints.end(); aPItr2++ )
					{
						double	aDist = mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);
						if(aDist < aMinDist)	aMinDist = aDist;
					}
				}
				if( aMinDist >= mDistErr )
				{
					aErr	= true;
					break;
				}
			}
			if(aErr) break;
		}

		if(aErr) {

			// マルチポイントの作成
			int		aNum = 0;
			double	aSumX = 0, aSumY = 0;
			IMultipointPtr	ipMPt_TmpPoints(CLSID_Multipoint);
			for(PointList::iterator aPItr1 = (*aALItr).ePoints.begin(); aPItr1 != (*aALItr).ePoints.end(); aPItr1++) {
				IPointPtr	ipPt_Tmp(CLSID_Point);
				ipPt_Tmp->PutCoords((*aPItr1).eLongitude, (*aPItr1).eLatitude);
				aSumX	+= (*aPItr1).eLongitude;
				aSumY	+= (*aPItr1).eLatitude;
				aNum++;
				((IPointCollectionPtr)ipMPt_TmpPoints)->AddPoint(ipPt_Tmp);
			}

			// 重心の算出
			double	aX = aSumX / aNum; 
			double	aY = aSumY / aNum;

			// 出力用フィーチャカーソル取得
			IFeatureCursorPtr	ipFCur_TmpPoints;
			ipFC_TmpPoints->Insert(VARIANT_TRUE, &ipFCur_TmpPoints);

			// フィーチャ準備
			IFeatureBufferPtr	ipFBuf_TmpPoints;
			ipFC_TmpPoints->CreateFeatureBuffer(&ipFBuf_TmpPoints);

			ipFBuf_TmpPoints->putref_Shape(ipMPt_TmpPoints);

			std::string tmp((*aALItr).eAddrCode);
			CComVariant	aVar_AddrCode = (*aALItr).eAddrCode.c_str();
			ipFBuf_TmpPoints->put_Value(mFID_AddrCode_TP, aVar_AddrCode);

			// 住所コードから住所名称取得（Bug8609対応）
			CString	KenKanji, ShiKanji, OazaKanji, AzaKanji;
			mJDBAccess.CodeToKanji( aALItr->eAddrCode.c_str(), KenKanji.GetBuffer( KEN_KANJI_MAX_LEN ), ShiKanji.GetBuffer( SHI_KANJI_MAX_LEN ), 
										OazaKanji.GetBuffer( OAZA_KANJI_MAX_LEN ), AzaKanji.GetBuffer( AZA_KANJI_MAX_LEN ) );

			// フィーチャ追加
			char szMessage[1024] = "";
			CComVariant	aOID_TP;
			if(FAILED(ipFCur_TmpPoints->InsertFeature(ipFBuf_TmpPoints, &aOID_TP)) || FAILED(ipFCur_TmpPoints->Flush())) {
				sprintf_s(szMessage, sizeof(szMessage), "0\t\t\t%d\t同一16桁ポイント群が分散(分散ポイントフィーチャの作成に失敗)\t%s\t%s\t%16.12f\t%15.12f\t%s\t%s\t%s\t%s\t\t",
					aNum, 
					aALItr->eCharPntName.empty()? aALItr->eAddrCode.c_str() : aALItr->eAddrCode.substr(0, 11).c_str(),
					aALItr->eCharPntName.c_str(), aX, aY,
					KenKanji, ShiKanji, OazaKanji, AzaKanji);
			} else {
				sprintf_s(szMessage, sizeof(szMessage), "0\t%s\t%ld\t%d\t同一16桁ポイント群が分散\t%s\t%s\t%16.12f\t%15.12f\t%s\t%s\t%s\t%s\t\t",
					mFC_Target, aOID_TP.lVal, aNum, 
					aALItr->eCharPntName.empty()? aALItr->eAddrCode.c_str() : aALItr->eAddrCode.substr(0, 11).c_str(),
					aALItr->eCharPntName.c_str(), aX, aY,
					KenKanji, ShiKanji, OazaKanji, AzaKanji);
			}
			mChkLog.OutputDirect(szMessage);
		}
	}
}

// 8桁コードエリアでキャッシュ作成
bool CheckJusyoDist::fnCreateCache8(const string &cAddrCode)
{

	CString whereClause;
	whereClause.Format(_T("%s = '%s' and %s like '%s___'"),
								city_admin::kCityCode, cAddrCode.substr(0, 5).c_str(),
								city_admin::kAddrCode, cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr ipFCur_CityAdmin;
	if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, aCount, ipFCur_CityAdmin))
	{
		return false;
	}

	if(aCount < 1)
	{
		cerr << cAddrCode << ": 行政界ポリゴンがありません" << endl;
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

bool CheckJusyoDist::Execute(int cArgc, char **cArgv)
{
	if( !fnAnalyzeArg(cArgc, cArgv) || !fnGetEnvVar() )
		return false;

	// GeoDBへ接続
	if(!Connect(mDBCon_Address))
	{
		return false;
	}

	// フィーチャクラス取得
	if((ipFC_GouPoint		= GetFeatureClass(_T(mFC_GouPoint)))	== NULL) return false;
	if((ipFC_CityAdmin		= GetFeatureClass(_T(mFC_CityAdmin)))	== NULL) return false;
	if((ipFC_CSAddrPoint	= GetFeatureClass(_T(mFC_CSAddrPoint)))	== NULL) return false;
	if((ipFC_KlinkPoint		= GetFeatureClass(_T(mFC_KlinkPoint)))	== NULL) return false;

	// フィールドインデックス取得
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kCityCode,			&mFID_CityCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrCode,			&mFID_AddrCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kGaikuFugo,			&mFID_GaikuFugo))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kExtGaikuFugo,		&mFID_ExtGaikuFugo))	return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAreaClass,			&mFID_AreaClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrClass,			&mFID_AddrClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrCode2,			&mFID_AddrCode2))		return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kGouNo,				&mFID_GouNo))			return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kGouType,			&mFID_GouType))			return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kPriority,			&mFID_PriorityF))		return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kExpGouNo,			&mFID_ExpGouNoF))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kInfoSrc,			&mFID_InfoSrcC))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kInputSeido,		&mFID_InputSeidoC))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kAddrCodeOrg,		&mFID_AddrCode_KP))		return false;
	if(!GetFieldID(ipFC_CSAddrPoint,	cs_addr_point::kAddrCode,		&mFID_AddrCode_CS))		return false;

	// 住所DBへの接続
	if( !mJDBAccess.Connect() )
	{
		cerr << "住所DBへの接続に失敗" << endl;
		return false;
	}

	// 市区町村リストの生成
	cerr << "市区町村リスト作成中...";
	if(!fnMakeCityList())
	{
		return false;
	}
	cerr << "完了" << endl;

	// チェック結果格納先フィーチャクラス作成
	if(!fnCreateTmpFC()) return false;

	// ログファイルのオープン
	if(!fnOpenChkLog()) return false;

	ipSpCacheMgr = ipWorkspace;

	// 例外取得部分
	try
	{
		// 5桁実行モード
		if( m_iExeMode & ON_FiveCode )
		{
			// 市区町村コードリストのループ
			for(const auto& city : mCityList)
			{
				cerr << city.eCityCode << ": チェック中..." << endl;

				// ８・11桁リストの作成／行政界のチェック
				if(!fnMakeACList(city.eCityCode))
				{
					continue;
				}
				mChkLog.Flush();

				// ８桁リストのループ
				for(const auto& ac8Rec : mACSet)
				{
					cerr << ac8Rec.eAC8 << ": チェック中..." << endl;

					// キャッシュ作成
					if(!fnCreateCache8(ac8Rec.eAC8))
					{
						continue;
					}

					// 11桁リストのループ
					for(const auto& ac11Rec: ac8Rec.eAC11Set)
					{
						cerr << ac11Rec.eAC11 << ": チェック中..." << endl;

						// 行政界ポリゴンのループ
						for(const auto& oid : ac11Rec.eOIDSet)
						{
							IFeaturePtr		ipF_CityAdmin;
							ipFC_CityAdmin->GetFeature(oid, &ipF_CityAdmin);
							fnAdminProc( ipF_CityAdmin );
						}

						// CSポイントの検索
						CString addrcode(ac11Rec.eAC11.c_str());
						if(!getCSAddrPoints(addrcode))
						{
							return false;
						}

						fnConvDataSet16();		// 16桁単位でデータ構造を変換
						fnCheckDataSet16();		// 16桁単位でポイント同士の距離チェック

						mAddrTmpSet.clear();
					}
				}
			}
		}
		// [Bug 7210] リスト指定で11桁対応して欲しい に対応 
		else if ( m_iExeMode & ON_ElevenCode )
		{
			// 11桁コードリストのループ
			for(const auto& city : mCityList)
			{
				mAddrTmpSet.clear();

				cerr << city.eCityCode << city.eAddrCode << ": チェック中..." << endl;

				using namespace sindy::schema;

				CString whereClause;
				whereClause.Format(_T("%s = '%s' and %s = '%s' and %s = %d"), 
					city_admin::kCityCode, city.eCityCode,
					city_admin::kAddrCode, city.eAddrCode,
					city_admin::kAddrClass, city_admin::addr_class::kResidenceIndicationAndChibanUndeveloped);

				IFeatureCursorPtr ipFCur_CityAdmin;
				long adminCount = 0;
				if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, adminCount, ipFCur_CityAdmin))
				{
					continue;
				}

				if(adminCount < 1)
				{
					cerr << "行政界ポリゴンがありません" << endl;
					continue;
				}

				// 取得した都市地図行政界ポリゴン上に存在する号ポイントデータを取得
				IFeaturePtr	ipF_CityAdmin;
				while(S_OK == ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) && ipF_CityAdmin)
				{
					if(!fnAdminProc( ipF_CityAdmin ))
					{
						continue;
					}
				}

				CString addrcode;
				addrcode.Format(_T("%s%s"), city.eCityCode, city.eAddrCode);
				if(!getCSAddrPoints(addrcode))
				{
					continue;
				}

				fnConvDataSet16();		// 16桁単位でデータ構造を変換
				fnCheckDataSet16();		// 16桁単位でポイント同士の距離チェック
			}
		}
		else if(m_iExeMode & ON_Addrcode8)
		{
			// 8桁コードリストのループ
			for(const auto city : mCityList)
			{
				mAddrTmpSet.clear();

				cerr << city.eCityCode << city.eAddrCode << ": チェック中..." << endl;

				using namespace sindy::schema;

				// 処理対象の住所8桁コードに合致する都市地図行政界ポリゴンを取得
				// この際、ADDRCODEだけでなく、ADDRCODE2カラムも条件のキーとして指定する
				CString whereClause;
				whereClause.Format(_T("%s = '%s' and (%s like '%s%%' or %s like '%s%%') and %s <> %d"), 
					city_admin::kCityCode, city.eCityCode,
					city_admin::kAddrCode, city.eAddrCode,
					city_admin::kAddrCode2, city.eAddrCode,
					city_admin::kAddrClass, city_admin::addr_class::kAzaUndeveloped);

				IFeatureCursorPtr ipCityAdminFeaCur;
				long adminCount = 0;
				if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, adminCount, ipCityAdminFeaCur))
				{
					continue;
				}

				// 条件に合致する都市地図行政界ポリゴンが存在しない場合は、以降の処理をスキップ
				if(adminCount < 1)
				{
					cerr << "合致する都市地図行政界ポリゴンがありません" << endl;
					continue;
				}

				// 取得した都市地図行政界ポリゴン上に存在する号ポイントデータを取得
				IFeaturePtr	ipCityAdminFea;
				while(S_OK == ipCityAdminFeaCur->NextFeature(&ipCityAdminFea) && ipCityAdminFea)
				{
					if(!getGouPoints( ipCityAdminFea ))
					{
						continue;
					}
				}

				// 処理対象の住所8桁コードに紐付くCSポイントの取得
				CString addrcode;
				addrcode.Format(_T("%s%s"), city.eCityCode, city.eAddrCode);
				if(!getCSAddrPoints(addrcode))
				{
					continue;
				}

				// 住所8桁コード＋地番（名称/コード）単位にデータ構造を変換
				fnConvDataSet8Chiban();

				// 住所8桁コード＋地番（名称/コード）単位で住所ポイント（号/CS）同士の距離チェックを実施
				fnCheckDataSet16();
			}
		}
		else
			return false;
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

bool CheckJusyoDist::getCSAddrPoints(const CString& addrcode)
{
	using namespace sindy::schema;

	// 指定されたaddrcodeに紐付くCSポイントの取得
	CString whereCaluse;
	whereCaluse.Format(_T("%s like '%s%%'"), cs_addr_point::kAddrCode, addrcode); 

	long pointCount = 0;
	IFeatureCursorPtr ipCsAddrPointFeaCur;
	if(!SearchByWhereClause(whereCaluse, ipFC_CSAddrPoint, pointCount, ipCsAddrPointFeaCur))
	{
		return false;
	}

	// 取得されたポイントが0件の場合は、処理せず終了
	if(pointCount == 0)
	{
		return true;
	}

	// CSポイントのループ
	IFeaturePtr	ipCsAddrPointFea;
	while(S_OK == ipCsAddrPointFeaCur->NextFeature(&ipCsAddrPointFea) && ipCsAddrPointFea)
	{
		fnCSAddrProc( ipCsAddrPointFea );
	}

	return true;
}


