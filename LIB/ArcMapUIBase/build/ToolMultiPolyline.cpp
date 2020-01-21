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

	// �t�B�[�h�o�b�N�`��̕ێ���Ԃ�1�߂�
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

	// ���N���b�N
	if ( Button == 1 )
	{
		if ( IsStartingFeedback() )
			CSketchFeedback::AddPoint( ipPoint );
		else {
			m_bShowContextMenu = false;
			CSketchFeedback::Start( ipPoint, sindyeSketch_Polyline );
		}
	}
	// �E�N���b�N
	else if ( Button == 2 ) 
	{
		if ( CSketchFeedback::IsStartingFeedback() )
		{
			 CSketchFeedback::RemovePoint( ipPoint );
		}
		else
		{
			// 1�O�̃p�[�g�̃��C���t�B�[�h�o�b�N�ɖ߂�
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

	// �t�B�[�h�o�b�N��~���͉������Ȃ�
	if ( ! CSketchFeedback::IsStartingFeedback() )
		return S_OK;

	// �t�H�[�h�o�b�N�`����i�[
	IGeometryPtr ipPolyline(CSketchFeedback::Stop());
	if( ipPolyline )
		m_FeedbackGeomList.push_back(ipPolyline);

	// Shift������Ă��Ȃ��Ƃ�
	if( (GetKeyState( VK_SHIFT ) & 0x80) == 0 )
		m_bShowContextMenu = true;	

	return S_OK;
}

HRESULT CToolMultiPolyline::OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
{
	bHandled = false;

	switch ( keyCode )
	{
		// ESC �L�[
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

	// �}���`�p�[�g�̃|�����C�����쐬
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

	// ��ԎQ�Ƃ����Ă�
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
