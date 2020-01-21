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

#include "StdAfx.h"
#include "ToolPointerBase.h"

ISymbolPtr AheCreateArrowLineSymbol( COLORREF rgb )
{
	IColorPtr ipColor( AheGetColor( rgb ) );

	ILinePropertiesPtr ipLineProperties( CLSID_CartographicLineSymbol );
	ILineDecorationPtr ipLineDecoration( CLSID_LineDecoration );

	IArrowMarkerSymbolPtr ipArrowMarkerSymbol( CLSID_ArrowMarkerSymbol );
	ipArrowMarkerSymbol->put_Style( esriAMSPlain );
	ipArrowMarkerSymbol->put_Color( ipColor );
	ISymbolPtr ipArrowSymbol( ipArrowMarkerSymbol );

	ISimpleLineDecorationElementPtr ipLineDecElement( CLSID_SimpleLineDecorationElement );

	ipLineDecElement->put_Rotate( VARIANT_TRUE );
	ipLineDecElement->put_MarkerSymbol( ipArrowMarkerSymbol );
	ipLineDecElement->AddPosition( 1 );

	ipLineDecoration->AddElement( ipLineDecElement );
			
	ipLineProperties->putref_LineDecoration( ipLineDecoration );
	ICartographicLineSymbolPtr ipCartographicLineSymbol( ipLineProperties );
			
	ipCartographicLineSymbol->put_Color( ipColor );
	ipCartographicLineSymbol->put_Width( 2 );
	ipCartographicLineSymbol->put_Cap( esriLCSSquare );
	ipCartographicLineSymbol->put_Join( esriLJSRound );

	return ipCartographicLineSymbol;
}

bool AheEquals( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	if( ipGeom1 && ipGeom2 )
	{
		VARIANT_BOOL vbEqual = VARIANT_FALSE;
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
		((IRelationalOperatorPtr)ipGeom1)->Equals( ipGeom2, &vbEqual );
#else
		((IClonePtr)ipGeom1)->IsEqual( (IClonePtr)ipGeom2, &vbEqual );
#endif
		return VB2bool(vbEqual);
	}
	else return ( ipGeom1 || ipGeom2 ) ? false : true;
}

// �����̃W�I���g���̒[�_���m���ڂ��Ă��邩�ǂ����̔���
bool AheIsTouch_EndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	bool bRet = false;
	if( ipGeom1 && ipGeom2 )
	{
		ICurvePtr ipLine2( ipGeom2 );
		if( ipLine2 )
		{
			IPointPtr ipPoint2_1, ipPoint2_2;
			ipLine2->get_FromPoint( &ipPoint2_1 );
			ipLine2->get_ToPoint( &ipPoint2_2 );

			// ipGeom2�̒[�_�̂����ǂ��炩��ipGeom1�̒[�_�ɐڂ��Ă���ꍇ�̂�true
			if( AheIsTouch_EndPointAndPoint( ipGeom1, ipPoint2_1 ) != AheIsTouch_EndPointAndPoint( ipGeom1, ipPoint2_2 ) ) 
				bRet = true;
		}
	}
	return bRet;
}

// �|�C���g���������̒[�_�Ɛڂ��Ă��邩�ǂ����̔���
bool AheIsTouch_EndPointAndPoint( IGeometry* ipGeom, IPoint* ipPoint )
{
	bool bRet = false;
	if( ipGeom && ipPoint )
	{
		ICurvePtr ipCurve( ipGeom );
		if( ipCurve )
		{
			IPointPtr ipFrom, ipTo;
			ipCurve->get_FromPoint( &ipFrom );
			ipCurve->get_ToPoint( &ipTo );

			if( AheEquals( ipPoint, ipFrom ) || AheEquals( ipPoint, ipTo ) )
				bRet = true;
		}
	}
	return bRet;
}

// �Е��̃W�I���g���̒��ԓ_�Ƃ����Е��̃W�I���g���̒[�_���ڂ��Ă��邩�ǂ����̔���
bool AheIsTouch_MidwayPointAndEndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	bool bRet = false;
	if( ipGeom1 && ipGeom2 )
	{
		IPointCollectionPtr ipCol( ipGeom1 );
		IEnumVertexPtr ipVertexes;
		if( ipCol )
			ipCol->get_EnumVertices( &ipVertexes );
		if( ipVertexes )
		{
			ipVertexes->Reset();
			// ��_�ڂ��擾
			// ���̈�_�ڂ��R�s�[���Q�Ƃ�������Ȃ��̂ŃR�s�[���擾���Ă���
			IPointPtr ipVertex1st;
			long lPart = -1, lVert = -1;
			ipVertexes->Next( &ipVertex1st, &lPart, &lVert );
			IPointPtr ipVertex( AheCreateClone( ipVertex1st ) );

			bool bFindTouch = false; // ipGeom2�̒[�_�ɐڂ��钆�ԓ_�����������ǂ���
			while( S_OK == ipVertexes->QueryNext( ipVertex, &lPart, &lVert ) )
			{
				// �����p�[�g�̏ꍇ���l�����āA�\���_�ԍ���0�̏ꍇ�͔�΂�
				if( 0 == lVert )
					continue;
				// �p�[�g�̍ŏI�_�̏ꍇ����΂�
				VARIANT_BOOL vbIsPartEnd = VARIANT_FALSE;
				ipVertexes->IsLastInPart( &vbIsPartEnd );
				if( vbIsPartEnd )
					continue;

				if( AheIsTouch_EndPointAndPoint( ipGeom2, ipVertex ) )
				{
					bFindTouch = true;
					break;
				}
			}
			if( bFindTouch )
				bRet = true;
		}
	}
	return bRet;
}

// ipGeom1��ipGeom2�����[�v�����Őڑ����Ă��邩�ǂ����̔���
bool AheIsTouch_EndPointNoRoop( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	return ( AheIsTouch_EndPoint( ipGeom1, ipGeom2 ) && 
		( ! AheIsTouch_MidwayPointAndEndPoint( ipGeom1, ipGeom2 ) ) );
}

double AheCalcDistance( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	double dRet = -1.0; // �Ԃ�l

	IProximityOperatorPtr ipProx( ipGeom1 );
	if( ipProx && ipGeom2 )
		ipProx->ReturnDistance( ipGeom2, &dRet );

	return dRet;
}

IGeometryPtr ConstructUnion( const std::list<CAdapt<IGeometryPtr> >& listGeometry )
{
	if( listGeometry.empty() )
		return NULL;

	// [bug 5908] ITopologicalOperator::ConstructUnion()���Ǝ��Ȍ������C���Ń}���`�p�[�g�ƂȂ邽�߁A1���q��
	//            listGeometry���������鏇�ɂȂ��Ă���O��
	IGeometryPtr ipUnionGeom;
	for( std::list<CAdapt<IGeometryPtr> >::const_iterator it = listGeometry.begin(); it != listGeometry.end(); ++it )
	{
		if( it != listGeometry.begin() )
			ipUnionGeom = AheUnionNetwork( ipUnionGeom, it->m_T, AheGetSpatialReference( ipUnionGeom ) );
		else
			ipUnionGeom = (IGeometryPtr)AheCreateClone( it->m_T );

		// �Ō�̗v�f�̒[�_��擪�Ƃ���
		ICurvePtr ipCurveLast( it->m_T ), ipCurve( ipUnionGeom );

		if ( ipCurveLast && ipCurve ) {
			if ( ! AheEquals( ipCurveLast, ipCurve ) ) {
				IPointPtr ipFromLast, ipToLast, ipFrom, ipTo;

				ipCurveLast->get_FromPoint( &ipFromLast );
				ipCurveLast->get_ToPoint( &ipToLast );
				if ( ipCurve ) {
					ipCurve->get_FromPoint( &ipFrom );

					if ( AheEquals( ipFromLast, ipFrom ) || AheEquals( ipToLast, ipFrom ) )
						ipCurve->ReverseOrientation();
				}
			}
		}
	}

	ICurvePtr ipCurve( ipUnionGeom );

	if ( ipCurve ) {
		IPointPtr ipFrom, ipTo;

		ipCurve->get_FromPoint( &ipFrom );
		ipCurve->get_ToPoint( &ipTo );

		if ( AheEquals( ipFrom, ipTo ) )
			ipCurve->ReverseOrientation();
	}

	return ipUnionGeom;
}

bool IsInvalidateGeometry(IGeometry* ipGeom)
{
	if( ! ipGeom )
		return false;

	// �`�󂪋�H
	VARIANT_BOOL vbIsEmpty = VARIANT_TRUE;
	ipGeom->get_IsEmpty( &vbIsEmpty );
	if ( vbIsEmpty )
		return false;

	esriGeometryType geomType = esriGeometryPoint;
	ipGeom->get_GeometryType( &geomType );

	switch( geomType )
	{
		case esriGeometryPoint:
			break;
		case esriGeometryPolyline:
		{
			double dLen = 0.0;
			if( FAILED(((IPolylinePtr)ipGeom)->get_Length( &dLen )) || 0.0 >= dLen )
				return false;
			break;
		}
		case esriGeometryPolygon:
		case esriGeometryEnvelope:
		{
			double dArea = 0.0;
			// r_sugawara_20110407144356.dmp �ł͂����ŗ����Ă����̂ŁAtry ... catch �ł������Ă����ibug 8651�j
			// TODO: get_GeometryType�͐���ɓ����Ă��邱�Ƃ���AIArea�ւ�QueryInterface�͖��Ȃ��Ǝv���B
			// ���炩�̏����̂Ƃ��ɁAget_Area�����s����̂��Ƃ������A���͂��̏������Ȃ񂾂��킩��Ȃ��̂�
			// ��O�⑫�ɂƂǂ߂Ă���
			try {
				if( FAILED(((IAreaPtr)ipGeom)->get_Area( &dArea )) )
					return false;
			}
			catch( ... ) {
				_ASSERTE( false );
				return false;
			}
			if( 0.0 >= dArea )
			{
				// �����v���ł����]���Đ������ibug 8436�j
				IPolygonPtr ipPoly(ipGeom);
				if( ! ipPoly ) // Envelope�̏ꍇ���l�����Ȃ��Ɨ�����ibug 8651�j
					return false;
				ipPoly->ReverseOrientation();
				if( FAILED(((IAreaPtr)ipPoly)->get_Area( &dArea )) || 0.0 >= dArea )
					return false;
			}
			break;
		}
		default:
		{
			// ������0�ȉ��H
			ICurvePtr ipC = (IGeometryPtr)ipGeom;

			if ( ipC ) {
				double dbl = 0;

				ipC->get_Length( &dbl );
				if ( dbl <= 0.0 )
					return false;
			}

			// �����ƕ���0�ȉ��H
			IEnvelopePtr ipEnv;
			ipGeom->get_Envelope( &ipEnv );

			if ( ipEnv ) {
				double dbw=0, dbh=0;

				ipEnv->get_Height( &dbh );
				ipEnv->get_Width( &dbw );

				if ( dbw <= 0.0 || dbh <= 0.0 )
					return false;
			}
			break;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// CToolPointer
CToolPointer::CToolPointer()
{
	InitTolerance();
}
CToolPointer::CToolPointer( IApplication* ipApp, bool bWithArea/*=false*/ ) : m_cApp(ipApp), m_bWithArea(bWithArea)
{
	InitTolerance();
}
CToolPointer::~CToolPointer()
{
	init();
}

HRESULT CToolPointer::OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& /*bHandled*/)
{
	return S_OK;
}

HRESULT CToolPointer::OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& /*bHandled*/)
{
	// [Bug 5851]
	if ( m_bWithArea && Button != 4 ) { // ���{�^���h���b�O���͌����G���A�\���͂��Ȃ��i�X�N���[���̏ꍇ�ɂ�����j�ibug 6273�j
		// �����G���A�`��
		if( ! m_cFeedback.DrawSearchArea( ipPoint ) )
			m_cFeedback.StartDrawSearchArea( m_cApp.GetFocusMapScreenDisplay(), ipPoint, m_nTolerance );
	}

	return S_OK;
}

HRESULT CToolPointer::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& /*bHandled*/)
{
	return S_OK;
}

HRESULT CToolPointer::OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled)
{
	// �g��k���E�X�N���[��
	BOOL blHandled = FALSE;
	AheZooMoveKeyDownProc( m_cApp.GetFocusMap(), keyCode, Shift, &blHandled );
	bHandled = BOOL2bool(blHandled);

	return S_OK;
}
HRESULT CToolPointer::AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& /*bHandled*/)
{
	if( m_bWithArea && Display && esriViewForeground == phase )
	{
		OLE_HANDLE hDC = 0;
		if( SUCCEEDED ( Display->get_hDC( &hDC ) ) )
			m_cFeedback.ResetSearchArea( hDC );
	}
	return S_OK;
}
HRESULT CToolPointer::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled)
{
	if( ! handled )
		return E_POINTER;
	*handled = VARIANT_FALSE;
	return S_OK;
}
void CToolPointer::clear()
{
	m_cFeedback.StopDrawSearchArea();
}
void CToolPointer::init()
{
	clear();
	m_cApp = NULL;
}
IGeometryPtr CToolPointer::GetSearchArea( IPoint* ipPoint, ISpatialReference* ipSpRef/* = NULL*/ )
{
	if( ! m_cFeedback.GetScreenDisplay() ) // �����G���A��\�������Ɏg�p���悤�Ƃ����ꍇ�ɗ����邱�Ƃ�h��
		m_cFeedback.SetScreenDisplay( m_cApp.GetFocusMapScreenDisplay(), &m_nTolerance ); // �����Ȃ肱�����Ă΂ꂽ�ꍇ��CFeedback::m_nTolerance������������Ă��Ȃ��I�I
	return m_cFeedback.GetSearchArea( ipPoint, ipSpRef );
}

void CToolPointer::InitTolerance()
{
	// �I��臒l��ǂݍ���
	CSiNDYeSettings cSet;
	cSet.Load();
	m_nTolerance = cSet.GetTolerance();
}

/////////////////////////////////////////////////////////////////////////////////////////
// CToolLineTrace
HRESULT CToolLineTrace::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = false; // �W�I���g�����ǉ��ł����ꍇ�ɂ̂�true�ɂȂ�܂�

	if( ! m_ipClass )
		return S_OK;

	switch( Button )
	{
		case 2:    // �E�N���b�N
		{
			undo();
			break;
		}
		case 1:    // ���N���b�N
		{
			if ( m_listFinishGeometry.empty() ) {
				// �W�I���g����ǉ�����
				IGeometryPtr ipSearchGeom( m_cFeedback.GetSearchArea( ipPoint, AheGetSpatialReference( m_ipClass ) ) ); // XY�������p�|���S���ɕϊ�

				if ( ipSearchGeom ) {
					IGeometryPtr ipHilightGeometry( ConstructUnion( m_listHilightGeometry ) ); // �[�_��r�p
					ISpatialReferencePtr ipSpRef;

					((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
					ipSearchGeom->putref_SpatialReference( ipSpRef );

					// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
					// ���������ƌ`��Ō�������悤�ɏC��
					IFeatureCursorPtr ipCursor;
					if( SUCCEEDED( m_ipClass->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipSearchGeom ), NULL, NULL, m_strWhere), VARIANT_FALSE, &ipCursor ) ) )
					{
						// �������Ɋi�[
						double dbMin = 9999999.9f;
						std::list<CAdapt<IFeaturePtr>> listFeatures;
						std::map<double,std::list<CAdapt<IFeaturePtr>>> mapFeatures;

						IFeaturePtr ipFeature;
						while( S_OK == ipCursor->NextFeature( &ipFeature ) )
						{
							if( ! ipFeature )
								break;

							IGeometryPtr ipGeom;
							ipFeature->get_Shape( &ipGeom );

							double dDist = AheCalcDistance( ipPoint, ipGeom );
							if ( dbMin > dDist )
								dbMin = dDist;

							mapFeatures[dDist].push_back( ipFeature );
						}

						if ( m_bUseMin )
							listFeatures = mapFeatures[dbMin];
						else
						{
							for ( std::map<double,std::list<CAdapt<IFeaturePtr>>>::const_iterator itFeatures = mapFeatures.begin(); itFeatures != mapFeatures.end(); ++itFeatures )
							{
								for ( std::list<CAdapt<IFeaturePtr>>::const_iterator itFeature = itFeatures->second.begin(); itFeature != itFeatures->second.end(); ++itFeature )
									listFeatures.push_back( (*itFeature) );
							}
						}

						std::map<double,std::pair<CAdapt<IGeometryPtr>,CAdapt<IFeaturePtr>> > mapGeometry; // �����ł����W�I���g����map
						// �n�C���C�g�W�I���g�������ɑ��݂���ꍇ�͂���ɂȂ�����̂�I��
						// �n�C���C�g�W�I���g�����Ȃ��ꍇ��XY�Ɉ�ԋ߂����̂�I��
						for ( std::list<CAdapt<IFeaturePtr>>::const_iterator itFeature = listFeatures.begin(); itFeature != listFeatures.end(); ++itFeature )
						{
							IFeaturePtr ipFeature = itFeature->m_T;
							bool bAdd = true;

							if( ! ipFeature )
								break;

							IGeometryPtr ipGeom;
							ipFeature->get_Shape( &ipGeom );

							if ( ! m_listHilightGeometry.empty() ) {
								if ( ! m_bClosedPermission ) {	// �z�������Ȃ��H
									if ( ! AheIsTouch_EndPointNoRoop( ipHilightGeometry, ipGeom ) )
										bAdd = false;
								} else {
									if ( ! m_bTouchMidwayPoint ) {
										if ( AheIsTouch_MidwayPointAndEndPoint( ipHilightGeometry, ipGeom ) )
											bAdd = false;
									}

									IPointPtr ipPoint2_1, ipPoint2_2;

									((ICurvePtr)ipGeom)->get_FromPoint( &ipPoint2_1 );
									((ICurvePtr)ipGeom)->get_ToPoint( &ipPoint2_2 );

									if ( ! AheIsTouch_EndPointAndPoint( ipHilightGeometry, ipPoint2_1 ) && ! AheIsTouch_EndPointAndPoint( ipHilightGeometry, ipPoint2_2 ) )
										bAdd = false;
								}
							}

							if ( bAdd )
							{
								// �����̃W�I���g���ƈ�v���Ă�����̂͏Ȃ�
								for( std::list<CAdapt<IGeometryPtr> >::const_iterator it = m_listHilightGeometry.begin(); it != m_listHilightGeometry.end(); ++it )
								{
									VARIANT_BOOL vbEqual = VARIANT_FALSE;
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
									((IRelationalOperatorPtr)ipGeom)->Equals( it->m_T, &vbEqual );
#else
									((IClonePtr)ipGeom)->IsEqual( (IClonePtr)it->m_T, &vbEqual );
#endif
									if( vbEqual )
									{
										bAdd = false;
										break;
									}
								}

								// [Bug 6623]�����t�B�[�`����I���ł��Ă��܂�
								if ( bAdd && ! m_listHilightFeatures.empty() )
								{
									// ���ɑI���ς݂̃t�B�[�`���͏��O����B
									for( std::list<CAdapt<IFeaturePtr> >::const_iterator it = m_listHilightFeatures[m_listHilightFeatures.size()-1].begin(); it != m_listHilightFeatures[m_listHilightFeatures.size()-1].end(); ++it )
									{
										if ( ipFeature == it->m_T )
										{
											bAdd = false;
											break;
										}
									}
								}

								if( bAdd )
									mapGeometry[AheCalcDistance( ipPoint, ipGeom )] = std::pair<CAdapt<IGeometryPtr>,CAdapt<IFeaturePtr>>(ipGeom,ipFeature);
							}
						}
						ipCursor = NULL;

						// ��ԋ߂����̂��m��
						if( ! mapGeometry.empty() )
						{
							IGeometryPtr ipGeometry = (IGeometryPtr)AheCreateClone(mapGeometry.begin()->second.first.m_T);
							IPointPtr ipFromPoint, ipToPoint;

							((ICurvePtr)ipGeometry)->get_FromPoint( &ipFromPoint );
							((ICurvePtr)ipGeometry)->get_ToPoint( &ipToPoint );

							if ( m_bClickDir && m_listHilightGeometry.empty() ) {
								IProximityOperatorPtr ipProximityOperator = ipPoint;

								if ( ipProximityOperator ) {
									double dDistanceFrom=0.0, dDistanceTo=0.0;

									ipProximityOperator->ReturnDistance( ipFromPoint, &dDistanceFrom );
									ipProximityOperator->ReturnDistance( ipToPoint, &dDistanceTo );

									if ( (dDistanceFrom < dDistanceTo) )
										((ICurvePtr)ipGeometry)->ReverseOrientation();
								}
							}

							m_listHilightGeometry.push_back( ipGeometry );
							m_ipHilightGeometry = ConstructUnion( m_listHilightGeometry );

							// �������ς�����ꍇ�̓��X�g�̒��g���ς��Ȃ��Ƃ����Ȃ��̂Ń`�F�b�N
							ICurvePtr ipCurveLast( ipGeometry ), ipCurve( ipHilightGeometry );
							bool bReverse = false;

							if ( ipCurveLast && ipCurve ) {
								if ( ! AheEquals( ipCurveLast, ipCurve ) ) {
									IPointPtr ipFromLast, ipToLast, ipFrom, ipTo;

									ipCurveLast->get_FromPoint( &ipFromLast );
									ipCurveLast->get_ToPoint( &ipToLast );
									if ( ipCurve ) {
										ipCurve->get_FromPoint( &ipFrom );

										if ( AheEquals( ipFromLast, ipFrom ) || AheEquals( ipToLast, ipFrom ) )
											bReverse = true;
									}
								}
							}

							// ���[�v���Ă���ꍇ�͓���ւ���K�v�Ȃ��̂�
							if ( bReverse ) {
								ICurvePtr ipCurve( m_ipHilightGeometry );

								if ( ipCurve ) {
									IPointPtr ipFrom, ipTo;

									ipCurve->get_FromPoint( &ipFrom );
									ipCurve->get_ToPoint( &ipTo );

									if ( AheEquals( ipFrom, ipTo ) )
										bReverse = false;
								}
							}

							long Index = m_listHilightFeatures.size();

							if ( ! m_listHilightFeatures.empty() )
								m_listHilightFeatures[Index] = m_listHilightFeatures[Index-1];

							if ( bReverse )
								m_listHilightFeatures[Index].reverse();

							m_listHilightFeatures[Index].push_back( mapGeometry.begin()->second.second.m_T );

							// �n�C���C�g�X�V
							((IActiveViewPtr)m_cApp.GetFocusMap())->PartialRefresh( esriViewForeground, NULL, NULL );

							bHandled = true; // // �W�I���g�����ǉ��ł����ꍇ�ɂ̂�true
						}
					}
				}
			}
			break;
		}
		default:
			break;
	}
	return S_OK;
}
HRESULT CToolLineTrace::AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
{
	bHandled = false;
	if( Display && esriViewForeground == phase )
	{
		DrawHilight( Display );
	}
	return CToolPointer::AfterDraw(Display, phase, bHandled );
}
HRESULT CToolLineTrace::OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled)
{
	bHandled = false;
	switch( keyCode )
	{
		case VK_ESCAPE:
			ClearHilight();
			bHandled = true;
			break;
		default: break;
	}
	if( ! bHandled )
		return CToolPointer::OnKeyDown(keyCode, Shift, bHandled);

	return S_OK;
}
HRESULT CToolLineTrace::OnDblClick(bool& bHandled)
{
	bHandled = true;

	SetFinish();

	return CToolPointer::OnDblClick(bHandled);
}
HRESULT CToolLineTrace::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled)
{
	*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
	return S_OK;
}
void CToolLineTrace::DrawHilight( IDisplay* Display )
{
	if( ! m_ipHilightSymbol )
		m_ipHilightSymbol = AheCreateArrowLineSymbol( m_color );

	if ( m_ipHilightGeometry ) {
		Display->SetSymbol( m_ipHilightSymbol );
		Display->DrawPolyline( m_ipHilightGeometry );
	}
}
void CToolLineTrace::ClearHilight()
{
	if( ! m_listHilightGeometry.empty() )
	{
		m_listHilightGeometry.clear();
		m_listHilightFeatures.clear();

		m_ipHilightGeometry = NULL;
		IActiveViewPtr ipView( m_cApp.GetFocusMap() );
		if( ipView ) // �I�����ɂ͊���View�����݂��Ȃ��ꍇ������̂�
			ipView->PartialRefresh( esriViewForeground, NULL, NULL );
	}
}
void CToolLineTrace::clear()
{
	CToolPointer::clear();
	ClearHilight();
	m_listFinishFeatures.clear();
	m_listFinishGeometry.clear();
	m_ipFinishGeometry = NULL;
}
void CToolLineTrace::init()
{
	CToolPointer::init();
	// �n�C���C�g�N���A
	clear();
	// �����o�N���A
	m_ipClass = NULL;
	m_listHilightGeometry.clear();
	m_ipHilightSymbol = NULL;

	m_ipHilightGeometry = NULL;
	m_listHilightFeatures.clear();
	m_listFinishFeatures.clear();
	m_listFinishGeometry.clear();
	m_ipFinishGeometry = NULL;
}
void CToolLineTrace::undo()
{
	if( ! m_listHilightGeometry.empty() )
	{
		if ( ! m_listHilightFeatures.empty() )
			m_listHilightFeatures.erase( m_listHilightFeatures.find( m_listHilightFeatures.size()-1 ) );

		std::list<CAdapt<IGeometryPtr> >::iterator it = m_listHilightGeometry.begin();
		std::advance( it, m_listHilightGeometry.size() - 1 );
		m_listHilightGeometry.erase( it );

		m_ipHilightGeometry = ConstructUnion( m_listHilightGeometry );

		m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

		m_bShowContextMenu = false; // �R���e�L�X�g���j���[���o���Ȃ�
	}
	else
		m_bShowContextMenu = true; // �R���e�L�X�g���j���[���o��
}
IArrayPtr CToolLineTrace::GetFeaturesorShapes(long nMode, bool bFinish/*=true*/)
{
	IArrayPtr ipArray( CLSID_Array );

	if ( nMode == 0 ) {
		std::list<CAdapt<IGeometryPtr> > listDatas = (bFinish) ? m_listFinishGeometry : m_listHilightGeometry;

		for( std::list<CAdapt<IGeometryPtr> >::const_iterator it = listDatas.begin(); it != listDatas.end(); ++it )
			ipArray->Add(it->m_T);
	} else {
		std::list<CAdapt<IFeaturePtr> > listDatas = (bFinish) ? m_listFinishFeatures : (m_listHilightFeatures.empty() ? std::list<CAdapt<IFeaturePtr>>() : m_listHilightFeatures[m_listHilightFeatures.size()-1]);

		for( std::list<CAdapt<IFeaturePtr> >::const_iterator it = listDatas.begin(); it != listDatas.end(); ++it )
			ipArray->Add(it->m_T);
	}

	return ipArray;
}
void CToolLineTrace::SetFinish()
{
	if( ! m_listHilightGeometry.empty() ) {
		m_listFinishGeometry = m_listHilightGeometry;
		m_listFinishFeatures = m_listHilightFeatures.empty() ? std::list<CAdapt<IFeaturePtr>>() : m_listHilightFeatures[m_listHilightFeatures.size()-1];
		m_ipFinishGeometry = AheGetClone( m_ipHilightGeometry );

		if ( m_bVirtualFinish ) {
			m_bFinished = true;
		} else {
			m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
			ClearHilight();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// CToolRealTimeLineTrace
HRESULT CToolRealTimeLineTrace::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = false; // �W�I���g�����ǉ��ł����ꍇ�ɂ̂�true�ɂȂ�܂�

	if( ! m_ipClass )
		return S_OK;

	switch( Button ) {
		case 2:    // �E�N���b�N
			if ( isTracing() ) {	// �g���[�X�J�n���Ă���H
				m_bShowContextMenu = false; // �R���e�L�X�g���j���[���o���Ȃ�

				back_public();	// ���߂�
			} else
				m_bShowContextMenu = true; // �R���e�L�X�g���j���[���o��

			break;
		case 1:    // ���N���b�N
			if ( ! isTracing() ) {	// �g���[�X�J�n���Ă��Ȃ��H
				if ( startTrace( ipPoint ) )	// �g���[�X�J�n
					bHandled = true; // �g���[�X�J�n�ł����ꍇ�ɂ̂�true
			}
			break;
		default:
			break;
	}
	return S_OK;
}

HRESULT CToolRealTimeLineTrace::OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	if ( isTracing() || (m_bVirtualFinish && ! m_bFinished) ) {	// �g���[�X�J�n���Ă���H
		setAhead( Button == 1 );

		if ( trace( ipPoint ) )
			bHandled = true; // �g���[�X�ł����ꍇ�ɂ̂�true
	}

	return S_OK;
}

HRESULT CToolRealTimeLineTrace::AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
{
	bHandled = false;
	if( Display && esriViewForeground == phase )
	{
		DrawHilight( Display );
	}
	return CToolPointer::AfterDraw(Display, phase, bHandled );
}

HRESULT CToolRealTimeLineTrace::OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled)
{
	bHandled = false;

	switch ( keyCode ) {
		case VK_ESCAPE: // ESC
			stopTrace();
			ClearHilight();
			bHandled = true;
			break;
	}

	if ( !bHandled )
		CToolPointer::OnKeyDown(keyCode, Shift, bHandled);

	return S_OK;
}

HRESULT CToolRealTimeLineTrace::OnDblClick(bool& bHandled)
{
	bHandled = true;

	SetFinish();

	return CToolPointer::OnDblClick(bHandled);
}

HRESULT CToolRealTimeLineTrace::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled)
{
	*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
	return S_OK;
}

void CToolRealTimeLineTrace::DrawHilight( IDisplay* Display )
{
	if( ! m_ipHilightSymbol )
		m_ipHilightSymbol = AheCreateArrowLineSymbol( m_color );

	if ( m_ipHilightGeometry ) {
		Display->SetSymbol( m_ipHilightSymbol );
		Display->DrawPolyline( m_ipHilightGeometry );
	}
}

void CToolRealTimeLineTrace::ClearHilight(bool bRedraw/*=true*/)
{
	if( ! m_listHilightGeometry.empty() )
	{
		m_ipHilightGeometry = NULL;
		m_listHilightGeometry.clear();

		if ( bRedraw ) {
			IActiveViewPtr ipView( m_cApp.GetFocusMap() );
			if( ipView ) // �I�����ɂ͊���View�����݂��Ȃ��ꍇ������̂�
				ipView->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
}

void CToolRealTimeLineTrace::clear()
{
	CToolPointer::clear();
	ClearHilight();
	m_ipFeatureArray = NULL;
	m_ipFinishGeometry = NULL;
}

void CToolRealTimeLineTrace::init()
{
	CToolPointer::init();
	// �n�C���C�g�N���A
	clear();
	// �����o�N���A
	m_ipClass = NULL;
	m_listHilightGeometry.clear();
	m_ipHilightSymbol = NULL;
	m_ipHilightGeometry = NULL;
	m_ipFeatureArray = NULL;
	m_ipFinishGeometry = NULL;
}

void CToolRealTimeLineTrace::undo()
{
	back_public();
}

IArrayPtr CToolRealTimeLineTrace::GetFeaturesorShapes(long nMode, bool bFinish/*=true*/)
{
	return (bFinish) ? m_ipFeatureArray : getArray();
}

IArrayPtr CToolRealTimeLineTrace::getArray() const
{
	if ( isTracing() )
		return m_pLinkSnapTrace->getLinkArray();

	return NULL;
}

void CToolRealTimeLineTrace::setAhead(bool bAhead)
{
	if ( isTracing() )
		m_pLinkSnapTrace->setAhead(bAhead);
}

bool CToolRealTimeLineTrace::isTracing() const
{
	return (bool)m_pLinkSnapTrace;
}

bool CToolRealTimeLineTrace::startTrace(IPoint* ipPoint)
{
	stopTrace();

	if ( ! m_ipClass || ! ipPoint )
		return false;

	// �t�B�[�`���L���b�V�����g�����́A�������Ɏg���|�C���g�̋�ԎQ�Ƃ�
	// �t�B�[�`���N���X�̋�ԎQ�Ƃɂ��킹�Ă����Ȃ��ƃL���b�V������擾�����t�B�[�`���̌`�󂪂����̂Œ���
	ISpatialReferencePtr ipSpRef;

	((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
	ipPoint->putref_SpatialReference( ipSpRef );

	boost::shared_ptr<LinkSnapTrace> pLinkSnapTrace = (boost::shared_ptr<LinkSnapTrace>)new LinkSnapTrace(m_ipClass);

	IDisplayTransformationPtr ipTrans;
	m_cApp.GetFocusMapScreenDisplay()->get_DisplayTransformation( &ipTrans );

	double dPoint = 0.0;
	ipTrans->FromPoints(20.0, &dPoint);

	pLinkSnapTrace->setTolerance(dPoint, 4.0);

	// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
	// �����������Z�b�g
	pLinkSnapTrace->setWhere(m_strWhere);

	if ( ! pLinkSnapTrace->start( ipPoint ) )
		return false;

	m_pLinkSnapTrace = pLinkSnapTrace;

	AddHilight( GetLastFeature(), true, ipPoint );

	return true;
}

bool CToolRealTimeLineTrace::back_public()
{
	if ( ! isTracing() )
		return false;

	bool bRet = m_pLinkSnapTrace->back_public();
	if ( bRet ) {
		if ( ! m_listHilightGeometry.empty() ) {
			std::list<CAdapt<IGeometryPtr> >::iterator it = m_listHilightGeometry.begin();
			std::advance( it, m_listHilightGeometry.size() - 1 );
			m_listHilightGeometry.erase( it );
		}

		m_ipHilightGeometry = ConstructUnion( m_listHilightGeometry );

		((IActiveViewPtr)m_cApp.GetFocusMap())->PartialRefresh( esriViewForeground, NULL, NULL );
	}

	// �Ȃ��Ȃ������~����
	if ( m_listHilightGeometry.empty() )
		stopTrace();

	return bRet;
}

bool CToolRealTimeLineTrace::trace(IPoint* ipPoint)
{
	if ( ! isTracing() || ! ipPoint )
		return false;

	long nBefore = GetFeatureCount();

	// �t�B�[�`���L���b�V�����g�����́A�������Ɏg���|�C���g�̋�ԎQ�Ƃ�
	// �t�B�[�`���N���X�̋�ԎQ�Ƃɂ��킹�Ă����Ȃ��ƃL���b�V������擾�����t�B�[�`���̌`�󂪂����̂Œ���
	ISpatialReferencePtr ipSpRef;

	((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
	ipPoint->putref_SpatialReference( ipSpRef );

	bool bRet = m_pLinkSnapTrace->trace( ipPoint );
	if ( bRet ) {
		long nAfter = GetFeatureCount();

		if ( nAfter >= nBefore ) {	// �ǉ�/�ύX
			AddHilight( GetLastFeature(), (nAfter > nBefore) );
		} else if ( nAfter < nBefore ) {	// �폜
			if ( ! m_listHilightGeometry.empty() ) {
				std::list<CAdapt<IGeometryPtr> >::iterator it = m_listHilightGeometry.begin();
				std::advance( it, m_listHilightGeometry.size() - 1 );
				m_listHilightGeometry.erase( it );
			}

			m_ipHilightGeometry = ConstructUnion( m_listHilightGeometry );

			((IActiveViewPtr)m_cApp.GetFocusMap())->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}

	return bRet;
}

void CToolRealTimeLineTrace::stopTrace()
{
	m_pLinkSnapTrace.reset();
}

IFeaturePtr CToolRealTimeLineTrace::GetLastFeature()
{
	IFeaturePtr ipFeature;
	IArrayPtr ipArray = getArray();

	if ( ipArray ) {
		long nCount = 0;

		ipArray->get_Count( &nCount );
		IUnknownPtr ipUnk;

		ipArray->get_Element( nCount-1, &ipUnk );
		ipFeature = ipUnk;
	}

	return ipFeature;
}

long CToolRealTimeLineTrace::GetFeatureCount()
{
	long nCount = 0;
	IArrayPtr ipArray = getArray();

	if ( ipArray )
		ipArray->get_Count( &nCount );

	return nCount;
}

void CToolRealTimeLineTrace::AddHilight(IFeature* ipFeature, bool bAdd, IPoint* ipPoint/*=NULL*/)
{
	if ( ipFeature ) {
		IGeometryPtr ipGeometry;

		ipFeature->get_ShapeCopy( &ipGeometry );
		if ( ipGeometry ) {
			IPointPtr ipFromPoint, ipToPoint;

			((ICurvePtr)ipGeometry)->get_FromPoint( &ipFromPoint );
			((ICurvePtr)ipGeometry)->get_ToPoint( &ipToPoint );

			if ( m_listHilightGeometry.empty() ) {
				// �N���b�N�����ʒu�ɋ߂��ق��ɂɌ��������킹��
				IProximityOperatorPtr ipProximityOperator = ipPoint;

				if ( ipProximityOperator ) {
					double dDistanceFrom=0.0, dDistanceTo=0.0;

					ipProximityOperator->ReturnDistance( ipFromPoint, &dDistanceFrom );
					ipProximityOperator->ReturnDistance( ipToPoint, &dDistanceTo );

					if ( (dDistanceFrom < dDistanceTo) )
						((ICurvePtr)ipGeometry)->ReverseOrientation();
				}
			}

			if ( bAdd || m_listHilightGeometry.empty() )
				m_listHilightGeometry.push_back( ipGeometry );
			else
				(*m_listHilightGeometry.rbegin()) = ipGeometry;

			m_ipHilightGeometry = ConstructUnion( m_listHilightGeometry );

			bool bBack = false;

			if ( ! m_bClosedPermission ) {	// �z�������Ȃ��H
				ICurvePtr ipCurve( m_ipHilightGeometry );

				if ( ipCurve ) {
					IPointPtr ipFrom, ipTo;

					ipCurve->get_FromPoint( &ipFrom );
					ipCurve->get_ToPoint( &ipTo );

					if ( AheEquals( ipFrom, ipTo ) )
						bBack = true;
				}
			} else {
				if ( ! m_bTouchMidwayPoint ) {	// ����������Ȃ��H
					ICurvePtr ipCurve( m_ipHilightGeometry );

					if ( ipCurve ) {
						IPointPtr ipFrom, ipTo;

						ipCurve->get_FromPoint( &ipFrom );
						ipCurve->get_ToPoint( &ipTo );

						IPointCollectionPtr ipCol( m_ipHilightGeometry );
						IEnumVertexPtr ipVertexes;

						if( ipCol )
							ipCol->get_EnumVertices( &ipVertexes );

						if( ipVertexes ) {
							ipVertexes->Reset();

							// ��_�ڂ��擾
							// ���̈�_�ڂ��R�s�[���Q�Ƃ�������Ȃ��̂ŃR�s�[���擾���Ă���
							IPointPtr ipVertex1st;
							long lPart = -1, lVert = -1;
							ipVertexes->Next( &ipVertex1st, &lPart, &lVert );
							IPointPtr ipVertex( AheCreateClone( ipVertex1st ) );

							while( S_OK == ipVertexes->QueryNext( ipVertex, &lPart, &lVert ) )
							{
								// �����p�[�g�̏ꍇ���l�����āA�\���_�ԍ���0�̏ꍇ�͔�΂�
								if( 0 == lVert )
									continue;
								// �p�[�g�̍ŏI�_�̏ꍇ����΂�
								VARIANT_BOOL vbIsPartEnd = VARIANT_FALSE;
								ipVertexes->IsLastInPart( &vbIsPartEnd );
								if( vbIsPartEnd )
									continue;

								if ( AheEquals( ipFrom, ipVertex ) || AheEquals( ipTo, ipVertex ) ) {
									bBack = true;
									break;
								}
							}
						}
					}
				}
			}

			// NULL�����猳�ɖ߂��Ƃ�
			if ( ! m_ipHilightGeometry || bBack )
				back_public();

			((IActiveViewPtr)m_cApp.GetFocusMap())->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
}

void CToolRealTimeLineTrace::SetFinish()
{
	if( ! m_listHilightGeometry.empty() ) {
		m_ipFeatureArray = getArray();
		m_ipFinishGeometry = AheGetClone( m_ipHilightGeometry );

		if ( m_bVirtualFinish ) {
			m_bFinished = true;
		} else {
			m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON

			stopTrace();
			ClearHilight();
		}
	}
}
