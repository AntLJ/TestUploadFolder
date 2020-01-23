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

/**
 * @file MoveFeedback.h
 * @brief CMoveFeedback クラス実装ファイル
 * @author 地図DB制作部システム開発グループ 古川 貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "MoveFeedback.h"

#include "Feature.h"
#include "GlobalFunctions.h"
#include "util.h"
#include "EnumSplitPoint.h"
#include "EnumSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

CMoveFeedback::CMoveFeedback() : m_dAngle( 0 ), m_dScale( 1 )
{
	m_dBaseX = m_dBaseY = -1;
	m_dMoveX = m_dMoveY = 0;
	m_emMoveMode = sindyeMove_SegmentWithin;
}

CMoveFeedback::~CMoveFeedback()
{
	Stop();
}

// 伸縮禁止なテーブルタイプを設定する
void CMoveFeedback::SetCantElasticType( sindyTableType::ECode emCode ){ m_listCantElastic.push_back( emCode ); }

std::pair<double,double> CMoveFeedback::Stop( double* pdScale /*=NULL*/, double* pdAngle /*=NULL*/ )
{
	IPolylinePtr ipPolyline;
	IPointPtr ipPoint;

	if( NULL != m_ipMoveLineFeedback )
	{
		m_ipMoveLineFeedback->Stop( &ipPolyline );
		m_ipMoveLineFeedback = NULL;
	}

	// セグメント部分
	for( segment_feedback_iterator it = segment_feedback_begin(); it != segment_feedback_end(); ++it )
		it->Feedback->Stop( &ipPolyline );
	m_listSegmentFeedback.clear();

	// ポイント部分
	for( point_feedback_iterator it2 = point_feedback_begin(); it2 != point_feedback_end(); ++it2 )
		it2->Feedback->Stop( &ipPoint );
	m_listPointFeedback.clear();

	std::pair<double,double> pairMove;
	pairMove.first = m_dMoveX;
	pairMove.second = m_dMoveY;

	// 回転角度取得
	if( pdAngle )
		*pdAngle = m_dAngle;
	// 拡大縮小率取得
	if( pdScale )
		*pdScale = m_dScale;

	// 初期化
	m_dBaseX = m_dBaseY = -1;
	m_dMoveX = m_dMoveY = 0;
	m_dAngle = 0;
	m_dScale = 1;

	m_listCantElastic.clear();

	return pairMove;
}

void CMoveFeedback::Refresh( OLE_HANDLE hDC )
{
	if( NULL != m_ipMoveLineFeedback )
	{
		LOGASSERTE_IF( SUCCEEDED( m_ipMoveLineFeedback->Refresh( hDC ) ), sindyErr_COMFunctionFailed );
	}
	for( segment_feedback_iterator it = segment_feedback_begin(); it != segment_feedback_end(); ++it )
		LOGASSERTE_IF( SUCCEEDED( it->Feedback->Refresh( hDC ) ), sindyErr_COMFunctionFailed );
	for( point_feedback_iterator it = point_feedback_begin(); it != point_feedback_end(); ++it )
		LOGASSERTE_IF( SUCCEEDED( it->Feedback->Refresh( hDC ) ), sindyErr_COMFunctionFailed );
}

void CMoveFeedback::segment_feedback_push_back( IPoint* StartPoint )
{
	NEWLINEFEEDBACK stFeedback;
	stFeedback.Feedback.CreateInstance( CLSID_NewLineFeedback );
	stFeedback.Feedback->putref_Display( m_ipDisp );
	stFeedback.Feedback->Start( StartPoint );
	StartPoint->QueryCoords( &stFeedback.X, &stFeedback.Y );
	m_listSegmentFeedback.push_back( stFeedback );
}

void CMoveFeedback::point_feedback_push_back( IPoint* StartPoint )
{
	MOVEPOINTFEEDBACK stFeedback;
	stFeedback.Feedback.CreateInstance( CLSID_MovePointFeedback );
	stFeedback.Feedback->putref_Display( m_ipDisp );
	stFeedback.Feedback->Start( StartPoint, StartPoint );
	StartPoint->QueryCoords( &stFeedback.X, &stFeedback.Y );
	m_listPointFeedback.push_back( stFeedback );
}

void CMoveFeedback::MoveTo( IPoint* Point )
{
	LOGASSERTE_IF( Point, sindyErr_ArgIsNull )
	{
		if( NULL != m_ipMoveLineFeedback )
		{
			LOGASSERTE_IF( SUCCEEDED( m_ipMoveLineFeedback->MoveTo( Point ) ), sindyErr_COMFunctionFailed );
		}

		// 相対距離を求める
		double dMoveX, dMoveY;
		Point->QueryCoords( &dMoveX, &dMoveY );
		m_dMoveX = dMoveX - m_dBaseX;
		m_dMoveY = dMoveY - m_dBaseY;

		IPointPtr ipNewPoint( CLSID_Point );

		for( segment_feedback_iterator it = segment_feedback_begin(); it != segment_feedback_end(); ++it )
		{
			ipNewPoint->PutCoords( it->X + m_dMoveX, it->Y + m_dMoveY );
			it->Feedback->MoveTo( ipNewPoint );
		}
		for( point_feedback_iterator it = point_feedback_begin(); it != point_feedback_end(); ++it )
		{
			ipNewPoint->PutCoords( it->X + m_dMoveX, it->Y + m_dMoveY );
			it->Feedback->MoveTo( ipNewPoint );
		}
	}
}
void CMoveFeedback::Rotate( double dAngle )
{
	if( !CanTransform() )
		return;

	if( m_ipMoveLineFeedback )
	{
		IPolylinePtr ipMovePolyline;
		LOGASSERTE_IF( SUCCEEDED( m_ipMoveLineFeedback->Stop( &ipMovePolyline ) ), sindyErr_COMFunctionFailed );
		if( ipMovePolyline )
		{
			// ポリラインの中心を取得する
			IPointPtr ipCenter = GetCenterPoint( CGeometry( ipMovePolyline ) );
			ITransform2DPtr ipTransform( ipMovePolyline );
			ipTransform->Rotate( ipCenter, dAngle );
			m_dAngle += dAngle;			// 回転した角度を保持する

			// フィードバッグのスタート地点はベース+移動量
			ISpatialReferencePtr ipSpRef;
			ipCenter->get_SpatialReference( &ipSpRef );
			IPointPtr ipStart( CLSID_Point );
			ipStart->putref_SpatialReference( ipSpRef );
			double dx = 0;
			double dy = 0;
			GetMovePoint( &dx, &dy );
			ipStart->PutCoords( dx, dy );
			m_ipMoveLineFeedback->Start( ipMovePolyline, ipStart );
		}
	}
}

void CMoveFeedback::Scale( double dScale )
{
	if( !CanTransform() )
		return;

	if( m_ipMoveLineFeedback )
	{
		IPolylinePtr ipMovePolyline;
		LOGASSERTE_IF( SUCCEEDED( m_ipMoveLineFeedback->Stop( &ipMovePolyline ) ), sindyErr_COMFunctionFailed );
		if( ipMovePolyline )
		{
			// ポリラインの中心を取得する
			IPointPtr ipCenter = GetCenterPoint( CGeometry( ipMovePolyline ) );
			ITransform2DPtr ipTransform( ipMovePolyline );
			ipTransform->Scale( ipCenter, dScale, dScale );
			m_dScale *= dScale;		// 現在のスケールを保持する

			// フィードバッグのスタート地点はベース+移動量
			ISpatialReferencePtr ipSpRef;
			ipCenter->get_SpatialReference( &ipSpRef );
			IPointPtr ipStart( CLSID_Point );
			ipStart->putref_SpatialReference( ipSpRef );
			double dx = 0;
			double dy = 0;
			GetMovePoint( &dx, &dy );
			ipStart->PutCoords( dx, dy );
			m_ipMoveLineFeedback->Start( ipMovePolyline, ipStart );
		}
	}
}

IPointPtr CMoveFeedback::GetCenterPoint( const CGeometry& cGeom )
{
	IEnvelopePtr ipEnv = cGeom.GetEnvelope();
	if( !ipEnv )
		return NULL;
	IAreaPtr ipArea( ipEnv );
	if( !ipArea )
		return NULL;

	IPointPtr ipCentroid;
	ipArea->get_Centroid( &ipCentroid );

	return ipCentroid;
}

bool CMoveFeedback::CanTransform()
{
	if( !m_ipMoveLineFeedback )
		return false;

	// ポイントやセグメントのフィードバッグがある場合は、回転・拡大縮小はできない
	if( !( m_listPointFeedback.empty() && m_listSegmentFeedback.empty() ) )
		return false;

	return true;
}
void CMoveFeedback::GetMovePoint(double* pX, double* pY)
{
	if( pX && pY )
	{
		*pX = m_dBaseX + m_dMoveX;
		*pY = m_dBaseY + m_dMoveY;
	}
}

sindyErrCode CMoveFeedback::Start( const CGeometry& cGeom, IPoint* ipBasePoint, const CContainer& listFeatures, sindyeMoveMode emMode, bool& bFeedbackStarted )
{
	// 引数チェック
	if( ! ipBasePoint ) return sindyErr_ArgIsNull;

	// 初期化
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	m_emMoveMode = emMode;
	m_dAngle = 0;
	m_dScale = 1;

	bFeedbackStarted = false;	// フィードバックが開始されたかどうかの判定用

	// ラバーバンド作成
	// 完全に内包されるフィーチャの部分は平行移動でよいが、クロスするフィーチャはそのクロスするセグメントがラバーバンドとなるようにする

	// 内包フィーチャの形状をUNIONする
	IGeometryCollectionPtr ipSlideGeometries( CLSID_Polyline );
	IPointCollectionPtr ipAddPoints( CLSID_Multipoint );
	IPointCollectionPtr	ipMovePoints;	// ポイントオブジェクトフィードバック形状作成用
	CGeometry cMoveSegments( CLSID_Polyline );

	// 空間参照
	ISpatialReferencePtr ipSpRef = cGeom.GetSpatialReference();
	((IGeometryPtr)ipSlideGeometries)->putref_SpatialReference( ipSpRef );
	((IGeometryPtr)ipAddPoints)->putref_SpatialReference( ipSpRef );
	cMoveSegments.PutRefSpatialReference( ipSpRef );

	for( CContainer::const_iterator it = listFeatures.begin(); it != listFeatures.end(); ++it )
	{
		for( CContainer::const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			const CFeature* pFeat = dynamic_cast<const CFeature*>(it2->get());
			LOGASSERTEERR_IF( pFeat, sindyErr_AlgorithmFailed )
			{
				switch( pFeat->GetShape()->GetGeometryType() )
				{
					// ポイントの場合はどんなモードでも平行移動しかありえない
					case esriGeometryPoint:
						if( NULL == ipMovePoints )
							ipMovePoints.CreateInstance( CLSID_Multipoint );

						ipMovePoints->AddPoint( (IPointPtr)pFeat->GetShapeCopy() );	// フィーチャのジオメトリがそのまま使用されては困るので、コピーを渡す
						break;
						// ポリライン・ポリゴンの場合
					case esriGeometryPolyline:
					case esriGeometryPolygon:
						// 1. 移動モードがWholeIntersectの場合は全て平行移動
						//    本当ならDisjointで確認すべきだが、まあ良いでしょう…
						// 2. Containsの場合はどんなモードでも平行移動となる
						if( sindyeMove_WholeIntersect == m_emMoveMode ||
							cGeom.Contains( *pFeat->GetShape() ) )
						{
							// ポリラインに変換して格納
							LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( *pFeat->GetShape() ) ) ), sindyErr_GeometryFunctionFailed );
						}
						// 何らかの関連がある場合にはモードに応じて切り分け
						else if( ! cGeom.Disjoint( (IGeometry*)(*pFeat->GetShape()) ) )
						{
							switch( m_emMoveMode )
							{
								case sindyeMove_Point:
	//								// 未実装
	//								break;
								case sindyeMove_SegmentIntersect:
								case sindyeMove_SegmentWithin:
								{
									// 伸縮禁止対象かどうか調べる
									for( std::list<sindyTableType::ECode>::const_iterator itType = m_listCantElastic.begin(); itType != m_listCantElastic.end(); ++itType )
									{
										if( it->first == *itType )
										{
											emErr = sindyErr_CantElasticTableType;	// 伸縮不可能なテーブルタイプ
											break;
										}
										// [bug 5791] 関連付けられたものも検索対象にする
										// ToDo: CContainer::QueryFeatures()で選択形状との関係性を指定して選択できるようになるとよい
										for( CContainer::const_iterator itRel = it2->get()->begin(); itRel != it2->get()->end(); ++itRel )
										{
											if( itRel->first == *itType ) // 関連付いたものが伸縮不可だった
											{
												for( CContainer::const_rows_iterator itRelRow = itRel->second.begin(); itRelRow != itRel->second.end(); ++itRelRow )
												{
													// 選択範囲に内包されてる場合は伸縮可なので見逃す
													IGeometryPtr ipGeom = (IGeometry *)(*(itRelRow->get()->GetShape()));
													if( cGeom.Contains( ipGeom ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( ipGeom ) ) ) , sindyErr_GeometryFunctionFailed );
													}
													// 内包でなく選択範囲に引っかかってるものは伸縮不可
													else if( !cGeom.Disjoint( ipGeom ) )
													{
														emErr = sindyErr_CantElasticTableType;
														break;
													}
												}
											}
										}
									}
									if( sindyErr_CantElasticTableType == emErr )
										break;

									CGeometry cIntersect;	// ラバーバンドを作成するための構成点作成ポイント格納用
									
									// 空間参照の最小単位距離を取得
									double dMinMapLength = AheGetXYDomain1UnitLength( (*it2)->GetShape()->GetSpatialReference() );

									cIntersect = cGeom.Intersect( *pFeat->GetShape(), esriGeometry0Dimension );
									LOGASSERTE_IF( cIntersect != NULL, sindyErr_AlgorithmFailed )
									{
	#ifdef _DEBUG
										cIntersect.Trace( cIntersect, _T("インターセクト形状") );
	#endif // ifdef _DEBUG
										// 構成点追加しないモードの場合は、クエリ形状内にある一番近い点
										// を採用するため、交差する点に一番近いセグメントのFROMあるいは
										// TOで交差する点を置き換える
										CGeometry::point_iterator itCrossPoint = cIntersect.point_begin();
										while(  itCrossPoint != cIntersect.point_end() )
										{
											// 交差ポイントに一番近いセグメントを探す
											long lPart, lIndex;
											LOGASSERTE_IF( pFeat->GetShape()->HitTestSegment( *itCrossPoint, dMinMapLength, NULL, NULL, &lPart, &lIndex ), sindyErr_AlgorithmFailed )	// 交差しているので必ずある
											{
												// セグメント取得
												_ISegmentPtr ipSegment = pFeat->GetShape()->GetSegment( lIndex );
												IPointPtr ipFromOrTo;
												bool bIsContain = false;
												for( int i = 0; i < 2; ++i )
												{
													LOGASSERTE_IF( NULL != ipSegment &&
														( ( i == 0 && SUCCEEDED( ipSegment->get_FromPoint( &ipFromOrTo ) ) ) ||
														  ( i == 1 && SUCCEEDED( ipSegment->get_ToPoint( &ipFromOrTo ) ) ) ), sindyErr_GeometryFunctionFailed )
													{
														// FROM、TOのうちクエリ形状内にあるほうで交差する点を変更する
														if( cGeom.Contains( ipFromOrTo ) )
														{
															bIsContain = true;
															// セグメントは後でラバーバンドにするので取っておく（重複は許してはいけない）
		#if 0
															LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( ipSegment ) ), sindyErr_GeometryFunctionFailed );
		#else
															// [BUG] セグメントはクローンを渡す
															// そうしないと、ここで渡したセグメントの部分がなぜかプロジェクションを変更できなくなる
															// 具体的には、CFeatures::CheckLogic()時のセグメント距離チェック時に直行座標系に変換して
															// 距離を算出しているが、ここで渡したセグメントだけなぜか常に緯度経度系の距離を返すように
															// なる。
															// 恐らくはArcObjectsのバグだろうが、これで回避しておく。
															// 後で質問を投げること！！
		#pragma message( __AFILE__LINE__ "warn : [バグ回避コード] セグメントはコピーを使用しないとプロジェクションが変更できなくなる！？（未報告）")
															LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( (_ISegmentPtr)AheCreateClone(ipSegment) ) ), sindyErr_GeometryFunctionFailed );
		#endif // if 0

															// 同一点が既に存在しないかどうかをチェック
															CGeometry cFromOrTo( ipFromOrTo );
															bool bFindSame = false;
															for( CGeometry::const_point_iterator itCrossPointTmp = cIntersect.point_begin(); itCrossPointTmp != cIntersect.point_end(); ++itCrossPointTmp )
															{
																if( cFromOrTo.Equals( *itCrossPointTmp ) )
																{
																	bFindSame = true;
																	itCrossPoint.erase();
																	break;
																}
															}
															if( ! bFindSame && cIntersect.point_end() != itCrossPoint ) {
																LOGASSERTE_IF( sindyErr_NoErr == itCrossPoint.update( ipFromOrTo ), sindyErr_GeometryFunctionFailed );
																break;
															}
														}
													}
													if(  bIsContain )
														break;
												}
												if( bIsContain )
													continue;
												else {
													// FROMもTOもないということは、クエリ形状がそのセグメントを遇数回またいでいるということなので、飛ばす
													// この時、itCrossPointが採用されるとセグメントを分断したようなラバーバンドが出るので削除する
													itCrossPoint.erase();
													break;
												}
											}
											++itCrossPoint;	// いらないはずだけど一応残しておく 2006/06/06 by furukawa.
										}
									}
									// 内包セグメントをラバーバンド用ジオメトリに追加する
									for( CGeometry::const_segment_iterator it = pFeat->GetShape()->segment_begin(); it != pFeat->GetShape()->segment_end(); ++it )
									{
										if( cGeom.Contains( (_ISegment*)it ) )
											LOGASSERTEERR_IF( SUCCEEDED( ((ISegmentCollectionPtr)ipSlideGeometries)->AddSegment( (_ISegmentPtr)AheCreateClone( (_ISegment*)it ) ) ), sindyErr_GeometryFunctionFailed );
									}
									break;
								}
								default:
	/*								// 未実装
									// 交差する点で分割
									CGeometry::Trace( *pFeat->GetShape(), _T("変換する前") );
									CGeometry cPolyline = CGeometry::ToPolyline( *pFeat->GetShape() );	// ポリラインに変換
									CGeometry::Trace( *pFeat->GetShape(), _T("変換後") );
									LOGASSERTEERR_IF( cPolyline != NULL, sindyErr_GeometryFunctionFailed )
									{
										// ポイントが一つ以下のマルチポイントのEnumVertexでは
										// SplitPointsが落ちる（例外がでる）ので回避（ESRI未報告）
										long lIntersectPointCount = cIntersect.GetPointCount();
										if( 1 == lIntersectPointCount )
										{
											long lNewPart = -1, lIndex = -1;
											if( cPolyline.Split( cIntersect.GetPoint(0), &lNewPart, &lIndex ) )
											{
												// 内包するものを探して追加
												for( CGeometry::const_part_iterator it = cPolyline.part_begin(); it != cPolyline.part_end(); ++it )
												{
													if( cGeom.Contains( *it ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometry( *it ) ), sindyErr_GeometryFunctionFailed );
													}
												}
												if( ! ( sindyeMove_SegmentIntersect == m_emMoveMode || sindyeMove_SegmentWithin == m_emMoveMode ) )
												{
													// 新しいパートの第一点目がラバーバンド発生用の点になる
													LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->AddPoint( cPolyline.GetPoint( lNewPart, 0 ) ) ), sindyErr_GeometryFunctionFailed );
												}
											}
		#if 0
											else {
												// 開始点・終了点ではスプリットできない（あたりまえ）ので
												// その場合は構成点追加モードでは無いときのみそのセグメントを
												// ラバーバンドセグメントとして追加する
												LOGASSERTEERR_IF( 0 == lIndex || lIndex == cPolyline.GetPointCount() - 1, sindyErr_GeometryFunctionFailed )
												{
													LOGASSERTEERR_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( cPolyline.GetSegment( ( lIndex == 0 ) ? lIndex : lIndex - 1 ) ) ), sindyErr_GeometryFunctionFailed );
												}
											}
		#endif // if 0 <- いらないっぽい
										}
										else if( 1 < lIntersectPointCount )
										{
											CGeometry::Trace( cIntersect, _T("cIntersect") );
											CEnumSplitPoint cEnumSplitPoint;
											LOGASSERTEERR_IF( sindyErr_NoErr == ( emErr = cPolyline.Split( cIntersect.GetEnumVertex(), dMinMapLength, (IEnumSplitPoint**)cEnumSplitPoint.Release() ) ), sindyErr_GeometryFunctionFailed )
											{
												// 内包するものを探して追加
												for( CGeometry::const_part_iterator it = cPolyline.part_begin(); it != cPolyline.part_end(); ++it )
												{
													if( cGeom.Contains( *it ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometry( *it ) ), sindyErr_GeometryFunctionFailed );
													}
												}
												if( ! ( sindyeMove_SegmentIntersect == m_emMoveMode || sindyeMove_SegmentWithin == m_emMoveMode ) )
												{
													// 分割点はラバーバンド発生に使用するので取っておく
													for( CEnumSplitPoint::const_iterator it = cEnumSplitPoint.begin(); it != cEnumSplitPoint.end(); ++it )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->AddPoint( *it ) ), sindyErr_GeometryFunctionFailed );
													}
												}
											}
										}
									}
	*/
									break;
							}
						}
						break;
					default:
						break;
				}
			}
		}
	}
	if( sindyErr_NoErr == emErr )
	{
		// 移動ベースポイント生成
/*		IPointPtr ipBasePoint;
		IAreaPtr ipArea( (IGeometry*)cGeom );
		if( NULL != ipArea )
		{
			ipArea->get_Centroid( &ipBasePoint );
			ipBasePoint->QueryCoords( &m_dBaseX, &m_dBaseY );
		}
		else
			ipBasePoint = AheCreateClone( (IGeometry*)cGeom );*/
		ipBasePoint->QueryCoords( &m_dBaseX, &m_dBaseY );

		// セグメントの分ライン作成用ラバーバンドを作成する
		LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.segment_unique() ), sindyErr_GeometryFunctionFailed )
		{
			for( CGeometry::const_segment_iterator it = cMoveSegments.segment_begin(); it != cMoveSegments.segment_end(); ++it )
			{
				// クエリ形状外を開始点とし、クエリ形状内まで移動しておく
				IPointPtr ipStart, ipMove;
				if( cGeom.Contains( it.from() ) && ( ! cGeom.Contains( it.to() ) ) )
				{
					ipStart = it.to();
					ipMove  = it.from();
				}
				else if( cGeom.Contains( it.to() ) && ( ! cGeom.Contains( it.from() ) ) )
				{
					ipStart = it.from();
					ipMove  = it.to();
				}
				else {
					if( sindyeMove_SegmentIntersect == m_emMoveMode ||
						sindyeMove_SegmentWithin == m_emMoveMode )
					{
						// 一つも構成点を含まないように選択した場合にはそのセグメントを平行移動用ラバーバンドに追加する必要がある
						LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( (_ISegment*)it ) ) ), sindyErr_GeometryFunctionFailed )
							continue;	// ラバーバンド作成は飛ばす
					}
					else {
						LOGASSERTEERR_IF( ( cGeom.Contains( it.from() ) && ( ! cGeom.Contains( it.to() ) ) ) || ( cGeom.Contains( it.to() ) && ( ! cGeom.Contains( it.from() ) ) ), sindyErr_AlgorithmFailed );
					}
				}
				LOGASSERTEERR_IF( NULL != ipStart && NULL != ipMove, sindyErr_AlgorithmFailed )
				{
					NEWLINEFEEDBACK stFeedback;
					stFeedback.Feedback.CreateInstance( CLSID_NewLineFeedback );
					stFeedback.Feedback->putref_Display( m_ipDisp );
					stFeedback.Feedback->Start( ipStart );
					ipMove->QueryCoords( &stFeedback.X, &stFeedback.Y );
					m_listSegmentFeedback.push_back( stFeedback );

					bFeedbackStarted = true;
				}
			}
		}
		if( sindyErr_NoErr == emErr )
		{
			// 平行移動用ラバーバンド作成
			long lGeomCount = -1;
			LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->get_GeometryCount( &lGeomCount ) ), sindyErr_GeometryFunctionFailed )
			{
				if( 0 < lGeomCount )
				{
					ITopologicalOperatorPtr ipTopo;
					LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
					{
						CGeometry::Trace( (IGeometryPtr)ipTopo, _T("平行移動対象セグメント（Simplify前）") );
						LOGASSERTEERR_IF( sindyErr_NoErr == emErr && SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed )
						{
							IMoveLineFeedbackPtr ipFeedback;
							LOGASSERTEERR_IF( SUCCEEDED( ipFeedback.CreateInstance( CLSID_MoveLineFeedback ) ), sindyErr_COMCreateInstanceFailed )
							{
								ipFeedback->putref_Display( m_ipDisp );
				//					ipFeedback->putref_Symbol( AheCreateLineSymbol( AheCreateColor(RGB(255,0,0)), 2, esriSLSSolid, true ) );
								CGeometry::Trace( (IGeometryPtr)ipTopo, _T("平行移動対象セグメント") );
								LOGASSERTEERR_IF( SUCCEEDED( ipFeedback->Start( (IPolylinePtr)ipTopo, ipBasePoint ) ), sindyErr_COMFunctionFailed )
								{
									m_ipMoveLineFeedback = ipFeedback;
									bFeedbackStarted = true;
								}
							}
						}
					}
					// 分割点の分ライン作成用ラバーバンドを作成する
					LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
					{
						LOGASSERTEERR_IF( sindyErr_NoErr == emErr && SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed )
						{
							long lPointCount = 0;
							LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->get_PointCount( &lPointCount ) ), sindyErr_GeometryFunctionFailed )
							{
								for( long i = 0; i < lPointCount; ++i )
								{
									IPointPtr ipPoint;
									LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->get_Point( i, &ipPoint ) ), sindyErr_GeometryFunctionFailed )
									{
										segment_feedback_push_back( ipPoint );
										bFeedbackStarted = true;
									}
								}
							}
						}
					}
				}
			}
			// ポイント用のラバーバンド作成
			if( NULL != ipMovePoints )
			{
				// 重複点を削除するためにSimplifyする
				ITopologicalOperatorPtr ipTopo;
				LOGASSERTEERR_IF( SUCCEEDED( ipMovePoints->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed )
					{
						_ISetPtr ipFeedback;
						LOGASSERTEERR_IF( SUCCEEDED( ipFeedback.CreateInstance( CLSID_GroupFeedback ) ), sindyErr_COMCreateInstanceFailed )
						{
							long lCount = 0;
							ipMovePoints->get_PointCount( &lCount );
							for( long i = 0; i < lCount; ++i )
							{
								IPointPtr ipPoint;
								LOGASSERTEERR_IF( SUCCEEDED( ipMovePoints->get_Point( i, &ipPoint ) ), sindyErr_GeometryFunctionFailed )
								{
									point_feedback_push_back( ipPoint );
									bFeedbackStarted = true;
								}
							}
						}
					}
				}
			}
		}
	}

	if( sindyErr_NoErr != emErr ) Stop();	// エラーの場合は全てクリアする

	return emErr;
}

} // sindy
