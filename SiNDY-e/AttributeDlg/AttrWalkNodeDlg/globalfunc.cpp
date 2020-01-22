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
#include "globalfunc.h"


namespace gf {

// ���ӌ���
IFeatureCursorPtr AroundSearch( IFeature* ipFeature, IFeatureClass* ipFeatClass, double dRange, LPCTSTR strWhereClause/*=NULL*/ )
{
	IFeatureCursorPtr ipRetCursor;

	if( ipFeature && ipFeatClass )
	{
		IGeometryPtr ipTargetGeo;
		ipFeature->get_ShapeCopy( &ipTargetGeo );
		if( ipTargetGeo )
		{
			IEnvelopePtr ipEnv( CLSID_Envelope );
			ipTargetGeo->get_Envelope( &ipEnv );
			double dConvRange = 0.0;
			IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
			ipUnitConv->ConvertUnits( dRange, esriMeters, esriDecimalDegrees, &dConvRange );
			// TODO: ���������������������炻������g�p����
			// ���̕��@�ł́A�ꏊ�ɂ���ẮA�w���m�ȓ��Ɉ���������Ȃ��̂ŁA�K���ɍL����
			// �^�C���͂ރ��b�V���̒[�Ŏ������Ƃ��ɁA���L�̒l���x�Ɋg���Ȃ��ƁA�v����͎w�胁�[�g���ȓ��ł�
			// ���o�ł��Ȃ��t�B�[�`�������݂�������
			dConvRange *= 1.3;
			if(FAILED(ipEnv->Expand( dConvRange, dConvRange, VARIANT_FALSE )))
				return nullptr;

			// ��Ԍ���
			AheSelectByShapeAndGetCount( ipEnv, ipFeatClass, &ipRetCursor, esriSpatialRelIntersects, NULL, strWhereClause );
		}
	}

	return ipRetCursor;
}

// �|�C���g�t�B�[�`����_�ł�����
void BlinkFeature( IScreenDisplay* ipScDisp, LONG lOID, IFeatureClass* ipFeatClass, DWORD sleepTime/*=100*/, unsigned int blinkCount/*=4*/ )
{
	IFeaturePtr ipFeature;
	ipFeatClass->GetFeature( lOID, &ipFeature );
	if( ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		IPointPtr ipPoint(ipGeom);
		if( ipPoint )
		{
			for(unsigned int i=0; i<blinkCount; ++i)
			{
				AheDrawPoint( ipScDisp, ipPoint, TRUE, 8, 200, 200, 200, esriSMSSquare, TRUE, TRUE ); ::Sleep( sleepTime );
				AheDrawPoint( ipScDisp, ipPoint, TRUE, 8, 200, 200, 200, esriSMSSquare, TRUE, TRUE ); ::Sleep( sleepTime );
			}
		}
	}
}

CString VariantToCString( const CComVariant& variant, LPCTSTR strNull )
{
	// VT_NULL�łȂ���Ε�����^��ێ�����Ƃ����O��ł̏���
	CString str;
	if(VT_BSTR==variant.vt)
	{
		// ������^�ł���Ȃ當������i�[
		str = variant.bstrVal;
	}
	else
	{
		// ������^�łȂ��Ȃ�NULL��������i�[
		str = strNull;

	}
	return str;
}

} // namespace gf
