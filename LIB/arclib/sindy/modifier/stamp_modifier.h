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

/**
 * @file stamp_modifier.h
 * @brief 更新時更新情報刻印クラス定義
 *
 * @section lib_stamp_modifier 更新時更新情報刻印クラス 
 * - sindy::stamp_modifier - 更新時に更新情報を刻印する。
 */
#ifndef SINDY_STAMP_MODIFIER_H_
#define SINDY_STAMP_MODIFIER_H_

#include <sindy/stamper/unary_modify_stamper.h>

namespace sindy {

/**
 * @brief 更新時更新情報刻印クラス
 *
 * 更新関数と@ref sindy::unary_modify_stamperを組合せ、更新時に更新情報を刻印するクラス。
 *
 * @par テンプレート引数:
 * @a TFunction : 更新用単項関数。渡されたオブジェクトに更新をかけた場合trueを、かけなかった場合falseを返す。
 */
template <typename TFunction>
class stamp_modifier
{
public:
///@name 構築/消滅
//@{
	/**
	 * @param cFunction [in] 更新関数。
	 * @param pModifyStamper [in,ref,nullable] 更新情報刻印オブジェクト。更新情報の刻印を行わない場合null。
	 */
	stamp_modifier(TFunction cFunction, const unary_modify_stamper* pModifyStamper) :
	m_Function(cFunction),
	m_pModifyStamper(pModifyStamper)
	{
	}

	/**
	 * @param cFunction [in] 更新関数。
	 * @param rModifyStamper [in,ref] 更新情報刻印オブジェクト。
	 */
	stamp_modifier(TFunction cFunction, const unary_modify_stamper& rModifyStamper) :
	m_Function(cFunction),
	m_pModifyStamper(&rModifyStamper)
	{
	}
//@}

///@name 演算
//@{
	/**
	 * @brief 更新が為された場合に限り、更新情報を刻印する。
	 *
	 * @retval true 更新が為され、更新情報の刻印に成功（明示的に刻印しないのも含む）したか、刻印しなかった。
	 * @retval false 更新に失敗した。あるいは更新情報の刻印に失敗した。
	 */
	template <typename TRow>
	bool operator()(TRow const& rRow) const
	{
		return m_Function(rRow) && (m_pModifyStamper ? (*m_pModifyStamper)(rRow) : true);
	}
//@}

private:
// 変数
	TFunction m_Function; ///< 更新関数。
	const unary_modify_stamper* m_pModifyStamper; ///< 更新情報刻印オブジェクト。
};

/**
 * @defgroup make_stamp_modifier_group stamp_modifier 生成関数
 */
//@{
template <typename TFunction>
stamp_modifier<TFunction> make_stamp_modifier(TFunction cFunction, const unary_modify_stamper* pModifyStamper = 0)
{
	return stamp_modifier<TFunction>(cFunction, pModifyStamper);
}

template <typename TFunction>
stamp_modifier<TFunction> make_stamp_modifier(TFunction cFunction, const unary_modify_stamper& rModifyStamper)
{
	return stamp_modifier<TFunction>(cFunction, rModifyStamper);
}
//@}

} // namespace sindy

#endif // SINDY_STAMP_MODIFIER_H_
