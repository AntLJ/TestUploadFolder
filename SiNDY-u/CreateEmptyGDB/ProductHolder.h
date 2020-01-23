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
 * @brief 作成物を保持するクラス
 */
class CProductHolder
{
public:
	/**
	 * @brief コンストラクタ
	 * @param createInfoMap [in] レイヤ・ドメイン・
	 * フィーチャデータセット生成のために必要な情報
	 */
	CProductHolder(const create_info::CREATEINFOMAP& createInfoMap):
		m_createInfoMap( createInfoMap )
	{}

	/**
	 * @brief コンストラクタでセットした情報に従って
	 * レイヤ・ドメイン・フィーチャデータセットを生成
	 * @retval true  生成に成功
	 * @retval false 生成に失敗
	 */
	bool create();

	/**
	 * @brief 生成したレイヤ・ドメイン・フィーチャデータセットを取得
	 */
	const product::ProductDef& getResult() const { return m_productDef; }

protected:
	/// 生成するレイヤ・ドメイン・フィーチャデータセットの情報
	const create_info::CREATEINFOMAP& m_createInfoMap;
	/// 生成した生成するレイヤ・ドメイン・フィーチャデータセット
	product::ProductDef m_productDef;
};
