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

// Link.cpp: Link �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "Link.h"

using namespace sindy::schema;

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Link::Link() :
m_nObjectId(0),
m_nFromNodeId(0),
m_nToNodeId(0),
m_nRoadClass(0),
m_nNaviClass(0),
m_nRoadWidth(0),
m_nLinkClass(0)
{

}

Link::Link(const IFeaturePtr& ipFeature) :
m_nObjectId(0),
m_nFromNodeId(0),
m_nToNodeId(0),
m_nRoadClass(0),
m_nNaviClass(0),
m_nRoadWidth(0),
m_nLinkClass(0)
{
	setFeature(ipFeature);
}

Link::~Link()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

void Link::setFeature(const IFeaturePtr& ipFeature)
{
	using namespace road_link;

	ATLASSERT(ipFeature);
	CComVariant va;

	// �I�u�W�F�N�gID�擾
	ipFeature->get_OID(&m_nObjectId);

	// From�m�[�hID�擾
	va = GetAttribute(ipFeature, kFromNodeID);
	va.ChangeType(VT_I4);
	m_nFromNodeId = va.lVal;

	// To�m�[�hID�擾
	va = GetAttribute(ipFeature, kToNodeID);
	va.ChangeType(VT_I4);
	m_nToNodeId = va.lVal;

	// ���H��ʎ擾
	va = GetAttribute(ipFeature, kRoadClass);
	va.ChangeType(VT_I4);
	m_nRoadClass = va.lVal;

	// �o�H��ʎ擾
	va = GetAttribute(ipFeature, kNaviClass);
	va.ChangeType(VT_I4);
	m_nNaviClass = va.lVal;

	// ���H�����敪�擾
	va = GetAttribute(ipFeature, kRoadWidth);
	va.ChangeType(VT_I4);
	m_nRoadWidth = va.lVal;

	// �����N��ʎ擾
	va = GetAttribute(ipFeature, kMainLinkClass);
	va.ChangeType(VT_I4);
	m_nLinkClass = va.lVal;

	// �H���ԍ��擾
	va = GetAttribute(ipFeature, kRoadNo);
	va.ChangeType(VT_I4);
	m_nRoadNo= va.lVal;

	// �H���R�[�h�擾
	va = GetAttribute(ipFeature, kRoadCode);
	va.ChangeType(VT_I4);
	m_nRoadCode = va.lVal;

	// �`��擾
	ipFeature->get_ShapeCopy(&m_ipGeometry);
}

CComVariant Link::GetAttribute( const _IRowPtr& ipRow, long lIndex )
{
	CComVariant vaValue;

	// �����`�F�b�N
	if( ! ( ipRow && 0 <= lIndex ) )
	{
		//_ASSERTE( ipRow && 0 <= lIndex ); ������
		return vaValue;
	}

	if( FAILED( ipRow->get_Value( lIndex, &vaValue ) ) )
		_ASSERTE( SUCCEEDED( ipRow->get_Value( lIndex, &vaValue ) ) );

	return vaValue;
}

CComVariant Link::GetAttribute( const _IRowPtr& ipRow, LPCTSTR lpcszFieldName )
{
	// �����`�F�b�N
	if( ! lpcszFieldName )
	{
		_ASSERTE( lpcszFieldName );
		return CComVariant();
	}

	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	return GetAttribute( ipRow, FindField( ipFields, lpcszFieldName ) );
}

// �t�B�[���h���Ńt�B�[���h�C���f�b�N�X���擾����
long Link::FindField( const IFieldsPtr& ipFields, LPCTSTR lpcszFieldName )
{
	if( ! ( ipFields && lpcszFieldName ) )
	{
		_ASSERTE( ipFields && lpcszFieldName );
		return -1;
	}

	// FindField���Ƒ啶�����������l������K�v���o�Ă��邽�߁A
	// ���ʂ������[�v���܂킷���Ƃŉ��
	long lFieldCount = -1;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; ++i )
	{
		IFieldPtr ipField;
		CComBSTR bstrName;

		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrName );

		// �e�[�u�������ɂ��Ή�����
		CString strFieldName( bstrName );
		INT iPos = strFieldName.ReverseFind('.');
		if( iPos > 0 ) 
			strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );

		if( 0 == lstrcmpi( lpcszFieldName, strFieldName ) )
			return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
// �擾����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// ���菈��
//////////////////////////////////////////////////////////////////////

}
