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

class UndergroundCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out          [in] ログ出力管理クラスの参照
	 * @param modufyFilter [in] 編集履歴フィルタ
	 */
	UndergroundCheckFunction(COutPut& out, CComVariant modifyFilter):
		m_output(out)
		, m_modifyFilter(modifyFilter)
	{}

	/**
	 * @brief	地下街ポリゴンのチェック
	 * @param	meshcode	[in]	チェック対象メッシュコード
	 * @param	ipMeshGeo	[in]	チェック対象のジオメトリ
	 * @param	underArea   [in]	チェック対象地下街ポリゴン群クラス
	 * @param	walkLink   	[in]	チェック参照用歩行者ノード群クラス
	 */
	void CheckUnderground( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo, 
		const FeaturesPack& underArea,
		const FeaturesPack& walkLink
		);

private:
	/**
	 * @brief エラーコード1600に該当するリンクをエラー出力
	 * @param underLinkIdAll [in] 処理中メッシュに重なる地下通路リンクのID
	 * @param outsideLinkId  [in] 処理中メッシュに重なる地下通路リンクの内、
	 * いずれの地下街ポリゴンにも含まれない地下通路リンク
	 * @param underAreaUnion [in] 処理中メッシュに重なる地下街ポリゴンを全て
	 * Unionした形状
	 * @param underAreaId  [in] 処理中のメッシュに重なる地下街ポリゴンのID
	 * @param walkLik      [in] 処理中のメッシュ+隣接9メッシュ内の全ての歩行者リンク群クラス
	 * @param underArea    [in] 処理中のメッシュ+隣接9メッシュ内の全ての地下街ポリゴン群クラス
	 * @param meshcode     [in] 処理中のメッシュのメッシュコード
	 */
	void errLinkPartIncludedArea(
		const std::set<long>& underLinkIdAll
		, const std::set<long>& outsideLinkId
		, const IGeometryPtr& underAreaUnion
		, const std::set<long>& underAreaId
		, const FeaturesPack& walkLink
		, const FeaturesPack& underArea
		, const CString& meshcode);

	/**
	 * @brief チェック対象リンクがエラーコード1600の出力対象になるか確認
	 * @param linkInfo      [in] チェック対象のリンク
	 * @param fieldMap      [in] チェック対象のフィールドマップ
	 * @param underAreaId   [in] 処理中のメッシュに重なる地下街ポリゴンのID
	 * @param underArea     [in] 処理中のメッシュ+隣接9メッシュ内の全ての地下街ポリゴンの情報
	 * @param crossPoint    [in] チャック対象リンクと地下街ポリゴンの交差位置
	 * @retval true  エラー出力対象である
	 * @retval false エラー出力対象ではない
	 */
	bool checkModifyTimeToPartIncludedErr(
		const GeoClass& linkInfo
		, const sindy::CFieldMap& fieldMap
		, const std::set<long>& underAreaId
		, const FeaturesPack& underArea
		, const std::vector<IPointPtr>& crossPoint);

	/**
	 * @brief エラーコード1601に該当するリンクをエラー出力
	 * @param outLinks     [in] 処理中のメッシュ内で、いずれの地下街ポリゴンにも含まれない地下通路リンク
	 * @param walkLik      [in] 処理中のメッシュ+隣接9メッシュ内の全ての歩行者リンクの情報
	 * @param meshcode     [in] 処理中のメッシュのメッシュコード
	 */
	void errOutLink(
		const std::set<long>& outLinks
		, const FeaturesPack& walkLink
		, const CString& meshcode);

	/**
	 * @brief エラーコード1602に該当するリンクをエラー出力
	 * @param underLinks   [in] 処理中メッシュ内の地下通路リンク
	 * @param unionArea    [in] 処理中メッシュ内の地下通路ポリゴンの形状をUnionした形状
	 * @param walkLik      [in] 処理中のメッシュ+隣接9メッシュ内の全ての歩行者リンクの情報
	 * @param meshcode     [in] 処理中のメッシュのメッシュコード
	 */
	void errNoneExist(
		const std::set<long>& underLinks
		, const IGeometryPtr& unionArea
		, const FeaturesPack& walkLink
		, const CString& meshcode);

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
};
