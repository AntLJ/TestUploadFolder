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
#include "PointRotate.h"

// �_�����]�����Ĉ��X,Y���������Ƃ���ɂ���
IGeometryPtr	PointRotate::
ExecRotate	( IGeometryPtr&	c_ipGeo )
{
	IGeometryPtr	a_ipRetGeo;	// �ԋpGeometry

	a_ipRetGeo	= c_ipGeo;

	try {
		// �W�I���g���^�C�v�̔���
		esriGeometryType	a_eGeoType;
		if( FAILED(a_ipRetGeo->get_GeometryType ( &a_eGeoType )) ) {
			fprintf	( stderr, "�W�I���g���^�C�v�̎擾�Ɏ��s�@PointRotate::ExecRotate\n" );
			return	( NULL );
		}
		if( a_eGeoType != esriGeometryPolyline && a_eGeoType != esriGeometryPolygon ) {
			return	( NULL );
		}
		// 
		IGeometryCollectionPtr	a_ipGeoColl;
		a_ipGeoColl	= a_ipRetGeo;

		long	a_lGeoColl = 0;
		if( FAILED(a_ipGeoColl->get_GeometryCount ( &a_lGeoColl )) ) {
			fprintf	( stderr, "�W�I���g�����̎擾�Ɏ��s�@PointRotate::ExecRotate\n" );
			return	( NULL );
		}
		
		int i = 0;
		for( i = 0; i < a_lGeoColl; i++ )
		{
			IGeometryPtr	a_ipCurGeo;
			if( FAILED(a_ipGeoColl->get_Geometry (i, &a_ipCurGeo )) ) {
				fprintf	( stderr, "�W�I���g���̎擾�Ɏ��s�@PointRotate::ExecRotate\n" );
				return	( NULL );
			}

			IPointCollectionPtr	a_ipPtsColl;
			a_ipPtsColl	= a_ipCurGeo;

			long	a_lPtsCount = 0;
			if( FAILED(a_ipPtsColl->get_PointCount ( &a_lPtsCount )) ) {
				fprintf	( stderr, "�|�C���g���̎擾�Ɏ��s�@PointRotate::ExecRotate\n" );
				return	( NULL );
			}

			int j = 0;
			std::list<PtNum>	a_vpPtNum;
			PtNum				a_vpMinPoint;	// �ŏ�XY�̈ʒu(X,Y�����l�̏ꍇ�͕��т̑����ق�
			for( j = 0; j < a_lPtsCount; j++ )
			{
				IPointPtr	a_ipCurPt;
				PtNum		a_pTmp;
				if( FAILED(a_ipPtsColl->get_Point ( j, &a_ipCurPt )) ) {
					fprintf	( stderr, "�|�C���g�̎擾�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				// ���בւ��p�̏����擾
				a_pTmp.m_lNum	= j;
				if( FAILED(a_ipCurPt->get_X ( &a_pTmp.m_dX )) ) {
					fprintf	( stderr, "X���W�̎擾�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				if( FAILED(a_ipCurPt->get_Y ( &a_pTmp.m_dY )) ) {
					fprintf	( stderr, "Y���W�̎擾�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				if( j == 0 ) {
					a_vpMinPoint	= a_pTmp;
				}
				if( a_pTmp < a_vpMinPoint ) {
					a_vpMinPoint	= a_pTmp;
				}
				a_vpPtNum.push_back ( a_pTmp );
			}
			if( a_eGeoType == esriGeometryPolygon ) {
				// �W�I���g���^�C�v��Polygon�̏ꍇ�A�Ō�̓_����x�폜
				a_vpPtNum.pop_back ();

				// ���בւ�����
				for( j = 0; j < a_vpMinPoint.m_lNum; j++ )
				{
					a_vpPtNum.push_back ( *a_vpPtNum.begin() );	// �n�_��ǉ�
					a_vpPtNum.erase ( a_vpPtNum.begin() );		// �v�f�̍Ō�ɒǉ������n�_�͍폜
				}
				// �W�I���g���^�C�v��Polygon�̏ꍇ�A�Ō�̓_�Ɏn�_�Ɠ����_��ǉ�(PtNum�̓_��̔ԍ��͂��̂��Ƃł͈Ӗ����Ȃ��̂Ŗ����j
				a_vpPtNum.push_back (a_vpMinPoint);
			}else {
				// �n�_�ƏI�_�̈ʒu���r
				if( a_vpPtNum.back() < *a_vpPtNum.begin() ) {
					// �_����t�ɂ���
					a_vpPtNum.reverse();
				}
			}
			// �_����X�V����
			std::list<PtNum>::iterator	a_iIndx;
			for( j = 0, a_iIndx = a_vpPtNum.begin(); j < a_lPtsCount; j++, a_iIndx++ )
			{
				IPointPtr	a_ipCurPt;
				PtNum		a_pTmp;
				if( FAILED(a_ipPtsColl->get_Point ( j, &a_ipCurPt )) ) {
					fprintf	( stderr, "�|�C���g�̎擾�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				// ���בւ��p�̏����擾
				a_pTmp.m_lNum	= j;
				if( FAILED(a_ipCurPt->put_X ( (*a_iIndx).m_dX )) ) {
					fprintf	( stderr, "X���W�̐ݒ�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				if( FAILED(a_ipCurPt->put_Y ( (*a_iIndx).m_dY )) ) {
					fprintf	( stderr, "Y���W�̐ݒ�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
				if( FAILED(a_ipPtsColl->UpdatePoint ( j, a_ipCurPt )) ) {	// �X�V
					fprintf	( stderr, "�|�C���g�̍X�V�Ɏ��s�@PointRotate::ExecRotate�@%d\n", j );
					return	( NULL );
				}
			}
		}
	}
	catch (...) {
		fprintf	( stderr, "��O�����@PointRotate::ExecRotate\n" );
		return	( NULL );
	}
	return	( a_ipRetGeo );
}
