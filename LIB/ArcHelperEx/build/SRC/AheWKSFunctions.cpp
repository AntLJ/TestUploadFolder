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
 * @file WKSFunctions.cpp
 * @brief <b>WKSPoint���g�p�������Z�֐������t�@�C��</b>\n
 * ���̃t�@�C���́AWKSPoint���g�p�������Z�̂��߂̊֐�����������Ă��܂��B
 * IGeometry���n�߂Ƃ���COM�C���^�[�t�F�[�X�ł̉��Z�ɔ�ׁA�����ɓ���
 * ���鎖���ł��܂��B
 * WKSPoint���擾����ɂ́AIPointCollection::QueryWKSPoint���g�p���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "AheWKSFunctions.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��_�̋��������߂�
double WKS2PLength( const WKSPoint& p1, const WKSPoint& p2 )
{
	double dx = p2.X-p1.X;
	double dy = p2.Y-p1.Y;
	return sqrt(dx*dx+dy*dy);
}

// �O�_�̓��ς����߂�
double WKS3PInnerproduct( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 )
{
	return (p1.X-p2.X)*(p3.X-p2.X)+(p1.Y-p2.Y)*(p3.Y-p2.Y);
}

// �O�_�̂Ȃ��p�����߂�
double WKS3PAngle( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 )
{
	// ���� / ( �ӂP���� x �ӂQ���� )���Ƃ�l�� -1 �` 1�ł���A���ꂪ -180�� �` 180�� �͈̔͂ɑ�������
	// �܂�A1���� 0.00555�c�Ȃ̂ŁA�����_�ȉ�5��������΂������ƂɂȂ�
	// �����ŁA�v�Z�덷�ɂ�� -1 �` 1 �͈̔͂��o�Ȃ��悤�ɂ��邽�߂Ɉ�x�ۂ߂邽�߂� float �ɗ��Ƃ�
	// ����ɂ��A�ق� 180����360���̏ꍇ�Ɍv�Z�덷�� acos �����s���邱�Ƃ͂Ȃ��Ȃ�͂�
	return acos((double)(float)(WKS3PInnerproduct(p1,p2,p3)/(WKS2PLength(p1,p2)*WKS2PLength(p2,p3))));
}

// �x�N�g�����m�̌���������s��
bool WKSIsCross( const WKSPoint& pV1From, const WKSPoint& pV1To, const WKSPoint& pV2From, const WKSPoint& pV2To, bool bTouchIsCross/* = true*/, bool* bParallel/* = NULL*/, double* dV1t/* = NULL*/, double* dV2t/* = NULL*/ )
{
	bool bRet = false;	// �Ԃ�l
	if( bParallel ) *bParallel = false;

	double Ax = pV1To.X - pV1From.X;
	double Ay = pV1To.Y - pV1From.Y;
	double Bx = pV2To.X - pV2From.X;
	double By = pV2To.Y - pV2From.Y;
	double DivideBase = Bx*Ay - By*Ax;
	
	// ���s����:�{���Ȃ炵�����l��݂���ׂ������A�ʓ|�Ȃ̂�0�Ŕ���
	if( 0.0f != DivideBase )
	{
		double t = (Ax*(pV2From.Y-pV1From.Y)-Ay*(pV2From.X-pV1From.X))/DivideBase;
		double s = (Bx*(pV2From.Y-pV1From.Y)-By*(pV2From.X-pV1From.X))/DivideBase;

		if( dV1t ) *dV1t = t;
		if( dV2t ) *dV2t = s;

		// �������͐ڐG
		if( 0.0f <= t && t <= 1.0f && 
			0.0f <= s && s <= 1.0f )
		{
			if( bTouchIsCross )
				bRet = true;
			else {
				// �ڐG�������Ƃ������Ȃ��ꍇ
				if( 0.0f < t && t < 1.0f &&
					0.0f < s && s < 1.0f )
					bRet = true;
			}
		}
	}
	// ���s
	else {
		if( bParallel ) *bParallel = true;
	}

	return bRet;
}

// ��`���m�̌���������s��
bool WKSIsCross( const WKSEnvelope& rect1, const WKSEnvelope& rect2 )
{
	return ( ( rect1.XMin > rect2.XMax ) ||
			 ( rect1.XMax < rect2.XMin ) ||
			 ( rect1.YMin > rect2.YMax ) ||
			 ( rect1.YMax < rect2.YMin ) ) ? false : true;
}
