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
 * @file unary_modify_stamper.h
 * @brief 単項版更新情報刻印クラステンプレート。
 */
#ifndef SINDY_UNARY_MODIFY_STAMPER_H_
#define SINDY_UNARY_MODIFY_STAMPER_H_

#include <functional>
#include <sindy/stamper/modify_stamper.h>
//#include <SiNDYAttrCommunity.h>

namespace sindy {

/**
 * @brief 更新情報刻印クラス
 *
 * 渡されたオブジェクトを@ref modify_stamper継承クラスに渡し、更新情報を刻印するクラス。<br>
 * 刻印する@ref term_update_typeはコンストラクタで定める。
 */
class unary_modify_stamper : public std::unary_function<IRowBuffer*, bool>
{
public:
//@{ @name 構築/消滅
	/**
	 * @param pModifyStamper [in,ref,nullable] 実際に更新情報の刻印を行う関数オブジェクト。nullを渡した場合、刻印は行わない。
	 * @param eUpdateType [in] 更新内容。
	 *
	 * @note
	 * @a pModifyStamperはnullを許可しない方が、誤って「更新情報を刻印しないオブジェクト」を生成してしまわない分、安全ではある。<br>
	 * 筆者は「マージ処理時には更新情報を刻印すべきではない」と考えているため、nullによる非刻印設定を残している。
	 */
	unary_modify_stamper(const modify_stamper* pModifyStamper, schema::ipc_table::update_type::ECode eUpdateType) :
	m_pModifyStamper(pModifyStamper),
	m_eUpdateType(eUpdateType)
	{
	}

	/**
	 * @param rModifyStamper [in,ref] 実際に更新情報の刻印を行う関数オブジェクト。
	 * @param eUpdateType [in] 更新内容。
	 */
	unary_modify_stamper(const modify_stamper& rModifyStamper, schema::ipc_table::update_type::ECode eUpdateType) :
	m_pModifyStamper(&rModifyStamper),
	m_eUpdateType(eUpdateType)
	{
	}

	~unary_modify_stamper()
	{
	}
//@}

//@{ @name 演算
	/**
	 * @brief 更新情報の刻印を行う
	 *
	 * @param ipRowBuffer [in] 刻印対象オブジェクト。
	 * @retval true 刻印に成功したか、刻印しなかった。
	 * @retval false 刻印に失敗した。
	 */
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		return ! m_pModifyStamper || (*m_pModifyStamper)(ipRowBuffer, m_eUpdateType);
	}
//@}

private:
// 変数
	const modify_stamper* m_pModifyStamper; ///< 更新情報刻印オブジェクト。
	const schema::ipc_table::update_type::ECode m_eUpdateType; ///< 更新内容。
};

} // namespace sindy

#endif // SINDY_UNARY_MODIFY_STAMPER_H_
