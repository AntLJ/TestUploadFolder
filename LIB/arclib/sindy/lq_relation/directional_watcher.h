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

#ifndef SINDY_DIRECTIONAL_LQ_RELATION_WATCHER_H_
#define SINDY_DIRECTIONAL_LQ_RELATION_WATCHER_H_

#include <sindy/lq_relation/watcher.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// directional_lq_relation_watcher 定義
//////////////////////////////////////////////////////////////////////
	
#pragma warning( push )
#pragma warning( disable : 4250 )

class directional_lq_relation_watcher : public lq_relation_watcher, virtual public directional_lq_relation_mutable
{
public:
// コンストラクタとデストラクタ
	directional_lq_relation_watcher(
		const boost::shared_ptr<directional_lq_relation_mutable>& pMutable);

	directional_lq_relation_watcher(
		const boost::shared_ptr<lq_relation_edit>& pEditor,
		const boost::shared_ptr<const directional_lq_relation_exist>& pExister);

// 取得
	virtual const lq_relation_exist* get_exister() const;

// 純粋仮想関数の解決
	virtual bool exist_lq(const link_id_type& id) const;
	virtual bool exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const;

private:
// 変数
	const boost::shared_ptr<const directional_lq_relation_exist> m_pExister;
};

#pragma warning( pop )

//////////////////////////////////////////////////////////////////////
// lq_relation_directional_watcher 実装
//////////////////////////////////////////////////////////////////////

inline directional_lq_relation_watcher::directional_lq_relation_watcher(
	const boost::shared_ptr<directional_lq_relation_mutable>& pMutable) :
lq_relation_watcher(pMutable, boost::shared_ptr<const lq_relation_exist>()),
m_pExister(pMutable)
{
}

inline directional_lq_relation_watcher::directional_lq_relation_watcher(
	const boost::shared_ptr<lq_relation_edit>& pEditor,
	const boost::shared_ptr<const directional_lq_relation_exist>& pExister) :
lq_relation_watcher(pEditor, boost::shared_ptr<const lq_relation_exist>()),
m_pExister(pExister)
{
}

inline const lq_relation_exist* directional_lq_relation_watcher::get_exister() const
{
	return m_pExister.get();
}

inline bool directional_lq_relation_watcher::exist_lq(const link_id_type& id) const
{
	if(! m_pExister)
		throw std::runtime_error("exister is null.");

	return m_pExister->exist_lq(id);
}

inline bool directional_lq_relation_watcher::exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const
{
	if(! m_pExister)
		throw std::runtime_error("exister is null.");

	return m_pExister->exist_directional_lq(link_id, link_dir);
}

} // namespace sindy

#endif // SINDY_DIRECTIONAL_LQ_RELATION_WATCHER_H_
