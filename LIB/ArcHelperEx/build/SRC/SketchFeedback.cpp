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

// SketchFeedback.cpp: CSketchFeedback クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SketchFeedback.h"

#include "AheSymbolFunctions.h"
#include "DrawFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CSketchFeedback::CSketchFeedback()
{
}

CSketchFeedback::~CSketchFeedback()
{

}

bool CSketchFeedback::StartPointFeedback( IPoint* ipStart )
{
	IMovePointFeedbackPtr ipPointFeedback( CLSID_MovePointFeedback );

	if( ipPointFeedback != NULL )
	{
		ipPointFeedback->putref_Display( m_ipDisp );
		ipPointFeedback->putref_Symbol( NULL );

		ipPointFeedback->Start( ipStart, ipStart );
		m_ipFeedback = ipPointFeedback;

		return true;
	}

	return false;
}

bool CSketchFeedback::StartEnvelopeFeedback( IPoint* ipStart )
{
	INewEnvelopeFeedbackPtr ipFeedback( CLSID_NewEnvelopeFeedback );

	if( ipFeedback != NULL )
	{
		ipFeedback->putref_Display( m_ipDisp );
		ipFeedback->putref_Symbol( NULL );

		ipFeedback->Start( ipStart );
		m_ipFeedback = ipFeedback;

		return true;
	}

	return false;
}

bool CSketchFeedback::StartLineFeedback( IPoint* ipStart )
{
	INewLineFeedbackPtr ipLineFeedback( CLSID_NewLineFeedback );

	if( ipLineFeedback != NULL )
	{
		ipLineFeedback->putref_Display( m_ipDisp );
		ipLineFeedback->putref_Symbol( GetSketchSymbol() );

		ipLineFeedback->Start( ipStart );
		m_ipFeedback = ipLineFeedback;

		return StartVertexFeedback( ipStart );
	}

	return false;
}

bool CSketchFeedback::StartPolygonFeedback( IPoint* ipStart )
{
	INewPolygonFeedbackPtr ipPolygonFeedback( CLSID_NewPolygonFeedback );

	if( ipPolygonFeedback != NULL )
	{
		ipPolygonFeedback->putref_Display( m_ipDisp );
		ipPolygonFeedback->putref_Symbol( GetSketchSymbol() );

		ipPolygonFeedback->Start( ipStart );
		m_ipFeedback = ipPolygonFeedback;

		if ( StartVertexFeedback( ipStart ) )
		{
			m_ipPolygonGeom = NULL;
			m_ipPolygonGeom.CreateInstance(CLSID_Polygon);
			m_ipPolygonGeom->AddPoint( ipStart );
			return true;
		}
	}

	return false;
}

bool CSketchFeedback::StartVertexFeedback( IPoint* ipStart )
{
	IMovePointFeedbackPtr ipPointFeedback( CLSID_MovePointFeedback );

	if( ipPointFeedback != NULL )
	{
		ipPointFeedback->putref_Display( m_ipDisp );
		ipPointFeedback->putref_Symbol( GetSketchVertexSymbol() );
		ipPointFeedback->Start( ipStart, ipStart );

		m_listVertexFeedback.push_back( ipPointFeedback );
	
		return true;
	}

	return false;
}

BOOL CSketchFeedback::MoveTo( IPointPtr ipMovePoint )
{
//	GetEditProp();	// 初期化

	if( m_ipFeedback != NULL )
		m_ipFeedback->MoveTo( ipMovePoint );

/*	if( m_cSnapPointFeedback.DrawSearchArea( ipMovePoint ) == FALSE )
		m_cSnapPointFeedback.StartDrawSearchArea( m_cArcHelper.GetScreenDisplay( FALSE ), ipMovePoint, AheGetTolerance() );*/

	return TRUE;
}

BOOL CSketchFeedback::AddPoint( IPointPtr ipAddPoint )
{
	INewLineFeedbackPtr ipLineFeedback( m_ipFeedback );
	INewPolygonFeedbackPtr ipPolygonFeedback( m_ipFeedback );

	if( ipLineFeedback != NULL )
	{
		ipLineFeedback->AddPoint( ipAddPoint );
	}
	else if( ipPolygonFeedback != NULL )
	{
/*		// 形状をコピー
		IClonePtr ipClone;
		((IClonePtr)m_ipPolygonGeom)->Clone(&ipClone);

		// １点追加
		((IPointCollectionPtr)ipClone)->AddPoint(ipAddPoint);
		IPolygonPtr ipPolygon( (IGeometryPtr)(ipClone ) );
		
		// 形状チェック
		if(ipPolygon != NULL)
		{
			ipPolygon->SimplifyPreserveFromTo();
			LONG lExteriorRingCount;
			ipPolygon->get_ExteriorRingCount( &lExteriorRingCount );
			
			// 不正形状
			if( lExteriorRingCount > 1 )
			{ 
				return FALSE;
			}
		}
*/		ipPolygonFeedback->AddPoint( ipAddPoint );
		m_ipPolygonGeom->AddPoint(ipAddPoint);
	}
	else
		return FALSE;

	// 頂点を描く
	IMovePointFeedbackPtr ipPointFeedback( CLSID_MovePointFeedback );
	if( ipPointFeedback != NULL )
	{
		ipPointFeedback->putref_Display( m_ipDisp );
		ipPointFeedback->putref_Symbol( GetSketchVertexSymbol() );
		ipPointFeedback->Start( ipAddPoint, ipAddPoint );

		m_listVertexFeedback.push_back( ipPointFeedback );
	}

	
	return TRUE;
}

BOOL CSketchFeedback::RemovePoint( IPointPtr ipMovePoint )
{
	IGeometryPtr ipGeom = Stop();
	m_ipFeedback = NULL;
	if( ipGeom == NULL ) return FALSE;

	esriGeometryType type;
	ipGeom->get_GeometryType( &type );

	IPointCollectionPtr ipPointCol( ipGeom );
	if( ipPointCol == NULL ) return FALSE;

	LONG lPointCount = -1;
	ipPointCol->get_PointCount( &lPointCount );

	// 1つ前の点を削除する
	// ポリゴンの時はPointCountが頂点数より1個多いため調整
	LONG lCount = ( esriGeometryPolygon == type && lPointCount > 2 ) ? lPointCount - 2 : lPointCount - 1;

	// 最後の点だった場合このまま抜ける
	if( 0 >= lCount ) 
		return TRUE;

	IPointPtr ipPoint( CLSID_Point );
	for( LONG i = 0; i < lCount; i++ )
	{
		ipPoint->PutCoords( 0.0, 0.0 );
		ipPointCol->QueryPoint( i, ipPoint );

		if( i == 0 )
		{
			switch( type )
			{
				case esriGeometryPolyline: StartLineFeedback( ipPoint );    break;
				case esriGeometryPolygon:  StartPolygonFeedback( ipPoint ); break;
				case esriGeometryPoint:    StartPointFeedback( ipPoint );   break;
				default: _ASSERTE( FALSE ); return FALSE;
			}
		}
		else
			AddPoint( ipPoint );
	}

	MoveTo( ipMovePoint );

	return TRUE;
}

BOOL CSketchFeedback::RestartFeedback( IGeometryPtr ipFeedbackGeom )
{
	if( ipFeedbackGeom == NULL ) return FALSE;

	IPointCollectionPtr ipCol( ipFeedbackGeom );
	IEnumVertexPtr	ipEnumVert;
	LONG lPointCount = -1;
	if( ipCol != NULL )
	{
		ipCol->get_PointCount( &lPointCount );
		ipCol->get_EnumVertices( &ipEnumVert );
		if( ipEnumVert != NULL ) ipEnumVert->Reset();
	}

	IPointPtr ipPoint;
	LONG lPartIndex = -1, lVertexIndex = -1;

	esriGeometryType type;
	ipFeedbackGeom->get_GeometryType( &type );
	
	switch( type )
	{
		case esriGeometryPoint:
			StartPointFeedback( (IPointPtr)ipFeedbackGeom );
			return TRUE;
			break;
		case esriGeometryPolyline:
			if( ipEnumVert != NULL )
				ipEnumVert->Next( &ipPoint, &lPartIndex, &lVertexIndex );
			if( ipPoint == NULL ) return FALSE;
			StartLineFeedback( ipPoint );
			break;
			
		case esriGeometryPolygon:
			// ポリゴンは頂点の数が1個多いので調整
			for(int i=0; i<lPointCount-1; i++){
				IPointPtr p;
				ipCol->get_Point( i, &p );
				if(i==0)
					StartPolygonFeedback( p );
				else
					AddPoint( p );
			}
			return TRUE;
			break;
			
		default:
			return FALSE;
	}
	LONG lCount = 0;
	while( S_OK == ipEnumVert->Next( &ipPoint, &lPartIndex, &lVertexIndex ) )
	{
		if( lPointCount - 1 > ++lCount )
			AddPoint( ipPoint );
	}
	return TRUE;
}

IGeometryPtr CSketchFeedback::Stop()
{
	IGeometryPtr ipRetGeom;
	INewLineFeedbackPtr ipLineFeedback( m_ipFeedback );
	INewPolygonFeedbackPtr ipPolygonFeedback( m_ipFeedback );
	IMovePointFeedbackPtr ipPointFeedback( m_ipFeedback );
	INewEnvelopeFeedbackPtr ipEnvFeedback( m_ipFeedback );

	// シンボル初期化
	m_ipSketchSymbol = m_ipVertexSymbol = NULL;

	if( ipLineFeedback != NULL )
	{
		IPolylinePtr ipPolyline;
		ipLineFeedback->Stop( &ipPolyline );

		ipRetGeom = ipPolyline;
	}
	else if( ipEnvFeedback != NULL )
	{
		IEnvelopePtr ipEnv;
		ipEnvFeedback->Stop( &ipEnv );

		ipRetGeom = ipEnv;
	}
	else if( ipPolygonFeedback != NULL )
	{
		IPolygonPtr ipPolygon;
		ipPolygonFeedback->Stop( &ipPolygon );

		// ここでは形状をそのまま返すので、使う側でSimplifyPreserveFromTo等をするように！！！

		ipRetGeom = ipPolygon;

		// 2点以下のときは何故かStopが形状を返さないため
		if( m_listVertexFeedback.size() == 2 )
		{
			IPointCollectionPtr ipCol;
			ipCol.CreateInstance(CLSID_Polygon);
			for(int i=0; i<2; i++)
			{
				IPointPtr ipPoint;
				m_ipPolygonGeom->get_Point(i, &ipPoint);
				ipCol->AddPoint(ipPoint);
			}
			ipRetGeom = ipCol;
		}
	}
	else if( ipPointFeedback != NULL )
	{
		IPointPtr ipPoint;
		ipPointFeedback->Stop( &ipPoint );

		ipRetGeom = ipPoint;
	}

	for( std::list< CAdapt< IMovePointFeedbackPtr > >::iterator it = m_listVertexFeedback.begin(); it != m_listVertexFeedback.end(); it++ )
	{
		IPointPtr ipPoint;
		it->m_T->Stop( &ipPoint );
	}

	m_listVertexFeedback.clear();
	m_ipFeedback = NULL;
	m_ipPolygonGeom = NULL;

	return ipRetGeom;
}

void CSketchFeedback::Refresh( OLE_HANDLE hDC )
{
	if( m_ipFeedback != NULL ) m_ipFeedback->Refresh( hDC );
//	m_cSnapPointFeedback.ResetSearchArea( hDC );

	for( std::list< CAdapt< IMovePointFeedbackPtr > >::iterator it = m_listVertexFeedback.begin(); it != m_listVertexFeedback.end(); it++ )
		it->m_T->Refresh( hDC );
}

BOOL CSketchFeedback::IsStartingFeedback()
{
	if( m_ipFeedback == NULL ) 
		return FALSE;
	else
		return TRUE;
}

ISymbolPtr CSketchFeedback::GetSketchSymbol()
{
	if( m_ipSketchSymbol == NULL )
	{
//		IEditPropertiesPtr ipEditProp( GetEditProp() );

/*		if( ipEditProp != NULL )
			ipEditProp->get_SketchSymbol( &m_ipSketchSymbol );
		else*/
			// [Bug 5851] とりあえずEditToolのデフォルトとおんなじにした
			m_ipSketchSymbol = AheCreateLineSymbol( AheCreateColor(RGB(127,127,255)), 1, esriSLSSolid, true );
	}

	return m_ipSketchSymbol;
}

/*ISymbolPtr CSketchFeedback::GetSnapSymbol()
{
	if( m_ipSnapSymbol == NULL )
	{
		IEditPropertiesPtr ipEditProp( GetEditProp() );

		if( ipEditProp != NULL )
			ipEditProp->get_SnapSymbol( &ipSnapSymbol );
		else
			m_ipSnapSymbol = AheGetPointSymbol( 4, 200, 200, 200, esriSMSSquare, FALSE, FALSE );
	}

	return m_ipSnapSymbol;
}*/

ISymbolPtr CSketchFeedback::GetSketchVertexSymbol()
{
	if( m_ipVertexSymbol == NULL )
	{
//		IEditPropertiesPtr ipEditProp( GetEditProp() );

/*		if( ipEditProp != NULL )
			ipEditProp->get_SketchVertexSymbol( &m_ipVertexSymbol );
		else*/
			m_ipVertexSymbol = AheCreatePointSymbol( AheCreateColor(RGB(0,200,20)), 3, esriSMSSquare, true, false );
	}

	return m_ipVertexSymbol;
}

/*IEditPropertiesPtr CSketchFeedback::GetEditProp()
{
	if( m_ipEditProp == NULL )
	{
		IApplicationPtr ipApp( CLSID_AppRef );
		if( ipApp != NULL )
		{
			m_cArcHelper.Init( ipApp );
			m_ipEditProp = m_cArcHelper.GetEditor();
		}
	}

	return m_ipEditProp;
}*/

LONG CSketchFeedback::GetPointCount()
{
	return m_listVertexFeedback.size();
}

IPointPtr CSketchFeedback::GetPoint( long lNum )
{
	IPointPtr ipRetPoint;

	if( lNum < 0 )
		return ipRetPoint;

	if( m_listVertexFeedback.size() >= (size_t)lNum )
	{
		long l = 0;
		for( std::list< CAdapt< IMovePointFeedbackPtr > >::iterator it = m_listVertexFeedback.begin(); it != m_listVertexFeedback.end(); ++it, ++l )
		{
			if( l == lNum )
			{
				it->m_T->Stop( &ipRetPoint );
				it->m_T->Start( ipRetPoint, ipRetPoint );
			}
		}
	}
	return ipRetPoint;
}
