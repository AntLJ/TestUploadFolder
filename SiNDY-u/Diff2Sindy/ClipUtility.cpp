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
#include <algorithm>
#include <TDC/sindylib_core/sindymacroutil.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_core/GeometryRule.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include "Notifier.h"

using namespace sindy;

#include <ArcHelperEx/GlobalFunctions.h>
#include <boost/assign.hpp>

namespace sindy {
_ISetPtr MultiGeometry2Geometries( IGeometry* geom ); // GeometryBase.cpp
} // sindy

ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk ); // AheGlobals.h

// �W�I���g���̃��x���|�C���g���擾����
IPointPtr GetLabelPoint( IGeometryPtr geom )
{
	IAreaPtr area(geom);
	if( !area )
		return geom; // �����|�C���g�Ȃ炻�̂܂܃|�C���g���Ԃ�

	IPointPtr ret;
	area->get_LabelPoint( &ret );
	return ret;
}

// �W�I���g��������
IGeometryPtr AddGeometry( IGeometryPtr geom1, IGeometryPtr geom2 )
{
	IGeometryCollectionPtr ret( CLSID_GeometryBag );
	((IGeometryPtr)ret)->putref_SpatialReference( AheGetSpatialReference(geom1) );
	std::list<CAdapt<IGeometryPtr>> listGeom = boost::assign::list_of(geom1)(geom2);
	for( auto& geom : listGeom )
	{
		IGeometryBagPtr tmp(geom.m_T);
		if( !tmp )
			ret->AddGeometry( geom.m_T );
		else {
			IGeometryCollectionPtr col(geom.m_T);
			if( !col )
				continue;
			long count = 0;
			col->get_GeometryCount(&count);
			for( long i = 0; i < count; ++i )
			{
				IGeometryPtr part;
				col->get_Geometry( i, &part );
				ret->AddGeometry( part );
			}
		}
	}
	return ret;
}

// �W�I���g�����󂩂ǂ������肷��
bool IsEmpty( IGeometryPtr geom )
{
	if( !geom )
		return true;

	VARIANT_BOOL isEmpty = VARIANT_FALSE;
	geom->get_IsEmpty( &isEmpty );
	if( isEmpty )
		return true;

	bool ret = true;
	esriGeometryType emType = esriGeometryAny;
	geom->get_GeometryType( &emType );
	switch( emType )
	{
	case esriGeometryPolyline:
	case esriGeometryPath:
	{
		IPolycurvePtr curve(geom);
		double len = 0;
		curve->get_Length( &len );
		if( len > 0.0 )
			ret = false;
		break;
	}
	case esriGeometryPolygon:
	case esriGeometryRing:
	{
		IAreaPtr area(geom);
		double dArea = 0;
		area->get_Area(&dArea);
		if( dArea > 0.0 )
			ret = false;
		break;
	}
	case esriGeometryBag:
	{
		long count = 0;
		IGeometryCollectionPtr col(geom);
		col->get_GeometryCount(&count);
		for( long i = 0; i < count; ++i )
		{
			IGeometryPtr part;
			col->get_Geometry( i, &part );
			if( !IsEmpty(part) )
				return false;
		}
		break;
	}
	default:
		ret = false;
		break;
	}
	return ret;
}

void CopyRow( CRowBase& row, IGeometryPtr geom, CTableContainer& table )
{
	CRowBase& newRow = *table.Create();
	newRow.CreateCache();
	row.CopyTo( newRow, true, false );
	newRow.CRowBase::SetShape(geom);
}

// �}���`�p�[�g�`���ʃt�B�[�`���Ƃ��ĕ�������
void MultiGeometry2Geometries( CRowBase& row, CTableContainer& table, IGeometryPtr clip )
{
	_ISetPtr clipGeoms( sindy::MultiGeometry2Geometries( (IGeometry*)*row.GetShape() ) );
	long count = 0;
	clipGeoms->Reset();
	clipGeoms->get_Count( &count );
	_ASSERTE( 0 < count );
	IRelationalOperatorPtr rel(clip);
	bool orgSetted = false;
	for( long i = 0; i < count; ++i )
	{
		IUnknownPtr unk;
		clipGeoms->Next( &unk );
		if( IsEmpty(unk) )
			continue;

		// clip�`��Ɋ܂܂��`�󂩂ǂ������f
		IGeometryPtr geom(unk);
		((ITopologicalOperatorPtr)geom)->Simplify(); // Simple����Ȃ��Ɖ��Z�����s����̂�
		VARIANT_BOOL isContain = VARIANT_FALSE;
		rel->Contains( geom, &isContain ); // Simplify���Ă�̂ɂ����ς�����Ȃ�m��Ȃ�

		// ���X��row�ɂ܂��Z�b�g���Ă��Ȃ��͂��̂܂܃Z�b�g����
		// ���łɌ��X��row�ɃZ�b�g�ς݂̏ꍇ�́A��ɃZ�b�g�����`��ŐV�����t�B�[�`�������AisContain�Ȍ`������߂�row�ɃZ�b�g����B
		// �������邱�Ƃɂ��AisContain�Ȍ`���D�悵��row�ɃZ�b�g����Ȃ��Ƃ������Ƃ��Ȃ��悤�ɂ���B
		if( !orgSetted || isContain )
		{
			if( orgSetted )
				CopyRow( row, (IGeometry*)*row.CRowBase::GetShape(), table );
			row.CRowBase::SetShape(geom);
			orgSetted = true;
		}
		else
			CopyRow( row, geom, table );
	}
}

/**
 * @brief �w�肳�ꂽ��`�ɋ߂��\���_����`�ɃX�i�b�v������
 *
 * */
void AdjustVertex( CGeometryBase& shape, IEnvelopePtr ipEnv, double threshold )
{
	WKSEnvelope env = {};
	ipEnv->QueryWKSCoords(&env);
	long geomCount = shape.GetPartCount();
	for( long i = 0; i < geomCount; ++i )
	{
		CGeometryBase part( shape.GetPart(i) );
		long pointCount = part.GetPointCount();
		for( long j = 0; j < pointCount; ++j )
		{
			IPointPtr point;
			(((IPointCollectionPtr)(IGeometry*)part)->get_Point(j, &point));
			WKSPoint p = {}, p2 = {};
			point->QueryCoords( &p.X, &p.Y );
			p2 = p;
			if( threshold > abs(env.XMax - p.X) )
				p2.X = env.XMax;
			else if( threshold > abs(env.XMin - p.X) )
				p2.X = env.XMin;
			if( threshold > abs(env.YMax - p.Y) )
				p2.Y = env.YMax;
			else if( threshold > abs(env.YMin - p.Y) )
				p2.Y = env.YMin;
			// �X�V
			if( p.X != p2.X || p.Y != p2.Y )
			{
				IPointPtr orgPoint( AheCreateClone(point) );
				point->PutCoords( p2.X, p2.Y );
				part.UpdatePoint(j, point);
			}
		}
	}
}

void ErrorReport( const CRowBase& row, const uh::tstring& msg )
{
	IPointPtr p = GetLabelPoint((IGeometry*)*row.GetShape());
	WKSPoint pp = {};
	p->QueryCoords( &pp.X, &pp.Y );
	std::wstringstream ss;
	ss << msg << _T("�F���R[" << (LPCTSTR)GETCOMERRORSTRING() << "]");
	notifier::reportObjInfo( row.GetOwnerTableName(), row.GetOID(), pp.X, pp.Y, notifier::error_level::error, ss.str(), std::wcerr );
}

/**
 * @brief �|���S���ŃN���b�s���O����
 * @param con [in] row����������R���e�i
 * @param row [in] �N���b�s���O�Ώ�
 * @param ipEnv [in] �N���b�s���O�`��
 */
void Clip( CTableContainer& con, CRowBase& row, IGeometryPtr ipClip )
{
	CGeometryBase& shape = *row.GetShape();
	// �N���b�v�`��ɂ��̂������߂��\���_������ƁA�ǂ����Ă��������Ɉ���������
	// �ׂɈӐ}�����ꏊ�ŃN���b�v�ł��Ȃ��ibug 10595, bug 10496�j
	// �Ȃ̂ŁA���O�ɍ폜���Ă��܂�
	// �N���b�v��ɖ������ړ����鏈���ł��A�ǂ����Ă������ł��Ȃ��P�[�X��������
	// ���߁A������ɏ����ύX�ibug 10496�j
	//
	// �N���b�v�O�̌`��ƃ��b�V����`�ƂŌ�_�����߁A���̌�_�ƈ��̂������l����
	// ���݂���\���_�������I�Ɉړ�����B
//	AdjustVertex( shape, ipEnv, threshold );

	IGeometryPtr clipGeom;

	// ���b�V�����E�Ő؂���A�����Ώۃ��b�V���̌����������c��
	// �ŏ�Cut2�ł�낤�Ƃ������A��͂�؂�Ȃ�������G���[���ł��肵�Ă��܂������ł��Ȃ�
	// Inersect+Difference�ő�p���邪�A���̍�Difference�Ń��b�V���|���S�����g�p����Ƃ�͂�
	// �Ȃ������܂������ł��Ȃ����Ƃ������B�B�B
	// ������Intersect�������ʂ�Difference����Ă݂���S���G���[���łȂ��Ȃ����̂ŁA���̂��
	// ���ł��B���W�b�N�ύX����ۂ͒��ӂ��邱�ƁI�I
	ITopologicalOperator2Ptr target((IGeometry*)(shape));
	if( FAILED( target->Intersect( ipClip, shape.GetDimension(), &clipGeom ) ) )
	{
		ErrorReport( row, _T("intersect�Ɏ��s�AClip�ł��܂���") );
		return;
	}

	IGeometryPtr diffGeom;
	if( FAILED( target->Difference( clipGeom, &diffGeom ) ) )
	{
		ErrorReport( row, _T("Difference�Ɏ��s�AClip�ł��܂���") );
		return;
	}

	if( !IsEmpty(diffGeom) )
		clipGeom = AddGeometry( clipGeom, diffGeom );

	if( IsEmpty(clipGeom) )
	{
		ErrorReport( row, _T("Clip�Ɏ��s�AClip�ł��܂���") );
		return;
	}
	// �}���`�p�[�g�̉\��������̂ŁA��������
	row.SetShape( clipGeom );
	MultiGeometry2Geometries( row, con, ipClip );
}
