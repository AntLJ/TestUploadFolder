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

// CheckWNRelPoiPoint.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <sindy/libschema.h>
#include <sindy/workspace.h>
#include <crd_cnv\coord_converter.h>
#include "CheckWNRelPoiPoint.h"
#ifdef _DEBUG_DISTANCE
#include <iomanip>
#endif _DEBUG_DISTANCE

CCheckWNRelPoiPoint::CCheckWNRelPoiPoint(CParam& cParam):m_isError(false) {
	// パラメータ情報保持
	m_CParam = cParam;
}

CCheckWNRelPoiPoint::~CCheckWNRelPoiPoint() {
}

bool CCheckWNRelPoiPoint::execute() {
	// 初期処理(各データ読込)
	if (!init()) {
		// 異常終了
		return false;
	}

	// メッシュ毎にチェック処理
	std::vector<std::string>::iterator itrMesh = m_vecMeshList.begin();
	for (; itrMesh != m_vecMeshList.end(); ++itrMesh) {
		// 改行なしでメッシュコード表示
		CLog::WriteRunLogNotNewLine((boost::format("%s\t") % itrMesh->c_str()).str());

		std::string shapeDir = makeShapeDirName(itrMesh->c_str());
		bool bContinue = false;
		// チェック(ⅱ,ⅲ,ⅳ)
		if (!checkRelation(shapeDir, itrMesh->c_str(), m_mapRelTbl, m_mapPoiData, bContinue)) {
			if (bContinue) {
				RUN_LOG(DSP_NG);
				m_isError = true;
				continue;
			}
			else {
				// 異常終了
				RUN_LOG(DSP_HYPHEN);
				return false;
			}
		}
			
		RUN_LOG(DSP_OK);
	}

	// 最後に関連テーブル過多チェック(ⅴ)
	if (!checkRelTbl()) {
		m_isError = true;
	}

	return true;
}

bool CCheckWNRelPoiPoint::init() {
	std::string sLayer, sNid, sPid, sMesh, sErrMsg;		// エラーログ表示用

	CRelPoiTbl cRelPOITbl;
	// 関連テーブル読込＆チェック(ⅰ)
	if (!cRelPOITbl.readRelTbl(m_CParam.getRelTbl(), m_mapRelTbl)) {
		return false;
	}

	std::set<std::string> setCodeList;					// コードリストコンテナ 
	// コンテンツコードリスト読込
	if (!nsCommon::loadList2set(m_CParam.getCodeList().c_str(), setCodeList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}
	
	CPoiFile cPOIFile;
	// POIファイル読込
	if (!cPOIFile.readPoiList(m_CParam.getPoiList(), setCodeList, m_mapPoiData)) {
		return false;
	}

	// メッシュリスト読込
	if (!nsCommon::loadList2vec(m_CParam.getMeshList().c_str(), m_vecMeshList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}
	// メッシュコードチェック
	if (!checkMeshCode()) {
		return false;
	}

	// 世界測地系パラメータ取得
	if (!m_Ccrd_cnv.JGD2000_RP_Init((char*)m_CParam.getParam().c_str())) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("世界測地系変換用パラメータ取得失敗, file=%s") % m_CParam.getParam().c_str()).str());
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::checkMeshCode() {
	std::string sLayer, sNid, sPid;		// エラーログ表示用
	bool bRet = true;

	std::vector<std::string>::iterator itrMesh = m_vecMeshList.begin();
	for (; itrMesh != m_vecMeshList.end(); ++itrMesh) {
		CString csMeshCode(itrMesh->c_str());
		// メッシュコード数値チェック
		if (csMeshCode != csMeshCode.SpanIncluding(CHECK_INTEGER_STR)) {
			ERR_LOG(sLayer, sNid, sPid, itrMesh->c_str(), "メッシュコード不正");
			bRet = false;
			continue;
		}

		int iMeshCode = atoi(itrMesh->c_str());
		// メッシュコード範囲チェック
		if (iMeshCode < 100000 || 1000000 <= iMeshCode) {
			ERR_LOG(sLayer, sNid, sPid, itrMesh->c_str(), "メッシュコード不正");
			bRet = false;
			continue;
		}
	}

	return bRet;
}

std::string CCheckWNRelPoiPoint::makeShapeDirName(const std::string& strMesh) {
	int iMeshCode = atoi(strMesh.c_str());
	std::stringstream	ssDirPath;

	ssDirPath << m_CParam.getWnDir().c_str() << "\\" << iMeshCode/100 << "\\" << iMeshCode;

	return ssDirPath.str();
}

bool CCheckWNRelPoiPoint::checkRelation(const std::string& sShapeDir,
										const std::string& sMeshcode,
										const MAP_REL_TBL& mapRelTbl,
										const MAP_POI_DATA& mapPoiData,
										bool& bContinue) {
	const std::string sLayerDef, sNidDef, sPidDef;	// エラーログ出力(指定無し)用

	CString csLayerName;
	CString csMeshCode(sMeshcode.c_str());
	csLayerName.Format(_T("%s_%s"), (LPCTSTR)csMeshCode, MWN_LAYER);
	std::string sLayer = (boost::format("%s") % CT2CA(csLayerName)).str();	// エラーログ出力用レイヤ名

	CString csShapeFile;
	CString csShapeDir(sShapeDir.c_str());
	csShapeFile.Format(_T("%s\\%s.dbf"), (LPCTSTR)csShapeDir, (LPCTSTR)csLayerName);

	// 道路・歩行者ノードデータ存在チェック
	if (!PathFileExists(csShapeFile)) {
		ERR_LOG(sLayerDef, sNidDef, sPidDef, sMeshcode, "対象メッシュの道路・歩行者ノードデータなし");
		bContinue = true;
		return false;
	}

	// 道路・歩行者ノードデータ情報取得

	IWorkspacePtr ipWorkspace = sindy::create_workspace(csShapeDir);
	if (!ipWorkspace) {
		ERR_LOG(sLayerDef, sNidDef, sPidDef, sMeshcode, (boost::format("ワークスペース接続失敗, dir=%s") % CT2CA(csShapeDir) ).str());
		return false;
	}

	IFeatureClassPtr ipFetureclass;
	if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(csLayerName), &ipFetureclass))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, "オープン失敗");
		return false;
	}

	long oidIndex = 0;
	std::string sErrMsg;	// エラーログ出力用エラーメッセージ
	if (!nsCommon::getFieldIndex(ipFetureclass, shp05::schema::kObjectId, oidIndex, sErrMsg)) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, sErrMsg);
		return false;
	}

	CString csWhere;
	csWhere.Format(_T("%s%s"), SQL_NODECLS, CA2CT(m_CParam.getNodeClass().c_str()));
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(csWhere)))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, (boost::format("SQL設定失敗, where句[%s]") % CT2CA(csWhere)).str());
		return false;
	}

	IFeatureCursorPtr ipFeatureCursor;
	// NODECLASS_C が設定ファイルの値と一致するレコードを検索
	if (FAILED(ipFetureclass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor))) {
		ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, "カーソル取得失敗");
		return false;
	}

#ifdef _DEBUG_DISTANCE
	std::cout << "NODE\tPOI\tMESH\tDIST" << std::endl;
#endif _DEBUG_DISTANCE

	std::string sRelTblLayer = nsCommon::getLayerName(m_CParam.getRelTbl());	// エラーログ出力用関連テーブルレイヤ名
	IFeaturePtr ipFeature;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
		CComVariant objectId;
		// NODECLASS_C が設定ファイルの値と一致するレコードのオブジェクトIDを取得
		if (FAILED(ipFeature->get_Value(oidIndex, &objectId))) {
			ERR_LOG(sLayer, sNidDef, sPidDef, sMeshcode, (boost::format("値取得失敗, field=%s")
					% shp05::schema::kObjectId).str());
			return false;
		}
		objectId.ChangeType(VT_I4);
		std::string sNid = std::to_string(objectId.lVal);	// エラーログ出力用NODE ID

		auto iteRelTbl = m_mapRelTbl.find(objectId.lVal);
		// 取得したobjectIdがチェック対象関連テーブルコンテナのキーにあるかチェック
		if (iteRelTbl == m_mapRelTbl.end()) {
			// 【エラーⅱ】対象ノード種別(WKNDCLS_C)のOBJCTIDが関連テーブルに存在しない。
			ERR_LOG(sRelTblLayer, sNid, sPidDef, sMeshcode, "関連テーブルに対象ノードIDの存在無し");
			bContinue = true;
			continue;
		}

		// 対象キーの要素(関連POI)を取得
		long lPoiId = iteRelTbl->second;
		std::string sPid = std::to_string(lPoiId);			// エラーログ出力用POI ID

		// 道路・歩行者ノードコンテナに詰込む
		// ※要素のPOI_IDは道路・歩行者ノードのデータではないが、のちの比較を容易にするため便宜上入れる
		m_mapMwnChk.insert(MAP_REL_TBL::value_type(objectId.lVal, lPoiId));
		
		// 関連POIがPOIデータコンテナにあるかチェック
		auto itePoi = m_mapPoiData.find(lPoiId);
		if (itePoi == m_mapPoiData.end()) {
			// 【エラーⅲ】関連POIがPOIファイルに存在しない。
			ERR_LOG(sRelTblLayer, sNid, sPid, sMeshcode, "関連テーブルに存在する対象POI IDがPOIファイルに存在無し");
			bContinue = true;
			continue;
		}
		// 対象キーの要素(経度・緯度)を取得
		WKSPoint poiPoint = itePoi->second;

		IGeometryPtr ipGeometry;
		// 道路・歩行者ノードの形状取得
		if (FAILED(ipFeature->get_ShapeCopy(&ipGeometry))) {
			ERR_LOG(sLayer, sNid, sPid, sMeshcode, "形状取得失敗");
			return false;
		}
		
		WKSPoint wnPoint;
		if (!getPoint(ipGeometry, wnPoint)) {
			ERR_LOG(sLayer, sNid, sPid, sMeshcode, "座標取得失敗");
			return false;
		}

#ifdef _DEBUG_DISTANCE
		std::cout << "\t" << sNid << "\t" << sPid << "\t" << sMeshcode << "\t" << std::flush;
#endif _DEBUG_DISTANCE
		double dDistance = 0;
		if (!checkDiscance(poiPoint, wnPoint, dDistance)) {
			// 【エラーⅳ】道路・歩行者ノードと関連付くPOIの距離が閾値以下かチェック
			ERR_LOG(sRelTblLayer, sNid, sPid, sMeshcode, (boost::format("ノード⇔POI間距離閾値超過, distance=%.10f") % dDistance).str());
			bContinue = true;
			continue;
		}
	}

	if (bContinue) {
		return false;
	}
	return true;
}

bool CCheckWNRelPoiPoint::checkRelTbl() {
	if (m_mapRelTbl != m_mapMwnChk ) {
		auto iteRelTbl = m_mapRelTbl.begin();
		// 取得したobjectIdがチェック対象関連テーブルコンテナのキーにあるかチェック
		for (; iteRelTbl != m_mapRelTbl.end(); ++iteRelTbl) {
			auto iteMwnChk = m_mapMwnChk.find(iteRelTbl->first);
			if (iteMwnChk == m_mapMwnChk.end()) {
				std::string sLayer = nsCommon::getLayerName(m_CParam.getRelTbl());
				std::string sNid = std::to_string(iteRelTbl->first);
				std::string sPid = std::to_string(iteRelTbl->second);
				std::string sMesh;
				// 【エラーⅴ】対象メッシュの道路・歩行者ノードの対象OBJECTID以外のノードIDが関連テーブルに存在する。
				ERR_LOG(sLayer, sNid, sPid, sMesh, "関連テーブルに対象外ノードIDの存在有り");
			}
		}
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::getPoint(const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint) {
	esriGeometryType aGeomType;
	if (FAILED(ipGeometry->get_GeometryType(&aGeomType))) {
		return false;
	}

	if (esriGeometryPoint != aGeomType) {
		return false;
	}

	IPointPtr ipPoint(ipGeometry);
	if (FAILED(ipPoint->get_X(&(rWKSPoint.X))) || FAILED(ipPoint->get_Y(&(rWKSPoint.Y)))) {
		return false;
	}

	return true;
}

bool CCheckWNRelPoiPoint::checkDiscance(const WKSPoint& poiPoint, const WKSPoint& wnPoint, double& dDistance) {
	dDistance = m_Ccrd_cnv.GetDist_JGD2000(poiPoint.X, poiPoint.Y, wnPoint.X, wnPoint.Y);
#ifdef _DEBUG_DISTANCE
	std::cout << std::fixed << std::setprecision(10) << dDistance << std::endl;
#endif _DEBUG_DISTANCE
	if (dDistance > std::stod(m_CParam.getDistance().c_str())) {
		return false;
	}

	return true;
}
