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
#include "BaseDataCreator.h"

/**
 * @brief   地名代表ポイントデータ生成クラス（派生クラス）
 */
class PlaceNameRepCreator : public BaseDataCreator
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   arg [in]  引数
	 */
	PlaceNameRepCreator(const Arguments& arg);

	/**
	 * @brief   デストラクタ
	 */
	virtual ~PlaceNameRepCreator() {}

	/**
	 * @brief   初期化
	 */
	bool init();

	/**
	 * @brief   データ生成
	 */
	bool create();

};

