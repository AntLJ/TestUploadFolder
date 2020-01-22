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

// CheckBaseRepExist.cpp: CheckBaseRepExist クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/tstring.h>
#include "CheckBaseRepExist.h"

using namespace std;
using namespace sindy::schema;

string	gMessage;

const char* gFieldStr = "\t住所コード２";	// [Bug9463]で追加

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CheckBaseRepExist::CheckBaseRepExist():
	m_fIdGPGouNumber(-1), m_fIdCACityCode(-1), m_fIdCAAddrCode(-1),
	m_fIdCSAddrCode(-1), m_fIdBRAddrCode(-1),
	m_Argc(0), m_Argv(nullptr), m_fpStatList(nullptr)
{
	m_checkMode	= kNone;
}

CheckBaseRepExist::~CheckBaseRepExist()
{

}

bool CheckBaseRepExist::
fnGetEnvVar()
{
#ifdef _DEBUG
	//m_dbAddress = "FIX201511/FIX201511/SDE.DEFAULT/5151/plum";
	m_dbAddress = "\\\\win\\tdc\\ced-ref\\verification\\SiNDY-u\\map\\CheckBaseRepExist\\PGDB\\検証用FGDB.gdb";
	m_fcGouPoint = "GOU_POINT";
	m_fcCityAdmin = "CITY_ADMIN";
	m_fcBaseRep = "BASE_REP_POINT";
	m_fcCSAddrPoint = "CS_ADDR_POINT";
	return true;
#endif

	char	*aTmpStr;

	// TODO: 一通りチェックして、設定されていない環境変数は全部まとめて出力するようにする
	aTmpStr	= getenv("DB_ADDRESS");
	if(!aTmpStr) {
		cout << "環境変数 DB_ADDRESS が設定されていません" << endl;
		cerr << "環境変数 DB_ADDRESS が設定されていません" << endl;
		return false;
	} else {
		m_dbAddress = aTmpStr;
	}

	aTmpStr	= getenv("FC_GOUPOINT");
	if(!aTmpStr) {
		cout << "環境変数 FC_GOUPOINT が設定されていません" << endl;
		cerr << "環境変数 FC_GOUPOINT が設定されていません" << endl;
		return false;
	} else {
		m_fcGouPoint = aTmpStr;
	}

	aTmpStr	= getenv("FC_CITYADMIN");
	if(!aTmpStr) {
		cout << "環境変数 FC_CITYADMIN が設定されていません" << endl;
		cerr << "環境変数 FC_CITYADMIN が設定されていません" << endl;
		return false;
	} else {
		m_fcCityAdmin = aTmpStr;
	}

	aTmpStr	= getenv("FC_BASEREP");
	if(!aTmpStr) {
		cout << "環境変数 FC_BASEREP が設定されていません" << endl;
		cerr << "環境変数 FC_BASEREP が設定されていません" << endl;
		return false;
	} else {
		m_fcBaseRep = aTmpStr;
	}

	aTmpStr	= getenv("FC_CSADDRPOINT");
	if(!aTmpStr) {
		cout << "環境変数 FC_CSADDRPOINT が設定されていません" << endl;
		cerr << "環境変数 FC_CSADDRPOINT が設定されていません" << endl;
		return false;
	} else {
		m_fcCSAddrPoint = aTmpStr;
	}

	return true;

}

bool CheckBaseRepExist::
fnAnalyzeArg()
{
	for(int i = 1; i < m_Argc && m_Argv[i][0] == '-'; i++) {

		switch(m_Argv[i][1]) {

		case 'c':
			switch(m_Argv[i][2]) {
			case 's':
				m_checkMode	= kCitySingle;
				i++;
				if(i < m_Argc) m_cityCode = m_Argv[i];
				else {
					cout << "引数が足りません" << endl;
					cerr << "引数が足りません" << endl;
					return false;
				}
				break;
			case 'l':
				m_checkMode	= kCityList;
				i++;
				if(i < m_Argc) m_fnCityList = m_Argv[i];
				else {
					cout << "引数が足りません" << endl;
					cerr << "引数が足りません" << endl;
					return false;
				}
				break;
			case 'a':
				m_checkMode	= kCityAll;
				break;
			default:
				cout << "不正なオプションです: " << m_Argv[i] << endl;
				cerr << "不正なオプションです: " << m_Argv[i] << endl;
				return false;
			}
			break;

		case 'o':
			i++;
			if(i < m_Argc) m_fnLogFile = m_Argv[i];
			else {
				cout << "引数が足りません" << endl;
				cerr << "引数が足りません" << endl;
				return false;
			}
			break;

		case 's':
			i++;
			if(i < m_Argc) m_fnStatList = m_Argv[i];
			else {
				cout << "引数が足りません" << endl;
				cerr << "引数が足りません" << endl;
				return false;
			}
			break;

		case '?':
			return false;
			break;

		default:
			cout << "不正なオプションです: " << m_Argv[i] << endl;
			cerr << "不正なオプションです: " << m_Argv[i] << endl;
			return false;

		}

	}

	if(m_checkMode == kNone) {
		cout << "オプション -c* が指定されていません" << endl;
		cerr << "オプション -c* が指定されていません" << endl;
		return false;
	}

	return true;

}

bool CheckBaseRepExist::
fnCreateCityList()
{

	ifstream	fin;

	switch(m_checkMode) {

	case kCitySingle:
		m_cityList.insert(m_cityCode);
		break;

	case kCityList:
		char		aBuf[100];
		fin.open(m_fnCityList.c_str());
		if(fin.fail()) {
			cout << "ファイル " << m_fnCityList << " をオープンできません" << endl;
			cerr << "ファイル " << m_fnCityList << " をオープンできません" << endl;
			return false;
		}
		while(!fin.eof()) {
			string	aCityCode;
			fin.getline(aBuf, 100);
			if(fin.fail()) {
				cout << "ファイル " << m_fnCityList << " の読み込みでエラーが発生しました" << endl;
				cerr << "ファイル " << m_fnCityList << " の読み込みでエラーが発生しました" << endl;
				return false;
			}
			aCityCode = aBuf;
			m_cityList.insert(aCityCode.substr(0, 5));
		}
		fin.close();
		break;

	case kCityAll:

		{
			_ICursorPtr	ipCur_CityAdmin;
			_IRowPtr		ipR_CityAdmin;
			ipCur_CityAdmin = SearchByWhereClause(
				uh::str_util::format(_T("%s > '00000'"), city_admin::kCityCode),
				m_fcCityAdmin.c_str(),
				uh::str_util::format(_T("distinct %s"), city_admin::kCityCode));
			while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
				string	aCityCode;
				CComVariant	aTmpVar;
				ipR_CityAdmin->get_Value(0, &aTmpVar);
				aCityCode = ExtractStr(aTmpVar);
				m_cityList.insert(aCityCode);
			}
		}
		break;

	default:
		return false;

	}

	return true;

}

// 実行の初期化
bool CheckBaseRepExist::
Init( void )
{
	if(!fnGetEnvVar())	return false;
	if(!fnAnalyzeArg())	return false;

	if(m_fnLogFile.empty()) {
		cout << "ログ出力先が指定されていません" << endl;
		cerr << "ログ出力先が指定されていません" << endl;
		return false;
	}

	if(m_fnStatList.empty()) {
		cout << "ステータスリスト出力先が指定されていません" << endl;
		cerr << "ステータスリスト出力先が指定されていません" << endl;
		return false;
	}

	if(!Connect(m_dbAddress.c_str()))	return false;

	// フィーチャクラスの取得
	m_ipFcGouPoint		= GetFeatureClass(m_fcGouPoint.c_str());
	if(!m_ipFcGouPoint)	return false;
	m_ipFcCityAdmin		= GetFeatureClass(m_fcCityAdmin.c_str());
	if(!m_ipFcCityAdmin)	return false;
	m_ipFcBaseRep		= GetFeatureClass(m_fcBaseRep.c_str());
	if(!m_ipFcBaseRep)	return false;

	m_ipFcCSAddrPoint	= GetFeatureClass(m_fcCSAddrPoint.c_str());
	if(!m_ipFcCSAddrPoint)	return false;

	// フィールド・インデックスの取得
	if(!GetFieldID(m_ipFcGouPoint,		gou_point::kGouNo,	&m_fIdGPGouNumber))		return false;
	if(!GetFieldID(m_ipFcCityAdmin,		city_admin::kCityCode,		&m_fIdCACityCode))		return false;
	if(!GetFieldID(m_ipFcCityAdmin,		city_admin::kAddrCode,		&m_fIdCAAddrCode))		return false;
	if(!GetFieldID(m_ipFcBaseRep,		base_rep_point::kAddrCode,		&m_fIdBRAddrCode))		return false;
	if(!GetFieldID(m_ipFcCSAddrPoint,	cs_addr_point::kAddrCode,		&m_fIdCSAddrCode))		return false;

	if(!m_jdbAccess.Connect()) {
		cout << m_jdbAccess.eErrMsg << endl;
		cerr << m_jdbAccess.eErrMsg << endl;
		return false;
	}

	// ログファイルのオープン
	if(!m_errLog.Open(m_fnLogFile.c_str())) {
		cout << "ログファイルが開けません" << endl;
		cerr << "ログファイルが開けません" << endl;
		return false;
	}
	if(!m_errLog.SetProgramID(PROGRAM_ID)) {
		cout << "プログラムＩＤを設定できません" << endl;
		cerr << "プログラムＩＤを設定できません" << endl;
		return false;
	}
	if(!m_errLog.SetErrCodeTable(gECTable, ECT_MAX_RECORD)) {
		cout << "エラーコードテーブルを設定できません" << endl;
		cerr << "エラーコードテーブルを設定できません" << endl;
		return false;
	}

	// ステータスリスト(出力)のオープン
	m_fpStatList = fopen(m_fnStatList.c_str(), "w");
	if(!m_fpStatList) {
		cout << "ステータスリストが開けません" << endl;
		cerr << "ステータスリストが開けません" << endl;
		return false;
	}

	// ツール名をログへ出力
	m_errLog.OutputComment("Base_Rep_Point 存在チェック by H.Hyodo");

	// GeoDB接続文字列をログへ出力
	gMessage = "DB_ADDRESS : " + m_dbAddress;
	m_errLog.OutputComment(gMessage.c_str());

	// チェック開始時刻をログへ出力
	m_errLog.OutputTime();
	m_errLog.Flush();

	return true;
}

// 大字／字コードリストの作成
void CheckBaseRepExist::
MakeOazaAzaList( const string& crCityCode, AdminList& crAddrList )
{
	_ICursorPtr	ipCur_CityAdmin;
	ipCur_CityAdmin = SearchByWhereClause(
		uh::str_util::format(_T("%s = '%s' and %s <> '999999'"),
			city_admin::kCityCode, uh::toTStr(crCityCode).c_str(), city_admin::kAddrCode),
		m_fcCityAdmin.c_str(),
		uh::str_util::format(_T("distinct %s"), city_admin::kAddrCode));
	_IRowPtr		ipR_CityAdmin;
	while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
		adminfo	aAdmInfo;
		CComVariant	aAddr1Var;	// 住所コード１
		ipR_CityAdmin->get_Value(0, &aAddr1Var);
		aAdmInfo.mCode6 = ExtractStr(aAddr1Var);
		crAddrList.insert(aAdmInfo);	// 住所コード1のリスト
	}

	// [Bug9463]住所コード２が存在する場合（<>000000）
	ipCur_CityAdmin = SearchByWhereClause(
		uh::str_util::format(_T("%s = '%s' and %s <> '000000'"),
			city_admin::kCityCode, uh::toTStr(crCityCode).c_str(), city_admin::kAddrCode2),
		m_fcCityAdmin.c_str(),
		uh::str_util::format(_T("distinct %s"), city_admin::kAddrCode2));
	while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
		adminfo	aAdmInfo2;
		CComVariant	aAddr2Var;	// 住所コード２
		ipR_CityAdmin->get_Value(0, &aAddr2Var);
		aAdmInfo2.mCode6 = ExtractStr(aAddr2Var);
		aAdmInfo2.mInfoType = adminfo::ADDR2;
		crAddrList.insert(aAdmInfo2);	// 住所コード2のリスト
	}

	// 大字代表コードの追加
	AdminList aOazaRepList;
	AdminList::iterator	aAddrItr;
	for(aAddrItr = crAddrList.begin(); aAddrItr != crAddrList.end(); ++aAddrItr) {
		if("100" < (*aAddrItr).mCode6.substr(3, 3) && (*aAddrItr).mCode6.substr(3, 3) <= "899") {	// [bug 8183](加除コード)対応 2010.08.27
			bool	aIsValid = false;
			adminfo	aOazaRepCode;
			aOazaRepCode.mInfoType = (*aAddrItr).mInfoType;
			aOazaRepCode.mCode6 = (*aAddrItr).mCode6.substr(0, 3) + "000";
			m_jdbAccess.IsValid((crCityCode + aOazaRepCode.mCode6).c_str(), &aIsValid);
			if(aIsValid) {
				aOazaRepList.insert(aOazaRepCode);
			}
		}
	}
	for(aAddrItr = aOazaRepList.begin(); aAddrItr != aOazaRepList.end(); aAddrItr++) {
		crAddrList.insert(*aAddrItr);
	}
}

// Base_Rep_Point のリスト作成
void CheckBaseRepExist::
MakeBaseRepPointList( const string& cCityCode, AOList& cAOListBR )
{
	IFeatureCursorPtr	ipFCur_BaseRep;
	long	aBRCount = 0;
	ipFCur_BaseRep = SearchByWhereClause(
		uh::str_util::format(_T("%s like '%s%%'"), base_rep_point::kAddrCode, uh::toTStr(cCityCode).c_str()),
		m_ipFcBaseRep,
		&aBRCount);
	if(aBRCount > 0) {
		IFeaturePtr		ipF_BaseRep;
		while(ipFCur_BaseRep->NextFeature(&ipF_BaseRep) == S_OK) {
			AORec	aAORec;
			CComVariant	aTmpVar;
			ipF_BaseRep->get_Value(m_fIdBRAddrCode, &aTmpVar);
			aAORec.eAddrCode = ExtractStr(aTmpVar);
			ipF_BaseRep->get_OID(&aAORec.eOID);
			cAOListBR.insert(aAORec);
		}
	}
}

// 11桁行政界の内部に含まれる号のカウントを取得
void CheckBaseRepExist::
GetContainedGouCount( const string& crCityCode,
						 const adminfo& crAdmInfo,
						 bool&	crOazaRepFlag,
						 long& crOID_CityAdmin,
						 long& crGPTotal )
{
	// 同一11桁ポリゴンの検索
	long	aCACount = 0;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	// 住所コード１、２の切り替え
	string	aAddrCodeField;
	if( crAdmInfo.mInfoType == adminfo::ADDR1 ) {
		aAddrCodeField	= city_admin::kAddrCode;
	}else {
		aAddrCodeField	= city_admin::kAddrCode2;
	}

	if(crAdmInfo.mCode6.substr(3, 3) == "000") {
		crOazaRepFlag = true;
		ipFCur_CityAdmin = SearchByWhereClause(
			uh::str_util::format(_T("%s = '%s' and %s like '%s%%'"),
				city_admin::kCityCode, uh::toTStr(crCityCode).c_str(),
				uh::toTStr(aAddrCodeField).c_str(),
				uh::toTStr(crAdmInfo.mCode6.substr(0, 3)).c_str()),
			m_ipFcCityAdmin,
			&aCACount);
	} else {
		ipFCur_CityAdmin = SearchByWhereClause(
			uh::str_util::format(_T("%s = '%s' and %s = '%s'"),
				city_admin::kCityCode, uh::toTStr(crCityCode).c_str(),
				uh::toTStr(aAddrCodeField).c_str(),
				uh::toTStr(crAdmInfo.mCode6).c_str()),
			m_ipFcCityAdmin,
			&aCACount);
	}

	// 検証用データだと、何も取得できない事があり得る
	if(!ipFCur_CityAdmin)
		return;

	// 同一11桁ポリゴンのループ
	IFeaturePtr	ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK && ipF_CityAdmin) {

		ipF_CityAdmin->get_OID(&crOID_CityAdmin);

		IGeometryPtr	ipG_CityAdmin;
		ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

		// 有効な号ポイントのカウント
		long	aGPCount = 0;
		IFeatureCursorPtr	ipFCur_GouPoint;
		ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, m_ipFcGouPoint, &aGPCount);
		if(aGPCount > 0) {
			IFeaturePtr	ipF_GouPoint;
			while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
				CComVariant	aGouVar;	// 号番号
				ipF_GouPoint->get_Value(m_fIdGPGouNumber, &aGouVar);
				if(aGouVar.vt != VT_BSTR) --aGPCount;
			}
		}
		crGPTotal += aGPCount;
	}
}

// 号ポイントのチェック
void CheckBaseRepExist::
CheckGoupoint( const string& crCityCode,
				 const adminfo& crAdmInfo,
				 bool crOazaRepFlag,
				 long crOID_CityAdmin,
				 long crGPTotal )
{
	// 号ポイントのチェック
	gMessage = crCityCode + crAdmInfo.mCode6;
	string	aTmpMessage;
	long	aErrorCode	= 0;
	if(crGPTotal == 0) {
		// 11桁が有効（廃止・無効でない）か
		bool	aIsValid = false;
		m_jdbAccess.IsValid((crCityCode + crAdmInfo.mCode6).c_str(), &aIsValid);
		if(aIsValid) {

			if(crOazaRepFlag) {
				aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_NO_OAZA_REP : ERR_NO_OAZA_REP_ADDR2;
				aTmpMessage	= "代表点なし（大字）";
			} else {
				aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_NO_BASE_REP : ERR_NO_BASE_REP_ADDR2;
				aTmpMessage	= "代表点なし";
			}

		} else {
			aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_INVALID_CODE : ERR_INVALID_CODE_ADDR2;
			aTmpMessage	= "廃止 or 無効";
		}
	} else {
		aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_FROM_GOU_POINT : ERR_FROM_GOU_POINT_ADDR2;
		aTmpMessage	= "代表点あり（号ポイント）";
	}
	// メッセージ出力
	m_errLog.OutputErr(m_fcCityAdmin.c_str(), crOID_CityAdmin, aErrorCode, gMessage.c_str());
	fprintf(m_fpStatList, "%s%s\t%s%s\n", crCityCode.c_str(), crAdmInfo.mCode6.c_str(), aTmpMessage.c_str(), crAdmInfo.mInfoType==adminfo::ADDR1?"":gFieldStr);
}

// Base_Rep_Point に関するチェック
void CheckBaseRepExist::
CheckBaseRepPoint( const AOList& crAOListBR )
{
	AOList::iterator aItr;
	for(aItr = crAOListBR.begin(); aItr != crAOListBR.end(); aItr++) {
		if(("100" < (*aItr).eAddrCode.substr(8, 3) && (*aItr).eAddrCode.substr(8, 3) <= "999") ) {	// [bug 8183](加除コード)対応 2010.08.27
			AOList::iterator aItrF = crAOListBR.find((*aItr).eAddrCode.substr(0, 8) + "000");
			if(aItrF == crAOListBR.end()) {
				gMessage = (*aItr).eAddrCode;
				m_errLog.OutputErr(m_fcBaseRep.c_str(), (*aItr).eOID, ERR_NO_OAZA_REP, gMessage.c_str());
			}
		}
	}
}

// CS_Addr_Point に関するチェック
void CheckBaseRepExist::
CheckCSAddrPoint( const AOList& crAOListBR, const string& crCityCode )
{
	// CS_Addr_Point に関するチェック
	long	aCSCount = 0;
	IFeatureCursorPtr	ipFCur_CSAddrPoint;
	ipFCur_CSAddrPoint = SearchByWhereClause(
		uh::str_util::format(_T("%s like '%s%%'"), cs_addr_point::kAddrCode, uh::toTStr(crCityCode).c_str()),
		m_ipFcCSAddrPoint,
		&aCSCount);
	if(aCSCount > 0) {
		IFeaturePtr	ipF_CSAddrPoint;
		while(ipFCur_CSAddrPoint->NextFeature(&ipF_CSAddrPoint) == S_OK) {
			string	aAddrCode;
			CComVariant	aAddrVar;
			ipF_CSAddrPoint->get_Value(m_fIdCSAddrCode, &aAddrVar);
			aAddrCode = ExtractStr(aAddrVar);
			AOList::iterator aBRItr = crAOListBR.find(aAddrCode.substr(0, 11));
			if(aBRItr == crAOListBR.end()) { // 存在しない → エラー
				long	aOID_CSAddrPoint = 0;
				ipF_CSAddrPoint->get_OID(&aOID_CSAddrPoint);
				gMessage = aAddrCode;
				m_errLog.OutputErr(m_fcCSAddrPoint.c_str(), aOID_CSAddrPoint, ERR_NO_BASE_REP, gMessage.c_str());
			}
		}
	}
}

void CheckBaseRepExist::
CheckAreacodeMaster(const string& cityCode, const AOList& aoList)
{
	list<string> addrCodes;
	if( !m_jdbAccess.GetAddrCodeByCityCode(cityCode, addrCodes) ) {
		// JDB_Access側でエラーメッセージが出るので、ここでは特になにもしない
		return;
	}

	for(const auto& addrCode : addrCodes) {

		if( 0 == addrCode.substr(5,6).compare("000000") ) {
			// 下6桁が000000だったら検索対象外
			continue;
		}

		// 見つからなかったらエラーログへ
		const auto& findResult = find_if(aoList.begin(), aoList.end(),
			[addrCode](const AORec& rec){
				return (0 == addrCode.compare(rec.eAddrCode));
		});
		if( aoList.end() == findResult ) {
			m_errLog.OutputErr("AREACODE_MASTER", 0, ERR_NOT_MATCH_AREACODEMASTER, addrCode.c_str());
		}
	}
}

bool CheckBaseRepExist::
Execute(int cArgc, char **cArgv)
{
	m_Argc	= cArgc;
	m_Argv	= cArgv;

	// 初期化
	if( !Init () ) {
		return false;
	}

	// 市区町村コードリストの作成
	if(!fnCreateCityList()) {
		return false;
	}

	// 市区町村ループ
	for(const auto& aCity : m_cityList) {

		// 大字／字コードリストの作成
		AdminList	aAddrList;
		MakeOazaAzaList( aCity, aAddrList );

		// Base_Rep_Point のリスト作成
		AOList	aAOListBR;
		MakeBaseRepPointList( aCity, aAOListBR );

		// 大字／字コードリストのループ
		for(const auto& aAddr : aAddrList) {

			cout << aCity << aAddr.mCode6 << ": チェック中..." << endl;

			bool	aOazaRepFlag = false;

			// 当該11桁を持つ Base_Rep_Point が存在するか？
			string aAddrCode = aCity + aAddr.mCode6;
			AOList::iterator aBRItr = aAOListBR.find(aAddrCode);
			if(aBRItr != aAOListBR.end()) {	// 存在している → チェック終了
				fprintf(m_fpStatList, "%s%s\t代表点あり%s\n", aCity.c_str(), aAddr.mCode6.c_str(), aAddr.mInfoType==adminfo::ADDR1?"":gFieldStr);
				continue;
			}

			// 同一11桁ポリゴンの検索
			long	aGPTotal = 0;
			long	aOID_CityAdmin = 0;
			GetContainedGouCount( aCity, aAddr, aOazaRepFlag, aOID_CityAdmin, aGPTotal );

			// 号ポイントのチェック
			CheckGoupoint( aCity, aAddr, aOazaRepFlag, aOID_CityAdmin, aGPTotal );

			// ログのフラッシュ
			m_errLog.Flush();
			fflush(m_fpStatList);

		}

		// Base_Rep_Point に関するチェック
		CheckBaseRepPoint( aAOListBR );

		// CS_Addr_Point に関するチェック
		CheckCSAddrPoint( aAOListBR, aCity );

		// Areacode_Master に関するチェック
		CheckAreacodeMaster( aCity, aAOListBR );

		m_errLog.Flush();
	}

	// チェック終了時刻をログへ出力
	m_errLog.OutputTime();

	// ログファイルのクローズ
	m_errLog.Flush();
	m_errLog.Close();
	fclose(m_fpStatList);

	return true;

}
