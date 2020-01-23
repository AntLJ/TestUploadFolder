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

/**
 * @file PointCollection.cpp
 * CGeometry�N���XIPointCollection�C���^�[�t�F�[�X�����t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "util.h"
#include "sindycomutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// �W�I���g���̑��\���_�����擾����
long CGeometry::GetPointCount() const
{
	long lCount = -1;	// �Ԃ�l
	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_GeometryNotFound )
	{
		// �|�C���g�ȊO�̓R���N�V�����ɗ��Ƃ�
		if( esriGeometryPoint != GetGeometryType() )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
			{
				LOGASSERTE_IF( SUCCEEDED( ipCol->get_PointCount( &lCount ) ), sindyErr_GeometryFunctionFailed );
			}
		}
		// �|�C���g�͏��1
		else lCount = 1;
	}

	return lCount;
}

// �W�I���g���̍\���_���擾����
CPoint CGeometry::GetPoint( long index ) const
{
	IPointPtr ipPoint;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	IPointCollectionPtr ipCol = ipGeom;

	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_GeometryNotFound )
	{
		switch( GetGeometryType() )
		{
			case esriGeometryPoint:
				LOGASSERTE_IF( 0 == index, sindyErr_ArgLimitOver )
				{
					// IPointCollection::get_Point()�̓R�s�[���쐬����̂ł���ɓ�������킹��
					ipPoint = AheCreateClone( ipGeom );
				}
				break;
			case esriGeometryMultipoint:
			{
				// IPointCollection::get_Point()��Multipoint�̏ꍇ�͎Q�Ƃ�Ԃ��̂ł���ɓ�������킹��
				IPointPtr ipTmpPoint;
				LOGASSERTE_IF( NULL != ipCol, sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipCol->get_Point( index, &ipTmpPoint ) ), sindyErr_GeometryNotFound ); // �����͈Ⴄ��������Ȃ�
				}
				ipPoint = AheCreateClone( ipTmpPoint );
				break;
			}
			default:
				LOGASSERTE_IF( NULL != ipCol, sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipCol->get_Point( index, &ipPoint ) ), sindyErr_GeometryPointNotFound ); // �����͈Ⴄ��������Ȃ�
				}
				break;
		}
	}

	return CPoint(ipPoint);
}

// �W�I���g���̍\���_���擾����
CPoint CGeometry::GetPoint( long part, long index ) const
{
	IPointPtr ipPoint;	// �Ԃ�l

	LOGASSERTE_IF( GetPartCount() > part, sindyErr_ArgLimitOver )
	{
		if( part == 0 )
			return GetPoint( index );
		else {
			CGeometry cGeom = GetPart( part );
			return cGeom.GetPoint( index );
		}
	}
	LOGASSERTE_IF( false, sindyErr_AlgorithmFailed );	// �r���ŋA��̂ł����ɂ��Ă͂����Ȃ�
	return CPoint();
}

// �\���_�̃C�e���[�^���擾����
IEnumVertexPtr CGeometry::GetEnumVertex() const
{
	IEnumVertexPtr ipRet;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		IPointCollectionPtr ipCol;
		LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTE_IF( SUCCEEDED( ipCol->get_EnumVertices( &ipRet ) ), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipRet;
}

sindyErrCode CGeometry::UpdatePoint( long i, IPoint* p )
{
	sindyErrCode emErr = sindyErr_NoErr; // �Ԃ�l

	point_iterator it = point_begin();
	if( sindyErr_NoErr == ( emErr = it.advance( i ) ) )
		return it.update( p );

	return emErr;
}

} // sindy
