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
 * @brief CMoveFeedback �N���X�����t�@�C��
 * @author �n�}DB���암�V�X�e���J���O���[�v �Ð� �M�G
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

// �L�k�֎~�ȃe�[�u���^�C�v��ݒ肷��
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

	// �Z�O�����g����
	for( segment_feedback_iterator it = segment_feedback_begin(); it != segment_feedback_end(); ++it )
		it->Feedback->Stop( &ipPolyline );
	m_listSegmentFeedback.clear();

	// �|�C���g����
	for( point_feedback_iterator it2 = point_feedback_begin(); it2 != point_feedback_end(); ++it2 )
		it2->Feedback->Stop( &ipPoint );
	m_listPointFeedback.clear();

	std::pair<double,double> pairMove;
	pairMove.first = m_dMoveX;
	pairMove.second = m_dMoveY;

	// ��]�p�x�擾
	if( pdAngle )
		*pdAngle = m_dAngle;
	// �g��k�����擾
	if( pdScale )
		*pdScale = m_dScale;

	// ������
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

		// ���΋��������߂�
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
			// �|�����C���̒��S���擾����
			IPointPtr ipCenter = GetCenterPoint( CGeometry( ipMovePolyline ) );
			ITransform2DPtr ipTransform( ipMovePolyline );
			ipTransform->Rotate( ipCenter, dAngle );
			m_dAngle += dAngle;			// ��]�����p�x��ێ�����

			// �t�B�[�h�o�b�O�̃X�^�[�g�n�_�̓x�[�X+�ړ���
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
			// �|�����C���̒��S���擾����
			IPointPtr ipCenter = GetCenterPoint( CGeometry( ipMovePolyline ) );
			ITransform2DPtr ipTransform( ipMovePolyline );
			ipTransform->Scale( ipCenter, dScale, dScale );
			m_dScale *= dScale;		// ���݂̃X�P�[����ێ�����

			// �t�B�[�h�o�b�O�̃X�^�[�g�n�_�̓x�[�X+�ړ���
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

	// �|�C���g��Z�O�����g�̃t�B�[�h�o�b�O������ꍇ�́A��]�E�g��k���͂ł��Ȃ�
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
	// �����`�F�b�N
	if( ! ipBasePoint ) return sindyErr_ArgIsNull;

	// ������
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	m_emMoveMode = emMode;
	m_dAngle = 0;
	m_dScale = 1;

	bFeedbackStarted = false;	// �t�B�[�h�o�b�N���J�n���ꂽ���ǂ����̔���p

	// ���o�[�o���h�쐬
	// ���S�ɓ�����t�B�[�`���̕����͕��s�ړ��ł悢���A�N���X����t�B�[�`���͂��̃N���X����Z�O�����g�����o�[�o���h�ƂȂ�悤�ɂ���

	// ����t�B�[�`���̌`���UNION����
	IGeometryCollectionPtr ipSlideGeometries( CLSID_Polyline );
	IPointCollectionPtr ipAddPoints( CLSID_Multipoint );
	IPointCollectionPtr	ipMovePoints;	// �|�C���g�I�u�W�F�N�g�t�B�[�h�o�b�N�`��쐬�p
	CGeometry cMoveSegments( CLSID_Polyline );

	// ��ԎQ��
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
					// �|�C���g�̏ꍇ�͂ǂ�ȃ��[�h�ł����s�ړ��������肦�Ȃ�
					case esriGeometryPoint:
						if( NULL == ipMovePoints )
							ipMovePoints.CreateInstance( CLSID_Multipoint );

						ipMovePoints->AddPoint( (IPointPtr)pFeat->GetShapeCopy() );	// �t�B�[�`���̃W�I���g�������̂܂܎g�p����Ă͍���̂ŁA�R�s�[��n��
						break;
						// �|�����C���E�|���S���̏ꍇ
					case esriGeometryPolyline:
					case esriGeometryPolygon:
						// 1. �ړ����[�h��WholeIntersect�̏ꍇ�͑S�ĕ��s�ړ�
						//    �{���Ȃ�Disjoint�Ŋm�F���ׂ������A�܂��ǂ��ł��傤�c
						// 2. Contains�̏ꍇ�͂ǂ�ȃ��[�h�ł����s�ړ��ƂȂ�
						if( sindyeMove_WholeIntersect == m_emMoveMode ||
							cGeom.Contains( *pFeat->GetShape() ) )
						{
							// �|�����C���ɕϊ����Ċi�[
							LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( *pFeat->GetShape() ) ) ), sindyErr_GeometryFunctionFailed );
						}
						// ���炩�̊֘A������ꍇ�ɂ̓��[�h�ɉ����Đ؂蕪��
						else if( ! cGeom.Disjoint( (IGeometry*)(*pFeat->GetShape()) ) )
						{
							switch( m_emMoveMode )
							{
								case sindyeMove_Point:
	//								// ������
	//								break;
								case sindyeMove_SegmentIntersect:
								case sindyeMove_SegmentWithin:
								{
									// �L�k�֎~�Ώۂ��ǂ������ׂ�
									for( std::list<sindyTableType::ECode>::const_iterator itType = m_listCantElastic.begin(); itType != m_listCantElastic.end(); ++itType )
									{
										if( it->first == *itType )
										{
											emErr = sindyErr_CantElasticTableType;	// �L�k�s�\�ȃe�[�u���^�C�v
											break;
										}
										// [bug 5791] �֘A�t����ꂽ���̂������Ώۂɂ���
										// ToDo: CContainer::QueryFeatures()�őI���`��Ƃ̊֌W�����w�肵�đI���ł���悤�ɂȂ�Ƃ悢
										for( CContainer::const_iterator itRel = it2->get()->begin(); itRel != it2->get()->end(); ++itRel )
										{
											if( itRel->first == *itType ) // �֘A�t�������̂��L�k�s������
											{
												for( CContainer::const_rows_iterator itRelRow = itRel->second.begin(); itRelRow != itRel->second.end(); ++itRelRow )
												{
													// �I��͈͂ɓ����Ă�ꍇ�͐L�k�Ȃ̂Ō�����
													IGeometryPtr ipGeom = (IGeometry *)(*(itRelRow->get()->GetShape()));
													if( cGeom.Contains( ipGeom ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( ipGeom ) ) ) , sindyErr_GeometryFunctionFailed );
													}
													// ����łȂ��I��͈͂Ɉ����������Ă���̂͐L�k�s��
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

									CGeometry cIntersect;	// ���o�[�o���h���쐬���邽�߂̍\���_�쐬�|�C���g�i�[�p
									
									// ��ԎQ�Ƃ̍ŏ��P�ʋ������擾
									double dMinMapLength = AheGetXYDomain1UnitLength( (*it2)->GetShape()->GetSpatialReference() );

									cIntersect = cGeom.Intersect( *pFeat->GetShape(), esriGeometry0Dimension );
									LOGASSERTE_IF( cIntersect != NULL, sindyErr_AlgorithmFailed )
									{
	#ifdef _DEBUG
										cIntersect.Trace( cIntersect, _T("�C���^�[�Z�N�g�`��") );
	#endif // ifdef _DEBUG
										// �\���_�ǉ����Ȃ����[�h�̏ꍇ�́A�N�G���`����ɂ����ԋ߂��_
										// ���̗p���邽�߁A��������_�Ɉ�ԋ߂��Z�O�����g��FROM���邢��
										// TO�Ō�������_��u��������
										CGeometry::point_iterator itCrossPoint = cIntersect.point_begin();
										while(  itCrossPoint != cIntersect.point_end() )
										{
											// �����|�C���g�Ɉ�ԋ߂��Z�O�����g��T��
											long lPart, lIndex;
											LOGASSERTE_IF( pFeat->GetShape()->HitTestSegment( *itCrossPoint, dMinMapLength, NULL, NULL, &lPart, &lIndex ), sindyErr_AlgorithmFailed )	// �������Ă���̂ŕK������
											{
												// �Z�O�����g�擾
												_ISegmentPtr ipSegment = pFeat->GetShape()->GetSegment( lIndex );
												IPointPtr ipFromOrTo;
												bool bIsContain = false;
												for( int i = 0; i < 2; ++i )
												{
													LOGASSERTE_IF( NULL != ipSegment &&
														( ( i == 0 && SUCCEEDED( ipSegment->get_FromPoint( &ipFromOrTo ) ) ) ||
														  ( i == 1 && SUCCEEDED( ipSegment->get_ToPoint( &ipFromOrTo ) ) ) ), sindyErr_GeometryFunctionFailed )
													{
														// FROM�ATO�̂����N�G���`����ɂ���ق��Ō�������_��ύX����
														if( cGeom.Contains( ipFromOrTo ) )
														{
															bIsContain = true;
															// �Z�O�����g�͌�Ń��o�[�o���h�ɂ���̂Ŏ���Ă����i�d���͋����Ă͂����Ȃ��j
		#if 0
															LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( ipSegment ) ), sindyErr_GeometryFunctionFailed );
		#else
															// [BUG] �Z�O�����g�̓N���[����n��
															// �������Ȃ��ƁA�����œn�����Z�O�����g�̕������Ȃ����v���W�F�N�V������ύX�ł��Ȃ��Ȃ�
															// ��̓I�ɂ́ACFeatures::CheckLogic()���̃Z�O�����g�����`�F�b�N���ɒ��s���W�n�ɕϊ�����
															// �������Z�o���Ă��邪�A�����œn�����Z�O�����g�����Ȃ�����Ɉܓx�o�x�n�̋�����Ԃ��悤��
															// �Ȃ�B
															// ���炭��ArcObjects�̃o�O���낤���A����ŉ�����Ă����B
															// ��Ŏ���𓊂��邱�ƁI�I
		#pragma message( __AFILE__LINE__ "warn : [�o�O����R�[�h] �Z�O�����g�̓R�s�[���g�p���Ȃ��ƃv���W�F�N�V�������ύX�ł��Ȃ��Ȃ�I�H�i���񍐁j")
															LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( (_ISegmentPtr)AheCreateClone(ipSegment) ) ), sindyErr_GeometryFunctionFailed );
		#endif // if 0

															// ����_�����ɑ��݂��Ȃ����ǂ������`�F�b�N
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
													// FROM��TO���Ȃ��Ƃ������Ƃ́A�N�G���`�󂪂��̃Z�O�����g��������܂����ł���Ƃ������ƂȂ̂ŁA��΂�
													// ���̎��AitCrossPoint���̗p�����ƃZ�O�����g�𕪒f�����悤�ȃ��o�[�o���h���o��̂ō폜����
													itCrossPoint.erase();
													break;
												}
											}
											++itCrossPoint;	// ����Ȃ��͂������ǈꉞ�c���Ă��� 2006/06/06 by furukawa.
										}
									}
									// ����Z�O�����g�����o�[�o���h�p�W�I���g���ɒǉ�����
									for( CGeometry::const_segment_iterator it = pFeat->GetShape()->segment_begin(); it != pFeat->GetShape()->segment_end(); ++it )
									{
										if( cGeom.Contains( (_ISegment*)it ) )
											LOGASSERTEERR_IF( SUCCEEDED( ((ISegmentCollectionPtr)ipSlideGeometries)->AddSegment( (_ISegmentPtr)AheCreateClone( (_ISegment*)it ) ) ), sindyErr_GeometryFunctionFailed );
									}
									break;
								}
								default:
	/*								// ������
									// ��������_�ŕ���
									CGeometry::Trace( *pFeat->GetShape(), _T("�ϊ�����O") );
									CGeometry cPolyline = CGeometry::ToPolyline( *pFeat->GetShape() );	// �|�����C���ɕϊ�
									CGeometry::Trace( *pFeat->GetShape(), _T("�ϊ���") );
									LOGASSERTEERR_IF( cPolyline != NULL, sindyErr_GeometryFunctionFailed )
									{
										// �|�C���g����ȉ��̃}���`�|�C���g��EnumVertex�ł�
										// SplitPoints��������i��O���ł�j�̂ŉ���iESRI���񍐁j
										long lIntersectPointCount = cIntersect.GetPointCount();
										if( 1 == lIntersectPointCount )
										{
											long lNewPart = -1, lIndex = -1;
											if( cPolyline.Split( cIntersect.GetPoint(0), &lNewPart, &lIndex ) )
											{
												// �������̂�T���Ēǉ�
												for( CGeometry::const_part_iterator it = cPolyline.part_begin(); it != cPolyline.part_end(); ++it )
												{
													if( cGeom.Contains( *it ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometry( *it ) ), sindyErr_GeometryFunctionFailed );
													}
												}
												if( ! ( sindyeMove_SegmentIntersect == m_emMoveMode || sindyeMove_SegmentWithin == m_emMoveMode ) )
												{
													// �V�����p�[�g�̑��_�ڂ����o�[�o���h�����p�̓_�ɂȂ�
													LOGASSERTEERR_IF( SUCCEEDED( ipAddPoints->AddPoint( cPolyline.GetPoint( lNewPart, 0 ) ) ), sindyErr_GeometryFunctionFailed );
												}
											}
		#if 0
											else {
												// �J�n�_�E�I���_�ł̓X�v���b�g�ł��Ȃ��i������܂��j�̂�
												// ���̏ꍇ�͍\���_�ǉ����[�h�ł͖����Ƃ��݂̂��̃Z�O�����g��
												// ���o�[�o���h�Z�O�����g�Ƃ��Ēǉ�����
												LOGASSERTEERR_IF( 0 == lIndex || lIndex == cPolyline.GetPointCount() - 1, sindyErr_GeometryFunctionFailed )
												{
													LOGASSERTEERR_IF( sindyErr_NoErr == ( emErr = cMoveSegments.AddSegment( cPolyline.GetSegment( ( lIndex == 0 ) ? lIndex : lIndex - 1 ) ) ), sindyErr_GeometryFunctionFailed );
												}
											}
		#endif // if 0 <- ����Ȃ����ۂ�
										}
										else if( 1 < lIntersectPointCount )
										{
											CGeometry::Trace( cIntersect, _T("cIntersect") );
											CEnumSplitPoint cEnumSplitPoint;
											LOGASSERTEERR_IF( sindyErr_NoErr == ( emErr = cPolyline.Split( cIntersect.GetEnumVertex(), dMinMapLength, (IEnumSplitPoint**)cEnumSplitPoint.Release() ) ), sindyErr_GeometryFunctionFailed )
											{
												// �������̂�T���Ēǉ�
												for( CGeometry::const_part_iterator it = cPolyline.part_begin(); it != cPolyline.part_end(); ++it )
												{
													if( cGeom.Contains( *it ) )
													{
														LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometry( *it ) ), sindyErr_GeometryFunctionFailed );
													}
												}
												if( ! ( sindyeMove_SegmentIntersect == m_emMoveMode || sindyeMove_SegmentWithin == m_emMoveMode ) )
												{
													// �����_�̓��o�[�o���h�����Ɏg�p����̂Ŏ���Ă���
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
		// �ړ��x�[�X�|�C���g����
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

		// �Z�O�����g�̕����C���쐬�p���o�[�o���h���쐬����
		LOGASSERTE_IF( sindyErr_NoErr == ( emErr = cMoveSegments.segment_unique() ), sindyErr_GeometryFunctionFailed )
		{
			for( CGeometry::const_segment_iterator it = cMoveSegments.segment_begin(); it != cMoveSegments.segment_end(); ++it )
			{
				// �N�G���`��O���J�n�_�Ƃ��A�N�G���`����܂ňړ����Ă���
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
						// ����\���_���܂܂Ȃ��悤�ɑI�������ꍇ�ɂ͂��̃Z�O�����g�𕽍s�ړ��p���o�[�o���h�ɒǉ�����K�v������
						LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->AddGeometryCollection( (IGeometryCollectionPtr)CGeometry::ToPolyline( (_ISegment*)it ) ) ), sindyErr_GeometryFunctionFailed )
							continue;	// ���o�[�o���h�쐬�͔�΂�
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
			// ���s�ړ��p���o�[�o���h�쐬
			long lGeomCount = -1;
			LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->get_GeometryCount( &lGeomCount ) ), sindyErr_GeometryFunctionFailed )
			{
				if( 0 < lGeomCount )
				{
					ITopologicalOperatorPtr ipTopo;
					LOGASSERTEERR_IF( SUCCEEDED( ipSlideGeometries->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
					{
						CGeometry::Trace( (IGeometryPtr)ipTopo, _T("���s�ړ��ΏۃZ�O�����g�iSimplify�O�j") );
						LOGASSERTEERR_IF( sindyErr_NoErr == emErr && SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed )
						{
							IMoveLineFeedbackPtr ipFeedback;
							LOGASSERTEERR_IF( SUCCEEDED( ipFeedback.CreateInstance( CLSID_MoveLineFeedback ) ), sindyErr_COMCreateInstanceFailed )
							{
								ipFeedback->putref_Display( m_ipDisp );
				//					ipFeedback->putref_Symbol( AheCreateLineSymbol( AheCreateColor(RGB(255,0,0)), 2, esriSLSSolid, true ) );
								CGeometry::Trace( (IGeometryPtr)ipTopo, _T("���s�ړ��ΏۃZ�O�����g") );
								LOGASSERTEERR_IF( SUCCEEDED( ipFeedback->Start( (IPolylinePtr)ipTopo, ipBasePoint ) ), sindyErr_COMFunctionFailed )
								{
									m_ipMoveLineFeedback = ipFeedback;
									bFeedbackStarted = true;
								}
							}
						}
					}
					// �����_�̕����C���쐬�p���o�[�o���h���쐬����
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
			// �|�C���g�p�̃��o�[�o���h�쐬
			if( NULL != ipMovePoints )
			{
				// �d���_���폜���邽�߂�Simplify����
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

	if( sindyErr_NoErr != emErr ) Stop();	// �G���[�̏ꍇ�͑S�ăN���A����

	return emErr;
}

} // sindy
