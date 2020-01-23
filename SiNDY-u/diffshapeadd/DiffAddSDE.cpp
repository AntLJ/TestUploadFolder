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
#include "DiffAddSDE.h"
//#include "CreateFeature.h"
#include <sindy/workspace.h>
#include <sindy/schema.h>

#define	RECONCILE_COUNT	20	// リコンサイル回数

#define BASEDIST	2.0

bool CDiffAddSDE::run()
{
	crd_cnv cnv;
	m_cnv = cnv;

	// SpatialReference 取得
	long lLinkShapeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(_T("SHAPE")));
	long lNodeShapeIndex = m_cInputData.ipNodeClass->GetFields()->_FindField(_bstr_t(_T("SHAPE")));
	IFieldPtr ipLinkShapeField = m_cInputData.ipLinkClass->GetFields()->GetField(lLinkShapeIndex);
	IFieldPtr ipNodeShapeField = m_cInputData.ipNodeClass->GetFields()->GetField(lNodeShapeIndex);
	IGeometryDefPtr ipLinkGeomDef, ipNodeGeomDef;
	ipLinkShapeField->get_GeometryDef(&ipLinkGeomDef);
	ipNodeShapeField->get_GeometryDef(&ipNodeGeomDef);
	ipLinkGeomDef->get_SpatialReference(&m_ipLinkSpRef);
	ipNodeGeomDef->get_SpatialReference(&m_ipNodeSpRef);
	if(!ipLinkGeomDef || !ipNodeGeomDef)
		return false;

	// フィールドインデックス取得
	m_lNodeClassIndex = m_cInputData.ipNodeClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_node::kNodeClass));
	m_lRoadClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kRoadClass));
	m_lNaviClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kNaviClass));
	m_lMainLinkClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kMainLinkClass));
	m_lRoadWidthIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kRoadWidth));
	m_lUpDownClassIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kUpDownClass));
	m_lFromNodeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kFromNodeID));
	m_lToNodeIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(sindy::schema::road_link::kToNodeID));
	m_lMIDIndex = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
	m_lMDivision = m_cInputData.ipLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
	if(m_lNodeClassIndex < 0 || m_lRoadClassIndex < 0 || m_lNaviClassIndex < 0 || m_lMainLinkClassIndex < 0 || m_lRoadWidthIndex < 0 || m_lUpDownClassIndex < 0 || m_lFromNodeIndex < 0 || m_lToNodeIndex < 0)
		return false;

	// 初期値を設定する
	m_mapNodeFieldValue[m_lNodeClassIndex] = 0;
	m_mapLinkFieldValue[m_lRoadClassIndex] = 6;
	m_mapLinkFieldValue[m_lNaviClassIndex] = 6;
	m_mapLinkFieldValue[m_lMainLinkClassIndex] = 1;
	m_mapLinkFieldValue[m_lRoadWidthIndex] = 2;
	m_mapLinkFieldValue[m_lUpDownClassIndex] = 0;

	// テスト用のサーバでMIDやMDivisionが無ければ設定しない
	if(m_lMIDIndex >= 0 && m_lMDivision >=0){
		m_mapLinkFieldValue[m_lMIDIndex] = 0;
		m_mapLinkFieldValue[m_lMDivision] = 0;
	}



	bool bReturn = false;

	// インポート先のワークスペース編集開始 ======================================
	IWorkspaceEditPtr ipWorkspaceEdit;
	esriWorkspaceType esriType;
	m_cInputData.ipSDEWork->get_Type(&esriType);
	// SDE接続なら編集開始終了を行う
	if(esriType == esriRemoteDatabaseWorkspace){
		ipWorkspaceEdit = m_cInputData.ipSDEWork;
		if(!ipWorkspaceEdit){
			std::cerr << "#ERROR\tIWorkspaceEditがNULL\n";
			return false;
		}
		// おまじない（SDE_LOGFILEを強制的に作成させる）
		omajinai();

		// 編集開始
		std::cout << "◇◇編集開始◇◇\n";
		if(S_OK != ipWorkspaceEdit->StartEditing(VARIANT_FALSE)){
			std::cerr << "#ERROR\tStartEditing()失敗\n";
			return false;
		}
		if(S_OK != ipWorkspaceEdit->StartEditOperation()){
			std::cerr << "#ERROR\tStartEditOperation()失敗\n";
			return false;
		}
	}
	// =================================================================================

	// データ取得＋インポート処理
	bool bImport = false;
	m_cInputData.ipLinkClass->Insert(VARIANT_TRUE, &m_ipImportLinkCursor);
	m_cInputData.ipNodeClass->Insert(VARIANT_TRUE, &m_ipImportNodeCursor);
	if(!m_ipImportLinkCursor || !m_ipImportNodeCursor){
		std::cerr << "#ERROR\tインポート用フィーチャカーソル取得失敗\n";
	} else {
		// データ取得
		if(get_diffdata()){
			// インポート処理
			bImport = diff_import();
		}
	}


	// インポート先のワークスペース編集終了 ======================================
	if(esriType == esriRemoteDatabaseWorkspace){
		if(bImport){
			HRESULT hr = ipWorkspaceEdit->StopEditOperation();
			if(S_OK == hr){
				bool bReconcile = true;
				int lCount = 0;
				// 編集終了処理
				while(!DoReconcile(ipWorkspaceEdit)){
					lCount++;
					if(lCount > RECONCILE_COUNT){
						CString strReconcile;
						strReconcile.Format(_T("#ERROR\tリコンサイルを%d回行いましたが成功しませんでしたので終了しました"), RECONCILE_COUNT);
						std::cerr << CT2A(strReconcile) << "\n";
						bReconcile = false;
						break;
					}
				}
				// ここまでOKなら最終戻り値をtrueにする
				if(bReconcile)
					bReturn = true;
			} else {
				if(S_OK != ipWorkspaceEdit->AbortEditOperation())
					std::cerr << "#ERROR\tAbortOperation()失敗\n";
				else
					std::cerr << "#NOTICE\tAbortOperation()実行OK\n";

				if(S_OK != ipWorkspaceEdit->StopEditing(VARIANT_FALSE))
					std::cerr << "#ERROR\tStopEditOperation()失敗\n";
				else
					std::cerr << "#NOTICE\tStopEditOperation()実行O\n";
			}
		} else {
			if(S_OK != ipWorkspaceEdit->AbortEditOperation())
				std::cerr << "#ERROR\tAbortOperation()失敗\n";
			else
				std::cerr << "#NOTICE\tAbortOperation()実行OK\n";

			if(S_OK != ipWorkspaceEdit->StopEditing(VARIANT_FALSE))
				std::cerr << "#ERROR\tStopEditOperation()失敗\n";
			else
				std::cerr << "#NOTICE\tStopEditOperation()実行O\n";
		}
	}
	// =================================================================================

	return bReturn;
}

// 単なるディレクトリ解析処理
bool CDiffAddSDE::get_diffdata()
{
	// 差分データ全部かメッシュリストでインポートするか
	if(m_cInputData.strMeshlist.IsEmpty()){
		// 差分データ全部
		// ↓差分データのディレクトリ構造通りに辿るだけ。辿ったところにデータがあれば追加処理をする
		CString strDiffShapeSearch = m_cInputData.strDiffShapeDir + _T("\\*");
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(strDiffShapeSearch, &fd);
		if(hFind != INVALID_HANDLE_VALUE){
			// ルートパス以下のディレクトリ探索
			do {
				if(fd.cFileName[0] == '.')
					continue;
				CString strNextDir = m_cInputData.strDiffShapeDir + _T("\\") + fd.cFileName;
				DWORD dwFileAttribute = ::GetFileAttributes(strNextDir);
				if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY){
					CString strDiffShapeSearch2 = strNextDir + _T("\\*");
					WIN32_FIND_DATA fd2;
					HANDLE hFind2 = FindFirstFile(strDiffShapeSearch2, &fd2);
					if(hFind != INVALID_HANDLE_VALUE){
						do {
							if(fd2.cFileName[0] == '.')
								continue;
							CString strWorkspaceDir = strNextDir + _T("\\") + fd2.cFileName;
							DWORD dwFileAttribute = ::GetFileAttributes(strWorkspaceDir);
							if(dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY){
								// メッシュリストとして追加
								m_cInputData.setMeshlist;
								CString strShpName = strNextDir + _T("\\") + fd2.cFileName + _T("\\") + fd2.cFileName + _T("_diff.shp");
								if(PathFileExists(strNextDir))
									m_cInputData.setMeshlist.insert(fd2.cFileName);
							}
						} while( FindNextFile(hFind2, &fd2));
						FindClose(hFind2);
					}
				}
			} while( FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
	}

	std::cout << "◆差分データ取得処理開始◆\n";
	long lCount = 0, lSize = m_cInputData.setMeshlist.size();
	CString strMesh;
	for(std::set<CString>::const_iterator it = m_cInputData.setMeshlist.begin(); it != m_cInputData.setMeshlist.end(); ++it){
		strMesh = *it;
		std::cout << ++lCount << " / " << lSize << " (" << CT2A(strMesh) << ")\r";
		// ワークスペースと差分データクラスを取得する
		CString strDiffWork = m_cInputData.strDiffShapeDir + _T("\\") + strMesh.Left(4) + _T("\\") + strMesh;
		if(!get_diffdata(strDiffWork, strMesh)){
			return false;
		}
	}
	std::cout << lCount << " / " << lSize << " (" << CT2A(strMesh) << ")\n";
	std::cout << "◆差分データ取得処理終了◆\n";

	return true;
}

bool CDiffAddSDE::get_diffdata(const CString& strDiffWork, const CString& strMesh)
{
	// ワークスペースと差分データクラスを取得する
	IWorkspacePtr ipDiffWork = sindy::create_workspace(strDiffWork);
	if(ipDiffWork){
		IFeatureClassPtr ipDiffLinkClass;
		IFeatureWorkspacePtr(ipDiffWork)->OpenFeatureClass(CComBSTR(strMesh+_T("_diff")), &ipDiffLinkClass);
		if(ipDiffLinkClass){
			// 差分データ追加処理
			if(!get_diffdata_main(ipDiffWork, ipDiffLinkClass, strMesh))
				return false;
		} else {
			std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t差分フィーチャクラスオープン失敗\n";
		}
	} else {
		std::cerr << "#ERROR\t" << CT2A(strDiffWork) << "\t差分データワークスペースオープン失敗\n";
	}
	// フィーチャクラスがオープンできない程度ならスルーでOK
	return true;
}

bool CDiffAddSDE::get_diffdata_main(IWorkspace* ipDiffWork, IFeatureClass* ipDiffLinkClass, const CString& strMesh)
{
	// 差分データのフィールドインデックスを取得しておく
	long lFIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_FID));
	long lMDivisionIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MDivision));
	long lMIDIndex = ipDiffLinkClass->GetFields()->_FindField(_bstr_t(g_MID));
	if(lFIDIndex < 0 || lMDivisionIndex < 0 || lMIDIndex < 0){
		std::cerr << "#ERROR\t" << CT2A(strMesh) << "フィールドインデックス取得失敗\n";
		return false;
	}
	long lMesh = _ttol(strMesh);

	std::set<std::vector<XY>> setOldLinkGeom;	// 同一形状がある場合はインポートさせない

	// まずはインポート元（差分データ）のFromToノードIDの設定＋リンクIDの取得
	IFeatureCursorPtr ipDiffLinkCursor;
	ipDiffLinkClass->Search(NULL, VARIANT_FALSE, &ipDiffLinkCursor);
	if(!ipDiffLinkCursor){
		std::cerr << "#ERROR\t" << CT2A(strMesh) << "カーソル取得失敗\n";
		return false;
	}
	IFeaturePtr ipDiffLink;
	while(S_OK == ipDiffLinkCursor->NextFeature(&ipDiffLink)){
		IGeometryPtr ipGeom;
		CComVariant vaFID, vaMDivision, vaMID;
		ipDiffLink->get_Value(lFIDIndex, &vaFID);
		ipDiffLink->get_Value(lMDivisionIndex, &vaMDivision);
		ipDiffLink->get_Value(lMIDIndex, &vaMID);
		ipDiffLink->get_ShapeCopy(&ipGeom);
		if(!ipGeom){
			std::cerr << "#ERROR\t" << CT2A(strMesh) << "\tFID : " << vaFID.lVal << "\t形状NULL\n";
			continue;
		}
		CLinkOID cLinkOID;
		cLinkOID.lMesh = lMesh;
		cLinkOID.lFID = vaFID.lVal;
		long lPointCount = 0;
		IPointCollectionPtr(ipGeom)->get_PointCount(&lPointCount);
		std::vector<XY> vecXY;
		for(long l=0;l<lPointCount;++l){
			XY cXY;
			IPointPtr ipPoint;
			IPointCollectionPtr(ipGeom)->get_Point(l,&ipPoint);
			ipPoint->QueryCoords(&cXY.X,&cXY.Y);
			vecXY.push_back(cXY);
		}
		if(setOldLinkGeom.find(vecXY) != setOldLinkGeom.end()){
			std::cerr << "#ERROR\t" << CT2A(strMesh) << "\tFID : " << vaFID.lVal << "\t同一形状があるのでスルー\n";
			continue;
		}
		setOldLinkGeom.insert(vecXY);

		m_mapOldNewLink[vaFID.lVal] = 0;

		IPointPtr ipFPoint, ipTPoint;
		IPolylinePtr(ipGeom)->get_FromPoint(&ipFPoint);
		IPolylinePtr(ipGeom)->get_ToPoint(&ipTPoint);

		XY cFromXY, cToXY;
		ipFPoint->QueryCoords(&cFromXY.X,&cFromXY.Y);
		ipTPoint->QueryCoords(&cToXY.X,&cToXY.Y);

		long lFromID = 0, lToID = 0;
		if(m_mapOldNodeXY.insert(std::pair<XY,long>(cFromXY,m_lOldNodeID)).second){
			lFromID = m_lOldNodeID;
			m_mapOldNewNode[lFromID] = 0;
			m_mapOldNodeData[lFromID] = cFromXY;
			m_mapOldNodeXYs[lFromID] = cFromXY;
			m_lOldNodeID++;
		} else {
			lFromID = m_mapOldNodeXY[cFromXY];
		}
		if(m_mapOldNodeXY.insert(std::pair<XY,long>(cToXY,m_lOldNodeID)).second){
			lToID = m_lOldNodeID;
			m_mapOldNewNode[lToID] = 0;
			m_mapOldNodeData[lToID] = cToXY;
			m_mapOldNodeXYs[lToID] = cToXY;
			m_lOldNodeID++;
		} else {
			lToID = m_mapOldNodeXY[cToXY];
		}

		m_mapOldNodeLinks[lFromID].insert(cLinkOID);
		m_mapOldNodeLinks[lToID].insert(cLinkOID);
		m_mapOldLinkFTNode[cLinkOID] = std::pair<long,long>(lFromID,lToID);
		CDiffData cDiffData;
		cDiffData.cLinkOID = cLinkOID;
		cDiffData.lMDivision = vaMDivision.lVal;
		cDiffData.lMID = vaMID.lVal;
		cDiffData.ipGeom = ipGeom;
		m_mapOldLinkData[cLinkOID] = cDiffData;
	}

	return true;
}

bool CDiffAddSDE::diff_import()
{
	std::cout << "◆差分データインポート処理開始◆\n";

	ISpatialFilterPtr ipSpFil(CLSID_SpatialFilter);

	// 2次メッシュ端点だけが残ってしまうノードをキープしておく
	// あとでこのノードに紐付くリンクの端点を短くするため...
	std::set<long> setOldMeshEdgeNode;

	std::cout << "ノードインポート処理中...\n";
	long lNodeCount = 0, lNodeSize = m_mapOldNodeData.size();

	// インポート用のデータを取得したらインポートを開始する
	// まずはノードから
	for(std::map<long,XY>::const_iterator itNode = m_mapOldNodeData.begin(); itNode != m_mapOldNodeData.end(); ++itNode){
		lNodeCount++;
		if(lNodeCount % 100 == 0)
			std::cout << lNodeCount << " / " << lNodeSize << "\r";

		int imesh,ix,iy;
		m_cnv.LLtoMesh(itNode->second.X,itNode->second.Y,2,&imesh,&ix,&iy);
		IPointPtr ipNodePoint(CLSID_Point);
		ipNodePoint->PutCoords(itNode->second.X,itNode->second.Y);
		ipNodePoint->putref_SpatialReference(m_ipNodeSpRef);
		bool bMeshEdgeNode = false;
		// メッシュ境界線かチェック
		if(ix == 0 || ix == 1000000 || iy == 0 || iy == 1000000){
			m_mapNodeFieldValue[m_lNodeClassIndex] = 9;
			// メッシュ境界ノードに1本だけリンクが接続しているものは後で処理させる
			if(m_mapOldNodeLinks[itNode->first].size() == 1){
				bMeshEdgeNode = true;
				setOldMeshEdgeNode.insert(itNode->first);
			}
		} else {
			// ノードクラスの設定（端点、交差点、ダミーか）
			if(m_mapOldNodeLinks[itNode->first].size() == 1)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 2;	// 端点
			else if(m_mapOldNodeLinks[itNode->first].size() == 2)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 0;	// ダミー
			else if(m_mapOldNodeLinks[itNode->first].size() > 2)
				m_mapNodeFieldValue[m_lNodeClassIndex] = 1;	// 交差点
			else
				std::cerr << "#ERROR\t" << itNode->first << "のーどがういてる\n";
		}
		// メッシュ境界ノードに1本だけリンクが接続しているものは後で処理させる
		if(!bMeshEdgeNode){
			// 上記以外はノード作成処理
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipNodePoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << itNode->first << "のーどバッファがNULLってる\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << itNode->first << "のーどついかしっぱいだ\n";
				continue;
			}
			// 旧新ノードマップに登録
			m_mapOldNewNode[itNode->first] = vaImportNodeOID.lVal;
		}
	}
	std::cout << lNodeCount << " / " << lNodeSize << "\n";
	std::cout << "ノードインポート処理終了\n";

	std::cout << "リンクインポート処理中...\n";
	long lLinkCount = 0, lLinkSize = m_mapOldLinkData.size();

	// 次はリンクをインポート
	for(std::map<CLinkOID,CDiffData>::const_iterator itLink = m_mapOldLinkData.begin(); itLink != m_mapOldLinkData.end(); ++itLink){
		lLinkCount++;
		if(lLinkCount % 100 == 0)
			std::cout << lLinkCount << " / " << lLinkSize << "\r";

		// テストサーバなどでMIDやMDivisonフィールドが無い場合は設定しない
		if(m_lMIDIndex >= 0 && m_lMDivision >= 0){
			m_mapLinkFieldValue[m_lMIDIndex] = itLink->second.lMID;
			m_mapLinkFieldValue[m_lMDivision] = itLink->second.lMDivision;
		}
		IGeometryPtr ipSetLinkGeom = itLink->second.ipGeom;

		// メッシュ境界ノードに1本だけリンクが接続しているものは特殊処理
		// from側の処理
		if(setOldMeshEdgeNode.end() != setOldMeshEdgeNode.find(m_mapOldLinkFTNode[itLink->first].first)){
			ipSetLinkGeom = replaceMeshEdge(ipSetLinkGeom, true);
			if(!ipSetLinkGeom){
				std::cerr << "#NOTICE\t" << itLink->second.cLinkOID.lMesh << "\tOID : " << itLink->second.cLinkOID.lFID << "\t2次メッシュ境界リンクでリンク長が短いのでインポートしませんでした\n";
				// 形状NULLの場合は端点削除が必要か確認すること
				if(m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].second].size() == 1){
					// 削除
					delete_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second]);
				} else {
					// 2差路の場合は端点ノード、3差路の場合は機能無しノードになるので更新処理が必要
					update_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second], m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].second].size());
				}
				continue;
			}
			// 短くした端点のノードを追加してやる
			IPointPtr ipSetPoint;
			IPolylinePtr(ipSetLinkGeom)->get_FromPoint(&ipSetPoint);
			m_mapNodeFieldValue[m_lNodeClassIndex] = 2;
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipSetPoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].first << "端点のーどバッファがNULLってる\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].first << "端点のーどついかしっぱいだ\n";
				continue;
			}
			// 旧新ノードマップを更新
			m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first] = vaImportNodeOID.lVal;
		}
		// to側の処理
		if(setOldMeshEdgeNode.end() != setOldMeshEdgeNode.find(m_mapOldLinkFTNode[itLink->first].second)){
			ipSetLinkGeom = replaceMeshEdge(ipSetLinkGeom, false);
			if(!ipSetLinkGeom){
				std::cerr << "#NOTICE\t" << itLink->second.cLinkOID.lMesh << "\tOID : " << itLink->second.cLinkOID.lFID << "\t2次メッシュ境界リンクでリンク長が短いのでインポートしませんでした\n";
				// 形状NULLの場合は端点削除が必要か確認すること
				if(m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].first].size() == 1){
					// 削除
					delete_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first]);
				} else {
					// 2差路の場合は端点ノード、3差路の場合は機能無しノードになるので更新処理が必要
					update_createnode(m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first], m_mapOldNodeLinks[m_mapOldLinkFTNode[itLink->first].first].size());
				}
				continue;
			}
			// 短くした端点のノードを追加してやる
			IPointPtr ipSetPoint;
			IPolylinePtr(ipSetLinkGeom)->get_ToPoint(&ipSetPoint);
			m_mapNodeFieldValue[m_lNodeClassIndex] = 2;
			IFeatureBufferPtr ipNodeBuffer = create_FeatureBuffer(m_cInputData.ipNodeClass, IGeometryPtr(ipSetPoint), m_mapNodeFieldValue);
			if(!ipNodeBuffer){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].second << "端点のーどバッファがNULLってる\n";
				continue;
			}
			CComVariant vaImportNodeOID;
			if(S_OK != m_ipImportNodeCursor->InsertFeature(ipNodeBuffer, &vaImportNodeOID) || vaImportNodeOID.lVal < 0){
				std::cerr << "#ERROR\t" << m_mapOldLinkFTNode[itLink->first].second << "端点のーどついかしっぱいだ\n";
				continue;
			}
			// 旧新ノードマップを更新
			m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second] = vaImportNodeOID.lVal;
		}

		// 新FromToNodeIDを設定する
		m_mapLinkFieldValue[m_lFromNodeIndex] = m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].first];
		m_mapLinkFieldValue[m_lToNodeIndex] = m_mapOldNewNode[m_mapOldLinkFTNode[itLink->first].second];

		IFeatureBufferPtr ipLinkBuffer = create_FeatureBuffer(m_cInputData.ipLinkClass, ipSetLinkGeom, m_mapLinkFieldValue);
		if(!ipLinkBuffer){
			std::cerr << "#ERROR\t" << itLink->first.lMesh << " : " << itLink->first.lFID << "りんくバッファがNULLってる\n";
			continue;
		}
		CComVariant vaImportLinkOID;
		if(S_OK != m_ipImportLinkCursor->InsertFeature(ipLinkBuffer, &vaImportLinkOID) || vaImportLinkOID.lVal < 0){
			std::cerr << "#ERROR\t" << itLink->first.lMesh << " : " << itLink->first.lFID << "りんくついかしっぱいだ\n";
			continue;
		}
	}
	std::cout << lLinkCount << " / " << lLinkSize << "\n";
	std::cout << "リンクインポート処理終了\n";

	if(S_OK != m_ipImportNodeCursor->Flush()){
		std::cerr << "#ERROR\tのーどカーソルFlush()しっぱいだ\n";
		return false;
	}
	if(S_OK != m_ipImportLinkCursor->Flush()){
		std::cerr << "#ERROR\tりんくカーソルFlush()しっぱいだ\n";
		return false;
	}

	std::cout << "◆差分データインポート処理終了◆\n";

	return true;
}

IGeometryPtr CDiffAddSDE::replaceMeshEdge(IGeometry* ipGeom, bool bFrom)
{
	IClonePtr ipClone;
	IClonePtr(ipGeom)->Clone(&ipClone);
	IGeometryPtr ipLinkGeom(ipClone);

	long lPointCount = 0;
	IPointPtr ipPoint1, ipPoint2, ipSetPoint(CLSID_Point);
	WKSPoint wks1,wks2;
	double dBaseDist = BASEDIST;
	bool bBase = false;
	IPointCollectionPtr(ipLinkGeom)->get_PointCount(&lPointCount);

	long lRemoveCount = 0;
	for(lRemoveCount = 0; lRemoveCount < lPointCount-1; ++lRemoveCount){
		if(bFrom){
			IPointCollectionPtr(ipLinkGeom)->get_Point(lRemoveCount,&ipPoint1);
			IPointCollectionPtr(ipLinkGeom)->get_Point(lRemoveCount+1,&ipPoint2);
		} else {
			IPointCollectionPtr(ipLinkGeom)->get_Point(lPointCount-(lRemoveCount+1),&ipPoint1);
			IPointCollectionPtr(ipLinkGeom)->get_Point(lPointCount-(lRemoveCount+2),&ipPoint2);
		}
		ipPoint1->QueryCoords(&wks1.X,&wks1.Y);
		ipPoint2->QueryCoords(&wks2.X,&wks2.Y);
		double dDist = m_cnv.GetDist(wks1.X,wks1.Y,wks2.X,wks2.Y);
		double dLinkLen = 0.0;
		if(dDist < dBaseDist){
			dBaseDist -= dDist;
		} else {
			bBase = true;
			break;
		}
	}
	// リンクがBASEDIST以下の場合はインポート対象としない
	// ⇒FromToノードは既にインポートされているので削除処理が必要か確認する
	if(!bBase){
		return NULL;
	} else {
		WKSPoint wksF,wksT, wksHalf;
		wksF = wks1;
		wksT = wks2;
		for(long l = 0; l < 30; ++l){
			wksHalf.X = (wksF.X+wksT.X)/2.0;
			wksHalf.Y = (wksF.Y+wksT.Y)/2.0;
			double dDist = m_cnv.GetDist(wks1.X,wks1.Y,wksHalf.X,wksHalf.Y);
			if(0.001 > abs(dDist-dBaseDist)){
				break;
			}
			if(dDist > dBaseDist){
				wksT = wksHalf;
			} else {
				wksF = wksHalf;
			}
		}
		if(lRemoveCount > 0){
			if(bFrom)
				IPointCollectionPtr(ipLinkGeom)->RemovePoints(0, lRemoveCount);
			else
				IPointCollectionPtr(ipLinkGeom)->RemovePoints(lPointCount-(lRemoveCount), lRemoveCount);
		}
		ipSetPoint->PutCoords(wksHalf.X,wksHalf.Y);
		IPointCollectionPtr(ipLinkGeom)->get_PointCount(&lPointCount);
		if(bFrom)
			IPointCollectionPtr(ipLinkGeom)->UpdatePoint(0,ipSetPoint);
		else
			IPointCollectionPtr(ipLinkGeom)->UpdatePoint(lPointCount-1,ipSetPoint);

		return ipLinkGeom;
	}
}

bool CDiffAddSDE::delete_createnode(long lNewOID)
{
	IFeaturePtr ipDelNode;
	m_cInputData.ipNodeClass->GetFeature(lNewOID, &ipDelNode);
	if(!ipDelNode){
		std::cerr << "#ERROR\t" << lNewOID << "\t浮き端点ノード検索失敗\n";
		return false;
	}
	if(S_OK != ipDelNode->Delete()){
		std::cerr << "#ERROR\t" << lNewOID << "\t浮き端点ノード削除失敗\n";
		return false;
	}
	if(S_OK != ipDelNode->Store()){
		std::cerr << "#ERROR\t" << lNewOID << "\t浮き端点ノード削除後の更新失敗\n";
		return false;
	}
	return true;
}

bool CDiffAddSDE::update_createnode(long lNewOID, long lNodeCount)
{
	long lValue = sindy::schema::road_node::node_class::kTerminal;
	if(lNodeCount == 2){
		lValue = sindy::schema::road_node::node_class::kTerminal;	// 端点ノード
	} else if(lNodeCount == 3){
		lValue = 0;	// 機能無しノード
	} else if(lNodeCount > 3){
		// 4本以上なら1本削られても交差点ノードなので更新の必要なし
		return true;
	}
	
	IFeaturePtr ipUpdateNode;
	m_cInputData.ipNodeClass->GetFeature(lNewOID, &ipUpdateNode);
	if(!ipUpdateNode){
		std::cerr << "#ERROR\t" << lNewOID << "\t更新ノード検索失敗\n";
		return false;
	}

	if(S_OK != ipUpdateNode->put_Value(m_lNodeClassIndex, CComVariant(lValue))){
		std::cerr << "#ERROR\t" << lNewOID << "\t更新ノード削除失敗\n";
		return false;
	}
	if(S_OK != ipUpdateNode->Store()){
		std::cerr << "#ERROR\t" << lNewOID << "\t更新ノード削除後の更新失敗\n";
		return false;
	}
	return true;
}


IFeatureBufferPtr CDiffAddSDE::create_FeatureBuffer(IFeatureClass* ipFeatureClass, IGeometry* ipGeom, std::map<long,CComVariant>& mapFieldValue)
{
	IFeatureBufferPtr ipFeatureBuffer;
	if(S_OK != ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer) || !ipFeatureBuffer)
		return NULL;

	if(S_OK != ipFeatureBuffer->putref_Shape(IGeometryPtr(ipGeom)))
		return NULL;

	long lFieldCount = 0;
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields(&ipFields);
	ipFields->get_FieldCount(&lFieldCount);
	for(long l = 0; l < lFieldCount; l++){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		if(!ipField)
			return NULL;

		CComBSTR bstrFieldName;
		ipField->get_Name(&bstrFieldName);

		if(_tcsicmp(CString(bstrFieldName), _T("OBJECTID")) != 0 && _tcsicmp(CString(bstrFieldName), _T("SHAPE")) != 0){
			// 指定値があれば設定、無ければ初期値
			if(mapFieldValue.find(l) == mapFieldValue.end()){
				// 初期値
				CComVariant vaDefaultValue;
				// 編集者、更新日時、更新方法は決め打ち
				if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kOperator ) == 0)
					vaDefaultValue = CComBSTR(g_UpdateOperator);
				else if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kModifyDate) == 0)
					vaDefaultValue = CComBSTR(GetNowTime());
				else if(_tcsicmp(CString(bstrFieldName), sindy::schema::ipc_table::kUpdateType) == 0)
					vaDefaultValue = CComVariant((LONG)1);	// 新規作成
				else
					ipField->get_DefaultValue(&vaDefaultValue);
				ipFeatureBuffer->put_Value(l, vaDefaultValue);
			} else {
				// 指定値
				ipFeatureBuffer->put_Value(l, mapFieldValue[l]);
			}
		}
	}
	return ipFeatureBuffer;
}

CString CDiffAddSDE::GetNowTime()
{
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
	CString strStrModifyDate;
	strStrModifyDate.Format(TEXT( "%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return strStrModifyDate;
}


bool CDiffAddSDE::DoReconcile(IWorkspaceEdit* ipWorkspaceEdit)
{
	// StopEditing()に失敗したらリコンサイル
	HRESULT hr = ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
	bool bRet = false;
	switch(hr)
	{
		case FDO_E_VERSION_REDEFINED:
		{
			std::cout << "◆リコンサイル中...\n";
			std::cerr << "#NOTECE\tリコンサイルを行います\n";
			IVersionEditPtr ipVersionEdit(ipWorkspaceEdit);
			if( ipVersionEdit )
			{
				IVersionInfoPtr ipVersionInfo, ipParentVersionInfo;
				IVersionPtr ipVersion( ipVersionEdit );
				CComBSTR bstrVersionName, bstrParentVersionName;
				if(ipVersion){
					if( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ) )
						ipVersionInfo->get_VersionName( &bstrVersionName );
				}
				if(!CString(bstrVersionName).IsEmpty()){
					VARIANT_BOOL vbConflicts = VARIANT_FALSE;
					HRESULT hrReconcile = hr = ipVersionEdit->Reconcile( bstrParentVersionName, &vbConflicts );
					switch( hrReconcile )
					{
						case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:
							std::cerr << "#ERROR\tリコンサイル失敗：FDO_E_RECONCILE_VERSION_NOT_AVAILABLE\n";
							break;
						case FDO_E_VERSION_BEING_EDITED:
							std::cerr << "#ERROR\tリコンサイル失敗：FDO_E_VERSION_BEING_EDITED\n";
							break;
						case FDO_E_VERSION_NOT_FOUND:
							std::cerr << "#ERROR\tリコンサイル失敗：FDO_E_VERSION_NOT_FOUND\n";
							break;
						case S_OK:
							// コンフリクトした場合はエラーとせず、そのまま抜ける
							// コンフリクト時の処理は呼び出し先に任せる
							if( vbConflicts )
								std::cerr << "#ERROR\tリコンサイル成功：コンフリクトが発生しました\n";
							else {
								// ヘルプには、Reconcileが終了したらStopEditingが自動的に呼ばれるようなことが
								// 書いてあったので、何もしなくてもいいのかな？？
								std::cerr << "#NOTICE\tリコンサイル成功：編集終了\n";
								std::cout << "◇◇リコンサイル成功\t編集終了◇◇\n";
							}
							bRet = true;
						break;
						default:
							std::cerr << "#ERROR\tリコンサイル中に原因不明のエラーが発生しました\n";
							break;
					}
				}
				else
					std::cerr << "#ERROR\tIVersionEditからバージョン名の取得失敗\n";
			}
			else
				std::cerr << "#ERROR\tIVersionEditの取得失敗\n";
		}
			break;
		case S_OK:
			std::cout << "◇◇編集保存成功◇◇\n";
			bRet = true;
			break;
		default:
			std::cerr << "#ERROR\t編集保存の処理中にエラーが発生しました\n";
			break;
	}
	return bRet;
}




void CDiffAddSDE::omajinai()
{
	std::vector<long> tmplist;
	for(long l = 1; l <= 1000; ++l)	// 1000で良い？
		tmplist.push_back(l);

	_ICursorPtr ipCursor;

	SAFEARRAYBOUND bound = { (ULONG)tmplist.size(), 0 };
	SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
	LONG* pArray;
	SafeArrayAccessData( pSa, (void**)&pArray );

	long i = 0;
	for( std::vector<long>::const_iterator it = tmplist.begin(); it != tmplist.end(); ++it, ++i )
		pArray[i] = *it;

	SafeArrayUnaccessData( pSa );
	VARIANT vaArray;
	VariantInit( &vaArray );
	vaArray.vt = VT_ARRAY | VT_I4;
	vaArray.parray = pSa;

	ITablePtr(m_cInputData.ipLinkClass)->GetRows(vaArray, VARIANT_TRUE, &ipCursor);

	VariantClear( &vaArray );
}
