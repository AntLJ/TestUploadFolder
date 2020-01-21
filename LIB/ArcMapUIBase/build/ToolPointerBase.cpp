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

// 両方のジオメトリの端点同士が接しているかどうかの判定
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

			// ipGeom2の端点のうちどちらかがipGeom1の端点に接している場合のみtrue
			if( AheIsTouch_EndPointAndPoint( ipGeom1, ipPoint2_1 ) != AheIsTouch_EndPointAndPoint( ipGeom1, ipPoint2_2 ) ) 
				bRet = true;
		}
	}
	return bRet;
}

// ポイントが第一引数の端点と接しているかどうかの判定
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

// 片方のジオメトリの中間点ともう片方のジオメトリの端点が接しているかどうかの判定
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
			// 一点目を取得
			// この一点目がコピーか参照か分からないのでコピーを取得しておく
			IPointPtr ipVertex1st;
			long lPart = -1, lVert = -1;
			ipVertexes->Next( &ipVertex1st, &lPart, &lVert );
			IPointPtr ipVertex( AheCreateClone( ipVertex1st ) );

			bool bFindTouch = false; // ipGeom2の端点に接する中間点があったかどうか
			while( S_OK == ipVertexes->QueryNext( ipVertex, &lPart, &lVert ) )
			{
				// 複数パートの場合を考慮して、構成点番号が0の場合は飛ばす
				if( 0 == lVert )
					continue;
				// パートの最終点の場合も飛ばす
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

// ipGeom1とipGeom2がループ無しで接続しているかどうかの判定
bool AheIsTouch_EndPointNoRoop( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	return ( AheIsTouch_EndPoint( ipGeom1, ipGeom2 ) && 
		( ! AheIsTouch_MidwayPointAndEndPoint( ipGeom1, ipGeom2 ) ) );
}

double AheCalcDistance( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	double dRet = -1.0; // 返り値

	IProximityOperatorPtr ipProx( ipGeom1 );
	if( ipProx && ipGeom2 )
		ipProx->ReturnDistance( ipGeom2, &dRet );

	return dRet;
}

IGeometryPtr ConstructUnion( const std::list<CAdapt<IGeometryPtr> >& listGeometry )
{
	if( listGeometry.empty() )
		return NULL;

	// [bug 5908] ITopologicalOperator::ConstructUnion()だと自己交差ラインでマルチパートとなるため、1個ずつつ繋ぐ
	//            listGeometryがくっつける順になっている前提
	IGeometryPtr ipUnionGeom;
	for( std::list<CAdapt<IGeometryPtr> >::const_iterator it = listGeometry.begin(); it != listGeometry.end(); ++it )
	{
		if( it != listGeometry.begin() )
			ipUnionGeom = AheUnionNetwork( ipUnionGeom, it->m_T, AheGetSpatialReference( ipUnionGeom ) );
		else
			ipUnionGeom = (IGeometryPtr)AheCreateClone( it->m_T );

		// 最後の要素の端点を先頭とする
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

	// 形状が空？
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
			// r_sugawara_20110407144356.dmp ではここで落ちていたので、try ... catch でくくっておく（bug 8651）
			// TODO: get_GeometryTypeは正常に動いていることから、IAreaへのQueryInterfaceは問題ないと思う。
			// 何らかの条件のときに、get_Areaが失敗するのだともうが、今はその条件がなんだかわからないので
			// 例外補足にとどめておく
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
				// 反時計回りでも反転して生かす（bug 8436）
				IPolygonPtr ipPoly(ipGeom);
				if( ! ipPoly ) // Envelopeの場合を考慮しないと落ちる（bug 8651）
					return false;
				ipPoly->ReverseOrientation();
				if( FAILED(((IAreaPtr)ipPoly)->get_Area( &dArea )) || 0.0 >= dArea )
					return false;
			}
			break;
		}
		default:
		{
			// 距離が0以下？
			ICurvePtr ipC = (IGeometryPtr)ipGeom;

			if ( ipC ) {
				double dbl = 0;

				ipC->get_Length( &dbl );
				if ( dbl <= 0.0 )
					return false;
			}

			// 高さと幅が0以下？
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
	if ( m_bWithArea && Button != 4 ) { // 中ボタンドラッグ中は検索エリア表示はしない（スクロールの場合にちらつく）（bug 6273）
		// 検索エリア描画
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
	// 拡大縮小・スクロール
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
	if( ! m_cFeedback.GetScreenDisplay() ) // 検索エリアを表示せずに使用しようとした場合に落ちることを防ぐ
		m_cFeedback.SetScreenDisplay( m_cApp.GetFocusMapScreenDisplay(), &m_nTolerance ); // いきなりここが呼ばれた場合にCFeedback::m_nToleranceが初期化されていない！！
	return m_cFeedback.GetSearchArea( ipPoint, ipSpRef );
}

void CToolPointer::InitTolerance()
{
	// 選択閾値を読み込む
	CSiNDYeSettings cSet;
	cSet.Load();
	m_nTolerance = cSet.GetTolerance();
}

/////////////////////////////////////////////////////////////////////////////////////////
// CToolLineTrace
HRESULT CToolLineTrace::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = false; // ジオメトリが追加できた場合にのみtrueになります

	if( ! m_ipClass )
		return S_OK;

	switch( Button )
	{
		case 2:    // 右クリック
		{
			undo();
			break;
		}
		case 1:    // 左クリック
		{
			if ( m_listFinishGeometry.empty() ) {
				// ジオメトリを追加する
				IGeometryPtr ipSearchGeom( m_cFeedback.GetSearchArea( ipPoint, AheGetSpatialReference( m_ipClass ) ) ); // XYを検索用ポリゴンに変換

				if ( ipSearchGeom ) {
					IGeometryPtr ipHilightGeometry( ConstructUnion( m_listHilightGeometry ) ); // 端点比較用
					ISpatialReferencePtr ipSpRef;

					((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
					ipSearchGeom->putref_SpatialReference( ipSpRef );

					// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
					// 検索条件と形状で検索するように修正
					IFeatureCursorPtr ipCursor;
					if( SUCCEEDED( m_ipClass->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipSearchGeom ), NULL, NULL, m_strWhere), VARIANT_FALSE, &ipCursor ) ) )
					{
						// 距離順に格納
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

						std::map<double,std::pair<CAdapt<IGeometryPtr>,CAdapt<IFeaturePtr>> > mapGeometry; // 検索できたジオメトリのmap
						// ハイライトジオメトリが既に存在する場合はそれにつながるものを選択
						// ハイライトジオメトリがない場合はXYに一番近いものを選択
						for ( std::list<CAdapt<IFeaturePtr>>::const_iterator itFeature = listFeatures.begin(); itFeature != listFeatures.end(); ++itFeature )
						{
							IFeaturePtr ipFeature = itFeature->m_T;
							bool bAdd = true;

							if( ! ipFeature )
								break;

							IGeometryPtr ipGeom;
							ipFeature->get_Shape( &ipGeom );

							if ( ! m_listHilightGeometry.empty() ) {
								if ( ! m_bClosedPermission ) {	// 循環を許可しない？
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
								// 既存のジオメトリと一致しているものは省く
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

								// [Bug 6623]同じフィーチャを選択できてしまう
								if ( bAdd && ! m_listHilightFeatures.empty() )
								{
									// 既に選択済みのフィーチャは除外する。
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

						// 一番近いものを確保
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

							// 向きが変わった場合はリストの中身も変えないといけないのでチェック
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

							// ループしている場合は入れ替える必要ないので
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

							// ハイライト更新
							((IActiveViewPtr)m_cApp.GetFocusMap())->PartialRefresh( esriViewForeground, NULL, NULL );

							bHandled = true; // // ジオメトリが追加できた場合にのみtrue
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
		if( ipView ) // 終了時には既にViewが存在しない場合があるので
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
	// ハイライトクリア
	clear();
	// メンバクリア
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

		m_bShowContextMenu = false; // コンテキストメニューを出さない
	}
	else
		m_bShowContextMenu = true; // コンテキストメニューを出す
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
			m_bShowContextMenu = true;	// コンテキストメニューフラグON
			ClearHilight();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// CToolRealTimeLineTrace
HRESULT CToolRealTimeLineTrace::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = false; // ジオメトリが追加できた場合にのみtrueになります

	if( ! m_ipClass )
		return S_OK;

	switch( Button ) {
		case 2:    // 右クリック
			if ( isTracing() ) {	// トレース開始している？
				m_bShowContextMenu = false; // コンテキストメニューを出さない

				back_public();	// 一手戻す
			} else
				m_bShowContextMenu = true; // コンテキストメニューを出す

			break;
		case 1:    // 左クリック
			if ( ! isTracing() ) {	// トレース開始していない？
				if ( startTrace( ipPoint ) )	// トレース開始
					bHandled = true; // トレース開始できた場合にのみtrue
			}
			break;
		default:
			break;
	}
	return S_OK;
}

HRESULT CToolRealTimeLineTrace::OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	if ( isTracing() || (m_bVirtualFinish && ! m_bFinished) ) {	// トレース開始している？
		setAhead( Button == 1 );

		if ( trace( ipPoint ) )
			bHandled = true; // トレースできた場合にのみtrue
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
			if( ipView ) // 終了時には既にViewが存在しない場合があるので
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
	// ハイライトクリア
	clear();
	// メンバクリア
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

	// フィーチャキャッシュを使う時は、初期化に使うポイントの空間参照を
	// フィーチャクラスの空間参照にあわせておかないとキャッシュから取得したフィーチャの形状がずれるので注意
	ISpatialReferencePtr ipSpRef;

	((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
	ipPoint->putref_SpatialReference( ipSpRef );

	boost::shared_ptr<LinkSnapTrace> pLinkSnapTrace = (boost::shared_ptr<LinkSnapTrace>)new LinkSnapTrace(m_ipClass);

	IDisplayTransformationPtr ipTrans;
	m_cApp.GetFocusMapScreenDisplay()->get_DisplayTransformation( &ipTrans );

	double dPoint = 0.0;
	ipTrans->FromPoints(20.0, &dPoint);

	pLinkSnapTrace->setTolerance(dPoint, 4.0);

	// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
	// 検索条件をセット
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

	// なくなったら停止する
	if ( m_listHilightGeometry.empty() )
		stopTrace();

	return bRet;
}

bool CToolRealTimeLineTrace::trace(IPoint* ipPoint)
{
	if ( ! isTracing() || ! ipPoint )
		return false;

	long nBefore = GetFeatureCount();

	// フィーチャキャッシュを使う時は、初期化に使うポイントの空間参照を
	// フィーチャクラスの空間参照にあわせておかないとキャッシュから取得したフィーチャの形状がずれるので注意
	ISpatialReferencePtr ipSpRef;

	((IGeoDatasetPtr)m_ipClass)->get_SpatialReference( &ipSpRef );
	ipPoint->putref_SpatialReference( ipSpRef );

	bool bRet = m_pLinkSnapTrace->trace( ipPoint );
	if ( bRet ) {
		long nAfter = GetFeatureCount();

		if ( nAfter >= nBefore ) {	// 追加/変更
			AddHilight( GetLastFeature(), (nAfter > nBefore) );
		} else if ( nAfter < nBefore ) {	// 削除
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
				// クリックした位置に近いほうにに向きを合わせる
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

			if ( ! m_bClosedPermission ) {	// 循環を許可しない？
				ICurvePtr ipCurve( m_ipHilightGeometry );

				if ( ipCurve ) {
					IPointPtr ipFrom, ipTo;

					ipCurve->get_FromPoint( &ipFrom );
					ipCurve->get_ToPoint( &ipTo );

					if ( AheEquals( ipFrom, ipTo ) )
						bBack = true;
				}
			} else {
				if ( ! m_bTouchMidwayPoint ) {	// 分岐を許可しない？
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

							// 一点目を取得
							// この一点目がコピーか参照か分からないのでコピーを取得しておく
							IPointPtr ipVertex1st;
							long lPart = -1, lVert = -1;
							ipVertexes->Next( &ipVertex1st, &lPart, &lVert );
							IPointPtr ipVertex( AheCreateClone( ipVertex1st ) );

							while( S_OK == ipVertexes->QueryNext( ipVertex, &lPart, &lVert ) )
							{
								// 複数パートの場合を考慮して、構成点番号が0の場合は飛ばす
								if( 0 == lVert )
									continue;
								// パートの最終点の場合も飛ばす
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

			// NULLったら元に戻しとく
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
			m_bShowContextMenu = true;	// コンテキストメニューフラグON

			stopTrace();
			ClearHilight();
		}
	}
}
