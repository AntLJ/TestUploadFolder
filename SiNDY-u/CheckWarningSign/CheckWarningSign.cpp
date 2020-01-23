#include "stdafx.h"
#include "LogSys.h"
#include "TransParam.h"
#include "CheckWarningSign.h"

using namespace sindy::schema;

const double LATLNG_DIGIT_LEVEL = 0.0000125;	// 1メートルあたりの緯度経度値	(多少大きめにバッファを作成するため北海道の緯度を基準に設定)

// 警戒標識ポイント⇔道路リンクの直線距離の取得処理
bool directDistance(const IPointPtr& warningSignPoint, const IPointPtr& nearestPoint, double& pointToPointDistance)
{
	// 警戒標識ポイントと道路リンク最近傍ポイントのXY座標を取得
	double ws_x = 0.0;
	double ws_y = 0.0;
	double near_x = 0.0;
	double near_y =0.0;
	if(FAILED(warningSignPoint->QueryCoords(&ws_x,&ws_y)) ||
		FAILED(nearestPoint->QueryCoords(&near_x,&near_y))) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("ポイントのXY座標取得に失敗しました"));
		return false;
	}

	// 直線距離を算出
	crd_cnv crd;
	pointToPointDistance = crd.GetDist(ws_x, ws_y, near_x, near_y);
	return true;
}

// 道路表示種別の検索処理
bool getDomainName(const std::map<long, CString>& mapDomain, const long& codedVal, CString& domainName)
{
	auto it = mapDomain.find(codedVal);
	if(it != mapDomain.end()) {
		domainName = it->second;
	} else {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("道路表示種別コード値が存在しません"));
		return false;
	}
	return true;
}

// 最近傍道路リンク情報の出力処理
bool checkWarningSign(const TransParam& transParam, const IFeatureClassPtr& ipRoadFC, const ISpatialReferencePtr& ipRoadSPRef, const long& roadClassIndex, const std::map<long, CAdapt<IPointPtr>>& mapGeo, const std::map<long, CString>& mapDomain, std::ofstream &ofsOutputLog)
{
	// バッファ値(メートル)を緯度経度単位に変換
	double latLngBuffer = transParam.getBuffer() * LATLNG_DIGIT_LEVEL;

	for(std::map<long, CAdapt<IPointPtr>>::const_iterator it = mapGeo.begin(); it != mapGeo.end(); ++it)
	{
		// Buffer作成
		ITopologicalOperatorPtr ipTopo(it->second.m_T);
		IGeometryPtr ipBuffer;
		double bufferValue = latLngBuffer;
		if(FAILED(ipTopo->Buffer(bufferValue, &ipBuffer))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("警戒標識のバッファ取得に失敗しました"));
			return false;
		}

		//空間参照の付与
		if(FAILED(ipBuffer->putref_SpatialReference( ipRoadSPRef))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("空間参照の付与に失敗しました"));
			return false;
		}

		// Road_Linkの検索
		CString roadWhereCluse;
		roadWhereCluse = transParam.getRoadWhere();

		IFeatureCursorPtr ipRoadFeatureCursor;
		if (! common_proc::getFeatureCursor(ipRoadFC,road_link::kTableName, ipBuffer, esriSpatialRelIntersects, roadWhereCluse, ipRoadFeatureCursor))
			return false;

		long shOID = 0;			// 最近傍道路リンクのOID
		long shRoadClass = 0;	// 最近傍道路リンクの道路標示種別コード
		double shDistance = DBL_MAX;	// 警戒標識⇔最近傍道路リンク間の距離

		// 警戒標識と最近傍の道路リンク取得処理
		IFeaturePtr ipRoadFeature;
		while (ipRoadFeatureCursor->NextFeature( &ipRoadFeature) == S_OK) {
			long lOID = 0;
			CComVariant vaRoadClass;

			// 道路OIDの取得
			if(FAILED(ipRoadFeature->get_OID(&lOID))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("道路リンクのオブジェクトIDの取得に失敗しました"));
				return false;
			}

			// 道路表示種別の取得
			if(FAILED(ipRoadFeature->get_Value(roadClassIndex, &vaRoadClass))) {
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("道路表示種別の取得に失敗しました"));
				return false;
			}

			// 警戒標識ポイント ⇔ 道路リンク間の最短距離の算出
			double pointToPointDistance = -1.0;
			IGeometryPtr ipGeom;
			IPointPtr nearestPoint;
			if(FAILED(ipRoadFeature->get_ShapeCopy(&ipGeom))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("ジオメトリのコピーの取得に失敗しました"));
				return false;
			}

			IProximityOperatorPtr ipProx(ipGeom);
			if(FAILED(ipProx->ReturnNearestPoint(it->second.m_T, esriNoExtension, &nearestPoint))){
				LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("道路リンク近接ポイントの取得に失敗しました"));
				return false;
			}

			// 警戒標識と道路リンクの直線距離を取得
			if(! directDistance(it->second.m_T, nearestPoint, pointToPointDistance)){
				return false;
			}

			//最近傍道路リンクの候補を格納
			/*
			 * 距離が同一の場合は道路表示種別コード値の小さい道路リンクを候補とする。
			 * 距離、コード値が同一の場合はOIDが小さい道路リンクを候補とする。
			 */
			if(pointToPointDistance >= 0.0) {
				if((shDistance > pointToPointDistance) ||
					(shDistance == pointToPointDistance && shRoadClass > vaRoadClass.lVal) ||
					(shDistance == pointToPointDistance && shRoadClass == vaRoadClass.lVal && shOID > lOID)) {
					shOID = lOID;
					shRoadClass = vaRoadClass.lVal;
					shDistance = pointToPointDistance;
				}
			}
		}

		// ログに出力
		CString strOut;

		if(shOID) {
			CString roadClassName;
			// コード値ドメインから道路表示名称を取得
			if(! getDomainName(mapDomain, shRoadClass, roadClassName))
				return false;
			// 最近傍道路リンク情報を出力
			strOut.Format( _T("%ld\t%ld\t%s\t%0.10f"), it->first, shOID, roadClassName, shDistance);
		} else {
			// 警戒標識のOIDのみ出力
			strOut.Format( _T("%ld\t\t\t"), it->first);
		}
		ofsOutputLog << CT2A( strOut ) << std::endl;
	}
	return true;
}