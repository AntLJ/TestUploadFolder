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
#include "AheBuddyGlobals.h"

// STL
#include <algorithm>

// ArcHelperEx
#include <GlobalFunctions.h>
#include <AheGlobalsCoord.h>
#include <AheGeometryOp.h>
#include <AheGeometryConvert.h>
#include <AheGlobalsMisc.h>

// crd_cnv
#include <crd_cnv/crd_cnv.h>


#ifdef _DEBUG
// DEBUG
#include "../../LIB/sindy/workspace.h"
#endif


extern crd_cnv g_cnv;

namespace sindy{

#ifdef _DEBUG
/**
 * @brief  [DEBUG用] フィーチャを出力する
 * @param [in]  table    出力先のフィーチャクラス
 * @param [in]  出力するジオメトリ
 * @return 出力されたフィーチャ
 */
IFeaturePtr insertFeature( ITablePtr ipTable, IGeometryPtr ipGeom )
{
	_IRowPtr row;
	ipTable->CreateRow(&row);

	((IFeaturePtr)row)->putref_Shape(ipGeom);
	if( SUCCEEDED(row->Store()) )
		return row;
	return nullptr;
}

/**
 * @brief  [DEBUG用] 検索ラインの軌跡をフィーチャとして出力する
 * @detail 出力先のフィーチャ名は「TRACE_LINE」で固定
 * @param [in]  ipPolyline  出力するライン
 * @retval true  出力成功
 * @retval false 出力失敗
 */
bool outputSearchLine( IPolylinePtr ipPolyline )
{
	IFeatureWorkspacePtr ipWorkSpace = sindy::create_workspace(_T("..\\unittest\\TestData\\WalkNw.mdb"));
	IFeatureClassPtr ipFC;
	if( FAILED( ipWorkSpace->OpenFeatureClass(CComBSTR( _T("TRACE_LINE")), &ipFC ) ))
		return false;

	insertFeature( ipFC, ipPolyline );

	return true;
}

/**
 * @brief  [DEBUG用] 検索ラインの軌跡をフィーチャとして出力する
 * @detail 出力先のフィーチャ名は「BUFFER_LINK」で固定
 * @param [in]  ipBufGeo  出力するGeometry
 * @retval true  出力成功
 * @retval false 出力失敗
 */
bool outputBufferLine( IGeometryPtr ipBufGeo )
{
	IFeatureWorkspacePtr ipWorkSpace = sindy::create_workspace(_T("..\\unittest\\TestData\\WalkNw.mdb"));
	IFeatureClassPtr ipFC;
	if( FAILED( ipWorkSpace->OpenFeatureClass(CComBSTR( _T("BUFFER_LINK")), &ipFC ) ))
		return false;

	insertFeature( ipFC, ipBufGeo );

	return true;


}
#endif	// DEBUG


/**
 * @brief  現在の地理座標での距離からメートルを計算する
 * @param  [in]  coordinate  現在の地理座標系での距離
 * @param  [in]  geom        地理座標系の基準とするジオメトリ
 * @return メートルでの距離
 * @attention   X座標のみで計算し算出
 */
double coordinate2Meter( double coordinate, const IGeometryPtr& geom )
{
	// リファレンスポイント作成
	IPointPtr point;
	esriGeometryType geomType = esriGeometryPoint;
	geom->get_GeometryType( &geomType );
	switch( geomType )
	{
	case esriGeometryPoint:
		point = AheCreateClone(geom);
		break;
	case esriGeometryEnvelope:
		((IAreaPtr)geom)->get_Centroid(&point);
		break;
	default:
		((IPointCollectionPtr)geom)->get_Point(0,&point);
		break;
	}
 
	double dX(0.0f), dY(0.0f);
	point->QueryCoords( &dX, &dY );

	double dX2 = dX + coordinate , dY2 = dY;
	double dXDist(0.0f), dYDist(0.0f);
	return g_cnv.GetDistXY( dX, dY, dX2, dY2, &dXDist, &dYDist );
}


/**
 * @brief  メートルから現在の地理座標での距離を計算する
 * @note   UpdateGenerationDiff のソースコードから拝借
 * @param  [in]  meter メートル
 * @param  [in]  geom  地理座標系の基準とするジオメトリ
 * @return 地理座標系での距離
 * @attention   X座標のみで計算し算出
 */
double meter2Coordinate( double meter, const IGeometryPtr& geom )
{
		// リファレンスポイント作成
	IPointPtr point;
	esriGeometryType geomType = esriGeometryPoint;
	geom->get_GeometryType( &geomType );
	switch( geomType )
	{
	case esriGeometryPoint:
		point = AheCreateClone(geom);
		break;
	case esriGeometryEnvelope:
		((IAreaPtr)geom)->get_Centroid(&point);
		break;
	default:
		((IPointCollectionPtr)geom)->get_Point(0,&point);
		break;
	}

	double dX(0.0f), dY(0.0f);
	point->QueryCoords( &dX, &dY );

	double dLon(0.0f), dLat(0.0f);
	g_cnv.GetLLPt( dX, dY, meter, 0, &dLon, &dLat ); 

	return abs( dLon - dX );
}


/**
 * @brief  ポリラインをセグメント単位で分割したポリライン群で返す
 * @param  [in]  ipPolyline 分割するポリライン
 * @return 分割後のポリライン群
 */
std::vector<CAdapt<IPolylinePtr>> splitPolyline( const IPolylinePtr& ipPolyline )
{
	ISpatialReferencePtr ipSpRef;
	ipPolyline->get_SpatialReference( &ipSpRef );

	// 分割後のポリライン群格納用
	std::vector<CAdapt<IPolylinePtr>> retPolylines;

	ISegmentCollectionPtr ipSegCol( ipPolyline );
	long lSegCount(-1L);
	ipSegCol->get_SegmentCount( &lSegCount );
	for( long i = 0; i < lSegCount; ++i )
	{
		_ISegmentPtr ipPartSeg;
		ipSegCol->get_Segment( i, &ipPartSeg );

		ISegmentCollectionPtr ipTmpSegCol(CLSID__Path);
		((IGeometryPtr)ipTmpSegCol)->putref_SpatialReference(ipSpRef);
		ipTmpSegCol->AddSegment(ipPartSeg);

		IGeometryCollectionPtr ipTmpGeoCol(CLSID_Polyline);
		((IGeometryPtr)ipTmpGeoCol)->putref_SpatialReference(ipSpRef);
		ipTmpGeoCol->AddGeometry(IGeometryPtr(ipTmpSegCol));

		IPolylinePtr ipTmpPolyline(ipTmpGeoCol);
		ipTmpPolyline->putref_SpatialReference( ipSpRef );

		retPolylines.push_back(ipTmpPolyline);
	}

	return retPolylines;
}


/**
 * @brief  ポリラインからみてポイントが左右どちらの位置にあるかを判定する
 * @param  [in]  ipBasePolyline 基準となるポリライン
 * @param  [in]  ipPoint        判定するポイント
 * @return 右 or 左
 */
LinkRelationPosition getPointPosition( const IPolylinePtr& ipBasePolyline, const IPointPtr& ipPoint )
{
	IPointPtr ipFromPoint, ipToPoint;
	ipBasePolyline->get_FromPoint(&ipFromPoint);
	ipBasePolyline->get_ToPoint(&ipToPoint);

	double dX1(0.0f), dY1(0.0f), dX2(0.0f), dY2(0.0f), dX3(0.0f), dY3(0.0f);
	ipFromPoint->QueryCoords( &dX1, &dY1 );
	ipToPoint->QueryCoords( &dX2, &dY2 );
	ipPoint->QueryCoords( &dX3, &dY3 );

	// 外積で判定
	double dOuterProduct = (dX2 - dX1)*(dY3 - dY1) - (dY2 - dY1)*(dX3 - dX1);

	return ( dOuterProduct > 0 ) ? kLeft : kRight;
}

/**
 * @brief  検索ライン作成関数
 * @param  [in]  ipBaseLine       検索元となるポリライン
 * @param  [in]  ipOriginPt		　検索開始位置
 * @param  [in]  dSearchDistance  検索距離
 * @param  [in]  nDegree          検索元のポリラインを基準とした検索角度
 * @param  [out] ipSearchLine     検索ライン
 * @retval true  成功
 * @retval false 失敗
 */
bool makeSearchLine( const IPolylinePtr& ipBaseLine, const IPointPtr& ipOriginPt,
				    double dSearchDistance, int nDegree, IPolylinePtr& ipSearchLine)
{
	// 検索ラインを作成(ベースのラインを 移動->回転->長さ変更 して作る)
	ipSearchLine = AheCreateClone(ipBaseLine);

	//   始点位置を移動
	double dOrigX(0.0f), dOrigY(0.0f), dFromX(0.0f), dFromY(0.0f);
	ipOriginPt->QueryCoords( &dOrigX, &dOrigY );

	IPointPtr ipBaseFromPt;
	ipBaseLine->get_FromPoint(&ipBaseFromPt);
	ipBaseFromPt->QueryCoords( &dFromX, &dFromY );

	double dDistX = dOrigX - dFromX;
	double dDistY = dOrigY - dFromY;
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Move( dDistX, dDistY ))) {
		_ASSERTE(FALSE);
		return false;
	}

	//   検索ラインを指定角度回転   
	//     度数 -> ラジアン 変換
	double dRadian = ((double)nDegree / 360.0) * 2.0 * M_PI;
	//     始点位置から回転
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Rotate( ipOriginPt, dRadian ))){
		_ASSERTE(FALSE);
		return false;
	}

	//   検索ラインの長さを指定した長さに変換
	double dLength(0.0);
	ipSearchLine->get_Length( &dLength );
	double dRatio = dSearchDistance / dLength;
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Scale( ipOriginPt, dRatio, dRatio ))) {
		_ASSERTE(FALSE);
		return false;
	}
	return true;
}

/**
 * @brief  検索元のポリライン(ベース側)から、検索開始位置・距離・角度を指定した範囲内に、<br>
 *         検索先のポリラインが存在するかを返す
 * @detail 検索方向へラインを作成し、対象と交差するかどうかで判定する<br>
 *         見つからなかった場合は ePosition で kNotFound を返す(返り値は true)
 * @param  [in]  ipBaseLine       検索元となるポリライン
 * @param  [in]  ipTargetLine     検索先となるポリライン
 * @param  [in]  ipOriginPt       検索開始位置(ここを基準に指定角度、距離へ検索する)
 * @param  [in]  dSearchDistance  検索距離
 * @param  [in]  nDegree          検索元のポリラインを基準とした検索角度
 * @param  [out] ePosition        ターゲット側からみてベース側が左右どちらに存在するか
 * @param  [out] dFindDistance    ベースからターゲットへの距離
 * @retval true  正常に検索成功
 * @retval false 検索が不正終了
 */
bool findLine( const IPolylinePtr& ipBaseLine, const IPolylinePtr& ipTargetLine, const IPointPtr ipOriginPt, 
			   double dSearchDistance, int nDegree, LinkRelationPosition& ePosition, double& dFindDistance )
{
	dFindDistance = DBL_MAX;
	ePosition = kNotFound;

	// 検索ライン作成
	IPolylinePtr ipSearchLine;

	if( ! makeSearchLine( ipBaseLine, ipOriginPt, dSearchDistance, nDegree, ipSearchLine ) )
		return false;
		

// [DEBUG] 検索ラインを出力
//#ifdef _DEBUG
//	outputSearchLine( ipSearchLine );
//#endif


	// ポリライン同士が交差していなければ、見つからなかったと判定
	if( ( ! AheIsCross( ipSearchLine, ipTargetLine ) ))
		return true;


	// ポリラインが見つかったなら以下を取得する
	//   - 見つかったポリラインはベース側のポリラインから見て左右どちらに位置するか (-> ePosition)
	//   - 見つかったポリラインまでの距離 (-> dFindDistance)

	// ターゲットのポリラインを構成するラインから、検索ラインと交差するライン群を取得
	// (複数交差する箇所が見つかった場合を考慮)
	std::vector<CAdapt<IPolylinePtr>> lines = splitPolyline( ipTargetLine );
	std::vector<CAdapt<IPolylinePtr>> find_polylines;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = lines.begin(); line_iter != lines.end(); ++line_iter )
	{
		if( AheIsCross( ipSearchLine, line_iter->m_T ) )
			find_polylines.push_back( *line_iter );
	}

	// 見つかったラインのうち最短距離にあるラインに対して、
	// 左右位置情報と距離を取得する
	IPointPtr ipSearchFromPt;
	ipSearchLine->get_FromPoint(&ipSearchFromPt);
	dFindDistance = DBL_MAX;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = find_polylines.begin(); line_iter != find_polylines.end(); ++line_iter )
	{
		// AheIsCrossでクロス判定がでたとしてもAheGetCrossOrTouchPointsでは、精度の違いによりTouchと判定されるケースがあるため、
		// CrossとTouch両方のケースを取得する。
		IGeometryPtr ipGeo = AheGetCrossOrTouchPoints( line_iter->m_T, ipSearchLine );
		if( ! ipGeo ) {
			_ASSERTE(FALSE);
			return false;
		}

		double dist(DBL_MAX);
		if( FAILED((( IProximityOperatorPtr)ipGeo)->ReturnDistance( ipSearchFromPt, &dist))) {
			_ASSERTE(FALSE);
			return false;
		}

		if( dist < dFindDistance ) {
			dFindDistance = dist;
			ePosition = getPointPosition( *line_iter, ipSearchFromPt );
		}

	}

	return true;
}

/**
 * @brief  ポリラインの始点と終点の中点を取得する
 * @param  [in]  ipPolyline 中点を求めるポリライン
 * @return ポリラインの中点
 */
IPointPtr getMiddlePoint( const IPolylinePtr& ipPolyline )
{
	ISpatialReferencePtr ipRef;
	ipPolyline->get_SpatialReference( &ipRef );

	IPointPtr ipFromPt, ipToPt;
	ipPolyline->get_FromPoint(&ipFromPt);
	ipPolyline->get_ToPoint(&ipToPt);

	double dFromX(0.0f), dFromY(0.0f), dToX(0.0f), dToY(0.0f);	
	ipFromPt->QueryCoords( &dFromX, &dFromY );
	ipToPt->QueryCoords( &dToX,&dToY );

	double dDistX = dToX - dFromX;
	double dDistY = dToY - dFromY;

	IPointPtr ipMiddlePt(CLSID_Point);
	ipMiddlePt->putref_SpatialReference( ipRef );

	ipMiddlePt->put_X( dFromX + dDistX / 2.0 );
	ipMiddlePt->put_Y( dFromY + dDistY / 2.0 );

	return ipMiddlePt;
}


/**
 *	@brief 左右判定情報格納用の構造体
 */
struct RightLeftRelInfo
{
public:
	RightLeftRelInfo() : mRightCnt(0), mLeftCnt(0), mMinRightDist(DBL_MAX), mMinLeftDist(DBL_MAX){}
	~RightLeftRelInfo(){}

	// 情報のクリア
	void clear(){  mRightCnt = 0; mLeftCnt = 0; mMinRightDist = DBL_MAX; mMinLeftDist = DBL_MAX; }
	// 左右情報が格納されているかどうか
	bool isEmpty(){ return mRightCnt == 0 && mLeftCnt == 0; }

	// 左右情報を更新
	void addRelInfo( LinkRelationPosition position, double distance )
	{
		if( position == kRight ) {
			++mRightCnt;
			if( mMinRightDist > distance )
				mMinRightDist = distance;
		}
		else if( position == kLeft ) {
			++mLeftCnt;
			if( mMinLeftDist > distance )
				mMinLeftDist = distance;
		}	
	}

	// 左右の見つかった数と距離からどちら側に位置するリンクかを返す
	LinkRelationPosition getRightOrLeft()
	{
		using namespace sindy;

		if( isEmpty() )
			return kNotFound;

		if( mRightCnt > mLeftCnt )
			return kRight;
		else if( mRightCnt < mLeftCnt )
			return kLeft;

		if( mMinRightDist < mMinLeftDist )
			return kRight;
		else if( mMinRightDist > mMinLeftDist )
			return kLeft;
			
		// 距離まで一緒ならもう右を返してしまう
		return kRight;
	}

	// 指定された方向の最短距離を返す
	double getMinDist( LinkRelationPosition ePostion )
	{
		switch ( ePostion )
		{
			case kLeft:
				return mMinLeftDist;
			case kRight:
				return mMinRightDist;
			default:
				return 0;
		}
	}

private:
	long   mRightCnt;		//!< 右側に見つかったライン数
	long   mLeftCnt;		//!< 左側に見つかったライン数
	double mMinRightDist;	//!< 右側に見つかったラインへの最短距離
	double mMinLeftDist;	//!< 左側に見つかったラインへの最短距離
};


/**
 * @brief  ベースのポリラインからターゲットのポリラインへ検索して位置関係を取得する
 * @param  [in]  ipBasePolyline    ベース側のポリライン
 * @param  [in]  ipTargetPolyline  ターゲット側のポリライン
 * @param  [in]  dSearchDistance   検索距離
 * @param  [out] RelInfo           取得された位置関係
 */
bool searchBaseToTarget(const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchDistance, RightLeftRelInfo& RelInfo )
{
	RelInfo.clear();

	std::vector<CAdapt<IPolylinePtr>> BasePolylines = splitPolyline( ipBasePolyline );

	// 距離取得用
	double dDistance(DBL_MAX);
	LinkRelationPosition position = kNotFound;

	// 始点から左右に垂線を引いて検索
	IPointPtr ipFromPt;
	ipBasePolyline->get_FromPoint(&ipFromPt);
	//   左に検索
	if( ! findLine( BasePolylines.front(), ipTargetPolyline, ipFromPt, dSearchDistance, 90, position, dDistance))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kLeft, dDistance );
	//  右に検索
	if( ! findLine( BasePolylines.front(), ipTargetPolyline, ipFromPt, dSearchDistance, -90, position, dDistance ))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kRight, dDistance );

	// 終点から左右に垂線を引いて検索
	IPointPtr ipToPt;
	ipBasePolyline->get_ToPoint(&ipToPt);
	//   左に検索
	if( ! findLine( BasePolylines.back(), ipTargetPolyline, ipToPt, dSearchDistance, 90, position, dDistance))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kLeft, dDistance );
	//   右に検索
	if( ! findLine( BasePolylines.back(), ipTargetPolyline, ipToPt, dSearchDistance, -90, position, dDistance ))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kRight, dDistance );


	// ポリラインを構成する各セグメントの中点から検索
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = BasePolylines.begin(); line_iter != BasePolylines.end(); ++line_iter )
	{
		// セグメント中点から左右へ垂線を引いて検索
		IPointPtr ipSearchOrgPt = getMiddlePoint( *line_iter );
		//   左に検索
		if( ! findLine( *line_iter, ipTargetPolyline, ipSearchOrgPt, dSearchDistance, 90, position, dDistance))
			return false;
		if( position != kNotFound )
			RelInfo.addRelInfo( kLeft, dDistance );
		//   右に検索
		if( ! findLine( *line_iter, ipTargetPolyline, ipSearchOrgPt, dSearchDistance, -90, position, dDistance ))
			return false;
		if( position != kNotFound )
			RelInfo.addRelInfo( kRight, dDistance );
	}
	return true;
}


/**
 * @brief  ターゲットのポリラインからベースのポリラインへ検索して位置関係を取得する
 * @param  [in]  ipBasePolyline    ベース側のポリライン
 * @param  [in]  ipTargetPolyline  ターゲット側のポリライン
 * @param  [in]  dSearchDistance   検索距離
 * @param  [out] RelInfo           取得された位置関係
 * @retval true
 */
bool searchTargetToBsse(const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchDistance, RightLeftRelInfo& RelInfo )
{
	RelInfo.clear();

	std::vector<CAdapt<IPolylinePtr>> TargetPolylines = splitPolyline( ipTargetPolyline );

	// 距離取得用
	double dDistance(DBL_MAX);
	LinkRelationPosition position = kNotFound;

	// 始点から左右に垂線を引いて検索
	IPointPtr ipFromPt;
	ipTargetPolyline->get_FromPoint(&ipFromPt);
	//   左に検索
	if( ! findLine( TargetPolylines.front(), ipBasePolyline, ipFromPt, dSearchDistance, 90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );
	//   右に検索
	if( ! findLine( TargetPolylines.front(), ipBasePolyline, ipFromPt, dSearchDistance, -90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );

	// 終点から左右に垂線を引いて検索
	IPointPtr ipToPt;
	ipTargetPolyline->get_ToPoint(&ipToPt);
	//   左に検索　 
	if( ! findLine( TargetPolylines.back(), ipBasePolyline, ipToPt, dSearchDistance, 90, position, dDistance))
		return false;
	RelInfo.addRelInfo( position, dDistance );
	//    右に検索
	if( ! findLine( TargetPolylines.back(), ipBasePolyline, ipToPt, dSearchDistance, -90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );

	// ポリラインを構成する各セグメントの中点から検索
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = TargetPolylines.begin(); line_iter != TargetPolylines.end(); ++line_iter )
	{
		// セグメントの中点を取得
		IPointPtr ipSearchOrgPt = getMiddlePoint( *line_iter );

		// 中点から左右へ垂線を引いて検索
		//   左に検索
		if( ! findLine( *line_iter, ipBasePolyline, ipSearchOrgPt, dSearchDistance, 90, position, dDistance))
			return false;
		RelInfo.addRelInfo( position, dDistance );
		//   右に検索
		if( ! findLine( *line_iter, ipBasePolyline, ipSearchOrgPt, dSearchDistance, -90, position, dDistance ))
			return false;
		RelInfo.addRelInfo( position, dDistance );
	}
	return true;
}


// 基準のポリラインから見て、ターゲットのポリラインが左右どちらに位置するかを判定する
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline,  double dSearchMeter, LinkRelationPosition& eRelationInfo, double& dMinMeter )
{
	if( ! ipBasePolyline || ! ipTargetPolyline || dSearchMeter <= 0 )
		return false;

	eRelationInfo = kNotFound;
	dMinMeter = -1;

	// 左右判定情報格納用の構造体
	RightLeftRelInfo rel_info;

	// ベースとターゲットの長さを取得
	double dBaseLength(DBL_MAX), dTargetLength(DBL_MAX);
	ipBasePolyline->get_Length( &dBaseLength );
	ipTargetPolyline->get_Length( &dTargetLength );

	if( dBaseLength == 0.0 || dTargetLength == 0.0 )
		return false;

	// メートル->地理座標変換
	double dSearchDistance = meter2Coordinate( dSearchMeter, ipBasePolyline );

	// ベース側のポリラインの方が短ければ、ベース側を軸に検索し、
	// 見つからなければターゲット側を軸に検索
	if( dBaseLength < dTargetLength )
	{
		if( ! searchBaseToTarget( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
		
		if( rel_info.isEmpty() && ! searchTargetToBsse( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
	}
	else
	{
		// ターゲット側のポリラインの方が短ければ、ターゲット側を軸に検索し、
		// 見つからなければベース側を軸に検索
		if( ! searchTargetToBsse( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;

		if( rel_info.isEmpty() && ! searchBaseToTarget( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
	}

	if( ! rel_info.isEmpty() ) {
		eRelationInfo = rel_info.getRightOrLeft();
		dMinMeter = coordinate2Meter(rel_info.getMinDist( eRelationInfo ), ipBasePolyline);
		return true;
	}

	return true;
}

bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, LinkRelationPosition& eRelationInfo )
{
	// 一時格納用変数
	double minMeter = 0.0;
	return AheJudgeLinkRelation(ipBasePolyline, ipTargetPolyline, dSearchMeter, eRelationInfo, minMeter);
}

/**
 * @brief  チェック判定対象リンク取得関数
 * @param  [in] ipGeomCol 分断リンク集合
 * @param  [in] ipBasePolyline チェックの際に基準となるリンク
 * @param  [in] eRelationInfo 分断前リンクの基準リンクからの相対位置
 * @param  [in] dSerachMeter  相対位置検索距離(m)
 * @param  [out] ipJudgeLine チェック判定対象リンク
 * @retval true 成功
 * @retval false 失敗
 */
bool selectJudgeLine( const IGeometryCollectionPtr& ipGeomCol,
					  const IPolylinePtr& ipBasePolyline,
					  LinkRelationPosition eRelationInfo,
					  double dSearchMeter,
					  IPolylinePtr& ipJudgeLine )
{
	// 共通部分が分断されている場合は、各分断リンクに対して相対位置を取得し、分割前形状との相対位置が一致し、
	// 尚且つ基準リンクに一番近いリンクに対して、判定処理を行う。(一番近いリンクが複数ある場合はリンク長が、
	// 一番長いリンクで判定)

	// 垂線を伸ばした時の距離最小値格納用変数→初期値は、dSerachMeter+1.0を設定(dSearchMeterより長くなることは無いため(正確ではないので一応＋1.0する))
	double dMinJudgeMeter = dSearchMeter+1.0;

	// 判定対象になりうる分断リンクの距離最大値格納用変数
	long count = 0;
	if ( FAILED(ipGeomCol->get_GeometryCount(&count)) ) 
		return false;
	double dMaxPartLinkLength = 0.0;
	for (long i = 0; i < count; ++i) {
		IGeometryPtr ipPartGeo;
		if ( FAILED(ipGeomCol->get_Geometry(i, &ipPartGeo)) )
			return false;
		IPolylinePtr ipPartLine(ToPolyline(ipPartGeo));
		if ( !ipPartLine )
			return false;
		// 分断リンクに対して再度、左右判定を実施
		LinkRelationPosition ePartRelation = kNotFound;
		double dPartJudgeMeter = 0.0;
		if ( !AheJudgeLinkRelation(ipBasePolyline, ipPartLine, dSearchMeter, ePartRelation, dPartJudgeMeter) )
			return false;
		// 分断前リンクと相対位置が一致しない場合は飛ばす
		if ( eRelationInfo != ePartRelation ) 
			continue;
		// dMinJudgeDistacnceより近い場合は、更新
		// dMinJudgeDistanceと同じ距離の場合は、分断リンクの長さがdMaxPartLinkLengthより長い場合、更新
		double dPartLength = 0.0;
		if ( FAILED(((IPolylinePtr)ipPartLine)->get_Length(&dPartLength)) )
			return false;
		if ( dMinJudgeMeter > dPartJudgeMeter ) {
			ipJudgeLine = ipPartLine;
			dMinJudgeMeter = dPartJudgeMeter;
			dMaxPartLinkLength = dPartLength;
		} else if ( dMinJudgeMeter == dPartJudgeMeter ) {
			if ( dMaxPartLinkLength < dPartLength ) {
				ipJudgeLine = ipPartLine;
				dMinJudgeMeter = dPartJudgeMeter;
				dMaxPartLinkLength = dPartLength;
			}
		}
	}
	return true;
}

/**
 * @brief  セグメント形状バット先端のバッファを作成
 * @param  [in]  ipSegmentLine	変換元ポリライン
 * @param  [in]	 ipPreSegBuf	一つ前のセグメントのバッファ形状
 * @param  [in]	 ipPreToLeftPt	一つ前のセグメントのバッファ形状のセグメントの終点側の左側のポイント
 * @param  [in]	 ipPreToRightPt 一つ前のセグメントのバッファ形状のセグメントの終点側の右側のポイント
 * @param  [in]  dBufDsitance	バッファ距離(度)
 * @param  [out] ipSegmentBuf	変換元ポリラインのバッファ形状
 * @param  [out] ipConnectBuf	一つ前のセグメントのバッファ形状と今回のバッファ形状の隙間を埋める形状
 * @param  [out] ipOutToLeftPt	変換元ポリラインのバッファ形状のセグメントの終点側の左側のポイント
 * @param  [out] ipOutToRightPt	変換元ポリラインのバッファ形状のセグメントの終点側の右側のポイント
 * @retval true  成功
 * @retval false 失敗
 */
bool makeSegmentBatBuf( const IPolylinePtr& ipSegmentLine,
					    const IGeometryPtr& ipPreSegBuf,
					    const IPointPtr& ipPreToLeftPt,
						const IPointPtr& ipPreToRightPt,
						double dBufDistance,
						IGeometryPtr& ipSegmentBuf,
						IGeometryPtr& ipConnectBuf,
						IPointPtr& ipOutToLeftPt,
						IPointPtr& ipOutToRightPt)
{
	IPointPtr ipSegFromPt, ipSegToPt;
	if ( FAILED(ipSegmentLine->get_FromPoint(&ipSegFromPt)) )
		return false;
	if ( FAILED(ipSegmentLine->get_ToPoint(&ipSegToPt)) )
		return false;

	IPolylinePtr ipFromLeftLine, ipFromRightLine, ipToLeftLine, ipToRightLine;
	// 始点から左右にdBufDistance分のライン作成
	if ( ! makeSearchLine(ipSegmentLine, ipSegFromPt, dBufDistance, 90, ipFromLeftLine) )
		return false;
	if ( ! makeSearchLine(ipSegmentLine, ipSegFromPt, dBufDistance, -90, ipFromRightLine) )
		return false;
	// 終点から左右にdBufDistance分のライン作成
	if ( ! makeSearchLine(ipSegmentLine, ipSegToPt, dBufDistance, 90, ipToLeftLine) )
		return false;
	if ( ! makeSearchLine(ipSegmentLine, ipSegToPt, dBufDistance, -90, ipToRightLine) )
		return false;
	// 各ラインの終点取得
	IPointPtr ipFromLeftPt, ipFromRightPt, ipToLeftPt, ipToRightPt;
	if ( FAILED(ipFromLeftLine->get_ToPoint(&ipFromLeftPt)) ||
		 FAILED(ipFromRightLine->get_ToPoint(&ipFromRightPt)) ||
		 FAILED(ipToLeftLine->get_ToPoint(&ipToLeftPt)) ||
		 FAILED(ipToRightLine->get_ToPoint(&ipToRightPt)) )
		return false;

	ipOutToLeftPt = ipToLeftPt;
	ipOutToRightPt = ipToRightPt;

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipSegmentLine->get_SpatialReference(&ipSpRef)) )
		return false;

	// 元形状の始点・終点と取得した終点でポリゴン作成
	IGeometryCollectionPtr ipPolygon(CLSID_Polygon);
	if ( FAILED(IGeometryPtr(ipPolygon)->putref_SpatialReference(ipSpRef)) )
		return false;
	IPointCollectionPtr ipPtCol(CLSID_Ring);
	if ( FAILED(IGeometryPtr(ipPtCol)->putref_SpatialReference(ipSpRef)) )
		return false;
	if ( FAILED(ipPtCol->AddPoint(ipSegFromPt)) ||
		 FAILED(ipPtCol->AddPoint(ipFromLeftPt)) ||
		 FAILED(ipPtCol->AddPoint(ipToLeftPt)) ||
		 FAILED(ipPtCol->AddPoint(ipSegToPt)) ||
		 FAILED(ipPtCol->AddPoint(ipToRightPt)) ||
		 FAILED(ipPtCol->AddPoint(ipFromRightPt)) )
		 return false;
	IRingPtr ipRing(ipPtCol);
	if ( FAILED(ipRing->Close()) )
		return false;
	if ( FAILED(ipPolygon->AddGeometry(ipRing)) )
		return false;
	ipSegmentBuf = IGeometryPtr(ipPolygon);
	// セグメント間の接続部のポリゴン作成
	// ipPreSegBuf(一つ前のセグメントのバッファ)が存在しない場合は、処理を行わない。
	if ( !ipPreSegBuf ) 
		return true;
	IRelationalOperatorPtr ipRel(ipPreSegBuf);

	// 始点から左に伸ばしたラインがipPreSegBufに含まれる場合は、
	// 一つ前のセグメントの終点から右側の点(ipOutToRightPt)
	// 処理対象セグメントの始点
	// 処理対象セグメントの始点から右側の点(ipFromRightPt) で接続用のポリゴンを作成する。

	// 始点から右に伸ばしたラインがipPreSegBufと交わる場合
	// 処理対象セグメントの始点から左側の点(ipFromLeftPt)
	// 処理対象セグメントの始点
	// 一つ前のセグメントの終点から左側の点(ipOutToLeftPt)
		
	bool tgtLeft = false, tgtRight = false;
	VARIANT_BOOL vaLeftPtTouch = VARIANT_FALSE, vaLeftPtContain = VARIANT_FALSE, vaLeftLineCross = VARIANT_FALSE;
	if ( FAILED(ipRel->Touches(ipFromLeftPt, &vaLeftPtTouch)) ||
		 FAILED(ipRel->Contains(ipFromLeftPt, &vaLeftPtContain)) ||
		 FAILED(ipRel->Crosses(ipFromLeftLine, &vaLeftLineCross)) ) {
			_ASSERTE(FALSE);
			return false;
	}
	if  (vaLeftPtTouch || vaLeftPtContain || vaLeftLineCross)
		tgtRight = true;
	VARIANT_BOOL vaRightPtTouch = VARIANT_FALSE, vaRightPtContain = VARIANT_FALSE, vaRightLineCross = VARIANT_FALSE;
	if ( FAILED(ipRel->Touches(ipFromRightPt, &vaRightPtTouch)) ||
			FAILED(ipRel->Contains(ipFromRightPt, &vaRightPtContain)) ||
			FAILED(ipRel->Crosses(ipFromRightLine, &vaRightLineCross)) ) {
			_ASSERTE(FALSE);
			return false;
	}
	if (vaRightPtTouch || vaRightPtContain || vaRightLineCross)
		tgtLeft = true;
	IPointPtr ipPt1, ipPt2, ipPt3;
	// 両方ともTRUEの場合はタッチしているので、何もしない
	if ( tgtLeft && tgtRight ) 
		return true;
	// 隙間のポリゴン作成
	IGeometryCollectionPtr ipPolygon2(CLSID_Polygon);
	if ( FAILED(IGeometryPtr(ipPolygon2)->putref_SpatialReference(ipSpRef)) )
		return false;
	IPointCollectionPtr ipPtCol2(CLSID_Ring);
	if ( FAILED(IGeometryPtr(ipPtCol2)->putref_SpatialReference(ipSpRef)) )
		return false;
	if (!tgtLeft && !tgtRight) {
		// 両方ともFALSEの場合は、左右に隙間が空いているので少々上長だが
		// 処理対象セグメントの右側点・一つ前のセグメントの右側の点
		// 一つ前のセグメントの左側の点・処理対象セグメントの左側の点で矩形を作成する。
		if ( FAILED(ipPtCol2->AddPoint(ipFromRightPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipPreToRightPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipPreToLeftPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipFromLeftPt)) )
			return false;
	} else {
		if ( tgtRight ) {
			ipPt1 = ipPreToRightPt;
			ipPt2 = ipSegFromPt;
			ipPt3 = ipFromRightPt;
		} else {
			ipPt1 = ipFromLeftPt;
			ipPt2 = ipSegFromPt;
			ipPt3 = ipPreToLeftPt;
		}
		// 接続部のポリゴン作成
		if ( FAILED(ipPtCol2->AddPoint(ipPt1)) ||
			 FAILED(ipPtCol2->AddPoint(ipPt2)) ||
			 FAILED(ipPtCol2->AddPoint(ipPt3)) )
				return false;
	}
	IRingPtr ipRing2(ipPtCol2);
	if ( FAILED(ipRing2->Close()) )
		return false;
	if ( FAILED(ipPolygon2->AddGeometry(ipRing2)) )
		return false;
	ipConnectBuf = IGeometryPtr(ipPolygon2);
	return true;
}


/**
 * @brief  ポリライン形状からバット先端・べベル結合のバッファ形状を作成
 * @param  [in]  ipTargetPolyline	変換元ポリライン
 * @param  [in]  dBufDsitance		バッファ距離(度)
 * @param  [out] ipBuf				変換後形状
 * @retval true  成功
 * @retval false 失敗
 */
bool makeBatBevelBuffer( const IPolylinePtr& ipTargetPolyline, double dBufDistance, IGeometryPtr& ipBuf)
{
	ipBuf = NULL;
	// ポリラインをセグメント単位のポリラインに分解する。
	std::vector<CAdapt<IPolylinePtr>> TargetPolylines = splitPolyline( ipTargetPolyline );
	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipTargetPolyline->get_SpatialReference(&ipSpRef)) )
		return false;
	// セグメント毎にバット先端のバッファ形状と、始終点以外のポイントバッファ形状を作成
	IGeometryCollectionPtr ipGeoColl(CLSID_GeometryBag);
	if ( FAILED(IGeometryPtr(ipGeoColl)->putref_SpatialReference(ipSpRef)) )
		return false;
	long count = TargetPolylines.size();
	std::vector<CAdapt<IGeometryPtr>> bufferLines;
	IGeometryPtr ipPreSegBuf;
	IPointPtr ipPreToLeftPt, ipPreToRightPt;
	for ( long i = 0; i < count; ++i) {
		// セグメントからバット先端のバッファ作成
		IGeometryPtr ipSegBuf, ipConnectBuf;
		IPointPtr ipToLeftPt, ipToRightPt;
		if ( ! makeSegmentBatBuf(TargetPolylines[i].m_T, 
			                     ipPreSegBuf, 
								 ipPreToLeftPt, 
								 ipPreToRightPt,
								 dBufDistance,
								 ipSegBuf,
								 ipConnectBuf,
								 ipToLeftPt,
								 ipToRightPt) )
			return false;
		ipPreSegBuf = ipSegBuf;
		ipPreToLeftPt = ipToLeftPt;
		ipPreToRightPt = ipToRightPt;
		bufferLines.push_back(ipSegBuf);
		if ( FAILED(ipGeoColl->AddGeometry(ipSegBuf)) )
			return false;
		if ( ipConnectBuf ) {
			bufferLines.push_back(ipConnectBuf);
			if ( FAILED(ipGeoColl->AddGeometry(ipConnectBuf)) )
				return false;
		}
	}
	switch (count) {
		case 0:
			return false;
		case 1:
			ipBuf = bufferLines[0].m_T;
			break;
		default:
			IGeometryPtr ipPolygon(CLSID_Polygon);
			if ( FAILED(ipPolygon->putref_SpatialReference(ipSpRef)) )
				return false;

			if ( FAILED(((ITopologicalOperatorPtr)ipPolygon)->ConstructUnion(IEnumGeometryPtr(ipGeoColl))) ) {
				_ASSERTE(FALSE);
				return false;
			}
			ipBuf = ipPolygon;
			break;
	}
/* _DEBUGでPGDBでバッファ形状を出力したい場合はコメントを外す
#ifdef _DEBUG
			for ( const auto buffer : bufferLines ) {
				outputBufferLine(buffer.m_T);
			}
			outputBufferLine(ipBuf);
#endif
*/

	return true;
}

bool AheJudgeLinkRelationBuf( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, double dRoundOffMeter, double dNoCheckMeter, LinkRelationPosition& eRelationInfo )
{
	// dRoundOffMeter, dNoCheckMeterのチェック(その他はAheJudgeLinkRelationでチェックしているのでやらない)
	if( dRoundOffMeter < 0 || dNoCheckMeter < 0)
		return false;

	// 左右判定
	double dJudgeMeter = 0.0;
	if( ! AheJudgeLinkRelation(ipBasePolyline, ipTargetPolyline, dSearchMeter, eRelationInfo, dJudgeMeter))
		return false;

	// dRoundOffMeterが0の場合
	// 左右判定が判定できない場合はそのまま返す
	if ( eRelationInfo == kNotFound || dRoundOffMeter == 0 )
		return true;

	// ポリラインの距離を調べて、dNoCheckMeter未満の場合はそのまま結果を返す
	double dNoCheckDistance = meter2Coordinate(dNoCheckMeter, ipBasePolyline);
	double dBaseDistance = 0.0, dTargetDistance = 0.0;
	if ( FAILED(ipBasePolyline->get_Length(&dBaseDistance)) || FAILED(ipTargetPolyline->get_Length(&dTargetDistance) ))
		return false;
	if ( dBaseDistance < dNoCheckDistance || dTargetDistance < dNoCheckDistance )
		return true;

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipBasePolyline->get_SpatialReference(&ipSpRef)) )
		return false;

	// 基準リンクをdSearchMeterでバッファを取り、対象リンクとの共通部分を取得
	double dSearchDistance = meter2Coordinate(dSearchMeter, ipBasePolyline);
	IGeometryPtr ipBaseBuf(CLSID_Polygon);
	if ( FAILED(ipBaseBuf->putref_SpatialReference(ipSpRef)) )
		return false;
	if ( ! makeBatBevelBuffer(ipBasePolyline, dSearchDistance, ipBaseBuf) )
		return false;
	IGeometryPtr ipOverlapGeo = AheIntersect( ipBaseBuf, ipTargetPolyline, esriGeometry1Dimension );
	if ( !ipOverlapGeo )
		return false;
	IGeometryCollectionPtr ipGeomCol(ipOverlapGeo);
	long count = 0;
	if ( FAILED(ipGeomCol->get_GeometryCount(&count)) ) 
		return false;
	// 判定対象のリンクの選定
	IPolylinePtr ipJudgeLine;
	switch (count) {
	case 0:
		eRelationInfo = kNotFound;
		return true;
	case 1:
		ipJudgeLine = ipOverlapGeo;
		break;
	default:
		if ( !selectJudgeLine(ipGeomCol, ipBasePolyline, eRelationInfo, dSearchMeter, ipJudgeLine) )
			return false;
		break;
	}
	// 判定対象リンクの長さがdRoundOffMeterより短い場合は、相対位置を「見つからない」に変える。
	double dLapLength = 0.0;
	if ( FAILED(ipJudgeLine->get_Length(&dLapLength)) ) 
		return false;
	double dRoundOffDistance = meter2Coordinate(dRoundOffMeter, ipBasePolyline);
	if ( dLapLength < dRoundOffDistance )
		eRelationInfo = kNotFound;

	return true;
}

/**
 * @brief  ポリラインの構成点群を取得する
 * @param  [in]  ipPolyline  ポリライン
 * @return ポリラインの構成点群
 */
std::vector<CAdapt<IPointPtr>> getPointList( const IPolylinePtr& ipPolyline )
{
	std::vector<CAdapt<IPointPtr>> retPoints;

	IPointCollectionPtr ipPoints(ToMultipoint(ipPolyline));

	long count(0L);
	ipPoints->get_PointCount(&count);
	for( long i = 0; i < count; ++i )
	{
		IPointPtr ipPoint;
		ipPoints->get_Point( i, &ipPoint );

		retPoints.push_back( ipPoint );
	}

	return retPoints;
}


/**
 * @brief  任意のポイントを基準に、他のポリライン上の一番近いポイント、及びポイントまでの距離を取得する
 * @param  [in]  ipBasePoint       基準側のポイント
 * @param  [in]  ipTargetPolyline  一番近いポイントを検索される側のポリライン
 * @param  [out] ipNearestPoint    一番近いポイント
 * @param  [out] distance          一番近いポイントまでの距離
 * @return true   取得成功
 * @return false  取得失敗 
 */
bool getNearestPointAndDistance(  const IPointPtr& ipBasePoint, const IPolylinePtr& ipTargetPolyline, IPointPtr& ipNearestPoint, double& distance )
{
	// 最近傍点を取得
	if( FAILED((( IProximityOperatorPtr)ipTargetPolyline)->ReturnNearestPoint( ipBasePoint, esriSegmentExtension::esriNoExtension, &ipNearestPoint )))
	{
		_ASSERTE(FALSE);
		return false;
	}

	// 最近傍点への距離を取得
	if( FAILED((( IProximityOperatorPtr)ipTargetPolyline)->ReturnDistance( ipBasePoint, &distance)))
	{
		_ASSERTE(FALSE);
		return false;
	}
	return true;
}


/**
 * @brief  任意のポリラインを基準に、他のポリライン上の一番近いポイントを取得する
 * @param  [in]  ipBasePolyline    基準側のポリライン
 * @param  [in]  ipTargetPolyline  一番近いポイントを検索される側のポリライン
 * @return NULL以外  一番近いポイント
 * @return NULL      取得失敗
 */
IPointPtr getNearestPoint( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline )
{
	// ベース側、ターゲット側のポリラインの構成点群を取得
	std::vector<CAdapt<IPointPtr>> basePoints   = getPointList( ipBasePolyline );
	std::vector<CAdapt<IPointPtr>> targetPoints = getPointList( ipTargetPolyline );

	double nearest_dist(DBL_MAX);
	IPointPtr ipNearestPoint;

	for( std::vector<CAdapt<IPointPtr>>::const_iterator pt_iter = basePoints.begin();
		    pt_iter != basePoints.end();
		    ++pt_iter )
	{
		IPointPtr ipPoint = *pt_iter;

		// 最近傍点とそこまでの距離を取得
		IPointPtr ipTmpNearestPoint;
		double dist(DBL_MAX);
		if( ! getNearestPointAndDistance( ipPoint, ipTargetPolyline, ipTmpNearestPoint, dist))
			return NULL;

		// より近い最近傍点を取得
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPoint = ipTmpNearestPoint;
		}
	}

	for( std::vector<CAdapt<IPointPtr>>::const_iterator pt_iter = targetPoints.begin();
		    pt_iter != targetPoints.end();
		    ++pt_iter )
	{
		IPointPtr ipPoint = *pt_iter;
		
		// 最近傍点とそこまでの距離を取得
		IPointPtr ipTmpNearestPoint;
		double dist(DBL_MAX);
		if( ! getNearestPointAndDistance( ipPoint, ipBasePolyline, ipTmpNearestPoint, dist))
			return NULL;

		// より近いターゲット側の構成点を取得
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPoint = ipPoint;
		}
	}

	return ipNearestPoint;
}


/**
 * @brief  ポリラインを基準とし、他のポリラインを構成するセグメントの内、一番近いものを取得
 * @param  [in]  ipBasePolyline    基準側のポリライン
 * @param  [in]  ipTargetPolyline  一番近いセグメントを検索される側のポリライン
 * @return NULL以外  一番近いセグメント(Polyline型)
 * @return NULL      取得失敗
 */
IPolylinePtr getNearestSegment( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline )
{
	// ターゲット側のポリラインの構成点群を取得
	std::vector<CAdapt<IPolylinePtr>> targetPolylines = splitPolyline( ipTargetPolyline );

	// ターゲット側のポリラインから一番近いベース側のポイントを取得
	IPointPtr ipNearestBasePoint = getNearestPoint(ipTargetPolyline, ipBasePolyline);

	// ベース側の一番近いポイント(ipNearestBasePoint)を基準に、
	// ターゲット側の各セグメントへの距離を調べ一番近いセグメントを取得する
	double nearest_dist(DBL_MAX);
	IPolylinePtr ipNearestPolyline;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = targetPolylines.begin();
		    line_iter != targetPolylines.end();
		    ++line_iter )
	{
		IPolylinePtr ipPolyline = *line_iter;

		// 距離を取得
		double dist(DBL_MAX);
		if( FAILED((( IProximityOperatorPtr)ipPolyline)->ReturnDistance( ipNearestBasePoint, &dist)))
		{
			_ASSERTE(FALSE);
			return NULL;
		}
		// より近いセグメントを取得
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPolyline = ipPolyline;
		}
	}

	return ipNearestPolyline;
}


 // ターゲットのポリラインからベースのポリラインへ検索して位置関係を取得する
bool AheSimpleJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, unsigned int& nRelationInfo )
{
	if( ! ipBasePolyline || ! ipTargetPolyline )
		return false;

	std::vector<CAdapt<IPolylinePtr>> basePolylines  = splitPolyline( ipBasePolyline );

	// ベース側のポリラインを構成する各セグメントから見て、
	// 最も近いターゲット側の構成点が左右どちらにあるかを1つずつ判定する
	long lLeftCnt(0L), lRightCnt(0L);
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = basePolylines.begin();
		 line_iter != basePolylines.end();
		 ++line_iter )
	{
		// 最も近いターゲット側のポイントを取得
		IPointPtr ipNearestPoint = getNearestPoint( *line_iter, ipTargetPolyline );
		if( ! ipNearestPoint )
			return false;

		// セグメントから最も近いターゲット側の構成点に対して左右判定する
		if( getPointPosition( *line_iter, ipNearestPoint ) == kLeft )
			++lLeftCnt;
		else
			++lRightCnt;
	}
	
	// ベース側の各セグメントからの左右判定を集計して最終判定する
	if( lLeftCnt != lRightCnt ) {
		nRelationInfo = ( lLeftCnt > lRightCnt ? kLeft : kRight );
		return true;
	}


	// 左右判定の結論が出ない場合(判定結果の左右の数が同数の場合)
	// ターゲット側に最も近いベース側のセグメントから同様の左右判定を行う
	IPolylinePtr ipNearestPolyline = getNearestSegment(ipTargetPolyline, ipBasePolyline);
	if( ! ipNearestPolyline )
		return false;

	IPointPtr ipNearestPoint = getNearestPoint( ipNearestPolyline, ipTargetPolyline );
	if( ! ipNearestPoint )
		return false;

	nRelationInfo = getPointPosition( ipNearestPolyline, ipNearestPoint );

	return true;
}

}	// sindy



