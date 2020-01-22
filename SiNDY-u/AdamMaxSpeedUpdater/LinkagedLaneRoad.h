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

// SiNDYLib
#include <SiNDYLib/FieldMap.h>

/**
 *  @brief  車線リンクと道路リンクのペアの妥当度
 *          車線リンクに紐づく道路リンクが複数ある場合があり、
 *          その場合は最高速度の取得元として相応しいペアを1つ決める
 *          ここでは決められたペアの妥当性を定義する
 */
enum LinkageAccuracy
{
	kNoPair,			//!< 紐づく道路リンクがない
	kAccurate,			//!< ペアが最高速度の取得元として正確である
	kMostlyAccurate,	//!< ペアが最高速度の取得元として正確である(ただし取得元の候補が2つ以上あった)
	kInaccurate,		//!< ペアが最高速度の取得元として不正確な可能性がある
};

/**
*  @brief  車線リンクと道路リンクの紐付け情報管理クラス
*  @detail 車線リンクとそれに紐づく道路リンクとの対応管理を行う
*          車線リンクに複数の道路リンクが紐づく場合に、
*          最高速度の更新元としてどの道路リンクを選択するかの決定もこのクラスで行う
*/
class LinkagedLaneRoad
{
public:
	/**
	*  @brief   車線リンクと道路リンクの紐付き情報
	*  @detail  車線リンク 対 道路リンク = 1 対 多
	*/
	struct Record
	{
		IFeaturePtr m_laneLink;		//!< 車線リンク
		std::vector<IFeaturePtr> m_roadLinks;	//!< 車線リンクに紐づく道路リンク
	};
	typedef std::vector<Record>::iterator Iter;

public:
	/**
	*  @brief  コンストラクタ
	*  @param[in]  lane_fc        車線リンクフィーチャクラス
	*  @param[in]  road_fc        道路リンクフィーチャクラス
	*  @param[in]  rel_road_lane  道路リンク－車線リンク関連テーブル
	*/
	LinkagedLaneRoad(IFeatureClassPtr lane_fc, IFeatureClassPtr road_fc, ITablePtr rel_road_lane);
	virtual ~LinkagedLaneRoad() {};

public:
	Iter begin() { return m_data.begin(); }
	Iter end() { return m_data.end(); }

	/**
	*  @brief      データ読み込み
	*  @detail     引数で指定した車線リンクフィーチャに紐づく道路リンクを読み込み、
	*              内部で保持するコンテナに"車線リンク->道路リンク"の対応
	*  @param[in]  lane_links  車線リンクフィーチャ群
	*  @retval true  読み込み成功
	*  @retval false 読み込み失敗
	*/
	bool read(const std::vector<IFeaturePtr>& lane_links);

	/**
	*  @brief   車線リンクに紐づく道路リンクのうち優先度の高い道路リンクを選んで返す
	*  @detail  両リンクの種別から最高速度の更新元としての優先度の高い道路リンクを決める
	*  @param[in]  record 車線リンク1つ分の道路リンク紐付き情報
	*  @param[out] lane_link 車線リンク
	*  @param[out] road_link 道路リンク
	*  @return  最高速度更新における紐付けの妥当度
	*/
	LinkageAccuracy getHigherPriorityPairLink(const Record& record, IFeaturePtr& lane_link, IFeaturePtr& road_link) const;

private:
	/**
	*  @brief   車線リンクと道路リンクの組み合わせの優先度を返す
	*  @detail  両リンクの種別から最高速度の更新元としての優先度の高い組み合わせであるかを判断する
	*           数字が小さいほど優先度は高い
	*  @param[in] lane_link  車線リンク
	*  @param[in] road_link  道路リンク
	*  @retval  車線リンクと道路リンクの組み合わせの優先度
	*/
	long getPriority(IFeaturePtr lane_link, IFeaturePtr road_link) const;

	/**
	*  @brief   指定した車線リンクの属性が最高速度を更新するにあたって正確性の低いものであるかを返す
	*  @paran[in]  lane_link  車線リンク
	*  @retval true   正確性が低い
	*  @retval false  正確性が低くない
	*/
	bool isInaccuracyPair(IFeaturePtr lane_link) const;

private:
	IFeatureClassPtr m_roadLinkFc;		//!< 道路リンクフィーチャクラス
	ITablePtr        m_relRoadLaneT;	//!< 道路リンク-車線リンク関連テーブル

	sindy::CFieldMap m_relRoadLaneFM;	//!< 道路リンク－車線リンク関連テーブルのフィールドマップ
	sindy::CFieldMap m_lanelinkFM;		//!< 車線リンクフィーチャクラスのフィールドマップ
	sindy::CFieldMap m_roadlinkFM;		//!< 道路リンクフィーチャクラスのフィールドマップ

	std::vector<Record> m_data;		//!< 車線リンクと道路リンクの紐付き情報
};

