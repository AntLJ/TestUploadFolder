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
#include "DataDef.h"

/**
 * @brief 対象のGDBに空レイヤを作成する
 */
class CCreateEmptyLayer
{
public:
	/**
	 * @brief コンストラクタ
	 * @param targetPathList [in] 生成先のGDBパス一覧
	 * @param meshlist       [in] メッシュリストの読込み結果
	 * @param productDef     [in] GDBに生成するレイヤ・
	 * データセット・ドメインの情報
	 */
	CCreateEmptyLayer(
		const std::vector<uh::tstring>& targetPathList
		, const std::vector<uh::tstring>& meshlist
		, const product::ProductDef& productDef
		):
		m_targetPathList(targetPathList)
		, m_meshlist(meshlist)
		, m_productDef(productDef)
	{}

	/**
	 * @brief 全ての生成先に空レイヤ・データセット・ドメインを作成する
	 * @note 作成先に同名の空レイヤ・データセット・ドメインがすでに存在する場合、
	 * 作成しない
	 * @return 処理の成否
	 */
	bool create();

protected:
	const std::vector<uh::tstring>& m_targetPathList; //!< 生成先のGDBパス一覧
	const std::vector<uh::tstring>& m_meshlist; //!< メッシュリストの読込み結果
	/// GDBに生成するレイヤ・データセット・ドメインの情報
	const product::ProductDef& m_productDef;

};
