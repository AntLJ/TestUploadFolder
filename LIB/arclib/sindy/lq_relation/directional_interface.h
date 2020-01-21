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

#ifndef SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_
#define SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_

#include <sindy/lq_relation/interface.h>

namespace sindy {

/**
 * @brief 有向リンク列関係確認クラス。
 */
class directional_lq_relation_exist : virtual public lq_relation_exist
{
public:
///@name 純粋仮想関数
//@{
	/**
	 * @brief リンクに、指定した方向を向いたリンク列が関連付けられているか。
	 *
	 * @param link_id [in] リンクID。
	 * @param link_dir [in] リンク方向。
	 * @retval true リンク列が関連付けられている。
	 * @retval false リンク列は関連付けられていない。
	 */
	virtual bool exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const = 0;
//@}
};

/**
 * @brief 有向リンク列関係確認・編集クラス。
 */
class directional_lq_relation_mutable : virtual public directional_lq_relation_exist, virtual public lq_relation_mutable
{
};

} // namespace sindy

#endif // SINDY_DIRECTIONAL_LQ_RELATION_INTERFACE_H_
