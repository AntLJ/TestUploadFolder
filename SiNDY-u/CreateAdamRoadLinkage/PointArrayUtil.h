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

#pragma once
#include <vector>
#include <list>
#include <crd_cnv/crd_cnv.h>

// 点列を扱う関数群。比較的マッチング関係が多い。
namespace PointArrayUtil
{
	extern double threshold; // 中間座標判定のための許容値
	extern double angleThreshold;

	/**
	 * @brief 1番目の点と3番目の間に2番目の点が存在するかを判定する
	 * @note 計算誤差を考慮して、1番目の点と3番目の点がX軸、Y軸にほぼ平行な場合は微小な誤差を許容する
	 * @param point1 [in] 1番目の点
	 * @param point2 [in] 2番目の点
	 * @param point3 [in] 3番目の点
	 * @retval true 2番目の点は1番目と3番目の点の間に存在する
	 * @retval false 2番目の点は1番目と3番目の点の間に存在しない
	 */
	template<typename T1, typename T2>
	bool isBetween(
		const T1& point1,
		const T2& point2,
		const T1& point3)
	{
		// ポイント1と3のX座標の差が許容値以内であれば、
		if(fabs(point1.X-point3.X)<threshold)
		{
			// X座標は中間値より許容値の乖離まで許容する
			if(fabs((point1.X+point3.X)*0.5-point2.X)>threshold)
				return false;
		}
		else if((point1.X-point2.X)*(point3.X-point2.X) > 0)
		{
			// 許容値以内でなければ、座標の差をかけてはみ出し判定
			return false;
		}
		// ポイント1と3のY座標の差が許容値以内であれば、
		if(fabs(point1.Y-point3.Y)<threshold)
		{
			// Y座標は中間値より許容値の乖離まで許容する
			if(fabs((point1.Y+point3.Y)*0.5-point2.Y)>threshold)
				return false;
		}
		else if((point1.Y-point2.Y)*(point3.Y-point2.Y) > 0)
		{
			// 許容値以内でなければ、座標の差をかけてはみ出し判定
			return false;
		}

		return true;
	}

	/**
	 * @brief 2点間の距離を計算する
	 * @param point1 [in] ポイント1
	 * @param point2 [in] ポイント2
	 * @return 2点間の距離(m)
	 */
	template<typename T1, typename T2>
	double getDistance(
		const T1& point1,
		const T2& point2)
	{
		return g_cnv.GetDist(point1.X, point1.Y, point2.X, point2.Y);
	}

	/**
	 * @brief 点列の長さを計算する
	 * @param points [in] 点列
	 * @return 点列の長さ(m)
	 */
	template<typename T>
	double getLength(
		const std::vector<T>& points)
	{
		double length = 0;
		for(int i=0; i<points.size()-1; ++i)
			length += getDistance(points[i], points[i+1]);

		return length;
	}

	/**
	 * @brief 目的の点とセグメントの距離を求める
	 * @param point1 [in] セグメントの始点
	 * @param point2 [in] セグメントの終点
	 * @param targetPoint [in] 目的の点
	 * @return セグメントと目的の点の距離(単位:m)
	 */
	template<typename T1, typename T2>
	double getSegmentDist(const T1& point1, const T1& point2, const T2& targetPoint)
	{
		double a = point2.Y - point1.Y;
		double b = point1.X - point2.X;
		double c = point2.X*point1.Y - point1.X*point2.Y;

		double D = a*a + b*b;
		// Dが0ということはポイント1,2が同一点であるということ
		if(D==0)
			return getDistance(point1, targetPoint);
		
		// ポイント1-2のライン上で最も対象点に近いポイント
		WKSPoint nearestPoint = {};
		nearestPoint.X = (-a*b*targetPoint.Y+b*b*targetPoint.X-a*c) / D;
		nearestPoint.Y = (-a*b*targetPoint.X+a*a*targetPoint.Y-b*c) / D;

		if(isBetween(point1, nearestPoint, point2))
		{
			return getDistance(targetPoint, nearestPoint);
		}
		else
		{
			return min(getDistance(targetPoint, point1),
			           getDistance(targetPoint, point2));
		}
	}

	/**
	 * @brief 方向差を求める
	 * @note 結果は-πよりも大きくπ未満で返す
	 * @param angle1 [in] 方向1
	 * @param angle2 [in] 方向2
	 * @return 方向差(rad)
	 */
	double getAngleDiff(
		double angle1,
		double angle2);

	/**
	 * @brief 2つの方向リストに近似した角度があるか
	 * @note 角度許容値以内で近似した対があれば真、なければ偽
	 * @param angles1 [in] 方向リスト1
	 * @param angles2 [in] 方向リスト2
	 * @retval true リスト1,2の間に近似した方向の対がある
	 * @retval false リスト1,2の間には近似した方向の対がない
	 */
	bool hasSimilarAngle(
		const std::list<double>& angles1,
		const std::list<double>& angles2);

	/**
	 * @brief 点列の接続角度群を返す
	 * @note 実距離に合わせる必要はないので、緯度経度を2次平面として考える
	         対象インデックスが点群の範囲を超えたら空リストを返す
			 対象インデックスが0超過の場合は前の点から対象の点までの方向を追加
			 対象インデックスが末尾以外の場合は対象の点から次の点までの方向を追加
	 * @param points [in] 点群
	 * @param index [in] 対象の点のインデックス
	 */
	template<typename T1>
	std::list<double> getConnectAngles(
		const std::vector<T1>& points,
		unsigned int index
		)
	{
		std::list<double> result;

		// 範囲が適正でなければ、空のリストを返す
		if(index < 0 || index >= points.size())
			return result;

		// 対象インデックスが0より大きい場合は、ひとつ前のインデックスから対象インデックスに向かう方向を追加
		if(0 < index)
			result.push_back(atan2(points[index].Y - points[index-1].Y, points[index].X - points[index-1].X));

		// 対象インデックスが末尾でなければ、対象インデックスから次のインデックスに向かう方向を追加
		if(index < points.size()-1)
			result.push_back(atan2(points[index+1].Y - points[index].Y, points[index+1].X - points[index].X));

		return result;
	}

	/**
	 * @brief 2つの点列が最も遠ざかる距離を計算する
	 * @param points1 [in] 点列1
	 * @param points2 [in] 点列2
	 * @return 2点列が最も遠ざかる距離
	 */
	template<typename T1, typename T2>
	double getFarthestDistance(
		const std::vector<T1>& points1,
		const std::vector<T2>& points2)
	{
		double maxDist = -DBL_MAX;
		int segmentNum = points2.size()-1;

		// ライン1の各点とライン2セグメントの距離チェック
		for(const auto& point : points1)
		{
			double curMinDist = DBL_MAX;
			for(int i=0; i<segmentNum; ++i)
			{
				const T2& fromPt = points2[i];
				const T2& toPt   = points2[i+1];
				curMinDist = min(curMinDist, getSegmentDist(fromPt, toPt, point));
			}
			maxDist = max(maxDist, curMinDist);
		}
		if(maxDist == -DBL_MAX)
			throw std::runtime_error("getFarthestDistance: Logic Error");
		
		return maxDist;
	}

	/**
	 * @brief 2点の間の任意の位置の点を求める
	 * @param point1 [in] ポイント1
	 * @param point2 [in] ポイント2
	 * @param ratio [in] 割合(0でポイント1と同じ。1でポイント2と同じ。0.5で中間点)
	 * @return 割合に応じた座標
	 */
	WKSPoint dealPoint(
		const WKSPoint& point1,
		const WKSPoint& point2,
		double ratio);
	
	//WKSPointZ dealPoint(
	//	const WKSPointZ& point1,
	//	const WKSPointZ& point2,
	//	double ratio);
	/**
	 * @brief 目的の点からあるセグメントを通る直線に投影した点の座標を得る
	 * @note 要は直線上の点で目的の点に最も近い座標を得る。
	 * @note 投影点にはZ値が付かないので注意
	 * @param targetPoint [in] 目的の点
	 * @param fromPoint [in] セグメントの始点
	 * @param toPoint [in] セグメントの終点
	 * @param projPoint [out] 投影点
	 * @retval -1 投影点はセグメントの始点の向こう側に存在する
	 * @retval  0 投影点はセグメントの中間に存在する
	 * @retval  1 投影点はセグメントの終点の向こう側に存在する
	 */
	template<typename T>
	int getProjectPoint(
		const T& targetPoint,
		const WKSPoint& fromPoint,
		const WKSPoint& toPoint,
		WKSPoint& projPoint)
	{
		// 今見ているセグメントの式を計算(ax+by+c=0の係数)
		double a = toPoint.Y - fromPoint.Y;
		double b = fromPoint.X - toPoint.X;
		double c = toPoint.X * fromPoint.Y - fromPoint.X * toPoint.Y;

		// a*a + b*bが0の場合、0除算が起きるため計算不能(セグメントの2点が同一座標の場合に起こる)
		if( a*a + b*b == 0 ){
			return false;
		}
		// aが0、すなわちセグメントがX軸に平行であるときは、交点のX座標は対象ポイントのX座標、Y座標はセグメントのY座標
		if(a == 0){
			projPoint.X = targetPoint.X;
			projPoint.Y = fromPoint.Y;
		}
		// bが0、すなわちセグメントがY軸に平行であるときは、交点のX座標はセグメントのX座標、Y座標は対象ポイントのY座標
		else if(b == 0){
			projPoint.X = fromPoint.X;
			projPoint.Y = targetPoint.Y;
		}
		// どちらにも該当しない場合は、セグメントの式から交点を求める
		else{
			projPoint.X = ( -a*b*targetPoint.Y + b*b*targetPoint.X - a*c ) / ( a*a + b*b );
			projPoint.Y = ( -a*b*targetPoint.X + a*a*targetPoint.Y - b*c ) / ( a*a + b*b );
		}
		// 最後にZ値を設定する
		double fromDist = getDistance(fromPoint, projPoint);
		double toDist   = getDistance(toPoint,   projPoint);

		// 交点のX,Y座標がセグメントに納まっている場合は、true
		if(isBetween(fromPoint, projPoint, toPoint))
		{
			return 0;
		}
		// 交点がFrom側、すなわち交点→From→Toの位置関係なら-1を返す
		if(isBetween(projPoint, fromPoint, toPoint)){
			return -1;
		}
		else{
			return 1;
		}
	}

	/**
	 * @brief 点列に対して対象のポイントがどの位置に位置するかを求める
	 * @note セグメントの方向群は、空であれば考慮しない
	 * @param pointVec [in] 点列
	 * @param targetPoint [in] 対象のポイント
	 * @param nearPoint [in] 点列上で対象のポイントに最も近いポイント
	 * @param connectAngles [in] 対象のポイントに接続するセグメントの方向群(逆方向は異なるとする)
	                             SIPのマッチングでは交差点考慮しないため、1か2のはず
	 * @param matchingRange [in] マッチングの範囲(最も近い点がこの距離以上ならDBL_MAX返す)
	 * @param useProximity [in] 近似の有無(trueが設定されていると、最も近いポイントが点列の端のセグメントをはみ出してもOK)
	 * @return シーケンス(最も近いポイントが点列の第3点と第4点の3:7の位置にあれば、3.3と表記)
	 * @retval DBL_MAX 対象のポイントは規定以上に離れているため計算不可
	 */
	double getPointSeq(
		const std::vector<WKSPoint>& pointVec,
		const WKSPoint& targetPoint,
		WKSPoint& nearPoint,
		const std::list<double>& connectAngles,
		double matchingRange = DBL_MAX,
		bool useProximity = false
		);

	/**
	 * @brief 点列のシーケンスに応じた座標を取得する
	 * @param srcLine [in] 元点列
	 * @param seq [in] シーケンス(点列の第3点と第4点の3:7の位置にあれば、3.3と表記するような値)
	 * @return シーケンスに応じた座標
	 */
	template<typename T>
	T getSeqPoint(
		const std::vector<T>& srcLine,
		double seq)
	{
		assert(0 <= seq && seq <= double(srcLine.size()));

		double amari = seq - double(int(seq));

		if(amari == 0)
			return srcLine[int(seq)];

		int pos = int(seq);
		const T& prePoint  = srcLine[pos];
		const T& nextPoint = srcLine[pos+1];
		T point = dealPoint(prePoint, nextPoint, amari);
		return point;
	}
	/**
	 * @brief 点列をシーケンスでカットする
	 * @note シーケンスとは、点列の第3点と第4点の3:7の位置にあれば、3.3と表記するような値
	 * @param srcLine [in] 元点列
	 * @param fromSeq [in] 始点シーケンス
	 * @param toSeq [in] 終点シーケンス
	 */
	template<typename T>
	std::vector<T> cutLine(
		const std::vector<T>& srcLine,
		double fromSeq,
		double toSeq)
	{
		assert(0 <= fromSeq && toSeq <= double(srcLine.size()) && fromSeq < toSeq);
		std::vector<T> resultLine;

		// ひとまず範囲内をコピー(std::copyは指定された終端の1個手前までをコピーするのでtoSeq+1)
		copy(srcLine.begin()+int(fromSeq), srcLine.begin()+int(toSeq)+1, back_inserter(resultLine));
		
		// Fromシーケンスが整数値でなければ、最初の点を置き換える
		// その前にコピーした最初の点は、端数切り捨てでお目当ての点より前のはずだから。
		if(fromSeq - double(int(fromSeq)) != 0)
		{
			resultLine[0] = getSeqPoint(srcLine, fromSeq);
		}

		// Toシーケンスが整数値でなければ、末尾に点を追加する
		// その前にコピーした最後の点は、端数切捨てでお目当ての点より手前で切れているはずだから。
		if(toSeq - double(int(toSeq)) != 0)
		{
			resultLine.push_back(getSeqPoint(srcLine, toSeq));
		}

		return resultLine;
	}

	/**
	 * @brief 元ラインを目的ラインの区間で切断する
	 * @note 切断に失敗するのは、元ラインと目的ラインの区間が重なり合わない、あるいは方向が異なる場合
	 * @param srcLine [in] 元ライン
	 * @param dstLine [in] 目的ライン
	 * @param resultLine [out] 得られたライン
	 * @param fromMatch [out] 目的ラインの始点が元ラインに投影できたか
	 * @param toMatch [out] 目的ラインの終点が元ラインに投影できたか
	 * @param matchingRange [in] マッチング範囲
	 * @return 切断の可否
	 */
	template<typename T1, typename T2>
	bool cutLineByLine(
		const std::vector<T1>& srcLine,
		const std::vector<T2>& dstLine,
		std::vector<T1>& resultLine,
		bool& fromMatch,
		bool& toMatch,
		double matchingRange = DBL_MAX )
	{
		fromMatch = false;
		toMatch = false;

		T1 nearPoint = {};

		const auto& srcFromPoint = srcLine.front();
		const auto& srcToPoint   = srcLine.back();
		const auto& dstFromPoint = dstLine.front();
		const auto& dstToPoint   = dstLine.back();
		std::list<double> dstFromAngles = PointArrayUtil::getConnectAngles(dstLine, 0);
		std::list<double> dstToAngles   = PointArrayUtil::getConnectAngles(dstLine, dstLine.size()-1);

		// まず、目的ラインの端点から元のラインに点を投影してみる
		double dstFromSeq = getPointSeq(srcLine, dstFromPoint, nearPoint, dstFromAngles, matchingRange);
		double dstToSeq   = getPointSeq(srcLine, dstToPoint,   nearPoint, dstToAngles,   matchingRange);

		// 両端が投影できた場合。元のラインの方が目的ラインを包含しているケース
		if(dstFromSeq != DBL_MAX && dstToSeq != DBL_MAX)
		{
			// シーケンスの並びが逆になるなら、方向が違うということでfalse
			if(dstFromSeq >= dstToSeq)
				return false;

			resultLine = cutLine(srcLine, dstFromSeq, dstToSeq);
			return (getFarthestDistance(resultLine, dstLine) <= matchingRange);
		}

		// 両端が投影できないなら、目的ラインの方がFromTo方向に長いケースが考えられる
		if(dstFromSeq == DBL_MAX && dstToSeq == DBL_MAX)
		{
			// 元ラインの端点を目的ラインに投影する
			T2 nearPoint = {};
			std::list<double> srcFromAngles = PointArrayUtil::getConnectAngles(srcLine, 0);
			std::list<double> srcToAngles   = PointArrayUtil::getConnectAngles(srcLine, srcLine.size()-1);

			double srcFromSeq = getPointSeq(dstLine, srcFromPoint, nearPoint, srcFromAngles, matchingRange);
			double srcToSeq   = getPointSeq(dstLine, srcToPoint,   nearPoint, srcToAngles  , matchingRange);
			// 元ラインの両端が投影できないとだめ。方向が入れ替わってもダメ
			if(srcFromSeq == DBL_MAX || srcToSeq == DBL_MAX || srcFromSeq >= srcToSeq)
				return false;

			resultLine = srcLine;
			return (getFarthestDistance(srcLine, dstLine) <= matchingRange);
		}
		// 目的ラインの始点が投影できない場合
		if(dstFromSeq == DBL_MAX)
		{
			// 同じ方向を向いているなら、元ラインの始点が目的ラインに落とせるはず
			T2 nearPoint = {};
			std::list<double> srcFromAngles = PointArrayUtil::getConnectAngles(srcLine, 0);

			double srcFromSeq = getPointSeq(dstLine, srcFromPoint, nearPoint, srcFromAngles, matchingRange);
			if(srcFromSeq == DBL_MAX)
				return false;

			dstFromSeq = 0;
		}

		if(dstToSeq == DBL_MAX)
		{
			// 同じ方向を向いているなら、元ラインの終点が目的ラインに落とせるはず
			T2 nearPoint = {};
			std::list<double> srcToAngles   = PointArrayUtil::getConnectAngles(srcLine, srcLine.size()-1);

			double srcToSeq   = getPointSeq(dstLine, srcToPoint, nearPoint, srcToAngles, matchingRange);
			if(srcToSeq == DBL_MAX)
				return false;

			dstToSeq = (srcLine.size()-1);
		}
		if(dstFromSeq >= dstToSeq)
			return false;

		resultLine = cutLine(srcLine, dstFromSeq, dstToSeq);

		return (getFarthestDistance(resultLine, dstLine) <= matchingRange);
	}

	/**
	 * @brief 目的点列に対して、元点列が覆っているカバー率を計算する
	 * @note 平たく言えば、目的点列の端点から接続セグメントの法線を伸ばして、その足が元点列にぶつかる範囲のこと
	         目的点列の始終点の法線が、元点列にマッチング距離内でぶつかれば、カバー率1ということ
	 * @param srcLine [in] 元点列
	 * @param dstLine [in] 目的点列
	 * @param maxDist [in] 目的点列が元点列を覆っている範囲内で、両点列が最も遠ざかる距離
	 * @param matchingRange [in] マッチング距離
	 * @return カバー率(1で完全に目的点列が元点列を覆っている。0で全く範囲が合わない)
	 */
	template<typename T1, typename T2>
	double calcCoverRatio(
		const std::vector<T1>& srcLine,
		const std::vector<T2>& dstLine,
		double& maxDist,
		double matchingRange = DBL_MAX )
	{
		assert(!srcLine.empty() && !dstLine.empty());
		
		// 目的点列を元点列でカットしてみる。できなければカバー率0
		bool fromMatch = false, toMatch = false;
		std::vector<T1> cutDstLine;
		if(!cutLineByLine(dstLine, srcLine, cutDstLine, fromMatch, toMatch, matchingRange))
			return 0;

		// 目的点列とカットされた元点列の長さを求める
		double dstLength = getLength(dstLine);
		double cutLength = getLength(cutDstLine);

		// カットされた元点列の長さが0の場合はカバー率0(そういう状況はないと思われるが、0除算よけ)
		if(dstLength == 0)
			return 0;
			
		return cutLength / dstLength;
	}
}