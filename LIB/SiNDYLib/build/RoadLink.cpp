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

// RoadLink.cpp: CRoadLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoadLink.h"
#include "meshutil.h"
#include "ErrorObject.h"
#include "RuleModel.h"
#include "AheGeometryOp.h"
#include "GlobalFunctions.h"
#include "EnumSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR START_VECTOR_AND_NEXT             = _T("開始ベクトル（%.1lfcm）とその次のベクトル");
	LPCTSTR END_VECTOR_AND_PRE                = _T("終了ベクトル（%.1lfcm）とその前のベクトル");
	LPCTSTR DISTANCE_LESS_THAN_50_MSG         = _T("%sの角度が%.1lf°ですが、正規化座標距離で50未満の場合は170°以上なければいけません。");
	LPCTSTR DISTANCE_BETWEEN_50_AND_100_MSG   = _T("%sの角度が%.1lf°ですが、正規化座標距離で50以上100未満の場合は150°以上なければいけません。");
	LPCTSTR DISTANCE_BETWEEN_100_AND_250_MSG  = _T("%sの角度が%.1lf°ですが、正規化座標距離で100以上250未満の場合は120°以上なければいけません。");
	LPCTSTR DISTANCE_BETWEEN_250_AND_500_MSG  = _T("%sの角度が%.1lf°ですが、正規化座標距離で250以上500未満の場合は90°以上なければいけません。");
	LPCTSTR DISTANCE_BETWEEN_500_AND_1000_MSG = _T("%sの角度が%.1lf°ですが、正規化座標距離で500以上1000未満の場合は40°以上なければいけません。");
	LPCTSTR DISTANCE_OVER_1000_MSG            = _T("%sの角度が%.1lf°ですが、正規化座標距離で1000以上の場合は20°以上なければいけません。");
#else
	LPCTSTR START_VECTOR_AND_NEXT             = _T("Start vector (%.1lfcm) and next vector");
	LPCTSTR END_VECTOR_AND_PRE                = _T("End vector (%.1lfcm) and previous vector");
	LPCTSTR DISTANCE_LESS_THAN_50_MSG         = _T("The angel of %s is %.1lf degree, but if it is less than 50 in the normalized coordinates distance, it must be over 170 degree");
	LPCTSTR DISTANCE_BETWEEN_50_AND_100_MSG   = _T("The angel of %s is %.1lf degree, but if it is between 50 and 100 in the normalized coordinates distance, it must be over 150 degree");
	LPCTSTR DISTANCE_BETWEEN_100_AND_250_MSG  = _T("The angel of %s is %.1lf degree, but if it is between 100 and 250 in the normalized coordinates distance, it must be over 120 degree");
	LPCTSTR DISTANCE_BETWEEN_250_AND_500_MSG  = _T("The angel of %s is %.1lf degree, but if it is between 250 and 500 in the normalized coordinates distance, it must be over 90 degree");
	LPCTSTR DISTANCE_BETWEEN_500_AND_1000_MSG = _T("The angel of %s is %.1lf degree, but if it is between 500 and 1000 in the normalized coordinates distance, it must be over 150 degree");
	LPCTSTR DISTANCE_OVER_1000_MSG            = _T("The angel of %s is %.1lf degree, but if it is over 1000 in the normalized coordinates distance, it must be over 20 degree");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
namespace road_link {
using namespace errorcode;
using namespace sindy::schema;
/*
 * @brief 両端に道路ノードが存在しているか
 * @retval 成功　sindyErr_NoErr
 * @retval 失敗　それ以外のエラー
 */
sindyErrCode CRoadLink::CheckNodeID() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
/*	int nFromID = GetFrom_NodeID();

	if ( -1 == nFromID ) {
		/// そもそも番号が入ってない
		return errorcode::sindyErr_NoFromNodeId;
	}

	/// From Node を検索		
	if ( FALSE  ) {
		/// ノードを検索したら見つからない
		return errorcode::sindyErr_NoFromNodeId;
	}

	if ( FALSE ) {
		/// ノードと道路が接していない
		return errorcode::sindyErr_NoFromNodeId;
	}

	int nToID = GetTo_NodeID();

	if ( -1 == nToID ) {
		/// そもそも番号が入ってない
		return errorcode::sindyErr_NoToNodeId;
	}

	/// To Node を検索		
	if ( FALSE  ) {
		/// ノードを検索したら見つからない
		return errorcode::sindyErr_NoToNodeId;
	}

	if ( FALSE ) {
		/// ノードと道路が接していない
		return errorcode::sindyErr_NoToNodeId;
	}
	
	return errorcode::sindyErr_NoErr;*/
	emErr = sindyErr_NoImplimentation;
	return emErr;
}

// FROM側の端点を取得する
CPoint CRoadLink::GetFromPoint() const
{
	return GetShape()->GetPoint(0);
}

/**
 * @brief <b>TO側の端点を取得する</b>\n
 * return CPoint
 */
CPoint CRoadLink::GetToPoint() const
{
	return GetShape()->GetPoint(GetShape()->GetPointCount()-1);
}

// x, yがメッシュ区画線上の場合にもう一つのメッシュコードを返す
unsigned int ANOTHERMESH( unsigned int meshcode, int ix, int iy )
{
	if( 0 == ix )
		meshcode = DECMESH_X(meshcode);
	else if( 100000 == ix )
		meshcode = INCMESH_X(meshcode);
	if( 0 == iy )
		meshcode = DECMESH_Y(meshcode);
	else if( 1000000 == iy )
		meshcode = INCMESH_Y(meshcode);

	return meshcode;
}

// 整合性をチェックする
errorcode::sindyErrCode CRoadLink::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	// 基本チェック
	CFeature::CheckLogic( cRule, cErrs );

	// 形状チェック
	CErrorInfos cErrInfos;
	const CGeometry* pGeom = GetShape();

	// [ADAMバグ 41]2次メッシュ境界付近の構成点を消そうとするとエラーメッセージが出て編集できない
	// ADAMリンクは二次メッシュチェックしない
	if((0!=_tcsicmp(GetTableName(), adam_v2::lane_link::kTableName)) &&
	   (0!=_tcsicmp(GetTableName(), adam_v2::updown_link::kTableName)) &&
	   (0!=_tcsicmp(GetTableName(), adam_v2::compart_link::kTableName)) && 
	   (0!=_tcsicmp(GetTableName(), adam_v2::border_link::kTableName)))
	{
		long lCount = 0;
		const long lPointCount = pGeom->GetPointCount();
		unsigned int nMeshCode = 0;
		unsigned int nMeshCodeEdge1 = 0, nMeshCodeEdge2 = 0; // 0番目が二次メッシュ区画線上の場合は一時的にこっちに格納する
		for( CGeometry::const_point_iterator it = pGeom->point_begin(); it != pGeom->point_end(); ++it, ++lCount )
		{
			// 構成点が存在するメッシュコードを取得
			int ix, iy;
			unsigned int nMeshCodeTmp = LL2MESH( it.get_x(), it.get_y(), 2, ix, iy );

			// 0番目の場合
			if( lCount == 0 )
			{
				// 0番目が二次メッシュ区画線上の場合は一時的に別のものに入れ、1番目の時に
				// 比較してからnMeshCodeに格納する
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy) )
				{
					// [CHECK] 0番目がメッシュ4隅に存在しないかどうか
					if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == iy) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadEndPointExist4MeshEdge, sindyErrLevel_ERR ) ); // 0番目がメッシュ4隅に存在する

					nMeshCodeEdge1 = nMeshCodeTmp;
					nMeshCodeEdge2 = ANOTHERMESH( nMeshCodeEdge1, ix, iy );
				}
				else
					nMeshCode = nMeshCodeTmp;
			}
			// 最終点の場合
			else if( lCount == lPointCount - 1 )
			{
				// 最終点が二次メッシュ区画線上の場合
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy) )
				{
					// [CHECK] 最終点がメッシュ4隅に存在しないかどうか
					if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == ix) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadEndPointExist4MeshEdge, sindyErrLevel_ERR ) ); // 最終点がメッシュ4隅に存在する

					// [CHECK] セグメントが二次メッシュ区画線を跨いでいないか（最終点が二次メッシュ区画線上にある場合）
					if( nMeshCode != nMeshCodeTmp && nMeshCode != ANOTHERMESH( nMeshCodeTmp, ix, iy ) )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 最終点がメッシュに接しており、それが別のメッシュの中にある場合
				}
				// [CHECK] セグメントが二次メッシュ区画線を跨いでいないか（最終点が二次メッシュ区画線上にない場合）
				else if( nMeshCode != nMeshCodeTmp )
				{
					if( 0 == nMeshCode )
					{
						if( nMeshCodeEdge1 == nMeshCodeTmp || nMeshCodeEdge2 == nMeshCodeTmp )
							nMeshCode = nMeshCodeTmp;
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 0番目がメッシュに接しており、2番目がさらに別のメッシュの中にある場合
					}
					else
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 最終点がメッシュに接しておらず、別のメッシュの中にある場合
				}
			}
			// 中間点の場合
			else {
				// [CHECK] 端点以外が二次メッシュ区画線と接していないか
				if( (0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy)  )
					cErrInfos.push_back( CErrorInfo( sindyErr_RoadMiddlePointExistMeshEdge, sindyErrLevel_ERR ) );	// 中間点がメッシュに接している場合
				else {
					// [CHECK] セグメントが二次メッシュ区画線を跨いでいないか
					if( 0 == nMeshCode )
					{
						if( nMeshCodeEdge1 == nMeshCodeTmp || nMeshCodeEdge2 == nMeshCodeTmp )
							nMeshCode = nMeshCodeTmp;
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 0番目がメッシュに接しており、2番目がさらに別のメッシュの中にある場合
					}
					else if( nMeshCode != nMeshCodeTmp )
						cErrInfos.push_back( CErrorInfo( sindyErr_RoadSegmentCrossMeshEdge, sindyErrLevel_ERR ) ); // 中間点が別のメッシュにある場合
				}
			}
		}
	}
	// セグメントチェック（bug 8463）
	// 歩道リンクではチェックしない(SJ歩行者Phase2対応)
	// ADAMリンク(車線リンク、上下線リンク)は二次メッシュチェックしない
	// TODO RoadLinkのチェック上で歩道リンクのチェックをするという処理が、おそらく、設計思想に合ってない
	// 道路リンクとの共通部分を抽出して分離するべきではないか
	if(0!=_tcsicmp(GetTableName(), walk_link::kTableName) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::lane_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::updown_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::compart_link::kTableName)) &&
	  (0!=_tcsicmp(GetTableName(), adam_v2::border_link::kTableName)))
	{
		long lSegmentCount = pGeom->GetSegmentCount(), cnt = 0;
		ISegmentCollectionPtr ipSegCol( (IGeometry*)*pGeom );
		for( CGeometry::const_segment_iterator it = pGeom->segment_begin(); it != pGeom->segment_end(); ++it, ++cnt )
		{
			// [CHECK] [ROADCHECK 02197] 接続しているノード（ノードID:'node_id'）との接続形状が不正
			// [bug 1870] ノードに接続する部分の角度が以下の条件の場合
			//            ノードに接続するベクトルの長さをＬ、ノードに接続するベクトルと
			//				次のベクトル間の角度をＲとしたとき、
			//				 1. Ｌが正規化座標距離 50未満で、Ｒが170度未満。
			//				 2. Ｌが正規化座標距離 50以上　100未満で、Ｒが150度未満。
			//				 3. Ｌが正規化座標距離 100以上 250未満で、Ｒが120度未満。
			//				 4. Ｌが正規化座標距離 250以上 500未満で、Ｒが 90度未満。
			//				 5. Ｌが正規化座標距離 500以上 1000未満で、Ｒが 40度未満。
			//				 6. Ｌが正規化座標距離 1000以上で、Ｒが 20度未満。
			if( lSegmentCount > 1 && ( cnt == 0 || cnt == lSegmentCount - 1 ) )
			{
				IPointPtr ipTmpFromPoint(it.from()), ipTmpToPoint(it.to());
				double dLenMeter = AheGetMeterLength( ipTmpFromPoint, ipTmpToPoint );
				CString strTmp;
				IPointPtr ip1, ip2, ip3;
				if( cnt == 0 )
				{
					_ISegmentPtr ipSegNext;
					ipSegCol->get_Segment( 1, &ipSegNext );
					ip1 = ipTmpFromPoint;
					ip2 = ipTmpToPoint;
					ipSegNext->get_ToPoint( &ip3 );
					strTmp.Format(START_VECTOR_AND_NEXT, (DOUBLE)dLenMeter * 100.0 );
				}
				else {
					_ISegmentPtr ipSegPrev;
					ipSegCol->get_Segment( lSegmentCount - 2, &ipSegPrev );
					ipSegPrev->get_FromPoint( &ip1 );
					ip2 = ipTmpFromPoint;
					ip3 = ipTmpToPoint;
					strTmp.Format(END_VECTOR_AND_PRE, (DOUBLE)dLenMeter * 100.0 );
				}
				DOUBLE dAngle = AheGetAngle( ip1, ip2, ip3 );
				double dMeshXYLen = AheGetMeshXYLength( ipTmpFromPoint, ipTmpToPoint ); // MESH XY距離で比較（bug 8463）
				CString strMsg;
				if( dMeshXYLen < 50.0                                && ! ( -170.0 >= dAngle || 170.0 <= dAngle ) ) strMsg.Format(DISTANCE_LESS_THAN_50_MSG, strTmp, fabs(dAngle) );
				else if(   50.0 <= dMeshXYLen && dMeshXYLen <  100.0 && ! ( -150.0 >= dAngle || 150.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_50_AND_100_MSG, strTmp, fabs(dAngle) );
				else if(  100.0 <= dMeshXYLen && dMeshXYLen <  250.0 && ! ( -120.0 >= dAngle || 120.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_100_AND_250_MSG, strTmp, fabs(dAngle) );
				else if(  250.0 <= dMeshXYLen && dMeshXYLen <  500.0 && ! (  -90.0 >= dAngle ||  90.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_250_AND_500_MSG, strTmp, fabs(dAngle) );
				else if(  500.0 <= dMeshXYLen && dMeshXYLen < 1000.0 && ! (  -40.0 >= dAngle ||  40.0 <= dAngle ) ) strMsg.Format(DISTANCE_BETWEEN_500_AND_1000_MSG, strTmp, fabs(dAngle) );
				else if( 1000.0 <= dMeshXYLen                        && ! (  -20.0 >= dAngle ||  20.0 <= dAngle ) ) strMsg.Format(DISTANCE_OVER_1000_MSG, strTmp, fabs(dAngle) );
				if( ! strMsg.IsEmpty() )
					cErrInfos.push_back( CErrorInfo( sindyErr_CheckReturnWarning, sindyErrLevel_WARN, (IGeometryPtr)AheCreateClone(ip2), -1, -1, _T("%s"), strMsg ) );
			}
		}
	}
	// 内包関係フィーチャの単体チェック
	// ネットワークリンクと内包関係フィーチャの複合チェック
	for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()), 
		itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
		itContain != itContainEnd; ++itContain )
	{
		for( const_rows_iterator itContainFeat = begin(AheTableName2TableType(itContain->first)), 
			itContainFeatEnd = end(AheTableName2TableType(itContain->first));
			itContainFeat != itContainFeatEnd; ++itContainFeat )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			(*itContainFeat)->Changed( &emUpdateType );

			// 削除されていないもののみチェック
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// LINK_IDが正しいかどうかのチェック
				if ( GetOID() != (*itContainFeat)->GetLongValueByFieldName(itContain->second, -1 ) )
					cErrs.push_back( CErrorObject( *itContainFeat->get(), CErrorInfo( sindyErr_RelationIDInvalid, sindyErrLevel_ERR ) ) );

				GetShape()->Trace( _T("道路") );
				(*itContainFeat)->Trace( _T("リンク内属性") );

				// 道路にちゃんと内包されているかチェック
				if ( ! GetShape()->Equals( (*itContainFeat)->GetShapeCopy() ) && ! GetShape()->Contains( (*itContainFeat)->GetShapeCopy() ) )
				{
					this->Trace( _T("道路"));
					(*itContainFeat)->Trace(_T("リンク内属性"));
					cErrs.push_back( CErrorObject( *itContainFeat->get(), CErrorInfo( sindyErr_RuleRelationshipContainGeometryNotWithinRoad, sindyErrLevel_ERR ) ) );
				}
			}
		}
	}

	// 外部テーブル
	for( CRuleRelationTable::const_value_iterator itOuter = g_cRuleRelationTable.begin(GetTableName()), 
		itOuterEnd = g_cRuleRelationTable.end(GetTableName());
		itOuter != itOuterEnd; ++itOuter )
	{
		for( const_rows_iterator itOuterObj = begin(AheTableName2TableType(itOuter->get<1>())),
			itOuterObjEnd = end(AheTableName2TableType(itOuter->get<1>()));
			itOuterObj != itOuterObjEnd; ++itOuterObj )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			(*itOuterObj)->Changed( &emUpdateType );

			// 削除されていないもののみチェック
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// LINK_IDが正しいかどうかのチェック
				if ( GetOID() != (*itOuterObj)->GetLongValueByFieldName(itOuter->get<2>(), -1 ) )
					cErrs.push_back( CErrorObject( *itOuterObj->get(), CErrorInfo( sindyErr_RelationIDInvalid, sindyErrLevel_ERR ) ) );
			}
		}
	}

	// 属性チェック

	if( sindyErr_NoErr != cErrInfos.GetDefaultErrCode() )
	{
		CErrorObject cErr( *this );
		cErr.push_back( cErrInfos );
		cErrs.push_back( cErr );
	}
	return cErrs.GetDefaultErrCode();
}

bool CRoadLink::IsPECLink() const
{
	schema::road_link::road_class::ECode sRoadClass = GetRoadClassCode();

	if ( sRoadClass != -1 && sRoadClass == schema::road_link::road_class::kParking/*駐車場構内道路リンク（PEC）*/ || sRoadClass == schema::road_link::road_class::kMatching/* マッチング用リンク（PEC）*/ )
		return true;

	return false;
}

} // road_link

} // sindy
