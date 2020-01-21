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

#ifndef __SINDY_FUNCTIONAL_EQUAL_TO_H__
#define __SINDY_FUNCTIONAL_EQUAL_TO_H__

#include <functional>

namespace sindy {

/////////////////////////////////////////////////////////////////////////////
// 定義
/////////////////////////////////////////////////////////////////////////////

/// @brief SiNDY版equal_to
template <typename TInterface>
struct equal_to
{
};

/**
 * @brief プロパティの一致を持って真と成す関数オブジェクト
 *
 * TInterface には以下の型が使用可能。
 * - IUnknown およびその派生クラス、またはそのポインタ。
 * - _com_ptr_t およびその派生クラス。
 */
template <typename TInterface>
struct data_equal
{
public:
	typedef interface_to_pointer<TInterface>::type first_argument_type;
	typedef bool result_type;

	/**
	 * @brief プロパティの一致判定を行う
	 *
	 * @warning TData はデストラクタで自己を破棄できなければならない。
	 */
	template <typename TData>
	bool operator()(first_argument_type pInterface, const TData& rData) const
	{
		if(pInterface == 0)
			return false;

		TData data;
		traits<TInterface>::accessor<TData>::get(pInterface, &data);
		return data == rData;
	}
};

// 定義
/////////////////////////////////////////////////////////////////////////////

/// @brief _variant_t型equal_to特殊化
template <>
struct equal_to<_variant_t> : public std::binary_function<_variant_t, VARIANT, bool>
{
	bool operator()(const _variant_t& lhs, const VARIANT& rhs) const;
};

/////////////////////////////////////////////////////////////////////////////
// 実装
/////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4310)
/**
 * @brief 属性値を比較する
 *
 * 基本は_variant_t::operator==()。<br>
 * 両引数のVARIANT型がVT_UNKNOWNの場合に限り、IRelationalOperator::Equals()による形状比較を行う。
 *
 * @param lhs [in] 属性値1。
 * @param rhs [in] 属性値2。
 * @retval true 一致。
 * @retval false 不一致。
 */
inline bool equal_to<_variant_t>::operator()(const _variant_t& lhs, const VARIANT& rhs) const
{
	if(lhs.vt == VT_UNKNOWN || rhs.vt == VT_UNKNOWN) {
		IRelationalOperatorPtr ipRelOp = lhs.punkVal;
		IGeometryPtr ipGeometry = rhs.punkVal;

		if(ipRelOp != 0 || ipGeometry != 0) {
			return equals()(ipRelOp, ipGeometry);
		}
	}

	return lhs == rhs;
}
#pragma warning(pop)

} // namespace sindy

#endif // __SINDY_FUNCTIONAL_EQUAL_TO_H__
