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
#include "PointArrayUtil.h"
#include <algorithm>
#include <crd_cnv/crd_cnv.h>
extern crd_cnv g_cnv;	///< ���W�ϊ��N���X

namespace PointArrayUtil
{
	double threshold = 0.0000001; // ���ԍ��W����̂��߂̋��e�l(�����v�Z���ƌv�Z�덷�̖�肠��)�c0.0000001�x(���悻1cm)
	double angleThreshold = 30;

	WKSPoint dealPoint(
		const WKSPoint& point1,
		const WKSPoint& point2,
		double ratio)
	{
		WKSPoint resultPoint = {
			point1.X * (1-ratio) + point2.X * ratio,
			point1.Y * (1-ratio) + point2.Y * ratio
		};
		return resultPoint;
	}
	//WKSPointZ dealPoint(
	//	const WKSPointZ& point1,
	//	const WKSPointZ& point2,
	//	double ratio)
	//{
	//	WKSPointZ resultPoint = {
	//		point1.X * (1-ratio) + point2.X * ratio,
	//		point1.Y * (1-ratio) + point2.Y * ratio,
	//		point1.Z * (1-ratio) + point2.Z * ratio
	//	};
	//	return resultPoint;
	//}
	double getPointSeq(
		const std::vector<WKSPoint>& pointVec,
		const WKSPoint& targetPoint,
		WKSPoint& nearPoint,
		const std::list<double>& connectAngles,
		double matchingRange/* = DBL_MAX*/,
		bool useProximity/* = false*/
		)
	{
		int vecSize = pointVec.size();
		// �|�C���g��Ɉ�v����_�����邩(10cm�ȓ��ł���Έ�v���Ă���Ƃ݂Ȃ�)
		for(int i=0; i<vecSize;++i){
			const WKSPoint& point = pointVec[i];
			if( getDistance(point, targetPoint) < 0.10 )
			{
				nearPoint = point;
				return double(i);
			}
		}

		double sequence = DBL_MAX;
		double closestDist = DBL_MAX;

		// �ǂ̃|�C���g��ԂɊ܂܂�邩
		for(int i=0; i<vecSize-1;++i){
			// �Z�O�����g���ɓ����Ă����OK
			WKSPoint curCrossPoint = {};
			bool segmentInside = (0 == getProjectPoint(targetPoint, pointVec[i], pointVec[i+1], curCrossPoint));
			if(useProximity && (i==0 || i==vecSize-2))
			{
				std::list<double> angleList = getConnectAngles(pointVec, i);
				if(!hasSimilarAngle(angleList, connectAngles))
					continue;

				// �ߎ�����őΏۂ��[�̃Z�O�����g�ł���΁A�Z�O�����g���O�Ɋւ�炸�ߎ��v�Z���s��
				const WKSPoint& sidePoint = (i==0?pointVec.front():pointVec.back());
				double dist = getDistance(targetPoint, sidePoint);
				if(dist < matchingRange && dist < closestDist)
				{
					closestDist = dist;
					sequence = (i==0?0:vecSize-1);
					nearPoint = curCrossPoint;
				}
			}

			if( segmentInside )
			{
				double dist = getDistance(targetPoint, curCrossPoint);

				// �Z�O�����g�̒��ɓ��e�ł��Ă��A�ڑ��������قȂ�ꍇ�̓}�b�`���O�s�Ƃ���
				// �Z�O�����g�̕�����1�ł��邽�߁A�|�C���g�̐ڑ������͎g���Ȃ��̂Ōv�Z����
				std::list<double> angleList;
				angleList.push_back(atan2(pointVec[i+1].Y - pointVec[i].Y, pointVec[i+1].X - pointVec[i].X));
				if(!hasSimilarAngle(angleList, connectAngles))
					continue;

				if( closestDist > dist && matchingRange > dist){
					closestDist = dist;
					double dist1 = sqrt( pow(pointVec[i  ].X-curCrossPoint.X, 2)+ pow(pointVec[i  ].Y-curCrossPoint.Y, 2) );
					double dist2 = sqrt( pow(pointVec[i+1].X-curCrossPoint.X, 2)+ pow(pointVec[i+1].Y-curCrossPoint.Y, 2) );
					sequence = (double)i + dist1 / ( dist1 + dist2 );
					nearPoint = curCrossPoint;
				}
			}
		}
		
		// �e�V�[�P���X�̃Z�O�����g���m�Ɋp�x�������getProjectPoint�Ŏ��Ȃ��ꏊ���o�Ă���(ex. >�E�݂����Ȉʒu�֌W)
		// ���������ꏊ�ɂ���_�͂����΂�߂��|�C���g��T���Ă��
		for(int i=1; i<vecSize-1; ++i )
		{
			WKSPoint tmpPoint = {};
			if( getProjectPoint(targetPoint, pointVec[i-1], pointVec[i  ], tmpPoint) == 1
			 && getProjectPoint(targetPoint, pointVec[i  ], pointVec[i+1], tmpPoint) == -1 ){

				// �|�C���g�ɋ߂��Ă��A���̃|�C���g�̐ڑ��������^����ꂽ�����ƈقȂ�ꍇ�̓}�b�`���O�s�Ƃ���
				std::list<double> angleList = getConnectAngles(pointVec, i);
				if(!hasSimilarAngle(angleList, connectAngles))
					continue;

				double dist = getDistance(targetPoint, pointVec[i]);
				if( dist < closestDist && matchingRange > dist )
				{
					closestDist = dist;
					sequence = i;
					nearPoint = pointVec[i];
				}
			}
		}
		if(matchingRange < closestDist)
			return DBL_MAX;

		return sequence;
	}

	double getAngleDiff(
		double angle1,
		double angle2)
	{
		// �����������߁A-2�΁�P��2�΂̊Ԃɕϊ�
		double angleDiff = fmod(angle2 - angle1, 2*M_PI);

		// �������̐�Βl���΂𒴂���Ȃ�A�������t�]������
		// ex.225�x�̏ꍇ�́A-135�x�ɂ���B���̏ꍇ��-360+255=-135�Ƃ������̃C���[�W
		if(fabs(angleDiff) > M_PI)
			angleDiff = 2*M_PI*(angleDiff>0?-1:1) + angleDiff;
		
		return angleDiff;
	}

	bool hasSimilarAngle(
		const std::list<double>& angles1,
		const std::list<double>& angles2)
	{
		for(auto angle1 : angles1)
		{
			for(auto angle2 : angles2)
			{
				if(fabs(getAngleDiff(angle1, angle2)) < M_PI*angleThreshold/180)
				{
					return true;
				}
			}
		}
		return false;
	}
}