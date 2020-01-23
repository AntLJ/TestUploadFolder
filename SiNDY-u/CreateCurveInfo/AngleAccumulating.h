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

#include "CurveInfoData.h"

namespace curve_info
{
	/**
	 * @brief 角度の累積に関する概念
	 * @note	セグメント列を順に辿る過程で得られる「各セグメント対のなす角の角度」の情報群を扱う。
	 */
	namespace angle_accumulating
	{
		/**
		 * @brief カーブの方向
		 */
		namespace  curve_direction
		{
			enum ECode
			{
				straight,	///< 直進
				left,		///< 左
				right,		///< 右
				both,		///< 左右両方
			};
		}

		/**
		 * @brief 角度累積状態
		 */
		struct Accumulating
		{
			const SingleRoad& m_rSingleRoad;	///< 対象一本道
			SegmentIt m_BeginSegment;			///< 累積開始セグメント
			SegmentIt m_EndSegment;				///< 累積終了セグメント
			double m_Angle;						///< 累積角度

			const double m_CurveCertifyAngle;			///< カーブ認定角度（度）

			/**
			 * @brief コンストラクタ
			 * @param[in]	crSingleRoad	対象一本道
			 */
			Accumulating(
				const SingleRoad& crSingleRoad,
				double cDirectionAngle
			) :
				m_rSingleRoad( crSingleRoad ),
				m_EndSegment( crSingleRoad.end() ),
				m_CurveCertifyAngle( cDirectionAngle )
			{
				reset();
			}

			/**
			 * @brief 角度の累積が開始済みであるか否かを判定する
			 * @return	判定結果
			 */
			bool hasBegun() const
			{
				return m_BeginSegment != m_rSingleRoad.end();
			}

			/**
			 * @brief 角度の累積が開始したセグメントを記録する
			 * @param[in]	citSegment	開始セグメント
			 */
			void beginAt( const SegmentIt citSegment )
			{
				m_BeginSegment = citSegment;
			}

			/**
			 * @brief 開始セグメントを初期状態に戻す
			 */
			void resetBegin()
			{
				m_BeginSegment = m_rSingleRoad.end();
			}

			/**
			 * @brief 角度の累積が終了済みであるか否かを判定する
			 * @return	判定結果
			 */
			bool hasEnded() const
			{
				return m_EndSegment != m_rSingleRoad.end();
			}

			/**
			 * @brief 角度の累積が終了したセグメントを記録する
			 * @param[in]	citSegment	終了セグメント
			 */
			void endAt( SegmentIt citSegment )
			{
				m_EndSegment = citSegment;
			}

			/**
			 * @brief 終了セグメントを初期状態に戻す
			 */
			void resetEnd()
			{
				m_EndSegment = m_rSingleRoad.end();
			}

			/**
			 * @brief 角度を足す
			 * @param[in]	cAngle	角度
			 */
			void addAngle( double cAngle )
			{
				m_Angle += fabs( cAngle );
			}

			/**
			 * @brief 初期状態に戻す
			 */
			void reset()
			{
				m_BeginSegment = m_rSingleRoad.end();
				m_EndSegment = m_rSingleRoad.end();
				m_Angle = 0;
			}

			/**
			 * @brief 既にカーブと認定されているか否かを判定する
			 * @return	判定結果
			 */
			bool isCurved()
			{
				return m_CurveCertifyAngle <= m_Angle;
			}
		};

		/**
		 * @brief 左右の角度累積状態
		 */
		struct LRAccumulating
		{
			Accumulating m_LeftAccumulating;	///< 左側角度累積状態
			Accumulating m_RightAccumulating;	///< 右側角度累積状態

			const SingleRoad& m_rSingleRoad;	///< 対象一本道

			double m_StraightAngle;				///< 直線と見なす角度
			double m_CurveCertifyAngle;			///< カーブ認定角度（度）

			ISpatialReferencePtr m_pSpRef;	///< 使用する空間参照

			// TODO: 構造体にする？ "InflectionPointInfo"とか。
			SegmentIt m_itInflectionSegment;	///< 変曲点セグメント（その上に変曲点が存在するセグメント）
			IPointPtr m_pInflectionPoint;		///< 最後に見つかった変曲点（未発見ならNULL）

			curve_direction::ECode m_CurrentDirection;	///< 現在どちらに曲がっているか、もしくは直線か

			/**
			 * @brief コンストラクタ
			 * @param[in]	cSingleRoad		対象一本道
			 * @param[in]	cStraightAngle	直線と見なす角度
			 * @param[in]	cDirectionAngle	方向を決定する角度（度）
			 * @param[in]	cpSpRef			使用する空間参照
			 */
			LRAccumulating(
				const SingleRoad& cSingleRoad,
				double cStraightAngle,
				double cDirectionAngle,
				ISpatialReferencePtr cpSpRef
			) : 
				m_LeftAccumulating( cSingleRoad, cDirectionAngle ),
				m_RightAccumulating( cSingleRoad, cDirectionAngle ),
				m_rSingleRoad( cSingleRoad ),
				m_StraightAngle( cStraightAngle ),
				m_CurveCertifyAngle( cDirectionAngle ),
				m_pSpRef( cpSpRef ),
				m_CurrentDirection( curve_direction::straight )
			{
				reset();
			}

			/**
			 * @brief 角度累積状態を更新
			 * @param[in]	cAngle				角度
			 * @param[in]	citCurrentSegment	着目中のセグメント
			 */
			void update( double cAngle, SegmentIt citCurrentSegment );

			/**
			 * @brief 角度累積状態を更新（左右の共通部分）
			 * @param[in]	cAngle				角度
			 * @param[in]	citCurrentSegment	着目中のセグメント
			 * @param[in]	cDirection			方向
			 */
			void update( const double cAngle, const SegmentIt citCurrentSegment, const curve_direction::ECode cDirection );

			/**
			 * @brief 角度累積状態を初期状態に戻す
			 */
			void reset()
			{
				m_LeftAccumulating.reset();
				m_RightAccumulating.reset();
				m_itInflectionSegment = m_rSingleRoad.end();
				m_pInflectionPoint = NULL;
			}

			/**
			 * @brief 角度が「カーブ認定角度」以上に累積している方向を返す
			 * @note	通常、bothにはならない想定。
			 * @return 方向
			 */
			curve_direction::ECode direction()
			{
				bool left = m_CurveCertifyAngle < m_LeftAccumulating.m_Angle;
				bool right = m_CurveCertifyAngle < m_RightAccumulating.m_Angle;

				if      (left && right) { return curve_direction::both; }
				else if (left)          { return curve_direction::left; }
				else if (right)         { return curve_direction::right; }
				else                    { return curve_direction::straight; }
			}
		};
	} // namespace angle_accumulating
} // namespace curve_info
