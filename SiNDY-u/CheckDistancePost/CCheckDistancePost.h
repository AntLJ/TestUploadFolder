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
#include "CLogger.h"

using namespace std;

namespace sindy
{
	
/**
 * @class CCheckDistancePost
 * @brief 距離標チェックツールクラス
 * @file  CCheckDistancePost.h
 * @author ビットエイジ　秦
 * $Id$
*/
class CCheckDistancePost
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CCheckDistancePost()
	{
	}

	/**
	 * @brief デストラクタ
	 */
	virtual ~CCheckDistancePost(){}

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
	 * @param mapValues [in] 距離標値群(距離標値, OID)
	*/
	void searchDistancePoint(const std::map<double, long>& mapValues);

	/**
	 * @brief 距離標ポイント群取得
	 * @return 距離標ポイント群(距離標値, CSRowBase)
	*/
	std::map<double, CSPRowBase> findDistancePoints();

	/**
	 * @brief 距離標ポイント取得
	 * @param dbVal [in] 距離標値
	 * @return 距離標ポイント
	*/
	CSPRowBase findDistancePoint(double dbVal);

	/**
	 * @brief 距離標ポイント群形状取得
	 * @return 距離標ポイント群形状
	*/
	IGeometryPtr createDistancePointGeom();

	/**
	 * @brief 道路検索
	 * @param ipGeom [in] 検索形状
	 * @param dbBuf [in] バッファ値
	 * @param roadCode [in] 路線コード ※基点、次点が対象路線コードを持つ道路に乗っているときに限る
	 * @return 道路群
	*/
	CFeatureClass::Container searchRoads(const IGeometryPtr& ipGeom, double dbBuff, const long& roadCode = -1);

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
	* @brief 距離標ポイントフィーチャクラス取得
	* @return 距離標ポイントフィーチャクラス
	*/
	IFeatureClassPtr getDistancePost() { return m_cDistancePostClass; }

	/**
	* @brief 道路ノードフィーチャクラス取得
	* @return 道路ノードフィーチャクラス
	*/
	IFeatureClassPtr getRoadNode() { return m_cRoadNodeClass; }

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
	 * @brief 距離標値間の距離と距離標ポイント間の距離をチェック [bug 12155]
	 * @param pointLen [in] 距離標ポイント間の道路距離
	 * @param distanceDiff [in] 距離標値の差
	 * @param pitch [in] ピッチ
	 * @param range [in] 許容値
	 * @return エラーがあるかどうか
	*/
	bool checkRoadLen(const double& pointLen, const double& distanceDiff, const double& pitch, const double& range);

	/**
	 * @brief COMオブジェクトのコピーを作成する
	 * @param ipUnk [in] クローン元
	 * @return クローン
	*/
	static IClonePtr Clone( const IUnknownPtr& ipUnk );

	/**
	 * @brief 切り詰め
	 * @param dbVal [in] 値
	 * @param nLen [in] 桁
	 * @return 切り詰めた値
	*/
	CString convertStr(double dbVal, long nLen=2);

	/**
	 * @brief 分割点までの距離取得 [bug12155]
	 * @param ipPolyline [in] ライン形状
	 * @param ipPoint [in] 分割ポイント
	 * @return 分割点までの距離 [m]
	*/
	double split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint);

	/**
	 * @brief 道路検索（OID指定）[bug12155]
	 * @param listRouteIDs [in] OID群
	 * @return 検索道路群
	*/
	CFeatureClass::Container searchRoadsByOID(const std::list<long>& listRouteIDs);

	/**
	* @brief 対象路線上の道路であるかチェック [bug12155]
	* @param roadContainer [in] 道路群
	* @param roadCode      [in] 路線コード
	* @param roadCodeIdx   [in] 路線コードのフィールドインデックス
	* @retval true  対象路線上の道路である
	* @retval false 対象路線上の道路でない
	*/
	bool checkOnHWY(const CFeatureClass::Container& roadContainer, const long& roadCode, long roadCodeIdx);

	/**
	* @brief 全体の経路形状を作成する [bug12155]
	*        routePolylineは形状を持っている前提で道路リンク群をroutePolylineに継ぎ足していく。
	* @param routePolyline [in, out] 経路形状
	* @param roadContainer [in] 道路リンク群
	* @param routeIDs      [in] 道路リンクのOID群
	* @param ipPrjCoordinateSystem [in] 座標系変換用
	*/
	void getRoutePolyline( IPolylinePtr& routePolyline, const CFeatureClass::Container& roadContainer, const list<long>& routeIDs, const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem );

	/**
	* @brief ポリラインに乗っているポイント間の距離を取得する [bug12155]
	*        (point2 - point1)
	* @param polyline [in] ポリライン形状
	* @param point1   [in] ポイント形状1
	* @param point2   [in] ポイント形状2
	* @return ポイント間距離 [m]
	*/
	double getPointLen( const IPolylinePtr& polyline, const IPointPtr& point1, const IPointPtr& point2 );

private:
	CWorkspace m_cWorkspace;                    ///< ワークスペース
	CFeatureClass m_cDistancePostClass;         ///< 距離標ポイントクラス
	CFeatureClass m_cRoadLinkClass;             ///< 道路リンククラス
	CFeatureClass m_cRoadNodeClass;             ///< 道路ノードクラス
	CFeatureClass m_cBaseMeshClass;             ///< ２次メッシュクラス

	std::map<long, boost::tuple<long, long, CString>> m_mapRoadList; ///< 路線リスト群
};

}
