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

// �_��������֐��Q�B��r�I�}�b�`���O�֌W�������B
namespace PointArrayUtil
{
	extern double threshold; // ���ԍ��W����̂��߂̋��e�l
	extern double angleThreshold;

	/**
	 * @brief 1�Ԗڂ̓_��3�Ԗڂ̊Ԃ�2�Ԗڂ̓_�����݂��邩�𔻒肷��
	 * @note �v�Z�덷���l�����āA1�Ԗڂ̓_��3�Ԗڂ̓_��X���AY���ɂقڕ��s�ȏꍇ�͔����Ȍ덷�����e����
	 * @param point1 [in] 1�Ԗڂ̓_
	 * @param point2 [in] 2�Ԗڂ̓_
	 * @param point3 [in] 3�Ԗڂ̓_
	 * @retval true 2�Ԗڂ̓_��1�Ԗڂ�3�Ԗڂ̓_�̊Ԃɑ��݂���
	 * @retval false 2�Ԗڂ̓_��1�Ԗڂ�3�Ԗڂ̓_�̊Ԃɑ��݂��Ȃ�
	 */
	template<typename T1, typename T2>
	bool isBetween(
		const T1& point1,
		const T2& point2,
		const T1& point3)
	{
		// �|�C���g1��3��X���W�̍������e�l�ȓ��ł���΁A
		if(fabs(point1.X-point3.X)<threshold)
		{
			// X���W�͒��Ԓl��苖�e�l�̘����܂ŋ��e����
			if(fabs((point1.X+point3.X)*0.5-point2.X)>threshold)
				return false;
		}
		else if((point1.X-point2.X)*(point3.X-point2.X) > 0)
		{
			// ���e�l�ȓ��łȂ���΁A���W�̍��������Ă͂ݏo������
			return false;
		}
		// �|�C���g1��3��Y���W�̍������e�l�ȓ��ł���΁A
		if(fabs(point1.Y-point3.Y)<threshold)
		{
			// Y���W�͒��Ԓl��苖�e�l�̘����܂ŋ��e����
			if(fabs((point1.Y+point3.Y)*0.5-point2.Y)>threshold)
				return false;
		}
		else if((point1.Y-point2.Y)*(point3.Y-point2.Y) > 0)
		{
			// ���e�l�ȓ��łȂ���΁A���W�̍��������Ă͂ݏo������
			return false;
		}

		return true;
	}

	/**
	 * @brief 2�_�Ԃ̋������v�Z����
	 * @param point1 [in] �|�C���g1
	 * @param point2 [in] �|�C���g2
	 * @return 2�_�Ԃ̋���(m)
	 */
	template<typename T1, typename T2>
	double getDistance(
		const T1& point1,
		const T2& point2)
	{
		return g_cnv.GetDist(point1.X, point1.Y, point2.X, point2.Y);
	}

	/**
	 * @brief �_��̒������v�Z����
	 * @param points [in] �_��
	 * @return �_��̒���(m)
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
	 * @brief �ړI�̓_�ƃZ�O�����g�̋��������߂�
	 * @param point1 [in] �Z�O�����g�̎n�_
	 * @param point2 [in] �Z�O�����g�̏I�_
	 * @param targetPoint [in] �ړI�̓_
	 * @return �Z�O�����g�ƖړI�̓_�̋���(�P��:m)
	 */
	template<typename T1, typename T2>
	double getSegmentDist(const T1& point1, const T1& point2, const T2& targetPoint)
	{
		double a = point2.Y - point1.Y;
		double b = point1.X - point2.X;
		double c = point2.X*point1.Y - point1.X*point2.Y;

		double D = a*a + b*b;
		// D��0�Ƃ������Ƃ̓|�C���g1,2������_�ł���Ƃ�������
		if(D==0)
			return getDistance(point1, targetPoint);
		
		// �|�C���g1-2�̃��C����ōł��Ώۓ_�ɋ߂��|�C���g
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
	 * @brief �����������߂�
	 * @note ���ʂ�-�΂����傫���Ζ����ŕԂ�
	 * @param angle1 [in] ����1
	 * @param angle2 [in] ����2
	 * @return ������(rad)
	 */
	double getAngleDiff(
		double angle1,
		double angle2);

	/**
	 * @brief 2�̕������X�g�ɋߎ������p�x�����邩
	 * @note �p�x���e�l�ȓ��ŋߎ������΂�����ΐ^�A�Ȃ���΋U
	 * @param angles1 [in] �������X�g1
	 * @param angles2 [in] �������X�g2
	 * @retval true ���X�g1,2�̊Ԃɋߎ����������̑΂�����
	 * @retval false ���X�g1,2�̊Ԃɂ͋ߎ����������̑΂��Ȃ�
	 */
	bool hasSimilarAngle(
		const std::list<double>& angles1,
		const std::list<double>& angles2);

	/**
	 * @brief �_��̐ڑ��p�x�Q��Ԃ�
	 * @note �������ɍ��킹��K�v�͂Ȃ��̂ŁA�ܓx�o�x��2�����ʂƂ��čl����
	         �ΏۃC���f�b�N�X���_�Q�͈̔͂𒴂�����󃊃X�g��Ԃ�
			 �ΏۃC���f�b�N�X��0���߂̏ꍇ�͑O�̓_����Ώۂ̓_�܂ł̕�����ǉ�
			 �ΏۃC���f�b�N�X�������ȊO�̏ꍇ�͑Ώۂ̓_���玟�̓_�܂ł̕�����ǉ�
	 * @param points [in] �_�Q
	 * @param index [in] �Ώۂ̓_�̃C���f�b�N�X
	 */
	template<typename T1>
	std::list<double> getConnectAngles(
		const std::vector<T1>& points,
		unsigned int index
		)
	{
		std::list<double> result;

		// �͈͂��K���łȂ���΁A��̃��X�g��Ԃ�
		if(index < 0 || index >= points.size())
			return result;

		// �ΏۃC���f�b�N�X��0���傫���ꍇ�́A�ЂƂO�̃C���f�b�N�X����ΏۃC���f�b�N�X�Ɍ�����������ǉ�
		if(0 < index)
			result.push_back(atan2(points[index].Y - points[index-1].Y, points[index].X - points[index-1].X));

		// �ΏۃC���f�b�N�X�������łȂ���΁A�ΏۃC���f�b�N�X���玟�̃C���f�b�N�X�Ɍ�����������ǉ�
		if(index < points.size()-1)
			result.push_back(atan2(points[index+1].Y - points[index].Y, points[index+1].X - points[index].X));

		return result;
	}

	/**
	 * @brief 2�̓_�񂪍ł��������鋗�����v�Z����
	 * @param points1 [in] �_��1
	 * @param points2 [in] �_��2
	 * @return 2�_�񂪍ł��������鋗��
	 */
	template<typename T1, typename T2>
	double getFarthestDistance(
		const std::vector<T1>& points1,
		const std::vector<T2>& points2)
	{
		double maxDist = -DBL_MAX;
		int segmentNum = points2.size()-1;

		// ���C��1�̊e�_�ƃ��C��2�Z�O�����g�̋����`�F�b�N
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
	 * @brief 2�_�̊Ԃ̔C�ӂ̈ʒu�̓_�����߂�
	 * @param point1 [in] �|�C���g1
	 * @param point2 [in] �|�C���g2
	 * @param ratio [in] ����(0�Ń|�C���g1�Ɠ����B1�Ń|�C���g2�Ɠ����B0.5�Œ��ԓ_)
	 * @return �����ɉ��������W
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
	 * @brief �ړI�̓_���炠��Z�O�����g��ʂ钼���ɓ��e�����_�̍��W�𓾂�
	 * @note �v�͒�����̓_�ŖړI�̓_�ɍł��߂����W�𓾂�B
	 * @note ���e�_�ɂ�Z�l���t���Ȃ��̂Œ���
	 * @param targetPoint [in] �ړI�̓_
	 * @param fromPoint [in] �Z�O�����g�̎n�_
	 * @param toPoint [in] �Z�O�����g�̏I�_
	 * @param projPoint [out] ���e�_
	 * @retval -1 ���e�_�̓Z�O�����g�̎n�_�̌��������ɑ��݂���
	 * @retval  0 ���e�_�̓Z�O�����g�̒��Ԃɑ��݂���
	 * @retval  1 ���e�_�̓Z�O�����g�̏I�_�̌��������ɑ��݂���
	 */
	template<typename T>
	int getProjectPoint(
		const T& targetPoint,
		const WKSPoint& fromPoint,
		const WKSPoint& toPoint,
		WKSPoint& projPoint)
	{
		// �����Ă���Z�O�����g�̎����v�Z(ax+by+c=0�̌W��)
		double a = toPoint.Y - fromPoint.Y;
		double b = fromPoint.X - toPoint.X;
		double c = toPoint.X * fromPoint.Y - fromPoint.X * toPoint.Y;

		// a*a + b*b��0�̏ꍇ�A0���Z���N���邽�ߌv�Z�s�\(�Z�O�����g��2�_��������W�̏ꍇ�ɋN����)
		if( a*a + b*b == 0 ){
			return false;
		}
		// a��0�A���Ȃ킿�Z�O�����g��X���ɕ��s�ł���Ƃ��́A��_��X���W�͑Ώۃ|�C���g��X���W�AY���W�̓Z�O�����g��Y���W
		if(a == 0){
			projPoint.X = targetPoint.X;
			projPoint.Y = fromPoint.Y;
		}
		// b��0�A���Ȃ킿�Z�O�����g��Y���ɕ��s�ł���Ƃ��́A��_��X���W�̓Z�O�����g��X���W�AY���W�͑Ώۃ|�C���g��Y���W
		else if(b == 0){
			projPoint.X = fromPoint.X;
			projPoint.Y = targetPoint.Y;
		}
		// �ǂ���ɂ��Y�����Ȃ��ꍇ�́A�Z�O�����g�̎������_�����߂�
		else{
			projPoint.X = ( -a*b*targetPoint.Y + b*b*targetPoint.X - a*c ) / ( a*a + b*b );
			projPoint.Y = ( -a*b*targetPoint.X + a*a*targetPoint.Y - b*c ) / ( a*a + b*b );
		}
		// �Ō��Z�l��ݒ肷��
		double fromDist = getDistance(fromPoint, projPoint);
		double toDist   = getDistance(toPoint,   projPoint);

		// ��_��X,Y���W���Z�O�����g�ɔ[�܂��Ă���ꍇ�́Atrue
		if(isBetween(fromPoint, projPoint, toPoint))
		{
			return 0;
		}
		// ��_��From���A���Ȃ킿��_��From��To�̈ʒu�֌W�Ȃ�-1��Ԃ�
		if(isBetween(projPoint, fromPoint, toPoint)){
			return -1;
		}
		else{
			return 1;
		}
	}

	/**
	 * @brief �_��ɑ΂��đΏۂ̃|�C���g���ǂ̈ʒu�Ɉʒu���邩�����߂�
	 * @note �Z�O�����g�̕����Q�́A��ł���΍l�����Ȃ�
	 * @param pointVec [in] �_��
	 * @param targetPoint [in] �Ώۂ̃|�C���g
	 * @param nearPoint [in] �_���őΏۂ̃|�C���g�ɍł��߂��|�C���g
	 * @param connectAngles [in] �Ώۂ̃|�C���g�ɐڑ�����Z�O�����g�̕����Q(�t�����͈قȂ�Ƃ���)
	                             SIP�̃}�b�`���O�ł͌����_�l�����Ȃ����߁A1��2�̂͂�
	 * @param matchingRange [in] �}�b�`���O�͈̔�(�ł��߂��_�����̋����ȏ�Ȃ�DBL_MAX�Ԃ�)
	 * @param useProximity [in] �ߎ��̗L��(true���ݒ肳��Ă���ƁA�ł��߂��|�C���g���_��̒[�̃Z�O�����g���͂ݏo���Ă�OK)
	 * @return �V�[�P���X(�ł��߂��|�C���g���_��̑�3�_�Ƒ�4�_��3:7�̈ʒu�ɂ���΁A3.3�ƕ\�L)
	 * @retval DBL_MAX �Ώۂ̃|�C���g�͋K��ȏ�ɗ���Ă��邽�ߌv�Z�s��
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
	 * @brief �_��̃V�[�P���X�ɉ��������W���擾����
	 * @param srcLine [in] ���_��
	 * @param seq [in] �V�[�P���X(�_��̑�3�_�Ƒ�4�_��3:7�̈ʒu�ɂ���΁A3.3�ƕ\�L����悤�Ȓl)
	 * @return �V�[�P���X�ɉ��������W
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
	 * @brief �_����V�[�P���X�ŃJ�b�g����
	 * @note �V�[�P���X�Ƃ́A�_��̑�3�_�Ƒ�4�_��3:7�̈ʒu�ɂ���΁A3.3�ƕ\�L����悤�Ȓl
	 * @param srcLine [in] ���_��
	 * @param fromSeq [in] �n�_�V�[�P���X
	 * @param toSeq [in] �I�_�V�[�P���X
	 */
	template<typename T>
	std::vector<T> cutLine(
		const std::vector<T>& srcLine,
		double fromSeq,
		double toSeq)
	{
		assert(0 <= fromSeq && toSeq <= double(srcLine.size()) && fromSeq < toSeq);
		std::vector<T> resultLine;

		// �ЂƂ܂��͈͓����R�s�[(std::copy�͎w�肳�ꂽ�I�[��1��O�܂ł��R�s�[����̂�toSeq+1)
		copy(srcLine.begin()+int(fromSeq), srcLine.begin()+int(toSeq)+1, back_inserter(resultLine));
		
		// From�V�[�P���X�������l�łȂ���΁A�ŏ��̓_��u��������
		// ���̑O�ɃR�s�[�����ŏ��̓_�́A�[���؂�̂Ăł��ړ��Ă̓_���O�̂͂�������B
		if(fromSeq - double(int(fromSeq)) != 0)
		{
			resultLine[0] = getSeqPoint(srcLine, fromSeq);
		}

		// To�V�[�P���X�������l�łȂ���΁A�����ɓ_��ǉ�����
		// ���̑O�ɃR�s�[�����Ō�̓_�́A�[���؎̂Ăł��ړ��Ă̓_����O�Ő؂�Ă���͂�������B
		if(toSeq - double(int(toSeq)) != 0)
		{
			resultLine.push_back(getSeqPoint(srcLine, toSeq));
		}

		return resultLine;
	}

	/**
	 * @brief �����C����ړI���C���̋�ԂŐؒf����
	 * @note �ؒf�Ɏ��s����̂́A�����C���ƖړI���C���̋�Ԃ��d�Ȃ荇��Ȃ��A���邢�͕������قȂ�ꍇ
	 * @param srcLine [in] �����C��
	 * @param dstLine [in] �ړI���C��
	 * @param resultLine [out] ����ꂽ���C��
	 * @param fromMatch [out] �ړI���C���̎n�_�������C���ɓ��e�ł�����
	 * @param toMatch [out] �ړI���C���̏I�_�������C���ɓ��e�ł�����
	 * @param matchingRange [in] �}�b�`���O�͈�
	 * @return �ؒf�̉�
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

		// �܂��A�ړI���C���̒[�_���猳�̃��C���ɓ_�𓊉e���Ă݂�
		double dstFromSeq = getPointSeq(srcLine, dstFromPoint, nearPoint, dstFromAngles, matchingRange);
		double dstToSeq   = getPointSeq(srcLine, dstToPoint,   nearPoint, dstToAngles,   matchingRange);

		// ���[�����e�ł����ꍇ�B���̃��C���̕����ړI���C�����܂��Ă���P�[�X
		if(dstFromSeq != DBL_MAX && dstToSeq != DBL_MAX)
		{
			// �V�[�P���X�̕��т��t�ɂȂ�Ȃ�A�������Ⴄ�Ƃ������Ƃ�false
			if(dstFromSeq >= dstToSeq)
				return false;

			resultLine = cutLine(srcLine, dstFromSeq, dstToSeq);
			return (getFarthestDistance(resultLine, dstLine) <= matchingRange);
		}

		// ���[�����e�ł��Ȃ��Ȃ�A�ړI���C���̕���FromTo�����ɒ����P�[�X���l������
		if(dstFromSeq == DBL_MAX && dstToSeq == DBL_MAX)
		{
			// �����C���̒[�_��ړI���C���ɓ��e����
			T2 nearPoint = {};
			std::list<double> srcFromAngles = PointArrayUtil::getConnectAngles(srcLine, 0);
			std::list<double> srcToAngles   = PointArrayUtil::getConnectAngles(srcLine, srcLine.size()-1);

			double srcFromSeq = getPointSeq(dstLine, srcFromPoint, nearPoint, srcFromAngles, matchingRange);
			double srcToSeq   = getPointSeq(dstLine, srcToPoint,   nearPoint, srcToAngles  , matchingRange);
			// �����C���̗��[�����e�ł��Ȃ��Ƃ��߁B����������ւ���Ă��_��
			if(srcFromSeq == DBL_MAX || srcToSeq == DBL_MAX || srcFromSeq >= srcToSeq)
				return false;

			resultLine = srcLine;
			return (getFarthestDistance(srcLine, dstLine) <= matchingRange);
		}
		// �ړI���C���̎n�_�����e�ł��Ȃ��ꍇ
		if(dstFromSeq == DBL_MAX)
		{
			// ���������������Ă���Ȃ�A�����C���̎n�_���ړI���C���ɗ��Ƃ���͂�
			T2 nearPoint = {};
			std::list<double> srcFromAngles = PointArrayUtil::getConnectAngles(srcLine, 0);

			double srcFromSeq = getPointSeq(dstLine, srcFromPoint, nearPoint, srcFromAngles, matchingRange);
			if(srcFromSeq == DBL_MAX)
				return false;

			dstFromSeq = 0;
		}

		if(dstToSeq == DBL_MAX)
		{
			// ���������������Ă���Ȃ�A�����C���̏I�_���ړI���C���ɗ��Ƃ���͂�
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
	 * @brief �ړI�_��ɑ΂��āA���_�񂪕����Ă���J�o�[�����v�Z����
	 * @note �����������΁A�ړI�_��̒[�_����ڑ��Z�O�����g�̖@����L�΂��āA���̑������_��ɂԂ���͈͂̂���
	         �ړI�_��̎n�I�_�̖@�����A���_��Ƀ}�b�`���O�������łԂ���΁A�J�o�[��1�Ƃ�������
	 * @param srcLine [in] ���_��
	 * @param dstLine [in] �ړI�_��
	 * @param maxDist [in] �ړI�_�񂪌��_��𕢂��Ă���͈͓��ŁA���_�񂪍ł��������鋗��
	 * @param matchingRange [in] �}�b�`���O����
	 * @return �J�o�[��(1�Ŋ��S�ɖړI�_�񂪌��_��𕢂��Ă���B0�őS���͈͂�����Ȃ�)
	 */
	template<typename T1, typename T2>
	double calcCoverRatio(
		const std::vector<T1>& srcLine,
		const std::vector<T2>& dstLine,
		double& maxDist,
		double matchingRange = DBL_MAX )
	{
		assert(!srcLine.empty() && !dstLine.empty());
		
		// �ړI�_������_��ŃJ�b�g���Ă݂�B�ł��Ȃ���΃J�o�[��0
		bool fromMatch = false, toMatch = false;
		std::vector<T1> cutDstLine;
		if(!cutLineByLine(dstLine, srcLine, cutDstLine, fromMatch, toMatch, matchingRange))
			return 0;

		// �ړI�_��ƃJ�b�g���ꂽ���_��̒��������߂�
		double dstLength = getLength(dstLine);
		double cutLength = getLength(cutDstLine);

		// �J�b�g���ꂽ���_��̒�����0�̏ꍇ�̓J�o�[��0(���������󋵂͂Ȃ��Ǝv���邪�A0���Z�悯)
		if(dstLength == 0)
			return 0;
			
		return cutLength / dstLength;
	}
}