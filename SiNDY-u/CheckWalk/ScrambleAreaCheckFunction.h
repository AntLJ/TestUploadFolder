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
#include "util.h"
#include "OutputLog.h"
#include "DatabaseControl.h"
#include "Properties.h"

class CCheckWalk;
class ScrambleAreaCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param [in] out                       ログ出力管理クラスの参照
	 * @param [in] modufyFilter              編集履歴フィルタ
	 * @param [in] dataType                  チェック対象のデータタイプ
	 */
	ScrambleAreaCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief スクランブルエリアの全てのチェックを実行
	 * @param [in] meshcode            処理中の都市地図メッシュコード
	 * @param [in] ipMeshGeo           都市地図メッシュ形状
	 * @param [in] scrambleArea        スクランブルエリア群クラス
	 * @param [in] walkLink            歩行者リンク群クラス
	 * @param [in] walkLinkRTree       歩行者リンクRTree
	 * @param [in] checktarget         チェック対象
	 */
	void check(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		properties::check_target checktarget
	) const;

	/**
	 * @brief 例外モード用のチェック
	 * @param [in] scrambleArea        スクランブルエリア群クラス
	 */
	void checkException(
		const FeaturesPack& scrambleArea
	);

protected:

	/**
	 * @brief スクランブルエリア形状が不正であるかチェック
	 * @note エラーコード:1800
	 * @param [in] ipTable  スクランブルエリアテーブル
	 */
	void checkScrambleAreaGeometry(const ITablePtr& ipTable) const;

	/**
	 * @brief スクランブルエリア内にスクランブルフラグが付与された横断歩道が1つも無いかチェック
	 * @param [in] scrambleAreaObject      チェック対象の スクランブルエリア
	 * @param [in] walkLink                歩行者リンク群クラス
	 * @param [in] walkLinkRTree           歩行者リンクRTree
	 */
	void checkScrambleAreaCrossWalkLink(
		const GeoClass& scrambleAreaObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	) const;

	/**
	 * @brief スクランブル種別コードが不正な値かチェック
	 * @param [in] scrambleAreaClass       スクランブル種別
	 */
	void checkScrambleAreaClass(
		long  scrambleAreaClass
	) const;

protected:
	COutPut& m_output;                                                   //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter;                                          //!< 編集履歴フィルタ
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< データタイプ
};
