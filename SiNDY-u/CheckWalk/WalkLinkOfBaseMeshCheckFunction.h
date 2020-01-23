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

class WalkLinkOfBaseMeshCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out              [in] ログ出力管理クラスの参照
	 * @param modufyFilter     [in] 編集履歴フィルタ
	 * @param onewayClassMap   [in] 一方通行規制コードのコード値ドメインマップ
	 * @param walkNoPassage    [in] チェック対象の通行禁止規制
	 */
	WalkLinkOfBaseMeshCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, std::map<long, CString>& onewayClassMap
		, MultiRowsPack& walkNoPassage
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_onewayClassMap(onewayClassMap)
		, m_walkNoPassage(walkNoPassage)
	{}

	/**
	 * @brief ベースメッシュ単位で行う歩行者リンクの全チェック
	 * @param ipMeshGeo       [in] ベースメッシュ形状
	 * @param ipBoundaryMesh  [in] ベースメッシュの枠線
	 * @param cityMeshMap     [in] 都市地図メッシュコードと形状の対応表
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void checkWalkLinkOfBaseMesh(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs
		);

	/**
	 * @brief releaseモード用のチェック
	 * @param ipMeshGeo       [in] ベースメッシュ形状
	 * @param ipBoundaryMesh  [in] ベースメッシュの枠線
	 * @param cityMeshMap     [in] 都市地図メッシュコードと形状の対応表
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs
		);

protected:
	COutPut& m_output;                                //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter;                       //!< 編集履歴フィルタ
	std::map<long, CString> m_onewayClassMap;         //!< 一方通行規制コードのコード値ドメインマップ
	MultiRowsPack& m_walkNoPassage;					  //!< チェック対象の通行禁止規制
};
