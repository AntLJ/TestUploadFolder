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
#include "AheBugEvasion.h"
#include "AheGeometryOP.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int exception_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

   if (code == EXCEPTION_ACCESS_VIOLATION)
      return EXCEPTION_EXECUTE_HANDLER;

   else
      return EXCEPTION_CONTINUE_SEARCH;
}

HRESULT IWkb_ExportToWkb( IWkb* ipWkb, long* pBytes, BYTE* pBuf )
{
	// �ςȌ`��ɂ����Access Violation���o��̂ō���
	__try {
		return ipWkb->ExportToWkb( pBytes, pBuf );
	}
	__except( exception_filter( GetExceptionCode(), GetExceptionInformation() ) ) {
		ATLTRACE(_T("%sIWkb::ExportToWkb() : ��O : ExceptionCode[%X]\n"), __FILE__LINE__, GetExceptionCode() );
	}
	return E_FAIL;
}

HRESULT IDataset_getWorkspace( IDataset* ipDataset, IWorkspace** ppWork )
{
	if( ! ipDataset || ! ppWork )
		return E_POINTER;

	// �Ԃт�������(bug 1765)
	// �]����try...catch�ł͈���������Ȃ��̂ŁA�\������O���g�p
	__try {
		return ipDataset->get_Workspace( ppWork );
	}
	__except( exception_filter( GetExceptionCode(), GetExceptionInformation() ) ) {
		ATLTRACE(_T("%sIDataset::get_Workspace() : ��O : ExceptionCode[%X]\n"), __FILE__LINE__, GetExceptionCode() );
	}
	return E_FAIL;
}

IGeometryPtr _ITopologicalOperatorAvoidBug( IGeometryPtr ipBase, IGeometryPtr ipOther, bool bIntersect )
{
	IGeometryPtr ipDiff;
	IGeometryPtr ipBaseClone = AheCreateClone( ipBase );
	IGeometryPtr ipOtherClone = AheCreateClone( ipOther );
	IPointCollectionPtr ipPointColBase( ipBaseClone );
	IPointPtr ipAnchorPoint;
	ipPointColBase->get_Point( 0, &ipAnchorPoint );

	ITransform2DPtr ipBaseTransForm = ipBaseClone;
	ITransform2DPtr ipOtherTransForm = ipOtherClone;
	// 1���W�A����]����i45�x�Ƃ��ł��������j
	ipBaseTransForm->Rotate( ipAnchorPoint, 1.0 );
	ipOtherTransForm->Rotate( ipAnchorPoint, 1.0 );

	ITopologicalOperatorPtr ipTopoTransForm = ipBaseTransForm;
	HRESULT hr = S_OK;
	if( bIntersect )
		hr = ipTopoTransForm->Intersect( (IGeometryPtr)ipOtherTransForm, esriGeometry1Dimension, &ipDiff );
	else
		hr = ipTopoTransForm->Difference( (IGeometryPtr)ipOtherTransForm, &ipDiff );

	// �����ł͂������Ɏ��s���Ȃ����낤
	// ���s������A�g������NULL�߂܂��Čx�����o��
	if( hr != S_OK )
		return NULL;
	// Intersect�`������̊p�x�ɖ߂�
	((ITransform2DPtr)ipDiff)->Rotate( ipAnchorPoint, -1.0 );
	return ipDiff;
}

// Arc10.1�o�O���p�R�[�h�iSiNDY-Q��p�j
IGeometryPtr ITopologicalOperator_Intersect( IGeometryPtr ipBase, IGeometryPtr ipOther )
{
	ITopologicalOperatorPtr ipTopo( ipBase );
	IGeometryPtr ipDiff;
	// �{���ł���΁A������Intersect�̑�2�����������Ƃ��ēn���Ă�����K�v�����邪�A
	// �K�v�ɂȂ�����A�n����悤�ɂ���ׂ�
	HRESULT hr = ipTopo->Intersect( ipOther, esriGeometry1Dimension, &ipDiff );
	if( hr != S_OK )
		ipDiff = _ITopologicalOperatorAvoidBug( ipBase, ipOther, true );
	return ipDiff;
}

// Arc10.1�o�O���p�R�[�h�iSiNDY-Q��p�j
IGeometryPtr ITopologicalOperator_Difference( IGeometryPtr ipBase, IGeometryPtr ipOther )
{
	ITopologicalOperatorPtr ipTopo( ipBase );
	IGeometryPtr ipDiff;
	HRESULT hr = ipTopo->Difference( ipOther, &ipDiff );
	if( hr != S_OK )
		ipDiff = _ITopologicalOperatorAvoidBug( ipBase, ipOther, false );
	return ipDiff;
}

