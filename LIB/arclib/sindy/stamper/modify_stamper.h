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
 * @file modify_stamper.h
 * @brief 更新情報刻印クラステンプレート。
 *
 * sindy::modify_stamperを定義するヘッダ。
 */
#ifndef SINDY_MODIFY_STAMPER_H_
#define SINDY_MODIFY_STAMPER_H_

#include <functional>
#include <sindy/schema/common.h>

namespace sindy {

/**
 * @brief 更新情報刻印クラスインタフェース
 *
 * オブジェクトと更新内容属性値を渡し、オブジェクトに更新情報を刻印するクラス。<br>
 */
class modify_stamper : public std::binary_function<IRowBuffer*, schema::ipc_table::update_type::ECode, bool>
{
public:
//@{ @name 構築/消滅
	modify_stamper() {}
	virtual ~modify_stamper() {}
//@}

//@{ @name 演算
	/**
	 * @brief 更新情報の刻印を行う
	 *
	 * @param ipRowBuffer [in] 更新情報刻印対象オブジェクト。
	 * @param eUpdateType [in] 更新内容。
	 * @retval true 刻印に成功した。
	 * @retval false 刻印に失敗した。
	 *
	 * @attention
	 * 刻印失敗を通告するのは例外の役目であり、真理値を返しているのは古い実装の名残である。
	 */
	bool operator()(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const
	{
		return stamp(ipRowBuffer, eUpdateType);
	}
//@}

protected:
//@{ @name 純粋仮想関数
	/**
	 * @brief 更新情報の刻印を行う。
	 *
	 * @param ipRowBuffer [in] 更新情報刻印対象。
	 * @param eUpdateType [in] 更新内容。
	 * @retval true 刻印に成功した。
	 * @retval false 刻印に失敗した。
	 */
	virtual bool stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const = 0;
//@}
};

} // namespace sindy

#endif // SINDY_MODIFY_STAMPER_H_
