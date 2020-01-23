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
#include <ArcHelperEx/GlobalFunctions.h>

// testGeom��baseGeom�ɍ\���_��v�Ŋ܂܂�邩�ǂ����m�F����
bool isContainAsSegment( IGeometryPtr baseGeom, IGeometryPtr testGeom )
{
	if( !( baseGeom && testGeom ) )
		return false;

	esriGeometryType baseType = esriGeometryAny;
	esriGeometryType testType = esriGeometryAny;
	baseGeom->get_GeometryType( &baseType );
	testGeom->get_GeometryType( &testType );

	// �|���S���ƃ|�����C���̔���̂݃T�|�[�g
	if( ! ( esriGeometryPolygon == baseType && esriGeometryPolyline == testType ) )
		return false;

	// �}���`�p�[�g�̏ꍇ�͂��ꂼ��̃p�[�g�Ŕ��f����
	long geomCount = 0;
	IGeometryCollectionPtr(testGeom)->get_GeometryCount(&geomCount);
	if( 0 >= geomCount )
		return false;

	for( long i = 0; i < geomCount; ++i )
	{
		IGeometryPtr testPartGeom;
		IGeometryCollectionPtr(testGeom)->get_Geometry(i, &testPartGeom);

		// test�̍\���_��base�̍\���_�����Ԃɕ���ł��邩�ǂ����m�F����
		IHitTestPtr hittest( baseGeom );
		IPointCollectionPtr col(testPartGeom);
		long count = 0;
		col->get_PointCount(&count);
		if( 0 >= count )
			return false;

		IPointPtr hitPoint;
		long prevHitIndex = 0;
		for( long i = 0; i < count; ++i )
		{
			IPointPtr point;
			col->get_Point( i, &point );
			if( !hitPoint )
				hitPoint = AheCreateClone(point);
			double dist = 0;
			long partIndex = 0;
			long hitIndex = 0;
			VARIANT_BOOL isRightSide = VARIANT_FALSE;
			VARIANT_BOOL hit = VARIANT_FALSE;
			// ����0�Ńq�b�g���邩����
			hittest->HitTest( point, 0.0, esriGeometryPartVertex, hitPoint, &dist, &partIndex, &hitIndex, &isRightSide, &hit );
			if( !hit )
				return false;

			// �q�b�g�����p�[�g�̍\���_�����擾
			IGeometryPtr partGeom;
			IGeometryCollectionPtr(baseGeom)->get_Geometry( partIndex, &partGeom );
			long partPointCount = 0;
			IPointCollectionPtr(partGeom)->get_PointCount( &partPointCount );

			// �q�b�g�����\���_���A�O��q�b�g�����\���_�̒��O���ォ�ǂ����m�F
			if( 0 != i && 
				! ( 
				1 == abs( prevHitIndex - hitIndex )  || // �q�b�g�ʒu���ד��m
				// �q�b�g�ʒu���n�I�_���ׂ��ꍇ
				( 0 == hitIndex     && partPointCount -2 == prevHitIndex ) || // �n�_�ƏI�_-1
				( 0 == prevHitIndex && partPointCount -2 == hitIndex     ) || // �n�_�ƏI�_-1
				( 1 == hitIndex     && partPointCount -1 == prevHitIndex ) || // �I�_�Ǝn�_+1
				( 1 == prevHitIndex && partPointCount -1 == hitIndex     )    // �n�_�ƏI�_+1
				))
				return false;

			prevHitIndex = hitIndex;
		}
	}
	return true;
}

// testGeom��baseGeom�ɍ\���_��v�Ŋ܂܂�邩�ǂ����m�F����
bool isContainAsVertex( IGeometryPtr baseGeom, IGeometryPtr testGeom )
{
	if( !( baseGeom && testGeom ) )
		return false;

	esriGeometryType baseType = esriGeometryAny;
	esriGeometryType testType = esriGeometryAny;
	baseGeom->get_GeometryType( &baseType );
	testGeom->get_GeometryType( &testType );

	// �|�����C���ƃ}���`�|�C���g�̔���̂݃T�|�[�g
	if( ! ( esriGeometryPolyline == baseType && esriGeometryMultipoint == testType ) )
		return false;

	// �}���`�p�[�g�̏ꍇ�͂��ꂼ��̃p�[�g�Ŕ��f����
	long geomCount = 0;
	IGeometryCollectionPtr(testGeom)->get_GeometryCount(&geomCount);
	if( 0 >= geomCount )
		return false;

	for( long i = 0; i < geomCount; ++i )
	{
		IGeometryPtr testPartGeom;
		IGeometryCollectionPtr(testGeom)->get_Geometry(i, &testPartGeom);

		// test�̍\���_��base�̍\���_�����Ԃɕ���ł��邩�ǂ����m�F����
		IHitTestPtr hittest( baseGeom );
		IPointPtr hitPoint;
		long prevHitIndex = 0;

		IPointPtr point(testPartGeom);
		if( !hitPoint )
			hitPoint = AheCreateClone(point);
		double dist = 0;
		long partIndex = 0;
		long hitIndex = 0;
		VARIANT_BOOL isRightSide = VARIANT_FALSE;
		VARIANT_BOOL hit = VARIANT_FALSE;
		// ����0�Ńq�b�g���邩����
		hittest->HitTest( point, 0.0, esriGeometryPartVertex, hitPoint, &dist, &partIndex, &hitIndex, &isRightSide, &hit );
		// �܂�Ԃ��`��͑z�肵�Ȃ��i���̏����ȑO�ɒׂ���Ă���ׂ��j
		// �P���ɍ\���_�Ƀq�b�g������}�[�W�ΏۂƂ���
		if( hit )
			return true;
	}
	return false;
}
