#include "stdafx.h"
#include "ConvertHeightNode.h"
#include "MeshList.h"
#include "BaseMesh.h"
#include "HeightNodeInfo.h"
#include "RoadWalkLinkInfo.h"
#include "CnvData.h"
#include "MakeSHP.h"


// 実行
bool CConvertHeightNode::execute()
{
	// メッシュリスト読み込み
	MeshList meshlist;
	if (! meshlist.SetMeshList(m_args.m_meshList.c_str())) 
		return false;
	
	// ワークスペース取得
	IWorkspacePtr ipMeshWorkspace, ipHeightNodeWorkspace;
	if (! common_proc::GetWorkspace(m_args.m_baseMeshDB.c_str(), ipMeshWorkspace) ||
		! common_proc::GetWorkspace(m_args.m_heightNoodeDB.c_str(), ipHeightNodeWorkspace))
		return false;
	// フィーチャクラス取得
	IFeatureClassPtr ipBaseMeshFC,ipHeightNodeFC;
	if (! common_proc::GetFeatureClass(ipMeshWorkspace, sindy::schema::basemesh::kTableName, m_args.m_baseMeshSchema.c_str(), ipBaseMeshFC))
		return false;
	if (! common_proc::GetFeatureClass(ipHeightNodeWorkspace, sindy::schema::height_node::kTableName, m_args.m_heightNodeSchema.c_str(), ipHeightNodeFC))
		return false;
	// 相対高さノード空間参照取得
	ISpatialReferencePtr ipSpRef;
	if (S_OK != ((IGeoDatasetPtr)ipHeightNodeFC)->get_SpatialReference(&ipSpRef))
		return false;
	// 各種クラスの初期化(静的メンバの設定)
	HeightNodeInfo initHeightNodeInfo(ipHeightNodeFC, ipSpRef);
	if (! initHeightNodeInfo.Init())
		return false;
	RoadWalkLinkInfo initRoadWalkInfo;
	initRoadWalkInfo.Init(m_args.m_inputDir.c_str());
	CnvData initCnvData;
	initCnvData.Init(m_args.m_buffer);
	MakeSHP makeSHP;
	if (! makeSHP.Init(m_args.m_outputDir.c_str()))
		return false;

	// メッシュ毎の処理
	CString procMsg;
	bool ret = true;
	for (const auto mesh :meshlist) {
		procMsg.Format(_T("%d\t"), mesh);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// 処理対処メッシュ形状取得
		BaseMesh baseMesh(ipBaseMeshFC);
		if (! baseMesh.SetGeometry(mesh)) {
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// 処理対象相対高さ情報取得
		HeightNodeInfo heightNodeInfo(ipHeightNodeFC, ipSpRef);
		if (! heightNodeInfo.SetHeightNodeInfo(baseMesh.GetGeometry())){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// 道路・歩行者リンク情報取得
		RoadWalkLinkInfo roadWalkLinkInfo;
		if (! roadWalkLinkInfo.SetRoadWalkLinkInfo(mesh)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// 汎用相対高さShape変換
		CnvData cnvData;
		if (! cnvData.ConverteHeightNode(mesh, heightNodeInfo, roadWalkLinkInfo)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}

#ifdef _DEBUG_DIV //　2GB分割テスト用コード(2GBを越えるデータが現状存在しないため、ループをして無理やり2GB分割を起こす)
		long divid_num = 0;
		// SHPよりもDBFのほうが早く2GBを越えるので、DBFでテスト
		// DBFのヘッダー部・レコードサイズを考慮すると33554428レコードで2GBを越える
		// ⇒ 257(ヘッダ部) + n(レコード数) * 64(レコードサイズ) + 1(終端の固定バイト) > 2147483647 ⇒　n = 33554428
		divid_num = 33554428;
		// 2GBを越えるために何回回すか計算
		long loop_size = (divid_num + cnvData.size()-1)/cnvData.size();
		long total_records = 0;
		for (long i = 0; i < loop_size; ++i) {
#endif

		// Shape変換
		if (! makeSHP.InsertRecord(cnvData)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		LogSys::GetInstance().WriteProcLog(true, false, _T("OK\n"));

#ifdef _DEBUG_DIV
		total_records += cnvData.size();
		}
		CString outputTotalRecords;
		outputTotalRecords.Format(_T("合計変換レコード:%d\n"), total_records);
		LogSys::GetInstance().WriteProcLog(true, false, outputTotalRecords);
#endif
	}
	// Shapeが1ファイルのみの場合のリネーム処理
	if (! makeSHP.ReName(m_args.m_outputDir.c_str()))
		return false;
	return ret;
}