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

#include "stdafx.h"
#include "CheckDlcBld.h"

bool CCheckDlcBld::init(const Arguments& args)
{
	IWorkspacePtr ipWorkspace;
	if(!GetWorkspace(args.strSpatialDB.c_str(), ipWorkspace))
		return false;
	IFeatureClassPtr ipBuildingClass;
	if(!GetFeatureClass(sindy::schema::building::kTableName, ipWorkspace, ipBuildingClass))
		return false;

	CString strSpFC = sindy::schema::building::kTableName;
	if(!args.strSpatialDBSchema.empty())
		strSpFC = CString(args.strSpatialDBSchema.c_str()) + _T(".") + strSpFC;

	if(!GetSpatialReference(strSpFC, ipBuildingClass, m_ipSpRef))
		return false;

	if(!getMeshlist(args.strMeshlist.c_str(), m_setMesh))
		return false;

	m_strShapeDir = args.strShapeDir.c_str();

	return true;
}

bool CCheckDlcBld::getMeshlist(const CString& strMeshlist, std::set<CString>& setMesh)
{
	CString strMsg;
	std::ifstream ifs(strMeshlist);
	if(ifs.fail()){
		strMsg.Format(_T("メッシュリストオープン失敗"));
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg, strMeshlist);
		return false;
	}

	bool bMesh = true;
	char szLine[128];
	while(ifs.getline(szLine,128)){
		CString strMeshCode(szLine);
		if(strMeshCode.IsEmpty() || strMeshCode.Left(1) == _T("#"))
			continue;

		strMeshCode.Trim(_T(" "));
		strMeshCode.Trim(_T("　"));

		// メッシュリストのメッシュコードは8桁（都市地図メッシュ）でないとNG
		if(8 != strMeshCode.GetLength()){
			strMsg.Format(_T("都市地図メッシュコードでない\t%s"), strMeshCode);
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			bMesh = false;
			continue;
		}
		// 数字のみで構成されていること
		if(0 != strMeshCode.Compare(strMeshCode.SpanIncluding(_T("0123456789")))){
			strMsg.Format(_T("メッシュコードに数字以外が使用されている\t%s"), strMeshCode);
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			bMesh = false;
			continue;
		}
		setMesh.insert(strMeshCode);
	}
	return bMesh;
}

bool CCheckDlcBld::run(bool& bErr)
{
	bErr = false;

	long lCounter = 0, lMeshSize = m_setMesh.size();
	for(const auto mesh : m_setMesh){
		lCounter++;
		CString strPrint;
		strPrint.Format(_T("#%s   (%ld / %ld 件)"), mesh, lCounter, lMeshSize);
		CLog::GetInstance().PrintLog(true, false, true, false, strPrint);

		// 都市地図メッシュのshapefileパスを取得
		CString strWorkspacePath = getShapeWorkspacePath(mesh);

		// ワークスペース取得
		IWorkspacePtr ipWork;
		if(!GetWorkspace(strWorkspacePath, ipWork)){
			bErr = true;
			continue;
		}

		// 建物レイヤ、装飾線レイヤ取得
		CString strBldName, strDlcName;
		strBldName.Format(_T("%s_%s"), mesh, shp05::schema::citymap::building::kSuffix);
		strDlcName.Format(_T("%s_%s"), mesh, shp05::schema::citymap::bld_line::kSuffix);
		IFeatureClassPtr ipBldClass, ipDlcClass;
		if(!GetFeatureClass(strBldName, ipWork, ipBldClass) ||
			!GetFeatureClass(strDlcName, ipWork, ipDlcClass)){
			bErr = true;
			continue;
		}

		// 建物レイヤのOBJECTIDフィールドインデックス取得
		long lBldObjectidIndex = -1;
		IFieldsPtr ipBldFields;
		if(!GetFields(strDlcName, ipBldClass, ipBldFields)){
			bErr = true;
			continue;
		}
		if(!GetFieldIndex(strDlcName, shp05::schema::kObjectId, ipBldFields, lBldObjectidIndex) || 0 > lBldObjectidIndex){
			bErr = true;
			continue;
		}

		// 装飾線レイヤの建物IDフィールドインデックス取得
		long lDlcBldIdIndex = -1;
		IFieldsPtr ipDlcFields;
		if(!GetFields(strDlcName, ipDlcClass, ipDlcFields)){
			bErr = true;
			continue;
		}
		if(!GetFieldIndex(strDlcName, shp05::schema::citymap::bld_line::kBldId, ipDlcFields, lDlcBldIdIndex) || 0 > lDlcBldIdIndex){
			bErr = true;
			continue;
		}

		// 建物データを予めマッピングしておく
		IFeatureCursorPtr ipBldCursor;
		if(!GetFeatureCursor(strBldName, ipBldClass, NULL, ipBldCursor)){
			bErr = true;
			continue;
		}
		std::map<long,CAdapt<IFeaturePtr>> mapBldFeature;
		IFeaturePtr ipBld;
		while(S_OK == ipBldCursor->NextFeature(&ipBld) && ipBld){
			// 建物フィーチャのOBJECTID取得
			CComVariant vaBldObjectId;
			if(!GetValue(strBldName, shp05::schema::kObjectId, ipBld, lBldObjectidIndex, vaBldObjectId)){
				bErr = true;
				continue;
			}
			mapBldFeature.insert(std::make_pair(vaBldObjectId.lVal, ipBld));
		}

		// 装飾線レイヤ基準でフィーチャ取得ループ
		IFeatureCursorPtr ipDlcCursor;
		if(!GetFeatureCursor(strDlcName, ipDlcClass, NULL, ipDlcCursor)){
			bErr = true;
			continue;
		}
		IFeaturePtr ipDlc;
		while(S_OK == ipDlcCursor->NextFeature(&ipDlc) && ipDlc){
			// 装飾線フィーチャのFID取得
			long lDlcFID = 0;
			if(!GetOID(strDlcName, ipDlc, lDlcFID)){
				bErr = true;
				continue;
			}

			// 装飾線フィーチャの建物ID取得
			CComVariant vaBldId;
			if(!GetValue(strDlcName, shp05::schema::citymap::bld_line::kBldId, ipDlc, lDlcBldIdIndex, vaBldId)){
				bErr = true;
				continue;
			}
			// 装飾線の形状取得
			IGeometryPtr ipDlcGeom;
			if(!GetGeometry(strDlcName, ipDlc, ipDlcGeom)){
				bErr = true;
				continue;
			}
			// 装飾線の空間参照付与
			if(!SetSpatialReference(strDlcName, m_ipSpRef, ipDlcGeom)){
				bErr = true;
				continue;
			}
			std::vector<IGeometryPtr> vecDlcSegGeom;
			if(!GetSegmentCenterPoints(strDlcName, lDlcFID, ipDlcGeom, vecDlcSegGeom)){
				bErr = true;
				continue;
			}

			// 建物IDの建物フィーチャ取得
			const auto itBldFeature = mapBldFeature.find(vaBldId.lVal);
			if(mapBldFeature.cend() == itBldFeature){
				// ここに来る時点でエラーだが、エラー内容をさらに細分化する
				// 取得できない場合は近隣の建物フィーチャを検索してみる
				if(!ArouondBld(strBldName, vaBldId.lVal, ipBldClass, lBldObjectidIndex, strDlcName, lDlcFID, ipDlcGeom, vecDlcSegGeom, false, false)){
					// 近隣の建物フィーチャにも内包されない場合
					CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgExistBld2, strDlcName, lDlcFID, strBldName, std::to_string(vaBldId.lVal).c_str());
				} else {
					// 近隣の建物フィーチャに内包された場合
					CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgExistBld3, strDlcName, lDlcFID, strBldName, std::to_string(vaBldId.lVal).c_str());
				}
				bErr = true;
				continue;
			}

			// 建物の形状取得
			IGeometryPtr ipBldGeom;
			if(!GetGeometry(strBldName, itBldFeature->second.m_T, ipBldGeom)){
				bErr = true;
				continue;
			}
			// 建物の空間参照付与
			if(!SetSpatialReference(strBldName, m_ipSpRef, ipBldGeom)){
				bErr = true;
				continue;
			}

			// 形状マッチしているかチェック（簡易版）
			bool bMatch = false;
			if(!MatchingBldDlc(strBldName, vaBldId.lVal, ipBldGeom, strDlcName, lDlcFID, ipDlcGeom, bMatch)){
				bErr = true;
				continue;
			}
			// マッチしていれば次の装飾線へ
			if(bMatch)
				continue;

			// 形状マッチしているかチェック（詳細版）
			std::vector<IGeometryPtr> vecDlcNotMatchSegGeom;
			if(!MatchingBldDlcSegment(strBldName, vaBldId.lVal, ipBldGeom, strDlcName, lDlcFID, vecDlcSegGeom, vecDlcNotMatchSegGeom)){
				bErr = true;
				continue;
			}
			// マッチしていれば次の装飾線へ
			if(vecDlcNotMatchSegGeom.empty())
				continue;

			// 一部だけ一致しているかどうか
			bool bSomeMatch = false;
			if(vecDlcSegGeom.size() != vecDlcNotMatchSegGeom.size()){
				bSomeMatch = true;
			}

			// 近隣の建物ポリゴンに紐付いているかチェック
			if(!ArouondBld(strBldName, vaBldId.lVal, ipBldClass, lBldObjectidIndex, strDlcName, lDlcFID, ipDlcGeom, vecDlcNotMatchSegGeom, bSomeMatch)){
				bErr = true;
				continue;
			}
		}
	}
	return true;
}

CString CCheckDlcBld::getShapeWorkspacePath(const CString& strMesh)
{
	CString strPath;
	strPath.Format(_T("%s\\%s\\%s\\%s"), m_strShapeDir, strMesh.Left(4), strMesh.Left(6), strMesh);
	return strPath;
}

bool CCheckDlcBld::MatchingBldDlc(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, bool& bMatch)
{
	// マッチングフラグを初期化
	bMatch = false;

	// 建物をベースに装飾線がContainsしているか
	IRelationalOperatorPtr ipBldRelOp(ipBldGeom);
	if(!ipBldRelOp){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IRelationalOperatorへ変換の失敗"));
		return false;
	}
	VARIANT_BOOL vaIsContains = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Contains(ipDlcGeom, &vaIsContains))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcContains, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Containsしていれば終了
	if(vaIsContains){
		bMatch = true;
		return true;
	}

	// 建物をベースに装飾線がDisjointしているか
	VARIANT_BOOL vaIsDisjoint = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Disjoint(ipDlcGeom, &vaIsDisjoint))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcDisjoint, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Disjointしていれば終了（近隣建物を検索させる）
	if(vaIsDisjoint)
		return true;

	// Disjointしていない場合は何かしら接しているはずなので細かく確認する

	// 建物をベースに装飾線がCrossesしているか
	VARIANT_BOOL vaIsCrosses = VARIANT_FALSE;
	if(FAILED(ipBldRelOp->Crosses(ipDlcGeom, &vaIsCrosses))){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcCross, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}
	// Crossしていれば不正形状で終了
	if(vaIsCrosses){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcCross, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
		return false;
	}

	return true;
}

bool CCheckDlcBld::MatchingBldDlcSegment(const CString& strBldName, long lBldOid, IGeometryPtr ipBldGeom, const CString& strDlcName, long lDlcFid, const std::vector<IGeometryPtr>& vecDlcSegGeom, std::vector<IGeometryPtr>& vecNotMatchDlcSegGeom)
{
	// 建物をベースに装飾線がContainsしているか
	IRelationalOperatorPtr ipBldRelOp(ipBldGeom);
	if(!ipBldRelOp){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IRelationalOperatorへ変換の失敗"));
		return false;
	}

	for(long l = 0; l < vecDlcSegGeom.size(); ++l){
		IGeometryPtr ipDlcSegCenterGeom = vecDlcSegGeom.at(l);
		// 建物をベースにこの中点がContainsしているか
		VARIANT_BOOL vaIsCenterContains = VARIANT_FALSE;
		if(FAILED(ipBldRelOp->Contains(ipDlcSegCenterGeom, &vaIsCenterContains))){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcContains, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
			return false;
		}
		// Containsしていれば次のセグメントへ
		if(vaIsCenterContains)
			continue;

		// 建物をベースに中点がTouchesしているか
		VARIANT_BOOL vaIsCenterTouches = VARIANT_FALSE;
		if(FAILED(ipBldRelOp->Touches(ipDlcSegCenterGeom, &vaIsCenterTouches))){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, err_code::NgBldDlcTouches, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
			return false;
		}
		// Touchesしていない場合は未マッチングコンテナへ格納
		if(!vaIsCenterTouches)
			vecNotMatchDlcSegGeom.push_back(ipDlcSegCenterGeom);
	}

	return true;
}

bool CCheckDlcBld::GetSegmentCenterPoints(const CString& strTableName, long lOID, IGeometryPtr ipGeom, std::vector<IGeometryPtr>& vecSegCenterPointGeom)
{
	ISegmentCollectionPtr ipSegCol(ipGeom);
	if(!ipSegCol){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("ISegmentCollectionへ変換の失敗"));
		return false;
	}
	long lSegCount = 0;
	if(S_OK != ipSegCol->get_SegmentCount(&lSegCount)){
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("セグメント数取得失敗"));
		return false;
	}
	for(long l = 0; l < lSegCount; ++l){
		// セグメントを取得
		_ISegmentPtr ipSeg;
		if(S_OK != ipSegCol->get_Segment(l, &ipSeg)){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("セグメント取得失敗"));
			return false;
		}
		// セグメントの中点を取得
		IPointPtr ipCenterPoint(CLSID_Point);
		ipSeg->QueryPoint(esriNoExtension, 0.5, VARIANT_TRUE, ipCenterPoint);

		IGeometryPtr ipCenterGeom(ipCenterPoint);
		if(!ipCenterGeom){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("IGeometryへ変換失敗"));
			return false;
		}

		// 空間参照を付与
		if(!SetSpatialReference(strTableName, m_ipSpRef, ipCenterGeom))
			return false;

		vecSegCenterPointGeom.push_back(ipCenterGeom);
	}
	return true;
}

bool CCheckDlcBld::ArouondBld(const CString& strBldName, long lBldOid, IFeatureClassPtr ipBldClass, long lBldObjectIdIndex, const CString& strDlcName, long lDlcFid, IGeometryPtr ipDlcGeom, const std::vector<IGeometryPtr>& vecDlcSegGeom, bool bSomeMatch, bool bMsg)
{
	// 装飾線形状で建物を空間検索する
	ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
	if(S_OK != ipSpFilter->putref_Geometry(ipDlcGeom) ||
		S_OK != ipSpFilter->put_SpatialRel(esriSpatialRelIntersects)){
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, _T("空間フィルタ作成失敗"));
			return false;
	}
	IFeatureCursorPtr ipBldCursor;
	if(!GetFeatureCursor(strBldName, ipBldClass, ipSpFilter, ipBldCursor))
		return false;

	std::vector<IGeometryPtr> vecTmp = vecDlcSegGeom;
	IFeaturePtr ipBld;
	while(S_OK == ipBldCursor->NextFeature(&ipBld) && ipBld){
		// 建物のOBJECTIDを取得
		CComVariant vaBldObjectId;
		if(!GetValue(strBldName, shp05::schema::kObjectId, ipBld, lBldObjectIdIndex, vaBldObjectId))
			return false;

		// 既に検索済みの建物はスルー
		if(vaBldObjectId.lVal == lBldOid)
			continue;

		// 建物の形状取得
		IGeometryPtr ipBldGeom;
		if(!GetGeometry(strBldName, ipBld, ipBldGeom))
			return false;

		// 建物に空間参照付与
		if(!SetSpatialReference(strBldName, m_ipSpRef, ipBldGeom))
			return false;

		// 形状マッチしているかチェック（簡易版）
		bool bMatch = false;
		if(!MatchingBldDlc(strBldName, vaBldObjectId.lVal, ipBldGeom, strDlcName, lDlcFid, ipDlcGeom, bMatch)){
			return false;
		}
		// マッチしていれば次の装飾線へ
		if(bMatch){
			if(bMsg)
				CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld4, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
			return true;
		}

		// 形状マッチしているかチェック（詳細版
		std::vector<IGeometryPtr> vecNotMatchTmp;
		if(MatchingBldDlcSegment(strBldName, vaBldObjectId.lVal, ipBldGeom, strDlcName, lDlcFid, vecTmp, vecNotMatchTmp)){
			if(vecNotMatchTmp.empty()){
				if(bMsg){
					if(bSomeMatch)
						CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld5, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
					else
						CLog::GetInstance().PrintLog(false, true, false, true, err_type::warning, err_code::NgExistBld4, strDlcName, lDlcFid, strBldName, std::to_string(vaBldObjectId.lVal).c_str());
				}
				return true;
			}
		}
		vecTmp = vecNotMatchTmp;
	}
	if(bMsg){
		// マッチングするものが無ければエラー
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::error, err_code::NgBldDlcContTouchNear, strDlcName, lDlcFid, strBldName, std::to_string(lBldOid).c_str());
	}

	return false;
}
