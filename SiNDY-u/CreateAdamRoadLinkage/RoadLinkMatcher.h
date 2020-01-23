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

#include <SiNDYLib/FieldMap.h>

/**
 * @brief WKSPointの<演算子をオーバーロード
 * @param lhs [in] 比較対象のポイント(左辺)
 * @param rhs [in] 比較対象のポイント(右辺)
 * @retval true 左辺の方が小さい
 * @retval false 左辺の方が小さくない
 */
inline bool operator < (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X != rhs.X ? lhs.X < rhs.X : lhs.Y < rhs.Y;
}

/**
 * @brief WKSPointの!=演算子をオーバーロード
 * @param lhs [in] 比較対象のポイント(左辺)
 * @param rhs [in] 比較対象のポイント(右辺)
 * @retval true 両辺は異なる
 * @retval false 両辺は等しい
 */
inline bool operator != (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X != rhs.X || lhs.Y != rhs.Y;
}

/**
 * @brief WKSPointの!=演算子をオーバーロード
 * @param lhs [in] 比較対象のポイント(左辺)
 * @param rhs [in] 比較対象のポイント(右辺)
 * @retval true 両辺は等しい
 * @retval false 両辺は異なる
 */
inline bool operator == (const WKSPoint& lhs, const WKSPoint& rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

typedef std::vector<WKSPoint> WKSPoints; //!< 点列の定義

class RoadLinkMatcher
{
public:

	/**
	 * @brief 方向
	 */
	enum Direction{
		kForword,	//!< 正方向
		kReverse,	//!< 逆方向
		kBoth		//!< 両方向
	};

public:
	/**
	 * @brief コンストラクタ
	 */
	RoadLinkMatcher(const ITablePtr& roadlink_t, double matching_range);

	/**
	 * @brief デストラクタ
	 */
	virtual ~RoadLinkMatcher(){}

public:
	/**
	 * @brief 指定フィーチャ(src_feature)と位置的に一番マッチものを指定フィーチャ群(dest_features)から取得する
	 * @note  マッチするフィーチャが見つからない場合は nullptr を返す
	 * @param[in]  src_feature    マッチング元のフィーチャ
	 * @param[in]  dest_features  マッチング候補のフィーチャ群
	 * @param[out] is_reverse     マッチングされた道路リンクが逆方向規制を持っているかどうか
	 * @param[in]  as_src_is_reverse    マッチング元のフィーチャを逆向きとしてマッチング処理を実施する
	 */
	IFeaturePtr getMatchLink(const IFeaturePtr& src_feature, const std::vector<IFeaturePtr>& dest_features, bool& is_reverse, bool as_src_is_reverse = false);

private:
	/**
	 * @brief フィーチャの構成点をWKSPointのベクタにして返す
	 * @note  フィーチャはポリラインであることを前提とする
	 * @param[in] feature  元フィーチャ
	 * @param[in] reverse  構成点の並び順を逆にするか
	 */
	WKSPoints featureToWKSPoints(const IFeaturePtr& feature, bool reverse = false);

	/**
	 * @brief 点列のマッチング率を算出する
	 * @param[in] src_points 道路リンクの点列
	 * @param[in] dest_points  SIPリンク群の点列
	 * @retval DBL_MAX     マッチング失敗
	 * @retval DBL_MAX以外 マッチング率
	 */
	double getMatchingRate(const WKSPoints& src_points, const WKSPoints& dest_points);

	/**
	 * @brief 点列のマッチングレベルを算出する
	 * @note マッチングレベルは両点列が最も遠ざかる距離。0でSIPリンクと道路リンクの重なる領域が完全一致
	 * @param[in] linkPoints 道路リンクの点列
	 * @param[in] shpPoints  SIPリンク群の点列
	 * @param[in] coverRatio SIPリンク列が道路リンクを覆っているカバー率
	 */
	//double getMatchLevel( const WKSPoints& linkPoints, const WKSPoints& shpPoints, double coverRatio);

	/**
	 * @brief 道路リンクフィーチャがどちら向きかを規制から判断する
	 * @note  引数のfeature は道路リンクであることを前提とする
	 * @param[in] feature  道路リンクフィーチャ
	 * @retval true  逆方向規制を持っている
	 * @retval false 逆方向規制を持っていない
	 */
	Direction getDirection( const IFeaturePtr& feature );

private:
	double m_MatchingRange;			//!< マッチング範囲

	sindy::CFieldMap m_RoadLinkFM;	//!< 道路リンクのフィールドマップ
};

