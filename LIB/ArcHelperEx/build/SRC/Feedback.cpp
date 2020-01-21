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

// Feedback.cpp: CFeedback �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "Feedback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CFeedback::CFeedback() : m_piFeedback(NULL), m_nTolerance(8)
{
}

CFeedback::~CFeedback()
{

}

// �t�B�[�h�o�b�N���J�n
BOOL CFeedback::StartDrawSearchArea( IScreenDisplayPtr piScreen, IPointPtr piPoint, UINT unTolerance)
{
	// �|�C���g�t�B�[�h�o�b�N�C���X�^���X�쐬
	IMovePointFeedbackPtr piPointFeedback;
	if( SUCCEEDED( piPointFeedback.CreateInstance( CLSID_MovePointFeedback)))
	{
		// IScreenDisplay�����ƂɁA�\���~�̔��a�Z�o
		double dbTwipe = unTolerance;
		OLE_HANDLE hWnd;
		if( SUCCEEDED( piScreen->get_hWnd( &hWnd)))
		{
			HDC hDC = ::GetDC( reinterpret_cast<HWND>( hWnd));
			if( NULL != hDC)
			{
				int nLogicX = GetDeviceCaps( hDC, LOGPIXELSX);
				int nLogicY = GetDeviceCaps( hDC, LOGPIXELSY);
				int nLogical = max( nLogicX, nLogicY);
				dbTwipe = nLogical;
				dbTwipe *= unTolerance;
				dbTwipe /= static_cast<double>( 72);

				::ReleaseDC( reinterpret_cast<HWND>( hWnd), hDC);
			}
		}

		// �|�C���g�t�B�[�h�o�b�N������
		piPointFeedback->putref_Display( piScreen );
		piPointFeedback->putref_Symbol( AheGetPointSymbol( dbTwipe, 192, 192, 0, esriSMSCircle, TRUE, TRUE));
		piPointFeedback->Start( piPoint, piPoint );
		m_piFeedback = piPointFeedback;
		m_nTolerance = unTolerance;
		m_ipScreenDisplay = piScreen;

		return TRUE;
	}
	return FALSE;
}

// �t�B�[�h�o�b�N�̕`��
BOOL CFeedback::DrawSearchArea( IPointPtr piPoint)
{
	if( NULL == m_piFeedback) return FALSE;

	m_piFeedback->MoveTo( piPoint);

	return TRUE;
}

// �t�B�[�h�o�b�N�̍ĕ`��
void CFeedback::ResetSearchArea( OLE_HANDLE hDC/* = NULL */)
{
	if( NULL == m_piFeedback) return;

	m_piFeedback->Refresh( hDC );
}

// �t�B�[�h�o�b�N���~
void CFeedback::StopDrawSearchArea()
{
	if( NULL == m_piFeedback) return;

	IPointPtr piPoint;
	m_piFeedback->Stop( &piPoint);
	m_piFeedback = NULL;
}
// �����G���A���擾����iArcObjects�o�O���Łj
#ifdef _DEBUG
void AheTraceSpatialReference( ISpatialReference* ipSpRef, LPCTSTR lpcszHeader ); // AheGlobalFunctions.h
#endif // ifdef _DEBUG
IGeometryPtr CFeedback::GetSearchArea( IPointPtr ipPoint, DOUBLE* dSearchRadius, ISpatialReferencePtr ipSpRef/* = NULL*/ ) const
{
	if( ipPoint == NULL || dSearchRadius == NULL || ! m_ipScreenDisplay ) return NULL;

	IGeometryPtr ipSearchArea;
	ITopologicalOperatorPtr ipTopo( ipPoint );
	
	*dSearchRadius = -1;	// ������
	if( ipTopo == NULL )
	{
		ATLASSERT( ipTopo != NULL );
		return NULL;
	}

	if( ipSpRef == NULL )
		ipPoint->get_SpatialReference( &ipSpRef );

	double dMinTolerance = -1.0;
	if( ipSpRef )
	{
#ifdef _DEBUG
		AheTraceSpatialReference( ipSpRef, _T("aaa") );
#endif // ifdef _DEBUG
		((ISpatialReferenceTolerancePtr)ipSpRef)->get_XYTolerance( &dMinTolerance );
		dMinTolerance *= 2.0; // XY���e�l���̂܂܂ł͂�͂莸�s����悤���c
	}

	// ���a���v�Z�i���a�� +10 ���Ȃ��~�Ōv�Z�j
	IGeometryPtr ipSearchAreaTmp;
	double dSearchRadiusTmp = AheConvertPixelsToMapUnits( m_ipScreenDisplay, m_nTolerance );
	if( 0 < dMinTolerance )
		dSearchRadiusTmp = max( dMinTolerance, dSearchRadiusTmp ); // XY���e�lx2��菬�����ꍇ�͎��s����̂ŁA���̏ꍇ��XY���e�lx2�Ƃ���
	ipTopo->Buffer( dSearchRadiusTmp, &ipSearchAreaTmp );
#ifdef _DEBUG
	esriGeometryType emType;
	ipSearchAreaTmp->get_GeometryType( &emType );
	_ASSERT_EXPR( esriGeometryPolygon == emType, _T("CFeedback::GetSearchArea() : ITopologicalOperator::Buffer()�Ɏ��s���܂���") );
#endif // ifdef _DEBUG
	ipSearchAreaTmp->putref_SpatialReference( ipSpRef );
	ipSearchAreaTmp->SnapToSpatialReference();
	IAreaPtr ipAreaTmp( ipSearchAreaTmp );

	if( ! ipAreaTmp )
		return AheGetClone(ipPoint); // �o�b�t�@�����O�ł��Ȃ������ꍇ�͂��傤���Ȃ��̂œn���ꂽ�|�C���g��Ԃ�

	{
		IPointPtr ipCenterPoint, ipStartPoint;
		((IAreaPtr)ipSearchAreaTmp)->get_Centroid( &ipCenterPoint );
		((IPointCollectionPtr)ipSearchAreaTmp)->get_Point( 0, &ipStartPoint );
		if( ((IProximityOperatorPtr)ipCenterPoint)->ReturnDistance( ipStartPoint, dSearchRadius ) != S_OK)
			*dSearchRadius = 0;
	}

	// ���͂��ꂽ�|�C���g��I���G���A���a�Ńo�b�t�@���A�I���G���A���쐬
	//!< �~�Ō�������Ƌ͂��ɓ���t�B�[�`���������ł��Ȃ��ꍇ����I�I
	//!< ���傤���Ȃ��̂Ńh�b�g����10�������₵�Ă��isee Message-Id: <20021219.164152.16173513.furukawa@mr.ipc.pioneer.co.jp>�j
	dSearchRadiusTmp = AheConvertPixelsToMapUnits( m_ipScreenDisplay, 10 );
	HRESULT hr = ((ITopologicalOperatorPtr)ipSearchAreaTmp)->Buffer( dSearchRadiusTmp, &ipSearchArea );
	// Buffer�Ƃ�Ȃ��قǊg�傳��Ă���Ƃ��ɂ́A�������l�p�`������đΉ�
	if( !ipSearchArea )
	{
		IGeometryPtr ipGeom( CLSID_Envelope );
		double dX = 0.0, dY = 0.0;
		ipPoint->QueryCoords( &dX, &dY ); 
		((IEnvelopePtr)ipGeom)->PutCoords( dX - dMinTolerance, dY - dMinTolerance, dX + dMinTolerance, dY + dMinTolerance );
		ipSearchArea = ipGeom;
	}
#ifdef _DEBUG
	ipSearchArea->get_GeometryType( &emType );
	_ASSERT_EXPR( esriGeometryPolygon == emType, _T("CFeedback::GetSearchArea() : ITopologicalOperator::Buffer()�Ɏ��s���܂���") );
#endif // ifdef _DEBUG
	ipSearchArea->putref_SpatialReference( ipSpRef );
	ipSearchArea->SnapToSpatialReference();

	return ipSearchArea;
}

// �����G���A���擾����
IGeometryPtr CFeedback::GetSearchArea( IPointPtr ipPoint, ISpatialReferencePtr ipSpRef ) const
{
	if( ! ipPoint || ! m_ipScreenDisplay )
	{
		_ASSERTE( ipPoint );
		return NULL;
	}

	IGeometryPtr ipRet; // �Ԃ�l
	ITopologicalOperatorPtr ipTopo( ipPoint );
	if( ! ipTopo )
	{
		_ASSERTE( ipTopo != NULL );
		return NULL;
	}

	if( ! ipSpRef )
		ipPoint->get_SpatialReference( &ipSpRef );

	double dSearchRadius = AheConvertPixelsToMapUnits( m_ipScreenDisplay, m_nTolerance );
	if( SUCCEEDED( ipTopo->Buffer( dSearchRadius, &ipRet ) ) )
	{
		// ipTopo::Buffer���������āAipRet��NULL�Ƃ������Ƃ����邩�ǂ����͂킩��Ȃ����O�̂���
		if( !ipRet )
		{
			double dMinTolerance = -1.0;
			if( ipSpRef )
			{
				((ISpatialReferenceTolerancePtr)ipSpRef)->get_XYTolerance( &dMinTolerance );
				dMinTolerance *= 2.0; // XY���e�l���̂܂܂ł͂�͂莸�s����悤���c
			}
			IGeometryPtr ipGeom( CLSID_Envelope );
			double dX = 0.0, dY = 0.0;
			ipPoint->QueryCoords( &dX, &dY ); 
			((IEnvelopePtr)ipGeom)->PutCoords( dX - dMinTolerance, dY - dMinTolerance, dX + dMinTolerance, dY + dMinTolerance );
			ipRet = ipGeom;
		}

		ipRet->putref_SpatialReference( ipSpRef );
		return ipRet;
	}

	// �o�b�t�@�����O�Ɏ��s�����ꍇ�͑S����������ׂ̈�
	// �^����ꂽ�|�C���g�̕�����Ԃ�
	_ASSERTE(false);
	return AheGetClone( ipPoint );
}
