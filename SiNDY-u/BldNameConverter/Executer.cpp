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
#include "SourceReader.h"
#include "BDNCooker.h"
#include "ShapeMaker.h"

#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>


namespace po = boost::program_options;

/**
 *	@brief メッシュ形状取得関数	
 *	@param mesh [in] 対象メッシュ
 *	@param ipCityMes [in] CityMeshフィーチャクラス
 *	@param ipMeshGeometry [out] 形状
 *	@return true 成功
 *	@return false 失敗
 */
bool get_mesh_geometry(long mesh, const IFeatureClassPtr& ipCityMesh, IGeometryPtr& ipMeshGeometry)
{
	CString errMsg;
	// where句作成
	CString whereClause;
	whereClause.Format(_T("%s = %d"), sindy::schema::citymesh::kCityMeshCode, mesh);
	// filter作成	
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
		errMsg.Format(_T("フィルタ作成に失敗しました"), whereClause);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// cursor取得
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(ipCityMesh->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor))) {
		errMsg.Format(_T("カーソル取得に失敗しました：%s"), whereClause);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// フィーチャ取得
	IFeaturePtr ipFeature;
	if (ipFeatureCursor->NextFeature(&ipFeature) != S_OK || ipFeature == NULL) {
		errMsg.Format(_T("対象メッシュ形状が存在しません:%d"), mesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName);
		return false;
	}
	// 形状取得
	if (FAILED(ipFeature->get_ShapeCopy(&ipMeshGeometry))) {
		long oid;
		ipFeature->get_OID(&oid);
		errMsg.Format(_T("メッシュ形状の取得に失敗しました：%d"), mesh);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::citymesh::kTableName, oid);
		return false;
	}
	return true;
}

/**
 *	@biref リスト取得関数
 *	@param filePath [in] リストファイルのパス	
 *	@parma list [out] 読み込んだリスト
 *	@return true 成功
 *	@return false 失敗	
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
 *	@brief リスト取得関数	
 *	@param filePath [in] リストファイルのパス
 *	@param list [out] 読み込んだリスト
 *	@return true 成功	
 *	@return false 失敗
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

bool exec(const po::variables_map &vmContent, bool isTest)
{
	// 各種DBへの接続
	IWorkspacePtr ipBaseWorkspace, ipMapWorkspace, ipPoiWorkspace, ipMeshWorkspace;
	if (! common_proc::GetWorkspace(vmContent[BASE_DB].as<uh::tstring>().c_str(), ipBaseWorkspace) ||
		! common_proc::GetWorkspace(vmContent[MAP_DB].as<uh::tstring>().c_str(), ipMapWorkspace) ||
		! common_proc::GetWorkspace(vmContent[MESH_DB].as<uh::tstring>().c_str(), ipMeshWorkspace))
		return false;
	// 各種テーブルオープン
	IFeatureClassPtr ipBldNamePt, ipBld, ipCityMesh;
	if (! common_proc::GetTable(sindy::schema::sj::buildingname_point::kTableName, ipBaseWorkspace, ipBldNamePt) ||
		! common_proc::GetTable(sindy::schema::building::kTableName, ipMapWorkspace, ipBld) ||
		! common_proc::GetTable(sindy::schema::citymesh::kTableName, ipMeshWorkspace, ipCityMesh))
		return false;
	// 各種テーブルインデックス取得
	FieldIndexMap bldNamePtIndeces, bldIndeces;
	if (! common_proc::GetFieldIndeces(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt, &bldNamePtIndeces) ||
		! common_proc::GetFieldIndeces(sindy::schema::building::kTableName, (ITablePtr)ipBld, &bldIndeces))
		return false;
	// メッシュリスト読込
	std::set<long> meshList;
	if (! get_list(vmContent[MESH_LIST].as<uh::tstring>().c_str(), &meshList))
		return false;
	// 処理対象外建物種別リスト読み込み
	std::set<long> bldExcList;
	if (! get_list(vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str(), &bldExcList))
		return false;
	// 出力ディレクトリ+Citymap取得 ⇒　存在しない場合は作成
	CString outRootDir;
	outRootDir.Format(_T("%s\\Citymap\\"), vmContent[OUTPUT_DIR].as<uh::tstring>().c_str());
	if (! MakeSureDirectoryPathExists(CT2CA(outRootDir))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("出力ディレクトリ作成に失敗しました"));
		return false;
	}
	//　検証形状出力用 GeometryDef取得(出力はポイントなので、建物ビル名称ポイントのものを取得)
	IGeometryDefPtr ipGeometryDef;
	if (! common_proc::GetGeometryDef(sindy::schema::sj::buildingname_point::kTableName, ipBldNamePt, ipGeometryDef))
		return false;
	// 処理クラス定義
	SourceReader sourceReader(ipBldNamePt, ipBld, &bldNamePtIndeces, &bldIndeces, &bldExcList);
	BDNCooker bdnCooker(&bldNamePtIndeces);
	ShapeMaker shpMaker(outRootDir, isTest, ipGeometryDef);
	// 都市地図メッシュ単位での処理	
	bool retval = true;
	CString procMsg;
	for (const auto& cityMesh : meshList)
	{
		procMsg.Format(_T("%d...\t"), cityMesh);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// メッシュ形状取得
		IGeometryPtr ipMeshGeometry;
		if (! get_mesh_geometry(cityMesh, ipCityMesh, ipMeshGeometry)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// 不正なデータが存在したかチェックする変数
		bool existWrongDataRead = true;
		// データ取得
		IDFeatureMap bldFeatures;
		IDFeatureMap bldNamePtFeatures;
		IDFeatureMap bldNamePtIDs;
 		if (! sourceReader.readSource(ipMeshGeometry, &bldFeatures, &bldNamePtFeatures, &bldNamePtIDs, &existWrongDataRead)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// 不正なデータが存在したかチェックする変数
		bool existWrongDataCnv = true;
		// データ加工
		IDRecordMap cnvInfo;
		if (! bdnCooker.cookBDN(ipMeshGeometry, bldFeatures, bldNamePtFeatures, &bldNamePtIDs, &cnvInfo, &existWrongDataCnv)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		// Shape出力
		if (! shpMaker.makeShape(cnvInfo, cityMesh)) {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG\n"));
			continue;
		}
		if (existWrongDataRead && existWrongDataCnv) {
			LogSys::GetInstance().WriteProcLog(true, false, _T("OK\n"));
		} else {
			retval = false;
			LogSys::GetInstance().WriteProcLog(true, false, _T("NG(不正データ未変換)\n"));
		}
	}
	return retval;
}
