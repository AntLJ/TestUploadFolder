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

#ifndef SINDY_LQ_RELATION_EDIT_WATCHER_H_
#define SINDY_LQ_RELATION_EDIT_WATCHER_H_

#include <boost/utility.hpp>
#include <boost/logic/tribool.hpp> 
#include <sindy/lq_relation/interface.h>

namespace sindy {

/**
 * @brief リンク列関係操作によるフラグの状態変化を監視するクラス。
 *
 * リンク列関係確認オブジェクトを返す関数 get_exister() を実装することで、より厳密な監視が行える。
 */
class lq_relation_edit_watcher : virtual public lq_relation_edit, boost::noncopyable
{
public:
// 定義
	typedef boost::tribool flag_type; ///< フラグを立てる(true)か降ろす(false)か要確認(indeterminate)か。

	typedef std::map<long, flag_type> flag_map; ///< リンクID別フラグ操作一覧。
	typedef flag_map::value_type value_type;
	typedef flag_map::const_iterator const_iterator;

	typedef boost::shared_ptr<lq_relation_edit> lq_relation_edit_ref;

// コンストラクタとデストラクタ
	lq_relation_edit_watcher();
	lq_relation_edit_watcher(const lq_relation_edit_ref& pEditor);

// 設定
	void clear();

// 取得
	const_iterator begin() const;
	const_iterator end() const;

	virtual const lq_relation_exist* get_exister() const { return 0; }

// 純粋仮想関数の解決
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id);
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id);

private:
// 変数
	const lq_relation_edit_ref m_pEditor; ///< 編集者。
	flag_map m_FlagMap; ///< フラグ操作一覧。
};

//////////////////////////////////////////////////////////////////////
// lq_relation_edit_watcher 実装
//////////////////////////////////////////////////////////////////////

inline lq_relation_edit_watcher::lq_relation_edit_watcher()
{
}

inline lq_relation_edit_watcher::lq_relation_edit_watcher(const lq_relation_edit_ref& pEditor) :
m_pEditor(pEditor)
{
}

inline void lq_relation_edit_watcher::clear()
{
	m_FlagMap.clear();
}

inline lq_relation_edit_watcher::const_iterator lq_relation_edit_watcher::begin() const
{
	return m_FlagMap.begin();
}

inline lq_relation_edit_watcher::const_iterator lq_relation_edit_watcher::end() const
{
	return m_FlagMap.end();
}

inline bool lq_relation_edit_watcher::attach_lq(
	const link_id_type& link_id,
	const link_dir_type& link_dir,
	const inf_id_type& inf_id)
{
	const lq_relation_exist* pExist = get_exister();

	if(pExist) {
		flag_map::iterator it = m_FlagMap.end();

		// リンクに初めてのリンク列が関連付けられたら、フラグを立てる。
		if(! pExist->exist_lq(link_id)) {
			it = m_FlagMap.insert(value_type(link_id.value(), true)).first;
		}

		// リンク列を関連付ける。
		bool bResult = m_pEditor ? m_pEditor->attach_lq(link_id, link_dir, inf_id) : true;

		// リンクに初めてのリンク列を関連付けようとして失敗したら、フラグを降ろそうとしていない限り、フラグ操作を取りやめる。
		if(it != m_FlagMap.end() && (bResult ^ (static_cast<bool>(it->second)))) {
			m_FlagMap.erase(it);
		}
		
		return bResult;
	}
	else {
		if(! m_pEditor || m_pEditor->attach_lq(link_id, link_dir, inf_id)) {
			m_FlagMap[link_id.value()] = true;
			return true;
		}
		else
			return false;
	}
}

inline bool lq_relation_edit_watcher::detach_lq(
	const link_id_type& link_id,
	const inf_id_type& inf_id)
{
	// リンク列を取り除く。
	if(m_pEditor && m_pEditor->detach_lq(link_id, inf_id))
		return false;

	const lq_relation_exist* pExist = get_exister();

	if(pExist) {
		// リンクから全てのリンク列が取り除かれたら、フラグを降ろす。
		if(! pExist->exist_lq(link_id)) {
			flag_map::iterator it = m_FlagMap.insert(value_type(link_id.value(), false)).first;
			// フラグを立てる予定になっていた場合は、フラグ操作を取りやめる。
			if(it->second) {
				m_FlagMap.erase(it);
			}
		}
	}
	else {
		m_FlagMap[link_id.value()] = boost::indeterminate;
	}

	return true;
}

} // namespace sindy

#endif // SINDY_LQ_RELATION_FLAG_WATCHER_H_
