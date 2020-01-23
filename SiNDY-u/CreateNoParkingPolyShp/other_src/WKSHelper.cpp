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
* @file WKSHelper.cpp
*
* @brief WKSPoint���g�p�����W�I���g���n�֐������t�@�C��
*/
#include "stdafx.h"
#include <math.h>
#include "WKSHelper.h"

/// 3�_�̓��ς����߂�
DOUBLE innerproduct( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 )
{
	return ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
}

/// 2�_�̒��������߂�
DOUBLE length( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 )
{
	DOUBLE dx = x2 - x1;
	DOUBLE dy = y2 - y1;

	return sqrt( dx*dx + dy*dy );
}

/// 2�_����Ȃ�����̊p�x�����߂�
double angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 )
{
	DOUBLE dx,dy,t,a;
	INT    area;

	dx=x2-x1;
	dy=y2-y1;

	if(dx>=0.0)    {
		if(dy>=0.0)    { area=0;                         }  // ��P�ی�
		else           { area=3; t=dx; dx= -dy; dy=   t; }  // ��S�ی�
	}
	else{
		if(dy>=0.0)    { area=1; t=dx; dx=  dy; dy= - t; }  // ��Q�ی�
		else           { area=2;       dx= -dx; dy= -dy; }  // ��R�ی�
	}

	if(dy>dx)a=(DOUBLE)Pai/(DOUBLE)2.0-atan(dx/dy);		// �S�T�x�ȏ�
		else a=                       atan(dy/dx);		// �S�T�x�ȉ�

	return a+(DOUBLE)area*((DOUBLE)Pai/(DOUBLE)2.0);
}

/// 3�_����Ȃ�����̊p�x�����߂�
DOUBLE angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 )
{
	DOUBLE dInnerProduct = innerproduct( x1, y1, x2, y2, x3, y3 );
	DOUBLE d = length( x1, y1, x2, y2 ) * length( x2, y2, x3, y3 );

	return ( acos( dInnerProduct / d ) * (DOUBLE)180.0 ) / (DOUBLE)Pai;
}

/// �_������_�𒆐S�ɉ�]������
void rotation( DOUBLE xc, DOUBLE yc, DOUBLE rot, DOUBLE xsrc, DOUBLE ysrc, DOUBLE* xret, DOUBLE* yret )
{
	double cc,ss;

	cc=cos(-rot);
	ss=sin(-rot);

	xsrc-=xc;
	ysrc-=yc;

	*xret=  xsrc*cc+ysrc*ss+xc;
	*yret= -xsrc*ss+ysrc*cc+yc;
}

/// �p�x�i0�`360�j���烉�W�A���֕ϊ�����
DOUBLE angle2rad( DOUBLE angle ){ return ( angle * (DOUBLE)Pai ) / (DOUBLE)180.0; }

/// ���W�A������p�x�i0�`360�j�֕ϊ�����
DOUBLE rad2angle( DOUBLE rad ){ return ( rad * (DOUBLE)180 ) / (DOUBLE)Pai; }

/// �w�肳�ꂽ�����O�����v��肩�ǂ������ׂ�
BOOL IsClockwise( RING& ring, WKSPoint*& pPoints )
{
	LONG prev, curr = ring.XMIN, next;
	LONG count = ring.END - ring.START;

	prev = ( curr == ring.START   ? ring.END - 1 : curr - 1 );
	next = ( curr == ring.END - 1 ? ring.START   : curr + 1 );

	// prev �� X���W�� curr ��X���W�������Ȃ�Aprev.Y < curr.Y �Ȃ�E���Bnext �͂��̋t
	// prev�Acurr�Anext ��X���W�����ׂē����ɂȂ邱�Ƃ̓A���S���Y���゠�肦�Ȃ�
	if( pPoints[prev].X == pPoints[curr].X ) 
	{
		if( pPoints[prev].Y < pPoints[curr].Y )
		{
			//ring.IsClockwise = TRUE;
			return TRUE;
		}
		else {
			//ring.IsClockwise = FALSE;
			return FALSE;
		}
	}
	if( pPoints[next].X == pPoints[curr].X )
	{
		if( pPoints[next].Y > pPoints[curr].Y )
		{
			//ring.IsClockwise = TRUE;
			return TRUE;
		}
		else {
			//ring.IsClockwise = FALSE;
			return FALSE;
		}
	}

	// prev - curr - next �̊p�x�����߂�
	DOUBLE dAngle1 = angle( pPoints[prev].X, pPoints[prev].Y, pPoints[curr].X, pPoints[curr].Y );
	DOUBLE dAngle2 = angle( pPoints[curr].X, pPoints[curr].Y, pPoints[next].X, pPoints[next].Y );
	DOUBLE dAngle3 = rad2angle( dAngle1 - dAngle2 );
	DOUBLE dAngle;
	
	if( -180 < dAngle3 && dAngle3 < 0 ) dAngle = dAngle3;
	else if( -360 < dAngle3 && dAngle3 < -180 ) dAngle = - ( dAngle3 + (DOUBLE)180.0 );
	else dAngle = (DOUBLE)180.0 - dAngle3;

	if( dAngle < 0 )
	{
		//ring.IsClockwise = FALSE;
		return FALSE;
	}
	else {
		//ring.IsClockwise = TRUE;
		return TRUE;
	}
}

/// �_ test �� base �̃����O�ɓ����邩
sindyeChkInOut inside( const RING& base, LONG test, const WKSPoint*& pPoints )
{
	DOUBLE tX = pPoints[test].X, tY = pPoints[test].Y;
	LONG lCrossCount = 0;

	for( LONG i = base.START; i < base.END; i++ )
	{
		LONG nexti = i + 1;
		LONG previ = ( i == base.START ? base.END - 1 : i - 1 );

		// ����_�̏ꍇ�͐�΂ɂ��肦�Ȃ�
		if( pPoints[i].X == tX && pPoints[i].Y == tY )
		{
			// �����O���m�œ���̓_������
			return sindyeChkInOutSame;
		}

		// i ���� nexti ��Y���W�� tY ����ŁA���� i - nexti �� tX ���ׂ��ł���Ƃ��̂݉��Z
		if( pPoints[i].Y >= tY || pPoints[nexti].Y >= tY )
		{
			// i �� test ��X���W�������ꍇ
			if( pPoints[i].X == tX )
			{
				// previ�Ai�Anexti�AtX ���S�ē��� X���W�Ȃ�J�E���g���Ȃ�
				if( pPoints[previ].X == tX && pPoints[i].X == tX && pPoints[nexti].X == tX )
					continue;
				// �p�^�[���F1 previ�Anexti �� tX ���ׂ��ł��鎞�J�E���g
				else if( ( ( pPoints[previ].X < tX && tX <= pPoints[nexti].X ) || ( pPoints[nexti].X < tX && tX <= pPoints[previ].X ) ) && tY < pPoints[i].Y ) 
					lCrossCount++;
				// �p�^�[���F4 nexti�Aprevi �ǂ���� X���W�� tX �ƈႤ�ꍇ�̓J�E���g���Ȃ�
				else if( pPoints[nexti].X != tX && pPoints[previ].X != tX )
					continue;
			}
			// ����ȊO
			else if( ( pPoints[i].X < tX && tX < pPoints[nexti].X ) || ( pPoints[nexti].X < tX && tX < pPoints[i].X ) ) 
			{
				// ��_�� tY ����Ȃ�J�E���g
				DOUBLE crossY = (( tX - pPoints[i].X ) * ( pPoints[nexti].Y - pPoints[i].Y ))/( pPoints[nexti].X - pPoints[i].X ) + pPoints[i].Y;
				if( crossY > tY )
					lCrossCount++;
			}
		}
	}
	// �J�E���g����Ȃ����
	return ( ( lCrossCount % 2 ) == 1 ? sindyeChkInOutIn : sindyeChkInOutOut );
}

/// base��test�̃����O�ɑ΂��ăW�I���g���̊֌W�𒲂ׂ�
sindyeSpatialRel TestRelationship( const RING& base, const RING& test, WKSPoint*& pPoints, WKSPoint& errPoint )
{
	errPoint.X = -1; errPoint.Y = -1;	// ������
	LONG lIn1 = 0, lOut1 = 0;
	LONG lIn2 = 0, lOut2 = 0;
	LONG i = 0;

	for( i = base.START; i < base.END; i++ )
	{
		switch( inside( test, i, (const WKSPoint *&)pPoints ) )
		{
			case sindyeChkInOutIn:
				lIn1++;
				break;
			case sindyeChkInOutOut:
				lOut1++;
				break;
			case sindyeChkInOutSame:
				errPoint = pPoints[i];
				return sindyeSpatialRelCross;
				break;
			default:
				break;
		}
		if( lOut1 > 0 && lIn1 > 0 ) 
		{
			errPoint = pPoints[i];
			return sindyeSpatialRelCross;
		}
	}

	for( i = test.START; i < test.END; i++ )
	{
		switch( inside( base, i, (const WKSPoint *&)pPoints ) )
		{
			case sindyeChkInOutIn:
				lIn2++;
				break;
			case sindyeChkInOutOut:
				lOut2++;
				break;
			case sindyeChkInOutSame:
				errPoint = pPoints[i];
				return sindyeSpatialRelCross;
				break;
			default:
				break;
		}
		if( lOut2 > 0 && lIn2 > 0 ) 
		{
			errPoint = pPoints[i];
			return sindyeSpatialRelCross;
		}
	}

	if( lIn1 > 0 && lOut1 == 0 && lIn2 == 0 && lOut2 > 0 ) 
		return sindyeSpatialRelWithin;
	else if( lIn1 == 0 && lOut1 > 0 && lIn2 > 0 && lOut2 == 0 ) 
		return sindyeSpatialRelContain;
	else 
		return sindyeSpatialRelNone;
}
/// WKSPoint -> RING �R���o�[�^
void InitRing( LONG lPointCount, WKSPoint*& pPoints, list<RING>& listRings )
{
	BOOL bRet = TRUE;
	if( lPointCount < 3 )
	{
		//�|�C���g����3�����̓|���S������Ȃ�
		_ASSERTE( lPointCount > 2 );
		return;
	}

	// �e�����O�̊J�n�E�I���y�сAX���ő�̎��̃C���f�b�N�X���L��
	list<RING> listInRing;
	RING stRing;
	LONG lCount = -1;

	stRing.START = stRing.END = stRing.XMIN = stRing.NUM = -1;	// ������
	for( LONG i = 0; i < lPointCount; i++ )
	{
		if( stRing.START == -1 )
		{
			stRing.START = i;
			stRing.XMIN  = i;
			stRing.NUM   = (++lCount);
		}
		else if( pPoints[i].X == pPoints[stRing.START].X && pPoints[i].Y == pPoints[stRing.START].Y )
		{
			// �����O�̏I���

			stRing.END = i;
			// ���v���Ȃ�Exterior�A�����łȂ����Interior�Ƃ���
			stRing.IsClockwise = IsClockwise( stRing, pPoints );
			listRings.push_back( stRing );

			stRing.START = stRing.END = stRing.XMIN = stRing.NUM = -1;	// ������
		}
		// X���W������ȏꍇ��Y���W���������ق���I������
		else if( pPoints[stRing.XMIN].X > pPoints[i].X )
			stRing.XMIN = i;
		else if( pPoints[stRing.XMIN].X == pPoints[i].X && pPoints[stRing.XMIN].Y > pPoints[i].Y )
			stRing.XMIN = i;
	}
}

/// �����O�̕�܊֌W���`�F�b�N����
void CheckExteriorInterior( WKSPoint*& pPoints, list<RING>& listRings )
{
	// �S�Ẵ����O�ɑ΂��āA��܊֌W���`�F�b�N
	int	a_iIndx1	= 0;	// 2003.10.02
	for( list<RING>::iterator itRing1 = listRings.begin(); itRing1 != listRings.end(); itRing1++, a_iIndx1++ )
	{
		int	a_iIndx2	= a_iIndx1;	// 2003.10.02
		WKSPoint errPoint;
		list<RING>::iterator itRing2 = itRing1;
		advance( itRing2, 1 );
		a_iIndx2++;
		for( ; itRing2 != listRings.end(); itRing2++, a_iIndx2++ )
		{
			sindyeSpatialRel rel = TestRelationship( *itRing1, *itRing2, pPoints, errPoint );
			SIMPLERING ring1, ring2;
			ring1.INDX	= a_iIndx1;	// 2003.10.02
			ring1.START = itRing1->START;
			ring1.END   = itRing1->END;
			ring2.INDX	= a_iIndx2;	// 2003.10.02
			ring2.START = itRing2->START;
			ring2.END   = itRing2->END;
			switch( rel )
			{
				case sindyeSpatialRelWithin:
					itRing1->Within.push_back( ring2 );
					itRing2->Contain.push_back( ring1 );
					break;
				case sindyeSpatialRelContain:
					itRing1->Contain.push_back( ring2 );
					itRing2->Within.push_back( ring1 );
					break;
				case sindyeSpatialRelCross:
					itRing1->Cross.push_back( ring2 );
					itRing2->Cross.push_back( ring1 );
					break;
				default: 
					itRing1->Disjoint.push_back( ring2 );
					itRing2->Disjoint.push_back( ring1 );
					break;
			}
		}
	}
#ifdef DEBUG
	// �`�F�b�N
/*	for( itRing1 = listRings.begin(); itRing1 != listRings.end(); itRing1++ )
	{
		// ���̃����O���܂�ł���̂ɑ��̃����O�Ɋ܂܂�Ă͂����Ȃ�
		if( itRing1->Contain.size() > 0 && itRing1->Within.size() > 0 )
			_ASSERTE( ! ( itRing1->Contain.size() > 0 && itRing1->Within.size() > 0 ) );
		// ���̃����O�ƃN���X���Ă͂����Ȃ�
		if( itRing1->Cross.size() > 0 )
			_ASSERTE( itRing1->Cross.size() < 1 );
		// �����Ȃ̂ɑ��̃����O���܂�ł͂����Ȃ�
		if( itRing1->IsClockwise == FALSE && itRing1->Contain.size() > 0 )
			_ASSERTE( ! ( itRing1->IsClockwise == FALSE && itRing1->Contain.size() > 0 ) );
		// �E���Ȃ̂ɑ��̃����O�Ɋ܂܂�Ă͂����Ȃ�
		if( itRing1->IsClockwise == TRUE && itRing1->Within.size() > 0 )
			_ASSERTE( ! ( itRing1->IsClockwise == TRUE && itRing1->Within.size() > 0 ) );
		// �����Ȃ̂ɂǂ̃����O�ɂ��܂܂�Ă��Ȃ��̂͂����Ȃ�
		if( itRing1->IsClockwise == FALSE && itRing1->Within.size() < 1 )
			_ASSERTE( ! ( itRing1->IsClockwise == FALSE && itRing1->Within.size() < 1 ) );
	}*/
#endif
}
