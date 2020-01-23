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

/**
 * @brief ループ処理 実装クラス
 * @note  ListProcessor又は、そのサブクラスに本クラスを実装したクラスを渡し
 * 　　　 ループを回す想定
 */
template <typename T>
class Processor
{
public:
	Processor<T>(){};
	virtual ~Processor<T>(){};

	/**
	 * @brief  処理実装メソッド
	 * @param  src  [in]	ループ処理で渡される値
	 * @retval
	 */
	virtual bool proc(const T& src){return true;}

	/// @brief ループ処理前実装メソッド
	virtual bool preprocess(){ return true; }

	/// @brief ループ処理
	virtual bool postprocess(){ return true; }

	/** 
	 * @brief  一括処理実装メソッド
	 * @param  srcs [in]  処理対象群
	 * @retval 
	 */
	virtual bool bulkproc(const std::vector<T>& srcs){return true;}

private:

};
