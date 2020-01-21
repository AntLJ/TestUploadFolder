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
#include "ToolMultiPolyline.h"


bool CToolMultiPolyline::Restart( IGeometry* ipFeedbackGeom )
{
	m_bShowContextMenu = false;

	// フィードバック形状の保持状態を1つ戻す
	if( ! m_FeedbackGeomList.empty() )
	{
		CSketchFeedback::RestartFeedback( m_FeedbackGeomList.back() );
		m_FeedbackGeomList.pop_back();
		return true;
	}

	return BOOL2bool(CSketchFeedback::RestartFeedback( ipFeedbackGeom ));
}


HRESULT CToolMultiPolyline::OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = true;
	return S_OK;
}

HRESULT CToolMultiPolyline::OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
{
	bHandled = true;

	// 左クリック
	if ( Button == 1 )
	{
		if ( IsStartingFeedback() )
			CSketchFeedback::AddPoint( ipPoint );
		else {
			m_bShowContextMenu = false;
			CSketchFeedback::Start( ipPoint, sindyeSketch_Polyline );
		}
	}
	// 右クリック
	else if ( Button == 2 ) 
	{
		if ( CSketchFeedback::IsStartingFeedback() )
		{
			 CSketchFeedback::RemovePoint( ipPoint );
		}
		else
		{
			// 1つ前のパートのラインフィードバックに戻す
			if( ! m_FeedbackGeomList.empty() )
			{
				CSketchFeedback::RestartFeedback( m_FeedbackGeomList.back() );
				m_FeedbackGeomList.pop_back();
			}
			else
			{
				m_bShowContextMenu = true;
			}
		}
	}

	return S_OK;
}

HRESULT CToolMultiPolyline::OnDblClick(bool& bHandled)
{
	bHandled = true;

	// フィードバック停止時は何もしない
	if ( ! CSketchFeedback::IsStartingFeedback() )
		return S_OK;

	// フォードバック形状を格納
	IGeometryPtr ipPolyline(CSketchFeedback::Stop());
	if( ipPolyline )
		m_FeedbackGeomList.push_back(ipPolyline);

	// Shift押されていないとき
	if( (GetKeyState( VK_SHIFT ) & 0x80) == 0 )
		m_bShowContextMenu = true;	

	return S_OK;
}

HRESULT CToolMultiPolyline::OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
{
	bHandled = false;

	switch ( keyCode )
	{
		// ESC キー
		case VK_ESCAPE:
			clear();
			bHandled = true;
			break;

		default:
			break;
	}

	return S_OK;
}

IGeometryPtr CToolMultiPolyline::GetShape() const
{
	if( m_FeedbackGeomList.empty() )
		return NULL;

	ISpatialReferencePtr ipSpRef;
	m_FeedbackGeomList.front().m_T->get_SpatialReference( &ipSpRef );
	
	IGeometryCollectionPtr ipResultGeomCol(CLSID_Polyline);

	// マルチパートのポリラインを作成
	for( CIter iter = m_FeedbackGeomList.begin(); iter != m_FeedbackGeomList.end(); ++iter )
	{
		IGeometryPtr ipGeom( *iter );
		IGeometryCollectionPtr ipGeomCol( ipGeom );

		long lGeomCount = -1;
		ipGeomCol->get_GeometryCount( &lGeomCount );
		for( long i = 0; i < lGeomCount; ++i )
		{
			IGeometryPtr ipPartGeom;
			ipGeomCol->get_Geometry( i, &ipPartGeom );
			ipResultGeomCol->AddGeometry( ipPartGeom );
		}
	}

	// 空間参照をあてる
	IGeometryPtr ipRetGeom( ipResultGeomCol );
	ipRetGeom->putref_SpatialReference( ipSpRef );

	return ipRetGeom;
}

void CToolMultiPolyline::init()
{
	CToolPolyline::init();
	clear();
}

void CToolMultiPolyline::clear()
{ 
	m_FeedbackGeomList.clear();
	if ( CSketchFeedback::IsStartingFeedback() )
		CSketchFeedback::Stop();
}
