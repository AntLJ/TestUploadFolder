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

#include <Workspace.h>
#include <GeometryBase.h>
#include <ArcHelperEx/Mesh.h>
#include "CFeatureClass.h"
#include "RowBase.h"
#include <boost/tuple/tuple.hpp>
#include "tracer/LinkTracer.h"

using namespace std;

namespace sindy
{
	
/**
 * @class CCreateDistancePost
 * @brief 距離標インポートクラス
 * @file  CCreateDistancePost.h
 * @author ビットエイジ　秦
 * $Id$
*/
class CCreateDistancePost
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CCreateDistancePost()
	{
	};

	/**
	 * @brief デストラクタ
	 */
	~CCreateDistancePost(){};

	/**
	 * @brief 処理DB接続
	 * @param lpcszConnection [in] 接続文字列
	 * @return 接続できたかどうか
	*/
	bool Connect(LPCTSTR lpcszConnection);

	/**
	 * @brief Distance_Post_Point取得
	 * @return 取得できたかどうか
	*/
	bool OpenDistancePostPoint();

	/**
	 * @brief ROAD_LINK/ROAD_NODE取得
	 * @return 取得できたかどうか
	*/
	bool OpenNetwork();

	/**
	 * @brief メッシュ取得
	 * @return 取得できたかどうか
	*/
	bool OpenMesh();

	/**
	 * @brief 対象道路リスト情報取得
	 * @param nLineNo [in] ファイル上の行番号
	 * @return 道路リスト情報
	*/
	boost::tuple<long, long, CString> GetRoadList(long nLineNo) { return m_mapRoadList[nLineNo]; }

	/**
	 * @brief 路線リスト確認
	 * @param lpcszFile [in] 路線リストファイルパス
	 * @param dbPitch [in] ピッチ
	 * @param mapContainer [out] 
	 * @param mapErr [out] エラーがある路線コード群
	 * @return 正常かどうか 0:正常、それ以外エラー
	*/
	long CheckRoadList(LPCTSTR lpcszFile, double dbPitch, std::map<long, std::pair<std::list<double>, std::map<double, long>>>& mapContainer, std::map<long,long>& mapErr);

	/**
	 * @brief 距離標ポイント検索
	 * @param mapValues [in] 距離標値群(OID,距離標)
	*/
	void searchDistancePoint(const std::map<double, long>& mapValues);

	/**
	 * @brief 距離標ポイント群取得
	 * @return 距離標ポイント群
	*/
	std::map<double, CSPRowBase> findDistancePoints();

	/**
	 * @brief 距離標ポイント取得
	 * @param dbVal [in] 距離標値
	 * @return 距離標ポイント
	*/
	CSPRowBase findDistancePoint(double dbVal);

	/**
	 * @brief 距離標ポイント検索
	 * @param ipGeom [in] 検索形状
	 * @param dbBuf [in] バッファ値
	 * @return 道路OID群
	*/
	std::list<long> searchDistancePostOnRoad(const IGeometryPtr& ipGeom, double dbBuf);

	/**
	 * @brief 道路検索
	 * @param ipGeom [in] 検索形状
	 * @param dbBuf [in] バッファ値
	 * @return 道路群
	*/
	CFeatureClass::Container searchRoads(const IGeometryPtr& ipGeom, double dbBuf);

	/**
	 * @brief 道路除外処理
	 * @param cRoads [in/out] 道路群
	 * @param listNotExcludeRoads [in] 除外してはいけない道路群
	 * @param listRouteIDs [in] 経路探索済み道路OID群
	*/
	void excludeRoad(CFeatureClass::Container& cRoads, const std::list<long>& listNotExcludeRoads, const std::list<long>& listRouteIDs);

	/**
	 * @brief 道路検索用形状取得
	 * @param cRoads [in/out] 道路群
	 * @param nBase [in] オフセット値
	 * @param listExcludeMesh [in] 除外メッシュ群
	 * @param listMesh [in/out] 検索対象メッシュコード群
	 * @return 検索形状
	*/
	IGeometryPtr searchRoadGeoms(const CFeatureClass::Container& cRoads, long nBase, const std::list<long>& listExcludeMesh, std::list<long>& listMesh);

	/**
	 * @brief 道路リンクフィーチャクラス取得
	 * @return 道路リンクフィーチャクラス
	*/
	IFeatureClassPtr getRoadLink() { return m_cRoadLinkClass; }

	/**
	 * @brief 距離標ポイント形状作成
	 * @param ipPolyline [in] ライン形状
	 * @param dbFirst [in] 始点
	 * @param dbEnd [in] 終点
	 * @param dbPitch [in] ピッチ
	 * @param dbLen [in] 処理対象距離
	 * @param nSplitNum [in] 分割数
	 * @param mapPoints [in/out] 作成ポイント群
	 * @param dbLenStart [in] 始点距離(道路の始点から始点までの距離)
	 * @param dbLenEnd [in] 終点距離(道路の終点から終点までの距離)
	 * @param bPitch [in] ピッチ単位で分割するかどうか（単位合わせした点と補正点時のみ有効）
	 * @return 作成できたかどうか
	*/
	bool createDistancePoint(const IPolylinePtr& ipPolyline, double dbFirst, double dbEnd, double dbPitch, double dbLen, long nSplitNum, std::map<double, IPointPtr>& mapPoints, double dbLenStart, double dbLenEnd, bool bPitch);

	/**
	 * @brief 距離標ポイント作成
	 * @param ipPoint [in] 作成ポイント形状
	 * @param dbVal [in] 距離標値
	 * @param nLineNo [in] ファイル行番号
	 * @return 距離標ポイント
	*/
	CSPRowBase createDistancePoint(const IPointPtr& ipPoint, double dbVal, long nLineNo);

	/**
	 * @brief 点取得（距離）
	 * @param ipPolyline [in] ライン形状
	 * @param dbVal [in] 距離
	 * @return 分割点
	*/
	IPointPtr split_distance(const IPolylinePtr& ipPolyline, double dbVal);

	/**
	 * @brief 分割点取得（ポイント）
	 * @param ipPolyline [in] ライン形状
	 * @param ipPoint [in] 分割ポイント
	 * @param dbVal [in] 距離
	 * @param dbLen [out] 距離（ラインの始点から分割点までの距離)
	 * @return 分割点
	*/
	IPointPtr split_point(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint, double dbVal, double& dbLen);

	/**
	 * @brief 編集開始
	*/
	void startEdit();

	/**
	 * @brief 編集終了
	*/
	void stopEdit();

	/**
	 * @brief 編集破棄
	*/
	void abortEdit();

	/**
	 * @brief コストファイル読み込み(道路種別)
	 * @param lpcszFile [in] コストファイルパス
	*/
	void ReadRoadCost(LPCTSTR lpcszFile);

	/**
	 * @brief コストファイル読み込み(リンク種別)
	 * @param lpcszFile [in] コストファイルパス
	*/
	void ReadLinkCost(LPCTSTR lpcszFile);

	/**
	 * @brief 単位変換
	 * @param dbValue [in] 変換元値
	 * @param esriSrc [in] 変換元単位
	 * @param esriDest [in] 変換先単位
	 * @return 変換値
	*/
	double convert(double dbValue, esriUnits esriSrc, esriUnits esriDest);

	/**
	 * @brief 距離(km)取得
	 * @param ipPolyline [in] ライン形状
	 * @return 距離(km)
	*/
	double GetLength( const IPolylinePtr& ipPolyline);

	/**
	 * @brief フィールド番号取得
	 * @param lpcszField [in] フィールド名
	 * @return フィールド番号
	*/
	long GetFieldIndex(LPCTSTR lpcszField);

	/**
	 * @brief COMオブジェクトのコピーを作成する
	 * @param ipUnk [in] クローン元
	 * @return クローン
	*/
	static IClonePtr Clone( const IUnknownPtr& ipUnk );

	/**
	 * @brief 道路検索（OID指定）
	 * @param listRouteIDs [in] OID群
	 * @return 検索道路群
	*/
	CFeatureClass::Container searchRoads(const std::list<long>& listRouteIDs);

	/**
	 * @brief 分割点までの距離取得
	 * @param ipPolyline [in] ライン形状
	 * @param ipPoint [in] 分割ポイント
	 * @return 分割点までの距離
	*/
	double split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint);

	/**
	 * @brief 分割した2点間の距離を取得
	 * @param ipPolyline [in] ライン形状
	 * @param ipPoint1 [in] 始点
	 * @param ipPoint2 [out] 次点
	 * @return 分割した2点間の距離
	*/
	double split_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint1, const IPointPtr& ipPoint2);

	/**
	 * @brief 切り詰め
	 * @param dbVal [in] 値
	 * @param nLen [in] 桁
	 * @return 切り詰めた値
	*/
	CString convertStr(double dbVal, long nLen=2);

	/**
	* @brief 生成する距離標ポイントを取得する
	* @param existDistanceList		[in]  既存距離標ポイント群
	* @param mapRows				[in]  既存距離標ポイントのCRowBase群
	* @param ipPrjCoordinateSystem	[in]  座標系変更用
	* @param ipAllPolyline			[out] 路線形状格納用
	* @param mapPoints				[out] 生成する距離標ポイント格納用
	* @param dbPitch				[in]  処理ピッチ
	* @param dbBuff					[in]  リンク特定バッファ
	* @param correctionPointLen		[in]  補正点間距離
	* @param errorMsg				[out] エラーメッセージ格納用
	* @retval true : 取得成功(エラー無し)
	* @retval false: 取得失敗(エラー有り)
	*/
	bool getCreateDistancePost(
		const std::list<double>& existDistanceList, 
		const std::map<double, sindy::CSPRowBase>& mapRows, 
		const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem, 
		IPolylinePtr& ipAllPolyline,
		std::map<double, IPointPtr>& mapPoints, 
		double dbPitch,
		double dbBuff,
		double correctionPointLen, 
		CString& errorMsg);

private:
	CWorkspace m_cWorkspace;                    ///< ワークスペース
	CFeatureClass m_cDistancePostClass;         ///< 距離標ポイントクラス
	CFeatureClass m_cRoadLinkClass;             ///< 道路リンククラス
	CFeatureClass m_cRoadNodeClass;             ///< 道路ノードクラス
	CFeatureClass m_cBaseMeshClass;             ///< ２次メッシュクラス

	std::map<long, boost::tuple<long, long, CString>> m_mapRoadList; ///< 路線リスト群
};

}
