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

#pragma once

#include "diffshapeadd.h"

// いつも作ってるけど...WKSPoint流用でも...
struct XY {
	double X;
	double Y;
	bool operator<(const XY& cXY) const {
		return (this->X != cXY.X ? this->X < cXY.X : this->Y < cXY.Y);
	}
	bool operator==(const XY& cXY) const {
		return (this->X == cXY.X && this->Y == cXY.Y);
	}
};

// 差分SHPの認識用（リンクIDはメッシュ単位でユニークのため）
struct CLinkOID {
	long lMesh;
	long lFID;
	bool operator<(const CLinkOID& cLinkOID) const {
		return (this->lMesh != cLinkOID.lMesh ? this->lMesh < cLinkOID.lMesh : this->lFID < cLinkOID.lFID);
	}
	bool operator==(const CLinkOID& cLinkOID) const {
		return (this->lMesh == cLinkOID.lMesh && this->lFID == cLinkOID.lFID);
	}
};

// 差分SHPデータ用の構造体
struct CDiffData {
	CLinkOID cLinkOID;
	long lMID;
	long lMDivision;
	IGeometryPtr ipGeom;
};

class CDiffAddSDE {
public:
	CDiffAddSDE() : m_lOldNodeID(0) {}
	CDiffAddSDE(CInputData& cInputData) : m_lOldNodeID(0) { m_cInputData = cInputData; }
	~CDiffAddSDE(){}

public:
	bool run();

private:
	// 単なるディレクトリ解析処理
	bool get_diffdata();

	// ワークスペースオープンのみ
	bool get_diffdata(const CString& strDiffWork, const CString& strMesh);

	// 差分データを保持する
	bool get_diffdata_main(IWorkspace* ipDiffWork, IFeatureClass* ipDiffLinkClass, const CString& strMesh);

	// 差分データのFromToノードの新旧IDを入れ替えながらインポート
	bool diff_import();

	// リンクのメッシュ境界上処理（2.0短くする処理）
	IGeometryPtr replaceMeshEdge(IGeometry* ipGeom, bool bFrom);

	// リンクのメッシュ境界上処理時に発生する不要ノード削除処理
	bool delete_createnode(long lNewOID);

	// リンクのメッシュ境界上処理時に発生するノード属性更新処理
	bool update_createnode(long lNewOID, long lNodeCount);

	// 指定フィールドの値を付与してフィーチャ新規作成
	IFeatureBufferPtr create_FeatureBuffer(IFeatureClass* ipFeatureClass, IGeometry* ipGeom, std::map<long,CComVariant>& mapFieldValue);

	// 現在の時間を取得
	CString GetNowTime();

	// リコンサイルポスト用関数
	bool DoReconcile(IWorkspaceEdit* ipWorkspaceEdit);

	/**
	 * @brief	おまじない
	 * @note	SDE_LOGFILEを強制的に作成させる処理（単にGetRows()するだけだけど...）
	 */
	void omajinai();

private:
	CInputData m_cInputData;
	crd_cnv m_cnv;

	// インポート用のカーソル
	IFeatureCursorPtr m_ipImportLinkCursor;
	IFeatureCursorPtr m_ipImportNodeCursor;

	// SDEの空間参照（インポート時に差分データの形状に当て込むため）
	ISpatialReferencePtr m_ipLinkSpRef;
	ISpatialReferencePtr m_ipNodeSpRef;


	// インポートデータの新旧ID対応マップ
	long m_lOldNodeID;
	std::map<long,long> m_mapOldNewNode;
	std::map<long,long> m_mapOldNewLink;
	std::map<XY,long> m_mapOldNodeXY;
	std::map<long,XY> m_mapOldNodeXYs;
	std::map<long,std::set<CLinkOID>> m_mapOldNodeLinks;
	std::map<CLinkOID,std::pair<long,long>> m_mapOldLinkFTNode;
	std::map<long,XY> m_mapOldNodeData;
	std::map<CLinkOID,CDiffData> m_mapOldLinkData;


	// SDEのフィールドインデックス
	long m_lNodeClassIndex;
	long m_lRoadClassIndex;
	long m_lNaviClassIndex;
	long m_lMainLinkClassIndex;
	long m_lRoadWidthIndex;
	long m_lUpDownClassIndex;
	long m_lFromNodeIndex;
	long m_lToNodeIndex;
	long m_lMIDIndex;
	long m_lMDivision;

	// 新規追加時の初期値設定用
	std::map<long,CComVariant> m_mapNodeFieldValue;
	std::map<long,CComVariant> m_mapLinkFieldValue;
/*
> ・RoadClass_c		6(その他道)
> ・NAVIClass_c		6(その他道)
> ・Main_LinkClass_c	1(本線上下非分離リンク)
> ・RoadWidth_c		2(幅員5.5m以上13.0m未満)
> ・UPDOWNClass_c		0(対象外)
*/
};

