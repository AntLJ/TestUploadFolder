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

#include "LogSys.h"
#include "common.h"
#include "BldNameImporter.h"
#include "SurveyPointManager.h"

#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <sindy/workspace.h>
#include <TDC/sindylib_core/Workspace.h>
#include <WinLib/CStringTokenizer.h>

namespace po = boost::program_options;

/**
 * @brief	リスト取得関数
 * @param	filePath [in] リストファイルのパス
 * @param	list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::vector<StrPair>* list)
{
	CString errMsg;
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while(std::getline(ifs, temp)) {
		CString strLine = CA2CT(temp.c_str());
		CStringTokenizer cTokenizer(strLine, _T("\t"), TOKEN_RET_EMPTY_ALL);
		long tokenCount = cTokenizer.CountTokens();
		if (tokenCount != 2) {
			errMsg.Format(_T("置換文字列リストのフォーマットが不正です。(タブ区切りで2フィールドになっていない),%s"), filePath);
			return false;
		}
		StrPair strPair;
		strPair.e_orgStr = cTokenizer.GetNextToken();
		CString replaceStr = cTokenizer.GetNextToken();
		if (replaceStr.CompareNoCase(_T("(削除)")) == 0 || 
			replaceStr.CompareNoCase(_T("（削除)")) == 0 ||
			replaceStr.CompareNoCase(_T("(削除）")) == 0 ||
			replaceStr.CompareNoCase(_T("（削除）")) == 0 ||
			replaceStr.CompareNoCase(_T(" ")) == 0 )
			strPair.e_replaceStr = _T("");
		else
			strPair.e_replaceStr = replaceStr;
		list->push_back(strPair);
	}
	return true;
}

/**
 * @brief	リスト取得関数
 * @param	filePath [in] リストファイルのパス
 * @param	list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::vector<CString>* list)
{
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->push_back(record);
	}
	return true;
}

/**
 * @brief  リスト取得関数
 * @param  filePath [in] リストファイルのパス
 * @param  list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::vector<long>* list)
{
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->push_back(_ttol(record));
	}
	return true;
}

/**
 * @brief	リスト取得関数
 * @param	filePath [in] リストファイルのパス
 * @param	list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::set<CString>* list)
{
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->insert(record);
	}
	return true;
}

/**
 * @brief  リスト取得関数
 * @param  filePath [in] リストファイルのパス
 * @param  list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::set<long>* list)
{
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->insert(_ttol(record));
	}
	return true;
}

/**
 * @brief  リスト取得関数
 * @param  filePath [in] リストファイルのパス
 * @param  list [out] 読み込んだリスト
 * @return true 成功
 * @return false 失敗
 */
bool get_list(LPCTSTR filePath, std::map<long, long>* list)
{
	CString errMsg;
	// ファイルオープン
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%sの読み込みに失敗しました"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// 読み込み
	std::string temp;
	while(std::getline(ifs, temp)) {
		CString strLine = CA2CT(temp.c_str());
		CStringTokenizer cTokenizer(strLine, _T("\t"), TOKEN_RET_EMPTY_ALL);
		long tokenCount = cTokenizer.CountTokens();
		if (tokenCount != 2) {
			errMsg.Format(_T("採用条件記述リストのフォーマットが不正です。(タブ区切りで2フィールドになっていない),%s"), filePath);
			return false;
		}
		long priority, matchPer;
		priority = _ttol(cTokenizer.GetNextToken());
		matchPer = _ttol(cTokenizer.GetNextToken());
		list->insert(std::map<long, long>::value_type(priority, matchPer));
	}
	return true;
}

/**
 * @brief  フィールドインデックス取得関数
 * @param  tableName [in] テーブル名
 * @param  ipTable [in] テーブル
 * @param  indeces [out] インデックス情報
 * @return true 成功
 * @return false 失敗
 */
bool get_field_indeces(LPCTSTR tableName, const ITablePtr& ipTable, std::map<CString, long>* indeces)
{
	CString errMsg;
	indeces->clear();
	// フィールド群取得
	IFieldsPtr ipFields;
	if (FAILED(ipTable->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド群取得に失敗しました"), tableName);
		return false;
	}
	// フィールド数取得
	long fieldCount;
	if (FAILED(ipFields->get_FieldCount(&fieldCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フィールド数取得に失敗しました"), tableName);
		return false;
	}
	// 各フィールド名・フィールドインデックス取得
	for (long i = 0; i < fieldCount; ++i) {
		// フィールド取得
		IFieldPtr ipField;
		if (FAILED(ipFields->get_Field(i, &ipField))) {
			errMsg.Format(_T("フィールド取得に失敗しました。index = %d"), i);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		// フィールド名取得
		CComBSTR fieldName;
		if (FAILED(ipField->get_Name(&fieldName))) {
			errMsg.Format(_T("フィールド名取得に失敗しました。 index = %d"), i);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		// index登録
		indeces->insert(std::pair<CString, long>(CString(fieldName).MakeLower(), i));		
	}
	return true;
}

/**
 * @brief  テーブル取得チェック関数
 * @param  tableName [in] テーブル名
 * @param  ipTable [in] テーブル
 * @return true テーブル取得成功
 * @return false テーブル取得失敗
 */
bool chk_table(LPCTSTR tableName, ITablePtr& ipTable)
{
	if (nullptr == ipTable){
		CString errMsg;
		errMsg.Format(_T("%sのオープンに失敗しました"), tableName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	return true;
}

/**
 * @brief  リコンサイル処理付きの編集終了処理
 * @param  workspace [in] ワークスペース
 * @return HRESULT 
 */
HRESULT StopEditingOrReconcile(sindy::CWorkspace& workspace)
{
	CString errMsg;
	IWorkspaceEditPtr ipWork((IWorkspace*)workspace);
	if (! ipWork)
		return false;

	HRESULT hr = ipWork->StopEditing( VARIANT_TRUE );
	if(FDO_E_VERSION_REDEFINED != hr){
		if( S_OK == hr )
			LogSys::GetInstance().WriteProcLog(true, true, _T("StopEditing(saveEdits=true)....OK\n"));
		else
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("StopEditing(SaveEdits=true)......FAILED!!\n"));
		return hr;
		
	}

	// リコンサイルが必要な場合
	VARIANT_BOOL vbConflicts = VARIANT_FALSE;
	IVersionEdit4Ptr ipVerEdit( ipWork );
	hr = ipVerEdit->Reconcile4( 
		CComBSTR(workspace.GetNameString()->GetVersionName()), 
		VARIANT_TRUE,        // 排他的にリコンサイル処理する
		VARIANT_TRUE,        // コンフリクトが起きたらAbortする
		VARIANT_FALSE,       // TODO: ChildWinsってなに？？
		VARIANT_TRUE,        // カラムレベルでのリコンサイル処理をする
		&vbConflicts );
	switch( hr )
	{
	case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:	errMsg = _T("Reconcile : [FDO_E_RECONCILE_VERSION_NOT_AVAILABLE]"); break;
	case FDO_E_VERSION_BEING_EDITED:			errMsg = _T("Reconcile : [FDO_E_VERSION_BEING_EDITED]"); break;
	case FDO_E_VERSION_NOT_FOUND:				errMsg = _T("Reconcile : [FDO_E_VERSION_NOT_FOUND]"); break;
	case FDO_E_NOT_EDITING:						errMsg = _T("Reconcile : [FDO_E_NOT_EDITING]"); break;
	case S_OK:
		// コンフリクトした場合
		if( vbConflicts )
		{
			errMsg = _T("ERROR : Reconcile : コンフリクトが起きたため、編集を破棄しました。");
			hr = E_FAIL;
		}
		else {
			LogSys::GetInstance().WriteProcLog(true, true, _T("Reconciling...OK\n"));
			hr = StopEditingOrReconcile( workspace ); // リコンサイル後は再度保存処理（StopEditing）が必要
		}
		break;
	default:
		errMsg = _T("ERROR : Reconcile : リコンサイルで原因不明のエラー[%ld]\n");
		break;
	}
	if ( S_OK != hr )
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
	return hr;
}

/**
 * @brief  編集終了関数
 * @param  workspace [in] ワークスペース
 * @param  save [in] 保存の有無　true:保存する false:保存しない
 * @retval true 正常終了
 * @retval false 異常終了
 */
bool StopEdit(sindy::CWorkspace& workspace, bool save)
{
	if (save) {
		// 編集セッション終了
		if (sindy::errorcode::sindyErr_NoErr != workspace.StopEditOperation()) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("編集セッション終了に失敗しました"));
			return false;
		}
		// リコンサイル処理付きの編集終了処理
		if (FAILED(StopEditingOrReconcile(workspace))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("編集終了に失敗しました"));
			return false;
		}
	} else {
		workspace.AbortEditOperation();
		workspace.StopEditing(false);
	}
	return true;
}

/**
 * @brief  編集開始関数
 * @param  workspace [in] ワークスペース
 * @param  nonversioned [in] バージョン対応の有無 true:バージョン日対応 false:バージョン対応
 * @retval true 正常終了
 * @retval false 異常終了
 */
bool StartEdit(sindy::CWorkspace& workspace, bool nonversioned)
{
	// 編集開始
	IMultiuserWorkspaceEditPtr multiWork((IWorkspace*)workspace);
	if (multiWork) {
		if (FAILED(multiWork->StartMultiuserEditing( nonversioned ? esriMESMNonVersioned : esriMESMVersioned ))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("編集開始に失敗しました"));
			return false;
		}
	} else {
		if (sindy::errorcode::sindyErr_NoErr != workspace.StartEditing(false)) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("編集開始に失敗しました"));
			return false;
		}
	}
	// 編集セッション開始
	if ( sindy::errorcode::sindyErr_NoErr != workspace.StartEditOperation()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("編集セッション開始に失敗しました"));
		StopEdit(workspace, false);
		return false;
	}
	LogSys::GetInstance().WriteProcLog(false, true, _T("StartEditing(UndoRedo=false)...OK\n"));
	return true;
}

/**
 * @brief  ワークスペース取得関数
 * @param  connectInfo [in] ワークスペースへのパス(DB名)
 * @param  workspace [out] ワークスペース
 * @retval true 成功
 * @retval false 失敗
 */
bool get_workspace(LPCTSTR connectInfo, sindy::CWorkspace* workspace)
{
	CString errMsg;
	IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectInfo);
	if (! ipWorkspaceName) {
		errMsg.Format(_T("接続文字列が無効です : %s"), connectInfo);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	IUnknownPtr ipUnknown;
	if (FAILED(((INamePtr)ipWorkspaceName)->Open(&ipUnknown))) {
		errMsg.Format(_T("%sの接続に失敗しました"), connectInfo);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	*workspace = IWorkspacePtr(ipUnknown);
	return true;
}

/**
 * @brief 住所DBへの接続
 * @param strAddrDB [in] 接続文字列（<user>@<service> 形式）
 * @param con [out] 接続先DB
 * @retval true  成功
 * @retval false 失敗
 */
bool openAddrDB(const CString& strAddrDB, ADODB::_ConnectionPtr& con)
{
	// 接続文字列からユーザ・サービス名を取得
	int nPos = strAddrDB.Find( _T('@') );
	if( 0 >= nPos )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("住所DB接続文字列が無効です : ") + strAddrDB );
		return false;
	}
	CString strDBUser = strAddrDB.Left( nPos );
	CString strDBSrvc = strAddrDB.Mid( nPos + 1 );
	if( strDBUser.IsEmpty() || strDBSrvc.IsEmpty() )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("住所DB接続文字列が無効です : ") + strAddrDB );
		return false;
	}
	
	// 住所DBに接続
	CString strConnect;
	strConnect.Format( _T(" Provider=OraOLEDB.Oracle; Data Source=%s; User ID=%s; Password=%s; "), strDBSrvc, strDBUser, strDBUser );
	con->ConnectionString = _bstr_t( strConnect );
	try
	{
		if( FAILED( con->Open( "", "", "", ADODB::adConnectUnspecified ) ) )
		{
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("住所DBに接続できません : ") + strAddrDB );
			return false;
		}
	}
	catch( const _com_error& e )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("住所DB接続時に例外発生 : ") + strAddrDB );
		throw e;
	}

	return true;
}

bool exec(const po::variables_map &vmContent, bool isUpdate, bool isSurvey)
{
	// 各種DBへの接続
	sindy::CWorkspace  srcWorkspace, mapWorkspace, addrWorkspace, baseWorkspace, poiWorkspace, twnWorkspace, meshWorkspace;
	if (! get_workspace(vmContent[SRC_DB].as<uh::tstring>().c_str(), &srcWorkspace) ||			// 素材DB
		! get_workspace(vmContent[MAP_DB].as<uh::tstring>().c_str(), &mapWorkspace) ||			// MAPDB
		! get_workspace(vmContent[ADDR_DB].as<uh::tstring>().c_str(), &addrWorkspace) ||		// 行政界DB
		! get_workspace(vmContent[POI_DB].as<uh::tstring>().c_str(), &poiWorkspace) ||			// POIDB
		! get_workspace(vmContent[BASE_DB].as<uh::tstring>().c_str(), &baseWorkspace))			// ベースDB
		return false;

	// TWNDB、住所DBは調査対象ポイント出力モードのみ接続
	ADODB::_ConnectionPtr conAddrDB(__uuidof(ADODB::Connection));
	if (isSurvey) {
		if (! get_workspace(vmContent[MESH_DB].as<uh::tstring>().c_str(), &meshWorkspace))		// メッシュDB
			return false;
		if (! get_workspace(vmContent[TWN_DB].as<uh::tstring>().c_str(), &twnWorkspace))		// TWNDB
			return false;
		if (! openAddrDB(vmContent[AM_DB].as<uh::tstring>().c_str(), conAddrDB))
			return false;
	}
	
	// 各種テーブルのオープン(両モード共通)
	IFeatureClassPtr ipBldSrcPt, ipBld, ipCityAdm, ipPoiPtOrg, ipBldNamePt;
	ITablePtr ipBldAddInfo;
	ipBldSrcPt = srcWorkspace.OpenTable(sindy::schema::sj::buildingname_src_point::kTableName);
	ipBldAddInfo = srcWorkspace.OpenTable(sindy::schema::sj::buildingname_src_addinfo::kTableName);
	ipBld = mapWorkspace.OpenTable(sindy::schema::building::kTableName);
	ipCityAdm = addrWorkspace.OpenTable(sindy::schema::city_admin::kTableName);
	ipPoiPtOrg = poiWorkspace.OpenTable(sindy::schema::sindyk::poi_point_org::kTableName);
	ipBldNamePt = baseWorkspace.OpenTable(sindy::schema::sj::buildingname_point::kTableName);
	if (! chk_table(sindy::schema::sj::buildingname_src_point::kTableName, (ITablePtr)ipBldSrcPt) ||
		! chk_table(sindy::schema::sj::buildingname_src_addinfo::kTableName, ipBldAddInfo) ||
		! chk_table(sindy::schema::building::kTableName, (ITablePtr)ipBld) ||
		! chk_table(sindy::schema::city_admin::kTableName, (ITablePtr)ipCityAdm) ||
		! chk_table(sindy::schema::sindyk::poi_point_org::kTableName, (ITablePtr)ipPoiPtOrg) ||
		! chk_table(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt))
		return false;
	// DB更新用テーブルのオープン
	ITablePtr ipRelBldName;
	if (isUpdate) {
		ipRelBldName = baseWorkspace.OpenTable(sindy::schema::sj::rel_buildingname::kTableName);
		if (! chk_table(sindy::schema::sj::rel_buildingname::kTableName, ipRelBldName))
			return false;
	}
	// 調査対象ポイント出力用のオープン
	IFeatureClassPtr ipTwnpage, ipCityMesh;
	if (isSurvey) {
		ipTwnpage = twnWorkspace.OpenTable(vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str());
		ipCityMesh = meshWorkspace.OpenTable(sindy::schema::citymesh::kTableName);
		if (! chk_table(vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str(), (ITablePtr)ipTwnpage))
			return false;
		if (! chk_table(sindy::schema::citymesh::kTableName, (ITablePtr)ipCityMesh))
			return false;
	}

	// 各種テーブルのインデックス取得
	std::map<CString, long> bldSrcIndeces, bldAddInfoIndeces, bldIndeces, poiPtOrgIndeces, bldNamePtIndeces;
	if (! get_field_indeces(sindy::schema::sj::buildingname_src_point::kTableName, (ITablePtr)ipBldSrcPt, &bldSrcIndeces) ||
		! get_field_indeces(sindy::schema::sj::buildingname_src_addinfo::kTableName, ipBldAddInfo, &bldAddInfoIndeces) ||
		! get_field_indeces(sindy::schema::building::kTableName, (ITablePtr)ipBld, &bldIndeces) ||
		! get_field_indeces(sindy::schema::sindyk::poi_point_org::kTableName, (ITablePtr)ipPoiPtOrg, &poiPtOrgIndeces) ||
		! get_field_indeces(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt, &bldNamePtIndeces))
		return false;
	// DB更新用テーブルのインデックス取得
	std::map<CString, long> relBldNameIndeces;
	if (isUpdate) {
		if (! get_field_indeces(sindy::schema::sj::rel_buildingname::kTableName, ipRelBldName, &relBldNameIndeces))
			return false;
	}
	// 調査対象ポイント出力用テーブルのインデックス取得
	std::map<CString, long> twnpageIndeces, cityMeshIndexes, cityAdminIndexes;
	if (isSurvey) {
		if (! get_field_indeces(vmContent[TWN_DB].as<uh::tstring>().c_str(), (ITablePtr)ipTwnpage, &twnpageIndeces) ||
			! get_field_indeces(sindy::schema::citymesh::kTableName, (ITablePtr)ipCityMesh, &cityMeshIndexes) ||
			! get_field_indeces(sindy::schema::city_admin::kTableName, (ITablePtr)ipCityAdm, &cityAdminIndexes))
			return false;
	}

	// 各種リストの読み込み
	std::set<CString> tgtAddrList;
	std::vector<CString> poiPriList;
	std::vector<CString> supPoiPriList;
	std::vector<long> srcPriListPrior;
	std::vector<long> srcPriListMatch;
	std::set<long> bldExcList;
	std::vector<StrPair> replaceList;
	std::map<long, long> condList;
	if (! get_list(vmContent[TGT_ADDR_LIST].as<uh::tstring>().c_str(), &tgtAddrList) ||			// 処理対象行政界リスト
		! get_list(vmContent[SRC_PRI_LIST_PRIOR].as<uh::tstring>().c_str(), &srcPriListPrior) ||	// 素材優先度種別リスト（優先利用）
		! get_list(vmContent[SRC_PRI_LIST_MATCH].as<uh::tstring>().c_str(), &srcPriListMatch) ||	// 素材優先度種別リスト（マッチング）
		! get_list(vmContent[POI_PRI_LIST].as<uh::tstring>().c_str(), &poiPriList) ||			// 注記POI優先度種別リスト
		! get_list(vmContent[SUP_POI_PRI_LIST].as<uh::tstring>().c_str(), &supPoiPriList) ||	// 補完用注記POI優先度種別リスト
		! get_list(vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str(), &bldExcList) ||			// 処理対象外建物種別リスト
		! get_list(vmContent[REPLACE_LIST].as<uh::tstring>().c_str(), &replaceList) ||			// 置換文字列リスト
		! get_list(vmContent[COND_LIST].as<uh::tstring>().c_str(), &condList))					// 採用条件記述リスト
		return false;

	// 編集開始
	// DB更新モード
	if (isUpdate) {
		if (! StartEdit(baseWorkspace, true))
			return false;
	}

	// 調査対象ポイント出力モード
	SurveyPointManager surveyPtMgr( ipCityMesh, ipCityAdm, cityMeshIndexes, cityAdminIndexes, bldSrcIndeces, twnpageIndeces, vmContent[SOURCENAME].as<uh::tstring>().c_str(), vmContent[SOURCEDATE].as<uh::tstring>().c_str(), conAddrDB );
	if (isSurvey) {
		CString fileName = vmContent[SURVEY_LIST].as<uh::tstring>().c_str();
		if(! surveyPtMgr.open( fileName ) )
		{
			std::cerr << CT2A( fileName ) << " のオープンに失敗" << std::endl;
			return false;
		}
	}

	// インポートクラス定義
	BldNameImporter bldNameImporter(srcPriListPrior,
									srcPriListMatch,
								    poiPriList,
									supPoiPriList,
									replaceList,
									bldExcList,
									condList,
									bldSrcIndeces,
									bldAddInfoIndeces,
									bldIndeces,
									twnpageIndeces,
									poiPtOrgIndeces,
									bldNamePtIndeces,
									relBldNameIndeces,
									ipBldSrcPt,
									ipBldAddInfo,
									ipBld,
									ipCityAdm,
									ipTwnpage,
									ipPoiPtOrg,
									ipBldNamePt,
									ipRelBldName,
									isUpdate,
									isSurvey,
									surveyPtMgr);

	if(! bldNameImporter.Init() ) return false;

	// 行政界コード毎に処理を実施
	for (const auto& addrCode : tgtAddrList) {
		if (! bldNameImporter.Import(addrCode)) {
			LogSys::GetInstance().WriteProcLog(true, false, addrCode + _T("\tNG\n"));
			if (isUpdate) 
				StopEdit(baseWorkspace, false);
			return false;
		} else {
			LogSys::GetInstance().WriteProcLog(true, false, addrCode + _T("\tOK\n"));
		}
	}

	// 編集終了
	// DB更新モード
	if (isUpdate) {
		if (! StopEdit(baseWorkspace, true))
			return false;
	}
	// 調査対象ポイント出力モード
	if (isSurvey) {
		surveyPtMgr.close();
	}
	return true;
}
