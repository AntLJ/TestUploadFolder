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

#include "Arguments.h"
#include "ChildVerEditWorkspace.h"

class LinkagedLaneRoad;

/**
 * @brief   メインクラス
 */
class CAdamMaxSpeedUpdater
{
public:
	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	explicit CAdamMaxSpeedUpdater( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CAdamMaxSpeedUpdater(){};

public:
	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:
	/**
	* @brief   ログ初期化
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool initLog() const;

	/**
	* @brief   テーブルのオープン
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool openTable();

	/**
	* @brief   実処理
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool run();

	/**
	* @brief   車線リンクの制限速度コード（最大）を更新する
	* @param[in]  linkagedLinks  車線リンクと道路リンクの関連情報群
	* @param[in]  basemesh_no    ベースメッシュ番号(ログ出力用)
	* @retval true  更新成功
	* @retval false 更新失敗
	*/
	bool updateMaxSpeed(LinkagedLaneRoad& linkagedLinks, long basemesh_no);

	/**
	* @brief   車線リンクと道路リンクの最高速度を比較し結果を出力する
	* @param[in]  linkagedLinks  車線リンクと道路リンクの関連情報群
	* @param[in]  basemesh_no    ベースメッシュ番号(ログ出力用)
	*/
	void compareMaxSpeed(LinkagedLaneRoad& linkagedLinks, long basemesh_no);

	/**
	* @brief   車線リンクと道路リンクの関連情報を読み込む
	* @param[in]  basemesh_no          ベースメッシュ番号(ログ出力用)
	* @param[in]  where_clause         車線リンク検索条件
	* @param[in]  exclude_group_code   車線リンク除外対象のグループ種別
	* @param[in, out] linkaged_links   車線リンクと道路リンクの関連情報
	* @retval true  読み込み成功
	* @retval false 読み込み失敗
	*/
	bool readLinkagedLaneRoad(
		long basemesh_no, const CString& lane_where_clause, const std::list<long>& exclude_group_code, 
		LinkagedLaneRoad& linkaged_links
		);

	/**
	* @brief   道路リンクフィーチャを指定し、更新する車線リンクの制限速度コード(最大)の値を取得する
	* @param[in]  road_link  道路リンクフィーチャ
	* @return  制限速度コード(最大)の値
	*/
	long getUpdateMaxSpeedCode(const IFeaturePtr road_link) const;

	/**
	* @brief   車線リンクコンテナから指定グループ種別のリンクを除外する
	* @param[in]       exclude_codes  除外対象のグループ種別
	* @param[in, out]  lane_links     車線リンク群
	* @retval true  読み込み成功
	* @retval false 読み込み失敗
	*/
	bool excludeByGroupCode(const std::list<long>& exclude_codes, std::vector<IFeaturePtr>& lane_links) const;

	/**
	* @brief   車線リンク::制限速度コード(最大) と道路リンク::最高速度コード を比較する
	* @param[in]  lane_speed_c  車線リンク::制限速度コード(最大) 
	* @param[in]  road_speed_c  道路リンク::最高速度コード 
	* @retval  true  同じ速度である
	* @retval  false 異なる速度である
	*/
	bool isEquallMaxSpeed(long lane_speed_c, long road_speed_c) const;

	/**
	* @brief   編集開始
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool startEdit();

	/**
	* @brief   編集終了
	*/
	void stopEdit();

	/**
	* @brief   編集中断
	*/
	void abortEdit();

	/**
	* @brief   後処理
	* @param [in] isSucceeded 正常終了したか否か
	*/
	void finish(bool isSucceeded) const;

	/**
	* @brief 子バージョン名を返す
	* @return 子バージョン名
	*/
	CString getChildVersionName() const;

private:
	Arguments m_args;      //!< 引数

	ChildVerEditWorkspace m_workspace;		//!< 編集セッション制御

	ITablePtr m_laneLinkFc;					//!< 車線リンクテーブル
	ITablePtr m_roadLinkFc;					//!< 道路リンクテーブル
	ITablePtr m_basemeshTable;				//!< ベースメッシュテーブル
	ITablePtr m_relRoadLinkLaneLinkT;		//!< 道路リンク－車線リンク関連テーブル
	ITablePtr m_laneLinkGroupT;				//!< 車線リンクグループテーブル

	sindy::CFieldMap m_laneLinkFM;		//!< 車線リンクのフィールドマップ
	sindy::CFieldMap m_roadLinkFM;		//!< 道路リンクのフィールドマップ
	sindy::CFieldMap m_laneLinkGroupFM;	//!< 車線リンクグループのフィールドマップ
};

