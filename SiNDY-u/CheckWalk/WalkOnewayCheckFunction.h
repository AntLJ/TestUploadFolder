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

class WalkOnewayCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out           [in] ログ出力管理クラスの参照
	 * @param modufyFilter  [in] 編集履歴フィルタ
	 */
	WalkOnewayCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
	{}

	/**
	 * @brief 歩行者一方通行規制テーブルの全チェック
	 * @param ipMeshGeo        [in] チェック対象メッシュのジオメトリ
	 * @param walkLink  	   [in] 歩行者リンクMap
	 * @param walkOneway       [in] 歩行者一方通行規制情報
	 * @param walkNoPassage    [in] 歩行者通行禁止規制情報
	 */
	void checkWalkOneway(
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, GeoClass>& walkLinkMap,
		const MultiRowsPack& walkOneway,
		const MultiRowsPack& walkNoPassage
	);

	// TODO: checkWalkNoPassageと統合可能な箇所を統合
	/**
	 * @brief releaseモード用のチェック
	 * @param ipMeshGeo        [in] チェック対象メッシュのジオメトリ
	 * @param walkLink  	   [in] 歩行者リンクMap
	 * @param walkOneway       [in] 歩行者一方通行規制情報
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const MultiRowsPack& walkOneway
	);

	/**
	 * @brief 例外モード用のチェック
	 * @param walkOneway       [in] 歩行者一方通行規制情報
	 */
	void checkException(
		const MultiRowsPack& walkOneway
	);

private:
	// TODO: 以下はWalkNoPassageCheckFunctionがもつメンバーと統合する
	/**
	 * @brief	歩行者通行禁止規制テーブルのレコード間チェック
	 * @param	meshcode	[in]	メッシュコード
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
	 * @brief	指定されたリンクIDに関連する時間規制を取得し、配列に格納する。
	 * @param	linkID	[in]	リンクID
	 * @param	walkNopassage	[in]	時間規制関連テーブルのMap
	 * @param	timRegList	[out]	属性と時間規制情報がペアになったリスト
	 */
	void createTimeRegList(
		long linkID,
		const MultiRowsPack& walkNopassage,
		std::vector< std::pair< _IRowPtr, CTimeReg >>& timRegList
	);

	/**
	 * @brief リンクIDが不正な値かチェック
	 * @param [in] linkID         リンクID
	 */
	void checkLinkID(
		long  linkID
	) const;

	/**
	* @brief リンク方向コードが不正な値かチェック
	* @param [in] linkDir         リンク方向コード
	*/
	void checkLinkDir(
		long  linkDir
	) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};
