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
#include "ArcHelperEx/AheGeometryOp.h"

extern crd_cnv g_cnv;

// �R���X�g���N�^
GeoClass::GeoClass( const IFeaturePtr ipFeature )
{
	m_ipFeature = ipFeature;
	m_ObjectID = ipFeature->GetOID();
	m_ipGeometry = ipFeature->GetShape();
}

BGBox GeoClass::GetBox(double range) const
{
	IEnvelopePtr ipEnvelope;
	m_ipGeometry->get_Envelope(&ipEnvelope);
	WKSPoint minPoint = {}, maxPoint = {};
	if(range == 0)
	{
		minPoint.X = ipEnvelope->GetXMin(); minPoint.Y = ipEnvelope->GetYMin();
		maxPoint.X = ipEnvelope->GetXMax(); maxPoint.Y = ipEnvelope->GetYMax();
	}
	else
	{
		g_cnv.GetLLPt(ipEnvelope->GetXMin(), ipEnvelope->GetYMin(), -range, -range, &(minPoint.X), &(minPoint.Y));
		g_cnv.GetLLPt(ipEnvelope->GetXMax(), ipEnvelope->GetYMax(),  range,  range, &(maxPoint.X), &(maxPoint.Y));
	}
	return BGBox(BGPoint(minPoint.X, minPoint.Y), BGPoint(maxPoint.X, maxPoint.Y));
}

// long�l���擾
long GeoClass::GetLongValue(int index) const
{
	if ( !m_ipFeature ) {
		return -1;
	}
	CComVariant val = m_ipFeature->GetValue( index );
	return val.vt != VT_NULL ? val.lVal : -1;
}

// ������Ƃ��Ď擾
CString GeoClass::GetStringValue(int index) const
{
	return m_ipFeature ? uh::str_util::ToString( m_ipFeature->GetValue( index ) ) : _T( "" );
}

// �w��̑����l�̔�r
bool GeoClass::CompareAt(const int index, const GeoClass& cmp) const
{
	return  m_ipFeature && cmp.m_ipFeature ? m_ipFeature->GetValue(index) == cmp.m_ipFeature->GetValue(index) : false;
}

// ���̑����l���擾
CComVariant GeoClass::GetValue(int index) const
{
	return m_ipFeature ? m_ipFeature->GetValue(index) : CComVariant();
}

bool GeoClass::IsOverlap( IGeometryPtr ipGeometry ) const
{
	return TRUE==AheIsOverlap2(ipGeometry, m_ipGeometry);
}

bool GeoClass::IsEqual(IGeometryPtr ipGeometry) const
{
	return TRUE==AheIsEqual(m_ipGeometry, ipGeometry);
}

bool GeoClass::IsWithin( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel(m_ipGeometry );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if( isDisjoint == VARIANT_TRUE)
		return false;
	VARIANT_BOOL isWithin = VARIANT_FALSE;
	ipRel->Within(ipGeometry, &isWithin );
	if( isWithin == VARIANT_TRUE)
		return true;

	return false;

}

bool GeoClass::IsDisjoint( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel( m_ipGeometry );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if( isDisjoint == VARIANT_TRUE)
		return true;

	return false;
}

bool GeoClass::IsCross( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel(m_ipGeometry );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if( isDisjoint == VARIANT_TRUE)
		return false;
	VARIANT_BOOL isCross = VARIANT_FALSE;
	ipRel->Crosses(ipGeometry, &isCross );
	if( isCross == VARIANT_TRUE)
		return true;

	return false;
}

// �B��̃R���X�g���N�^
FeaturesPack::FeaturesPack( ITablePtr ipTable, const sindy::CFieldMap& fieldMap )
{
	m_ipTable = ipTable;
	m_FieldMap = fieldMap;
}

// �R�s�[�R���X�g���N�^���ۂ����A��ɁAm_FeatureMap�̓R�s�[�������Ȃ��ꍇ�p
FeaturesPack::FeaturesPack( const FeaturesPack& src, const bool isCopyFeatures)
{
	m_ipTable = src.m_ipTable;
	m_FieldMap = src.m_FieldMap;
	if ( isCopyFeatures ) {
		m_FeatureMap = src.m_FeatureMap;
	}
}
