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

#ifndef SINDY_LQ_RELATION_INTERFACE_H_
#define SINDY_LQ_RELATION_INTERFACE_H_

#include <arctl/field.h>
#include <sindy/schema/road.h>

namespace sindy {

struct lq_relation_base
{
	typedef arctl::field::value_t<schema::link_queue::kLinkID, long> link_id_type;
	typedef arctl::field::value_t<schema::link_queue::kLinkDir, long> link_dir_type;

	ARCTL_FIELD_VALUE_CLASS(
		field_value_type,
		((link_id_type))
		((link_dir_type))
	);
};

/**
 * @brief リンク列関係確認クラス。
 */
class lq_relation_exist : public lq_relation_base
{
public:
// コンストラクタとデストラクタ
	virtual ~lq_relation_exist() {}

///@name 純粋仮想関数
//@{
	/**
	 * @brief 指定したリンクにリンク列は関連付いているか。
	 *
	 * @param id [in] リンクID。
	 * @retval true リンク列が関連付けられている。
	 * @retval false リンク列は関連付けられていない。
	 */
	virtual bool exist_lq(const link_id_type& id) const = 0;
//@}
};

/**
 * @brief リンク列関係編集クラス。
 */
class lq_relation_edit : public lq_relation_base
{
public:
// 定義
	typedef arctl::field::value_t<schema::link_queue::kInfID, long> inf_id_type;

	ARCTL_FIELD_VALUE_CLASS(
		field_value_type,
		((lq_relation_base::field_value_type))
		((inf_id_type))
	);

// コンストラクタとデストラクタ
	virtual ~lq_relation_edit() {}

///@name 純粋仮想関数
//@{
	/**
	 * @brief リンクにリンク列を関連付ける。
	 *
	 * @param link_id [in] リンクID。
	 * @param link_dir [in] リンク方向。
	 * @param inf_id [in] リンク列ID。
	 * @retval true 関連付けに成功。
	 * @retval false 関連付けに失敗。既に関連付けられている場合を含む。
	 */
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id) = 0;

	/**
	 * @brief リンクから、リンク列への関連付けを解除する。
	 *
	 * @param link_id [in] リンクID。
	 * @param inf_id [in] リンク列ID。
	 * @retval true 関連付けの解除に成功。
	 * @retval false 関連付けの解除に失敗。関連付けられていない場合を含む。
	 */
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id) = 0;
//@}
};

/**
 * @brief リンク列関係確認・編集クラス。
 */
class lq_relation_mutable : virtual public lq_relation_exist, virtual public lq_relation_edit
{
};

} // namespace sindy

#endif // SINDY_LQ_RELATION_INTERFACE_H_
