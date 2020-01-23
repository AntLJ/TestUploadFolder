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
extern crd_cnv g_cnv;	///< 座標変換クラス

namespace PointArrayUtil
{
	double threshold = 0.0000001; // 中間座標判定のための許容値(厳密計算だと計算誤差の問題あり)…0.0000001度(およそ1cm)
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
		// ポイント列に一致する点があるか(10cm以内であれば一致しているとみなす)
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

		// どのポイント列間に含まれるか
		for(int i=0; i<vecSize-1;++i){
			// セグメント内に入っていればOK
			WKSPoint curCrossPoint = {};
			bool segmentInside = (0 == getProjectPoint(targetPoint, pointVec[i], pointVec[i+1], curCrossPoint));
			if(useProximity && (i==0 || i==vecSize-2))
			{
				std::list<double> angleList = getConnectAngles(pointVec, i);
				if(!hasSimilarAngle(angleList, connectAngles))
					continue;

				// 近似ありで対象が端のセグメントであれば、セグメント内外に関わらず近似計算を行う
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

				// セグメントの中に投影できても、接続方向が異なる場合はマッチング不可とする
				// セグメントの方向は1つであるため、ポイントの接続方向は使えないので計算する
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
		
		// 各シーケンスのセグメント同士に角度があるとgetProjectPointで取れない場所が出てくる(ex. >・みたいな位置関係)
		// そういう場所にある点はいちばん近いポイントを探してやる
		for(int i=1; i<vecSize-1; ++i )
		{
			WKSPoint tmpPoint = {};
			if( getProjectPoint(targetPoint, pointVec[i-1], pointVec[i  ], tmpPoint) == 1
			 && getProjectPoint(targetPoint, pointVec[i  ], pointVec[i+1], tmpPoint) == -1 ){

				// ポイントに近くても、そのポイントの接続方向が与えられた方向と異なる場合はマッチング不可とする
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
		// 方向差を求め、-2π＜P＜2πの間に変換
		double angleDiff = fmod(angle2 - angle1, 2*M_PI);

		// 方向差の絶対値がπを超えるなら、符号を逆転させる
		// ex.225度の場合は、-135度にする。その場合は-360+255=-135という式のイメージ
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