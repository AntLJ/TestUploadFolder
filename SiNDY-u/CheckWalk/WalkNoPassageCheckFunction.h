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

class WalkNoPassageCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out           [in] ログ出力管理クラスの参照
	 * @param modufyFilter  [in] 編集履歴フィルタ
	 */
	WalkNoPassageCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief 歩行者通行禁止規制テーブルの全チェック
	 * @param ipMeshGeo        [in] チェック対象メッシュのジオメトリ
	 * @param walkLink         [in] 歩行者リンク群クラス
	 * @param walkNoPassage    [in] 歩行者通行禁止規制情報
	 */
	void checkWalkNoPassage(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkNoPassage
	);

	// TODO: checkWalkNoPassageと統合可能な箇所を統合
	/**
	 * @brief releaseモード用のチェック
	 * @param ipMeshGeo        [in] チェック対象メッシュのジオメトリ
	 * @param walkLink         [in] 歩行者リンク群クラス
	 * @param walkNoPassage    [in] 歩行者通行禁止規制情報
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkNoPassage
	);

	/**
	 * @brief 例外モード用のチェック
	 * @param walkNoPassage       [in] 歩行者通行禁止規制情報
	 */
	void checkException(
		const MultiRowsPack& walkNoPassage
	);

private:

	/**
	 * @brief	歩行者通行禁止規制テーブルのレコード間チェック
	 * @param	linkID	[in]	リンクID
	 * @param	timeRegList	[in]	同一リンクIDに紐づく通行禁止規制情報のリスト
	 * @param	sameTimeErrCode	[in]	時間が完全一致の場合のエラーコード
	 * @param	overlapTimeErrCode	[in]	時間が被っていたの場合のエラーコード
	 */
	void checkTimeRegulationRel(
		long linkID,
		const std::vector<std::pair<_IRowPtr, CTimeReg>>& timeRegList,
		const sindy::CFieldMap& fieldMap,
		const err_code::ECode sameTimeErrCode,
		const err_code::ECode overlapTimeErrCode
	);

	/**
	 * @brief リンクIDが不正な値かチェック
	 * @param [in] linkID         リンクID
	 */
	void checkLinkID(
		long  linkID
	) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};
