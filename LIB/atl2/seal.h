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

#ifndef ATL2_SEAL_H_
#define ATL2_SEAL_H_

namespace atl2 {

/**
 * @brief アドレス演算子のオーバーロードを無効化した型を提供するクラス。
 *
 * @par CAdaptとの相違点:
 * - コンストラクタをテンプレート化しているため、値の代入に関してより高い柔軟性を有する。（元クラスのexplicit宣言は厳守）
 * - operator&() 以外、元クラスのメソッドをそのまま使用できる。
 */
template <typename Base>
class seal : public Base
{
public:
	/**
	 * @brief デフォルトコンストラクタ。
	 */
	seal() {}

	/**
	 * @brief 元クラスのexplicit宣言を考慮した単項コンストラクタ。
	 */
	template <typename T>
	seal(const T& rT) : Base(static_cast<const Base&>(rT)) {}

	/// @name アドレス演算子オーバーロードの封印。
	//@{
	seal* operator&() { return this; }
	const seal* operator&() const { return this; }
	//@}
};

} // namespace atl2

#endif // ATL2_SEAL_H_
